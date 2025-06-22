/*!
 * \file   include/TFEL/Math/T2toT2/t2tot2.ixx
 * \brief  This file implements the methods of the class t2tot2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2IXX
#define LIB_TFEL_MATH_T2TOT2IXX

#include <cmath>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/T2toT2/TensorProductLeftDerivativeExpr.hxx"
#include "TFEL/Math/T2toT2/TensorProductRightDerivativeExpr.hxx"

#if defined(__GNUG__) && (not defined(__CLANG__)) && \
    (not defined(__INTEL_COMPILER))
#if ((__GNUC__ > 4) ||                            \
     ((__GNUC__ == 4) && ((__GNUC_MINOR__ > 9) || \
                          (__GNUC_MINOR__ == 9 && __GNUC_PATCHLEVEL__ > 0))))
#define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#else
// don't define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#endif
#elif defined(_MSC_VER)
// don't define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#else
#define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#endif

namespace tfel::math {

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE t2tot2<N, base_type<T>> t2tot2<N, T>::fromRotationMatrix(
      const rotation_matrix<T>& r) {
    using base = base_type<T>;
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return t2tot2<N, base>::Id();
    } else if constexpr (N == 2) {
      constexpr auto zero = base{0};
      constexpr auto one = base{1};
      tfel::math::t2tot2<2u, base> rt;
      const auto a = r(0, 0);
      const auto b = r(0, 1);
      const auto d = r(1, 0);
      const auto e = r(1, 1);
      // a*d*v[4]+a*d*v[3]+d*d*v[1]+a*a*v[0];
      rt(0, 4) = a * d;
      rt(0, 3) = a * d;
      rt(0, 2) = zero;
      rt(0, 1) = d * d;
      rt(0, 0) = a * a;
      // b*e*v[4]+b*e*v[3]+e*e*v[1]+b*b*v[0];
      rt(1, 4) = b * e;
      rt(1, 3) = b * e;
      rt(1, 2) = zero;
      rt(1, 1) = e * e;
      rt(1, 0) = b * b;
      // b*d*v[4]+a*e*v[3]+d*e*v[1]+a*b*v[0];
      rt(3, 4) = b * d;
      rt(3, 3) = a * e;
      rt(3, 2) = zero;
      rt(3, 1) = d * e;
      rt(3, 0) = a * b;
      // a*e*v[4]+b*d*v[3]+d*e*v[1]+a*b*v[0];
      rt(4, 4) = a * e;
      rt(4, 3) = b * d;
      rt(4, 2) = zero;
      rt(4, 1) = d * e;
      rt(4, 0) = a * b;
      // the last one
      rt(2, 4) = zero;
      rt(2, 3) = zero;
      rt(2, 2) = one;
      rt(2, 1) = zero;
      rt(2, 0) = zero;
      return rt;
    } else {
      const auto a = r(0, 0);
      const auto b = r(0, 1);
      const auto c = r(0, 2);
      const auto d = r(1, 0);
      const auto e = r(1, 1);
      const auto f = r(1, 2);
      const auto g = r(2, 0);
      const auto h = r(2, 1);
      const auto i = r(2, 2);
      tfel::math::t2tot2<3u, T> rt;
      // d*g*v[8]+d*g*v[7]+a*g*v[6]+a*g*v[5]+a*d*v[4]+a*d*v[3]+g*g*v[2]+d*d*v[1]+a*a*v[0];
      rt(0, 0) = a * a;
      rt(0, 1) = d * d;
      rt(0, 2) = g * g;
      rt(0, 3) = a * d;
      rt(0, 4) = a * d;
      rt(0, 5) = a * g;
      rt(0, 6) = a * g;
      rt(0, 7) = d * g;
      rt(0, 8) = d * g;
      // e*h*v[8]+e*h*v[7]+b*h*v[6]+b*h*v[5]+b*e*v[4]+b*e*v[3]+h*h*v[2]+e*e*v[1]+b*b*v[0];
      rt(1, 0) = b * b;
      rt(1, 1) = e * e;
      rt(1, 2) = h * h;
      rt(1, 3) = b * e;
      rt(1, 4) = b * e;
      rt(1, 5) = b * h;
      rt(1, 6) = b * h;
      rt(1, 7) = e * h;
      rt(1, 8) = e * h;
      // f*i*v[8]+f*i*v[7]+c*i*v[6]+c*i*v[5]+c*f*v[4]+c*f*v[3]+i*i*v[2]+f*f*v[1]+c*c*v[0];
      rt(2, 0) = c * c;
      rt(2, 1) = f * f;
      rt(2, 2) = i * i;
      rt(2, 3) = c * f;
      rt(2, 4) = c * f;
      rt(2, 5) = c * i;
      rt(2, 6) = c * i;
      rt(2, 7) = f * i;
      rt(2, 8) = f * i;
      // e*g*v[8]+d*h*v[7]+b*g*v[6]+a*h*v[5]+b*d*v[4]+a*e*v[3]+g*h*v[2]+d*e*v[1]+a*b*v[0];
      rt(3, 8) = e * g;
      rt(3, 7) = d * h;
      rt(3, 6) = b * g;
      rt(3, 5) = a * h;
      rt(3, 4) = b * d;
      rt(3, 3) = a * e;
      rt(3, 2) = g * h;
      rt(3, 1) = d * e;
      rt(3, 0) = a * b;
      // d*h*v[8]+e*g*v[7]+a*h*v[6]+b*g*v[5]+a*e*v[4]+b*d*v[3]+g*h*v[2]+d*e*v[1]+a*b*v[0];
      rt(4, 8) = d * h;
      rt(4, 7) = e * g;
      rt(4, 6) = a * h;
      rt(4, 5) = b * g;
      rt(4, 4) = a * e;
      rt(4, 3) = b * d;
      rt(4, 2) = g * h;
      rt(4, 1) = d * e;
      rt(4, 0) = a * b;
      // f*g*v[8]+d*i*v[7]+c*g*v[6]+a*i*v[5]+c*d*v[4]+a*f*v[3]+g*i*v[2]+d*f*v[1]+a*c*v[0];
      rt(5, 8) = f * g;
      rt(5, 7) = d * i;
      rt(5, 6) = c * g;
      rt(5, 5) = a * i;
      rt(5, 4) = c * d;
      rt(5, 3) = a * f;
      rt(5, 2) = g * i;
      rt(5, 1) = d * f;
      rt(5, 0) = a * c;
      // d*i*v[8]+f*g*v[7]+a*i*v[6]+c*g*v[5]+a*f*v[4]+c*d*v[3]+g*i*v[2]+d*f*v[1]+a*c*v[0];
      rt(6, 8) = d * i;
      rt(6, 7) = f * g;
      rt(6, 6) = a * i;
      rt(6, 5) = c * g;
      rt(6, 4) = a * f;
      rt(6, 3) = c * d;
      rt(6, 2) = g * i;
      rt(6, 1) = d * f;
      rt(6, 0) = a * c;
      // f*h*v[8]+e*i*v[7]+c*h*v[6]+b*i*v[5]+c*e*v[4]+b*f*v[3]+h*i*v[2]+e*f*v[1]+b*c*v[0];
      rt(7, 8) = f * h;
      rt(7, 7) = e * i;
      rt(7, 6) = c * h;
      rt(7, 5) = b * i;
      rt(7, 4) = c * e;
      rt(7, 3) = b * f;
      rt(7, 2) = h * i;
      rt(7, 1) = e * f;
      rt(7, 0) = b * c;
      // e*i*v[8]+f*h*v[7]+b*i*v[6]+c*h*v[5]+b*f*v[4]+c*e*v[3]+h*i*v[2]+e*f*v[1]+b*c*v[0];
      rt(8, 8) = e * i;
      rt(8, 7) = f * h;
      rt(8, 6) = b * i;
      rt(8, 5) = c * h;
      rt(8, 4) = b * f;
      rt(8, 3) = c * e;
      rt(8, 2) = h * i;
      rt(8, 1) = e * f;
      rt(8, 0) = b * c;
      return rt;
    }
  }  // end of t2tot2<N,T>::fromRotationMatrix

  template <unsigned short N, typename T>
  template <typename TensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<implementsTensorConcept<TensorType>() &&
                           getSpaceDimension<TensorType>() == N &&
                           isAssignableTo<numeric_type<TensorType>, T>(),
                       Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>
      t2tot2<N, T>::tpld(const TensorType& B) {
    return Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>(B);
  }  // end of t2tot2<N,T>

  template <unsigned short N, typename T>
  template <typename TensorType, typename T2toT2Type>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsTensorConcept<TensorType>() &&
          implementsT2toT2Concept<T2toT2Type>() &&
          getSpaceDimension<TensorType>() == N &&
          getSpaceDimension<T2toT2Type>() == N &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<TensorType>,
                                                      numeric_type<T2toT2Type>,
                                                      OpMult>::Result,
                         T>(),
      Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>
  t2tot2<N, T>::tpld(const TensorType& B, const T2toT2Type& C) {
    return Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>(B, C);
  }

  template <unsigned short N, typename T>
  template <typename TensorType>
  TFEL_HOST_DEVICE
      std::enable_if_t<implementsTensorConcept<TensorType>() &&
                           getSpaceDimension<TensorType>() == N &&
                           isAssignableTo<numeric_type<TensorType>, T>(),
                       Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>
      t2tot2<N, T>::tprd(const TensorType& A) {
    return Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>(A);
  }

  template <unsigned short N, typename T>
  template <typename TensorType, typename T2toT2Type>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsTensorConcept<TensorType>() &&
          implementsT2toT2Concept<T2toT2Type>() &&
          getSpaceDimension<TensorType>() == N &&
          getSpaceDimension<T2toT2Type>() == N &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<TensorType>,
                                                      numeric_type<T2toT2Type>,
                                                      OpMult>::Result,
                         T>(),
      Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>
  t2tot2<N, T>::tprd(const TensorType& A, const T2toT2Type& C) {
    return Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>(A, C);
  }

  template <unsigned short N, typename T>
  constexpr tfel::math::t2tot2<N, base_type<T>>
  t2tot2<N, T>::transpose_derivative() {
    using base = base_type<T>;
    constexpr auto c0 = base{0};
    constexpr auto c1 = base{1};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c1, c0, c0,  //
              c0, c1, c0,  //
              c0, c0, c1};
    } else if constexpr (N == 2) {
      return {c1, c0, c0, c0, c0,  //
              c0, c1, c0, c0, c0,  //
              c0, c0, c1, c0, c0,  //
              c0, c0, c0, c0, c1,  //
              c0, c0, c0, c1, c0};
    } else {
      return {c1, c0, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c1, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c1, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c1, c0, c0, c0, c0,  //
              c0, c0, c0, c1, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c1, c0, c0,  //
              c0, c0, c0, c0, c0, c1, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c1,  //
              c0, c0, c0, c0, c0, c0, c0, c1, c0};
    }
  }

  template <unsigned short N, typename T>
  constexpr t2tot2<N, base_type<T>> t2tot2<N, T>::Id() {
    using base = base_type<T>;
    constexpr auto c0 = base{0};
    constexpr auto c1 = base{1};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c1, c0, c0,  //
              c0, c1, c0,  //
              c0, c0, c1};
    } else if constexpr (N == 2) {
      return {c1, c0, c0, c0, c0,  //
              c0, c1, c0, c0, c0,  //
              c0, c0, c1, c0, c0,  //
              c0, c0, c0, c1, c0,  //
              c0, c0, c0, c0, c1};
    } else {
      return {c1, c0, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c1, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c1, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c1, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c1, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c1, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c1, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c1, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c1};
    }
  }  // end of Id

  template <unsigned short N, typename T>
  constexpr t2tot2<N, base_type<T>> t2tot2<N, T>::IxI() {
    using base = base_type<T>;
    constexpr auto c1 = base{1};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c1, c1, c1,  //
              c1, c1, c1,  //
              c1, c1, c1};
    } else if constexpr (N == 2) {
      constexpr auto c0 = base{0};
      return {c1, c1, c1, c0, c0,  //
              c1, c1, c1, c0, c0,  //
              c1, c1, c1, c0, c0,  //
              c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0};
    } else {
      constexpr auto c0 = base{0};
      return {c1, c1, c1, c0, c0, c0, c0, c0, c0,  //
              c1, c1, c1, c0, c0, c0, c0, c0, c0,  //
              c1, c1, c1, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0, c0, c0, c0};
    }
  }  // end of IxI

  template <unsigned short N, typename T>
  constexpr t2tot2<N, base_type<T>> t2tot2<N, T>::K() {
    using base = base_type<T>;
    static_assert((N == 1) || (N == 2) || (N == 3));
    constexpr auto c2_3 = base{2} / base{3};
    constexpr auto mc1_3 = -base{1} / base{3};
    if constexpr (N == 1) {
      return {c2_3,  mc1_3, mc1_3,  //
              mc1_3, c2_3,  mc1_3,  //
              mc1_3, mc1_3, c2_3};
    } else if constexpr (N == 2) {
      constexpr auto c0 = base{0};
      constexpr auto c1 = base{1};
      return {c2_3,  mc1_3, mc1_3, c0, c0,  //
              mc1_3, c2_3,  mc1_3, c0, c0,  //
              mc1_3, mc1_3, c2_3,  c0, c0,  //
              c0,    c0,    c0,    c1, c0,  //
              c0,    c0,    c0,    c0, c1};
    } else {
      constexpr auto c0 = base{0};
      constexpr auto c1 = base{1};
      return {c2_3,  mc1_3, mc1_3, c0, c0, c0, c0, c0, c0,  //
              mc1_3, c2_3,  mc1_3, c0, c0, c0, c0, c0, c0,  //
              mc1_3, mc1_3, c2_3,  c0, c0, c0, c0, c0, c0,  //
              c0,    c0,    c0,    c1, c0, c0, c0, c0, c0,  //
              c0,    c0,    c0,    c0, c1, c0, c0, c0, c0,  //
              c0,    c0,    c0,    c0, c0, c1, c0, c0, c0,  //
              c0,    c0,    c0,    c0, c0, c0, c1, c0, c0,  //
              c0,    c0,    c0,    c0, c0, c0, c0, c1, c0,  //
              c0,    c0,    c0,    c0, c0, c0, c0, c0, c1};
    }
  }  // end of K

  template <unsigned short N, typename T>
  template <
      typename T2toST2Type,
      std::enable_if_t<((implementsT2toST2Concept<T2toST2Type>()) &&
                        (isAssignableTo<numeric_type<T2toST2Type>, T>()) &&
                        (getSpaceDimension<T2toST2Type>() == N)),
                       bool>>
  constexpr t2tot2<N, T>::t2tot2(const T2toST2Type& s) {
    convert(*this, s);
  }  // end of t2tot2<N, T>::t2tot2

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_HOST_DEVICE TFEL_MATH_INLINE2 void t2tot2<N, T>::copy(
      const InputIterator src) {
    tfel::fsalgo::copy<TensorDimeToSize<N>::value *
                       TensorDimeToSize<N>::value>::exe(src, *this);
  }

  template <typename T2toT2Type>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsT2toT2Concept<T2toT2Type>(),
      t2tot2<getSpaceDimension<T2toT2Type>(), numeric_type<T2toT2Type>>>
  change_basis(const T2toT2Type& s,
               const rotation_matrix<numeric_type<T2toT2Type>>& r) {
    constexpr auto N = getSpaceDimension<T2toT2Type>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return s;
    } else {
      using real = base_type<numeric_type<T2toT2Type>>;
      using t2tot2 = tfel::math::t2tot2<getSpaceDimension<T2toT2Type>(), real>;
      const auto sr = t2tot2::fromRotationMatrix(r);
      const auto sir = t2tot2::fromRotationMatrix(transpose(r));
      return sr * s * sir;
    }
  }  // end of change_basis

  template <typename TensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      t2tot2<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  computeVelocityGradientDerivative(const TensorType& F) {
    using res =
        t2tot2<getSpaceDimension<TensorType>(), numeric_type<TensorType>>;
    const auto iF = invert(F);
    return res::tpld(iF);
  }

  template <typename TensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      t2tot2<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  computeSpinRateDerivative(const TensorType& F) {
    constexpr auto N = getSpaceDimension<TensorType>();
    using value_type = numeric_type<TensorType>;
    using base = base_type<TensorType>;
    using res = t2tot2<N, value_type>;
    using resbase = t2tot2<N, base>;
    const auto iF = invert(F);
    const auto itF = invert(transpose(F));
    const auto dt = resbase::transpose_derivative();
    return eval((res::tpld(iF) - res::tprd(itF, dt)) / 2);
  }

  template <typename TensorType>
  TFEL_HOST_DEVICE std::enable_if_t<
      implementsTensorConcept<TensorType>() &&
          isScalar<numeric_type<TensorType>>(),
      t2tot2<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  computeDeterminantSecondDerivative(const TensorType& t) {
    constexpr auto N = getSpaceDimension<TensorType>();
    using NumType = numeric_type<TensorType>;
    constexpr auto zero = NumType{0};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {zero, t[2], t[1], t[2], zero, t[0], t[1], t[0], zero};
    } else if constexpr (N == 2) {
      return {zero,  t[2], t[1], zero, zero,  t[2],  zero, t[0], zero,
              zero,  t[1], t[0], zero, -t[4], -t[3], zero, zero, -t[3],
              -t[2], zero, zero, zero, -t[4], zero,  -t[2]};
    } else {
      return {zero,  t[2],  t[1],  zero,  zero,  zero,  zero,  -t[8], -t[7],
              t[2],  zero,  t[0],  zero,  zero,  -t[6], -t[5], zero,  zero,
              t[1],  t[0],  zero,  -t[4], -t[3], zero,  zero,  zero,  zero,
              zero,  zero,  -t[3], -t[2], zero,  t[8],  zero,  zero,  t[5],
              zero,  zero,  -t[4], zero,  -t[2], zero,  t[7],  t[6],  zero,
              zero,  -t[5], zero,  t[7],  zero,  -t[1], zero,  t[3],  zero,
              zero,  -t[6], zero,  zero,  t[8],  zero,  -t[1], zero,  t[4],
              -t[7], zero,  zero,  zero,  t[5],  t[4],  zero,  -t[0], zero,
              -t[8], zero,  zero,  t[6],  zero,  zero,  t[3],  zero,  -t[0]};
    }
  }  // end of computeDeterminantSecondDerivative

  template <typename T, typename T2toST2Type>
  TFEL_HOST_DEVICE
      std::enable_if_t<((implementsT2toST2Concept<T2toST2Type>()) &&
                        (isAssignableTo<numeric_type<T2toST2Type>, T>())),
                       void>
      convert(t2tot2<getSpaceDimension<T2toST2Type>(), T>& d,
              const T2toST2Type& s) {
    constexpr auto N = getSpaceDimension<T2toST2Type>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      tfel::fsalgo::copy<9u>::exe(s.begin(), d.begin());
    } else if constexpr (N == 2) {
      constexpr auto icste = Cste<T>::isqrt2;
      d(0, 0) = s(0, 0);
      d(0, 1) = s(0, 1);
      d(0, 2) = s(0, 2);
      d(0, 3) = s(0, 3);
      d(0, 4) = s(0, 4);
      d(1, 0) = s(1, 0);
      d(1, 1) = s(1, 1);
      d(1, 2) = s(1, 2);
      d(1, 3) = s(1, 3);
      d(1, 4) = s(1, 4);
      d(2, 0) = s(2, 0);
      d(2, 1) = s(2, 1);
      d(2, 2) = s(2, 2);
      d(2, 3) = s(2, 3);
      d(2, 4) = s(2, 4);
      d(4, 0) = d(3, 0) = s(3, 0) * icste;
      d(4, 1) = d(3, 1) = s(3, 1) * icste;
      d(4, 2) = d(3, 2) = s(3, 2) * icste;
      d(4, 3) = d(3, 3) = s(3, 3) * icste;
      d(4, 4) = d(3, 4) = s(3, 4) * icste;
    } else {
      constexpr auto icste = Cste<T>::isqrt2;
      d(0, 0) = s(0, 0);
      d(0, 1) = s(0, 1);
      d(0, 2) = s(0, 2);
      d(0, 3) = s(0, 3);
      d(0, 4) = s(0, 4);
      d(0, 5) = s(0, 5);
      d(0, 6) = s(0, 6);
      d(0, 7) = s(0, 7);
      d(0, 8) = s(0, 8);
      d(1, 0) = s(1, 0);
      d(1, 1) = s(1, 1);
      d(1, 2) = s(1, 2);
      d(1, 3) = s(1, 3);
      d(1, 4) = s(1, 4);
      d(1, 5) = s(1, 5);
      d(1, 6) = s(1, 6);
      d(1, 7) = s(1, 7);
      d(1, 8) = s(1, 8);
      d(2, 0) = s(2, 0);
      d(2, 1) = s(2, 1);
      d(2, 2) = s(2, 2);
      d(2, 3) = s(2, 3);
      d(2, 4) = s(2, 4);
      d(2, 5) = s(2, 5);
      d(2, 6) = s(2, 6);
      d(2, 7) = s(2, 7);
      d(2, 8) = s(2, 8);
      d(4, 0) = d(3, 0) = s(3, 0) * icste;
      d(4, 1) = d(3, 1) = s(3, 1) * icste;
      d(4, 2) = d(3, 2) = s(3, 2) * icste;
      d(4, 3) = d(3, 3) = s(3, 3) * icste;
      d(4, 4) = d(3, 4) = s(3, 4) * icste;
      d(4, 5) = d(3, 5) = s(3, 5) * icste;
      d(4, 6) = d(3, 6) = s(3, 6) * icste;
      d(4, 7) = d(3, 7) = s(3, 7) * icste;
      d(4, 8) = d(3, 8) = s(3, 8) * icste;
      d(6, 0) = d(5, 0) = s(4, 0) * icste;
      d(6, 1) = d(5, 1) = s(4, 1) * icste;
      d(6, 2) = d(5, 2) = s(4, 2) * icste;
      d(6, 3) = d(5, 3) = s(4, 3) * icste;
      d(6, 4) = d(5, 4) = s(4, 4) * icste;
      d(6, 5) = d(5, 5) = s(4, 5) * icste;
      d(6, 6) = d(5, 6) = s(4, 6) * icste;
      d(6, 7) = d(5, 7) = s(4, 7) * icste;
      d(6, 8) = d(5, 8) = s(4, 8) * icste;
      d(8, 0) = d(7, 0) = s(5, 0) * icste;
      d(8, 1) = d(7, 1) = s(5, 1) * icste;
      d(8, 2) = d(7, 2) = s(5, 2) * icste;
      d(8, 3) = d(7, 3) = s(5, 3) * icste;
      d(8, 4) = d(7, 4) = s(5, 4) * icste;
      d(8, 5) = d(7, 5) = s(5, 5) * icste;
      d(8, 6) = d(7, 6) = s(5, 6) * icste;
      d(8, 7) = d(7, 7) = s(5, 7) * icste;
      d(8, 8) = d(7, 8) = s(5, 8) * icste;
    }
  }  // end of convert

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOT2IXX */
