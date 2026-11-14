/*!
 * \file   tests/Math/tvector2.cxx
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
#endif /* NDEBUG */

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

struct TinyVectorTest2 final : public tfel::tests::TestCase {
  TinyVectorTest2()
      : tfel::tests::TestCase("TFEL/Math", "TinyVectorTest2") {
  }  // end of TinyVectorTest2
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 1.e-14;
    tvector<2> v;
    tvector<2> v2;
    tmatrix<2, 2> m(0.);
    v(0) = 1.2;
    v(1) = 2.;
    m(0, 0) = 1.;
    m(0, 1) = 0.5;
    m(1, 0) = 0.5;
    m(1, 1) = 1.;
    v2 = m * v;
    TFEL_TESTS_ASSERT(std::abs(v2(0) - 2.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(v2(1) - 2.6) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TinyVectorTest2, "TinyVectorTest2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("tvector2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
