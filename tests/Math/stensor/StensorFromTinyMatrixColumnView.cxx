/*!
 * \file   tests/Math/StensorFromTinyMatrixColumnView.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 16/10/2008
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
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

struct StensorFromTinyMatrixColumnViewTest final
    : public tfel::tests::TestCase {
  StensorFromTinyMatrixColumnViewTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "StensorFromTinyMatrixColumnViewTest") {
  }  // end of StensorFromTinyMatrixColumnViewTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 1.e-14;
    tmatrix<3, 2> m(0.);
    auto ms = map_derivative<0, 0, stensor<1>, double>(m);
    auto ms2 = map_derivative<0, 1, stensor<1>, double>(m);
    stensor<1> s(1.);
    ms(0) = 1.;
    ms(1) = 2.;
    ms(2) = 3.;
    s += ms;
    ms2 = ms + 2. * s;
    // the first column of m holds ms, the second one ms2
    for (unsigned short i = 0; i != 3; ++i) {
      TFEL_TESTS_ASSERT(std::abs(m(i, 0) - (i + 1)) < eps);
      TFEL_TESTS_ASSERT(std::abs(s(i) - (i + 2)) < eps);
      TFEL_TESTS_ASSERT(std::abs(m(i, 1) - (3 * i + 5)) < eps);
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(StensorFromTinyMatrixColumnViewTest,
                          "StensorFromTinyMatrixColumnViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StensorFromTinyMatrixColumnView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
