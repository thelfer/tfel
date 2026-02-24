/*!
 * \file   include/TFEL/Math/Tensor/TensorConceptOperations.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04/12/2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORCONCEPTOPERATIONSIXX
#define LIB_TFEL_MATH_TENSORCONCEPTOPERATIONSIXX

namespace tfel::math {

  template <TensorConcept TensorType1, TensorConcept TensorType2>
  TFEL_HOST_DEVICE constexpr auto
  operator|(const TensorType1& a, const TensorType2& b) noexcept requires(
      !isInvalid<
          BinaryOperationResult<TensorType1, TensorType2, OpDotProduct>>()) {
    constexpr auto N = getSpaceDimension<TensorType1>();
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    static_assert(N == getSpaceDimension<TensorType2>());
    if constexpr (N == 1u) {
      return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    } else if constexpr (N == 2u) {
      return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3] +
             a[4] * b[4];
    } else {
      return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3] +
             a[4] * b[4] + a[5] * b[5] + a[6] * b[6] + a[7] * b[7] +
             a[8] * b[8];
    }
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSORCONCEPTOPERATIONSIXX */
