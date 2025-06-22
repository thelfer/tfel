/*!
 * \file   tests/Tests/testproxy2.cxx
 * \author Helfer Thomas
 * \date   11 Apr 10
 * \brief a simple test of the TestSuite class
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <cassert>
#include <stdexcept>

#include <fstream>

#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"

TFEL_TESTS_FUNCTION(test1, "TFEL/Tests", "suite1") { return true; }

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  ofstream f("testproxy2.txt");
  if (!f) {
    string msg("can't open file 'testproxy2.txt'");
    throw(runtime_error(msg));
  }
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(f, false);
  TestResult r = manager.execute();
  assert(r.success());
  return EXIT_SUCCESS;
}  // end of main
