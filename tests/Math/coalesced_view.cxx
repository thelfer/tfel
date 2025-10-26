/*!
 * \file   coalesced_view.cxx
 * \brief
 * \author th202608
 * \date   24/10/2025
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
#include "TFEL/Math/Array/CoalescedView.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"

struct StensorCoalescedViewTest final : public tfel::tests::TestCase {
  StensorCoalescedViewTest()
      : tfel::tests::TestCase("TFEL/Math", "StensorCoalescedViewTest") {
  }  // end of StensorCoalescedViewTest
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
    return this->result;
  }  // end of execute
 private:
  //! \brief criterion value
  static constexpr auto eps = 1e-14;
  void test1() {
    using namespace tfel::math;
    int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
    std::array ptrs{&values[0], &values[2], &values[4], &values[6]};
    std::array<const int*, 4> ptrs2{&values[1], &values[3], &values[5],
                                    &values[7]};
    auto s1 = map<stensor<2u, int>>(ptrs);
    const auto s2 = map<const stensor<2u, int>>(ptrs2);
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
      int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
      std::array ptrs{&values[0], &values[2], &values[4], &values[6]};
      std::array ptrs2{&values[1], &values[3], &values[5], &values[7]};
      auto s1 = map<stensor<2u, int>>(ptrs);
      auto s2 = map<stensor<2u, int>>(ptrs2);
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
      const int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
      std::array ptrs{&values[0], &values[2], &values[4], &values[6]};
      std::array ptrs2{&values[1], &values[3], &values[5], &values[7]};
      auto s1 = map<const stensor<2u, int>>(ptrs);
      auto s2 = map<const stensor<2u, int>>(ptrs2);
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
      std::array<int, 8> values = {1, 10, 2, 20, 3, 30, 4, 40};
      std::array ptrs{&values[0], &values[2], &values[4], &values[6]};
      std::array ptrs2{&values[1], &values[3], &values[5], &values[7]};
      auto s1 = map<stensor<2u, int>>(ptrs);
      const auto s2 = map<stensor<2u, int>>(ptrs2);
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
      std::array ptrs{&values[0], &values[1], &values[2], &values[3]};
      auto s1 = map<stensor<2u, int>>(ptrs);
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
      std::array ptrs{&values[0], &values[1], &values[2], &values[3]};
      auto s1 = map<stensor<2u, int>>(ptrs);
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
      std::array ptrs{&values[0], &values[1], &values[2], &values[3]};
      auto s1 = map<stensor<2u, int>>(ptrs);
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
      std::array ptrs{&values[0], &values[1], &values[2], &values[3]};
      auto s1 = map<stensor<2u, int>>(ptrs);
      s1 *= 2;
      return values;
    }
    ();
    constexpr auto s2 = []() constexpr->std::array<int, 4> {
      std::array<int, 4> values = {0, 1, 2, 3};
      std::array ptrs{&values[0], &values[1], &values[2], &values[3]};
      auto s1 = map<stensor<2u, int>>(ptrs);
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
  void test8() { auto os = std::ostringstream{};
    using namespace tfel::math;
    std::array<int, 4> values = {0, 1, 2, 3};
    std::array ptrs{&values[0], &values[1], &values[2], &values[3]};
    auto s1 = map<stensor<2u, int>>(ptrs);
    os << s1;
    TFEL_TESTS_CHECK_EQUAL(os.str(), "[ 0 1 2 3 ]");
  }
  void test9() { auto os = std::ostringstream{};
    using namespace tfel::math;
    std::array<int, 3> values = {1, 2, 3};
    std::array ptrs{&values[0], &values[1], &values[2]};
    auto s1 = map<tvector<3u, int>>(ptrs);
    os << s1;
    TFEL_TESTS_CHECK_EQUAL(os.str(), "( 1 2 3 )");
  }
  void test10() { auto os = std::ostringstream{};
    using namespace tfel::math;
    std::array<int, 4> values = {0, 1, 2, 3};
    std::array ptrs{&values[0], &values[1], &values[2], &values[3]};
    auto s1 = map<tmatrix<2u, 2u, int>>(ptrs);
    os << s1;
    TFEL_TESTS_CHECK_EQUAL(os.str(), "[[0,1][2,3]]");
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorCoalescedViewTest, "StensorCoalescedViewTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CoalescedView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
