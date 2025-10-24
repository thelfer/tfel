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
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/Array/CoalescedView.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"

struct StensorCoalescedViewTest final : public tfel::tests::TestCase {
  StensorCoalescedViewTest()
      : tfel::tests::TestCase("TFEL/Math", "StensorCoalescedViewTest") {
  }  // end of StensorCoalescedViewTest
  tfel::tests::TestResult execute() override {
    this->test1();
    return this->result;
  }  // end of execute
 private:
  //! \brief criterion value
  static constexpr auto eps = 1e-14;
  //! \brief create a view from raw memory
  void test1() {
    using namespace tfel::math;
    constexpr auto s = []() constexpr->stensor<2, int> {
      int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
      std::array ptrs{&values[0], &values[2], &values[4], &values[6]};
      std::array ptrs2{&values[1], &values[3], &values[5], &values[7]};
      auto s1 = map<stensor<2u, int>>(ptrs);
      auto s2 = map<stensor<2u, int>>(ptrs2);
      return 2 * s1 + s2;
    }
    ();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 12);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 24);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 36);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 48);
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorCoalescedViewTest, "StensorCoalescedViewTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CoalescedView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
