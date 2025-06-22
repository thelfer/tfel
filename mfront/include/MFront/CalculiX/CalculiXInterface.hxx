/*!
 * \file   mfront/include/MFront/CalculiX/CalculiXInterface.hxx
 * \brief  This file implements the CalculiXInterface class.
 * \author Thomas Helfer
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CALCULIX_CALL_HXX
#define LIB_MFRONT_CALCULIX_CALL_HXX

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

#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXData.hxx"
#include "MFront/CalculiX/CalculiXConfig.hxx"
#include "MFront/CalculiX/CalculiXTraits.hxx"
#include "MFront/CalculiX/CalculiXException.hxx"
#include "MFront/CalculiX/CalculiXBehaviourHandler.hxx"
#include "MFront/CalculiX/CalculiXInterfaceExceptions.hxx"

namespace calculix {

  /*!
   * \class  CalculiXInterface
   * \brief This class create an interface between a behaviour class
   * and the calculix finite element code
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
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CalculiXInterface
      : protected CalculiXInterfaceExceptions {
    //! the only supported modelling hypothesis
    static constexpr auto H =
        tfel::material::ModellingHypothesis::TRIDIMENSIONAL;

    TFEL_CALCULIX_INLINE2 static int exe(const CalculiXData& d) {
      using BV = Behaviour<H, CalculiXReal, false>;
      using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
      const bool is_defined_ = MTraits::is_defined;
      using Handler =
          typename std::conditional<is_defined_, CallBehaviour,
                                    UnsupportedHypothesisHandler>::type;
      try {
        Handler::exe(d);
      } catch (const CalculiXException& e) {
        CalculiXInterfaceExceptions::treatCalculiXException(MTraits::getName(),
                                                            e);
        return -2;
      } catch (const tfel::material::OutOfBoundsException& e) {
        CalculiXInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                            e);
        return -3;
      } catch (const tfel::material::DivergenceException& e) {
        CalculiXInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                            e);
        return -4;
      } catch (const tfel::material::MaterialException& e) {
        CalculiXInterfaceExceptions::treatMaterialException(MTraits::getName(),
                                                            e);
        return -5;
      } catch (const tfel::exception::TFELException& e) {
        CalculiXInterfaceExceptions::treatTFELException(MTraits::getName(), e);
        return -6;
      } catch (const std::exception& e) {
        CalculiXInterfaceExceptions::treatStandardException(MTraits::getName(),
                                                            e);
        return -7;
      } catch (...) {
        CalculiXInterfaceExceptions::treatUnknownException(MTraits::getName());
        return -8;
      }
      return 0;
    }  // end of exe

   private:
    struct UnsupportedHypothesisHandler {
      TFEL_CALCULIX_INLINE2 static void exe(const CalculiXData&) {
        using BV = Behaviour<H, CalculiXReal, false>;
        using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
        throw(CalculiXInvalidModellingHypothesis(MTraits::getName()));
      }
    };  // end of struct UnsupportedHypothesisHandler

    struct CallBehaviour {
      TFEL_CALCULIX_INLINE2 static void exe(const CalculiXData& d) {
        typedef CalculiXBehaviourHandler<H, Behaviour> AHandler;
        using BV = Behaviour<H, CalculiXReal, false>;
        using ATraits = CalculiXTraits<BV>;
        const bool bs = ATraits::requiresStiffnessTensor;
        const bool ba = ATraits::requiresThermalExpansionCoefficientTensor;
        using Integrator = typename AHandler::template Integrator<bs, ba>;
        // #ifndef MFRONT_CALCULIX_NORUNTIMECHECKS
        // 	AHandler::checkNPROPS(d.NPROPS);
        // 	AHandler::checkNSTATV(d.NSTATV);
        // #endif /* MFRONT_CALCULIX_NORUNTIMECHECKS */
        Integrator i(d);
        i.exe(d);
      }
    };  // end of struct CallBehaviour2
  };    // end of struct CalculiXInterface

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALL_HXX */
