/*!
 * \file  mfront/include/MFront/Cyrano/CyranoBehaviourHandler.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_HXX
#define LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_CYRANO_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CYRANO_CALL_HXX */

#include <algorithm>
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "MFront/Cyrano/CyranoComputeStiffnessTensor.hxx"

namespace cyrano {

  /*!
   * structure in charge of calling the behaviour integrate method.
   * This structure handles two cases wether or not we shall handle
   * local substepping.
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoBehaviourHandler
      : public CyranoInterfaceExceptions {
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL
        DrivingVariableInitialiserWithStressFreeExpansion
        : public CyranoInterfaceExceptions {
      //! a simple alias
      typedef Behaviour<H, CyranoReal, false> BV;
      //! a simple alias for the behaviour data
      typedef typename BV::BehaviourData BData;
      //! a simple alias for the integration data
      typedef typename BV::IntegrationData IData;
      //! a simple alias
      typedef tfel::material::ModellingHypothesisToSpaceDimension<H>
          ModellingHypothesisToSpaceDimension;
      // spatial dimension
      static constexpr unsigned short N =
          ModellingHypothesisToSpaceDimension::value;
      /*!
       * \param[out] bData  : behaviour data
       * \param[out] iData  : integration data
       * \param[in]  b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static void exe(BData &bData,
                                         IData &iData,
                                         const CyranoReal *const STRAN,
                                         const CyranoReal *const DSTRAN) {
        using std::pair;
        using tfel::fsalgo::copy;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        CyranoReal eto[3];
        CyranoReal deto[3];
        // creating a fake behaviour to compute the stress-free expansion
        // this is not really elegant by can't do better
        pair<StressFreeExpansionType, StressFreeExpansionType> s;
        BV b(bData, iData);
        b.computeStressFreeExpansion(s);
        const auto &s0 = s.first;
        const auto &s1 = s.second;
        // convert to MFront conventions
        eto[0] = STRAN[0] - s0[0];
        eto[1] = STRAN[2] - s0[1];
        eto[2] = STRAN[1] - s0[2];
        deto[0] = DSTRAN[0] - (s1[0] - s0[0]);
        deto[1] = DSTRAN[2] - (s1[1] - s0[1]);
        deto[2] = DSTRAN[1] - (s1[2] - s0[2]);
        bData.setCYRANOBehaviourDataDrivingVariables(eto);
        iData.setCYRANOIntegrationDataDrivingVariables(deto);
      }  // end of exe
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static void exe(BV &b,
                                         const CyranoReal *const STRAN,
                                         const CyranoReal *const DSTRAN) {
        using std::pair;
        using tfel::fsalgo::copy;
        typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
        CyranoReal eto[3];
        CyranoReal deto[3];
        pair<StressFreeExpansionType, StressFreeExpansionType> s;
        b.computeStressFreeExpansion(s);
        const auto &s0 = s.first;
        const auto &s1 = s.second;
        // convert to MFront conventions
        eto[0] = STRAN[0] - s0[0];
        eto[1] = STRAN[2] - s0[1];
        eto[2] = STRAN[1] - s0[2];
        deto[0] = DSTRAN[0] - (s1[0] - s0[0]);
        deto[1] = DSTRAN[2] - (s1[1] - s0[1]);
        deto[2] = DSTRAN[1] - (s1[2] - s0[2]);
        b.setCYRANOBehaviourDataDrivingVariables(eto);
        b.setCYRANOIntegrationDataDrivingVariables(deto);
      }  // end of exe

    };  // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL
        DrivingVariableInitialiserWithoutStressFreeExpansion {
      //! a simple alias
      typedef Behaviour<H, CyranoReal, false> BV;
      //! a simple alias for the behaviour data
      typedef typename BV::BehaviourData BData;
      //! a simple alias for the integration data
      typedef typename BV::IntegrationData IData;
      /*!
       * \param[out] bData  : behaviour data
       * \param[out] iData  : integration data
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static void exe(BData &bData,
                                         IData &iData,
                                         const CyranoReal *const STRAN,
                                         const CyranoReal *const DSTRAN) {
        CyranoReal eto[3];
        CyranoReal deto[3];
        eto[0] = STRAN[0];
        eto[1] = STRAN[2];
        eto[2] = STRAN[1];
        deto[0] = DSTRAN[0];
        deto[1] = DSTRAN[2];
        deto[2] = DSTRAN[1];
        bData.setCYRANOBehaviourDataDrivingVariables(eto);
        iData.setCYRANOIntegrationDataDrivingVariables(deto);
      }  // end of exe
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static void exe(BV &b,
                                         const CyranoReal *const STRAN,
                                         const CyranoReal *const DSTRAN) {
        CyranoReal eto[3];
        CyranoReal deto[3];
        eto[0] = STRAN[0];
        eto[1] = STRAN[2];
        eto[2] = STRAN[1];
        deto[0] = DSTRAN[0];
        deto[1] = DSTRAN[2];
        deto[2] = DSTRAN[1];
        b.setCYRANOBehaviourDataDrivingVariables(eto);
        b.setCYRANOIntegrationDataDrivingVariables(deto);
      }  // end of exe
    };   // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    /*!
     * An helper structure which is used to compute the stiffness
     * tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer {
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_CYRANO_INLINE static void exe(BData &data,
                                         const CyranoReal *const props) {
        typedef CyranoTraits<BV> Traits;
        CyranoComputeStiffnessTensor<Traits::stype>::exe(
            data.getStiffnessTensor(), props);
      }  // end of exe
    };   // end of struct StiffnessOperatorInitializer

    /*!
     * An helper structure which is used to compute the thermal
     * expansion tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer {
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_CYRANO_INLINE static void exe(BData &data,
                                         const CyranoReal *const props) {
        typedef CyranoTraits<BV> Traits;
        const unsigned short o = CyranoTraits<BV>::elasticPropertiesOffset;
        CyranoComputeThermalExpansionCoefficientTensor<H, Traits::stype>::exe(
            props + o, data.getThermalExpansionCoefficientTensor());
      }  // end of exe
    };   // end of struct ThermalExpansionCoefficientTensorInitializer

    /*!
     * an helper class which don't do any initialisation
     */
    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer {
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef typename BV::BehaviourData BData;
      TFEL_CYRANO_INLINE static void exe(BData &, const CyranoReal *const) {}
    };  // end of struct DoNothingInitializer

    /*!
     * A helper structure used to handle the case where the behaviour
     * is not defined.
     *
     * For example, if the behaviour is only defined in 1D, this
     * structure will be used if the user tries to use it in 2D or 3D.
     */
    struct TFEL_VISIBILITY_LOCAL Error {
      TFEL_CYRANO_INLINE Error(const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const CyranoReal *const,
                               const tfel::material::OutOfBoundsPolicy) {
      }  // end of Error

      TFEL_NORETURN void exe(CyranoReal *const,
                             CyranoReal *const,
                             CyranoReal *const) {
        typedef tfel::material::MechanicalBehaviourTraits<
            Behaviour<H, CyranoReal, false>>
            Traits;
        throw(CyranoInvalidDimension(Traits::getName(), 1u));
      }  // end of Error::exe

    };  // end of struct Error

    template <const bool bs,  // requires StiffnessOperator
              const bool ba>  // requires ThermalExpansionCoefficientTensor
    struct TFEL_VISIBILITY_LOCAL IntegratorWithTimeStepping {
      //! A simple alias
      typedef
          typename std::conditional<bs,
                                    StiffnessOperatorInitializer,
                                    DoNothingInitializer>::type SInitializer;
      //! A simple alias
      typedef typename std::conditional<
          ba,
          ThermalExpansionCoefficientTensorInitializer,
          DoNothingInitializer>::type AInitializer;

      TFEL_CYRANO_INLINE
      IntegratorWithTimeStepping(const CyranoReal *const DTIME,
                                 const CyranoReal *const STRAN,
                                 const CyranoReal *const DSTRAN,
                                 const CyranoReal *const TEMP,
                                 const CyranoReal *const DTEMP,
                                 const CyranoReal *const PROPS,
                                 const CyranoReal *const PREDEF,
                                 const CyranoReal *const DPRED,
                                 CyranoReal *const STATEV,
                                 CyranoReal *const STRESS,
                                 const tfel::material::OutOfBoundsPolicy op)
          : bData(TEMP,
                  PROPS + CyranoTraits<BV>::propertiesOffset,
                  STATEV,
                  PREDEF),
            iData(DTIME, DTEMP, DPRED),
            dt(*DTIME),
            policy(op) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasStressFreeExpansion,
            DrivingVariableInitialiserWithStressFreeExpansion,
            DrivingVariableInitialiserWithoutStressFreeExpansion>::type
            DVInitializer;
        SInitializer::exe(this->bData, PROPS);
        AInitializer::exe(this->bData, PROPS);
        DVInitializer::exe(this->bData, this->iData, STRAN, DSTRAN);
        // initial stress
        CyranoReal sig[3];
        // turning to MFront conventions
        sig[0] = STRESS[0];
        sig[1] = STRESS[2];
        sig[2] = STRESS[1];
        this->bData.setCYRANOBehaviourDataThermodynamicForces(sig);
      }  // end of IntegratorWithTimeStepping

      TFEL_CYRANO_INLINE2 void exe(CyranoReal *const DDSOE,
                                   CyranoReal *const STRESS,
                                   CyranoReal *const STATEV) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasConsistentTangentOperator,
            typename std::conditional<
                Traits::isConsistentTangentOperatorSymmetric,
                SymmetricConsistentTangentOperatorComputer,
                GeneralConsistentTangentOperatorComputer>::type,
            ConsistentTangentOperatorIsNotAvalaible>::type
            ConsistentTangentOperatorHandler;
        typedef typename std::conditional<
            Traits::hasPredictionOperator, StandardPredictionOperatorComputer,
            PredictionOperatorIsNotAvalaible>::type PredictionOperatorComputer;
        CyranoReal sig[3];
        unsigned int subSteps = 0u;
        unsigned int iterations = 1u;
        if (this->dt < 0.) {
          throwNegativeTimeStepException(Traits::getName());
        }
        while ((iterations != 0) &&
               (subSteps != CyranoTraits<BV>::maximumSubStepping)) {
          auto r = BV::SUCCESS;
          BV behaviour(this->bData, this->iData);
          try {
            behaviour.initialize();
            behaviour.setOutOfBoundsPolicy(this->policy);
            behaviour.checkBounds();
            const auto smflag = TangentOperatorTraits<
                BV::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
            if ((-3.25 < *DDSOE) && (*DDSOE < -2.75)) {
              r = PredictionOperatorComputer::exe(behaviour, smflag,
                                                  BV::TANGENTOPERATOR);
            } else if ((-2.25 < *DDSOE) && (*DDSOE < -1.75)) {
              r = PredictionOperatorComputer::exe(behaviour, smflag,
                                                  BV::SECANTOPERATOR);
            } else if ((-1.25 < *DDSOE) && (*DDSOE < -0.75)) {
              r = PredictionOperatorComputer::exe(behaviour, smflag,
                                                  BV::ELASTIC);
            } else if ((-0.25 < *DDSOE) && (*DDSOE < 0.25)) {
              r = behaviour.integrate(smflag, BV::NOSTIFFNESSREQUESTED);
            } else if ((0.75 < *DDSOE) && (*DDSOE < 1.25)) {
              r = behaviour.integrate(smflag, BV::ELASTIC);
            } else if ((1.75 < *DDSOE) && (*DDSOE < 2.25)) {
              r = behaviour.integrate(smflag, BV::SECANTOPERATOR);
            } else if ((2.75 < *DDSOE) && (*DDSOE < 3.25)) {
              r = behaviour.integrate(smflag, BV::TANGENTOPERATOR);
            } else if ((3.75 < *DDSOE) && (*DDSOE < 4.25)) {
              r = behaviour.integrate(smflag, BV::CONSISTENTTANGENTOPERATOR);
            } else {
              throwInvalidDDSOEException(Traits::getName(), *DDSOE);
            }
            if (r == BV::FAILURE) {
              if (*DDSOE < -0.5) {
                throwPredictionComputationFailedException(Traits::getName());
              }
            }
          } catch (const tfel::material::DivergenceException &e) {
#ifdef MFRONT_CYRANO_VERBOSE
            std::cerr << "no convergence : " << e.what() << std::endl;
#endif /* LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_HXX */
            r = BV::FAILURE;
          }
          if ((r == BV::SUCCESS) ||
              ((r == BV::UNRELIABLE_RESULTS) &&
               (!CyranoTraits<BV>::doSubSteppingOnInvalidResults))) {
            --(iterations);
            behaviour.checkBounds();
            this->iData.updateDrivingVariables(this->bData);
            behaviour.updateExternalStateVariables();
            this->bData = static_cast<const BData &>(behaviour);
            if (iterations == 0) {
              if ((*DDSOE > 0.5) || (*DDSOE < -0.5)) {
                ConsistentTangentOperatorHandler::exe(behaviour, DDSOE);
              }
            }
          } else if ((r == BV::UNRELIABLE_RESULTS) &&
                     (CyranoTraits<BV>::doSubSteppingOnInvalidResults)) {
            iterations = static_cast<unsigned int>(iterations * 2u);
            this->iData.scale(this->bData, 0.5);
          } else {
            ++subSteps;
            iterations = static_cast<unsigned int>(iterations * 2u);
            this->iData.scale(this->bData, 0.5);
          }
        }
        if ((subSteps == CyranoTraits<BV>::maximumSubStepping) &&
            (iterations != 0)) {
          throwMaximumNumberOfSubSteppingReachedException(Traits::getName());
        }
        this->bData.CYRANOexportStateData(sig, STATEV);
        // back to cyrano conventions
        STRESS[0] = sig[0];
        STRESS[1] = sig[2];
        STRESS[2] = sig[1];
      }  // end of IntegratorWithTimeStepping::exe

     private:
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef typename BV::BehaviourData BData;
      typedef typename BV::IntegrationData IData;

      BData bData;
      IData iData;
      CyranoReal dt;
      tfel::material::ModellingHypothesis::Hypothesis hypothesis;
      tfel::material::OutOfBoundsPolicy policy;

    };  // end of struct IntegratorWithTimeStepping

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

      TFEL_CYRANO_INLINE Integrator(const CyranoReal *const DTIME,
                                    const CyranoReal *const STRAN,
                                    const CyranoReal *const DSTRAN,
                                    const CyranoReal *const TEMP,
                                    const CyranoReal *const DTEMP,
                                    const CyranoReal *const PROPS,
                                    const CyranoReal *const PREDEF,
                                    const CyranoReal *const DPRED,
                                    const CyranoReal *const STATEV,
                                    const CyranoReal *const STRESS,
                                    const tfel::material::OutOfBoundsPolicy op)
          : behaviour(DTIME,
                      TEMP,
                      DTEMP,
                      PROPS + CyranoTraits<BV>::propertiesOffset,
                      STATEV,
                      PREDEF,
                      DPRED),
            dt(*DTIME) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasStressFreeExpansion,
            DrivingVariableInitialiserWithStressFreeExpansion,
            DrivingVariableInitialiserWithoutStressFreeExpansion>::type
            DVInitializer;
        this->behaviour.setOutOfBoundsPolicy(op);
        // elastic tensor
        SInitializer::exe(this->behaviour, PROPS);
        // thermal expansion coefficient
        AInitializer::exe(this->behaviour, PROPS);
        // initialising driving variables
        DVInitializer::exe(this->behaviour, STRAN, DSTRAN);
        // initalising stresses
        CyranoReal sig[3];
        // convert to MFront conventions
        sig[0] = STRESS[0];
        sig[1] = STRESS[2];
        sig[2] = STRESS[1];
        this->behaviour.setCYRANOBehaviourDataThermodynamicForces(sig);
        // initialise the behaviour
        this->behaviour.initialize();
      }  // end of Integrator::Integrator

      TFEL_CYRANO_INLINE2
      void exe(CyranoReal *const DDSOE,
               CyranoReal *const STRESS,
               CyranoReal *const STATEV) {
        using namespace tfel::material;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef typename std::conditional<
            Traits::hasConsistentTangentOperator,
            typename std::conditional<
                Traits::isConsistentTangentOperatorSymmetric,
                SymmetricConsistentTangentOperatorComputer,
                GeneralConsistentTangentOperatorComputer>::type,
            ConsistentTangentOperatorIsNotAvalaible>::type
            ConsistentTangentOperatorHandler;
        typedef typename std::conditional<
            Traits::hasPredictionOperator, StandardPredictionOperatorComputer,
            PredictionOperatorIsNotAvalaible>::type PredictionOperatorComputer;
        CyranoReal sig[3];
        if (this->dt < 0.) {
          throwNegativeTimeStepException(Traits::getName());
        }
        behaviour.checkBounds();
        auto r = BV::SUCCESS;
        const auto smflag = TangentOperatorTraits<
            BV::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
        if ((-3.25 < *DDSOE) && (*DDSOE < -2.75)) {
          r = PredictionOperatorComputer::exe(this->behaviour, smflag,
                                              BV::TANGENTOPERATOR);
        } else if ((-2.25 < *DDSOE) && (*DDSOE < -1.75)) {
          r = PredictionOperatorComputer::exe(this->behaviour, smflag,
                                              BV::SECANTOPERATOR);
        } else if ((-1.25 < *DDSOE) && (*DDSOE < -0.75)) {
          r = PredictionOperatorComputer::exe(this->behaviour, smflag,
                                              BV::ELASTIC);
        } else if ((-0.25 < *DDSOE) && (*DDSOE < 0.25)) {
          r = this->behaviour.integrate(smflag, BV::NOSTIFFNESSREQUESTED);
        } else if ((0.75 < *DDSOE) && (*DDSOE < 1.25)) {
          r = this->behaviour.integrate(smflag, BV::ELASTIC);
        } else if ((1.75 < *DDSOE) && (*DDSOE < 2.25)) {
          r = this->behaviour.integrate(smflag, BV::SECANTOPERATOR);
        } else if ((2.75 < *DDSOE) && (*DDSOE < 3.25)) {
          r = this->behaviour.integrate(smflag, BV::TANGENTOPERATOR);
        } else if ((3.75 < *DDSOE) && (*DDSOE < 4.25)) {
          r = this->behaviour.integrate(smflag, BV::CONSISTENTTANGENTOPERATOR);
        } else {
          throwInvalidDDSOEException(Traits::getName(), *DDSOE);
        }
        if (r == BV::FAILURE) {
          // Il manque un vraie gestion locale de résultats imprécis
          if (*DDSOE < -0.5) {
            throwPredictionComputationFailedException(Traits::getName());
          } else {
            throwBehaviourIntegrationFailedException(Traits::getName());
          }
        }
        // if(this->behaviour.integrate(smflag,BV::NOSTIFFNESSREQUESTED)==BV::FAILURE){
        //   throwBehaviourIntegrationFailedException(Traits::getName());
        // }
        behaviour.checkBounds();
        this->behaviour.CYRANOexportStateData(sig, STATEV);
        STRESS[0] = sig[0];
        STRESS[1] = sig[2];
        STRESS[2] = sig[1];
        if ((*DDSOE > 0.5) || (*DDSOE < -0.5)) {
          ConsistentTangentOperatorHandler::exe(this->behaviour, DDSOE);
        }
      }  // end of Integrator::exe

     private:
      typedef Behaviour<H, CyranoReal, false> BV;
      BV behaviour;
      CyranoReal dt;
    };  // end of struct Integrator

    struct StandardPredictionOperatorComputer {
      typedef Behaviour<H, CyranoReal, false> BV;
      static typename BV::IntegrationResult exe(BV &b,
                                                const typename BV::SMFlag smf,
                                                const typename BV::SMType smt) {
        return b.computePredictionOperator(smf, smt);
      }  // end of exe
    };

    struct PredictionOperatorIsNotAvalaible {
      typedef Behaviour<H, CyranoReal, false> BV;
      static typename BV::IntegrationResult exe(BV &,
                                                const typename BV::SMFlag,
                                                const typename BV::SMType) {
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        throwPredictionOperatorIsNotAvalaible(Traits::getName());
        return BV::FAILURE;
      }  // end of exe
    };

    struct ConsistentTangentOperatorIsNotAvalaible {
      typedef Behaviour<H, CyranoReal, false> BV;
      static void exe(BV &, CyranoReal *const) {
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
      }  // end of exe
    };

    struct SymmetricConsistentTangentOperatorComputer {
      typedef Behaviour<H, CyranoReal, false> BV;
      static void exe(const BV &bv, CyranoReal *const DDSOE) {
        using namespace tfel::math;
        ST2toST2View<1u, CyranoReal> Kt{DDSOE};
        const auto &Dt = bv.getTangentOperator();
        // conversion vers les conventions cyrano
        Kt(0, 0) = Dt(0, 0);
        Kt(1, 0) = Dt(2, 0);
        Kt(2, 0) = Dt(1, 0);
        Kt(0, 1) = Dt(0, 2);
        Kt(1, 1) = Dt(2, 2);
        Kt(2, 1) = Dt(1, 2);
        Kt(0, 2) = Dt(0, 1);
        Kt(1, 2) = Dt(2, 1);
        Kt(2, 2) = Dt(1, 1);
      }  // end of exe
    };

    struct GeneralConsistentTangentOperatorComputer {
      typedef Behaviour<H, CyranoReal, false> BV;
      static void exe(const BV &bv, CyranoReal *const DDSOE) {
        using namespace tfel::math;
        ST2toST2View<1u, CyranoReal> Kt{DDSOE};
        const auto &Dt = bv.getTangentOperator();
        // conversion vers les conventions cyrano
        Kt(0, 0) = Dt(0, 0);
        Kt(1, 0) = Dt(2, 0);
        Kt(2, 0) = Dt(1, 0);
        Kt(0, 1) = Dt(0, 2);
        Kt(1, 1) = Dt(2, 2);
        Kt(2, 1) = Dt(1, 2);
        Kt(0, 2) = Dt(0, 1);
        Kt(1, 2) = Dt(2, 1);
        Kt(2, 2) = Dt(1, 1);
        // les conventions fortran....
        std::swap(Kt(0, 1), Kt(1, 0));
        std::swap(Kt(0, 2), Kt(2, 0));
        std::swap(Kt(1, 2), Kt(2, 1));
      }  // end of exe
    };

    TFEL_CYRANO_INLINE2 static void checkNPROPS(const CyranoInt NPROPS) {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset = CyranoTraits<BV>::propertiesOffset;
      const unsigned short nprops = CyranoTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ =
          offset + nprops == 0 ? 1u : offset + nprops;
      const bool is_defined_ = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if ((NPROPS != NPROPS_) && is_defined_) {
        throwUnMatchedNumberOfMaterialProperties(Traits::getName(), NPROPS_,
                                                 NPROPS);
      }
    }  // end of checkNPROPS

    TFEL_CYRANO_INLINE2 static void checkNSTATV(const CyranoInt NSTATV) {
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((NSTATV_ != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), NSTATV_,
                                             NSTATV);
      }
    }  // end of checkNSTATV

  };  // end of struct CyranoBehaviourHandler

}  // end of namespace cyrano

#include "MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx"
#include "MFront/Cyrano/CyranoOrthotropicBehaviourHandler.hxx"

#endif /* LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_HXX */
