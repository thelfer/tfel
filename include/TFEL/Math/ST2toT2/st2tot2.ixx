/*!
 * \file   include/TFEL/Math/ST2toT2/st2tot2.ixx
 * \brief  This file implements the methods of the class st2tot2.
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2_IXX_
#define LIB_TFEL_MATH_ST2TOT2_IXX_

#include <cmath>
#include <iterator>
#include <algorithm>
#include <type_traits>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/ST2toT2/StensorProductLeftDerivativeExpr.hxx"
#include "TFEL/Math/ST2toT2/StensorProductRightDerivativeExpr.hxx"

namespace tfel {

  namespace math {

    // Assignement operator
    template <typename Child>
    template <typename ST2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond &&
            ST2toT2Traits<Child>::dime == ST2toT2Traits<ST2toT2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename ST2toT2Traits<ST2toT2Type>::NumType,
                typename ST2toT2Traits<Child>::NumType>::cond,
        Child&>::type
    st2tot2_base<Child>::operator=(const ST2toT2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<
          TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
          StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
          StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::copy(src,
                                                                      child);
      return child;
    }

    template <typename Child>
    template <typename ST2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond &&
            ST2toT2Traits<Child>::dime == ST2toT2Traits<ST2toT2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename ST2toT2Traits<ST2toT2Type>::NumType,
                typename ST2toT2Traits<Child>::NumType>::cond,
        Child&>::type
    st2tot2_base<Child>::operator+=(const ST2toT2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::
          plusEqual(child, src);
      return child;
    }

    // Assignement operator
    template <typename Child>
    template <typename ST2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond &&
            ST2toT2Traits<Child>::dime == ST2toT2Traits<ST2toT2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename ST2toT2Traits<ST2toT2Type>::NumType,
                typename ST2toT2Traits<Child>::NumType>::cond,
        Child&>::type
    st2tot2_base<Child>::operator-=(const ST2toT2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::
          minusEqual(child, src);
      return child;
    }

    // *= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename ST2toT2Traits<Child>::NumType,
                                    T2,
                                    OpMult>::type,
                typename ST2toT2Traits<Child>::NumType>::value,
        Child&>::type
    st2tot2_base<Child>::operator*=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::
          multByScalar(child, s);
      return child;
    }

    // /= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename ST2toT2Traits<Child>::NumType,
                                    T2,
                                    OpDiv>::type,
                typename ST2toT2Traits<Child>::NumType>::value,
        Child&>::type
    st2tot2_base<Child>::operator/=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value,
                       StensorDimeToSize<ST2toT2Traits<Child>::dime>::value>::
          divByScalar(child, s);
      return child;
    }

    template <unsigned short N, typename T>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>>::type
    st2tot2<N, T>::tpld(const StensorType& b) {
      return Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>(b);
    }  // end of st2tot2<N,T>

    template <unsigned short N, typename T>
    template <typename StensorType, typename ST2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond &&
            StensorTraits<StensorType>::dime == N &&
            ST2toST2Traits<ST2toST2Type>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename ST2toST2Traits<ST2toST2Type>::NumType,
                    OpMult>::Result,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>>::type
    st2tot2<N, T>::tpld(const StensorType& b, const ST2toST2Type& C) {
      return Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>(b, C);
    }

    template <unsigned short N, typename T>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>>::type
    st2tot2<N, T>::tprd(const StensorType& a) {
      return Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>(a);
    }

    template <unsigned short N, typename T>
    template <typename StensorType, typename ST2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond &&
            StensorTraits<StensorType>::dime == N &&
            ST2toST2Traits<ST2toST2Type>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename ST2toST2Traits<ST2toST2Type>::NumType,
                    OpMult>::Result,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>>::type
    st2tot2<N, T>::tprd(const StensorType& a, const ST2toST2Type& C) {
      return Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>(a, C);
    }

    template <unsigned short N, typename T>
    constexpr st2tot2<N, T>::st2tot2() {}  // end of st2tot2<N,T>::st2tot2

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr st2tot2<N, T>::st2tot2(const T2& init)
        : fsarray<TensorDimeToSize<N>::value * StensorDimeToSize<N>::value, T>(
              init) {}

    template <unsigned short N, typename T>
    constexpr st2tot2<N, T>::st2tot2(const st2tot2<N, T>& src)
        : ST2toT2Concept<st2tot2<N, T>>(src),
          fsarray<TensorDimeToSize<N>::value * StensorDimeToSize<N>::value, T>(
              src) {}

    template <unsigned short N, typename T>
    template <typename T2, typename Op>
    st2tot2<N, T>::st2tot2(const Expr<st2tot2<N, T2>, Op>& src) {
      matrix_utilities<TensorDimeToSize<N>::value, StensorDimeToSize<N>::value,
                       StensorDimeToSize<N>::value>::copy(src, *this);
    }

    template <unsigned short N, typename T>
    st2tot2<N, T>& st2tot2<N, T>::operator=(const st2tot2<N, T>& src) {
      fsarray<TensorDimeToSize<N>::value * StensorDimeToSize<N>::value,
              T>::operator=(src);
      return *this;
    }

    template <unsigned short N, typename T>
    T& st2tot2<N, T>::operator()(const unsigned short i,
                                 const unsigned short j) {
      return this->v[StensorDimeToSize<N>::value * i + j];
    }

    template <unsigned short N, typename T>
    constexpr const T& st2tot2<N, T>::operator()(const unsigned short i,
                                                 const unsigned short j) const {
      return this->v[StensorDimeToSize<N>::value * i + j];
    }

    template <unsigned short N, typename T>
    template <typename InputIterator>
    TFEL_MATH_INLINE2 void st2tot2<N, T>::copy(const InputIterator src) {
      tfel::fsalgo::copy<TensorDimeToSize<N>::value *
                         StensorDimeToSize<N>::value>::exe(src, *this);
    }

    template <unsigned short N, typename T>
    typename st2tot2<N, T>::RunTimeProperties
    st2tot2<N, T>::getRunTimeProperties(void) const {
      return RunTimeProperties();
    }  // end of st2tot2<N,T>::getRunTimeProperties

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_ST2TOT2_IXX_ */
