/*!
 * \file   include/TFEL/FSAlgorithm/fill.hxx
 * \brief  This file implements the fill class.
 * \author Thomas Helfer
 * \date   30 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FSALGORITHM_FILL_HXX
#define LIB_TFEL_FSALGORITHM_FILL_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::fsalgo {

  /*!
   * \class fill
   * \brief Fill assigns the value value to every element in the range [first,
   * last). That is, for every iterator i in [first, first+N), it performs the
   * assignment *i = value. \param N number of element to be filled.
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/fill.html
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <unsigned int N>
  struct fill {
    /*!
     * \param  p: iterator to the elements being filled.
     * \param  q: value.
     */
    template <typename OutputIterator, typename T>
    static void exe(OutputIterator p, const T& q) {
      if constexpr (N >= 1) {
        *p = q;
        fill<N - 1>::exe(++p, q);
      } else {
        static_cast<void>(p);
        static_cast<void>(q);
      }
    }
  };  // end of fill

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_FILL_HXX */
