/*!
 * \file   tests/Tests/testmanager.cxx
 * \author Thomas Helfer
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
#include <fstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestManager.hxx"
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
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<test1>;
  using Wrapper2 = TestFunctionWrapper<test2>;
  auto& m = TestManager::getTestManager();
  auto a = std::make_shared<Wrapper1>("test1");
  auto b = std::make_shared<Wrapper2>("test2");
  std::ofstream f("testmanager-3.txt");
  tfel::raise_if(!f, "can't open file 'testmanager-3.txt'");
  m.addTest("suite1", a);
  m.addTest("suite1", b);
  m.addTest("suite2", a);
  m.addTest("suite3", a);
  m.addTestOutput("suite1", "testmanager-1.txt");
  m.addTestOutput("testmanager-2.txt");
  m.addTestOutput("suite3", f, false);
  // tests are meant to fail
  return m.execute().success() ? EXIT_FAILURE : EXIT_SUCCESS;
}  // end of main
