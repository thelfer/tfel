/*!
 * \file   tests/Material/StiffnessTensorTest.cxx
 * \brief  This file tests the computation of some stiffness tensors
 * \date   26/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/Lame.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

struct StiffnessTensorTest final : public tfel::tests::TestCase {
  StiffnessTensorTest()
      : tfel::tests::TestCase("TFEL/Material", "StiffnessTensorTest") {
  }  // end of StiffnessTensorTest
  tfel::tests::TestResult execute() override {
    this->testPlate<ModellingHypothesis::TRIDIMENSIONAL>();
    this->testPlate<ModellingHypothesis::PLANESTRAIN>();
    this->testPlate<ModellingHypothesis::GENERALISEDPLANESTRAIN>();
    this->testPlate<ModellingHypothesis::PLANESTRESS>();
    this->testAxisymmetricalGeneralisedPlaneStress();
    return this->result;
  }  // end of execute
 private:
  using ModellingHypothesis = tfel::material::ModellingHypothesis;
  //! \brief the plate axes are the same in all the supported hypotheses
  template <ModellingHypothesis::Hypothesis H>
  void testPlate() {
    using namespace tfel::material;
    constexpr auto N = ModellingHypothesisToSpaceDimension<H>::value;
    constexpr auto smt =
        GetDefaultStiffnessTensorAlterationCharacteristic<H>::value;
    tfel::math::st2tost2<N, double> D1, D2;
    computeOrthotropicStiffnessTensor<H, smt, OrthotropicAxesConvention::PLATE>(
        D1, 200e9, 100e9, 50e9, 0.3, 0.2, 0.1, 70e9, 40e9, 20e9);
    computeOrthotropicStiffnessTensor<H, smt,
                                      OrthotropicAxesConvention::DEFAULT>(
        D2, 200e9, 100e9, 50e9, 0.3, 0.2, 0.1, 70e9, 40e9, 20e9);
    for (unsigned short i = 0; i != D1.size(0); ++i) {
      for (unsigned short j = 0; j != D1.size(1); ++j) {
        TFEL_TESTS_CHECK_EQUAL(D1(i, j), D2(i, j));
      }
    }
  }  // end of testPlate
  /*!
   * \brief the altered stiffness tensors of the axisymmetrical generalised
   * plane stress hypothesis condense the axial component (index 1)
   */
  void testAxisymmetricalGeneralisedPlaneStress() {
    using namespace tfel::material;
    using StiffnessTensor = tfel::math::st2tost2<1u, double>;
    constexpr auto H =
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
    constexpr auto altered = StiffnessTensorAlterationCharacteristic::ALTERED;
    constexpr auto unaltered =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    const auto E = 150e9;
    const auto nu = 0.3;
    StiffnessTensor D, Da1(nan), Da2(nan), Da3(nan);
    computeIsotropicStiffnessTensor<H, unaltered>(D, E, nu);
    computeIsotropicStiffnessTensor<H, altered>(Da1, E, nu);
    computeAlteredElasticStiffness<H, double>::exe(Da2, computeLambda(E, nu),
                                                   computeMu(E, nu));
    ComputeAlteredStiffnessTensor<H>::exe(Da3, D);
    this->checkAxialCondensation(D, Da1);
    this->checkAxialCondensation(D, Da2);
    this->checkAxialCondensation(D, Da3);
    StiffnessTensor Do, Dao1(nan), Dao2(nan);
    computeOrthotropicStiffnessTensor<H, unaltered>(Do, 200e9, 100e9, 50e9, 0.3,
                                                    0.2, 0.1, 70e9, 40e9, 20e9);
    computeOrthotropicStiffnessTensor<H, altered>(Dao1, 200e9, 100e9, 50e9, 0.3,
                                                  0.2, 0.1, 70e9, 40e9, 20e9);
    ComputeAlteredStiffnessTensor<H>::exe(Dao2, Do);
    this->checkAxialCondensation(Do, Dao1);
    this->checkAxialCondensation(Do, Dao2);
  }  // end of testAxisymmetricalGeneralisedPlaneStress
  //! \brief check that Da is D condensed on the axial component
  void checkAxialCondensation(const tfel::math::st2tost2<1u, double>& D,
                              const tfel::math::st2tost2<1u, double>& Da) {
    const auto eps = 1e-14 * D(0, 0);
    for (unsigned short i = 0; i != 3; ++i) {
      for (unsigned short j = 0; j != 3; ++j) {
        const auto Da_ij =
            ((i == 1) || (j == 1)) ? 0. : D(i, j) - D(i, 1) * D(1, j) / D(1, 1);
        TFEL_TESTS_ASSERT(std::abs(Da(i, j) - Da_ij) < eps);
      }
    }
  }  // end of checkAxialCondensation
};

TFEL_TESTS_GENERATE_PROXY(StiffnessTensorTest, "StiffnessTensorTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StiffnessTensorTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
