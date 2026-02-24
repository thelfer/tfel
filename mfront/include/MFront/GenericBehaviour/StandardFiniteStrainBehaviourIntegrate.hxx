/*!
 * \file
 * mfront/include/MFront/GenericBehaviour/StandardFiniteStrainIntegrate.hxx
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

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_STANDARDFINITESTRAINBEHAVIOURINTEGRATE_HXX
#define LIB_MFRONT_GENERICBEHAVIOUR_STANDARDFINITESTRAINBEHAVIOURINTEGRATE_HXX

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/GenericBehaviour/Integrate.hxx"

namespace mfront::gb::finite_strain {

  /*!
   * \brief execute the given initialize function
   * \tparam Behaviour: class describing the initialize function.
   * \tparam m: method implementing the initialize function.
   * \tparam use_initial_state: boolean stating if the postprocessing uses
   * the initial state of the material.
   * \param[out] initialize_variables: pointer to the values of the
   * initialize function variables.
   * \param[in] d: behaviour data.
   * \param[in] p: out of bounds policy.
   */
  template <typename Behaviour, void (Behaviour::*m)(const real* const)>
  int executeInitializeFunction(mfront_gb_BehaviourData& d,
                                const real* const initialize_variables,
                                const tfel::material::OutOfBoundsPolicy p) {
    using namespace tfel::material;
    using Traits = GenericBehaviourTraits<Behaviour>;
    using real = mfront::gb::real;
    //
    constexpr auto hypothesis = Traits::hypothesis;
    constexpr auto N = ModellingHypothesisToSpaceDimension<hypothesis>::value;
    constexpr auto TensorSize =
        ModellingHypothesisToTensorSize<hypothesis>::value;
    // stress measure
    const auto sm = getStressMeasure(d.K);
    if (sm == StressMeasure::INVALID_STRESS_MEASURE) {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    tfel::math::stensor<N, real> s0;
    auto* const thermodynamic_forces0_old = d.s0.thermodynamic_forces;
    auto* const thermodynamic_forces1_old = d.s1.thermodynamic_forces;
    if (sm != StressMeasure::CAUCHY) {
      tfel::math::tensor<N, real> F0;
      tfel::fsalgo::copy<TensorSize>::exe(d.s0.gradients, F0.begin());
      if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                    (hypothesis == ModellingHypothesis::
                                       AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        if constexpr (Traits::has_axial_deformation_gradient_offset) {
          const auto F0zz = d.s0.internal_state_variables
                                [Traits::axial_deformation_gradient_offset];
          if constexpr (hypothesis == ModellingHypothesis::PLANESTRESS) {
            F0[2] += F0zz;
          } else {
            F0[1] += F0zz;
          }
        } else {
          reportError(d,
                      "The axial deformation gradient is "
                      "not computed by the behaviour");
          return -1;
        }
      }
      if (sm == StressMeasure::PK1) {
        tfel::math::TensorView<N, real> pk0(d.s0.thermodynamic_forces);
        s0 =
            tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk0, F0);
      } else if (sm == StressMeasure::PK2) {
        tfel::math::StensorView<N, real> S0(d.s0.thermodynamic_forces);
        s0 =
            tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S0, F0);
      } else {
        reportError(d, "invalid choice for the stress measure");
        return -1;
      }
      d.s0.thermodynamic_forces = s0.begin();
      d.s1.thermodynamic_forces = s0.begin();
    }
    const auto r = mfront::gb::executeInitializeFunction<Behaviour, m>(
        initialize_variables, d, p);
    d.s0.thermodynamic_forces = thermodynamic_forces0_old;
    d.s1.thermodynamic_forces = thermodynamic_forces1_old;
    return r;
  }  // end of executeInitializeFunction

  /*!
   * \brief integrate a finite strain behaviour.
   *
   * \tparam Behaviour: class implementing the behaviour
   * \param[in,out] d: behaviour data
   * \param[in] p: out of bounds policy
   */
  template <typename Behaviour>
  int integrate(mfront_gb_BehaviourData& d,
                const tfel::material::OutOfBoundsPolicy p) {
    using namespace tfel::material;
    using Traits = GenericBehaviourTraits<Behaviour>;
    using real = mfront::gb::real;
    //
    constexpr auto hypothesis = Traits::hypothesis;
    constexpr auto N = ModellingHypothesisToSpaceDimension<hypothesis>::value;
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
    tfel::math::stensor<N, real> s0;
    tfel::math::stensor<N, real> s1;
    auto* const thermodynamic_forces0_old = d.s0.thermodynamic_forces;
    auto* const thermodynamic_forces1_old = d.s1.thermodynamic_forces;
    if (sm != StressMeasure::CAUCHY) {
      tfel::math::tensor<N, real> F0;
      tfel::fsalgo::copy<TensorSize>::exe(d.s0.gradients, F0.begin());
      if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                    (hypothesis == ModellingHypothesis::
                                       AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        if constexpr (Traits::has_axial_deformation_gradient_offset) {
          const auto F0zz = d.s0.internal_state_variables
                                [Traits::axial_deformation_gradient_offset];
          if constexpr (hypothesis == ModellingHypothesis::PLANESTRESS) {
            F0[2] += F0zz;
          } else {
            F0[1] += F0zz;
          }
        } else {
          reportError(d,
                      "The axial deformation gradient is "
                      "not computed by the behaviour");
          return -1;
        }
      }
      if (sm == StressMeasure::PK1) {
        tfel::math::ConstTensorView<N, real> pk0(d.s0.thermodynamic_forces);
        s0 =
            tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk0, F0);
      } else if (sm == StressMeasure::PK2) {
        tfel::math::ConstStensorView<N, real> S0(d.s0.thermodynamic_forces);
        s0 =
            tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S0, F0);
      } else {
        reportError(d, "invalid choice for the stress measure");
        return -1;
      }
      d.s0.thermodynamic_forces = s0.begin();
      d.s1.thermodynamic_forces = s1.begin();
    }
    const auto r = mfront::gb::integrate<Behaviour>(d, smf, p);
    d.s0.thermodynamic_forces = thermodynamic_forces0_old;
    d.s1.thermodynamic_forces = thermodynamic_forces1_old;
    if ((r) && (sm != StressMeasure::CAUCHY)) {
      tfel::math::tensor<N, real> F1;
      tfel::fsalgo::copy<TensorSize>::exe(d.s1.gradients, F1.begin());
      if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                    (hypothesis == ModellingHypothesis::
                                       AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        if constexpr (Traits::has_axial_deformation_gradient_offset) {
          const auto F1zz = d.s1.internal_state_variables
                                [Traits::axial_deformation_gradient_offset];
          if constexpr (hypothesis == ModellingHypothesis::PLANESTRESS) {
            F1[2] += F1zz;
          } else {
            F1[1] += F1zz;
          }
        } else {
          reportError(d,
                      "the axial deformation gradient is not defined "
                      "as an internal state variable");
          return -1;
        }
      }
      if (sm == StressMeasure::PK1) {
        tfel::math::TensorView<N, real> pk1(d.s1.thermodynamic_forces);
        pk1 =
            tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s1, F1);
      } else if (sm == StressMeasure::PK2) {
        tfel::math::StensorView<N, real> S1(d.s1.thermodynamic_forces);
        S1 =
            tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s1, F1);
      } else {
        reportError(d, "invalid choice for the stress measure");
        return -1;
      }
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
            void (Behaviour::*m)(real* const,
                                 const typename Behaviour::BehaviourData&),
            const bool use_initial_state>
  int executePostProcessing(real* const post_processing_variables,
                            mfront_gb_BehaviourData& d,
                            const tfel::material::OutOfBoundsPolicy p) {
    using namespace tfel::material;
    using Traits = GenericBehaviourTraits<Behaviour>;
    using real = mfront::gb::real;
    //
    constexpr auto hypothesis = Traits::hypothesis;
    constexpr auto N = ModellingHypothesisToSpaceDimension<hypothesis>::value;
    constexpr auto TensorSize =
        ModellingHypothesisToTensorSize<hypothesis>::value;
    // stress measure
    const auto sm = getStressMeasure(d.K);
    if (sm == StressMeasure::INVALID_STRESS_MEASURE) {
      reportError(d, "invalid choice for the stress measure");
      return -1;
    }
    tfel::math::stensor<N, real> s0;
    tfel::math::stensor<N, real> s1;
    auto* const thermodynamic_forces0_old = d.s0.thermodynamic_forces;
    auto* const thermodynamic_forces1_old = d.s1.thermodynamic_forces;
    if (sm != StressMeasure::CAUCHY) {
      tfel::math::tensor<N, real> F0;
      tfel::math::tensor<N, real> F1;
      tfel::fsalgo::copy<TensorSize>::exe(d.s0.gradients, F0.begin());
      tfel::fsalgo::copy<TensorSize>::exe(d.s1.gradients, F1.begin());
      if constexpr ((hypothesis == ModellingHypothesis::PLANESTRESS) ||
                    (hypothesis == ModellingHypothesis::
                                       AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        if constexpr (Traits::has_axial_deformation_gradient_offset) {
          const auto F0zz = d.s0.internal_state_variables
                                [Traits::axial_deformation_gradient_offset];
          const auto F1zz = d.s1.internal_state_variables
                                [Traits::axial_deformation_gradient_offset];
          if constexpr (hypothesis == ModellingHypothesis::PLANESTRESS) {
            F0[2] += F0zz;
            F1[2] += F1zz;
          } else {
            F0[1] += F0zz;
            F1[1] += F1zz;
          }
        } else {
          reportError(d,
                      "The axial deformation gradient is "
                      "not computed by the behaviour");
          return -1;
        }
      }
      if (sm == StressMeasure::PK1) {
        tfel::math::TensorView<N, real> pk0(d.s0.thermodynamic_forces);
        tfel::math::TensorView<N, real> pk1(d.s1.thermodynamic_forces);
        s0 =
            tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk0, F0);
        s1 =
            tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk1, F1);
      } else if (sm == StressMeasure::PK2) {
        tfel::math::StensorView<N, real> S0(d.s0.thermodynamic_forces);
        tfel::math::StensorView<N, real> S1(d.s1.thermodynamic_forces);
        s0 =
            tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S0, F0);
        s1 =
            tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S1, F1);
      } else {
        reportError(d, "invalid choice for the stress measure");
        return -1;
      }
      d.s0.thermodynamic_forces = s0.begin();
      d.s1.thermodynamic_forces = s1.begin();
    }
    const auto r =
        mfront::gb::executePostProcessing<Behaviour, m, use_initial_state>(
            post_processing_variables, d, p);
    d.s0.thermodynamic_forces = thermodynamic_forces0_old;
    d.s1.thermodynamic_forces = thermodynamic_forces1_old;
    return r;
  }  // end of executePostProcessing

}  // end of namespace mfront::gb::finite_strain

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_STANDARDFINITESTRAINBEHAVIOURINTEGRATE_HXX \
        */
