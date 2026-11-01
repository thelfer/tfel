/*!
 * \file   tests/Math/tmatrix2.cxx
 * \brief
 * \author Thomas Helfer
 * \date 12/04/2008
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

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

struct TinyMatrixTest2 final : public tfel::tests::TestCase {
  TinyMatrixTest2()
      : tfel::tests::TestCase("TFEL/Math", "TinyMatrixTest2") {
  }  // end of TinyMatrixTest2
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 1.e-14;
    tmatrix<2, 2, double> m;
    tvector<2, double> v;
    tvector<2, double> v2;
    m(0, 0) = 0.;
    m(0, 1) = 1.;
    m(1, 0) = 2.;
    m(1, 1) = 3.;
    v(0) = 0.5;
    v(1) = 0.25;
    v2 = v * m;
    // v2(j) = v(i) m(i, j)
    TFEL_TESTS_ASSERT(std::abs(v2(0) - 0.5) < eps);
    TFEL_TESTS_ASSERT(std::abs(v2(1) - 1.25) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TinyMatrixTest2, "TinyMatrixTest2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("tmatrix2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
