/*!
 * \file  mfront/include/MFront/Europlexus/EuroplexusBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_EPX_EUROPLEXUSBEHAVIOURHANDLER_H_
#define LIB_MFRONT_EPX_EUROPLEXUSBEHAVIOURHANDLER_H_

#ifndef LIB_MFRONT_EPX_CALL_H_
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_EPX_CALL_H_ */

#include "TFEL/Math/Matrix/TMatrixView.hxx"
#include "TFEL/Math/T2toST2/T2toST2View.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"

#include "MFront/Europlexus/EuroplexusInterfaceExceptions.hxx"
#include "MFront/Europlexus/EuroplexusComputeStiffnessTensor.hxx"
#include "MFront/Europlexus/EuroplexusComputeThermalExpansionCoefficientTensor.hxx"

namespace epx {

  template <EuroplexusBehaviourType btype>
  struct EuroplexusTangentOperatorFlag;

  template <>
  struct EuroplexusTangentOperatorFlag<epx::SMALLSTRAINSTANDARDBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value =
        TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template <>
  struct EuroplexusTangentOperatorFlag<epx::FINITESTRAINSTANDARDBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value =
        TangentOperatorTraits::DSIG_DF;
  };

  template <EuroplexusBehaviourType btype, unsigned short N>
  struct EuroplexusTangentOperatorType;

  template <unsigned short N>
  struct EuroplexusTangentOperatorType<epx::SMALLSTRAINSTANDARDBEHAVIOUR, N> {
    using type = tfel::math::st2tost2<N, EuroplexusReal>;
    using view_type = tfel::math::ST2toST2View<N, EuroplexusReal>;
  };

  template <unsigned short N>
  struct EuroplexusTangentOperatorType<epx::FINITESTRAINSTANDARDBEHAVIOUR, N> {
    using type = tfel::math::t2tost2<N, EuroplexusReal>;
    using view_type = tfel::math::T2toST2View<N, EuroplexusReal>;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL EuroplexusBehaviourHandler
      : public EuroplexusInterfaceExceptions {
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL
        DrivingVariableInitialiserWithStressFreeExpansion
        : public EuroplexusInterfaceExceptions {
      //! a simple alias
      typedef Behaviour<H, EuroplexusReal, false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_EPX_INLINE static void exe(BV& b,
                                      const EuroplexusReal* const STRAN,
                                      const EuroplexusReal* const DSTRAN,
                                      const StressFreeExpansionHandler& sfeh) {
        using std::pair;
        using tfel::fsalgo::copy;
        using namespace tfel::material;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        const EuroplexusInt N = ModellingHypothesisToSpaceDimension<H>::value;
        EuroplexusReal dv0[EuroplexusTraits<BV>::DrivingVariableSize];
        EuroplexusReal dv1[EuroplexusTraits<BV>::DrivingVariableSize];
        copy<EuroplexusTraits<BV>::DrivingVariableSize>::exe(STRAN, dv0);
        copy<EuroplexusTraits<BV>::DrivingVariableSize>::exe(DSTRAN, dv1);
        // check that the function pointer are not null
        if (sfeh == nullptr) {
          throwUnsupportedStressFreeExpansionException(Traits::getName());
        }
        pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        const auto& s0 = s.first;
        const auto& s1 = s.second;
        sfeh(dv0, dv1, &s0[0], &s1[0], N);
        b.setEUROPLEXUSBehaviourDataDrivingVariables(dv0);
        b.setEUROPLEXUSIntegrationDataDrivingVariables(dv1);
      }  // end of exe

    };  // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL
        DrivingVariableInitialiserWithoutStressFreeExpansion {
      //! a simple alias
      typedef Behaviour<H, EuroplexusReal, false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_EPX_INLINE static void exe(BV& b,
                                      const EuroplexusReal* const STRAN,
                                      const EuroplexusReal* const DSTRAN,
                                      const StressFreeExpansionHandler&) {
        b.setEUROPLEXUSBehaviourDataDrivingVariables(STRAN);
        b.setEUROPLEXUSIntegrationDataDrivingVariables(DSTRAN);
      }  // end of exe
    };   // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_EPX_INLINE static void exe(BData& data,
                                      const EuroplexusReal* const props) {
        typedef EuroplexusTraits<BV> Traits;
        const bool buas = Traits::requiresUnAlteredStiffnessTensor;
        EuroplexusComputeStiffnessTensor<EuroplexusTraits<BV>::btype, H,
                                         EuroplexusTraits<BV>::etype,
                                         buas>::exe(data.getStiffnessTensor(),
                                                    props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_EPX_INLINE static void exe(BData& data,
                                      const EuroplexusReal* const props) {
        const unsigned short o = EuroplexusTraits<BV>::elasticPropertiesOffset;
        EuroplexusComputeThermalExpansionCoefficientTensor<
            EuroplexusTraits<BV>::btype, H, EuroplexusTraits<BV>::stype>::
            exe(props + o, data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_EPX_INLINE static void exe(BData&, const EuroplexusReal* const) {}
    };  // end of struct DoNothingInitializer

    template <const bool bs,  // requires StiffnessOperator
              const bool ba>  // requires ThermalExpansionCoefficientTensor
    struct TFEL_VISIBILITY_LOCAL Integrator {
      // a simple alias
      using BV = Behaviour<H, EuroplexusReal, false>;

      typedef
          typename std::conditional<bs,
                                    StiffnessOperatorInitializer,
                                    DoNothingInitializer>::type SInitializer;

      typedef typename std::conditional<
          ba,
          ThermalExpansionCoefficientTensorInitializer,
          DoNothingInitializer>::type AInitializer;

      TFEL_EPX_INLINE Integrator(const EPXData& d,
                                 const EuroplexusReal* dv0,
                                 const EuroplexusReal* dv1)
          : behaviour(
                &(d.DTIME),
                d.TEMP,
                d.DTEMP,
                d.PROPS + EuroplexusTraits<BV>::elasticPropertiesOffset +
                    EuroplexusTraits<BV>::thermalExpansionPropertiesOffset,
                d.STATEV,
                d.PREDEF,
                d.DPRED),
            dt(d.DTIME) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasStressFreeExpansion,
            DrivingVariableInitialiserWithStressFreeExpansion,
            DrivingVariableInitialiserWithoutStressFreeExpansion>::type
            DVInitializer;
        SInitializer::exe(this->behaviour, d.PROPS);
        AInitializer::exe(this->behaviour, d.PROPS);
        DVInitializer::exe(this->behaviour, dv0, dv1, d.sfeh);
        this->behaviour.setEUROPLEXUSBehaviourDataThermodynamicForces(d.STRESS);
        this->behaviour.setOutOfBoundsPolicy(d.op);
        this->behaviour.initialize();
      }  // end of Integrator::Integrator

      TFEL_EPX_INLINE2
      void exe(const EPXData& d) {
        using Traits = tfel::material::MechanicalBehaviourTraits<BV>;
        using ConsistentTangentOperatorHandler = typename std::conditional<
            Traits::hasConsistentTangentOperator,
            typename std::conditional<
                Traits::isConsistentTangentOperatorSymmetric,
                SymmetricConsistentTangentOperatorComputer,
                GeneralConsistentTangentOperatorComputer>::type,
            ConsistentTangentOperatorIsNotAvalaible>::type;
        EuroplexusReal* const PNEWDT = d.PNEWDT;
        EuroplexusReal* const STRESS = d.STRESS;
        EuroplexusReal* const DDSDDE = d.DDSDDE;
        EuroplexusReal* const STATEV = d.STATEV;
        if (this->dt < 0.) {
          throwNegativeTimeStepException(Traits::getName());
        }
        this->behaviour.checkBounds();
        const auto r = (*DDSDDE < -0.5)
                           ? this->computePredictionOperator(*DDSDDE)
                           : this->integrate(PNEWDT, STRESS, STATEV, DDSDDE);
        if (r == BV::SUCCESS) {
          *(d.STATUS) = 0;
          if ((*DDSDDE > 0.5) || (*DDSDDE < -0.5)) {
            ConsistentTangentOperatorHandler::exe(this->behaviour, DDSDDE);
          }
        } else {
          if (!(*PNEWDT < 1)) {
            throwInvalidTimeStepScalingFactorOnFailure(Traits::getName(),
                                                       *PNEWDT);
          }
          *(d.STATUS) = -1;
        }
      }  // end of Integrator::exe

     private:
      TFEL_EPX_INLINE2 typename BV::IntegrationResult computePredictionOperator(
          const EuroplexusReal DDSDDE) {
        using Traits = tfel::material::MechanicalBehaviourTraits<BV>;
        using PredictionOperatorComputer =
            typename std::conditional<Traits::hasPredictionOperator,
                                      StandardPredictionOperatorComputer,
                                      PredictionOperatorIsNotAvalaible>::type;
        const auto smflag =
            EuroplexusTangentOperatorFlag<EuroplexusTraits<BV>::btype>::value;
        const auto smtype = [](const EuroplexusReal& v) {
          if ((-3.25 < v) && (v < -2.75)) {
            return BV::TANGENTOPERATOR;
          } else if ((-2.25 < v) && (v < -1.75)) {
            return BV::SECANTOPERATOR;
          } else if ((-1.25 < v) && (v < -0.75)) {
            return BV::ELASTIC;
          }
          return BV::NOSTIFFNESSREQUESTED;
        }(DDSDDE);
        if (smtype == BV::NOSTIFFNESSREQUESTED) {
          throwInvalidDDSDDEValueException(Traits::getName(), DDSDDE);
        }
        const auto r =
            PredictionOperatorComputer::exe(this->behaviour, smflag, smtype);
        if (r == BV::FAILURE) {
          throwPredictionComputationFailedException(Traits::getName());
        }
        return r;
      }

      TFEL_EPX_INLINE2 typename BV::IntegrationResult integrate(
          EuroplexusReal* const PNEWDT,
          EuroplexusReal* const STRESS,
          EuroplexusReal* const STATEV,
          const EuroplexusReal* const DDSDDE) {
        using Traits = tfel::material::MechanicalBehaviourTraits<BV>;
        const auto smflag =
            EuroplexusTangentOperatorFlag<EuroplexusTraits<BV>::btype>::value;
        auto r = BV::SUCCESS;
        this->behaviour.checkBounds();
        auto tsf = behaviour.computeAPrioriTimeStepScalingFactor(*PNEWDT);
        *PNEWDT = tsf.second;
        if (!tsf.first) {
          r = BV::FAILURE;
          return r;
        }
        try {
          if ((-0.25 < *DDSDDE) && (*DDSDDE < 0.25)) {
            r = this->behaviour.integrate(smflag, BV::NOSTIFFNESSREQUESTED);
          } else if ((0.75 < *DDSDDE) && (*DDSDDE < 1.25)) {
            r = this->behaviour.integrate(smflag, BV::ELASTIC);
          } else if ((1.75 < *DDSDDE) && (*DDSDDE < 2.25)) {
            r = this->behaviour.integrate(smflag, BV::SECANTOPERATOR);
          } else if ((2.75 < *DDSDDE) && (*DDSDDE < 3.25)) {
            r = this->behaviour.integrate(smflag, BV::TANGENTOPERATOR);
          } else if ((3.75 < *DDSDDE) && (*DDSDDE < 4.25)) {
            r = this->behaviour.integrate(smflag,
                                          BV::CONSISTENTTANGENTOPERATOR);
          } else {
            throwInvalidDDSDDEValueException(Traits::getName(), *DDSDDE);
          }
        } catch (tfel::material::DivergenceException&) {
          r = BV::FAILURE;
          *PNEWDT = behaviour.getMinimalTimeStepScalingFactor();
        }
        if (r == BV::SUCCESS) {
          tsf = behaviour.computeAPosterioriTimeStepScalingFactor(*PNEWDT);
          if (!tsf.first) {
            r = BV::FAILURE;
          }
          *PNEWDT = std::min(tsf.second, *PNEWDT);
        }
        if (r == BV::SUCCESS) {
          this->behaviour.checkBounds();
          this->behaviour.EUROPLEXUSexportStateData(STRESS, STATEV);
        }
        return r;
      }

      BV behaviour;
      EuroplexusReal dt;
    };  // end of struct Integrator

    struct StandardPredictionOperatorComputer {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      static typename BV::IntegrationResult exe(BV& b,
                                                const typename BV::SMFlag smf,
                                                const typename BV::SMType smt) {
        return b.computePredictionOperator(smf, smt);
      }  // end of exe
    };

    struct PredictionOperatorIsNotAvalaible {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      static typename BV::IntegrationResult exe(BV&,
                                                const typename BV::SMFlag,
                                                const typename BV::SMType) {
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        throwPredictionOperatorIsNotAvalaible(Traits::getName());
        return BV::FAILURE;
      }  // end of exe
    };

    struct ConsistentTangentOperatorIsNotAvalaible {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      static void exe(BV&, EuroplexusReal* const) {
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
      }  // end of exe
    };

    struct ConsistentTangentOperatorComputer {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      static void exe(const BV& bv, EuroplexusReal* const DDSOE) {
        using tfel::material::ModellingHypothesisToSpaceDimension;
        const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
        using TangentOperatorType =
            typename EuroplexusTangentOperatorType<EuroplexusTraits<BV>::btype,
                                                   N>::type;
        using TangentOperatorViewType =
            typename EuroplexusTangentOperatorType<EuroplexusTraits<BV>::btype,
                                                   N>::view_type;
        TangentOperatorViewType Dt{DDSOE};
        Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
      }  // end of exe
    };

    struct SymmetricConsistentTangentOperatorComputer {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      static void exe(const BV& bv, EuroplexusReal* const DDSOE) {
        ConsistentTangentOperatorComputer::exe(bv, DDSOE);
      }  // end of exe
    };

    struct GeneralConsistentTangentOperatorComputer {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      static void exe(const BV& bv, EuroplexusReal* const DDSOE) {
        using tfel::material::ModellingHypothesisToSpaceDimension;
        const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
        using TangentOperatorViewType =
            typename EuroplexusTangentOperatorType<EuroplexusTraits<BV>::btype,
                                                   N>::view_type;
        ConsistentTangentOperatorComputer::exe(bv, DDSOE);
        TangentOperatorViewType Dt{DDSOE};
        //	EuroplexusTangentOperator::transpose(Dt);
      }  // end of exe
    };

    TFEL_EPX_INLINE2 static void checkNPROPS(const EuroplexusInt NPROPS) {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H, EuroplexusReal, false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      constexpr const unsigned short offset =
          (EuroplexusTraits<BV>::elasticPropertiesOffset +
           EuroplexusTraits<BV>::thermalExpansionPropertiesOffset);
      constexpr const unsigned short nprops =
          EuroplexusTraits<BV>::material_properties_nb + offset;
      constexpr const bool is_defined_ = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if ((NPROPS != nprops) && is_defined_) {
        throwUnMatchedNumberOfMaterialProperties(Traits::getName(), nprops,
                                                 NPROPS);
      }
    }  // end of checkNPROPS

    TFEL_EPX_INLINE2 static void checkNSTATV(const EuroplexusInt NSTATV) {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      constexpr const unsigned short nstatv = Traits::internal_variables_nb;
      constexpr const bool is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((nstatv != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), nstatv, NSTATV);
      }
    }  // end of checkNSTATV

    TFEL_EPX_INLINE2 static void checkNPREDEF(const EuroplexusInt NPREDEF) {
      typedef Behaviour<H, EuroplexusReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      constexpr const unsigned short npredef = Traits::external_variables_nb;
      constexpr const bool is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((npredef != NPREDEF) && is_defined_) {
        throwUnMatchedNumberOfExternalStateVariables(Traits::getName(), npredef,
                                                     NPREDEF);
      }
    }  // end of checkNPREDEF

  };  // end of struct EuroplexusBehaviourHandler

}  // end of namespace epx

#endif /* LIB_MFRONT_EPX_EUROPLEXUSBEHAVIOURHANDLER_H_ */
