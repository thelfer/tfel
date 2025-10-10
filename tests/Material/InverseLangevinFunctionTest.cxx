/*!
 * \file   tests/Material/InverseLangevinFunction.cxx
 * \brief
 * \author Thomas Hefler
 * \date   08/09/2022
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

#include <array>
#include <cmath>
#include <limits>
#include <cassert>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/InverseLangevinFunction.hxx"

struct InverseLangevinFunctionTest final : public tfel::tests::TestCase {
  InverseLangevinFunctionTest()
      : tfel::tests::TestCase("TFEL/Material", "InverseLangevinFunctionTest") {
  }  // end of InverseLangevinFunctionTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 protected:
  static constexpr auto reference_values = std::array<double, 4u>{
      10.373684210526, 9.9806683997261, 8.8784947381229, 10};
  //
  void test1() {
    using namespace tfel::material;
    using ApproximationFunctions = InverseLangevinFunctionApproximations;
    constexpr auto eps = double{1e-10};
    constexpr auto y = double{0.9};
    const auto values = std::array<double, 4u>{
        computeApproximateInverseLangevinFunction<
            ApproximationFunctions::COHEN_1991>(y),
        computeApproximateInverseLangevinFunction<
            ApproximationFunctions::JEDYNAK_2015>(y),
        computeApproximateInverseLangevinFunction<
            ApproximationFunctions::MORCH_2022>(y),
        computeBergstromBoyce1998ApproximateInverseLangevinFunction(y)};
    for (std::array<double, 4u>::size_type i = 0; i != values.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(values[i] - reference_values[i]) < eps);
    }
  }  // end of test1
  void test2() {
    using namespace tfel::material;
    using ApproximationFunctions = InverseLangevinFunctionApproximations;
    constexpr auto eps = double{1e-10};
    constexpr auto eps2 = double{1e-6};
    constexpr auto y = double{0.9};
    constexpr auto dy = double{1e-8};
    const auto f1 = static_cast<std::pair<double, double> (*)(double)>(
        computeApproximateInverseLangevinFunctionAndDerivative<
            ApproximationFunctions::COHEN_1991, double>);
    const auto f2 = static_cast<std::pair<double, double> (*)(double)>(
        computeApproximateInverseLangevinFunctionAndDerivative<
            ApproximationFunctions::JEDYNAK_2015, double>);
    const auto f3 = static_cast<std::pair<double, double> (*)(double)>(
        computeApproximateInverseLangevinFunctionAndDerivative<
            ApproximationFunctions::MORCH_2022, double>);
    const auto f4 = static_cast<std::pair<double, double> (*)(double)>(
        computeBergstromBoyce1998ApproximateInverseLangevinFunctionAndDerivative<
            double>);
    const auto values =
        std::array<std::pair<double, double>, 4u>{f1(y), f2(y), f3(y), f4(y)};
    const auto dvalues = std::array<double, 4u>{
        (f1(y + dy).first - f1(y - dy).first) / (2 * dy),
        (f2(y + dy).first - f2(y - dy).first) / (2 * dy),
        (f3(y + dy).first - f3(y - dy).first) / (2 * dy),
        (f4(y + dy).first - f4(y - dy).first) / (2 * dy)};
    for (std::array<double, 4u>::size_type i = 0; i != values.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(values[i].first - reference_values[i]) < eps);
      TFEL_TESTS_ASSERT(std::abs(values[i].second - dvalues[i]) < eps2);
    }
  }  // end of test2
};

TFEL_TESTS_GENERATE_PROXY(InverseLangevinFunctionTest,
                          "InverseLangevinFunctionTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("InverseLangevinFunctionTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
