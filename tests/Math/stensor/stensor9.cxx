/*!
 * \file   tests/Math/stensor9.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23 dec. 2013
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

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

struct STensorTest9_3D final : public tfel::tests::TestCase {
  STensorTest9_3D()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest9-3D") {
  }  // end of STensorTest9
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 6.e-1;
    const double sqrt2 = sqrt(2.);
    const double values[6] = {-1, -2, 3, 4 * sqrt2, 6 * sqrt2, 5 * sqrt2};
    stensor<3u, double> s(values);
    stensor<3u, double> n0;
    stensor<3u, double> n1;
    stensor<3u, double> n2;
    st2tost2<3u, double> dn0;
    st2tost2<3u, double> dn1;
    st2tost2<3u, double> dn2;
    tvector<3u, double> vp;
    tmatrix<3u, 3u, double> m;
    s.computeEigenVectors(vp, m);
    stensor<3u, double>::computeEigenTensors(n0, n1, n2, m);
    stensor<3u, double>::computeEigenTensorsDerivatives(dn0, dn1, dn2, vp, m,
                                                        1.e-7);
    stensor<3u, double> exp_s(s.buildFromEigenValuesAndVectors(
        exp(vp(0)), exp(vp(1)), exp(vp(1)), m));
    st2tost2<3u, double> dexp;
    dexp = exp(vp(0)) * ((n0 ^ n0) + dn0) + exp(vp(1)) * ((n1 ^ n1) + dn1) +
           exp(vp(2)) * ((n2 ^ n2) + dn1);
    TFEL_TESTS_ASSERT(abs(exp_s(0) - 10.966e3) < eps);
    TFEL_TESTS_ASSERT(abs(exp_s(1) - 8.0687e3) < eps);
    TFEL_TESTS_ASSERT(abs(exp_s(2) - 20.217e3) < eps);
    TFEL_TESTS_ASSERT(abs(exp_s(3) - 9.4065e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(exp_s(4) - 14.890e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(exp_s(5) - 12.772e3 * sqrt2) < eps);
    // derivative
    TFEL_TESTS_ASSERT(abs(dexp(0, 0) - 4.0476e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(0, 1) - 1.9848e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(0, 2) - 4.9338e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(0, 3) - 2.8929e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(0, 4) - 4.5648e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(0, 5) - 3.1293e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(1, 0) - 1.9848e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(1, 1) - 2.4686e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(1, 2) - 3.6153e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(1, 3) - 2.2902e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(1, 4) - 2.6788e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(1, 5) - 3.0958e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(2, 0) - 4.9338e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(2, 1) - 3.6153e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(2, 2) - 11.668e3) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(2, 3) - 4.2234e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(2, 4) - 7.6461e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(2, 5) - 6.5468e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(3, 0) - 2.8929e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(3, 1) - 2.2902e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(3, 2) - 4.2234e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(3, 3) - 2.5757e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(3, 4) - 3.5192e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(3, 5) - 3.1467e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(4, 0) - 4.5648e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(4, 1) - 2.6788e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(4, 2) - 7.6461e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(4, 3) - 3.5192e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(4, 4) - 5.9146e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(4, 5) - 4.5353e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(5, 0) - 3.1293e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(5, 1) - 3.0958e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(5, 2) - 6.5468e3 * sqrt2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(5, 3) - 3.1467e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(5, 4) - 4.5353e3 * 2) < eps);
    TFEL_TESTS_ASSERT(abs(dexp(5, 5) - 4.5181e3 * 2) < eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTest9_3D, "STensorTest9-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("stensor9.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
