/*!
 * \file  mfront/include/MFront/Europlexus/EuroplexusInterfaceExceptions.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_EPX_EPXINTERFACEEXCEPTIONS_H_
#define LIB_MFRONT_EPX_EPXINTERFACEEXCEPTIONS_H_

#include <string>

#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/Material/MaterialException.hxx"

#include "MFront/Europlexus/Europlexus.hxx"
#include "MFront/Europlexus/EuroplexusConfig.hxx"
#include "MFront/Europlexus/EuroplexusException.hxx"

namespace epx {

  /*!
   * This structure is called when we fall in a case that the europlexus
   * interface is not able to handle. Normally, this case shall have
   * been handled during the code generation (see the
   * EuroplexusInterface class).
   */
  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusUnSupportedCaseHandler {
    /*!
     * \brief throw an exception
     */
    TFEL_NORETURN static void exe(const EuroplexusReal *const,
                                  const EuroplexusReal *const,
                                  const EuroplexusReal *const,
                                  const EuroplexusReal *const,
                                  const EuroplexusReal *const,
                                  const EuroplexusReal *const,
                                  const EuroplexusInt *const,
                                  const EuroplexusReal *const,
                                  const EuroplexusReal *const,
                                  EuroplexusReal *const,
                                  const EuroplexusInt *const,
                                  EuroplexusReal *const,
                                  const StressFreeExpansionHandler &);
  };  // end of struct EuroplexusUnSupportedCaseHandler

  /*!
   * \class  EuroplexusInterfaceExceptions
   * \brief  Exceptions class for Europlexus interfaces
   * \author Helfer Thomas
   * \date   12/12/2011
   */
  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusInterfaceExceptions {
    /*!
     * \brief throw an EuroplexusException. This method shall be called when
     * the number of materials properties declared by the beahviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    TFEL_NORETURN static void throwUnMatchedNumberOfMaterialProperties(
        const std::string &, const unsigned short n1, const EuroplexusInt n2);
    /*!
     * \brief throw an EuroplexusException. This method shall be called when
     * the number of state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    TFEL_NORETURN static void throwUnMatchedNumberOfStateVariables(
        const std::string &, const unsigned short n1, const EuroplexusInt n2);
    /*!
     * \brief throw an EuroplexusException. This method shall be called when
     * the number of external state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of external state variables declared by the
     * behaviour \param[in] n2 : number of external state variables declared by
     * the interface
     */
    TFEL_NORETURN static void throwUnMatchedNumberOfExternalStateVariables(
        const std::string &, const unsigned short n1, const EuroplexusInt n2);
    /*!
     * \brief display the error message out of an EuroplexusException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the EuroplexusException to be treated
     */
    static void treatEuroplexusException(const std::string &,
                                         const EuroplexusException &);
    /*!
     * \brief display the error message out of a material exception to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the material exception to be treated
     */
    static void treatMaterialException(
        const std::string &, const tfel::material::MaterialException &);
    /*!
     * \brief display the error message out of a generic tfel
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void treatTFELException(const std::string &,
                                   const tfel::exception::TFELException &);
    /*!
     * \brief display the error message out of a generic standard
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void treatStandardException(const std::string &,
                                       const std::exception &);
    /*!
     * \brief display the error message when an unknown exception is caught
     * \param[in] b : behaviour name
     */
    static void treatUnknownException(const std::string &);
    /*!
     * \brief throw an EuroplexusException if the time step is negative
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwNegativeTimeStepException(
        const std::string &);
    /*!
     * \brief throw an AbaqusException if the prediction computation
     * failed
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwPredictionComputationFailedException(
        const std::string &);
    /*!
     * \brief throw an EuroplexusException if the DDSDDE parameter is invalid
     * \param[in] b : behaviour name
     * \param[in] v : DDSDDE value
     */
    TFEL_NORETURN static void throwInvalidDDSDDEValueException(
        const std::string &, const EuroplexusReal);
    /*!
     * \brief throw an AbaqusException if the a consistent tangent
     * operator has to been requested and that the behaviour does not
     * provide one.
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwConsistentTangentOperatorIsNotAvalaible(
        const std::string &);
    /*!
     * \brief throw an AbaqusException if the a prediction operator has
     * to been requested and that the behaviour does not provide one.
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwPredictionOperatorIsNotAvalaible(
        const std::string &);
    /*!
     * \brief throw an EuroplexusException if the time step scaling factor
     * is greater or egal to one on failure
     * \param[in] b: behaviour name
     * \param[in] v: time step scaling factor value
     */
    TFEL_NORETURN static void throwInvalidTimeStepScalingFactorOnFailure(
        const std::string &, const EuroplexusReal);
    /*!
     * \brief display an error message if the behaviour shall handle
     * stress free expansion and that the umat interface can't
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwUnsupportedStressFreeExpansionException(
        const std::string &);
    /*!
     * \brief display an error message if the value of the NTENS
     * parameter is not valid
     */
    static void displayUnsupportedHypothesisMessage();

  };  // end of struct EuroplexusInterfaceExceptions

}  // end of namespace epx

#endif /* LIB_MFRONT_EPX_EPXINTERFACEEXCEPTIONS_H_ */
