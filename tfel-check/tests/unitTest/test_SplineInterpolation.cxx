/*!
 * \file   tfel-check/tests/unitTest/test_SplineInterpolation.cpp
 * \brief  test of the `SplineInterpolation` class.
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
#include "TFEL/Check/SplineInterpolation.hxx"

struct test_SplineInterpolation : public tfel::tests::TestCase {
  test_SplineInterpolation()
      : tfel::tests::TestCase("TFEL/Check", "SplineInterpolation") {}

  tfel::tests::TestResult execute() override {
    constexpr const double eps = 1.e-14;
    tfel::check::SplineInterpolation i;
    i.interpolate({1., 2.}, {5., 7.});
    TFEL_TESTS_ASSERT(std::abs(6. - i.getValue(1.5)) < eps);
    TFEL_TESTS_ASSERT(std::abs(10. - i.getValue(1.5)) > eps);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1., 2., 3.}, {5., 7.}),
                           std::exception);
    TFEL_TESTS_CHECK_THROW(i.interpolate({}, {5., 7.}), std::exception);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1., 2.}, {}), std::exception);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(test_SplineInterpolation, "SplineInterpolation");
