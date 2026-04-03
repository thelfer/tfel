/*!
 * \file   GasEquationOfStateTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include "TFEL/Math/Parser/ConstantExternalFunction.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/GasEquationOfState.hxx"

struct GasEquationOfStateTest final : public tfel::tests::TestCase {
  GasEquationOfStateTest()
      : tfel::tests::TestCase("MTest", "GasEquationOfStateTest") {
  }  // end of GasEquationOfStateTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math;
    mtest::EvolutionManager m;
    m.insert({"R", mtest::make_evolution(8.31415)});
    mtest::GasEquationOfState s("P*V-n*R*T", m);
    const double P = 1.e5;
    const double V = 0.022414;
    const double n = 1;
    const double T = 273.15;
    TFEL_TESTS_ASSERT(
        std::abs(s.computeNumberOfMoles(P, V, T) - 0.98696171678913) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s.computePressure(V, n, T) - 101321.05257875) <
                      1e-6);
    TFEL_TESTS_ASSERT(std::abs(s.computeIsothermalBulkModulus(V, n, T) -
                               101321.05257875) < 1e-6);
  }  // end of test1
  void test2() {
    mtest::EvolutionManager m;
    m.insert({"R", mtest::make_evolution(8.31415)});
    mtest::GasEquationOfState s(
        "P*V-(1+2.343e-9*(1-exp(-0.0128*(T-400)))*P+5.2e-9*T**-3*P**2)*n*R*T",
        m);
    const double P = 4.e7;
    const double V = 0.022414;
    const double n = 1;
    const double T = 1023.15;
    TFEL_TESTS_ASSERT(
        std::abs(s.computeNumberOfMoles(P, V, T) - 95.6874860501282) < 1.e-13);
    TFEL_TESTS_ASSERT(std::abs(s.computePressure(V, n, T) - 379860.66477839) <
                      1e-6);
  }  // end of test2
};

TFEL_TESTS_GENERATE_PROXY(GasEquationOfStateTest, "GasEquationOfStateTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("GasEquationOfState.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
