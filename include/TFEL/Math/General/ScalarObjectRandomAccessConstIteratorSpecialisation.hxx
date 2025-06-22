/*!
 * \file
 * include/TFEL/Math/General/ScalarObjectRandomAccessConstIteratorSpecialisation.hxx
 * \brief  This file specialises the ScalarObjectRandomAccessConstIterator an
 * ObjectScalarRandomAccessConstIterator for common operation \author Thomas
 * Helfer \date   17 Oct 2006 \copyright Copyright (C) 2006-2025 CEA/DEN, EDF
 * R&D. All rights reserved. This project is publicly released under either the
 * GNU GPL Licence with linking exception or the CECILL-A licence. A copy of
 * thoses licences are delivered with the sources of TFEL. CEA or EDF may also
 * distribute this project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SCALAROBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_HXX
#define LIB_TFEL_SCALAROBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_HXX

#include "TFEL/Math/General/ScalarObjectRandomAccessConstIterator.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"

namespace tfel {

  namespace math {

    /*
     * Partial specialisation for OpMult.
     * \see ScalarObjectRandomAccessConstIterator.
     */
    template <typename A, typename B>
    class ScalarObjectRandomAccessConstIterator<A, B, OpMult> {
      /*
       * a static assertion ensuring that A is a scalar.
       */
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      /*
       * Name
       */
      typedef typename B::const_iterator ConstIteratorType;
      /*
       * a static assertion ensuring that ConstIteratorType is a
       * random access.
       */
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorType>::cond));
      /*
       * Type pointed to by the iterator.
       */
      typedef
          typename std::iterator_traits<ConstIteratorType>::value_type NumTypeB;

      /*
       * Stores the scalar with which all iterators will be multiplied.
       */
      A a;
      /*
       * Iterator of B.
       */
      ConstIteratorType p;

      /*
       * Type returned by the difference of two B's iterators.
       */
      typedef typename std::iterator_traits<ConstIteratorType>::difference_type
          Distance;

     public:
      /*
       * Category of the iterator.
       */
      typedef std::random_access_iterator_tag iterator_category;
      /*
       * Result of the multplication between the scalar and the NumTypeB
       */
      typedef typename ComputeBinaryResult<A, NumTypeB, OpMult>::Handle NumType;
      /*
       * Type returned by the * operator.
       */
      typedef const NumType value_type;
      /*
       * Type of the difference of two ScalarObjectRandomAccessConstIterator's.
       */
      typedef Distance difference_type;
      /*
       * Type of a pointer to NumType
       */
      typedef const NumType* pointer;
      /*
       * Type of a reference to NumType
       */
      typedef const NumType& reference;

      /*
       * Default Constructor
       */
      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator() {}

      /*
       * Constructor
       * \param const A, a scalar.
       * \param const ConstIteratorType, an iterator.
       */
      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator(const A a_,
                                            const ConstIteratorType p_)
          : a(a_), p(p_) {}

      /*
       * Copy Constructor
       * \param const ScalarObjectRandomAccessConstIterator&, source.
       */
      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator(
          const ScalarObjectRandomAccessConstIterator& src)
          : a(src.a), p(src.p) {}

      /*
       * Assignement operator
       * \param const ScalarObjectRandomAccessConstIterator&, source.
       * \param const ScalarObjectRandomAccessConstIterator&, result (this).
       */
      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator=(
          const ScalarObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        a = src.a;
        p = src.p;
        return *this;
      }

      /*
       * operator *
       * \param void
       * \return value_type
       */
      TFEL_MATH_INLINE
      const value_type operator*() const { return a * (*p); }

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator++() {
        p++;
        return *this;
      }

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator++(int) {
        ++p;
        return *this;
      }

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator+=(const Distance n) {
        p += n;
        return *this;
      }

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator+(const Distance n) {
        return ScalarObjectRandomAccessConstIterator(a, p + n);
      }

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator-=(const Distance n) {
        p -= n;
        return *this;
      }

      TFEL_MATH_INLINE
      ScalarObjectRandomAccessConstIterator& operator-(const Distance n) {
        return ScalarObjectRandomAccessConstIterator(a, p - n);
      }

      TFEL_MATH_INLINE
      const value_type operator[](const Distance n) const { return a * (p[n]); }

      TFEL_MATH_INLINE
      Distance operator-(
          const ScalarObjectRandomAccessConstIterator& src) const {
        return p - src.p;
      }

      TFEL_MATH_INLINE
      bool operator==(const ScalarObjectRandomAccessConstIterator& src) const {
        return p == src.p;
      }

      TFEL_MATH_INLINE
      bool operator!=(const ScalarObjectRandomAccessConstIterator& src) const {
        return p != src.p;
      }
    };

    /*
     * Partial specialisation for OpDiv
     */
    template <typename A, typename B>
    class ScalarObjectRandomAccessConstIterator<A, B, OpDiv> {
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
      typedef typename ComputeBinaryResult<A, NumTypeB, OpDiv>::Handle NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator() {}

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator(
          const A a_, const ConstIteratorType p_)
          : a(a_), p(p_) {}

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator(
          const ScalarObjectRandomAccessConstIterator& src)
          : a(src.a), p(src.p) {}

      TFEL_MATH_INLINE ScalarObjectRandomAccessConstIterator& operator=(
          const ScalarObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        a = src.a;
        p = src.p;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return a / (*p); }

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
        return a / (p[n]);
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

    /*
     * Partial specialisation for OpMult
     */
    template <typename A, typename B>
    class ObjectScalarRandomAccessConstIterator<A, B, OpMult> {
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
      typedef typename ComputeBinaryResult<NumTypeA, B, OpMult>::Handle NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator() {}

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator(
          const ConstIteratorType p_, const B b_)
          : b(b_), p(p_) {}

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator(
          const ObjectScalarRandomAccessConstIterator& src)
          : b(src.b), p(src.p) {}

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator=(
          const ObjectScalarRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        b = src.b;
        p = src.p;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return (*p) * b; }

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
        return (p[n]) * b;
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

    /*
     * Partial specialisation for OpDiv
     */
    template <typename A, typename B>
    class ObjectScalarRandomAccessConstIterator<A, B, OpDiv> {
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
      typedef typename ComputeBinaryResult<NumTypeA, B, OpDiv>::Handle NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator() {}

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator(
          const ConstIteratorType p_, const B b_)
          : b(b_), p(p_) {}

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator(
          const ObjectScalarRandomAccessConstIterator& src)
          : b(src.b), p(src.p) {}

      TFEL_MATH_INLINE ObjectScalarRandomAccessConstIterator& operator=(
          const ObjectScalarRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        b = src.b;
        p = src.p;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return (*p) / b; }

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
        return (p[n]) / b;
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

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_SCALAROBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_HXX */
