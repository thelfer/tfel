/*!
 * \file   tests/Math/broyden.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 09/04/2008
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
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/Broyden.hxx"
#include "TFEL/Math/TinyBroydenSolver.hxx"

struct BroydenSolver
    : public tfel::math::TinyBroydenSolver<2u, double, BroydenSolver> {
  BroydenSolver() {
    this->zeros = {1., 2.};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
    this->jacobian = {1, 2,  //
                      2, 16};
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros; }

  //
  bool computeResidual() noexcept {
    auto& x = this->zeros;
    this->fzeros = {x(0) + 2 * x(1) - 2, x(0) * x(0) + 4 * x(1) * x(1) - 4};
    return true;
  }  // end of computeResidual

};  // end of struct BroydenSolver

/*!
 * \brief solver of x * x - 4 = 0 from x = 3, where the estimate is bounded
 * by 1 in `processNewEstimate`: the first correction (-5) is bounded to -2.
 */
struct BoundedBroydenSolver
    : public tfel::math::TinyBroydenSolver<1u, double, BoundedBroydenSolver> {
  BoundedBroydenSolver() {
    this->zeros = {3.};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
    this->jacobian = {1};
  }

  bool solve() { return this->solveNonLinearSystem(); }

  bool computeResidual() noexcept {
    this->fzeros = {this->zeros(0) * this->zeros(0) - 4};
    return true;
  }  // end of computeResidual

  void processNewEstimate() noexcept {
    this->zeros(0) = std::max(this->zeros(0), 1.);
  }  // end of processNewEstimate

  void updateOrCheckJacobian() noexcept {
    if (this->iter == 1) {
      this->first_update = this->jacobian(0, 0);
    }
  }  // end of updateOrCheckJacobian

  //! \brief jacobian after the first Broyden update
  double first_update = 0;
};  // end of struct BoundedBroydenSolver

struct TinyBroydenSolverTest final : public tfel::tests::TestCase {
  TinyBroydenSolverTest()
      : tfel::tests::TestCase("TFEL/Math", "TinyBroydenSolverTest") {
  }  // end of TinyBroydenSolverTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    return this->result;
  }  // end of execute
 private:
  static const tfel::math::tvector<2, double> f(
      const tfel::math::tvector<2, double>& x) {
    tfel::math::tvector<2, double> y;
    y(0) = 1 + x(0) * (2 * x(0) - 4) - 2 * x(1) * x(1) * x(1);
    y(1) = -4 + x(0) * x(0) * x(0) * x(0) + 4 * x(1) * (1 + x(1) * x(1) * x(1));
    return y;
  }
  static const tfel::math::tvector<2, double> f2(
      const tfel::math::tvector<2, double>& x) {
    return {x(0) + 2 * x(1) - 2, x(0) * x(0) + 4 * x(1) * x(1) - 4};
  }

  void test1() {
    using namespace tfel::math;
    auto x0 = tvector<2u, double>{1.5, -0.5};
    auto A = tmatrix<2u, 2u, double>{4 * (x0(0) - 1), -6 * x0(1) * x0(1),  //
                                     4 * x0(0) * x0(0) * x0(0),
                                     4 * (4 * x0(1) * x0(1) * x0(1) + 1)};
    const auto [b, x] = broyden<2, double, f>(x0, A, 1.e-11, 20);
    TFEL_TESTS_ASSERT(b);
    const auto vf = f(x);
    TFEL_TESTS_ASSERT(norm(vf) < 1e-11);
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1.5560974843220530275) < 1.e-11);
    TFEL_TESTS_ASSERT(std::abs(x(1) + 0.57565076961884345) < 1.e-11);
  }

  void test2() {
    using namespace tfel::math;
    const auto x0 = tvector<2u, double>{1., 2.};
    auto A = tmatrix<2u, 2u, double>{1, 2,  //
                                     2, 16};
    const auto [b, x] = broyden<2, double, f2>(x0, A, 1.e-15, 20);
    TFEL_TESTS_ASSERT(b);
    const auto vf = f2(x);
    TFEL_TESTS_ASSERT(norm(vf) < 1.e-11);
    TFEL_TESTS_ASSERT(std::abs(x(0)) < 1.e-11);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1.e-11);
  }

  void test3() {
    using namespace tfel::math;
    const auto x0 = tvector<2u, double>{1.5, -0.5};
    const auto [b, x] = broyden<2, double, f2>(x0, 1.e-11, 20);
    TFEL_TESTS_ASSERT(b);
    const auto vf = f2(x);
    TFEL_TESTS_ASSERT(norm(vf) < 1.e-11);
    TFEL_TESTS_ASSERT(std::abs(x(0) - 2) < 1.e-11);
    TFEL_TESTS_ASSERT(std::abs(x(1)) < 1.e-11);
    // convergence at the last allowed iteration
    TFEL_TESTS_ASSERT((broyden<2, double, f2>(x0, 1.e-11, 16).first));
  }

  void test4() {
    BroydenSolver s;
    TFEL_TESTS_ASSERT(s.solve());
    const auto x = s.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0)) < 1.e-11);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1.e-11);
  }

  void test5() {
    // the Broyden update uses the step actually taken, i.e. the bounded
    // one: after the first update, the jacobian is the secant slope
    // (F(1) - F(3)) / (1 - 3) = 4
    BoundedBroydenSolver s;
    TFEL_TESTS_ASSERT(s.solve());
    TFEL_TESTS_ASSERT(std::abs(s.first_update - 4) < 1.e-12);
  }
};

TFEL_TESTS_GENERATE_PROXY(TinyBroydenSolverTest, "TinyBroydenSolverTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyBroydenSolver.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
