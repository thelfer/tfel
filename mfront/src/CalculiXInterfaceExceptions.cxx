/*!
 * \file   mfront/src/CalculiXInterfaceExceptions.cxx
 * \brief  This file implements the CalculiXInterfaceExceptions class.
 * \author Thomas Helfer
 * \date   12/12/2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <sstream>
#include <iostream>

#include "TFEL/Raise.hxx"
#include "MFront/CalculiX/CalculiXInterfaceExceptions.hxx"

namespace calculix {

  void CalculiXInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const CalculiXInt n2) {
    std::ostringstream msg;
    msg << "CalculiXInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties : "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<CalculiXException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void CalculiXInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const CalculiXInt n2) {
    std::ostringstream msg;
    msg << "CalculiXInterfaceExceptions::throwUnMatchedNumberOfStateVariables "
           ": "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<CalculiXException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void CalculiXInterfaceExceptions::treatCalculiXException(
      const std::string& b, const CalculiXException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an CalculiXException : " << e.what()
              << std::endl;
  }  // end of CalculiXInterfaceExceptions::treatCalculiXException

  void CalculiXInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException : " << e.what()
              << std::endl;
  }  // end of treatMaterialException

  void CalculiXInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception : " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void CalculiXInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception : " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void CalculiXInterfaceExceptions::treatUnknownException(
      const std::string& b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void CalculiXInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    tfel::raise(
        "CalculiXInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        b + "'");
  }  // end of CalculiXInterfaceExceptions::throwNegativeTimeStepException

  void CalculiXInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    tfel::raise(
        "CalculiXInterfaceExceptions::"
        "throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'");
  }

  void CalculiXInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "CalculiXInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator");
  }  // end of
     // CalculiXInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  CalculiXInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "CalculiXInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator");
  }  // end of
     // CalculiXInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  CalculiXInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    tfel::raise(
        "CalculiXInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        b +
        "' can handle stress-free expansion but "
        "the CalculiX interface can't");
  }  // end of
     // CalculiXInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void CalculiXInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    std::cout
        << "CalculiXInterfaceExceptions::displayUnsupportedHypothesisMessage : "
        << "unsupported hypothesis" << std::endl;
  }

  void CalculiXUnSupportedCaseHandler::exe(const CalculiXReal* const,
                                           const CalculiXReal* const,
                                           CalculiXReal* const,
                                           const CalculiXReal* const,
                                           const CalculiXReal* const,
                                           const CalculiXReal* const,
                                           const CalculiXReal* const,
                                           const CalculiXReal* const,
                                           const CalculiXInt* const,
                                           const CalculiXReal* const,
                                           const CalculiXReal* const,
                                           CalculiXReal* const,
                                           const CalculiXInt* const,
                                           CalculiXReal* const,
                                           const StressFreeExpansionHandler&) {
    tfel::raise(
        "CalculiXUnSupportedCaseHandler::exe : "
        "we fall in a case that the calculix interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace calculix
