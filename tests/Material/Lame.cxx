/*!
 * \file   tests/Material/Lame.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/06/2021
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

#include <limits>
#include <cstdlib>
#include <iostream>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Material/Lame.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct LameTest final : public tfel::tests::TestCase {
  LameTest()
      : tfel::tests::TestCase("TFEL/Material", "Lame") {}  // end of LameTest
  tfel::tests::TestResult execute() override {
    this->template test1<float, false>();
    this->template test1<double, false>();
    this->template test1<long double, false>();
    this->template test1<float, true>();
    this->template test1<double, true>();
    this->template test1<long double, true>();
    return this->result;
  }

 private:
  template <typename NumericType, bool use_qt>
  void test1() {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = typename tfel::config::Types<1u, NumericType, use_qt>::real;
    constexpr auto eps = 10 * std::numeric_limits<stress>::epsilon();
    constexpr auto young = stress{150e9};
    constexpr auto nu = real{0.3};
    constexpr auto l = tfel::material::computeLambda(young, nu);
    constexpr auto m = tfel::material::computeMu(young, nu);
    constexpr auto l_ref = young * nu / ((1 + nu) * (1 - 2 * nu));
    constexpr auto m_ref = young / (2 * (1 + nu));
    TFEL_TESTS_STATIC_ASSERT(my_abs(l - l_ref) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(m - m_ref) < eps);
  }
};  // end of struct LameTest

TFEL_TESTS_GENERATE_PROXY(LameTest, "Lame");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Lame.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
