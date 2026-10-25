/*!
 * \file   tests/Math/qt5.cxx
 * \brief  This file tests mathematical functions on references to quantities
 * \date 25/10/2026
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
#include <limits>
#include <cstdlib>
#include <iostream>
#include <type_traits>
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct QtReferenceTest final : public tfel::tests::TestCase {
  QtReferenceTest()
      : tfel::tests::TestCase("TFEL/Math", "qt5") {}  // end of QtReferenceTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  //! \brief absolute value of references to quantities
  void test1() {
    using namespace tfel::math;
    using stress = qt<unit::Stress, double>;
    using real = qt<unit::NoUnit, double>;
    constexpr auto eps = 1e-14;
    double values[6] = {-1, 2, 3, 4, 5, 6};
    const double* const cvalues = values;
    // element of a view
    auto s = map<stensor<3u, stress>>(values);
    const auto a1 = abs(s(0));
    TFEL_TESTS_STATIC_ASSERT((std::is_same_v<decltype(a1), const stress>));
    TFEL_TESTS_ASSERT(std::abs(a1.getValue() - 1) < eps);
    // element of a read-only view
    const auto cs = map<const stensor<3u, stress>>(cvalues);
    const auto a2 = abs(cs(0));
    TFEL_TESTS_STATIC_ASSERT((std::is_same_v<decltype(a2), const stress>));
    TFEL_TESTS_ASSERT(std::abs(a2.getValue() - 1) < eps);
    // scalar views
    auto p = map<stress>(values);
    const auto a3 = abs(p);
    TFEL_TESTS_STATIC_ASSERT((std::is_same_v<decltype(a3), const stress>));
    TFEL_TESTS_ASSERT(std::abs(a3.getValue() - 1) < eps);
    auto e = map<real>(values);
    const auto a4 = abs(e);
    TFEL_TESTS_STATIC_ASSERT((std::is_same_v<decltype(a4), const real>));
    TFEL_TESTS_ASSERT(std::abs(a4.getValue() - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(abs(s(1)).getValue() - 2) < eps);
  }  // end of test1
  //! \brief IEEE754 functions on read-only references to quantities
  void test2() {
    using namespace tfel::math;
    using stress = qt<unit::Stress, double>;
    const double values[2] = {1, std::numeric_limits<double>::quiet_NaN()};
    const auto s = map<const tvector<2u, stress>>(values);
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<decltype(s(0)), const_qt_ref<unit::Stress, double>>));
    TFEL_TESTS_ASSERT(ieee754::isfinite(s(0)));
    TFEL_TESTS_ASSERT(!ieee754::isnan(s(0)));
    TFEL_TESTS_ASSERT(ieee754::fpclassify(s(0)) == FP_NORMAL);
    TFEL_TESTS_ASSERT(!ieee754::isfinite(s(1)));
    TFEL_TESTS_ASSERT(ieee754::isnan(s(1)));
    TFEL_TESTS_ASSERT(ieee754::fpclassify(s(1)) == FP_NAN);
  }  // end of test2
};

TFEL_TESTS_GENERATE_PROXY(QtReferenceTest, "QtReferenceTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qt5.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
