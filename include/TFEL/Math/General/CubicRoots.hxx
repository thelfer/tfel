/*!
 * \file   include/TFEL/Math/General/CubicRoots.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   03 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CUBICROOTS_HXX
#define LIB_TFEL_MATH_CUBICROOTS_HXX

#include <cmath>
#include <limits>
#include <algorithm>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel::math {

  /*!
   * struct gathering tools function to compute the roots of a cubic
   * polynomial
   */
  struct CubicRoots {
#if !((defined _WIN32) || (defined _WIN64) || (defined __CYGWIN__))
#if defined _BSD_SOURCE || defined _SVID_SOURCE || defined _ISOC99_SOURCE || \
    ((defined _XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600)) ||                   \
    ((defined _POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L))
    /*!
     * \return the cubic root of a real number of type float
     * \param[in] x : value
     */
    static TFEL_MATH_INLINE float cbrt(const float x) { return ::cbrtf(x); }
    /*!
     * \return the cubic root of a real number of type double
     * \param[in] x : value
     */
    static TFEL_MATH_INLINE double cbrt(const double x) { return ::cbrt(x); }
    /*!
     * \return the cubic root of a real number of type long double
     * \param[in] x : value
     */
    static TFEL_MATH_INLINE long double cbrt(const long double x) {
      return ::cbrtl(x);
    }
#endif /* LIB_TFEL_MATH_CUBICROOTS_HXX */
#endif /* LIB_TFEL_MATH_CUBICROOTS_HXX */
    /*!
     * \return the cubic root of a real number
     * \param[in] x : value
     */
    template <typename T>
    static TFEL_MATH_INLINE T cbrt(const T x) {
      constexpr const auto one_third = T(1) / T(3);
      if (x < 0) {
        return -std::pow(-x, one_third);
      }
      return std::pow(x, one_third);
    }

    /*!
     * find the real roots of a cubic polynomial by Cardan's method
     * \return the number of real roots.
     * a3 X^{3}+a2 X^{2}+a1 X^{1}+a0 = 0
     * \param[out] x1 : first  root
     * \param[out] x2 : second root
     * \param[out] x3 : third  root
     * \param[in]  a3 : coefficient of the polynom
     * \param[in]  a2 : coefficient of the polynom
     * \param[in]  a1 : coefficient of the polynom
     * \param[in]  a0 : coefficient of the polynom
     * \note due to numerical errors, this function may find only one real
     * root when three are expected (for example, when searching the
     * eigen values of a symmetric tensor). If only one real root is
     * found:
     * - x1 contains this real root
     * - x2 and x3 contain the real part of the two other roots
     */
    template <typename T>
    static TFEL_MATH_INLINE2 std::enable_if_t<
        tfel::typetraits::IsReal<T>::cond &&
            tfel::typetraits::IsFundamentalNumericType<T>::cond,
        unsigned short>
    find_roots(
        T& x1, T& x2, T& x3, const T a3, const T a2, const T a1, const T a0) {
      constexpr const auto C_1_2 = T{1} / T{2};
      constexpr const auto C_1_3 = T{1} / T{3};
      constexpr const auto C_2_3 = T{2} * C_1_3;
      constexpr const auto C_m1_27 = T{-1} / T{27};
      constexpr const auto C_2_27 = T{2} / T{27};
      constexpr const auto C_27_2 = T{27} / T{2};
      constexpr const auto C_3SQRT3_2 = Cste<T>::sqrt3 * T(3) / T(2);
      constexpr const auto C_SQRT3_3 = Cste<T>::sqrt3 * C_1_3;
      const T prec = 100 * std::numeric_limits<T>::min();
      if (tfel::math::abs(a3) <= prec) {
        return 0u;
      }
      // normalisation des coefficients pour se ramener à une équation de la
      // forme X^3+A*X+B*X+C=0
      T tmp = T(1) / a3;
      T tmp2 = a2 * tmp;
      T tmp3 = C_1_3 * tmp2;
      // mise sous forme réduite (élimination du terme quadratique)
      // X^{3}+pX+q=0
      const T p = tmp * (a1 - tmp3 * a2);
      const T q = tmp * (a0 - tmp3 * a1 + C_2_27 * tmp2 * tmp2 * a2);
      // traitement des cas particuliers
      if (tfel::math::abs(p) < prec) {
        const T cbrt_q = CubicRoots::cbrt(q);
        if (tfel::math::abs(cbrt_q) < prec) {
          x1 = x2 = x3 = -tmp3;
          return 3u;
        } else if (q > 0) {
          x1 = -tmp3 + cbrt_q;
          x2 = -tmp3 + C_1_2 * cbrt_q;
          x3 = -tmp3 + C_1_2 * cbrt_q;
          return 1u;
        } else {
          x3 = -tmp3 + cbrt_q;
          x1 = -tmp3 + C_1_2 * cbrt_q;
          x2 = -tmp3 + C_1_2 * cbrt_q;
          return 1u;
        }
      }
      if (tfel::math::abs(q) < prec) {
        if (p > 0) {
          x1 = -tmp3;
          x2 = -tmp3;
          x3 = -tmp3;
          return 1u;
        } else {
          const T sqrt_p = std::sqrt(-p);
          x2 = -tmp3 + sqrt_p;
          x1 = -tmp3;
          x3 = -tmp3 - sqrt_p;
          return 3u;
        }
      }
      const T delta = -T(4) * p * p * p - T(27) * q * q;
      if (delta < 0) {
        const T tmp4 = std::sqrt(C_m1_27 * delta);
        const T u = CubicRoots::cbrt((-q + tmp4) / (T(2)));
        const T v = CubicRoots::cbrt((-q - tmp4) / (T(2)));
        const T upv = u + v;
        x1 = upv - tmp3;
        x2 = -upv / 2 - tmp3;
        x3 = x2;
        if (tfel::math::abs(u - v) <
            100 * tfel::math::abs(upv) * std::numeric_limits<T>::epsilon()) {
          return 3u;
        } else {
          return 1u;
        }
      }
      if (tfel::math::abs(delta) < prec) {
        if (tfel::math::abs(p) > prec) {
          const T tmp5 = 3 * q / p;
          x1 = tmp5 - tmp3;
          x2 = -tmp5 / 2 - tmp3;
          x3 = x2;
          return 3u;
        } else {
          x1 = -tmp3;
          x2 = -tmp3;
          x3 = -tmp3;
          return 3u;
        }
      }
      tmp = -C_27_2 * q;
      tmp2 = C_3SQRT3_2 * std::sqrt(delta);
      const T rho = std::sqrt(tmp * tmp + tmp2 * tmp2);
      const T theta = std::atan2(tmp2, tmp);
      tmp = CubicRoots::cbrt(rho);
      const T ux = tmp * std::cos(theta * C_1_3);
      const T uy = tmp * std::sin(theta * C_1_3);
      x1 = -tmp3 + C_2_3 * ux;
      x2 = -tmp3 - C_1_3 * ux - C_SQRT3_3 * uy;
      x3 = -tmp3 - C_1_3 * ux + C_SQRT3_3 * uy;
      return 3u;
    }

    /*!
     * \return the number of real roots.
     * \param[out] x1 : first  root
     * \param[out] x2 : second root
     * \param[out] x3 : third  root
     * \param[in]  a3 : coefficient of the polynom
     * \param[in]  a2 : coefficient of the polynom
     * \param[in]  a1 : coefficient of the polynom
     * \param[in]  a0 : coefficient of the polynom
     * \param[in] b   : if true, use a newton raphson algorithm to
     *                  refine the roots found
     */
    template <typename T>
    static TFEL_MATH_INLINE2 std::enable_if_t<
        tfel::typetraits::IsReal<T>::cond &&
            tfel::typetraits::IsFundamentalNumericType<T>::cond,
        unsigned short>
    exe(T& x1,
        T& x2,
        T& x3,
        const T a3,
        const T a2,
        const T a1,
        const T a0,
        const bool b = false) {
      const auto nb = CubicRoots::find_roots(x1, x2, x3, a3, a2, a1, a0);
      if ((nb > 0) && (b)) {
        CubicRoots::improve(x1, a3, a2, a1, a0);
        if (nb == 3u) {
          CubicRoots::improve(x2, a3, a2, a1, a0);
          CubicRoots::improve(x3, a3, a2, a1, a0);
        }
      }
      return nb;
    }  // end of exe

   protected:
    template <typename T>
    static void improve(T& vp, const T a3, const T a2, const T a1, const T a0) {
      using integer = unsigned short;
      auto f = [&a3, &a2, &a1, &a0](const T x) {
        return ((a3 * x + a2) * x + a1) * x + a0;
      };
      auto df = [&a3, &a2, &a1](const T x) {
        return (3 * a3 * x + 2 * a2) * x + a1;
      };
      constexpr const auto emin = std::numeric_limits<T>::min();
      constexpr const auto eps = std::numeric_limits<T>::epsilon();
      const auto prec = 10 * std::max(emin, tfel::math::abs(vp) * eps);
      auto x = vp;
      auto dfv = df(x);
      constexpr integer iter_max = 50;
      if (tfel::math::abs(dfv) < 100 * emin) {
        return;
      }
      auto x1 = x - f(x) / dfv;
      auto iter = integer(0);
      while ((tfel::math::abs(x1 - x) > prec) && (iter < iter_max)) {
        x = x1;
        dfv = df(x);
        if (tfel::math::abs(dfv) < 100 * emin) {
          return;
        }
        x1 = x - f(x) / dfv;
        ++iter;
      }
      if (tfel::math::abs(f(x)) < tfel::math::abs(f(vp))) {
        vp = x;
      }
    }
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_CUBICROOTS_HXX */
