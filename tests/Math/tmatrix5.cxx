/*!
 * \file  tests/Math/tmatrix5.cxx
 * \brief
 * \author Helfer Thomas
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

struct TMatrixRowViewTest final : public tfel::tests::TestCase {
  TMatrixRowViewTest()
      : tfel::tests::TestCase("TFEL/Math", "TMatrixRowViewTest") {
  }  // end of TMatrixRowViewTest

  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    tmatrix<2u, 3u> m;
    m(0, 0) = 1.;
    m(0, 1) = 3.;
    m(0, 2) = 4.;
    m(1, 0) = -2.;
    m(1, 1) = 2.;
    m(1, 2) = -1.;
    tvector<3u> r0 = (static_cast<const tmatrix<2u, 3u>&>(m)).row_view<0>();
    tvector<3u> r1 = (static_cast<const tmatrix<2u, 3u>&>(m)).row_view<1>();
    m.row_view<0>() = 2. * r0;
    m.row_view<1>() = 3. * r1;
    const double eps = 1.e-14;
    TFEL_TESTS_ASSERT(abs(m(0, 0) - 2.) < eps);
    TFEL_TESTS_ASSERT(abs(m(0, 1) - 6.) < eps);
    TFEL_TESTS_ASSERT(abs(m(0, 2) - 8.) < eps);
    TFEL_TESTS_ASSERT(abs(m(1, 0) + 6.) < eps);
    TFEL_TESTS_ASSERT(abs(m(1, 1) - 6.) < eps);
    TFEL_TESTS_ASSERT(abs(m(1, 2) + 3.) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TMatrixRowViewTest, "TMatrixRowViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("TMatrixRowView.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
