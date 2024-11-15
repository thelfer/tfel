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
#include <typeinfo>
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
    using namespace tfel::material;
    constexpr auto eps = 10 * std::numeric_limits<real>::epsilon();
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.3};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    const auto A = computeSphereLocalisationTensor<real,stress>(young,nu,young_i,nu_i);
    
    //tests the scheme functions
    using namespace tfel::material;
    {const auto Chom0 = computeDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,A);
    const auto muhom0=(Chom0(0,0)-Chom0(0,1))/2;
    const auto khom0=(Chom0(0,0)+2*Chom0(0,1))/3;
    const auto nu0 = (3*khom0-2*muhom0)/(2*muhom0+6*khom0);
    const auto E0 = 2*muhom0*(1+nu0);
    const auto Chom1 =computeMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,A);
    const auto muhom1=(Chom1(0,0)-Chom1(0,1))/2;
    const auto khom1=(Chom1(0,0)+2*Chom1(0,1))/3;
    const auto nu1 = (3*khom1-2*muhom1)/(2*muhom1+6*khom1);
    const auto E1 = 2*muhom1*(1+nu1);
    const auto pair2 = computeSphereDiluteScheme<real,stress>(young,nu,f,young_i,nu_i);
    const auto E2=std::get<0>(pair2);
    const auto nu2=std::get<1>(pair2);
    const auto pair3 = computeSphereMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i);
    const auto E3=std::get<0>(pair3);
    const auto nu3=std::get<1>(pair3);
    
    bool value = true;
    std::cout << (E0-E2).getValue() << " " << eps << '\n';
    value = value and (my_abs(E0-E2)<stress{eps});
    value = value and (my_abs(nu0-nu2)<eps);
    value = value and (my_abs(E1-E3)<stress{eps});
    value = value and (my_abs(nu1-nu3)<eps);
    if (value){
    std::cout << "Mori-Tanaka and Dilute Scheme for spheres 3d ok" << '\n';
    }
    else{std::cout << "Mori-Tanaka and Dilute Scheme for spheres 3d does not work" << '\n';};
    
    
    computeIsotropicDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,a,b,c);
    computeTransverseIsotropicDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,n_a,a,b,c);
    computeOrientedDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,n_a,a,n_b,b,c);
    computeIsotropicMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,a,b,c);
    computeTransverseIsotropicMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,n_a,a,b,c);
    computeOrientedMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,n_a,a,n_b,b,c);
    }
    
    //must return a warning
    {//computeDiluteScheme<real,stress>(stress{-1},nu,f,young_i,nu_i,A);
    //computeMoriTanakaScheme<real,stress>(young,real{2},f,young_i,nu_i,A);
    //computeSphereDiluteScheme<real,stress>(young,nu,real{2},young_i,nu_i);
    //computeIsotropicDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,length{-2},b,c);
    const tfel::math::tvector<3u,real> n_0= {0.,0.,0.};
    //computeTransverseIsotropicDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,n_0,a,b,c);
    const tfel::math::tvector<3u,real> n_ = {0.,std::sqrt(2)/2,std::sqrt(2)/2};
    //computeOrientedDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,n_a,a,n_,b,c);
    //computeIsotropicMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,a,length{-1},c);
    //computeTransverseIsotropicMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,n_0,a,b,c);
    //computeOrientedMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,n_a,a,n_,b,c);
    }
  

    //These functions must return a very similar thing
    
    
    
    
    
    
    
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
