/*!
 * \file   tests/Math/stensor9.cxx
 * \brief
 * \author Thomas Helfer
 * \date 23/12/2013
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

#include "TFEL/Math/qt.hxx"
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
    const auto [q0, q1, q2] =
        stensor<3u, qt<unit::Stress>>::computeEigenTensors(m);
    TFEL_TESTS_ASSERT(abs(q2(5) - n2(5)) < 1.e-14);
    stensor<3u, double>::computeEigenTensorsDerivatives(dn0, dn1, dn2, vp, m,
                                                        1.e-7);
    stensor<3u, double> exp_s(s.buildFromEigenValuesAndVectors(
        exp(vp(0)), exp(vp(1)), exp(vp(2)), m));
    st2tost2<3u, double> dexp;
    dexp = exp(vp(0)) * ((n0 ^ n0) + dn0) + exp(vp(1)) * ((n1 ^ n1) + dn1) +
           exp(vp(2)) * ((n2 ^ n2) + dn2);
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
    // exponential of s computed by mpmath (expm, 40 digits)
    const double exp_ref[6] = {
        10966.187461159059,         8068.6757603006099,
        20216.736463761345,         9406.5155226577201 * sqrt2,
        14889.605884263750 * sqrt2, 12771.926783682471 * sqrt2};
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(abs(exp_s(i) - exp_ref[i]) < 1.e-13 * exp_ref[2]);
    }
    return this->result;
  }  // end of execute
};

/*!
 * \brief same computations for a symmetric tensor whose eigenvalues
 * have comparable magnitudes, so that the three terms of the
 * exponential and of its derivative have comparable weights
 */
struct STensorTest9_3D_2 final : public tfel::tests::TestCase {
  STensorTest9_3D_2()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest9-3D-2") {
  }  // end of STensorTest9_3D_2
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    const double values[6] = {-0.1,        -0.2,        0.3,
                              0.4 * sqrt2, 0.6 * sqrt2, 0.5 * sqrt2};
    const stensor<3u, double> s(values);
    const auto exp_s = exponential(s);
    // exponential of s computed by mpmath (expm, 40 digits)
    const double exp_ref[6] = {
        1.2209312103103473,          1.0619455180265054,
        1.7760906599295133,          0.56054500425357944 * sqrt2,
        0.86774527955243638 * sqrt2, 0.73568736044066169 * sqrt2};
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(abs(exp_s(i) - exp_ref[i]) < 1.e-13);
    }
    // derivative compared to a centered finite difference of the
    // exponential, as in stensor_isotropic_function.cxx
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
    const st2tost2<3u, double> dexp = exp(vp(0)) * ((n0 ^ n0) + dn0) +
                                      exp(vp(1)) * ((n1 ^ n1) + dn1) +
                                      exp(vp(2)) * ((n2 ^ n2) + dn2);
    constexpr double eps = 1.e-7;
    constexpr double prec = 5.e-8;
    for (unsigned short i = 0; i != 6; ++i) {
      auto s2 = s;
      s2(i) += eps;
      const auto exp_f = exponential(s2);
      s2(i) -= 2 * eps;
      const auto exp_b = exponential(s2);
      for (unsigned short j = 0; j != 6; ++j) {
        const auto nd = (exp_f(j) - exp_b(j)) / (2 * eps);
        TFEL_TESTS_ASSERT(abs(dexp(j, i) - nd) < prec);
      }
    }
    return this->result;
  }  // end of execute
 private:
  //! \return the exponential of a symmetric tensor
  static tfel::math::stensor<3u, double> exponential(
      const tfel::math::stensor<3u, double>& s) {
    tfel::math::tvector<3u, double> vp;
    tfel::math::tmatrix<3u, 3u, double> m;
    s.computeEigenVectors(vp, m);
    return tfel::math::stensor<3u, double>::buildFromEigenValuesAndVectors(
        std::exp(vp(0)), std::exp(vp(1)), std::exp(vp(2)), m);
  }  // end of exponential
};

TFEL_TESTS_GENERATE_PROXY(STensorTest9_3D, "STensorTest9-3D");
TFEL_TESTS_GENERATE_PROXY(STensorTest9_3D_2, "STensorTest9-3D-2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("stensor9.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
