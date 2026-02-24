/*!
 * \file   tests/Math/stensor8.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12 nov. 2013
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

struct STensorTest8_2D final : public tfel::tests::TestCase {
  STensorTest8_2D()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest8-2D") {
  }  // end of STensorTest8
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    typedef stensor<2u, double> Stensor;
    const double eps = 1.e-9;
    const double eps2 = 1.e-6;
    const double sqrt2 = sqrt(2.);
    const double values[6] = {8.2, 4.5, 7.2, 2.3 * sqrt2};
    Stensor s(values);
    tvector<3u, double> vp;
    st2tost2<2u, double> dn0;
    stensor<2u, double> dvp0;
    st2tost2<2u, double> dn1;
    stensor<2u, double> dvp1;
    st2tost2<2u, double> dn2;
    stensor<2u, double> dvp2;
    for (unsigned short i = 0; i != 4u; ++i) {
      tvector<3u, double> v0;
      tvector<3u, double> v1;
      tvector<3u, double> v2;
      Stensor s2(s);
      s2(i) += eps;
      s2.computeEigenValues(vp);
      const double vp0_p = vp[0];
      const double vp1_p = vp[1];
      const double vp2_p = vp[2];
      s2.computeEigenVector(v0, vp0_p);
      s2.computeEigenVector(v1, vp1_p);
      s2.computeEigenVector(v2, vp2_p);
      const Stensor dn0_p = Stensor::buildFromVectorDiadicProduct(v0);
      const Stensor dn1_p = Stensor::buildFromVectorDiadicProduct(v1);
      const Stensor dn2_p = Stensor::buildFromVectorDiadicProduct(v2);
      s2(i) -= 2 * eps;
      s2.computeEigenValues(vp);
      const double vp0_m = vp[0];
      const double vp1_m = vp[1];
      const double vp2_m = vp[2];
      s2.computeEigenVector(v0, vp0_m);
      s2.computeEigenVector(v1, vp1_m);
      s2.computeEigenVector(v2, vp2_m);
      const Stensor dn0_m = Stensor::buildFromVectorDiadicProduct(v0);
      const Stensor dn1_m = Stensor::buildFromVectorDiadicProduct(v1);
      const Stensor dn2_m = Stensor::buildFromVectorDiadicProduct(v2);
      dvp0(i) = (vp0_p - vp0_m) / (2 * eps);
      dvp1(i) = (vp1_p - vp1_m) / (2 * eps);
      dvp2(i) = (vp2_p - vp2_m) / (2 * eps);
      for (unsigned short j = 0; j != 4u; ++j) {
        dn0(j, i) = (dn0_p(j) - dn0_m(j)) / (2 * eps);
        dn1(j, i) = (dn1_p(j) - dn1_m(j)) / (2 * eps);
        dn2(j, i) = (dn2_p(j) - dn2_m(j)) / (2 * eps);
      }
    }
    tmatrix<3u, 3u, double> m;
    st2tost2<2u, double> dn0_ds, dn1_ds, dn2_ds;
    stensor<2u, double> dvp0_ds, dvp1_ds, dvp2_ds;
    s.computeEigenVectors(vp, m);
    stensor<2u, double>::computeEigenValuesDerivatives(dvp0_ds, dvp1_ds,
                                                       dvp2_ds, m);
    stensor<2u, double>::computeEigenTensorsDerivatives(dn0_ds, dn1_ds, dn2_ds,
                                                        vp, m, 1.e-7);
    for (unsigned short i = 0; i != 4u; ++i) {
      TFEL_TESTS_ASSERT(abs(dvp0_ds(i) - dvp0(i)) < eps2);
      TFEL_TESTS_ASSERT(abs(dvp1_ds(i) - dvp1(i)) < eps2);
      TFEL_TESTS_ASSERT(abs(dvp2_ds(i) - dvp2(i)) < eps2);
      for (unsigned short j = 0; j != 4u; ++j) {
        TFEL_TESTS_ASSERT(abs(dn0_ds(i, j) - dn0(i, j)) < eps2);
        TFEL_TESTS_ASSERT(abs(dn1_ds(i, j) - dn1(i, j)) < eps2);
        TFEL_TESTS_ASSERT(abs(dn2_ds(i, j) - dn2(i, j)) < eps2);
      }
    }
    return this->result;
  }  // end of execute
};

struct STensorTest8_3D final : public tfel::tests::TestCase {
  STensorTest8_3D()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest8-3D") {
  }  // end of STensorTest8
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    typedef stensor<3u, double> Stensor;
    const double eps = 1.e-9;
    const double eps2 = 1.e-5;
    const double sqrt2 = sqrt(2.);
    const double values[6] = {8.2,         4.5,         7.2,
                              2.3 * sqrt2, 5.4 * sqrt2, 1.9 * sqrt2};
    Stensor s(values);
    tvector<3u, double> vp;
    st2tost2<3u, double> dn0;
    stensor<3u, double> dvp0;
    st2tost2<3u, double> dn1;
    stensor<3u, double> dvp1;
    st2tost2<3u, double> dn2;
    stensor<3u, double> dvp2;
    for (unsigned short i = 0; i != 6; ++i) {
      tvector<3u, double> v0;
      tvector<3u, double> v1;
      tvector<3u, double> v2;
      Stensor s2(s);
      s2(i) += eps;
      s2.computeEigenValues(vp);
      const double vp0_p = vp[0];
      const double vp1_p = vp[1];
      const double vp2_p = vp[2];
      s2.computeEigenVector(v0, vp0_p);
      s2.computeEigenVector(v1, vp1_p);
      s2.computeEigenVector(v2, vp2_p);
      const Stensor dn0_p = Stensor::buildFromVectorDiadicProduct(v0);
      const Stensor dn1_p = Stensor::buildFromVectorDiadicProduct(v1);
      const Stensor dn2_p = Stensor::buildFromVectorDiadicProduct(v2);
      s2(i) -= 2 * eps;
      s2.computeEigenValues(vp);
      const double vp0_m = vp[0];
      const double vp1_m = vp[1];
      const double vp2_m = vp[2];
      s2.computeEigenVector(v0, vp0_m);
      s2.computeEigenVector(v1, vp1_m);
      s2.computeEigenVector(v2, vp2_m);
      const Stensor dn0_m = Stensor::buildFromVectorDiadicProduct(v0);
      const Stensor dn1_m = Stensor::buildFromVectorDiadicProduct(v1);
      const Stensor dn2_m = Stensor::buildFromVectorDiadicProduct(v2);
      dvp0(i) = (vp0_p - vp0_m) / (2 * eps);
      dvp1(i) = (vp1_p - vp1_m) / (2 * eps);
      dvp2(i) = (vp2_p - vp2_m) / (2 * eps);
      for (unsigned short j = 0; j != 6; ++j) {
        dn0(j, i) = (dn0_p(j) - dn0_m(j)) / (2 * eps);
        dn1(j, i) = (dn1_p(j) - dn1_m(j)) / (2 * eps);
        dn2(j, i) = (dn2_p(j) - dn2_m(j)) / (2 * eps);
      }
    }
    tmatrix<3u, 3u, double> m;
    st2tost2<3u, double> dn0_ds, dn1_ds, dn2_ds;
    stensor<3u, double> dvp0_ds, dvp1_ds, dvp2_ds;
    s.computeEigenVectors(vp, m);
    stensor<3u, double>::computeEigenValuesDerivatives(dvp0_ds, dvp1_ds,
                                                       dvp2_ds, m);
    stensor<3u, double>::computeEigenTensorsDerivatives(dn0_ds, dn1_ds, dn2_ds,
                                                        vp, m, 1.e-7);
    for (unsigned short i = 0; i != 6u; ++i) {
      TFEL_TESTS_ASSERT(abs(dvp0_ds(i) - dvp0(i)) < eps2);
      TFEL_TESTS_ASSERT(abs(dvp1_ds(i) - dvp1(i)) < eps2);
      TFEL_TESTS_ASSERT(abs(dvp2_ds(i) - dvp2(i)) < eps2);
      for (unsigned short j = 0; j != 6u; ++j) {
        TFEL_TESTS_ASSERT(abs(dn0_ds(i, j) - dn0(i, j)) < eps2);
        TFEL_TESTS_ASSERT(abs(dn1_ds(i, j) - dn1(i, j)) < eps2);
        TFEL_TESTS_ASSERT(abs(dn2_ds(i, j) - dn2(i, j)) < eps2);
      }
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTest8_2D, "STensorTest8-2D");
TFEL_TESTS_GENERATE_PROXY(STensorTest8_3D, "STensorTest8-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("stensor8.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
