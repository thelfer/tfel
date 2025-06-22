/*!
 * \file   include/TFEL/FSAlgorithm/for_each.hxx
 * \brief  This file implements the for_each class.
 * \author Helfer Thomas
 * \date   30 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FOR_EACH_HXX_
#define LIB_TFEL_FOR_EACH_HXX_

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace fsalgo {

    /*!
     * \class for_each
     * \brief For_each applies the function object f to each element
     * in the range [first, first+N); f's return value, if any, is
     * ignored. Applications are performed in forward order, i.e. from
     * first to last. For_each returns the function object after it
     * has been applied to each element.
     * \param N number of element to which f will be applied
     *
     * This documentation is mostly inspired from:
     * http://www.sgi.com/tech/stl/for_each.html
     *
     * \see for_each.cxx for some elementary tests.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <unsigned int N>
    struct for_each {
      /*!
       * \param InputIterator input iterator
       * \param UnaryFunction unary function which is applied to the elements
       *
       * \return UnaryFunction
       *
       * \pre
       * - InputIterator is a model of Input Iterator
       * - UnaryFunction is a model of Unary Function
       * - UnaryFunction does not apply any non-constant operation through its
       * argument.
       * - InputIterator's value type is convertible to UnaryFunction's argument
       * type.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename UnaryFunction>
      static TFEL_FSALGORITHM_INLINE void exe(InputIterator p,
                                              UnaryFunction& f) {
        f(*p);
        for_each<N - 1>::exe(++p, f);
      }
    };

    /*!
     * \brief partial specialisation of struct for_each to end recursion.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct for_each<0u> {
      /*!
       * \return UnaryFunction
       * \sa for_each<N>::exe() for details
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename UnaryFunction>
      static TFEL_FSALGORITHM_INLINE void exe(InputIterator, UnaryFunction&) {}
    };

  }  // end of namespace fsalgo

}  // end of namespace tfel

#endif /* LIB_TFEL_FOR_EACH_HXX_ */
