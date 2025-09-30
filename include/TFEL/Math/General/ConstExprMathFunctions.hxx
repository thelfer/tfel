/*!
 * \file  ConstExprMathFunctions.hxx
 * \brief
 * \author Thomas Helfer
 * \date   18 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONSTEXPRMATHFUNCTIONS_HXX
#define LIB_TFEL_MATH_CONSTEXPRMATHFUNCTIONS_HXX

#include <limits>

namespace tfel::math::constexpr_fct {

  /*!
   * \brief compute the absolute value of a number using a TFEL_HOST_DEVICE
   * constexpr function.
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr T abs(const T v) {
    return (v < 0) ? -v : v;
  }

  template <typename T>
  TFEL_HOST_DEVICE constexpr T sqrt_helper(const T x, const T g) {
    return abs(g - x / g) < 2 * std::numeric_limits<T>::epsilon()
               ? g
               : sqrt_helper(x, (g + x / g) / T{2});
  }

  /*!
   * \brief compute the square root of a number using the Heron
   * algorithm using a TFEL_HOST_DEVICE constexpr function.
   *
   * This function is meant to compute some usefull constant such as
   * sqrt(2) and sqrt(3) at compile-time.
   * \tparam T : numerical type
   * \pre T must be a floatting point type
   * \pre std::numeric_limits<T>::epsilon must be defined
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr T sqrt(const T v) {
    return sqrt_helper(v, T{1});
  }

}  // namespace tfel::math::constexpr_fct

#endif /* LIB_TFEL_MATH_CONSTEXPRMATHFUNCTIONS_HXX */
