/*!
 * \file   tests/Math/newton_raphson.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   02 Aug 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/TinyNewtonRaphson.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"

struct NewtonRaphsonSolver
    : public tfel::math::TinyNewtonRaphson<2u, double, NewtonRaphsonSolver> {
  NewtonRaphsonSolver() {
    this->zeros = {0., 0.};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros; }

 protected:
  //
  friend struct tfel::math::TinyNewtonRaphson<2u, double, NewtonRaphsonSolver>;
  //
  bool computeResidualAndJacobian() noexcept {
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
  }  // end of computeResidualAndJacobian

};   // end of struct NewtonRaphsonSolver

struct NewtonRaphsonSolver2
    : public tfel::math::TinyNewtonRaphson<1u, double, NewtonRaphsonSolver2> {
  NewtonRaphsonSolver2() {
    this->zeros = {100};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros(0); }

protected:
 //
 friend struct tfel::math::TinyNewtonRaphson<1u, double, NewtonRaphsonSolver2>;

 bool computeResidualAndJacobian() noexcept {
   auto& f = this->fzeros(0);
   auto& x = this->zeros(0);
   auto& J = this->jacobian(0, 0);
   f = x * x - 2.;
   J = 2 * x;
   return true;
  }  // end of computFdF
};   // end of struct NewtonRaphsonSolver2

struct TinyNewtonRaphsonTest final : public tfel::tests::TestCase {
  TinyNewtonRaphsonTest()
      : tfel::tests::TestCase("TFEL/Math", "TinyNewtonRaphsonTest") {
  }  // end of TinyNewtonRaphsonTest
  tfel::tests::TestResult execute() override {
//    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    auto nr = NewtonRaphsonSolver{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(abs(x(1) - 1) < 1e-14);
  }
  void test2() {
    using namespace tfel::math;
    auto nr = NewtonRaphsonSolver2{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(abs(x - Cste<double>::sqrt2) < 1e-14);
  }
};

TFEL_TESTS_GENERATE_PROXY(TinyNewtonRaphsonTest, "TinyNewtonRaphsonTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyNewtonRaphson.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
