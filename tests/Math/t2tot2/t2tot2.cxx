/*!
 * \file   tests/Math/t2tot2/t2tot2.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30 jan 2007
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
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct T2toT2Test_1D final : public tfel::tests::TestCase {
  T2toT2Test_1D()
      : tfel::tests::TestCase("TFEL/Math", "T2toT2Test-1D") {
  }  // end of STensorTest8
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace std;
    using namespace tfel::math;
    constexpr t2tot2<3> t1(1.5);
    constexpr t2tot2<3> t2(4.);
    constexpr t2tot2<3> t3 = t1 + 0.5 * t2;
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(0, 0) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(1, 0) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(2, 0) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(3, 0) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(0, 1) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(1, 1) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(2, 1) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(3, 1) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(0, 2) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(1, 2) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(2, 2) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(3, 2) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(0, 3) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(1, 3) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(2, 3) - 3.5) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(t3(3, 3) - 3.5) < 1.e-14);
  }
  void test2() {
    using namespace tfel::math;
    constexpr auto s = t2tot2{{1, 0, 2},  //
                              {-1, 1, 6},
                              {3, 4, 1}};
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<decltype(s), const t2tot2<1u, int>>));
    TFEL_TESTS_STATIC_ASSERT(s(0, 0) == 1);
    TFEL_TESTS_STATIC_ASSERT(s(0, 1) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(0, 2) == 2);
    TFEL_TESTS_STATIC_ASSERT(s(1, 0) == -1);
    TFEL_TESTS_STATIC_ASSERT(s(1, 1) == 1);
    TFEL_TESTS_STATIC_ASSERT(s(1, 2) == 6);
    TFEL_TESTS_STATIC_ASSERT(s(2, 0) == 3);
    TFEL_TESTS_STATIC_ASSERT(s(2, 1) == 4);
    TFEL_TESTS_STATIC_ASSERT(s(2, 2) == 1);
  }
};

struct T2toT2Test_2D final : public tfel::tests::TestCase {
  T2toT2Test_2D()
      : tfel::tests::TestCase("TFEL/Math", "T2toT2Test-2D") {
  }  // end of STensorTest8
  tfel::tests::TestResult execute() override {
    this->test1();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math;
    constexpr auto s = t2tot2{{1, 0, 2, 0, 0},  //
                              {-1, 1, 6, 0, 0},
                              {3, 4, 1, 0, 0},
                              {0, 0, 0, 1, 0},
                              {0, 0, 0, 0, -1}};
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<decltype(s), const t2tot2<2u, int>>));
    TFEL_TESTS_STATIC_ASSERT(s(0, 0) == 1);
    TFEL_TESTS_STATIC_ASSERT(s(0, 1) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(0, 2) == 2);
    TFEL_TESTS_STATIC_ASSERT(s(0, 3) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(0, 4) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(1, 0) == -1);
    TFEL_TESTS_STATIC_ASSERT(s(1, 1) == 1);
    TFEL_TESTS_STATIC_ASSERT(s(1, 2) == 6);
    TFEL_TESTS_STATIC_ASSERT(s(1, 3) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(1, 4) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(2, 0) == 3);
    TFEL_TESTS_STATIC_ASSERT(s(2, 1) == 4);
    TFEL_TESTS_STATIC_ASSERT(s(2, 2) == 1);
    TFEL_TESTS_STATIC_ASSERT(s(2, 3) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(2, 3) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(2, 4) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(3, 0) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(3, 1) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(3, 2) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(3, 3) == 1);
    TFEL_TESTS_STATIC_ASSERT(s(3, 4) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(4, 0) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(4, 1) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(4, 2) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(4, 3) == 0);
    TFEL_TESTS_STATIC_ASSERT(s(4, 4) == -1);
  }
};

TFEL_TESTS_GENERATE_PROXY(T2toT2Test_1D, "T2toT2Test-1D");
TFEL_TESTS_GENERATE_PROXY(T2toT2Test_2D, "T2toT2Test-2D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("t2tot2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
