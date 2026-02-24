/*!
 * \file   tests/Tests/testfunctionwrapper.cxx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief a simple test of the TestFunctionWrapper class
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
#include <memory>

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
  std::unique_ptr<Test> a(new Wrapper1("test1"));
  std::unique_ptr<Test> b(new Wrapper2("test2"));
  // first test
  assert(a->name() == "test1");
  assert(a->execute().success());
  // second test
  assert(b->name() == "test2");
  assert(!b->execute().success());
  return EXIT_SUCCESS;
}  // end of main
