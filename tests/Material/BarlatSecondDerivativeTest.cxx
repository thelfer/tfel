/*!
 * \file   tests/Material/BarlatSecondDerivativeTest.cxx
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
#include "TFEL/Material/Hosford1972YieldCriterion.hxx"
#include "TFEL/Material/Barlat2004YieldCriterion.hxx"

struct BarlatSecondDerivativeTest final : public tfel::tests::TestCase {
  BarlatSecondDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "BarlatSecondDerivative") {
  }  // end of BarlatSecondDerivativeTest

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
      test1a<1u>(v);
      test1b<1u>(v);
      test1c<1u>(v);
      test1a<2u>(v);
      test1b<2u>(v);
      test1c<2u>(v);
      test1a<3u>(v);
      test1b<3u>(v);
      test1c<3u>(v);
    }
    for (const auto v : {v1, v2, v3, v4}) {
      test2a<1u>(v);
      test2a<2u>(v);
      test2a<3u>(v);
      test2b<1u>(v);
      test2b<2u>(v);
      test2b<3u>(v);
    }
    for (const auto v : {v1, v2, v3, v4}) {
      test3<1u>(v);
      test3<2u>(v);
      test3<3u>(v);
    }
    return this->result;
  }  // end of execute
 private:
  // This tests compares the Barlat stress for a Barlat exponent egal
  // to 2 to the Von Mises stress
  template <unsigned short N>
  void test1a(const double* values) {
    using tfel::material::computeBarlatStress;
    using tfel::material::makeBarlatLinearTransformation;
    using tfel::math::stensor;
    const auto seps = 1.e-10;
    const auto s = stensor<N, double>{values};
    const auto seq = sigmaeq(s);
    // no structured binding yet
    const auto l =
        makeBarlatLinearTransformation<N, double>(1, 1, 1, 1, 1, 1, 1, 1, 1);
    const auto hseq = computeBarlatStress(s, l, l, 2, seps);
    if (std::abs(seq - hseq) > seps) {
      std::cout << seq << " " << hseq << " " << seps << std::endl;
    }
    TFEL_TESTS_ASSERT(std::abs(seq - hseq) < seps);
  }
  // This tests compares the Barlat stress and its first derivative
  // for a Barlat exponent egal to 2 to the Von Mises stress and its
  // first derivative
  template <unsigned short N>
  void test1b(const double* values) {
    using tfel::material::computeBarlatStressNormal;
    using tfel::material::makeBarlatLinearTransformation;
    using tfel::math::st2tost2;
    using tfel::math::stensor;
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 1.e-8;
    const auto s = stensor<N, double>{values};
    const auto seq = sigmaeq(s);
    const auto n = (seq < seps ? stensor<N, double>(double(0))
                               : eval(3 * deviator(s) / (2 * seq)));
    // no structured binding yet
    double hseq;
    tfel::math::stensor<N, double> hn;
    const auto l =
        makeBarlatLinearTransformation<N, double>(1, 1, 1, 1, 1, 1, 1, 1, 1);
    std::tie(hseq, hn) = computeBarlatStressNormal(s, l, l, 2, seps);
    if (std::abs(seq - hseq) > seps) {
      std::cout << seq << " " << hseq << " " << seps << std::endl;
    }
    TFEL_TESTS_ASSERT(std::abs(seq - hseq) < seps);
    for (size_type i = 0; i != s.size(); ++i) {
      const auto en = std::abs(n[i] - hn[i]);
      if (en > eps1) {
        std::cout << i << " " << n(i) << " " << hn(i) << " " << en << std::endl;
      }
      TFEL_TESTS_ASSERT(en < eps1);
    }
  }
  // This tests compares the Barlat stress and its first and second
  // derivatives for a Barlat exponent egal to 2 to the Von Mises
  // stress and its first and second derivatives
  template <unsigned short N>
  void test1c(const double* values) {
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 1.e-8;
    const auto eps2 = 2.e-8;
    const auto s = stensor<N, double>{values};
    const auto seq = sigmaeq(s);
    const auto n = (seq < seps ? stensor<N, double>(double(0))
                               : eval(3 * deviator(s) / (2 * seq)));
    const auto dn =
        (seq < seps ? st2tost2<N, double>(double(0))
                    : eval((st2tost2<N, double>::M() - (n ^ n)) / seq));
    // no structured binding yet
    double bseq;
    stensor<N, double> bn;
    st2tost2<N, double> dbn;
    const auto l =
        makeBarlatLinearTransformation<N, double>(1, 1, 1, 1, 1, 1, 1, 1, 1);
    std::tie(bseq, bn, dbn) =
        computeBarlatStressSecondDerivative(s, l, l, 2, seps);
    if (std::abs(seq - bseq) > seps) {
      std::cout << seq << " " << bseq << " " << seps << std::endl;
    }
    TFEL_TESTS_ASSERT(std::abs(seq - bseq) < seps);
    for (size_type i = 0; i != s.size(); ++i) {
      const auto en = std::abs(n[i] - bn[i]);
      if (en > eps1) {
        std::cout << i << " " << n(i) << " " << bn(i) << " " << en << std::endl;
      }
      TFEL_TESTS_ASSERT(en < eps1);
      for (size_type j = 0; j != s.size(); ++j) {
        const auto edn = std::abs(dbn(i, j) - dn(i, j));
        if (edn >= eps2) {
          std::cout << i << " " << j << " " << dn(i, j) << " " << dbn(i, j)
                    << " " << edn << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(edn) < eps2);
      }
    }
  }
  // This tests compares the first derivative of the Barlat stress for
  // a Barlat exponent egal to 8 to a numerical approximation. All
  // coefficients of the linear transformations are equal to one, so
  // the Barlat stress reduces to the Hosford stress.
  template <unsigned short N>
  void test2a(const double* values) {
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 1.e-7;
    const auto ds = 1.e-5;
    const auto s = stensor<N, double>{values};
    const auto l =
        makeBarlatLinearTransformation<N, double>(1, 1, 1, 1, 1, 1, 1, 1, 1);
    double seq, seqH;
    stensor<N, double> n, nH;
    std::tie(seq, n) = computeBarlatStressNormal(s, l, l, 8, seps);
    std::tie(seqH, nH) = computeHosfordStressNormal(s, 8, seps);
    stensor<N, double> dn;
    TFEL_TESTS_ASSERT(std::abs(seq - seqH) < seps);
    for (size_type i = 0; i != s.size(); ++i) {
      auto sb = s;
      sb[i] += ds;
      const auto seq_p = computeBarlatStress(sb, l, l, 8, seps);
      sb[i] -= 2 * ds;
      const auto seq_m = computeBarlatStress(sb, l, l, 8, seps);
      dn[i] = (seq_p - seq_m) / (2 * ds);
    }
    for (size_type i = 0; i != s.size(); ++i) {
      const auto en1 = std::abs(n[i] - nH[i]);
      const auto en2 = std::abs(dn[i] - n[i]);
      if (en1 > eps1) {
        std::cout << i << " " << n(i) << " " << nH(i) << " " << en1
                  << std::endl;
      }
      if (en2 > eps1) {
        std::cout << i << " " << n(i) << " " << dn(i) << " " << en2
                  << std::endl;
      }
      TFEL_TESTS_ASSERT(en1 < eps1);
      TFEL_TESTS_ASSERT(en2 < eps1);
    }
  }
  // This tests compares the first derivative of the Barlat stress for
  // a Barlat exponent egal to 8 to a numerical approximation.
  template <unsigned short N>
  void test2b(const double* values) {
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 3.e-8;
    const auto ds = 1.e-5;
    const auto s = stensor<N, double>{values};
    const auto l1 = makeBarlatLinearTransformation<N, double>(
        -0.069888, 0.936408, 0.079143, 1.003060, 0.524741, 1.363180, 1.023770,
        1.069060, 0.954322);
    const auto l2 = makeBarlatLinearTransformation<N, double>(
        -0.981171, 0.476741, 0.575316, 0.866827, 1.145010, -0.079294, 1.051660,
        1.147100, 1.404620);
    double seq;
    stensor<N, double> n;
    std::tie(seq, n) = computeBarlatStressNormal(s, l1, l2, 8, seps);
    stensor<N, double> dn;
    for (size_type i = 0; i != s.size(); ++i) {
      auto sb = s;
      sb[i] += ds;
      const auto seq_p = computeBarlatStress(sb, l1, l2, 8, seps);
      sb[i] -= 2 * ds;
      const auto seq_m = computeBarlatStress(sb, l1, l2, 8, seps);
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
  // This tests compares the first derivative of the Barlat stress
  // for a Barlat exponent egal to 8 to a numerical approximation
  template <unsigned short N>
  void test3(const double* values) {
    using namespace tfel::math;
    using namespace tfel::material;
    constexpr const auto a = double(8);
    const auto l1 = makeBarlatLinearTransformation<N, double>(
        -0.069888, 0.936408, 0.079143, 1.003060, 0.524741, 1.363180, 1.023770,
        1.069060, 0.954322);
    const auto l2 = makeBarlatLinearTransformation<N, double>(
        -0.981171, 0.476741, 0.575316, 0.866827, 1.145010, -0.079294, 1.051660,
        1.147100, 1.404620);
    using size_type = typename stensor<N, double>::size_type;
    const auto seps = 1.e-10;
    const auto eps1 = 2.e-6;
    const auto eps2 = 2.e-4;
    const auto ds = 1.e-6;
    const auto s = stensor<N, double>{values};
    double seq;
    stensor<N, double> n;
    st2tost2<N, double> dn;
    std::tie(seq, n, dn) =
        computeBarlatStressSecondDerivative(s, l1, l2, a, seps);
    tfel::math::st2tost2<N, double> dnn;
    for (size_type i = 0; i != s.size(); ++i) {
      auto sb = s;
      sb[i] += ds;
      double seq_p, seq_m;
      stensor<N, double> n_p, n_m;
      std::tie(seq_p, n_p) = computeBarlatStressNormal(sb, l1, l2, a, seps);
      sb[i] -= 2 * ds;
      std::tie(seq_m, n_m) = computeBarlatStressNormal(sb, l1, l2, a, seps);
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
};  // end of BarlatSecondDerivativeTest

TFEL_TESTS_GENERATE_PROXY(BarlatSecondDerivativeTest,
                          "BarlatSecondDerivativeTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("BarlatSecondDerivative.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
