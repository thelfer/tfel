/*!
 * \file   include/TFEL/Math/Quantity/qtLimits.hxx
 * \brief  This file declares the numeric_limits classes for quantities.
 * \author Thomas Helfer
 * \date   26 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_QTLIMITS_HXX
#define LIB_TFEL_QTLIMITS_HXX

#include <limits>

#include "TFEL/Math/qt.hxx"

namespace std {

  /*
   * Partial specialisation for quantities
   */
  template <typename unit, typename T>
  struct numeric_limits<tfel::math::qt<unit, T>> {
    static constexpr bool is_specialized = numeric_limits<T>::is_specialized;

    static constexpr int digits = numeric_limits<T>::digits;
    static constexpr int digits10 = numeric_limits<T>::digits10;
    static constexpr bool is_signed = numeric_limits<T>::is_signed;
    static constexpr bool is_integer = numeric_limits<T>::is_integer;
    static constexpr bool is_exact = numeric_limits<T>::is_exact;
    static constexpr int radix = numeric_limits<T>::radix;

    static constexpr int min_exponent = numeric_limits<T>::min_exponent;
    static constexpr int min_exponent10 = numeric_limits<T>::min_exponent10;
    static constexpr int max_exponent = numeric_limits<T>::max_exponent;
    static constexpr int max_exponent10 = numeric_limits<T>::max_exponent10;

    static constexpr bool has_infinity = numeric_limits<T>::has_infinity;
    static constexpr bool has_quiet_NaN = numeric_limits<T>::has_quiet_NaN;
    static constexpr bool has_signaling_NaN =
        numeric_limits<T>::has_signaling_NaN;
    static constexpr float_denorm_style has_denorm =
        numeric_limits<T>::has_denorm;
    static constexpr bool has_denorm_loss = numeric_limits<T>::has_denorm_loss;

    static constexpr bool is_iec559 = numeric_limits<T>::is_iec559;
    static constexpr bool is_bounded = numeric_limits<T>::is_bounded;
    static constexpr bool is_modulo = numeric_limits<T>::is_modulo;

    static constexpr bool traps = numeric_limits<T>::traps;
    static constexpr bool tinyness_before = numeric_limits<T>::tinyness_before;
    static constexpr float_round_style round_style =
        numeric_limits<T>::round_style;

    static tfel::math::qt<unit, T> min() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::min());
    }
    static tfel::math::qt<unit, T> max() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::max());
    }
    static tfel::math::qt<unit, T> epsilon() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::epsilon());
    }
    static tfel::math::qt<unit, T> round_error() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::round_error());
    }
    static tfel::math::qt<unit, T> infinity() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::infinity());
    }
    static tfel::math::qt<unit, T> quiet_NaN() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::quiet_NaN());
    }
    static tfel::math::qt<unit, T> signaling_NaN() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::signaling_NaN());
    }
    static tfel::math::qt<unit, T> denorm_min() noexcept {
      return tfel::math::qt<unit, T>(numeric_limits<T>::denorm_min());
    }
  };

}  // end of namespace std

#endif /* LIB_TFEL_QTLIMITS_HXX */
