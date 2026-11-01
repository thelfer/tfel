/*!
 * \file   integerparser.cxx
 * \brief
 * \author Thomas Helfer
 * \date 22/02/2016
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
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Math/IntegerEvaluator.hxx"

struct IntegerParserTest final : public tfel::tests::TestCase {
  IntegerParserTest()
      : tfel::tests::TestCase("TFEL/Math", "IntegerParserTest") {
  }  // end of IntegerParserTest
  tfel::tests::TestResult execute() override {
    this->check("x< 2 ? 3 : 1", {{1, 3}, {2, 1}, {5, 1}});
    this->check("x<=2 ? 3 : 1", {{1, 3}, {2, 3}, {5, 1}});
    this->check("x>=2 ? 3 : 1", {{1, 1}, {2, 3}, {5, 3}});
    this->check("x> 2 ? 3 : 1", {{1, 1}, {2, 1}, {5, 3}});
    this->check("x< 2 && x>-2 ? 3 : 1", {{0, 3}, {-3, 1}, {3, 1}});
    this->check("x> 2 || x<-2 ? 3 : 1", {{0, 1}, {-3, 3}, {3, 3}});
    this->check("x==0 ? 2 : 1", {{0, 2}, {-1, 1}, {1, 1}});
    TFEL_TESTS_ASSERT(tfel::math::IntegerEvaluator("2+-1").getValue() == 1);
    TFEL_TESTS_ASSERT(tfel::math::IntegerEvaluator("2*3/4").getValue() == 1);
    TFEL_TESTS_ASSERT(tfel::math::IntegerEvaluator("2*(2+1)/2").getValue() ==
                      3);
    TFEL_TESTS_CHECK_THROW(tfel::math::IntegerEvaluator("2/(1-1)").getValue(),
                           std::runtime_error);
    // integer formulas with variables
    auto ev = tfel::math::IntegerEvaluator("x*(y-1)/2");
    TFEL_TESTS_ASSERT(ev.getNumberOfVariables() == 2);
    ev.setVariableValue("x", 3);
    ev.setVariableValue("y", 4);
    TFEL_TESTS_ASSERT(ev.getValue() == 4);
    ev.setVariableValue("y", -2);
    TFEL_TESTS_ASSERT(ev.getValue() == -4);
    auto ev2 = tfel::math::IntegerEvaluator(std::vector<std::string>{"x", "y"},
                                            "x-2*y");
    ev2.setVariableValue(0, 1);
    ev2.setVariableValue(1, 3);
    TFEL_TESTS_ASSERT(ev2.getValue() == -5);
    TFEL_TESTS_CHECK_THROW(
        tfel::math::IntegerEvaluator(std::vector<std::string>{"x"}, "x+y"),
        std::runtime_error);
    return this->result;
  }  // end of execute
 private:
  void check(const std::string& f, const std::map<double, double>& values) {
    tfel::math::Evaluator ev(f);
    for (const auto v : values) {
      ev.setVariableValue("x", v.first);
      TFEL_TESTS_ASSERT(std::abs(ev.getValue() - v.second) < 1.e-14);
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(IntegerParserTest, "IntegerParserTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("IntegerParser.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
