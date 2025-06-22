/*!
 * \file   tests/Material/HosfordSecondDerivativeTest.cxx
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
#include "TFEL/Material/Hosford.hxx"

struct HosfordSecondDerivativeTest final : public tfel::tests::TestCase {
  HosfordSecondDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "HosfordSecondDerivative") {
  }  // end of HosfordSecondDerivativeTest

  tfel::tests::TestResult execute() override {
    // stresses in "MPa" :)
    const double Id[6] = {1., 1., 1., 0., 0., 0.};
    const double v1[6] = {1.27453824166446,  0.77207083708966,
                          0.24525337568425,  -1.7790370858361e-4,
                          3.613971630283e-3, -1.7873236537153e-2};
    const double v2[6] = {
        1.27453824166446, 1.27453824166446, 0.24525337568425, 0, 0, 0};
    const double v3[6] = {
        0.24525337568425, 0.24525337568425, 1.27453824166446, 0, 0, 0};
    const double v4[6] = {
        -0.24525337568425, -0.24525337568425, 1.27453824166446, 0, 0, 0};
    for (const auto v : {Id, v1, v2, v3, v4}) {
      test1<1u>(v);
      test1<2u>(v);
      test1<3u>(v);
    }
    for (const auto v : {Id, v1, v2, v3, v4}) {
      test2<1u>(v);
      test2<2u>(v);
      test2<3u>(v);
    }
    for (const auto v : {v1, v2, v3, v4}) {
      test3<1u>(v);
      test3<2u>(v);
      test3<3u>(v);
    }
    return this->result;
  }  // end of execute
 private:
  // This tests compares the Hosford stress and its first and second
  // derivatives for an Hosford exponent egal to 2 to the Von Mises
  // stress and its first and second derivatives
  template <unsigned short N>
  void test1(const double* values) {
    using tfel::material::computeHosfordStressSecondDerivative;
    using tfel::math::st2tost2;
    using tfel::math::stensor;
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 1.e-8;
    const auto eps2 = 1.e-8;
    const auto s = stensor<N, double>{values};
    const auto seq = sigmaeq(s);
    const auto n = (seq < seps ? stensor<N, double>(double(0))
                               : eval(3 * deviator(s) / (2 * seq)));
    const auto dn =
        (seq < seps ? st2tost2<N, double>(double(0))
                    : eval((st2tost2<N, double>::M() - (n ^ n)) / seq));
    // no structured binding yet
    double hseq;
    tfel::math::stensor<N, double> hn;
    tfel::math::st2tost2<N, double> dhn;
    std::tie(hseq, hn, dhn) = computeHosfordStressSecondDerivative(s, 2, seps);
    TFEL_TESTS_ASSERT(std::abs(seq - hseq) < seps);
    for (size_type i = 0; i != s.size(); ++i) {
      const auto en = std::abs(n[i] - hn[i]);
      if (en > eps1) {
        std::cout << i << " " << n(i) << " " << hn(i) << " " << en << std::endl;
      }
      TFEL_TESTS_ASSERT(en < eps1);
      for (size_type j = 0; j != s.size(); ++j) {
        const auto edn = std::abs(dhn(i, j) - dn(i, j));
        if (edn >= eps2) {
          std::cout << i << " " << j << " " << dn(i, j) << " " << dhn(i, j)
                    << " " << edn << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(edn) < eps2);
      }
    }
  }
  // This tests compares the first derivative of the Hosford stress
  // for an Hosford exponent egal to 8 to a numerical approximation
  template <unsigned short N>
  void test2(const double* values) {
    using tfel::material::computeHosfordStress;
    using tfel::material::computeHosfordStressNormal;
    using tfel::math::st2tost2;
    using tfel::math::stensor;
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 3.e-8;
    const auto ds = 1.e-5;
    const auto s = stensor<N, double>{values};
    double seq;
    tfel::math::stensor<N, double> n;
    std::tie(seq, n) = computeHosfordStressNormal(s, 8, seps);
    tfel::math::stensor<N, double> dn;
    for (size_type i = 0; i != s.size(); ++i) {
      auto sb = s;
      sb[i] += ds;
      const auto seq_p = computeHosfordStress(sb, 8, seps);
      sb[i] -= 2 * ds;
      const auto seq_m = computeHosfordStress(sb, 8, seps);
      dn[i] = (seq_p - seq_m) / (2 * ds);
    }
    for (size_type i = 0; i != s.size(); ++i) {
      const auto en = std::abs(dn[i] - n[i]);
      if (en > eps1) {
        std::cout << i << " " << n(i) << " " << dn(i) << " " << en << std::endl;
      }
      TFEL_TESTS_ASSERT(en < eps1);
    }
  }
  // This tests compares the first derivative of the Hosford stress
  // for an Hosford exponent egal to 8 to a numerical approximation
  template <unsigned short N>
  void test3(const double* values) {
    using tfel::material::computeHosfordStressNormal;
    using tfel::material::computeHosfordStressSecondDerivative;
    using tfel::math::st2tost2;
    using tfel::math::stensor;
    constexpr const auto a = double(8);
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 2.e-6;
    const auto eps2 = 2.e-4;
    const auto ds = 1.e-6;
    const auto s = stensor<N, double>{values};
    double seq;
    stensor<N, double> n;
    st2tost2<N, double> dn;
    std::tie(seq, n, dn) = computeHosfordStressSecondDerivative(s, a, seps);
    tfel::math::st2tost2<N, double> dnn;
    for (size_type i = 0; i != s.size(); ++i) {
      auto sb = s;
      sb[i] += ds;
      double seq_p, seq_m;
      stensor<N, double> n_p, n_m;
      std::tie(seq_p, n_p) = computeHosfordStressNormal(sb, a, seps);
      sb[i] -= 2 * ds;
      std::tie(seq_m, n_m) = computeHosfordStressNormal(sb, a, seps);
      for (size_type j = 0; j != s.size(); ++j) {
        dnn(j, i) = (n_p(j) - n_m(j)) / (2 * ds);
      }
    }
    for (size_type i = 0; i != s.size(); ++i) {
      for (size_type j = 0; j != s.size(); ++j) {
        const auto en = std::abs(dnn(i, j) - dn(i, j));
        const auto e = std::max(std::abs(dnn(i, j)) * eps2, eps1);
        if (en > e) {
          std::cout << i << " " << j << " " << dnn(i, j) << " " << dn(i, j)
                    << " " << en << " " << e << std::endl;
        }
        TFEL_TESTS_ASSERT(en < e);
      }
    }
  }
};  // end of HosfordSecondDerivativeTest

TFEL_TESTS_GENERATE_PROXY(HosfordSecondDerivativeTest,
                          "HosfordSecondDerivativeTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("HosfordSecondDerivative.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
