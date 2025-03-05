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
    constexpr bool qt = true;
    using stress = typename tfel::config::Types<1u, real, qt>::stress;
    using length = typename tfel::config::Types<1u, real, qt>::length;

    this->template test_matrix_inclusion<real, stress, length>();
    this->template test_polycrystal<real, stress, length>();
    // this->template testHS_2D<real, stress>();

    return this->result;
  }

 private:
  template <typename real, typename stress, typename length>
  void test_matrix_inclusion() {
  static constexpr auto eps = std::numeric_limits<real>::epsilon();
    using namespace tfel::material::homogenization::elasticity;
    length a = length(10);
    length b = length(1);
    length c = length(1);
    tfel::math::tvector<3u, real> n_a = {1, 0, 0};
    tfel::math::tvector<3u, real> n_b = {0, 1, 0};
    
    const auto young0 = stress{1e9};
    const auto seps=young0*eps;
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
    Spheroid<length> spheroid1(a,c);
    IsotropicDistribution<3u,real,stress,length> distrib1(ellipsoid1,real(0.5),C_0);
    unsigned short int index = 0;
    TransverseIsotropicDistribution<real,stress,length> distrib2(spheroid1,real(0.02),C_i,n_a,index);                 
    //IsotropicDistribution<3u,real,stress,length> distrib2(ellipsoid1,real(0.5),C_i);
    //OrientedDistribution<3u,real,stress,length> distrib2(spheroid1,real(0.02),C_i,n_a,n_b); 
    
    MatrixInclusionMicrostructure<3u, real, length, stress> micro1(C_0);
    micro1.addInclusionPhase(distrib1);
    micro1.addInclusionPhase(distrib2);
    
    tfel::math::stensor<3u,real> E={1,0,0,0,0,0};
    auto zs=stress(0);
    tfel::math::stensor<3u,stress> P={zs,zs,zs,zs,zs,zs};
    std::vector<tfel::math::stensor<3u,stress>> polarizations = {P,P,P};
    auto h_s_1=computeDilute<3u, real, length, stress>(micro1,polarizations,false,E,true);
    auto Chom_DS_iso=h_s_1.homogenized_stiffness;
    //auto h_s_2=computeDilute<3u, real, length, stress>(micro1,polarizations,false,E,false);
    //auto Chom_DS_non_iso=h_s_2.homogenized_stiffness;
    //for (int i=0;i<6;i++)
    //for (int j=0;j<6;j++){
    //  TFEL_TESTS_ASSERT(my_abs(Chom_DS_iso(i,j)- Chom_DS_non_iso(i,j)) < stress{seps});
    //}
    
    HomogenizationScheme<3u, real, length, stress> h_s_MT1=computeMoriTanaka<3u, real, length, stress>(micro1,polarizations,false,E,true);
    auto Chom_MT_iso=h_s_MT1.homogenized_stiffness;
    //HomogenizationScheme<3u, real, length, stress> h_s_MT2=computeMoriTanaka<3u, real, length, stress>(micro1,polarizations,false,E,true);
    //auto Chom_MT_non_iso=h_s_MT2.homogenized_stiffness;
    //for (int i=0;i<6;i++)
    //for (int j=0;j<6;j++){
    //  TFEL_TESTS_ASSERT(my_abs(Chom_MT_iso(i,j)- Chom_MT_non_iso(i,j)) < stress{seps});
    //}
    std::cout<< Chom_MT_iso(0,0).getValue() <<" " <<Chom_DS_iso(0,0).getValue()<<std::endl;
  };

 private:
  template <typename real, typename stress, typename length>
  void test_polycrystal() {
  static constexpr auto eps = std::numeric_limits<real>::epsilon();
    using namespace tfel::material::homogenization::elasticity;
    length a = length(2);
    length b = length(1);
    length c = length(3);
    tfel::math::tvector<3u, real> n_a = {1, 0, 0};
    tfel::math::tvector<3u, real> n_b = {0, 1, 0};
    tfel::math::tvector<3u, real> n_c = {0, 0, 1};
    
    tfel::math::tvector<3u, real> n_a_ = {std::sqrt(2)/2, std::sqrt(2)/2, 0};
    tfel::math::tvector<3u, real> n_b_ = {-std::sqrt(2)/2, std::sqrt(2)/2, 0};
    Ellipsoid<length> ellipsoid1(a,b,c);

    const auto young0 = stress{1e9};
    const auto seps=young0*eps;
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
        
    GrainPhase<3u,real,stress,length> grain1(real(0.33),C_0,ellipsoid1, n_a, n_b);
    GrainPhase<3u, real, stress,length> grain2(real(0.33),C_i, ellipsoid1, n_a_, n_b_);
    GrainPhase<3u, real, stress,length> grain3(real(0.34),C_i, ellipsoid1, n_a, n_c);

    Polycrystal<3u, real, length, stress> crystal;
    crystal.addGrain(grain1);
    crystal.addGrain(grain2);
    crystal.addGrain(grain3);
    
    tfel::math::stensor<3u,real> E={1,0,0,0,0,0};
    auto zs=stress(0);
    tfel::math::stensor<3u,stress> P={zs,zs,zs,zs,zs,zs};
    std::vector<tfel::math::stensor<3u,stress>> polarizations = {P,P};
    //auto h_s_SC1=computeSelfConsistent<3u, real, length, stress>(crystal,polarizations,false,E,true,200);
    //auto Chom_SC_iso=h_s_SC1.homogenized_stiffness;
    auto h_s_SC2=computeSelfConsistent<3u, real, length, stress>(crystal,polarizations,false,E,false,20,6);
    auto Chom_SC_non_iso=h_s_SC2.homogenized_stiffness;
    //for (int i=0;i<6;i++)
    //for (int j=0;j<6;j++){
    //  TFEL_TESTS_ASSERT(my_abs(Chom_SC_iso(i,j)- Chom_SC_non_iso(i,j)) < stress{1000000*seps});
    //  std::cout<<i<<" "<<j<<" "<< (Chom_SC_iso(i,j)-Chom_SC_non_iso(i,j)).getValue()<<" "<<eps<<std::endl;
    //}
    
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
