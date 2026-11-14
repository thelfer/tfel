/*!
 * \file   tests/Math/tvector3.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 08/04/2008
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

struct TinyVectorTest3 final : public tfel::tests::TestCase {
  TinyVectorTest3()
      : tfel::tests::TestCase("TFEL/Math", "TinyVectorTest3") {
  }  // end of TinyVectorTest3
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 1.e-14;
    tvector<2> v1;
    tvector<2> v2;
    tmatrix<2, 2> m;
    v1(0) = 0.5;
    v1(1) = 0.25;
    v2(0) = 4.;
    v2(1) = 2.;
    m = v1 ^ v2;
    // m(i, j) = v1(i) v2(j)
    TFEL_TESTS_ASSERT(std::abs(m(0, 0) - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 1) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 0) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 1) - 0.5) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TinyVectorTest3, "TinyVectorTest3");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("tvector3.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
