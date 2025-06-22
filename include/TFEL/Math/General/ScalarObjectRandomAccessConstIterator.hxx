/*!
 * \file   include/TFEL/Math/General/ScalarObjectRandomAccessConstIterator.hxx
 * \brief  This file declares the ScalarObjectRandomAccessConstIterator class
 * \author Thomas Helfer
 * \date   17 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SCALAROBJECTRANDOMACCESSCONSTITERATOR_HXX
#define LIB_TFEL_SCALAROBJECTRANDOMACCESSCONSTITERATOR_HXX

#include <iterator>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/IsRandomAccessIterator.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"

namespace tfel {

  namespace math {

    template <typename A, typename B, typename Op>
    class ScalarObjectRandomAccessConstIterator {
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      typedef typename B::const_iterator ConstIteratorType;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorType>::cond));
      typedef
          typename std::iterator_traits<ConstIteratorType>::value_type NumTypeB;

      A a;
      ConstIteratorType p;

      typedef typename std::iterator_traits<ConstIteratorType>::difference_type
          Distance;

     public:
      typedef std::random_access_iterator_tag iterator_category;
      typedef typename ComputeBinaryResult<A, NumTypeB, Op>::Handle NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator() {}

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator(const A a_,
                                            const ConstIteratorType p_)
          : a(a_), p(p_) {}

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator(
          const ScalarObjectRandomAccessConstIterator& src)
          : a(src.a), p(src.p) {}

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator=(
          const ScalarObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        a = src.a;
        p = src.p;
        return *this;
      }

      TFEL_MATH_INLINE
      value_type operator*() const { return Op::apply(a, *p); }

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator& operator++() {
        p++;
        return *this;
      }

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator& operator++(int) {
        ++p;
        return *this;
      }

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator& operator+=(
          const Distance n) {
        p += n;
        return *this;
      }

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator& operator+(
          const Distance n) {
        return ScalarObjectRandomAccessConstIterator(a, p + n);
      }

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator& operator-=(
          const Distance n) {
        p -= n;
        return *this;
      }

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator& operator-(
          const Distance n) {
        return ScalarObjectRandomAccessConstIterator(a, p - n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const {
        return Op::apply(a, p[n]);
      }

      TFEL_MATH_INLINE Distance
      operator-(const ScalarObjectRandomAccessConstIterator& src) const {
        return p - src.p;
      }

      TFEL_MATH_INLINE bool operator==(
          const ScalarObjectRandomAccessConstIterator& src) const {
        return p == src.p;
      }

      TFEL_MATH_INLINE bool operator!=(
          const ScalarObjectRandomAccessConstIterator& src) const {
        return p != src.p;
      }
    };

    template <typename A, typename B, typename Op>
    class ObjectScalarRandomAccessConstIterator {
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      typedef typename A::const_iterator ConstIteratorType;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorType>::cond));
      typedef
          typename std::iterator_traits<ConstIteratorType>::value_type NumTypeA;

      B b;
      ConstIteratorType p;

     public:
      typedef typename std::iterator_traits<ConstIteratorType>::difference_type
          Distance;

      typedef std::random_access_iterator_tag iterator_category;
      typedef typename ComputeBinaryResult<NumTypeA, B, Op>::Handle NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator() {}

      TFEL_MATH_INLINE
      ObjectScalarRandomAccessConstIterator(const ConstIteratorType p_,
                                            const B b_)
          : b(b_), p(p_) {}

      TFEL_MATH_INLINE
      ObjectScalarRandomAccessConstIterator(
          const ObjectScalarRandomAccessConstIterator& src)
          : b(src.b), p(src.p) {}

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator=(
          const ObjectScalarRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        b = src.b;
        p = src.p;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return Op::apply(*p, b); }

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator++() {
        p++;
        return *this;
      }

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator++(int) {
        ++p;
        return *this;
      }

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator+=(
          const Distance n) {
        p += n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator+(
          const Distance n) {
        return ObjectScalarRandomAccessConstIterator(p + n, b);
      }

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator-=(
          const Distance n) {
        p -= n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator-(
          const Distance n) {
        return ObjectScalarRandomAccessConstIterator(p - n, b);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const {
        return Op::apply(p[n], b);
      }

      TFEL_MATH_INLINE Distance
      operator-(const ObjectScalarRandomAccessConstIterator& src) const {
        return p - src.p;
      }

      TFEL_MATH_INLINE bool operator==(
          const ObjectScalarRandomAccessConstIterator& src) const {
        return p == src.p;
      }

      TFEL_MATH_INLINE bool operator!=(
          const ObjectScalarRandomAccessConstIterator& src) const {
        return p != src.p;
      }
    };

    template <typename A, typename B, typename Op>
    TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator<A, B, Op> operator+(
        const typename ScalarObjectRandomAccessConstIterator<A, B, Op>::Distance
            n,
        const ScalarObjectRandomAccessConstIterator<A, B, Op>& p) {
      return p + n;
    }

    template <typename A, typename B, typename Op>
    TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator<A, B, Op> operator+(
        const typename ObjectScalarRandomAccessConstIterator<A, B, Op>::Distance
            n,
        const ObjectScalarRandomAccessConstIterator<A, B, Op>& p) {
      return p + n;
    }

  }  // end of namespace math

  namespace typetraits {

    template <typename A, typename B, typename Op>
    struct IsInvalid<
        tfel::math::ScalarObjectRandomAccessConstIterator<A, B, Op>> {
     private:
      typedef typename tfel::math::
          ScalarObjectRandomAccessConstIterator<A, B, Op>::value_type T;

     public:
      /*!
       *  Result
       */
      static constexpr bool cond = IsInvalid<T>::cond;
    };

    template <typename A, typename B, typename Op>
    struct IsInvalid<
        tfel::math::ObjectScalarRandomAccessConstIterator<A, B, Op>> {
     private:
      typedef typename tfel::math::
          ObjectScalarRandomAccessConstIterator<A, B, Op>::value_type T;

     public:
      /*!
       *  Result
       */
      static constexpr bool cond = IsInvalid<T>::cond;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#include "TFEL/Math/General/ScalarObjectRandomAccessConstIteratorSpecialisation.hxx"

#endif /* LIB_TFEL_SCALAROBJECTRANDOMACCESSCONSTITERATOR_HXX */
