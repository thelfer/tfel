/*!
 * \file  mfront/include/MFront/DianaFEA/DianaFEAInterfaceExceptions.hxx
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

#ifndef LIB_MFRONT_DIANAFEA_DIANAFEAINTERFACEEXCEPTIONS_HXX
#define LIB_MFRONT_DIANAFEA_DIANAFEAINTERFACEEXCEPTIONS_HXX

#include <string>

#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/Material/MaterialException.hxx"

#include "MFront/DianaFEA/DianaFEA.hxx"
#include "MFront/DianaFEA/DianaFEAConfig.hxx"
#include "MFront/DianaFEA/DianaFEAException.hxx"

namespace dianafea {

  /*!
   * This structure is called when we fall in a case that the dianafea
   * interface is not able to handle. Normally, this case shall have
   * been handled during the code generation (see the
   * DianaFEAInterface class).
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAUnSupportedCaseHandler {
    /*!
     * \brief throw an exception
     */
    [[noreturn]] static void exe(
        const DianaFEAReal *const,
        const DianaFEAReal *const,
        DianaFEAReal *const,
        const DianaFEAReal *const,
        const DianaFEAReal *const,
        const DianaFEAReal *const,
        const DianaFEAReal *const,
        const DianaFEAReal *const,
        const DianaFEAInt *const,
        const DianaFEAReal *const,
        const DianaFEAReal *const,
        DianaFEAReal *const,
        const DianaFEAInt *const,
        DianaFEAReal *const,
        const StressFreeExpansionHandler<DianaFEAReal> &);
  };  // end of struct DianaFEAUnSupportedCaseHandler

  /*!
   * \class  DianaFEAInterfaceExceptions
   * \brief  Exceptions class for DianaFEA interfaces
   * \author Thomas Helfer
   * \date   12/12/2011
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAInterfaceExceptions {
    /*!
     * \brief throw an DianaFEAException. This method shall be called when
     * the number of materials properties declared by the beahviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    [[noreturn]] static void throwUnMatchedNumberOfMaterialProperties(
        const std::string &, const unsigned short n1, const DianaFEAInt n2);

    /*!
     * \brief throw an DianaFEAException. This method shall be called when
     * the number of state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    [[noreturn]] static void throwUnMatchedNumberOfStateVariables(
        const std::string &, const unsigned short n1, const DianaFEAInt n2);

    /*!
     * \brief display the error message out of an DianaFEAException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the DianaFEAException to be treated
     */
    static void treatDianaFEAException(const std::string &,
                                       const DianaFEAException &);
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
     * \brief throw an DianaFEAException if the time step is negative
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwNegativeTimeStepException(
        const std::string &);
    /*!
     * \brief throw an DianaFEAException if the prediction computation
     * failed
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwPredictionComputationFailedException(
        const std::string &);
    /*!
     * \brief throw an DianaFEAException if the a consistent tangent
     * operator has to been requested and that the behaviour does not
     * provide one.
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwConsistentTangentOperatorIsNotAvalaible(
        const std::string &);
    /*!
     * \brief throw an DianaFEAException if the a prediction operator has
     * to been requested and that the behaviour does not provide one.
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwPredictionOperatorIsNotAvalaible(
        const std::string &);
    /*!
     * \brief display an error message if the behaviour shall handle
     * stress free expansion and that the umat interface can't
     * \param[in] b : behaviour name
     */
    [[noreturn]] static void throwUnsupportedStressFreeExpansionException(
        const std::string &);
    /*!
     * \brief display an error message if the value of the NTENS
     * parameter is not valid
     */
    static void displayUnsupportedHypothesisMessage();

  };  // end of struct DianaFEAInterfaceExceptions

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEA_DIANAFEAINTERFACEEXCEPTIONS_HXX */
