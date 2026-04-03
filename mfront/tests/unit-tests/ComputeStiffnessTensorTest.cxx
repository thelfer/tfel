/*!
 * \file   ComputeStiffnessTensorTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   2 may 2016
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
#include "MFront/BehaviourDescription.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"

struct ComputeStiffnessTensorTest final : public tfel::tests::TestCase {
  ComputeStiffnessTensorTest()
      : tfel::tests::TestCase("MFront", "ComputeStiffnessTensorTest") {
  }  // end of ComputeStiffnessTensorTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute()
 private:
  void test1() {
    this->check("Implicit",
                "@RequireStiffnessTensor;"
                "@ComputeStiffnessTensor {150e9,0.3};");
    this->check("Implicit",
                "@ComputeStiffnessTensor {150e9,0.3};"
                "@RequireStiffnessTensor;");
    this->check("Implicit",
                "@ComputeStiffnessTensor {150e9,0.3};"
                "@MaterialProperty stress young;");
    this->check("Implicit",
                "@ComputeStiffnessTensor {150e9,0.3};"
                "@MaterialProperty stress yg;"
                "yg.setGlossaryName(\"YoungModulus\");");
    this->check("Implicit",
                "@ComputeStiffnessTensor {150e9,0.3};"
                "@MaterialProperty real nu;");
    this->check("Implicit",
                "@OrthotrophicBehaviours;"
                "@ComputeStiffnessTensor {150e9,0.3};");
    this->check("Implicit",
                "@ComputeStiffnessTensor {150e9,0.3};"
                "@MaterialProperty real nu;"
                "nu.setGlossaryName(\"PoissonRatio\");");
    this->check("Implicit",
                "@MaterialProperty real nu;"
                "nu.setGlossaryName(\"PoissonRatio\");"
                "@ComputeStiffnessTensor {150e9,0.3};");
    this->check("Implicit",
                "@OrthotropicBehaviour;"
                "@ComputeStiffnessTensor {150e9,,0.3};");
    this->check("Implicit",
                "@ComputeStiffnessTensor {150e9,150e9,150e9,"
                "                         0.3,0.3,0.3,"
                "                         150e9,150e9,150e9};");
    this->check("Implicit",
                "@OrthotropicBehaviour;"
                "@IsotropicElasticBehaviour;"
                "@ComputeStiffnessTensor {150e9,150e9,150e9,"
                "                         0.3,0.3,0.3,"
                "                         150e9,150e9,150e9};");
  }
  void test2() {
    const auto h = tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto& f = mfront::DSLFactory::getDSLFactory();
    auto dsl = std::dynamic_pointer_cast<mfront::AbstractBehaviourDSL>(
        f.createNewDSL("Implicit"));
    dsl->analyseString("@ComputeStiffnessTensor {150e9,0.3};");
    const auto& bd = dsl->getBehaviourDescription().getBehaviourData(h);
    TFEL_TESTS_ASSERT(bd.getParameters().size() == 2u);
    TFEL_TESTS_ASSERT(bd.isGlossaryNameUsed("YoungModulus"));
    TFEL_TESTS_ASSERT(bd.isGlossaryNameUsed("PoissonRatio"));
    TFEL_TESTS_ASSERT(bd.hasParameter("young"));
    TFEL_TESTS_ASSERT(bd.hasGlossaryName("young"));
    TFEL_TESTS_ASSERT(bd.getExternalName("young") == "YoungModulus");
    TFEL_TESTS_ASSERT(bd.hasParameter("nu"));
    TFEL_TESTS_ASSERT(bd.hasGlossaryName("nu"));
    TFEL_TESTS_ASSERT(bd.getExternalName("nu") == "PoissonRatio");
  }
  void test3() {
    const auto h = tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto& f = mfront::DSLFactory::getDSLFactory();
    auto dsl = std::dynamic_pointer_cast<mfront::AbstractBehaviourDSL>(
        f.createNewDSL("Implicit"));
    dsl->analyseString(
        "@OrthotropicBehaviour;"
        "@ComputeStiffnessTensor {150e9,150e9,150e9,"
        "                         0.3,0.3,0.3,"
        "                         150e9,150e9,150e9};");
    const auto& bd = dsl->getBehaviourDescription().getBehaviourData(h);
    TFEL_TESTS_ASSERT(bd.getParameters().size() == 9u);
    for (const auto& v :
         std::map<std::string, std::string>{{"young1", "YoungModulus1"},
                                            {"young2", "YoungModulus2"},
                                            {"young3", "YoungModulus3"},
                                            {"nu12", "PoissonRatio12"},
                                            {"nu23", "PoissonRatio23"},
                                            {"nu13", "PoissonRatio13"},
                                            {"mu12", "ShearModulus12"},
                                            {"mu23", "ShearModulus23"},
                                            {"mu13", "ShearModulus13"}}) {
      TFEL_TESTS_ASSERT(bd.hasParameter(v.first));
      TFEL_TESTS_ASSERT(bd.isGlossaryNameUsed(v.second));
      TFEL_TESTS_ASSERT(bd.hasGlossaryName(v.first));
      TFEL_TESTS_ASSERT(bd.getExternalName(v.first) == v.second);
    }
  }
  void check(const std::string& n, const std::string& s) {
    auto& f = mfront::DSLFactory::getDSLFactory();
    auto dsl = f.createNewDSL(n);
    try {
      dsl->analyseString(s);
    } catch (std::runtime_error& e) {
      std::cerr << e.what() << std::endl;
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

TFEL_TESTS_GENERATE_PROXY(ComputeStiffnessTensorTest,
                          "ComputeStiffnessTensorTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  mfront::initDSLs();
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DSL.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
