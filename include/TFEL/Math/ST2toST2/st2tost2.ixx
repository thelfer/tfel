/*!
 * \file   include/TFEL/Math/ST2toST2/st2tost2.ixx
 * \brief  This file implements the methods of the class st2tost2.
 * \author Thomas Helfer
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_IXX
#define LIB_TFEL_MATH_ST2TOST2_IXX

#include <cmath>
#include <iterator>
#include <algorithm>
#include <type_traits>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/TinyMatrixInvert.hxx"
#include "TFEL/Math/ST2toST2/ConvertT2toST2ToST2toST2Expr.hxx"
#include "TFEL/Math/ST2toST2/BuildFromRotationMatrix.hxx"
#include "TFEL/Math/ST2toST2/StensorSymmetricProductDerivative.hxx"
#include "TFEL/Math/ST2toST2/SymmetricStensorProductDerivative.hxx"
#include "TFEL/Math/ST2toST2/ChangeBasis.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == N &&
                       isAssignableTo<numeric_type<StensorType>, T>(),
                   Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>>
  st2tost2<N, T>::dsquare(const StensorType& s) {
    return Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>(s);
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
      Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>>
  st2tost2<N, T>::dsquare(const StensorType& s, const ST2toST2Type& C) {
    return Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>(s, C);
  }

  template <unsigned short N, typename T>
  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>() &&
                       getSpaceDimension<StensorType>() == N &&
                       isAssignableTo<numeric_type<StensorType>, T>(),
                   tfel::math::st2tost2<N, T>>
  st2tost2<N, T>::stpd(const StensorType& s) {
    return StensorSymmetricProductDerivative<N, T>::exe(s);
  }

  template <unsigned short N, typename T>
  template <typename T2toST2Type>
  std::enable_if_t<implementsT2toST2Concept<T2toST2Type>() &&
                       getSpaceDimension<T2toST2Type>() == N &&
                       isAssignableTo<numeric_type<T2toST2Type>, T>(),
                   Expr<st2tost2<N, T>, ConvertT2toST2ToST2toST2Expr<N>>>
  st2tost2<N, T>::convert(const T2toST2Type& src) {
    return Expr<st2tost2<N, T>, ConvertT2toST2ToST2toST2Expr<N>>(src);
  }

  template <unsigned short N, typename T>
  st2tost2<N, base_type<T>> st2tost2<N, T>::fromRotationMatrix(
      const rotation_matrix<T>& r) {
    return st2tost2_internals::BuildFromRotationMatrix<N, T>::exe(r);
  }  // end of st2tost2<N,T>::fromRotationMatrix

  template <unsigned short N, typename T>
  constexpr st2tost2<N, T> st2tost2<N, T>::Id() {
    constexpr auto c0 = T{0};
    constexpr auto c1 = T{1};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c1, c0, c0,  //
              c0, c1, c0,  //
              c0, c0, c1};
    } else if constexpr (N == 2) {
      return {c1, c0, c0, c0,  //
              c0, c1, c0, c0,  //
              c0, c0, c1, c0,  //
              c0, c0, c0, c1};
    } else {
      return {c1, c0, c0, c0, c0, c0,  //
              c0, c1, c0, c0, c0, c0,  //
              c0, c0, c1, c0, c0, c0,  //
              c0, c0, c0, c1, c0, c0,  //
              c0, c0, c0, c0, c1, c0,  //
              c0, c0, c0, c0, c0, c1};
    }
  }  // end of st2tost2<N,T>::Id

  template <unsigned short N, typename T>
  constexpr st2tost2<N, T> st2tost2<N, T>::IxI() {
    constexpr auto c1 = T{1};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c1, c1, c1,  //
              c1, c1, c1,  //
              c1, c1, c1};
    } else if constexpr (N == 2) {
      constexpr auto c0 = T{0};
      return {c1, c1, c1, c0,  //
              c1, c1, c1, c0,  //
              c1, c1, c1, c0,  //
              c0, c0, c0, c0};
    } else {
      constexpr auto c0 = T{0};
      return {c1, c1, c1, c0, c0, c0,  //
              c1, c1, c1, c0, c0, c0,  //
              c1, c1, c1, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0,  //
              c0, c0, c0, c0, c0, c0};
    }
  }  // end of st2tost2<N,T>::Id

  template <unsigned short N, typename T>
  constexpr st2tost2<N, T> st2tost2<N, T>::K() {
    constexpr auto c2_3 = T{2} / T{3};
    constexpr auto mc1_3 = -T{1} / T{3};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c2_3,  mc1_3, mc1_3,  //
              mc1_3, c2_3,  mc1_3,  //
              mc1_3, mc1_3, c2_3};
    } else if constexpr (N == 2) {
      constexpr auto c0 = T{0};
      constexpr auto c1 = T{1};
      return {c2_3,  mc1_3, mc1_3, c0,  //
              mc1_3, c2_3,  mc1_3, c0,  //
              mc1_3, mc1_3, c2_3,  c0,  //
              c0,    c0,    c0,    c1};
    } else {
      constexpr auto c0 = T{0};
      constexpr auto c1 = T{1};
      return {c2_3,  mc1_3, mc1_3, c0, c0, c0,  //
              mc1_3, c2_3,  mc1_3, c0, c0, c0,  //
              mc1_3, mc1_3, c2_3,  c0, c0, c0,  //
              c0,    c0,    c0,    c1, c0, c0,  //
              c0,    c0,    c0,    c0, c1, c0,  //
              c0,    c0,    c0,    c0, c0, c1};
    }
  }  // end of st2tost2<N,T>::K

  template <unsigned short N, typename T>
  constexpr st2tost2<N, T> st2tost2<N, T>::M() {
    constexpr auto c1 = T{1};
    constexpr auto mc1_2 = -T{1} / T{2};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c1,    mc1_2, mc1_2,  //
              mc1_2, c1,    mc1_2,  //
              mc1_2, mc1_2, c1};
    } else if constexpr (N == 2) {
      constexpr auto c0 = T{0};
      constexpr auto c3_2 = T{3} / T{2};
      return {c1,    mc1_2, mc1_2, c0,  //
              mc1_2, c1,    mc1_2, c0,  //
              mc1_2, mc1_2, c1,    c0,  //
              c0,    c0,    c0,    c3_2};
    } else {
      constexpr auto c0 = T{0};
      constexpr auto c3_2 = T{3} / T{2};
      return {c1,    mc1_2, mc1_2, c0,   c0,   c0,  //
              mc1_2, c1,    mc1_2, c0,   c0,   c0,  //
              mc1_2, mc1_2, c1,    c0,   c0,   c0,  //
              c0,    c0,    c0,    c3_2, c0,   c0,  //
              c0,    c0,    c0,    c0,   c3_2, c0,  //
              c0,    c0,    c0,    c0,   c0,   c3_2};
    }
  }  // end of st2tost2<N,T>::M

  template <unsigned short N, typename T>
  constexpr st2tost2<N, T> st2tost2<N, T>::J() {
    constexpr auto c1_3 = T{1} / T{3};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {c1_3, c1_3, c1_3,  //
              c1_3, c1_3, c1_3,  //
              c1_3, c1_3, c1_3};
    } else if constexpr (N == 2) {
      constexpr auto c0 = T{0};
      return {c1_3, c1_3, c1_3, c0,  //
              c1_3, c1_3, c1_3, c0,  //
              c1_3, c1_3, c1_3, c0,  //
              c0,   c0,   c0,   c0};
    } else {
      constexpr auto c0 = T{0};
      return {c1_3, c1_3, c1_3, c0, c0, c0,  //
              c1_3, c1_3, c1_3, c0, c0, c0,  //
              c1_3, c1_3, c1_3, c0, c0, c0,  //
              c0,   c0,   c0,   c0, c0, c0,  //
              c0,   c0,   c0,   c0, c0, c0,  //
              c0,   c0,   c0,   c0, c0, c0};
    }
  }  // end of st2tost2<N,T>::J

  template <unsigned short N, typename T>
  template <typename InputIterator>
  TFEL_MATH_INLINE2 void st2tost2<N, T>::copy(const InputIterator src) {
    tfel::fsalgo::copy<StensorDimeToSize<N>::value *
                       StensorDimeToSize<N>::value>::exe(src, *this);
  }

  template <typename ST2toST2Type>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>(),
      st2tost2<getSpaceDimension<ST2toST2Type>(),
               BinaryOperationResult<base_type<numeric_type<ST2toST2Type>>,
                                     numeric_type<ST2toST2Type>,
                                     OpDiv>>>
  invert(const ST2toST2Type& s) {
    static constexpr unsigned short N = getSpaceDimension<ST2toST2Type>();
    static constexpr unsigned short StensorSize = StensorDimeToSize<N>::value;
    using NumType = numeric_type<ST2toST2Type>;
    using real = base_type<ST2toST2Type>;
    using iNumType = BinaryOperationResult<real, NumType, OpDiv>;
    st2tost2<N, iNumType> is;
    tmatrix<StensorSize, StensorSize, real> m;
    for (unsigned short i = 0; i != StensorSize; ++i) {
      for (unsigned short j = 0; j != StensorSize; ++j) {
        m(i, j) = base_type_cast(s(i, j));
      }
    }
    TinyMatrixInvert<StensorSize, real>::exe(m);
    for (unsigned short i = 0; i != StensorSize; ++i) {
      for (unsigned short j = 0; j != StensorSize; ++j) {
        is(i, j) = iNumType(m(i, j));
      }
    }
    return is;
  }  // end of invert

  template <typename ST2toST2Type>
  std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>(),
      st2tost2<getSpaceDimension<ST2toST2Type>(), numeric_type<ST2toST2Type>>>
  change_basis(const ST2toST2Type& s,
               const rotation_matrix<numeric_type<ST2toST2Type>>& r) {
    return st2tost2_internals::ChangeBasis<
        getSpaceDimension<ST2toST2Type>()>::exe(s, r);
  }

  template <typename ST2toST2Type, typename TensorType>
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       implementsTensorConcept<TensorType>() &&
                       getSpaceDimension<ST2toST2Type>() ==
                           getSpaceDimension<TensorType>(),
                   st2tost2<getSpaceDimension<ST2toST2Type>(),
                            BinaryOperationResult<numeric_type<ST2toST2Type>,
                                                  numeric_type<TensorType>,
                                                  OpMult>>>
  push_forward(const ST2toST2Type& C, const TensorType& F) {
    st2tost2<getSpaceDimension<ST2toST2Type>(),
             BinaryOperationResult<numeric_type<ST2toST2Type>,
                                   numeric_type<TensorType>, OpMult>>
        r;
    push_forward(r, C, F);
    return r;
  }

  template <typename ST2toST2Type, typename TensorType>
  std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<ST2toST2Type>() == getSpaceDimension<TensorType>(),
      st2tost2<getSpaceDimension<ST2toST2Type>(),
               typename ComputeBinaryResult<numeric_type<ST2toST2Type>,
                                            numeric_type<TensorType>,
                                            OpMult>::Result>>
  pull_back(const ST2toST2Type& C, const TensorType& F) {
    const auto iF = invert(F);
    return push_forward(C, iF);
  }

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>() &&
          isScalar<numeric_type<StensorType>>(),
      st2tost2<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeDeterminantSecondDerivative(const StensorType& s) {
    using NumType = numeric_type<StensorType>;
    constexpr auto N = getSpaceDimension<StensorType>();
    constexpr auto zero = NumType{0};
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {zero, s[2], s[1], s[2], zero, s[0], s[1], s[0], zero};
    } else if constexpr (N == 2) {
      return {zero, s[2], s[1], zero,  s[2], zero, s[0],  zero,
              s[1], s[0], zero, -s[3], zero, zero, -s[3], -s[2]};
    } else {
      constexpr auto icste = Cste<NumType>::isqrt2;
      return {zero,  s[2],  s[1],  zero,         zero,         -s[5],
              s[2],  zero,  s[0],  zero,         -s[4],        zero,
              s[1],  s[0],  zero,  -s[3],        zero,         zero,
              zero,  zero,  -s[3], -s[2],        s[5] * icste, s[4] * icste,
              zero,  -s[4], zero,  s[5] * icste, -s[1],        s[3] * icste,
              -s[5], zero,  zero,  s[4] * icste, s[3] * icste, -s[0]};
    }
  }  // end of computeDeterminantSecondDerivative

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>() &&
          isScalar<numeric_type<StensorType>>(),
      st2tost2<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeDeviatorDeterminantSecondDerivative(const StensorType& s) {
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    if constexpr (N == 1) {
      return {-(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9};
    } else if constexpr (N == 2) {
      return {-(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              s[3] / 3,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              s[3] / 3,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              (-2 * s[3]) / 3,
              s[3] / 3,
              s[3] / 3,
              (-2 * s[3]) / 3,
              -(2 * s[2] - s[1] - s[0]) / 3};
    } else {
      using NumType = numeric_type<StensorType>;
      constexpr auto icste = Cste<NumType>::isqrt2;
      return {-(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              s[3] / 3,
              s[4] / 3,
              (-2 * s[5]) / 3,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              s[3] / 3,
              (-2 * s[4]) / 3,
              s[5] / 3,
              -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
              -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
              (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
              (-2 * s[3]) / 3,
              s[4] / 3,
              s[5] / 3,
              s[3] / 3,
              s[3] / 3,
              (-2 * s[3]) / 3,
              -(2 * s[2] - s[1] - s[0]) / 3,
              s[5] * icste,
              s[4] * icste,
              s[4] / 3,
              (-2 * s[4]) / 3,
              s[4] / 3,
              s[5] * icste,
              (s[2] - 2 * s[1] + s[0]) / 3,
              s[3] * icste,
              (-2 * s[5]) / 3,
              s[5] / 3,
              s[5] / 3,
              s[4] * icste,
              s[3] * icste,
              (s[2] + s[1] - 2 * s[0]) / 3};
    }
  }  // end of computeDeviatorDeterminantSecondDerivative

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH8ST2TOST2_IXX */
