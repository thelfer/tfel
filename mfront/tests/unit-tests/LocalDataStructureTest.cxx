/*!
 * \file   LocalDataStructureTest.cxx
 * \brief
 * \author THOMAS HELFER
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
#include <algorithm>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"

struct LocalDataStructureTest final : public tfel::tests::TestCase {
  LocalDataStructureTest()
      : tfel::tests::TestCase("MFront", "LocalDataStructureTest") {
  }  // end of LocalDataStructureTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }

 private:
  void test1() {
    using ModellingHypothesis = mfront::LocalDataStructure::ModellingHypothesis;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto ps = ModellingHypothesis::PLANESTRESS;
    mfront::LocalDataStructure lds;
    lds.addVariable(uh, {"stress", "v"});
    // twice the same variable
    TFEL_TESTS_CHECK_THROW(lds.addVariable(uh, {"stress", "v"}),
                           std::runtime_error);
    // variable already declared
    TFEL_TESTS_CHECK_THROW(lds.addVariable(ps, {"stress", "v"}),
                           std::runtime_error);
  }  // end of execute
  void test2() {
    using ModellingHypothesis = mfront::LocalDataStructure::ModellingHypothesis;
    const auto ps = ModellingHypothesis::PLANESTRESS;
    const auto as = ModellingHypothesis::AXISYMMETRICAL;
    mfront::LocalDataStructure lds;
    lds.addVariable(ps, {"stress", "v"});
    lds.addVariable(as, {"stress", "v"});
    const auto& mh = lds.getSpecialisedHypotheses();
    TFEL_TESTS_ASSERT(mh.size() == 2u);
    TFEL_TESTS_ASSERT(std::find(mh.begin(), mh.end(), ps) != mh.end());
    TFEL_TESTS_ASSERT(std::find(mh.begin(), mh.end(), as) != mh.end());
  }  // end of execute
  void test3() {
    using ModellingHypothesis = mfront::LocalDataStructure::ModellingHypothesis;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto ps = ModellingHypothesis::PLANESTRESS;
    const auto as = ModellingHypothesis::AXISYMMETRICAL;
    mfront::LocalDataStructure lds;
    lds.addVariable(ps, {"stress", "v"});
    lds.addVariable(as, {"stress", "v"});
    mfront::BehaviourDescription bd;
    // invalid data structure name
    TFEL_TESTS_CHECK_THROW(bd.addLocalDataStructure(lds), std::runtime_error);
    lds.name = "seb_data";
    bd.addLocalDataStructure(lds);
    bd.setModellingHypotheses({ps, as});
    const auto& mh = bd.getDistinctModellingHypotheses();
    TFEL_TESTS_ASSERT(mh.size() == 2u);
    const auto& d = bd.getBehaviourData(ps);
    const auto& lvs = d.getLocalVariables();
    TFEL_TESTS_ASSERT(lvs.size() == 1u);
    const auto& v = lvs[0];
    TFEL_TESTS_ASSERT(v.name == "seb_data");
    TFEL_TESTS_ASSERT(v.type == "struct{\nstress v;\n}");
    TFEL_TESTS_ASSERT(bd.getBehaviourData(uh).getLocalVariables().size() == 0u);
  }
};

TFEL_TESTS_GENERATE_PROXY(LocalDataStructureTest, "LocalDataStructureTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LocalDataStructure.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
