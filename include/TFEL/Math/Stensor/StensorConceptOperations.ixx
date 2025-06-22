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

#ifndef LIB_TFEL_STENSORCONCEPTOPERATIONSIXX
#define LIB_TFEL_STENSORCONCEPTOPERATIONSIXX

namespace tfel {

  namespace math {

    template <typename T1, typename T2>
    typename std::enable_if<
        tfel::meta::Implements<T1, StensorConcept>::cond &&
            tfel::meta::Implements<T2, StensorConcept>::cond &&
            StensorTraits<T1>::dime == 1u && StensorTraits<T2>::dime == 1u &&
            !tfel::typetraits::IsInvalid<
                typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::
                cond,
        typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
    operator|(const T1& a, const T2& b) {
      return a(0) * b(0) + a(1) * b(1) + a(2) * b(2);
    }

    template <typename T1, typename T2>
    typename std::enable_if<
        tfel::meta::Implements<T1, StensorConcept>::cond &&
            tfel::meta::Implements<T2, StensorConcept>::cond &&
            StensorTraits<T1>::dime == 2u && StensorTraits<T2>::dime == 2u &&
            !tfel::typetraits::IsInvalid<
                typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::
                cond,
        typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
    operator|(const T1& a, const T2& b) {
      return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3);
    }

    template <typename T1, typename T2>
    typename std::enable_if<
        tfel::meta::Implements<T1, StensorConcept>::cond &&
            tfel::meta::Implements<T2, StensorConcept>::cond &&
            StensorTraits<T1>::dime == 3u && StensorTraits<T2>::dime == 3u &&
            !tfel::typetraits::IsInvalid<
                typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::
                cond,
        typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
    operator|(const T1& a, const T2& b) {
      return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3) +
             a(4) * b(4) + a(5) * b(5);
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_STENSORCONCEPTOPERATIONSIXX */
