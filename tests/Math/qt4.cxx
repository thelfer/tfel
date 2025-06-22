/*!
 * \file   tests/Math/qt4.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/11/2021
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
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct StensorQtTest final : public tfel::tests::TestCase {
  StensorQtTest()
      : tfel::tests::TestCase("TFEL/Math", "qt4") {}  // end of qtTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute
  void test1() {
    using namespace tfel::math;
    using stress = qt<Stress, double>;
    constexpr const auto eps = 1e-14;
    constexpr auto v1 = stress{-12};
    constexpr auto v2 = -v1;
    TFEL_TESTS_STATIC_ASSERT((std::is_same_v<decltype(v2), const stress>));
    TFEL_TESTS_STATIC_ASSERT(my_abs(v2.getValue() - 12) < eps);
  }  // end of test2
  void test2() {
    using namespace tfel::math;
    using real = qt<NoUnit, double>;
    constexpr const auto eps = 1e-14;
    const stensor<3u, real> s = -stensor<3u, real>::Id();
    TFEL_TESTS_ASSERT(my_abs(s[0].getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s[1].getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s[2].getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s[3].getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s[4].getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s[5].getValue()) < eps);
  }  // end of test2
  void test3() {
    using namespace tfel::math;
    using real = qt<NoUnit, double>;
    constexpr const auto eps = 1e-14;
    const st2tost2<1u, real> s = -st2tost2<1u, real>::Id();
    TFEL_TESTS_ASSERT(my_abs(s(0, 0).getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(0, 1).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(0, 2).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(1, 1).getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(1, 0).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(1, 2).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(2, 2).getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(2, 0).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(2, 1).getValue()) < eps);
  }  // end of test3
  void test4() {
    using namespace tfel::math;
    using real = qt<NoUnit, double>;
    constexpr const auto eps = 1e-14;
    const t2tot2<1u, real> s = -t2tot2<1u, real>::Id();
    TFEL_TESTS_ASSERT(my_abs(s(0, 0).getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(0, 1).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(0, 2).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(1, 1).getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(1, 0).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(1, 2).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(2, 2).getValue() + 1) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(2, 0).getValue()) < eps);
    TFEL_TESTS_ASSERT(my_abs(s(2, 1).getValue()) < eps);
  }  // end of test4
};

TFEL_TESTS_GENERATE_PROXY(StensorQtTest, "StensorQt");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qt4.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
