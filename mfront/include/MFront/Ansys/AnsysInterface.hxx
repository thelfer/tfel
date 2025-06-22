/*!
 * \file   mfront/include/MFront/Ansys/AnsysInterface.hxx
 * \brief  This file implements the AnsysInterface class.
 * \author Thomas Helfer
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ANSYS_ANSYSINTERFACE_HXX
#define LIB_MFRONT_ANSYS_ANSYSINTERFACE_HXX

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

#include "MFront/Ansys/Ansys.hxx"
#include "MFront/Ansys/AnsysData.hxx"
#include "MFront/Ansys/AnsysConfig.hxx"
#include "MFront/Ansys/AnsysTraits.hxx"
#include "MFront/Ansys/AnsysException.hxx"
#include "MFront/Ansys/AnsysBehaviourHandler.hxx"
#include "MFront/Ansys/AnsysInterfaceExceptions.hxx"

namespace ansys {

  /*!
   * \class  AnsysInterface
   * \brief This class create an interface between a behaviour class
   * and the ansys finite element code
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
  struct TFEL_VISIBILITY_LOCAL AnsysInterface
      : protected AnsysInterfaceExceptions {
    TFEL_ANSYS_INLINE2 static int exe(const AnsysData& d) {
      using BV = Behaviour<H, AnsysReal, false>;
      using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
      const bool is_defined_ = MTraits::is_defined;
      using Handler =
          typename std::conditional<is_defined_, CallBehaviour,
                                    UnsupportedHypothesisHandler>::type;
      try {
        if (!Handler::exe(d)) {
          // behaviour integration failure
          return -1;
        }
      } catch (const AnsysException& e) {
        AnsysInterfaceExceptions::treatAnsysException(MTraits::getName(), e);
        return -2;
      } catch (const tfel::material::OutOfBoundsException& e) {
        AnsysInterfaceExceptions::treatMaterialException(MTraits::getName(), e);
        return -3;
      } catch (const tfel::material::DivergenceException& e) {
        AnsysInterfaceExceptions::treatMaterialException(MTraits::getName(), e);
        return -4;
      } catch (const tfel::material::MaterialException& e) {
        AnsysInterfaceExceptions::treatMaterialException(MTraits::getName(), e);
        return -5;
      } catch (const tfel::exception::TFELException& e) {
        AnsysInterfaceExceptions::treatTFELException(MTraits::getName(), e);
        return -6;
      } catch (const std::exception& e) {
        AnsysInterfaceExceptions::treatStandardException(MTraits::getName(), e);
        return -7;
      } catch (...) {
        AnsysInterfaceExceptions::treatUnknownException(MTraits::getName());
        return -8;
      }
      // everything's ok
      return 0;
    }  // end of exe

   private:
    struct UnsupportedHypothesisHandler {
      TFEL_ANSYS_INLINE2 static bool exe(const AnsysData&) {
        using BV = Behaviour<H, AnsysReal, false>;
        using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
        throw(AnsysInvalidModellingHypothesis(MTraits::getName()));
      }
    };  // end of struct UnsupportedHypothesisHandler

    struct CallBehaviour {
      TFEL_ANSYS_INLINE2 static bool exe(const AnsysData& d) {
        typedef AnsysBehaviourHandler<H, Behaviour> AHandler;
        using BV = Behaviour<H, AnsysReal, false>;
        using ATraits = AnsysTraits<BV>;
        const bool bs = ATraits::requiresStiffnessTensor;
        const bool ba = ATraits::requiresThermalExpansionCoefficientTensor;
        using Integrator = typename AHandler::template Integrator<bs, ba>;
        AHandler::checkNPROPS(d.NPROPS);
        AHandler::checkNSTATV(d.NSTATV);
        Integrator i(d);
        return i.exe(d);
      }
    };  // end of struct CallBehaviour2
  };    // end of struct AnsysInterface

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYS_ANSYSINTERFACE_HXX */
