/*!
 * \file   tests/Math/broyden2.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   09 avr 2008
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
#include "TFEL/Math/Broyden2.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"
#include "TFEL/Math/TinyBroyden2Solver.hxx"

struct Broyden2Solver
    : public tfel::math::TinyBroyden2Solver<2u, double, Broyden2Solver> {
  Broyden2Solver() {
    using namespace tfel::math;
    this->zeros = {1., 2.};
    this->epsilon = 1.e-14;
    this->iterMax = 500;
    auto J = tmatrix<2u, 2u, double>{1, 2,  //
                                     2, 16};
    this->inv_jacobian = tmatrix<2u, 2u, double>::Id();
    TinyMatrixSolve<2u, double>::exe(J, this->inv_jacobian);
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros; }

  //
  bool computeResidual() noexcept {
    auto& x = this->zeros;
    this->fzeros = {x(0) + 2 * x(1) - 2, x(0) * x(0) + 4 * x(1) * x(1) - 4};
    return true;
  }  // end of computeResidual

};  // end of struct Broyden2Solver

struct TinyBroyden2SolverTest final : public tfel::tests::TestCase {
  TinyBroyden2SolverTest()
      : tfel::tests::TestCase("TFEL/Math", "TinyBroyden2SolverTest") {
  }  // end of TinyBroyden2SolverTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
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
    auto J = tmatrix<2u, 2u, double>{4 * (x0(0) - 1), -6 * x0(1) * x0(1),  //
                                     4 * x0(0) * x0(0) * x0(0),
                                     4 * (4 * x0(1) * x0(1) * x0(1) + 1)};
    auto iJ = tmatrix<2u, 2u, double>::Id();
    TinyMatrixSolve<2u, double>::exe(J, iJ);
    const auto [b, x] = broyden2<2, double, f>(x0, iJ, 1.e-11, 20);
    TFEL_TESTS_ASSERT(b);
    const auto vf = f(x);
    TFEL_TESTS_ASSERT(norm(vf) < 1e-11);
    TFEL_TESTS_ASSERT(abs(x(0) - 1.5560974843220530275) < 1.e-11);
    TFEL_TESTS_ASSERT(abs(x(1) + 0.57565076961884345) < 1.e-11);
  }

  void test2() {
    using namespace tfel::math;
    const auto x0 = tvector<2u, double>{1., 2.};
    auto J = tmatrix<2u, 2u, double>{1, 2,  //
                                     2, 16};
    auto iJ = tmatrix<2u, 2u, double>::Id();
    TinyMatrixSolve<2u, double>::exe(J, iJ);
    const auto [b, x] = broyden2<2, double, f2>(x0, iJ, 1.e-15, 40);
    TFEL_TESTS_ASSERT(b);
    const auto vf = f2(x);
    TFEL_TESTS_ASSERT(norm(vf) < 1.e-11);
    TFEL_TESTS_ASSERT(abs(x(0)) < 1.e-11);
    TFEL_TESTS_ASSERT(abs(x(1) - 1) < 1.e-11);
  }

  void test3() {
    using namespace tfel::math;
    const auto x0 = tvector<2u, double>{1.5, -0.5};
    const auto [b, x] = broyden2<2, double, f2>(x0, 1.e-11, 20);
    TFEL_TESTS_ASSERT(b);
    const auto vf = f2(x);
    TFEL_TESTS_ASSERT(norm(vf) < 1.e-11);
    TFEL_TESTS_ASSERT(abs(x(0) - 2) < 1.e-11);
    TFEL_TESTS_ASSERT(abs(x(1)) < 1.e-11);
  }

  void test4() {
    Broyden2Solver s;
    TFEL_TESTS_ASSERT(s.solve());
    const auto x = s.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0)) < 1.e-11);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1.e-11);
  }
};

TFEL_TESTS_GENERATE_PROXY(TinyBroyden2SolverTest, "TinyBroyden2SolverTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyBroyden2Solver.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
