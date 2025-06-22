/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2Concept.ixx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2_CONCEPT_IXX
#define LIB_TFEL_MATH_ST2TOT2_CONCEPT_IXX 1

#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel {

  namespace math {

    template <class T>
    TFEL_MATH_INLINE typename ST2toT2Traits<T>::NumType
    ST2toT2Concept<T>::operator()(const unsigned short i,
                                  const unsigned short j) const {
      return static_cast<const T&>(*this).operator()(i, j);
    }  // end of ST2toT2Concept<T>::operator()

    template <typename ST2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond,
        typename tfel::typetraits::AbsType<
            typename ST2toT2Traits<ST2toT2Type>::NumType>::type>::type
    abs(const ST2toT2Type& v) {
      unsigned short i;
      unsigned short j;
      typedef typename ST2toT2Traits<ST2toT2Type>::NumType NumType;
      typedef typename tfel::typetraits::AbsType<NumType>::type AbsNumType;
      AbsNumType a(0);
      for (i = 0; i < TensorDimeToSize<ST2toT2Traits<ST2toT2Type>::dime>::value;
           ++i) {
        for (j = 0;
             j < StensorDimeToSize<ST2toT2Traits<ST2toT2Type>::dime>::value;
             ++j) {
          a += abs(v(i, j));
        }
      }
      return a;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_ST2TOT2_CONCEPT_IXX */
