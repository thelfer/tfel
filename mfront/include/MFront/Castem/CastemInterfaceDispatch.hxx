/*!
 * \file  mfront/include/MFront/Castem/CastemInterfaceDispatch.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 24 juil. 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_CASTEMINTERFACEDISPATCH_HXX
#define LIB_MFRONT_CASTEM_CASTEMINTERFACEDISPATCH_HXX

#include <type_traits>
#include "TFEL/Material/MechanicalBehaviourTraits.hxx"

#include "MFront/Castem/Castem.hxx"
#include "MFront/Castem/CastemTraits.hxx"
#include "MFront/Castem/CastemInterfaceExceptions.hxx"

#include "MFront/Castem/CastemOutOfBoundsPolicy.hxx"
#include "MFront/Castem/CastemIsotropicBehaviour.hxx"
#include "MFront/Castem/CastemOrthotropicBehaviour.hxx"
#include "MFront/Castem/CastemComputeThermalExpansionCoefficientTensor.hxx"
#include "MFront/Castem/CastemGetModellingHypothesis.hxx"
#include "MFront/Castem/CastemBehaviourHandler.hxx"
#include "MFront/Castem/CastemIsotropicBehaviourHandler.hxx"
#include "MFront/Castem/CastemOrthotropicBehaviourHandler.hxx"
#include "MFront/Castem/CastemGenericPlaneStressHandler.hxx"

namespace castem {

  /*!
   * \class  CastemInterfaceDispatch
   * \brief
   * \author Thomas Helfer
   * \date   24 Jul 2013
   */
  template <
      CastemBehaviourType type,
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemInterfaceDispatch : public CastemInterfaceExceptions {
    TFEL_CASTEM_INLINE2 static void exe(const CastemInt *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        const CastemInt *const,
                                        const CastemReal *const,
                                        const CastemReal *const,
                                        CastemReal *const,
                                        const CastemInt *const,
                                        CastemReal *const,
                                        CastemReal *const,
                                        const tfel::material::OutOfBoundsPolicy,
                                        const StressFreeExpansionHandler &) {
      typedef tfel::material::ModellingHypothesis MH;
      CastemInterfaceExceptions::
          throwInvalidBehaviourTypeAndModellingHypothesis(type,
                                                          MH::toString(H));
    }  // end of exe
  };   // end of struct CastemInterfaceDispatch

  /*!
   * \class  CastemInterfaceDispatch
   * \brief  partial specialisation for models
   * \author Thomas Helfer
   * \date   24 Jul 2013
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Model>
  struct CastemInterfaceDispatch<MODEL, H, Model>
      : public CastemInterfaceExceptions {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemInt *const,
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
      using namespace tfel::material;
      using ModelImplementation = Model<H, CastemReal, false>;
      using Traits = CastemTraits<ModelImplementation>;
      using MTraits = MechanicalBehaviourTraits<ModelImplementation>;
      using CBHandler = CastemBehaviourHandler<MODEL, H, Model>;
      constexpr auto is_defined_ = MTraits::is_defined;
      constexpr auto bs = Traits::requiresStiffnessTensor;
      constexpr auto ba = Traits::requiresThermalExpansionCoefficientTensor;
      static_assert(!bs, "mdoels can't require a stiffness tensor");
      static_assert(
          !ba, "mdoels can't require a thermal expansion coefficient tensor");
      using Handler = std::conditional_t<
          is_defined_,
          std::conditional_t<
              Traits::useTimeSubStepping,
              typename CBHandler::template IntegratorWithTimeStepping<bs, ba>,
              typename CBHandler::template Integrator<bs, ba>>,
          typename CBHandler::Error>;
      if constexpr (is_defined_) {
        CBHandler::checkNPROPS(*NPROPS);
        CBHandler::checkNSTATV(*NSTATV);
      } else {
        static_cast<void>(NPROPS);
        static_cast<void>(NSTATV);
      }
      Handler handler(DTIME, STRAN, DSTRAN, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, STRESS, op, sfeh);
      handler.exe(DDSDDE, STRESS, STATEV, PNEWDT);
    }  // end of exe
  };   // end of struct CastemInterfaceDispatch

  /*!
   * \class  CastemInterfaceDispatch
   * \brief  partial specialisation for small strain behaviours
   * \author Thomas Helfer
   * \date   24 Jul 2013
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemInterfaceDispatch<STANDARDSTRAINBASEDBEHAVIOUR, H, Behaviour>
      : public CastemInterfaceExceptions {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemInt *const NTENS,
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
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef Behaviour<H, CastemReal, false> BV;
      //! a simple alias
      typedef CastemTraits<BV> Traits;
      typedef typename std::conditional<
          Traits::stype == castem::ISOTROPIC,
          CastemIsotropicBehaviourHandler<STANDARDSTRAINBASEDBEHAVIOUR, H,
                                          Behaviour>,
          CastemOrthotropicBehaviourHandler<STANDARDSTRAINBASEDBEHAVIOUR, H,
                                            Behaviour>>::type Handler;
      CastemInterfaceExceptions::checkNTENSValue(
          *NTENS, Traits::ThermodynamicForceVariableSize);
      Handler::exe(DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
                   NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, PNEWDT, op,
                   sfeh);
    }  // end of exe
  };   // end of struct CastemInterfaceDispatch

  /*!
   * \class  CastemInterfaceDispatch
   * \brief  partial specialisation for small strain behaviours
   * \author Thomas Helfer
   * \date   24 Jul 2013
   */
  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemInterfaceDispatch<
      STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      Behaviour> : public CastemInterfaceExceptions {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemInt *const NTENS,
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
      using namespace std;
      using namespace tfel::meta;
      typedef tfel::material::ModellingHypothesis MH;
      using BV = Behaviour<MH::PLANESTRESS, CastemReal, false>;
      using GPSBV = Behaviour<MH::GENERALISEDPLANESTRAIN, CastemReal, false>;
      typedef CastemTraits<BV> Traits;
      constexpr bool usesGenericPlaneStressHandler =
          (!tfel::material::MechanicalBehaviourTraits<BV>::is_defined) &&
          (tfel::material::MechanicalBehaviourTraits<GPSBV>::is_defined);
      using Handler = typename std::conditional<
          usesGenericPlaneStressHandler,
          CastemGenericPlaneStressHandler<Behaviour>,
          typename std::conditional<
              Traits::stype == castem::ISOTROPIC,
              CastemIsotropicBehaviourHandler<STANDARDSTRAINBASEDBEHAVIOUR,
                                              MH::PLANESTRESS, Behaviour>,
              CastemOrthotropicBehaviourHandler<STANDARDSTRAINBASEDBEHAVIOUR,
                                                MH::PLANESTRESS,
                                                Behaviour>>::type>::type;
      CastemInterfaceExceptions::checkNTENSValue(
          *NTENS, Traits::ThermodynamicForceVariableSize);
      Handler::exe(DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
                   NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, PNEWDT, op,
                   sfeh);
    }
  };

  /*!
   * \class  CastemInterfaceDispatch
   * \brief  partial specialisation for small strain behaviours
   * \author Thomas Helfer
   * \date   24 Jul 2013
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemInterfaceDispatch<STANDARDFINITESTRAINBEHAVIOUR, H, Behaviour>
      : public CastemInterfaceExceptions {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemInt *const NTENS,
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
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef Behaviour<H, CastemReal, false> BV;
      //! a simple alias
      typedef CastemTraits<BV> Traits;
      typedef typename std::conditional<
          Traits::stype == castem::ISOTROPIC,
          CastemIsotropicBehaviourHandler<STANDARDFINITESTRAINBEHAVIOUR, H,
                                          Behaviour>,
          CastemOrthotropicBehaviourHandler<STANDARDFINITESTRAINBEHAVIOUR, H,
                                            Behaviour>>::type Handler;
      CastemInterfaceExceptions::checkNTENSValue(
          *NTENS, Traits::ThermodynamicForceVariableSize);
      Handler::exe(DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
                   NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, PNEWDT, op,
                   sfeh);
    }  // end of exe
  };   // end of struct CastemInterfaceDispatch

  /* cohesive zones models */
  template <
      tfel::material::ModellingHypothesis::Hypothesis hypothesis,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemCohesiveZoneModelInterfaceDispatch2D
      : public CastemInterfaceExceptions {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemInt *const NTENS,
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
      using namespace std;
      using namespace tfel::meta;
      typedef Behaviour<hypothesis, CastemReal, false> BV;
      //! a simple alias
      typedef CastemTraits<BV> Traits;
      typedef typename std::conditional<
          Traits::stype == castem::ISOTROPIC,
          CastemIsotropicBehaviourHandler<COHESIVEZONEMODEL, hypothesis,
                                          Behaviour>,
          CastemOrthotropicBehaviourHandler<COHESIVEZONEMODEL, hypothesis,
                                            Behaviour>>::type Handler;
      CastemInterfaceExceptions::checkNTENSValue(
          *NTENS, Traits::ThermodynamicForceVariableSize);
      CastemReal D[4];
      CastemReal u[2], du[2], t[2];
      u[0] = STRAN[1];
      u[1] = STRAN[0];
      du[0] = DSTRAN[1];
      du[1] = DSTRAN[0];
      t[0] = STRESS[1];
      t[1] = STRESS[0];
      Handler::exe(DTIME, DROT, DDSDDE, u, du, TEMP, DTEMP, PROPS, NPROPS,
                   PREDEF, DPRED, STATEV, NSTATV, t, PNEWDT, op, sfeh);
      D[0] = DDSDDE[0];
      D[1] = DDSDDE[1];
      D[2] = DDSDDE[2];
      D[3] = DDSDDE[3];
      DDSDDE[0] = D[3];
      DDSDDE[1] = D[2];
      DDSDDE[2] = D[1];
      DDSDDE[3] = D[0];
      STRESS[0] = t[1];
      STRESS[1] = t[0];
    }  // end of exe
  };   // end of struct CastemInterfaceDispatch

  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemInterfaceDispatch<
      COHESIVEZONEMODEL,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      Behaviour>
      : public CastemCohesiveZoneModelInterfaceDispatch2D<
            tfel::material::ModellingHypothesis::PLANESTRAIN,
            Behaviour> {};

  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemInterfaceDispatch<
      COHESIVEZONEMODEL,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      Behaviour>
      : public CastemCohesiveZoneModelInterfaceDispatch2D<
            tfel::material::ModellingHypothesis::AXISYMMETRICAL,
            Behaviour> {};

  /* cohesive zones models */
  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemInterfaceDispatch<
      COHESIVEZONEMODEL,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      Behaviour> : public CastemInterfaceExceptions {
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemInt *const NTENS,
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
      using namespace std;
      using namespace tfel::meta;
      typedef tfel::material::ModellingHypothesis MH;
      typedef Behaviour<MH::TRIDIMENSIONAL, CastemReal, false> BV;
      //! a simple alias
      typedef CastemTraits<BV> Traits;
      typedef typename std::conditional<
          Traits::stype == castem::ISOTROPIC,
          CastemIsotropicBehaviourHandler<COHESIVEZONEMODEL, MH::TRIDIMENSIONAL,
                                          Behaviour>,
          CastemOrthotropicBehaviourHandler<
              COHESIVEZONEMODEL, MH::TRIDIMENSIONAL, Behaviour>>::type Handler;
      CastemInterfaceExceptions::checkNTENSValue(
          *NTENS, Traits::ThermodynamicForceVariableSize);
      CastemReal D[9];
      CastemReal u[3], du[3], t[3];
      u[0] = STRAN[2];
      u[1] = STRAN[0];
      u[2] = STRAN[1];
      du[0] = DSTRAN[2];
      du[1] = DSTRAN[0];
      du[2] = DSTRAN[1];
      t[0] = STRESS[2];
      t[1] = STRESS[0];
      t[2] = STRESS[1];
      Handler::exe(DTIME, DROT, DDSDDE, u, du, TEMP, DTEMP, PROPS, NPROPS,
                   PREDEF, DPRED, STATEV, NSTATV, t, PNEWDT, op, sfeh);
      D[0] = DDSDDE[0];
      D[1] = DDSDDE[1];
      D[2] = DDSDDE[2];
      D[3] = DDSDDE[3];
      D[4] = DDSDDE[4];
      D[5] = DDSDDE[5];
      D[6] = DDSDDE[6];
      D[7] = DDSDDE[7];
      D[8] = DDSDDE[8];
      DDSDDE[0] = D[4];
      DDSDDE[1] = D[5];
      DDSDDE[2] = D[3];
      DDSDDE[3] = D[7];
      DDSDDE[4] = D[8];
      DDSDDE[5] = D[6];
      DDSDDE[6] = D[1];
      DDSDDE[7] = D[2];
      DDSDDE[8] = D[0];
      STRESS[0] = t[1];
      STRESS[1] = t[2];
      STRESS[2] = t[0];
    }  // end of exe
  };   // end of struct CastemInterfaceDispatch

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMINTERFACEDISPATCH_HXX */
