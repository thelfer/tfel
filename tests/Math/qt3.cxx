/*!
 * \file   tests/Math/qt3.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/08/2006
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
#include <cassert>
#include <iostream>
#include <cstdlib>
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct QtRefTest final : public tfel::tests::TestCase {
  QtRefTest() : tfel::tests::TestCase("TFEL/Math", "qt") {}  // end of qtTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
  void test1() {
    using namespace tfel::math;
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<double, ViewStorageType<stensor<3u, double>>>));
    TFEL_TESTS_STATIC_ASSERT(
        (isQuantity<numeric_type<stensor<3u, qt<Mass>>>>()));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<qt<Mass>, numeric_type<stensor<3u, qt<Mass>>>>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<double, base_type<qt<Mass, double>>>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<double, ViewStorageType<stensor<3u, qt<Mass>>>>));
  }
  void test2() {
#if (not defined __INTEL_COMPILER) && (not defined __clang__)
    using namespace tfel::math;
    constexpr auto eps = double{1e-14};
    constexpr auto s = []() constexpr->stensor<3, qt<Stress, double>> {
      double stress_values[6] = {0, 1, 2, 3, 4, 5};
      return map<stensor<3, qt<Stress>>>(stress_values);
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[0].getValue()) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[1].getValue() - 1) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[2].getValue() - 2) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[3].getValue() - 3) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[4].getValue() - 4) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[5].getValue() - 5) < eps);
#endif /* (not defined __INTEL_COMPILER) && (not defined __clang__) */
  }    // end of test2
  void test3() {
    using namespace tfel::math;
    using stress = qt<Stress, double>;
    constexpr auto eps = double{1e-14};
    auto sig = stensor<3u, stress>{stress{0}, stress{1}, stress{2},
                                   stress{3}, stress{4}, stress{5}};
    double stress_values[6];
    map<stensor<3u, qt<Stress, double>>>(stress_values) = sig;
    TFEL_TESTS_ASSERT(my_abs(stress_values[0]) < eps);
    TFEL_TESTS_ASSERT(my_abs(stress_values[1] - 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(stress_values[2] - 2) < eps);
    TFEL_TESTS_ASSERT(my_abs(stress_values[3] - 3) < eps);
    TFEL_TESTS_ASSERT(my_abs(stress_values[4] - 4) < eps);
    TFEL_TESTS_ASSERT(my_abs(stress_values[5] - 5) < eps);
  }  // end of test3
};

TFEL_TESTS_GENERATE_PROXY(QtRefTest, "QtRefTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qt3.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
