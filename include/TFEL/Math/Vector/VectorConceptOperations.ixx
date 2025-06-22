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

  template <typename T1, typename T2>
  TFEL_HOST_DEVICE
      std::enable_if_t<IsVectorVectorOperationValid<T1, T2, OpDotProduct>::cond,
                       BinaryOperationResult<T1, T2, OpDotProduct>>
      operator|(const T1& a, const T2& b) {
    typedef BinaryOperationResult<T1, T2, OpDotProduct> Result;
    typedef BinaryOperationHandler<T1, T2, OpDotProduct> Handle;
    return Handle::template exe<Result, T1, T2>(a, b);
  }

  template <typename T1>
  TFEL_HOST_DEVICE
      std::enable_if_t<IsEuclidianNormValid<T1>::cond,
                       typename tfel::typetraits::RealPartType<
                           BinaryOperationResult<T1, T1, OpDotProduct>>::type>
      norm(const T1& v) {
    return std::sqrt(real(v | v));
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VECTORCONCEPTOPERATIONSIXX */
