/*!
 * \file
 * mfront/tests/unit-tests/BehaviourDescriptionBoundsHandlingTest_bounds.cxx
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

struct BehaviourDescriptionBoundsHandlingTest final
    : public tfel::tests::TestCase {
  BehaviourDescriptionBoundsHandlingTest()
      : tfel::tests::TestCase("MFront",
                              "BehaviourDescriptionBoundsHandlingTest") {
  }  // end of BehaviourDescriptionBoundsHandlingTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute

 private:
  // standard bounds checks
  void test1() {
    using namespace mfront;
    using tfel::material::ModellingHypothesis;
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    BehaviourDescription bd;
    bd.addLocalVariable(h, VariableDescription("real", "A", 1u, 0u));
    bd.addLocalVariable(h, VariableDescription("real", "B", 2u, 0u));
    bd.addLocalVariable(h, VariableDescription("real", "C", 8u, 0u));
    VariableBoundsDescription b1;
    b1.boundsType = VariableBoundsDescription::LOWER;
    b1.lowerBound = -3;
    VariableBoundsDescription b2;
    b2.boundsType = VariableBoundsDescription::LOWER;
    b2.lowerBound = -2;
    // setting bounds to an undeclared variable
    TFEL_TESTS_CHECK_THROW(bd.setBounds(h, "D", b1), std::exception);
    // setting bounds on a
    bd.setBounds(h, "A", b1);
    const auto& a = bd.getBehaviourData(h).getVariableDescription("A");
    TFEL_TESTS_ASSERT(a.hasBounds());
    TFEL_TESTS_CHECK_THROW(a.hasBounds(12), std::exception);
    // trying to override already declared bounds
    TFEL_TESTS_CHECK_THROW(bd.setBounds(h, "A", b2), std::exception);
    // setting bounds on all components of b
    bd.setBounds(h, "B", b1);
    const auto& b = bd.getBehaviourData(h).getVariableDescription("B");
    TFEL_TESTS_ASSERT(b.hasBounds());
    for (unsigned short i = 0; i != b.arraySize; ++i) {
      TFEL_TESTS_ASSERT(b.hasBounds(i));
    }
    // trying to override already declared bounds
    TFEL_TESTS_CHECK_THROW(bd.setBounds(h, "B", 3, b2), std::exception);
    // setting bounds on some components of c
    bd.setBounds(h, "C", 2, b1);
    bd.setBounds(h, "C", 6, b2);
    const auto& c = bd.getBehaviourData(h).getVariableDescription("C");
    TFEL_TESTS_ASSERT(!c.hasBounds());
    for (unsigned short i = 0; i != c.arraySize; ++i) {
      if ((i == 2u) || (i == 6u)) {
        TFEL_TESTS_ASSERT(c.hasBounds(i));
      } else {
        TFEL_TESTS_ASSERT(!c.hasBounds(i));
      }
    }
    TFEL_TESTS_CHECK_THROW(bd.setBounds(h, "C", 2, b2), std::exception);
  }
  // physical bounds checks
  void test2() {
    using namespace mfront;
    using tfel::material::ModellingHypothesis;
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    BehaviourDescription bd;
    bd.addLocalVariable(h, VariableDescription("real", "A", 1u, 0u));
    bd.addLocalVariable(h, VariableDescription("real", "B", 2u, 0u));
    bd.addLocalVariable(h, VariableDescription("real", "C", 8u, 0u));
    VariableBoundsDescription b1;
    b1.boundsType = VariableBoundsDescription::LOWER;
    b1.lowerBound = -3;
    VariableBoundsDescription b2;
    b2.boundsType = VariableBoundsDescription::LOWER;
    b2.lowerBound = -2;
    // setting bounds to an undeclared variable
    TFEL_TESTS_CHECK_THROW(bd.setPhysicalBounds(h, "D", b1), std::exception);
    // setting bounds on a
    bd.setPhysicalBounds(h, "A", b1);
    const auto& a = bd.getBehaviourData(h).getVariableDescription("A");
    TFEL_TESTS_ASSERT(a.hasPhysicalBounds());
    TFEL_TESTS_CHECK_THROW(a.hasPhysicalBounds(12), std::exception);
    // trying to override already declared bounds
    TFEL_TESTS_CHECK_THROW(bd.setPhysicalBounds(h, "A", b2), std::exception);
    // setting bounds on all components of b
    bd.setPhysicalBounds(h, "B", b1);
    const auto& b = bd.getBehaviourData(h).getVariableDescription("B");
    TFEL_TESTS_ASSERT(b.hasPhysicalBounds());
    for (unsigned short i = 0; i != b.arraySize; ++i) {
      TFEL_TESTS_ASSERT(b.hasPhysicalBounds(i));
    }
    // trying to override already declared bounds
    TFEL_TESTS_CHECK_THROW(bd.setPhysicalBounds(h, "B", 3, b2), std::exception);
    // setting bounds on some components of c
    bd.setPhysicalBounds(h, "C", 2, b1);
    bd.setPhysicalBounds(h, "C", 6, b2);
    const auto& c = bd.getBehaviourData(h).getVariableDescription("C");
    TFEL_TESTS_ASSERT(!c.hasPhysicalBounds());
    for (unsigned short i = 0; i != c.arraySize; ++i) {
      if ((i == 2u) || (i == 6u)) {
        TFEL_TESTS_ASSERT(c.hasPhysicalBounds(i));
      } else {
        TFEL_TESTS_ASSERT(!c.hasPhysicalBounds(i));
      }
    }
    TFEL_TESTS_CHECK_THROW(bd.setPhysicalBounds(h, "C", 2, b2), std::exception);
  }
  // bounds and physical bounds compatibility checks
  void test3() {
    using namespace mfront;
    using tfel::material::ModellingHypothesis;
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    BehaviourDescription bd;
    bd.addLocalVariable(h, VariableDescription("real", "A", 1u, 0u));
    VariableBoundsDescription b1;
    b1.boundsType = VariableBoundsDescription::LOWERANDUPPER;
    b1.lowerBound = -3;
    b1.upperBound = 3;
    VariableBoundsDescription b2;
    b2.boundsType = VariableBoundsDescription::LOWERANDUPPER;
    b2.lowerBound = -4;
    b2.upperBound = 3;
    VariableBoundsDescription b3;
    b3.boundsType = VariableBoundsDescription::LOWERANDUPPER;
    b3.lowerBound = -2;
    b3.upperBound = -4;
    bd.setPhysicalBounds(h, "A", b1);
    TFEL_TESTS_CHECK_THROW(bd.setBounds(h, "A", b2), std::exception);
    TFEL_TESTS_CHECK_THROW(bd.setBounds(h, "A", b3), std::exception);
  }
};

TFEL_TESTS_GENERATE_PROXY(BehaviourDescriptionBoundsHandlingTest,
                          "BehaviourDescriptionBoundsHandlingTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("BehaviourDescription.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
