/*!
 * \file   include/TFEL/FSAlgorithm/inner_product.hxx
 * \brief  This file implements the inner_product class.
 * \author Thomas Helfer
 * \date   30 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FSALGORITHM_INNER_PRODUCT_HXX
#define LIB_TFEL_FSALGORITHM_INNER_PRODUCT_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::fsalgo {

  /*!
   * \class inner_product
   * \brief inner_product calculates a generalized inner product of the ranges
   * [first1,first1+N) and [first2,first2+N) \param N number of element of each
   * ranges
   *
   * This documentation is mostly inspired from:
   * http://www.sgi.com/tech/stl/inner_product.html
   *
   * \see inner_product.cxx for some elementary tests.
   *
   * \author Thomas Helfer
   * \date   30 Jun 2006
   */
  template <unsigned int N>
  struct inner_product {
    /*!
     * The first version of inner_product returns init plus the inner product of
     * the two ranges [1]. That is, it first initializes the result to init and
     * then computes:
     * \f[\displaystyle\mathrm{init}+\sum_{i=0}^{N-1}(*(p+i))*(*(q+i))\f]
     *
     * \param q: iterator to the first range
     * \param q: iterator to the second range
     * \param T initial value
     *
     * \return T, result of the inner product
     *
     * \pre
     * - InputIterator1 is a model of Input Iterator.
     * - InputIterator2 is a model of Input Iterator.
     * - T is a model of Assignable.
     * - If x is an object of type T, y is an object of InputIterator1's value
     * type, and z is an object of InputIterator2's value type, then x + y * z
     * is defined.
     * - The type of x + y * z is convertible to T.
     */
    template <typename InputIterator1, typename InputIterator2, typename T>
    TFEL_HOST_DEVICE static constexpr T exe(InputIterator1 p,
                                            InputIterator2 q,
                                            const T init) {
      if constexpr (N >= 1) {
        const auto r = init + (*p) * (*q);
        return inner_product<N - 1>::exe(++p, ++q, r);
      } else {
        static_cast<void>(p);
        static_cast<void>(q);
        return init;
      }
    }

    /*!
     * The second version of inner_product is identical to the first, except
     * that it uses two user-supplied function objects instead of operator+ and
     * operator*. That is, it first initializes the result to init and then, for
     * each iterator i in [first1, last1), in order from the beginning to the
     * end of the range, updates the result by result = binary_op1(result,
     * binary_op2(*i, *(first2 + (i - first1))).
     *
     * \param InputIterator1 iterator to the first range
     * \param InputIterator2 iterator to the second range
     * \param T initial value
     * \param BinaryFunction1
     * \param BinaryFunction2
     *
     * \return T, result
     *
     * \pre
     * - InputIterator1 is a model of Input Iterator.
     * - InputIterator2 is a model of Input Iterator.
     * - T is a model of Assignable.
     * - BinaryFunction1 is a model of Binary Function.
     * - BinaryFunction2 is a model of Binary Function.
     * - InputIterator1's value type is convertible to BinaryFunction2's first
     * argument type.
     * - InputIterator2's value type is convertible to BinaryFunction2's second
     * argument type.
     * - T is convertible to BinaryFunction1's first argument type.
     * - BinaryFunction2's return type is convertible to BinaryFunction1's
     * second argument type.
     * - BinaryFunction1's return type is convertible to T.
     */
    template <typename InputIterator1,
              typename InputIterator2,
              typename T,
              typename BinaryFunction1,
              typename BinaryFunction2>
    TFEL_HOST_DEVICE static constexpr T exe(InputIterator1 p,
                                            InputIterator2 q,
                                            T init,
                                            BinaryFunction1 binary_op1,
                                            BinaryFunction2 binary_op2) {
      if constexpr (N >= 1) {
        const auto r = binary_op1(init, binary_op2(*p, *q));
        return inner_product<N - 1>::exe(++p, ++q, r, binary_op1, binary_op2);
      } else {
        static_cast<void>(p);
        static_cast<void>(q);
        static_cast<void>(binary_op1);
        static_cast<void>(binary_op2);
        return init;
      }
    }

    /*!
     * The third version of inner_product returns the inner product
     * of the two ranges.
     * That is, it computes:
     * \f[\displaystyle\sum_{i=0}^{N-1}(*(p+i))*(*(q+i))\f]
     *
     * \param InputIterator1 iterator to the first range
     * \param InputIterator2 iterator to the second range
     *
     * \return T, result of the inner product
     *
     * \pre
     * - InputIterator1 is a model of Input Iterator.
     * - InputIterator2 is a model of Input Iterator.
     * - T is a model of Assignable.
     * - If x is an object of type T, y is an object of InputIterator1's value
     * type, and z is an object of InputIterator2's value type, then x + y * z
     * is defined.
     * - The type of x + y * z is convertible to T.
     */
    template <typename T, typename InputIterator1, typename InputIterator2>
    TFEL_HOST_DEVICE static constexpr T exe(InputIterator1 p,
                                            InputIterator2 q) {
      if constexpr (N == 0) {
        return T{};
      } else {
        const auto r = (*p) * (*q);
        return inner_product<N - 1>::exe(++p, ++q, r);
      }
    }
  };

}  // end of namespace tfel::fsalgo

#endif /* LIB_TFEL_FSALGORITHM_INNER_PRODUCT_HXX */
