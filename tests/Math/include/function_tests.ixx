/*!
 * \file   function_tests.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   28 Jun 2006
 */

#ifndef _LIB_TFEL_FUNCTION_IXX_
#define _LIB_TFEL_FUNCTION_IXX_ 

#include <cmath>

template<class T>
void function(const tfel::math::VectorConcept<T>& x,
	      typename tfel::math::VectorTraits<T>::NumType v0,
	      typename tfel::math::VectorTraits<T>::NumType v1,
	      typename tfel::math::VectorTraits<T>::NumType v2,
	      typename tfel::math::VectorTraits<T>::NumType eps,
	      unsigned int test_number)
{
  assert(std::abs(x(0)-v0)<=eps);
  assert(std::abs(x(1)-v1)<=eps);
  assert(std::abs(x(2)-v2)<=eps);
}

#endif /* _LIB_TFEL_FUNCTION_IXX */

