/*!
 * \file   tests/Math/tensor/tensor_components.cxx
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

#include "TFEL/Math/tensor.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <unsigned short N>
struct TensorComponentsTest final : public tfel::tests::TestCase {
  TensorComponentsTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "TensorComponentsTest" + std::to_string(N)) {
  }  // end of TensorComponentsTest
  tfel::tests::TestResult execute() override {
    this->testGenerateIdComponent();
    return this->result;
  }  // end of execute
 private:
  void testGenerateIdComponent() {
    using namespace tfel::math;
    using size_type = typename tensor<N, double>::size_type;
    const auto id = tensor<N, double>::Id();
    for (size_type i = 0; i != TensorDimeToSize<N>::value; ++i) {
      TFEL_TESTS_ASSERT(
          std::abs(tensor<N, double>::generateIdComponent(i) - id(i)) <
          1.e-14);
    }
  }
};

using TensorComponentsTest1D = TensorComponentsTest<1u>;
using TensorComponentsTest2D = TensorComponentsTest<2u>;
using TensorComponentsTest3D = TensorComponentsTest<3u>;
TFEL_TESTS_GENERATE_PROXY(TensorComponentsTest1D, "TensorComponentsTest-1D");
TFEL_TESTS_GENERATE_PROXY(TensorComponentsTest2D, "TensorComponentsTest-2D");
TFEL_TESTS_GENERATE_PROXY(TensorComponentsTest3D, "TensorComponentsTest-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("tensorComponents.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
