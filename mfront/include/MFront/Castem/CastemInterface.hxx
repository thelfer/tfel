/*!
 * \file   mfront/include/MFront/Castem/CastemInterface.hxx
 * \brief  This file implements the CastemInterface class.
 * \author Thomas Helfer
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_CALL_HXX
#define LIB_MFRONT_CASTEM_CALL_HXX

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/Material/MaterialException.hxx"

#include "MFront/Castem/Castem.hxx"
#include "MFront/Castem/CastemException.hxx"
#include "MFront/Castem/CastemInterfaceExceptions.hxx"
#include "MFront/Castem/CastemInterfaceDispatch.hxx"

namespace castem {

  /*!
   * \class CastemInterface
   * \brief This class create an interface * between an MFront
   * behaviour and the Cast3M finite element solver
   *
   * \note : most of branching is done at compile-time. to the very
   * exeception of the stress-free expansion which are handled through
   * two functions pointers which take into account the specificities
   * of finite strain strategy used. The choice of introducing those
   * runtime-functions comes from the fact that we did not want a code
   * duplication between two different finite strain strategies (to
   * reduce both compile-time and library size).
   *
   * \author Thomas Helfer
   * \date   28 Jul 2006
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemInterface
      : protected CastemInterfaceExceptions {
    /*!
     * Main entry point This barely make a dispatch based on the
     * spatial dimension and the modelling hypothesis.
     */
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
        CastemInt *const KINC,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using tfel::material::MechanicalBehaviourTraits;
      typedef Behaviour<H, CastemReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      typedef CastemTraits<BV> CastemTraits;
      try {
        CastemInterfaceDispatch<CastemTraits::btype, H, Behaviour>::exe(
            NTENS, DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
            NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, PNEWDT, op, sfeh);
      } catch (const CastemIntegrationFailed &e) {
        CastemInterfaceExceptions::treatCastemException(Traits::getName(), e);
        *KINC = -1;
      } catch (const CastemException &e) {
        CastemInterfaceExceptions::treatCastemException(Traits::getName(), e);
        *KINC = -2;
      } catch (const tfel::material::MaterialException &e) {
        CastemInterfaceExceptions::treatMaterialException(Traits::getName(), e);
        *KINC = -3;
      } catch (const tfel::exception::TFELException &e) {
        CastemInterfaceExceptions::treatTFELException(Traits::getName(), e);
        *KINC = -4;
      } catch (const std::exception &e) {
        CastemInterfaceExceptions::treatStandardException(Traits::getName(), e);
        *KINC = -5;
      } catch (...) {
        CastemInterfaceExceptions::treatUnknownException(Traits::getName());
        *KINC = -6;
      }
    }  // end of CastemInterface::callBehaviour

  };  // end of struct CastemInterface

}  // end of namespace castem

#include "MFront/Castem/CastemBehaviourHandler.hxx"
#include "MFront/Castem/CastemGenericPlaneStressHandler.hxx"

#endif /* LIB_MFRONT_CASTEM_CALL_HXX */
