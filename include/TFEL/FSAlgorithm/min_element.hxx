/*!
 * \file   include/TFEL/FSAlgorithm/min_element.hxx
 * \brief  This file implements the min_element class.
 * \author Thomas Helfer
 * \date   30 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MIN_ELEMENT_HXX
#define LIB_TFEL_MIN_ELEMENT_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace fsalgo {

    /*!
     * \class min_element
     * \brief min_element finds the smallest element in the range [first,
     * first+N) \param N number of element to be transformed.
     *
     * This documentation is mostly inspired from:
     * http://www.sgi.com/tech/stl/min_element.html
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <unsigned int N>
    struct min_element {
      /*
       * Internal function member used by min_element<N>::exe
       * \param ForwardIterator, iterator to the elements.
       *
       * \return ForwardIterator to the minimum element
       *
       * \pre
       * - ForwardIterator is a model of Forward Iterator.
       * - ForwardIterator's value type is LessThan Comparable.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe_(ForwardIterator p,
                                                          ForwardIterator q) {
        ForwardIterator result;
        if (*p < *q) {
          result = p;
        } else {
          result = q;
        }
        return min_element<N - 1>::exe_(++p, result);
      }

      /*
       * Internal function member used by min_element<N>::exe
       *
       * \param ForwardIterator, iterator to the elements.
       * \param BinaryPredicate, comparison function
       *
       * \return ForwardIterator to the minimum element
       *
       * \pre
       * - ForwardIterator is a model of Forward Iterator.
       * - BinaryPredicate is a model of Binary Predicate.
       * - ForwardIterator's value type is convertible to BinaryPredicate's
       * first argument type and second argument type.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator, typename BinaryPredicate>
      static TFEL_FSALGORITHM_INLINE ForwardIterator
      exe_(ForwardIterator p, ForwardIterator q, BinaryPredicate comp) {
        ForwardIterator result;
        if (comp(*p, *q)) {
          result = p;
        } else {
          result = q;
        }
        return min_element<N - 1>::exe_(++p, result, comp);
      }

     public:
      /*!
       * It returns the first iterator i in [first, first+N) such that no other
       * iterator in [first, last) points to a value greater than *i. The return
       * value is first+N if and only if [first, last) is an empty range.
       *
       * \param ForwardIterator, iterator to the elements.
       *
       * \return ForwardIterator to the minimum element
       *
       * \pre
       * - ForwardIterator is a model of Forward Iterator.
       * - ForwardIterator's value type is LessThan Comparable.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe(ForwardIterator p) {
        ForwardIterator result = p;
        return min_element<N - 1>::exe_(++p, result);
      }

      /*!
       * This version version returns the first iterator i in [first, first+N)
       * such that, for every iterator j in [first, first+N), comp(*i, *j) is
       * false, where comp is a binary predicate given in argument
       *
       * \param ForwardIterator, iterator to the elements.
       * \param BinaryPredicate, comparison function
       *
       * \return ForwardIterator to the minimum element
       *
       * \pre
       * - ForwardIterator is a model of Forward Iterator.
       * - BinaryPredicate is a model of Binary Predicate.
       * - ForwardIterator's value type is convertible to BinaryPredicate's
       * first argument type and second argument type.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator, typename BinaryPredicate>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe(ForwardIterator p,
                                                         BinaryPredicate comp) {
        ForwardIterator result = p;
        return min_element<N - 1>::exe_(++p, result, comp);
      }
    };

    /*!
     * \brief partial specialisation used to end recursion when using the
     * min_element algorithm.
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <>
    struct min_element<1u> {
      /*!
       * \return the ForwardIterator to end the recursion
       * \sa min_element<N>::exe_() for more details.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe_(ForwardIterator p,
                                                          ForwardIterator q) {
        if (*p < *q) {
          return p;
        }
        return q;
      }

      /*!
       * \return the ForwardIterator to end the recursion
       * \sa min_element<N>::exe_() for more details.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator, typename BinaryPredicate>
      static TFEL_FSALGORITHM_INLINE ForwardIterator
      exe_(ForwardIterator p, ForwardIterator q, BinaryPredicate comp) {
        if (comp(*p, *q)) {
          return p;
        }
        return q;
      }

     public:
      /*!
       * \return the ForwardIterator to end the recursion
       * \sa min_element<N>::exe() for more details.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe(ForwardIterator p) {
        return p;
      }

      /*!
       * \return the ForwardIterator to end the recursion
       * \sa min_element<N>::exe() for more details.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator, typename BinaryPredicate>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe(ForwardIterator p,
                                                         BinaryPredicate) {
        return p;
      }
    };

    /*!
     * \brief partial specialisation used to end recursion when using the
     * min_element algorithm.
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <>
    struct min_element<0u> {
      /*!
       * \return the ForwardIterator
       * \sa min_element<N>::exe() for more details.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe(ForwardIterator p) {
        return p;
      }

      /*!
       * \return the ForwardIterator
       * \sa min_element<N>::exe() for more details.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator, typename BinaryPredicate>
      static TFEL_FSALGORITHM_INLINE ForwardIterator exe(ForwardIterator p,
                                                         BinaryPredicate) {
        return p;
      }
    };

  }  // end of namespace fsalgo

}  // end of namespace tfel

#endif /* LIB_TFEL_MIN_ELEMENT_HXX */
