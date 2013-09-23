/*! 
 * \file   vector.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 avr 2010
 */

#include<string>

#include"TFEL/Math/vector.hxx"
#include"TFEL/Python/VectorConverter.hxx"

void declareTFELMathVector(void)
{
  using namespace tfel::python;
  using tfel::math::vector;
  initializeVectorConverter<vector<double> >();
}

