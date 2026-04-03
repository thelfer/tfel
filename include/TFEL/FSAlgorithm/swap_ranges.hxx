/*!
 * \file   include/TFEL/FSAlgorithm/swap_ranges.hxx
 * \brief  This file implements the swap_ranges class.
 * \author Thomas Helfer
 * \date   30 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Config/TFELConfig.hxx"

#ifndef LIB_TFEL_FSALGORITHM_SWAP_RANGES_HXX
#define LIB_TFEL_FSALGORITHM_SWAP_RANGES_HXX

namespace tfel::fsalgo {

  /*!
   * \class swap_ranges
   * \brief swap_ranges swaps each of the elements in the range [first1,
   * first1+N) with the corresponding element in the range [first2, first2 + N).
   * That is, for each integer n such that 0 <= i < N, it swaps *(first1 + i)
   * and *(first2 + i). The return value is first2 + N. \param N size of the
   * ranges to be swapped.
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/swap_ranges.html
   *
   * \author Thomas Helfer
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
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <typename ForwardIterator1, typename ForwardIterator2>
    static TFEL_FSALGORITHM_INLINE ForwardIterator2 exe(ForwardIterator1 p,
                                                        ForwardIterator2 q) {
      if constexpr (N >= 1) {
        std::swap(*q, *p);
        return swap_ranges<N - 1>::exe(++p, ++q);
      } else {
        static_cast<void>(p);
        return q;
      }
    }
  };

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_SWAP_RANGES_HXX */
