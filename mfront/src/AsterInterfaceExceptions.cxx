/*!
 * \file   mfront/src/AsterInterfaceExceptions.cxx
 * \brief  This file implements the AsterInterfaceExceptions class.
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

#include "MFront/Aster/AsterInterfaceExceptions.hxx"

namespace aster {

  void AsterInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string& b, const unsigned short n1, const AsterInt n2) {
    using namespace std;
    ostringstream msg;
    msg << "AsterInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties "
           ": "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    throw(AsterException(msg.str()));
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void AsterInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string& b, const unsigned short n1, const AsterInt n2) {
    using namespace std;
    ostringstream msg;
    msg << "AsterInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    throw(AsterException(msg.str()));
  }  // end of throwUnMatchedNumberOfStateVariables

  void AsterInterfaceExceptions::treatAsterException(const std::string& b,
                                                     const AsterException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown an AsterException : " << e.what() << endl;
  }  // end of AsterInterfaceExceptions::treatAsterException

  void AsterInterfaceExceptions::treatMaterialException(
      const std::string& b, const tfel::material::MaterialException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown an MaterialException : " << e.what() << endl;
  }  // end of treatMaterialException

  void AsterInterfaceExceptions::treatTFELException(
      const std::string& b, const tfel::exception::TFELException& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown a generic tfel exception : " << e.what() << endl;
  }  // end of treatTFELException

  void AsterInterfaceExceptions::treatStandardException(
      const std::string& b, const std::exception& e) {
    using namespace std;
    cout << "The behaviour '" << b
         << "' has thrown a generic standard exception : " << e.what() << endl;
  }  // end of treatStandardException

  void AsterInterfaceExceptions::treatUnknownException(const std::string& b) {
    using namespace std;
    cout << "The behaviour '" << b << "' has thrown an unknown exception"
         << endl;
  }  // end of treatUnknownException

  void AsterInterfaceExceptions::throwNegativeTimeStepException(
      const std::string& b) {
    using namespace std;
    string msg("AsterInterfaceExceptions::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '" + b + "'";
    throw(runtime_error(msg));
  }  // end of AsterInterfaceExceptions::throwNegativeTimeStepException

  void AsterInterfaceExceptions::throwInvalidDDSOEException(
      const std::string& b, const AsterReal v) {
    using namespace std;
    ostringstream msg;
    msg << "AsterInterfaceExceptions::throwInvalidDDSOEException : "
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
  }  // end of AsterInterfaceExceptions::throwInvalidDDSOEException

  void AsterInterfaceExceptions::throwBehaviourIntegrationFailedException(
      const std::string& b) {
    using namespace std;
    string msg(
        "AsterInterfaceExceptions::throwBehaviourIntegrationFailedException "
        ": ");
    msg += "integration failed for behaviour '" + b + "'";
    throw(runtime_error(msg));
  }  // end of
     // AsterInterfaceExceptions::throwBehaviourIntegrationFailedException

  void AsterInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string& b) {
    using namespace std;
    string msg(
        "AsterInterfaceExceptions::throwPredictionComputationFailedException "
        ": ");
    msg += "prediction computation failed for behaviour '" + b + "'";
    throw(runtime_error(msg));
  }

  void AsterInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string& b) {
    using namespace std;
    string msg(
        "AsterInterfaceExceptions::throwPredictionOperatorIsNotAvalaible : ");
    msg += "behaviour '" + b + "' can't compute a prediction operator";
    throw(runtime_error(msg));
  }  // end of
     // AsterInterfaceExceptions::throwBehaviourIntegrationFailedException

  void AsterInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string& b) {
    using namespace std;
    string msg(
        "AsterInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible : ");
    msg += "behaviour '" + b + "' can't compute a consistent tangent operator";
    throw(runtime_error(msg));
  }  // end of
     // AsterInterfaceExceptions::throwBehaviourIntegrationFailedException

  void AsterInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string& b) {
    using namespace std;
    string msg(
        "AsterInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException : ");
    msg += "behaviour '" + b +
           "' can handle stress-free expansion but the Aster interface can't";
    throw(runtime_error(msg));
  }  // end of
     // AsterInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void AsterInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    using namespace std;
    cout << "AsterInterfaceExceptions::displayUnsupportedHypothesisMessage : "
         << "unsupported hypothesis" << endl;
  }

  void AsterReduceTangentOperator<1u>::exe(AsterReal* const DDSOE,
                                           const AsterReal* const K) {
    DDSOE[0] = K[0];
    DDSOE[1] = K[1];
    DDSOE[2] = K[2];
    DDSOE[3] = K[6];
    DDSOE[4] = K[7];
    DDSOE[5] = K[8];
    DDSOE[6] = K[12];
    DDSOE[7] = K[13];
    DDSOE[8] = K[14];
  }  // end of AsterReduceTangentOperator<1u>::exe

  void AsterReduceTangentOperator<2u>::exe(AsterReal* const DDSOE,
                                           const AsterReal* const K) {
    DDSOE[0] = K[0];
    DDSOE[1] = K[1];
    DDSOE[2] = K[2];
    DDSOE[3] = K[3];
    DDSOE[4] = K[6];
    DDSOE[5] = K[7];
    DDSOE[6] = K[8];
    DDSOE[7] = K[9];
    DDSOE[8] = K[12];
    DDSOE[9] = K[13];
    DDSOE[10] = K[14];
    DDSOE[11] = K[15];
    DDSOE[12] = K[18];
    DDSOE[13] = K[19];
    DDSOE[14] = K[20];
    DDSOE[15] = K[21];
  }  // end of AsterReduceTangentOperator<1u>::exe

  void AsterUnSupportedCaseHandler::exe(const AsterReal* const,
                                        const AsterReal* const,
                                        AsterReal* const,
                                        const AsterReal* const,
                                        const AsterReal* const,
                                        const AsterReal* const,
                                        const AsterReal* const,
                                        const AsterReal* const,
                                        const AsterInt* const,
                                        const AsterReal* const,
                                        const AsterReal* const,
                                        AsterReal* const,
                                        const AsterInt* const,
                                        AsterReal* const,
                                        const StressFreeExpansionHandler&) {
    using namespace std;
    string msg(
        "AsterUnSupportedCaseHandler::exe : "
        "we fall in a case that the aster interface "
        "is not able to handle.");
    throw(runtime_error(msg));
  }  // end of exe

}  // end of namespace aster
