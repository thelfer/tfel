/*!
 * \file  tests/Math/tinymatrixsolve3.cxx
 * \brief tests of the resolution of linear systems sharing the same matrix,
 * the right-hand sides being the columns of a matrix
 * \author Tristan Chenaille
 * \date 22/11/2026
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
#include <limits>
#include <string>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

template <typename T>
struct Name;

template <>
struct Name<long double> {
  static std::string getName() { return "long double"; }
};

template <>
struct Name<double> {
  static std::string getName() { return "double"; }
};

template <>
struct Name<float> {
  static std::string getName() { return "float"; }
};

template <typename T>
struct TinyMatrixSolveMultipleRightHandSidesTest final
    : public tfel::tests::TestCase {
  TinyMatrixSolveMultipleRightHandSidesTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "TinyMatrixSolveMultipleRightHandSidesTest<" +
                                  Name<T>::getName() + ">") {
  }  // end of TinyMatrixSolveMultipleRightHandSidesTest

  tfel::tests::TestResult execute() override {
    // a matrix whose decomposition requires to permute its rows
    this->check({T(0), T(2), T(1), T(-1),  //
                 T(1), T(0), T(0), T(3),   //
                 T(3), T(0), T(1), T(2),   //
                 T(2), T(-1), T(4), T(0)},
                false);
    // a matrix whose decomposition leaves its rows in place
    this->check({T(8), T(2), T(1), T(-1),  //
                 T(1), T(6), T(0), T(3),   //
                 T(3), T(0), T(9), T(2),   //
                 T(2), T(-1), T(4), T(7)},
                true);
    this->checkNullPivot();
    return this->result;
  }  // end of execute

 private:
  /*!
   * \brief check that the columns of the solution computed for all the
   * right-hand sides at once are the solutions of each right-hand side
   * \param[in] m: matrix of the linear systems
   * \param[in] b: true if the rows of the matrix are expected to be left in
   * place by the decomposition
   */
  void check(const tfel::math::tmatrix<4, 4, T>& m, const bool b) {
    using namespace tfel::math;
    using size_type = typename tmatrix<4, 3, T>::size_type;
    using Solver = TinyMatrixSolve<4, T>;
    const T eps = 100 * std::numeric_limits<T>::epsilon();
    const auto rhs = tmatrix<4, 3, T>{T(5),  T(1), T(0),   //
                                      T(-1), T(0), T(2),   //
                                      T(-2), T(0), T(-3),  //
                                      T(4),  T(0), T(1)};
    auto lu = m;
    TinyPermutation<4> p;
    TFEL_TESTS_ASSERT(Solver::decomp(lu, p));
    TFEL_TESTS_ASSERT(p.isIdentity() == b);
    auto x = rhs;
    TFEL_TESTS_ASSERT(Solver::back_substitute(lu, p, x));
    for (size_type j = 0; j != 3; ++j) {
      // resolution of the system associated with the current column
      auto c = tvector<4, T>{};
      for (size_type i = 0; i != 4; ++i) {
        c(i) = rhs(i, j);
      }
      TFEL_TESTS_ASSERT(Solver::back_substitute(lu, p, c));
      for (size_type i = 0; i != 4; ++i) {
        TFEL_TESTS_ASSERT(std::abs(x(i, j) - c(i)) < eps);
        // residual
        auto r = -rhs(i, j);
        for (size_type k = 0; k != 4; ++k) {
          r += m(i, k) * x(k, j);
        }
        TFEL_TESTS_ASSERT(std::abs(r) < eps);
      }
    }
    // decomposition and resolution in a single call
    auto m2 = m;
    auto x2 = rhs;
    TFEL_TESTS_ASSERT(Solver::exe(m2, x2));
    for (size_type i = 0; i != 4; ++i) {
      for (size_type j = 0; j != 3; ++j) {
        TFEL_TESTS_ASSERT(std::abs(x2(i, j) - x(i, j)) < eps);
      }
    }
  }  // end of check
  //! \brief check that a null pivot is reported
  void checkNullPivot() {
    using namespace tfel::math;
    using Solver = TinyMatrixSolve<4, T>;
    using SolverWithoutExceptions = TinyMatrixSolve<4, T, false>;
    auto lu = tmatrix<4, 4, T>::Id();
    lu(2, 2) = T(0);
    const TinyPermutation<4> p;
    auto x = tmatrix<4, 3, T>(T(1));
    TFEL_TESTS_ASSERT(!SolverWithoutExceptions::back_substitute(lu, p, x));
    TFEL_TESTS_CHECK_THROW(Solver::back_substitute(lu, p, x), LUNullPivot);
  }  // end of checkNullPivot

};  // end of TinyMatrixSolveMultipleRightHandSidesTest

using FloatTinyMatrixSolveMultipleRightHandSidesTest =
    TinyMatrixSolveMultipleRightHandSidesTest<float>;
using DoubleTinyMatrixSolveMultipleRightHandSidesTest =
    TinyMatrixSolveMultipleRightHandSidesTest<double>;
using LongDoubleTinyMatrixSolveMultipleRightHandSidesTest =
    TinyMatrixSolveMultipleRightHandSidesTest<long double>;

TFEL_TESTS_GENERATE_PROXY(FloatTinyMatrixSolveMultipleRightHandSidesTest,
                          "TinyMatrixSolveMultipleRightHandSidesTest<float>");
TFEL_TESTS_GENERATE_PROXY(DoubleTinyMatrixSolveMultipleRightHandSidesTest,
                          "TinyMatrixSolveMultipleRightHandSidesTest<double>");
TFEL_TESTS_GENERATE_PROXY(
    LongDoubleTinyMatrixSolveMultipleRightHandSidesTest,
    "TinyMatrixSolveMultipleRightHandSidesTest<long double>");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("TinyMatrixSolveMultipleRightHandSides.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
