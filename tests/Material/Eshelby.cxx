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
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.3};
    //tests the Eshelby tensor functions
    using namespace tfel::material;
    if (true) {const auto S = EshelbyTensorSphere(nu);}
    
    if (true) {const auto S1 = AxisymmetricalEshelbyTensor(nu,real{0.5});
    const auto S2 = AxisymmetricalEshelbyTensor(nu,real{10});
    const auto S3 = AxisymmetricalEshelbyTensor(nu,real{1});}
    
    if (true) {const auto S = EshelbyTensorGeneral(nu,length{1},length{3},length{2});}
    
    if (true) {const auto S1 = EshelbyTensor<real,length>(nu,length{1},length{3},length{2});
    const auto S2 = EshelbyTensor<real,length>(nu,length{1},length{1},length{2});
    const auto S3 = EshelbyTensor<real,length>(nu,length{1},length{1},length{1});}
    
    //must return a warning
    //const auto S = EshelbyTensorSphere(real{1});
    //const auto S = AxisymmetricalEshelbyTensor(nu,real{0});
    //const auto S = AxisymmetricalEshelbyTensor(nu,real{-10});
    //const auto S = EshelbyTensorGeneral(nu,length{-1},length{3},length{2});
    //const auto S = EshelbyTensor<real,length>(nu,length{-1},length{-1},length{1});
    
    //These functions must return the same thing
    if (true) {const auto S1 = EshelbyTensorSphere(nu);
    const auto S2 = AxisymmetricalEshelbyTensor(nu,real{1});
    const auto S3 = EshelbyTensor<real,length>(nu,length{2},length{2},length{2});
    const auto S4 = EshelbyTensorGeneral(nu,length{3},length{3},length{3});
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
    
    if (true) {const auto S1 = AxisymmetricalEshelbyTensor(nu,real{10});
    const auto S2 = EshelbyTensorGeneral(nu,length{30},length{3},length{3});
    const auto S3 = EshelbyTensor<real,length>(nu,length{3},length{3},length{30});
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
    const auto a =length{1};
    const auto b =length{3};
    const auto c =length{0.2};
    using namespace tfel::material;
    
    if (true) {const auto A1 = SphereLocalisationTensor<real,stress>(young,nu,C_i);
    const auto A2 = AxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,a/b);
    const auto A3 = GeneralEllipsoidLocalisationTensor<real,stress,length>(young,nu,C_i,n_a,a,n_b,b,c);}
    
    //must return a warning
    //const auto A = SphereLocalisationTensor<real,stress>(young,real{1},C_i);
    //const auto A = AxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,real{0});
    //const auto A = GeneralEllipsoidLocalisationTensor<real,stress,length>(young,nu,C_i,n_a,a,n_b,length{0},c);
    
    //These functions must return the same thing
    if (true) {const auto S1 = SphereLocalisationTensor<real,stress>(young,nu,C_i);
    const auto S2 = AxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,length{2}/length{2});
    const auto S3 = GeneralEllipsoidLocalisationTensor<real,stress,length>(young,nu,C_i,n_a,length{2},n_b,length{2},length{2});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (S1(i,j)==S2(i,j));
    		value = value and (S1(i,j)==S3(i,j));
    	};
    };
    
    std::cout << value << '\n';}
    
    if (true) {const auto S1 = AxisymmetricalEllipsoidLocalisationTensor<real,stress>(young,nu,C_i,n_a,length{20}/length{2});
    const auto S2 = GeneralEllipsoidLocalisationTensor<real,stress,length>(young,nu,C_i,n_a,length{20},n_b,length{2},length{2});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (S1(i,j)==S2(i,j));
    		//std::cout << S1(i,j) <<" " << i<< " " << j<< " " << S2(i,j) << value << '\n';
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
