/*!
 * \file   mfront/include/MFront/Abaqus/AbaqusInterface.hxx
 * \brief  This file implements the AbaqusInterface class.
 * \author Thomas Helfer
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUS_CALL_HXX
#define LIB_MFRONT_ABAQUS_CALL_HXX

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <type_traits>

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"

#include "TFEL/Material/MechanicalBehaviourTraits.hxx"
#include "TFEL/Material/MaterialException.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusData.hxx"
#include "MFront/Abaqus/AbaqusConfig.hxx"
#include "MFront/Abaqus/AbaqusTraits.hxx"
#include "MFront/Abaqus/AbaqusException.hxx"
#include "MFront/Abaqus/AbaqusBehaviourHandler.hxx"
#include "MFront/Abaqus/AbaqusInterfaceExceptions.hxx"

namespace abaqus {

  /*!
   * \class  AbaqusInterface
   * \brief This class create an interface between a behaviour class
   * and the abaqus finite element code
   *
   * \note : most of branching is done at compile-time. to the very
   * exeception of the stress-free expansion which are handled through
   * two functions pointers which take into account the specificities
   * of finite strain strategy used. The choice of introducing those
   * runtime-functions comes from the fact that we did not want a code
   * duplication between two different finite strain strategies (to
   * reduce both compile-time and library size).
   */
  template <
      tfel::material::ModellingHypothesis::Hypothesis H,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusInterface
      : protected AbaqusInterfaceExceptions {
    TFEL_ABAQUS_INLINE2 static int exe(const AbaqusData& d) {
      using BV = Behaviour<H, AbaqusReal, false>;
      using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
      const bool is_defined_ = MTraits::is_defined;
      using Handler =
          typename std::conditional<is_defined_, CallBehaviour,
                                    UnsupportedHypothesisHandler>::type;
      try {
        Handler::exe(d);
      } catch (const AbaqusException& e) {
        AbaqusInterfaceExceptions::treatAbaqusException(MTraits::getName(), e);
        return -2;
      } catch (const tfel::material::OutOfBoundsException& e) {
        AbaqusInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                          e);
        return -3;
      } catch (const tfel::material::DivergenceException& e) {
        AbaqusInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                          e);
        return -4;
      } catch (const tfel::material::MaterialException& e) {
        AbaqusInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                          e);
        return -5;
      } catch (const tfel::exception::TFELException& e) {
        AbaqusInterfaceExceptions::treatTFELException(MTraits::getName(), e);
        return -6;
      } catch (const std::exception& e) {
        AbaqusInterfaceExceptions::treatStandardException(MTraits::getName(),
                                                          e);
        return -7;
      } catch (...) {
        AbaqusInterfaceExceptions::treatUnknownException(MTraits::getName());
        return -8;
      }
      return 0;
    }  // end of exe

   private:
    struct UnsupportedHypothesisHandler {
      TFEL_ABAQUS_INLINE2 static void exe(const AbaqusData&) {
        using BV = Behaviour<H, AbaqusReal, false>;
        using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
        throw(AbaqusInvalidModellingHypothesis(MTraits::getName()));
      }
    };  // end of struct UnsupportedHypothesisHandler

    struct CallBehaviour {
      TFEL_ABAQUS_INLINE2 static void exe(const AbaqusData& d) {
        typedef AbaqusBehaviourHandler<H, Behaviour> AHandler;
        using BV = Behaviour<H, AbaqusReal, false>;
        using ATraits = AbaqusTraits<BV>;
        TFEL_CONSTEXPR const auto bs = ATraits::requiresStiffnessTensor;
        TFEL_CONSTEXPR const auto ba =
            ATraits::requiresThermalExpansionCoefficientTensor;
        using Integrator = typename AHandler::template Integrator<bs, ba>;
#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS
        AHandler::checkNPROPS(d.NPROPS);
        AHandler::checkNSTATV(d.NSTATV);
        AHandler::checkNTENS(d.NTENS);
#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */
        Integrator i(d);
        i.exe(d);
      }
    };  // end of struct CallBehaviour2
  };    // end of struct AbaqusInterface

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_CALL_HXX */
