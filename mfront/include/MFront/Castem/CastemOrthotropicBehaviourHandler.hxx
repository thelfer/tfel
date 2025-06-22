/*!
 * \file  mfront/include/MFront/Castem/CastemOrthotropicBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_CASTEM_CASTEMORTHOTROPICBEHAVIOURHANDLER_H_
#define LIB_MFRONT_CASTEM_CASTEMORTHOTROPICBEHAVIOURHANDLER_H_

#ifndef LIB_MFRONT_CASTEM_CASTEMBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CASTEM_CASTEMBEHAVIOURHANDLER_H_ */

#include "MFront/Castem/CastemRotationMatrix.hxx"

namespace castem {

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemOrthotropicSmallStrainBehaviourHandler1D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemOrthotropicSmallStrainBehaviourHandler2D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemOrthotropicSmallStrainBehaviourHandler3D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemOrthotropicFiniteStrainBehaviourHandler1D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemOrthotropicFiniteStrainBehaviourHandler2D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemOrthotropicFiniteStrainBehaviourHandler3D;

  /*!
   * An helper structure to make an appropriate dispatch based on the
   * spatial dimension
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicSmallStrainBehaviourDispatcher {
    //! a simple alias
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H>
        ModellingHypothesisToSpaceDimension;
    // spatial dimension
    static constexpr unsigned short N =
        ModellingHypothesisToSpaceDimension::value;
    // the dispatch
    typedef typename std::conditional<
        N == 1,
        CastemOrthotropicSmallStrainBehaviourHandler1D<H, Behaviour>,
        typename std::conditional<
            N == 2,
            CastemOrthotropicSmallStrainBehaviourHandler2D<H, Behaviour>,
            CastemOrthotropicSmallStrainBehaviourHandler3D<H, Behaviour>>::
            type>::type type;
  };  // end of struct CastemOrthotropicSmallStrainBehaviourDispatcher;

  /*!
   * An helper structure to make an appropriate dispatch based on the
   * spatial dimension
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
      CastemOrthotropicFiniteStrainBehaviourDispatcher {
    //! a simple alias
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H>
        ModellingHypothesisToSpaceDimension;
    // spatial dimension
    static constexpr unsigned short N =
        ModellingHypothesisToSpaceDimension::value;
    // the dispatch
    typedef typename std::conditional<
        N == 1,
        CastemOrthotropicFiniteStrainBehaviourHandler1D<H, Behaviour>,
        typename std::conditional<
            N == 2,
            CastemOrthotropicFiniteStrainBehaviourHandler2D<H, Behaviour>,
            CastemOrthotropicFiniteStrainBehaviourHandler3D<H, Behaviour>>::
            type>::type type;
  };  // end of struct CastemOrthotropicFiniteStrainBehaviourDispatcher;

  /*!
   * The handler for orthotropic behaviours
   * By default, this is unsupported (cohesive zone models)
   */
  template <
      CastemBehaviourType type,
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicBehaviourHandler
      : public CastemUnSupportedCaseHandler {
    using CastemUnSupportedCaseHandler::exe;
  };

  /*!
   * The handler for small strain orthotropic behaviours
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
      CastemOrthotropicBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
                                        H,
                                        Behaviour>
      : public CastemOrthotropicSmallStrainBehaviourDispatcher<H, Behaviour>::
            type {
    typedef
        typename CastemOrthotropicSmallStrainBehaviourDispatcher<H, Behaviour>::
            type Handler;
    using Handler::exe;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicSmallStrainBehaviourHandler1D
      : private CastemBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
                                       H,
                                       Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const,
        CastemReal *const DDSDDE,
        const CastemReal *const STRAN,
        const CastemReal *const DSTRAN,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR, H, Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[7]);
      ThermalExpansionCoefficientCheck::exe(PROPS[8]);
      ThermalExpansionCoefficientCheck::exe(PROPS[9]);
      Handler handler(DTIME, STRAN, DSTRAN, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, STRESS, op, sfeh);
      handler.exe(DDSDDE, STRESS, STATEV, PNEWDT);
    }  // end of CastemOrthotropicBehaviourHander<1u,Behaviour>::exe
  };   // end of struct CastemOrthotropicBehaviourHander<1u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicSmallStrainBehaviourHandler2D
      : private CastemBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
                                       H,
                                       Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const STRAN,
        const CastemReal *const DSTRAN,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR, H, Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemReal s[4];
      CastemReal e[4];
      CastemReal de[4];
      CastemRotationMatrix2D m(PROPS + 7, DROT);
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      ThermalExpansionCoefficientCheck::exe(PROPS[12]);
      m.rotateStressesForward(STRESS, s);
      m.rotateStrainsForward(STRAN, e);
      m.rotateStrainsForward(DSTRAN, de);
      const bool bDDSDDE = std::abs(*DDSDDE) > 0.5;
      Handler handler(DTIME, e, de, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      s, op, sfeh);
      handler.exe(DDSDDE, s, STATEV, PNEWDT);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSDDE) {
        m.rotateTangentOperatorBackward(DDSDDE);
      }
    }  // end of CastemOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of CastemOrthotropicBehaviourHander<2u,Behaviour>

  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicSmallStrainBehaviourHandler2D<
      tfel::material::ModellingHypothesis::PLANESTRESS,
      Behaviour>
      : private CastemBehaviourHandler<
            SMALLSTRAINSTANDARDBEHAVIOUR,
            tfel::material::ModellingHypothesis::PLANESTRESS,
            Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const STRAN,
        const CastemReal *const DSTRAN,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::PLANESTRESS;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR, H, Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemReal s[4];
      CastemReal e[4];
      CastemReal de[4];
      CastemRotationMatrix2D m(PROPS + 4, DROT);
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      m.rotateStressesForward(STRESS, s);
      m.rotateStrainsForward(STRAN, e);
      m.rotateStrainsForward(DSTRAN, de);
      const bool bDDSDDE = std::abs(*DDSDDE) > 0.5;
      Handler handler(DTIME, e, de, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      s, op, sfeh);
      handler.exe(DDSDDE, s, STATEV, PNEWDT);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSDDE) {
        m.rotateTangentOperatorBackward(DDSDDE);
      }
    }  // end of CastemOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of CastemOrthotropicBehaviourHander<2u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicSmallStrainBehaviourHandler3D
      : private CastemBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
                                       H,
                                       Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const STRAN,
        const CastemReal *const DSTRAN,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR, H, Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemReal s[6];
      CastemReal e[6];
      CastemReal de[6];
      // Passage au repère matériau
      CastemRotationMatrix3D m(PROPS + 9, DROT);
      m.rotateStressesForward(STRESS, s);
      m.rotateStrainsForward(STRAN, e);
      m.rotateStrainsForward(DSTRAN, de);
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[16]);
      ThermalExpansionCoefficientCheck::exe(PROPS[17]);
      ThermalExpansionCoefficientCheck::exe(PROPS[18]);
      const bool bDDSDDE = std::abs(*DDSDDE) > 0.5;
      Handler handler(DTIME, e, de, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      s, op, sfeh);
      handler.exe(DDSDDE, s, STATEV, PNEWDT);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSDDE) {
        m.rotateTangentOperatorBackward(DDSDDE);
      }
    }  // end of CastemOrthotropicBehaviourHandler<3u,Behaviour>::exe
  };

  /*!
   * The handler for finite strain orthotropic behaviours
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
      CastemOrthotropicBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,
                                        H,
                                        Behaviour>
      : public CastemOrthotropicFiniteStrainBehaviourDispatcher<H, Behaviour>::
            type {
    typedef typename CastemOrthotropicFiniteStrainBehaviourDispatcher<
        H,
        Behaviour>::type Handler;
    using Handler::exe;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicFiniteStrainBehaviourHandler1D
      : private CastemBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,
                                       H,
                                       Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const,
        CastemReal *const DDSDDE,
        const CastemReal *const F0,
        const CastemReal *const F1,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR, H,
                                     Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[7]);
      ThermalExpansionCoefficientCheck::exe(PROPS[8]);
      ThermalExpansionCoefficientCheck::exe(PROPS[9]);
      Handler handler(DTIME, F0, F1, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      STRESS, op, sfeh);
      handler.exe(DDSDDE, STRESS, STATEV, PNEWDT);
    }  // end of CastemOrthotropicBehaviourHander<1u,Behaviour>::exe
  };   // end of struct CastemOrthotropicBehaviourHander<1u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicFiniteStrainBehaviourHandler2D
      : private CastemBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,
                                       H,
                                       Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const F0,
        const CastemReal *const F1,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR, H,
                                     Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemReal s[4];
      CastemReal F0m[9];
      CastemReal F1m[9];
      CastemRotationMatrix2D m(PROPS + 7, DROT);
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      ThermalExpansionCoefficientCheck::exe(PROPS[12]);
      m.rotateStressesForward(STRESS, s);
      m.rotateDeformationGradientForward(F0, F0m);
      m.rotateDeformationGradientForward(F1, F1m);
      const bool bDDSDDE = std::abs(*DDSDDE) > 0.5;
      Handler handler(DTIME, F0m, F1m, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, s, op, sfeh);
      handler.exe(DDSDDE, s, STATEV, PNEWDT);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSDDE) {
        m.rotateTangentOperatorBackward(DDSDDE);
      }
    }  // end of CastemOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of CastemOrthotropicBehaviourHander<2u,Behaviour>

  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicFiniteStrainBehaviourHandler2D<
      tfel::material::ModellingHypothesis::PLANESTRESS,
      Behaviour>
      : private CastemBehaviourHandler<
            FINITESTRAINSTANDARDBEHAVIOUR,
            tfel::material::ModellingHypothesis::PLANESTRESS,
            Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const F0,
        const CastemReal *const F1,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::PLANESTRESS;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR, H,
                                     Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemReal s[4];
      CastemReal F0m[9];
      CastemReal F1m[9];
      CastemRotationMatrix2D m(PROPS + 4, DROT);
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      ThermalExpansionCoefficientCheck::exe(PROPS[12]);
      m.rotateStressesForward(STRESS, s);
      m.rotateDeformationGradientForward(F0, F0m);
      m.rotateDeformationGradientForward(F1, F1m);
      const bool bDDSDDE = std::abs(*DDSDDE) > 0.5;
      Handler handler(DTIME, F0m, F1m, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, s, op, sfeh);
      handler.exe(DDSDDE, s, STATEV, PNEWDT);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSDDE) {
        m.rotateTangentOperatorBackward(DDSDDE);
      }
    }  // end of CastemOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of CastemOrthotropicBehaviourHander<2u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemOrthotropicFiniteStrainBehaviourHandler3D
      : private CastemBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,
                                       H,
                                       Behaviour> {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const F0,
        const CastemReal *const F1,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR, H,
                                     Behaviour>
          CastemBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CastemBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CastemBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CastemBehaviourHandler::Error>::type Handler;
      typedef typename std::conditional<
          MTraits::handlesThermalExpansion,
          typename CastemBehaviourHandler::
              CheckThermalExpansionCoefficientIsNull,
          typename CastemBehaviourHandler::
              DontCheckThermalExpansionCoefficientIsNull>::type
          ThermalExpansionCoefficientCheck;
      CastemReal sm[6];
      CastemReal F0m[9];
      CastemReal F1m[9];
      // Passage au repère matériau
      CastemRotationMatrix3D m(PROPS + 9, DROT);
      m.rotateStressesForward(STRESS, sm);
      m.rotateDeformationGradientForward(F0, F0m);
      m.rotateDeformationGradientForward(F1, F1m);
      CastemBehaviourHandler::checkNPROPS(*NPROPS);
      CastemBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[16]);
      ThermalExpansionCoefficientCheck::exe(PROPS[17]);
      ThermalExpansionCoefficientCheck::exe(PROPS[18]);
      const bool bDDSDDE = std::abs(*DDSDDE) > 0.5;
      Handler handler(DTIME, F0m, F1m, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, sm, op, sfeh);
      handler.exe(DDSDDE, sm, STATEV, PNEWDT);
      m.rotateStressesBackward(sm, STRESS);
      if (bDDSDDE) {
        m.rotateTangentOperatorBackward(DDSDDE);
      }
    }  // end of CastemOrthotropicBehaviourHandler<3u,Behaviour>::exe

  };  // end of struct CastemOrthotropicBehaviourHandler<3u,Behaviour>

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMORTHOTROPICBEHAVIOURHANDLER_H_ */
