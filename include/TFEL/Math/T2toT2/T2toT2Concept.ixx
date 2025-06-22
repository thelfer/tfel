/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2Concept.ixx
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

#ifndef LIB_TFEL_MATH_T2TOT2_CONCEPT_IXX
#define LIB_TFEL_MATH_T2TOT2_CONCEPT_IXX 1

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/LU/LUDecomp.hxx"
#include "TFEL/Math/LU/TinyPermutation.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel {

  namespace math {

    template <class T>
    TFEL_MATH_INLINE typename T2toT2Traits<T>::NumType
    T2toT2Concept<T>::operator()(const unsigned short i,
                                 const unsigned short j) const {
      return static_cast<const T&>(*this).operator()(i, j);
    }  // end of T2toT2Concept<T>::operator()

    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond,
        typename tfel::typetraits::AbsType<
            typename T2toT2Traits<T2toT2Type>::NumType>::type>::type
    abs(const T2toT2Type& v) {
      unsigned short i;
      unsigned short j;
      typedef typename T2toT2Traits<T2toT2Type>::NumType NumType;
      typedef typename tfel::typetraits::AbsType<NumType>::type AbsNumType;
      AbsNumType a(0);
      for (i = 0; i < TensorDimeToSize<T2toT2Traits<T2toT2Type>::dime>::value;
           ++i) {
        for (j = 0; j < TensorDimeToSize<T2toT2Traits<T2toT2Type>::dime>::value;
             ++j) {
          a += abs(v(i, j));
        }
      }
      return a;
    }

    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            (T2toT2Traits<T2toT2Type>::dime == 1u) &&
            tfel::typetraits::IsScalar<T2toT2NumType<T2toT2Type>>::cond,
        typename ComputeUnaryResult<T2toT2NumType<T2toT2Type>,
                                    Power<3>>::Result>::type
    det(const T2toT2Type& s) {
      const auto a = s(0, 0);
      const auto b = s(0, 1);
      const auto c = s(0, 2);
      const auto d = s(1, 0);
      const auto e = s(1, 1);
      const auto f = s(1, 2);
      const auto g = s(2, 0);
      const auto h = s(2, 1);
      const auto i = s(2, 2);
      return a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g);
    }  // end of det

    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            ((T2toT2Traits<T2toT2Type>::dime == 2u) ||
             (T2toT2Traits<T2toT2Type>::dime == 3u)) &&
            tfel::typetraits::IsScalar<T2toT2NumType<T2toT2Type>>::cond,
        typename ComputeUnaryResult<
            T2toT2NumType<T2toT2Type>,
            Power<T2toT2Traits<T2toT2Type>::dime>>::Result>::type
    det(const T2toT2Type& s) {
      using real = T2toT2NumType<T2toT2Type>;
      constexpr const auto N = T2toT2Traits<T2toT2Type>::dime;
      constexpr const auto ts = TensorDimeToSize<N>::value;
      tmatrix<ts, ts, real> m;
      TinyPermutation<ts> p;
      tfel::fsalgo::copy<ts * ts>::exe(s.begin(), m.begin());
      int r = 1;
      try {
        r = LUDecomp::exe(m, p);
      } catch (...) {
        return {};
      }
      const auto v = DiagonalTermProduct<ts, real>::exe(m);
      return r == 1 ? v : -v;
    }  // end of det

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2_CONCEPT_IXX */
