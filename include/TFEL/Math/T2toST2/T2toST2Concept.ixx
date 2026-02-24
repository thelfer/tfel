/*!
 * \file   include/TFEL/Math/T2toST2/T2toST2Concept.ixx
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

#ifndef LIB_TFEL_MATH_T2TOST2_CONCEPT_IXX
#define LIB_TFEL_MATH_T2TOST2_CONCEPT_IXX 1

#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel::math {

  TFEL_HOST_DEVICE constexpr auto abs(const T2toST2Concept auto& t) noexcept {
    using T2toST2Type = decltype(t);
    using NumType = numeric_type<T2toST2Type>;
    using AbsNumType = typename tfel::typetraits::AbsType<NumType>::type;
    auto a = AbsNumType{};
    constexpr auto ssize =
        StensorDimeToSize<getSpaceDimension<T2toST2Type>()>::value;
    constexpr auto tsize =
        TensorDimeToSize<getSpaceDimension<T2toST2Type>()>::value;
    for (unsigned short i = 0; i < ssize; ++i) {
      for (unsigned short j = 0; j < tsize; ++j) {
        a += abs(t(i, j));
      }
    }
    return a;
  }  // end of abs

  template <T2toST2Concept T2toST2ResultType,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void
  computePushForwardDerivativeWithRespectToDeformationGradient(
      T2toST2ResultType& dTdF,
      const StensorType& S,
      const TensorType& F) noexcept  //
      requires(tfel::typetraits::IsFundamentalNumericType<
               numeric_type<TensorType>>::cond&&
                   isAssignableTo<numeric_type<StensorType>,
                                  numeric_type<T2toST2ResultType>>()) {
    using value_type = numeric_type<T2toST2ResultType>;
    constexpr auto N = getSpaceDimension<T2toST2ResultType>();
    static_assert(getSpaceDimension<StensorType>() == N);
    static_assert(getSpaceDimension<TensorType>() == N);
    constexpr auto zero = value_type{0};
    if (N == 1u) {
      // derivative with respect to F0
      dTdF(0, 0) = 2 * F[0] * S[0];
      dTdF(1, 0) = dTdF(2, 0) = zero;
      // derivative with respect to F1
      dTdF(1, 1) = 2 * F[1] * S[1];
      dTdF(0, 1) = dTdF(2, 1) = zero;
      // derivative with respect to F1
      dTdF(0, 2) = dTdF(1, 2) = zero;
      dTdF(2, 2) = 2 * F[2] * S[2];
    } else if (N == 2u) {
      constexpr auto cste = Cste<value_type>::sqrt2;
      constexpr auto icste = Cste<value_type>::isqrt2;
      // derivative with respect to F0
      dTdF(0, 0) = 2 * (F[0] * S[0] + F[3] * S[3] * icste);
      dTdF(3, 0) = F[1] * S[3] + cste * F[4] * S[0];
      dTdF(1, 0) = dTdF(2, 0) = zero;
      // derivative with respect to F1
      dTdF(1, 1) = 2 * (F[1] * S[1] + F[4] * S[3] * icste);
      dTdF(3, 1) = F[0] * S[3] + cste * F[3] * S[1];
      dTdF(0, 1) = dTdF(2, 1) = zero;
      // derivative with respect to F2
      dTdF(2, 2) = 2 * F[2] * S[2];
      dTdF(0, 2) = dTdF(1, 2) = dTdF(3, 2) = zero;
      // derivative with respect to F3
      dTdF(0, 3) = 2 * (F[0] * S[3] * icste + F[3] * S[1]);
      dTdF(3, 3) = F[4] * S[3] + cste * F[1] * S[1];
      dTdF(1, 3) = dTdF(2, 3) = zero;
      // derivative with respect to F4
      dTdF(1, 4) = 2 * (F[1] * S[3] * icste + F[4] * S[0]);
      dTdF(3, 4) = F[3] * S[3] + cste * F[0] * S[0];
      dTdF(0, 4) = dTdF(2, 4) = zero;
    } else if (N == 3u) {
      constexpr auto cste = Cste<value_type>::sqrt2;
      constexpr auto icste = Cste<value_type>::isqrt2;
      // derivative with respect to F0
      dTdF(0, 0) =
          2 * (F[3] * S[3] * icste + F[5] * S[4] * icste + F[0] * S[0]);
      dTdF(3, 0) = F[7] * S[4] + F[1] * S[3] + cste * F[4] * S[0];
      dTdF(4, 0) = F[2] * S[4] + F[8] * S[3] + cste * F[6] * S[0];
      dTdF(1, 0) = dTdF(2, 0) = dTdF(5, 0) = zero;
      // derivative with respect to F1
      dTdF(1, 1) =
          2 * (F[1] * S[1] + F[7] * S[5] * icste + F[4] * S[3] * icste);
      dTdF(3, 1) = F[5] * S[5] + F[0] * S[3] + cste * F[3] * S[1];
      dTdF(5, 1) = F[2] * S[5] + F[6] * S[3] + cste * F[8] * S[1];
      dTdF(0, 1) = dTdF(2, 1) = dTdF(4, 1) = zero;
      // derivative with respect to F2
      dTdF(2, 2) =
          2 * (F[2] * S[2] + F[8] * S[5] * icste + F[6] * S[4] * icste);
      dTdF(4, 2) = F[3] * S[5] + F[0] * S[4] + cste * F[5] * S[2];
      dTdF(5, 2) = F[1] * S[5] + F[4] * S[4] + cste * F[7] * S[2];
      dTdF(0, 2) = dTdF(1, 2) = dTdF(3, 2) = zero;
      // derivative with respect to F3
      dTdF(0, 3) =
          2 * (F[3] * S[1] + F[5] * S[5] * icste + F[0] * S[3] * icste);
      dTdF(3, 3) = F[7] * S[5] + F[4] * S[3] + cste * F[1] * S[1];
      dTdF(4, 3) = F[2] * S[5] + F[6] * S[3] + cste * F[8] * S[1];
      dTdF(1, 3) = dTdF(2, 3) = dTdF(5, 3) = zero;
      // derivative with respect to F4
      dTdF(1, 4) =
          2 * (F[1] * S[3] * icste + F[7] * S[4] * icste + F[4] * S[0]);
      dTdF(3, 4) = F[5] * S[4] + F[3] * S[3] + cste * F[0] * S[0];
      dTdF(5, 4) = F[2] * S[4] + F[8] * S[3] + cste * F[6] * S[0];
      dTdF(0, 4) = dTdF(2, 4) = dTdF(4, 4) = zero;
      // derivative with respect to F5
      dTdF(0, 5) =
          2 * (F[3] * S[5] * icste + F[5] * S[2] + F[0] * S[4] * icste);
      dTdF(3, 5) = F[1] * S[5] + F[4] * S[4] + cste * F[7] * S[2];
      dTdF(4, 5) = F[8] * S[5] + F[6] * S[4] + cste * F[2] * S[2];
      dTdF(1, 5) = dTdF(2, 5) = dTdF(5, 5) = zero;
      // derivative with respect to F6
      dTdF(2, 6) =
          2 * (F[2] * S[4] * icste + F[8] * S[3] * icste + F[6] * S[0]);
      dTdF(4, 6) = F[5] * S[4] + F[3] * S[3] + cste * F[0] * S[0];
      dTdF(5, 6) = F[7] * S[4] + F[1] * S[3] + cste * F[4] * S[0];
      dTdF(0, 6) = dTdF(1, 6) = dTdF(3, 6) = zero;
      // derivative with respect to F7
      dTdF(1, 7) =
          2 * (F[1] * S[5] * icste + F[4] * S[4] * icste + F[7] * S[2]);
      dTdF(3, 7) = F[3] * S[5] + F[0] * S[4] + cste * F[5] * S[2];
      dTdF(5, 7) = F[8] * S[5] + F[6] * S[4] + cste * F[2] * S[2];
      dTdF(0, 7) = dTdF(2, 7) = dTdF(4, 7) = zero;
      // derivative with respect to F8
      dTdF(2, 8) =
          2 * (F[2] * S[5] * icste + F[8] * S[1] + F[6] * S[3] * icste);
      dTdF(4, 8) = F[5] * S[5] + F[0] * S[3] + cste * F[3] * S[1];
      dTdF(5, 8) = F[7] * S[5] + F[4] * S[3] + cste * F[1] * S[1];
      dTdF(0, 8) = dTdF(1, 8) = dTdF(3, 8) = zero;
    }
  }  // end of computePushForwardDerivativeWithRespectToDeformationGradient

  template <T2toST2Concept T2toST2ResultType,
            T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void
  computeCauchyStressDerivativeFromKirchhoffStressDerivative(
      T2toST2ResultType& ds,
      const T2toST2Type& dt_K,
      const StensorType& s,
      const TensorType& F) noexcept  //
      requires(
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>()) {
    const auto iJ = 1 / det(F);
    const auto dJ = computeDeterminantDerivative(F);
    ds = iJ * (dt_K - (s ^ dJ));
  }

  template <T2toST2Concept T2toST2ResultType,
            T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void
  computeKirchhoffStressDerivativeFromCauchyStressDerivative(
      T2toST2ResultType& dt_K,
      const T2toST2Type& ds,
      const StensorType& s,
      const TensorType& F) noexcept  //
      requires(
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>()) {
    const auto J = det(F);
    const auto dJ = computeDeterminantDerivative(F);
    dt_K = J * ds + (s ^ dJ);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOST2_CONCEPT_IXX */
