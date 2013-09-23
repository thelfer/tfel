/*! 
 * \file  math.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 aoû 2010
 */

#include<boost/python.hpp>

void declareTFELMathVector(void);
void declaretvector(void);
void declarestensor(void);

BOOST_PYTHON_MODULE(math)
{
  declaretvector();
  declarestensor();
  declareTFELMathVector();
}
