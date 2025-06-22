/*!
 * \file   tests/Math/stensor7.cxx
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

#include <cassert>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/stensor.hxx"

struct STensorTest7 final : public tfel::tests::TestCase {
  STensorTest7()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest7") {
  }  // end of STensorTest7
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-12;
    const double sqrt2 = sqrt(2.);
    const double v[6] = {8.2, 4.5, 7.2, 2.3 * sqrt2, 5.4 * sqrt2, 1.9 * sqrt2};
    stensor<3u, double> s(v);
    tvector<3u, double> vp;
    tmatrix<3u, 3u, double> m;
    s.computeEigenVectors(vp, m);
    TFEL_TESTS_ASSERT(abs(vp[0] - 14.05272884116390) < eps);
    TFEL_TESTS_ASSERT(abs(vp[1] - 2.26200044044714) < eps);
    TFEL_TESTS_ASSERT(abs(vp[2] - 3.58527071838896) < eps);
    // logarithm
    // values were compouted using the logm function of GNU Octave
    stensor<3u, double> sl1 = logarithm(s);
    stensor<3u, double> sl2 =
        stensor<3u, double>::buildLogarithmFromEigenValuesAndVectors(vp, m);
    TFEL_TESTS_ASSERT(abs(sl1(0) - 1.741738293015791) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(1) - 1.393218028542997) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(2) - 1.600943836737604) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(3) - 0.320395103076525 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(4) - 0.847053247774536 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(5) - 0.226951092162203 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(0) - sl2(0)) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(1) - sl2(1)) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(2) - sl2(2)) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(3) - sl2(3)) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(4) - sl2(4)) < eps);
    TFEL_TESTS_ASSERT(abs(sl1(5) - sl2(5)) < eps);
    // positive and neagtive part
    // since s has only positive eigen value, sp and s are equal.
    stensor<3u, double> sp1 = positive_part(s);
    stensor<3u, double> sp2 =
        stensor<3u, double>::buildPositivePartFromEigenValuesAndVectors(vp, m);
    stensor<3u, double> sn1 = negative_part(s);
    stensor<3u, double> sn2 =
        stensor<3u, double>::buildNegativePartFromEigenValuesAndVectors(vp, m);
    TFEL_TESTS_ASSERT(abs(s(0) - sp1(0) - sn1(0)) < eps);
    TFEL_TESTS_ASSERT(abs(s(1) - sp1(1) - sn1(1)) < eps);
    TFEL_TESTS_ASSERT(abs(s(2) - sp1(2) - sn1(2)) < eps);
    TFEL_TESTS_ASSERT(abs(s(3) - sp1(3) - sn1(3)) < eps);
    TFEL_TESTS_ASSERT(abs(s(4) - sp1(4) - sn1(4)) < eps);
    TFEL_TESTS_ASSERT(abs(s(5) - sp1(5) - sn1(5)) < eps);
    TFEL_TESTS_ASSERT(abs(sp1(0) - sp2(0)) < eps);
    TFEL_TESTS_ASSERT(abs(sp1(1) - sp2(1)) < eps);
    TFEL_TESTS_ASSERT(abs(sp1(2) - sp2(2)) < eps);
    TFEL_TESTS_ASSERT(abs(sp1(3) - sp2(3)) < eps);
    TFEL_TESTS_ASSERT(abs(sp1(4) - sp2(4)) < eps);
    TFEL_TESTS_ASSERT(abs(sp1(5) - sp2(5)) < eps);
    TFEL_TESTS_ASSERT(abs(sn1(0) - sn2(0)) < eps);
    TFEL_TESTS_ASSERT(abs(sn1(1) - sn2(1)) < eps);
    TFEL_TESTS_ASSERT(abs(sn1(2) - sn2(2)) < eps);
    TFEL_TESTS_ASSERT(abs(sn1(3) - sn2(3)) < eps);
    TFEL_TESTS_ASSERT(abs(sn1(4) - sn2(4)) < eps);
    TFEL_TESTS_ASSERT(abs(sn1(5) - sn2(5)) < eps);
    tvector<3u, double> vp_p;
    tvector<3u, double> vp_n;
    tmatrix<3u, 3u, double> m_p;
    tmatrix<3u, 3u, double> m_n;
    sp1.computeEigenVectors(vp_p, m_p);
    sn1.computeEigenVectors(vp_n, m_n);
    for (unsigned short i = 0; i != 3; ++i) {
      TFEL_TESTS_ASSERT(abs(vp_p(i) - max(vp(i), 0.)) < eps);
      TFEL_TESTS_ASSERT(abs(vp_n(i) - min(vp(i), 0.)) < eps);
      for (unsigned short j = 0; j != 3; ++j) {
        TFEL_TESTS_ASSERT(abs(m_p(i, j) - m(i, j)) < eps);
        if (i == j) {
          TFEL_TESTS_ASSERT(abs(m_n(i, j) - 1.) < eps);
        } else {
          TFEL_TESTS_ASSERT(abs(m_n(i, j) - 0.) < eps);
        }
      }
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTest7, "STensorTest7");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("stensor7.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
