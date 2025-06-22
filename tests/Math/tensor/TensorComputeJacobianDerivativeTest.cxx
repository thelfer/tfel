/*!
 * \file   TensorComputeJacobianDerivativeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   24 déc. 2016
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

#include "TFEL/Math/tensor.hxx"

template <unsigned short N, typename F, typename real>
static tfel::math::tensor<N, real> getNumericalApproximation(
    const F& f, const tfel::math::tensor<N, real>& C, const real e) {
  tfel::math::tensor<N, real> r;
  for (unsigned short j = 0; j != C.size(); ++j) {
    auto Cp = C;
    auto Cm = C;
    Cp(j) += e;
    Cm(j) -= e;
    const auto Fp = f(Cp);
    const auto Fm = f(Cm);
    const auto dF = (Fp - Fm) / (2 * e);
    r(j) = dF;
  }
  return r;
}  // end of getNumericalApproximation

struct TensorComputeJacobianDerivativeTest final
    : public tfel::tests::TestCase {
  TensorComputeJacobianDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "TensorComputeJacobianDerivativeTest") {
  }  // end of TensorComputeJacobianDerivativeTest
  tfel::tests::TestResult execute() override {
    const double sqrt2 = std::sqrt(2.);
    const double v0[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double v1[9] = {8.2e-2, -4.5e-2, 7.2e-2, 2.3e-2 * sqrt2, 0,
                          0.,     7.2e-2,  0e-2,   2.3e-2 * sqrt2};
    const double v2[9] = {-8.2e-2, 4.5e-2, 7.2e-2, 0e-2, 2.3e-2 * sqrt2, 0.};
    const double v3[9] = {8.2e-2,         4.5e-2, -7.2e-2, 0e-2, 0.e-2,
                          2.3e-2 * sqrt2, 4.5e-2, -7.2e-2, 0e-2};
    const double v4[9] = {8.2e-2,          4.5e-2,           -7.2e-2,
                          3.14e-2 * sqrt2, -6.42e-2 * sqrt2, 2.3e-2 * sqrt2,
                          4.5e-2,          -7.2e-2,          0e-2};
    this->check(tfel::math::tensor<1u, double>(v0));
    this->check(tfel::math::tensor<2u, double>(v0));
    this->check(tfel::math::tensor<3u, double>(v0));
    this->check(tfel::math::tensor<1u, double>(v1));
    this->check(tfel::math::tensor<2u, double>(v1));
    this->check(tfel::math::tensor<3u, double>(v1));
    this->check(tfel::math::tensor<1u, double>(v2));
    this->check(tfel::math::tensor<2u, double>(v2));
    this->check(tfel::math::tensor<3u, double>(v2));
    this->check(tfel::math::tensor<1u, double>(v3));
    this->check(tfel::math::tensor<2u, double>(v3));
    this->check(tfel::math::tensor<3u, double>(v3));
    this->check(tfel::math::tensor<1u, double>(v4));
    this->check(tfel::math::tensor<2u, double>(v4));
    this->check(tfel::math::tensor<3u, double>(v4));
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void check(const tfel::math::tensor<N, double>& s) {
    auto J = [](const tfel::math::tensor<N, double>& v) { return det(v); };
    TFEL_CONSTEXPR const auto eps = 1e-2;
    TFEL_CONSTEXPR const auto prec = 1e-13;
    auto ndJ = getNumericalApproximation(J, s, eps);
    auto dJ = computeDeterminantDerivative(s);
    for (unsigned short i = 0; i != s.size(); ++i) {
      if (std::abs(dJ(i) - ndJ(i)) > prec) {
        std::cout << "dJ : " << i << " " << dJ(i) << " " << ndJ(i) << " "
                  << std::abs(dJ(i) - ndJ(i)) << std::endl;
      }
      TFEL_TESTS_ASSERT(std::abs(dJ(i) - ndJ(i)) < prec);
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(TensorComputeJacobianDerivativeTest,
                          "TensorComputeJacobianDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TensorComputeJacobianDerivativeTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
