/*!
 * \file   tests/Math/NumericalIntegration/GaussKronrodQuadratureTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/10/2025
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

#include <cmath>
#include <numbers>
#include <cstdlib>
#include <iostream>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/NumericalIntegration/GaussKronrodQuadrature.hxx"

struct GaussKronrodQuadratureTest final : public tfel::tests::TestCase {
  GaussKronrodQuadratureTest()
      : tfel::tests::TestCase("TFEL/Math/NumericalIntegration",
                              "GaussKronrodQuadrature") {}
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    //    this->test6();
    return this->result;
  }

 private:
  void test1() {
    constexpr auto a = double{};
    constexpr auto b = std::numbers::pi_v<double> / 4;
    const auto or1 = tfel::math::gauss_kronrod_integrate(
        [](const double x) noexcept { return std::sin(x); }, a, b);
    TFEL_TESTS_ASSERT(or1.has_value());
    TFEL_TESTS_ASSERT(std::abs(std::get<1>(*or1)) < 1e-14);
  }  // end of test1
  void test2() {
    constexpr auto a = double{};
    constexpr auto b = double{10};
    const auto oi = tfel::math::gauss_kronrod_integrate(
        [](const double x) noexcept { return std::exp(-x * x); }, a, b,
        {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
    TFEL_TESTS_ASSERT(oi.has_value());
    // comparison with octave's result
    if (oi.has_value()) {
      TFEL_TESTS_ASSERT(std::abs(*oi - 0.8862269254528) < 1e-11);
    }
  } // end of test2
  void test3() {
    const auto local_abs = [](const double e) { return e > 0 ? e : -e; };
    // left unbounded integral
    auto f1 = [](const double t) { return 1 / (1 + t * t); };
    constexpr auto or2 = tfel::math::gauss_kronrod_integrate(
        f1, -std::numeric_limits<double>::max(), double{},
        {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
    TFEL_TESTS_STATIC_ASSERT(or2.has_value());
    TFEL_TESTS_STATIC_ASSERT(local_abs(*or2 - std::numbers::pi_v<double> / 2) <
                             1e-12);
    // right unbounded integral
    constexpr auto or3 = tfel::math::gauss_kronrod_integrate(
        f1, double{}, std::numeric_limits<double>::max(),
        {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
    TFEL_TESTS_STATIC_ASSERT(or3.has_value());
    TFEL_TESTS_STATIC_ASSERT(local_abs(*or3 - std::numbers::pi_v<double> / 2) <
                             1e-12);
    // unbounded integral
    constexpr auto or4 = tfel::math::gauss_kronrod_integrate(
        f1, -std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max(),
        {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
    TFEL_TESTS_STATIC_ASSERT(or4.has_value());
    TFEL_TESTS_STATIC_ASSERT(local_abs(*or4 - std::numbers::pi_v<double>) <
                             1e-12);
  } // end of test3
  void test4() {
    constexpr auto a = double{};
    constexpr auto b = std::numbers::pi_v<double> / 4;
    const auto or1 = tfel::math::gauss_kronrod_integrate(
        [](const double x) noexcept -> tfel::math::tvector<2u> {
          return {std::sin(x), std::cos(x)};
        },
        a, b);
    TFEL_TESTS_ASSERT(or1.has_value());
    if (or1.has_value()) {
      const auto i = std::get<0>(*or1);
      TFEL_TESTS_ASSERT(std::abs(i[0] + std::cos(b) - 1) < 1e-14);
      TFEL_TESTS_ASSERT(std::abs(i[1] - std::sin(b)) < 1e-14);
      TFEL_TESTS_ASSERT(std::abs(std::get<1>(*or1)) < 1e-14);
    }
  } // end of test4
  void test5() {
    constexpr auto a = double{};
    constexpr auto b = std::numbers::pi_v<double> / 4;
    const auto or1 = tfel::math::gauss_kronrod_integrate(
							 [](const double x) noexcept -> tfel::math::tvector<2u, double> {
							   return {std::sin(x), std::cos(x)};
							 },
							 a, b,
							 {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
    TFEL_TESTS_ASSERT(or1.has_value());
    if (or1.has_value()) {
      TFEL_TESTS_ASSERT(std::abs((*or1)[0] + std::cos(b) - 1) < 1e-14);
      TFEL_TESTS_ASSERT(std::abs((*or1)[1] - std::sin(b)) < 1e-14);
    }
  }  // end of test5
  //  void test6() {
    // using time = tfel::config::ScalarTypes<double, true>::time;
    // using strainrate = tfel::config::ScalarTypes<double, true>::strainrate;
    // constexpr auto a = time{};
    // constexpr auto b = time{3600};
    // constexpr auto c = tfel::math::derivative_type<strainrate, time>{2.3e-5};
    // const auto or1 = tfel::math::gauss_kronrod_integrate(
    //  							 [c](const time t) noexcept { return c * t; }, a, b);
    // TFEL_TESTS_ASSERT(or1.has_value());
    // if (or1.has_value()) {
    //   TFEL_TESTS_ASSERT(tfel::math::abs(*or1 - strain(c * b * b / 2)) <
    // 			strain(1e-14));
    // }
  //  }  // end of test6
};

TFEL_TESTS_GENERATE_PROXY(GaussKronrodQuadratureTest,
                          "GaussKronrodQuadratureTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("gausskronrodquadrature.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
