/*!
 * \file   include/TFEL/Math/General/DotProduct.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_DOTPRODUCT_HXX
#define LIB_TFEL_DOTPRODUCT_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"

namespace tfel {

  namespace math {

    /*!
     * \class dotProduct
     * \brief dotProduct calculates a generalized inner product of the ranges
     * [first1,first1+N) and [first2,first2+N) \param N number of element of
     * each ranges
     *
     * Contrary to inner_product, dotProduct takes automatically into account
     * the fact that complex must be conjugated
     *
     * \see inner_product
     *
     * \author Thomas Helfer
     * \date   01 Sep 2006
     */
    template <unsigned int N>
    struct dotProduct {
      /*!
       * The first version of dotProduct returns init plus the inner product of
       * the two ranges [1]. That is, it first initializes the result to init
       * and then computes:
       * \f[\displaystyle\mathrm{init}+\sum_{i=0}^{N-1}\overline{(*(p+i))}*(*(q+i))\f]
       *
       * \param InputIterator1 iterator to the first range
       * \param InputIterator2 iterator to the second range
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
      static TFEL_MATH_INLINE T exe(InputIterator1 p,
                                    InputIterator2 q,
                                    T init) {
        const T value = init + (conj(*p)) * (*q);
        return dotProduct<N - 1>::exe(++p, ++q, value);
      }

    };  // end of struct dotProduct<N>

    /*!
     * \brief partial specialisation used to end recursion when using the
     * dotProduct algorithm.
     *
     * \author Thomas Helfer
     * \date   30 Jun 2006
     */
    template <>
    struct dotProduct<0u> {
      /*!
       * \return the result at the end the recursion.
       * \sa dotProduct<N>::exe() for details.
       *
       * \author Thomas Helfer
       * \date   30 Jun 2006
       */
      template <typename InputIterator1, typename InputIterator2, typename T>
      static TFEL_MATH_INLINE T exe(InputIterator1, InputIterator2, T init) {
        return init;
      }

    };  // end of struct dotProduct<0u>

    template <unsigned int N, unsigned int M>
    struct DotProduct {
      template <typename T, typename T1, typename T2>
      static TFEL_MATH_INLINE const T exe(const T1& a, const T2& b) {
        return a(M) * b(M) +
               DotProduct<N - 1, M + 1>::template exe<T, T1, T2>(a, b);
      }
    };

    template <unsigned int M>
    struct DotProduct<0u, M> {
      template <typename T, typename T1, typename T2>
      static TFEL_MATH_INLINE const T exe(const T1&, const T2&) {
        return T(0);
      }
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_DOTPRODUCT_HXX */
