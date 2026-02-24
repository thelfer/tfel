/*!
 * \file   tests/Math/tmatrix7.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/03/2024
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tmatrix.hxx"

struct TMatrixProductTest final : public tfel::tests::TestCase {
  TMatrixProductTest()
      : tfel::tests::TestCase("TFEL/Math", "TMatrixProductTest") {
  }  // end of TMatrixProductTest

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    static constexpr auto m1 = tmatrix<2u, 2u, int>{0, 1,  //
                                                    2, 3};
    static constexpr auto m2 = tmatrix<2u, 2u, int>{4, -1,  //
                                                    5, 2};
    constexpr auto m3 = m1 * m2;
    TFEL_TESTS_STATIC_ASSERT(m3(0, 0) == 5);
    TFEL_TESTS_STATIC_ASSERT(m3(0, 1) == 2);
    TFEL_TESTS_STATIC_ASSERT(m3(1, 0) == 23);
    TFEL_TESTS_STATIC_ASSERT(m3(1, 1) == 4);
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(TMatrixProductTest, "TMatrixProductTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TMatrixSubMatrixView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
