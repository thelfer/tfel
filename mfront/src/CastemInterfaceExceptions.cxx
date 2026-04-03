/*!
 * \file   mfront/src/CastemInterfaceExceptions.cxx
 * \brief  This file implements the CastemInterfaceExceptions class.
 * \author Thomas Helfer
 * \date   12/12/2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "TFEL/Raise.hxx"
#include "MFront/Castem/CastemInterfaceExceptions.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

static bool getDisplayErrorMessageFlag() {
  const auto* e = ::getenv("CASTEM_DISPLAY_ERROR_MESSAGE");
  if (e == nullptr) {
    return true;
  }
  return ::strcmp(e, "true") == 0;
}

static bool displayErrorMessage() {
  static bool b = getDisplayErrorMessageFlag();
  return b;
}  // end of displayErrorMessage

namespace castem {

  void CastemInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string_view b, const unsigned short n1, const CastemInt n2) {
    std::ostringstream msg;
    msg << "CastemInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties : "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<CastemException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void CastemInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string_view b, const unsigned short n1, const CastemInt n2) {
    std::ostringstream msg;
    msg << "CastemInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<CastemException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void CastemInterfaceExceptions::treatCastemException(
      const std::string_view b, const CastemException& e) {
    if (displayErrorMessage()) {
      std::cout << "The behaviour '" << b
                << "' has thrown an CastemException : " << e.what()
                << std::endl;
    }
  }  // end of treatCastemException

  void CastemInterfaceExceptions::treatMaterialException(
      const std::string_view b, const tfel::material::MaterialException& e) {
    if (displayErrorMessage()) {
      std::cout << "The behaviour '" << b
                << "' has thrown an MaterialException : " << e.what()
                << std::endl;
    }
  }  // end of treatMaterialException

  void CastemInterfaceExceptions::treatTFELException(
      const std::string_view b, const tfel::exception::TFELException& e) {
    if (displayErrorMessage()) {
      std::cout << "The behaviour '" << b
                << "' has thrown a generic tfel exception : " << e.what()
                << std::endl;
    }
  }  // end of treatTFELException

  void CastemInterfaceExceptions::treatStandardException(
      const std::string_view b, const std::exception& e) {
    if (displayErrorMessage()) {
      std::cout << "The behaviour '" << b
                << "' has thrown a generic standard exception : " << e.what()
                << std::endl;
    }
  }  // end of treatStandardException

  void CastemInterfaceExceptions::treatUnknownException(
      const std::string_view b) {
    if (displayErrorMessage()) {
      std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
                << std::endl;
    }
  }  // end of treatUnknownException

  void CastemInterfaceExceptions::throwNegativeTimeStepException(
      const std::string_view b) {
    tfel::raise(
        "CastemInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        std::string{b} + "'");
  }  // end of throwNegativeTimeStepException

  void CastemInterfaceExceptions::throwBehaviourInitializationFailed(
      const std::string_view b) {
    tfel::raise("initialization of behaviour '" + std::string{b} + "' failed");
  }  // end of throwBehaviourIntegrationFailedException

  void CastemInterfaceExceptions::throwBehaviourIntegrationFailedException(
      const std::string_view b) {
    tfel::raise<CastemIntegrationFailed>("integration failed for behaviour '" +
                                         std::string{b} + "'");
  }  // end of throwBehaviourIntegrationFailedException

  void
  CastemInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException(
      const std::string_view b) {
    tfel::raise<CastemIntegrationFailed>(
        "maximum number of sub stepping reached failed for behaviour '" +
        std::string{b} + "'");
  }  // end of
     // CastemInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException

  void CastemInterfaceExceptions::
      throwPlaneStressMaximumNumberOfIterationsReachedException(
          const std::string_view b) {
    tfel::raise<CastemIntegrationFailed>(
        "maximum number of iterations of the plane stress algorithm "
        "reached failed for behaviour '" +
        std::string{b} + "'");
  }  // end of
     // CastemInterfaceExceptions::throwPlaneStressMaximumNumberOfIterationsReachedException

  void CastemInterfaceExceptions::checkNTENSValue(const CastemInt NTENS,
                                                  const unsigned short s) {
    if (NTENS != s) {
      tfel::raise(
          "CastemInterfaceExceptions::checkNTENSValue : "
          "invalid value for the NTENS parameter "
          "(got '" +
          std::to_string(NTENS) +
          "', "
          "expected '" +
          std::to_string(static_cast<unsigned int>(s)) + "')");
    }
  }  // end of checkNTENSValue

  void
  CastemInterfaceExceptions::displayInvalidModellingHypothesisErrorMessage() {
    if (displayErrorMessage()) {
      std::cout << "CastemInterfaceExceptions::"
                << "displayInvalidModellingHypothesisErrorMessage : "
                << "invalid value for the NDI parameter" << std::endl;
    }
  }

  void
  CastemInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis(
      const CastemBehaviourType, const std::string_view h) {
    tfel::raise(
        "CastemInterfaceExceptions::"
        "throwInvalidBehaviourTypeAndModellingHypothesis: "
        "behaviour can't be used in '" +
        std::string{h} + "'");
  }  // end of
     // CastemInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis

  void CastemInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string_view b) {
    tfel::raise(
        "CastemInterfaceExceptions::throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        std::string{b} + "'");
  }

  void CastemInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string_view b) {
    tfel::raise(
        "CastemInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        std::string{b} + "' can't compute a prediction operator");
  }  // end of
     // CastemInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CastemInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string_view b) {
    tfel::raise(
        "CastemInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        std::string{b} + "' can't compute a consistent tangent operator");
  }  // end of
     // CastemInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CastemInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string_view b) {
    tfel::raise(
        "CastemInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        std::string{b} +
        "' can handle stress-free expansion "
        "but the Castem interface can't");
  }  // end of
     // CastemInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void CastemInterfaceExceptions::throwThermalExpansionCoefficientShallBeNull(
      const std::string_view b) {
    tfel::raise(
        "CastemInterfaceExceptions::"
        "throwThermalExpansionCoefficientShallBeNull: "
        "behaviour '" +
        std::string{b} +
        "' handle thermal expansion, so "
        "the thermal expansion declared in Castem must be null.");
  }

  void CastemInterfaceExceptions::
      throwTangentOperatorNotAvailableThroughGenericPlaneStressHandler(
          const std::string_view) {
    tfel::raise(
        "tangent operator is not available in the "
        "generic plane stress handler");
  }

  void CastemInterfaceExceptions::throwInvalidDDSDDEException(
      const std::string_view b, const CastemReal v) {
    tfel::raise(
        "CastemInterfaceExceptions::throwInvalidDDSDDEException : "
        "an invalid value for the DDSDDE parameter has been given "
        "('" +
        std::to_string(v) +
        "')"
        " to the behaviour '" +
        std::string{b} +
        "'.\n"
        "The following values are accepted:\n"
        "-3 : compute the prediction tangent operator, no behaviour "
        "integration\n"
        "-2 : compute the prediction secant  operator, no behaviour "
        "integration\n"
        "-1 : compute the prediction elastic operator, no behaviour "
        "integration\n"
        " 0 : integrate the behaviour over the time step, no stiffness "
        "requested\n"
        " 1 : integrate the behaviour over the time step, elastic stiffness "
        "requested\n"
        " 2 : integrate the behaviour over the time step, secant  operator  "
        "requested\n"
        " 3 : integrate the behaviour over the time step, tagent  operator  "
        "requested\n"
        " 4 : integrate the behaviour over the time step, consistent tagent "
        "operator requested");
  }  // end of throwInvalidDDSDDEException

  void CastemUnSupportedCaseHandler::exe(const CastemReal* const,
                                         const CastemReal* const,
                                         CastemReal* const,
                                         const CastemReal* const,
                                         const CastemReal* const,
                                         const CastemReal* const,
                                         const CastemReal* const,
                                         const CastemReal* const,
                                         const CastemInt* const,
                                         const CastemReal* const,
                                         const CastemReal* const,
                                         CastemReal* const,
                                         const CastemInt* const,
                                         CastemReal* const,
                                         const StressFreeExpansionHandler&) {
    tfel::raise(
        "CastemUnSupportedCaseHandler::exe : "
        "we fall in a case that the castem interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace castem
