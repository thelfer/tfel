/*!
 * \file   include/TFEL/Math/Vector/VectorConceptOperations.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04/02/2008
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VECTORCONCEPTOPERATIONSIXX
#define LIB_TFEL_MATH_VECTORCONCEPTOPERATIONSIXX

namespace tfel::math {

  template <typename T1, typename T2>
  std::enable_if_t<IsVectorVectorOperationValid<T1, T2, OpDotProduct>::cond,
                   typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>
  operator|(const T1& a, const T2& b) {
    typedef typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result Result;
    typedef typename ComputeBinaryResult<T1, T2, OpDotProduct>::Handle Handle;
    return Handle::template exe<Result, T1, T2>(a, b);
  }

  template <typename T1>
  std::enable_if_t<
      IsEuclidianNormValid<T1>::cond,
      typename tfel::typetraits::RealPartType<
          typename ComputeBinaryResult<T1, T1, OpDotProduct>::Result>::type>
  norm(const T1& v) {
    return std::sqrt(real(v | v));
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VECTORCONCEPTOPERATIONSIXX */
