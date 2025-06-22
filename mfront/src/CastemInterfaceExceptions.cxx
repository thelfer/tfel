/*!
 * \file   mfront/src/CastemInterfaceExceptions.cxx
 * \brief  This file implements the CastemInterfaceExceptions class.
 * \author Helfer Thomas
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

namespace castem {

  void CastemInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const CastemInt n2) {
    using namespace std;
    ostringstream msg;
    msg << "CastemInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties : "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    throw(CastemException(msg.str()));
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void CastemInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const CastemInt n2) {
    using namespace std;
    ostringstream msg;
    msg << "CastemInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    throw(CastemException(msg.str()));
  }  // end of throwUnMatchedNumberOfStateVariables

  void CastemInterfaceExceptions::treatCastemException(
      const std::string& b, const CastemException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown an CastemException : " << e.what() << endl;
  }  // end of treatCastemException

  void CastemInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown an MaterialException : " << e.what() << endl;
  }  // end of treatMaterialException

  void CastemInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown a generic tfel exception : " << e.what() << endl;
  }  // end of treatTFELException

  void CastemInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown a generic standard exception : " << e.what() << endl;
  }  // end of treatStandardException

  void CastemInterfaceExceptions::treatUnknownException(const std::string& b) {
    using namespace std;
    cout << "The behaviour '" << b << "' has thrown an unknown exception"
         << endl;
  }  // end of treatUnknownException

  void CastemInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    using namespace std;
    string msg("CastemInterfaceExceptions::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '" + b + "'";
    throw(runtime_error(msg));
  }  // end of CastemInterfaceExceptions::throwNegativeTimeStepException

  void CastemInterfaceExceptions::throwBehaviourIntegrationFailedException(
      const std::string& b) {
    throw(CastemIntegrationFailed("integration failed for behaviour '" + b +
                                  "'"));
  }  // end of
     // CastemInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  CastemInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException(
      const std::string& b) {
    throw(CastemIntegrationFailed(
        "maximum number of sub stepping reached failed for behaviour '" + b +
        "'"));
  }  // end of
     // CastemInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException

  void CastemInterfaceExceptions::
      throwPlaneStressMaximumNumberOfIterationsReachedException(
          const std::string& b) {
    throw(CastemIntegrationFailed(
        "maximum number of iterations of the plane stress algorithm "
        "reached failed for behaviour '" +
        b + "'"));
  }  // end of
     // CastemInterfaceExceptions::throwPlaneStressMaximumNumberOfIterationsReachedException

  void CastemInterfaceExceptions::checkNTENSValue(const CastemInt NTENS,
                                                  const unsigned short s) {
    using namespace std;
    if (NTENS != s) {
      throw(
          runtime_error("CastemInterfaceExceptions::checkNTENSValue : "
                        "invalid value for the NTENS parameter "
                        "(got '" +
                        to_string(NTENS) +
                        "', "
                        "expected '" +
                        to_string(static_cast<unsigned int>(s)) + "')"));
    }
  }  // end of CastemInterfaceExceptions::checkNTENSValue

  void
  CastemInterfaceExceptions::displayInvalidModellingHypothesisErrorMessage() {
    std::cout << "CastemInterfaceExceptions::"
                 "displayInvalidModellingHypothesisErrorMessage : "
              << "invalid value for the NDI parameter" << std::endl;
  }

  void
  CastemInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis(
      const CastemBehaviourType, const std::string& h) {
    throw(
        std::runtime_error("CastemInterfaceExceptions::"
                           "throwInvalidBehaviourTypeAndModellingHypothesis: "
                           "behaviour can't be used in '" +
                           h + "'"));
  }  // end of
     // CastemInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis

  void CastemInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    throw(std::runtime_error(
        "CastemInterfaceExceptions::throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        b + "'"));
  }

  void CastemInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    throw(std::runtime_error(
        "CastemInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a prediction operator"));
  }  // end of
     // CastemInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CastemInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    throw(std::runtime_error(
        "CastemInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        b + "' can't compute a consistent tangent operator"));
  }  // end of
     // CastemInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CastemInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    throw(
        std::runtime_error("CastemInterfaceExceptions::"
                           "throwUnsupportedStressFreeExpansionException: "
                           "behaviour '" +
                           b +
                           "' can handle stress-free expansion "
                           "but the Castem interface can't"));
  }  // end of
     // CastemInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void CastemInterfaceExceptions::throwThermalExpansionCoefficientShallBeNull(
      const std::string& b) {
    throw(std::runtime_error(
        "CastemInterfaceExceptions::"
        "throwThermalExpansionCoefficientShallBeNull: "
        "behaviour '" +
        b +
        "' handle thermal expansion, so "
        "the thermal expansion declared in Castem must be null."));
  }

  void CastemInterfaceExceptions::
      throwTangentOperatorNotAvailableThroughGenericPlaneStressHandler(
          const std::string&) {
    throw(
        std::runtime_error("tangent operator is not available in the "
                           "generic plane stress handler"));
  }

  void CastemInterfaceExceptions::throwInvalidDDSDDEException(
      const std::string& b, const CastemReal v) {
    std::ostringstream msg;
    msg << "CastemInterfaceExceptions::throwInvalidDDSDDEException : "
        << "an invalid value for the DDSDDE parameter has been given ('" << v
        << "')"
        << " to the behaviour '" << b << "'.\n"
        << "The following values are accepted:\n"
        << "-3 : compute the prediction tangent operator, no behaviour "
           "integration\n"
        << "-2 : compute the prediction secant  operator, no behaviour "
           "integration\n"
        << "-1 : compute the prediction elastic operator, no behaviour "
           "integration\n"
        << " 0 : integrate the behaviour over the time step, no stiffness "
           "requested\n"
        << " 1 : integrate the behaviour over the time step, elastic stiffness "
           "requested\n"
        << " 2 : integrate the behaviour over the time step, secant  operator  "
           "requested\n"
        << " 3 : integrate the behaviour over the time step, tagent  operator  "
           "requested\n"
        << " 4 : integrate the behaviour over the time step, consistent tagent "
           "operator requested\n";
    throw(std::runtime_error(msg.str()));
  }  // end of CastemInterfaceExceptions::throwInvalidDDSDDEException

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
    throw(
        std::runtime_error("CastemUnSupportedCaseHandler::exe : "
                           "we fall in a case that the castem interface "
                           "is not able to handle."));
  }  // end of exe

}  // end of namespace castem
