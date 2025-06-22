/*!
 * \file   DifferenceRandomAccessIteratorTest.cxx
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
#include "TFEL/Math/General/DifferenceRandomAccessIterator.hxx"

struct DifferenceRandomAccessIteratorTest final : public tfel::tests::TestCase {
  DifferenceRandomAccessIteratorTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "DifferenceRandomAccessIteratorTest") {
  }  // end of DifferenceRandomAccessIteratorTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    constexpr const double eps = std::numeric_limits<double>::epsilon();
    using array = std::array<double, 12>;
    using iterator =
        tfel::math::DifferenceRandomAccessIterator<array::const_iterator>;
    const array values = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
    auto b = iterator(values.begin(), values.begin());
    const auto e = iterator(values.end(), values.end());
    while (b != e) {
      TFEL_TESTS_ASSERT(std::abs(*b) < eps);
      ++b;
    }
  }
  void test2() {
    constexpr const double eps = std::numeric_limits<double>::epsilon();
    using array = std::array<double, 12>;
    using iterator =
        tfel::math::DifferenceRandomAccessIterator<array::const_iterator>;
    const array v = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
    const array v2 = {{9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7, 8}};
    auto b = iterator(v.begin(), v2.begin());
    array::size_type i = 0;
    while (i != v.size()) {
      TFEL_TESTS_ASSERT(std::abs(b[i] - (v[i] - v2[i])) < eps);
      ++i;
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(DifferenceRandomAccessIteratorTest,
                          "DifferenceRandomAccessIteratorTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DifferenceRandomAccessIteratorTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
