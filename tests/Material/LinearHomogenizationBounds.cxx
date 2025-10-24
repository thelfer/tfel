/*!
 * \file   tests/Material/LinearHomogenizationBounds.cxx
 * \brief
 * \author Antoine Martin
 * \date   23 January 2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif NDEBUG

#include <limits>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#ifndef _LIBCPP_VERSION
#include "TFEL/Material/LinearHomogenizationBounds.hxx"
#endif /* _LIBCPP_VERSION */
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#ifndef _LIBCPP_VERSION
template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}
#endif /* _LIBCPP_VERSION */

struct LinearHomogenizationBoundsTest final : public tfel::tests::TestCase {
  LinearHomogenizationBoundsTest()
      : tfel::tests::TestCase("TFEL/Material", "LinearHomogenizationBounds") {
  }  // end of LinearHomogenizationBoundsTest
  tfel::tests::TestResult execute() override {
    using real = double;
    constexpr bool qt = true;
    using stress = typename tfel::config::Types<1u, real, qt>::stress;

    this->template testHS_3D<stress>();
    this->template testHS_2D<stress>();
    this->template testHS_3D<real>();
    this->template testHS_2D<real>();

    return this->result;
  }

 private:
  template <tfel::math::ScalarConcept stress>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              stress>()) void testHS_3D() {
#ifndef _LIBCPP_VERSION
    // we just compare a direct formula for two phases and the function for N=2
    // phases
    using real = tfel::types::real<stress>;
    using namespace tfel::material::homogenization::elasticity;
    constexpr auto eps = 100 * tfel::math::constexpr_fct::sqrt(
                                   std::numeric_limits<real>::epsilon());

    const auto young1 = stress{1e9};
    const auto seps = young1 * eps;
    const auto nu1 = real{0.3};
    const auto young2 = stress{150e9};
    const auto nu2 = real{0.2};
    std::array<real, 2> tab_f;
    tab_f[0] = real(0.1);
    tab_f[1] = real(0.9);
    std::array<stress, 2> tab_K;
    tab_K[0] = young1 / 3 / (1 - 2 * nu1);
    tab_K[1] = young2 / 3 / (1 - 2 * nu2);
    std::array<stress, 2> tab_mu;
    tab_mu[0] = young1 / 2 / (1 + nu1);
    tab_mu[1] = young2 / 2 / (1 + nu2);
    const auto pair = computeIsotropicHashinShtrikmanBounds<3u, 2, stress>(
        tab_f, tab_K, tab_mu);
    const auto LB = std::get<0>(pair);
    const auto UB = std::get<1>(pair);
    const auto K_L = LB.kappa;
    const auto mu_L = LB.mu;
    const auto K_U = UB.kappa;
    const auto mu_U = UB.mu;

    const auto phi1 = tab_f[0];
    const auto phi2 = tab_f[1];
    const auto K1 = tab_K[0];
    const auto K2 = tab_K[1];
    const auto mu1 = tab_mu[0];
    const auto mu2 = tab_mu[1];
    const auto d = 3;
    const auto K_Lbis =
        phi1 * K1 + phi2 * K2 -
        phi1 * phi2 * (K1 - K2) * (K1 - K2) /
            (phi1 * K2 + phi2 * K1 + 2 * (d - 1) / real(d) * mu1);
    const auto H1 =
        mu1 * (d * K1 / 2 + (d + 1) * (d - 2) * mu1 / real(d)) / (K1 + 2 * mu1);
    const auto mu_Lbis = phi1 * mu1 + phi2 * mu2 -
                         phi1 * phi2 * (mu1 - mu2) * (mu1 - mu2) /
                             (phi1 * mu2 + phi2 * mu1 + H1);
    const auto K_Ubis =
        phi1 * K1 + phi2 * K2 -
        phi1 * phi2 * (K1 - K2) * (K1 - K2) /
            (phi1 * K2 + phi2 * K1 + 2 * (d - 1) / real(d) * mu2);
    const auto H2 =
        mu2 * (d * K2 / 2 + (d + 1) * (d - 2) * mu2 / real(d)) / (K2 + 2 * mu2);
    const auto mu_Ubis = phi1 * mu1 + phi2 * mu2 -
                         phi1 * phi2 * (mu1 - mu2) * (mu1 - mu2) /
                             (phi1 * mu2 + phi2 * mu1 + H2);

    TFEL_TESTS_ASSERT(my_abs(K_L - K_Lbis) < seps);
    TFEL_TESTS_ASSERT(my_abs(mu_L - mu_Lbis) < seps);
    TFEL_TESTS_ASSERT(my_abs(K_U - K_Ubis) < seps);
    TFEL_TESTS_ASSERT(my_abs(mu_U - mu_Ubis) < seps);
#endif /* _LIBCPP_VERSION */
  }

 private:
  template <tfel::math::ScalarConcept stress>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              stress>()) void testHS_2D() {
#ifndef _LIBCPP_VERSION
    // we just compare a direct formula for two phases and the function for N=2
    // phases
    using real = tfel::types::real<stress>;
    using namespace tfel::material::homogenization::elasticity;
    constexpr auto eps = 100 * tfel::math::constexpr_fct::sqrt(
                                   std::numeric_limits<real>::epsilon());

    const auto young1 = stress{1e9};
    const auto seps = young1 * eps;
    const auto nu1 = real{0.3};
    const auto young2 = stress{150e9};
    const auto nu2 = real{0.2};
    std::array<real, 2> tab_f;
    tab_f[0] = real(0.2);
    tab_f[1] = real(0.8);
    std::array<stress, 2> tab_K;
    tab_K[0] = young1 / 3 / (1 - 2 * nu1);
    tab_K[1] = young2 / 3 / (1 - 2 * nu2);
    std::array<stress, 2> tab_mu;
    tab_mu[0] = young1 / 2 / (1 + nu1);
    tab_mu[1] = young2 / 2 / (1 + nu2);
    const auto pair = computeIsotropicHashinShtrikmanBounds<2u, 2, stress>(
        tab_f, tab_K, tab_mu);
    const auto LB = std::get<0>(pair);
    const auto UB = std::get<1>(pair);
    const auto K_L = LB.kappa;
    const auto mu_L = LB.mu;
    const auto K_U = UB.kappa;
    const auto mu_U = UB.mu;

    const auto phi1 = tab_f[0];
    const auto phi2 = tab_f[1];
    const auto K1 = tab_K[0];
    const auto K2 = tab_K[1];
    const auto mu1 = tab_mu[0];
    const auto mu2 = tab_mu[1];
    const auto d = 2;
    const auto K_Lbis =
        phi1 * K1 + phi2 * K2 -
        phi1 * phi2 * (K1 - K2) * (K1 - K2) /
            (phi1 * K2 + phi2 * K1 + 2 * (d - 1) / real(d) * mu1);
    const auto H1 =
        mu1 * (d * K1 / 2 + (d + 1) * (d - 2) * mu1 / real(d)) / (K1 + 2 * mu1);
    const auto mu_Lbis = phi1 * mu1 + phi2 * mu2 -
                         phi1 * phi2 * (mu1 - mu2) * (mu1 - mu2) /
                             (phi1 * mu2 + phi2 * mu1 + H1);
    const auto K_Ubis =
        phi1 * K1 + phi2 * K2 -
        phi1 * phi2 * (K1 - K2) * (K1 - K2) /
            (phi1 * K2 + phi2 * K1 + 2 * (d - 1) / real(d) * mu2);
    const auto H2 =
        mu2 * (d * K2 / 2 + (d + 1) * (d - 2) * mu2 / real(d)) / (K2 + 2 * mu2);
    const auto mu_Ubis = phi1 * mu1 + phi2 * mu2 -
                         phi1 * phi2 * (mu1 - mu2) * (mu1 - mu2) /
                             (phi1 * mu2 + phi2 * mu1 + H2);

    TFEL_TESTS_ASSERT(my_abs(K_L - K_Lbis) < seps);
    TFEL_TESTS_ASSERT(my_abs((mu_L - mu_Lbis) / mu_L) < eps);
    TFEL_TESTS_ASSERT(my_abs(K_U - K_Ubis) < seps);
    TFEL_TESTS_ASSERT(my_abs((mu_U - mu_Ubis) / mu_U) < eps);
#endif /* _LIBCPP_VERSION */
  }
};  // end of struct LinearHomogenizationBoundsTest

TFEL_TESTS_GENERATE_PROXY(LinearHomogenizationBoundsTest,
                          "LinearHomogenizationBounds");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LinearHomogenizationBounds.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
