/*!
 * \file   StandardElastoViscoPlasticityBrickTest.cxx
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

struct StandardElastoViscoPlasticityBrickTest final
    : public tfel::tests::TestCase {
  StandardElastoViscoPlasticityBrickTest()
      : tfel::tests::TestCase("MFront",
                              "StandardElastoViscoPlasticityBrickTest") {
    mfront::initDSLs();
  }  // end of StandardElastoViscoPlasticityBrickTest

  tfel::tests::TestResult execute() override {
    // young is automatically declared and associated with the YoungModulus
    // glossary name
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@MaterialProperty stress young;\n");
    this->check(
        "@MaterialProperty stress young;\n"
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n");
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@MaterialProperty stress E;\n"
        "E.setGlossaryName(\"YoungModulus\");");
    // nu is automatically declared and associated with the PoissonRatio
    // glossary name
    this->check(
        "@MaterialProperty stress nu;\n"
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n");
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@MaterialProperty stress nu;\n");
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@MaterialProperty stress n;\n"
        "n.setGlossaryName(\"PoissonRatio\");");
    // defining the elastic properties in the brick in not compatible with the
    // use of @RequireStiffnessTensor, @ElasticMaterialProperties,
    // @ComputeStiffnessTensor
    this->check(
        "@ElasticMaterialProperties {110e9,0.3};\n"
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n");
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@ElasticMaterialProperties {110e9,0.3};\n");
    this->check(
        "@RequireStiffnessTensor;\n"
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n");
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@RequireStiffnessTensor;\n");
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@ComputeStiffnessTensor{110e9,0.3};\n");
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "stress_potential : Hooke{\n"
        "young_modulus:150e9,\n"
        "poisson_ratio:0.3\n"
        "}\n"
        "};\n"
        "@ComputeStiffnessTensor{110e9,0.3};\n");
    // a stress potential must be defined
    this->check(
        "@Brick StandardElastoViscoPlasticity{\n"
        "};\n");
    return this->result;
  }

 private:
  void check(const std::string& s) {
    auto& f = mfront::DSLFactory::getDSLFactory();
    auto dsl = f.createNewDSL("Implicit");
    try {
      dsl->analyseString(s);
    } catch (std::runtime_error& e) {
      TestCase::registerResult("analysing '" + s +
                                   "' "
                                   "has thrown as expected "
                                   "(" +
                                   std::string(e.what()) + ")",
                               true);
      return;
    }
    TestCase::registerResult(
        "analysing '" + s + "' did *not* throw as expected", false);
  }
};

TFEL_TESTS_GENERATE_PROXY(StandardElastoViscoPlasticityBrickTest,
                          "StandardElastoViscoPlasticityBrickTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StandardElastoViscoPlasticityTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
