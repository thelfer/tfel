/*!
 * \file  mfront/include/MFront/Abaqus/AbaqusBehaviourHandler.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSBEHAVIOURHANDLER_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSBEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_ABAQUS_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_ABAQUS_CALL_HXX */

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/TMatrixView.hxx"
#include "TFEL/Math/T2toST2/T2toST2View.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

#include "MFront/Abaqus/AbaqusData.hxx"
#include "MFront/Abaqus/AbaqusTangentOperator.hxx"
#include "MFront/Abaqus/AbaqusInterfaceExceptions.hxx"
#include "MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"
#include "MFront/Abaqus/AbaqusComputeThermalExpansionCoefficientTensor.hxx"

namespace abaqus {

  template <AbaqusBehaviourType btype>
  struct AbaqusTangentOperatorFlag;

  template <>
  struct AbaqusTangentOperatorFlag<abaqus::STANDARDSTRAINBASEDBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr auto value =
        TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template <>
  struct AbaqusTangentOperatorFlag<abaqus::STANDARDFINITESTRAINBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr auto value = TangentOperatorTraits::ABAQUS;
  };

  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct AbaqusStensorSize
      : public tfel::material::ModellingHypothesisToStensorSize<H> {
  };  // end of struct AbaqusStensorSize

  template <>
  struct AbaqusStensorSize<tfel::material::ModellingHypothesis::PLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 3u;
  };  // end of struct AbaqusStensorSize

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusBehaviourHandler
      : public AbaqusInterfaceExceptions {
    //! a simple alias
    using BV = Behaviour<H, AbaqusReal, false>;
    //! a simple alias
    using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithStressFreeExpansion
        : public AbaqusInterfaceExceptions {
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ABAQUS_INLINE static void exe(
          BV& b,
          const AbaqusReal* const STRAN,
          const AbaqusReal* const DSTRAN,
          const StressFreeExpansionHandler<AbaqusReal>& sfeh) {
        using namespace tfel::material;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        const AbaqusInt N = ModellingHypothesisToSpaceDimension<H>::value;
        AbaqusReal dv0[AbaqusTraits<BV>::GradientSize];
        AbaqusReal dv1[AbaqusTraits<BV>::GradientSize];
        tfel::fsalgo::copy<AbaqusTraits<BV>::GradientSize>::exe(STRAN, dv0);
        tfel::fsalgo::copy<AbaqusTraits<BV>::GradientSize>::exe(DSTRAN, dv1);
        // check that the function pointer are not null
        if (sfeh == nullptr) {
          throwUnsupportedStressFreeExpansionException(MTraits::getName());
        }
        std::pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        const auto& s0 = s.first;
        const auto& s1 = s.second;
        sfeh(dv0, dv1, &s0[0], &s1[0], N);
        b.setABAQUSBehaviourDataGradients(dv0);
        b.setABAQUSIntegrationDataGradients(dv1);
      }  // end of exe

    };  // end of struct GradientInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithoutStressFreeExpansion {
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ABAQUS_INLINE static void exe(
          BV& b,
          const AbaqusReal* const STRAN,
          const AbaqusReal* const DSTRAN,
          const StressFreeExpansionHandler<AbaqusReal>&) {
        b.setABAQUSBehaviourDataGradients(STRAN);
        b.setABAQUSIntegrationDataGradients(DSTRAN);
      }  // end of exe
    };   // end of struct GradientInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef typename BV::BehaviourData BData;
      TFEL_ABAQUS_INLINE static void exe(BData& data,
                                         const AbaqusReal* const props) {
        typedef AbaqusTraits<BV> Traits;
        const bool buas = Traits::requiresUnAlteredStiffnessTensor;
        AbaqusComputeStiffnessTensor<AbaqusTraits<BV>::btype, H,
                                     AbaqusTraits<BV>::etype,
                                     buas>::exe(data.getStiffnessTensor(),
                                                props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef typename BV::BehaviourData BData;
      TFEL_ABAQUS_INLINE static void exe(BData& data,
                                         const AbaqusReal* const props) {
        const auto o = AbaqusTraits<BV>::elasticPropertiesOffset;
        AbaqusComputeThermalExpansionCoefficientTensor<
            AbaqusTraits<BV>::btype, H, AbaqusTraits<BV>::stype>::
            exe(props + o, data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef typename BV::BehaviourData BData;
      TFEL_ABAQUS_INLINE static void exe(BData&, const AbaqusReal* const) {}
    };  // end of struct DoNothingInitializer

    struct TFEL_VISIBILITY_LOCAL Integrator {
      //! structure in charge of calling the computeInternalEnergy method
      struct TFEL_VISIBILITY_LOCAL InternalEnergyComputer {
        template <typename T>
        TFEL_ABAQUS_INLINE static void exe(T& Psi_s, const BV& b) {
          b.computeInternalEnergy(Psi_s);
        }
      };  // end of struct InternalEnergyComputer
      //! structure in charge of calling the computeDissipatedEnergy method
      struct TFEL_VISIBILITY_LOCAL DissipatedEnergyComputer {
        template <typename T>
        TFEL_ABAQUS_INLINE static void exe(T& Psi_d, const BV& b) {
          b.computeDissipatedEnergy(Psi_d);
        }  // end of exe
      };   // end of struct DissipatedEnergyComputer
      //! place holder for tag dispatching
      struct TFEL_VISIBILITY_LOCAL DoNothingEnergyComputer {
        template <typename T>
        TFEL_ABAQUS_INLINE static void exe(T&, const BV&) {}
      };  // end of struct DoNothingEnergyComputer
      //! a simple alias
      using SInitializer =
          typename std::conditional<AbaqusTraits<BV>::requiresStiffnessTensor,
                                    StiffnessOperatorInitializer,
                                    DoNothingInitializer>::type;
      //! a simple alias
      using AInitializer = typename std::conditional<
          AbaqusTraits<BV>::requiresThermalExpansionCoefficientTensor,
          ThermalExpansionCoefficientTensorInitializer,
          DoNothingInitializer>::type;
      //! a simple alias
      using IEnergyComputer =
          typename std::conditional<MTraits::hasComputeInternalEnergy,
                                    InternalEnergyComputer,
                                    DoNothingEnergyComputer>::type;
      //! a simple alias
      using DEnergyComputer =
          typename std::conditional<MTraits::hasComputeDissipatedEnergy,
                                    DissipatedEnergyComputer,
                                    DoNothingEnergyComputer>::type;

      TFEL_ABAQUS_INLINE Integrator(const AbaqusData& d)
          : behaviour(&(d.DTIME),
                      d.TEMP,
                      d.DTEMP,
                      d.PROPS + AbaqusTraits<BV>::elasticPropertiesOffset +
                          AbaqusTraits<BV>::thermalExpansionPropertiesOffset,
                      d.STATEV,
                      d.PREDEF,
                      d.DPRED,
                      d.DROT),
            dt(d.DTIME) {
        using DVInitializer = typename std::conditional<
            MTraits::hasStressFreeExpansion,
            GradientInitialiserWithStressFreeExpansion,
            GradientInitialiserWithoutStressFreeExpansion>::type;
        SInitializer::exe(this->behaviour, d.PROPS);
        AInitializer::exe(this->behaviour, d.PROPS);
        DVInitializer::exe(this->behaviour, d.STRAN, d.DSTRAN, d.sfeh);
        this->behaviour.setABAQUSBehaviourDataThermodynamicForces(d.STRESS,
                                                                  d.DROT);
        this->behaviour.setOutOfBoundsPolicy(d.op);
        this->behaviour.initialize();
      }  // end of Integrator::Integrator

      TFEL_ABAQUS_INLINE2
      void exe(const AbaqusData& d) {
        using ConsistentTangentOperatorHandler = typename std::conditional<
            MTraits::hasConsistentTangentOperator,
            ExtractAndConvertTangentOperator<H>,
            ConsistentTangentOperatorIsNotAvalaible>::type;
        if (this->dt < 0.) {
          throwNegativeTimeStepException(MTraits::getName());
        }
        this->behaviour.checkBounds();
        auto r = BV::SUCCESS;
        const auto smflag =
            AbaqusTangentOperatorFlag<AbaqusTraits<BV>::btype>::value;
        auto tsf = behaviour.computeAPrioriTimeStepScalingFactor(*(d.PNEWDT));
        *(d.PNEWDT) = tsf.second;
        if (!tsf.first) {
          r = BV::FAILURE;
        } else {
          try {
            r = this->behaviour.integrate(smflag,
                                          BV::CONSISTENTTANGENTOPERATOR);
          } catch (tfel::material::DivergenceException&) {
            r = BV::FAILURE;
          }
          if (r == BV::FAILURE) {
            *(d.PNEWDT) = behaviour.getMinimalTimeStepScalingFactor();
          } else {
            tsf =
                behaviour.computeAPosterioriTimeStepScalingFactor(*(d.PNEWDT));
            if (!tsf.first) {
              r = BV::FAILURE;
            }
            *(d.PNEWDT) = std::min(tsf.second, *(d.PNEWDT));
          }
        }
        if (r == BV::FAILURE) {
          return;
        }
        this->behaviour.checkBounds();
        this->behaviour.ABAQUSexportStateData(d.STRESS, d.STATEV);
        ConsistentTangentOperatorHandler::exe(this->behaviour, d.DDSDDE);
        IEnergyComputer::exe(*(d.SSE), this->behaviour);
        DEnergyComputer::exe(*(d.SPD), this->behaviour);
      }  // end of Integrator::exe

     private:
      struct ConsistentTangentOperatorIsNotAvalaible {
        static void exe(BV&, AbaqusReal* const) {
          throwConsistentTangentOperatorIsNotAvalaible(MTraits::getName());
        }  // end of exe
      };
      BV behaviour;
      AbaqusReal dt;
    };  // end of struct Integrator

    TFEL_ABAQUS_INLINE2 static void checkNPROPS(const AbaqusInt NPROPS) {
      const unsigned short offset =
          (AbaqusTraits<BV>::elasticPropertiesOffset +
           AbaqusTraits<BV>::thermalExpansionPropertiesOffset);
      const unsigned short nprops = AbaqusTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset + nprops == 0 ? 1 : offset + nprops;
      const bool is_defined_ = MTraits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if ((NPROPS != NPROPS_) && is_defined_) {
        throwUnMatchedNumberOfMaterialProperties(MTraits::getName(), NPROPS_,
                                                 NPROPS);
      }
    }  // end of checkNPROPS

    TFEL_ABAQUS_INLINE2 static void checkNSTATV(const AbaqusInt NSTATV) {
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv = Traits::internal_variables_nb;
      const bool is_defined_ = MTraits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((nstatv != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), nstatv, NSTATV);
      }
    }  // end of checkNSTATV

    TFEL_ABAQUS_INLINE2 static void checkNTENS(const AbaqusInt ntens) {
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      TFEL_CONSTEXPR const auto size = AbaqusStensorSize<H>::value;
      TFEL_CONSTEXPR const bool is_defined_ = MTraits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((ntens != size) && is_defined_) {
        throwInvalidTensorSize(Traits::getName(), ntens, size);
      }
    }  // end of checkNSTATV

  };  // end of struct AbaqusBehaviourHandler

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSBEHAVIOURHANDLER_HXX */
