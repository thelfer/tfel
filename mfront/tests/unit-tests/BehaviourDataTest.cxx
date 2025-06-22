/*!
 * \file   mfront/tests/unit-tests/BehaviourDataTest.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  12 june 2015
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
#include <algorithm>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

#include "MFront/BehaviourData.hxx"

struct BehaviourDataTest final : public tfel::tests::TestCase {
  BehaviourDataTest()
      : tfel::tests::TestCase("MFront", "BehaviourDataTest") {
  }  // end of BehaviourDataTest

  tfel::tests::TestResult execute() override {
    using namespace mfront;
    this->test(&BehaviourData::addMaterialProperty);
    this->test(&BehaviourData::addStateVariable);
    this->test(&BehaviourData::addAuxiliaryStateVariable);
    this->test(&BehaviourData::addIntegrationVariable);
    this->test(&BehaviourData::addExternalStateVariable);
    this->test(&BehaviourData::addLocalVariable);
    this->test(&BehaviourData::addParameter);
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute

 private:
  using BehaviourData = mfront::BehaviourData;
  using VariableDescription = mfront::VariableDescription;
  using StaticVariableDescription = mfront::StaticVariableDescription;

  void test(void (BehaviourData::*m)(const VariableDescription&,
                                     const BehaviourData::RegistrationStatus)) {
    using std::runtime_error;
    BehaviourData bd;
    const auto v = VariableDescription{"real", "p", 1, 0u};
    const auto v2 = VariableDescription{"real", "p2", 1, 0u};
    const auto v3 = StaticVariableDescription{"real", "p", 0u, 1.e-4};
    (bd.*m)(v, BehaviourData::UNREGISTRED);
    TFEL_TESTS_CHECK_THROW(
        bd.addMaterialProperty(v, BehaviourData::UNREGISTRED), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addStateVariable(v, BehaviourData::UNREGISTRED),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(
        bd.addAuxiliaryStateVariable(v, BehaviourData::UNREGISTRED),
        runtime_error);
    TFEL_TESTS_CHECK_THROW(
        bd.addIntegrationVariable(v, BehaviourData::UNREGISTRED),
        runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addLocalVariable(v, BehaviourData::UNREGISTRED),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addParameter(v, BehaviourData::UNREGISTRED),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(
        bd.addExternalStateVariable(v, BehaviourData::UNREGISTRED),
        runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addStaticVariable(v3, BehaviourData::UNREGISTRED),
                           runtime_error);
  }  // end of test

  void test2() {
    using namespace std;
    using tfel::glossary::Glossary;
    BehaviourData bd;
    const auto v = VariableDescription{"stress", "young", 1, 0u};
    const auto v2 = VariableDescription{"real", "nu", 1, 0u};
    const auto v3 = VariableDescription{"real", "p", 1, 0u};
    const auto v4 = VariableDescription{"real", "seq", 1, 0u};
    const auto v5 = VariableDescription{"real", Glossary::YoungModulus, 1, 0u};
    const auto v6 = VariableDescription{"real", "SomeStressNorm", 1, 0u};
    TFEL_TESTS_ASSERT(!bd.isGlossaryNameUsed(Glossary::YoungModulus));
    TFEL_TESTS_ASSERT(!bd.isGlossaryNameUsed(Glossary::PoissonRatio));
    {  // young is not yet declared
      TFEL_TESTS_CHECK_THROW(
          bd.setGlossaryName("young", Glossary::YoungModulus), runtime_error);
      TFEL_TESTS_CHECK_THROW(bd.setEntryName("young", Glossary::YoungModulus),
                             runtime_error);
    }
    bd.addMaterialProperty(v, BehaviourData::UNREGISTRED);
    TFEL_TESTS_ASSERT(bd.getExternalNames(bd.getMaterialProperties()).size() ==
                      1u);
    TFEL_TESTS_ASSERT(bd.getExternalName("young") == "young");
    TFEL_TESTS_ASSERT(!bd.hasGlossaryName("young"));
    TFEL_TESTS_ASSERT(!bd.hasEntryName("young"));
    bd.setGlossaryName("young", Glossary::YoungModulus);
    TFEL_TESTS_ASSERT(bd.isGlossaryNameUsed(Glossary::YoungModulus));
    TFEL_TESTS_ASSERT(!bd.isGlossaryNameUsed(Glossary::PoissonRatio));
    TFEL_TESTS_ASSERT(bd.getExternalName("young") == Glossary::YoungModulus);
    TFEL_TESTS_ASSERT(bd.hasGlossaryName("young"));
    TFEL_TESTS_ASSERT(!bd.hasEntryName("young"));
    TFEL_TESTS_CHECK_THROW(bd.setEntryName("young", Glossary::YoungModulus),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("young", Glossary::PoissonRatio),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("nu", Glossary::YoungModulus),
                           runtime_error);
    bd.addMaterialProperty(v2, BehaviourData::UNREGISTRED);
    TFEL_TESTS_ASSERT(bd.getExternalNames(bd.getMaterialProperties()).size() ==
                      2u);
    {
      // Softening slople can't be used as a glossary name
      TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("nu", "SofteningSlope"),
                             runtime_error);
      // PoissonRatio can't be used as an entry name
      TFEL_TESTS_CHECK_THROW(bd.setEntryName("nu", Glossary::PoissonRatio),
                             runtime_error);
      // YoungModulus is already used
      TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("nu", Glossary::YoungModulus),
                             runtime_error);
    }
    TFEL_TESTS_ASSERT(!bd.hasGlossaryName("nu"));
    TFEL_TESTS_ASSERT(bd.getExternalName("nu") == "nu");
    bd.setGlossaryName("nu", Glossary::PoissonRatio);
    TFEL_TESTS_ASSERT(bd.isGlossaryNameUsed(Glossary::YoungModulus));
    TFEL_TESTS_ASSERT(bd.isGlossaryNameUsed(Glossary::PoissonRatio));
    TFEL_TESTS_ASSERT(bd.hasGlossaryName("nu"));
    TFEL_TESTS_ASSERT(!bd.hasEntryName("nu"));
    TFEL_TESTS_ASSERT(bd.getExternalName("nu") == Glossary::PoissonRatio);
    bd.addStateVariable(v3, BehaviourData::UNREGISTRED);
    TFEL_TESTS_ASSERT(!bd.hasGlossaryName("p"));
    TFEL_TESTS_ASSERT(!bd.hasEntryName("p"));
    TFEL_TESTS_ASSERT(bd.getExternalName("p") == "p");
    {
      // Softening slople can't be used as a glossary name
      TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("p", "SofteningSlope"),
                             runtime_error);
      // PoissonRatio can't be used as an entry name
      TFEL_TESTS_CHECK_THROW(bd.setEntryName("p", Glossary::PoissonRatio),
                             runtime_error);
      // YoungModulus is already used
      TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("p", Glossary::YoungModulus),
                             runtime_error);
    }
    bd.setEntryName("p", "AStateVariable");
    {
      const auto& enames = bd.getExternalNames(bd.getMaterialProperties());
      TFEL_TESTS_ASSERT(enames.size() == 2u);
      TFEL_TESTS_ASSERT(find(enames.begin(), enames.end(),
                             Glossary::YoungModulus) != enames.end());
      TFEL_TESTS_ASSERT(find(enames.begin(), enames.end(),
                             Glossary::PoissonRatio) != enames.end());
    }
    {
      const auto& enames = bd.getExternalNames(bd.getStateVariables());
      TFEL_TESTS_ASSERT(enames.size() == 1u);
      TFEL_TESTS_ASSERT(find(enames.begin(), enames.end(), "AStateVariable") !=
                        enames.end());
    }
    bd.addAuxiliaryStateVariable(v4, BehaviourData::UNREGISTRED);
    TFEL_TESTS_ASSERT(!bd.hasGlossaryName("seq"));
    TFEL_TESTS_ASSERT(!bd.hasEntryName("seq"));
    TFEL_TESTS_ASSERT(bd.getExternalName("seq") == "seq");
    {
      // Softening slople can't be used as a glossary name
      TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("seq", "SofteningSlope"),
                             runtime_error);
      // PoissonRatio can't be used as an entry name
      TFEL_TESTS_CHECK_THROW(bd.setEntryName("seq", Glossary::PoissonRatio),
                             runtime_error);
      // YoungModulus is already used
      TFEL_TESTS_CHECK_THROW(bd.setGlossaryName("seq", Glossary::YoungModulus),
                             runtime_error);
      // p is already used
      TFEL_TESTS_CHECK_THROW(bd.setEntryName("seq", "p"), runtime_error);
      // AStateVariable is already used
      TFEL_TESTS_CHECK_THROW(bd.setEntryName("seq", "AStateVariable"),
                             runtime_error);
    }
    bd.setEntryName("seq", "SomeStressNorm");
    {
      const auto& enames = bd.getExternalNames(bd.getAuxiliaryStateVariables());
      TFEL_TESTS_ASSERT(enames.size() == 1u);
      TFEL_TESTS_ASSERT(find(enames.begin(), enames.end(), "SomeStressNorm") !=
                        enames.end());
    }
    {
      auto enames = vector<string>{};
      bd.getExternalNames(enames, bd.getStateVariables());
      TFEL_TESTS_ASSERT(enames.size() == 1u);
      TFEL_TESTS_ASSERT(find(enames.begin(), enames.end(), "AStateVariable") !=
                        enames.end());
      bd.appendExternalNames(enames, bd.getAuxiliaryStateVariables());
      TFEL_TESTS_ASSERT(enames.size() == 2u);
      TFEL_TESTS_ASSERT(find(enames.begin(), enames.end(), "AStateVariable") !=
                        enames.end());
      TFEL_TESTS_ASSERT(find(enames.begin(), enames.end(), "SomeStressNorm") !=
                        enames.end());
    }
    // will throw since v5 name is a glossary name
    TFEL_TESTS_CHECK_THROW(bd.addLocalVariable(v5, BehaviourData::UNREGISTRED),
                           runtime_error);
    // will throw since v6 name as already been used as an entry name
    TFEL_TESTS_CHECK_THROW(bd.addLocalVariable(v6, BehaviourData::UNREGISTRED),
                           runtime_error);
    {
      TFEL_TESTS_ASSERT(!bd.isUsedAsEntryName(v.name));
      TFEL_TESTS_ASSERT(!bd.isUsedAsEntryName(v2.name));
      TFEL_TESTS_ASSERT(!bd.isUsedAsEntryName(v3.name));
      TFEL_TESTS_ASSERT(!bd.isUsedAsEntryName(v4.name));
      TFEL_TESTS_ASSERT(!bd.isUsedAsEntryName(v5.name));
      TFEL_TESTS_ASSERT(bd.isUsedAsEntryName("AStateVariable"));
      TFEL_TESTS_ASSERT(bd.isUsedAsEntryName("SomeStressNorm"));
    }
    TFEL_TESTS_ASSERT(bd.getVariableNameFromGlossaryNameOrEntryName(
                          Glossary::YoungModulus) == "young");
    TFEL_TESTS_ASSERT(bd.getVariableNameFromGlossaryNameOrEntryName(
                          Glossary::PoissonRatio) == "nu");
    TFEL_TESTS_ASSERT(
        bd.getVariableNameFromGlossaryNameOrEntryName("AStateVariable") == "p");
    TFEL_TESTS_ASSERT(bd.getVariableNameFromGlossaryNameOrEntryName(
                          "SomeStressNorm") == "seq");
    TFEL_TESTS_CHECK_THROW(
        bd.getVariableNameFromGlossaryNameOrEntryName("unknown"),
        runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getPersistentVariableDescription("unknown"),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getIntegrationVariableDescription("unknown"),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getStateVariableDescription("unknown"),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getMaterialProperties().getVariable("unknown"),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getLocalVariables().getVariable("unknown"),
                           runtime_error);
    TFEL_TESTS_ASSERT(bd.getMaterialProperties().getVariable("young").name ==
                      "young");
    TFEL_TESTS_ASSERT(bd.getMaterialProperties().getVariable("young").type ==
                      "stress");
    TFEL_TESTS_ASSERT(bd.getMaterialProperties().getVariable("nu").name ==
                      "nu");
    TFEL_TESTS_ASSERT(bd.getMaterialProperties().getVariable("nu").type ==
                      "real");
    TFEL_TESTS_ASSERT(!bd.hasCode(BehaviourData::ComputeStress));
    TFEL_TESTS_ASSERT(bd.isMaterialPropertyName("young"));
    TFEL_TESTS_ASSERT(bd.isMaterialPropertyName("nu"));
    TFEL_TESTS_ASSERT(!bd.isLocalVariableName("young"));
    TFEL_TESTS_ASSERT(!bd.isPersistentVariableName("young"));
    TFEL_TESTS_ASSERT(!bd.isIntegrationVariableName("young"));
    TFEL_TESTS_ASSERT(!bd.isIntegrationVariableIncrementName("young"));
    TFEL_TESTS_ASSERT(!bd.isStateVariableName("young"));
    TFEL_TESTS_ASSERT(!bd.isStateVariableIncrementName("young"));
    TFEL_TESTS_ASSERT(!bd.isAuxiliaryStateVariableName("young"));
    TFEL_TESTS_ASSERT(!bd.isExternalStateVariableName("young"));
    TFEL_TESTS_ASSERT(!bd.isExternalStateVariableIncrementName("young"));
    TFEL_TESTS_ASSERT(!bd.isParameterName("young"));
    TFEL_TESTS_ASSERT(!bd.isStaticVariableName("young"));
    TFEL_TESTS_ASSERT(!bd.isMaterialPropertyName("unknown"));
    TFEL_TESTS_ASSERT(bd.isPersistentVariableName("p"));
    TFEL_TESTS_ASSERT(bd.isIntegrationVariableName("p"));
    TFEL_TESTS_ASSERT(bd.isPersistentVariableName("seq"));
    TFEL_TESTS_ASSERT(bd.isStateVariableName("p"));
    TFEL_TESTS_ASSERT(bd.isStateVariableIncrementName("dp"));
    const auto r = bd.getVariablesNames();
    TFEL_TESTS_ASSERT(r.size() == 5);
    TFEL_TESTS_ASSERT(r.find("young") != r.end());
    TFEL_TESTS_ASSERT(r.find("nu") != r.end());
    TFEL_TESTS_ASSERT(r.find("p") != r.end());
    TFEL_TESTS_ASSERT(r.find("seq") != r.end());
    TFEL_TESTS_ASSERT(r.find("T") != r.end());
  }

  void test3() {
    using namespace std;
    BehaviourData bd;
    TFEL_TESTS_ASSERT(!bd.hasAttribute("attr1"));
    bd.setAttribute("attr1", string("value1"), true);
    TFEL_TESTS_CHECK_THROW(bd.getAttribute<bool>("attr1"), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getAttribute<unsigned short>("attr1"),
                           runtime_error);
    TFEL_TESTS_ASSERT(bd.getAttribute<string>("attr1") == "value1");
    bd.getAttribute<string>("attr1") = "value2";
    TFEL_TESTS_ASSERT(bd.getAttribute<string>("attr1") == "value2");
    bd.setAttribute("attr1", string("value1"), true);
    TFEL_TESTS_ASSERT(bd.getAttribute<string>("attr1") == "value2");
    TFEL_TESTS_CHECK_THROW(bd.setAttribute("attr1", string("value1"), false),
                           runtime_error);
    bd.setAttribute("attr2", false, true);
    TFEL_TESTS_ASSERT(!bd.getAttribute<bool>("attr2"));
    bd.setAttribute("attr3", static_cast<unsigned short>(12u), true);
    const auto& a = bd.getAttributes();
    TFEL_TESTS_ASSERT(a.size() == 3u);
    TFEL_TESTS_ASSERT(a.find("attr1") != a.end());
    TFEL_TESTS_ASSERT(a.find("attr2") != a.end());
    TFEL_TESTS_ASSERT(a.find("attr3") != a.end());
    const auto& a1 = a.at("attr1");
    const auto& a2 = a.at("attr2");
    const auto& a3 = a.at("attr3");
    TFEL_TESTS_ASSERT(a1.is<string>());
    TFEL_TESTS_ASSERT(a2.is<bool>());
    TFEL_TESTS_ASSERT(a3.is<unsigned short>());
  }  // end of test3
  void test4() {
    BehaviourData bd;
    bd.reserveName("young");
    const auto v = VariableDescription{"real", "young", 1, 0u};
    TFEL_TESTS_CHECK_THROW(
        bd.addMaterialProperty(v, BehaviourData::UNREGISTRED),
        std::runtime_error);
    bd.addMaterialProperty(v, BehaviourData::ALREADYREGISTRED);
  }  // end of test4
};

TFEL_TESTS_GENERATE_PROXY(BehaviourDataTest, "BehaviourDataTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LocalDataStructure.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
