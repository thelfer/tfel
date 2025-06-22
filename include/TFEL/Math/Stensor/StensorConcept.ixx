/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.ixx
 * \author Helfer Thomas
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
    typename StensorTraits<T>::NumType StensorConcept<T>::operator()(
        const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template <class T>
    typename StensorTraits<T>::NumType StensorConcept<T>::operator[](
        const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template <class T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                            typename StensorTraits<T>::NumType>::type
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
            typename StensorTraits<T>::NumType>::type
        exe(const T& s) {
          typedef typename StensorTraits<T>::NumType NumType;
          typedef typename tfel::typetraits::BaseType<NumType>::type base;
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
            typename StensorTraits<T>::NumType>::type
        exe(const T& s) {
          typedef typename StensorTraits<T>::NumType NumType;
          typedef typename tfel::typetraits::BaseType<NumType>::type base;
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
            typename StensorTraits<T>::NumType>::type
        exe(const T& s) {
          typedef typename StensorTraits<T>::NumType NumType;
          typedef typename tfel::typetraits::BaseType<NumType>::type base;
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
            typename StensorTraits<StensorType>::NumType>::type
        exe(const StensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2));
        }
      };

      template <>
      struct StensorAbs<2u> {
        template <typename StensorType>
        TFEL_MATH_INLINE static typename tfel::typetraits::AbsType<
            typename StensorTraits<StensorType>::NumType>::type
        exe(const StensorType& s) {
          return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
                 tfel::math::abs(s(2)) + tfel::math::abs(s(3));
        }
      };

      template <>
      struct StensorAbs<3u> {
        template <typename StensorType>
        TFEL_MATH_INLINE static typename tfel::typetraits::AbsType<
            typename StensorTraits<StensorType>::NumType>::type
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
        typename tfel::typetraits::AbsType<
            typename StensorTraits<StensorType>::NumType>::type>::type
    abs(const StensorType& s) {
      return tfel::math::internals::StensorAbs<
          StensorTraits<StensorType>::dime>::exe(s);
    }

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                            typename StensorTraits<T>::NumType>::type
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
      using NumType = typename StensorTraits<T>::NumType;
      using real = typename tfel::typetraits::BaseType<NumType>::type;
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
      using NumType = typename StensorTraits<T>::NumType;
      using real = typename tfel::typetraits::BaseType<NumType>::type;
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
      using NumType = typename StensorTraits<T>::NumType;
      using real = typename tfel::typetraits::BaseType<NumType>::type;
      TFEL_CONSTEXPR const auto one_third = real{1} / real{3};
      const NumType tr = one_third * trace(s);
      return Result{s(0) - tr, s(1) - tr, s(2) - tr, s(3), s(4), s(5)};
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* STENSOR_CONCEPT_IMPL_ */
