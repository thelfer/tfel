/*!
 * \file   tests/Math/derivative_view.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct DerivativeViewTest final : public tfel::tests::TestCase {
  DerivativeViewTest()
      : tfel::tests::TestCase("TFEL/Math", "DerivativeViewTest") {
  }  // end of DerivativeViewTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute

 private:
  //
  template <unsigned short N, unsigned short M>
  static constexpr bool checkMatrixValuesAtCompileTime(
      const tfel::math::tmatrix<N, M, int>& m1,
      const tfel::math::tmatrix<N, M, int>& m2) {
    using size_type = typename tfel::math::tmatrix<N, M, int>::size_type;
    for (size_type i = 0; i != m1.getNbRows(); ++i) {
      for (size_type j = 0; j != m1.getNbCols(); ++j) {
        if (m1(i, j) != m2(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
  //
  template <unsigned short N, unsigned short M>
  void checkMatrixValuesAtRuntimeTime(
      const tfel::math::tmatrix<N, M, int>& m1,
      const tfel::math::tmatrix<N, M, int>& m2) {
    using size_type = typename tfel::math::tmatrix<N, M, int>::size_type;
    for (size_type i = 0; i != m1.getNbRows(); ++i) {
      for (size_type j = 0; j != m1.getNbCols(); ++j) {
        TFEL_TESTS_ASSERT(m1(i, j) == m2(i, j));
      }
    }
  }
  //
  void test1() {
    using namespace tfel::math;
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<int&, derivative_view_from_tiny_matrix<3, int, int>>));
    TFEL_TESTS_STATIC_ASSERT((std::is_same_v<int, derivative_type<int, int>>));
    constexpr auto expected = tmatrix<6, 6, int>{0, 0, 0, 0, 0, 0,  //
                                                 0, 1, 0, 0, 0, 0,  //
                                                 0, 0, 0, 2, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0};
    constexpr auto m = []() constexpr {
      auto r = tmatrix<6, 6, int>{0};
      derivative_view_from_tiny_matrix<6, int, int>(r(1, 1)) = 1;
      derivative_view_from_tiny_matrix<6, int, int>(r(2, 3)) = 2;
      return r;
    }
    ();
    constexpr auto m2 = []() constexpr {
      auto r = tmatrix<6, 6, int>{0};
      map_derivative<1, 1, int, int>(r) = 1;
      map_derivative<2, 3, int, int>(r) = 2;
      return r;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m, expected));
    this->checkMatrixValuesAtRuntimeTime(m, expected);
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m2, expected));
    this->checkMatrixValuesAtRuntimeTime(m2, expected);
  }
  //
  void test2() {
    using namespace tfel::math;
    constexpr auto expected = tmatrix<5, 3, int>{
        0, 0, 0,  //
        0, 1, 0,  //
        0, 1, 0,  //
        0, 1, 0,  //
        0, 0, 0   //
    };
    constexpr auto m = []() constexpr {
      auto r = tmatrix<5, 3, int>{0};
      auto s = derivative_view_from_tiny_matrix<3, stensor<2u, int>, int>(
          &(r(1, 1)));
      s = stensor<2u, int>::Id();
      return r;
    }
    ();
    constexpr auto m2 = []() constexpr {
      auto r = tmatrix<5, 3, int>{0};
      map_derivative<1, 1, stensor<2u, int>, int>(r) = stensor<2u, int>::Id();
      return r;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m, expected));
    this->checkMatrixValuesAtRuntimeTime(m, expected);
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m2, expected));
    this->checkMatrixValuesAtRuntimeTime(m2, expected);
  }
  //
  void test3() {
    using namespace tfel::math;
    constexpr auto expected = tmatrix<2, 5, int>{
        0, 1, 1, 1, 0,  //
        0, 0, 0, 0, 0   //
    };
    constexpr auto m = []() constexpr {
      auto r = tmatrix<2, 5, int>{0};
      auto s = derivative_view_from_tiny_matrix<5, int, stensor<2u, int>>(
          &(r(0, 1)));
      s = stensor<2u, int>::Id();
      return r;
    }
    ();
    constexpr auto m2 = []() constexpr {
      auto r = tmatrix<2, 5, int>{0};
      map_derivative<0, 1, int, stensor<2u, int>>(r) = stensor<2u, int>::Id();
      return r;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m, expected));
    this->checkMatrixValuesAtRuntimeTime(m, expected);
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m2, expected));
    this->checkMatrixValuesAtRuntimeTime(m2, expected);
  }
  //
  void test4() {
    using namespace tfel::math;
    constexpr auto expected = tmatrix<5, 5, int>{
        0, 0, 0, 0, 0,  //
        0, 1, 1, 1, 0,  //
        0, 1, 1, 1, 0,  //
        0, 1, 1, 1, 0,  //
        0, 0, 0, 0, 0   //
    };
    constexpr auto m = []() constexpr {
      auto r = tmatrix<5, 5, int>{0};
      auto s = derivative_view_from_tiny_matrix<5, stensor<2u, int>,
                                                stensor<2u, int>>(&(r(1, 1)));
      s = st2tost2<2u, int>::IxI();
      return r;
    }
    ();
    constexpr auto m2 = []() constexpr {
      auto r = tmatrix<5, 5, int>{0};
      map_derivative<1, 1, stensor<2u, int>, stensor<2u, int>>(r) =
          st2tost2<2u, int>::IxI();
      return r;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m, expected));
    this->checkMatrixValuesAtRuntimeTime(m, expected);
    TFEL_TESTS_STATIC_ASSERT(checkMatrixValuesAtCompileTime(m2, expected));
    this->checkMatrixValuesAtRuntimeTime(m2, expected);
  }
};  // end of struct DerivativeViewTest

TFEL_TESTS_GENERATE_PROXY(DerivativeViewTest, "DerivativeViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DerivativeView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
