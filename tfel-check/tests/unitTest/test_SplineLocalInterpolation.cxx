/*!
 * \file tfel-check/tests/unitTest/test_SplineLocalInterpolation.cxx
 * \brief tests of the `SplineLocalInterpolation` class
 * \author rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Check/SplineLocalInterpolation.hxx"

struct test_SplineLocalInterpolation : public tfel::tests::TestCase {
  test_SplineLocalInterpolation()
      : tfel::tests::TestCase("TFEL/Check", "SplineLocalInterpolation") {}

  tfel::tests::TestResult execute() override {
    constexpr const double eps = 1.e-14;
    tfel::check::SplineLocalInterpolation i;
    i.interpolate({1., 2., 3.}, {3., 5., 7.});
    TFEL_TESTS_ASSERT(std::abs(4. - i.getValue(1.5)) < eps);
    TFEL_TESTS_ASSERT(std::abs(10. - i.getValue(1.5)) > eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(test_SplineLocalInterpolation,
                          "SplineLocalInterpolation");
