/*!
 * \file   strided_derivative_view.cxx
 * \brief
 * \author Tristan Chenaille
 * \date   08/06/2026
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

struct StridedDerivativeViewTest final : public tfel::tests::TestCase {
  StridedDerivativeViewTest()
      : tfel::tests::TestCase("TFEL/Math", "StridedDerivativeViewTest") {
  }  // end of StridedDerivativeViewTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute

 private:
  // Checks that the SoA buffer holds `expected` strided by `stride`, i.e. the
  // element \((i, j)\) of the base matrix is at index `(i * M + j) * stride` and
  // every other slot is left untouched (zero).
  template <unsigned short N, unsigned short M, std::size_t SZ>
  static constexpr bool checkStridedValuesAtCompileTime(
      const std::array<int, SZ>& buf,
      const tfel::math::tmatrix<N, M, int>& expected,
      const std::size_t stride) {
    for (std::size_t k = 0; k != SZ; ++k) {
      auto e = 0;
      if ((k % stride == 0) && (k / stride < static_cast<std::size_t>(N) * M)) {
        const auto lin = k / stride;
        e = expected(static_cast<unsigned short>(lin / M),
                     static_cast<unsigned short>(lin % M));
      }
      if (buf[k] != e) {
        return false;
      }
    }
    return true;
  }
  //
  template <unsigned short N, unsigned short M, std::size_t SZ>
  void checkStridedValuesAtRuntimeTime(
      const std::array<int, SZ>& buf,
      const tfel::math::tmatrix<N, M, int>& expected,
      const std::size_t stride) {
    for (std::size_t k = 0; k != SZ; ++k) {
      auto e = 0;
      if ((k % stride == 0) && (k / stride < static_cast<std::size_t>(N) * M)) {
        const auto lin = k / stride;
        e = expected(static_cast<unsigned short>(lin / M),
                     static_cast<unsigned short>(lin % M));
      }
      TFEL_TESTS_ASSERT(buf[k] == e);
    }
  }
  //
  void test1() {
    using namespace tfel::math;
    constexpr std::size_t stride = 2;
    constexpr auto expected = tmatrix<6, 6, int>{0, 0, 0, 0, 0, 0,  //
                                                 0, 1, 0, 0, 0, 0,  //
                                                 0, 0, 0, 2, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0,  //
                                                 0, 0, 0, 0, 0, 0};
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 6 * 6 * stride>{};
      map_derivative_strided<1, 1, int, int, 6, 6>(a.data(), stride) = 1;
      map_derivative_strided<2, 3, int, int, 6, 6>(a.data(), stride) = 2;
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(
        checkStridedValuesAtCompileTime(buf, expected, stride));
    this->checkStridedValuesAtRuntimeTime(buf, expected, stride);
  }
  //
  void test2() {
    using namespace tfel::math;
    constexpr std::size_t stride = 2;
    constexpr auto expected = tmatrix<5, 3, int>{
        0, 0, 0,  //
        0, 1, 0,  //
        0, 1, 0,  //
        0, 1, 0,  //
        0, 0, 0   //
    };
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 5 * 3 * stride>{};
      map_derivative_strided<1, 1, stensor<2u, int>, int, 5, 3>(a.data(),
                                                                stride) =
          stensor<2u, int>::Id();
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(
        checkStridedValuesAtCompileTime(buf, expected, stride));
    this->checkStridedValuesAtRuntimeTime(buf, expected, stride);
  }
  //
  void test3() {
    using namespace tfel::math;
    constexpr std::size_t stride = 2;
    constexpr auto expected = tmatrix<2, 5, int>{
        0, 1, 1, 1, 0,  //
        0, 0, 0, 0, 0   //
    };
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 2 * 5 * stride>{};
      map_derivative_strided<0, 1, int, stensor<2u, int>, 2, 5>(a.data(),
                                                                stride) =
          stensor<2u, int>::Id();
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(
        checkStridedValuesAtCompileTime(buf, expected, stride));
    this->checkStridedValuesAtRuntimeTime(buf, expected, stride);
  }
  //
  void test4() {
    using namespace tfel::math;
    constexpr std::size_t stride = 2;
    constexpr auto expected = tmatrix<5, 5, int>{
        0, 0, 0, 0, 0,  //
        0, 1, 1, 1, 0,  //
        0, 1, 1, 1, 0,  //
        0, 1, 1, 1, 0,  //
        0, 0, 0, 0, 0   //
    };
    constexpr auto buf = []() constexpr {
      auto a = std::array<int, 5 * 5 * stride>{};
      map_derivative_strided<1, 1, stensor<2u, int>, stensor<2u, int>, 5, 5>(
          a.data(), stride) = st2tost2<2u, int>::IxI();
      return a;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(
        checkStridedValuesAtCompileTime(buf, expected, stride));
    this->checkStridedValuesAtRuntimeTime(buf, expected, stride);
  }
};  // end of struct StridedDerivativeViewTest

TFEL_TESTS_GENERATE_PROXY(StridedDerivativeViewTest, "StridedDerivativeViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StridedDerivativeView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
