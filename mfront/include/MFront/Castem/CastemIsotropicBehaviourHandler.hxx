/*!
 * \file  mfront/include/MFront/Castem/CastemIsotropicBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_CASTEM_CASTEMISOTROPICBEHAVIOURHANDLER_H_
#define LIB_MFRONT_CASTEM_CASTEMISOTROPICBEHAVIOURHANDLER_H_

#ifndef LIB_MFRONT_CASTEM_CASTEMBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CASTEM_CASTEMBEHAVIOURHANDLER_H_ */

#include "TFEL/FSAlgorithm/copy.hxx"

namespace castem {

  template <
      CastemBehaviourType type,
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemIsotropicBehaviourHandlerBase
      : public CastemBehaviourHandler<type, H, Behaviour> {
    TFEL_CASTEM_INLINE static void exe(
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
      typedef MechanicalBehaviourTraits<Behaviour<H, CastemReal, false>>
          MTraits;
      typedef CastemTraits<Behaviour<H, CastemReal, false>> Traits;
      typedef CastemBehaviourHandler<type, H, Behaviour> CastemBehaviourHandler;
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
      ThermalExpansionCoefficientCheck::exe(PROPS[3]);
      Handler handler(DTIME, STRAN, DSTRAN, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, STRESS, op, sfeh);
      handler.exe(DDSDDE, STRESS, STATEV, PNEWDT);
    }  // end of CastemIsotropicBehaviourHandlerBase::exe

  };  // end of struct CastemIsotropicBehaviourHandlerBase

  template <
      CastemBehaviourType type,
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemIsotropicBehaviourHandler
      : public CastemIsotropicBehaviourHandlerBase<type, H, Behaviour> {
    using CastemIsotropicBehaviourHandlerBase<type, H, Behaviour>::exe;
  };

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
      CastemIsotropicBehaviourHandler<COHESIVEZONEMODEL, H, Behaviour>
      : public CastemIsotropicBehaviourHandlerBase<COHESIVEZONEMODEL,
                                                   H,
                                                   Behaviour> {
    TFEL_CASTEM_INLINE static void exe(
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
      // local material properties to match castem conventions
      // In Cast3M, the tangential stiffness is given by PROPS[0],
      // the normal stiffness by PROPS[1].
      // In MFront, we want the normal stiffness to be the first one
      using namespace tfel::material;
      using tfel::fsalgo::copy;
      typedef Behaviour<H, CastemReal, false> BV;
      typedef MechanicalBehaviourTraits<BV> MTraits;
      const unsigned short offset = CastemTraits<BV>::propertiesOffset;
      const unsigned short nprops = MTraits::material_properties_nb;
      const unsigned short NPROPS_ =
          offset + nprops == 0 ? 1u : offset + nprops;
      CastemReal mp[NPROPS_];
      copy<NPROPS_>::exe(PROPS, mp);
      mp[0] = PROPS[1];
      mp[1] = PROPS[0];
      CastemIsotropicBehaviourHandlerBase<COHESIVEZONEMODEL, H, Behaviour>::exe(
          DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP, mp, NPROPS, PREDEF,
          DPRED, STATEV, NSTATV, STRESS, PNEWDT, op, sfeh);
    }
  };

}  // namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMISOTROPICBEHAVIOURHANDLER_H_ */
