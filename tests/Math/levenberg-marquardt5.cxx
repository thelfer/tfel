/*!
 * \file   tests/Math/levenberg-marquardt5.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02/07/2021
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
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/TinyLevenbergMarquardtSolver.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"

struct LevenbergMarquardtSolver
    : public tfel::math::
          TinyLevenbergMarquardtSolver<2u, double, LevenbergMarquardtSolver> {
  LevenbergMarquardtSolver() {
    this->zeros = {0., 0.};
    this->epsilon = 1.e-14;
    this->iterMax = 50;
    this->levmar_mu0 = 1.e-6;
    this->levmar_p0 = 1.e-4;
    this->levmar_p1 = 0.25;
    this->levmar_p2 = 0.75;
    this->levmar_m = 1.e-8;
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros; }

  //
  bool computeResidual() noexcept {
    constexpr double a = 1.;
    constexpr double b = 10.;
    auto& f = this->fzeros;
    auto& x = this->zeros;
    auto& J = this->jacobian;
    f(0) = a * (1 - x(0));
    f(1) = b * (x(1) - x(0) * x(0));
    J(0, 0) = -a;
    J(0, 1) = 0.;
    J(1, 0) = -2 * b * x(0);
    J(1, 1) = b;
    return true;
  }  // end of computeResidual

};  // end of struct LevenbergMarquardtSolver

struct LevenbergMarquardtSolver2
    : public tfel::math::
          TinyLevenbergMarquardtSolver<1u, double, LevenbergMarquardtSolver2> {
  LevenbergMarquardtSolver2() {
    this->zeros = {10};
    this->epsilon = 1.e-14;
    this->iterMax = 10;
    this->levmar_mu0 = 1.e-6;
    this->levmar_p0 = 1.e-4;
    this->levmar_p1 = 0.25;
    this->levmar_p2 = 0.75;
    this->levmar_m = 1.e-8;
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros(0); }

  bool computeResidual() noexcept {
    auto& f = this->fzeros(0);
    auto& x = this->zeros(0);
    auto& J = this->jacobian(0, 0);
    f = x * x - 2.;
    J = 2 * x;
    return true;
  }  // end of computFdF
};   // end of struct LevenbergMarquardtSolver2

struct LevenbergMarquardtSolver3
    : public tfel::math::
          TinyLevenbergMarquardtSolver<2u, double, LevenbergMarquardtSolver3> {
  LevenbergMarquardtSolver3() {
    this->zeros = {0.5, 0.5};
    this->epsilon = 1.e-12;
    this->iterMax = 10;
    this->levmar_mu0 = 1.e-6;
    this->levmar_p0 = 1.e-4;
    this->levmar_p1 = 0.25;
    this->levmar_p2 = 0.75;
    this->levmar_m = 1.e-8;
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros; }

  bool computeResidual() noexcept {
    auto& x = this->zeros;
    auto& f = this->fzeros;
    auto& J = this->jacobian;
    f = {1 - x(0), 100 * (x(1) - x(0) * x(0))};
    J = {-1., 0.,  //
         -200 * x(0), 100.};
    return true;
  }  // end of computFdF
};   // end of struct LevenbergMarquardtSolver3

struct TinyLevenbergMarquardtSolverTest final : public tfel::tests::TestCase {
  TinyLevenbergMarquardtSolverTest()
      : tfel::tests::TestCase("TFEL/Math", "TinyLevenbergMarquardtSolverTest") {
  }  // end of TinyLevenbergMarquardtSolverTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    auto nr = LevenbergMarquardtSolver{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
  void test2() {
    using namespace tfel::math;
    auto nr = LevenbergMarquardtSolver2{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x - Cste<double>::sqrt2) < 1e-14);
  }
  void test3() {
    using namespace tfel::math;
    auto nr = LevenbergMarquardtSolver3{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
};

TFEL_TESTS_GENERATE_PROXY(TinyLevenbergMarquardtSolverTest,
                          "TinyLevenbergMarquardtSolverTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyLevenbergMarquardtSolver.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
