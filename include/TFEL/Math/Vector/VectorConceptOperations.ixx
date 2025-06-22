/*!
 * \file   include/TFEL/Math/Vector/VectorConceptOperations.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   04 fév 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX_
#define LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX_

namespace tfel {

  namespace math {

    template <typename T1, typename T2>
    typename std::enable_if<
        IsVectorVectorOperationValid<T1, T2, OpDotProduct>::cond,
        typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
    operator|(const T1& a, const T2& b) {
      typedef typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result Result;
      typedef typename ComputeBinaryResult<T1, T2, OpDotProduct>::Handle Handle;
      return Handle::template exe<Result, T1, T2>(a, b);
    }

    template <typename T1>
    typename std::enable_if<
        IsEuclidianNormValid<T1>::cond,
        typename tfel::typetraits::RealPartType<
            typename ComputeBinaryResult<T1, T1, OpDotProduct>::Result>::type>::
        type
        norm(const T1& v) {
      return std::sqrt(real(v | v));
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX_ */
