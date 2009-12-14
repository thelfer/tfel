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
void function(const tfel::math::VectorConcept<T>& x){
  std::cout << x(0) << " " << x(1) << " " << x(2) << std::endl;
  std::cout << "------" << std::endl;
}  

template<class T>
void function(const tfel::math::VectorConcept<T>& x,
	      typename tfel::math::VectorTraits<T>::NumType v0,
	      typename tfel::math::VectorTraits<T>::NumType v1,
	      typename tfel::math::VectorTraits<T>::NumType v2,
	      typename tfel::math::VectorTraits<T>::NumType eps,
	      unsigned int test_number)
{
#ifdef VERBOSE
  std::cout << "Test : " << test_number << std::endl;
#endif
  assert(std::abs(x(0)-v0)<=eps);
  assert(std::abs(x(1)-v1)<=eps);
  assert(std::abs(x(2)-v2)<=eps);
}


template<class T>
void function(const tfel::math::StensorConcept<T>& x){
  std::cout << x(0) << " " << x(1) << " " << x(2) << std::endl;
  std::cout << "------" << std::endl;
}  


#endif /* _LIB_TFEL_FUNCTION_IXX */

