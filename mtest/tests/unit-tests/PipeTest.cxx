/*!
 * \file   PipeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27 nov. 2015
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
#include <iostream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MTest/PipeProfileHandler.hxx"
#include "MTest/PipeTest.hxx"

struct PipeTestUnitTest final : public tfel::tests::TestCase {
  PipeTestUnitTest()
      : tfel::tests::TestCase("Test", "PipeTestUnitTest") {
  }  // end of PipeTestUnitTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute()
 private:
  void test1() {
    mtest::PipeTest t;
    TFEL_TESTS_CHECK_THROW(t.setInnerRadius(-4.), std::runtime_error);
    t.setInnerRadius(4.e-3);
    TFEL_TESTS_CHECK_THROW(t.setInnerRadius(4.2e-3), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.setOuterRadius(-4.), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.setOuterRadius(3.2e-3), std::runtime_error);
    t.setOuterRadius(4.5e-3);
    TFEL_TESTS_CHECK_THROW(t.setOuterRadius(4.6e-3), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.setNumberOfElements(-1), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.setNumberOfElements(0), std::runtime_error);
    t.setNumberOfElements(10);
    TFEL_TESTS_CHECK_THROW(t.setNumberOfElements(10), std::runtime_error);
  }
  void test2() {
    mtest::PipeTest t;
    t.setOuterRadius(4.5e-3);
    TFEL_TESTS_CHECK_THROW(t.setInnerRadius(5.e-3), std::runtime_error);
  }
};

TFEL_TESTS_GENERATE_PROXY(PipeTestUnitTest, "PipeTestUnitTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("PipeTest.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
