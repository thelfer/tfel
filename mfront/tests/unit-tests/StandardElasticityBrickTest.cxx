/*!
 * \file   StandardElasticityBrickTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02 mai 2016
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

#include "MFront/InitDSLs.hxx"
#include "MFront/DSLFactory.hxx"

struct StandardElasticityBrickTest final : public tfel::tests::TestCase {
  StandardElasticityBrickTest()
      : tfel::tests::TestCase("MFront", "StandardElasticityBrickTest") {
  }  // end of StandardElasticityBrickTest

  tfel::tests::TestResult execute() override {
    // this->check("@MaterialProperty young;\n"
    // 		"young.setGlossaryName(\"YoungModulus\");");
    return this->result;
  }

 private:
  void check(const std::string& s) {
    auto& f = mfront::DSLFactory::getDSLFactory();
    auto dsl = f.createNewParser("Implicit");
    try {
      dsl->analyseString("@DSL \"StandardElasticity\";\n" + s);
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

TFEL_TESTS_GENERATE_PROXY(StandardElasticityBrickTest,
                          "StandardElasticityBrickTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StandardElasticityTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
