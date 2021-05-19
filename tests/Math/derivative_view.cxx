/*!
 * \file   tests/Math/derivative_view.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/06/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/Array/FixedSizeArrayDerivative.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

namespace tfel::math {
}  // end of namespace tfel::math

struct DerivativeViewTest final : public tfel::tests::TestCase {
  DerivativeViewTest()
      : tfel::tests::TestCase("TFEL/Math", "DerivativeViewTest") {
  }  // end of DerivativeViewTest

  tfel::tests::TestResult execute() override {
    this->test1();
    return this->result;
  }  // end of execute

 private:
  //
  void test1() {}

};  // end of struct DerivativeViewTest

TFEL_TESTS_GENERATE_PROXY(DerivativeViewTest, "DerivativeViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DerivativeView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
