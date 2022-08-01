/*!
 * \file   CubicSplineInterpolationTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/07/2022
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
};

TFEL_TESTS_GENERATE_PROXY(CubicSplineInterpolationTest, "CubicSplineInterpolationTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CubicSplineInterpolation.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
