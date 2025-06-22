/*!
 * \file   tests/Tests/testmanager.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
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

#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"

#if !(defined _MSC_VER)
#define TFEL_TESTS_STATIC static
#else
#define TFEL_TESTS_STATIC
#endif

TFEL_TESTS_STATIC bool test1() { return true; }

TFEL_TESTS_STATIC bool test2() { return false; }

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<test1>;
  using Wrapper2 = TestFunctionWrapper<test2>;
  auto& manager = TestManager::getTestManager();
  auto a = std::make_shared<Wrapper1>("test1");
  auto b = std::make_shared<Wrapper2>("test2");
  ofstream f("testmanager-3.txt");
  if (!f) {
    throw(runtime_error("can't open file 'testmanager-3.txt'"));
  }
  manager.addTest("suite1", a);
  manager.addTest("suite1", b);
  manager.addTest("suite2", a);
  manager.addTest("suite3", a);
  manager.addTestOutput("suite1", "testmanager-1.txt");
  manager.addTestOutput("testmanager-2.txt");
  manager.addTestOutput("suite3", f, false);
  auto r = manager.execute();
  assert(!r.success());
  return EXIT_SUCCESS;
}  // end of main
