/*!
 * \file   ConvertLogarithmicStrainTangentOperatorTest.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   29 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Material/Lame.hxx"
#include "TFEL/Math/ST2toST2/ConvertLogarithmicStrainTangentOperator.hxx"

struct ConvertLogarithmicStrainTangentOperatorTest
    : public tfel::tests::TestCase {
  ConvertLogarithmicStrainTangentOperatorTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "ConvertLogarithmicStrainTangentOperator") {
  }  // end of ConvertLogarithmicStrainTangentOperatorTest

  tfel::tests::TestResult execute() {
    //    this->test<1>();
    // this->test<2>();
    // this->test<3>();
    return this->result;
  }  // end of execute
 private:
};  // end of ConvertLogarithmicStrainTangentOperatorTest

TFEL_TESTS_GENERATE_PROXY(ConvertLogarithmicStrainTangentOperatorTest,
                          "ConvertLogarithmicStrainTangentOperatorTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ConvertLogarithmicStrainTangentOperator.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
