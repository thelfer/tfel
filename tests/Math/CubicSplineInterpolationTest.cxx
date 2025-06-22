/*!
 * \file   CubicSplineInterpolationTest.cxx
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
#include <cstdlib>
#include <iostream>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/Quantity/qtIO.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/CubicSpline.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct CubicSplineInterpolationTest final : public tfel::tests::TestCase {
  CubicSplineInterpolationTest()
      : tfel::tests::TestCase("TFEL/Math", "CubicSplineInterpolationTest") {
  }  // end of CubicSplineInterpolationTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    using Point = tfel::math::CubicSplineCollocationPoint<double, double>;
    constexpr auto points = std::array<Point, 1u>{Point{1, 50, 0}};
    constexpr auto v1 =
        tfel::math::computeCubicSplineInterpolation<true>(points, 0);
    constexpr auto v2 =
        tfel::math::computeCubicSplineInterpolation<false>(points, 0);
    TFEL_TESTS_STATIC_ASSERT(my_abs(v1 - 50) < 10 * eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(v2 - 50) < 10 * eps);
  }
  void test2() {
    using CollocationPoint =
        tfel::math::CubicSplineCollocationPoint<double, double>;
    constexpr auto eps = 10 * std::numeric_limits<double>::epsilon();
    constexpr auto pts = std::array<CollocationPoint, 3u>{
        CollocationPoint{0, 1, 0.75}, CollocationPoint{1, 2, 1.5},
        CollocationPoint{2, 4, 2.25}};
    //
    constexpr auto f_values = std::array<double, 4u>{
        tfel::math::computeCubicSplineInterpolation<true>(pts, -1),
        tfel::math::computeCubicSplineInterpolation<true>(pts, 0.4),
        tfel::math::computeCubicSplineInterpolation<true>(pts, 1.2),
        tfel::math::computeCubicSplineInterpolation<true>(pts, 3),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[0] - 0.25) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[1] - 1.316) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[2] - 2.328) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[3] - 6.25) < eps);
    //
    constexpr auto fdf_values = std::array<std::pair<double, double>, 4u>{
        tfel::math::computeCubicSplineInterpolationAndDerivative<true>(pts, -1),
        tfel::math::computeCubicSplineInterpolationAndDerivative<true>(pts,
                                                                       0.4),
        tfel::math::computeCubicSplineInterpolationAndDerivative<true>(pts,
                                                                       1.2),
        tfel::math::computeCubicSplineInterpolationAndDerivative<true>(pts, 3),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(fdf_values[0]) - 0.25) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(fdf_values[0]) - 0.75) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(fdf_values[1]) - 1.316) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(fdf_values[1]) - 0.87) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(fdf_values[2]) - 2.328) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(fdf_values[2]) - 1.77) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(fdf_values[3]) - 6.25) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<1>(fdf_values[3]) - 2.25) < eps);
  }
  void test3() {
#if (not defined __INTEL_COMPILER) && (not defined __clang__)
    using time = tfel::math::qt<tfel::math::Time, double>;
    using stress = tfel::math::qt<tfel::math::Stress, double>;
    using stressrate = tfel::math::derivative_type<stress, time>;
    using CollocationPoint =
        tfel::math::CubicSplineCollocationPoint<time, stress>;
    using tfel::math::computeCubicSplineInterpolationAndDerivative;
    constexpr auto eps1 = 10 * std::numeric_limits<stress>::epsilon();
    constexpr auto eps2 = 10 * std::numeric_limits<stressrate>::epsilon();
    constexpr auto pts = std::array<CollocationPoint, 3u>{
        CollocationPoint{time{0}, stress{1}, stressrate{0.75}},  //
        CollocationPoint{time{1}, stress{2}, stressrate{1.5}},   //
        CollocationPoint{time{2}, stress{4}, stressrate{2.25}}};
    //
    constexpr auto f_values = std::array<stress, 4u>{
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{-1}),
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{0.4}),
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{1.2}),
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{3}),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[0] - stress{0.25}) < eps1);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[1] - stress{1.316}) < eps1);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[2] - stress{2.328}) < eps1);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[3] - stress{6.25}) < eps1);
    //
    constexpr auto fdf_values = std::array<std::pair<stress, stressrate>, 4u>{
        computeCubicSplineInterpolationAndDerivative<true>(pts, time{-1}),
        computeCubicSplineInterpolationAndDerivative<true>(pts, time{0.4}),
        computeCubicSplineInterpolationAndDerivative<true>(pts, time{1.2}),
        computeCubicSplineInterpolationAndDerivative<true>(pts, time{3}),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(fdf_values[0]) - stress{0.25}) <
                             eps1);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[0]) - stressrate{0.75}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[1]) - stress{1.316}) < eps1);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[1]) - stressrate{0.87}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[2]) - stress{2.328}) < eps1);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[2]) - stressrate{1.77}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(std::get<0>(fdf_values[3]) - stress{6.25}) <
                             eps1);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[3]) - stressrate{2.25}) < eps2);
#endif /* __clang__ */
  }
  void test4() {
#if (not defined __INTEL_COMPILER) && (not defined __clang__)
    using time = tfel::math::qt<tfel::math::Time, double>;
    using stress = tfel::math::qt<tfel::math::Stress, double>;
    using stressrate = tfel::math::derivative_type<stress, time>;
    using StressArray = tfel::math::fsarray<2u, stress>;
    using StressRateArray = tfel::math::fsarray<2u, stressrate>;
    using CollocationPoint = tfel::math::CubicSplineCollocationPoint<
        time, tfel::math::fsarray<2u, stress>>;
    using tfel::math::computeCubicSplineInterpolationAndDerivative;
    constexpr auto eps = 10 * std::numeric_limits<stress>::epsilon();
    constexpr auto eps2 = 10 * std::numeric_limits<stressrate>::epsilon();
    constexpr auto pts = std::array<CollocationPoint, 3u>{
        CollocationPoint{time{0}, StressArray{stress{1}, stress{1}},
                         StressRateArray{stressrate{0.75}, stressrate{0.75}}},
        CollocationPoint{time{1}, StressArray{stress{2}, stress{2}},
                         StressRateArray{stressrate{1.5}, stressrate{1.5}}},
        CollocationPoint{time{2}, StressArray{stress{4}, stress{4}},
                         StressRateArray{stressrate{2.25}, stressrate{2.25}}}};
    //
    constexpr auto f_values = std::array<StressArray, 4u>{
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{-1}),
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{0.4}),
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{1.2}),
        tfel::math::computeCubicSplineInterpolation<true>(pts, time{3}),
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[0][0] - stress{0.25}) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[0][1] - stress{0.25}) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[1][0] - stress{1.316}) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[1][1] - stress{1.316}) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[2][0] - stress{2.328}) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[2][1] - stress{2.328}) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[3][0] - stress{6.25}) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f_values[3][1] - stress{6.25}) < eps);
    //
    constexpr auto fdf_values =
        std::array<std::pair<StressArray, StressRateArray>, 4u>{
            computeCubicSplineInterpolationAndDerivative<true>(pts, time{-1}),
            computeCubicSplineInterpolationAndDerivative<true>(pts, time{0.4}),
            computeCubicSplineInterpolationAndDerivative<true>(pts, time{1.2}),
            computeCubicSplineInterpolationAndDerivative<true>(pts, time{3}),
        };
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[0])[0] - stress{0.25}) < eps);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[0])[1] - stress{0.25}) < eps);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[1])[0] - stress{1.316}) < eps);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[1])[1] - stress{1.316}) < eps);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[2])[0] - stress{2.328}) < eps);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[2])[1] - stress{2.328}) < eps);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[3])[0] - stress{6.25}) < eps);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<0>(fdf_values[3])[1] - stress{6.25}) < eps);
    //
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[0])[0] - stressrate{0.75}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[0])[1] - stressrate{0.75}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[1])[0] - stressrate{0.87}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[1])[1] - stressrate{0.87}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[2])[0] - stressrate{1.77}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[2])[1] - stressrate{1.77}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[3])[0] - stressrate{2.25}) < eps2);
    TFEL_TESTS_STATIC_ASSERT(
        my_abs(std::get<1>(fdf_values[3])[1] - stressrate{2.25}) < eps2);
#endif /* __clang__ */
  }
};

TFEL_TESTS_GENERATE_PROXY(CubicSplineInterpolationTest,
                          "CubicSplineInterpolationTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CubicSplineInterpolation.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
