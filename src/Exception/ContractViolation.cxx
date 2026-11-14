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

#include <atomic>
#include <cstdlib>
#include <iostream>
#include "TFEL/ContractViolation.hxx"

namespace tfel {

  //! \brief function called by `reportContractViolation`, if any
  static std::atomic<ContractViolationHandler> contract_violation_handler{
      nullptr};

  ContractViolationHandler setContractViolationHandler(
      const ContractViolationHandler h) {
    return contract_violation_handler.exchange(h);
  }  // end of setContractViolationHandler

  void reportContractViolation(const char* const msg) {
    const auto h = contract_violation_handler.load();
    if (h != nullptr) {
      h(msg);
    }
    std::cerr << msg << '\n';
    std::abort();
  }

}  // namespace tfel
