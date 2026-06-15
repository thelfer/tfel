/*!
 * \file   tests/Math/st2tost2/ST2toST2FromTinyMatrixView.cxx
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

#include <cassert>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

struct ST2toST2FromTinyMatrixViewTest final : public tfel::tests::TestCase {
  ST2toST2FromTinyMatrixViewTest()
      : tfel::tests::TestCase("TFEL/Math", "ST2toST2FromTinyMatrixView") {
  }  // end of ST2toST2FromTinyMatrixViewTest

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    using size_type = unsigned short;
    auto m = tmatrix<6, 6, int>::Id();
    auto ms = map_derivative<0, 0, stensor<1, int>, stensor<1, int>>(m);
    auto ms2 = map_derivative<3, 3, stensor<1, int>, stensor<1, int>>(m);
    for (size_type i = 0; i != 3; ++i) {
      for (size_type j = 0; j != 3; ++j) {
        if (i == j) {
          TFEL_TESTS_ASSERT(ms(i, i) == 1);
          TFEL_TESTS_ASSERT(ms2(i, i) == 1);
        } else {
          TFEL_TESTS_ASSERT(ms(i, j) == 0);
          TFEL_TESTS_ASSERT(ms2(i, i) == 1);
        }
      }
    }
    return this->result;
  }  // end of execute
};   // end of ST2toST2FromTinyMatrixViewTest

TFEL_TESTS_GENERATE_PROXY(ST2toST2FromTinyMatrixViewTest,
                          "ST2toST2FromTinyMatrixViewTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ST2toST2FromTinyMatrixView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
