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
    using namespace tfel::material;
    //constexpr auto eps = 10 * std::numeric_limits<stress>::epsilon();
    const auto young = stress{1e9};
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.3};
    const auto a = length{0.4};
    const auto b = length{0.3};
    const auto c = length{0.2};
    const auto f = real{0.5};
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::math::st2tost2<3u,stress> C_i;
    tfel::material::computeIsotropicStiffnessTensorII<3u,value,stress,real>(C_i,young_i,nu_i);
    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
    const auto A = computeSphereLocalisationTensor<real,stress>(young,nu,C_i);
    
    //tests the scheme functions
    using namespace tfel::material;
    if (true) {const auto Chom1 = diluteScheme<real,stress>(young,nu,f,C_i,A);
    const auto Chom2 = moriTanakaScheme<real,stress>(young,nu,f,C_i,A);
    const auto Chom3 = sphereDiluteScheme<real,stress>(young,nu,f,C_i);
    const auto Chom4 = sphereMoriTanakaScheme<real,stress>(young,nu,f,C_i);
    const auto Chom5 = isotropicDiluteScheme<real,stress>(young,nu,f,C_i,a,b,c);
    const auto Chom6 = transverseIsotropicDiluteScheme<real,stress>(young,nu,f,C_i,n_a,a,b,c);
    const auto Chom7 = orientedDiluteScheme<real,stress>(young,nu,f,C_i,n_a,a,n_b,b,c);
    const auto Chom8 = isotropicMoriTanakaScheme<real,stress>(young,nu,f,C_i,a,b,c);
    const auto Chom9 = transverseIsotropicMoriTanakaScheme<real,stress>(young,nu,f,C_i,n_a,a,b,c);
    const auto Chom10 = orientedMoriTanakaScheme<real,stress>(young,nu,f,C_i,n_a,a,n_b,b,c);
    }
    
    //must return a warning
    if (true) {//const auto Chom1 = diluteScheme<real,stress>(stress{-1},nu,f,C_i,A);
    //const auto Chom2 = moriTanakaScheme<real,stress>(young,real{2},f,C_i,A);
    //const auto Chom3 = sphereDiluteScheme<real,stress>(young,nu,real{2},C_i);
    //const auto Chom4 = isotropicDiluteScheme<real,stress>(young,nu,f,C_i,length{-2},b,c);
    const tfel::math::tvector<3u,real> n_0= {0.,0.,0.};
    //const auto Chom5 = transverseIsotropicDiluteScheme<real,stress>(young,nu,f,C_i,n_0,a,b,c);
    const tfel::math::tvector<3u,real> n_ = {0.,std::sqrt(2)/2,std::sqrt(2)/2};
    //const auto Chom6 = orientedDiluteScheme<real,stress>(young,nu,f,C_i,n_a,a,n_,b,c);
    //const auto Chom7 = isotropicMoriTanakaScheme<real,stress>(young,nu,f,C_i,a,length{-1},c);
    //const auto Chom8 = transverseIsotropicMoriTanakaScheme<real,stress>(young,nu,f,C_i,n_0,a,b,c);
    //const auto Chom9 = orientedMoriTanakaScheme<real,stress>(young,nu,f,C_i,n_a,a,n_,b,c);
    }
    

    //These functions must return the same thing
    if (true){
    //const auto nuhom1 = Chom(0,1)/(Chom(0,1)+Chom(0,0));
    //const auto Ehom1 = (1+nuhom)*(Chom(0,0)-Chom(0,1));
    //const auto nuhom2 = Chom2(0,1)/(Chom2(0,1)+Chom2(0,0));
    //const auto Ehom2 = (1+nuhom2)*(Chom2(0,0)-Chom2(0,1));
    //bool value = true;
    //value = value and (Ehom1==Ehom2)
    //std::cout <<value << '\n';
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
