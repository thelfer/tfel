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
  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == N &&
                       isAssignableTo<numeric_type<StensorType>, T>(),
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
          isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                               numeric_type<ST2toST2Type>,
                                               OpMult>,
                         T>(),
      Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>>
  st2tot2<N, T>::tpld(const StensorType& b, const ST2toST2Type& C) {
    return Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>(b, C);
  }

  template <unsigned short N, typename T>
  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == N &&
                       isAssignableTo<numeric_type<StensorType>, T>(),
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
          isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                               numeric_type<ST2toST2Type>,
                                               OpMult>,
                         T>(),
      Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>>
  st2tot2<N, T>::tprd(const StensorType& a, const ST2toST2Type& C) {
    return Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>(a, C);
  }

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_MATH_INLINE2 void st2tot2<N, T>::copy(const InputIterator src) {
    tfel::fsalgo::copy<TensorDimeToSize<N>::value *
                       StensorDimeToSize<N>::value>::exe(src, *this);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOT2_IXX */
