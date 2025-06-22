/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.ixx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef STENSOR_CONCEPT_IMPL_
#define STENSOR_CONCEPT_IMPL_ 1

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel {

  namespace math {

    template <class T>
    StensorNumType<T> StensorConcept<T>::operator()(
        const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template <class T>
    StensorNumType<T> StensorConcept<T>::operator[](
        const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template <class T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                            StensorNumType<T>>::type
    trace(const T& s) {
      return s(0) + s(1) + s(2);
    }

    namespace internals {

      struct SigmaEqImplBase {
        template <typename T>
        static T square(const T x) {
          return x * x;
        }
      };

      template <unsigned short N>
      struct SigmaEqImpl;

      template <>
      struct SigmaEqImpl<1u> : public SigmaEqImplBase {
        template <class T>
        static typename std::enable_if<
            tfel::meta::Implements<T, StensorConcept>::cond,
            StensorNumType<T>>::type
        exe(const T& s) {
          typedef StensorNumType<T> NumType;
          typedef tfel::typetraits::base_type<NumType> base;
          TFEL_CONSTEXPR const auto one_third = base{1} / base{3};
          TFEL_CONSTEXPR const auto cste = base{3} / base{2};
          const NumType tr = one_third * trace(s);
          return std::sqrt(cste * (SigmaEqImplBase::square(s(0) - tr) +
                                   SigmaEqImplBase::square(s(1) - tr) +
                                   SigmaEqImplBase::square(s(2) - tr)));
        }
      };

      template <>
      struct SigmaEqImpl<2u> : public SigmaEqImplBase {
        template <class T>
        static typename std::enable_if<
            tfel::meta::Implements<T, StensorConcept>::cond,
            StensorNumType<T>>::type
        exe(const T& s) {
          typedef StensorNumType<T> NumType;
          typedef tfel::typetraits::base_type<NumType> base;
          TFEL_CONSTEXPR const auto one_third = base(1) / base(3);
          TFEL_CONSTEXPR const auto cste = base(3) / base(2);
          const auto tr = one_third * trace(s);
          return std::sqrt(cste * (SigmaEqImplBase::square(s(0) - tr) +
                                   SigmaEqImplBase::square(s(1) - tr) +
                                   SigmaEqImplBase::square(s(2) - tr) +
                                   SigmaEqImplBase::square(s(3))));
        }
      };

      template <>
      struct SigmaEqImpl<3u> : public SigmaEqImplBase {
        template <class T>
        static typename std::enable_if<
            tfel::meta::Implements<T, StensorConcept>::cond,
            StensorNumType<T>>::type
        exe(const T& s) {
          typedef StensorNumType<T> NumType;
          typedef tfel::typetraits::base_type<NumType> base;
          TFEL_CONSTEXPR const auto one_third = base(1) / base(3);
          TFEL_CONSTEXPR const auto cste = base(3) / base(2);
          const auto tr = one_third * trace(s);
          return std::sqrt(cste * (SigmaEqImplBase::square(s(0) - tr) +
                                   SigmaEqImplBase::square(s(1) - tr) +
                                   SigmaEqImplBase::square(s(2) - tr) +
                                   SigmaEqImplBase::square(s(3)) +
                                   SigmaEqImplBase::square(s(4)) +
                                   SigmaEqImplBase::square(s(5))));
        }
      };

      template <unsigned short N>
      struct StensorAbs;

      template <>
      struct StensorAbs<1u> {
        template <typename StensorType>
        TFEL_MATH_INLINE static typename tfel::typetraits::AbsType<
            StensorNumType<StensorType>>::type
        exe(const StensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2));
        }
      };

      template <>
      struct StensorAbs<2u> {
        template <typename StensorType>
        TFEL_MATH_INLINE static typename tfel::typetraits::AbsType<
            StensorNumType<StensorType>>::type
        exe(const StensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2)) + tfel::math::abs(s(3));
        }
      };

      template <>
      struct StensorAbs<3u> {
        template <typename StensorType>
        TFEL_MATH_INLINE static typename tfel::typetraits::AbsType<
            StensorNumType<StensorType>>::type
        exe(const StensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2)) + tfel::math::abs(s(3)) +
                 tfel::math::abs(s(4)) + tfel::math::abs(s(5));
        }
      };

    }  // end of namespace internals

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        typename tfel::typetraits::AbsType<StensorNumType<StensorType>>::type>::
        type
        abs(const StensorType& s) {
      return tfel::math::internals::StensorAbs<
          StensorTraits<StensorType>::dime>::exe(s);
    }

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                            StensorNumType<T>>::type
    sigmaeq(const T& s) {
      typedef tfel::math::internals::SigmaEqImpl<StensorTraits<T>::dime> Impl;
      return Impl::exe(s);
    }

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 1u,
                            typename StensorType<T>::type>::type
    deviator(const T& s) {
      using Result = typename StensorType<T>::type;
      using NumType = StensorNumType<T>;
      using real = tfel::typetraits::base_type<NumType>;
      TFEL_CONSTEXPR const auto one_third = real{1} / real{3};
      const NumType tr = one_third * trace(s);
      return Result{s(0) - tr, s(1) - tr, s(2) - tr};
    }

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 2u,
                            typename StensorType<T>::type>::type
    deviator(const T& s) {
      using Result = typename StensorType<T>::type;
      using NumType = StensorNumType<T>;
      using real = tfel::typetraits::base_type<NumType>;
      TFEL_CONSTEXPR const auto one_third = real{1} / real{3};
      const NumType tr = one_third * trace(s);
      return Result{s(0) - tr, s(1) - tr, s(2) - tr, s(3)};
    }

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 3u,
                            typename StensorType<T>::type>::type
    deviator(const T& s) {
      using Result = typename StensorType<T>::type;
      using NumType = StensorNumType<T>;
      using real = tfel::typetraits::base_type<NumType>;
      TFEL_CONSTEXPR const auto one_third = real{1} / real{3};
      const NumType tr = one_third * trace(s);
      return Result{s(0) - tr, s(1) - tr, s(2) - tr, s(3), s(4), s(5)};
    }

    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 1u &&
            StensorTraits<StensorResultType>::dime == 1u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(StensorResultType& dJ, const StensorType& s) {
      dJ[0] = s[1] * s[2];
      dJ[1] = s[0] * s[2];
      dJ[2] = s[0] * s[1];
    }  // end of ComputeDeterminantDerivative

    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 2u &&
            StensorTraits<StensorResultType>::dime == 2u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(StensorResultType& dJ, const StensorType& s) {
      dJ[0] = s[1] * s[2];
      dJ[1] = s[0] * s[2];
      dJ[2] = s[0] * s[1] - s[3] * s[3] / 2;
      dJ[3] = -s[3] * s[2];
    }  // end of ComputeDeterminantDerivative

    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 3u &&
            StensorTraits<StensorResultType>::dime == 3u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(StensorResultType& dJ, const StensorType& s) {
      using NumType = StensorNumType<StensorType>;
      using real = tfel::typetraits::base_type<NumType>;
      TFEL_CONSTEXPR const auto one_half = real(1) / real(2);
      TFEL_CONSTEXPR const auto icste = Cste<real>::isqrt2;
      dJ[0] = s[1] * s[2] - s[5] * s[5] * one_half;
      dJ[1] = s[0] * s[2] - s[4] * s[4] * one_half;
      dJ[2] = s[0] * s[1] - s[3] * s[3] * one_half;
      dJ[3] = icste * s[4] * s[5] - s[2] * s[3];
      dJ[4] = icste * s[3] * s[5] - s[1] * s[4];
      dJ[5] = icste * s[3] * s[4] - s[0] * s[5];
    }  // end of ComputeDeterminantDerivative

  }  // end of namespace math

}  // end of namespace tfel

#endif /* STENSOR_CONCEPT_IMPL_ */
