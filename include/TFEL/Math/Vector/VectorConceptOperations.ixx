/*!
 * \file   include/TFEL/Math/Vector/VectorConceptOperations.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04/02/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VECTORCONCEPTOPERATIONSIXX
#define LIB_TFEL_MATH_VECTORCONCEPTOPERATIONSIXX

namespace tfel::math {

  template <VectorConcept T1, VectorConcept T2>
  TFEL_HOST_DEVICE constexpr auto operator|(const T1& a, const T2& b) requires(
      !isInvalid<BinaryOperationResult<T1, T2, OpDotProduct>>()) {
    typedef BinaryOperationResult<T1, T2, OpDotProduct> Result;
    typedef BinaryOperationHandler<T1, T2, OpDotProduct> Handle;
    return Handle::template exe<Result, T1, T2>(a, b);
  }

  template <VectorConcept T1>
  TFEL_HOST_DEVICE auto norm(const T1& v) requires(
      !isInvalid<BinaryOperationResult<T1, T1, OpDotProduct>>()) {
    return power<1, 2>(real(v | v));
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VECTORCONCEPTOPERATIONSIXX */
