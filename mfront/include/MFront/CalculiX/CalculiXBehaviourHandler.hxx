/*!
 * \file  mfront/include/MFront/CalculiX/CalculiXBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_CALCULIX_CALCULIXBEHAVIOURHANDLER_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXBEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_CALCULIX_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CALCULIX_CALL_HXX */

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/TMatrixView.hxx"
#include "TFEL/Math/T2toST2/T2toST2View.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

#include "MFront/CalculiX/CalculiXData.hxx"
#include "MFront/CalculiX/CalculiXTangentOperator.hxx"
#include "MFront/CalculiX/CalculiXInterfaceExceptions.hxx"
#include "MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx"
#include "MFront/CalculiX/CalculiXComputeThermalExpansionCoefficientTensor.hxx"

namespace calculix {

  /*!
   * \brief a metafunction returning a description of the consistent
   * tangent operator
   * \tparam btype: type of the behaviour considered
   */
  template <CalculiXBehaviourType btype>
  struct CalculiXTangentOperatorFlag;
  /*!
   * \brief partial specialisation of `CalculiXTangentOperatorFlag`
   * metafunction for strain based behaviours.
   */
  template <>
  struct CalculiXTangentOperatorFlag<calculix::STANDARDSTRAINBASEDBEHAVIOUR> {
    //! a simple alias
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    //! a simple alias
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>
        TangentOperatorTraits;
    //! \return value of the metafunction
    static constexpr auto value =
        TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };
  /*!
   * \brief partial specialisation of `CalculiXTangentOperatorFlag`
   * metafunction for strain based behaviours.
   */
  template <>
  struct CalculiXTangentOperatorFlag<calculix::STANDARDFINITESTRAINBEHAVIOUR> {
    //! a simple alias
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;
    //! a simple alias
    typedef tfel::material::TangentOperatorTraits<
        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR>
        TangentOperatorTraits;
    //! \return value of the metafunction
    static constexpr auto value = TangentOperatorTraits::DS_DEGL;
  };
  /*!
   * \brief structure used to write a behaviour
   * \tparam H: hypothesis
   * \tparam Behaviour: behaviour
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CalculiXBehaviourHandler
      : public CalculiXInterfaceExceptions {
    /*!
     * \brief An helper structure used to initialise the driving
     * variables.
     */
    struct TFEL_VISIBILITY_LOCAL
        DrivingVariableInitialiserWithStressFreeExpansion
        : public CalculiXInterfaceExceptions {
      //! a simple alias
      typedef Behaviour<H, CalculiXReal, false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  DV0  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DV1 : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_CALCULIX_INLINE static void exe(
          BV& b,
          const CalculiXReal* const DV1,
          const CalculiXReal* const DV0,
          const StressFreeExpansionHandler& sfeh) {
        using std::pair;
        using tfel::fsalgo::copy;
        using namespace tfel::material;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        const CalculiXInt N = ModellingHypothesisToSpaceDimension<H>::value;
        CalculiXReal dv0[CalculiXTraits<BV>::DrivingVariableSize];
        CalculiXReal dv1[CalculiXTraits<BV>::DrivingVariableSize];
        copy<CalculiXTraits<BV>::DrivingVariableSize>::exe(DV0, dv0);
        copy<CalculiXTraits<BV>::DrivingVariableSize>::exe(DV1, dv1);
        // check that the function pointer are not null
        if (sfeh == nullptr) {
          throwUnsupportedStressFreeExpansionException(Traits::getName());
        }
        pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        const auto& s0 = s.first;
        const auto& s1 = s.second;
        sfeh(dv0, dv1, &s0[0], &s1[0], N);
        b.setCALCULIXBehaviourDataDrivingVariables(dv0);
        b.setCALCULIXIntegrationDataDrivingVariables(dv1);
      }  // end of exe

    };  // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL
        DrivingVariableInitialiserWithoutStressFreeExpansion {
      //! a simple alias
      typedef Behaviour<H, CalculiXReal, false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  DV0  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DV1 : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_CALCULIX_INLINE static void exe(BV& b,
                                           const CalculiXReal* const DV0,
                                           const CalculiXReal* const DV1,
                                           const StressFreeExpansionHandler&) {
        b.setCALCULIXBehaviourDataDrivingVariables(DV0);
        b.setCALCULIXIntegrationDataDrivingVariables(DV1);
      }  // end of exe
    };   // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef Behaviour<H, CalculiXReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_CALCULIX_INLINE static void exe(BData& data,
                                           const CalculiXReal* const props) {
        CalculiXComputeStiffnessTensor<CalculiXTraits<BV>::etype>::exe(
            data.getStiffnessTensor(), props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef Behaviour<H, CalculiXReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_CALCULIX_INLINE static void exe(BData& data,
                                           const CalculiXReal* const props) {
        const auto o = CalculiXTraits<BV>::elasticPropertiesOffset;
        CalculiXComputeThermalExpansionCoefficientTensor<CalculiXTraits<
            BV>::stype>::exe(props + o,
                             data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef Behaviour<H, CalculiXReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_CALCULIX_INLINE static void exe(BData&, const CalculiXReal* const) {}
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

      TFEL_CALCULIX_INLINE Integrator(const CalculiXData& d)
          : behaviour(&(d.DTIME),
                      d.TEMP,
                      d.MPROPS + CalculiXTraits<BV>::elasticPropertiesOffset +
                          CalculiXTraits<BV>::thermalExpansionPropertiesOffset,
                      d.STATEV0),
            dt(d.DTIME) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasStressFreeExpansion,
            DrivingVariableInitialiserWithStressFreeExpansion,
            DrivingVariableInitialiserWithoutStressFreeExpansion>::type
            DVInitializer;
        SInitializer::exe(this->behaviour, d.MPROPS);
        AInitializer::exe(this->behaviour, d.MPROPS);
        DVInitializer::exe(this->behaviour, d.DV0, d.DV1, d.sfeh);
        this->behaviour.setCALCULIXBehaviourDataThermodynamicForces(d.STRESS);
        this->behaviour.setOutOfBoundsPolicy(d.op);
        this->behaviour.initialize();
      }  // end of Integrator::Integrator

      TFEL_CALCULIX_INLINE2
      void exe(const CalculiXData& d) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasConsistentTangentOperator, ExtractTangentOperator,
            ConsistentTangentOperatorIsNotAvalaible>::type
            ConsistentTangentOperatorHandler;
        if (this->dt < 0.) {
          throwNegativeTimeStepException(Traits::getName());
        }
        this->behaviour.checkBounds();
        auto r = BV::SUCCESS;
        *(d.PNEWDT) = CalculiXReal(1);
        const auto smflag =
            CalculiXTangentOperatorFlag<CalculiXTraits<BV>::btype>::value;
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
        this->behaviour.CALCULIXexportStateData(d.STRESS, d.STATEV1);
        ConsistentTangentOperatorHandler::exe(this->behaviour, d.DDSDDE);
      }  // end of Integrator::exe

     private:
      /*!
       * \brief An helper structure used when the consistent tangent
       * operator can't be computed.
       */
      struct ConsistentTangentOperatorIsNotAvalaible {
        //! a simple alias
        typedef Behaviour<H, CalculiXReal, false> BV;
        /*!
         * \brief a function whose only purpose is to throw
         * an exception
         */
        TFEL_NORETURN static void exe(BV&, CalculiXReal* const) {
          typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
          throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
        }  // end of exe
      };
      //! a simple alias
      typedef Behaviour<H, CalculiXReal, false> BV;
      //! the behaviour wrapped
      BV behaviour;
      //! the time increment
      CalculiXReal dt;
    };  // end of struct Integrator
    /*!
     * \brief check that the number of material properties given by
     * `CalculiX` is greater than the expected number of material
     * properties
     * \param[in] NPROPS: number of material properties given by
     * `CalculiX`.
     */
    TFEL_CALCULIX_INLINE2 static void checkNPROPS(const CalculiXInt NPROPS) {
      using namespace tfel::material;
      typedef Behaviour<H, CalculiXReal, false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const auto offset =
          (CalculiXTraits<BV>::elasticPropertiesOffset +
           CalculiXTraits<BV>::thermalExpansionPropertiesOffset);
      const auto nprops = CalculiXTraits<BV>::material_properties_nb;
      const auto NPROPS_ = offset + nprops == 0 ? 1 : offset + nprops;
      const auto is_defined_ = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if ((NPROPS != NPROPS_) && is_defined_) {
        throwUnMatchedNumberOfMaterialProperties(Traits::getName(), NPROPS_,
                                                 NPROPS);
      }
    }  // end of checkNPROPS
    /*!
     * \brief check that the number of state variables given by
     * `CalculiX` is greater than the expected number of state
     * variables.
     *
     * \param[in] NSTATV: number of state variables given by
     * `CalculiX`.
     */
    TFEL_CALCULIX_INLINE2 static void checkNSTATV(const CalculiXInt NSTATV) {
      typedef Behaviour<H, CalculiXReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const auto nstatv = Traits::internal_variables_nb;
      const auto is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((nstatv != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), nstatv, NSTATV);
      }
    }  // end of checkNSTATV

  };  // end of struct CalculiXBehaviourHandler

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXBEHAVIOURHANDLER_HXX */
