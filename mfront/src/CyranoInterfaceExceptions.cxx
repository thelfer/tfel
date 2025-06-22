/*!
 * \file   mfront/src/CyranoInterfaceExceptions.cxx
 * \brief  This file implements the CyranoInterfaceExceptions class.
 * \author Helfer Thomas
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
    using namespace std;
    ostringstream msg;
    msg << "CyranoInterfaceExceptions::"
           "throwUnMatchedNumberOfMaterialProperties : "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    throw(CyranoException(msg.str()));
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void CyranoInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const CyranoInt n2) {
    using namespace std;
    ostringstream msg;
    msg << "CyranoInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    throw(CyranoException(msg.str()));
  }  // end of throwUnMatchedNumberOfStateVariables

  void CyranoInterfaceExceptions::treatCyranoException(
      const std::string& b, const CyranoException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown an CyranoException : " << e.what() << endl;
  }  // end of treatCyranoException

  void CyranoInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown an MaterialException : " << e.what() << endl;
  }  // end of treatMaterialException

  void CyranoInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown a generic tfel exception : " << e.what() << endl;
  }  // end of treatTFELException

  void CyranoInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown a generic standard exception : " << e.what() << endl;
  }  // end of treatStandardException

  void CyranoInterfaceExceptions::treatUnknownException(const std::string& b) {
    using namespace std;
    cout << "The behaviour '" << b << "' has thrown an unknown exception"
         << endl;
  }  // end of treatUnknownException

  void CyranoInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    using namespace std;
    string msg("CyranoInterfaceExceptions::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '" + b + "'";
    throw(runtime_error(msg));
  }  // end of CyranoInterfaceExceptions::throwNegativeTimeStepException

  void CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException(
      const std::string& b) {
    using namespace std;
    string msg;
    msg += "integration failed for behaviour '" + b + "'";
    throw(CyranoIntegrationFailed(msg));
  }  // end of
     // CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  CyranoInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException(
      const std::string& b) {
    using namespace std;
    string msg;
    msg += "maximum number of sub stepping reached failed for behaviour '" + b +
           "'";
    throw(CyranoIntegrationFailed(msg));
  }  // end of
     // CyranoInterfaceExceptions::throwMaximumNumberOfSubSteppingReachedException

  void CyranoInterfaceExceptions::
      throwPlaneStressMaximumNumberOfIterationsReachedException(
          const std::string& b) {
    using namespace std;
    string msg;
    msg += "maximum number of iterations of the plane stress algorithm ";
    msg += "reached failed for behaviour '" + b + "'";
    throw(CyranoIntegrationFailed(msg));
  }  // end of
     // CyranoInterfaceExceptions::throwPlaneStressMaximumNumberOfIterationsReachedException

  void CyranoInterfaceExceptions::checkNTENSValue(const CyranoInt NTENS,
                                                  const unsigned short s) {
    using namespace std;
    if (NTENS != s) {
      string msg(
          "CyranoInterfaceExceptions::checkNTENSValue : "
          "invalid value for the NTENS parameter "
          "(got '" +
          to_string(NTENS) +
          "', "
          "expected '" +
          to_string(static_cast<unsigned int>(s)) + "')");
      throw(runtime_error(msg));
    }
  }  // end of CyranoInterfaceExceptions::checkNTENSValue

  void
  CyranoInterfaceExceptions::displayInvalidModellingHypothesisErrorMessage() {
    using namespace std;
    cout << "CyranoInterfaceExceptions::"
            "displayInvalidModellingHypothesisErrorMessage : "
         << "invalid value for the NDI parameter" << endl;
  }

  void
  CyranoInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis(
      const std::string& h) {
    using namespace std;
    string msg(
        "CyranoInterfaceExceptions::"
        "throwInvalidBehaviourTypeAndModellingHypothesis : "
        "behaviour can't be used in '" +
        h + "'");
    throw(runtime_error(msg));
  }  // end of
     // CyranoInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis

  void CyranoInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    using namespace std;
    string msg(
        "CyranoInterfaceExceptions::throwPredictionComputationFailedException "
        ": ");
    msg += "prediction computation failed for behaviour '" + b + "'";
    throw(runtime_error(msg));
  }

  void CyranoInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    using namespace std;
    string msg(
        "CyranoInterfaceExceptions::throwPredictionOperatorIsNotAvalaible : ");
    msg += "behaviour '" + b + "' can't compute a prediction operator";
    throw(runtime_error(msg));
  }  // end of
     // CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CyranoInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    using namespace std;
    string msg(
        "CyranoInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible : ");
    msg += "behaviour '" + b + "' can't compute a consistent tangent operator";
    throw(runtime_error(msg));
  }  // end of
     // CyranoInterfaceExceptions::throwBehaviourIntegrationFailedException

  void CyranoInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    using namespace std;
    string msg(
        "CyranoInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException : ");
    msg += "behaviour '" + b +
           "' can handle stress-free expansion but the Cyrano interface can't";
    throw(runtime_error(msg));
  }  // end of
     // CyranoInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void CyranoInterfaceExceptions::throwThermalExpansionCoefficientShallBeNull(
      const std::string& b) {
    using namespace std;
    string msg(
        "CyranoInterfaceExceptions::"
        "throwThermalExpansionCoefficientShallBeNull : ");
    msg += "behaviour '" + b +
           "' handle thermal expansion, so the thermal expansion "
           "declared in Cyrano must be null.";
    throw(runtime_error(msg));
  }

  void CyranoInterfaceExceptions::throwInvalidDDSOEException(
      const std::string& b, const CyranoReal v) {
    using namespace std;
    ostringstream msg;
    msg << "CyranoInterfaceExceptions::throwInvalidDDSOEException : "
        << "an invalid value for the DDSOE parameter has been given ('" << v
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
    throw(runtime_error(msg.str()));
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
    using namespace std;
    string msg(
        "CyranoUnSupportedCaseHandler::exe : "
        "we fall in a case that the cyrano interface "
        "is not able to handle.");
    throw(runtime_error(msg));
  }  // end of exe

}  // end of namespace cyrano
