/*!
 * \file   tests/Math/tvector5.cxx
 * \brief
 * \author Thomas Helfer
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

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"

struct TVectorTest5 final : public tfel::tests::TestCase {
  TVectorTest5()
      : tfel::tests::TestCase("TFEL/Math", "TVectorTesT5") {
  }  // end of TVectorTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute

 private:
  void test1() {
    constexpr auto eps = double(1.e-14);
    const double v1_values[3u] = {1.2, -4.2, -0.3};
    tfel::math::tvector<3u, double> v1(v1_values);
    for (unsigned short i = 0; i != 3; ++i) {
      TFEL_TESTS_ASSERT(std::abs(v1[i] - v1_values[i]) < eps);
    }
  }  // end of test1
  void test2() {
    constexpr auto eps = double(1.e-14);
    const tfel::math::tvector<3u, double> v1{1.2, -4.2, -0.3};
    const auto [a, b, c] = v1;
    TFEL_TESTS_ASSERT(std::abs(a - 1.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(b + 4.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(c + 0.3) < eps);
  }  // end of test2
  void test3() {
    constexpr auto eps = double(1.e-14);
    tfel::math::fsarray<3u, double> v1(0);
    auto& [a, b, c] = v1;
    a = 1.2;
    b = -4.2;
    c = -0.3;
    TFEL_TESTS_ASSERT(std::abs(v1[0] - 1.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(v1[1] + 4.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(v1[2] + 0.3) < eps);
  }  // end of test3
};

TFEL_TESTS_GENERATE_PROXY(TVectorTest5, "TVectorTest5");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("tvector5.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
