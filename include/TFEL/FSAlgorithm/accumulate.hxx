/*!
 * \file   include/TFEL/FSAlgorithm/accumulate.hxx
 * \brief  This file implements the accumulate class.
 * \author Thomas Helfer
 * \date   30 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FSALGORITHM_ACCUMULATE_HXX
#define LIB_TFEL_FSALGORITHM_ACCUMULATE_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::fsalgo {

  /*!
   * \class accumulate
   * \brief accumulate computes the sum (or some other binary operation) of N
   * elements of a container. \param N number of element to be summed.
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/transform.html
   *
   * \see accumulate.cxx for some elementary tests.
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <unsigned int N>
  struct accumulate {
    /*!
     * \param InputIterator iterator of the sequence to be summed
     * \param T  type of the result
     *
     * \return result of the summation:
     * \f[\displaystyle\mathrm{init}+\sum_{i=0}^{N-1}*(p+i)\f]
     *
     * \pre
     * - InputIterator is a model of Input Iterator.
     * - T is a model of Assignable.
     * - If x is an object of type T and y is an object of InputIterator's value
     * type, then x + y is defined.
     * - The return type of x + y is convertible to T.
     */
    template <typename InputIterator, typename T>
    static T exe(InputIterator p, const T init) {
      if constexpr (N >= 1) {
        const auto result = *p + init;
        return accumulate<N - 1>::exe(++p, result);
      } else {
        static_cast<void>(p);
        return init;
      }
    }

    /*!
     *
     * \param InputIterator  iterator of the sequence to be operated on
     * \param T              type of the result
     * \param BinaryFunction function doing the operation
     * \return result of the operation :
     *
     \f[\displaystyle\mathrm{Op}\left(*(p+N-1),\ldots,\mathrm{Op}\left(*p,\mathrm{init}\right)\ldots\right)\f]
     *
     * \pre
     * - InputIterator is a model of Input Iterator.
       - T is a model of Assignable.
       - BinaryFunction is a model of Binary Function.
       - T is convertible to BinaryFunction's first argument type.
       - The value type of InputIterator is convertible to BinaryFunction's
     second argument type.
       - BinaryFunction's return type is convertible to T.
     *
     */
    template <class InputIterator, class T, class BinaryFunction>
    static T exe(InputIterator p, const T init, BinaryFunction binary_op) {
      if constexpr (N >= 1) {
        const auto result = binary_op(*p, init);
        return accumulate<N - 1>::exe(++p, result, binary_op);
      } else {
        static_cast<void>(p);
        static_cast<void>(binary_op);
        return init;
      }
    }
  };

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_ACCUMULATE_HXX */
