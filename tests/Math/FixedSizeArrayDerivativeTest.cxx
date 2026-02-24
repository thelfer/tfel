/*!
 * \file   FixedSizeArrayDerivativeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Array/FixedSizeArrayDerivative.hxx"

// on OpenSolaris and gcc10, std::abs is not constexpr
static constexpr double my_abs(const double x) {
  return x > 0 ? x : -x;
}  // end of my_abs

struct FSDerivativeArrayTest final : public tfel::tests::TestCase {
  FSDerivativeArrayTest()
      : tfel::tests::TestCase("TFEL/Math", "FSDerivativeArrayTest") {
  }  // end of FSDerivativeArrayTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math;
    constexpr auto eps = double{1e-14};
    using Stensor6 =
        FixedSizeArrayDerivative<st2tost2<2u, double>, stensor<2u, double>>;
    constexpr Stensor6 d(1);

    TFEL_TESTS_STATIC_ASSERT(Stensor6::arity == 3);
    TFEL_TESTS_STATIC_ASSERT(d.size() == 64);
    TFEL_TESTS_STATIC_ASSERT(d.size(0) == 4);
    TFEL_TESTS_STATIC_ASSERT(d.size(1) == 4);
    TFEL_TESTS_STATIC_ASSERT(d.size(2) == 4);
    TFEL_TESTS_STATIC_ASSERT(my_abs(d(0, 0, 0) - 1) < eps);
  }
  void test2() {
    using MyStensor4 =
        tfel::math::FixedSizeArrayDerivative<tfel::math::stensor<2u, double>,
                                             tfel::math::stensor<2u, double>>;
    constexpr auto eps = double{1e-14};
    constexpr MyStensor4 d(2);
    TFEL_TESTS_STATIC_ASSERT(MyStensor4::arity == 2);
    TFEL_TESTS_STATIC_ASSERT(d.size() == 16);
    TFEL_TESTS_STATIC_ASSERT(d.size(0) == 4);
    TFEL_TESTS_STATIC_ASSERT(d.size(1) == 4);
    TFEL_TESTS_ASSERT(std::abs(d(0, 0) - 2) < eps);
  }
  void test3() {
    using MyStensor4 =
        tfel::math::FixedSizeArrayDerivative<tfel::math::stensor<2u, double>,
                                             tfel::math::stensor<2u, double>>;
    constexpr auto eps = double{1e-14};
    MyStensor4 d(2);
    TFEL_TESTS_STATIC_ASSERT(MyStensor4::arity == 2);
    TFEL_TESTS_STATIC_ASSERT(d.size() == 16);
    TFEL_TESTS_STATIC_ASSERT(d.size(0) == 4);
    TFEL_TESTS_STATIC_ASSERT(d.size(1) == 4);
    d *= 4;
    for (const auto& v : d) {
      TFEL_TESTS_ASSERT(std::abs(v - 8) < eps);
    }
    MyStensor4 d2(0);
    for (MyStensor4::size_type i = 0; i != d2.size(0); ++i) {
      d2(i, i) = 1;
    }
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<tfel::math::ComputeObjectTag<MyStensor4>::type,
                        tfel::math::FixedSizeArrayDerivativeTag<
                            tfel::math::StensorTag, tfel::math::StensorTag>>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<
            tfel::math::result_type<int, MyStensor4, tfel::math::OpMult>,
            MyStensor4>));
    d += 2 * d2;
    for (MyStensor4::size_type i = 0; i != d.size(0); ++i) {
      for (MyStensor4::size_type j = 0; j != d.size(1); ++j) {
        if (i == j) {
          TFEL_TESTS_ASSERT(std::abs(d(i, j) - 10) < eps);
        } else {
          TFEL_TESTS_ASSERT(std::abs(d(i, j) - 8) < eps);
        }
      }
    }
    const auto d3 = eval(d - 5 * d2);
    for (MyStensor4::size_type i = 0; i != d3.size(0); ++i) {
      for (MyStensor4::size_type j = 0; j != d3.size(1); ++j) {
        if (i == j) {
          TFEL_TESTS_ASSERT(std::abs(d3(i, j) - 5) < eps);
        } else {
          TFEL_TESTS_ASSERT(std::abs(d3(i, j) - 8) < eps);
        }
      }
    }
  }
  // test of the product of two derivative array
  void test4() {
    using MyStensor4 =
        tfel::math::FixedSizeArrayDerivative<tfel::math::stensor<1u, double>,
                                             tfel::math::stensor<1u, double>>;
    using size_type = MyStensor4::size_type;
    constexpr auto eps = double{1e-14};
    const auto a = MyStensor4{1,  2,  3,   //
                              0,  3,  -2,  //
                              -3, -1, -5};
    const auto b = MyStensor4{1,  2,  4,   //
                              -2, 3,  -2,  //
                              -7, -1, -2};
    const auto ab = MyStensor4{-24, 5,  -6,  //
                               8,   11, -2,  //
                               34,  -4, 0};
    const auto c = a * b;
    for (size_type i = 0; i != c.size(0); ++i) {
      for (size_type j = 0; j != c.size(1); ++j) {
        TFEL_TESTS_ASSERT(std::abs(ab(i, j) - c(i, j)) < eps);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(FSDerivativeArrayTest, "FSDerivativeArrayTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("fixedsizederivativearray.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
