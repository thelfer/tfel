/*!
 * \file   test_model.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   23 juin 2016
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
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Material/MaterialException.hxx"
#include "MFront/B4C_ConcentrationModel.hxx"
#include "MFront/SiC_IrradiationSwellingModel_GoFaster.hxx"
#include "MFront/UO2_Shrinkage_RAPHAEL2008.hxx"
#include "MFront/FastNeutronFluence.hxx"
#include "MFront/NeutronFluence.hxx"

struct B4CConcentrationModelTest final : public tfel::tests::TestCase {
  B4CConcentrationModelTest()
      : tfel::tests::TestCase("MFront/Model", "B4CConcentrationModelTest") {
  }  // end of B4CConcentrationModelTest
  virtual tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
  //! destructor
  virtual ~B4CConcentrationModelTest() = default;

 private:
  void test1() {
    mfront::B4C_ConcentrationModel<double> cm;
    double bu = 0;
    double b10 = 5.06732753005997e+28;
    TFEL_TESTS_CHECK_THROW((cm.Compute(bu, b10, -1., 0., 0., 0., 3600)),
                           tfel::material::OutOfBoundsException);
  }
  void test2() {
    mfront::B4C_ConcentrationModel<double> cm;
    const double b10_0 = 5.06732753005997e+28;
    const double c = 8.35838E-09;
    const double c_1 = 8.35838E-09;
    double bu = 0;
    double b10 = b10_0;
    double t = 0.0;
    const double te = 41696641.0;
    const double dt = (te - t) / 100;
    while (std::abs(te - t) > dt / 2) {
      const double bu_1 = bu;
      const double b10_1 = b10;
      cm.Compute(bu, b10, bu_1, b10_1, c, c_1, dt);
      t += dt;
      TFEL_TESTS_ASSERT(std::abs(b10 - b10_0 * exp(-c * t)) < b10_0 * 1e-6);
    }
  }
  void test3() {
    mfront::SiC_IrradiationSwellingModel_GoFaster<double> sm;
    const double T = 1603.15;
    const double f = 2.e15;
    double n = 0;
    double s = 0.;
    double t = 0.0;
    const double te = 41696641.0;
    const double dt = (te - t) / 100;
    while (std::abs(te - t) > dt / 2) {
      s = sm.compute(T, T, f, f, n, n, s, dt);
      n += f * dt;
      t += dt;
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(B4CConcentrationModelTest,
                          "B4CConcentrationModelTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("test-model.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
