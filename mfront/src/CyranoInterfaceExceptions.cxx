/*!
 * \file   mfront/src/CyranoInterfaceExceptions.cxx
 * \brief  This file implements the CyranoInterfaceExceptions class.
 * \author Thomas Helfer
 * \date   21 fév 2014
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
#include "MFront/Cyrano/CyranoInterfaceExceptions.hxx"

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

namespace cyrano {

  void CyranoInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const CyranoInt n2) {
    std::ostringstream msg;
    msg << "CyranoInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties : "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<CyranoException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void CyranoInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const CyranoInt n2) {
    std::ostringstream msg;
    msg << "CyranoInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<CyranoException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void CyranoInterfaceExceptions::treatCyranoException(
      const std::string& b, const CyranoException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an CyranoException : " << e.what() << std::endl;
  }  // end of treatCyranoException

  void CyranoInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException : " << e.what()
              << std::endl;
  }  // end of treatMaterialException

  void CyranoInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception : " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void CyranoInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception : " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void CyranoInterfaceExceptions::treatUnknownException(const std::string& b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void CyranoInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    tfel::raise(
        "CyranoInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        b + "'");
  }  // end of CyranoInterfaceExceptions::throwNegativeTimeStepException

  void CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException(
      const std::string& b) {
    tfel::raise<CyranoIntegrationFailed>("integration failed for behaviour '" +
                                         b + "'");
  }  // end of
     // CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  CyranoInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException(
      const std::string& b) {
    tfel::raise<CyranoIntegrationFailed>(
        "maximum number of sub stepping reached "
        "failed for behaviour '" +
        b + "'");
  }  // end of
     // CyranoInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException

  void CyranoInterfaceExceptions::
      throwPlaneStressMaximumNumberOfIterationsReachedException(
          const std::string& b) {
    tfel::raise<CyranoIntegrationFailed>(
        "maximum number of iterations of "
        "the plane stress algorithm "
        "reached failed for behaviour '" +
        b + "'");
  }  // end of
     // CyranoInterfaceExceptions::throwPlaneStressMaximumNumberOfIterationsReachedException

  void CyranoInterfaceExceptions::checkNTENSValue(const CyranoInt NTENS,
                                                  const unsigned short s) {
    tfel::raise_if(NTENS != s,
                   "CyranoInterfaceExceptions::checkNTENSValue: "
                   "invalid value for the NTENS parameter "
                   "(got '" +
                       std::to_string(NTENS) + "', expected '" +
                       std::to_string(static_cast<unsigned int>(s)) + "')");
  }  // end of CyranoInterfaceExceptions::checkNTENSValue

  void
  CyranoInterfaceExceptions::displayInvalidModellingHypothesisErrorMessage() {
    std::cout << "CyranoInterfaceExceptions::"
                 "displayInvalidModellingHypothesisErrorMessage : "
              << "invalid value for the NDI parameter" << std::endl;
  }

  void
  CyranoInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis(
      const std::string& h) {
    tfel::raise(
        "CyranoInterfaceExceptions::"
        "throwInvalidBehaviourTypeAndModellingHypothesis : "
        "behaviour can't be used in '" +
        h + "'");
  }  // end of
     // CyranoInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis

  void CyranoInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    tfel::raise(
        "CyranoInterfaceExceptions::throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'");
  }

  void CyranoInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "CyranoInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator");
  }  // end of
     // CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CyranoInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    tfel::raise(
        "CyranoInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator");
  }  // end of
     // CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CyranoInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    tfel::raise(
        "CyranoInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        b +
        "' can handle stress-free expansion "
        "but the Cyrano interface can't");
  }  // end of
     // CyranoInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void CyranoInterfaceExceptions::throwThermalExpansionCoefficientShallBeNull(
      const std::string& b) {
    tfel::raise(
        "CyranoInterfaceExceptions::"
        "throwThermalExpansionCoefficientShallBeNull: "
        "behaviour '" +
        b +
        "' handle thermal expansion, so the thermal expansion "
        "declared in Cyrano must be null.");
  }

  void CyranoInterfaceExceptions::throwInvalidDDSOEException(
      const std::string& b, const CyranoReal v) {
    tfel::raise(
        "CyranoInterfaceExceptions::throwInvalidDDSOEException : "
        "an invalid value for the DDSOE parameter has been given "
        "('" +
        std::to_string(v) +
        "') "
        "to the behaviour '" +
        b +
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
        "operator requested\n");
  }  // end of CyranoInterfaceExceptions::throwInvalidDDSOEException

  void CyranoUnSupportedCaseHandler::exe(const CyranoReal* const,
                                         const CyranoReal* const,
                                         const CyranoReal* const,
                                         const CyranoReal* const,
                                         const CyranoReal* const,
                                         const CyranoReal* const,
                                         const CyranoReal* const,
                                         const CyranoReal* const,
                                         const CyranoInt* const,
                                         const CyranoReal* const,
                                         const CyranoReal* const,
                                         CyranoReal* const,
                                         const CyranoInt* const,
                                         CyranoReal* const) {
    tfel::raise(
        "CyranoUnSupportedCaseHandler::exe : "
        "we fall in a case that the cyrano interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace cyrano
