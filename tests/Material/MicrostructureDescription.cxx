/*!
 * \file   tests/Material/MicrostructureDescription.cxx
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
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct MicrostructureDescriptionTest final : public tfel::tests::TestCase {
  MicrostructureDescriptionTest()
      : tfel::tests::TestCase("TFEL/Material", "MicrostructureDescription") {
  }  // end of MicrostructureDescriptionTest

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
    IsotropicDistribution<stress> distrib1(ellipsoid1,real(0.5),C_i);
    IsotropicDistribution<stress> distrib2(spheroid1,real(0.5),C_i);
    
    const auto A_iso1 = distrib1.computeMeanLocalisator(C_0);
    const auto A_iso2 = distrib2.computeMeanLocalisator(C_0);
    
    for (int i=0 ; i<6; i++)
    for (int j=0 ; j<6; j++){
    TFEL_TESTS_ASSERT(my_abs(A_iso1(i,j)- A_iso2(i,j)) < eps);
    }
    
    unsigned short int index = 0;
    TransverseIsotropicDistribution<stress> distrib3(spheroid1,real(0.5),C_i,n_b,index);                 
    OrientedDistribution<stress> distrib4(ellipsoid1,real(0.5),C_i,n_b,n_a);
    
    const auto A_Or_1 = distrib3.computeMeanLocalisator(C_0, true);
    const auto A_Or_2 = distrib4.computeMeanLocalisator(C_0, true);
    
    for (int i=0 ; i<6; i++)
    for (int j=0 ; j<6; j++){
    TFEL_TESTS_ASSERT(my_abs(A_Or_1(i,j)- A_Or_2(i,j)) < eps);
    }
    
    unsigned short int index2 = 1;
    TransverseIsotropicDistribution<stress> distrib5(ellipsoid1,real(0.5),C_i,n_a,index2);                 
    TransverseIsotropicDistribution<stress> distrib6(spheroid1,real(0.5),C_i,n_a,index2);
    
    const auto A_TI_1 = distrib5.computeMeanLocalisator(C_0, true);
    const auto A_TI_2 = distrib6.computeMeanLocalisator(C_0, true);
    
    for (int i=0 ; i<6; i++)
    for (int j=0 ; j<6; j++){
    TFEL_TESTS_ASSERT(my_abs(A_TI_1(i,j)- A_TI_2(i,j)) < eps);
    }
    
    
    ParticulateMicrostructure<3u, stress> micro1(C_0);
    micro1.addInclusionPhase(distrib1);
    micro1.addInclusionPhase(distrib2);
    micro1.addInclusionPhase(distrib2);
   
     
  };

};  // end of struct MicrostructureDescriptionTest

TFEL_TESTS_GENERATE_PROXY(MicrostructureDescriptionTest,
                          "MicrostructureDescription");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MicrostructureDescription.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
