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

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/LU/LUDecomp.hxx"
#include "TFEL/Math/LU/TinyPermutation.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2TransposeExpr.hxx"

namespace tfel::math {

  template <typename ST2toST2Type>
  std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>(),
      typename tfel::typetraits::AbsType<numeric_type<ST2toST2Type>>::type>
  abs(const ST2toST2Type& v) {
    using NumType = numeric_type<ST2toST2Type>;
    using IndexType = index_type<ST2toST2Type>;
    using AbsNumType = typename tfel::typetraits::AbsType<NumType>::type;
    constexpr auto ssize =
        StensorDimeToSize<getSpaceDimension<ST2toST2Type>()>::value;
    AbsNumType a(0);
    for (IndexType i = 0; i < ssize; ++i) {
      for (IndexType j = 0; j < ssize; ++j) {
        a += abs(v(i, j));
      }
    }
    return a;
  }

  template <typename ST2toST2Type>
  auto transpose(ST2toST2Type&& t)
      -> std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>(),
                          Expr<EvaluationResult<ST2toST2Type>,
                               ST2toST2TransposeExpr<decltype(t)>>> {
    return Expr<EvaluationResult<ST2toST2Type>,
                ST2toST2TransposeExpr<decltype(t)>>(
        std::forward<ST2toST2Type>(t));
  }

  template <typename ST2toST2Type>
  std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>() &&
          (getSpaceDimension<ST2toST2Type>() == 1u) &&
          isScalar<numeric_type<ST2toST2Type>>(),
      typename ComputeUnaryResult<numeric_type<ST2toST2Type>, Power<3>>::Result>
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
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       ((getSpaceDimension<ST2toST2Type>() == 2u) ||
                        (getSpaceDimension<ST2toST2Type>() == 3u)) &&
                       isScalar<numeric_type<ST2toST2Type>>(),
                   typename ComputeUnaryResult<
                       numeric_type<ST2toST2Type>,
                       Power<getSpaceDimension<ST2toST2Type>()>>::Result>
  det(const ST2toST2Type& s) {
    using real = numeric_type<ST2toST2Type>;
    constexpr auto N = getSpaceDimension<ST2toST2Type>();
    constexpr auto ts = StensorDimeToSize<N>::value;
    tmatrix<ts, ts, real> m;
    TinyPermutation<ts> p;
    tfel::fsalgo::copy<ts * ts>::exe(s.begin(), m.begin());
    const auto r = LUDecomp<false>::exe(m, p);
    if (!r.first) {
      return {};
    }
    auto v = base_type<real>{1};
    for (const index_type<ST2toST2Type> i = 0; i != ts; ++i) {
      v *= m(i, i);
    }
    return r.second == 1 ? v : -v;
  }  // end of det

}  // namespace tfel::math

#include "TFEL/Math/ST2toST2/ST2toST2ConceptPushForward.ixx"

#endif /* LIB_TFEL_MATH_ST2TOST2_CONCEPT_IXX */
