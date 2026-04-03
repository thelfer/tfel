/*!
 * \file   tests/Math/parser12.cxx
 * \brief
 * \author Thomas Helfer
 * \date   14/04/2022
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

struct ParserTest12 final : public tfel::tests::TestCase {
  ParserTest12()
      : tfel::tests::TestCase("TFEL/Math", "ParserTest12") {
  }  // end of ParserTest12
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    constexpr auto x = double{1.1};
    constexpr auto eps = double{1e-14};
    //
    auto e0 = tfel::math::Evaluator("x**0");
    e0.setVariableValue("x", x);
    TFEL_TESTS_ASSERT(std::abs(e0.getValue() - 1) < eps);
    TFEL_TESTS_ASSERT(e0.getCxxFormula() == "1");
    //
    for (auto n = 1; n != 20; ++n) {
      auto e = tfel::math::Evaluator("x**" + std::to_string(n));
      auto ie = tfel::math::Evaluator("x**-" + std::to_string(n));
      e.setVariableValue("x", x);
      ie.setVariableValue("x", x);
      TFEL_TESTS_ASSERT(std::abs(e.getValue() - std::pow(x, n)) < eps);
      TFEL_TESTS_ASSERT(std::abs(ie.getValue() - std::pow(x, -n)) < eps);
      const auto ef = [n]() -> std::string {
        if (n == 1) {
          return "x";
        }
        if (n <= 16) {
          return "tfel::math::power<" + std::to_string(n) + ">(x)";
        }
        return "std::pow(x," + std::to_string(n) + ")";
      }();
      const auto ief = [n] {
        if (n <= 16) {
          return "tfel::math::power<-" + std::to_string(n) + ">(x)";
        }
        return "std::pow(x,-(" + std::to_string(n) + "))";
      }();
      TFEL_TESTS_ASSERT(e.getCxxFormula() == ef);
      TFEL_TESTS_ASSERT(ie.getCxxFormula() == ief);
    }
    //
    auto e1 = tfel::math::Evaluator("x**(2+3)");
    e1.setVariableValue("x", x);
    TFEL_TESTS_ASSERT(std::abs(e1.getValue() - std::pow(x, 5)) < eps);
    TFEL_TESTS_ASSERT(e1.getCxxFormula() == "tfel::math::power<5>(x)");
    //
    auto e2 = tfel::math::Evaluator("x**(2.+3)");
    e2.setVariableValue("x", x);
    TFEL_TESTS_ASSERT(std::abs(e2.getValue() - std::pow(x, 5)) < eps);
    TFEL_TESTS_ASSERT(e2.getCxxFormula() == "tfel::math::power<5>(x)");
  }
  void test2() {
    auto check = [this](const std::string& f, const std::string& df_dx) {
      const auto e = tfel::math::Evaluator(f);
      const auto dx =
          std::dynamic_pointer_cast<tfel::math::Evaluator>(e.differentiate("x"))
              ->getCxxFormula();
      TFEL_TESTS_ASSERT(dx == df_dx);
    };
    check("x**2", "(2)*(x)");
    check("x**3", "(3)*(tfel::math::power<2>(x))");
    check("x**-2", "(-2)*(tfel::math::power<-3>(x))");
    check("power<0>(x)", "0");
    check("power<1>(x)", "1");
    check("power<2>(x)", "(2)*(x)");
    check("power<-2>(x)", "(-2)*(tfel::math::power<-3>(x))");
  }  // end of test2
  void test3() {
    auto check = [this](const std::string& f, const std::string& df_dx,
                        const std::string& df_dy) {
      const auto e = tfel::math::Evaluator(f);
      const auto dx =
          std::dynamic_pointer_cast<tfel::math::Evaluator>(e.differentiate("x"))
              ->getCxxFormula();
      const auto dy =
          std::dynamic_pointer_cast<tfel::math::Evaluator>(e.differentiate("y"))
              ->getCxxFormula();
      TFEL_TESTS_ASSERT(dx == df_dx);
      TFEL_TESTS_ASSERT(dy == df_dy);
    };
    check("2 * x + y", "(2)*(1)", "1");
    check("x**2 * y", "((2)*(x))*(y)", "(tfel::math::power<2>(x))*(1)");
    check("exp(x)+y", "exp(x)", "1");
    check("x <0 ?  y : 2*y", "0", "((x)<(0)) ? (1) : ((2)*(1))");
  }  // end of test3
  void test4() {
    const auto e = tfel::math::Evaluator("x**5.4");
    const auto dx =
        std::dynamic_pointer_cast<tfel::math::Evaluator>(e.differentiate("x"))
            ->getCxxFormula();
    const auto f = "(5.4)*(std::pow(x," + std::to_string(4.4) + "))";
    TFEL_TESTS_ASSERT(dx == f);
  }  // end of test4
  void test5() {
    const auto e = tfel::math::Evaluator("2**x");
    const auto dx =
        std::dynamic_pointer_cast<tfel::math::Evaluator>(e.differentiate("x"))
            ->getCxxFormula();
    const auto f = "(log(2))*(std::pow(2,x))";
    TFEL_TESTS_ASSERT(dx == f);
  }  // end of test5
};

TFEL_TESTS_GENERATE_PROXY(ParserTest12, "ParserTest12");

/* coverity[UNCAUGHT_EXCEPT] */
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Parser12.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
