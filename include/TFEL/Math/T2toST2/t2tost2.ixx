/*!
 * \file   include/TFEL/Math/T2toST2/t2tost2.ixx
 * \brief  This file implements the methods of the class t2tost2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2_IXX
#define LIB_TFEL_MATH_T2TOST2_IXX

#include <cmath>
#include <iterator>
#include <algorithm>
#include <type_traits>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/T2toST2/LeftCauchyGreenTensorDerivativeExpr.hxx"
#include "TFEL/Math/T2toST2/RightCauchyGreenTensorDerivativeExpr.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto t2tost2<N, T>::dCdF(
      const TensorType& F) noexcept
      requires(getSpaceDimension<TensorType>() == N &&
               isAssignableTo<numeric_type<TensorType>, T>()) {
    return Expr<t2tost2<N, T>, RightCauchyGreenTensorDerivativeExpr<N>>(F);
  }  // end of t2tost2::dCdF

  template <unsigned short N, typename T>
  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto t2tost2<N, T>::dBdF(
      const TensorType& F) noexcept
      requires(getSpaceDimension<TensorType>() == N &&
               isAssignableTo<numeric_type<TensorType>, T>()) {
    return Expr<t2tost2<N, T>, LeftCauchyGreenTensorDerivativeExpr<N>>(F);
  }  // end of t2tost2::dBdF

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void t2tost2<N, T>::import(
      const base_type<T>* const src) noexcept {
    tfel::fsalgo::transform<StensorDimeToSize<N>::value *
                            TensorDimeToSize<N>::value>::exe(src, this->begin(),
                                                             [](const auto& v) {
                                                               return T(v);
                                                             });
  }  // end of import

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void t2tost2<N, T>::copy(const auto p) noexcept {
    tfel::fsalgo::copy<StensorDimeToSize<N>::value *
                       TensorDimeToSize<N>::value>::exe(p, *this);
  }  // end of copy

  TFEL_HOST_DEVICE constexpr auto convertToT2toST2(
      const T2toT2Concept auto& t) noexcept {
    using T2toT2Type = decltype(t);
    using value_type = numeric_type<T2toT2Type>;
    constexpr auto N = getSpaceDimension<T2toT2Type>();
    if constexpr (N == 1) {
      t2tost2<1u, value_type> r;
      r(0, 0) = t(0, 0);
      r(0, 1) = t(0, 1);
      r(0, 2) = t(0, 2);
      r(1, 0) = t(1, 0);
      r(1, 1) = t(1, 1);
      r(1, 2) = t(1, 2);
      r(2, 0) = t(2, 0);
      r(2, 1) = t(2, 1);
      r(2, 2) = t(2, 2);
      return r;
    } else if constexpr (N == 2) {
      constexpr auto icste = Cste<value_type>::isqrt2;
      t2tost2<2u, value_type> r;
      r(0, 0) = t(0, 0);
      r(0, 3) = t(0, 3);
      r(0, 4) = t(0, 4);
      r(0, 1) = t(0, 1);
      r(0, 2) = t(0, 2);
      r(3, 0) = (t(3, 0) + t(4, 0)) * icste;
      r(3, 3) = (t(3, 3) + t(4, 3)) * icste;
      r(3, 4) = (t(3, 4) + t(4, 4)) * icste;
      r(3, 1) = (t(3, 1) + t(4, 1)) * icste;
      r(3, 2) = (t(3, 2) + t(4, 2)) * icste;
      r(1, 0) = t(1, 0);
      r(1, 3) = t(1, 3);
      r(1, 4) = t(1, 4);
      r(1, 1) = t(1, 1);
      r(1, 2) = t(1, 2);
      r(2, 0) = t(2, 0);
      r(2, 3) = t(2, 3);
      r(2, 4) = t(2, 4);
      r(2, 1) = t(2, 1);
      r(2, 2) = t(2, 2);
      return r;
    } else {
      constexpr auto icste = Cste<value_type>::isqrt2;
      t2tost2<3u, value_type> r;
      r(0, 0) = t(0, 0);
      r(0, 3) = t(0, 3);
      r(0, 5) = t(0, 5);
      r(0, 4) = t(0, 4);
      r(0, 1) = t(0, 1);
      r(0, 7) = t(0, 7);
      r(0, 6) = t(0, 6);
      r(0, 8) = t(0, 8);
      r(0, 2) = t(0, 2);
      r(3, 0) = (t(3, 0) + t(4, 0)) * icste;
      r(3, 3) = (t(3, 3) + t(4, 3)) * icste;
      r(3, 5) = (t(3, 5) + t(4, 5)) * icste;
      r(3, 4) = (t(3, 4) + t(4, 4)) * icste;
      r(3, 1) = (t(3, 1) + t(4, 1)) * icste;
      r(3, 7) = (t(3, 7) + t(4, 7)) * icste;
      r(3, 6) = (t(3, 6) + t(4, 6)) * icste;
      r(3, 8) = (t(3, 8) + t(4, 8)) * icste;
      r(3, 2) = (t(3, 2) + t(4, 2)) * icste;
      r(4, 0) = (t(5, 0) + t(6, 0)) * icste;
      r(4, 3) = (t(5, 3) + t(6, 3)) * icste;
      r(4, 5) = (t(5, 5) + t(6, 5)) * icste;
      r(4, 4) = (t(5, 4) + t(6, 4)) * icste;
      r(4, 1) = (t(5, 1) + t(6, 1)) * icste;
      r(4, 7) = (t(5, 7) + t(6, 7)) * icste;
      r(4, 6) = (t(5, 6) + t(6, 6)) * icste;
      r(4, 8) = (t(5, 8) + t(6, 8)) * icste;
      r(4, 2) = (t(5, 2) + t(6, 2)) * icste;
      r(1, 0) = t(1, 0);
      r(1, 3) = t(1, 3);
      r(1, 5) = t(1, 5);
      r(1, 4) = t(1, 4);
      r(1, 1) = t(1, 1);
      r(1, 7) = t(1, 7);
      r(1, 6) = t(1, 6);
      r(1, 8) = t(1, 8);
      r(1, 2) = t(1, 2);
      r(5, 0) = (t(7, 0) + t(8, 0)) * icste;
      r(5, 3) = (t(7, 3) + t(8, 3)) * icste;
      r(5, 5) = (t(7, 5) + t(8, 5)) * icste;
      r(5, 4) = (t(7, 4) + t(8, 4)) * icste;
      r(5, 1) = (t(7, 1) + t(8, 1)) * icste;
      r(5, 7) = (t(7, 7) + t(8, 7)) * icste;
      r(5, 6) = (t(7, 6) + t(8, 6)) * icste;
      r(5, 8) = (t(7, 8) + t(8, 8)) * icste;
      r(5, 2) = (t(7, 2) + t(8, 2)) * icste;
      r(2, 0) = t(2, 0);
      r(2, 3) = t(2, 3);
      r(2, 5) = t(2, 5);
      r(2, 4) = t(2, 4);
      r(2, 1) = t(2, 1);
      r(2, 7) = t(2, 7);
      r(2, 6) = t(2, 6);
      r(2, 8) = t(2, 8);
      r(2, 2) = t(2, 2);
      return r;
    }
  }  // end of convertToT2toST2

  TFEL_HOST_DEVICE constexpr auto computeRateOfDeformationDerivative(
      const TensorConcept auto& F) noexcept {
    using TensorType = decltype(F);
    using value_type = numeric_type<TensorType>;
    constexpr auto N = getSpaceDimension<TensorType>();
    using Result = t2tost2<N, numeric_type<TensorType>>;
    if constexpr (N == 1) {
      const auto iF = invert(F);
      constexpr auto zero = value_type(0);
      return Result{iF[0], zero, zero, zero, iF[1], zero, zero, zero, iF[2]};
    } else if constexpr (N == 2) {
      constexpr auto icste = Cste<value_type>::isqrt2;
      constexpr auto zero = value_type(0);
      const auto iF = invert(F);
      return Result{iF[0],         zero,  zero,          iF[4],
                    zero,          zero,  iF[1],         zero,
                    zero,          iF[3], zero,          zero,
                    iF[2],         zero,  zero,          iF[3] * icste,
                    iF[4] * icste, zero,  iF[1] * icste, iF[0] * icste};
    } else {
      constexpr auto icste = Cste<value_type>::isqrt2;
      constexpr auto zero = value_type(0);
      const auto iF = invert(F);
      return Result{iF[0],         zero,          zero,          iF[4],
                    zero,          iF[6],         zero,          zero,
                    zero,          zero,          iF[1],         zero,
                    zero,          iF[3],         zero,          zero,
                    iF[8],         zero,          zero,          zero,
                    iF[2],         zero,          zero,          zero,
                    iF[5],         zero,          iF[7],         iF[3] * icste,
                    iF[4] * icste, zero,          iF[1] * icste, iF[0] * icste,
                    iF[8] * icste, zero,          iF[6] * icste, zero,
                    iF[5] * icste, zero,          iF[6] * icste, iF[7] * icste,
                    zero,          iF[2] * icste, iF[0] * icste, zero,
                    iF[4] * icste, zero,          iF[7] * icste, iF[8] * icste,
                    zero,          iF[5] * icste, zero,          iF[3] * icste,
                    iF[2] * icste, iF[1] * icste};
    }
  }

  template <T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto
  computeCauchyStressDerivativeFromKirchhoffStressDerivative(
      const T2toST2Type& dt,
      const StensorType& s,
      const TensorType& F) noexcept  //
      requires(getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<StensorType>() &&
               getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<TensorType>() &&
               tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<TensorType>>::cond) {
    using stress =
        typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                     numeric_type<StensorType>, OpPlus>::Result;
    t2tost2<getSpaceDimension<T2toST2Type>(), stress> r;
    computeCauchyStressDerivativeFromKirchhoffStressDerivative(r, dt, s, F);
    return r;
  }

  template <T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto
  computeKirchhoffStressDerivativeFromCauchyStressDerivative(
      const T2toST2Type& ds,
      const StensorType& s,
      const TensorType& F) noexcept  //
      requires(getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<StensorType>() &&
               getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<TensorType>() &&
               tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<TensorType>>::cond) {
    using stress =
        typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                     numeric_type<StensorType>, OpPlus>::Result;
    t2tost2<getSpaceDimension<T2toST2Type>(), stress> r;
    computeKirchhoffStressDerivativeFromCauchyStressDerivative(r, ds, s, F);
    return r;
  }

  template <T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto computePushForwardDerivative(
      const T2toST2Type& K,
      const StensorType& S,
      const TensorType& F) noexcept  //
      requires(getSpaceDimension<StensorType>() ==
                   getSpaceDimension<T2toST2Type>() &&
               getSpaceDimension<TensorType>() ==
                   getSpaceDimension<T2toST2Type>() &&
               tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<TensorType>>::cond) {
    using stress =
        typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                     numeric_type<StensorType>, OpPlus>::Result;
    t2tost2<getSpaceDimension<T2toST2Type>(), stress> r;
    computePushForwardDerivative(r, K, S, F);
    return r;
  }

  template <T2toST2Concept T2toST2ResultType,
            T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void computePushForwardDerivative(
      T2toST2ResultType& dT_dF,
      const T2toST2Type& dS_dF,
      const StensorType& S,
      const TensorType& F) noexcept  //
      requires(tfel::typetraits::IsFundamentalNumericType<
               numeric_type<TensorType>>::cond&&
                   isAssignableTo<
                       typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                    numeric_type<StensorType>,
                                                    OpPlus>::Result,
                       numeric_type<T2toST2ResultType>>()) {
    constexpr auto N = getSpaceDimension<T2toST2ResultType>();
    static_assert(getSpaceDimension<T2toST2Type>() == N);
    static_assert(getSpaceDimension<StensorType>() == N);
    static_assert(getSpaceDimension<TensorType>() == N);
    using value_type = numeric_type<T2toST2ResultType>;
    using real = base_type<value_type>;
    auto d1 = t2tost2<N, value_type>{};
    computePushForwardDerivativeWithRespectToDeformationGradient(d1, S, F);
    auto d2 = st2tost2<N, real>{};
    computePushForwardDerivative(d2, F);
    dT_dF = d1 + d2 * dS_dF;
  }  // end of computePushForwardDerivative

  template <T2toST2Concept T2toST2Type>
  TFEL_HOST_DEVICE constexpr auto change_basis(
      const T2toST2Type& s,
      const rotation_matrix<numeric_type<T2toST2Type>>& r) noexcept {
    constexpr auto N = getSpaceDimension<T2toST2Type>();
    using Result = t2tost2<N, numeric_type<T2toST2Type>>;
    if constexpr (N == 1) {
      return Result{s};
    } else {
      using t2tot2 = tfel::math::t2tot2<N, numeric_type<T2toST2Type>>;
      using st2tost2 = tfel::math::st2tost2<N, numeric_type<T2toST2Type>>;
      const auto sr = st2tost2::fromRotationMatrix(r);
      const auto sir = t2tot2::fromRotationMatrix(transpose(r));
      return Result{sr * s * sir};
    }
  }  // end of change_basis

  template <unsigned short N, typename ValueType>
  template <std::size_t... d>
  TFEL_HOST_DEVICE constexpr t2tost2<N, ValueType>::t2tost2(
      ValueType const (&... arrays)[d])  //
      requires((sizeof...(d) == StensorDimeToSize<N>::value) &&
               ((d == TensorDimeToSize<N>::value) && ...)) {
    auto init_row = [this](
                        const typename t2tost2::size_type i,
                        ValueType const(&values)[TensorDimeToSize<N>::value]) {
      for (typename t2tost2::size_type j = 0u; j < TensorDimeToSize<N>::value;
           ++j) {
        this->operator()(i, j) = values[j];
      }
    };
    auto i = typename t2tost2::size_type{};
    (init_row(i++, arrays), ...);
  }  // end of t2tost2

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOST2_IXX */
