/*!
 * \file   include/TFEL/Math/ST2toT2/st2tot2.ixx
 * \brief  This file implements the methods of the class st2tot2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2_IXX
#define LIB_TFEL_MATH_ST2TOT2_IXX

#include <cmath>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/ST2toT2/StensorProductLeftDerivativeExpr.hxx"
#include "TFEL/Math/ST2toT2/StensorProductRightDerivativeExpr.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto st2tot2<N, T>::tpld(
      const StensorType& b) noexcept
      requires(getSpaceDimension<StensorType>() == N &&
               isAssignableTo<numeric_type<StensorType>, T>()) {
    return Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>(b);
  }  // end of st2tot2<N,T>

  template <unsigned short N, typename ValueType>
  template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
  TFEL_HOST_DEVICE constexpr auto st2tot2<N, ValueType>::tpld(
      const StensorType& b, const ST2toST2Type& C) noexcept
      requires(getSpaceDimension<StensorType>() == N &&
               getSpaceDimension<ST2toST2Type>() == N &&
               isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                                    numeric_type<ST2toST2Type>,
                                                    OpMult>,
                              ValueType>()) {
    return Expr<st2tot2<N, ValueType>, StensorProductLeftDerivativeExpr<N>>(b,
                                                                            C);
  }

  template <unsigned short N, typename T>
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto st2tot2<N, T>::tprd(
      const StensorType& a) noexcept
      requires(getSpaceDimension<StensorType>() == N &&
               isAssignableTo<numeric_type<StensorType>, T>()) {
    return Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>(a);
  }

  template <unsigned short N, typename T>
  template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
  TFEL_HOST_DEVICE constexpr auto st2tot2<N, T>::tprd(
      const StensorType& a, const ST2toST2Type& C) noexcept
      requires(getSpaceDimension<StensorType>() == N &&
               getSpaceDimension<ST2toST2Type>() == N &&
               isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                                    numeric_type<ST2toST2Type>,
                                                    OpMult>,
                              T>()) {
    return Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>(a, C);
  }

  template <unsigned short N, typename ValueType>
  template <std::size_t... d>
  TFEL_HOST_DEVICE constexpr st2tot2<N, ValueType>::st2tot2(
      ValueType const (&... arrays)[d])  //
      requires((sizeof...(d) == TensorDimeToSize<N>::value) &&
               ((d == StensorDimeToSize<N>::value) && ...)) {
    auto init_row = [this](
                        const typename st2tot2::size_type i,
                        ValueType const(&values)[StensorDimeToSize<N>::value]) {
      for (typename st2tot2::size_type j = 0u; j < StensorDimeToSize<N>::value;
           ++j) {
        this->operator()(i, j) = values[j];
      }
    };
    auto i = typename st2tot2::size_type{};
    (init_row(i++, arrays), ...);
  }  // end of st2tot2

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void st2tot2<N, T>::import(
      const base_type<T>* const src) noexcept {
    tfel::fsalgo::transform<
        TensorDimeToSize<N>::value *
        StensorDimeToSize<N>::value>::exe(src, this->begin(),
                                          [](const auto& v) { return T(v); });
  }  // end of import

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void st2tot2<N, T>::copy(const auto p) noexcept {
    tfel::fsalgo::copy<TensorDimeToSize<N>::value *
                       StensorDimeToSize<N>::value>::exe(p, *this);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOT2_IXX */
