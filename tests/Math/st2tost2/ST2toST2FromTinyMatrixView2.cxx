/*!
 * \file   tests/Math/st2tost2/ST2toST2FromTinyMatrixView2.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   17 oct 2008
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
#include <limits>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

struct ST2toST2FromTinyMatrixView2Test : public tfel::tests::TestCase {
  ST2toST2FromTinyMatrixView2Test()
      : tfel::tests::TestCase("TFEL/Math", "ST2toST2FromTinyMatrixView2") {
  }  // end of ST2toST2FromTinyMatrixView2

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    constexpr auto C_1_3 = double(1) / 3;
    const auto n = stensor<1>(2);
    auto m = tmatrix<6, 6>(0.);
    //
    map_derivative<0, 0, stensor<1>, stensor<1>>(m) = st2tost2<1>::Id();
    map_derivative<0, 3, stensor<1>, stensor<1>>(m) = st2tost2<1>::K();
    map_derivative<3, 0, stensor<1>, stensor<1>>(m) = st2tost2<1>::M();
    map_derivative<3, 3, stensor<1>, stensor<1>>(m) = n ^ n;
    // ms1
    TFEL_TESTS_ASSERT(std::abs(m(0, 0) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 1)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 2)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 0)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 1) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 2)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 0)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 0)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 2) - 1) < eps);
    // ms2
    TFEL_TESTS_ASSERT(std::abs(m(0, 3) - 2 * C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 4) + C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 5) + C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 3) + C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 4) - 2 * C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 5) + C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 3) + C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 4) + C_1_3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 5) - 2 * C_1_3) < eps);
    // ms3
    TFEL_TESTS_ASSERT(std::abs(m(3, 0) - 1.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(3, 1) + 0.5) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(3, 2) + 0.5) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(4, 0) + 0.5) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(4, 1) - 1.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(4, 2) + 0.5) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(5, 0) + 0.5) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(5, 0) + 0.5) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(5, 2) - 1.) < eps);
    // ms4
    TFEL_TESTS_ASSERT(std::abs(m(3, 3) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(3, 4) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(3, 5) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(4, 3) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(4, 4) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(4, 5) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(5, 3) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(5, 4) - 4.) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(5, 5) - 4.) < eps);
    return this->result;
  }  // end of execute

};  // end of ST2toST2FromTinyMatrixView2Test

TFEL_TESTS_GENERATE_PROXY(ST2toST2FromTinyMatrixView2Test,
                          "ST2toST2FromTinyMatrixView2Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ST2toST2FromTinyMatrixView2.xml");
  TestResult r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
