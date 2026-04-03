/*!
 * \file   mfront/src/AsterInterfaceExceptions.cxx
 * \brief  This file implements the AsterInterfaceExceptions class.
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
#include "MFront/Aster/AsterInterfaceExceptions.hxx"

namespace aster {

  void AsterInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(
      const std::string_view b, const unsigned short n1, const AsterInt n2) {
    std::ostringstream msg;
    msg << "AsterInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties "
           ": "
        << "the number of material properties does not match. The behaviour '"
        << b << "' requires " << n1 << " material properties, and " << n2
        << " material properties were declared";
    tfel::raise<AsterException>(msg.str());
  }  // end of throwUnMatchedNumberOfMaterialProperties

  void AsterInterfaceExceptions::throwUnMatchedNumberOfStateVariables(
      const std::string_view b, const unsigned short n1, const AsterInt n2) {
    std::ostringstream msg;
    msg << "AsterInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
        << "the number of internal state variables does not match. The "
           "behaviour '"
        << b << "' requires " << n1 << " state variables, and " << n2
        << " state variables were declared";
    tfel::raise<AsterException>(msg.str());
  }  // end of throwUnMatchedNumberOfStateVariables

  void AsterInterfaceExceptions::treatAsterException(const std::string_view b,
                                                     const AsterException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an AsterException : " << e.what() << std::endl;
  }  // end of AsterInterfaceExceptions::treatAsterException

  void AsterInterfaceExceptions::treatMaterialException(
      const std::string_view b, const tfel::material::MaterialException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown an MaterialException : " << e.what()
              << std::endl;
  }  // end of treatMaterialException

  void AsterInterfaceExceptions::treatTFELException(
      const std::string_view b, const tfel::exception::TFELException& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic tfel exception : " << e.what()
              << std::endl;
  }  // end of treatTFELException

  void AsterInterfaceExceptions::treatStandardException(
      const std::string_view b, const std::exception& e) {
    std::cout << "The behaviour '" << b
              << "' has thrown a generic standard exception : " << e.what()
              << std::endl;
  }  // end of treatStandardException

  void AsterInterfaceExceptions::treatUnknownException(
      const std::string_view b) {
    std::cout << "The behaviour '" << b << "' has thrown an unknown exception"
              << std::endl;
  }  // end of treatUnknownException

  void AsterInterfaceExceptions::throwNegativeTimeStepException(
      const std::string_view b) {
    tfel::raise(
        "AsterInterfaceExceptions::throwNegativeTimeStepException: "
        "negative time step detected for behaviour '" +
        std::string{b} + "'");
  }  // end of AsterInterfaceExceptions::throwNegativeTimeStepException

  void AsterInterfaceExceptions::throwInvalidDDSOEException(
      const std::string_view b, const AsterReal v) {
    std::ostringstream msg;
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
    tfel::raise(msg.str());
  }  // end of AsterInterfaceExceptions::throwInvalidDDSOEException

  void AsterInterfaceExceptions::throwBehaviourInitializationFailedException(
      const std::string_view b) {
    tfel::raise(
        "AsterInterfaceExceptions::"
        "throwBehaviourInitializationFailedException: "
        "initialization of behaviour '" +
        std::string{b} + "' failed");
  }  // end of throwBehaviourInitializationFailedException

  void AsterInterfaceExceptions::throwBehaviourIntegrationFailedException(
      const std::string_view b) {
    tfel::raise(
        "AsterInterfaceExceptions::throwBehaviourIntegrationFailedException: "
        "integration failed for behaviour '" +
        std::string{b} + "'");
  }  // end of throwBehaviourIntegrationFailedException

  void AsterInterfaceExceptions::throwPredictionComputationFailedException(
      const std::string_view b) {
    tfel::raise(
        "AsterInterfaceExceptions::throwPredictionComputationFailedException: "
        "prediction computation failed for behaviour '" +
        std::string{b} + "'");
  }

  void AsterInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(
      const std::string_view b) {
    tfel::raise(
        "AsterInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
        "behaviour '" +
        std::string{b} + "' can't compute a prediction operator");
  }  // end of throwBehaviourIntegrationFailedException

  void AsterInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(
      const std::string_view b) {
    tfel::raise(
        "AsterInterfaceExceptions::"
        "throwConsistentTangentOperatorIsNotAvalaible: "
        "behaviour '" +
        std::string{b} + "' can't compute a consistent tangent operator");
  }  // end of throwBehaviourIntegrationFailedException

  void AsterInterfaceExceptions::throwUnsupportedStressFreeExpansionException(
      const std::string_view b) {
    tfel::raise(
        "AsterInterfaceExceptions::"
        "throwUnsupportedStressFreeExpansionException: "
        "behaviour '" +
        std::string{b} +
        "' can handle stress-free expansion but the Aster interface can't");
  }  // end of throwUnsupportedStressFreeExpansionException

  void AsterInterfaceExceptions::displayUnsupportedHypothesisMessage() {
    std::cout
        << "AsterInterfaceExceptions::displayUnsupportedHypothesisMessage : "
        << "unsupported hypothesis" << std::endl;
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
    tfel::raise(
        "AsterUnSupportedCaseHandler::exe : "
        "we fall in a case that the aster interface "
        "is not able to handle.");
  }  // end of exe

}  // end of namespace aster
