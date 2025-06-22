/*!
 * \file
 * tests/Math/ConvertionBetweenCorotationnalCauchyStressAndSecondPioloKirchhoffStressTest.cxx
 * \brief
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"

struct
    ConvertionBetweenCorotationnalCauchyStressAndSecondPioloKirchhoffStressTest
        final : public tfel::tests::TestCase {
  ConvertionBetweenCorotationnalCauchyStressAndSecondPioloKirchhoffStressTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "ConvertionBetweenCorotationnalCauchyStressAndSec"
                              "ondPioloKirchhoffStressTest") {
  }  // end of
     // ConvertionBetweenCorotationnalCauchyStressAndSecondPioloKirchhoffStressTest
  virtual tfel::tests::TestResult execute() override {
    this->test1<1u>();
    this->test1<2u>();
    this->test1<3u>();
    this->test2<1u>();
    this->test2<2u>();
    this->test2<3u>();
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void test1() {
    using namespace tfel::math;
    const double eps = 1.e-13;
    const double u_values[6u] = {1.04, 0.98, 1.13, 0.08, -0.05, 0.12};
    const double s_values[6u] = {150, 45, -53, 32, 121, -98};
    stensor<N, double> U(u_values);
    stensor<N, double> S(s_values);
    const auto s1 =
        convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(S, U);
    const auto s2 = 1 / det(U) * syme(U * S * U);
    for (typename stensor<N, double>::size_type i = 0; i != s1.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(s1[i] - s2[i]) < eps);
    }
  }  // end of test1
  template <unsigned short N>
  void test2() {
    using namespace tfel::math;
    const double eps = 1.e-13;
    const double u_values[6u] = {1.04, 0.98, 1.13, 0.08, -0.05, 0.12};
    const double s_values[6u] = {150, 45, -53, 32, 121, -98};
    stensor<N, double> U(u_values);
    stensor<N, double> s(s_values);
    const auto S1 =
        convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(s, U);
    const auto iU = invert(U);
    const auto S2 = det(U) * syme(iU * s * iU);
    for (typename stensor<N, double>::size_type i = 0; i != S1.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(S1[i] - S2[i]) < eps);
    }
  }  // end of test2
};

TFEL_TESTS_GENERATE_PROXY(
    ConvertionBetweenCorotationnalCauchyStressAndSecondPioloKirchhoffStressTest,
    "ConvertionBetweenCorotationnalCauchyStressAndSecondPioloKirchhoffStressTes"
    "t");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput(
      "ConvertionBetweenCorotationnalCauchyStressAndSecondPioloKirchhoffStressT"
      "est.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
