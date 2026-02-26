/*!
 * \file   include/TFEL/Math/Tensor/TensorConcept.ixx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSOR_CONCEPT_IXX
#define LIB_TFEL_MATH_TENSOR_CONCEPT_IXX 1

#include "TFEL/FSAlgorithm/transform.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Forward/tvector.hxx"
#include "TFEL/Math/Forward/tensor.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/Tensor/MatrixViewFromTensor.hxx"
#include "TFEL/Math/Tensor/TensorTransposeExpr.hxx"

namespace tfel::math::internals {

  template <unsigned short N>
  struct TensorConceptMatrixAccessOperator;

  template <>
  struct TensorConceptMatrixAccessOperator<1u> {
    template <TensorConcept TensorType>
    static TFEL_MATH_INLINE numeric_type<TensorType> exe(
        const TensorType& t, const unsigned short i, const unsigned short j) {
      if ((i == j) && (i < 3)) {
        return t(i);
      }
      return {0};
    }
  };

  template <>
  struct TensorConceptMatrixAccessOperator<2u> {
    template <TensorConcept TensorType>
    TFEL_MATH_INLINE static numeric_type<TensorType> exe(
        const TensorType& t, const unsigned short i, const unsigned short j) {
      if ((i == j) && (i < 3)) {
        return t(i);
      } else if ((i == 0) && (j == 1)) {
        return t(3);
      } else if ((i == 1) && (j == 0)) {
        return t(4);
      }
      return {0};
    }
  };

  template <>
  struct TensorConceptMatrixAccessOperator<3u> {
    template <TensorConcept TensorType>
    TFEL_MATH_INLINE static numeric_type<TensorType> exe(
        const TensorType& t, const unsigned short i, const unsigned short j) {
      if ((i == j) && (i < 3)) {
        return t(i);
      } else if ((i == 0) && (j == 1)) {
        return t(3);
      } else if ((i == 1) && (j == 0)) {
        return t(4);
      } else if ((i == 0) && (j == 2)) {
        return t(5);
      } else if ((i == 2) && (j == 0)) {
        return t(6);
      } else if ((i == 1) && (j == 2)) {
        return t(7);
      } else if ((i == 2) && (j == 1)) {
        return t(8);
      }
      throw(TensorInvalidIndexesException());
    }
  };

}  // end of namespace tfel::math::internals

namespace tfel::math {

  template <typename T>
  TFEL_HOST numeric_type<T> TensorConceptBase<T>::operator()(
      const unsigned short i, const unsigned short j) const {
    using tfel::math::internals::TensorConceptMatrixAccessOperator;
    if ((i > 2) || (j > 2)) {
      throw(TensorInvalidIndexesException());
    }
    return TensorConceptMatrixAccessOperator<getSpaceDimension<T>()>::exe(
        static_cast<const T&>(*this), i, j);
  }

  TFEL_HOST_DEVICE constexpr auto trace(const TensorConcept auto& s) noexcept {
    return s(0) + s(1) + s(2);
  }  // end of trace

  TFEL_HOST_DEVICE constexpr auto abs(const TensorConcept auto& t) noexcept {
    using TensorType = decltype(t);
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1u) {
      return tfel::math::abs(t(0)) + tfel::math::abs(t(1)) +
             tfel::math::abs(t(2));
    } else if constexpr (N == 2u) {
      return tfel::math::abs(t(0)) + tfel::math::abs(t(1)) +
             tfel::math::abs(t(2)) + tfel::math::abs(t(3)) +
             tfel::math::abs(t(4));
    } else {
      return tfel::math::abs(t(0)) + tfel::math::abs(t(1)) +
             tfel::math::abs(t(2)) + tfel::math::abs(t(3)) +
             tfel::math::abs(t(4)) + tfel::math::abs(t(5)) +
             tfel::math::abs(t(6)) + tfel::math::abs(t(7)) +
             tfel::math::abs(t(8));
    }
  }  // end of

  TFEL_HOST_DEVICE constexpr auto syme(const TensorConcept auto& t) noexcept {
    using TensorType = decltype(t);
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    using Result = stensor<N, numeric_type<TensorType>>;
    if constexpr (N == 1u) {
      return Result{t[0], t[1], t[2]};
    } else if constexpr (N == 2u) {
      constexpr auto icste = Cste<numeric_type<TensorType>>::isqrt2;
      return Result{t[0], t[1], t[2], icste * (t[3] + t[4])};
    } else if constexpr (N == 3u) {
      constexpr auto icste = Cste<numeric_type<TensorType>>::isqrt2;
      return Result{t[0],
                    t[1],
                    t[2],
                    icste * (t[3] + t[4]),
                    icste * (t[5] + t[6]),
                    icste * (t[7] + t[8])};
    }
  }  // end of syme

  TFEL_HOST_DEVICE constexpr auto computeRightCauchyGreenTensor(
      const TensorConcept auto& t) noexcept {
    using TensorType = decltype(t);
    using real = numeric_type<TensorType>;
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    using Result = stensor<N, decltype(t(0) * t(0))>;
    if constexpr (N == 1u) {
      return Result{t[0] * t[0], t[1] * t[1], t[2] * t[2]};
    } else if constexpr (N == 2u) {
      constexpr auto cste = Cste<real>::sqrt2;
      return Result{t[0] * t[0] + t[4] * t[4], t[1] * t[1] + t[3] * t[3],
                    t[2] * t[2], cste * (t[1] * t[4] + t[0] * t[3])};
    } else if constexpr (N == 3u) {
      constexpr auto cste = Cste<real>::sqrt2;
      return Result{t[0] * t[0] + t[4] * t[4] + t[6] * t[6],
                    t[1] * t[1] + t[3] * t[3] + t[8] * t[8],
                    t[2] * t[2] + t[5] * t[5] + t[7] * t[7],
                    cste * (t[6] * t[8] + t[1] * t[4] + t[0] * t[3]),
                    cste * (t[4] * t[7] + t[2] * t[6] + t[0] * t[5]),
                    cste * (t[2] * t[8] + t[1] * t[7] + t[3] * t[5])};
    }
  }  // end of computeRightCauchyGreenTensor

  TFEL_HOST_DEVICE constexpr auto computeLeftCauchyGreenTensor(
      const TensorConcept auto& t) noexcept {
    using TensorType = decltype(t);
    using real = numeric_type<TensorType>;
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    using Result = stensor<N, decltype(t(0) * t(0))>;
    if constexpr (N == 1u) {
      return Result{t[0] * t[0], t[1] * t[1], t[2] * t[2]};
    } else if constexpr (N == 2u) {
      constexpr auto cste = Cste<real>::sqrt2;
      return Result{t[0] * t[0] + t[3] * t[3], t[1] * t[1] + t[4] * t[4],
                    t[2] * t[2], cste * (t[0] * t[4] + t[1] * t[3])};
    } else if constexpr (N == 3u) {
      constexpr auto cste = Cste<real>::sqrt2;
      return Result{t[0] * t[0] + t[3] * t[3] + t[5] * t[5],
                    t[1] * t[1] + t[4] * t[4] + t[7] * t[7],
                    t[2] * t[2] + t[6] * t[6] + t[8] * t[8],
                    cste * (t[5] * t[7] + t[0] * t[4] + t[1] * t[3]),
                    cste * (t[3] * t[8] + t[0] * t[6] + t[2] * t[5]),
                    cste * (t[1] * t[8] + t[2] * t[7] + t[4] * t[6])};
    }
  }  // end of computeLeftCauchyGreenTensor

  TFEL_HOST_DEVICE constexpr auto computeGreenLagrangeTensor(
      const TensorConcept auto& t) noexcept {
    using TensorType = decltype(t);
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    using Result = stensor<N, decltype(t(0) * t(0))>;
    if constexpr (N == 1u) {
      return Result{(t[0] * t[0] - 1) / 2, (t[1] * t[1] - 1) / 2,
                    (t[2] * t[2] - 1) / 2};
    } else if constexpr (N == 2u) {
      constexpr auto icste = Cste<numeric_type<TensorType>>::isqrt2;
      return Result{((t[0] * t[0] - 1) + t[4] * t[4]) / 2,
                    ((t[1] * t[1] - 1) + t[3] * t[3]) / 2,  //
                    (t[2] * t[2] - 1) / 2,                  //
                    icste * (t[0] * t[3] + t[1] * t[4])};
    } else if constexpr (N == 3u) {
      constexpr auto icste = Cste<numeric_type<TensorType>>::isqrt2;
      return Result{((t[0] * t[0] - 1) + t[4] * t[4] + t[6] * t[6]) / 2,
                    ((t[1] * t[1] - 1) + t[3] * t[3] + t[8] * t[8]) / 2,
                    ((t[2] * t[2] - 1) + t[5] * t[5] + t[7] * t[7]) / 2,
                    icste * (t[6] * t[8] + t[1] * t[4] + t[0] * t[3]),
                    icste * (t[4] * t[7] + t[2] * t[6] + t[0] * t[5]),
                    icste * (t[2] * t[8] + t[1] * t[7] + t[3] * t[5])};
    }
  }  // end of computeGreenLagrangeTensor

  TFEL_HOST_DEVICE constexpr auto pushForward(
      const StensorConcept auto& p, const TensorConcept auto& F) noexcept {
    return push_forward(p, F);
  }  // end of pushForward

  TFEL_HOST_DEVICE constexpr auto push_forward(
      const StensorConcept auto& p, const TensorConcept auto& F) noexcept {
    using StensorType = decltype(p);
    using TensorType = decltype(F);
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    static_assert(N == getSpaceDimension<TensorType>(),
                  "arguments must have the same space dimension");
    using Result = stensor<N, decltype(p[0] * F[0] * F[0])>;
    if constexpr (N == 1u) {
      return Result{p[0] * F[0] * F[0], p[1] * F[1] * F[1], p[2] * F[2] * F[2]};
    } else if constexpr (N == 2u) {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      return Result{
          p[1] * F[3] * F[3] + cste * p[3] * F[0] * F[3] + p[0] * F[0] * F[0],
          p[0] * F[4] * F[4] + cste * p[3] * F[1] * F[4] + p[1] * F[1] * F[1],
          p[2] * F[2] * F[2],
          (p[3] * F[3] + cste * p[0] * F[0]) * F[4] +
              cste * p[1] * F[1] * F[3] + p[3] * F[0] * F[1]};
    } else if constexpr (N == 3u) {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      return Result{
          p[2] * F[5] * F[5] +
              (cste * p[5] * F[3] + cste * p[4] * F[0]) * F[5] +
              p[1] * F[3] * F[3] + cste * p[3] * F[0] * F[3] +
              p[0] * F[0] * F[0],
          p[2] * F[7] * F[7] +
              (cste * p[4] * F[4] + cste * p[5] * F[1]) * F[7] +
              p[0] * F[4] * F[4] + cste * p[3] * F[1] * F[4] +
              p[1] * F[1] * F[1],
          p[1] * F[8] * F[8] +
              (cste * p[3] * F[6] + cste * p[5] * F[2]) * F[8] +
              p[0] * F[6] * F[6] + cste * p[4] * F[2] * F[6] +
              p[2] * F[2] * F[2],
          (cste * p[2] * F[5] + p[5] * F[3] + p[4] * F[0]) * F[7] +
              (p[4] * F[4] + p[5] * F[1]) * F[5] +
              (p[3] * F[3] + cste * p[0] * F[0]) * F[4] +
              cste * p[1] * F[1] * F[3] + p[3] * F[0] * F[1],
          (p[5] * F[5] + cste * p[1] * F[3] + p[3] * F[0]) * F[8] +
              (p[4] * F[5] + p[3] * F[3] + cste * p[0] * F[0]) * F[6] +
              cste * p[2] * F[2] * F[5] + p[5] * F[2] * F[3] +
              p[4] * F[0] * F[2],
          (p[5] * F[7] + p[3] * F[4] + cste * p[1] * F[1]) * F[8] +
              (p[4] * F[6] + cste * p[2] * F[2]) * F[7] +
              (cste * p[0] * F[4] + p[3] * F[1]) * F[6] + p[4] * F[2] * F[4] +
              p[5] * F[1] * F[2]};
    }
  }  // end of push_forward

  TFEL_HOST_DEVICE constexpr auto
  convertCauchyStressToSecondPiolaKirchhoffStress(
      const StensorConcept auto& s, const TensorConcept auto& F) noexcept {
    using StensorType = decltype(s);
    using TensorType = decltype(F);
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    static_assert(N == getSpaceDimension<TensorType>(),
                  "arguments must have the same space dimension");
    using Result = stensor<N, decltype(s[0] * F[1] * F[2] / (F[0]))>;
    if constexpr (N == 1u) {
      return Result{s[0] * F[1] * F[2] / (F[0]), s[1] * F[0] * F[2] / (F[1]),
                    s[2] * F[0] * F[1] / (F[2])};
    } else if constexpr (N == 2u) {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      const auto iF = invert(F);
      const auto J = det(F);
      return Result{(s[1] * iF[3] * iF[3] + cste * s[3] * iF[0] * iF[3] +
                     s[0] * iF[0] * iF[0]) *
                        J,
                    (s[0] * iF[4] * iF[4] + cste * s[3] * iF[1] * iF[4] +
                     s[1] * iF[1] * iF[1]) *
                        J,
                    s[2] * iF[2] * iF[2] * J,
                    ((s[3] * iF[3] + cste * s[0] * iF[0]) * iF[4] +
                     cste * s[1] * iF[1] * iF[3] + s[3] * iF[0] * iF[1]) *
                        J};
    } else {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      const auto iF = invert(F);
      const auto J = det(F);
      return Result{
          (s[2] * iF[5] * iF[5] +
           (cste * s[5] * iF[3] + cste * s[4] * iF[0]) * iF[5] +
           s[1] * iF[3] * iF[3] + cste * s[3] * iF[0] * iF[3] +
           s[0] * iF[0] * iF[0]) *
              J,
          (s[2] * iF[7] * iF[7] +
           (cste * s[4] * iF[4] + cste * s[5] * iF[1]) * iF[7] +
           s[0] * iF[4] * iF[4] + cste * s[3] * iF[1] * iF[4] +
           s[1] * iF[1] * iF[1]) *
              J,
          (s[1] * iF[8] * iF[8] +
           (cste * s[3] * iF[6] + cste * s[5] * iF[2]) * iF[8] +
           s[0] * iF[6] * iF[6] + cste * s[4] * iF[2] * iF[6] +
           s[2] * iF[2] * iF[2]) *
              J,
          ((cste * s[2] * iF[5] + s[5] * iF[3] + s[4] * iF[0]) * iF[7] +
           (s[4] * iF[4] + s[5] * iF[1]) * iF[5] +
           (s[3] * iF[3] + cste * s[0] * iF[0]) * iF[4] +
           cste * s[1] * iF[1] * iF[3] + s[3] * iF[0] * iF[1]) *
              J,
          ((s[5] * iF[5] + cste * s[1] * iF[3] + s[3] * iF[0]) * iF[8] +
           (s[4] * iF[5] + s[3] * iF[3] + cste * s[0] * iF[0]) * iF[6] +
           cste * s[2] * iF[2] * iF[5] + s[5] * iF[2] * iF[3] +
           s[4] * iF[0] * iF[2]) *
              J,
          ((s[5] * iF[7] + s[3] * iF[4] + cste * s[1] * iF[1]) * iF[8] +
           (s[4] * iF[6] + cste * s[2] * iF[2]) * iF[7] +
           (cste * s[0] * iF[4] + s[3] * iF[1]) * iF[6] + s[4] * iF[2] * iF[4] +
           s[5] * iF[1] * iF[2]) *
              J};
    }
  }  // end of convertCauchyStressToSecondPiolaKirchhoffStress

  TFEL_HOST_DEVICE constexpr auto
  convertSecondPiolaKirchhoffStressToCauchyStress(
      const StensorConcept auto& p, const TensorConcept auto& F) noexcept {
    using StensorType = decltype(p);
    using TensorType = decltype(F);
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    static_assert(N == getSpaceDimension<TensorType>(),
                  "arguments must have the same space dimension");
    using Result = stensor<N, decltype(p[0] * F[0] / (F[1] * F[2]))>;
    if constexpr (N == 1u) {
      const auto iJ = 1 / (F[0] * F[1] * F[2]);
      return Result{p[0] * F[0] * F[0] * iJ, p[1] * F[1] * F[1] * iJ,
                    p[2] * F[2] * F[2] * iJ};
    } else if constexpr (N == 2u) {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      const auto iJ = 1 / det(F);
      return Result{(p[1] * F[3] * F[3] + cste * p[3] * F[0] * F[3] +
                     p[0] * F[0] * F[0]) *
                        iJ,
                    (p[0] * F[4] * F[4] + cste * p[3] * F[1] * F[4] +
                     p[1] * F[1] * F[1]) *
                        iJ,
                    p[2] * F[2] * F[2] * iJ,
                    ((p[3] * F[3] + cste * p[0] * F[0]) * F[4] +
                     cste * p[1] * F[1] * F[3] + p[3] * F[0] * F[1]) *
                        iJ};
    } else {
      constexpr auto cste = Cste<numeric_type<StensorType>>::sqrt2;
      const auto iJ = 1 / det(F);
      return Result{(p[2] * F[5] * F[5] +
                     (cste * p[5] * F[3] + cste * p[4] * F[0]) * F[5] +
                     p[1] * F[3] * F[3] + cste * p[3] * F[0] * F[3] +
                     p[0] * F[0] * F[0]) *
                        iJ,
                    (p[2] * F[7] * F[7] +
                     (cste * p[4] * F[4] + cste * p[5] * F[1]) * F[7] +
                     p[0] * F[4] * F[4] + cste * p[3] * F[1] * F[4] +
                     p[1] * F[1] * F[1]) *
                        iJ,
                    (p[1] * F[8] * F[8] +
                     (cste * p[3] * F[6] + cste * p[5] * F[2]) * F[8] +
                     p[0] * F[6] * F[6] + cste * p[4] * F[2] * F[6] +
                     p[2] * F[2] * F[2]) *
                        iJ,
                    ((cste * p[2] * F[5] + p[5] * F[3] + p[4] * F[0]) * F[7] +
                     (p[4] * F[4] + p[5] * F[1]) * F[5] +
                     (p[3] * F[3] + cste * p[0] * F[0]) * F[4] +
                     cste * p[1] * F[1] * F[3] + p[3] * F[0] * F[1]) *
                        iJ,
                    ((p[5] * F[5] + cste * p[1] * F[3] + p[3] * F[0]) * F[8] +
                     (p[4] * F[5] + p[3] * F[3] + cste * p[0] * F[0]) * F[6] +
                     cste * p[2] * F[2] * F[5] + p[5] * F[2] * F[3] +
                     p[4] * F[0] * F[2]) *
                        iJ,
                    ((p[5] * F[7] + p[3] * F[4] + cste * p[1] * F[1]) * F[8] +
                     (p[4] * F[6] + cste * p[2] * F[2]) * F[7] +
                     (cste * p[0] * F[4] + p[3] * F[1]) * F[6] +
                     p[4] * F[2] * F[4] + p[5] * F[1] * F[2]) *
                        iJ};
    }
  }  // end of convertSecondPiolaKirchhoffStressToCauchyStress

  TFEL_HOST_DEVICE constexpr auto det(const TensorConcept auto& t) noexcept {
    using TensorType = decltype(t);
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1u) {
      return t(0) * t(1) * t(2);
    } else if constexpr (N == 2u) {
      return (t(0) * t(1) - t(3) * t(4)) * t(2);
    } else {
      const auto a = t(0);
      const auto b = t(3);
      const auto c = t(5);
      const auto d = t(4);
      const auto e = t(1);
      const auto f = t(7);
      const auto g = t(6);
      const auto h = t(8);
      const auto i = t(2);
      return a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g);
    }
  }  // end of det

  TFEL_HOST_DEVICE constexpr auto computeDeterminantDerivative(
      TensorConcept auto& dJ, const TensorConcept auto& F) noexcept {
    using TensorType = decltype(F);
    using TensorResultType = decltype(dJ);
    constexpr auto N = getSpaceDimension<TensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    static_assert(N == getSpaceDimension<TensorResultType>(),
                  "arguments must have the same space dimension");
    static_assert(
        isAssignableTo<UnaryOperationResult<numeric_type<TensorType>, Power<2>>,
                       numeric_type<TensorResultType>>(),
        "invalid arguments");
    if constexpr (N == 1u) {
      dJ[0] = F[1] * F[2];
      dJ[1] = F[0] * F[2];
      dJ[2] = F[0] * F[1];
    } else if constexpr (N == 2u) {
      dJ[0] = F[1] * F[2];
      dJ[1] = F[0] * F[2];
      dJ[2] = F[0] * F[1] - F[3] * F[4];
      dJ[3] = -F[2] * F[4];
      dJ[4] = -F[2] * F[3];
    } else {
      dJ[0] = F[1] * F[2] - F[7] * F[8];
      dJ[1] = F[0] * F[2] - F[5] * F[6];
      dJ[2] = F[0] * F[1] - F[3] * F[4];
      dJ[3] = F[6] * F[7] - F[2] * F[4];
      dJ[4] = F[5] * F[8] - F[2] * F[3];
      dJ[5] = F[4] * F[8] - F[1] * F[6];
      dJ[6] = F[3] * F[7] - F[1] * F[5];
      dJ[7] = F[3] * F[6] - F[0] * F[8];
      dJ[8] = F[4] * F[5] - F[0] * F[7];
    }
  }  // end of computeDeterminantDerivative

  TFEL_HOST void polar_decomposition(TensorConcept auto& R,
                                     StensorConcept auto& U,
                                     const TensorConcept auto& F) {
    using TensorType = decltype(R);
    using TensorType2 = decltype(F);
    using StensorType = decltype(U);
    static_assert(std::is_same<numeric_type<StensorType>,
                               numeric_type<TensorType2>>::value &&
                      std::is_same<base_type<numeric_type<TensorType2>>,
                                   numeric_type<TensorType>>::value &&
                      (getSpaceDimension<TensorType>() ==
                       getSpaceDimension<TensorType2>()) &&
                      (getSpaceDimension<TensorType>() ==
                       getSpaceDimension<StensorType>()),
                  "invalid arguments");
    if constexpr (getSpaceDimension<TensorType>() == 1u) {
      typedef numeric_type<TensorType2> T;
      using real = base_type<T>;
      R = tensor<1u, real>::Id();
      for (unsigned short i = 0; i != 3u; ++i) {
        U(i) = F(i);
      }
    } else {
      using tfel::fsalgo::transform;
      using T = numeric_type<TensorType2>;
      using base = base_type<T>;
      using T2 = BinaryOperationResult<T, T, OpMult>;
      constexpr auto dime = getSpaceDimension<TensorType>();
      const auto id = stensor<dime, base>::Id();
      const auto C = computeRightCauchyGreenTensor(F);
      const auto vp_C = C.computeEigenValues();
      tvector<3u, T> vp_U;
      transform<3u>::exe(vp_C.begin(), vp_U.begin(),
                         [](const T2& v) { return std::sqrt(v); });
      const auto i1 = vp_U[0] + vp_U[1] + vp_U[2];
      const auto i2 = vp_U[0] * vp_U[1] + vp_U[0] * vp_U[2] + vp_U[1] * vp_U[2];
      const auto i3 = vp_U[0] * vp_U[1] * vp_U[2];
      const auto D = i1 * i2 - i3;
      const auto C2 = square(C);
      U = 1 / D * (-C2 + (i1 * i1 - i2) * C + i1 * i3 * id);
      //     U = 1 / D * (-square(C) + (i1 * i1 - i2) * C + i1 * i3 * id);
      const auto U_1 = (C - i1 * U + i2 * id) * (1 / i3);
      R = F * U_1;
    }
  }  // end of polar_decomposition

  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto matrix_view(TensorType&& t) noexcept {
    using NumType = numeric_type<decltype(t)>;
    return Expr<tmatrix<3u, 3u, NumType>,
                MatrixViewFromTensorExpr<decltype(t)>>(
        std::forward<TensorType>(t));
  }  // end of matrix_view

  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto transpose(TensorType&& t) noexcept {
    constexpr auto N = getSpaceDimension<TensorType>();
    if constexpr (N == 1u) {
      return Expr<tensor<1u, numeric_type<decltype(t)>>,
                  TensorTransposeExpr1D<decltype(t)>>(
          std::forward<TensorType>(t));
    } else if constexpr (N == 2u) {
      return Expr<tensor<2u, numeric_type<decltype(t)>>,
                  TensorTransposeExpr2D<decltype(t)>>(
          std::forward<TensorType>(t));
    } else {
      return Expr<tensor<3u, numeric_type<decltype(t)>>,
                  TensorTransposeExpr3D<decltype(t)>>(
          std::forward<TensorType>(t));
    }
  }  // end of transpose

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSOR_CONCEPT_IXX */
