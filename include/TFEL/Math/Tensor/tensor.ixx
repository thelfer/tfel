/*!
 * \file   include/TFEL/Math/Tensor/tensor.ixx
 * \brief  This file implements the methods of the class tensor.
 * \author Thomas Helfer
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSOR_IXX
#define LIB_TFEL_MATH_TENSOR_IXX

#include <cmath>
#include <iterator>
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  void tensor<N, T>::buildFromFortranMatrix(tensor<N, T>& t,
                                            const base_type<T>* const v) {
    t[0] = T(v[0]);
    t[1] = T(v[4]);
    t[2] = T(v[8]);
    if constexpr ((N == 2) || (N == 3)) {
      t[3] = T(v[3]);
      t[4] = T(v[1]);
    }
    if constexpr (N == 3) {
      t[5] = T(v[6]);
      t[6] = T(v[2]);
      t[7] = T(v[7]);
      t[8] = T(v[5]);
    }
  }  // end of void tensor<N,T>::buildFromFortranMatrix

  template <unsigned short N, typename T>
  tensor<N, T> tensor<N, T>::buildFromFortranMatrix(
      const base_type<T>* const v) {
    tensor<N, T> t;
    tensor<N, T>::buildFromFortranMatrix(t, v);
    return t;
  }  // end of void tensor<N,T>::buildFromFortranMatrix

  template <unsigned short N, typename ValueType>
  constexpr ValueType tensor<N, ValueType>::operator()(
      const typename tensor::size_type i) const {
    return GenericFixedSizeArrayBase::operator()(i);
  }  // end of operator()

  template <unsigned short N, typename ValueType>
  constexpr ValueType& tensor<N, ValueType>::operator()(
      const typename tensor::size_type i) {
    return GenericFixedSizeArrayBase::operator()(i);
  }

  template <unsigned short N, typename T>
  T tensor<N, T>::operator()(const typename tensor<N, T>::size_type i,
                             const typename tensor<N, T>::size_type j) const {
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if ((i > 2) || (j > 2)) {
      throw(TensorInvalidIndexesException());
    }
    if constexpr (N == 1) {
      return (i == j) ? (*this)(i) : T(0);
    } else if constexpr (N == 2) {
      if (i == j) {
        return (*this)(i);
      } else if ((i == 0) && (j == 1)) {
        return (*this)(3);
      } else if ((i == 1) && (j == 0)) {
        return (*this)(4);
      }
      return T(0);
    } else {
      if (i == j) {
        return (*this)(i);
      } else if ((i == 0) && (j == 1)) {
        return (*this)(3);
      } else if ((i == 1) && (j == 0)) {
        return (*this)(4);
      } else if ((i == 0) && (j == 2)) {
        return (*this)(5);
      } else if ((i == 2) && (j == 0)) {
        return (*this)(6);
      } else if ((i == 1) && (j == 2)) {
        return (*this)(7);
      }
      return (*this)(8);
    }
  }  // end of operator()

  template <unsigned short N, typename T>
  void tensor<N, T>::import(const base_type<T>* const src) {
    tfel::fsalgo::transform<TensorDimeToSize<N>::value>::exe(
        src, this->begin(), [](const auto& v) { return T(v); });
  }

  template <unsigned short N, typename T>
  void tensor<N, T>::write(base_type<T>* const t) const {
    tfel::fsalgo::transform<TensorDimeToSize<N>::value>::exe(
        this->cbegin(), t, [](const auto& v) { return base_type_cast(v); });
  }

  template <unsigned short N, typename T>
  void tensor<N, T>::changeBasis(const rotation_matrix<T>& m) noexcept {
    const auto rt = change_basis(*this, m);
    *this = rt;
  }  // end of changeBasis

  template <unsigned short N, typename T>
  constexpr tensor<N, base_type<T>> tensor<N, T>::Id() {
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    constexpr auto zero = base_type<T>{0};
    constexpr auto one = base_type<T>{1};
    if constexpr (N == 1) {
      return {one, one, one};
    } else if constexpr (N == 2) {
      return {one, one, one, zero, zero};
    }
    return {one, one, one, zero, zero, zero, zero, zero, zero};
  }  // end of tensor<N,T>::Id

  template <unsigned short N, typename T>
  template <typename InputIterator>
  void tensor<N, T>::copy(const InputIterator src) {
    tfel::fsalgo::copy<TensorDimeToSize<N>::value>::exe(src, *this);
  }

  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_MATH_INLINE2 std::enable_if_t<isScalar<T>(), void> exportToBaseTypeArray(
      const tensor<N, T>& t, OutputIterator p) {
    tfel::fsalgo::transform<TensorDimeToSize<N>::value>::exe(
        t.cbegin(), p, [](const auto& v) { return base_type_cast(v); });
  }

  template <typename TensorType>
  std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      tensor<getSpaceDimension<TensorType>(),
             typename ComputeBinaryResult<base_type<numeric_type<TensorType>>,
                                          numeric_type<TensorType>,
                                          OpDiv>::Result>>
  invert(const TensorType& t) noexcept {
    using real = base_type<numeric_type<TensorType>>;
    constexpr auto one = real(1);
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1) {
      return {one / t[0], one / t[1], one / t[2]};
    } else if constexpr (N == 2) {
      const auto id = one / (t[0] * t[1] - t[3] * t[4]);
      return {t[1] * id, t[0] * id, one / t[2], -t[3] * id, -t[4] * id};
    } else {
      const auto id = one / det(t);
      return {
          (t[1] * t[2] - t[7] * t[8]) * id, (t[0] * t[2] - t[5] * t[6]) * id,
          (t[0] * t[1] - t[3] * t[4]) * id, (t[5] * t[8] - t[2] * t[3]) * id,
          (t[6] * t[7] - t[2] * t[4]) * id, (t[3] * t[7] - t[1] * t[5]) * id,
          (t[4] * t[8] - t[1] * t[6]) * id, (t[4] * t[5] - t[0] * t[7]) * id,
          (t[3] * t[6] - t[0] * t[8]) * id};
    }
  }  // end of invert

  template <typename TensorType>
  std::enable_if_t<implementsTensorConcept<TensorType>(),
                   tensor<getSpaceDimension<TensorType>(),
                          typename ComputeUnaryResult<numeric_type<TensorType>,
                                                      Power<2>>::Result>>
  computeDeterminantDerivative(const TensorType& F) {
    tensor<
        getSpaceDimension<TensorType>(),
        typename ComputeUnaryResult<numeric_type<TensorType>, Power<2>>::Result>
        dJ;
    computeDeterminantDerivative(dJ, F);
    return dJ;
  }

  template <typename TensorType>
  std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      tensor<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  change_basis(const TensorType& t,
               const rotation_matrix<numeric_type<TensorType>>& r) noexcept {
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1) {
      return t;
    } else if constexpr (N == 2) {
      const auto a = r(0, 0);
      const auto b = r(0, 1);
      const auto d = r(1, 0);
      const auto e = r(1, 1);
      return {a * d * t[4] + a * d * t[3] + d * d * t[1] + a * a * t[0],
              b * e * t[4] + b * e * t[3] + e * e * t[1] + b * b * t[0], t[2],
              b * d * t[4] + a * e * t[3] + d * e * t[1] + a * b * t[0],
              a * e * t[4] + b * d * t[3] + d * e * t[1] + a * b * t[0]};
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
      return {d * g * t[8] + d * g * t[7] + a * g * t[6] + a * g * t[5] +
                  a * d * t[4] + a * d * t[3] + g * g * t[2] + d * d * t[1] +
                  a * a * t[0],
              e * h * t[8] + e * h * t[7] + b * h * t[6] + b * h * t[5] +
                  b * e * t[4] + b * e * t[3] + h * h * t[2] + e * e * t[1] +
                  b * b * t[0],
              f * i * t[8] + f * i * t[7] + c * i * t[6] + c * i * t[5] +
                  c * f * t[4] + c * f * t[3] + i * i * t[2] + f * f * t[1] +
                  c * c * t[0],
              e * g * t[8] + d * h * t[7] + b * g * t[6] + a * h * t[5] +
                  b * d * t[4] + a * e * t[3] + g * h * t[2] + d * e * t[1] +
                  a * b * t[0],
              d * h * t[8] + e * g * t[7] + a * h * t[6] + b * g * t[5] +
                  a * e * t[4] + b * d * t[3] + g * h * t[2] + d * e * t[1] +
                  a * b * t[0],
              f * g * t[8] + d * i * t[7] + c * g * t[6] + a * i * t[5] +
                  c * d * t[4] + a * f * t[3] + g * i * t[2] + d * f * t[1] +
                  a * c * t[0],
              d * i * t[8] + f * g * t[7] + a * i * t[6] + c * g * t[5] +
                  a * f * t[4] + c * d * t[3] + g * i * t[2] + d * f * t[1] +
                  a * c * t[0],
              f * h * t[8] + e * i * t[7] + c * h * t[6] + b * i * t[5] +
                  c * e * t[4] + b * f * t[3] + h * i * t[2] + e * f * t[1] +
                  b * c * t[0],
              e * i * t[8] + f * h * t[7] + b * i * t[6] + c * h * t[5] +
                  b * f * t[4] + c * e * t[3] + h * i * t[2] + e * f * t[1] +
                  b * c * t[0]};
    }
  }  // end of change_basis

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      tensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  unsyme(const StensorType& s) {
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1) {
      return {s[0], s[1], s[2]};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<numeric_type<StensorType>>::isqrt2;
      const auto s01 = s[3] * cste;
      return {s[0], s[1], s[2], s01, s01};
    } else {
      constexpr auto cste = Cste<numeric_type<StensorType>>::isqrt2;
      const auto s01 = s[3] * cste;
      const auto s02 = s[4] * cste;
      const auto s12 = s[5] * cste;
      return {s[0], s[1], s[2], s01, s01, s02, s02, s12, s12};
    }
  }

  template <typename StensorType, typename TensorType>
  std::enable_if_t<(implementsStensorConcept<StensorType>() &&
                    implementsTensorConcept<TensorType>()),
                   tensor<getSpaceDimension<StensorType>(),
                          result_type<numeric_type<StensorType>,
                                      numeric_type<TensorType>,
                                      OpMult>>>
  convertCauchyStressToFirstPiolaKirchhoffStress(const StensorType& s,
                                                 const TensorType& F) {
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    static_assert(getSpaceDimension<TensorType>() == N);
    if constexpr (N == 1) {
      return {s[0] * F[1] * F[2], F[0] * s[1] * F[2], F[0] * s[2] * F[1]};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      constexpr auto icste = Cste<numeric_type<StensorType>>::isqrt2;
      return {-(s[3] * F[2] * F[3] - cste * s[0] * F[1] * F[2]) * icste,
              -(s[3] * F[2] * F[4] - cste * F[0] * s[1] * F[2]) * icste,
              F[0] * s[2] * F[1] - s[2] * F[3] * F[4],
              -(cste * s[0] * F[2] * F[4] - F[0] * s[3] * F[2]) * icste,
              -(cste * s[1] * F[2] * F[3] - s[3] * F[1] * F[2]) * icste};
    } else {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      return {-((2 * s[0] * F[7] - cste * s[3] * F[5]) * F[8] -
                cste * s[4] * F[3] * F[7] + cste * s[4] * F[1] * F[5] +
                cste * s[3] * F[2] * F[3] - 2 * s[0] * F[1] * F[2]) /
                  2,
              ((cste * s[3] * F[6] - cste * F[0] * s[5]) * F[7] -
               2 * s[1] * F[5] * F[6] + cste * s[5] * F[4] * F[5] -
               cste * s[3] * F[2] * F[4] + 2 * F[0] * s[1] * F[2]) /
                  2,
              ((cste * s[4] * F[4] - cste * F[0] * s[5]) * F[8] +
               (cste * s[5] * F[3] - cste * s[4] * F[1]) * F[6] -
               2 * s[2] * F[3] * F[4] + 2 * F[0] * s[2] * F[1]) /
                  2,
              ((2 * s[0] * F[6] - cste * F[0] * s[4]) * F[7] -
               cste * s[3] * F[5] * F[6] + cste * s[4] * F[4] * F[5] -
               2 * s[0] * F[2] * F[4] + cste * F[0] * s[3] * F[2]) /
                  2,
              -((cste * s[3] * F[7] - 2 * s[1] * F[5]) * F[8] -
                cste * s[5] * F[3] * F[7] + cste * s[5] * F[1] * F[5] +
                2 * s[1] * F[2] * F[3] - cste * s[3] * F[1] * F[2]) /
                  2,
              ((2 * s[0] * F[4] - cste * F[0] * s[3]) * F[8] +
               (cste * s[3] * F[3] - 2 * s[0] * F[1]) * F[6] -
               cste * s[4] * F[3] * F[4] + cste * F[0] * s[4] * F[1]) /
                  2,
              -((cste * s[4] * F[7] - cste * s[5] * F[5]) * F[8] -
                2 * s[2] * F[3] * F[7] + 2 * s[2] * F[1] * F[5] +
                cste * s[5] * F[2] * F[3] - cste * s[4] * F[1] * F[2]) /
                  2,
              ((cste * s[3] * F[4] - 2 * F[0] * s[1]) * F[8] +
               (2 * s[1] * F[3] - cste * s[3] * F[1]) * F[6] -
               cste * s[5] * F[3] * F[4] + cste * F[0] * s[5] * F[1]) /
                  2,
              ((cste * s[4] * F[6] - 2 * F[0] * s[2]) * F[7] -
               cste * s[5] * F[5] * F[6] + 2 * s[2] * F[4] * F[5] -
               cste * s[4] * F[2] * F[4] + cste * F[0] * s[5] * F[2]) /
                  2};
    }
  }  // end of convertCauchyStressToFirstPiolaKirchhoffStress

  template <typename TensorType, typename TensorType2>
  std::enable_if_t<(implementsTensorConcept<TensorType>() &&
                    implementsTensorConcept<TensorType2>()),
                   stensor<getSpaceDimension<TensorType>(),
                           result_type<numeric_type<TensorType>,
                                       numeric_type<TensorType2>,
                                       OpMult>>>
  convertFirstPiolaKirchhoffStressToCauchyStress(const TensorType& P,
                                                 const TensorType2& F) {
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    static_assert(getSpaceDimension<TensorType2>() == N);
    if constexpr (N == 1) {
      return {P[0] / (F[1] * F[2]), P[1] / (F[0] * F[2]), P[2] / (F[0] * F[1])};
    } else if constexpr (N == 2) {
      constexpr auto cste = Cste<numeric_type<TensorType2>>::sqrt2;
      const auto iJ = 1 / det(F);
      return {(P[3] * F[3] + F[0] * P[0]) * iJ,
              (P[4] * F[4] + P[1] * F[1]) * iJ, P[2] * F[2] * iJ,
              cste * (P[1] * F[3] + F[0] * P[4]) * iJ};
    } else {
      constexpr auto cste = Cste<numeric_type<TensorType2>>::sqrt2;
      const auto iJ = 1 / det(F);
      return {(P[5] * F[5] + P[3] * F[3] + F[0] * P[0]) * iJ,
              (P[7] * F[7] + P[4] * F[4] + P[1] * F[1]) * iJ,
              (P[8] * F[8] + P[6] * F[6] + P[2] * F[2]) * iJ,
              cste * (P[7] * F[5] + P[1] * F[3] + F[0] * P[4]) * iJ,
              cste * (P[2] * F[5] + P[8] * F[3] + F[0] * P[6]) * iJ,
              cste * (P[2] * F[7] + P[6] * F[4] + P[8] * F[1]) * iJ};
    }
  }  // end of convertFirstPiolaKirchhoffStressToCauchyStress

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSOR_IXX */
