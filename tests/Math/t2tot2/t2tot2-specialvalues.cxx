/*!
 * \file   t2tot2-specialvalues.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   16 juin 2016
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
#include "TFEL/Math/t2tot2.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <unsigned short N>
struct T2tot2SpecialValuesTest final : public tfel::tests::TestCase {
  T2tot2SpecialValuesTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "T2tot2SpecialValuesTest" + std::to_string(N)) {
  }  // end of T2tot2SpecialValuesTest
  virtual tfel::tests::TestResult execute() override {
    this->testId();
    this->testIdxId();
    this->testIdxId2();
    this->testK();
    this->testK2();
    return this->result;
  }  // end of execute
 private:
  void testId() {
    using namespace tfel::math;
    using size_type = typename t2tot2<N, double>::size_type;
    const auto id = t2tot2<N, double>::Id();
    for (size_type i = 0; i != TensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != TensorDimeToSize<N>::value; ++j) {
        const double v = (i == j) ? 1. : 0.;
        TFEL_TESTS_ASSERT(std::abs(id(i, j) - v) < 1.e-14);
      }
    }
  }
  void testIdxId() {
    using namespace tfel::math;
    using size_type = typename t2tot2<N, double>::size_type;
    const auto IdxId = t2tot2<N, double>::IxI();
    for (size_type i = 0; i != TensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != TensorDimeToSize<N>::value; ++j) {
        const double v = ((i < 3) && (j < 3)) ? 1. : 0.;
        TFEL_TESTS_ASSERT(std::abs(IdxId(i, j) - v) < 1.e-14);
      }
    }
  }
  void testIdxId2() {
    using namespace tfel::math;
    using size_type = typename t2tot2<N, double>::size_type;
    const auto IdxId = t2tot2<N, double>::IxI();
    const auto IdxId2 = tensor<N, double>::Id() ^ tensor<N, double>::Id();
    for (size_type i = 0; i != TensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != TensorDimeToSize<N>::value; ++j) {
        TFEL_TESTS_ASSERT(std::abs(IdxId(i, j) - IdxId2(i, j)) < 1.e-14);
      }
    }
  }
  void testK() {
    using namespace tfel::math;
    using size_type = typename t2tot2<N, double>::size_type;
    const auto k = t2tot2<N, double>::K();
    for (size_type i = 0; i != TensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != TensorDimeToSize<N>::value; ++j) {
        const double v = [i, j]() -> double {
          if ((i < 3) && (j < 3)) {
            return ((i == j) ? 2. : -1.) / 3.;
          }
          return i == j ? 1 : 0;
        }();
        TFEL_TESTS_ASSERT(std::abs(k(i, j) - v) < 1.e-14);
      }
    }
  }
  void testK2() {
    using namespace tfel::math;
    using size_type = typename t2tot2<N, double>::size_type;
    const auto k = t2tot2<N, double>::K();
    const auto k2 = t2tot2<N, double>::Id() - t2tot2<N, double>::IxI() / 3;
    for (size_type i = 0; i != TensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != TensorDimeToSize<N>::value; ++j) {
        TFEL_TESTS_ASSERT(std::abs(k(i, j) - k2(i, j)) < 1.e-14);
      }
    }
  }
};

using T2tot2SpecialValuesTest1D = T2tot2SpecialValuesTest<1u>;
using T2tot2SpecialValuesTest2D = T2tot2SpecialValuesTest<2u>;
using T2tot2SpecialValuesTest3D = T2tot2SpecialValuesTest<3u>;
TFEL_TESTS_GENERATE_PROXY(T2tot2SpecialValuesTest1D,
                          "T2tot2SpecialValuesTest-1D");
TFEL_TESTS_GENERATE_PROXY(T2tot2SpecialValuesTest2D,
                          "T2tot2SpecialValuesTest-2D");
TFEL_TESTS_GENERATE_PROXY(T2tot2SpecialValuesTest3D,
                          "T2tot2SpecialValuesTest-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("t2tot2-specialvalues.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
