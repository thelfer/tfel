/*!
 * \file  tests/Math/parser7.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 17 jun 2010
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

#include <iostream>

#include <cmath>
#include <limits>
#include <cstdlib>
#include <cassert>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"

#include "TFEL/Math/Evaluator.hxx"

template <double (*fct)(double)>
struct TestFunction final : public tfel::tests::Test {
  TestFunction(const std::string& f_,
               const double xa_,
               const double xb_,
               const double dx_,
               const double e_ = std::numeric_limits<double>::epsilon())
      : fn("Evaluator : " + f_), f(f_), xa(xa_), xb(xb_), dx(dx_), e(e_) {
    assert(xa < xb);
    assert(dx > 0);
    assert(e > 0);
  }  // end of TestFunction

  std::string name() const override { return fn; }

  std::string classname() const override { return "TFEL/Math"; }

  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    Evaluator ev(this->f + "(x)");
    double x;
    for (x = this->xa; x <= this->xb; x += this->dx) {
      ev.setVariableValue("x", x);
      if (std::abs(fct(x) - ev.getValue()) > e) {
        return false;
      }
    }
    return true;
  }  // end of execute

 private:
  const std::string fn;
  const std::string f;
  const double xa;
  const double xb;
  const double dx;
  const double e;

};  // end of struct TestFunction

#define TFEL_MATH_PARSER7_TESTFUNCTION(F, XA, XB, DX) \
  manager.addTest("parser7",                          \
                  std::make_shared<TestFunction<::F>>(#F, XA, XB, DX))

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("parser7.xml");
  TFEL_MATH_PARSER7_TESTFUNCTION(sin, -1., 1., 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(cos, -1., 1., 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(tan, -1., 1., 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(exp, -1., 1., 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(sinh, -1., 1., 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(cosh, -1., 1., 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(log, 0.1, 1.0, 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(log10, 0.1, 1.0, 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(asin, -0.5, 0.5, 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(acos, -0.5, 0.5, 0.1);
  TFEL_MATH_PARSER7_TESTFUNCTION(atan, -0.5, 0.5, 0.1);
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
