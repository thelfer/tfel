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
  using namespace tfel::material;
  using real=double;
  const bool qt= true;
  using stress =
         typename tfel::config::Types<1u, real, qt>::stress;
    using length =
        typename tfel::config::Types<1u, real, qt>::length;
    using namespace tfel::material;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    const auto A = computeSphereLocalisationTensor<real,stress>(young,nu,young_i,nu_i);
    
    this->template compile<real, stress, length>();
    this->template errors<real, stress, length>();
    this->template test3<real, stress, length>();
    this->template test4<real, stress, length>();
    this->template test5<real, stress, length>();
    this->template test6<real, stress, length>();
    this->template test7<real, stress, length>();
    return this->result;
  }

 private:
  template <typename real, typename stress, typename length>
  void compile() {
    //tests compilation of the scheme functions
    using namespace tfel::material;
    constexpr auto eps = 100*std::sqrt( std::numeric_limits<real>::epsilon());
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto f = real{0.5};
    const auto A = computeSphereLocalisationTensor<real,stress>(young,nu,young_i,nu_i);
    const auto Chom0 = computeDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,A);
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
    std::cout << (E2-E0).getValue() << " " << eps << '\n';
    value = value and (my_abs(E0-E2)<stress{eps});
    value = value and (my_abs(nu0-nu2)<eps);
    value = value and (my_abs(E1-E3)<stress{eps});
    value = value and (my_abs(nu1-nu3)<eps);
    if (value){
    std::cout << "Mori-Tanaka and Dilute Scheme for spheres 3d ok" << '\n';
    }
    else{std::cout << "Mori-Tanaka and Dilute Scheme for spheres 3d does not work" << '\n';};
    };

 private:
  template <typename real, typename stress, typename length>
  void errors() {
  using namespace tfel::material;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto A = computeSphereLocalisationTensor<real,stress>(young,nu,young_i,nu_i);
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    //must return an error
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
    };
    
     private:
  template <typename real, typename stress, typename length>
  void test3() {
    {bool value = true;
    constexpr auto eps = 10 * std::numeric_limits<real>::epsilon();
    using namespace tfel::material;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    //SphereDiluteScheme must be equal to IsotropicDiluteScheme, TransverseIsotropicDiluteScheme and OrientedDiluteScheme when a=b=c
    const auto pair0 = computeSphereDiluteScheme<real,stress>(young,nu,f,young_i,nu_i);
    const auto E0=std::get<0>(pair0);
    const auto nu0=std::get<1>(pair0);
    const auto pair1 = computeIsotropicDiluteScheme<real,stress>(young,nu,f,young_i,nu_i,a,a,a);
    const auto E1=std::get<0>(pair1);
    const auto nu1=std::get<1>(pair1);
    const auto Chom2 = computeTransverseIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,a,a);
    const auto mu2=(Chom2(0,0)-Chom2(0,1))/2;
    const auto ka2=(Chom2(0,0)+2*Chom2(0,1))/3;
    const auto nu2 = (3*ka2-2*mu2)/(2*mu2+6*ka2);
    const auto E2 = 2*mu2*(1+nu2);
    const auto Chom3 = computeOrientedDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,n_b,a,a);
    const auto mu3=(Chom3(0,0)-Chom3(0,1))/2;
    const auto ka3=(Chom3(0,0)+2*Chom3(0,1))/3;
    const auto nu3 = (3*ka3-2*mu3)/(2*mu3+6*ka3);
    const auto E3 = 2*mu3*(1+nu3);
    
    value = value and (my_abs(E0-E1)<stress{eps});
    std::cout << (E0-E1).getValue() << " "<< eps << '\n';
    value = value and (my_abs(nu0-nu1)<eps);
    value = value and (my_abs(E0-E2)<stress{eps});
    std::cout << (E2-E0).getValue() << " "<< eps << '\n';
    value = value and (my_abs(nu0-nu2)<eps);
    value = value and (my_abs(E0-E3)<stress{eps});
    std::cout << (E0-E3).getValue() << " "<< eps << '\n';
    value = value and (my_abs(nu0-nu3)<eps);
    
    if (value){
    std::cout << "Dilute Scheme test 1 ok" << '\n';
    }
    else{std::cout << "Dilute scheme test 1 does not work" << '\n';};
    }
    };
    
    
        private:
  template <typename real, typename stress, typename length>
  void test4() {
    {bool value = true;
    constexpr auto eps = 100*std::sqrt( std::numeric_limits<real>::epsilon());
    using namespace tfel::material;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    //TransverseIsotropicDiluteScheme must be equal to OrientedDiluteScheme when b=c
    const auto Chom2 = computeTransverseIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,b,b);
    const auto mu2=(Chom2(0,0)-Chom2(0,1))/2;
    const auto ka2=(Chom2(0,0)+2*Chom2(0,1))/3;
    const auto nu2 = (3*ka2-2*mu2)/(2*mu2+6*ka2);
    const auto E2 = 2*mu2*(1+nu2);
    const auto Chom3 = computeOrientedDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,n_b,b,b);
    const auto mu3=(Chom3(0,0)-Chom3(0,1))/2;
    const auto ka3=(Chom3(0,0)+2*Chom3(0,1))/3;
    const auto nu3 = (3*ka3-2*mu3)/(2*mu3+6*ka3);
    const auto E3 = 2*mu3*(1+nu3);
    value = value and (my_abs(E2-E3)<stress{eps});
    value = value and (my_abs(nu2-nu3)<eps);
    //std::cout << (E2-E3).getValue() << " "<< eps << '\n';
   
    if (value){
    std::cout << "Dilute Scheme test 2 ok" << '\n';
    }
    else{std::cout << "Dilute scheme test 2 does not work" << '\n';};
    }
    };
    
    
     private:
  template <typename real, typename stress, typename length>
  void test5() {
    {bool value = true;
    constexpr auto eps = 10 * std::numeric_limits<real>::epsilon();
    using namespace tfel::material;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    //SphereMoriTanakaScheme must be equal to IsotropicMoriTanakaScheme, TransverseIsotropicMoriTanakaScheme and OrientedMoriTanakaScheme when a=b=c
    const auto pair0 = computeSphereMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i);
    const auto E0=std::get<0>(pair0);
    const auto nu0=std::get<1>(pair0);
    const auto pair1 = computeIsotropicMoriTanakaScheme<real,stress>(young,nu,f,young_i,nu_i,a,a,a);
    const auto E1=std::get<0>(pair1);
    const auto nu1=std::get<1>(pair1);
    const auto Chom2 = computeTransverseIsotropicMoriTanakaScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,a,a);
    const auto mu2=(Chom2(0,0)-Chom2(0,1))/2;
    const auto ka2=(Chom2(0,0)+2*Chom2(0,1))/3;
    const auto nu2 = (3*ka2-2*mu2)/(2*mu2+6*ka2);
    const auto E2 = 2*mu2*(1+nu2);
    const auto Chom3 = computeOrientedMoriTanakaScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,n_b,a,a);
    const auto mu3=(Chom3(0,0)-Chom3(0,1))/2;
    const auto ka3=(Chom3(0,0)+2*Chom3(0,1))/3;
    const auto nu3 = (3*ka3-2*mu3)/(2*mu3+6*ka3);
    const auto E3 = 2*mu3*(1+nu3);
    
    value = value and (my_abs(E0-E1)<stress{eps});
    std::cout << (E0-E1).getValue() << " "<< eps << '\n';
    value = value and (my_abs(nu0-nu1)<eps);
    value = value and (my_abs(E0-E2)<stress{eps});
    std::cout << (E2-E0).getValue() << " "<< eps << '\n';
    value = value and (my_abs(nu0-nu2)<eps);
    value = value and (my_abs(E0-E3)<stress{eps});
    std::cout << (E0-E3).getValue() << " "<< eps << '\n';
    value = value and (my_abs(nu0-nu3)<eps);
    //std::cout << value << '\n';
    
    if (value){
    std::cout << "MoriTanaka Scheme test ok" << '\n';
    }
    else{std::cout << "MoriTanaka scheme test does not work" << '\n';};
    }
    };
    
    
     private:
  template <typename real, typename stress, typename length>
  void test6() {
    {bool value = true;
    constexpr auto eps = 100*std::sqrt(std::numeric_limits<real>::epsilon());
    using namespace tfel::material;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    //IsotropicDiluteScheme when a is near b must be near IsotropicDiluteScheme when a=b 
    const auto pair0 = computeIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,a,a+length{0.0000001},c);
    const auto E0=std::get<0>(pair0);
    const auto nu0=std::get<1>(pair0);
    
    const auto pair1 = computeIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,a,c,a+length{0.0000001});
    const auto E1=std::get<0>(pair1);
    const auto nu1=std::get<1>(pair1);
    
    const auto pair2 = computeIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,c,a,a+length{0.0000001});
    const auto E2=std::get<0>(pair2);
    const auto nu2=std::get<1>(pair2);
    
    const auto pair3 = computeIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,a,c,a);
    const auto E3=std::get<0>(pair3);
    const auto nu3=std::get<1>(pair3);
    
    const auto pair4 = computeIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,c,a,a);
    const auto E4=std::get<0>(pair4);
    const auto nu4=std::get<1>(pair4);
    
    value = value and (my_abs(E1-E0)<stress{10*eps});
    //std::cout << (E0-E1).getValue() << " "<< E1.getValue() << '\n';
    //std::cout << value << '\n';
    value = value and (my_abs(nu1-nu0)<eps);
    value = value and (my_abs(E1-E2)<stress{10*eps});
    //std::cout << E0.getValue() << " "<< E2.getValue() << '\n';
    //std::cout << value << '\n';
    value = value and (my_abs(nu1-nu2)<eps);
    value = value and (my_abs(E1-E3)<stress{10*eps});
    //std::cout << E0.getValue() << " "<< E3.getValue() << '\n';
    //std::cout << value << '\n';
    value = value and (my_abs(nu1-nu3)<eps);
    value = value and (my_abs(E1-E4)<stress{10*eps});
    //std::cout << E3.getValue() << " "<< E4.getValue() << '\n';
    //std::cout << value << '\n';
    value = value and (my_abs(nu1-nu4)<eps);
    
    if (value){
    std::cout << "Isotropic Dilute Scheme test ok" << '\n';
    }
    else{std::cout << "Isotropic Dilute Scheme test does not work" << '\n';};
    }
    };
    
    
  private:
  template <typename real, typename stress, typename length>
  void test7() {
    {bool value = true;
    constexpr auto eps = 100*std::sqrt(std::numeric_limits<real>::epsilon());
    using namespace tfel::material;
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    //TransverseIsotropicDiluteScheme when b is near c must be near TransverseIsotropicDiluteScheme when b=c 
    const auto Chom0 = computeTransverseIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,b,b);
    const auto mu0=(Chom0(0,0)-Chom0(0,1))/2;
    const auto ka0=(Chom0(0,0)+2*Chom0(0,1))/3;
    const auto nu0 = (3*ka0-2*mu0)/(2*mu0+6*ka0);
    const auto E0 = 2*mu0*(1+nu0);
    
    const auto Chom1 = computeTransverseIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,b,b-length{0.0000001});
    const auto mu1=(Chom1(0,0)-Chom1(0,1))/2;
    const auto ka1=(Chom1(0,0)+2*Chom1(0,1))/3;
    const auto nu1 = (3*ka1-2*mu1)/(2*mu1+6*ka1);
    const auto E1 = 2*mu1*(1+nu1);
    
    value = value and (my_abs(E1-E0)<stress{10*eps});
    //std::cout << (E0-E1).getValue() << " "<< eps << '\n';
    value = value and (my_abs(nu1-nu0)<eps);
    
     //TransverseIsotropicDiluteScheme when a is near b must be near TransverseIsotropicDiluteScheme when a=b 
    const auto Chom2 = computeTransverseIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,a,c);
    const auto mu2=(Chom2(0,0)-Chom2(0,1))/2;
    const auto ka2=(Chom2(0,0)+2*Chom2(0,1))/3;
    const auto nu2 = (3*ka2-2*mu2)/(2*mu2+6*ka2);
    const auto E2 = 2*mu2*(1+nu2);
    
    const auto Chom3 = computeTransverseIsotropicDiluteScheme<real,stress,length>(young,nu,f,young_i,nu_i,n_a,a,a+length{0.0000001},c);
    const auto mu3=(Chom3(0,0)-Chom3(0,1))/2;
    const auto ka3=(Chom3(0,0)+2*Chom3(0,1))/3;
    const auto nu3 = (3*ka3-2*mu3)/(2*mu3+6*ka3);
    const auto E3 = 2*mu3*(1+nu3);

    value = value and (my_abs(E2-E3)<stress{10*eps});
    //std::cout << (E2-E3).getValue() << " "<< value << '\n';
    value = value and (my_abs(nu2-nu3)<eps);
 
    
    if (value){
    std::cout << "Transverse Isotropic Dilute Scheme test ok" << '\n';
    }
    else{std::cout << "Transverse Isotropic Dilute Scheme test does not work" << '\n';};
    }
    };
    
    
  
};  // end of struct EshelbyBasedHomogenizationTest

TFEL_TESTS_GENERATE_PROXY(EshelbyBasedHomogenizationTest, "EshelbyBasedHomogenization");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("EshelbyBasedHomogenization.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
