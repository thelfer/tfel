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

namespace tfel::math {

  template <unsigned short N, typename ValueType>
  template <std::size_t... d>
  TFEL_HOST_DEVICE constexpr st2tost2<N, ValueType>::st2tost2(
      ValueType const (&... arrays)[d])  //
      requires((sizeof...(d) == StensorDimeToSize<N>::value) &&
               ((d == StensorDimeToSize<N>::value) && ...)) {
    auto init_row = [this](
                        const typename st2tost2::size_type i,
                        ValueType const(&values)[StensorDimeToSize<N>::value]) {
      for (typename st2tost2::size_type j = 0u; j < StensorDimeToSize<N>::value;
           ++j) {
        this->operator()(i, j) = values[j];
      }
    };
    auto i = typename st2tost2::size_type{};
    (init_row(i++, arrays), ...);
  }  // end of st2tost2

  template <unsigned short N, typename T>
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto st2tost2<N, T>::dsquare(
      const StensorType& s) noexcept
      requires(getSpaceDimension<StensorType>() == N &&
               isAssignableTo<numeric_type<StensorType>, T>()) {
    return Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>(s);
  }

  template <unsigned short N, typename T>
  template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
  TFEL_HOST_DEVICE constexpr auto st2tost2<N, T>::dsquare(
      const StensorType& s, const ST2toST2Type& C) noexcept
      requires(getSpaceDimension<StensorType>() == N &&
               getSpaceDimension<ST2toST2Type>() == N &&
               isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                                    numeric_type<ST2toST2Type>,
                                                    OpMult>,
                              T>()) {
    return Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>(s, C);
  }  // end of dsquare

  template <unsigned short N, typename T>
  template <StensorConcept StensorType>
  TFEL_HOST_DEVICE constexpr auto st2tost2<N, T>::stpd(
      const StensorType& s) noexcept
      requires(getSpaceDimension<StensorType>() == N &&
               isAssignableTo<numeric_type<StensorType>, T>()) {
    return StensorSymmetricProductDerivative<N, T>::exe(s);
  }

  template <unsigned short N, typename T>
  template <T2toST2Concept T2toST2Type>
  TFEL_HOST_DEVICE constexpr auto st2tost2<N, T>::convert(
      const T2toST2Type& src) noexcept
      requires(getSpaceDimension<T2toST2Type>() == N &&
               isAssignableTo<numeric_type<T2toST2Type>, T>()) {
    return Expr<st2tost2<N, T>, ConvertT2toST2ToST2toST2Expr<N>>(src);
  }  // end of convert

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr auto st2tost2<N, T>::fromRotationMatrix(
      const rotation_matrix<T>& r) noexcept {
    return st2tost2_internals::BuildFromRotationMatrix<N, T>::exe(r);
  }  // end of st2tost2<N,T>::fromRotationMatrix

  template <unsigned short N, typename T>
  constexpr st2tost2<N, T> st2tost2<N, T>::Id() noexcept {
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
  constexpr st2tost2<N, T> st2tost2<N, T>::IxI() noexcept {
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
  constexpr st2tost2<N, T> st2tost2<N, T>::K() noexcept {
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
  constexpr st2tost2<N, T> st2tost2<N, T>::M() noexcept {
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
  constexpr st2tost2<N, T> st2tost2<N, T>::J() noexcept {
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

  template <unsigned short N>
  TFEL_HOST_DEVICE constexpr unsigned short VoigtIndex(unsigned short i,
                                                       unsigned short j) {
    if (N == 1 or N == 2 or N == 3) {
      if ((i == 0) and (j == 0)) {
        return 0;
      } else if ((i == 1) and (j == 1)) {
        return 1;
      } else if ((i == 2) and (j == 2)) {
        return 2;
      } else if (N == 1) {
        tfel::reportContractViolation("indices are not valid");
      } else if (N == 2 or N == 3) {
        if (((i == 0) and (j == 1)) || ((i == 1) and (j == 0))) {
          return 3;
        } else if (N == 2) {
          tfel::reportContractViolation("indices are not valid");
        } else if (N == 3) {
          if (((i == 0) and (j == 2)) || ((i == 2) and (j == 0))) {
            return 4;
          } else if (((i == 1) and (j == 2)) || ((i == 2) and (j == 1))) {
            return 5;
          } else {
            tfel::reportContractViolation("indices are not valid");
          }
        }
      }
    } else {
      tfel::reportContractViolation("dimension is not valid");
    }
  };

  template <typename NumType, typename T>
  TFEL_HOST_DEVICE constexpr void setComponent(ST2toST2Concept auto& A,
                                               unsigned short i,
                                               unsigned short j,
                                               unsigned short k,
                                               unsigned short l,
                                               const T& Aijkl) noexcept
      requires(isAssignableTo<NumType, T>()) {
    using ST2toST2Type = decltype(A);
    const auto N = getSpaceDimension<ST2toST2Type>();
    const auto I = VoigtIndex<N>(i, j);
    const auto J = VoigtIndex<N>(k, l);
    constexpr auto cste = Cste<NumType>::sqrt2;
    A(I, J) = Aijkl;
    if (I > 2) {
      A(I, J) = A(I, J) * cste;
    }
    if (J > 2) {
      A(I, J) = A(I, J) * cste;
    }
  }

  TFEL_HOST_DEVICE constexpr auto getComponent(const ST2toST2Concept auto& A,
                                               unsigned short i,
                                               unsigned short j,
                                               unsigned short k,
                                               unsigned short l) {
    using ST2toST2Type = decltype(A);
    using T = numeric_type<ST2toST2Type>;
    const auto N = getSpaceDimension<ST2toST2Type>();
    const int I = VoigtIndex<N>(i, j);
    const int J = VoigtIndex<N>(k, l);
    auto Aijkl = T(A(I, J));
    constexpr auto icste = Cste<T>::isqrt2;
    if (I > 2) {
      Aijkl *= icste;
    }
    if (J > 2) {
      Aijkl *= icste;
    }
    return Aijkl;
  }

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void st2tost2<N, T>::import(
      const base_type<T>* const src) noexcept {
    tfel::fsalgo::transform<
        StensorDimeToSize<N>::value *
        StensorDimeToSize<N>::value>::exe(src, this->begin(),
                                          [](const auto& v) { return T(v); });
  }

  template <unsigned short N, typename T>
  TFEL_HOST_DEVICE constexpr void st2tost2<N, T>::copy(const auto p) noexcept {
    tfel::fsalgo::copy<StensorDimeToSize<N>::value *
                       StensorDimeToSize<N>::value>::exe(p, *this);
  }

  TFEL_HOST constexpr auto invert(const ST2toST2Concept auto& s) {
    using ST2toST2Type = decltype(s);
    constexpr auto N = getSpaceDimension<ST2toST2Type>();
    constexpr auto StensorSize = StensorDimeToSize<N>::value;
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

  template <ST2toST2Concept ST2toST2Type>
  TFEL_HOST_DEVICE constexpr auto change_basis(
      const ST2toST2Type& s,
      const rotation_matrix<numeric_type<ST2toST2Type>>& r) noexcept {
    constexpr auto N = getSpaceDimension<ST2toST2Type>();
    using Result = st2tost2<N, numeric_type<ST2toST2Type>>;
    if constexpr (N == 1) {
      return Result{s};
    } else {
      using st2tost2 =
          tfel::math::st2tost2<N, tfel::math::numeric_type<ST2toST2Type>>;
      const auto sr = st2tost2::fromRotationMatrix(r);
      const auto sir = st2tost2::fromRotationMatrix(transpose(r));
      return Result{sr * s * sir};
    }
  }  // end of change_basis

  template <ST2toST2Concept ST2toST2Type, TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto push_forward(const ST2toST2Type& C,
                                               const TensorType& F) noexcept
      requires(getSpaceDimension<ST2toST2Type>() ==
               getSpaceDimension<TensorType>()) {
    st2tost2<getSpaceDimension<ST2toST2Type>(),
             BinaryOperationResult<numeric_type<ST2toST2Type>,
                                   numeric_type<TensorType>, OpMult>>
        r;
    push_forward(r, C, F);
    return r;
  }  // end of push_forward

  template <ST2toST2Concept ST2toST2Type, TensorConcept TensorType>
  TFEL_HOST constexpr auto pull_back(
      const ST2toST2Type& C,
      const TensorType& F) requires(getSpaceDimension<ST2toST2Type>() ==
                                    getSpaceDimension<TensorType>()) {
    const auto iF = invert(F);
    return push_forward(C, iF);
  }  // end of pull_back

  TFEL_HOST_DEVICE constexpr auto computeDeterminantSecondDerivative(
      const StensorConcept auto& s) noexcept {
    using StensorType = decltype(s);
    using NumType = numeric_type<StensorType>;
    constexpr auto N = getSpaceDimension<StensorType>();
    constexpr auto zero = NumType{0};
    static_assert((N == 1) || (N == 2) || (N == 3));
    using Result = st2tost2<N, numeric_type<StensorType>>;
    if constexpr (N == 1) {
      return Result{zero, s[2], s[1], s[2], zero, s[0], s[1], s[0], zero};
    } else if constexpr (N == 2) {
      return Result{zero, s[2], s[1], zero,  s[2], zero, s[0],  zero,
                    s[1], s[0], zero, -s[3], zero, zero, -s[3], -s[2]};
    } else {
      constexpr auto icste = Cste<NumType>::isqrt2;
      return Result{
          zero,  s[2],  s[1],  zero,         zero,         -s[5],
          s[2],  zero,  s[0],  zero,         -s[4],        zero,
          s[1],  s[0],  zero,  -s[3],        zero,         zero,
          zero,  zero,  -s[3], -s[2],        s[5] * icste, s[4] * icste,
          zero,  -s[4], zero,  s[5] * icste, -s[1],        s[3] * icste,
          -s[5], zero,  zero,  s[4] * icste, s[3] * icste, -s[0]};
    }
  }  // end of computeDeterminantSecondDerivative

  TFEL_HOST_DEVICE constexpr auto computeDeviatorDeterminantSecondDerivative(
      const StensorConcept auto& s) noexcept {
    using StensorType = decltype(s);
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    using Result =
        st2tost2<getSpaceDimension<StensorType>(), numeric_type<StensorType>>;
    if constexpr (N == 1) {
      return Result{-(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
                    (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
                    -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
                    (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9,
                    -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
                    -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
                    -(2 * s[2] - 4 * s[1] + 2 * s[0]) / 9,
                    -(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
                    (4 * s[2] - 2 * s[1] - 2 * s[0]) / 9};
    } else if constexpr (N == 2) {
      return Result{-(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
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
      return Result{-(2 * s[2] + 2 * s[1] - 4 * s[0]) / 9,
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
