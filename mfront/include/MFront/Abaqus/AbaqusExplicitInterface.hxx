/*!
 * \file   mfront/include/MFront/Abaqus/AbaqusExplicitInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <type_traits>

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"

#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "TFEL/Material/MechanicalBehaviourTraits.hxx"
#include "TFEL/Material/MaterialException.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusConfig.hxx"
#include "MFront/Abaqus/AbaqusTraits.hxx"
#include "MFront/Abaqus/AbaqusException.hxx"
#include "MFront/Abaqus/AbaqusExplicitData.hxx"
#include "MFront/Abaqus/AbaqusTangentOperator.hxx"
#include "MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"
#include "MFront/Abaqus/AbaqusComputeThermalExpansionCoefficientTensor.hxx"
#include "MFront/Abaqus/AbaqusInterfaceExceptions.hxx"

namespace abaqus {

  /*!
   * \class  AbaqusExplicitInterface
   * \brief This class create an interface between a behaviour class
   * and the abaqus finite element code
   *
   * \note : most of branching is done at compile-time. to the very
   * exeception of the stress-free expansion which are handled through
   * two functions pointers which take into account the specificities
   * of finite strain strategy used. The choice of introducing those
   * runtime-functions comes from the fact that we did not want a code
   * duplication between two different finite strain strategies (to
   * reduce both compile-time and library size).
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      typename T,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusExplicitInterface
      : protected AbaqusInterfaceExceptions {
    //! space dimension
    static constexpr unsigned short N =
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    //! simple alias
    using MechanicalBehaviourBase = tfel::material::MechanicalBehaviourBase;
    using BV = Behaviour<H, T, false>;
    using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
    using ATraits = AbaqusTraits<BV>;
    //! structure in charge of initializing the stiffness operator
    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef typename BV::BehaviourData BData;
      TFEL_ABAQUS_INLINE static void exe(BData& data, const T* const props) {
        const bool buas = ATraits::requiresUnAlteredStiffnessTensor;
        AbaqusComputeStiffnessTensor<ATraits::btype, H, ATraits::etype,
                                     buas>::exe(data.getStiffnessTensor(),
                                                props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer
    //! structure in charge of initializing the stiffness operator
    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef typename BV::BehaviourData BData;
      TFEL_ABAQUS_INLINE static void exe(BData& data, const T* const props) {
        const unsigned short o = ATraits::elasticPropertiesOffset;
        AbaqusComputeThermalExpansionCoefficientTensor<
            ATraits::btype, H,
            ATraits::stype>::exe(props + o,
                                 data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer
    //! place holder for tag dispatching
    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef typename BV::BehaviourData BData;
      TFEL_ABAQUS_INLINE static void exe(BData&, const T* const) {}
    };  // end of struct DoNothingInitializer
    //! structure in charge of calling the computeInternalEnergy method
    struct TFEL_VISIBILITY_LOCAL InternalEnergyComputer {
      TFEL_ABAQUS_INLINE static void exe(T& e, const BV& b, const T& rho) {
        auto Psi_s = e * rho;
        b.computeInternalEnergy(Psi_s);
        e = Psi_s / rho;
      }
    };
    //! structure in charge of calling the computeDissipatedEnergy method
    struct TFEL_VISIBILITY_LOCAL DissipatedEnergyComputer {
      TFEL_ABAQUS_INLINE static void exe(T& e, const BV& b, const T& rho) {
        auto Psi_d = e * rho;
        b.computeDissipatedEnergy(Psi_d);
        e = Psi_d / rho;
      }
    };
    //! place holder for tag dispatching
    struct TFEL_VISIBILITY_LOCAL DoNothingEnergyComputer {
      TFEL_ABAQUS_INLINE static void exe(T&, const BV&, const T&) {}
    };  // end of struct DoNothingEnergyComputer
    static constexpr bool bs = ATraits::requiresStiffnessTensor;
    static constexpr bool ba =
        ATraits::requiresThermalExpansionCoefficientTensor;
    static constexpr bool bi = MTraits::hasComputeInternalEnergy;
    static constexpr bool bd = MTraits::hasComputeDissipatedEnergy;
    using SInitializer = typename std::conditional<bs,
                                                   StiffnessOperatorInitializer,
                                                   DoNothingInitializer>::type;
    using AInitializer =
        typename std::conditional<ba,
                                  ThermalExpansionCoefficientTensorInitializer,
                                  DoNothingInitializer>::type;
    using IEnergyComputer = typename std::
        conditional<bi, InternalEnergyComputer, DoNothingEnergyComputer>::type;
    using DEnergyComputer =
        typename std::conditional<bd,
                                  DissipatedEnergyComputer,
                                  DoNothingEnergyComputer>::type;
    /*!
     * \param[out] D: elastic stiffness
     * \param[out] d: data
     */
    TFEL_ABAQUS_INLINE2 static int computeElasticPrediction(
        T* const D, const AbaqusExplicitData<T>& d) {
      //! simple alias
      using TangentOperatorTraits = tfel::material::TangentOperatorTraits<
          MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>;
      constexpr T zero = T{0};
      const tfel::math::stensor<N, T> e(zero);
      const tfel::math::stensor<N, T> de(zero);
      const tfel::math::stensor<N, T> s(zero);
      Behaviour<H, T, false> b(d);
      SInitializer::exe(b, d.eth_props);
      AInitializer::exe(b, d.eth_props);
      b.setBehaviourDataGradients(e);
      b.setBehaviourDataThermodynamicForces(s);
      b.setIntegrationDataGradients(de);
      b.setOutOfBoundsPolicy(d.policy);
      if (!b.initialize()) {
        return 1;
      }
      b.checkBounds();
      const auto smf = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
      const auto r =
          b.computePredictionOperator(smf, Behaviour<H, T, false>::ELASTIC);
      ExtractAndConvertTangentOperator<H>::exe(b, D);
      return (r == Behaviour<H, T, false>::SUCCESS) ? 0 : 1;
    }
    /*!
     * \param[out] D: elastic stiffness
     * \param[out] d: data
     */
    TFEL_ABAQUS_INLINE2 static int computeElasticPrediction2(
        T* const D, const AbaqusExplicitData<T>& d) {
      //! simple alias
      using TangentOperatorTraits = tfel::material::TangentOperatorTraits<
          MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR>;
      constexpr T zero = T{0};
      const tfel::math::tensor<N, T> F0(tfel::math::tensor<N, T>::Id());
      const tfel::math::tensor<N, T> F1(tfel::math::tensor<N, T>::Id());
      const tfel::math::stensor<N, T> s(zero);
      Behaviour<H, T, false> b(d);
      SInitializer::exe(b, d.eth_props);
      AInitializer::exe(b, d.eth_props);
      b.setBehaviourDataGradients(F0);
      b.setIntegrationDataGradients(F1);
      b.setBehaviourDataThermodynamicForces(s);
      b.setOutOfBoundsPolicy(d.policy);
      if (!b.initialize()) {
        return 1;
      }
      b.checkBounds();
      const auto smf = TangentOperatorTraits::ABAQUS;
      const auto r =
          b.computePredictionOperator(smf, Behaviour<H, T, false>::ELASTIC);
      ExtractAndConvertTangentOperator<H>::exe(b, D);
      return (r == Behaviour<H, T, false>::SUCCESS) ? 0 : 1;
    }
    /*!
     * \brief integrate a behaviour written at small strain
     * \param[out/in] s: stress tensor
     * \param[out] d:  data
     * \param[out] e:  strain tensor
     * \param[out] de: strain tensor increment
     */
    template <typename StreeFreeExpansionHandler>
    TFEL_ABAQUS_INLINE2 static int integrate(
        tfel::math::stensor<N, T>& s,
        const AbaqusExplicitData<T>& d,
        const tfel::math::stensor<N, T>& e,
        const tfel::math::stensor<N, T>& de,
        const StreeFreeExpansionHandler& sfeh) {
      //! simple alias
      using TangentOperatorTraits = tfel::material::TangentOperatorTraits<
          MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>;
      using DVInitializer = typename std::conditional<
          tfel::material::MechanicalBehaviourTraits<BV>::hasStressFreeExpansion,
          GradientInitialiserWithStressFreeExpansion,
          GradientInitialiserWithoutStressFreeExpansion>::type;
      Behaviour<H, T, false> b(d);
      SInitializer::exe(b, d.eth_props);
      AInitializer::exe(b, d.eth_props);
      DVInitializer::exe(b, e, de, sfeh);
      b.setBehaviourDataThermodynamicForces(s);
      b.setOutOfBoundsPolicy(d.policy);
      if (!b.initialize()) {
        return -1;
      }
      b.checkBounds();
      const auto smf = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
      auto r_dt = std::numeric_limits<T>::max();
      auto tsf = b.computeAPrioriTimeStepScalingFactor(r_dt);
      if (!tsf.first) {
        return -1;
      }
      if (b.integrate(smf, Behaviour<H, T, false>::NOSTIFFNESSREQUESTED) ==
          Behaviour<H, T, false>::FAILURE) {
        return -1;
      }
      tsf = b.computeAPosterioriTimeStepScalingFactor(r_dt);
      if (!tsf.first) {
        return -1;
      }
      b.checkBounds();
      b.exportStateData(s, d);
      IEnergyComputer::exe(d.enerInternNew, b, d.density);
      DEnergyComputer::exe(d.enerInelasNew, b, d.density);
      return 0;
    }
    /*!
     * \brief integrate a behaviour written at small strain
     * \param[out/in] s: stress tensor
     * \param[out] d:  data
     * \param[out] e:  strain tensor
     * \param[out] de: strain tensor increment
     */
    TFEL_ABAQUS_INLINE2 static int integrate(
        tfel::math::stensor<N, T>& s,
        const AbaqusExplicitData<T>& d,
        const tfel::math::tensor<N, T>& F0,
        const tfel::math::tensor<N, T>& F1) {
      //! simple alias
      using TangentOperatorTraits = tfel::material::TangentOperatorTraits<
          MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR>;
      Behaviour<H, T, false> b(d);
      SInitializer::exe(b, d.eth_props);
      AInitializer::exe(b, d.eth_props);
      b.setBehaviourDataGradients(F0);
      b.setIntegrationDataGradients(F1);
      b.setBehaviourDataThermodynamicForces(s);
      b.setOutOfBoundsPolicy(d.policy);
      if (!b.initialize()) {
        return -1;
      }
      b.checkBounds();
      const auto smf = TangentOperatorTraits::ABAQUS;
      auto r_dt = std::numeric_limits<T>::max();
      auto tsf = b.computeAPrioriTimeStepScalingFactor(r_dt);
      if (!tsf.first) {
        return -1;
      }
      if (b.integrate(smf, Behaviour<H, T, false>::NOSTIFFNESSREQUESTED) ==
          Behaviour<H, T, false>::FAILURE) {
        return -1;
      }
      tsf = b.computeAPosterioriTimeStepScalingFactor(r_dt);
      if (!tsf.first) {
        return -1;
      }
      b.checkBounds();
      b.exportStateData(s, d);
      IEnergyComputer::exe(d.enerInternNew, b, d.density);
      DEnergyComputer::exe(d.enerInelasNew, b, d.density);
      return 0;
    }

   private:
    //! An helper structure used to initialise the driving variables
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithStressFreeExpansion
        : public AbaqusInterfaceExceptions {
      /*!
       * \param[out] b:    behaviour
       * \param[in]  e:    driving variable at the beginning of the
       *                   time step
       * \param[in]  de:   driving variable at the end of the
       *                   time step or driving variable increment
       * \param[in]  sfeh: function handling the stress-free expansion
       *                   at the beginning of the time step
       */
      template <typename SFEHType>
      TFEL_ABAQUS_INLINE static void exe(BV& b,
                                         tfel::math::stensor<N, T> e,
                                         tfel::math::stensor<N, T> de,
                                         const SFEHType& sfeh) {
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        // check that the function pointer are not null
        std::pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        sfeh(e, de, s.first, s.second);
        b.setBehaviourDataGradients(e);
        b.setIntegrationDataGradients(de);
      }  // end of exe
    };   // end of struct GradientInitialiserWithStressFreeExpansion
    //! An helper structure used to initialise the driving variables
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithoutStressFreeExpansion {
      /*!
       * \param[out] b:    behaviour
       * \param[in]  e:    driving variable at the beginning of the
       *                   time step
       * \param[in]  de:   driving variable at the end of the
       *                   time step or driving variable increment
       * \param[in]  sfeh: function handling the stress-free expansion
       *                   at the beginning of the time step
       */
      template <typename SFEHType>
      TFEL_ABAQUS_INLINE static void exe(BV& b,
                                         const tfel::math::stensor<N, T>& e,
                                         const tfel::math::stensor<N, T>& de,
                                         const SFEHType&) {
        b.setBehaviourDataGradients(e);
        b.setIntegrationDataGradients(de);
      }  // end of exe
    };   // end of struct GradientInitialiserWithoutStressFreeExpansion
  };     // end of struct AbaqusExplicitInterface

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX */
