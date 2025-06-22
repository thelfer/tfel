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

#include <iostream>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  /*!
   * \brief report a contract violation
   * \param[in] msg: message
   */
  [[noreturn]] TFEL_VISIBILITY_EXPORT void reportContractViolation(
      const char* const msg);

}  // namespace tfel

#endif /* LIB_TFEL_CONTRACTVIOLATION_HXX */
