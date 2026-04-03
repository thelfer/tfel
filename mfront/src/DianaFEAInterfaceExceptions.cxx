/*!
 * \file   mfront/src/DianaFEAInterfaceExceptions.cxx
 * \brief  This file implements the DianaFEAInterfaceExceptions class.
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
#include "MFront/DianaFEA/DianaFEAInterfaceExceptions.hxx"

namespace dianafea {

  void DianaFEAInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const DianaFEAInt n2) {
    std::ostringstream msg;
    msg << "DianaFEAInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties : "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<DianaFEAException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void DianaFEAInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const DianaFEAInt n2) {
    std::ostringstream msg;
    msg << "DianaFEAInterfaceExceptions::throwUnMatchedNumberOfStateVariables "
           ": "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<DianaFEAException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void DianaFEAInterfaceExceptions::treatDianaFEAException(
      const std::string& b, const DianaFEAException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an DianaFEAException : " << e.what()
              << std::endl;
  }  // end of DianaFEAInterfaceExceptions::treatDianaFEAException

  void DianaFEAInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException : " << e.what()
              << std::endl;
  }  // end of treatMaterialException

  void DianaFEAInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception : " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void DianaFEAInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception : " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void DianaFEAInterfaceExceptions::treatUnknownException(
      const std::string& b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void DianaFEAInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    tfel::raise(
        "DianaFEAInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        b + "'");
  }  // end of DianaFEAInterfaceExceptions::throwNegativeTimeStepException

  void DianaFEAInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    tfel::raise(
        "DianaFEAInterfaceExceptions::"
        "throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'");
  }

  void DianaFEAInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "DianaFEAInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator");
  }  // end of
     // DianaFEAInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  DianaFEAInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "DianaFEAInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator");
  }  // end of
     // DianaFEAInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  DianaFEAInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    tfel::raise(
        "DianaFEAInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        b +
        "' can handle stress-free expansion but the DianaFEA interface can't");
  }  // end of
     // DianaFEAInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void DianaFEAInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    std::cout
        << "DianaFEAInterfaceExceptions::displayUnsupportedHypothesisMessage : "
        << "unsupported hypothesis" << std::endl;
  }

  void DianaFEAUnSupportedCaseHandler::exe(
      const DianaFEAReal* const,
      const DianaFEAReal* const,
      DianaFEAReal* const,
      const DianaFEAReal* const,
      const DianaFEAReal* const,
      const DianaFEAReal* const,
      const DianaFEAReal* const,
      const DianaFEAReal* const,
      const DianaFEAInt* const,
      const DianaFEAReal* const,
      const DianaFEAReal* const,
      DianaFEAReal* const,
      const DianaFEAInt* const,
      DianaFEAReal* const,
      const StressFreeExpansionHandler<DianaFEAReal>&) {
    tfel::raise(
        "DianaFEAUnSupportedCaseHandler::exe : "
        "we fall in a case that the dianafea interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace dianafea
