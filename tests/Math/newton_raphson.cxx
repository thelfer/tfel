/*!
 * \file   tests/Math/newton_raphson.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   02 Aug 2006
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
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/LUSolve.hxx"
#include "TFEL/Math/TinyNewtonRaphsonSolver.hxx"

struct NewtonRaphsonSolver
    : public tfel::math::
          TinyNewtonRaphsonSolver<2u, double, NewtonRaphsonSolver> {
  NewtonRaphsonSolver() {
    this->zeros = {0., 0.};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
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

};  // end of struct NewtonRaphsonSolver

struct NewtonRaphsonSolver2
    : public tfel::math::
          TinyNewtonRaphsonSolver<1u, double, NewtonRaphsonSolver2> {
  NewtonRaphsonSolver2() {
    this->zeros = {100};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
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
};   // end of struct NewtonRaphsonSolver2

struct NewtonRaphsonSolver3
    : public tfel::math::
          TinyNewtonRaphsonSolver<2u, double, NewtonRaphsonSolver3> {
  NewtonRaphsonSolver3() {
    this->zeros = {0.5, 0.5};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
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
};   // end of struct NewtonRaphsonSolver3

template <unsigned short N, typename NumericType>
struct HeapAllocatedWorkspace {
  HeapAllocatedWorkspace()
      : fzeros(N), zeros(N), delta_zeros(N), jacobian(N, N) {}
  //! \brief residual vector
  tfel::math::vector<NumericType> fzeros;
  //! \brief current estimate of the unknowns
  tfel::math::vector<NumericType> zeros;
  //! \brief current correction
  tfel::math::vector<NumericType> delta_zeros;
  //! \brief jacobian matrix
  tfel::math::matrix<NumericType> jacobian;
};

struct NewtonRaphsonSolver4
    : public tfel::math::TinyNewtonRaphsonSolver<2u,
                                                 double,
                                                 NewtonRaphsonSolver4,
                                                 HeapAllocatedWorkspace> {
  NewtonRaphsonSolver4() {
    this->zeros = {0.5, 0.5};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
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

  bool solveLinearSystem(tfel::math::matrix<double>& m,
                         tfel::math::vector<double>& v) const noexcept {
    try {
      tfel::math::LUSolve::exe(m, v);
    } catch (...) {
      return false;
    }
    return true;
  }

};  // end of struct NewtonRaphsonSolver4

template <unsigned short N, typename NumericType>
struct ExternallyAllocatedWorkspace {
  ExternallyAllocatedWorkspace(NumericType* const v)
      : fzeros(v), zeros(v + N), delta_zeros(v + 2 * N), jacobian(v + 3 * N) {}
  //! \brief residual vector
  tfel::math::View<tfel::math::tvector<N, NumericType>> fzeros;
  //! \brief current estimate of the unknowns
  tfel::math::View<tfel::math::tvector<N, NumericType>> zeros;
  //! \brief current correction
  tfel::math::View<tfel::math::tvector<N, NumericType>> delta_zeros;
  //! \brief jacobian matrix
  tfel::math::View<tfel::math::tmatrix<N, N, NumericType>> jacobian;
};

struct NewtonRaphsonSolver5
    : public tfel::math::TinyNewtonRaphsonSolver<2u,
                                                 double,
                                                 NewtonRaphsonSolver5,
                                                 ExternallyAllocatedWorkspace> {
  NewtonRaphsonSolver5(double* const buffer)
      : tfel::math::TinyNewtonRaphsonSolver<2u,
                                            double,
                                            NewtonRaphsonSolver5,
                                            ExternallyAllocatedWorkspace>(
            buffer) {
    this->zeros = {0.5, 0.5};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
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

};  // end of struct NewtonRaphsonSolver5

struct NewtonRaphsonSolver6
    : public tfel::math::TinyNewtonRaphsonSolver<4u,
                                                 double,
                                                 NewtonRaphsonSolver6,
                                                 ExternallyAllocatedWorkspace> {
  NewtonRaphsonSolver6(double* const buffer)
      : tfel::math::TinyNewtonRaphsonSolver<4u,
                                            double,
                                            NewtonRaphsonSolver6,
                                            ExternallyAllocatedWorkspace>(
            buffer) {
    this->zeros = {0.5, 0.5, 1., 1.};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros; }

  bool computeResidual() noexcept {
    auto& x = this->zeros;
    auto& f = this->fzeros;
    auto& J = this->jacobian;
    f = {1 - x(0), 100 * (x(1) - x(0) * x(0)), x(2), x(3)};
    J = {
        -1.,         0.,   0., 0.,  //
        -200 * x(0), 100., 0., 0.,  //
        0.,          0.,   1., 0.,  //
        0.,          0.,   0., 1.   //
    };
    return true;
  }  // end of computFdF

};  // end of struct NewtonRaphsonSolver6

struct TinyNewtonRaphsonSolverTest final : public tfel::tests::TestCase {
  TinyNewtonRaphsonSolverTest()
      : tfel::tests::TestCase("TFEL/Math", "TinyNewtonRaphsonSolverTest") {
  }  // end of TinyNewtonRaphsonSolverTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    this->test6();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    auto nr = NewtonRaphsonSolver{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
  void test2() {
    using namespace tfel::math;
    auto nr = NewtonRaphsonSolver2{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x - Cste<double>::sqrt2) < 1e-14);
  }
  void test3() {
    auto nr = NewtonRaphsonSolver3{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
  void test4() {
    auto nr = NewtonRaphsonSolver4{};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
  void test5() {
    auto buffer = std::vector<double>{};
    buffer.resize(10);
    auto nr = NewtonRaphsonSolver5{buffer.data()};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
  }
  void test6() {
    auto buffer = std::vector<double>{};
    buffer.resize(28);
    auto nr = NewtonRaphsonSolver6{buffer.data()};
    TFEL_TESTS_ASSERT(nr.solve());
    const auto x = nr.getCurrentEstimate();
    TFEL_TESTS_ASSERT(std::abs(x(0) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(1) - 1) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(2)) < 1e-14);
    TFEL_TESTS_ASSERT(std::abs(x(3)) < 1e-14);
  }
};

TFEL_TESTS_GENERATE_PROXY(TinyNewtonRaphsonSolverTest,
                          "TinyNewtonRaphsonSolverTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyNewtonRaphsonSolver.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
