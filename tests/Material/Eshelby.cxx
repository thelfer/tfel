/*!
 * \file   tests/Material/Eshelby.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/06/2021
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
#include "TFEL/Material/Eshelby.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct EshelbyTest final : public tfel::tests::TestCase {
  EshelbyTest()
      : tfel::tests::TestCase("TFEL/Material", "Eshelby") {}  // end of EshelbyTest
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
    using length =
        typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    //constexpr auto eps = 10 * std::numeric_limits<stress>::epsilon();
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.3};
    const auto a =length{1};
    const auto b = length{0.5};
    const auto c = length{0.501};
    const auto S = tfel::material::EshelbyTensor(nu,a,b,c);
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::math::st2tost2<3u,stress> C_i;
    tfel::material::computeIsotropicStiffnessTensorII<3u,value,stress,real>(C_i,young_i,nu_i);
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    using namespace tfel::material;
    //const auto A = SphereLocalisationTensor<real,stress>(young,nu,C_i);
    const auto A = GeneralEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,a,n_b,b,c);
    //const auto A = AxisymEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,a/b);
    for (const auto& i : {0,1,2,3,4,5}){
    	for (const auto& j : {0,1,2,3,4,5}){
    		std::cout << i << " " << j << " " << A(i,j) << '\n';
    					     };
    					 };
    //constexpr auto l_ref = young * nu / ((1 + nu) * (1 - 2 * nu));
    //constexpr auto m_ref = young / (2 * (1 + nu));
    //TFEL_TESTS_STATIC_ASSERT(my_abs(l - l_ref) < eps);
    //TFEL_TESTS_STATIC_ASSERT(my_abs(m - m_ref) < eps);
  }
};  // end of struct EshelbyTest

TFEL_TESTS_GENERATE_PROXY(EshelbyTest, "Eshelby");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Eshelby.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
