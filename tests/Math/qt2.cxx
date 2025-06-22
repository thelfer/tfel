/*!
 * \file   tests/Math/qt2.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   18 Jul 2006
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
    this->test4();
    this->test5();
    this->test6();
    this->test7();
    this->test8();
    this->test9();
    return this->result;
  }  // end of execute
  void test1() {
    using namespace tfel::math;
    constexpr qt<Mass> m3 = []() constexpr {
      auto v1 = double{50.};
      auto v2 = double{100.};
      qt_ref<Mass> m1(v1);
      qt_ref<Mass> m2(v2);
      return m1 + 0.5 * m2;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(my_abs(m3.getValue() - 100.) < 1.e-14);
  }  // end of test1
  void test2() {
    using namespace tfel::math;
    constexpr qt<Mass> m3 = []() constexpr {
      auto v1 = double{50.};
      auto v2 = double{100.};
      qt_ref<Mass> m1(v1);
      qt_ref<Mass> m2(v2);
      return m1 + 0.5 * m2;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(my_abs(m3.getValue() - 100.) < 1.e-14);
  }  // end of test2
  void test3() {
    using namespace tfel::math;
    constexpr qt<Mass> m3 = []() constexpr {
      const auto v1 = double{50.};
      const auto v2 = double{100.};
      const_qt_ref<Mass> m1(v1);
      const_qt_ref<Mass> m2(v2);
      return m1 + 0.5 * m2;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(my_abs(m3.getValue() - 100.) < 1.e-14);
  }  // end of test3
  void test4() {
    using namespace tfel::math;
    constexpr qt<Mass> m3 = []() constexpr {
      auto v1 = double{50.};
      const auto v2 = double{100.};
      qt_ref<Mass> m1(v1);
      const_qt_ref<Mass> m2(v2);
      return m1 + 0.5 * m2;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(my_abs(m3.getValue() - 100.) < 1.e-14);
  }  // end of test4
  void test5() {
    using namespace tfel::math;
    constexpr qt<Mass> m3 = []() constexpr {
      auto increase_mass = [](const qt<Mass>& m) constexpr { return 2 * m; };
      auto v1 = double{50.};
      const_qt_ref<Mass> m1(v1);
      return increase_mass(m1);
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(my_abs(m3.getValue() - 100.) < 1.e-14);
  }  // end of test5
  void test6() {
#if (not defined __INTEL_COMPILER) && (not defined __clang__)
    using namespace tfel::math;
    using strain = qt<NoUnit>;
    constexpr auto eeps = strain{1e-14};
    constexpr auto dp = strain(1e-3);
    constexpr auto n = stensor<3u, strain>{
        strain{1}, strain{-0.5}, strain{-0.5}, strain{0}, strain{0}, strain{0}};
    const auto devp = dp * n;
    TFEL_TESTS_ASSERT(my_abs(devp(0) - dp) < eeps);
    TFEL_TESTS_ASSERT(my_abs(2 * devp(1) + dp) < eeps);
    TFEL_TESTS_ASSERT(my_abs(2 * devp(2) + dp) < eeps);
    TFEL_TESTS_ASSERT(my_abs(devp(3)) < eeps);
    TFEL_TESTS_ASSERT(my_abs(devp(4)) < eeps);
    TFEL_TESTS_ASSERT(my_abs(devp(5)) < eeps);
#endif /* (not defined __INTEL_COMPILER) && (not defined __clang__) */
  }
  void test7() {
    using namespace tfel::math;
    constexpr auto eps = 1e-14;
    constexpr auto value = []() constexpr {
      constexpr qt<Mass> m(100.);
      constexpr qt<Acceleration> a(2);
      auto vf = double{};
      auto f = qt_ref<Force>(vf);
      f = m * a;
      return vf;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(my_abs(value - 200.) < eps);
  }
  void test8() {
    using namespace tfel::math;
    constexpr auto eps = 1e-14;
    constexpr qt<NoUnit> m(100.);
    constexpr double m_value = m;
    TFEL_TESTS_STATIC_ASSERT(my_abs(m_value - 100.) < eps);
  }
  void test9() {
    using namespace tfel::math;
    constexpr auto eps = 1e-14;
    constexpr qt<NoUnit> q(1.2);
    const auto cos_q = std::cos(q);
    TFEL_TESTS_ASSERT(std::abs(cos_q - std::cos(1.2)) < eps);
  }
};

TFEL_TESTS_GENERATE_PROXY(QtRefTest, "QtRefTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qt2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
