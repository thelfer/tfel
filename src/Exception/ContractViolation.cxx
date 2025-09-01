/*!
 * \file  src/Exception/ContractViolation.cxx
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

#include <cstdlib>
#include <iostream>
#include "TFEL/ContractViolation.hxx"

namespace tfel {

  void reportContractViolation(const char* const msg) {
    std::cerr << msg << '\n';
    std::abort();
  }

}  // namespace tfel
