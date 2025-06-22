/*!
 * \file   tests/Tests/testcase.cxx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief a simple test of the TestCase class
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

#include <string>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct MyTest final : public tfel::tests::TestCase {
  MyTest() : TestCase("TFEL/Tests", "MyTest") {}  // end of MyTest
  tfel::tests::TestResult execute() override {
    TFEL_TESTS_ASSERT(true);
    TFEL_TESTS_ASSERT(1 != 2);
    TFEL_TESTS_CALLMETHOD(test);
    TFEL_TESTS_CHECK_THROW(tfel::raise("test"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(tfel::raise("test"), std::exception);
    TFEL_TESTS_CHECK_EQUAL(std::string("test"), "test");
    return this->result;
  }  // end of execute()
 private:
  bool test() const { return true; }
};  // end of struct MyTest

TFEL_TESTS_GENERATE_PROXY(MyTest, "TestSuite1");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  std::ofstream f("testcase.txt");
  tfel::raise_if(!f, "can't open file 'testproxy.txt'");
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(f, false);
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
