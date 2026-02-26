/*!
 * \file   FiniteStrainHeatTransferTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/01/2026
 */

#include <cstdlib>
#include <iostream>
#include "TFEL/Math/qt.hxx"
#include "TFEL/Material/FiniteStrainHeatTransfer.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct FiniteStrainHeatTransferTest final : public tfel::tests::TestCase {
  FiniteStrainHeatTransferTest()
      : tfel::tests::TestCase("TFEL/Material", "FiniteStrainHeatTransfer") {
  }  // end of FiniteStrainHeatTransferTest
  tfel::tests::TestResult execute() override {
    this->test<true>();
    this->test<false>();
    return this->result;
  }  // end of execute
 private:
  template <bool use_qt>
  void test() noexcept {
    using namespace tfel::math;
    using namespace tfel::material;
    using real = typename tfel::config::ScalarTypes<double, use_qt>::real;
    constexpr auto eps = tfel::types::thermalconductivity<real>{1e-14};
    constexpr auto eps2 =
        derivative_type<tfel::types::thermalconductivity<real>,
                        tfel::types::temperature<real>>{1e-14};
    constexpr auto k = tfel::types::thermalconductivity<real>{3};
    constexpr auto dk_dT =
        derivative_type<tfel::types::thermalconductivity<real>,
                        tfel::types::temperature<real>>{3};
    constexpr auto id = tensor<3u, double>::Id();
    constexpr auto K =
        computeThermalConductivyMatrixInReferenceConfiguration(k, id);
    constexpr auto dK_dT =
        computeThermalConductivyMatrixInReferenceConfiguration(dk_dT, id);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(0, 0) - k) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(0, 1)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(0, 2)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(1, 0)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(1, 1) - k) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(1, 2)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(2, 0)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(2, 1)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(2, 2) - k) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(0, 0) - dk_dT) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(0, 1)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(0, 2)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(1, 0)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(1, 1) - dk_dT) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(1, 2)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(2, 0)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(2, 1)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(2, 2) - dk_dT) < eps2);
  }  // end of test
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainHeatTransferTest,
                          "FiniteStrainHeatTransfer");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainHeatTransfer.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
