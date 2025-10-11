/*!
 * \file   tests/Material/IsotropicModuli.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   01/09/2025
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

#include <limits>
#include <cstdlib>
#include <iostream>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct IsotropicModuliTest final : public tfel::tests::TestCase {
  IsotropicModuliTest()
      : tfel::tests::TestCase("TFEL/Material", "IsotropicModuli") {
  }  // end of IsotropicModuliTest
  tfel::tests::TestResult execute() override {
    this->template test_1<float, true>();
    this->template test_1<float, false>();
    this->template test_1<double, true>();
    this->template test_1<double, false>();

    return this->result;
  }

 private:
  template <typename NumericType, bool use_qt>
  void test_1() {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = typename tfel::config::Types<1u, NumericType, use_qt>::real;
    constexpr auto eps = std::numeric_limits<real>::epsilon();
    constexpr auto kappa = stress{1.e9};
    constexpr auto mu = stress{2.e9};
    constexpr auto J = tfel::math::st2tost2<3u, real>::J();
    constexpr auto K = tfel::math::st2tost2<3u, real>::K();
    using namespace tfel::material;
    const auto tens = 3*kappa*J+2*mu*K;
    const auto yes_it_is_isotropic = isIsotropic<stress>(tens,eps);
    
    TFEL_TESTS_ASSERT(yes_it_is_isotropic);
    
    const auto KG = KGModuli<stress>(kappa,mu);
    const auto Enu=KG.ToYoungNu();
    const auto LG=KG.ToLambdaMu();
    const auto Enu_bis=LG.ToYoungNu();
   
    const auto E = Enu.young;
    const auto nu = Enu.nu;
    const auto E_bis = Enu_bis.young;
    const auto nu_bis = Enu_bis.nu;
    
    TFEL_TESTS_ASSERT( my_abs(E-E_bis)< stress(eps));
    TFEL_TESTS_ASSERT( my_abs(nu-nu_bis)< eps);
    
    const auto c11 = (1-nu)*E/(1+nu)/(1-2*nu);
    const auto c12 = nu*E/(1+nu)/(1-2*nu);
    const auto cmu = E/(1+nu);
    const auto z = stress(0);
    const tfel::math::st2tost2<3u,stress> tens2={c11,c12,c12,z, z, z,
    				     c12,c11,c12,z, z, z,
    				     c12,c12,c11,z, z, z,
    				     z,  z,  z, cmu,z, z,
    				     z,  z,  z, z, cmu, z,
    				     z,  z,  z, z, z, cmu };
    const auto err = relative_error<3u,stress> (tens,tens2);
    const auto C = computeIsotropicStiffnessTensor<stress>(KG);
    const auto KG_ = computeKGModuli<stress>(C);

    TFEL_TESTS_ASSERT(err < real(eps));
    TFEL_TESTS_ASSERT(my_abs((C(0,0) - c11)/c11) < 100*real(eps));
    TFEL_TESTS_ASSERT(my_abs((C(0,1) - c12)/c12) < 100*real(eps));
    TFEL_TESTS_ASSERT(my_abs((C(3,3) - cmu)/cmu) < 100*real(eps));
    
    TFEL_TESTS_ASSERT(my_abs((KG.kappa-KG_.kappa)/KG.kappa) < real(eps));
    TFEL_TESTS_ASSERT(my_abs((KG.mu-KG_.mu)/KG.mu) < 10*real(eps));
    
    const auto KappaMu= computeKappaMu<stress>(tens2);
    const auto Kappa_ = std::get<0>(KappaMu);
    const auto Mu_ = std::get<1>(KappaMu);
    
    TFEL_TESTS_ASSERT(my_abs(Kappa_-kappa)<stress(eps));
    
    TFEL_TESTS_ASSERT(my_abs(Mu_-mu)<stress(eps));
  }
};  // end of struct IsotropicModuliTest

TFEL_TESTS_GENERATE_PROXY(IsotropicModuliTest, "IsotropicModuli");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("IsotropicModuli.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
