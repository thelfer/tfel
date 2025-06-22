/*!
 * \file   tests/Tests/multipletestoutputs.cxx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief a simple test of the MultipleTestOutputs class
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

#include "TFEL/Tests/TestSuite.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"
#include "TFEL/Tests/StdStreamTestOutput.hxx"
#include "TFEL/Tests/MultipleTestOutputs.hxx"

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
  auto a = std::make_shared<Wrapper1>("test1");
  auto b = std::make_shared<Wrapper2>("test2");
  MultipleTestOutputs output;
  output.addTestOutput(
      std::make_shared<StdStreamTestOutput>("multipletestoutputs.txt"));
  TestSuite suite1("suite1");
  suite1.add(a);
  suite1.add(b);
  assert(!suite1.execute(output).success());
  return EXIT_SUCCESS;
}  // end of main
