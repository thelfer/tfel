/*!
 * \file   include/TFEL/Math/Stensor/StensorConceptOperations.ixx
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

#ifndef LIB_TFEL_MATH_STENSORCONCEPTOPERATIONS_IXX
#define LIB_TFEL_MATH_STENSORCONCEPTOPERATIONS_IXX

namespace tfel::math {

  template <StensorConcept StensorType1, StensorConcept StensorType2>
  TFEL_HOST_DEVICE constexpr BinaryOperationResult<StensorType1,
                                                   StensorType2,
                                                   OpDotProduct>
  operator|(const StensorType1& a, const StensorType2& b) noexcept requires(
      (getSpaceDimension<StensorType1>() ==
       getSpaceDimension<StensorType2>()) &&
      (!isInvalid<
          BinaryOperationResult<StensorType1, StensorType2, OpDotProduct>>())) {
    constexpr auto N = getSpaceDimension<StensorType1>();
    if constexpr (N == 1) {
      return a(0) * b(0) + a(1) * b(1) + a(2) * b(2);
    } else if constexpr (N == 2) {
      return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3);
    } else {
      return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3) +
             a(4) * b(4) + a(5) * b(5);
    }
  }  // end of operator|

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORCONCEPTOPERATIONS_IXX */
