/*!
 * \file  mfront/include/MFront/LSDYNA/LSDYNABehaviourHandler.hxx
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

#ifndef LIB_MFRONT_LSDYNA_LSDYNABEHAVIOURHANDLER_HXX
#define LIB_MFRONT_LSDYNA_LSDYNABEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_LSDYNA_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_LSDYNA_CALL_HXX */

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/TMatrixView.hxx"
#include "TFEL/Math/T2toST2/T2toST2View.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

#include "MFront/LSDYNA/LSDYNAData.hxx"
#include "MFront/LSDYNA/LSDYNATangentOperator.hxx"
#include "MFront/LSDYNA/LSDYNAInterfaceExceptions.hxx"
#include "MFront/LSDYNA/LSDYNAComputeStiffnessTensor.hxx"
#include "MFront/LSDYNA/LSDYNAComputeThermalExpansionCoefficientTensor.hxx"

namespace lsdyna {

  template <LSDYNABehaviourType btype>
  struct LSDYNATangentOperatorFlag;

  template <>
  struct LSDYNATangentOperatorFlag<lsdyna::STANDARDSTRAINBASEDBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr auto value =
        TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template <>
  struct LSDYNATangentOperatorFlag<lsdyna::STANDARDFINITESTRAINBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr auto value = TangentOperatorTraits::LSDYNA;
  };

  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct LSDYNAStensorSize
      : public tfel::material::ModellingHypothesisToStensorSize<H> {
  };  // end of struct LSDYNAStensorSize

  template <>
  struct LSDYNAStensorSize<tfel::material::ModellingHypothesis::PLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 3u;
  };  // end of struct LSDYNAStensorSize

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL LSDYNABehaviourHandler
      : public LSDYNAInterfaceExceptions {
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithStressFreeExpansion
        : public LSDYNAInterfaceExceptions {
      //! a simple alias
      typedef Behaviour<H, LSDYNAReal, false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_LSDYNA_INLINE static void exe(
          BV& b,
          const LSDYNAReal* const STRAN,
          const LSDYNAReal* const DSTRAN,
          const StressFreeExpansionHandler<LSDYNAReal>& sfeh) {
        using std::pair;
        using tfel::fsalgo::copy;
        using namespace tfel::material;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        const LSDYNAInt N = ModellingHypothesisToSpaceDimension<H>::value;
        LSDYNAReal dv0[LSDYNATraits<BV>::GradientSize];
        LSDYNAReal dv1[LSDYNATraits<BV>::GradientSize];
        copy<LSDYNATraits<BV>::GradientSize>::exe(STRAN, dv0);
        copy<LSDYNATraits<BV>::GradientSize>::exe(DSTRAN, dv1);
        // check that the function pointer are not null
        if (sfeh == nullptr) {
          throwUnsupportedStressFreeExpansionException(Traits::getName());
        }
        pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        const auto& s0 = s.first;
        const auto& s1 = s.second;
        sfeh(dv0, dv1, &s0[0], &s1[0], N);
        b.setLSDYNABehaviourDataGradients(dv0);
        b.setLSDYNAIntegrationDataGradients(dv1);
      }  // end of exe

    };  // end of struct GradientInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithoutStressFreeExpansion {
      //! a simple alias
      typedef Behaviour<H, LSDYNAReal, false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_LSDYNA_INLINE static void exe(
          BV& b,
          const LSDYNAReal* const STRAN,
          const LSDYNAReal* const DSTRAN,
          const StressFreeExpansionHandler<LSDYNAReal>&) {
        b.setLSDYNABehaviourDataGradients(STRAN);
        b.setLSDYNAIntegrationDataGradients(DSTRAN);
      }  // end of exe
    };   // end of struct GradientInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef Behaviour<H, LSDYNAReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_LSDYNA_INLINE static void exe(BData& data,
                                         const LSDYNAReal* const props) {
        typedef LSDYNATraits<BV> Traits;
        const bool buas = Traits::requiresUnAlteredStiffnessTensor;
        LSDYNAComputeStiffnessTensor<LSDYNATraits<BV>::btype, H,
                                     LSDYNATraits<BV>::etype,
                                     buas>::exe(data.getStiffnessTensor(),
                                                props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef Behaviour<H, LSDYNAReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_LSDYNA_INLINE static void exe(BData& data,
                                         const LSDYNAReal* const props) {
        const unsigned short o = LSDYNATraits<BV>::elasticPropertiesOffset;
        LSDYNAComputeThermalExpansionCoefficientTensor<
            LSDYNATraits<BV>::btype, H, LSDYNATraits<BV>::stype>::
            exe(props + o, data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef Behaviour<H, LSDYNAReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_LSDYNA_INLINE static void exe(BData&, const LSDYNAReal* const) {}
    };  // end of struct DoNothingInitializer

    template <const bool bs,  // requires StiffnessOperator
              const bool ba>  // requires ThermalExpansionCoefficientTensor
    struct TFEL_VISIBILITY_LOCAL Integrator {
      typedef
          typename std::conditional<bs,
                                    StiffnessOperatorInitializer,
                                    DoNothingInitializer>::type SInitializer;

      typedef typename std::conditional<
          ba,
          ThermalExpansionCoefficientTensorInitializer,
          DoNothingInitializer>::type AInitializer;

      TFEL_LSDYNA_INLINE Integrator(const LSDYNAData& d)
          : behaviour(&(d.DTIME),
                      d.TEMP,
                      d.DTEMP,
                      d.PROPS + LSDYNATraits<BV>::elasticPropertiesOffset +
                          LSDYNATraits<BV>::thermalExpansionPropertiesOffset,
                      d.STATEV,
                      d.PREDEF,
                      d.DPRED,
                      d.DROT),
            dt(d.DTIME) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasStressFreeExpansion,
            GradientInitialiserWithStressFreeExpansion,
            GradientInitialiserWithoutStressFreeExpansion>::type DVInitializer;
        SInitializer::exe(this->behaviour, d.PROPS);
        AInitializer::exe(this->behaviour, d.PROPS);
        DVInitializer::exe(this->behaviour, d.STRAN, d.DSTRAN, d.sfeh);
        this->behaviour.setLSDYNABehaviourDataThermodynamicForces(d.STRESS,
                                                                  d.DROT);
        this->behaviour.setOutOfBoundsPolicy(d.op);
        this->behaviour.initialize();
      }  // end of Integrator::Integrator

      TFEL_LSDYNA_INLINE2
      void exe(const LSDYNAData& d) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef
            typename std::conditional<Traits::hasConsistentTangentOperator,
                                      ExtractAndConvertTangentOperator<H>,
                                      ConsistentTangentOperatorIsNotAvalaible>::
                type ConsistentTangentOperatorHandler;
        if (this->dt < 0.) {
          throwNegativeTimeStepException(Traits::getName());
        }
        this->behaviour.checkBounds();
        auto r = BV::SUCCESS;
        const auto smflag =
            LSDYNATangentOperatorFlag<LSDYNATraits<BV>::btype>::value;
        auto tsf = behaviour.computeAPrioriTimeStepScalingFactor(*(d.PNEWDT));
        *(d.PNEWDT) = tsf.second;
        if (!tsf.first) {
          r = BV::FAILURE;
        } else {
          try {
            r = this->behaviour.integrate(smflag,
                                          BV::CONSISTENTTANGENTOPERATOR);
          } catch (DivergenceException&) {
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
        this->behaviour.LSDYNAexportStateData(d.STRESS, d.STATEV);
        ConsistentTangentOperatorHandler::exe(this->behaviour, d.DDSDDE);
      }  // end of Integrator::exe

     private:
      struct ConsistentTangentOperatorIsNotAvalaible {
        typedef Behaviour<H, LSDYNAReal, false> BV;
        static void exe(BV&, LSDYNAReal* const) {
          typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
          throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
        }  // end of exe
      };

      typedef Behaviour<H, LSDYNAReal, false> BV;
      BV behaviour;
      LSDYNAReal dt;
    };  // end of struct Integrator

    TFEL_LSDYNA_INLINE2 static void checkNPROPS(const LSDYNAInt NPROPS) {
      using namespace tfel::material;
      typedef Behaviour<H, LSDYNAReal, false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset =
          (LSDYNATraits<BV>::elasticPropertiesOffset +
           LSDYNATraits<BV>::thermalExpansionPropertiesOffset);
      const unsigned short nprops = LSDYNATraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset + nprops == 0 ? 1 : offset + nprops;
      const bool is_defined_ = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if ((NPROPS != NPROPS_) && is_defined_) {
        throwUnMatchedNumberOfMaterialProperties(Traits::getName(), NPROPS_,
                                                 NPROPS);
      }
    }  // end of checkNPROPS

    TFEL_LSDYNA_INLINE2 static void checkNSTATV(const LSDYNAInt NSTATV) {
      typedef Behaviour<H, LSDYNAReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv = Traits::internal_variables_nb;
      const bool is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((nstatv != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), nstatv, NSTATV);
      }
    }  // end of checkNSTATV

    TFEL_LSDYNA_INLINE2 static void checkNTENS(const LSDYNAInt ntens) {
      typedef Behaviour<H, LSDYNAReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      TFEL_CONSTEXPR const auto size = LSDYNAStensorSize<H>::value;
      TFEL_CONSTEXPR const bool is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((ntens != size) && is_defined_) {
        throwInvalidTensorSize(Traits::getName(), ntens, size);
      }
    }  // end of checkNSTATV

  };  // end of struct LSDYNABehaviourHandler

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNA_LSDYNABEHAVIOURHANDLER_HXX */
