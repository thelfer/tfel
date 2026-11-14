/*!
 * \file   include/TFEL/FSAlgorithm/generate.hxx
 * \brief  This file implements the generate class.
 * \author Thomas Helfer
 * \date 30/06/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FSALGORITHM_GENERATE_HXX
#define LIB_TFEL_FSALGORITHM_GENERATE_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::fsalgo {

  /*!
   * \class generate
   * \brief generate assigns the result of invoking gen, a function object that
   * takes no arguments, to each element in the range [first, first+N).
   * \tparam N: number of elements to which generate is applied.
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/generate.html
   *
   * \see generate.cxx for some elementary tests.
   *
   * \author Thomas Helfer
   * \date 30/06/2006
   */
  template <unsigned int N>
  struct generate {
    /*!
     * \tparam ForwardIterator: iterator to the elements
     * \tparam Generator: type of the generator
     * \param[in] p: iterator to the first element
     * \param[in] gen: generator
     */
    template <typename ForwardIterator, typename Generator>
    static void exe(ForwardIterator p, Generator gen) {
      if constexpr (N >= 1) {
        *p = gen();
        generate<N - 1>::exe(++p, gen);
      }
    }
  };

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_GENERATE_HXX */
