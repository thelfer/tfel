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

namespace tfel::math {

  TFEL_HOST_DEVICE constexpr auto abs(const T2toT2Concept auto& t) noexcept {
    using T2toT2Type = decltype(t);
    using NumType = numeric_type<T2toT2Type>;
    using AbsNumType = typename tfel::typetraits::AbsType<NumType>::type;
    constexpr auto ts =
        TensorDimeToSize<getSpaceDimension<T2toT2Type>()>::value;
    auto a = AbsNumType{};
    for (unsigned short i = 0; i < ts; ++i) {
      for (unsigned short j = 0; j < ts; ++j) {
        a += abs(t(i, j));
      }
    }
    return a;
  }

  TFEL_HOST_DEVICE constexpr auto det(const T2toT2Concept auto& s) noexcept {
    using T2toT2Type = decltype(s);
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
      constexpr auto ts = TensorDimeToSize<N>::value;
      using Result = unary_result_type<numeric_type<T2toT2Type>, Power<ts>>;
      using real = base_type<numeric_type<T2toT2Type>>;
      tmatrix<ts, ts, real> m;
      for (unsigned short i = 0; i != ts; ++i) {
        for (unsigned short j = 0; j != ts; ++j) {
          m(i, j) = base_type_cast(s(i, j));
        }
      }
      TinyPermutation<ts> p;
      const auto r = LUDecomp<false>::exe(m, p);
      if (!r.first) {
        return Result{};
      }
      auto v = base_type<real>{1};
      for (index_type<T2toT2Type> i = 0; i != ts; ++i) {
        v *= m(i, i);
      }
      return r.second == 1 ? Result{v} : -Result{v};
    }
  }  // end of det

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOT2_CONCEPT_IXX */
