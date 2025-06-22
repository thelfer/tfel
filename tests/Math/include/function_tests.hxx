/*!
 * \file   tests/Math/include/function_tests.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   28 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef _LIB_TFEL_FUNCTION_HXX_
#define _LIB_TFEL_FUNCTION_HXX_

#include "TFEL/Math/Vector/VectorConcept.hxx"

template <class T>
void function(const tfel::math::VectorConcept<T>&,
              typename tfel::math::VectorTraits<T>::NumType,
              typename tfel::math::VectorTraits<T>::NumType,
              typename tfel::math::VectorTraits<T>::NumType,
              typename tfel::math::VectorTraits<T>::NumType,
              unsigned int);

#include "function_tests.ixx"

#endif /* _LIB_TFEL_FUNCTION_HXX */
