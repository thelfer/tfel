/*!
 * \file   TFEL/Math/Forward/Unit.hxx
 * \brief
 * \author Thomas Helfer
 * \date   01/08/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_UNIT_HXX
#define LIB_TFEL_MATH_FORWARD_UNIT_HXX

#include <array>
#include <numeric>
#include <concepts>
#include <algorithm>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/Forward/General.hxx"

namespace tfel::math::unit {

  struct UnitExponent {
    int numerator;
    unsigned int denominator;
  };

  [[nodiscard]] constexpr bool isValid(const UnitExponent& e) noexcept {
    return e.denominator != 0;
  }

  [[nodiscard]] constexpr bool isIrreductible(const UnitExponent& e) noexcept {
    return std::gcd(e.numerator, e.denominator) == 1;
  }

  [[nodiscard]] constexpr bool operator==(const UnitExponent& e1,
                                          const UnitExponent& e2) noexcept {
    return ((e1.numerator == e2.numerator) &&
            (e1.denominator == e2.denominator));
  }  // end of operator==

  struct UnitExponents {
    std::array<UnitExponent, 7> exponents;
  };

  [[nodiscard]] constexpr bool isValid(const UnitExponents& e) noexcept {
    return (std::all_of(
               e.exponents.begin(), e.exponents.end(),
               [](const UnitExponent& e1) { return isValid(e1); })) &&  //
           (std::all_of(
               e.exponents.begin(), e.exponents.end(),
               [](const UnitExponent& e1) { return isIrreductible(e1); }));
  }  // end of isValid

  [[nodiscard]] constexpr bool operator==(const UnitExponents& e1,
                                          const UnitExponents& e2) noexcept {
    return (e1.exponents[0] == e2.exponents[0]) &&
           (e1.exponents[1] == e2.exponents[1]) &&
           (e1.exponents[2] == e2.exponents[2]) &&
           (e1.exponents[3] == e2.exponents[3]) &&
           (e1.exponents[4] == e2.exponents[4]) &&
           (e1.exponents[5] == e2.exponents[5]) &&
           (e1.exponents[6] == e2.exponents[6]);
  }  // end of operator==

  template <int N1 = 0,
            int N2 = 0,
            int N3 = 0,
            int N4 = 0,
            int N5 = 0,
            int N6 = 0,
            int N7 = 0,  //
            unsigned int D1 = 1,
            unsigned int D2 = 1,
            unsigned int D3 = 1,
            unsigned int D4 = 1,
            unsigned int D5 = 1,
            unsigned int D6 = 1,
            unsigned int D7 = 1>
  [[nodiscard]] constexpr UnitExponents makeUnitExponents() noexcept {
    return {.exponents = {UnitExponent{.numerator = N1, .denominator = D1},
                          UnitExponent{.numerator = N2, .denominator = D2},
                          UnitExponent{.numerator = N3, .denominator = D3},
                          UnitExponent{.numerator = N4, .denominator = D4},
                          UnitExponent{.numerator = N5, .denominator = D5},
                          UnitExponent{.numerator = N6, .denominator = D6},
                          UnitExponent{.numerator = N7, .denominator = D7}}};
  }  // end of makeUnitExponents

  /*!
   * \brief brief base class inherited by all unit classes
   *
   * \tparam e:  exponents defining the unit
   */
  template <UnitExponents e>
  requires(isValid(e))  //
      struct UnitBase {
  };

  template <int N1,
            int N2,
            int N3,
            int N4,
            int N5,
            int N6,
            int N7,
            unsigned int D1 = 1,
            unsigned int D2 = 1,
            unsigned int D3 = 1,
            unsigned int D4 = 1,
            unsigned int D5 = 1,
            unsigned int D6 = 1,
            unsigned int D7 = 1>
  struct Unit : UnitBase<makeUnitExponents<N1,
                                           N2,
                                           N3,
                                           N4,
                                           N5,
                                           N6,
                                           N7,  //
                                           D1,
                                           D2,
                                           D3,
                                           D4,
                                           D5,
                                           D6,
                                           D7>()> {};

  template <int N1 = 0,
            int N2 = 0,
            int N3 = 0,
            int N4 = 0,
            int N5 = 0,
            int N6 = 0,
            int N7 = 0>
  struct StandardUnit : Unit<N1, N2, N3, N4, N5, N6, N7> {};

  template <UnitExponents e>
  requires(isValid(e))  //
      [[nodiscard]] constexpr UnitExponents
      get_unit_exponents(const UnitBase<e>&) noexcept {
    return e;
  }

  template <typename T>
  concept UnitConcept = requires(const T& t) {
    { get_unit_exponents(t) } -> std::same_as<UnitExponents>;
  };

  template <UnitConcept UnitType>
  [[nodiscard]] constexpr UnitExponents get_unit_exponents() noexcept {
    return get_unit_exponents(UnitType{});
  }

  template <UnitConcept UnitType>
  constexpr inline UnitExponents exponents = get_unit_exponents<UnitType>();

  template <typename UnitType1, typename UnitType2>
  concept areUnitsEqual = (exponents<UnitType1> == exponents<UnitType2>);

  /*!
   * \brief Declares the null unit
   * \see StandardUnit
   */
  struct NoUnit : StandardUnit<> {};
  /*!
   * \brief Declares the Mass unit
   * \see StandardUnit
   */
  struct Mass : StandardUnit<1, 0, 0, 0, 0, 0, 0> {};
  /*!
   * \brief Declares the Length unit
   * \see StandardUnit
   */
  struct Length : StandardUnit<0, 1, 0, 0, 0, 0, 0> {};
  /*!
   * \brief Declares the Time unit
   * \see StandardUnit
   */
  struct Time : StandardUnit<0, 0, 1, 0, 0, 0, 0> {};
  /*!
   * \brief Declares the Ampere unit
   * \see StandardUnit
   */
  struct Ampere : StandardUnit<0, 0, 0, 1, 0, 0, 0> {};
  /*!
   * \brief Declares the Temperature unit
   * \see StandardUnit
   */
  struct Temperature : StandardUnit<0, 0, 0, 0, 1, 0, 0> {};
  //! a simple alias
  using Kelvin = Temperature;
  /*!
   * \brief Declares the Candela unit
   * \see StandardUnit
   */
  struct Candela : StandardUnit<0, 0, 0, 0, 0, 1, 0> {};
  /*!
   * \brief Declares the Mole unit
   * \see StandardUnit
   */
  struct Mole : StandardUnit<0, 0, 0, 0, 0, 0, 1> {};

  // Additional units

  /*!
   * \brief Declares the InvLength unit
   * \see StandardUnit
   */
  struct InvLength : StandardUnit<0, -1, 0, 0, 0, 0, 0> {};  // m-1

  /*!
   * \brief Declares the InvTemperature unit
   * \see StandardUnit
   */
  struct InvTemperature : StandardUnit<0, 0, 0, 0, -1, 0, 0> {};

  /*!
   * \brief Declares the Frequency unit
   * \see StandardUnit
   */
  struct Frequency : StandardUnit<0, 0, -1, 0, 0, 0, 0> {};  // s-1

  /*!
   * \brief Declares the Speed unit
   * \see StandardUnit
   */
  struct Speed : StandardUnit<0, 1, -1, 0, 0, 0, 0> {};  // m.s-1

  /*!
   * \brief Declares the Acceleration unit
   * \see StandardUnit
   */
  struct Acceleration : StandardUnit<0, 1, -2, 0, 0, 0, 0> {};  // m.s-2

  /*!
   * \brief Declares the Momentum unit
   * \see StandardUnit
   */
  struct Momentum : StandardUnit<1, 1, -1, 0, 0, 0, 0> {};  // kg.m.s-1

  /*!
   * \brief Declares the Momentum unit
   * \see StandardUnit
   */
  struct Force : StandardUnit<1, 1, -2, 0, 0, 0, 0> {};  // kg.m.s-2

  /*!
   * \brief Declares the Newton unit
   * \see StandardUnit
   */
  using Newton = Force;
  /*!
   * \brief Declares the Stress unit
   * \see StandardUnit
   */
  struct Stress : StandardUnit<1, -1, -2, 0, 0, 0, 0> {};  // kg.m-1.s-2

  /*!
   * \brief Declares the StressRate unit
   * \see StandardUnit
   */
  struct StressRate : StandardUnit<1, -1, -3, 0, 0, 0, 0> {};  // kg.m-1.s-3

  /*!
   * \brief Declares the Pressure unit
   * \see StandardUnit
   */
  using Pressure = Stress;

  /*!
   * \brief Declares the Energy unit
   * \see StandardUnit
   */
  struct Energy : StandardUnit<1, 2, -2, 0, 0, 0, 0> {};  // kg.m2.s-2

  /*!
   * \brief Declares the EnergyDensity unit
   * \see StandardUnit
   */
  using EnergyDensity = Stress;
  /*!
   * \brief Declares the Density unit
   * \see StandardUnit
   */
  struct Density : StandardUnit<1, -3, 0, 0, 0, 0, 0> {};  // k.m-3
  //! \brief Declares the TemperatureGradient unit
  struct TemperatureGradient : StandardUnit<0, -1, 0, 0, 1, 0, 0> {
  };  // K.m^{-1}
  //! \brief Declares the ThermalConductivity unit
  struct ThermalConductivity : StandardUnit<1, 1, -3, 0, -1, 0, 0> {
  };  // kg.m.s-3.K-1
  //! \brief Declares the HeatFluxDensity unit
  struct HeatFluxDensity : StandardUnit<1, 0, -3, 0, 0, 0, 0> {};  // kg.s-3

  [[nodiscard]] constexpr bool areAllDenominatorsOne(
      const UnitExponents& e) noexcept {
    return std::all_of(
        e.exponents.begin(), e.exponents.end(),
        [](const UnitExponent& e1) { return e1.denominator == 1; });
  }  // end of isValid

  /*!
   * \brief `UnitRebind` is a way to simply rebind some unit types from the
   * unit exponents.
   */
  template <UnitExponents e>
  requires(isValid(e))  //
      struct UnitRebind {
    using type = std::conditional_t<areAllDenominatorsOne(e),
                                    StandardUnit<e.exponents[0].numerator,
                                                 e.exponents[1].numerator,
                                                 e.exponents[2].numerator,
                                                 e.exponents[3].numerator,
                                                 e.exponents[4].numerator,
                                                 e.exponents[5].numerator,
                                                 e.exponents[6].numerator>,
                                    Unit<e.exponents[0].numerator,
                                         e.exponents[1].numerator,
                                         e.exponents[2].numerator,
                                         e.exponents[3].numerator,
                                         e.exponents[4].numerator,
                                         e.exponents[5].numerator,
                                         e.exponents[6].numerator,  //
                                         e.exponents[0].denominator,
                                         e.exponents[1].denominator,
                                         e.exponents[2].denominator,
                                         e.exponents[3].denominator,
                                         e.exponents[4].denominator,
                                         e.exponents[5].denominator,
                                         e.exponents[6].denominator>>;
  };

  template <>
  struct UnitRebind<exponents<NoUnit>> {
    using type = NoUnit;
  };
  template <>
  struct UnitRebind<exponents<Length>> {
    using type = Length;
  };
  template <>
  struct UnitRebind<exponents<Mass>> {
    using type = Mass;
  };
  template <>
  struct UnitRebind<exponents<Time>> {
    using type = Time;
  };
  template <>
  struct UnitRebind<exponents<Ampere>> {
    using type = Ampere;
  };
  template <>
  struct UnitRebind<exponents<Temperature>> {
    using type = Temperature;
  };
  template <>
  struct UnitRebind<exponents<Candela>> {
    using type = Candela;
  };
  template <>
  struct UnitRebind<exponents<Mole>> {
    using type = Mole;
  };
  template <>
  struct UnitRebind<exponents<InvLength>> {
    using type = InvLength;
  };
  template <>
  struct UnitRebind<exponents<InvTemperature>> {
    using type = InvTemperature;
  };
  template <>
  struct UnitRebind<exponents<Frequency>> {
    using type = Frequency;
  };
  template <>
  struct UnitRebind<exponents<Speed>> {
    using type = Speed;
  };
  template <>
  struct UnitRebind<exponents<Acceleration>> {
    using type = Acceleration;
  };
  template <>
  struct UnitRebind<exponents<Momentum>> {
    using type = Momentum;
  };
  template <>
  struct UnitRebind<exponents<Force>> {
    using type = Force;
  };
  template <>
  struct UnitRebind<exponents<Stress>> {
    using type = Stress;
  };
  template <>
  struct UnitRebind<exponents<StressRate>> {
    using type = StressRate;
  };
  template <>
  struct UnitRebind<exponents<Energy>> {
    using type = Energy;
  };
  template <>
  struct UnitRebind<exponents<Density>> {
    using type = Density;
  };
  template <>
  struct UnitRebind<exponents<TemperatureGradient>> {
    using type = TemperatureGradient;
  };
  template <>
  struct UnitRebind<exponents<ThermalConductivity>> {
    using type = ThermalConductivity;
  };
  template <>
  struct UnitRebind<exponents<HeatFluxDensity>> {
    using type = HeatFluxDensity;
  };

}  // end of namespace tfel::math::unit

#endif /* LIB_TFEL_MATH_FORWARD_UNIT_HXX */
