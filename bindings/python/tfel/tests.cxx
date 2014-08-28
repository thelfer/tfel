/*! 
 * \file   bindings/python/tfel/tests.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  31 août 2010
 */

#include<boost/python.hpp>

void declareTestResult(void);

BOOST_PYTHON_MODULE(tests)
{
  declareTestResult();
}
