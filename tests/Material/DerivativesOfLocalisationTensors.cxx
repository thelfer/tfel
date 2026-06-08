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
    
    this->template test_Hill_derivative<double, true>();
   
    return this->result;
  }

  
  // These functions must return the same thing
  template <typename NumericType, bool use_qt>
  void test_Hill_derivative() {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = typename tfel::config::Types<1u, NumericType, use_qt>::real;
    
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
   
    const tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    using namespace tfel::material;
    const auto KG0=KGModuli<stress>(stress(2),stress(1));
    const auto KGi=KGModuli<stress>(stress(200),stress(100));
    const std::array<real,2> dkg0 = {1.,0.};
    using namespace tfel::material::homogenization::elasticity;
    
    const auto PSphere_1 =
          computeSphereHillPolarisationTensor<stress>(KG0);
    const auto PSphere_2 =
          computeDerivativesOfAxisymmetricalHillTensor<stress>(
              KG0, n_a, real(1),dkg0);
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(PSphere_1(i, j) - PSphere_2(i, j)) < eps);
        }
      }
    }

};  // end of struct DerivativesOfLocalisationTensorsTest

TFEL_TESTS_GENERATE_PROXY(DerivativesOfLocalisationTensorsTest, "DerivativesOfLocalisationTensors");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DerivativesOfLocalisationTensors.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
