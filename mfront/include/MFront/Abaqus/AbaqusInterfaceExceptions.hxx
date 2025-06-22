/*!
 * \file  mfront/include/MFront/Abaqus/AbaqusInterfaceExceptions.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSINTERFACEEXCEPTIONS_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSINTERFACEEXCEPTIONS_HXX

#include <string>

#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/Material/MaterialException.hxx"

#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusConfig.hxx"
#include "MFront/Abaqus/AbaqusException.hxx"

namespace abaqus {

  /*!
   * This structure is called when we fall in a case that the abaqus
   * interface is not able to handle. Normally, this case shall have
   * been handled during the code generation (see the
   * AbaqusInterface class).
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusUnSupportedCaseHandler {
    /*!
     * \brief throw an exception
     */
    TFEL_NORETURN static void exe(
        const AbaqusReal *const,
        const AbaqusReal *const,
        AbaqusReal *const,
        const AbaqusReal *const,
        const AbaqusReal *const,
        const AbaqusReal *const,
        const AbaqusReal *const,
        const AbaqusReal *const,
        const AbaqusInt *const,
        const AbaqusReal *const,
        const AbaqusReal *const,
        AbaqusReal *const,
        const AbaqusInt *const,
        AbaqusReal *const,
        const StressFreeExpansionHandler<AbaqusReal> &);
  };  // end of struct AbaqusUnSupportedCaseHandler

  /*!
   * \class  AbaqusInterfaceExceptions
   * \brief  Exceptions class for Abaqus interfaces
   * \author Thomas Helfer
   * \date   12/12/2011
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusInterfaceExceptions {
    /*!
     * \brief throw an AbaqusException. This method shall be called
     * when the number of components of symmetric tensors for the
     * current hypothesis does not match the the number of components
     * of symmetric tensors given by `Abaqus/Standard`
     * \param[in] b  : behaviour name
     * \param[in] n1 : size of symmetric tensors for the current hypothesis
     * \param[in] n2 : size of symmetric tensors declared by the
     * `Abaqus/Standard`
     */
    TFEL_NORETURN static void throwInvalidTensorSize(const std::string &,
                                                     const unsigned short n1,
                                                     const AbaqusInt n2);
    /*!
     * \brief throw an AbaqusException. This method shall be called when
     * the number of materials properties declared by the behaviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    TFEL_NORETURN static void throwUnMatchedNumberOfMaterialProperties(
        const std::string &, const unsigned short n1, const AbaqusInt n2);
    /*!
     * \brief throw an AbaqusException. This method shall be called when
     * the number of state variables declared by the behaviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    TFEL_NORETURN static void throwUnMatchedNumberOfStateVariables(
        const std::string &, const unsigned short n1, const AbaqusInt n2);

    /*!
     * \brief display the error message out of an AbaqusException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the AbaqusException to be treated
     */
    static void treatAbaqusException(const std::string &,
                                     const AbaqusException &);
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
     * \brief throw an AbaqusException if the time step is negative
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

  };  // end of struct AbaqusInterfaceExceptions

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSINTERFACEEXCEPTIONS_HXX */
