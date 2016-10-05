/*!
 * \brief test_SplineInterpolation
 *
 * tests if SplineInterpolation class works well.
 *
 * \file test_SplineInterpolation.cpp
 *
 *
 * \author rp238441
 * \date
 *
 * Copyright © 2013
 */

#include<cmath>
#include<stdexcept>
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFELCheck/SplineInterpolation.hxx"

struct test_SplineInterpolation
  : public tfel::tests::TestCase
{
  test_SplineInterpolation()
    : tfel::tests::TestCase("TFELCheck","SplineInterpolation")
  {}

  virtual tfel::tests::TestResult
  execute() override
  {
    constexpr const double eps = 1.e-14;
    tfel_check::SplineInterpolation i;
    i.interpolate({1.,2.},{5.,7.});
    TFEL_TESTS_ASSERT(std::abs(6.-i.getValue(1.5))<eps);
    TFEL_TESTS_ASSERT(std::abs(10.-i.getValue(1.5))>eps);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1.,2.,3.},{5.,7.}),
			   std::exception);
    TFEL_TESTS_CHECK_THROW(i.interpolate({},{5.,7.}),
			   std::exception);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1.,2.},{}),
			   std::exception);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(test_SplineInterpolation,
			  "SplineInterpolation");

