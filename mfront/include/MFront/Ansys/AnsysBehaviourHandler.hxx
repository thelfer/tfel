/*!
 * \file  mfront/include/MFront/Ansys/AnsysBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_ANSYS_ANSYSBEHAVIOURHANDLER_HXX
#define LIB_MFRONT_ANSYS_ANSYSBEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_ANSYS_ANSYSINTERFACE_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_ANSYS_ANSYSINTERFACE_HXX */

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

#include "MFront/Ansys/AnsysData.hxx"
#include "MFront/Ansys/AnsysTangentOperator.hxx"
#include "MFront/Ansys/AnsysInterfaceExceptions.hxx"
#include "MFront/Ansys/AnsysComputeStiffnessTensor.hxx"
#include "MFront/Ansys/AnsysComputeThermalExpansionCoefficientTensor.hxx"

namespace ansys {

  template <AnsysBehaviourType btype>
  struct AnsysTangentOperatorFlag;

  template <>
  struct AnsysTangentOperatorFlag<ansys::STANDARDSTRAINBASEDBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value =
        TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template <>
  struct AnsysTangentOperatorFlag<ansys::STANDARDFINITESTRAINBEHAVIOUR> {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR>
        TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value =
        TangentOperatorTraits::ABAQUS;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AnsysBehaviourHandler
      : public AnsysInterfaceExceptions {
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithStressFreeExpansion
        : public AnsysInterfaceExceptions {
      //! a simple alias
      typedef Behaviour<H, AnsysReal, false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ANSYS_INLINE static void exe(
          BV& b,
          const AnsysReal* const STRAN,
          const AnsysReal* const DSTRAN,
          const StressFreeExpansionHandler<AnsysReal>& sfeh) {
        using std::pair;
        using tfel::fsalgo::copy;
        using namespace tfel::material;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        const AnsysInt N = ModellingHypothesisToSpaceDimension<H>::value;
        AnsysReal dv0[AnsysTraits<BV>::GradientSize];
        AnsysReal dv1[AnsysTraits<BV>::GradientSize];
        copy<AnsysTraits<BV>::GradientSize>::exe(STRAN, dv0);
        copy<AnsysTraits<BV>::GradientSize>::exe(DSTRAN, dv1);
        // check that the function pointer are not null
        if (sfeh == nullptr) {
          throwUnsupportedStressFreeExpansionException(Traits::getName());
        }
        pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        const auto& s0 = s.first;
        const auto& s1 = s.second;
        sfeh(dv0, dv1, &s0[0], &s1[0], N);
        b.setANSYSBehaviourDataGradients(dv0);
        b.setANSYSIntegrationDataGradients(dv1);
      }  // end of exe

    };  // end of struct GradientInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL GradientInitialiserWithoutStressFreeExpansion {
      //! a simple alias
      typedef Behaviour<H, AnsysReal, false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ANSYS_INLINE static void exe(
          BV& b,
          const AnsysReal* const STRAN,
          const AnsysReal* const DSTRAN,
          const StressFreeExpansionHandler<AnsysReal>&) {
        b.setANSYSBehaviourDataGradients(STRAN);
        b.setANSYSIntegrationDataGradients(DSTRAN);
      }  // end of exe
    };   // end of struct GradientInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef Behaviour<H, AnsysReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_ANSYS_INLINE static void exe(BData& data,
                                        const AnsysReal* const props) {
        typedef AnsysTraits<BV> Traits;
        const bool buas = Traits::requiresUnAlteredStiffnessTensor;
        AnsysComputeStiffnessTensor<AnsysTraits<BV>::btype, H,
                                    AnsysTraits<BV>::etype,
                                    buas>::exe(data.getStiffnessTensor(),
                                               props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef Behaviour<H, AnsysReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_ANSYS_INLINE static void exe(BData& data,
                                        const AnsysReal* const props) {
        const unsigned short o = AnsysTraits<BV>::elasticPropertiesOffset;
        AnsysComputeThermalExpansionCoefficientTensor<AnsysTraits<BV>::btype, H,
                                                      AnsysTraits<BV>::stype>::
            exe(props + o, data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef Behaviour<H, AnsysReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_ANSYS_INLINE static void exe(BData&, const AnsysReal* const) {}
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

      TFEL_ANSYS_INLINE Integrator(const AnsysData& d)
          : behaviour(&(d.DTIME),
                      d.TEMP,
                      d.DTEMP,
                      d.PROPS + AnsysTraits<BV>::elasticPropertiesOffset +
                          AnsysTraits<BV>::thermalExpansionPropertiesOffset +
                          AnsysTraits<BV>::orthotropicAxesOffset,
                      d.STATEV,
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
        this->behaviour.setANSYSBehaviourDataThermodynamicForces(d.STRESS,
                                                                 d.DROT);
        this->behaviour.setOutOfBoundsPolicy(d.op);
      }  // end of Integrator::Integrator

      TFEL_ANSYS_INLINE2
      bool exe(const AnsysData& d) {
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
          return false;
        }
        this->behaviour.checkBounds();
        const auto smflag =
            AnsysTangentOperatorFlag<AnsysTraits<BV>::btype>::value;
        const auto r =
            this->behaviour.integrate(smflag, BV::CONSISTENTTANGENTOPERATOR);
        if (r == BV::FAILURE) {
          return false;
        }
        this->behaviour.checkBounds();
        this->behaviour.ANSYSexportStateData(d.STRESS, d.STATEV);
        ConsistentTangentOperatorHandler::exe(this->behaviour, d.DDSDDE);
        return true;
      }  // end of Integrator::exe

     private:
      struct ConsistentTangentOperatorIsNotAvalaible {
        typedef Behaviour<H, AnsysReal, false> BV;
        static void exe(BV&, AnsysReal* const) {
          typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
          throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
        }  // end of exe
      };

      typedef Behaviour<H, AnsysReal, false> BV;
      BV behaviour;
      AnsysReal dt;
    };  // end of struct Integrator

    TFEL_ANSYS_INLINE2 static void checkNPROPS(const AnsysInt NPROPS) {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H, AnsysReal, false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const auto offset = (AnsysTraits<BV>::elasticPropertiesOffset +
                           AnsysTraits<BV>::thermalExpansionPropertiesOffset +
                           AnsysTraits<BV>::orthotropicAxesOffset);
      const auto nprops = AnsysTraits<BV>::material_properties_nb;
      const auto NPROPS_ = offset + nprops;
      const auto is_defined_ = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if ((NPROPS != NPROPS_) && is_defined_) {
        throwUnMatchedNumberOfMaterialProperties(Traits::getName(), NPROPS_,
                                                 NPROPS);
      }
    }  // end of checkNPROPS

    TFEL_ANSYS_INLINE2 static void checkNSTATV(const AnsysInt NSTATV) {
      typedef Behaviour<H, AnsysReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const auto nstatv = Traits::internal_variables_nb;
      const auto is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((nstatv != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), nstatv, NSTATV);
      }
    }  // end of checkNSTATV

  };  // end of struct AnsysBehaviourHandler

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYS_ANSYSBEHAVIOURHANDLER_HXX */
