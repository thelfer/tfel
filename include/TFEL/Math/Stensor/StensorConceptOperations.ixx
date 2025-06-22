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

#ifndef LIB_TFEL_MATH_STENSORCONCEPTOPERATIONSIXX
#define LIB_TFEL_MATH_STENSORCONCEPTOPERATIONSIXX

namespace tfel::math {

  template <typename StensorType1, typename StensorType2>
  std::enable_if_t<
      implementsStensorConcept<StensorType1>() &&
          implementsStensorConcept<StensorType2>() &&
          getSpaceDimension<StensorType1>() == 1u &&
          getSpaceDimension<StensorType2>() == 1u &&
          !isInvalid<BinaryOperationResult<StensorType1,
                                           StensorType2,
                                           OpDotProduct>>(),
      BinaryOperationResult<StensorType1, StensorType2, OpDotProduct>>
  operator|(const StensorType1& a, const StensorType2& b) {
    return a(0) * b(0) + a(1) * b(1) + a(2) * b(2);
  }

  template <typename StensorType1, typename StensorType2>
  std::enable_if_t<
      implementsStensorConcept<StensorType1>() &&
          implementsStensorConcept<StensorType2>() &&
          getSpaceDimension<StensorType1>() == 2u &&
          getSpaceDimension<StensorType2>() == 2u &&
          !isInvalid<BinaryOperationResult<StensorType1,
                                           StensorType2,
                                           OpDotProduct>>(),
      BinaryOperationResult<StensorType1, StensorType2, OpDotProduct>>
  operator|(const StensorType1& a, const StensorType2& b) {
    return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3);
  }

  template <typename StensorType1, typename StensorType2>
  std::enable_if_t<
      implementsStensorConcept<StensorType1>() &&
          implementsStensorConcept<StensorType2>() &&
          getSpaceDimension<StensorType1>() == 3u &&
          getSpaceDimension<StensorType2>() == 3u &&
          !isInvalid<BinaryOperationResult<StensorType1,
                                           StensorType2,
                                           OpDotProduct>>(),
      BinaryOperationResult<StensorType1, StensorType2, OpDotProduct>>
  operator|(const StensorType1& a, const StensorType2& b) {
    return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3) + a(4) * b(4) +
           a(5) * b(5);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORCONCEPTOPERATIONSIXX */
