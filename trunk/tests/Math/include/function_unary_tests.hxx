/*!
 * \file   function_unary_tests.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   28 Jun 2006
 */

#ifndef _LIB_TFEL_FUNCTION_HXX_
#define _LIB_TFEL_FUNCTION_HXX_ 

#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"

template<class T>
void function(const tfel::math::VectorConcept<T>&);

template<class T>
void function(const tfel::math::VectorConcept<T>&,
	      const typename tfel::math::VectorTraits<T>::NumType,
	      const typename tfel::math::VectorTraits<T>::NumType,
	      const typename tfel::math::VectorTraits<T>::NumType,
	      const typename tfel::math::VectorTraits<T>::NumType,
	      const unsigned int);

template<class T>
void function(const tfel::math::MatrixConcept<T>&,
	      const typename tfel::math::MatrixTraits<T>::NumType,
	      const typename tfel::math::MatrixTraits<T>::NumType,
	      const typename tfel::math::MatrixTraits<T>::NumType,
	      const typename tfel::math::MatrixTraits<T>::NumType,
	      const typename tfel::math::MatrixTraits<T>::NumType,
	      const unsigned int);

template<class T>
void function(const tfel::math::StensorConcept<T>&);

#include "function_unary_tests.ixx"

#endif /* _LIB_TFEL_FUNCTION_HXX */

