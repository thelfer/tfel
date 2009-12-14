/*!
 * \file   function_tests.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   28 Jun 2006
 */

#ifndef _LIB_TFEL_FUNCTION_HXX_
#define _LIB_TFEL_FUNCTION_HXX_ 

#include "Math/Vector/VectorConcept.hxx"
#include "Math/Stensor/StensorConcept.hxx"

template<class T>
void function(const tfel::math::VectorConcept<T>&);

template<class T>
void function(const tfel::math::VectorConcept<T>&,
	      typename tfel::math::VectorTraits<T>::NumType,
	      typename tfel::math::VectorTraits<T>::NumType,
	      typename tfel::math::VectorTraits<T>::NumType,
	      typename tfel::math::VectorTraits<T>::NumType,
	      unsigned int);

template<class T>
void function(const tfel::math::StensorConcept<T>&);

#include "function_tests.ixx"

#endif /* _LIB_TFEL_FUNCTION_HXX */

