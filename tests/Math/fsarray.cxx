/*!
 * \file  fsarray.cxx
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
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/Array/View.hxx"

template <int N>
struct structify {
  static constexpr int value = N;
};

struct FSArrayTest final : public tfel::tests::TestCase {
  FSArrayTest()
      : tfel::tests::TestCase("TFEL/Math", "FSArrayTest") {
  }  // end of FSArrayTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace std;
    using namespace tfel::math;
    constexpr fsarray<3u, int> a1(0);
    constexpr fsarray<3u, int> a2{a1};
    constexpr fsarray<3u, int> a3({3, 2, 6});
    constexpr fsarray<3u, int> a4{a3};
    constexpr fsarray<3u, int> a5 = {5, 3, 9};
    constexpr fsarray<3u, int> a6{a5};
    TFEL_TESTS_STATIC_ASSERT(a1[0] == 0);
    TFEL_TESTS_STATIC_ASSERT(a1[1] == 0);
    TFEL_TESTS_STATIC_ASSERT(a1[2] == 0);
    TFEL_TESTS_STATIC_ASSERT(a2[0] == 0);
    TFEL_TESTS_STATIC_ASSERT(a2[1] == 0);
    TFEL_TESTS_STATIC_ASSERT(a2[2] == 0);
    TFEL_TESTS_STATIC_ASSERT(a3[0] == 3);
    TFEL_TESTS_STATIC_ASSERT(a3[1] == 2);
    TFEL_TESTS_STATIC_ASSERT(a3[2] == 6);
    TFEL_TESTS_STATIC_ASSERT(a4[0] == 3);
    TFEL_TESTS_STATIC_ASSERT(a4[1] == 2);
    TFEL_TESTS_STATIC_ASSERT(a4[2] == 6);
    TFEL_TESTS_STATIC_ASSERT(a5[0] == 5);
    TFEL_TESTS_STATIC_ASSERT(a5[1] == 3);
    TFEL_TESTS_STATIC_ASSERT(a5[2] == 9);
    TFEL_TESTS_STATIC_ASSERT(a6[0] == 5);
    TFEL_TESTS_STATIC_ASSERT(a6[1] == 3);
    TFEL_TESTS_STATIC_ASSERT(a6[2] == 9);
    TFEL_TESTS_STATIC_ASSERT(*(a5.begin() + 0) == 5);
    TFEL_TESTS_STATIC_ASSERT(*(a5.begin() + 1) == 3);
    TFEL_TESTS_STATIC_ASSERT(*(a5.begin() + 2) == 9);
    TFEL_TESTS_STATIC_ASSERT(*(a5.rbegin() + 0) == 9);
    TFEL_TESTS_STATIC_ASSERT(*(a5.rbegin() + 1) == 3);
    TFEL_TESTS_STATIC_ASSERT(*(a5.rbegin() + 2) == 5);
  }  // end of test1
  void test2() {
    using namespace tfel::math;
    using usint = unsigned short;
    constexpr fsarray<3u, unsigned short> a1{{usint(2), usint(3), usint(4)}};
    constexpr fsarray<3u, int> a2{a1};
    TFEL_TESTS_STATIC_ASSERT(a1[0] == 2);
    TFEL_TESTS_STATIC_ASSERT(a1[1] == 3);
    TFEL_TESTS_STATIC_ASSERT(a1[2] == 4);
    TFEL_TESTS_STATIC_ASSERT(a2[0] == 2);
    TFEL_TESTS_STATIC_ASSERT(a2[1] == 3);
    TFEL_TESTS_STATIC_ASSERT(a2[2] == 4);
  }  // end of test2
  void test3() {
    int values[] = {0, 2, 0};
    const int values2[] = {0, 2, -2};
    auto v = tfel::math::map<tfel::math::fsarray<3u, int>>(values);
    const auto v2 = tfel::math::map<tfel::math::fsarray<3u, int>>(values2);
    TFEL_TESTS_ASSERT(v[0] == 0);
    TFEL_TESTS_ASSERT(v[1] == 2);
    TFEL_TESTS_ASSERT(v[2] == 0);
    v = {1, 2, 3};
    TFEL_TESTS_ASSERT(v[0] == 1);
    TFEL_TESTS_ASSERT(v[1] == 2);
    TFEL_TESTS_ASSERT(v[2] == 3);
    v -= v2;
    TFEL_TESTS_ASSERT(v[0] == 1);
    TFEL_TESTS_ASSERT(v[1] == 0);
    TFEL_TESTS_ASSERT(v[2] == 5);
  }
  void test4() {
    constexpr auto eps = double(1.e-14);
    const tfel::math::fsarray<3u, double> v1{1.2, -4.2, -0.3};
    const auto [a, b, c] = v1;
    TFEL_TESTS_ASSERT(std::abs(a - 1.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(b + 4.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(c + 0.3) < eps);
  }  // end of test4
  void test5() {
    constexpr auto eps = double(1.e-14);
    tfel::math::fsarray<3u, double> v1(0);
    auto& [a, b, c] = v1;
    a = 1.2;
    b = -4.2;
    c = -0.3;
    TFEL_TESTS_ASSERT(std::abs(v1[0] - 1.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(v1[1] + 4.2) < eps);
    TFEL_TESTS_ASSERT(std::abs(v1[2] + 0.3) < eps);
  }  // end of test5
};

TFEL_TESTS_GENERATE_PROXY(FSArrayTest, "FSArrayTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("fsarray.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
