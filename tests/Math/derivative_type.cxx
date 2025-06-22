/*!
 * \file   tests/Math/derivative_type.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct DerivativeTypeTest final : public tfel::tests::TestCase {
  DerivativeTypeTest()
      : tfel::tests::TestCase("TFEL/Math", "DerivativeTypeTest") {
  }  // end of DerivativeTypeTest

  tfel::tests::TestResult execute() override {
    this->test1();
    return this->result;
  }  // end of execute

 private:
  //
  void test1() {
    using namespace tfel::math;
    using Types = tfel::config::Types<3u>;
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<derivative_type<Types::strain, Types::time>,
                        Types::strainrate>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<derivative_type<Types::StressStensor, Types::time>,
                        Types::StressRateStensor>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<derivative_type<Types::stress, Types::StrainStensor>,
                        Types::StressStensor>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<
            derivative_type<Types::StressStensor, Types::StrainStensor>,
            Types::StiffnessTensor>));
  }  // end of test1

};  // end of struct DerivativeTypeTest

TFEL_TESTS_GENERATE_PROXY(DerivativeTypeTest, "DerivativeTypeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DerivativeType.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
