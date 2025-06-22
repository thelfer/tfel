/*!
 * \file   tests/Math/stensor5.cxx
 * \brief
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <cassert>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/stensor.hxx"

struct STensorTest5 final : public tfel::tests::TestCase {
  STensorTest5()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest5") {
  }  // end of STensorTest5
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    stensor<1, int> v1;
    stensor<1, double> v2;
    stensor<1, double> v3(0.);
    TFEL_TESTS_ASSERT(std::abs(v3(0)) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(v3(1)) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(v3(2)) < 1.e-14);
    v1(0) = 12;
    v1(1) = 13;
    v1(2) = 14;
    v2 = v1;
    TFEL_TESTS_ASSERT(std::abs(v2(0) - 12.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(v2(1) - 13.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(v2(2) - 14.) < 1.e-14);
    v2(0) = 7.f;
    v2(1) = 4.23f;
    v3 = 0.5 * v1 + v2;
    TFEL_TESTS_ASSERT(std::abs(v3(0) - 13.) < 1.e-7);
    TFEL_TESTS_ASSERT(std::abs(v3(1) - 10.73) < 1.e-7);
    TFEL_TESTS_ASSERT(std::abs(v3(2) - 21.) < 1.e-7);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTest5, "STensorTest5");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("stensor5.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
