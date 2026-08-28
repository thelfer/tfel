/*!
 * \file   tests/Material/MicrostructureDerivatives.cxx
 * \brief
 * \author Antoine Martin
 * \date   23 June 2026
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
#include "TFEL/Material/MicrostructureLinearHomogenization.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct MicrostructureDerivativesTest final
    : public tfel::tests::TestCase {
  MicrostructureDerivativesTest()
      : tfel::tests::TestCase("TFEL/Material",
                              "MicrostructureDerivatives") {
  }  // end of MicrostructureDerivativesTest

  tfel::tests::TestResult execute() override {
    using real = double;

    using stress = typename tfel::config::Types<1u, real, true>::stress;
    using length = typename tfel::config::Types<1u, real, true>::length;

    this->template test_1<real, stress, length>();
    this->template test_1<real, real, real>();
    
    return this->result;
  }

 private:
  template <typename real, typename stress, typename length>
  void test_1() {
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    using namespace tfel::material::homogenization::elasticity;
    length a = length(100);
    length b = length(1);
    length c = length(1);
    tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    tfel::math::tvector<3u, real> n_b = {0., 1., 0.};

    const auto KG0 = tfel::material::KGModuli<stress>(stress(1),stress(0.5));
    const auto KGi = tfel::material::KGModuli<stress>(stress(100),stress(40));

    Spheroid<length> spheroid1(a, b);
    IsotropicDistribution<stress> distrib1(spheroid1, real(0.02), KGi);

    unsigned short int index = 0;
    TransverseIsotropicDistribution<stress> distrib2(spheroid1, real(0.02), KGi,
                                                     n_b, index);

    ParticulateMicrostructure<3u, stress> micro1(KG0);
    micro1.addInclusionPhase(distrib1);
    micro1.addInclusionPhase(distrib2);

    auto h_DS = computeDilute<3u, stress>(micro1,0,{},true);

    auto dCDS_dkr = h_DS.derivative_of_homogenized_stiffness_wrt_kr;
    std::cout<<dCDS_dkr[0](0,0)<<std::endl;
    std::cout<<dCDS_dkr[1](0,0)<<std::endl;
    std::cout<<dCDS_dkr[2](0,0)<<std::endl;

    auto dCDS_dmur = h_DS.derivative_of_homogenized_stiffness_wrt_mur;
    std::cout<<dCDS_dmur[0](0,0)<<std::endl;
    std::cout<<dCDS_dmur[1](0,0)<<std::endl;
    std::cout<<dCDS_dmur[2](0,0)<<std::endl;
    
    
    auto h_MT = computeMoriTanaka<3u, stress>(micro1,0,{},true);
    auto dCMT_dkr = h_MT.derivative_of_homogenized_stiffness_wrt_kr;
    std::cout<<dCMT_dkr[0](0,0)<<std::endl;
    std::cout<<dCMT_dkr[1](0,0)<<std::endl;
    std::cout<<dCMT_dkr[2](0,0)<<std::endl;

    auto dCMT_dmur = h_MT.derivative_of_homogenized_stiffness_wrt_mur;
    std::cout<<dCMT_dmur[0](0,0)<<std::endl;
    std::cout<<dCMT_dmur[1](0,0)<<std::endl;
    std::cout<<dCMT_dmur[2](0,0)<<std::endl;

    auto h_SC = computeSelfConsistent<3u, stress>(micro1,1e-5,true,0,{},true);
    auto dCSC_dkr = h_SC.derivative_of_homogenized_stiffness_wrt_kr;
    //std::cout<<dCSC_dkr[0](0,0)<<std::endl;
    auto dCSC_dmur = h_SC.derivative_of_homogenized_stiffness_wrt_mur;
    //std::cout<<dCSC_dmur[0](0,0)<<std::endl;
    
  }  

};  // end of struct MicrostructureDerivativesTest

TFEL_TESTS_GENERATE_PROXY(MicrostructureDerivativesTest,
                          "MicrostructureDerivatives");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MicrostructureDerivatives.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
