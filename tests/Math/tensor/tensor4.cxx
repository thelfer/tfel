/*!
 * \file   tests/Math/tensor4.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  3 dec. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/Tensor/TensorConceptIO.hxx"

struct Tensor4Test1D final : public tfel::tests::TestCase {
  Tensor4Test1D()
      : tfel::tests::TestCase("TFEL/Math", "Tensor4Test1D") {
  }  // end of Tensor4Test1D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 10 * numeric_limits<double>::epsilon();
    tensor<1u, double> F;
    F(0) = 1.23;
    F(1) = 1.58;
    F(2) = -0.8;
    tensor<1u, double> R;
    stensor<1u, double> U;
    polar_decomposition(R, U, F);
    // check that Rt is a rotation
    tensor<1u, double> Id = transpose(R) * R;
    TFEL_TESTS_ASSERT(abs(Id(0) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(1) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(1) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(R(0) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(R(1) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(R(1) - 1.) < eps);
    // check that we can get back F
    tensor<1u, double> F1 = R * U;
    TFEL_TESTS_ASSERT(abs(F(0) - F1(0)) < eps);
    TFEL_TESTS_ASSERT(abs(F(1) - F1(1)) < eps);
    TFEL_TESTS_ASSERT(abs(F(1) - F1(1)) < eps);
    TFEL_TESTS_ASSERT(abs(F(0) - U(0)) < eps);
    TFEL_TESTS_ASSERT(abs(F(1) - U(1)) < eps);
    TFEL_TESTS_ASSERT(abs(F(1) - U(1)) < eps);
    return this->result;
  }  // end of execute
};

struct Tensor4Test2D final : public tfel::tests::TestCase {
  Tensor4Test2D()
      : tfel::tests::TestCase("TFEL/Math", "Tensor4Test2D") {
  }  // end of Tensor4Test2D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 100 * numeric_limits<double>::epsilon();
    tensor<2u, double> F = tensor<2u, double>::Id();
    F(3) = -1.23;
    tensor<2u, double> R;
    stensor<2u, double> U;
    polar_decomposition(R, U, F);
    // check that Rt is a rotation
    tensor<2u, double> Id = transpose(R) * R;
    TFEL_TESTS_ASSERT(abs(Id(0) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(1) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(2) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(3)) < eps);
    TFEL_TESTS_ASSERT(abs(Id(4)) < eps);
    // check that we can get back F
    tensor<2u, double> F2 = R * U;
    TFEL_TESTS_ASSERT(abs(F(0) - F2(0)) < eps);
    TFEL_TESTS_ASSERT(abs(F(1) - F2(1)) < eps);
    TFEL_TESTS_ASSERT(abs(F(2) - F2(2)) < eps);
    TFEL_TESTS_ASSERT(abs(F(3) - F2(3)) < eps);
    TFEL_TESTS_ASSERT(abs(F(4) - F2(4)) < eps);
    return this->result;
  }  // end of execute
};

struct Tensor4Test3D final : public tfel::tests::TestCase {
  Tensor4Test3D()
      : tfel::tests::TestCase("TFEL/Math", "Tensor4Test3D") {
  }  // end of Tensor4Test3D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 100 * numeric_limits<double>::epsilon();
    tensor<3u, double> F;
    F(0) = 1.23;
    F(1) = 1.58;
    F(2) = -0.8;
    F(3) = -1.33;
    F(4) = -0.12;
    F(5) = -2.33;
    F(6) = -0.22;
    F(7) = -0.92;
    F(8) = -0.67;
    tensor<3u, double> R;
    stensor<3u, double> U;
    polar_decomposition(R, U, F);
    // check that Rt is a rotation
    tensor<3u, double> Id = transpose(R) * R;
    TFEL_TESTS_ASSERT(abs(Id(0) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(1) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(2) - 1.) < eps);
    TFEL_TESTS_ASSERT(abs(Id(3)) < eps);
    TFEL_TESTS_ASSERT(abs(Id(4)) < eps);
    TFEL_TESTS_ASSERT(abs(Id(5)) < eps);
    TFEL_TESTS_ASSERT(abs(Id(6)) < eps);
    TFEL_TESTS_ASSERT(abs(Id(7)) < eps);
    TFEL_TESTS_ASSERT(abs(Id(8)) < eps);
    // check that we can get back F
    tensor<3u, double> F2 = R * U;
    TFEL_TESTS_ASSERT(abs(F(0) - F2(0)) < eps);
    TFEL_TESTS_ASSERT(abs(F(1) - F2(1)) < eps);
    TFEL_TESTS_ASSERT(abs(F(2) - F2(2)) < eps);
    TFEL_TESTS_ASSERT(abs(F(3) - F2(3)) < eps);
    TFEL_TESTS_ASSERT(abs(F(4) - F2(4)) < eps);
    TFEL_TESTS_ASSERT(abs(F(5) - F2(5)) < eps);
    TFEL_TESTS_ASSERT(abs(F(6) - F2(6)) < eps);
    TFEL_TESTS_ASSERT(abs(F(7) - F2(7)) < eps);
    TFEL_TESTS_ASSERT(abs(F(8) - F2(8)) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(Tensor4Test1D, "Tensor4Test1D");
TFEL_TESTS_GENERATE_PROXY(Tensor4Test2D, "Tensor4Test2D");
TFEL_TESTS_GENERATE_PROXY(Tensor4Test3D, "Tensor4Test3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("Tensor4.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
