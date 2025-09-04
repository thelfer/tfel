/*!
 * \file   tests/Material/MicrostructureLinearHomogenization.cxx
 * \brief
 * \author Antoine Martin
 * \date   23 January 2025
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
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Material/MicrostructureDescription.hxx"
#include "TFEL/Material/MicrostructureLinearHomogenization.ixx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct MicrostructureLinearHomogenizationTest final : public tfel::tests::TestCase {
  MicrostructureLinearHomogenizationTest()
      : tfel::tests::TestCase("TFEL/Material", "MicrostructureLinearHomogenization") {
  }  // end of MicrostructureLinearHomogenizationTest

  tfel::tests::TestResult execute() override {
    using real = double;
    
    using stress = typename tfel::config::Types<1u, real, true>::stress;
    using length = typename tfel::config::Types<1u, real, true>::length;

    this->template test_particulate<real, stress, length>();
    this->template test_particulate<real, real, real>();

    return this->result;
  }

 private:
  template <typename real, typename stress, typename length>
  void test_particulate() {
  static constexpr auto eps = std::numeric_limits<real>::epsilon();
    using namespace tfel::material::homogenization::elasticity;
    length a = length(10);
    length b = length(1);
    length c = length(1);
    tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    tfel::math::tvector<3u, real> n_b = {0., 1., 0.};
    
    const auto young0 = stress{1e9};
    const auto nu0 = real(0.2);
    const auto youngi = stress{10e9};
    const auto nui = real(0.3);
    
    tfel::math::st2tost2<3u, stress> C_0;
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::material::computeIsotropicStiffnessTensorII<3u, value, stress, real>(
        C_0, young0, nu0);
    tfel::math::st2tost2<3u, stress> C_i;
    tfel::material::computeIsotropicStiffnessTensorII<3u, value, stress, real>(
        C_i, youngi, nui);
        
    Ellipsoid<length> ellipsoid1(a,b,c);
    Spheroid<length> spheroid1(a,b);
    IsotropicDistribution<stress> distrib1(ellipsoid1,real(0.2),C_i);
    IsotropicDistribution<stress> distrib2(spheroid1,real(0.2),C_i);
    
    unsigned short int index = 0;
    TransverseIsotropicDistribution<stress> distrib3(spheroid1,real(0.2),C_i,n_b,index);                 
    OrientedDistribution<stress> distrib4(ellipsoid1,real(0.2),C_i,n_b,n_a);
    
    unsigned short int index2 = 1;
    TransverseIsotropicDistribution<stress> distrib5(ellipsoid1,real(0.2),C_i,n_a,index2);                 
    TransverseIsotropicDistribution<stress> distrib6(spheroid1,real(0.2),C_i,n_a,index2);
    
    ParticulateMicrostructure<3u, stress> micro1(C_0);
    micro1.addInclusionPhase(distrib1);
    micro1.addInclusionPhase(distrib3);
    
    ParticulateMicrostructure<3u, stress> micro2(C_0);
    micro2.addInclusionPhase(distrib2);
    micro2.addInclusionPhase(distrib4);
   
    tfel::math::stensor<3u,stress> P=tfel::math::stensor<3u,stress>::zero();
    std::vector<tfel::math::stensor<3u,stress>> polarizations = {P,P,P};
    
    auto h_s_1=computeDilute<3u, stress>(micro1,polarizations);
    auto Chom_DS_1=h_s_1.homogenized_stiffness;
    auto h_s_2=computeDilute<3u,stress>(micro2,polarizations);
    auto Chom_DS_2=h_s_2.homogenized_stiffness;
    
    for (int i=0;i<6;i++)
    for (int j=0;j<6;j++){
      TFEL_TESTS_ASSERT(my_abs(Chom_DS_1(i,j)- Chom_DS_2(i,j)) < stress{eps});
      //std::cout<<Chom_DS_1(i,j)-Chom_DS_2(i,j)<<" "<<eps<<std::endl;
    }
    
    micro1.removeInclusionPhase(0);
    micro1.addInclusionPhase(distrib5);
    micro2.removeInclusionPhase(0);
    micro2.addInclusionPhase(distrib6);
    
    HomogenizationScheme<3u, stress> h_s_MT1=computeMoriTanaka<3u, stress>(micro1,polarizations);
    auto Chom_MT_1=h_s_MT1.homogenized_stiffness;
    HomogenizationScheme<3u, stress> h_s_MT2=computeMoriTanaka<3u, stress>(micro2,polarizations);
    auto Chom_MT_2=h_s_MT2.homogenized_stiffness;
    for (int i=0;i<6;i++)
    for (int j=0;j<6;j++){
      TFEL_TESTS_ASSERT(my_abs(Chom_MT_1(i,j)- Chom_MT_2(i,j)) < stress{eps});
      //std::cout<<Chom_MT_1(i,j)-Chom_MT_2(i,j)<<" "<<std::endl;
    }

    micro1.removeInclusionPhase(0);
    micro1.removeInclusionPhase(0);
    micro1.addInclusionPhase(distrib1);
    micro2.removeInclusionPhase(0);
    micro2.removeInclusionPhase(0);
    micro2.addInclusionPhase(distrib2);
 
    auto h_s_SC1=computeSelfConsistent<3u, stress>(micro1,polarizations,12,false,12);
    auto Chom_SC_1=h_s_SC1.homogenized_stiffness;
    auto h_s_SC2=computeSelfConsistent<3u, stress>(micro2,polarizations,12,false,12);
    auto Chom_SC_2=h_s_SC2.homogenized_stiffness;
    for (int i=0;i<6;i++)
    for (int j=0;j<6;j++){
      TFEL_TESTS_ASSERT(my_abs(Chom_SC_1(i,j)- Chom_SC_2(i,j)) < stress{eps});
      //std::cout<<Chom_SC_1(i,j)-Chom_SC_2(i,j)<<" "<<std::endl;
    }
  };

};  // end of struct MicrostructureLinearHomogenizationTest

TFEL_TESTS_GENERATE_PROXY(MicrostructureLinearHomogenizationTest,
                          "MicrostructureLinearHomogenization");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MicrostructureLinearHomogenization.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
