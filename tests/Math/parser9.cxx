/*!
 * \file   tests/Math/parser9.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 sep 2007
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

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Evaluator.hxx"

struct ParserTest final : public tfel::tests::TestCase {
  ParserTest()
      : tfel::tests::TestCase("TFEL/Math", "ParserTest") {
  }  // end of ParserTest
  virtual tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    using namespace tfel::math::parser;
    std::vector<std::string> var(1, "x");
    auto m = std::make_shared<ExternalFunctionManager>();
    m->operator[]("u") = std::make_shared<Evaluator>(var, "cos(x)", m);
    m->operator[]("g") = std::make_shared<Evaluator>(var, "f(x)", m);
    m->operator[]("f") = std::make_shared<Evaluator>(var, "f(x)", m);
    TFEL_TESTS_CHECK_THROW(m->operator[]("f")->checkCyclicDependency("f"),
                           std::exception);
    m->operator[]("f") = std::make_shared<Evaluator>(var, "g(x)", m);
    TFEL_TESTS_CHECK_THROW(m->operator[]("f")->checkCyclicDependency("f"),
                           std::exception);
    m->operator[]("f") = std::make_shared<Evaluator>(var, "g(f(x))", m);
    TFEL_TESTS_CHECK_THROW(m->operator[]("f")->checkCyclicDependency("f"),
                           std::exception);
    m->operator[]("f") = std::make_shared<Evaluator>(var, "g(u(g(x)))", m);
    TFEL_TESTS_CHECK_THROW(m->operator[]("f")->checkCyclicDependency("f"),
                           std::exception);
    m->operator[]("f") = std::make_shared<Evaluator>(var, "g(u(f(x)))", m);
    TFEL_TESTS_CHECK_THROW(m->operator[]("f")->checkCyclicDependency("f"),
                           std::exception);
    m->operator[]("f") = std::make_shared<Evaluator>(var, "g(f(x)+1)", m);
    m->operator[]("g") = std::make_shared<Evaluator>(var, "sinh(x)", m);
    TFEL_TESTS_CHECK_THROW(m->operator[]("f")->checkCyclicDependency("f"),
                           std::exception);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ParserTest, "ParserTest9");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Parser9.xml");
  auto r = m.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
