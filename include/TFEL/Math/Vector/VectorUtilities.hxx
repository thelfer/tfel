/*!
 * \file   include/TFEL/Math/Vector/VectorUtilities.hxx
 * \brief  This file implements various helper functions used by tvector.
 * \author Thomas Helfer
 * \date   05 May 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_VECTOR_UTILITIES_HXX
#define LIB_TFEL_VECTOR_UTILITIES_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief the class VectorUtilities contains a set of functions to
     * be applied to every item of a vector.
     * Those functions are implemented recursively and inlined.
     * \tparam N : index treated
     */
    template <unsigned short N>
    struct VectorUtilities {
      /*!
       * \brief copy the element of a vector in another vector
       * \tparam T    : type of the copied vector
       * \tparam T2   : type of the destination vector
       * \param[in] a : source vector
       * \param[in] b : destination vector
       * \pre T value type must be assignable to T2 value type
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void copy(const T& a, T2& b) {
        b(N - 1) = a(N - 1);
        VectorUtilities<N - 1>::copy(a, b);
      }
      /*!
       * \brief add the element of a vector to the element of another
       * vector so that b(N-1)+=a(N-1)
       * \tparam T    : type of the modified vector
       * \tparam T2   : type of the added vector
       * \param[in] b : result vector
       * \param[in] a : vector added to b
       * \pre T2 value type must be assignable to T value type
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void PlusEqual(T& b, const T2& a) {
        b(N - 1) += a(N - 1);
        VectorUtilities<N - 1>::PlusEqual(b, a);
      }

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void MinusEqual(T& b, const T2& a) {
        b(N - 1) -= a(N - 1);
        VectorUtilities<N - 1>::MinusEqual(b, a);
      }

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void MultEqual(T& b, const T2& a) {
        b(N - 1) *= a(N - 1);
        VectorUtilities<N - 1>::MinusEqual(b, a);
      }

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void assign_to_scalar(T& a, const T2& b) {
        a(N - 1) = b;
        VectorUtilities<N - 1>::assign_to_scalar(a, b);
      }

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void scale(T& a, const T2& b) {
        a(N - 1) *= b;
        VectorUtilities<N - 1>::scale(a, b);
      }
    };
    /*!
     * \brief This partial specialisation for N=0 is used to end the
     * recursion
     */
    template <>
    struct VectorUtilities<0u> {
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void copy(const T&, T2&) {}

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void PlusEqual(T&, const T2&) {}

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void MinusEqual(T&, const T2&) {}

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void MultEqual(T&, const T2&) {}

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void assign_to_scalar(T&, const T2&) {}

      template <typename T, typename T2>
      static TFEL_MATH_INLINE void scale(T&, const T2&) {}
    };

    /*!
     * \brief This structure copy a vector to an array
     * \tparam N : size of the array to be copied
     */
    template <unsigned short N>
    struct vectorToTab {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[N - 1] = src(N - 1);
        vectorToTab<N - 1>::exe(src, dest);
      }
    };
    /*!
     * \brief partial specialisation for N=0 to end the recursion.
     */
    template <>
    struct vectorToTab<0u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(T&, T2&) {}
    };

    /*!
     * \brief partial specialisation for N=1.
     */
    template <>
    struct vectorToTab<1u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=2.
     */
    template <>
    struct vectorToTab<2u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=3.
     */
    template <>
    struct vectorToTab<3u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=4.
     */
    template <>
    struct vectorToTab<4u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[3] = src(3);
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=5.
     */
    template <>
    struct vectorToTab<5u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[4] = src(4);
        dest[3] = src(3);
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=6.
     */
    template <>
    struct vectorToTab<6u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[5] = src(5);
        dest[4] = src(4);
        dest[3] = src(3);
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=7.
     */
    template <>
    struct vectorToTab<7u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[6] = src(6);
        dest[5] = src(5);
        dest[4] = src(4);
        dest[3] = src(3);
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief Partial specialisation of the vectorToTab class for N=8.
     */
    template <>
    struct vectorToTab<8u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[7] = src(7);
        dest[6] = src(6);
        dest[5] = src(5);
        dest[4] = src(4);
        dest[3] = src(3);
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=9.
     */
    template <>
    struct vectorToTab<9u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[8] = src(8);
        dest[7] = src(7);
        dest[6] = src(6);
        dest[5] = src(5);
        dest[4] = src(4);
        dest[3] = src(3);
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };
    /*!
     * \brief partial specialisation for N=10.
     */
    template <>
    struct vectorToTab<10u> {
      /*!
       * copy a vector to its destination
       * \tparam T       : vector type of the source
       * \tparam T2      : array type of the destination
       * \param[in] src  : source
       * \param[in] dest : destination
       * \pre the destination vector must be greater than the source vector.
       * \pre the value type of T must be assignable to the value type T2.
       */
      template <typename T, typename T2>
      static TFEL_MATH_INLINE void exe(const T& src, T2& dest) {
        dest[9] = src(9);
        dest[8] = src(8);
        dest[7] = src(7);
        dest[6] = src(6);
        dest[5] = src(5);
        dest[4] = src(4);
        dest[3] = src(3);
        dest[2] = src(2);
        dest[1] = src(1);
        dest[0] = src(0);
      }
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_VECTOR_UTILITIES_HXX */
