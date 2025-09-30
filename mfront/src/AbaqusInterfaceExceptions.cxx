/*!
 * \file   mfront/src/AbaqusInterfaceExceptions.cxx
 * \brief  This file implements the AbaqusInterfaceExceptions class.
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
#include "MFront/Abaqus/AbaqusInterfaceExceptions.hxx"

namespace abaqus {

  void AbaqusInterfaceExceptions::throwInvalidTensorSize(
      const std::string& b, const unsigned short n1, const AbaqusInt n2) {
    std::ostringstream msg;
    msg << "AbaqusInterfaceExceptions::throwInvalidTensorSize: "
        << "the size of symmetric tensors for the current hypothesis does not "
           "match "
        << "`Abaqus` declaration (NTENS) for behaviour '" << b
        << "'. Expected '" << n1 << "', got '" << n2 << "'.";
    tfel::raise<AbaqusException>(msg.str());
  }  // end AbaqusInterfaceExceptions::throwInvalidTensorSize

  void AbaqusInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const AbaqusInt n2) {
    std::ostringstream msg;
    msg << "AbaqusInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties: "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<AbaqusException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void AbaqusInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const AbaqusInt n2) {
    std::ostringstream msg;
    msg << "AbaqusInterfaceExceptions::throwUnMatchedNumberOfStateVariables: "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<AbaqusException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void AbaqusInterfaceExceptions::treatAbaqusException(
      const std::string& b, const AbaqusException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an AbaqusException: " << e.what() << std::endl;
  }  // end of AbaqusInterfaceExceptions::treatAbaqusException

  void AbaqusInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException: " << e.what() << std::endl;
  }  // end of treatMaterialException

  void AbaqusInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception: " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void AbaqusInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception: " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void AbaqusInterfaceExceptions::treatUnknownException(const std::string& b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void AbaqusInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    tfel::raise(
        "AbaqusInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        b + "'");
  }  // end of AbaqusInterfaceExceptions::throwNegativeTimeStepException

  void AbaqusInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    tfel::raise(
        "AbaqusInterfaceExceptions::throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'");
  }

  void AbaqusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "AbaqusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator");
  }  // end of
     // AbaqusInterfaceExceptions::throwBehaviourIntegrationFailedException

  void AbaqusInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "AbaqusInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator");
  }  // end of
     // AbaqusInterfaceExceptions::throwBehaviourIntegrationFailedException

  void AbaqusInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    tfel::raise(
        "AbaqusInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        b +
        "' can handle stress-free expansion but the Abaqus interface can't");
  }  // end of
     // AbaqusInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void AbaqusInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    std::cout
        << "AbaqusInterfaceExceptions::displayUnsupportedHypothesisMessage: "
        << "unsupported hypothesis" << std::endl;
  }

  void AbaqusUnSupportedCaseHandler::exe(
      const AbaqusReal* const,
      const AbaqusReal* const,
      AbaqusReal* const,
      const AbaqusReal* const,
      const AbaqusReal* const,
      const AbaqusReal* const,
      const AbaqusReal* const,
      const AbaqusReal* const,
      const AbaqusInt* const,
      const AbaqusReal* const,
      const AbaqusReal* const,
      AbaqusReal* const,
      const AbaqusInt* const,
      AbaqusReal* const,
      const StressFreeExpansionHandler<AbaqusReal>&) {
    tfel::raise(
        "AbaqusUnSupportedCaseHandler::exe: "
        "we fall in a case that the abaqus interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace abaqus
