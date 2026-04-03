/*!
 * \file   tests/Material/BarlatSecondDerivativeTest2.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/11/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"
#include "TFEL/Material/Barlat2004YieldCriterion.hxx"

struct BarlatSecondDerivativeTest2 final : public tfel::tests::TestCase {
  BarlatSecondDerivativeTest2()
      : tfel::tests::TestCase("TFEL/Math", "BarlatSecondDerivative2") {
  }  // end of BarlatSecondDerivativeTest2

  tfel::tests::TestResult execute() override {
    // stresses in "MPa" :)
    const double v1[6] = {1.27453824166446,  0.77207083708966,
                          0.24525337568425,  -1.7790370858361e-4,
                          3.613971630283e-3, -1.7873236537153e-2};
    const double v2[6] = {
        1.27453824166446, 1.27453824166446, 0.24525337568425, 0, 0, 0};
    const double v3[6] = {
        0.24525337568425, 0.24525337568425, 1.27453824166446, 0, 0, 0};
    const double v4[6] = {
        -0.24525337568425, -0.24525337568425, 1.27453824166446, 0, 0, 0};
    for (const auto v : {v1, v2, v3, v4}) {
      //    for(const auto v : {v1}){
      test1(v);
      test2(v);
    }
    return this->result;
  }  // end of execute
 private:
  void test1(const double* values) {
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = typename stensor<3u, double>::size_type;
    const auto eps1 = 3.e-8;
    const auto ds = 1.e-5;
    const auto s = stensor<3u, double>{values};
    const auto l1 = makeBarlatLinearTransformation<3u, double>(
        -0.069888, 0.936408, 0.079143, 1.003060, 0.524741, 1.363180, 1.023770,
        1.069060, 0.954322);
    const auto l2 = makeBarlatLinearTransformation<3u, double>(
        -0.981171, 0.476741, 0.575316, 0.866827, 1.145010, -0.079294, 1.051660,
        1.147100, 1.404620);
    const auto seq = sigmaeq(s);
    const auto s1 = eval(l1 * s);
    const auto s2 = eval(l2 * s);
    // eigen values of s1 and s1
    const auto vp1 = s1.computeEigenValues();
    const auto vp2 = s2.computeEigenValues();
    // Barlat potential and its derivatives
    const auto d = computeBarlatStressSecondDerivative<stensor<3u, double>>(
        vp1, vp2, seq, 8);
    // numerical differentiation with respect to vp1
    tvector<6u, double> nd2Phi_dsvp12(0);
    tvector<9u, double> nd2Phi_dsvp1dsvp2(0);
    for (unsigned short i = 0; i != 3; ++i) {
      auto vp1_p = vp1;
      vp1_p[i] += ds;
      // seq is only a normalisation factor, not recomputed
      const auto d_p = computeBarlatStressSecondDerivative<stensor<3u, double>>(
          vp1_p, vp2, seq, 8);
      const auto dPhi_dsvp1_p = d_p.dPhi_dsvp1;
      const auto dPhi_dsvp2_p = d_p.dPhi_dsvp2;
      auto vp1_m = vp1;
      vp1_m[i] -= ds;
      const auto d_m = computeBarlatStressSecondDerivative<stensor<3u, double>>(
          vp1_m, vp2, seq, 8);
      const auto dPhi_dsvp1_m = d_m.dPhi_dsvp1;
      const auto dPhi_dsvp2_m = d_m.dPhi_dsvp2;
      if (i == 0) {
        nd2Phi_dsvp12[0] = (dPhi_dsvp1_p[0] - dPhi_dsvp1_m[0]) / (2 * ds);
        nd2Phi_dsvp12[3] += (dPhi_dsvp1_p[1] - dPhi_dsvp1_m[1]) / (2 * ds) / 2;
        nd2Phi_dsvp12[4] += (dPhi_dsvp1_p[2] - dPhi_dsvp1_m[2]) / (2 * ds) / 2;
        nd2Phi_dsvp1dsvp2[0] = (dPhi_dsvp2_p[0] - dPhi_dsvp2_m[0]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[1] = (dPhi_dsvp2_p[1] - dPhi_dsvp2_m[1]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[2] = (dPhi_dsvp2_p[2] - dPhi_dsvp2_m[2]) / (2 * ds);
      } else if (i == 1) {
        nd2Phi_dsvp12[1] = (dPhi_dsvp1_p[1] - dPhi_dsvp1_m[1]) / (2 * ds);
        nd2Phi_dsvp12[3] += (dPhi_dsvp1_p[0] - dPhi_dsvp1_m[0]) / (2 * ds) / 2;
        nd2Phi_dsvp12[5] += (dPhi_dsvp1_p[2] - dPhi_dsvp1_m[2]) / (2 * ds) / 2;
        nd2Phi_dsvp1dsvp2[3] = (dPhi_dsvp2_p[0] - dPhi_dsvp2_m[0]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[4] = (dPhi_dsvp2_p[1] - dPhi_dsvp2_m[1]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[5] = (dPhi_dsvp2_p[2] - dPhi_dsvp2_m[2]) / (2 * ds);
      } else {
        nd2Phi_dsvp12[2] = (dPhi_dsvp1_p[2] - dPhi_dsvp1_m[2]) / (2 * ds);
        nd2Phi_dsvp12[4] += (dPhi_dsvp1_p[0] - dPhi_dsvp1_m[0]) / (2 * ds) / 2;
        nd2Phi_dsvp12[5] += (dPhi_dsvp1_p[1] - dPhi_dsvp1_m[1]) / (2 * ds) / 2;
        nd2Phi_dsvp1dsvp2[6] = (dPhi_dsvp2_p[0] - dPhi_dsvp2_m[0]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[7] = (dPhi_dsvp2_p[1] - dPhi_dsvp2_m[1]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[8] = (dPhi_dsvp2_p[2] - dPhi_dsvp2_m[2]) / (2 * ds);
      }
    }
    for (size_type i = 0; i != 6; ++i) {
      const auto e = std::abs(nd2Phi_dsvp12[i] - d.d2Phi_dsvp12[i]);
      if (e > eps1) {
        std::cout << "d2Phi_dsvp12: " << i << " " << nd2Phi_dsvp12[i] << " "
                  << d.d2Phi_dsvp12[i] << " " << e << std::endl;
      }
      TFEL_TESTS_ASSERT(e < eps1);
    }
    for (size_type i = 0; i != 9; ++i) {
      const auto e = std::abs(nd2Phi_dsvp1dsvp2[i] - d.d2Phi_dsvp1dsvp2[i]);
      if (e > eps1) {
        std::cout << "d2Phi_dsvp1dsvp2: " << i << " " << nd2Phi_dsvp1dsvp2[i]
                  << " " << d.d2Phi_dsvp1dsvp2[i] << " " << e << std::endl;
      }
      TFEL_TESTS_ASSERT(e < eps1);
    }
  }
  void test2(const double* values) {
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = typename stensor<3u, double>::size_type;
    const auto eps1 = 3.e-8;
    const auto ds = 1.e-5;
    const auto s = stensor<3u, double>{values};
    const auto l1 = makeBarlatLinearTransformation<3u, double>(
        -0.069888, 0.936408, 0.079143, 1.003060, 0.524741, 1.363180, 1.023770,
        1.069060, 0.954322);
    const auto l2 = makeBarlatLinearTransformation<3u, double>(
        -0.981171, 0.476741, 0.575316, 0.866827, 1.145010, -0.079294, 1.051660,
        1.147100, 1.404620);
    const auto seq = sigmaeq(s);
    const auto s1 = eval(l1 * s);
    const auto s2 = eval(l2 * s);
    // eigen values of s1 and s1
    const auto vp1 = s1.computeEigenValues();
    const auto vp2 = s2.computeEigenValues();
    // Barlat potential and its derivatives
    const auto d = computeBarlatStressSecondDerivative<stensor<3u, double>>(
        vp1, vp2, seq, 8);
    // numerical differentiation with respect to vp2
    tvector<6u, double> nd2Phi_dsvp22(0);
    tvector<9u, double> nd2Phi_dsvp1dsvp2(0);
    for (unsigned short i = 0; i != 3; ++i) {
      auto vp2_p = vp2;
      vp2_p[i] += ds;
      // seq is only a normalisation factor, not recomputed
      const auto d_p = computeBarlatStressSecondDerivative<stensor<3u, double>>(
          vp1, vp2_p, seq, 8);
      const auto dPhi_dsvp2_p = d_p.dPhi_dsvp2;
      const auto dPhi_dsvp1_p = d_p.dPhi_dsvp1;
      auto vp2_m = vp2;
      vp2_m[i] -= ds;
      const auto d_m = computeBarlatStressSecondDerivative<stensor<3u, double>>(
          vp1, vp2_m, seq, 8);
      const auto dPhi_dsvp2_m = d_m.dPhi_dsvp2;
      const auto dPhi_dsvp1_m = d_m.dPhi_dsvp1;
      if (i == 0) {
        nd2Phi_dsvp22[0] = (dPhi_dsvp2_p[0] - dPhi_dsvp2_m[0]) / (2 * ds);
        nd2Phi_dsvp22[3] += (dPhi_dsvp2_p[1] - dPhi_dsvp2_m[1]) / (2 * ds) / 2;
        nd2Phi_dsvp22[4] += (dPhi_dsvp2_p[2] - dPhi_dsvp2_m[2]) / (2 * ds) / 2;
        nd2Phi_dsvp1dsvp2[0] = (dPhi_dsvp1_p[0] - dPhi_dsvp1_m[0]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[3] = (dPhi_dsvp1_p[1] - dPhi_dsvp1_m[1]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[6] = (dPhi_dsvp1_p[2] - dPhi_dsvp1_m[2]) / (2 * ds);
      } else if (i == 1) {
        nd2Phi_dsvp22[1] = (dPhi_dsvp2_p[1] - dPhi_dsvp2_m[1]) / (2 * ds);
        nd2Phi_dsvp22[3] += (dPhi_dsvp2_p[0] - dPhi_dsvp2_m[0]) / (2 * ds) / 2;
        nd2Phi_dsvp22[5] += (dPhi_dsvp2_p[2] - dPhi_dsvp2_m[2]) / (2 * ds) / 2;
        nd2Phi_dsvp1dsvp2[1] = (dPhi_dsvp1_p[0] - dPhi_dsvp1_m[0]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[4] = (dPhi_dsvp1_p[1] - dPhi_dsvp1_m[1]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[7] = (dPhi_dsvp1_p[2] - dPhi_dsvp1_m[2]) / (2 * ds);
      } else {
        nd2Phi_dsvp22[2] = (dPhi_dsvp2_p[2] - dPhi_dsvp2_m[2]) / (2 * ds);
        nd2Phi_dsvp22[4] += (dPhi_dsvp2_p[0] - dPhi_dsvp2_m[0]) / (2 * ds) / 2;
        nd2Phi_dsvp22[5] += (dPhi_dsvp2_p[1] - dPhi_dsvp2_m[1]) / (2 * ds) / 2;
        nd2Phi_dsvp1dsvp2[2] = (dPhi_dsvp1_p[0] - dPhi_dsvp1_m[0]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[5] = (dPhi_dsvp1_p[1] - dPhi_dsvp1_m[1]) / (2 * ds);
        nd2Phi_dsvp1dsvp2[8] = (dPhi_dsvp1_p[2] - dPhi_dsvp1_m[2]) / (2 * ds);
      }
    }
    for (size_type i = 0; i != 6; ++i) {
      const auto e = std::abs(nd2Phi_dsvp22[i] - d.d2Phi_dsvp22[i]);
      if (e > eps1) {
        std::cout << "d2Phi_dsvp22: " << i << " " << nd2Phi_dsvp22[i] << " "
                  << d.d2Phi_dsvp22[i] << " " << e << std::endl;
      }
      TFEL_TESTS_ASSERT(e < eps1);
    }
    for (size_type i = 0; i != 9; ++i) {
      const auto e = std::abs(nd2Phi_dsvp1dsvp2[i] - d.d2Phi_dsvp1dsvp2[i]);
      if (e > eps1) {
        std::cout << "d2Phi_dsvp1dsvp2: " << i << " " << nd2Phi_dsvp1dsvp2[i]
                  << " " << d.d2Phi_dsvp1dsvp2[i] << " " << e << std::endl;
      }
      TFEL_TESTS_ASSERT(e < eps1);
    }
  }

};  // end of BarlatSecondDerivativeTest2

TFEL_TESTS_GENERATE_PROXY(BarlatSecondDerivativeTest2,
                          "BarlatSecondDerivativeTest2");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("BarlatSecondDerivative2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
