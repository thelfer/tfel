/*!
 * \file   StridedRandomAccessIteratorTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 avril 2016
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
#include <array>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/General/StridedRandomAccessIterator.hxx"

struct StridedRandomAccessIteratorTest final : public tfel::tests::TestCase {
  StridedRandomAccessIteratorTest()
      : tfel::tests::TestCase("TFEL/Math", "StridedRandomAccessIteratorTest") {
  }  // end of StridedRandomAccessIteratorTest
  virtual tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    TFEL_CONSTEXPR const double eps = std::numeric_limits<double>::epsilon();
    using array = std::array<double, 12>;
    using iterator =
        tfel::math::StridedRandomAccessIterator<array::const_iterator>;
    const array values = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
    auto b = iterator(values.begin(), 3);
    auto b2 = iterator(values.begin() + 1, 4);
    const auto e = iterator(values.end(), 3);
    TFEL_TESTS_ASSERT(e - b == 4);
    TFEL_TESTS_ASSERT(std::abs(b[0] - 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(b[1] - 3) < eps);
    TFEL_TESTS_ASSERT(std::abs(b[2] - 6) < eps);
    TFEL_TESTS_ASSERT(std::abs(b[3] - 9) < eps);
    TFEL_TESTS_ASSERT(std::abs(b2[0] - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(b2[1] - 5) < eps);
    TFEL_TESTS_ASSERT(std::abs(b2[2] - 9) < eps);
    auto v = std::array<double, 4>();
    std::copy(b, e, v.begin());
    TFEL_TESTS_ASSERT(std::abs(v[0] - 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(v[1] - 3) < eps);
    TFEL_TESTS_ASSERT(std::abs(v[2] - 6) < eps);
    TFEL_TESTS_ASSERT(std::abs(v[3] - 9) < eps);
  }
  void test2() {
    TFEL_CONSTEXPR const double eps = std::numeric_limits<double>::epsilon();
    using array = std::array<double, 12>;
    using iterator = tfel::math::StridedRandomAccessIterator<array::iterator>;
    array values = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
    auto b = iterator(values.begin(), 3);
    const auto e = iterator(values.end(), 3);
    std::fill(b, e, 0);
    TFEL_TESTS_ASSERT(std::abs(values[0] - 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[1] - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[2] - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[3] - 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[4] - 4) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[5] - 5) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[6] - 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[7] - 7) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[8] - 8) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[9] - 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[10] - 10) < eps);
    TFEL_TESTS_ASSERT(std::abs(values[11] - 11) < eps);
  }
};

TFEL_TESTS_GENERATE_PROXY(StridedRandomAccessIteratorTest,
                          "StridedRandomAccessIteratorTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StridedRandomAccessIteratorTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
