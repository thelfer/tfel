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
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/MatrixViewFromTensor.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct TensorAbs;

      template <>
      struct TensorAbs<1u> {
        template <typename TensorType>
        static TFEL_MATH_INLINE
            typename tfel::typetraits::AbsType<TensorNumType<TensorType>>::type
            exe(const TensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2));
        }
      };

      template <>
      struct TensorAbs<2u> {
        template <typename TensorType>
        static TFEL_MATH_INLINE
            typename tfel::typetraits::AbsType<TensorNumType<TensorType>>::type
            exe(const TensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2)) + tfel::math::abs(s(3)) +
                 tfel::math::abs(s(4));
        }
      };

      template <>
      struct TensorAbs<3u> {
        template <typename TensorType>
        static TFEL_MATH_INLINE
            typename tfel::typetraits::AbsType<TensorNumType<TensorType>>::type
            exe(const TensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2)) + tfel::math::abs(s(3)) +
                 tfel::math::abs(s(4)) + tfel::math::abs(s(5)) +
                 tfel::math::abs(s(6)) + tfel::math::abs(s(7)) +
                 tfel::math::abs(s(8));
        }
      };

      template <unsigned short N>
      struct TensorConceptMatrixAccessOperator;

      template <>
      struct TensorConceptMatrixAccessOperator<1u> {
        template <typename TensorType>
        static TFEL_MATH_INLINE TensorNumType<TensorType> exe(
            const TensorType& t,
            const unsigned short i,
            const unsigned short j) {
          if ((i == j) && (i < 3)) {
            return t(i);
          }
          return {0};
        }
      };

      template <>
      struct TensorConceptMatrixAccessOperator<2u> {
        template <typename TensorType>
        TFEL_MATH_INLINE static TensorNumType<TensorType> exe(
            const TensorType& t,
            const unsigned short i,
            const unsigned short j) {
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
        template <typename TensorType>
        TFEL_MATH_INLINE static TensorNumType<TensorType> exe(
            const TensorType& t,
            const unsigned short i,
            const unsigned short j) {
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

    }  // end of namespace internals

    template <class T>
    TensorNumType<T> TensorConcept<T>::operator()(
        const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template <class T>
    TensorNumType<T> TensorConcept<T>::operator[](
        const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template <class T>
    TensorNumType<T> TensorConcept<T>::operator()(
        const unsigned short i, const unsigned short j) const {
      using tfel::math::internals::TensorConceptMatrixAccessOperator;
      if ((i > 2) || (j > 2)) {
        throw(TensorInvalidIndexesException());
      }
      return TensorConceptMatrixAccessOperator<TensorTraits<T>::dime>::exe(
          static_cast<const T&>(*this), i, j);
    }

    template <class T>
    typename std::enable_if<tfel::meta::Implements<T, TensorConcept>::cond,
                            TensorNumType<T>>::type
    trace(const T& s) {
      return s(0) + s(1) + s(2);
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        typename tfel::typetraits::AbsType<TensorNumType<TensorType>>::type>::
        type
        abs(const TensorType& s) {
      return tfel::math::internals::TensorAbs<
          TensorTraits<TensorType>::dime>::exe(s);
    }

    template <class T>
    typename std::enable_if<((tfel::meta::Implements<T, TensorConcept>::cond) &&
                             (TensorTraits<T>::dime == 1u)),
                            stensor<1u, TensorNumType<T>>>::type
    syme(const T& t) {
      stensor<1u, TensorNumType<T>> s;
      s[0] = t[0];
      s[1] = t[1];
      s[2] = t[2];
      return s;
    }

    template <class T>
    typename std::enable_if<((tfel::meta::Implements<T, TensorConcept>::cond) &&
                             (TensorTraits<T>::dime == 2u)),
                            stensor<2u, TensorNumType<T>>>::type
    syme(const T& t) {
      typedef TensorNumType<T> NumType;
      constexpr const auto icste = Cste<NumType>::isqrt2;
      return {t[0], t[1], t[2], icste * (t[3] + t[4])};
    }

    template <class T>
    typename std::enable_if<((tfel::meta::Implements<T, TensorConcept>::cond) &&
                             (TensorTraits<T>::dime == 3u)),
                            stensor<3u, TensorNumType<T>>>::type
    syme(const T& t) {
      typedef TensorNumType<T> NumType;
      constexpr const auto icste = Cste<NumType>::isqrt2;
      return {t[0],
              t[1],
              t[2],
              icste * (t[3] + t[4]),
              icste * (t[5] + t[6]),
              icste * (t[7] + t[8])};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<1u, TensorNumType<T>>>::type
    computeRightCauchyGreenTensor(const T& t) {
      return {t[0] * t[0], t[1] * t[1], t[2] * t[2]};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<2u, TensorNumType<T>>>::type
    computeRightCauchyGreenTensor(const T& t) {
      typedef TensorNumType<T> real;
      constexpr const auto cste = Cste<real>::sqrt2;
      return {t[0] * t[0] + t[4] * t[4], t[1] * t[1] + t[3] * t[3], t[2] * t[2],
              cste * (t[1] * t[4] + t[0] * t[3])};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<3u, TensorNumType<T>>>::type
    computeRightCauchyGreenTensor(const T& t) {
      typedef TensorNumType<T> real;
      constexpr const auto cste = Cste<real>::sqrt2;
      return {t[0] * t[0] + t[4] * t[4] + t[6] * t[6],
              t[1] * t[1] + t[3] * t[3] + t[8] * t[8],
              t[2] * t[2] + t[5] * t[5] + t[7] * t[7],
              cste * (t[6] * t[8] + t[1] * t[4] + t[0] * t[3]),
              cste * (t[4] * t[7] + t[2] * t[6] + t[0] * t[5]),
              cste * (t[2] * t[8] + t[1] * t[7] + t[3] * t[5])};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<1u, TensorNumType<T>>>::type
    computeLeftCauchyGreenTensor(const T& t) {
      return {t[0] * t[0], t[1] * t[1], t[2] * t[2]};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<2u, TensorNumType<T>>>::type
    computeLeftCauchyGreenTensor(const T& t) {
      typedef TensorNumType<T> real;
      constexpr const auto cste = Cste<real>::sqrt2;
      return {t[0] * t[0] + t[3] * t[3], t[1] * t[1] + t[4] * t[4], t[2] * t[2],
              cste * (t[0] * t[4] + t[1] * t[3])};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<3u, TensorNumType<T>>>::type
    computeLeftCauchyGreenTensor(const T& t) {
      typedef TensorNumType<T> real;
      constexpr const auto cste = Cste<real>::sqrt2;
      return {t[0] * t[0] + t[3] * t[3] + t[5] * t[5],
              t[1] * t[1] + t[4] * t[4] + t[7] * t[7],
              t[2] * t[2] + t[6] * t[6] + t[8] * t[8],
              cste * (t[5] * t[7] + t[0] * t[4] + t[1] * t[3]),
              cste * (t[3] * t[8] + t[0] * t[6] + t[2] * t[5]),
              cste * (t[1] * t[8] + t[2] * t[7] + t[4] * t[6])};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<1u, TensorNumType<T>>>::type
    computeGreenLagrangeTensor(const T& t) {
      return {(t[0] * t[0] - 1) / 2, (t[1] * t[1] - 1) / 2,
              (t[2] * t[2] - 1) / 2};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<2u, TensorNumType<T>>>::type
    computeGreenLagrangeTensor(const T& t) {
      using NumType = TensorNumType<T>;
      constexpr const auto icste = Cste<NumType>::isqrt2;
      return {(t[0] * t[0] + t[4] * t[4] - 1) / 2,
              (t[1] * t[1] + t[3] * t[3] - 1) / 2, (t[2] * t[2] - 1) / 2,
              icste * (t[0] * t[3] + t[1] * t[4])};
    }

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<3u, TensorNumType<T>>>::type
    computeGreenLagrangeTensor(const T& t) {
      using NumType = TensorNumType<T>;
      constexpr const auto icste = Cste<NumType>::isqrt2;
      return {(t[0] * t[0] + t[4] * t[4] + t[6] * t[6] - 1) / 2,
              (t[1] * t[1] + t[3] * t[3] + t[8] * t[8] - 1) / 2,
              (t[2] * t[2] + t[5] * t[5] + t[7] * t[7] - 1) / 2,
              icste * (t[6] * t[8] + t[1] * t[4] + t[0] * t[3]),
              icste * (t[4] * t[7] + t[2] * t[6] + t[0] * t[5]),
              icste * (t[2] * t[8] + t[1] * t[7] + t[3] * t[5])};
    }

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    pushForward(const T& p, const T2& F) {
      return push_forward(p, F);
    }  // end of pushForward

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    pushForward(const T& p, const T2& F) {
      return push_forward(p, F);
    }  // end of pushForward

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    pushForward(const T& p, const T2& F) {
      return push_forward(p, F);
    }  // end of pushForward

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    push_forward(const T& p, const T2& F) {
      return {p[0] * F[0] * F[0], p[1] * F[1] * F[1], p[2] * F[2] * F[2]};
    }  // end of push_forward

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    push_forward(const T& p, const T2& F) {
      typedef StensorNumType<T> NumType;
      constexpr const auto cste = Cste<NumType>::sqrt2;
      return {
          p[1] * F[3] * F[3] + cste * p[3] * F[0] * F[3] + p[0] * F[0] * F[0],
          p[0] * F[4] * F[4] + cste * p[3] * F[1] * F[4] + p[1] * F[1] * F[1],
          p[2] * F[2] * F[2],
          (p[3] * F[3] + cste * p[0] * F[0]) * F[4] +
              cste * p[1] * F[1] * F[3] + p[3] * F[0] * F[1]};
    }  // end of push_forward

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    push_forward(const T& p, const T2& F) {
      typedef StensorNumType<T> NumType;
      constexpr const auto cste = Cste<NumType>::sqrt2;
      return {p[2] * F[5] * F[5] +
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
                  (cste * p[0] * F[4] + p[3] * F[1]) * F[6] +
                  p[4] * F[2] * F[4] + p[5] * F[1] * F[2]};
    }  // end of push_forward

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T& s, const T2& F) {
      return {s[0] * F[1] * F[2] / (F[0]), s[1] * F[0] * F[2] / (F[1]),
              s[2] * F[0] * F[1] / (F[2])};
    }  // end of convertCauchyStressToSecondPiolaKirchhoffStress

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T& s, const T2& F) {
      typedef StensorNumType<T> stress;
      constexpr const auto cste = Cste<stress>::sqrt2;
      const auto iF = invert(F);
      const auto J = det(F);
      return {(s[1] * iF[3] * iF[3] + cste * s[3] * iF[0] * iF[3] +
               s[0] * iF[0] * iF[0]) *
                  J,
              (s[0] * iF[4] * iF[4] + cste * s[3] * iF[1] * iF[4] +
               s[1] * iF[1] * iF[1]) *
                  J,
              s[2] * iF[2] * iF[2] * J,
              ((s[3] * iF[3] + cste * s[0] * iF[0]) * iF[4] +
               cste * s[1] * iF[1] * iF[3] + s[3] * iF[0] * iF[1]) *
                  J};
    }  // end of convertCauchyStressToSecondPiolaKirchhoffStress

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T& s, const T2& F) {
      typedef StensorNumType<T> stress;
      constexpr const auto cste = Cste<stress>::sqrt2;
      const auto iF = invert(F);
      const auto J = det(F);
      return {(s[2] * iF[5] * iF[5] +
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
               (cste * s[0] * iF[4] + s[3] * iF[1]) * iF[6] +
               s[4] * iF[2] * iF[4] + s[5] * iF[1] * iF[2]) *
                  J};
    }  // end of convertCauchyStressToSecondPiolaKirchhoffStress

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T& p, const T2& F) {
      const auto iJ = 1 / (F[0] * F[1] * F[2]);
      return {p[0] * F[0] * F[0] * iJ, p[1] * F[1] * F[1] * iJ,
              p[2] * F[2] * F[2] * iJ};
    }  // end of convertSecondPiolaKirchhoffStressToCauchyStress

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T& p, const T2& F) {
      typedef StensorNumType<T> stress;
      constexpr const auto cste = Cste<stress>::sqrt2;
      const auto iJ = 1 / det(F);
      return {(p[1] * F[3] * F[3] + cste * p[3] * F[0] * F[3] +
               p[0] * F[0] * F[0]) *
                  iJ,
              (p[0] * F[4] * F[4] + cste * p[3] * F[1] * F[4] +
               p[1] * F[1] * F[1]) *
                  iJ,
              p[2] * F[2] * F[2] * iJ,
              ((p[3] * F[3] + cste * p[0] * F[0]) * F[4] +
               cste * p[1] * F[1] * F[3] + p[3] * F[0] * F[1]) *
                  iJ};
    }  // end of convertSecondPiolaKirchhoffStressToCauchyStress

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T& p, const T2& F) {
      typedef StensorNumType<T> stress;
      constexpr const auto cste = Cste<stress>::sqrt2;
      const auto iJ = 1 / det(F);
      return {(p[2] * F[5] * F[5] +
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
               (cste * p[0] * F[4] + p[3] * F[1]) * F[6] + p[4] * F[2] * F[4] +
               p[5] * F[1] * F[2]) *
                  iJ};
    }  // end of convertSecondPiolaKirchhoffStressToCauchyStress

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u,
        typename ComputeUnaryResult<TensorNumType<TensorType>,
                                    Power<3>>::Result>::type
    det(const TensorType& t) {
      return t(0) * t(1) * t(2);
    }  // end of

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u,
        typename ComputeUnaryResult<TensorNumType<TensorType>,
                                    Power<3>>::Result>::type
    det(const TensorType& t) {
      return (t(0) * t(1) - t(3) * t(4)) * t(2);
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u,
        typename ComputeUnaryResult<TensorNumType<TensorType>,
                                    Power<3>>::Result>::type
    det(const TensorType& t) {
      typedef TensorNumType<TensorType> T;
      const T a = t(0);
      const T b = t(3);
      const T c = t(5);
      const T d = t(4);
      const T e = t(1);
      const T f = t(7);
      const T g = t(6);
      const T h = t(8);
      const T i = t(2);
      return a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g);
    }

    template <typename TensorResultType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorResultType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u &&
            TensorTraits<TensorResultType>::dime == 1u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<TensorNumType<TensorType>,
                                            Power<2>>::Result,
                TensorNumType<TensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(TensorResultType& dJ, const TensorType& F) {
      dJ[0] = F[1] * F[2];
      dJ[1] = F[0] * F[2];
      dJ[2] = F[0] * F[1];
    }  // end of ComputeDeterminantDerivative

    template <typename TensorResultType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorResultType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u &&
            TensorTraits<TensorResultType>::dime == 2u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<TensorNumType<TensorType>,
                                            Power<2>>::Result,
                TensorNumType<TensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(TensorResultType& dJ, const TensorType& F) {
      dJ[0] = F[1] * F[2];
      dJ[1] = F[0] * F[2];
      dJ[2] = F[0] * F[1] - F[3] * F[4];
      dJ[3] = -F[2] * F[4];
      dJ[4] = -F[2] * F[3];
    }  // end of ComputeDeterminantDerivative

    template <typename TensorResultType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorResultType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u &&
            TensorTraits<TensorResultType>::dime == 3u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<TensorNumType<TensorType>,
                                            Power<2>>::Result,
                TensorNumType<TensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(TensorResultType& dJ, const TensorType& F) {
      dJ[0] = F[1] * F[2] - F[7] * F[8];
      dJ[1] = F[0] * F[2] - F[5] * F[6];
      dJ[2] = F[0] * F[1] - F[3] * F[4];
      dJ[3] = F[6] * F[7] - F[2] * F[4];
      dJ[4] = F[5] * F[8] - F[2] * F[3];
      dJ[5] = F[4] * F[8] - F[1] * F[6];
      dJ[6] = F[3] * F[7] - F[1] * F[5];
      dJ[7] = F[3] * F[6] - F[0] * F[8];
      dJ[8] = F[4] * F[5] - F[0] * F[7];
    }  // end of ComputeDeterminantDerivative

    template <typename TensorType, typename StensorType, typename TensorType2>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType2, TensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            std::is_same<StensorNumType<StensorType>,
                         TensorNumType<TensorType2>>::value &&
            std::is_same<
                tfel::typetraits::base_type<TensorNumType<TensorType2>>,
                TensorNumType<TensorType>>::value &&
            (TensorTraits<TensorType>::dime ==
             TensorTraits<TensorType2>::dime) &&
            (TensorTraits<TensorType>::dime ==
             StensorTraits<StensorType>::dime) &&
            (TensorTraits<TensorType>::dime == 1u),
        void>::type
    polar_decomposition(TensorType& R, StensorType& U, const TensorType2& F) {
      using tfel::fsalgo::copy;
      typedef TensorNumType<TensorType2> T;
      using real = tfel::typetraits::base_type<T>;
      R = tensor<1u, real>::Id();
      copy<3u>::exe(F.begin(), U.begin());
    }  // end of polar_decomposition

    template <typename TensorType, typename StensorType, typename TensorType2>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType2, TensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            std::is_same<StensorNumType<StensorType>,
                         TensorNumType<TensorType2>>::value &&
            std::is_same<
                tfel::typetraits::base_type<TensorNumType<TensorType2>>,
                TensorNumType<TensorType>>::value &&
            (TensorTraits<TensorType>::dime ==
             TensorTraits<TensorType2>::dime) &&
            (TensorTraits<TensorType>::dime ==
             StensorTraits<StensorType>::dime) &&
            ((TensorTraits<TensorType>::dime == 2u) ||
             (TensorTraits<TensorType>::dime == 3u)),
        void>::type
    polar_decomposition(TensorType& R, StensorType& U, const TensorType2& F) {
      using tfel::fsalgo::transform;
      using T = TensorNumType<TensorType2>;
      using base = tfel::typetraits::base_type<T>;
      using T2 = typename ComputeBinaryResult<T, T, OpMult>::Result;
      constexpr auto dime = TensorTraits<TensorType>::dime;
      const auto id = stensor<dime, base>::Id();
      tvector<3u, T2> vp_C;
      tvector<3u, T> vp_U;
      const auto C = computeRightCauchyGreenTensor(F);
      C.computeEigenValues(vp_C);
      transform<3u>::exe(vp_C.begin(), vp_U.begin(),
                         [](const T2& v) { return std::sqrt(v); });
      const auto i1 = vp_U[0] + vp_U[1] + vp_U[2];
      const auto i2 = vp_U[0] * vp_U[1] + vp_U[0] * vp_U[2] + vp_U[1] * vp_U[2];
      const auto i3 = vp_U[0] * vp_U[1] * vp_U[2];
      const auto D = i1 * i2 - i3;
      U = 1 / D * (-square(C) + (i1 * i1 - i2) * C + i1 * i3 * id);
      const auto U_1 = (C - i1 * U + i2 * id) * (1 / i3);
      R = F * U_1;
    }  // end of polar_decomposition

    template <typename TensorType>
    TFEL_MATH_INLINE auto matrix_view(TensorType&& t) ->
        typename std::enable_if<
            tfel::meta::Implements<typename std::decay<TensorType>::type,
                                   TensorConcept>::cond,
            Expr<tmatrix<3u,
                         3u,
                         TensorNumType<typename std::decay<decltype(t)>::type>>,
                 MatrixViewFromTensorExpr<decltype(t)>>>::type {
      using NumType = TensorNumType<typename std::decay<decltype(t)>::type>;
      return Expr<tmatrix<3u, 3u, NumType>,
                  MatrixViewFromTensorExpr<decltype(t)>>(
          std::forward<TensorType>(t));
    }  // end of matrix_view

    template <typename TensorType>
    TFEL_MATH_INLINE auto transpose(TensorType&& t) -> typename std::enable_if<
        tfel::meta::Implements<typename std::decay<TensorType>::type,
                               TensorConcept>::cond &&
            (TensorTraits<typename std::decay<TensorType>::type>::dime == 1u),
        Expr<tensor<1u, TensorNumType<typename std::decay<decltype(t)>::type>>,
             TensorTransposeExpr1D<decltype(t)>>>::type {
      return Expr<
          tensor<1u, TensorNumType<typename std::decay<decltype(t)>::type>>,
          TensorTransposeExpr1D<decltype(t)>>(std::forward<TensorType>(t));
    }  // end of transpose

    template <typename TensorType>
    TFEL_MATH_INLINE auto transpose(TensorType&& t) -> typename std::enable_if<
        tfel::meta::Implements<typename std::decay<TensorType>::type,
                               TensorConcept>::cond &&
            (TensorTraits<typename std::decay<TensorType>::type>::dime == 2u),
        Expr<tensor<2u, TensorNumType<typename std::decay<decltype(t)>::type>>,
             TensorTransposeExpr2D<decltype(t)>>>::type {
      return Expr<
          tensor<2u, TensorNumType<typename std::decay<decltype(t)>::type>>,
          TensorTransposeExpr2D<decltype(t)>>(std::forward<TensorType>(t));
    }  // end of transpose

    template <typename TensorType>
    TFEL_MATH_INLINE auto transpose(TensorType&& t) -> typename std::enable_if<
        tfel::meta::Implements<typename std::decay<TensorType>::type,
                               TensorConcept>::cond &&
            (TensorTraits<typename std::decay<TensorType>::type>::dime == 3u),
        Expr<tensor<3u, TensorNumType<typename std::decay<decltype(t)>::type>>,
             TensorTransposeExpr3D<decltype(t)>>>::type {
      return Expr<
          tensor<3u, TensorNumType<typename std::decay<decltype(t)>::type>>,
          TensorTransposeExpr3D<decltype(t)>>(std::forward<TensorType>(t));
    }  // end of transpose

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSOR_CONCEPT_IXX */
