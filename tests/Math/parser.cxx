/*!
 * \file   tests/Math/parser.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 21/09/2007
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

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Evaluator.hxx"

struct ParserTest final : public tfel::tests::TestCase {
  ParserTest()
      : tfel::tests::TestCase("TFEL/Math", "ParserTest") {
  }  // end of ParserTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    Evaluator ev("min(x,y)*x");
    ev.setVariableValue("x", 1.5);
    ev.setVariableValue("y", 2.54);
    TFEL_TESTS_ASSERT(std::abs(ev.getValue() - 2.25) < 1.e-14);
    ev.setVariableValue("x", 3.);
    TFEL_TESTS_ASSERT(std::abs(ev.getValue() - 7.62) < 1.e-14);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ParserTest, "ParserTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("parser.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
