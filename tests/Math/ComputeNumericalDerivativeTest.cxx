/*!
 * \file   ComputeNumericalDerivativeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   11/10/2021
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
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

struct ComputeNumericalDerivativeTest final : public tfel::tests::TestCase {
  ComputeNumericalDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "ComputeNumericalDerivativeTest") {
  }  // end of ComputeNumericalDerivativeTest
  tfel::tests::TestResult execute() override {
    test1();
    this->test2<1u>();
    this->test2<2u>();
    this->test2<3u>();
    this->test3<1u>();
    this->test3<2u>();
    this->test3<3u>();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    constexpr auto eps = 1.e-12;
    const auto df = tfel::math::computeNumericalDerivative(
        [](const auto& v) { return v * v; }, 0.5, 1.e-2);
    TFEL_TESTS_ASSERT(std::abs(df - 1) < eps);
  }
  template <unsigned short N>
  void test2() {
    using namespace tfel::math;
    constexpr auto eps = 1.e-12;
    using Stensor = stensor<N, double>;
    const auto trace_derivative = computeNumericalDerivative(
        [](const auto& s) { return trace(s); }, Stensor::Id(), 1.e-2);
    TFEL_TESTS_ASSERT(abs(trace_derivative - Stensor::Id()) < eps);
  }
  template <unsigned short N>
  void test3() {
    using namespace tfel::math;
    constexpr auto eps = 1.e-12;
    using Stensor = stensor<N, double>;
    using Stensor4 = st2tost2<N, double>;
    const auto deviator_derivative = computeNumericalDerivative(
        [](const auto& s) { return deviator(s); }, Stensor::Id(), 1.e-2);
    TFEL_TESTS_ASSERT(abs(deviator_derivative - Stensor4::K()) < eps);
  }
};

TFEL_TESTS_GENERATE_PROXY(ComputeNumericalDerivativeTest,
                          "ComputeNumericalDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ComputeNumericalDerivativeTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
