/*!
 * \file   tests/Math/qt4.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/11/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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

struct StensorQtTest final : public tfel::tests::TestCase {
  StensorQtTest() : tfel::tests::TestCase("TFEL/Math", "qt4") {}  // end of qtTest
  tfel::tests::TestResult execute() override {
    this->test1();
    return this->result;
  }  // end of execute
  void test1() {
    using namespace tfel::math;
    using real = qt<NoUnit, double>;
    constexpr const auto eps = real{1e-14};
    constexpr stensor<3u, real> s = -stensor<3u, real>::Id();
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[0].getValue() + 1) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[1].getValue() + 1) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[2].getValue() + 1) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[3].getValue()) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[4].getValue()) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(s[5].getValue()) < eps);
  }    // end of test2
};

TFEL_TESTS_GENERATE_PROXY(StensorQtTest, "StensorQt");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qt4.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
