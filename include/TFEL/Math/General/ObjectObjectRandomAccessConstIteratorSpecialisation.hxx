/*!
 * \file
 * include/TFEL/Math/General/ObjectObjectRandomAccessConstIteratorSpecialisation.hxx
 * \brief  This file declares the  class
 * \author Thomas Helfer
 * \date   17 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_HXX
#define LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_HXX

#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"

namespace tfel {

  namespace math {

    /*
     * Partial Specialisation
     */
    template <typename A, typename B>
    class ObjectObjectRandomAccessConstIterator<A, B, OpPlus> {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type
          NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type
          NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type
          Distance;

     public:
      typedef std::random_access_iterator_tag iterator_category;
      typedef typename ComputeBinaryResult<NumTypeA, NumTypeB, OpPlus>::Handle
          NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator() {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ConstIteratorTypeA pA_, const ConstIteratorTypeB pB_)
          : pA(pA_), pB(pB_) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ObjectObjectRandomAccessConstIterator& src)
          : pA(src.pA), pB(src.pB) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(
          const ObjectObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        pA = src.pA;
        pB = src.pB;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return (*pA) + (*pB); }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++() {
        pA++;
        pB++;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int) {
        ++pA;
        ++pB;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(
          const Distance n) {
        pA += n;
        pB += n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA + n, pB + n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(
          const Distance n) {
        pA -= n;
        pB -= n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA - n, pB - n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const {
        return (pA[n]) + (pB[n]);
      }

      TFEL_MATH_INLINE const Distance
      operator-(const ObjectObjectRandomAccessConstIterator& src) const {
        return pA - src.pA;
      }

      TFEL_MATH_INLINE const bool operator==(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA == src.pA) && (pB == src.pB);
      }

      TFEL_MATH_INLINE const bool operator!=(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA != src.pA) || (pB != src.pB);
      }
    };

    /*
     * Partial Specialisation
     */
    template <typename A, typename B>
    class ObjectObjectRandomAccessConstIterator<A, B, OpMinus> {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type
          NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type
          NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type
          Distance;

     public:
      typedef std::random_access_iterator_tag iterator_category;
      typedef typename ComputeBinaryResult<NumTypeA, NumTypeB, OpMinus>::Handle
          NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator() {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ConstIteratorTypeA pA_, const ConstIteratorTypeB pB_)
          : pA(pA_), pB(pB_) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ObjectObjectRandomAccessConstIterator& src)
          : pA(src.pA), pB(src.pB) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(
          const ObjectObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        pA = src.pA;
        pB = src.pB;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return (*pA) - (*pB); }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++() {
        pA++;
        pB++;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int) {
        ++pA;
        ++pB;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(
          const Distance n) {
        pA += n;
        pB += n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA + n, pB + n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(
          const Distance n) {
        pA -= n;
        pB -= n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA - n, pB - n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const {
        return (pA[n]) - (pB[n]);
      }

      TFEL_MATH_INLINE const Distance
      operator-(const ObjectObjectRandomAccessConstIterator& src) const {
        return pA - src.pA;
      }

      TFEL_MATH_INLINE const bool operator==(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA == src.pA) && (pB == src.pB);
      }

      TFEL_MATH_INLINE const bool operator!=(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA != src.pA) || (pB != src.pB);
      }
    };

    /*
     * Partial Specialisation
     */
    template <typename A, typename B>
    class ObjectObjectRandomAccessConstIterator<A, B, OpMult> {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type
          NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type
          NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type
          Distance;

     public:
      typedef std::random_access_iterator_tag iterator_category;
      typedef typename ComputeBinaryResult<NumTypeA, NumTypeB, OpMult>::Handle
          NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator() {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ConstIteratorTypeA pA_, const ConstIteratorTypeB pB_)
          : pA(pA_), pB(pB_) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ObjectObjectRandomAccessConstIterator& src)
          : pA(src.pA), pB(src.pB) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(
          const ObjectObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        pA = src.pA;
        pB = src.pB;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return (*pA) * (*pB); }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++() {
        pA++;
        pB++;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int) {
        ++pA;
        ++pB;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(
          const Distance n) {
        pA += n;
        pB += n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA + n, pB + n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(
          const Distance n) {
        pA -= n;
        pB -= n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA - n, pB - n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const {
        return (pA[n]) * (pB[n]);
      }

      TFEL_MATH_INLINE const Distance
      operator-(const ObjectObjectRandomAccessConstIterator& src) const {
        return pA - src.pA;
      }

      TFEL_MATH_INLINE const bool operator==(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA == src.pA) && (pB == src.pB);
      }

      TFEL_MATH_INLINE const bool operator!=(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA != src.pA) || (pB != src.pB);
      }
    };

    /*
     * Partial Specialisation
     */
    template <typename A, typename B>
    class ObjectObjectRandomAccessConstIterator<A, B, OpDiv> {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type
          NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type
          NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type
          Distance;

     public:
      typedef std::random_access_iterator_tag iterator_category;
      typedef typename ComputeBinaryResult<NumTypeA, NumTypeB, OpDiv>::Handle
          NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator() {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ConstIteratorTypeA pA_, const ConstIteratorTypeB pB_)
          : pA(pA_), pB(pB_) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(
          const ObjectObjectRandomAccessConstIterator& src)
          : pA(src.pA), pB(src.pB) {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(
          const ObjectObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        pA = src.pA;
        pB = src.pB;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*() const { return (*pA) / (*pB); }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++() {
        pA++;
        pB++;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int) {
        ++pA;
        ++pB;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(
          const Distance n) {
        pA += n;
        pB += n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA + n, pB + n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(
          const Distance n) {
        pA -= n;
        pB -= n;
        return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(
          const Distance n) {
        return ObjectObjectRandomAccessConstIterator(pA - n, pB - n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const {
        return (pA[n]) / (pB[n]);
      }

      TFEL_MATH_INLINE const Distance
      operator-(const ObjectObjectRandomAccessConstIterator& src) const {
        return pA - src.pA;
      }

      TFEL_MATH_INLINE const bool operator==(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA == src.pA) && (pB == src.pB);
      }

      TFEL_MATH_INLINE const bool operator!=(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA != src.pA) || (pB != src.pB);
      }
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_HXX */
