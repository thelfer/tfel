/*!
 * \file   mfront/include/MFront/LSDYNA/LSDYNAInterface.hxx
 * \brief  This file implements the LSDYNAInterface class.
 * \author Thomas Helfer
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LSDYNA_CALL_HXX
#define LIB_MFRONT_LSDYNA_CALL_HXX

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

#include "MFront/LSDYNA/LSDYNA.hxx"
#include "MFront/LSDYNA/LSDYNAData.hxx"
#include "MFront/LSDYNA/LSDYNAConfig.hxx"
#include "MFront/LSDYNA/LSDYNATraits.hxx"
#include "MFront/LSDYNA/LSDYNAException.hxx"
#include "MFront/LSDYNA/LSDYNABehaviourHandler.hxx"
#include "MFront/LSDYNA/LSDYNAInterfaceExceptions.hxx"

namespace lsdyna {

  /*!
   * \class  LSDYNAInterface
   * \brief This class create an interface between a behaviour class
   * and the lsdyna finite element code
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
  struct TFEL_VISIBILITY_LOCAL LSDYNAInterface
      : protected LSDYNAInterfaceExceptions {
    TFEL_LSDYNA_INLINE2 static int exe(const LSDYNAData& d) {
      using BV = Behaviour<H, LSDYNAReal, false>;
      using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
      const bool is_defined_ = MTraits::is_defined;
      using Handler =
          typename std::conditional<is_defined_, CallBehaviour,
                                    UnsupportedHypothesisHandler>::type;
      try {
        Handler::exe(d);
      } catch (const LSDYNAException& e) {
        LSDYNAInterfaceExceptions::treatLSDYNAException(MTraits::getName(), e);
        return -2;
      } catch (const tfel::material::OutOfBoundsException& e) {
        LSDYNAInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                          e);
        return -3;
      } catch (const tfel::material::DivergenceException& e) {
        LSDYNAInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                          e);
        return -4;
      } catch (const tfel::material::MaterialException& e) {
        LSDYNAInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                          e);
        return -5;
      } catch (const tfel::exception::TFELException& e) {
        LSDYNAInterfaceExceptions::treatTFELException(MTraits::getName(), e);
        return -6;
      } catch (const std::exception& e) {
        LSDYNAInterfaceExceptions::treatStandardException(MTraits::getName(),
                                                          e);
        return -7;
      } catch (...) {
        LSDYNAInterfaceExceptions::treatUnknownException(MTraits::getName());
        return -8;
      }
      return 0;
    }  // end of exe

   private:
    struct UnsupportedHypothesisHandler {
      TFEL_LSDYNA_INLINE2 static void exe(const LSDYNAData&) {
        using BV = Behaviour<H, LSDYNAReal, false>;
        using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
        throw(LSDYNAInvalidModellingHypothesis(MTraits::getName()));
      }
    };  // end of struct UnsupportedHypothesisHandler

    struct CallBehaviour {
      TFEL_LSDYNA_INLINE2 static void exe(const LSDYNAData& d) {
        typedef LSDYNABehaviourHandler<H, Behaviour> AHandler;
        using BV = Behaviour<H, LSDYNAReal, false>;
        using ATraits = LSDYNATraits<BV>;
        constexpr const auto bs = ATraits::requiresStiffnessTensor;
        constexpr const auto ba =
            ATraits::requiresThermalExpansionCoefficientTensor;
        using Integrator = typename AHandler::template Integrator<bs, ba>;
#ifndef MFRONT_LSDYNA_NORUNTIMECHECKS
        AHandler::checkNPROPS(d.NPROPS);
        AHandler::checkNSTATV(d.NSTATV);
        AHandler::checkNTENS(d.NTENS);
#endif /* MFRONT_LSDYNA_NORUNTIMECHECKS */
        Integrator i(d);
        i.exe(d);
      }
    };  // end of struct CallBehaviour2
  };    // end of struct LSDYNAInterface

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNA_CALL_HXX */
