/*!
 * \file   parser11.cxx
 * \brief
 * \author Thomas Helfer
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

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Evaluator.hxx"

struct ParserTest11 final : public tfel::tests::TestCase {
  ParserTest11()
      : tfel::tests::TestCase("TFEL/Math", "ParserTest11") {
  }  // end of ParserTest11
  tfel::tests::TestResult execute() override {
    this->check("12", "12");
    for (const std::string f :
         {"H", "exp", "log", "log10", "ln", "cos", "sin", "tan", "acos", "asin",
          "atan", "cosh", "sinh", "tanh"}) {
      this->check(f + "(x)", f + "(x)");
    }
    for (const std::string f : {"min", "max"}) {
      this->check(f + "(x,y)", f + "(x,y)");
    }
    this->check("power<0>(x)", "1");
    this->check("power<1>(x)", "x");
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    for (int i = 2; i != 16; ++i) {
      const auto p = "power<" + std::to_string(i) + ">";
      this->check(p + "(x)", "tfel::math::" + p + "(x)");
    }
#endif /* !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    this->check("x ** 17", "std::pow(x,17)");
    for (const std::string op : {"+", "-", "*", "/"}) {
      this->check("a" + op + "b", "(a)" + op + "(b)");
    }
    this->check("a**b", "std::pow(a,b)");
    this->check("-a", "-(a)");
    this->check("x>y?2*y:x-1",
                "((x)>(y)) ? "
                "((2)*(y)) : ((x)-(1))");
    return this->result;
  }  // end of execute
 private:
  void check(const std::string& f, const std::string& r) {
    tfel::math::Evaluator ev(f);
    TFEL_TESTS_ASSERT(ev.getCxxFormula() == r);
  }
};

TFEL_TESTS_GENERATE_PROXY(ParserTest11, "ParserTest11");

/* coverity[UNCAUGHT_EXCEPT] */
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Parser11.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
