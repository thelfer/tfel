/*!
 * \file  tfel-check/tests/unitTest/test_LinearInterpolation.cpp
 * \brief tests of the `LinearInterpolation` class.
 * \author rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Check/LinearInterpolation.hxx"

struct test_LinearInterpolation final : public tfel::tests::TestCase {
  test_LinearInterpolation()
      : tfel::tests::TestCase("TFEL/Check", "LinearInterpolation") {}
  tfel::tests::TestResult execute() override {
    constexpr const double eps = 1.e-14;
    tfel::check::LinearInterpolation i;
    i.interpolate({1., 2.}, {2., 4.});
    TFEL_TESTS_ASSERT(std::abs(3. - i.getValue(1.5)) < eps);
    TFEL_TESTS_ASSERT(std::abs(10. - i.getValue(1.5)) > eps);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1., 2., 3.}, {2., 4.}),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(i.interpolate({}, {2., 4.}), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1., 2.}, {}), std::runtime_error);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(test_LinearInterpolation, "LinearInterpolation");
