/*!
 * \file  include/TFEL/ContractViolation.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CONTRACTVIOLATION_HXX
#define LIB_TFEL_CONTRACTVIOLATION_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

#ifndef SYCL_LANGUAGE_VERSION
  /*!
   * \brief report a contract violation
   * \param[in] msg: message
   */
#ifndef TFEL_NO_REPORT_CONTRACT_VIOLATION
  [[noreturn]] TFELEXCEPTION_VISIBILITY_EXPORT TFEL_HOST void
  reportContractViolation(const char* const);
#else  /* TFEL_NO_REPORT_CONTRACT_VIOLATION */
  TFEL_HOST_DEVICE constexpr void reportContractViolation(const char* const) {
  }  // end of reportContractViolation
#endif /* TFEL_NO_REPORT_CONTRACT_VIOLATION */
#else  /* SYCL_LANGUAGE_VERSION  */
  TFEL_HOST_DEVICE constexpr void reportContractViolation(const char* const) {
  }  // end of reportContractViolation
#endif /* SYCL_LANGUAGE_VERSION  */

  //! \brief type of the functions called on a contract violation
  using ContractViolationHandler = void (*)(const char* const);
  /*!
   * \brief set the function called by `reportContractViolation` before
   * aborting the program. This function is meant to throw an exception, for
   * instance in the python bindings. If it returns, the program is aborted.
   * \return the previous handler
   * \param[in] h: new handler (`nullptr` restores the default behaviour)
   */
  TFELEXCEPTION_VISIBILITY_EXPORT TFEL_HOST ContractViolationHandler
  setContractViolationHandler(const ContractViolationHandler);

}  // namespace tfel

#endif /* LIB_TFEL_CONTRACTVIOLATION_HXX */
