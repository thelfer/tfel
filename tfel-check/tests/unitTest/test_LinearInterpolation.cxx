/*!
 * \brief test_LinearInterpolation
 *
 * tests if LinearInterpolation class works well.
 *
 * \file test_LinearInterpolation.cpp
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
#include"TFELCheck/LinearInterpolation.hxx"

struct test_LinearInterpolation final
  : public tfel::tests::TestCase
{
  test_LinearInterpolation()
    : tfel::tests::TestCase("TFELCheck",
			    "LinearInterpolation")
  {}
  virtual tfel::tests::TestResult
  execute() override
  {
    constexpr const double eps = 1.e-14;
    tfel_check::LinearInterpolation i;
    i.interpolate({1.,2.},{2.,4.});
    TFEL_TESTS_ASSERT(std::abs(3.-i.getValue(1.5))<eps);
    TFEL_TESTS_ASSERT(std::abs(10.-i.getValue(1.5))>eps);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1.,2.,3.},{2.,4.}),
			   std::runtime_error);
    TFEL_TESTS_CHECK_THROW(i.interpolate({},{2.,4.}),
			   std::runtime_error);
    TFEL_TESTS_CHECK_THROW(i.interpolate({1.,2.},{}),
			   std::runtime_error);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(test_LinearInterpolation,"LinearInterpolation");

