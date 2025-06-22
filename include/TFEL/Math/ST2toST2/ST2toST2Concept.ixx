/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2Concept.ixx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_CONCEPT_IXX
#define LIB_TFEL_MATH_ST2TOST2_CONCEPT_IXX 1

#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/LU/LUDecomp.hxx"
#include "TFEL/Math/LU/TinyPermutation.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2TransposeExpr.hxx"

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

    template <typename ST2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond &&
            (ST2toST2Traits<ST2toST2Type>::dime == 1u) &&
            tfel::typetraits::IsScalar<ST2toST2NumType<ST2toST2Type>>::cond,
        typename ComputeUnaryResult<ST2toST2NumType<ST2toST2Type>,
                                    Power<3>>::Result>::type
    det(const ST2toST2Type& s) {
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

    template <typename ST2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond &&
            ((ST2toST2Traits<ST2toST2Type>::dime == 2u) ||
             (ST2toST2Traits<ST2toST2Type>::dime == 3u)) &&
            tfel::typetraits::IsScalar<ST2toST2NumType<ST2toST2Type>>::cond,
        typename ComputeUnaryResult<
            ST2toST2NumType<ST2toST2Type>,
            Power<ST2toST2Traits<ST2toST2Type>::dime>>::Result>::type
    det(const ST2toST2Type& s) {
      using real = ST2toST2NumType<ST2toST2Type>;
      constexpr const auto N = ST2toST2Traits<ST2toST2Type>::dime;
      constexpr const auto ts = StensorDimeToSize<N>::value;
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

#include "TFEL/Math/ST2toST2/ST2toST2ConceptPushForward.ixx"

#endif /* LIB_TFEL_MATH_ST2TOST2_CONCEPT_IXX */
