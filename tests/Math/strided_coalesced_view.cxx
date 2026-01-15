/*!
 * \file   strided_coalesced_view.cxx
 * \brief
 * \author Tristan Chenaille
 * \date   15/01/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/Array/StridedCoalescedView.hxx"
#include "TFEL/Math/Array/StridedCoalescedViewsArray.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"

struct StensorStridedCoalescedViewTest final : public tfel::tests::TestCase {
  StensorStridedCoalescedViewTest()
      : tfel::tests::TestCase("TFEL/Math", "StensorStridedCoalescedViewTest") {
  }  // end of StensorStridedCoalescedViewTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test2b();
    this->test3();
    this->test4();
    this->test5();
    this->test6();
    this->test7();
    this->test8();
    this->test9();
    this->test10();
    this->test11();
    return this->result;
  }  // end of execute
 private:
  //! \brief criterion value
  static constexpr auto eps = 1e-14;
  void test1() {
    using namespace tfel::math;
    // SoA layout: [s0_c0, s1_c0, s0_c1, s1_c1, s0_c2, s1_c2, s0_c3, s1_c3]
    int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
    auto s1 = map_strided<stensor<2u, int>>(&values[0], 2);
    const auto s2 = map_strided<const stensor<2u, int>>(&values[1], 2);
    TFEL_TESTS_ASSERT(s1[0] == 1);
    TFEL_TESTS_ASSERT(s1[1] == 2);
    TFEL_TESTS_ASSERT(s1[2] == 3);
    TFEL_TESTS_ASSERT(s1[3] == 4);
    TFEL_TESTS_ASSERT(s1(0) == 1);
    TFEL_TESTS_ASSERT(s1(1) == 2);
    TFEL_TESTS_ASSERT(s1(2) == 3);
    TFEL_TESTS_ASSERT(s1(3) == 4);
    TFEL_TESTS_ASSERT(s2[0] == 10);
    TFEL_TESTS_ASSERT(s2[1] == 20);
    TFEL_TESTS_ASSERT(s2[2] == 30);
    TFEL_TESTS_ASSERT(s2[3] == 40);
    TFEL_TESTS_ASSERT(s2(0) == 10);
    TFEL_TESTS_ASSERT(s2(1) == 20);
    TFEL_TESTS_ASSERT(s2(2) == 30);
    TFEL_TESTS_ASSERT(s2(3) == 40);
  }
  void test2() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->stensor<2, int> {
      // SoA layout: [s0_c0, s1_c0, s0_c1, s1_c1, s0_c2, s1_c2, s0_c3, s1_c3]
      int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
      auto s1 = map_strided<stensor<2u, int>>(&values[0], 2);
      auto s2 = map_strided<stensor<2u, int>>(&values[1], 2);
      static_assert(StensorConcept<decltype(s1)>);
      static_assert(StensorConcept<decltype(s2)>);
      static_assert(s1.size(0) == 4);
      static_assert(s2.size(0) == 4);
      return 2 * s1 + s2;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 12);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 24);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 36);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 48);
  }
  void test2b() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->stensor<2, int> {
      // SoA layout: [s0_c0, s1_c0, s0_c1, s1_c1, s0_c2, s1_c2, s0_c3, s1_c3]
      const int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
      auto s1 = map_strided<const stensor<2u, int>>(&values[0], 2);
      auto s2 = map_strided<const stensor<2u, int>>(&values[1], 2);
      static_assert(StensorConcept<decltype(s1)>);
      static_assert(StensorConcept<decltype(s2)>);
      static_assert(s1.size(0) == 4);
      static_assert(s2.size(0) == 4);
      return 2 * s1 + s2;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 12);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 24);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 36);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 48);
  }
  void test3() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->std::array<int, 8> {
      // SoA layout: [s0_c0, s1_c0, s0_c1, s1_c1, s0_c2, s1_c2, s0_c3, s1_c3]
      std::array<int, 8> values = {1, 10, 2, 20, 3, 30, 4, 40};
      auto s1 = map_strided<stensor<2u, int>>(&values[0], 2);
      const auto s2 = map_strided<stensor<2u, int>>(&values[1], 2);
      s1 = s2;
      return values;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 10);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 10);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 20);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 20);
    TFEL_TESTS_STATIC_ASSERT(s[4] == 30);
    TFEL_TESTS_STATIC_ASSERT(s[5] == 30);
    TFEL_TESTS_STATIC_ASSERT(s[6] == 40);
    TFEL_TESTS_STATIC_ASSERT(s[7] == 40);
  }
  void test4() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->std::array<int, 4> {
      std::array<int, 4> values = {0, 1, 2, 3};
      auto s1 = map_strided<stensor<2u, int>>(&values[0], 1);
      s1 = stensor<2, int>::Id();
      return values;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 1);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 1);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 1);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 0);
  }
  void test5() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->std::array<int, 4> {
      std::array<int, 4> values = {0, 1, 2, 3};
      auto s1 = map_strided<stensor<2u, int>>(&values[0], 1);
      s1 += stensor<2, int>::Id();
      return values;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 1);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 2);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 3);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 3);
  }
  void test6() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->std::array<int, 4> {
      std::array<int, 4> values = {0, 1, 2, 3};
      auto s1 = map_strided<stensor<2u, int>>(&values[0], 1);
      s1 -= stensor<2, int>::Id();
      return values;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == -1);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 0);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 1);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 3);
  }
  void test7() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->std::array<int, 4> {
      std::array<int, 4> values = {0, 1, 2, 3};
      auto s1 = map_strided<stensor<2u, int>>(&values[0], 1);
      s1 *= 2;
      return values;
    }
    ();
    constexpr auto s2 = []() constexpr->std::array<int, 4> {
      std::array<int, 4> values = {0, 1, 2, 3};
      auto s1 = map_strided<stensor<2u, int>>(&values[0], 1);
      s1 /= 2;
      return values;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 0);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 2);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 4);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 6);
    TFEL_TESTS_STATIC_ASSERT(s2[0] == 0);
    TFEL_TESTS_STATIC_ASSERT(s2[1] == 0);
    TFEL_TESTS_STATIC_ASSERT(s2[2] == 1);
    TFEL_TESTS_STATIC_ASSERT(s2[3] == 1);
  }
  void test8() {
    auto os = std::ostringstream{};
    using namespace tfel::math;
    std::array<int, 4> values = {0, 1, 2, 3};
    auto s1 = map_strided<stensor<2u, int>>(&values[0], 1);
    os << s1;
    TFEL_TESTS_CHECK_EQUAL(os.str(), "[ 0 1 2 3 ]");
  }
  void test9() {
    auto os = std::ostringstream{};
    using namespace tfel::math;
    std::array<int, 3> values = {1, 2, 3};
    auto s1 = map_strided<tvector<3u, int>>(&values[0], 1);
    os << s1;
    TFEL_TESTS_CHECK_EQUAL(os.str(), "( 1 2 3 )");
  }
  void test10() {
    auto os = std::ostringstream{};
    using namespace tfel::math;
    std::array<int, 4> values = {0, 1, 2, 3};
    auto s1 = map_strided<tmatrix<2u, 2u, int>>(&values[0], 1);
    os << s1;
    TFEL_TESTS_CHECK_EQUAL(os.str(), "[[0,1][2,3]]");
  }
  void test11() {
    // Test StridedCoalescedViewsArray
    using namespace tfel::math;
    // SoA layout for 3 stensor<2>: 3 objects with 4 components each
    // Memory: [c0_s0, c0_s1, c0_s2, c1_s0, c1_s1, c1_s2, ...]
    std::array<int, 12> values = {1, 2, 3, 10, 20, 30, 100, 200, 300, 1000, 2000, 3000};
    auto arr = StridedCoalescedViewsFixedSizeVector<stensor<2u, int>, unsigned short, 3>(&values[0]);
    // Check object 0: components at indices 0, 3, 6, 9
    TFEL_TESTS_ASSERT(arr[0][0] == 1);
    TFEL_TESTS_ASSERT(arr[0][1] == 10);
    TFEL_TESTS_ASSERT(arr[0][2] == 100);
    TFEL_TESTS_ASSERT(arr[0][3] == 1000);
    // Check object 1: components at indices 1, 4, 7, 10
    TFEL_TESTS_ASSERT(arr[1][0] == 2);
    TFEL_TESTS_ASSERT(arr[1][1] == 20);
    TFEL_TESTS_ASSERT(arr[1][2] == 200);
    TFEL_TESTS_ASSERT(arr[1][3] == 2000);
    // Check object 2: components at indices 2, 5, 8, 11
    TFEL_TESTS_ASSERT(arr[2][0] == 3);
    TFEL_TESTS_ASSERT(arr[2][1] == 30);
    TFEL_TESTS_ASSERT(arr[2][2] == 300);
    TFEL_TESTS_ASSERT(arr[2][3] == 3000);
    // Test modification through view
    arr[1] = stensor<2, int>::Id();
    TFEL_TESTS_ASSERT(values[1] == 1);   // c0_s1
    TFEL_TESTS_ASSERT(values[4] == 1);   // c1_s1
    TFEL_TESTS_ASSERT(values[7] == 1);   // c2_s1
    TFEL_TESTS_ASSERT(values[10] == 0);  // c3_s1
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorStridedCoalescedViewTest,
                          "StensorStridedCoalescedViewTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StridedCoalescedView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
