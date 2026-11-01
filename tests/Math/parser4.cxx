/*!
 * \file   tests/Math/parser4.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 20/12/2008
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

struct ParserTest4 final : public tfel::tests::TestCase {
  ParserTest4()
      : tfel::tests::TestCase("TFEL/Math", "ParserTest4") {
  }  // end of ParserTest4
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using namespace tfel::math::parser;
    vector<string> var(1, "x");
    vector<string> nvar(1, "b");
    set<string> n;
    set<string> n2;
    auto manager = std::make_shared<ExternalFunctionManager>();
    manager->operator[]("a") = std::make_shared<Evaluator>("12", manager);
    auto f = std::make_shared<Evaluator>(var, "2*exp(a*x)", manager);
    auto g = std::make_shared<Evaluator>(var, "2*b*exp(a*x)", manager);
    auto h = g->createFunctionByChangingParametersIntoVariables(nvar);
    TFEL_TESTS_ASSERT(h->getNumberOfVariables() == 2);
    h->setVariableValue(0, 1.5);
    h->setVariableValue(1, 2.54);
    const auto v = 2 * 2.54 * exp(12 * 1.5);
    TFEL_TESTS_ASSERT(abs(h->getValue() - v) < 1.e-14 * v);
    f->getParametersNames(n);
    g->getParametersNames(n2);
    TFEL_TESTS_ASSERT(n == set<string>{"a"});
    TFEL_TESTS_ASSERT((n2 == set<string>{"a", "b"}));
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ParserTest4, "ParserTest4");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("parser4.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
