/*!
 * \file   tests/Material/DerivativesOfLocalisationTensors.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   07/06/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
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

#include "TFEL/Material/StiffnessTensor.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"
#include "TFEL/Material/DerivativesOfLocalisationTensors.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct DerivativesOfLocalisationTensorsTest final : public tfel::tests::TestCase {
  DerivativesOfLocalisationTensorsTest()
      : tfel::tests::TestCase("TFEL/Material", "DerivativesOfLocalisationTensors") {
  }  // end of DerivativesOfLocalisationTensorsTest

  tfel::tests::TestResult execute() override {
    this->template test_Hill_derivative<double, false>();
    this->template test_Hill_derivative<double, true>();

    //this->template test_loc_derivative<double, false>();
    //this->template test_loc_derivative<double, true>();
   
    return this->result;
  }

  
  // Test derivatives of Hill tensor by comparing on spherical case
  template <typename NumericType, bool use_qt>
  void test_Hill_derivative() {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = typename tfel::config::Types<1u, NumericType, use_qt>::real;
    
    using squared_compliance =
        typename tfel::config::Types<1u, NumericType, use_qt>::squared_compliance;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
   
    const tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    using namespace tfel::material;
    const auto k0=stress(2);
    const auto mu0 =stress(1);
    const auto ki=stress(200);
    const auto mui =stress(100);
    const auto KG0=KGModuli<stress>(k0,mu0);
    const auto KGi=KGModuli<stress>(ki,mui);
    const std::array<real,2> dkg0 = {1.,0.};
    using namespace tfel::material::homogenization::elasticity;
    
    const auto kP=k0+4*mu0/3;
    const auto dmuP=5*mu0/2*((k0+2*mu0)-kP)/(k0+2*mu0)/(k0+2*mu0);
    const auto muP=5*mu0/2*kP/(k0+2*mu0);
    const auto dPSphere_dk0_an =-1./3/kP/kP*tfel::math::st2tost2<3u,real>::J()-dmuP/2/muP/muP*tfel::math::st2tost2<3u,real>::K();
    const auto dPSphere_dk0 =
          computeDerivativesOfAxisymmetricalHillTensor<stress>(
              KG0, n_a, real(1),dkg0);
    const auto dPSphere_dk0_bis =
          computeDerivativesOfAxisymmetricalHillTensor<stress>(
              KG0, n_a, real(1.0001),dkg0);
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(dPSphere_dk0(i, j) - dPSphere_dk0_an(i, j)) < squared_compliance(eps));
          TFEL_TESTS_ASSERT(my_abs(dPSphere_dk0(i, j) - dPSphere_dk0_bis(i, j)) < squared_compliance(eps));
        }
      }
    const auto dmuP_dmu0=5./2*kP/(k0+2*mu0)+5*mu0/2*(4./3*(k0+2*mu0)-2*kP)/(k0+2*mu0)/(k0+2*mu0);
    const auto dPSphere_dmu0_an =-4./9/kP/kP*tfel::math::st2tost2<3u,real>::J()-dmuP_dmu0/2/muP/muP*tfel::math::st2tost2<3u,real>::K();
    const auto dPSphere_dmu0 =
          computeDerivativesOfAxisymmetricalHillTensor<stress>(
              KG0, n_a, real(1),{0.,1.});
    const auto dPSphere_dmu0_bis =
          computeDerivativesOfAxisymmetricalHillTensor<stress>(
              KG0, n_a, real(1.0001),{0.,1.});
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(dPSphere_dmu0(i, j) - dPSphere_dmu0_an(i, j)) < squared_compliance(eps));
          TFEL_TESTS_ASSERT(my_abs(dPSphere_dmu0(i, j) - dPSphere_dmu0_bis(i, j)) < squared_compliance(eps));
        }
      }
    }//end of test_Hill_derivative

    // Test derivatives of localisation tensor by comparing on spherical case
  template <typename NumericType, bool use_qt>
  void test_loc_derivative() {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = typename tfel::config::Types<1u, NumericType, use_qt>::real;
    
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
   
    const tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    using namespace tfel::material;
    const auto k0=stress(2);
    const auto mu0 =stress(1);
    const auto ki=stress(200);
    const auto mui =stress(100);
    const auto KG0=KGModuli<stress>(k0,mu0);
    const auto KGi=KGModuli<stress>(ki,mui);
    const std::array<real,2> dk0 = {1.,0.,0.,0.};
    const std::array<real,2> dmu0 = {0.,1.,0.,0.};
    const std::array<real,2> dki = {0.,0.,1.,0.};
    const std::array<real,2> dmui = {0.,0.,0.,1.};
    using namespace tfel::material::homogenization::elasticity;
    
    const auto dASphere_dk0 =
          computeDerivativesOfAxisymmetricalLocalisationTensor<stress>(
              KG0,KGi, n_a, real(1),dk0);
    
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          //TFEL_TESTS_ASSERT(my_abs(dASphere_dk0(i, j) - dASphere_dk0_an(i, j)) < squared_compliance(eps));
        }
      }
    
    const auto dASphere_dmu0 =
          computeDerivativesOfAxisymmetricalLocalisationTensor<stress>(
              KG0,KGi, n_a, real(1),dmu0);
   
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          //TFEL_TESTS_ASSERT(my_abs(dASphere_dmu0(i, j) - dASphere_dmu0_an(i, j)) < squared_compliance(eps));
        }
      }
    }//end of test_loc_derivative

};  // end of struct DerivativesOfLocalisationTensorsTest

TFEL_TESTS_GENERATE_PROXY(DerivativesOfLocalisationTensorsTest, "DerivativesOfLocalisationTensors");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DerivativesOfLocalisationTensors.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
