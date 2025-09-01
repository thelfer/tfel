/*!
 * \file   ComputeDeviatorDeterminantDerivativeTest.cxx
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

#include "TFEL/Math/stensor.hxx"

template <unsigned short N, typename real>
static tfel::math::stensor<N, real> getNumericalApproximation(
    const tfel::math::stensor<N, real>& C, const real e) {
  tfel::math::stensor<N, real> r;
  for (unsigned short j = 0; j != C.size(); ++j) {
    auto Cp = C;
    auto Cm = C;
    Cp(j) += e;
    Cm(j) -= e;
    const auto Jp = det(deviator(Cp));
    const auto Jm = det(deviator(Cm));
    const auto dJ = (Jp - Jm) / (2 * e);
    r(j) = dJ;
  }
  return r;
}  // end of getNumericalApproximation

struct ComputeDeviatorDeterminantDerivativeTest final
    : public tfel::tests::TestCase {
  ComputeDeviatorDeterminantDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "ComputeDeviatorDeterminantDerivativeTest") {
  }  // end of ComputeDeviatorDeterminantDerivativeTest
  tfel::tests::TestResult execute() override {
    const double sqrt2 = tfel::math::Cste<double>::sqrt2;
    const double v0[6] = {0, 0, 0, 0, 0, 0};
    const double v1[6] = {8.2e-2, -4.5e-2, 7.2e-2, 2.3e-2 * sqrt2, 0, 0.};
    const double v2[6] = {-8.2e-2, 4.5e-2, 7.2e-2, 0e-2, 2.3e-2 * sqrt2, 0.};
    const double v3[6] = {8.2e-2, 4.5e-2, -7.2e-2, 0e-2, 0.e-2, 2.3e-2 * sqrt2};
    const double v4[6] = {8.2e-2,          4.5e-2,           -7.2e-2,
                          3.14e-2 * sqrt2, -6.42e-2 * sqrt2, 2.3e-2 * sqrt2};
    this->check(tfel::math::stensor<1u, double>(v0));
    this->check(tfel::math::stensor<2u, double>(v0));
    this->check(tfel::math::stensor<3u, double>(v0));
    this->check(tfel::math::stensor<1u, double>(v1));
    this->check(tfel::math::stensor<2u, double>(v1));
    this->check(tfel::math::stensor<3u, double>(v1));
    this->check(tfel::math::stensor<1u, double>(v2));
    this->check(tfel::math::stensor<2u, double>(v2));
    this->check(tfel::math::stensor<3u, double>(v2));
    this->check(tfel::math::stensor<1u, double>(v3));
    this->check(tfel::math::stensor<2u, double>(v3));
    this->check(tfel::math::stensor<3u, double>(v3));
    this->check(tfel::math::stensor<1u, double>(v4));
    this->check(tfel::math::stensor<2u, double>(v4));
    this->check(tfel::math::stensor<3u, double>(v4));
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void check(const tfel::math::stensor<N, double>& s) {
    constexpr const auto eps = 1e-6;
    constexpr const auto prec = 2e-13;
    const auto ndJ = getNumericalApproximation(s, eps);
    const auto dJ = computeDeviatorDeterminantDerivative(s);
    for (unsigned short i = 0; i != s.size(); ++i) {
      if (std::abs(dJ(i) - ndJ(i)) > prec) {
        std::cout << "dJ : " << i << " " << dJ(i) << " " << ndJ(i) << " "
                  << std::abs(dJ(i) - ndJ(i)) << std::endl;
      }
      TFEL_TESTS_ASSERT(std::abs(dJ(i) - ndJ(i)) < prec);
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(ComputeDeviatorDeterminantDerivativeTest,
                          "ComputeDeviatorDeterminantDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ComputeDeviatorDeterminantDerivativeTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
