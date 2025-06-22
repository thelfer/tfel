/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2Concept.ixx
 * \brief
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_CONCEPT_IMPL_
#define LIB_TFEL_MATH_ST2TOST2_CONCEPT_IMPL_ 1

#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel {

  namespace math {

    template <class T>
    TFEL_MATH_INLINE typename ST2toST2Traits<T>::NumType
    ST2toST2Concept<T>::operator()(const unsigned short i,
                                   const unsigned short j) const {
      return static_cast<const T&>(*this).operator()(i, j);
    }  // end of ST2toST2Concept<T>::operator()

    template <typename ST2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond,
        typename tfel::typetraits::AbsType<
            typename ST2toST2Traits<ST2toST2Type>::NumType>::type>::type
    abs(const ST2toST2Type& v) {
      unsigned short i;
      unsigned short j;
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType NumType;
      typedef typename tfel::typetraits::AbsType<NumType>::type AbsNumType;
      AbsNumType a(0);
      for (i = 0;
           i < StensorDimeToSize<ST2toST2Traits<ST2toST2Type>::dime>::value;
           ++i) {
        for (j = 0;
             j < StensorDimeToSize<ST2toST2Traits<ST2toST2Type>::dime>::value;
             ++j) {
          a += abs(v(i, j));
        }
      }
      return a;
    }

    template <typename T>
    auto transpose(T&& t) -> typename std::enable_if<
        tfel::meta::Implements<typename std::decay<T>::type,
                               ST2toST2Concept>::cond,
        Expr<typename ST2toST2Type<typename std::decay<T>::type>::type,
             ST2toST2TransposeExpr<decltype(t)>>>::type {
      return Expr<typename ST2toST2Type<typename std::decay<T>::type>::type,
                  ST2toST2TransposeExpr<decltype(t)>>(std::forward<T>(t));
    }

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/ST2toST2/ST2toST2ConceptPushForward.ixx"

#endif /* LIB_TFEL_MATH_ST2TOST2_CONCEPT_IMPL_ */
