/*!
 * \file  mfront/include/MFront/Aster/AsterOrthotropicBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_ASTER_ASTERORTHOTROPICBEHAVIOURHANDLER_HXX
#define LIB_MFRONT_ASTER_ASTERORTHOTROPICBEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_HXX */

#include "MFront/Aster/AsterRotationMatrix.hxx"
#include "MFront/Aster/AsterFiniteStrainRotationMatrix.hxx"

namespace aster {

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct AsterOrthotropicSmallStrainBehaviourHandler2D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct AsterOrthotropicSmallStrainBehaviourHandler3D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct AsterOrthotropicFiniteStrainBehaviourHandler2D;

  //! forward declaration
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct AsterOrthotropicFiniteStrainBehaviourHandler3D;

  /*!
   * An helper structure to make an appropriate dispatch based on the
   * spatial dimension
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicSmallStrainBehaviourDispatcher {
    //! a simple alias
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H>
        ModellingHypothesisToSpaceDimension;
    // spatial dimension
    static constexpr unsigned short N =
        ModellingHypothesisToSpaceDimension::value;
    // the dispatch
    typedef typename std::conditional<
        N == 2,
        AsterOrthotropicSmallStrainBehaviourHandler2D<H, Behaviour>,
        AsterOrthotropicSmallStrainBehaviourHandler3D<H, Behaviour>>::type type;
  };  // end of struct AsterOrthotropicSmallStrainBehaviourDispatcher;

  /*!
   * An helper structure to make an appropriate dispatch based on the
   * spatial dimension
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicFiniteStrainBehaviourDispatcher {
    //! a simple alias
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H>
        ModellingHypothesisToSpaceDimension;
    // spatial dimension
    static constexpr unsigned short N =
        ModellingHypothesisToSpaceDimension::value;
    // the dispatch
    typedef typename std::conditional<
        N == 2,
        AsterOrthotropicFiniteStrainBehaviourHandler2D<H, Behaviour>,
        AsterOrthotropicFiniteStrainBehaviourHandler3D<H, Behaviour>>::type
        type;
  };  // end of struct AsterOrthotropicFiniteStrainBehaviourDispatcher;

  /*!
   * The handler for orthotropic behaviours
   * By default, this is unsupported (cohesive zone models)
   */
  template <
      AsterBehaviourType type,
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler
      : public AsterUnSupportedCaseHandler {
    using AsterUnSupportedCaseHandler::exe;
  };

  /*!
   * The handler for small strain orthotropic behaviours
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
      AsterOrthotropicBehaviourHandler<STANDARDSTRAINBASEDBEHAVIOUR,
                                       H,
                                       Behaviour>
      : public AsterOrthotropicSmallStrainBehaviourDispatcher<H,
                                                              Behaviour>::type {
    typedef
        typename AsterOrthotropicSmallStrainBehaviourDispatcher<H,
                                                                Behaviour>::type
            Handler;
    using Handler::exe;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicSmallStrainBehaviourHandler1D
      : private AsterBehaviourHandler<H, Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const,
        AsterReal *const DDSOE,
        const AsterReal *const STRAN,
        const AsterReal *const DSTRAN,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME, STRAN, DSTRAN, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, STRESS, op, sfeh);
      handler.exe(DDSOE, STRESS, STATEV, nullptr);
    }  // end of AsterOrthotropicBehaviourHander<1u,Behaviour>::exe
  };   // end of struct AsterOrthotropicBehaviourHander<1u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicSmallStrainBehaviourHandler2D
      : private AsterBehaviourHandler<H, Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const DROT,
        AsterReal *const DDSOE,
        const AsterReal *const STRAN,
        const AsterReal *const DSTRAN,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      AsterReal s[4];
      AsterReal e[4];
      AsterReal de[4];
      AsterRotationMatrix2D m(DROT);
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      m.rotateStressesForward(STRESS, s);
      m.rotateStrainsForward(STRAN, e);
      m.rotateStrainsForward(DSTRAN, de);
      const bool bDDSOE = std::abs(*DDSOE) > 0.5;
      Handler handler(DTIME, e, de, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      s, op, sfeh);
      handler.exe(DDSOE, s, STATEV, nullptr);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSOE) {
        m.rotateTangentOperatorBackward(DDSOE);
      }
    }  // end of AsterOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of AsterOrthotropicBehaviourHander<2u,Behaviour>

  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicSmallStrainBehaviourHandler2D<
      tfel::material::ModellingHypothesis::PLANESTRESS,
      Behaviour>
      : private AsterBehaviourHandler<
            tfel::material::ModellingHypothesis::PLANESTRESS,
            Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const DROT,
        AsterReal *const DDSOE,
        const AsterReal *const STRAN,
        const AsterReal *const DSTRAN,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::PLANESTRESS;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      AsterReal s[4];
      AsterReal e[4];
      AsterReal de[4];
      AsterRotationMatrix2D m(DROT);
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      m.rotateStressesForward(STRESS, s);
      m.rotateStrainsForward(STRAN, e);
      m.rotateStrainsForward(DSTRAN, de);
      const bool bDDSOE = std::abs(*DDSOE) > 0.5;
      Handler handler(DTIME, e, de, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      s, op, sfeh);
      handler.exe(DDSOE, s, STATEV, nullptr);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSOE) {
        m.rotateTangentOperatorBackward(DDSOE);
      }
    }  // end of AsterOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of AsterOrthotropicBehaviourHander<2u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicSmallStrainBehaviourHandler3D
      : private AsterBehaviourHandler<H, Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const DROT,
        AsterReal *const DDSOE,
        const AsterReal *const STRAN,
        const AsterReal *const DSTRAN,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      AsterReal s[6];
      AsterReal e[6];
      AsterReal de[6];
      // Passage au repère matériau
      AsterRotationMatrix3D m(DROT);
      m.rotateStressesForward(STRESS, s);
      m.rotateStrainsForward(STRAN, e);
      m.rotateStrainsForward(DSTRAN, de);
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      const bool bDDSOE = std::abs(*DDSOE) > 0.5;
      Handler handler(DTIME, e, de, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      s, op, sfeh);
      handler.exe(DDSOE, s, STATEV, nullptr);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSOE) {
        m.rotateTangentOperatorBackward(DDSOE);
      }
    }  // end of AsterOrthotropicBehaviourHandler<3u,Behaviour>::exe
  };

  /*!
   * The handler for finite strain orthotropic behaviours
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
      AsterOrthotropicBehaviourHandler<STANDARDFINITESTRAINBEHAVIOUR,
                                       H,
                                       Behaviour>
      : public AsterOrthotropicFiniteStrainBehaviourDispatcher<H, Behaviour>::
            type {
    typedef
        typename AsterOrthotropicFiniteStrainBehaviourDispatcher<H, Behaviour>::
            type Handler;
    using Handler::exe;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicFiniteStrainBehaviourHandler1D
      : private AsterBehaviourHandler<H, Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const,
        AsterReal *const DDSOE,
        const AsterReal *const F0,
        const AsterReal *const F1,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME, F0, F1, TEMP, DTEMP, PROPS, PREDEF, DPRED, STATEV,
                      STRESS, op, sfeh);
      handler.exe(DDSOE, STRESS, STATEV, F1);
    }  // end of AsterOrthotropicBehaviourHander<1u,Behaviour>::exe
  };   // end of struct AsterOrthotropicBehaviourHander<1u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicFiniteStrainBehaviourHandler2D
      : private AsterBehaviourHandler<H, Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const DROT,
        AsterReal *const DDSOE,
        const AsterReal *const F0,
        const AsterReal *const F1,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      AsterReal s[4];
      AsterReal F0m[9];
      AsterReal F1m[9];
      AsterFiniteStrainRotationMatrix2D m(DROT);
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      m.rotateStressesForward(STRESS, s);
      m.rotateDeformationGradientForward(F0, F0m);
      m.rotateDeformationGradientForward(F1, F1m);
      const bool bDDSOE = std::abs(*DDSOE) > 0.5;
      Handler handler(DTIME, F0m, F1m, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, s, op, sfeh);
      handler.exe(DDSOE, s, STATEV, F1m);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSOE) {
        m.rotateTangentOperatorBackward(DDSOE);
      }
    }  // end of AsterOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of AsterOrthotropicBehaviourHander<2u,Behaviour>

  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicFiniteStrainBehaviourHandler2D<
      tfel::material::ModellingHypothesis::PLANESTRESS,
      Behaviour>
      : private AsterBehaviourHandler<
            tfel::material::ModellingHypothesis::PLANESTRESS,
            Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const DROT,
        AsterReal *const DDSOE,
        const AsterReal *const F0,
        const AsterReal *const F1,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::PLANESTRESS;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      AsterReal s[4];
      AsterReal F0m[9];
      AsterReal F1m[9];
      AsterFiniteStrainRotationMatrix2D m(DROT);
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      m.rotateStressesForward(STRESS, s);
      m.rotateDeformationGradientForward(F0, F0m);
      m.rotateDeformationGradientForward(F1, F1m);
      const bool bDDSOE = std::abs(*DDSOE) > 0.5;
      Handler handler(DTIME, F0m, F1m, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, s, op, sfeh);
      handler.exe(DDSOE, s, STATEV, F1m);
      m.rotateStressesBackward(s, STRESS);
      if (bDDSOE) {
        m.rotateTangentOperatorBackward(DDSOE);
      }
    }  // end of AsterOrthotropicBehaviourHander<2u,Behaviour>::exe
  };   // end of AsterOrthotropicBehaviourHander<2u,Behaviour>

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicFiniteStrainBehaviourHandler3D
      : private AsterBehaviourHandler<H, Behaviour> {
    TFEL_ASTER_INLINE2 static void exe(
        const AsterReal *const DTIME,
        const AsterReal *const DROT,
        AsterReal *const DDSOE,
        const AsterReal *const F0,
        const AsterReal *const F1,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H, AsterReal, false>> MTraits;
      typedef AsterTraits<Behaviour<H, AsterReal, false>> Traits;
      typedef AsterBehaviourHandler<H, Behaviour> ABHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_, typename ABHandler::template Integrator<bs, ba>,
          typename ABHandler::Error>::type Handler;
      AsterReal sm[6];
      AsterReal F0m[9];
      AsterReal F1m[9];
      // Passage au repère matériau
      AsterFiniteStrainRotationMatrix3D m(DROT);
      m.rotateStressesForward(STRESS, sm);
      m.rotateDeformationGradientForward(F0, F0m);
      m.rotateDeformationGradientForward(F1, F1m);
      ABHandler::checkNPROPS(*NPROPS);
      ABHandler::checkNSTATV(*NSTATV);
      const bool bDDSOE = std::abs(*DDSOE) > 0.5;
      Handler handler(DTIME, F0m, F1m, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, sm, op, sfeh);
      handler.exe(DDSOE, sm, STATEV, F1m);
      m.rotateStressesBackward(sm, STRESS);
      if (bDDSOE) {
        m.rotateTangentOperatorBackward(DDSOE);
      }
    }  // end of AsterOrthotropicBehaviourHandler<3u,Behaviour>::exe

  };  // end of struct AsterOrthotropicBehaviourHandler<3u,Behaviour>

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTER_ASTERORTHOTROPICBEHAVIOURHANDLER_HXX */
