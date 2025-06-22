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

#ifndef LIB_TFEL_MATH_QTLIMITS_HXX
#define LIB_TFEL_MATH_QTLIMITS_HXX

#include <limits>
#include "TFEL/Math/qt.hxx"

namespace std {

  /*!
   * \brief partial specialisation for quantities
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct numeric_limits<
      tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    static constexpr bool is_specialized =
        numeric_limits<ValueType>::is_specialized;

    static constexpr int digits = numeric_limits<ValueType>::digits;
    static constexpr int digits10 = numeric_limits<ValueType>::digits10;
    static constexpr bool is_signed = numeric_limits<ValueType>::is_signed;
    static constexpr bool is_integer = numeric_limits<ValueType>::is_integer;
    static constexpr bool is_exact = numeric_limits<ValueType>::is_exact;
    static constexpr int radix = numeric_limits<ValueType>::radix;

    static constexpr int min_exponent = numeric_limits<ValueType>::min_exponent;
    static constexpr int min_exponent10 =
        numeric_limits<ValueType>::min_exponent10;
    static constexpr int max_exponent = numeric_limits<ValueType>::max_exponent;
    static constexpr int max_exponent10 =
        numeric_limits<ValueType>::max_exponent10;

    static constexpr bool has_infinity =
        numeric_limits<ValueType>::has_infinity;
    static constexpr bool has_quiet_NaN =
        numeric_limits<ValueType>::has_quiet_NaN;
    static constexpr bool has_signaling_NaN =
        numeric_limits<ValueType>::has_signaling_NaN;
    static constexpr float_denorm_style has_denorm =
        numeric_limits<ValueType>::has_denorm;
    static constexpr bool has_denorm_loss =
        numeric_limits<ValueType>::has_denorm_loss;

    static constexpr bool is_iec559 = numeric_limits<ValueType>::is_iec559;
    static constexpr bool is_bounded = numeric_limits<ValueType>::is_bounded;
    static constexpr bool is_modulo = numeric_limits<ValueType>::is_modulo;

    static constexpr bool traps = numeric_limits<ValueType>::traps;
    static constexpr bool tinyness_before =
        numeric_limits<ValueType>::tinyness_before;
    static constexpr float_round_style round_style =
        numeric_limits<ValueType>::round_style;

    static constexpr tfel::math::qt<UnitType, ValueType> min() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::min());
    }
    static constexpr tfel::math::qt<UnitType, ValueType> max() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::max());
    }
    static constexpr tfel::math::qt<UnitType, ValueType> epsilon() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::epsilon());
    }
    static constexpr tfel::math::qt<UnitType, ValueType>
    round_error() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::round_error());
    }
    static constexpr tfel::math::qt<UnitType, ValueType> infinity() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::infinity());
    }
    static constexpr tfel::math::qt<UnitType, ValueType> quiet_NaN() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::quiet_NaN());
    }
    static constexpr tfel::math::qt<UnitType, ValueType>
    signaling_NaN() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::signaling_NaN());
    }
    static constexpr tfel::math::qt<UnitType, ValueType> denorm_min() noexcept {
      return tfel::math::qt<UnitType, ValueType>(
          numeric_limits<ValueType>::denorm_min());
    }
  };

}  // end of namespace std

#endif /* LIB_TFEL_MATH_QTLIMITS_HXX */
