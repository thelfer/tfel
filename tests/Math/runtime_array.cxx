/*!
 * \file  runtime_array.cxx
 * \brief
 * \author Thomas Helfer
 * \date   07 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/runtime_array.hxx"
#include "TFEL/Math/Array/View.hxx"

struct RuntimeArrayTest final : public tfel::tests::TestCase {
  RuntimeArrayTest()
      : tfel::tests::TestCase("TFEL/Math", "RuntimeArrayTest") {
  }  // end of RuntimeArrayTest
  tfel::tests::TestResult execute() override {
    using usize_type = unsigned short;
    const tfel::math::runtime_array<int> a(3, 5);
    tfel::math::runtime_array<int> b(3);
    tfel::math::runtime_array<int> abc;
    TFEL_TESTS_ASSERT(a.size() == 3);
    TFEL_TESTS_ASSERT(a[0] == 5);
    TFEL_TESTS_ASSERT(a[1] == 5);
    TFEL_TESTS_ASSERT(a[2] == 5);
    // test of the assignement operator
    b = a;
    b[1] = 7;
    b[2] = 8;
    TFEL_TESTS_ASSERT(b.size() == 3);
    TFEL_TESTS_ASSERT(b[0] == 5);
    TFEL_TESTS_ASSERT(b[1] == 7);
    TFEL_TESTS_ASSERT(b[2] == 8);
    TFEL_TESTS_ASSERT(*(b.begin()) == 5);
    TFEL_TESTS_ASSERT(*(b.rbegin()) == 8);
    // test of the fill function
    b.fill(2);
    TFEL_TESTS_ASSERT(b[0] == 2);
    TFEL_TESTS_ASSERT(b[1] == 2);
    TFEL_TESTS_ASSERT(b[2] == 2);
    // test of the copy operator
    const auto c = a;
    TFEL_TESTS_ASSERT(c.size() == 3);
    TFEL_TESTS_ASSERT(c[0] == 5);
    TFEL_TESTS_ASSERT(c[1] == 5);
    TFEL_TESTS_ASSERT(c[2] == 5);
    // initialize from an std::initializer list
    tfel::math::runtime_array<int> d({1, 2, -3});
    TFEL_TESTS_ASSERT(d[0] == 1);
    TFEL_TESTS_ASSERT(d[1] == 2);
    TFEL_TESTS_ASSERT(d[2] == -3);
    // copy from another type
    tfel::math::runtime_array<usize_type> e(
        {usize_type{4}, usize_type{5}, usize_type{12}});
    d = e;
    TFEL_TESTS_ASSERT(d[0] == 4);
    TFEL_TESTS_ASSERT(d[1] == 5);
    TFEL_TESTS_ASSERT(d[2] == 12);
    // test of operator +=
    d += a;
    TFEL_TESTS_ASSERT(d[0] == 9);
    TFEL_TESTS_ASSERT(d[1] == 10);
    TFEL_TESTS_ASSERT(d[2] == 17);
    // creating a view of a preallocated memory
    int external_values[3] = {0, 2, 4};
    auto f =
        tfel::math::map<tfel::math::runtime_array<int>>(3, external_values);
    static_assert(
        std::is_same_v<decltype(f),
                       tfel::math::View<tfel::math::runtime_array<int>>>);
    TFEL_TESTS_ASSERT(f[0] == 0);
    TFEL_TESTS_ASSERT(f[1] == 2);
    TFEL_TESTS_ASSERT(f[2] == 4);
    //
    f -= d;
    TFEL_TESTS_ASSERT(f[0] == -9);
    TFEL_TESTS_ASSERT(f[1] == -8);
    TFEL_TESTS_ASSERT(f[2] == -13);
    const int const_external_values[3] = {1, -3, 9};
    auto g = tfel::math::map<tfel::math::runtime_array<int>>(
        3, const_external_values);
    static_assert(
        std::is_same_v<decltype(g),
                       tfel::math::ConstView<tfel::math::runtime_array<int>>>);
    TFEL_TESTS_ASSERT(g[0] == 1);
    TFEL_TESTS_ASSERT(g[1] == -3);
    TFEL_TESTS_ASSERT(g[2] == 9);
    f += g;
    TFEL_TESTS_ASSERT(f[0] == -8);
    TFEL_TESTS_ASSERT(f[1] == -11);
    TFEL_TESTS_ASSERT(f[2] == -4);
    // building a view from a pointer
    const auto p = static_cast<int*>(external_values);
    const auto h = tfel::math::map<tfel::math::runtime_array<int>>(3, p);
    static_assert(
        std::is_same_v<decltype(h),
                       const tfel::math::View<tfel::math::runtime_array<int>>>);
    TFEL_TESTS_ASSERT(h[0] == -8);
    TFEL_TESTS_ASSERT(h[1] == -11);
    TFEL_TESTS_ASSERT(h[2] == -4);
    // building a const view from a non const array
    const auto i = tfel::math::map<tfel::math::runtime_array<int>>(
        3, static_cast<const int*>(p));
    static_assert(std::is_same_v<
                  decltype(i),
                  const tfel::math::ConstView<tfel::math::runtime_array<int>>>);
    TFEL_TESTS_ASSERT(i[0] == -8);
    TFEL_TESTS_ASSERT(i[1] == -11);
    TFEL_TESTS_ASSERT(i[2] == -4);
    // end of the tests
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(RuntimeArrayTest, "RuntimeArrayTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("runtime_array.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
