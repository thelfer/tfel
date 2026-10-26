/*!
 * \file  tests/Math/division_by_scalar.cxx
 * \brief This file checks the in-place division of arrays and views by an
 * integer
 * \author Tristan Chenaille
 * \date 25/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Array/ViewsArray.hxx"
#include "TFEL/Math/Array/StridedCoalescedViewsArray.hxx"

struct DivisionByScalarTest final : public tfel::tests::TestCase {
  DivisionByScalarTest()
      : tfel::tests::TestCase("TFEL/Math", "DivisionByScalarTest") {
  }  // end of DivisionByScalarTest

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    constexpr auto eps = 1e-14;
    // fixed size array
    auto s = stensor<3u, double>{2, 4, 6, 8, 10, 12};
    s /= 2;
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(std::abs(s[i] - (i + 1)) < eps);
    }
    // fixed size array of quantities
    auto q = stensor<3u, qt<unit::Stress>>{
        qt<unit::Stress>(2), qt<unit::Stress>(4),  qt<unit::Stress>(6),
        qt<unit::Stress>(8), qt<unit::Stress>(10), qt<unit::Stress>(12)};
    q /= 2;
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(std::abs(q[i].getValue() - (i + 1)) < eps);
    }
    // runtime array
    auto v = vector<double>{2, 4, 6};
    v /= 2;
    for (unsigned short i = 0; i != 3; ++i) {
      TFEL_TESTS_ASSERT(std::abs(v[i] - (i + 1)) < eps);
    }
    // view
    auto values = std::array<double, 6>{2, 4, 6, 8, 10, 12};
    auto m = map<stensor<3u, double>>(values.data());
    m /= 2;
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(std::abs(values[i] - (i + 1)) < eps);
    }
    // array of views
    auto values2 = std::array<double, 3>{2, 4, 6};
    auto a = ViewsFixedSizeVector<double, unsigned short, 3>(values2.data());
    a /= 2;
    for (unsigned short i = 0; i != 3; ++i) {
      TFEL_TESTS_ASSERT(std::abs(values2[i] - (i + 1)) < eps);
    }
    // array of strided coalesced views
    auto values3 = std::array<double, 3>{2, 4, 6};
    auto a2 = StridedCoalescedViewsFixedSizeVector<double, unsigned int, 3>(
        values3.data());
    a2 /= 2;
    for (unsigned short i = 0; i != 3; ++i) {
      TFEL_TESTS_ASSERT(std::abs(values3[i] - (i + 1)) < eps);
    }
    // array of views of symmetric tensors
    auto values4 = std::array<double, 8>{2, 4, 6, 8, 10, 12, 14, 16};
    auto a3 = ViewsFixedSizeVector<stensor<2u, double>, unsigned short, 2>(
        values4.data());
    a3 /= 2;
    for (unsigned short i = 0; i != 8; ++i) {
      TFEL_TESTS_ASSERT(std::abs(values4[i] - (i + 1)) < eps);
    }
    // array of strided coalesced views of symmetric tensors
    auto values5 = std::array<double, 8>{2, 4, 6, 8, 10, 12, 14, 16};
    auto a4 =
        StridedCoalescedViewsFixedSizeVector<stensor<2u, double>, unsigned int,
                                             2>(values5.data());
    a4 /= 2;
    for (unsigned short i = 0; i != 8; ++i) {
      TFEL_TESTS_ASSERT(std::abs(values5[i] - (i + 1)) < eps);
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(DivisionByScalarTest, "DivisionByScalarTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("division_by_scalar.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
