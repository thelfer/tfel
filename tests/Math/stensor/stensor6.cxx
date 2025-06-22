/*!
 * \file   tests/Math/stensor6.cxx
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

struct STensorTest6 final : public tfel::tests::TestCase {
  STensorTest6()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest6") {
  }  // end of STensorTest6
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    stensor<3, double> s;
    stensor<2, double> s2;
    stensor<3, double> s3;
    stensor<2, double> s4;
    const double sqrt2 = sqrt(2.);
    s(0) = 2.3;
    s(1) = 4.7;
    s(2) = 3.1;
    s(3) = 6.2 * sqrt2;
    s(4) = 1.83 * sqrt2;
    s(5) = 0.32 * sqrt2;
    s2(0) = 2.3;
    s2(1) = 4.7;
    s2(2) = 3.1;
    s2(3) = 6.2 * sqrt(2.);
    s3 = invert(s);
    s4 = invert(s2);
    TFEL_TESTS_ASSERT(std::abs(det(s) + 94.36691) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s3(0) + 0.15331221505504) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s3(1) + 0.040068070470889) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s3(2) - 0.29279331070605) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s3(3) - 0.19746752330875 * sqrt2) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s3(4) - 0.070119918094171 * sqrt2) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s3(5) + 0.11243347906591 * sqrt2) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(det(s2) + 85.653) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s4(0) + 0.17010495837857) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s4(1) + 0.083242851972494) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s4(2) - 0.32258064516129) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s4(3) - 0.22439377488237 * sqrt2) < 1.e-13);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTest6, "STensorTest6");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("stensor6.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
