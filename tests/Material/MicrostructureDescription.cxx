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
    using namespace tfel::material::homogenization;
    length a = length(1);
    length b = length(2);
    length c = length(3);
    tfel::math::tvector<3u, real> n_a = {1, 0, 0};
    tfel::math::tvector<3u, real> n_b = {0, 1, 0};
    Ellipsoid<3u, real, length> ellipsoid1({a, b, c}, {n_a, n_b});

    const auto young = stress{1e9};
    const auto nu = real(0.2);
    tfel::math::st2tost2<3u, stress> C_0;
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::material::computeIsotropicStiffnessTensorII<3u, value, stress, real>(
        C_0, young, nu);

    InclusionPhase<3u, real, length, stress> inclusionPhase1(C_0, ellipsoid1,
                                                             real(0.1));
    
    InclusionPhase<3u, real, length, stress> inclusionPhase2(C_0, ellipsoid1,
                                                             real(0.2));
    
    MatrixPhase<3u, real, stress> matrix1(C_0);
    MatrixInclusionMicrostructure<3u, real, length, stress> micro1(matrix1);
    std::cout << micro1.get_matrix_fraction() << std::endl;
    std::cout << micro1.get_number_of_phases() << std::endl;
    micro1.addInclusionPhase(inclusionPhase1);
    std::cout << micro1.get_matrix_fraction() << std::endl;
    std::cout << micro1.get_number_of_phases() << std::endl;
     micro1.addInclusionPhase(inclusionPhase2);
    std::cout << micro1.get_matrix_fraction() << std::endl;
    std::cout << micro1.get_number_of_phases() << std::endl;
    auto phase1=micro1.get_inclusionPhase(0);
    std::cout<<phase1.fraction<<std::endl;
    micro1.removeInclusionPhase(0);
    
    auto phase2=micro1.get_inclusionPhase(0);
    std::cout<<phase2.fraction<<std::endl;
    
    
    
    //     TFEL_TESTS_ASSERT(my_abs(K_L - K_Lbis) < seps);
    //     TFEL_TESTS_ASSERT(my_abs(mu_L - mu_Lbis) < seps);
    //     TFEL_TESTS_ASSERT(my_abs(K_U - K_Ubis) < seps);
    //     TFEL_TESTS_ASSERT(my_abs(mu_U - mu_Ubis) < seps);
  };

 private:
  template <typename real, typename stress, typename length>
  void test_polycrystal() {
    using namespace tfel::material::homogenization;
    length a = length(1);
    length b = length(2);
    length c = length(3);
    tfel::math::tvector<3u, real> n_a = {1, 0, 0};
    tfel::math::tvector<3u, real> n_b = {0, 1, 0};
    Ellipsoid<3u, real, length> ellipsoid1({a, b, c}, {n_a, n_b});

    const auto young = stress{1e9};
    const auto nu = real(0.2);
    tfel::math::st2tost2<3u, stress> C_0;
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::material::computeIsotropicStiffnessTensorII<3u, value, stress, real>(
        C_0, young, nu);

    Grain<3u, real, length, stress> grain1(C_0, ellipsoid1, real(0.3));

    Grain<3u, real, length, stress> grain2(C_0, ellipsoid1, real(0.2));

    Polycrystal<3u, real, length, stress> crystal;
    crystal.addGrain(grain1);
    crystal.addGrain(grain2);
    std::cout << crystal.get_total_fraction() << std::endl;
    std::cout << crystal.get_number_of_grains() << std::endl;
    auto grain3=crystal.get_grain(0);
    std::cout<<grain3.fraction<<std::endl;
    crystal.removeGrain(1);
    
    auto phase2=crystal.get_grain(0);
    std::cout<<phase2.fraction<<std::endl;
    //     TFEL_TESTS_ASSERT(my_abs(K_L - K_Lbis) < seps);
    //     TFEL_TESTS_ASSERT(my_abs(mu_L - mu_Lbis) < seps);
    //     TFEL_TESTS_ASSERT(my_abs(K_U - K_Ubis) < seps);
    //     TFEL_TESTS_ASSERT(my_abs(mu_U - mu_Ubis) < seps);
    //   }
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
