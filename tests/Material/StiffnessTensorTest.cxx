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

#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
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
};

TFEL_TESTS_GENERATE_PROXY(StiffnessTensorTest, "StiffnessTensorTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StiffnessTensorTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
