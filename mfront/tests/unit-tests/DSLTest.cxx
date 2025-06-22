/*!
 * \file   DSLTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 sept. 2015
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

struct DSLTest final : public tfel::tests::TestCase {
  DSLTest() : tfel::tests::TestCase("MFront", "DSLTest") {}  // end of DSLTest

  tfel::tests::TestResult execute() override {
    // test of @Author keyword handling
    this->check("DefaultDSL", "@Author te @st1;");
    // test of @Date keyword handling
    this->check("DefaultDSL", "@Date 2/@1/2015;");
    // test of @Material keyword handling
    this->check("DefaultDSL", "@Material 1_;");
    // multiple call to @Material
    this->check("DefaultDSL",
                "@Material m;"
                "@Material m2;");
    // multiple call to @Material
    this->check("DefaultDSL",
                "@Material m;"
                "@Material m2;");
    // unknown modelling hypothesis
    this->check("DefaultDSL", "@ModellingHypothesis PlaneStran;");
    this->check("DefaultDSL", "@ModellingHypotheses {PlaneStrain,PlaneStess};");
    // invalid variable names
    this->check("DefaultDSL", "@StateVariable real 12a;");
    this->check("DefaultDSL", "@StateVariable real @12a;");
    this->check("DefaultDSL", "@StateVariable real -12a;");
    this->check("DefaultDSL", "@StateVariable real \"-12a\";");
    this->check("DefaultDSL", "@StateVariable real '1';");
    for (const auto& dsl : {"MaterialLaw", "Model"}) {
      this->check(dsl, "@Input 12a;");
      this->check(dsl, "@Input @12a;");
      this->check(dsl, "@Input -12a;");
      this->check(dsl, "@Input \"-12a\";");
      this->check(dsl, "@Input '1';");
    }
    // invalid state variable declarations
    this->check("DefaultDSL", "@StateVariable stress a, stress b;");
    // duplicated variables names
    this->check("DefaultDSL",
                "@MaterialProperty real v;"
                "@StateVariable real v;");
    this->check("DefaultDSL",
                "@MaterialProperty real v;"
                "@StaticVariable   real v;");
    this->check("DefaultDSL",
                "@MaterialProperty real v;"
                "@Parameter v = 12;");
    for (const auto& r :
         {"eto", "sig", "deto", "T", "dT", "dt", "computeStress",
          "computeFinalStress", "temperature", "stress", "strain"}) {
      for (const auto& t :
           {"MaterialProperty", "StateVariable", "AuxiliaryStateVariable",
            "ExternalStateVariable", "StaticVariable"}) {
        this->check("DefaultDSL",
                    "@" + std::string(t) + " real " + std::string(r) + ";");
      }
      this->check("DefaultDSL", "@Parameter " + std::string(r) + ";");
    }
    for (const auto& t :
         {"MaterialProperty", "StateVariable", "AuxiliaryStateVariable",
          "ExternalStateVariable", "StaticVariable"}) {
      this->check("Implicit", "@" + std::string(t) + " real eel;");
      this->check("RungeKutta", "@" + std::string(t) + " real eel;");
    }
    this->check("DefaultDSL",
                "@RequiresStiffnessTensor;"
                "@MaterialProperty real D;");
    for (const auto& dsl : {"MaterialLaw", "Model"}) {
      this->check(dsl,
                  "@Input a;"
                  "@Input a;");
    }
    // unknown state variable method test
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "a.setGlossaryName(\"test\");");
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "a.setEntryName(\"?test\");");
    // multiple glossary/entry name definition
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "a.setGlossaryName(\"Temperature\");");
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "@StateVariable real b;"
                "a.setGlossaryName(\"Porosity\");"
                "b.setGlossaryName(\"Porosity\");");
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "@StateVariable real b;"
                "a.setEntryName(\"Porosity\");"
                "b.setEntryName(\"Porosity\");");
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "@StateVariable real b;"
                "a.setEntryName(\"InternalPorosityPressure\");"
                "b.setEntryName(\"InternalPorosityPressure\");");
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "a.setGlossaryName(\"Porosity\");"
                "a.setGlossaryName(\"AxialGrowth\");");
    this->check("DefaultDSL",
                "@StateVariable real a;"
                "a.setEntryName(\"InternalPorosityPressure\");"
                "a.setEntryName(\"InternalPorosityPressure2\");");
    for (const auto& dsl : {"MaterialLaw", "Model"}) {
      for (const auto& t : {"Input", "Output"}) {
        this->check(dsl, "@" + std::string(t) +
                             " a;"
                             "a.setGlossaryName(\"test\");");
        this->check(dsl, "@" + std::string(t) +
                             " a;"
                             "a.setEntryName(\"?test\");");
        // multiple glossary/entry name definition
        this->check(dsl, "@" + std::string(t) +
                             " a;"
                             "@" +
                             std::string(t) +
                             " b;"
                             "a.setGlossaryName(\"Porosity\");"
                             "b.setGlossaryName(\"Porosity\");");
        this->check(dsl, "@" + std::string(t) +
                             " a;"
                             "@Input b;"
                             "a.setEntryName(\"Porosity\");"
                             "b.setEntryName(\"Porosity\");");
        this->check(dsl, "@" + std::string(t) +
                             " a;"
                             "@Input b;"
                             "a.setEntryName(\"InternalPorosityPressure\");"
                             "b.setEntryName(\"InternalPorosityPressure\");");
        this->check(dsl, "@" + std::string(t) +
                             " a;"
                             "a.setGlossaryName(\"Porosity\");"
                             "a.setGlossaryName(\"AxialGrowth\");");
        this->check(dsl, "@" + std::string(t) +
                             " a;"
                             "a.setEntryName(\"InternalPorosityPressure\");"
                             "a.setEntryName(\"InternalPorosityPressure2\");");
      }
    }
    // multiple outputs definition
    this->check("MaterialLaw", "@Output a;@Output b;");
    this->check("MaterialLaw", "@Output a,b;");
    this->check("Model", "@Output a,a;");
    // bounds on unknown variable
    this->check("DefaultDSL", "@Bounds v in [0:1];");
    // invalid bounds
    this->check("DefaultDSL", "@StateVariable real v;@Bounds v in [2:1];");
    this->check("DefaultDSL", "@StateVariable real v;@Bounds v in ]1:2];");
    this->check("DefaultDSL", "@StateVariable real v;@Bounds v in [1:2[;");
    this->check("DefaultDSL", "@StateVariable real v;@Bounds v in ]1:2[;");
    this->check("DefaultDSL", "@StateVariable real v;@Bounds v in [2:*];");
    this->check("DefaultDSL", "@StateVariable real v;@Bounds v in ]*:*[;");
    this->check("DefaultDSL", "@StateVariable real v;@Bounds v in ]-*:0];");
    this->check("DefaultDSL", "@ComputeThermalExpansion {\"test.mfront\"};");
    return this->result;
  }  // end of execute()
 private:
  void check(const std::string& n, const std::string& s) {
    auto& f = mfront::DSLFactory::getDSLFactory();
    auto dsl = f.createNewParser(n);
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

TFEL_TESTS_GENERATE_PROXY(DSLTest, "DSLTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  mfront::initDSLs();
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DSLTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
