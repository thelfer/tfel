/*!
 * \file   tests/Math/include/function_tests.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   28 Jun 2006
 */

#ifndef _LIB_TFEL_FUNCTION_HXX_
#define _LIB_TFEL_FUNCTION_HXX_ 

#include"TFEL/Math/Vector/VectorConcept.hxx"

template<class T>
void function(const tfel::math::VectorConcept<T>&,
	      typename tfel::math::VectorTraits<T>::NumType,
	      typename tfel::math::VectorTraits<T>::NumType,
	      typename tfel::math::VectorTraits<T>::NumType,
	      typename tfel::math::VectorTraits<T>::NumType,
	      unsigned int);

#include "function_tests.ixx"

#endif /* _LIB_TFEL_FUNCTION_HXX */

