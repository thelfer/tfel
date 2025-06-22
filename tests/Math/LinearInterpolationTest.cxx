/*!
 * \file   LinearInterpolationTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/07/2022
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
#include <array>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/LinearInterpolation.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct LinearInterpolationTest final : public tfel::tests::TestCase {
  LinearInterpolationTest()
      : tfel::tests::TestCase("TFEL/Math", "LinearInterpolationTest") {
  }  // end of LinearInterpolationTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
#ifndef _MSC_VER
    this->test6();
#endif /* _MSC_VER */
    return this->result;
  }  // end of execute
 private:
  void test1() {
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    constexpr std::array<double, 1u> abscissae{1};
    constexpr std::array<double, 1u> values{50};
    constexpr auto v1 =
        tfel::math::computeLinearInterpolation<true>(abscissae, values, 0);
    constexpr auto v2 =
        tfel::math::computeLinearInterpolation<false>(abscissae, values, 0);
    TFEL_TESTS_STATIC_ASSERT(my_abs(v1 - 50) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(v2 - 50) < 10 * eps);
  }
  void test2() {
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    constexpr std::array<double, 3u> abscissae{0, 1, 2};
    constexpr std::array<double, 3u> values{1, 2, 4};
    constexpr auto values1 = std::array<double, 5u>{
        tfel::math::computeLinearInterpolation<true>(abscissae, values, -1),
        tfel::math::computeLinearInterpolation<true>(abscissae, values, 0),
        tfel::math::computeLinearInterpolation<true>(abscissae, values, 0.5),
        tfel::math::computeLinearInterpolation<true>(abscissae, values, 1.5),
        tfel::math::computeLinearInterpolation<true>(abscissae, values, 3),
    };
    constexpr auto values2 = std::array<double, 5u>{
        tfel::math::computeLinearInterpolation<false>(abscissae, values, -1),
        tfel::math::computeLinearInterpolation<false>(abscissae, values, 0),
        tfel::math::computeLinearInterpolation<false>(abscissae, values, 0.5),
        tfel::math::computeLinearInterpolation<false>(abscissae, values, 1.5),
        tfel::math::computeLinearInterpolation<false>(abscissae, values, 3),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[0]) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[1] - 1) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[2] - 1.5) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[3] - 3) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[4] - 6) < 10 * eps);
    //
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[0] - 1) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[1] - 1) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[2] - 1.5) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[3] - 3) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[4] - 4) < 10 * eps);
  }
  void test3() {
    using time = tfel::math::qt<tfel::math::Time, double>;
    using stress = tfel::math::qt<tfel::math::Stress, double>;
    constexpr auto eps = std::numeric_limits<stress>::epsilon();
    constexpr std::array<time, 3u> abscissae{time{0}, time{1}, time{2}};
    constexpr std::array<stress, 3u> values{stress{1}, stress{2}, stress{4}};
    constexpr auto values1 = std::array<stress, 5u>{
        tfel::math::computeLinearInterpolation<true>(abscissae, values,
                                                     time{-1}),
        tfel::math::computeLinearInterpolation<true>(abscissae, values,
                                                     time{0}),
        tfel::math::computeLinearInterpolation<true>(abscissae, values,
                                                     time{0.5}),
        tfel::math::computeLinearInterpolation<true>(abscissae, values,
                                                     time{1.5}),
        tfel::math::computeLinearInterpolation<true>(abscissae, values,
                                                     time{3}),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[0]) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[1] - stress{1}) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[2] - stress{1.5}) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[3] - stress{3}) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values1[4] - stress{6}) < 10 * eps);
    //
    constexpr auto values2 = std::array<stress, 5u>{
        tfel::math::computeLinearInterpolation<false>(abscissae, values,
                                                      time{-1}),
        tfel::math::computeLinearInterpolation<false>(abscissae, values,
                                                      time{0}),
        tfel::math::computeLinearInterpolation<false>(abscissae, values,
                                                      time{0.5}),
        tfel::math::computeLinearInterpolation<false>(abscissae, values,
                                                      time{1.5}),
        tfel::math::computeLinearInterpolation<false>(abscissae, values,
                                                      time{3}),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[0] - stress{1}) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[1] - stress{1}) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[2] - stress{1.5}) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[3] - stress{3}) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(values2[4] - stress{4}) < 10 * eps);
  }
  void test4() {
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    constexpr std::array<double, 1u> abscissae{1};
    constexpr std::array<double, 1u> values{50};
    constexpr auto v1 =
        tfel::math::computeLinearInterpolationAndDerivative<true>(abscissae,
                                                                  values, 0);
    constexpr auto v2 =
        tfel::math::computeLinearInterpolationAndDerivative<false>(abscissae,
                                                                   values, 0);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(v1) - 50) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(v1)) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(v2) - 50) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(v2)) < 10 * eps);
  }
  void test5() {
    using time = tfel::math::qt<tfel::math::Time, double>;
    using stress = tfel::math::qt<tfel::math::Stress, double>;
    using stressrate = tfel::math::derivative_type<stress, time>;
    constexpr auto eps = std::numeric_limits<stress>::epsilon();
    constexpr auto eps2 = std::numeric_limits<stressrate>::epsilon();
    constexpr std::array<time, 3u> abscissae{time{0}, time{1}, time{2}};
    constexpr std::array<stress, 3u> values{stress{1}, stress{2}, stress{4}};
    constexpr auto v1 =
        tfel::math::computeLinearInterpolationAndDerivative<true>(
            abscissae, values, time{-1});
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<decltype(std::get<0>(v1)), const stress&>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<decltype(std::get<1>(v1)), const stressrate&>));
    constexpr auto values1 = std::array<std::pair<stress, stressrate>, 5u>{
        tfel::math::computeLinearInterpolationAndDerivative<true>(
            abscissae, values, time{-1}),
        tfel::math::computeLinearInterpolationAndDerivative<true>(
            abscissae, values, time{0}),
        tfel::math::computeLinearInterpolationAndDerivative<true>(
            abscissae, values, time{0.5}),
        tfel::math::computeLinearInterpolationAndDerivative<true>(
            abscissae, values, time{1.5}),
        tfel::math::computeLinearInterpolationAndDerivative<true>(
            abscissae, values, time{3}),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(values1[0])) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(values1[0]) - stressrate{1}) <
                             10 * eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(values1[1]) - stress{1}) <
                             10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(values1[1]) - stressrate{1}) <
                             10 * eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(values1[2]) - stress{1.5}) <
                             10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(values1[2]) - stressrate{1}) <
                             10 * eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(values1[3]) - stress{3}) <
                             10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(values1[3]) - stressrate{2}) <
                             10 * eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(values1[4]) - stress{6}) <
                             10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(values1[4]) - stressrate{2}) <
                             10 * eps2);
    //
    constexpr auto values2 = std::array<std::pair<stress, stressrate>, 5u>{
        tfel::math::computeLinearInterpolationAndDerivative<false>(
            abscissae, values, time{-1}),
        tfel::math::computeLinearInterpolationAndDerivative<false>(
            abscissae, values, time{0}),
        tfel::math::computeLinearInterpolationAndDerivative<false>(
            abscissae, values, time{0.5}),
        tfel::math::computeLinearInterpolationAndDerivative<false>(
            abscissae, values, time{1.5}),
        tfel::math::computeLinearInterpolationAndDerivative<false>(
            abscissae, values, time{3}),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(values2[0]) - stress{1}) <
                             10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(values2[0])) < 10 * eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(values2[1]) - stress{1}) <
                             10 * eps);
    TFEL_TESTS_STATIC_ASSERT(
        (my_abs(std::get<1>(values2[1]) - stressrate{1}) < 10 * eps2) ||
        (my_abs(std::get<1>(values2[1])) < 10 * eps2));
    TFEL_TESTS_STATIC_ASSERT(
        (my_abs(std::get<0>(values2[2]) - stress{1.5}) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT(
        (my_abs(std::get<1>(values2[2]) - stressrate{1}) < 10 * eps2));
    TFEL_TESTS_STATIC_ASSERT(
        (my_abs(std::get<0>(values2[3]) - stress{3}) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT(
        (my_abs(std::get<1>(values2[3]) - stressrate{2}) < 10 * eps2));
    TFEL_TESTS_STATIC_ASSERT(
        (my_abs(std::get<0>(values2[4]) - stress{4}) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT((my_abs(std::get<1>(values2[4])) < 10 * eps2));
  }
#ifndef _MSC_VER
  void test6() {
    using tvector = tfel::math::tvector<3u, double>;
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    constexpr auto id = tvector{1, 1, 1};
    constexpr std::array<double, 3u> abscissae{0, 1, 2};
    constexpr std::array<tvector, 3u> values{id, 2 * id, 4 * id};
    constexpr auto vd =
        tfel::math::computeLinearInterpolationAndDerivative<false>(abscissae,
                                                                   values, 0.5);
    constexpr auto v = std::get<0>(vd);
    constexpr auto d = std::get<1>(vd);
    TFEL_TESTS_STATIC_ASSERT((my_abs(v[0] - 1.5) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT((my_abs(v[1] - 1.5) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT((my_abs(v[2] - 1.5) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT((my_abs(d[0] - 1) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT((my_abs(d[1] - 1) < 10 * eps));
    TFEL_TESTS_STATIC_ASSERT((my_abs(d[2] - 1) < 10 * eps));
  }
#endif /* _MSC_VER */
};

TFEL_TESTS_GENERATE_PROXY(LinearInterpolationTest, "LinearInterpolationTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LinearInterpolation.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
