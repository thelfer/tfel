/*!
 * \brief test_SplineLocalInterpolation
 *
 * tests if SplineLocalInterpolation class works well.
 *
 * \file test_SplineLocalInterpolation.cpp
 *
 *
 * \author rp238441
 * \date
 *
 * Copyright © 2013
 */

#include<cmath>
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFELCheck/SplineLocalInterpolation.hxx"

struct test_SplineLocalInterpolation
  : public tfel::tests::TestCase
{
  test_SplineLocalInterpolation()
    : tfel::tests::TestCase("TFELCheck","SplineLocalInterpolation")
  {}

  virtual tfel::tests::TestResult
  execute() override
  {
    TFEL_CONSTEXPR const double eps = 1.e-14;
    tfel_check::SplineLocalInterpolation i;
    i.interpolate({1.,2.,3.},{3.,5.,7.});
    TFEL_TESTS_ASSERT(std::abs(4.-i.getValue(1.5))<eps);
    TFEL_TESTS_ASSERT(std::abs(10.-i.getValue(1.5))>eps);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(test_SplineLocalInterpolation,
			  "SplineLocalInterpolation");
