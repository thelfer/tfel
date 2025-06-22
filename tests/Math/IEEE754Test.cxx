/*!
 * \file   tests/Math/IEEE754Test.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 mars 2017
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
#include <limits>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/General/IEEE754.hxx"

struct IEEE754Test final : public tfel::tests::TestCase {
  IEEE754Test()
      : tfel::tests::TestCase("TFEL/Math", "IEEE754Test") {
  }  // end of IEEE754Test
  tfel::tests::TestResult execute() override {
    this->test<float>();
    this->test<double>();
    this->test<long double>();
    return this->result;
  }  // end of execute
 private:
  template <typename T>
  void test() {
    namespace ieee754 = tfel::math::ieee754;
    using limits = std::numeric_limits<T>;
    constexpr auto zero = T(0);
    constexpr auto one = T(1);
    constexpr const auto dn = limits::min() / 2;
    if (limits::has_quiet_NaN) {
      TFEL_TESTS_ASSERT(ieee754::fpclassify(limits::quiet_NaN()) == FP_NAN);
      TFEL_TESTS_ASSERT(ieee754::fpclassify(-limits::quiet_NaN()) == FP_NAN);
      TFEL_TESTS_ASSERT(ieee754::isnan(limits::quiet_NaN()));
      TFEL_TESTS_ASSERT(ieee754::isnan(-limits::quiet_NaN()));
      TFEL_TESTS_ASSERT(!ieee754::isfinite(limits::quiet_NaN()));
      TFEL_TESTS_ASSERT(!ieee754::isfinite(-limits::quiet_NaN()));
    }
    if (limits::has_signaling_NaN) {
      TFEL_TESTS_ASSERT(ieee754::fpclassify(limits::signaling_NaN()) == FP_NAN);
      TFEL_TESTS_ASSERT(ieee754::fpclassify(-limits::signaling_NaN()) ==
                        FP_NAN);
      TFEL_TESTS_ASSERT(ieee754::isnan(limits::signaling_NaN()));
      TFEL_TESTS_ASSERT(ieee754::isnan(-limits::signaling_NaN()));
      TFEL_TESTS_ASSERT(!ieee754::isfinite(limits::signaling_NaN()));
      TFEL_TESTS_ASSERT(!ieee754::isfinite(-limits::signaling_NaN()));
    }
    if (limits::has_infinity) {
      TFEL_TESTS_ASSERT(ieee754::fpclassify(limits::infinity()) == FP_INFINITE);
      TFEL_TESTS_ASSERT(ieee754::fpclassify(-limits::infinity()) ==
                        FP_INFINITE);
    }
    TFEL_TESTS_ASSERT(ieee754::fpclassify(zero) == FP_ZERO);
    TFEL_TESTS_ASSERT(ieee754::fpclassify(-zero) == FP_ZERO);
    TFEL_TESTS_ASSERT(!ieee754::isnan(zero));
    TFEL_TESTS_ASSERT(!ieee754::isnan(-zero));
    TFEL_TESTS_ASSERT(ieee754::isfinite(zero));
    TFEL_TESTS_ASSERT(ieee754::isfinite(-zero));
    TFEL_TESTS_ASSERT(ieee754::fpclassify(one) == FP_NORMAL);
    TFEL_TESTS_ASSERT(!ieee754::isnan(one));
    TFEL_TESTS_ASSERT(!ieee754::isnan(-one));
    TFEL_TESTS_ASSERT(ieee754::isfinite(one));
    TFEL_TESTS_ASSERT(ieee754::isfinite(-one));
    TFEL_TESTS_ASSERT(ieee754::fpclassify(dn) == FP_SUBNORMAL);
    TFEL_TESTS_ASSERT(!ieee754::isnan(dn));
    TFEL_TESTS_ASSERT(!ieee754::isnan(-dn));
    TFEL_TESTS_ASSERT(ieee754::isfinite(dn));
    TFEL_TESTS_ASSERT(ieee754::isfinite(-dn));
  }
};

TFEL_TESTS_GENERATE_PROXY(IEEE754Test, "IEEE754Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("IEEE754Test.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
