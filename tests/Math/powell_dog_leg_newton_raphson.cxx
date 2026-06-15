/*!
 * \file   tests/Math/powell_dog_leg_newton_raphson.cxx
 *
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
#include "TFEL/Math/TinyPowellDogLegNewtonRaphsonSolver.hxx"

struct PowellDogLegNewtonRaphsonSolver
    : public tfel::math::TinyPowellDogLegNewtonRaphsonSolver<
          2u,
          double,
          PowellDogLegNewtonRaphsonSolver> {
  PowellDogLegNewtonRaphsonSolver() {
    this->zeros = {0., 0.};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
    this->powell_dogleg_trust_region_size = 0.25;
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

};  // end of struct PowellDogLegNewtonRaphsonSolver

struct PowellDogLegNewtonRaphsonSolver2
    : public tfel::math::TinyPowellDogLegNewtonRaphsonSolver<
          1u,
          double,
          PowellDogLegNewtonRaphsonSolver2> {
  PowellDogLegNewtonRaphsonSolver2() {
    this->zeros = {10};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
    this->powell_dogleg_trust_region_size = 2;
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
};   // end of struct PowellDogLegNewtonRaphsonSolver2

struct PowellDogLegNewtonRaphsonSolver3
    : public tfel::math::TinyPowellDogLegNewtonRaphsonSolver<
          2u,
          double,
          PowellDogLegNewtonRaphsonSolver3> {
  PowellDogLegNewtonRaphsonSolver3() {
    this->zeros = {0.5, 0.5};
    this->epsilon = 1.e-12;
    this->iterMax = 20;
    this->powell_dogleg_trust_region_size = 0.2;
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
};   // end of struct PowellDogLegNewtonRaphsonSolver3

struct TinyPowellDogLegNewtonRaphsonSolverTest final
    : public tfel::tests::TestCase {
  TinyPowellDogLegNewtonRaphsonSolverTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "TinyPowellDogLegNewtonRaphsonSolverTest") {
  }  // end of TinyPowellDogLegNewtonRaphsonSolverTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    auto nr = PowellDogLegNewtonRaphsonSolver{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
  void test2() {
    using namespace tfel::math;
    auto nr = PowellDogLegNewtonRaphsonSolver2{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x - Cste<double>::sqrt2) < 1e-14);
  }
  void test3() {
    using namespace tfel::math;
    auto nr = PowellDogLegNewtonRaphsonSolver3{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
};

TFEL_TESTS_GENERATE_PROXY(TinyPowellDogLegNewtonRaphsonSolverTest,
                          "TinyPowellDogLegNewtonRaphsonSolverTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyPowellDogLegNewtonRaphsonSolver.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
