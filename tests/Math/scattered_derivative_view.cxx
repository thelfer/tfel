/*!
 * \file   scattered_derivative_view.cxx
 * \brief
 * \author Tristan Chenaille
 * \date 24/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <array>
#include <cstddef>
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

struct ScatteredDerivativeViewTest final : public tfel::tests::TestCase {
  ScatteredDerivativeViewTest()
      : tfel::tests::TestCase("TFEL/Math", "ScatteredDerivativeViewTest") {
  }  // end of ScatteredDerivativeViewTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    return this->result;
  }  // end of execute

 private:
  // Returns the pointers, ordered in row major format, to the components of
  // a matrix stored in column major format: the element \((i, j)\) of the
  // matrix is at index `j * N + i` of the buffer, so that the components of
  // a derivative are not separated by a constant stride.
  template <unsigned short N, unsigned short M>
  static constexpr std::array<int*, N * M> getPointers(
      std::array<int, N * M>& buf) {
    auto ptrs = std::array<int*, N * M>{};
    for (std::size_t i = 0; i != N; ++i) {
      for (std::size_t j = 0; j != M; ++j) {
        ptrs[i * M + j] = &buf[j * N + i];
      }
    }
    return ptrs;
  }
  // Checks that the buffer holds `expected` in column major format.
  template <unsigned short N, unsigned short M>
  static constexpr bool checkValuesAtCompileTime(
      const std::array<int, N * M>& buf,
      const tfel::math::tmatrix<N, M, int>& expected) {
    for (std::size_t k = 0; k != buf.size(); ++k) {
      if (buf[k] != expected(static_cast<unsigned short>(k % N),
                             static_cast<unsigned short>(k / N))) {
        return false;
      }
    }
    return true;
  }
  //
  template <unsigned short N, unsigned short M>
  void checkValuesAtRuntimeTime(
      const std::array<int, N * M>& buf,
      const tfel::math::tmatrix<N, M, int>& expected) {
    for (std::size_t k = 0; k != buf.size(); ++k) {
      TFEL_TESTS_ASSERT(buf[k] == expected(static_cast<unsigned short>(k % N),
                                           static_cast<unsigned short>(k / N)));
    }
  }
  //
  void test1() {
    using namespace tfel::math;
    constexpr auto expected = tmatrix<6, 6, int>{0, 0, 0, 0, 0, 0,  //
                                                 0, 1, 0, 0, 0, 0,  //
                                                 0, 0, 0, 2, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0};
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 6 * 6>{};
      const auto ptrs = getPointers<6, 6>(a);
      map_derivative_scattered<1, 1, int, int, 6, 6>(ptrs) = 1;
      map_derivative_scattered<2, 3, int, int, 6, 6>(ptrs) = 2;
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkValuesAtCompileTime(buf, expected));
    this->checkValuesAtRuntimeTime(buf, expected);
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
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 5 * 3>{};
      const auto ptrs = getPointers<5, 3>(a);
      map_derivative_scattered<1, 1, stensor<2u, int>, int, 5, 3>(ptrs) =
          stensor<2u, int>::Id();
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkValuesAtCompileTime(buf, expected));
    this->checkValuesAtRuntimeTime(buf, expected);
  }
  //
  void test3() {
    using namespace tfel::math;
    constexpr auto expected = tmatrix<2, 5, int>{
        0, 1, 1, 1, 0,  //
        0, 0, 0, 0, 0   //
    };
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 2 * 5>{};
      const auto ptrs = getPointers<2, 5>(a);
      map_derivative_scattered<0, 1, int, stensor<2u, int>, 2, 5>(ptrs) =
          stensor<2u, int>::Id();
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkValuesAtCompileTime(buf, expected));
    this->checkValuesAtRuntimeTime(buf, expected);
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
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 5 * 5>{};
      const auto ptrs = getPointers<5, 5>(a);
      map_derivative_scattered<1, 1, stensor<2u, int>, stensor<2u, int>, 5, 5>(
          ptrs) = st2tost2<2u, int>::IxI();
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(checkValuesAtCompileTime(buf, expected));
    this->checkValuesAtRuntimeTime(buf, expected);
  }
  // row and column indices given at run-time, non symmetric values
  void test5() {
    using namespace tfel::math;
    constexpr auto expected = tmatrix<5, 6, int>{
        0, 0, 0,  0,  0,  0,   //
        0, 0, 11, 12, 13, 14,  //
        0, 0, 21, 22, 23, 24,  //
        0, 0, 31, 32, 33, 34,  //
        0, 0, 41, 42, 43, 44   //
    };
    auto buf = std::array<int, 5 * 6>{};
    const auto ptrs = getPointers<5, 6>(buf);
    auto d = map_derivative_scattered<stensor<2u, int>, stensor<2u, int>, 5, 6>(
        ptrs, 1, 2);
    for (unsigned short i = 0; i != 4; ++i) {
      for (unsigned short j = 0; j != 4; ++j) {
        d(i, j) = 10 * (i + 1) + j + 1;
      }
    }
    this->checkValuesAtRuntimeTime(buf, expected);
    const st2tost2<2u, int> s = d;
    for (unsigned short i = 0; i != 4; ++i) {
      for (unsigned short j = 0; j != 4; ++j) {
        TFEL_TESTS_ASSERT(s(i, j) == 10 * (i + 1) + j + 1);
      }
    }
  }
};  // end of struct ScatteredDerivativeViewTest

TFEL_TESTS_GENERATE_PROXY(ScatteredDerivativeViewTest,
                          "ScatteredDerivativeViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ScatteredDerivativeView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
