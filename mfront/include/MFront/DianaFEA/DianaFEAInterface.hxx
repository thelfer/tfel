/*!
 * \file   mfront/include/MFront/DianaFEA/DianaFEAInterface.hxx
 * \brief  This file implements the DianaFEAInterface class.
 * \author Thomas Helfer
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DIANAFEA_CALL_HXX
#define LIB_MFRONT_DIANAFEA_CALL_HXX

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

#include "MFront/DianaFEA/DianaFEA.hxx"
#include "MFront/DianaFEA/DianaFEAData.hxx"
#include "MFront/DianaFEA/DianaFEAConfig.hxx"
#include "MFront/DianaFEA/DianaFEATraits.hxx"
#include "MFront/DianaFEA/DianaFEAException.hxx"
#include "MFront/DianaFEA/DianaFEABehaviourHandler.hxx"
#include "MFront/DianaFEA/DianaFEAInterfaceExceptions.hxx"

namespace dianafea {

  /*!
   * \class  DianaFEAInterface
   * \brief This class create an interface between a behaviour class
   * and the dianafea finite element code
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
  struct TFEL_VISIBILITY_LOCAL DianaFEAInterface
      : protected DianaFEAInterfaceExceptions {
    TFEL_DIANAFEA_INLINE2 static int exe(const DianaFEAData& d) {
      using BV = Behaviour<H, DianaFEAReal, false>;
      using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
      const bool is_defined_ = MTraits::is_defined;
      using Handler =
          typename std::conditional<is_defined_, CallBehaviour,
                                    UnsupportedHypothesisHandler>::type;
      try {
        Handler::exe(d);
      } catch (const DianaFEAException& e) {
        DianaFEAInterfaceExceptions::treatDianaFEAException(MTraits::getName(),
                                                            e);
        return -2;
      } catch (const tfel::material::OutOfBoundsException& e) {
        DianaFEAInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                            e);
        return -3;
      } catch (const tfel::material::DivergenceException& e) {
        DianaFEAInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                            e);
        return -4;
      } catch (const tfel::material::MaterialException& e) {
        DianaFEAInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                            e);
        return -5;
      } catch (const tfel::exception::TFELException& e) {
        DianaFEAInterfaceExceptions::treatTFELException(MTraits::getName(), e);
        return -6;
      } catch (const std::exception& e) {
        DianaFEAInterfaceExceptions::treatStandardException(MTraits::getName(),
                                                            e);
        return -7;
      } catch (...) {
        DianaFEAInterfaceExceptions::treatUnknownException(MTraits::getName());
        return -8;
      }
      return 0;
    }  // end of exe

   private:
    struct UnsupportedHypothesisHandler {
      TFEL_DIANAFEA_INLINE2 static void exe(const DianaFEAData&) {
        using BV = Behaviour<H, DianaFEAReal, false>;
        using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
        throw(DianaFEAInvalidModellingHypothesis(MTraits::getName()));
      }
    };  // end of struct UnsupportedHypothesisHandler

    struct CallBehaviour {
      TFEL_DIANAFEA_INLINE2 static void exe(const DianaFEAData& d) {
        typedef DianaFEABehaviourHandler<H, Behaviour> AHandler;
        using BV = Behaviour<H, DianaFEAReal, false>;
        using ATraits = DianaFEATraits<BV>;
        const bool bs = ATraits::requiresStiffnessTensor;
        const bool ba = ATraits::requiresThermalExpansionCoefficientTensor;
        using Integrator = typename AHandler::template Integrator<bs, ba>;
        AHandler::checkNPROPS(d.NPROPS);
        AHandler::checkNSTATV(d.NSTATV);
        Integrator i(d);
        i.exe(d);
      }
    };  // end of struct CallBehaviour2
  };    // end of struct DianaFEAInterface

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEA_CALL_HXX */
