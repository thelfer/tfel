/*!
 * \file   include/TFEL/Math/ST2toT2/st2tot2.ixx
 * \brief  This file implements the methods of the class st2tot2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2_IXX
#define LIB_TFEL_MATH_ST2TOT2_IXX

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

namespace tfel::math {

  // Assignement operator
  template <typename Child>
  template <typename ST2toT2Type>
  std::enable_if_t<implementsST2toT2Concept<ST2toT2Type>() &&
                       getSpaceDimension<Child>() ==
                           getSpaceDimension<ST2toT2Type>() &&
                       tfel::typetraits::IsAssignableTo<
                           numeric_type<ST2toT2Type>,
                           numeric_type<Child>>::cond,
                   Child&>
  st2tot2_base<Child>::operator=(const ST2toT2Type& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<
        TensorDimeToSize<getSpaceDimension<Child>()>::value,
        StensorDimeToSize<getSpaceDimension<Child>()>::value,
        StensorDimeToSize<getSpaceDimension<Child>()>::value>::copy(src, child);
    return child;
  }

  template <typename Child>
  template <typename ST2toT2Type>
  std::enable_if_t<implementsST2toT2Concept<ST2toT2Type>() &&
                       getSpaceDimension<Child>() ==
                           getSpaceDimension<ST2toT2Type>() &&
                       tfel::typetraits::IsAssignableTo<
                           numeric_type<ST2toT2Type>,
                           numeric_type<Child>>::cond,
                   Child&>
  st2tot2_base<Child>::operator+=(const ST2toT2Type& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<
        TensorDimeToSize<getSpaceDimension<Child>()>::value,
        StensorDimeToSize<getSpaceDimension<Child>()>::value,
        StensorDimeToSize<getSpaceDimension<Child>()>::value>::plusEqual(child,
                                                                         src);
    return child;
  }

  // Assignement operator
  template <typename Child>
  template <typename ST2toT2Type>
  std::enable_if_t<implementsST2toT2Concept<ST2toT2Type>() &&
                       getSpaceDimension<Child>() ==
                           getSpaceDimension<ST2toT2Type>() &&
                       tfel::typetraits::IsAssignableTo<
                           numeric_type<ST2toT2Type>,
                           numeric_type<Child>>::cond,
                   Child&>
  st2tot2_base<Child>::operator-=(const ST2toT2Type& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<
        TensorDimeToSize<getSpaceDimension<Child>()>::value,
        StensorDimeToSize<getSpaceDimension<Child>()>::value,
        StensorDimeToSize<getSpaceDimension<Child>()>::value>::minusEqual(child,
                                                                          src);
    return child;
  }

  // *= operator
  template <typename Child>
  template <typename T2>
  std::enable_if_t<
      tfel::typetraits::IsScalar<T2>::cond &&
          std::is_same<
              typename ResultType<numeric_type<Child>,
                                  T2,
                                  OpMult>::type,
              numeric_type<Child>>::value,
      Child&>
  st2tot2_base<Child>::operator*=(const T2 s) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<TensorDimeToSize<getSpaceDimension<Child>()>::value,
                     StensorDimeToSize<getSpaceDimension<Child>()>::value,
                     StensorDimeToSize<getSpaceDimension<Child>()>::value>::
        multByScalar(child, s);
    return child;
  }

  // /= operator
  template <typename Child>
  template <typename T2>
  std::enable_if_t<
      tfel::typetraits::IsScalar<T2>::cond &&
          std::is_same<
              typename ResultType<numeric_type<Child>,
                                  T2,
                                  OpDiv>::type,
              numeric_type<Child>>::value,
      Child&>
  st2tot2_base<Child>::operator/=(const T2 s) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<TensorDimeToSize<getSpaceDimension<Child>()>::value,
                     StensorDimeToSize<getSpaceDimension<Child>()>::value,
                     StensorDimeToSize<getSpaceDimension<Child>()>::value>::
        divByScalar(child, s);
    return child;
  }

  template <unsigned short N, typename T>
  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == N &&
                       tfel::typetraits::IsAssignableTo<
                           numeric_type<StensorType>,
                           T>::cond,
                   Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>>
  st2tot2<N, T>::tpld(const StensorType& b) {
    return Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>(b);
  }  // end of st2tot2<N,T>

  template <unsigned short N, typename T>
  template <typename StensorType, typename ST2toST2Type>
  std::enable_if_t<
      implementsStensorConcept<StensorType>() &&
          implementsST2toST2Concept<ST2toST2Type>() &&
          getSpaceDimension<StensorType>() == N &&
          getSpaceDimension<ST2toST2Type>() == N &&
          tfel::typetraits::IsAssignableTo<
              typename ComputeBinaryResult<
                  numeric_type<StensorType>,
                  numeric_type<ST2toST2Type>,
                  OpMult>::Result,
              T>::cond,
      Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>>
  st2tot2<N, T>::tpld(const StensorType& b, const ST2toST2Type& C) {
    return Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>(b, C);
  }

  template <unsigned short N, typename T>
  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == N &&
                       tfel::typetraits::IsAssignableTo<
                           numeric_type<StensorType>,
                           T>::cond,
                   Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>>
  st2tot2<N, T>::tprd(const StensorType& a) {
    return Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>(a);
  }

  template <unsigned short N, typename T>
  template <typename StensorType, typename ST2toST2Type>
  std::enable_if_t<
      implementsStensorConcept<StensorType>() &&
          implementsST2toST2Concept<ST2toST2Type>() &&
          getSpaceDimension<StensorType>() == N &&
          getSpaceDimension<ST2toST2Type>() == N &&
          tfel::typetraits::IsAssignableTo<
              typename ComputeBinaryResult<
                  numeric_type<StensorType>,
                  numeric_type<ST2toST2Type>,
                  OpMult>::Result,
              T>::cond,
      Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>>
  st2tot2<N, T>::tprd(const StensorType& a, const ST2toST2Type& C) {
    return Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>(a, C);
  }

  template <unsigned short N, typename T>
  constexpr st2tot2<N, T>::st2tot2() {}  // end of st2tot2<N,T>::st2tot2

  template <unsigned short N, typename T>
  template <
      typename T2,
      std::enable_if_t<tfel::typetraits::IsAssignableTo<T2, T>::cond, bool>>
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
    fsarray<TensorDimeToSize<N>::value * StensorDimeToSize<N>::value, T>::
    operator=(src);
    return *this;
  }

  template <unsigned short N, typename T>
  T& st2tot2<N, T>::operator()(const unsigned short i, const unsigned short j) {
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
  st2tot2<N, T>::getRunTimeProperties() const {
    return RunTimeProperties();
  }  // end of st2tot2<N,T>::getRunTimeProperties

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOT2_IXX */
