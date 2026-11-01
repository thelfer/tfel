/*!
 * \file   tests/Math/tensor2.cxx
 * \brief
 * \author Thomas Helfer
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

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"

struct RightCauchyGreenTensorTest final : public tfel::tests::TestCase {
  RightCauchyGreenTensorTest()
      : tfel::tests::TestCase("TFEL/Math", "RightCauchyGreenTensorTest") {
  }  // end of RightCauchyGreenTensorTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    tensor<2, double> F(0.);
    F(0) = 1.300;
    F(1) = 0.650;
    F(2) = 0.;
    F(3) = -0.3750;
    F(4) = 0.750;
    const stensor<2, double> e = computeRightCauchyGreenTensor(F);
    const stensor<2, double> U = square_root(e);
    // C = tF.F is diagonal for this deformation gradient
    const double eps = 1.e-14;
    TFEL_TESTS_ASSERT(abs(e(0) - 2.2525) < eps);
    TFEL_TESTS_ASSERT(abs(e(1) - 0.563125) < eps);
    TFEL_TESTS_ASSERT(abs(e(2)) < eps);
    TFEL_TESTS_ASSERT(abs(e(3)) < eps);
    TFEL_TESTS_ASSERT(abs(U(0) - sqrt(2.2525)) < eps);
    TFEL_TESTS_ASSERT(abs(U(1) - sqrt(0.563125)) < eps);
    TFEL_TESTS_ASSERT(abs(U(2)) < eps);
    TFEL_TESTS_ASSERT(abs(U(3)) < eps);
    return this->result;
  }  // end of execute
};

struct RightCauchyGreenTensorTest2 final : public tfel::tests::TestCase {
  RightCauchyGreenTensorTest2()
      : tfel::tests::TestCase("TFEL/Math", "RightCauchyGreenTensorTest2") {
  }  // end of RightCauchyGreenTensorTest2
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    tensor<3, double> F(0.);
    F(0) = 1;
    F(1) = 1;
    F(2) = 1.5;
    F(4) = -0.333;
    F(3) = 0.495;
    F(5) = 0.5;
    F(6) = 0.959;
    F(7) = -0.247;
    const stensor<3, double> e = computeRightCauchyGreenTensor(F);
    const stensor<3, double> U = square_root(e);
    // C = tF.F computed by hand
    const double eps = 1.e-14;
    const double sqrt2 = sqrt(2.);
    TFEL_TESTS_ASSERT(abs(e(0) - 2.03057) < eps);
    TFEL_TESTS_ASSERT(abs(e(1) - 1.245025) < eps);
    TFEL_TESTS_ASSERT(abs(e(2) - 2.561009) < eps);
    TFEL_TESTS_ASSERT(abs(e(3) - 0.162 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(e(4) - 2.020751 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(e(5) - 0.0005 * sqrt2) < eps);
    // square root of C computed by numpy (eigen-decomposition)
    TFEL_TESTS_ASSERT(abs(U(0) - 1.1880436209666456) < eps);
    TFEL_TESTS_ASSERT(abs(U(1) - 1.1127612086738372) < eps);
    TFEL_TESTS_ASSERT(abs(U(2) - 1.3955238503015746) < eps);
    TFEL_TESTS_ASSERT(abs(U(3) - 0.078700901874544371 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(U(4) - 0.78289751738308211 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(U(5) + 0.024365149596814156 * sqrt2) < eps);
    auto s = stensor<3, double>{};
    s.copy(e.begin());
    TFEL_TESTS_ASSERT(s(5) == e(5));
    auto t = tensor<3, double>{};
    t.copy(F.begin());
    TFEL_TESTS_ASSERT(t(7) == F(7));
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(RightCauchyGreenTensorTest,
                          "RightCauchyGreenTensorTest");
TFEL_TESTS_GENERATE_PROXY(RightCauchyGreenTensorTest2,
                          "RightCauchyGreenTensorTest2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("Tensor2.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
