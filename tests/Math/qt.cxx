/*!
 * \file   tests/Math/qt.cxx
 * \brief
 *
 * \author Helfer thomas
 * \date   06 Jun 2006
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
#include "TFEL/Math/qt.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct qtTest final : public tfel::tests::TestCase {
  qtTest() : tfel::tests::TestCase("TFEL/Math", "qt") {}  // end of qtTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    this->test6();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math;
    constexpr qt<Mass> m1(100.);
    constexpr qt<Mass> m2(100.);
    constexpr qt<Mass> m3 = m1 + 0.5 * m2;
    constexpr qt<Acceleration, unsigned short> a(2);
    constexpr qt<Force> f = m1 * a;
    const auto value = qt<NoUnit>(3);
    TFEL_TESTS_STATIC_ASSERT(!isQuantity<double>());
    TFEL_TESTS_STATIC_ASSERT(isQuantity<qt<Mass>>());
    TFEL_TESTS_STATIC_ASSERT(my_abs(m3.getValue() - 150.) < 1.e-14);
    TFEL_TESTS_STATIC_ASSERT(my_abs(f.getValue() - 200.) < 1.e-14);
    TFEL_TESTS_ASSERT(
        (my_abs(std::cos(qt<NoUnit>(12.)) - std::cos(12.)) < 1.e-14));
    TFEL_TESTS_ASSERT((my_abs(pow(value, 3.) - 27) < 1.e-14));
  }  // end of test1
  void test2() {
    using namespace tfel::math;
    using RootSquareMass = UnaryResultType<qt<Mass>, Power<1, 2>>::type;
    constexpr RootSquareMass v1(1.5);
    constexpr qt<Mass> v2 = v1 * v1;
    TFEL_TESTS_STATIC_ASSERT(my_abs(v2.getValue() - 2.25) < 1.e-14);
  }  // end of test2
  void test3() {
    using namespace tfel::math;
    constexpr qt<Mass, int> a(-12);
    constexpr qt<Mass, int> b(14);
    TFEL_TESTS_STATIC_ASSERT(a <= b);
    TFEL_TESTS_STATIC_ASSERT(a < b);
    TFEL_TESTS_STATIC_ASSERT(b > a);
    TFEL_TESTS_STATIC_ASSERT(b >= a);
    TFEL_TESTS_STATIC_ASSERT(b != a);
    TFEL_TESTS_STATIC_ASSERT(!(b == a));
    TFEL_TESTS_STATIC_ASSERT(abs(a).getValue() == 12);
  }  // end of test3
  void test4() {
    using namespace tfel::math;
    using time = qt<Time, double>;
    auto time_value = double(1);
    auto getTime = [&time_value]() -> double& { return time_value; };
    auto t = time(getTime());
    TFEL_TESTS_ASSERT(my_abs(t.getValue() - 1) < 1.e-14);
  }
  //! Test if implicit conversion works as expected
  void test5() {
    using namespace tfel::math;
    auto d = [](const qt<NoUnit, double>& v) constexpr {
      return 2 * v.getValue();
    };
    TFEL_TESTS_STATIC_ASSERT(my_abs(d(12) - 24) < 1.e-14);
  }
  // tests related to class template argument deduction
  void test6() {
    using namespace tfel::math;
    using time = qt<Time>;
    constexpr Quantity t = time{1.2};
    TFEL_TESTS_STATIC_ASSERT((std::is_same_v<decltype(t), const time>));
    TFEL_TESTS_STATIC_ASSERT(my_abs(t.getValue() - 1.2) < 1e-15);
  }
};

TFEL_TESTS_GENERATE_PROXY(qtTest, "qtTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qt.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
