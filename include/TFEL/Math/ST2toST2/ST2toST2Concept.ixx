/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2Concept.ixx
 * \brief
 * \author Thomas Helfer/Antoine Martin
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_CONCEPT_IXX
#define LIB_TFEL_MATH_ST2TOST2_CONCEPT_IXX 1

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/LU/LUDecomp.hxx"
#include "TFEL/Math/LU/TinyPermutation.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2TransposeExpr.hxx"

namespace tfel::math {
   
  TFEL_HOST_DEVICE constexpr auto abs(const ST2toST2Concept auto& v) noexcept {
    using ST2toST2Type = decltype(v);
    using NumType = numeric_type<ST2toST2Type>;
    using IndexType = index_type<ST2toST2Type>;
    using AbsNumType = typename tfel::typetraits::AbsType<NumType>::type;
    constexpr auto ssize =
        StensorDimeToSize<getSpaceDimension<ST2toST2Type>()>::value;
    auto a = AbsNumType{};
    for (IndexType i = 0; i < ssize; ++i) {
      for (IndexType j = 0; j < ssize; ++j) {
        a += abs(v(i, j));
      }
    }
    return a;
  }  // end of abs
   
   
  TFEL_HOST_DEVICE constexpr auto transpose(ST2toST2Concept auto&& t) noexcept {
    using ST2toST2Type = decltype(t);
    return Expr<EvaluationResult<ST2toST2Type>,
                ST2toST2TransposeExpr<decltype(t)>>(
        std::forward<ST2toST2Type>(t));
  }  // end of transpose
  
  
  TFEL_HOST_DEVICE constexpr auto trace(const ST2toST2Concept auto& A) noexcept {
    using ST2toST2Type = decltype(A);
    using NumType = numeric_type<ST2toST2Type>;
    using IndexType = index_type<ST2toST2Type>;
    constexpr auto size =
        StensorDimeToSize<getSpaceDimension<ST2toST2Type>()>::value;
    auto tr = NumType{};
    for (IndexType i = 0 ;i<size;i++){
       tr+=A(i,i);
    }
    return tr;
  } //end of trace
  
   TFEL_HOST_DEVICE constexpr auto quaddot(const ST2toST2Concept auto& A, const ST2toST2Concept auto& B) noexcept {
    return trace(A*B);
  } // end of quaddot
  
  TFEL_HOST_DEVICE constexpr auto norm(const ST2toST2Concept auto& A) noexcept {
    using ST2toST2Type = decltype(A); 
    using NumType = numeric_type<ST2toST2Type>;
    constexpr auto N =
        StensorDimeToSize<getSpaceDimension<ST2toST2Type>()>::value; 
    return NumType(power<1, 2>(NumType(quaddot(transpose(A),A))/NumType(N)));
  } // end of norm
   
   
  TFEL_HOST_DEVICE constexpr auto det(const ST2toST2Concept auto& s) noexcept {
    using ST2toST2Type = decltype(s);
    constexpr auto N = getSpaceDimension<ST2toST2Type>();
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
      constexpr auto ts = StensorDimeToSize<N>::value;
      using Result = UnaryResultType<numeric_type<ST2toST2Type>, Power<ts>>;
      using real = base_type<numeric_type<ST2toST2Type>>;
      tmatrix<ts, ts, real> m;
      tfel::fsalgo::transform<ts * ts>::exe(
          s.begin(), m.begin(), [](const auto v) { return base_type_cast(v); });
      TinyPermutation<ts> p;
      const auto r = LUDecomp<false>::exe(m, p);
      if (!r.first) {
        return Result{};
      }
      auto v = base_type<real>{1};
      for (const index_type<ST2toST2Type> i = 0; i != ts; ++i) {
        v *= m(i, i);
      }
      return r.second == 1 ? Result{v} : -Result{v};
    }
  }  // end of det

  template <ST2toST2Concept ST2toST2ResultType, TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void computePushForwardDerivative(
      ST2toST2ResultType& r,
      const TensorType& F) noexcept  //
      requires(tfel::typetraits::IsFundamentalNumericType<
               numeric_type<TensorType>>::cond) {
    constexpr auto N = getSpaceDimension<ST2toST2ResultType>();
    static_assert(getSpaceDimension<TensorType>() == N);
    using value_type = numeric_type<ST2toST2ResultType>;
    if constexpr (N == 1u) {
      constexpr auto zero = value_type{0};
      r(0, 0) = F[0] * F[0];
      r(1, 1) = F[1] * F[1];
      r(2, 2) = F[2] * F[2];
      r(1, 0) = r(2, 0) = r(0, 1) = zero;
      r(2, 1) = r(0, 2) = r(1, 2) = zero;
    } else if constexpr (N == 2u) {
      constexpr auto zero = value_type{0};
      constexpr auto cste = Cste<value_type>::sqrt2;
      constexpr auto icste = Cste<value_type>::isqrt2;
      r(0, 0) = F[0] * F[0];
      r(1, 0) = F[4] * F[4];
      r(3, 0) = F[0] * F[4] * cste;
      r(0, 1) = F[3] * F[3];
      r(1, 1) = F[1] * F[1];
      r(3, 1) = F[1] * F[3] * cste;
      r(2, 2) = F[2] * F[2];
      r(0, 3) = (F[0] * F[3] + F[0] * F[3]) * icste;
      r(1, 3) = (F[1] * F[4] + F[1] * F[4]) * icste;
      r(3, 3) = F[0] * F[1] + F[3] * F[4];
      r(2, 1) = r(0, 2) = r(1, 2) = r(3, 2) = r(2, 3) = r(2, 0) = zero;
    } else if constexpr (N == 3u) {
      constexpr auto cste = Cste<value_type>::sqrt2;
      constexpr auto icste = Cste<value_type>::isqrt2;
      r(0, 0) = F[0] * F[0];
      r(1, 0) = F[4] * F[4];
      r(2, 0) = F[6] * F[6];
      r(3, 0) = F[0] * F[4] * cste;
      r(4, 0) = F[0] * F[6] * cste;
      r(5, 0) = F[4] * F[6] * cste;
      r(0, 1) = F[3] * F[3];
      r(1, 1) = F[1] * F[1];
      r(2, 1) = F[8] * F[8];
      r(3, 1) = F[1] * F[3] * cste;
      r(4, 1) = F[3] * F[8] * cste;
      r(5, 1) = F[1] * F[8] * cste;
      r(0, 2) = F[5] * F[5];
      r(1, 2) = F[7] * F[7];
      r(2, 2) = F[2] * F[2];
      r(3, 2) = F[5] * F[7] * cste;
      r(4, 2) = F[2] * F[5] * cste;
      r(5, 2) = F[2] * F[7] * cste;
      r(0, 3) = (F[0] * F[3] + F[0] * F[3]) * icste;
      r(1, 3) = (F[1] * F[4] + F[1] * F[4]) * icste;
      r(2, 3) = (F[6] * F[8] + F[6] * F[8]) * icste;
      r(3, 3) = F[0] * F[1] + F[3] * F[4];
      r(4, 3) = F[0] * F[8] + F[3] * F[6];
      r(5, 3) = F[4] * F[8] + F[1] * F[6];
      r(0, 4) = (F[0] * F[5] + F[0] * F[5]) * icste;
      r(1, 4) = (F[4] * F[7] + F[4] * F[7]) * icste;
      r(2, 4) = (F[2] * F[6] + F[2] * F[6]) * icste;
      r(3, 4) = F[4] * F[5] + F[0] * F[7];
      r(4, 4) = F[5] * F[6] + F[0] * F[2];
      r(5, 4) = F[6] * F[7] + F[2] * F[4];
      r(0, 5) = 2 * F[3] * F[5] * icste;
      r(1, 5) = 2 * F[1] * F[7] * icste;
      r(2, 5) = 2 * F[2] * F[8] * icste;
      r(3, 5) = F[3] * F[7] + F[1] * F[5];
      r(4, 5) = F[5] * F[8] + F[2] * F[3];
      r(5, 5) = F[7] * F[8] + F[1] * F[2];
    }
  }  // end of computePushForwardDerivative

}  // namespace tfel::math

#include "TFEL/Math/ST2toST2/ST2toST2ConceptPushForward.ixx"

#endif /* LIB_TFEL_MATH_ST2TOST2_CONCEPT_IXX */
