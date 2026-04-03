/*!
 * \file   include/TFEL/FSAlgorithm/equal.hxx
 * \brief  this file implements the equal class.
 * \author Thomas Helfer
 * \date   30 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FSALGORITHM_EQUAL_HXX
#define LIB_TFEL_FSALGORITHM_EQUAL_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::fsalgo {

  /*!
   * \class equal
   * \brief Equal returns true if the two ranges [first1, first1+N) and [first2,
   * first2 + N) are identical when compared element-by-element, and otherwise
   * returns false \param N number of element to be compared.
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/equal.html
   *
   * \see transform.cxx for some elementary tests.
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <unsigned int N>
  struct equal {
    /*!
     * \param  InputIterator  iterator for the first  sequence
     * \param  InputIterator2 iterator for the second sequence
     *
     * \return true if all elements are equal
     *
     * \pre
     * - InputIterator1 is a model of Input Iterator.
     * - InputIterator2 is a model of Input Iterator.
     * - InputIterator1's value type is a model of Equality Comparable.
     * - InputIterator2's value type is a model of Equality Comparable.
     * - InputIterator1's value type can be compared for equality with
     InputIterator2's value type.

     */
    template <typename InputIterator, typename InputIterator2>
    static TFEL_FSALGORITHM_INLINE bool exe(InputIterator p, InputIterator2 q) {
      if constexpr (N >= 1) {
        return (*p == *q) && (equal<N - 1>::exe(++p, ++q));
      } else {
        static_cast<void>(p);
        static_cast<void>(q);
        return true;
      }
    }

    /*!
     * \param  InputIterator  iterator for the first  sequence
     * \param  InputIterator2 iterator for the second sequence
     * \param  BinaryPredicate predicate used to make comparison
     * \return true if all elements are one-by-one equal given the predicate
     * used.
     *
     * \pre
     * -  InputIterator1 is a model of Input Iterator.
     * -  InputIterator2 is a model of Input Iterator.
     * -  BinaryPredicate is a model of Binary Predicate.
     * -  InputIterator1's value type is convertible to BinaryPredicate's first
     * argument type.
     * -  InputIterator2's value type is convertible to BinaryPredicate's second
     * argument type.
     */
    template <typename InputIterator,
              typename InputIterator2,
              typename BinaryPredicate>
    static TFEL_FSALGORITHM_INLINE bool exe(InputIterator p,
                                            InputIterator2 q,
                                            BinaryPredicate binary_pred) {
      if constexpr (N >= 1) {
        return (binary_pred(*p, *q)) &&
               (equal<N - 1>::exe(++p, ++q, binary_pred));
      } else {
        static_cast<void>(p);
        static_cast<void>(q);
        static_cast<void>(binary_pred);
        return true;
      }
    }
  };

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_EQUAL_HXX */
