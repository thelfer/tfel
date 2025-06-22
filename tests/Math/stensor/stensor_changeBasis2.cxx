/*!
 * \file  tests/Math/stensor_changeBasis2.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <random>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

struct StensorChangeBasis2Test final : public tfel::tests::TestCase {
  StensorChangeBasis2Test()
      : tfel::tests::TestCase("TFEL/Math", "StensorChangeBasis2Test") {
  }  // end of StensorChangeBasis2Test

  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 1.e-4;
    const real pi = 4.0 * atan(1.0);
    tmatrix<3u, 3u, real> r;
    unsigned short i, j, k, t;
    default_random_engine g;
    for (t = 0; t != 10; ++t) {
      const real psi = uniform_real_distribution<real>{0., 2. * pi}(g);
      const real cthe = uniform_real_distribution<real>{-1, 1.}(g);
      const real the = acos(cthe);
      const real phi = uniform_real_distribution<real>{0., 2. * pi}(g);
      const real cospsi = cos(psi);
      const real costhe = cos(the);
      const real cosphi = cos(phi);
      const real sinpsi = sin(psi);
      const real sinthe = sin(the);
      const real sinphi = sin(phi);
      // rotation matrix
      r(0, 0) = cosphi * cospsi - sinphi * costhe * sinpsi;
      r(1, 0) = cosphi * sinpsi + sinphi * costhe * cospsi;
      r(2, 0) = sinphi * sinthe;
      r(0, 1) = -sinphi * cospsi - cosphi * costhe * sinpsi;
      r(1, 1) = -sinphi * sinpsi + cosphi * costhe * cospsi;
      r(2, 1) = cosphi * sinthe;
      r(0, 2) = sinthe * sinpsi;
      r(1, 2) = -sinthe * cospsi;
      r(2, 2) = costhe;
      // stresses as a symmetric matrix
      tmatrix<3u, 3u, real> sm(0.);
      for (i = 0; i != 3; ++i) {
        for (j = i; j != 3; ++j) {
          sm(i, j) = sm(j, i) = uniform_real_distribution<real>{0., 100e6}(g);
        }
      }
      // stresses as a symmetric tensor
      stensor<3u, real> st(0.);
      for (i = 0; i != 3; ++i) {
        st(i) = sm(i, i);
      }
      st(3) = sm(0, 1) * sqrt(real(2));
      st(4) = sm(0, 2) * sqrt(real(2));
      st(5) = sm(1, 2) * sqrt(real(2));
      // change basis
      tmatrix<3u, 3u, real> sm2;
      tmatrix<3u, 3u, real> tmp;
      for (i = 0; i != 3; ++i) {
        for (j = 0; j != 3; ++j) {
          tmp(i, j) = 0.;
          for (k = 0; k != 3; ++k) {
            tmp(i, j) += sm(i, k) * r(k, j);
          }
        }
      }
      for (i = 0; i != 3; ++i) {
        for (j = 0; j != 3; ++j) {
          sm2(i, j) = 0.;
          for (k = 0; k != 3; ++k) {
            sm2(i, j) += r(k, i) * tmp(k, j);
          }
        }
      }
      st.changeBasis(r);
      for (i = 0; i != 3; ++i) {
        TFEL_TESTS_ASSERT(abs(st(i) - sm2(i, i)) < eps);
      }
      TFEL_TESTS_ASSERT(abs(st(3) - sm2(0, 1) * sqrt(real(2))) < eps);
      TFEL_TESTS_ASSERT(abs(st(4) - sm2(0, 2) * sqrt(real(2))) < eps);
      TFEL_TESTS_ASSERT(abs(st(5) - sm2(1, 2) * sqrt(real(2))) < eps);
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(StensorChangeBasis2Test, "StensorChangeBasis2Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("StensorChangeBasis2.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
