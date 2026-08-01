/*!
 * \file   include/TFEL/Math/Quantity/Unit.hxx
 * \brief  This file declares the classes Unit, GenerateUnit, AddUnit_,
 * SubUnit_, PowUnit_ and RootUnit_. \author Thomas Helfer \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_UNIT_HXX
#define LIB_TFEL_MATH_UNIT_HXX

#include <algorithm>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Forward/Unit.hxx"

namespace tfel::math {

  [[nodiscard]] constexpr UnitExponent add(const UnitExponent &e1,
                                           const UnitExponent &e2) noexcept {
    const auto numerator = e1.numerator * static_cast<int>(e2.denominator) +
                           e2.numerator * static_cast<int>(e1.denominator);
    const auto denominator = e1.denominator * e2.denominator;
    const auto g = static_cast<unsigned int>(std::gcd(numerator, denominator));
    return {numerator / static_cast<int>(g), denominator / (g)};
  }

  [[nodiscard]] constexpr UnitExponent subtract(
      const UnitExponent &e1, const UnitExponent &e2) noexcept {
    const auto numerator = e1.numerator * static_cast<int>(e2.denominator) -
                           e2.numerator * static_cast<int>(e1.denominator);
    const auto denominator = e1.denominator * e2.denominator;
    const auto g = static_cast<unsigned int>(std::gcd(numerator, denominator));
    return {numerator / static_cast<int>(g), denominator / (g)};
  }

  [[nodiscard]] constexpr UnitExponent multiply(
      const UnitExponent &e1, const UnitExponent &e2) noexcept {
    const auto numerator = e1.numerator * e2.numerator;
    const auto denominator = e1.denominator * e2.denominator;
    const auto g = static_cast<unsigned int>(std::gcd(numerator, denominator));
    return {numerator / static_cast<int>(g), denominator / g};
  }

  [[nodiscard]] constexpr UnitExponents add(const UnitExponents &e1,
                                            const UnitExponents &e2) noexcept {
    return {.exponents = {add(e1.exponents[0], e2.exponents[0]),
                          add(e1.exponents[1], e2.exponents[1]),
                          add(e1.exponents[2], e2.exponents[2]),
                          add(e1.exponents[3], e2.exponents[3]),
                          add(e1.exponents[4], e2.exponents[4]),
                          add(e1.exponents[5], e2.exponents[5]),
                          add(e1.exponents[6], e2.exponents[6])}};
  }

  [[nodiscard]] constexpr UnitExponents subtract(
      const UnitExponents &e1, const UnitExponents &e2) noexcept {
    return {.exponents = {subtract(e1.exponents[0], e2.exponents[0]),
                          subtract(e1.exponents[1], e2.exponents[1]),
                          subtract(e1.exponents[2], e2.exponents[2]),
                          subtract(e1.exponents[3], e2.exponents[3]),
                          subtract(e1.exponents[4], e2.exponents[4]),
                          subtract(e1.exponents[5], e2.exponents[5]),
                          subtract(e1.exponents[6], e2.exponents[6])}};
  }

  [[nodiscard]] constexpr UnitExponents multiply(
      const UnitExponents &e1, const UnitExponents &e2) noexcept {
    return {.exponents = {multiply(e1.exponents[0], e2.exponents[0]),
                          multiply(e1.exponents[1], e2.exponents[1]),
                          multiply(e1.exponents[2], e2.exponents[2]),
                          multiply(e1.exponents[3], e2.exponents[3]),
                          multiply(e1.exponents[4], e2.exponents[4]),
                          multiply(e1.exponents[5], e2.exponents[5]),
                          multiply(e1.exponents[6], e2.exponents[6])}};
  }

  [[nodiscard]] constexpr bool areAllDenominatorsOne(
      const UnitExponents &e) noexcept {
    return std::all_of(
        e.exponents.begin(), e.exponents.end(),
        [](const UnitExponent &e1) { return e1.denominator == 1; });
  }  // end of isValid

  namespace internals {

    template <UnitConcept UnitType1, UnitConcept UnitType2>
    struct AddUnit {
      using type = typename unit::UnitRebind<add(exponents<UnitType1>,
                                                 exponents<UnitType2>)>::type;
    };

    template <UnitConcept UnitType1, UnitConcept UnitType2>
    struct SubtractUnit {
      using type = typename unit::UnitRebind<subtract(
          exponents<UnitType1>, exponents<UnitType2>)>::type;
    };

    template <int N, unsigned int D, UnitConcept UnitType1>
    struct PowerUnit {
      using type = typename unit::UnitRebind<multiply(
          exponents<UnitType1>,
          makeUnitExponents<N, N, N, N, N, N, N, D, D, D, D, D, D, D>())>::type;
    };

  }  // namespace internals

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_UNIT_HXX */
