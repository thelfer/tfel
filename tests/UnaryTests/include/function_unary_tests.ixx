/*!
 * \file   function_unary_tests.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   28 Jun 2006
 */

#ifndef _LIB_TFEL_FUNCTION_IXX_
#define _LIB_TFEL_FUNCTION_IXX_ 

#include<cmath>
#include<cstdlib>

#define ASSERT(X)                                                  \
if(!(X)){                                                          \
  std::cerr << "("<< #X << ")" << " is not satified" << std::endl; \
  std::exit(EXIT_FAILURE);                                         \
}                                                                  \
static_cast<void>(0)

template<class T>
void function(const tfel::math::VectorConcept<T>& x){
  std::cout << x(0) << " " << x(1) << " " << x(2) << std::endl;
  std::cout << "------" << std::endl;
}  

template<class T>
void function(const tfel::math::VectorConcept<T>& x,
	      const typename tfel::math::VectorTraits<T>::NumType v0,
	      const typename tfel::math::VectorTraits<T>::NumType v1,
	      const typename tfel::math::VectorTraits<T>::NumType v2,
	      const typename tfel::math::VectorTraits<T>::NumType eps,
	      const unsigned int test_number)
{
#ifdef VERBOSE
  std::cout << "Test : " << test_number << std::endl;
#else
  static_cast<void>(test_number);
#endif
  ASSERT(std::abs(x(0)-v0)<=eps);
  ASSERT(std::abs(x(1)-v1)<=eps);
  ASSERT(std::abs(x(2)-v2)<=eps);
}

template<class T>
void function(const tfel::math::MatrixConcept<T>& x,
	      const typename tfel::math::MatrixTraits<T>::NumType v0,
	      const typename tfel::math::MatrixTraits<T>::NumType v1,
	      const typename tfel::math::MatrixTraits<T>::NumType v2,
	      const typename tfel::math::MatrixTraits<T>::NumType v3,
	      const typename tfel::math::MatrixTraits<T>::NumType eps,
	      const unsigned int test_number)
{
#ifdef VERBOSE
  std::cout << "Test : " << test_number << std::endl;
#else
  static_cast<void>(test_number);
#endif
  ASSERT(std::abs(x(0,0)-v0)<=eps);
  ASSERT(std::abs(x(0,1)-v1)<=eps);
  ASSERT(std::abs(x(1,0)-v2)<=eps);
  ASSERT(std::abs(x(1,1)-v3)<=eps);
}

template<class T>
void function(const tfel::math::StensorConcept<T>& x){
  std::cout << x(0) << " " << x(1) << " " << x(2) << std::endl;
  std::cout << "------" << std::endl;
}  


#endif /* _LIB_TFEL_FUNCTION_IXX */

