/*!
 * \file   tests/Material/HomogenizationSecondMoments.cxx
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
#include "TFEL/Math/types.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#ifndef _LIBCPP_VERSION
#include "TFEL/Material/HomogenizationSecondMoments.hxx"
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

struct HomogenizationSecondMomentsTest final : public tfel::tests::TestCase {
  HomogenizationSecondMomentsTest()
      : tfel::tests::TestCase("TFEL/Material", "HomogenizationSecondMoments") {
  }  // end of HomogenizationSecondMomentsTest
  tfel::tests::TestResult execute() override {
    using real = double;
    constexpr bool qt = true;
    using stress = typename tfel::config::Types<1u, real, qt>::stress;

    this->template test_e2_r<stress>();
    this->template test_e2_s<stress>();
    this->template test_e2_r<real>();
    this->template test_e2_s<real>();

    return this->result;
  }

 private:
  template <tfel::math::ScalarConcept stress>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              stress>()) void test_e2_r() {
#ifndef _LIBCPP_VERSION
    //here we test for stiff inclusions in soft matrix. The derivative is performed by finite difference
    using real = tfel::types::real<stress>;
    constexpr real eps_d = 1e-6;
    using namespace tfel::material;
    const auto k0 = stress(2e9);
    const auto mu0 = stress(3e9);
    const auto ki = stress(5e12);
    const auto mui = stress(6e12);
    const auto kg = KGModuli<stress>(k0,mu0);
    const auto kgi = KGModuli<stress>(ki,mui);
    const real f = 0.2;
    using namespace tfel::material::homogenization::elasticity;
    const auto em2 = computeMeanSquaredHydrostaticStrain(kg,f,kgi,real(1),real(1));
    const auto em20=std::get<0>(em2);
    const auto em2i=std::get<1>(em2);
    const auto eeq2 = computeMeanSquaredEquivalentStrain(kg,f,kgi,real(1),real(1));
    const auto eeq20=std::get<0>(eeq2);
    const auto eeq2i=std::get<1>(eeq2);
    
    std::vector tab_f = {real(1-f),f};
    std::vector tab_k = {k0,ki};
    std::vector tab_mu = {mu0,mui};
    std::vector tab_dmu0 = {mu0+eps_d*mu0,mui};
    std::vector tab_dmui = {mu0,mui+eps_d*mui};
    std::vector tab_dk0 = {k0+eps_d*k0,ki};
    std::vector tab_dki = {k0,ki+eps_d*ki};
    
    const auto HS = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_k,tab_mu);
    const auto LB = std::get<0>(HS);
    
    const auto HS_dmu0 = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_k,tab_dmu0);
    const auto LB_dmu0 = std::get<0>(HS_dmu0);
    const auto dk_dmu0 = (std::get<0>(LB_dmu0)-std::get<0>(LB))/eps_d/mu0;
    const auto dmu_dmu0 = (std::get<1>(LB_dmu0)-std::get<1>(LB))/eps_d/mu0;
    
    const auto HS_dmui = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_k,tab_dmui);
    const auto LB_dmui = std::get<0>(HS_dmui);
    const auto dmu_dmui = (std::get<1>(LB_dmui)-std::get<1>(LB))/eps_d/mui;
    
    const auto HS_dk0 = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_dk0,tab_mu);
    const auto LB_dk0 = std::get<0>(HS_dk0);
    const auto dk_dk0 = (std::get<0>(LB_dk0)-std::get<0>(LB))/eps_d/k0;
    const auto dmu_dk0 = (std::get<1>(LB_dk0)-std::get<1>(LB))/eps_d/k0;
    
    const auto HS_dki = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_dki,tab_mu);
    const auto LB_dki = std::get<0>(HS_dki);
    const auto dk_dki = (std::get<0>(LB_dki)-std::get<0>(LB))/eps_d/ki;
    
    const auto der_k0 = 9*dk_dk0/2+3*dmu_dk0/2;
    const auto der_ki = 9*dk_dki/2;
    const auto der_mu0 = 9*dk_dmu0/2+3*dmu_dmu0/2;
    const auto der_mui = 3*dmu_dmui/2;
    
    const auto em20_ = 2/(1-f)*der_k0/9;
    const auto em2i_ = 2/f*der_ki/9;
    const auto eeq20_ = 2/(1-f)*der_mu0/3;
    const auto eeq2i_ = 2/f*der_mui/3;
    
    TFEL_TESTS_ASSERT(my_abs(em20-em20_) < eps_d);
    TFEL_TESTS_ASSERT(my_abs(em2i-em2i_) < eps_d);
    TFEL_TESTS_ASSERT(my_abs(eeq20-eeq20_) < eps_d);
    TFEL_TESTS_ASSERT(my_abs(eeq2i-eeq2i_) < eps_d);
    
#endif /* _LIBCPP_VERSION */
  }

 private:
  template <tfel::math::ScalarConcept stress>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              stress>()) void test_e2_s() {
#ifndef _LIBCPP_VERSION
    //here we test for soft inclusions in stiff matrix. The derivative is performed by finite difference
    using real = tfel::types::real<stress>;
    constexpr real eps_d = 1e-6;
    using namespace tfel::material;
    const auto k0 = stress(2e9);
    const auto mu0 = stress(3e9);
    const auto ki = stress(5e7);
    const auto mui = stress(6e7);
    const auto kg = KGModuli<stress>(k0,mu0);
    const auto kgi = KGModuli<stress>(ki,mui);
    const real f = 0.2;
    using namespace tfel::material::homogenization::elasticity;
    const auto em2 = computeMeanSquaredHydrostaticStrain(kg,f,kgi,real(1),real(1));
    const auto em20=std::get<0>(em2);
    const auto em2i=std::get<1>(em2);
    const auto eeq2 = computeMeanSquaredEquivalentStrain(kg,f,kgi,real(1),real(1));
    const auto eeq20=std::get<0>(eeq2);
    const auto eeq2i=std::get<1>(eeq2);
    
    std::vector tab_f = {real(1-f),f};
    std::vector tab_k = {k0,ki};
    std::vector tab_mu = {mu0,mui};
    std::vector tab_dmu0 = {mu0+eps_d*mu0,mui};
    std::vector tab_dmui = {mu0,mui+eps_d*mui};
    std::vector tab_dk0 = {k0+eps_d*k0,ki};
    std::vector tab_dki = {k0,ki+eps_d*ki};
    
    const auto HS = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_k,tab_mu);
    const auto UB = std::get<1>(HS);
    
    const auto HS_dk0 = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_dk0,tab_mu);
    const auto UB_dk0 = std::get<1>(HS_dk0);
    const auto dk_dk0 = (std::get<0>(UB_dk0)-std::get<0>(UB))/eps_d/k0;
    const auto dmu_dk0 = (std::get<1>(UB_dk0)-std::get<1>(UB))/eps_d/k0;
    
    const auto HS_dki = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_dki,tab_mu);
    const auto UB_dki = std::get<1>(HS_dki);
    const auto dk_dki = (std::get<0>(UB_dki)-std::get<0>(UB))/eps_d/ki;
    
    const auto HS_dmu0 = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_k,tab_dmu0);
    const auto UB_dmu0 = std::get<1>(HS_dmu0);
    const auto dk_dmu0 = (std::get<0>(UB_dmu0)-std::get<0>(UB))/eps_d/mu0;
    const auto dmu_dmu0 = (std::get<1>(UB_dmu0)-std::get<1>(UB))/eps_d/mu0;
    
    const auto HS_dmui = computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_k,tab_dmui);
    const auto UB_dmui = std::get<1>(HS_dmui);
    const auto dmu_dmui = (std::get<1>(UB_dmui)-std::get<1>(UB))/eps_d/mui;
    
    const auto der_k0 = 9*dk_dk0/2+3*dmu_dk0/2;
    const auto der_ki = 9*dk_dki/2;
    const auto der_mu0 = 9*dk_dmu0/2+3*dmu_dmu0/2;
    const auto der_mui = 3*dmu_dmui/2;
    
    const auto em20_ = 2/(1-f)*der_k0/9;
    const auto em2i_ = 2/f*der_ki/9;
    const auto eeq20_ = 2/(1-f)*der_mu0/3;
    const auto eeq2i_ = 2/f*der_mui/3;
    
    TFEL_TESTS_ASSERT(my_abs(em20-em20_) < eps_d);
    TFEL_TESTS_ASSERT(my_abs(em2i-em2i_) < eps_d);
    TFEL_TESTS_ASSERT(my_abs(eeq20-eeq20_) < eps_d);
    TFEL_TESTS_ASSERT(my_abs(eeq2i-eeq2i_) < eps_d);

#endif /* _LIBCPP_VERSION */
  }
};  // end of struct HomogenizationSecondMomentsTest

TFEL_TESTS_GENERATE_PROXY(HomogenizationSecondMomentsTest,
                          "HomogenizationSecondMoments");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("HomogenizationSecondMoments.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
