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

#include "TFEL/Math/LU/LUDecomp.hxx"
#include "TFEL/Math/LU/TinyPermutation.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel::math {

  template <typename T2toT2Type>
  std::enable_if_t<
      implementsT2toT2Concept<T2toT2Type>(),
      typename tfel::typetraits::AbsType<numeric_type<T2toT2Type>>::type>
  abs(const T2toT2Type& v) {
    unsigned short i;
    unsigned short j;
    typedef numeric_type<T2toT2Type> NumType;
    typedef typename tfel::typetraits::AbsType<NumType>::type AbsNumType;
    AbsNumType a(0);
    for (i = 0; i < TensorDimeToSize<getSpaceDimension<T2toT2Type>()>::value;
         ++i) {
      for (j = 0; j < TensorDimeToSize<getSpaceDimension<T2toT2Type>()>::value;
           ++j) {
        a += abs(v(i, j));
      }
    }
    return a;
  }

  template <typename T2toT2Type>
  std::enable_if_t<
      implementsT2toT2Concept<T2toT2Type>() &&
          isScalar<numeric_type<T2toT2Type>>(),
      typename ComputeUnaryResult<numeric_type<T2toT2Type>, Power<3>>::Result>
  det(const T2toT2Type& s) {
    constexpr auto N = getSpaceDimension<T2toT2Type>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
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
    } else {
      using real = numeric_type<T2toT2Type>;
      constexpr auto ts = TensorDimeToSize<N>::value;
      tmatrix<ts, ts, real> m;
      TinyPermutation<ts> p;
      tfel::fsalgo::copy<ts * ts>::exe(s.begin(), m.begin());
      const auto r = LUDecomp<false>::exe(m, p);
      if (!r.first) {
        return {};
      }
      auto v = base_type<real>{1};
      for (index_type<T2toT2Type> i = 0; i != ts; ++i) {
        v *= m(i, i);
      }
      return r.second == 1 ? v : -v;
    }
  }  // end of det

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOT2_CONCEPT_IXX */
