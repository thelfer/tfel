/*!
 * \file   mfront/src/LSDYNAInterfaceExceptions.cxx
 * \brief  This file implements the LSDYNAInterfaceExceptions class.
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
#include "MFront/LSDYNA/LSDYNAInterfaceExceptions.hxx"

namespace lsdyna {

  void LSDYNAInterfaceExceptions::throwInvalidTensorSize(
      const std::string& b, const unsigned short n1, const LSDYNAInt n2) {
    std::ostringstream msg;
    msg << "LSDYNAInterfaceExceptions::throwInvalidTensorSize: "
        << "the size of symmetric tensors for the current hypothesis does not "
           "match "
        << "`LSDYNA` declaration (NTENS) for behaviour '" << b
        << "'. Expected '" << n1 << "', got '" << n2 << "'.";
    tfel::raise<LSDYNAException>(msg.str());
  }  // end LSDYNAInterfaceExceptions::throwInvalidTensorSize

  void LSDYNAInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const LSDYNAInt n2) {
    std::ostringstream msg;
    msg << "LSDYNAInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties: "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<LSDYNAException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void LSDYNAInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const LSDYNAInt n2) {
    std::ostringstream msg;
    msg << "LSDYNAInterfaceExceptions::throwUnMatchedNumberOfStateVariables: "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<LSDYNAException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void LSDYNAInterfaceExceptions::treatLSDYNAException(
      const std::string& b, const LSDYNAException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an LSDYNAException: " << e.what() << std::endl;
  }  // end of LSDYNAInterfaceExceptions::treatLSDYNAException

  void LSDYNAInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException: " << e.what() << std::endl;
  }  // end of treatMaterialException

  void LSDYNAInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception: " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void LSDYNAInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception: " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void LSDYNAInterfaceExceptions::treatUnknownException(const std::string& b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void LSDYNAInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    tfel::raise(
        "LSDYNAInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        b + "'");
  }  // end of LSDYNAInterfaceExceptions::throwNegativeTimeStepException

  void LSDYNAInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    tfel::raise(
        "LSDYNAInterfaceExceptions::throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'");
  }

  void LSDYNAInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "LSDYNAInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator");
  }  // end of
     // LSDYNAInterfaceExceptions::throwBehaviourIntegrationFailedException

  void LSDYNAInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "LSDYNAInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator");
  }  // end of
     // LSDYNAInterfaceExceptions::throwBehaviourIntegrationFailedException

  void LSDYNAInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    tfel::raise(
        "LSDYNAInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        b +
        "' can handle stress-free expansion but the LSDYNA interface can't");
  }  // end of
     // LSDYNAInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void LSDYNAInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    std::cout
        << "LSDYNAInterfaceExceptions::displayUnsupportedHypothesisMessage: "
        << "unsupported hypothesis" << std::endl;
  }

  void LSDYNAUnSupportedCaseHandler::exe(
      const LSDYNAReal* const,
      const LSDYNAReal* const,
      LSDYNAReal* const,
      const LSDYNAReal* const,
      const LSDYNAReal* const,
      const LSDYNAReal* const,
      const LSDYNAReal* const,
      const LSDYNAReal* const,
      const LSDYNAInt* const,
      const LSDYNAReal* const,
      const LSDYNAReal* const,
      LSDYNAReal* const,
      const LSDYNAInt* const,
      LSDYNAReal* const,
      const StressFreeExpansionHandler<LSDYNAReal>&) {
    tfel::raise(
        "LSDYNAUnSupportedCaseHandler::exe: "
        "we fall in a case that the lsdyna interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace lsdyna
