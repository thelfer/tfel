/*!
 * \file   OrthotropicAxesConventionTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/01/2018
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

#include "MFront/InitDSLs.hxx"
#include "MFront/DSLFactory.hxx"

struct OrthotropicAxesConventionTest final : public tfel::tests::TestCase {
  OrthotropicAxesConventionTest()
      : tfel::tests::TestCase("MFront", "OrthotropicAxesConventionTest") {
  }  // end of OrthotropicAxesConventionTest

  tfel::tests::TestResult execute() override {
    this->check(
        "@ModellingHypothesis Axisymmetrical;"
        "@OrthotropicBehaviour<Plate>;");
    this->check(
        "@ModellingHypothesis AxisymmetricalGeneralisedPlaneStress;"
        "@OrthotropicBehaviour<Plate>;");
    this->check(
        "@ModellingHypothesis AxisymmetricalGeneralisedPlaneStrain;"
        "@OrthotropicBehaviour<Plate>;");
    this->check(
        "@OrthotropicBehaviour<Plate>;"
        "@ModellingHypothesis AxisymmetricalGeneralisedPlaneStrain;");
    this->check(
        "@ModellingHypotheses {Axisymmetrical,PlaneStrain};"
        "@OrthotropicBehaviour<Plate>;");
    return this->result;
  }  // end of execute()
 private:
  void check(const std::string& s) {
    auto& f = mfront::DSLFactory::getDSLFactory();
    auto dsl = f.createNewParser("Implicit");
    try {
      dsl->analyseString(s);
    } catch (std::runtime_error& e) {
      TestCase::registerResult("analysing '" + s +
                                   "' has thrown as expected "
                                   "(" +
                                   std::string(e.what()) + ")",
                               true);
      return;
    }
    TestCase::registerResult(
        "analysing '" + s + "' did *not* throw as expected", false);
  }
};

TFEL_TESTS_GENERATE_PROXY(OrthotropicAxesConventionTest,
                          "OrthotropicAxesConventionTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  mfront::initDSLs();
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("OrthotropicAxesConventionTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
