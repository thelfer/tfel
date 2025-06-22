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

#ifndef LIB_TFEL_ACCUMULATE_HXX
#define LIB_TFEL_ACCUMULATE_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace fsalgo {

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
       * - If x is an object of type T and y is an object of InputIterator's
       * value type, then x + y is defined.
       * - The return type of x + y is convertible to T.
       */
      template <typename InputIterator, typename T>
      static TFEL_FSALGORITHM_INLINE T exe(InputIterator p, T init) {
        T result = *p + init;
        return accumulate<N - 1>::exe(++p, result);
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
      static TFEL_FSALGORITHM_INLINE T exe(InputIterator p,
                                           T init,
                                           BinaryFunction binary_op) {
        T result = binary_op(*p, init);
        return accumulate<N - 1>::exe(++p, result, binary_op);
      }
    };

    /*!
     * \brief partial specialisation of struct accumulate to end recursion.
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <>
    struct accumulate<0u> {
      /*!
       * \return the value of init
       * \sa accumulate<N>::exe()
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename T>
      static TFEL_FSALGORITHM_INLINE T exe(InputIterator, T init) {
        return init;
      }

      /*!
       * \return the value of init
       * \sa accumulate<N>::exe()
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <class InputIterator, class T, class BinaryFunction>
      static TFEL_FSALGORITHM_INLINE T exe(InputIterator,
                                           T init,
                                           BinaryFunction) {
        return init;
      }
    };

  }  // end of namespace fsalgo

}  // end of namespace tfel

#endif /* LIB_TFEL_ACCUMULATE_HXX */
