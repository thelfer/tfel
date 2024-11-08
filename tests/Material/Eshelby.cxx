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
    using lg =
        typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.3};
    //tests the Eshelby tensor functions
    using namespace tfel::material;
    if (true) {const auto S = computeSphereEshelbyTensor(nu);}
    
    if (true) {const auto S1 = computeAxisymmetricalEshelbyTensor(nu,real{0.5});
    const auto S2 = computeAxisymmetricalEshelbyTensor(nu,real{10});
    const auto S3 = computeAxisymmetricalEshelbyTensor(nu,real{1});}
    if (true) {const auto S = computeEshelbyTensor(nu,lg{1},lg{3},lg{2});}
    
    if (true) {const auto S1 = computeEshelbyTensor(nu,lg{1},lg{3},lg{2});
    const auto S2 = computeEshelbyTensor(nu,lg{1},lg{1},lg{2});
    const auto S3 = computeEshelbyTensor(nu,lg{1},lg{1},lg{1});}
    
    //must return a warning
    //const auto S = computeSphereEshelbyTensor(real{1});
    //const auto S = computeAxisymmetricalEshelbyTensor(nu,real{0});
    //const auto S = computeAxisymmetricalEshelbyTensor(nu,real{-10});
    //const auto S = computeEshelbyTensor(nu,length{-1},length{3},length{2});
    //const auto S = computeEshelbyTensor<real,length>(nu,length{-1},length{-1},length{1});
    
    //These functions must return the same thing
    if (true) {const auto S1 = computeSphereEshelbyTensor(nu);
    const auto S2 = computeAxisymmetricalEshelbyTensor(nu,real{1});
    const auto S3 = computeEshelbyTensor(nu,lg{1},lg{2},lg{2});
    const auto S4 = computeEshelbyTensor(nu,lg{3},lg{3},lg{3});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (S1(i,j)==S2(i,j));
    		//std::cout << S1(i,j) << " " << S2(i,j) << " " << value << '\n';
    		value = value and (S1(i,j)==S3(i,j));
    		//std::cout << "13" << value << '\n';
    		value = value and (S1(i,j)==S4(i,j));
    		//std::cout << "14" << value << '\n';
    	};
    };
    std::cout <<value << '\n';}
    
    if (true) {const auto S1 = computeAxisymmetricalEshelbyTensor(nu,real{10});
    const auto S2 = computeEshelbyTensor(nu,lg{30},lg{3},lg{3});
    const auto S3 = computeEshelbyTensor(nu,lg{3},lg{3},lg{30});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (S1(i,j)==S2(i,j));
    		value = value and (S1(i,j)==S3(i,j));
    	};
    };
    
    std::cout << value << '\n';}
    
    //These functions must return a very similar thing
    
    
    
    
  

    //tests the localisation tensor functions
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::math::st2tost2<3u,stress> C_i;
    tfel::material::computeIsotropicStiffnessTensorII<3u,value,stress,real>(C_i,young_i,nu_i);
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    const auto a =lg{1};
    const auto b =lg{3};
    const auto c =lg{0.2};
    using namespace tfel::material;
    
    if (true) {const auto A1 = computeSphereLocalisationTensor<real,stress>(young,nu,C_i);
    const auto A2 = computeAxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,a/b);
    const auto A3 = computeEllipsoidLocalisationTensor<real,stress,lg>(young,nu,C_i,n_a,a,n_b,b,c);}
    
    //must return a warning
    //const auto A = computeSphereLocalisationTensor<real,stress>(young,real{1},C_i);
    //const auto A = computeAxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,real{0});
    //const auto A = computeEllipsoidLocalisationTensor<real,stress,length>(young,nu,C_i,n_a,a,n_b,length{0},c);
    
    //These functions must return the same thing
    if (true) {const auto A1 = computeSphereLocalisationTensor<real,stress>(young,nu,C_i);
    const auto A2 = computeAxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,lg{2}/lg{2});
    const auto A3 = computeEllipsoidLocalisationTensor<real,stress,lg>(young,nu,C_i,n_a,lg{2},n_b,lg{2},lg{2});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (A1(i,j)==A2(i,j));
    		value = value and (A1(i,j)==A3(i,j));
    	};
    };
    
    std::cout << value << '\n';}
    
    if (true) {const auto A1 = computeAxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,lg{20}/lg{2});
    const auto A2 = computeEllipsoidLocalisationTensor<real,stress,lg>(young,nu,C_i,n_a,lg{20},n_b,lg{2},lg{2});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (A1(i,j)==A2(i,j));
    		//std::cout << A1(i,j) <<" " << i<< " " << j<< " " << A2(i,j) << value << '\n';
    	};
    };
    
    std::cout << value << '\n';}
    
    //These functions must return a very similar thing


	


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
