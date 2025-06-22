/*!
 * \file   include/TFEL/FSAlgorithm/iota.hxx
 * \brief  This file implements the iota class.
 * \author Thomas Helfer
 * \date   01 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FSALGORITHM_IOTA_HXX
#define LIB_TFEL_FSALGORITHM_IOTA_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::fsalgo {

  /*!
   * \class iota
   * \brief iota assigns sequentially increasing values to a range. That is, it
   * assigns value to *first, value + 1 to *(first + 1) and so on. \param N
   * number of elements.
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/iota.html
   *
   * \see iota.cxx for some elementary tests.
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <unsigned int N>
  struct iota {
    /*!
     * \param ForwardIterator iterator to the range in concern
     * \param T  type of the value assigned to the sequence
     *
     * \return void
     *
     * \pre
     * - ForwardIterator is a model of Forward Iterator.
     * - ForwardIterator is mutable.
     * - T is Assignable.
     * - If x is an object of type T, then x++ is defined.
     * - T is convertible to ForwardIterator's value type.
     *
     */
    template <typename ForwardIterator, typename T>
    static TFEL_FSALGORITHM_INLINE void exe(ForwardIterator p, T value) {
      *p = value;
      iota<N - 1>::exe(++p, ++value);
    }
  };

  /*!
   * \brief partial specialisation used to end recursion when using the iota
   * algorithm.
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <>
  struct iota<0u> {
    /*!
     * \return void
     * \sa iota<N>::exe() for details
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <typename ForwardIterator, typename T>
    static TFEL_FSALGORITHM_INLINE void exe(ForwardIterator, T) {}
  };

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_IOTA_HXX */
