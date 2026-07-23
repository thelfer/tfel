/*!
 * \file   tests/Math/tmatrix_components.cxx
 * \brief
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
#include <cassert>
#include <iostream>

#include "TFEL/Math/tmatrix.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <unsigned short N>
struct TMatrixComponentsTest final : public tfel::tests::TestCase {
  TMatrixComponentsTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "TMatrixComponentsTest" + std::to_string(N)) {
  }  // end of TMatrixComponentsTest
  tfel::tests::TestResult execute() override {
    this->testGenerateIdComponent();
    return this->result;
  }  // end of execute
 private:
  void testGenerateIdComponent() {
    using namespace tfel::math;
    using size_type = typename tmatrix<N, N, double>::size_type;
    const auto id = tmatrix<N, N, double>::Id();
    for (size_type i = 0; i != N; ++i) {
      for (size_type j = 0; j != N; ++j) {
        TFEL_TESTS_ASSERT(
            std::abs(tmatrix<N, N, double>::generateIdComponent(i, j) -
                     id(i, j)) < 1.e-14);
      }
    }
  }
};

using TMatrixComponentsTest2 = TMatrixComponentsTest<2u>;
using TMatrixComponentsTest3 = TMatrixComponentsTest<3u>;
using TMatrixComponentsTest6 = TMatrixComponentsTest<6u>;
TFEL_TESTS_GENERATE_PROXY(TMatrixComponentsTest2, "TMatrixComponentsTest-2");
TFEL_TESTS_GENERATE_PROXY(TMatrixComponentsTest3, "TMatrixComponentsTest-3");
TFEL_TESTS_GENERATE_PROXY(TMatrixComponentsTest6, "TMatrixComponentsTest-6");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("tmatrixComponents.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
