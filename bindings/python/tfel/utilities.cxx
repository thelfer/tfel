/*! 
 * \file  bindings/python/tfel/utilities.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 aoû 2010
 */

#include<boost/python.hpp>

void declareTextData(void);

BOOST_PYTHON_MODULE(utilities)
{
  declareTextData();
}
