/*!
 * \file   integerparser.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   22 févr. 2016
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

#include "TFEL/Math/Evaluator.hxx"

struct IntegerParserTest final : public tfel::tests::TestCase {
  IntegerParserTest()
      : tfel::tests::TestCase("TFEL/Math", "IntegerParserTest") {
  }  // end of IntegerParserTest
  virtual tfel::tests::TestResult execute() override {
    this->check("x< 2 ? 3 : 1", {{1, 3}, {2, 1}, {5, 1}});
    this->check("x<=2 ? 3 : 1", {{1, 3}, {2, 3}, {5, 1}});
    this->check("x>=2 ? 3 : 1", {{1, 1}, {2, 3}, {5, 3}});
    this->check("x> 2 ? 3 : 1", {{1, 1}, {2, 1}, {5, 3}});
    this->check("x< 2 && x>-2 ? 3 : 1", {{0, 3}, {-3, 1}, {3, 1}});
    this->check("x> 2 || x<-2 ? 3 : 1", {{0, 1}, {-3, 3}, {3, 3}});
    this->check("x==0 ? 2 : 1", {{0, 2}, {-1, 1}, {1, 1}});
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
