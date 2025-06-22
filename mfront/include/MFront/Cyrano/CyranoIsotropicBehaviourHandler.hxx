/*!
 * \file  mfront/include/MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_CYRANO_CYRANOISOTROPICBEHAVIOURHANDLER_HXX
#define LIB_MFRONT_CYRANO_CYRANOISOTROPICBEHAVIOURHANDLER_HXX

#ifndef LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_HXX */

#include "TFEL/FSAlgorithm/copy.hxx"

namespace cyrano {

  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoIsotropicBehaviourHandler {
    TFEL_CYRANO_INLINE static void exe(
        const CyranoReal *const DTIME,
        const CyranoReal *const,
        CyranoReal *const DDSOE,
        const CyranoReal *const STRAN,
        const CyranoReal *const DSTRAN,
        const CyranoReal *const TEMP,
        const CyranoReal *const DTEMP,
        const CyranoReal *const PROPS,
        const CyranoInt *const NPROPS,
        const CyranoReal *const PREDEF,
        const CyranoReal *const DPRED,
        CyranoReal *const STATEV,
        const CyranoInt *const NSTATV,
        CyranoReal *const STRESS,
        const StressFreeExpansionHandler sfeh,
        const tfel::material::OutOfBoundsPolicy op) {
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef MechanicalBehaviourTraits<Behaviour<H, CyranoReal, false>>
          MTraits;
      typedef CyranoTraits<Behaviour<H, CyranoReal, false>> Traits;
      typedef CyranoBehaviourHandler<H, Behaviour> CyranoBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename std::conditional<
          is_defined_,
          typename std::conditional<
              Traits::useTimeSubStepping,
              typename CyranoBehaviourHandler::
                  template IntegratorWithTimeStepping<bs, ba>,
              typename CyranoBehaviourHandler::template Integrator<bs,
                                                                   ba>>::type,
          typename CyranoBehaviourHandler::Error>::type Handler;
      CyranoBehaviourHandler::checkNPROPS(*NPROPS);
      CyranoBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME, STRAN, DSTRAN, TEMP, DTEMP, PROPS, PREDEF, DPRED,
                      STATEV, STRESS, sfeh, op);
      handler.exe(DDSOE, STRESS, STATEV);
    }  // end of CyranoIsotropicBehaviourHandler::exe
  };

}  // namespace cyrano

#endif /* LIB_MFRONT_CYRANO_CYRANOISOTROPICBEHAVIOURHANDLER_HXX */
