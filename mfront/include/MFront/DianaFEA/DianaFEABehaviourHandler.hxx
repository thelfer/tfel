/*!
 * \file  mfront/include/MFront/DianaFEA/DianaFEABehaviourHandler.hxx
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

#ifndef LIB_MFRONT_DIANAFEA_DIANAFEABEHAVIOURHANDLER_HXX
#define LIB_MFRONT_DIANAFEA_DIANAFEABEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_DIANAFEA_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_DIANAFEA_CALL_HXX */

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "MFront/DianaFEA/DianaFEAData.hxx"
#include "MFront/DianaFEA/DianaFEATangentOperator.hxx"
#include "MFront/DianaFEA/DianaFEAInterfaceExceptions.hxx"
#include "MFront/DianaFEA/DianaFEAComputeStiffnessTensor.hxx"
#include "MFront/DianaFEA/DianaFEAComputeThermalExpansionCoefficientTensor.hxx"

namespace dianafea {

  template <DianaFEABehaviourType btype>
  struct DianaFEATangentOperatorFlag;

  template <>
  struct DianaFEATangentOperatorFlag<dianafea::STANDARDSTRAINBASEDBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value =
        TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL DianaFEABehaviourHandler
      : public DianaFEAInterfaceExceptions {
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithStressFreeExpansion
        : public DianaFEAInterfaceExceptions {
      //! a simple alias
      typedef Behaviour<H, DianaFEAReal, false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_DIANAFEA_INLINE static void exe(
          BV& b,
          const DianaFEAReal* const STRAN,
          const DianaFEAReal* const DSTRAN,
          const StressFreeExpansionHandler<DianaFEAReal>& sfeh) {
        using std::pair;
        using tfel::fsalgo::copy;
        using namespace tfel::material;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        const DianaFEAInt N = ModellingHypothesisToSpaceDimension<H>::value;
        DianaFEAReal dv0[DianaFEATraits<BV>::GradientSize];
        DianaFEAReal dv1[DianaFEATraits<BV>::GradientSize];
        copy<DianaFEATraits<BV>::GradientSize>::exe(STRAN, dv0);
        copy<DianaFEATraits<BV>::GradientSize>::exe(DSTRAN, dv1);
        // check that the function pointer are not null
        if (sfeh == nullptr) {
          throwUnsupportedStressFreeExpansionException(Traits::getName());
        }
        pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        const auto& s0 = s.first;
        const auto& s1 = s.second;
        sfeh(dv0, dv1, &s0[0], &s1[0], N);
        b.setDIANAFEABehaviourDataGradients(dv0);
        b.setDIANAFEAIntegrationDataGradients(dv1);
      }  // end of exe

    };  // end of struct GradientInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithoutStressFreeExpansion {
      //! a simple alias
      typedef Behaviour<H, DianaFEAReal, false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_DIANAFEA_INLINE static void exe(
          BV& b,
          const DianaFEAReal* const STRAN,
          const DianaFEAReal* const DSTRAN,
          const StressFreeExpansionHandler<DianaFEAReal>&) {
        b.setDIANAFEABehaviourDataGradients(STRAN);
        b.setDIANAFEAIntegrationDataGradients(DSTRAN);
      }  // end of exe
    };   // end of struct GradientInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef Behaviour<H, DianaFEAReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_DIANAFEA_INLINE static void exe(BData& data,
                                           const DianaFEAReal* const props) {
        typedef DianaFEATraits<BV> Traits;
        const bool buas = Traits::requiresUnAlteredStiffnessTensor;
        DianaFEAComputeStiffnessTensor<DianaFEATraits<BV>::btype, H,
                                       DianaFEATraits<BV>::etype,
                                       buas>::exe(data.getStiffnessTensor(),
                                                  props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef Behaviour<H, DianaFEAReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_DIANAFEA_INLINE static void exe(BData& data,
                                           const DianaFEAReal* const props) {
        const unsigned short o = DianaFEATraits<BV>::elasticPropertiesOffset;
        DianaFEAComputeThermalExpansionCoefficientTensor<
            DianaFEATraits<BV>::btype, H, DianaFEATraits<BV>::stype>::
            exe(props + o, data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef Behaviour<H, DianaFEAReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_DIANAFEA_INLINE static void exe(BData&, const DianaFEAReal* const) {}
    };  // end of struct DoNothingInitializer

    template <const bool bs,  // requires StiffnessOperator
              const bool ba>  // requires ThermalExpansionCoefficientTensor
    struct TFEL_VISIBILITY_LOCAL Integrator {
      //! An alias to the class handling the stiffness tensor computation
      using SInitializer =
          typename std::conditional<bs,
                                    StiffnessOperatorInitializer,
                                    DoNothingInitializer>::type;
      //! An alias to the class handling the thermal expansion coefficient
      //! computation
      using AInitializer = typename std::conditional<
          ba,
          ThermalExpansionCoefficientTensorInitializer,
          DoNothingInitializer>::type;

      TFEL_DIANAFEA_INLINE Integrator(const DianaFEAData& d)
          : behaviour(&(d.DTIME),
                      &(d.TEMP),
                      &(d.DTEMP),
                      d.PROPS + DianaFEATraits<BV>::elasticPropertiesOffset +
                          DianaFEATraits<BV>::thermalExpansionPropertiesOffset,
                      d.STATEV),
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
        this->behaviour.setDIANAFEABehaviourDataThermodynamicForces(d.STRESS);
        this->behaviour.setOutOfBoundsPolicy(d.op);
      }  // end of Integrator::Integrator

      TFEL_DIANAFEA_INLINE2
      void exe(const DianaFEAData& d) {
        using namespace tfel::material;
        using Traits = MechanicalBehaviourTraits<BV>;
        using ConsistentTangentOperatorHandler = typename std::conditional<
            Traits::hasConsistentTangentOperator,
            ExtractAndConvertTangentOperator<H>,
            ConsistentTangentOperatorIsNotAvalaible>::type;
        if (this->dt < 0.) {
          throwNegativeTimeStepException(Traits::getName());
        }
        if (!this->behaviour.initialize()) {
          return;
        }
        this->behaviour.checkBounds();
        auto r = BV::SUCCESS;
        const auto smflag =
            DianaFEATangentOperatorFlag<DianaFEATraits<BV>::btype>::value;
        try {
          r = this->behaviour.integrate(smflag, BV::CONSISTENTTANGENTOPERATOR);
        } catch (DivergenceException&) {
          r = BV::FAILURE;
        }
        if (r == BV::FAILURE) {
          return;
        }
        this->behaviour.checkBounds();
        this->behaviour.DIANAFEAexportStateData(d.STRESS, d.STATEV);
        ConsistentTangentOperatorHandler::exe(this->behaviour, d.DDSDDE);
      }  // end of Integrator::exe

     private:
      struct ConsistentTangentOperatorIsNotAvalaible {
        typedef Behaviour<H, DianaFEAReal, false> BV;
        static void exe(BV&, DianaFEAReal* const) {
          using Traits = tfel::material::MechanicalBehaviourTraits<BV>;
          throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
        }  // end of exe
      };
      //! a simple alias
      using BV = Behaviour<H, DianaFEAReal, false>;
      BV behaviour;
      DianaFEAReal dt;
    };  // end of struct Integrator

    TFEL_DIANAFEA_INLINE2 static void checkNPROPS(const DianaFEAInt NPROPS) {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H, DianaFEAReal, false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const auto offset =
          (DianaFEATraits<BV>::elasticPropertiesOffset +
           DianaFEATraits<BV>::thermalExpansionPropertiesOffset);
      const auto nprops = DianaFEATraits<BV>::material_properties_nb;
      const auto NPROPS_ = offset + nprops;
      const auto is_defined_ = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if ((NPROPS != NPROPS_) && is_defined_) {
        throwUnMatchedNumberOfMaterialProperties(Traits::getName(), NPROPS_,
                                                 NPROPS);
      }
    }  // end of checkNPROPS

    TFEL_DIANAFEA_INLINE2 static void checkNSTATV(const DianaFEAInt NSTATV) {
      typedef Behaviour<H, DianaFEAReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const auto nstatv = Traits::internal_variables_nb;
      const auto is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((nstatv != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), nstatv, NSTATV);
      }
    }  // end of checkNSTATV

  };  // end of struct DianaFEABehaviourHandler

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEA_DIANAFEABEHAVIOURHANDLER_HXX */
