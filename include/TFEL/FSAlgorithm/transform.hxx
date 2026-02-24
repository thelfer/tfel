/*!
 * \file   include/TFEL/FSAlgorithm/transform.hxx
 * \brief  This file implements the transform algorithm.
 * \author Thomas Helfer
 * \date   30 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Config/TFELConfig.hxx"

#ifndef LIB_TFEL_FSALGORITHM_TRANSFORM_HXX
#define LIB_TFEL_FSALGORITHM_TRANSFORM_HXX

namespace tfel::fsalgo {

  /*!
   * \class transform
   * \brief transform performs an operation on objects
   * \param N number of element to be transformed.
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/transform.html
   *
   * \see transform.cxx for some elementary tests.
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <unsigned int N>
  struct transform {
    /*!
     * The first version of transform::exe performs the operation op(*i) for
     * each iterator i in the range [first, first+N), and assigns the result of
     * that operation to *o, where o is the corresponding output iterator. The
     * return value is result + N.
     *
     * \param  InputIterator  iterator to the element to be transformed.
     * \param  OutputIterator iterator to which result is placed.
     * \param  UnaryFunction  function used to perform transformation.
     *
     * \return OutputIterator iterator after the last element transformed.
     *
     * \pre
     * - InputIterator must be a model of Input Iterator.
     * - OutputIterator must be a model of Output Iterator.
     * - UnaryFunction must be a model of Unary Function.
     * - InputIterator's value type must be convertible to UnaryFunction's
     * argument type.
     * - UnaryFunction's result type must be convertible to a type in
     * OutputIterator's set of value types.
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <typename InputIterator,
              typename OutputIterator,
              typename UnaryFunction>
    static constexpr OutputIterator exe(InputIterator p,
                                        OutputIterator q,
                                        UnaryFunction op) {
      *q = op(*p);
      return transform<N - 1>::exe(++p, ++q, op);
    }

    /*!
     * The second version of transform is very similar, except that it uses a
     * Binary Function instead of a Unary Function: it performs the operation
     * op(*i1, *i2) for each iterator i1 in the range [first1, last1) and
     * assigns the result to *o, where i2 is the corresponding iterator in the
     * second input range and where o is the corresponding output iterator. That
     * is, for each n such that 0 <= i < N, it performs the assignment *(result
     * + i) = op(*(first1 + i), *(first2 + i). The return value is result + N.
     *
     * \param  InputIterator1  iterator to the first  element to be transformed.
     * \param  InputIterator2  iterator to the second element to be transformed.
     * \param  OutputIterator iterator to which result is placed.
     * \param  UnaryFunction  function used to perform transformation
     *
     * \return OutputIterator iterator after the last element transformed.
     *
     * \pre
     * - InputIterator1 and InputIterator2  must be models of Input Iterator.
     * - OutputIterator must be a model of Output Iterator.
     * - BinaryFunction must be a model of Binary Function.
     * - InputIterator1's and InputIterator2's value types must be convertible,
     * respectively, to BinaryFunction's first and second argument types.
     * - UnaryFunction's result type must be convertible to a type in
     * OutputIterator's set of value types.
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <typename InputIterator1,
              typename InputIterator2,
              typename OutputIterator,
              typename BinaryFunction>
    static constexpr OutputIterator exe(InputIterator1 p,
                                        InputIterator2 q,
                                        OutputIterator r,
                                        BinaryFunction op) {
      *r = op(*p, *q);
      return transform<N - 1>::exe(++p, ++q, ++r, op);
    }
  };

  /*!
   * \brief partial specialisation used to end recursion when using the
   * transform algorithm.
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <>
  struct transform<0u> {
    /*!
     * \return the ouptut iterator to the end the recursion
     * \sa transform<N>::exe()
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <typename InputIterator,
              typename OutputIterator,
              typename UnaryFunction>
    static constexpr OutputIterator exe(InputIterator,
                                        OutputIterator q,
                                        UnaryFunction) {
      return q;
    }

    /*!
     * \return the ouptut iterator to the end the recursion
     * \sa transform<N>::exe()
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <typename InputIterator,
              typename InputIterator2,
              typename OutputIterator,
              typename BinaryFunction>
    static constexpr OutputIterator exe(InputIterator,
                                        InputIterator2,
                                        OutputIterator r,
                                        BinaryFunction) {
      return r;
    }
  };

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_TRANSFORM_HXX */
