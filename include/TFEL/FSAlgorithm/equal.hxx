/*!
 * \file   include/TFEL/FSAlgorithm/equal.hxx
 * \brief  this file implements the equal class.
 * \author Helfer Thomas
 * \date   30 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_EQUAL_HXX_
#define LIB_TFEL_EQUAL_HXX_

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace fsalgo {

    /*!
     * \class equal
     * \brief Equal returns true if the two ranges [first1, first1+N) and
     * [first2, first2 + N) are identical when compared element-by-element, and
     * otherwise returns false \param N number of element to be compared.
     *
     * This documentation is mostly inspired from:
     * http://www.sgi.com/tech/stl/equal.html
     *
     * \see transform.cxx for some elementary tests.
     *
     * \author Helfer Thomas
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
      static TFEL_FSALGORITHM_INLINE bool exe(InputIterator p,
                                              InputIterator2 q) {
        return (*p == *q) && (equal<N - 1>::exe(++p, ++q));
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
       * -  InputIterator1's value type is convertible to BinaryPredicate's
       * first argument type.
       * -  InputIterator2's value type is convertible to BinaryPredicate's
       * second argument type.
       */
      template <typename InputIterator,
                typename InputIterator2,
                typename BinaryPredicate>
      static TFEL_FSALGORITHM_INLINE bool exe(InputIterator p,
                                              InputIterator2 q,
                                              BinaryPredicate binary_pred) {
        return (binary_pred(*p, *q)) &&
               (equal<N - 1>::exe(++p, ++q, binary_pred));
      }
    };

    /*!
     * \brief partial specialisation of struct equal to end recursion.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct equal<0u> {
      /*!
       * \return True
       */
      template <typename InputIterator, typename InputIterator2>
      static TFEL_FSALGORITHM_INLINE bool exe(InputIterator, InputIterator2) {
        return true;
      }

      /*!
       * \return True
       */
      template <typename InputIterator,
                typename InputIterator2,
                typename BinaryPredicate>
      static TFEL_FSALGORITHM_INLINE bool exe(InputIterator,
                                              InputIterator2,
                                              BinaryPredicate) {
        return true;
      }
    };

  }  // end of namespace fsalgo

}  // end of namespace tfel

#endif /* LIB_TFEL_EQUAL_HXX_ */
