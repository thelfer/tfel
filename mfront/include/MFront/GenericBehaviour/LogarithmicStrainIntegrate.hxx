/*!
 * \file   mfront/include/MFront/GenericBehaviour/LogarithmicStrainIntegrate.hxx
 * \brief
 * \author Thomas Helfer
 * \date   31/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_LOGARITHMICSTRAININTEGRATE_HXX
#define LIB_MFRONT_GENERICBEHAVIOUR_LOGARITHMICSTRAININTEGRATE_HXX

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "TFEL/Material/LogarithmicStrainHandler.hxx"
#include "MFront/GenericBehaviour/GenericBehaviourTraits.hxx"
#include "MFront/GenericBehaviour/Integrate.hxx"

namespace mfront::gb::logarithmic_strain {

  /*!
   * \brief execute the given initialize function
   * \tparam Behaviour: class describing the initialize function.
   * \tparam m: method implementing the initialize function.
   * \param[in] initialize_variables: pointer to the values of the
   * initialize variables.
   * \param[in] d: behaviour data.
   * \param[in] p: out of bounds policy.
   */
  template <typename Behaviour, void (Behaviour::*m)(const real *const)>
  int executeInitializeFunction(mfront_gb_BehaviourData &d,
                                const real *const initialize_variables,
                                const tfel::material::OutOfBoundsPolicy p) {
    using namespace tfel::material;
    using mfront::gb::real;
    using Traits = GenericBehaviourTraits<Behaviour>;
    //
    constexpr auto hypothesis = Traits::hypothesis;
    constexpr auto N = ModellingHypothesisToSpaceDimension<hypothesis>::value;
    constexpr auto StensorSize =
        ModellingHypothesisToStensorSize<hypothesis>::value;
    constexpr auto TensorSize =
        ModellingHypothesisToTensorSize<hypothesis>::value;
    constexpr auto setting = LogarithmicStrainHandler<N, real>::LAGRANGIAN;
    // stress measure
    const auto sm = getStressMeasure(d.K);
    if (sm == StressMeasure::INVALID_STRESS_MEASURE) {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    //
    tfel::math::tensor<N, real> F0;
    tfel::fsalgo::copy<TensorSize>::exe(d.s0.gradients, F0.begin());
    LogarithmicStrainHandler<N, real> lgh0(setting, F0);
    auto e0 = lgh0.getHenckyLogarithmicStrain();
    if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                  (hypothesis ==
                   ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      if constexpr (Traits::has_axial_strain_offset) {
        lgh0.updateAxialDeformationGradient(std::exp(
            d.s0.internal_state_variables[Traits::axial_strain_offset]));
      } else {
        reportError(d,
                    "The behaviour does not declare the axial strain"
                    "as an internal state variable");
        return -1;
      }
    }
    auto T0 = tfel::math::stensor<N, real>{};
    if (sm == StressMeasure::CAUCHY) {
      auto s0 = tfel::math::stensor<N, real>{};
      tfel::fsalgo::copy<StensorSize>::exe(d.s0.thermodynamic_forces,
                                           s0.begin());
      T0 = lgh0.convertFromCauchyStress(s0);
    } else if (sm == StressMeasure::PK1) {
      auto pk0 = tfel::math::tensor<N, real>{};
      tfel::fsalgo::copy<TensorSize>::exe(d.s0.thermodynamic_forces,
                                          pk0.begin());
      const auto s0 =
          tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk0, F0);
      T0 = lgh0.convertFromCauchyStress(s0);
    } else if (sm == StressMeasure::PK2) {
      auto S0 = tfel::math::stensor<N, real>{};
      tfel::fsalgo::copy<StensorSize>::exe(d.s0.thermodynamic_forces,
                                           S0.begin());
      const auto s0 = convertSecondPiolaKirchhoffStressToCauchyStress(S0, F0);
      T0 = lgh0.convertFromCauchyStress(s0);
    } else {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    auto *const gradients0_old = d.s0.gradients;
    auto *const gradients1_old = d.s1.gradients;
    auto *const thermodynamic_forces0_old = d.s0.thermodynamic_forces;
    auto *const thermodynamic_forces1_old = d.s1.thermodynamic_forces;
    d.s0.gradients = e0.begin();
    d.s1.gradients = e0.begin();
    d.s0.thermodynamic_forces = T0.begin();
    d.s1.thermodynamic_forces = T0.begin();
    const auto r = mfront::gb::executeInitializeFunction<Behaviour, m>(
        initialize_variables, d, p);
    d.s0.gradients = gradients0_old;
    d.s1.gradients = gradients1_old;
    d.s0.thermodynamic_forces = thermodynamic_forces0_old;
    d.s1.thermodynamic_forces = thermodynamic_forces1_old;
    return r;
  }  // end of executeInitializeFunction

  /*!
   * \brief integrate the behaviour in the logarithmic strain space over a time
   * step.
   *
   * \tparam Behaviour: class implementing the behaviour
   * internal state variables.
   * \param[in,out] d: behaviour data
   * \param[in] p: out of bounds policy
   */
  template <typename Behaviour>
  int integrate(mfront_gb_BehaviourData &d,
                const tfel::material::OutOfBoundsPolicy p) {
    using namespace tfel::material;
    using mfront::gb::real;
    using Traits = GenericBehaviourTraits<Behaviour>;
    //
    constexpr auto hypothesis = Traits::hypothesis;
    constexpr auto N = ModellingHypothesisToSpaceDimension<hypothesis>::value;
    constexpr auto StensorSize =
        ModellingHypothesisToStensorSize<hypothesis>::value;
    constexpr auto TensorSize =
        ModellingHypothesisToTensorSize<hypothesis>::value;
    // stress measure
    const auto sm = getStressMeasure(d.K);
    if (sm == StressMeasure::INVALID_STRESS_MEASURE) {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    // stiffness type
    const auto smf = getTangentOperator(d.K);
    if (smf == FiniteStrainTangentOperator::C_TRUESDELL) {
      reportError(d, "invalid choice for consistent tangent operator");
      return -1;
    }
    //
    tfel::math::st2tost2<N, real> K;
    tfel::math::tensor<N, real> F0;
    tfel::math::tensor<N, real> F1;
    tfel::math::stensor<N, real> s0;
    tfel::fsalgo::copy<TensorSize>::exe(d.s0.gradients, F0.begin());
    tfel::fsalgo::copy<TensorSize>::exe(d.s1.gradients, F1.begin());
    const auto setting = (smf == FiniteStrainTangentOperator::DSIG_DF)
                             ? LogarithmicStrainHandler<N, real>::EULERIAN
                             : LogarithmicStrainHandler<N, real>::LAGRANGIAN;
    LogarithmicStrainHandler<N, real> lgh0(setting, F0);
    LogarithmicStrainHandler<N, real> lgh1(setting, F1);
    auto e0 = lgh0.getHenckyLogarithmicStrain();
    auto e1 = lgh1.getHenckyLogarithmicStrain();
    if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                  (hypothesis ==
                   ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      if constexpr (Traits::has_axial_strain_offset) {
        lgh0.updateAxialDeformationGradient(std::exp(
            d.s0.internal_state_variables[Traits::axial_strain_offset]));
      } else {
        reportError(d,
                    "The behaviour does not declare the axial strain"
                    "as an internal state variable");
        return -1;
      }
    }
    auto T0 = tfel::math::stensor<N, real>{};
    auto T1 = tfel::math::stensor<N, real>{};
    if (sm == StressMeasure::CAUCHY) {
      tfel::fsalgo::copy<StensorSize>::exe(d.s0.thermodynamic_forces,
                                           s0.begin());
      T0 = lgh0.convertFromCauchyStress(s0);
    } else if (sm == StressMeasure::PK1) {
      auto pk0 = tfel::math::tensor<N, real>{};
      tfel::fsalgo::copy<TensorSize>::exe(d.s0.thermodynamic_forces,
                                          pk0.begin());
      s0 = tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk0, F0);
      T0 = lgh0.convertFromCauchyStress(s0);
    } else if (sm == StressMeasure::PK2) {
      auto S0 = tfel::math::stensor<N, real>{};
      tfel::fsalgo::copy<StensorSize>::exe(d.s0.thermodynamic_forces,
                                           S0.begin());
      s0 = convertSecondPiolaKirchhoffStressToCauchyStress(S0, F0);
      T0 = lgh0.convertFromCauchyStress(s0);
    } else {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    auto *const gradients0_old = d.s0.gradients;
    auto *const gradients1_old = d.s1.gradients;
    auto *const thermodynamic_forces0_old = d.s0.thermodynamic_forces;
    auto *const thermodynamic_forces1_old = d.s1.thermodynamic_forces;
    auto *const K_old = d.K;
    K(0, 0) = d.K[0];
    d.s0.gradients = e0.begin();
    d.s1.gradients = e1.begin();
    d.s0.thermodynamic_forces = T0.begin();
    d.s1.thermodynamic_forces = T1.begin();
    d.K = K.begin();
    const auto bp = K(0, 0) < -0.5;
    const auto bk = K(0, 0) > 0.5;
    const auto r = mfront::gb::integrate<Behaviour>(
        d, Behaviour::STANDARDTANGENTOPERATOR, p);
    d.s0.gradients = gradients0_old;
    d.s1.gradients = gradients1_old;
    d.s0.thermodynamic_forces = thermodynamic_forces0_old;
    d.s1.thermodynamic_forces = thermodynamic_forces1_old;
    d.K = K_old;
    if (r) {
      if (bp) {
        if (smf == FiniteStrainTangentOperator::DSIG_DF) {
          const auto Cs = lgh0.convertToSpatialTangentModuli(K, T0);
          const auto Dt = convert<FiniteStrainTangentOperator::DTAU_DF,
                                  FiniteStrainTangentOperator::SPATIAL_MODULI>(
              Cs, F0, F0, s0);
          tfel::math::T2toST2View<N, real>(d.K) =
              convert<FiniteStrainTangentOperator::DSIG_DF,
                      FiniteStrainTangentOperator::DTAU_DF>(Dt, F0, F0, s0);
        } else if (smf == FiniteStrainTangentOperator::DS_DEGL) {
          tfel::math::ST2toST2View<N, real>(d.K) =
              lgh0.convertToMaterialTangentModuli(K, T0);
        } else if (smf == FiniteStrainTangentOperator::DPK1_DF) {
          const auto Cse = lgh0.convertToMaterialTangentModuli(K, T0);
          tfel::math::T2toT2View<N, real>(d.K) =
              convert<FiniteStrainTangentOperator::DPK1_DF,
                      FiniteStrainTangentOperator::DS_DEGL>(Cse, F0, F0, s0);
        } else if (smf == FiniteStrainTangentOperator::DTAU_DDF) {
          const auto Cs = lgh0.convertToSpatialTangentModuli(K, T0);
          const auto Dt = convert<FiniteStrainTangentOperator::DTAU_DF,
                                  FiniteStrainTangentOperator::SPATIAL_MODULI>(
              Cs, F0, F0, s0);
          tfel::math::T2toST2View<N, real>(d.K) =
              convert<FiniteStrainTangentOperator::DTAU_DDF,
                      FiniteStrainTangentOperator::DTAU_DF>(Dt, F0, F0, s0);
        } else {
          reportError(d, "invalid choice for consistent tangent operator");
          return -1;
        }
      } else {  // if(bp)
        if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                      (hypothesis ==
                       ModellingHypothesis::
                           AXISYMMETRICALGENERALISEDPLANESTRESS)) {
          if constexpr (Traits::has_axial_strain_offset) {
            lgh1.updateAxialDeformationGradient(std::exp(
                d.s1.internal_state_variables[Traits::axial_strain_offset]));
          } else {
            reportError(d,
                        "The behaviour does not declare the axial strain"
                        "as an internal state variable");
            return -1;
          }
        }
        const auto s1 = lgh1.convertToCauchyStress(T1);
        if (sm == StressMeasure::CAUCHY) {
          tfel::fsalgo::copy<StensorSize>::exe(s1.begin(),
                                               d.s1.thermodynamic_forces);
        } else if (sm == StressMeasure::PK1) {
          tfel::math::TensorView<N, real> pk1(d.s1.thermodynamic_forces);
          pk1 = tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s1,
                                                                           F1);
        } else if (sm == StressMeasure::PK2) {
          tfel::math::StensorView<N, real> S1(d.s1.thermodynamic_forces);
          S1 = tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s1,
                                                                           F1);
        } else {
          reportError(d, "invalid choice for the stress measure");
          return -1;
        }
        if (bk) {
          if (smf == FiniteStrainTangentOperator::DSIG_DF) {
            const auto Cs = lgh1.convertToSpatialTangentModuli(K, T1);
            const auto Dt =
                convert<FiniteStrainTangentOperator::DTAU_DF,
                        FiniteStrainTangentOperator::SPATIAL_MODULI>(Cs, F0, F1,
                                                                     s1);
            tfel::math::T2toST2View<N, real>(d.K) =
                convert<FiniteStrainTangentOperator::DSIG_DF,
                        FiniteStrainTangentOperator::DTAU_DF>(Dt, F0, F1, s1);
          } else if (smf == FiniteStrainTangentOperator::DS_DEGL) {
            tfel::math::ST2toST2View<N, real>(d.K) =
                lgh1.convertToMaterialTangentModuli(K, T1);
          } else if (smf == FiniteStrainTangentOperator::DPK1_DF) {
            const auto Cse = lgh1.convertToMaterialTangentModuli(K, T1);
            tfel::math::T2toT2View<N, real>(d.K) =
                convert<FiniteStrainTangentOperator::DPK1_DF,
                        FiniteStrainTangentOperator::DS_DEGL>(Cse, F0, F1, s1);
          } else if (smf == FiniteStrainTangentOperator::DTAU_DDF) {
            const auto Cs = lgh0.convertToSpatialTangentModuli(K, T1);
            const auto Dt =
                convert<FiniteStrainTangentOperator::DTAU_DF,
                        FiniteStrainTangentOperator::SPATIAL_MODULI>(Cs, F0, F1,
                                                                     s1);
            tfel::math::T2toST2View<N, real>(d.K) =
                convert<FiniteStrainTangentOperator::DTAU_DDF,
                        FiniteStrainTangentOperator::DTAU_DF>(Dt, F0, F1, s1);
          } else {
            reportError(d, "invalid choice for consistent tangent operator");
            return -1;
          }
        }  // end of if(bk)
      }    // end of if(bp)
    }
    return r;
  }  // end of integrate

  /*!
   * \brief execute the given post-processing
   * \tparam Behaviour: class describing the post-processing.
   * \tparam m: method implementing the post-processing.
   * \tparam use_initial_state: boolean stating if the postprocessing uses
   * the initial state of the material.
   * \param[out] post_processing_variables: pointer to the values of the
   * post-processing variables.
   * \param[in] d: behaviour data.
   * \param[in] p: out of bounds policy.
   */
  template <typename Behaviour,
            void (Behaviour::*m)(real *const,
                                 const typename Behaviour::BehaviourData &),
            const bool use_initial_state>
  int executePostProcessing(real *const post_processing_variables,
                            mfront_gb_BehaviourData &d,
                            const tfel::material::OutOfBoundsPolicy p) {
    using namespace tfel::material;
    using mfront::gb::real;
    using Traits = GenericBehaviourTraits<Behaviour>;
    //
    constexpr auto hypothesis = Traits::hypothesis;
    constexpr auto N = ModellingHypothesisToSpaceDimension<hypothesis>::value;
    constexpr auto StensorSize =
        ModellingHypothesisToStensorSize<hypothesis>::value;
    constexpr auto TensorSize =
        ModellingHypothesisToTensorSize<hypothesis>::value;
    // stress measure
    const auto sm = getStressMeasure(d.K);
    if (sm == StressMeasure::INVALID_STRESS_MEASURE) {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    // stiffness type
    const auto smf = getTangentOperator(d.K);
    if (smf == FiniteStrainTangentOperator::C_TRUESDELL) {
      reportError(d, "invalid choice for consistent tangent operator");
      return -1;
    }
    //
    tfel::math::tensor<N, real> F0;
    tfel::math::tensor<N, real> F1;
    tfel::fsalgo::copy<TensorSize>::exe(d.s0.gradients, F0.begin());
    tfel::fsalgo::copy<TensorSize>::exe(d.s1.gradients, F1.begin());
    const auto setting = (smf == FiniteStrainTangentOperator::DSIG_DF)
                             ? LogarithmicStrainHandler<N, real>::EULERIAN
                             : LogarithmicStrainHandler<N, real>::LAGRANGIAN;
    LogarithmicStrainHandler<N, real> lgh0(setting, F0);
    LogarithmicStrainHandler<N, real> lgh1(setting, F1);
    auto e0 = lgh0.getHenckyLogarithmicStrain();
    auto e1 = lgh1.getHenckyLogarithmicStrain();
    if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                  (hypothesis ==
                   ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      if constexpr (Traits::has_axial_strain_offset) {
        lgh0.updateAxialDeformationGradient(std::exp(
            d.s0.internal_state_variables[Traits::axial_strain_offset]));
        lgh1.updateAxialDeformationGradient(std::exp(
            d.s1.internal_state_variables[Traits::axial_strain_offset]));
      } else {
        reportError(d,
                    "The behaviour does not declare the axial strain"
                    "as an internal state variable");
        return -1;
      }
    }
    auto T0 = tfel::math::stensor<N, real>{};
    auto T1 = tfel::math::stensor<N, real>{};
    if (sm == StressMeasure::CAUCHY) {
      auto s0 = tfel::math::stensor<N, real>{};
      auto s1 = tfel::math::stensor<N, real>{};
      tfel::fsalgo::copy<StensorSize>::exe(d.s0.thermodynamic_forces,
                                           s0.begin());
      tfel::fsalgo::copy<StensorSize>::exe(d.s1.thermodynamic_forces,
                                           s1.begin());
      T0 = lgh0.convertFromCauchyStress(s0);
      T1 = lgh1.convertFromCauchyStress(s1);
    } else if (sm == StressMeasure::PK1) {
      auto pk0 = tfel::math::tensor<N, real>{};
      tfel::fsalgo::copy<TensorSize>::exe(d.s0.thermodynamic_forces,
                                          pk0.begin());
      const auto s0 =
          tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk0, F0);
      T0 = lgh0.convertFromCauchyStress(s0);
      //
      auto pk1 = tfel::math::tensor<N, real>{};
      tfel::fsalgo::copy<TensorSize>::exe(d.s1.thermodynamic_forces,
                                          pk1.begin());
      const auto s1 =
          tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk1, F1);
      T1 = lgh1.convertFromCauchyStress(s1);
    } else if (sm == StressMeasure::PK2) {
      auto S0 = tfel::math::stensor<N, real>{};
      tfel::fsalgo::copy<StensorSize>::exe(d.s0.thermodynamic_forces,
                                           S0.begin());
      const auto s0 = convertSecondPiolaKirchhoffStressToCauchyStress(S0, F0);
      T0 = lgh0.convertFromCauchyStress(s0);
      auto S1 = tfel::math::stensor<N, real>{};
      tfel::fsalgo::copy<StensorSize>::exe(d.s1.thermodynamic_forces,
                                           S1.begin());
      const auto s1 = convertSecondPiolaKirchhoffStressToCauchyStress(S1, F1);
      T1 = lgh1.convertFromCauchyStress(s1);
    } else {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    auto *const gradients0_old = d.s0.gradients;
    auto *const gradients1_old = d.s1.gradients;
    auto *const thermodynamic_forces0_old = d.s0.thermodynamic_forces;
    auto *const thermodynamic_forces1_old = d.s1.thermodynamic_forces;
    d.s0.gradients = e0.begin();
    d.s1.gradients = e1.begin();
    d.s0.thermodynamic_forces = T0.begin();
    d.s1.thermodynamic_forces = T1.begin();
    const auto r =
        mfront::gb::executePostProcessing<Behaviour, m, use_initial_state>(
            post_processing_variables, d, p);
    d.s0.gradients = gradients0_old;
    d.s1.gradients = gradients1_old;
    d.s0.thermodynamic_forces = thermodynamic_forces0_old;
    d.s1.thermodynamic_forces = thermodynamic_forces1_old;
    return r;
  }  // end of executePostProcessing

}  // end of namespace mfront::gb::logarithmic_strain

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_LOGARITHMICSTRAININTEGRATE_HXX */
