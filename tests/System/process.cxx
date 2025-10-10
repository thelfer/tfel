/*!
 * \file   tests/System/process.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   12 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>

#include "TFEL/System/ProcessManager.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::system;
  ProcessManager m;
  m.createProcess("./process_test_target");
  return EXIT_SUCCESS;
}
