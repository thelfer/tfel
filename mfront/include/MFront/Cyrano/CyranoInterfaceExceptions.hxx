/*!
 * \file  mfront/include/MFront/Cyrano/CyranoInterfaceExceptions.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANO_CYRANOINTERFACEEXCEPTIONS_HXX
#define LIB_MFRONT_CYRANO_CYRANOINTERFACEEXCEPTIONS_HXX

#include <string>
#include <string_view>

#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/Material/MaterialException.hxx"

#include "MFront/Cyrano/Cyrano.hxx"
#include "MFront/Cyrano/CyranoTraits.hxx"
#include "MFront/Cyrano/CyranoConfig.hxx"
#include "MFront/Cyrano/CyranoException.hxx"

namespace cyrano {

  /*!
   * This structure is called when we fall in a case that the cyrano
   * interface is not able to handle. Normally, this case shall have
   * been handled during the code generation (see the
   * CyranoInterface class).
   */
  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoUnSupportedCaseHandler {
    /*!
     * \brief throw an exception
     */
    [[noreturn]] static void exe(const CyranoReal *const,
                                 const CyranoReal *const,
                                 const CyranoReal *const,
                                 const CyranoReal *const,
                                 const CyranoReal *const,
                                 const CyranoReal *const,
                                 const CyranoReal *const,
                                 const CyranoReal *const,
                                 const CyranoInt *const,
                                 const CyranoReal *const,
                                 const CyranoReal *const,
                                 CyranoReal *const,
                                 const CyranoInt *const,
                                 CyranoReal *const);  // end of exe
  };  // end of struct CyranoUnSupportedCaseHandler

  /*!
   * \class  CyranoInterfaceExceptions
   * \brief  Base class for cyrano interfaces
   * \author Thomas Helfer
   * \date   12/12/2011
   */
  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoInterfaceExceptions {
    /*!
     * \brief throw a CyranoException. This method shall be called when
     * the number of materials properties declared by the beahviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    [[noreturn]] static void throwUnMatchedNumberOfMaterialProperties(
        const std::string_view, const unsigned short n1, const CyranoInt n2);

    /*!
     * \brief throw a CyranoException. This method shall be called when
     * the number of state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    [[noreturn]] static void throwUnMatchedNumberOfStateVariables(
        const std::string_view, const unsigned short n1, const CyranoInt n2);

    /*!
     * \brief display the error message out of a CyranoException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the CyranoException to be treated
     */
    static void treatCyranoException(const std::string_view,
                                     const CyranoException &);

    /*!
     * \brief display the error message out of a material exception to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the material exception to be treated
     */
    static void treatMaterialException(
        const std::string_view, const tfel::material::MaterialException &);

    /*!
     * \brief display the error message out of a generic tfel
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void treatTFELException(const std::string_view,
                                   const tfel::exception::TFELException &);

    /*!
     * \brief display the error message out of a generic standard
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void treatStandardException(const std::string_view,
                                       const std::exception &);

    /*!
     * \brief display the error message when an unknown exception is caught
     * \param[in] b : behaviour name
     */
    static void treatUnknownException(const std::string_view);

    /*!
     * \brief throw a CyranoException if the time step is negative
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwNegativeTimeStepException(
        const std::string_view);

    /*!
     * \brief throw a CyranoException if the behaviour integration
     * failed
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwBehaviourIntegrationFailedException(
        const std::string_view);

    /*!
     * \brief throw a CyranoException if the maximum number of sub
     * stepping has been reached
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwMaximumNumberOfSubSteppingReachedException(
        const std::string_view);

    /*!
     * \brief throw a CyranoException if the maximum number of sub
     * stepping has been reached
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void
    throwPlaneStressMaximumNumberOfIterationsReachedException(
        const std::string_view);
    /*!
     * \brief throw a CyranoException if the DDSOE parameter is invalid
     * \param[in] b : behaviour name
     * \param[in] v : DDSOE value
     */
    [[noreturn]] static void throwInvalidDDSOEException(const std::string_view,
                                                        const CyranoReal);
    /*!
     * \brief throw a CyranoException if the behaviour initialization failed
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwBehaviourInitializationFailedException(
        const std::string_view);
    /*!
     * \brief throw a CyranoException if the prediction computation
     * failed
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwPredictionComputationFailedException(
        const std::string_view);
    /*!
     * \brief throw a CyranoException if the a consistent tangent
     * operator has to been requested and that the behaviour does not
     * provide one.
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwConsistentTangentOperatorIsNotAvalaible(
        const std::string_view);
    /*!
     * \brief throw a CyranoException if the a prediction operator has
     * to been requested and that the behaviour does not provide one.
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwPredictionOperatorIsNotAvalaible(
        const std::string_view);
    /*!
     * \brief display an error message if the behaviour shall handle
     * stress free expansion and that the cyrano interface can't
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwUnsupportedStressFreeExpansionException(
        const std::string_view);
    /*!
     * \brief display an error message if the thermal expansion is not
     * null
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwThermalExpansionCoefficientShallBeNull(
        const std::string_view);
    /*!
     * \brief throw an error message if the value of the NTENS
     * parameter is not valid
     * \param[in] NTENS : NTENS value
     * \param[in] s     : expected size
     */
    static void checkNTENSValue(const CyranoInt, const unsigned short);
    /*!
     * \brief throw an error message if the behaviour type is not supported for
     * the given hypothesis \param[in] H    : hypothesis
     */
    [[noreturn]] static void throwInvalidBehaviourTypeAndModellingHypothesis(
        const std::string_view);
    /*!
     * \brief display an error message if the value of the NDI
     * parameter is not valid
     */
    static void displayInvalidModellingHypothesisErrorMessage();

  };  // end of struct CyranoInterfaceExceptions

}  // end of namespace cyrano

#endif /* LIB_MFRONT_CYRANO_CYRANOINTERFACEEXCEPTIONS_HXX */
