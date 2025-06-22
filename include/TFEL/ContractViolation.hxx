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
  [[noreturn]] TFEL_HOST TFELEXCEPTION_VISIBILITY_EXPORT void
  reportContractViolation(const char* const msg);
#else  /* TFEL_NO_REPORT_CONTRACT_VIOLATION */
  TFEL_HOST_DEVICE constexpr void reportContractViolation(const char* const) {
  }  // end of reportContractViolation
#endif /* TFEL_NO_REPORT_CONTRACT_VIOLATION */
#else  /* SYCL_LANGUAGE_VERSION  */
  TFEL_HOST_DEVICE constexpr void reportContractViolation(const char* const) {
  }  // end of reportContractViolation
#endif /* SYCL_LANGUAGE_VERSION  */

}  // namespace tfel

#endif /* LIB_TFEL_CONTRACTVIOLATION_HXX */
