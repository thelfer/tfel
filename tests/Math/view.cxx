/*!
 * \file   view.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12/05/2021
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

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Array/ViewsArray.hxx"

struct StensorViewTest final : public tfel::tests::TestCase {
  StensorViewTest()
      : tfel::tests::TestCase("TFEL/Math", "StensorViewTest") {
  }  // end of StensorViewTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    this->test6();
    this->test7();
    this->constexpr_test1();
    this->constexpr_test2();
    return this->result;
  }  // end of execute
 private:
  //! \brief criterion value
  static constexpr auto eps = 1e-14;
  //! \brief memory buffer
  tfel::math::tvector<12, double> array;
  //! \brief create an array view from raw memory
  static constexpr tfel::math::stensor<2u, int> get() {
    using namespace tfel::math;
    tvector<12, int> buffer{0};
    auto* const data = buffer.data();
    auto s = map_array<tvector<2u, stensor<2u, int>>>(data);
    s[0] = stensor<2u, int>::Id();
    s[1] = s[0] * 2;
    return s[1];
  }
  //! \brief create a view from raw memory
  void test1() {
    using namespace tfel::math;
    this->array = tvector<12, double>{0};
    auto* const data = this->array.data();
    this->check0();
    auto s1 = map<stensor<2u, double>>(data);
    s1 = stensor<2u, double>::Id();
    this->check1();
    auto s2 = map<stensor<2u, double>>(data + s1.size());
    s2 = s1 * 2;
    this->check2();
  }
  //! \brief create a view from a tiny vector
  void test2() {
    using namespace tfel::math;
    this->array = tvector<12, double>{0};
    this->check0();
    auto s1 = map<stensor<2u, double>>(this->array);
    s1 = stensor<2u, double>::Id();
    this->check1();
    // map using an offset
    auto s2 = map<stensor<2u, double>, getStensorSize<2>()>(this->array);
    s2 = s1 * 2;
    this->check2();
  }
  //! \brief create an array view from raw memory
  void test3() {
    using namespace tfel::math;
    this->array = tvector<12, double>{0};
    auto* const data = this->array.data();
    this->check0();
    auto s = map_array<tvector<2u, stensor<2u, double>>>(data);
    s[0] = stensor<2u, double>::Id();
    this->check1();
    s[1] = s[0] * 2;
    this->check2();
  }
  //! \brief create an array view from a tiny vector
  void test4() {
    using namespace tfel::math;
    this->array = tvector<12, double>{0};
    this->check0();
    auto s = map<2u, stensor<2u, double>>(this->array);
    s[0] = stensor<2u, double>::Id();
    this->check1();
    s[1] = s[0] * 2;
    this->check2();
  }
  //! \brief create an array view from a tiny vector
  void test5() {
    using namespace tfel::math;
    this->array = tvector<12, double>{0};
    this->check0();
    auto v1 = map<8u, double>(this->array);
    v1[0] = 1;
    v1[1] = 1;
    v1[2] = 1;
    v1[3] = 0;
    v1[4] = 2;
    v1[5] = 2;
    v1[6] = 2;
    v1[7] = 0;
    this->check2();
  }
  //! \brief create an array view from a tiny vector
  void test6() {
    using namespace tfel::math;
    this->array = tvector<12, double>{0};
    this->check0();
    auto s1 = map<stensor<2u, double>>(this->array);
    s1 = stensor<2u, double>::Id();
    auto v = map<4u, double>(this->array);
    TFEL_TESTS_ASSERT(std::abs(v[0] - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(v[1] - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(v[2] - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(v[3] - 0) < eps);
  }
  //! \brief create an array view from a tiny vector
  void test7() {
    using namespace tfel::math;
    this->array = tvector<12, double>{0};
    this->check0();
    auto s1 = map<stensor<2u, double>>(this->array);
    s1 = stensor<2u, double>::Id();
    auto v1 = map<tvector<4u, double>>(this->array);
    auto v2 = tvector<4u, double>{1, 2, 3, 4};
    v2 += v1;
    TFEL_TESTS_ASSERT(std::abs(v2[0] - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(v2[1] - 3) < eps);
    TFEL_TESTS_ASSERT(std::abs(v2[2] - 4) < eps);
    TFEL_TESTS_ASSERT(std::abs(v2[3] - 4) < eps);
  }

  void constexpr_test1() {
    constexpr auto s = StensorViewTest::get();
    TFEL_TESTS_STATIC_ASSERT(s[0] == 2);
    TFEL_TESTS_STATIC_ASSERT(s[1] == 2);
    TFEL_TESTS_STATIC_ASSERT(s[2] == 2);
    TFEL_TESTS_STATIC_ASSERT(s[3] == 0);
  }
  void constexpr_test2() {
    using namespace tfel::math;
    using ipolicy = stensor<2u, double>::indexing_policy;
    using base = selectViewArrayBase<stensor<2u, double>, ipolicy>;
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<base, MutableFixedSizeArrayBase<
                                  View<stensor<2u, double>, ipolicy>,  //
                                  ViewArrayPolicy<double, ipolicy>>>));
  }
  //! \brief an helper method to check values
  void check0() {
    for (const auto& v : this->array) {
      TFEL_TESTS_ASSERT(std::abs(v) < this->eps);
    }
  }
  //! \brief an helper method to check values
  void check1() {
    TFEL_TESTS_ASSERT(std::abs(array[0] - 1) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[1] - 1) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[2] - 1) < this->eps);
    for (tfel::math::tvector<10, double>::size_type i = 3;  //
         i != array.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(array[i]) < this->eps);
    }
  }
  //! \brief an helper method to check values
  void check2() {
    TFEL_TESTS_ASSERT(std::abs(array[0] - 1) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[1] - 1) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[2] - 1) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[3] - 0) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[4] - 2) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[5] - 2) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[6] - 2) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[7] - 0) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[8] - 0) < this->eps);
    TFEL_TESTS_ASSERT(std::abs(array[9] - 0) < this->eps);
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorViewTest, "StensorViewTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("View.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
