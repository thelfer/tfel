/*!
 * \file   tests/Math/tensor3.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  1 sept. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tensor.hxx"

struct Tensor3Test final : public tfel::tests::TestCase {
  Tensor3Test()
      : tfel::tests::TestCase("TFEL/Math", "Tensor3Test") {
  }  // end of Tensor3Test
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 10 * numeric_limits<double>::epsilon();
    tensor<3, double> t;
    t(0) = 0;
    t(3) = 1;
    t(5) = 0;
    t(4) = 3;
    t(1) = 2;
    t(7) = 3;
    t(6) = 4;
    t(8) = 5;
    t(2) = 6;
    tensor<3, double> t2 = invert(t);
    TFEL_TESTS_ASSERT(abs(det(t) + 6) < eps);
    TFEL_TESTS_ASSERT(abs(t2(0) - 0.5) < eps);
    TFEL_TESTS_ASSERT(abs(t2(1)) < eps);
    TFEL_TESTS_ASSERT(abs(t2(2) - 0.5) < eps);
    TFEL_TESTS_ASSERT(abs(t2(3) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(t2(4) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(t2(5) + 0.5) < eps);
    TFEL_TESTS_ASSERT(abs(t2(6) + 7. / 6.) < eps);
    TFEL_TESTS_ASSERT(abs(t2(7)) < eps);
    TFEL_TESTS_ASSERT(abs(t2(8) + 2. / 3.) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(Tensor3Test, "Tensor3Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("Tensor3.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
