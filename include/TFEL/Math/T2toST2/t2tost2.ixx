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

#ifndef LIB_TFEL_MATH_T2TOST2IXX
#define LIB_TFEL_MATH_T2TOST2IXX

#include <cmath>
#include <iterator>
#include <algorithm>
#include <type_traits>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/T2toST2/LeftCauchyGreenTensorDerivativeExpr.hxx"
#include "TFEL/Math/T2toST2/RightCauchyGreenTensorDerivativeExpr.hxx"

namespace tfel {

  namespace math {

    // Assignement operator
    template <typename Child>
    template <typename T2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            T2toST2Traits<Child>::dime == T2toST2Traits<T2toST2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename T2toST2Traits<T2toST2Type>::NumType,
                typename T2toST2Traits<Child>::NumType>::cond,
        Child&>::type
    t2tost2_base<Child>::operator=(const T2toST2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<
          StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::copy(src,
                                                                     child);
      return child;
    }

    template <typename Child>
    template <typename T2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            T2toST2Traits<Child>::dime == T2toST2Traits<T2toST2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename T2toST2Traits<T2toST2Type>::NumType,
                typename T2toST2Traits<Child>::NumType>::cond,
        Child&>::type
    t2tost2_base<Child>::operator+=(const T2toST2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<
          StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::plusEqual(child,
                                                                          src);
      return child;
    }

    // Assignement operator
    template <typename Child>
    template <typename T2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            T2toST2Traits<Child>::dime == T2toST2Traits<T2toST2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename T2toST2Traits<T2toST2Type>::NumType,
                typename T2toST2Traits<Child>::NumType>::cond,
        Child&>::type
    t2tost2_base<Child>::operator-=(const T2toST2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::
          minusEqual(child, src);
      return child;
    }

    // *= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename T2toST2Traits<Child>::NumType,
                                    T2,
                                    OpMult>::type,
                typename T2toST2Traits<Child>::NumType>::value,
        Child&>::type
    t2tost2_base<Child>::operator*=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::
          multByScalar(child, s);
      return child;
    }

    // /= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename T2toST2Traits<Child>::NumType,
                                    T2,
                                    OpDiv>::type,
                typename T2toST2Traits<Child>::NumType>::value,
        Child&>::type
    t2tost2_base<Child>::operator/=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::
          divByScalar(child, s);
      return child;
    }

    template <unsigned short N, typename T>
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                T>::cond,
        Expr<t2tost2<N, T>, RightCauchyGreenTensorDerivativeExpr<N>>>::type
    t2tost2<N, T>::dCdF(const TensorType& F) {
      return Expr<t2tost2<N, T>, RightCauchyGreenTensorDerivativeExpr<N>>(F);
    }  // end of t2tost2::dCdF

    template <unsigned short N, typename T>
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                T>::cond,
        Expr<t2tost2<N, T>, LeftCauchyGreenTensorDerivativeExpr<N>>>::type
    t2tost2<N, T>::dBdF(const TensorType& F) {
      return Expr<t2tost2<N, T>, LeftCauchyGreenTensorDerivativeExpr<N>>(F);
    }  // end of t2tost2::dBdF

    template <unsigned short N, typename T>
    constexpr t2tost2<N, T>::t2tost2() {}  // end of t2tost2<N,T>::t2tost2

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr t2tost2<N, T>::t2tost2(const std::initializer_list<T2>& init)
        : fsarray<StensorDimeToSize<N>::value * TensorDimeToSize<N>::value, T>(
              init) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr t2tost2<N, T>::t2tost2(const T2& init)
        : fsarray<StensorDimeToSize<N>::value * TensorDimeToSize<N>::value, T>(
              init) {}  // end of t2tost2<N,T>::t2tost2

    template <unsigned short N, typename T>
    constexpr t2tost2<N, T>::t2tost2(const t2tost2<N, T>& src)
        : T2toST2Concept<t2tost2<N, T>>(src),
          fsarray<StensorDimeToSize<N>::value * TensorDimeToSize<N>::value, T>(
              src) {}

    template <unsigned short N, typename T>
    template <typename T2, typename Op>
    t2tost2<N, T>::t2tost2(const Expr<t2tost2<N, T2>, Op>& src) {
      matrix_utilities<StensorDimeToSize<N>::value, TensorDimeToSize<N>::value,
                       TensorDimeToSize<N>::value>::copy(src, *this);
    }

    template <unsigned short N, typename T>
    t2tost2<N, T>& t2tost2<N, T>::operator=(const t2tost2<N, T>& src) {
      fsarray<StensorDimeToSize<N>::value * TensorDimeToSize<N>::value,
              T>::operator=(src);
      return *this;
    }

    template <unsigned short N, typename T>
    T& t2tost2<N, T>::operator()(const unsigned short i,
                                 const unsigned short j) {
      return this->v[TensorDimeToSize<N>::value * i + j];
    }

    template <unsigned short N, typename T>
    constexpr const T& t2tost2<N, T>::operator()(const unsigned short i,
                                                 const unsigned short j) const {
      return this->v[TensorDimeToSize<N>::value * i + j];
    }

    template <unsigned short N, typename T>
    template <typename InputIterator>
    TFEL_MATH_INLINE2 void t2tost2<N, T>::copy(const InputIterator src) {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value *
                         TensorDimeToSize<N>::value>::exe(src, *this);
    }

    template <unsigned short N, typename T>
    typename t2tost2<N, T>::RunTimeProperties
    t2tost2<N, T>::getRunTimeProperties() const {
      return RunTimeProperties();
    }  // end of t2tost2<N,T>::getRunTimeProperties

    template <typename T2toT2Type>
    typename std::enable_if<
        ((T2toT2Traits<T2toT2Type>::dime == 1u) &&
         tfel::meta::Implements<T2toT2Type, tfel::math::T2toT2Concept>::cond),
        t2tost2<1u, typename T2toT2Traits<T2toT2Type>::NumType>>::type
    convertToT2toST2(const T2toT2Type& t) {
      using value_type = typename T2toT2Traits<T2toT2Type>::NumType;
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
    }

    template <typename T2toT2Type>
    typename std::enable_if<
        ((T2toT2Traits<T2toT2Type>::dime == 2u) &&
         tfel::meta::Implements<T2toT2Type, tfel::math::T2toT2Concept>::cond),
        t2tost2<2u, typename T2toT2Traits<T2toT2Type>::NumType>>::type
    convertToT2toST2(const T2toT2Type& t) {
      using value_type = typename T2toT2Traits<T2toT2Type>::NumType;
      constexpr const auto icste = Cste<value_type>::isqrt2;
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
    }

    template <typename T2toT2Type>
    typename std::enable_if<
        ((T2toT2Traits<T2toT2Type>::dime == 3u) &&
         tfel::meta::Implements<T2toT2Type, tfel::math::T2toT2Concept>::cond),
        t2tost2<3u, typename T2toT2Traits<T2toT2Type>::NumType>>::type
    convertToT2toST2(const T2toT2Type& t) {
      using value_type = typename T2toT2Traits<T2toT2Type>::NumType;
      constexpr const auto icste = Cste<value_type>::isqrt2;
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

    template <typename TensorType>
    typename std::enable_if<
        (tfel::meta::Implements<TensorType, TensorConcept>::cond) &&
            (TensorTraits<TensorType>::dime == 1u),
        t2tost2<1u, typename TensorTraits<TensorType>::NumType>>::type
    computeRateOfDeformationDerivative(const TensorType& F) {
      using value_type = typename TensorTraits<TensorType>::NumType;
      const auto iF = invert(F);
      constexpr const auto zero = value_type(0);
      return {iF[0], zero, zero, zero, iF[1], zero, zero, zero, iF[2]};
    }

    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        (tfel::meta::Implements<TensorType, TensorConcept>::cond) &&
            (TensorTraits<TensorType>::dime == 2u),
        t2tost2<2u, typename TensorTraits<TensorType>::NumType>>::type
    computeRateOfDeformationDerivative(const TensorType& F) {
      using value_type = typename TensorTraits<TensorType>::NumType;
      constexpr const auto icste = Cste<value_type>::isqrt2;
      constexpr const auto zero = value_type(0);
      const auto iF = invert(F);
      return {iF[0],         zero,  zero,          iF[4],
              zero,          zero,  iF[1],         zero,
              zero,          iF[3], zero,          zero,
              iF[2],         zero,  zero,          iF[3] * icste,
              iF[4] * icste, zero,  iF[1] * icste, iF[0] * icste};
    }

    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        (tfel::meta::Implements<TensorType, TensorConcept>::cond) &&
            (TensorTraits<TensorType>::dime == 3u),
        t2tost2<3u, typename TensorTraits<TensorType>::NumType>>::type
    computeRateOfDeformationDerivative(const TensorType& F) {
      using value_type = typename TensorTraits<TensorType>::NumType;
      constexpr const auto icste = Cste<value_type>::isqrt2;
      constexpr const auto zero = value_type(0);
      const auto iF = invert(F);
      return {iF[0],         zero,          zero,          iF[4],
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

    template <typename T2toST2Type, typename StensorType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            T2toST2Traits<T2toST2Type>::dime ==
                StensorTraits<StensorType>::dime &&
            T2toST2Traits<T2toST2Type>::dime ==
                TensorTraits<TensorType>::dime &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tost2<T2toST2Traits<T2toST2Type>::dime,
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result>>::type
    computeCauchyStressDerivativeFromKirchhoffStressDerivative(
        const T2toST2Type& dt, const StensorType& s, const TensorType& F) {
      using stress = typename ComputeBinaryResult<
          typename T2toST2Traits<T2toST2Type>::NumType,
          typename StensorTraits<StensorType>::NumType, OpPlus>::Result;
      t2tost2<T2toST2Traits<T2toST2Type>::dime, stress> r;
      computeCauchyStressDerivativeFromKirchhoffStressDerivative(r, dt, s, F);
      return r;
    }

    template <typename T2toST2Type, typename StensorType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            T2toST2Traits<T2toST2Type>::dime ==
                StensorTraits<StensorType>::dime &&
            T2toST2Traits<T2toST2Type>::dime ==
                TensorTraits<TensorType>::dime &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tost2<T2toST2Traits<T2toST2Type>::dime,
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result>>::type
    computeKirchhoffStressDerivativeFromCauchyStressDerivative(
        const T2toST2Type& ds, const StensorType& s, const TensorType& F) {
      using stress = typename ComputeBinaryResult<
          typename T2toST2Traits<T2toST2Type>::NumType,
          typename StensorTraits<StensorType>::NumType, OpPlus>::Result;
      t2tost2<T2toST2Traits<T2toST2Type>::dime, stress> r;
      computeKirchhoffStressDerivativeFromCauchyStressDerivative(r, ds, s, F);
      return r;
    }

    template <typename T2toST2Type, typename StensorType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            StensorTraits<StensorType>::dime ==
                T2toST2Traits<T2toST2Type>::dime &&
            TensorTraits<TensorType>::dime ==
                T2toST2Traits<T2toST2Type>::dime &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tost2<T2toST2Traits<T2toST2Type>::dime,
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result>>::type
    computePushForwardDerivative(const T2toST2Type& K,
                                 const StensorType& S,
                                 const TensorType& F) {
      using stress = typename ComputeBinaryResult<
          typename T2toST2Traits<T2toST2Type>::NumType,
          typename StensorTraits<StensorType>::NumType, OpPlus>::Result;
      t2tost2<T2toST2Traits<T2toST2Type>::dime, stress> r;
      computePushForwardDerivative(r, K, S, F);
      return r;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOST2IXX */
