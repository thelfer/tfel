/*!
 * \file  tests/Math/tmatrix6.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 27 déc. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tmatrix.hxx"

struct TMatrixSubMatrixViewTest final : public tfel::tests::TestCase {
  TMatrixSubMatrixViewTest()
      : tfel::tests::TestCase("TFEL/Math", "TMatrixSubMatrixViewTest") {
  }  // end of TMatrixSubMatrixViewTest

  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-14;
    tmatrix<2u, 3u> m;
    m(0, 0) = 1.;
    m(0, 1) = 3.;
    m(0, 2) = 4.;
    m(1, 0) = -2.;
    m(1, 1) = 2.;
    m(1, 2) = -1.;
    tmatrix<1u, 2u> m2;
    m2 = m.submatrix_view<1, 1, 1, 2>();
    TFEL_TESTS_ASSERT(abs(m2(0, 0) - 2.) < eps);
    TFEL_TESTS_ASSERT(abs(m2(0, 1) + 1.) < eps);
    m2 *= 4;
    m.submatrix_view<0, 0, 1, 2>() = m2;
    TFEL_TESTS_ASSERT(abs(m(0, 0) - 8.) < eps);
    TFEL_TESTS_ASSERT(abs(m(0, 1) + 4.) < eps);
    TFEL_TESTS_ASSERT(abs(m(0, 2) - 4.) < eps);
    TFEL_TESTS_ASSERT(abs(m(1, 0) + 2.) < eps);
    TFEL_TESTS_ASSERT(abs(m(1, 1) - 2.) < eps);
    TFEL_TESTS_ASSERT(abs(m(1, 2) + 1.) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TMatrixSubMatrixViewTest, "TMatrixSubMatrixViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("TMatrixSubMatrixView.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
