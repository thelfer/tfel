/*!
 * \file  mfront/include/MFront/Aster/AsterInterfaceExceptions.hxx
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

#ifndef LIB_MFRONT_ASTER_ASTERINTERFACEEXCEPTIONS_H_
#define LIB_MFRONT_ASTER_ASTERINTERFACEEXCEPTIONS_H_

#include <string>

#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/Material/MaterialException.hxx"

#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterConfig.hxx"
#include "MFront/Aster/AsterException.hxx"

namespace aster {

  /*!
   * This structure is called when we fall in a case that the aster
   * interface is not able to handle. Normally, this case shall have
   * been handled during the code generation (see the
   * AsterInterface class).
   */
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterUnSupportedCaseHandler {
    /*!
     * \brief throw an exception
     */
    TFEL_NORETURN static void exe(const AsterReal *const,
                                  const AsterReal *const,
                                  AsterReal *const,
                                  const AsterReal *const,
                                  const AsterReal *const,
                                  const AsterReal *const,
                                  const AsterReal *const,
                                  const AsterReal *const,
                                  const AsterInt *const,
                                  const AsterReal *const,
                                  const AsterReal *const,
                                  AsterReal *const,
                                  const AsterInt *const,
                                  AsterReal *const,
                                  const StressFreeExpansionHandler &);
  };  // end of struct AsterUnSupportedCaseHandler

  /*!
   * \class  AsterInterfaceExceptions
   * \brief  Exceptions class for Aster interfaces
   * \author Helfer Thomas
   * \date   12/12/2011
   */
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInterfaceExceptions {
    /*!
     * \brief throw an AsterException. This method shall be called when
     * the number of materials properties declared by the beahviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    TFEL_NORETURN static void throwUnMatchedNumberOfMaterialProperties(
        const std::string &, const unsigned short n1, const AsterInt n2);

    /*!
     * \brief throw an AsterException. This method shall be called when
     * the number of state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    TFEL_NORETURN static void throwUnMatchedNumberOfStateVariables(
        const std::string &, const unsigned short n1, const AsterInt n2);

    /*!
     * \brief display the error message out of an AsterException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the AsterException to be treated
     */
    static void treatAsterException(const std::string &,
                                    const AsterException &);
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
     * \brief throw an AsterException if the time step is negative
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwNegativeTimeStepException(
        const std::string &);
    /*!
     * \brief throw an AsterException if the DDSOE parameter is invalid
     * \param[in] b : behaviour name
     * \param[in] v : DDSOE value
     */
    TFEL_NORETURN static void throwInvalidDDSOEException(const std::string &,
                                                         const AsterReal);
    /*!
     * \brief throw an AsterException if the prediction computation
     * failed
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwPredictionComputationFailedException(
        const std::string &);
    /*!
     * \brief throw an AsterException if the behaviour integration
     * failed
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwBehaviourIntegrationFailedException(
        const std::string &);
    /*!
     * \brief throw an AsterException if the a consistent tangent
     * operator has to been requested and that the behaviour does not
     * provide one.
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwConsistentTangentOperatorIsNotAvalaible(
        const std::string &);
    /*!
     * \brief throw an AsterException if the a prediction operator has
     * to been requested and that the behaviour does not provide one.
     * \param[in] b : behaviour name
     */
    TFEL_NORETURN static void throwPredictionOperatorIsNotAvalaible(
        const std::string &);
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

  };  // end of struct AsterInterfaceExceptions

  /*!
   * \brief an helper class used to reduce the tangent operator from
   * 3D to a lower dimension
   */
  template <unsigned short N>
  struct AsterReduceTangentOperator;

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterReduceTangentOperator<1u> {
    static void exe(AsterReal *const, const AsterReal *const);
  };

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterReduceTangentOperator<2u> {
    static void exe(AsterReal *const, const AsterReal *const);
  };

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTER_ASTERINTERFACEEXCEPTIONS_H_ */
