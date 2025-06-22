/*!
 * \file   include/TFEL/FSAlgorithm/swap_ranges.hxx
 * \brief  This file implements the swap_ranges class.
 * \author Helfer Thomas
 * \date   30 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>

#ifndef LIB_TFEL_SWAP_RANGES_HXX_
#define LIB_TFEL_SWAP_RANGES_HXX_

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace fsalgo {

    /*!
     * \class swap_ranges
     * \brief swap_ranges swaps each of the elements in the range [first1,
     * first1+N) with the corresponding element in the range [first2, first2 +
     * N). That is, for each integer n such that 0 <= i < N, it swaps *(first1 +
     * i) and *(first2 + i). The return value is first2 + N. \param N size of
     * the ranges to be swapped.
     *
     * This documentation is mostly inspired from:
     * http://www.sgi.com/tech/stl/swap_ranges.html
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <unsigned int N>
    struct swap_ranges {
      /*!
       * \param ForwardIterator1
       * \param ForwardIterator
       *
       * \return  q + N
       *
       * \pre
       * - ForwardIterator1 and ForwardIterator2 must both be models of Forward
       * Iterator.
       * - The value types of ForwardIterator1 and ForwardIterator2 must be
       * convertible to each other.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator1, typename ForwardIterator2>
      static TFEL_FSALGORITHM_INLINE ForwardIterator2 exe(ForwardIterator1 p,
                                                          ForwardIterator2 q) {
        std::swap(*q, *p);
        return swap_ranges<N - 1>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation used to end recursion when using the
     * transform algorithm.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct swap_ranges<0u> {
      /*!
       * \return  ForwardIterator2 to the end the recursion
       * \sa swap_ranges<N>::exe()
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename ForwardIterator1, typename ForwardIterator2>
      static TFEL_FSALGORITHM_INLINE ForwardIterator2 exe(ForwardIterator1,
                                                          ForwardIterator2 q) {
        return q;
      }
    };

  }  // end of namespace fsalgo

}  // end of namespace tfel

#endif /* LIB_TFEL_SWAP_RANGES_HXX_ */
