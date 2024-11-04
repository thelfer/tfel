/*!
 * \file   tests/Material/EshelbyBasedHomogenization.cxx
 * \brief
 * \author Antoine Martin
 * \date   25/10/2024
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Material/EshelbyBasedHomogenization.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct EshelbyBasedHomogenizationTest final : public tfel::tests::TestCase {
  EshelbyBasedHomogenizationTest()
      : tfel::tests::TestCase("TFEL/Material", "EshelbyBasedHomogenization") {}  // end of EshelbyBasedHomogenizationTest
  tfel::tests::TestResult execute() override {
    this->template test1<double, true>();
    //this->template test1<double, false>();
    //this->template test1<long double, false>();
    //this->template test1<float, true>();
    //this->template test1<double, true>();
    //this->template test1<long double, true>();
    return this->result;
  }

 private:
  template <typename NumericType, bool use_qt>
  void test1() {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = NumericType;
    using length =
        typename tfel::config::Types<1u, NumericType, use_qt>::length;
    //constexpr auto eps = 10 * std::numeric_limits<stress>::epsilon();
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.3};
    const auto a =length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.3};
    const auto S = tfel::material::EshelbyTensor<real,length>(nu,a,b,c);
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::math::st2tost2<3u,stress> C_i;
    tfel::material::computeIsotropicStiffnessTensorII<3u,value,stress,real>(C_i,young_i,nu_i);
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    using namespace tfel::material;
    //const auto A = SphereLocalisationTensor<real,stress>(young,nu,C_i);
    const auto Chom = IsotropicDiluteScheme<real,stress,length>(young,nu,real{0.5},C_i,a,b,c);
    //const auto Chom2 = SphereDiluteScheme<real,stress>(young,nu,real{0.5},C_i);
    const auto Chom2 = IsotropicDiluteScheme<real,stress,length>(young,nu,real{0.5},C_i,a,b,length{0.3001});
    const auto nuhom = Chom(0,1)/(Chom(0,1)+Chom(0,0));
    const auto Ehom = (1+nuhom)*(Chom(0,0)-Chom(0,1));
    std::cout << Ehom.getValue() << nuhom << '\n';
    const auto nuhom2 = Chom2(0,1)/(Chom2(0,1)+Chom2(0,0));
    const auto Ehom2 = (1+nuhom2)*(Chom2(0,0)-Chom2(0,1));
    std::cout << Ehom2.getValue() << nuhom2 << '\n';
    //for (const auto& i : {0,1,2,3,4,5}){
    //	for (const auto& j : {0,1,2,3,4,5}){
    //		std::cout << i << " " << j << " " << Chom(i,j).getValue() << '\n';
    //					     };
    //					 };
    //constexpr auto l_ref = young * nu / ((1 + nu) * (1 - 2 * nu));
    //constexpr auto m_ref = young / (2 * (1 + nu));
    //TFEL_TESTS_STATIC_ASSERT(my_abs(l - l_ref) < eps);
    //TFEL_TESTS_STATIC_ASSERT(my_abs(m - m_ref) < eps);
  }
};  // end of struct EshelbyBasedHomogenizationTest

TFEL_TESTS_GENERATE_PROXY(EshelbyBasedHomogenizationTest, "EshelbyBasedHomogenization");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("EshelbyBasedHomogenization.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
