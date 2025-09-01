/*!
 * \file   tests/Math/include/function_tests.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   28 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FUNCTIONIXX
#define LIB_TFEL_FUNCTIONIXX

#include <cmath>

template <class T>
void function(const tfel::math::VectorConcept<T>& x,
              typename tfel::math::MathObjectTraits<T>::NumType v0,
              typename tfel::math::MathObjectTraits<T>::NumType v1,
              typename tfel::math::MathObjectTraits<T>::NumType v2,
              typename tfel::math::MathObjectTraits<T>::NumType eps,
              unsigned int) {
  assert(std::abs(x(0) - v0) <= eps);
  assert(std::abs(x(1) - v1) <= eps);
  assert(std::abs(x(2) - v2) <= eps);
}

#endif /* LIB_TFEL_FUNCTION_IXX */
