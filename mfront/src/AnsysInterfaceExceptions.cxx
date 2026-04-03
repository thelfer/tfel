/*!
 * \file   mfront/src/AnsysInterfaceExceptions.cxx
 * \brief  This file implements the AnsysInterfaceExceptions class.
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
#include "MFront/Ansys/AnsysInterfaceExceptions.hxx"

namespace ansys {

  void AnsysInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const AnsysInt n2) {
    std::ostringstream msg;
    msg << "AnsysInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties "
           ": "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<AnsysException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void AnsysInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const AnsysInt n2) {
    std::ostringstream msg;
    msg << "AnsysInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<AnsysException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void AnsysInterfaceExceptions::treatAnsysException(const std::string& b,
                                                     const AnsysException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an AnsysException : " << e.what() << std::endl;
  }  // end of AnsysInterfaceExceptions::treatAnsysException

  void AnsysInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException : " << e.what()
              << std::endl;
  }  // end of treatMaterialException

  void AnsysInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception : " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void AnsysInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception : " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void AnsysInterfaceExceptions::treatUnknownException(const std::string& b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void AnsysInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    tfel::raise(
        "AnsysInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        b + "'");
  }  // end of AnsysInterfaceExceptions::throwNegativeTimeStepException

  void AnsysInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    tfel::raise(
        "AnsysInterfaceExceptions::throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'");
  }

  void AnsysInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "AnsysInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator");
  }  // end of
     // AnsysInterfaceExceptions::throwBehaviourIntegrationFailedException

  void AnsysInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "AnsysInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator");
  }  // end of
     // AnsysInterfaceExceptions::throwBehaviourIntegrationFailedException

  void AnsysInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    tfel::raise(
        "AnsysInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        b + "' can handle stress-free expansion but the Ansys interface can't");
  }  // end of
     // AnsysInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void AnsysInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    std::cout
        << "AnsysInterfaceExceptions::displayUnsupportedHypothesisMessage : "
        << "unsupported hypothesis" << std::endl;
  }

  void AnsysUnSupportedCaseHandler::exe(
      const AnsysReal* const,
      const AnsysReal* const,
      AnsysReal* const,
      const AnsysReal* const,
      const AnsysReal* const,
      const AnsysReal* const,
      const AnsysReal* const,
      const AnsysReal* const,
      const AnsysInt* const,
      const AnsysReal* const,
      const AnsysReal* const,
      AnsysReal* const,
      const AnsysInt* const,
      AnsysReal* const,
      const StressFreeExpansionHandler<AnsysReal>&) {
    tfel::raise(
        "AnsysUnSupportedCaseHandler::exe : "
        "we fall in a case that the ansys interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace ansys
