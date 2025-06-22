/*!
 * \file   include/TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx
 * \brief  This file declares the ObjectObjectRandomAccessConstIterator class
 * \author Helfer Thomas
 * \date   17 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATOR_H_
#define LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATOR_H_

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
    class ObjectObjectRandomAccessConstIterator {
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
      typedef
          typename ComputeBinaryResult<NumTypeA, NumTypeB, Op>::Handle NumType;
      typedef const NumType value_type;
      typedef Distance difference_type;
      typedef const NumType* pointer;
      typedef const NumType& reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator() {}

      TFEL_MATH_INLINE
      ObjectObjectRandomAccessConstIterator(const ConstIteratorTypeA pA_,
                                            const ConstIteratorTypeB pB_)
          : pA(pA_), pB(pB_) {}

      TFEL_MATH_INLINE
      ObjectObjectRandomAccessConstIterator(
          const ObjectObjectRandomAccessConstIterator& src)
          : pA(src.pA), pB(src.pB) {}

      TFEL_MATH_INLINE
      ObjectObjectRandomAccessConstIterator& operator=(
          const ObjectObjectRandomAccessConstIterator& src) {
        // self assignement does not have to be checked.
        pA = src.pA;
        pB = src.pB;
        return *this;
      }

      TFEL_MATH_INLINE value_type operator*(void) const {
        return Op::apply(*pA, *pB);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(void) {
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
        return Op::apply(pA[n], pB[n]);
      }

      TFEL_MATH_INLINE Distance
      operator-(const ObjectObjectRandomAccessConstIterator& src) const {
        return pA - src.pA;
      }

      TFEL_MATH_INLINE bool operator==(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA == src.pA) && (pB == src.pB);
      }

      TFEL_MATH_INLINE bool operator!=(
          const ObjectObjectRandomAccessConstIterator& src) const {
        return (pA != src.pA) || (pB != src.pB);
      }
    };

    template <typename A, typename B, typename Op>
    TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator<A, B, Op> operator+(
        const typename ObjectObjectRandomAccessConstIterator<A, B, Op>::Distance
            n,
        const ObjectObjectRandomAccessConstIterator<A, B, Op>& p) {
      return p + n;
    }

  }  // end of namespace math

  namespace typetraits {

    template <typename A, typename B, typename Op>
    struct IsInvalid<
        tfel::math::ObjectObjectRandomAccessConstIterator<A, B, Op>> {
     private:
      typedef typename tfel::math::
          ObjectObjectRandomAccessConstIterator<A, B, Op>::value_type T;

     public:
      /*!
       *  Result
       */
      static constexpr bool cond = IsInvalid<T>::cond;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#endif /* LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATOR_H_ */
