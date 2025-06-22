/*!
 * \file   mfront/tests/unit-tests/BehaviourDescriptionTest.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  29 may 2015
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

#include "MFront/BehaviourDescription.hxx"

struct BehaviourDescriptionTest final : public tfel::tests::TestCase {
  BehaviourDescriptionTest()
      : tfel::tests::TestCase("MFront", "BehaviourDescriptionTest") {
  }  // end of BehaviourDescriptionTest

  tfel::tests::TestResult execute() override {
    using namespace mfront;
    this->test1();
    this->test2(&BehaviourDescription::declareAsASmallStrainStandardBehaviour);
    {
      auto bd = BehaviourDescription{};
      bd.declareAsAFiniteStrainStandardBehaviour(true);
      TFEL_TESTS_CHECK_THROW(bd.declareAsASmallStrainStandardBehaviour(),
                             std::runtime_error);
      TFEL_TESTS_CHECK_THROW(bd.declareAsAFiniteStrainStandardBehaviour(true),
                             std::runtime_error);
      TFEL_TESTS_CHECK_THROW(bd.declareAsACohesiveZoneModel(),
                             std::runtime_error);
    }
    this->test2(&BehaviourDescription::declareAsACohesiveZoneModel);
    this->test3(&BehaviourDescription::addMaterialProperty);
    this->test3(&BehaviourDescription::addStateVariable);
    this->test3(&BehaviourDescription::addAuxiliaryStateVariable);
    this->test3(&BehaviourDescription::addIntegrationVariable);
    this->test3(&BehaviourDescription::addExternalStateVariable);
    this->test3(&BehaviourDescription::addLocalVariable);
    this->test3(&BehaviourDescription::addParameter);
    this->test4();
    this->test5();
    this->test6(&BehaviourDescription::addMaterialProperty);
    this->test6(&BehaviourDescription::addStateVariable);
    this->test6(&BehaviourDescription::addAuxiliaryStateVariable);
    this->test6(&BehaviourDescription::addIntegrationVariable);
    this->test6(&BehaviourDescription::addExternalStateVariable);
    this->test6(&BehaviourDescription::addLocalVariable);
    this->test6(&BehaviourDescription::addParameter);
    return this->result;
  }  // end of execute

 private:
  using BehaviourData = mfront::BehaviourData;
  using BehaviourDescription = mfront::BehaviourDescription;
  using VariableDescription = mfront::VariableDescription;
  using StaticVariableDescription = mfront::StaticVariableDescription;
  using ModellingHypothesis = tfel::material::ModellingHypothesis;
  using Hypothesis = ModellingHypothesis::Hypothesis;

  // check that the behaviour description fails appropriately
  void test1() {
    using namespace std;
    using namespace mfront;
    using tfel::material::ModellingHypothesis;
    const BehaviourDescription bd;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto psh = ModellingHypothesis::PLANESTRESS;
    TFEL_TESTS_CHECK_THROW(bd.getModellingHypotheses(), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getDistinctModellingHypotheses(), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.isModellingHypothesisSupported(uh),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.isModellingHypothesisSupported(psh),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getBehaviourData(psh), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getBehaviourName(), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getDSLName(), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.getClassName(), runtime_error);
  }  // end of test1

  void test2(void (BehaviourDescription::*m)()) {
    using namespace mfront;
    auto bd = BehaviourDescription{};
    (bd.*m)();
    TFEL_TESTS_CHECK_THROW(bd.declareAsASmallStrainStandardBehaviour(),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.declareAsAFiniteStrainStandardBehaviour(true),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.declareAsACohesiveZoneModel(),
                           std::runtime_error);
  }  // end of test2

  void test3(void (BehaviourDescription::*m)(
      const Hypothesis,
      const VariableDescription&,
      const BehaviourData::RegistrationStatus)) {
    using std::runtime_error;
    BehaviourDescription bd;
    const auto v = VariableDescription{"real", "p", 1, 0u};
    const auto v2 = VariableDescription{"real", "p2", 1, 0u};
    const auto v3 = StaticVariableDescription{"real", "p", 0u, 1.e-4};
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    (bd.*m)(h, v, BehaviourData::UNREGISTRED);
    TFEL_TESTS_CHECK_THROW(bd.addMaterialProperty(h, v), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addStateVariable(h, v), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addAuxiliaryStateVariable(h, v), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addIntegrationVariable(h, v), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addLocalVariable(h, v), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addParameter(h, v), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addExternalStateVariable(h, v), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addStaticVariable(h, v3), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addMaterialProperties(h, {v, v2}), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addStateVariables(h, {v, v2}), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addAuxiliaryStateVariables(h, {v, v2}),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addIntegrationVariables(h, {v, v2}),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addLocalVariables(h, {v, v2}), runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addExternalStateVariables(h, {v, v2}),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd.addStaticVariable(h, v3), runtime_error);
  }  // end of test3

  void test4() {
    using std::runtime_error;
    BehaviourDescription bd;
    TFEL_TESTS_ASSERT(!bd.useQt());
    bd.setUseQt(true);
    TFEL_TESTS_ASSERT(bd.useQt());
    TFEL_TESTS_CHECK_THROW(bd.setUseQt(false), runtime_error);
  }

  void test5() {
    using std::runtime_error;
    BehaviourDescription bd;
    TFEL_TESTS_ASSERT(!bd.isGradientName("test"));
    TFEL_TESTS_ASSERT(!bd.isGradientIncrementName("test"));
    BehaviourDescription bd2;
    bd2.declareAsASmallStrainStandardBehaviour();
    TFEL_TESTS_CHECK_THROW(bd2.declareAsASmallStrainStandardBehaviour(),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd2.declareAsAFiniteStrainStandardBehaviour(false),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd2.declareAsACohesiveZoneModel(), runtime_error);
    TFEL_TESTS_ASSERT(bd2.getBehaviourType() ==
                      BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR);
    TFEL_TESTS_ASSERT(bd2.isGradientName("eto"));
    TFEL_TESTS_ASSERT(bd2.isGradientIncrementName("deto"));
    TFEL_TESTS_ASSERT(bd2.getMainVariables().size() == 1);
    TFEL_TESTS_ASSERT(bd2.getMainVariables().begin()->first.name == "eto");
    TFEL_TESTS_ASSERT(bd2.getMainVariables().begin()->first.type ==
                      "StrainStensor");
    TFEL_TESTS_ASSERT(mfront::Gradient::isIncrementKnown(
        bd2.getMainVariables().begin()->first));
    TFEL_TESTS_ASSERT(bd2.getMainVariables().begin()->second.name == "sig");
    TFEL_TESTS_ASSERT(bd2.getMainVariables().begin()->second.type ==
                      "StressStensor");
    TFEL_TESTS_ASSERT(bd2.getTangentOperatorType() == "StiffnessTensor");
    TFEL_TESTS_ASSERT(bd2.getStressFreeExpansionType() == "StrainStensor");
    BehaviourDescription bd3;
    bd3.declareAsAFiniteStrainStandardBehaviour(true);
    TFEL_TESTS_ASSERT(bd3.getBehaviourType() ==
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR);
    TFEL_TESTS_CHECK_THROW(bd3.declareAsASmallStrainStandardBehaviour(),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd3.declareAsAFiniteStrainStandardBehaviour(true),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd3.declareAsACohesiveZoneModel(), runtime_error);
    TFEL_TESTS_ASSERT(bd3.isGradientName("F"));
    TFEL_TESTS_ASSERT(bd3.getMainVariables().size() == 1);
    TFEL_TESTS_ASSERT(bd3.getMainVariables().begin()->first.name == "F");
    TFEL_TESTS_ASSERT(bd3.getMainVariables().begin()->first.type ==
                      "DeformationGradientTensor");
    TFEL_TESTS_ASSERT(!mfront::Gradient::isIncrementKnown(
        bd3.getMainVariables().begin()->first));
    TFEL_TESTS_ASSERT(bd3.getMainVariables().begin()->second.name == "sig");
    TFEL_TESTS_ASSERT(bd3.getMainVariables().begin()->second.type ==
                      "StressStensor");
    TFEL_TESTS_ASSERT(bd3.getTangentOperatorType() ==
                      "FiniteStrainBehaviourTangentOperator<N,stress>");
    TFEL_TESTS_ASSERT(bd3.getStressFreeExpansionType() == "StrainStensor");
    BehaviourDescription bd4;
    bd4.declareAsACohesiveZoneModel();
    TFEL_TESTS_ASSERT(bd4.getBehaviourType() ==
                      BehaviourDescription::COHESIVEZONEMODEL);
    TFEL_TESTS_CHECK_THROW(bd4.declareAsASmallStrainStandardBehaviour(),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd4.declareAsAFiniteStrainStandardBehaviour(false),
                           runtime_error);
    TFEL_TESTS_CHECK_THROW(bd4.declareAsACohesiveZoneModel(), runtime_error);
    TFEL_TESTS_ASSERT(bd4.isGradientName("u"));
    TFEL_TESTS_ASSERT(bd4.getMainVariables().size() == 1);
    TFEL_TESTS_ASSERT(bd4.getMainVariables().begin()->first.name == "u");
    TFEL_TESTS_ASSERT(bd4.getMainVariables().begin()->first.type ==
                      "DisplacementTVector");
    TFEL_TESTS_ASSERT(mfront::Gradient::isIncrementKnown(
        bd4.getMainVariables().begin()->first));
    TFEL_TESTS_ASSERT(bd4.getMainVariables().begin()->second.name == "t");
    TFEL_TESTS_ASSERT(bd4.getMainVariables().begin()->second.type ==
                      "ForceTVector");
    TFEL_TESTS_ASSERT(bd4.getTangentOperatorType() ==
                      "tfel::math::tmatrix<N,N,stress>");
    TFEL_TESTS_CHECK_THROW(bd4.getStressFreeExpansionType(), runtime_error);
  }

  void test6(void (BehaviourDescription::*m)(
      const Hypothesis,
      const VariableDescription&,
      const BehaviourData::RegistrationStatus)) {
    using std::runtime_error;
    BehaviourDescription bd;
    const auto v = VariableDescription{"real", "p", 1, 0u};
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto h2 = ModellingHypothesis::PLANESTRESS;
    (bd.*m)(h2, v, BehaviourData::UNREGISTRED);
    TFEL_TESTS_CHECK_THROW((bd.*m)(h, v, BehaviourData::UNREGISTRED),
                           runtime_error);
  }
};

TFEL_TESTS_GENERATE_PROXY(BehaviourDescriptionTest, "BehaviourDescriptionTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("BehaviourDescription.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
