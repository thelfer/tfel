/*!
 * \file  include/TFEL/Math/Forward/qt.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 jan. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_QT_HXX
#define LIB_TFEL_MATH_FORWARD_QT_HXX

#include <array>
#include <numeric>
#include <concepts>
#include <algorithm>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/Forward/General.hxx"

namespace tfel::math::internals {

  /*!
   * \brief an helper structure which holds the value internally.
   * \param ValueType: the underlying numerical type.
   */
  template <StandardArithmeticTypeConcept ValueType,
            bool AllowImplicitConversion>
  struct QuantityValueOwnershipPolicy;
  /*!
   * \brief an helper structure which wraps a reference to an external value.
   * \param ValueType: the underlying numerical type.
   */
  template <StandardArithmeticTypeConcept ValueType,
            bool AllowImplicitConversion>
  struct QuantityReferenceOwnershipPolicy;

}  // end of namespace tfel::math::internals

namespace tfel::math {

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

  namespace internals {

    struct ExtractUnitBase {
     protected:
      static constexpr void test(...) noexcept {}
      template <UnitExponents exponents>
      static constexpr UnitBase<exponents> test(
          const UnitBase<exponents>&) noexcept {
        return {};
      }
    };

    template <typename T>
    struct ExtractUnit : ExtractUnitBase {
      using type = decltype(ExtractUnitBase::test(std::declval<T>()));
    };

    template <>
    struct ExtractUnit<tfel::meta::InvalidType> {
      using type = void;
    };

  }  // namespace internals

  template <typename T>
  concept UnitConcept =
      !std::is_void_v<typename internals::ExtractUnit<T>::type>;

  template <UnitExponents e>
  requires(isValid(e))  //
      [[nodiscard]] constexpr UnitExponents
      get_unit_exponents(const UnitBase<e>&) noexcept {
    return e;
  }

  template <UnitConcept UnitType>
  [[nodiscard]] constexpr UnitExponents get_unit_exponents() noexcept {
    return get_unit_exponents(UnitType{});
  }

  template <UnitConcept UnitType>
  constexpr inline UnitExponents exponents = get_unit_exponents<UnitType>();

  template <UnitConcept UnitType1, UnitConcept UnitType2>
  inline constexpr bool areUnitsEqual =
      std::same_as<typename internals::ExtractUnit<UnitType1>::type,
                   typename internals::ExtractUnit<UnitType2>::type>;

}  // end of namespace tfel::math

namespace tfel::math::unit {

  struct NoUnit : StandardUnit<> {};
  /*!
   * \brief Declares the Mass unit
   * \see StandardUnit
   */
  using Mass = StandardUnit<1, 0, 0, 0, 0, 0, 0>;
  /*!
   * \brief Declares the Length unit
   * \see StandardUnit
   */
  using Length = StandardUnit<0, 1, 0, 0, 0, 0, 0>;
  /*!
   * \brief Declares the Time unit
   * \see StandardUnit
   */
  using Time = StandardUnit<0, 0, 1, 0, 0, 0, 0>;
  /*!
   * \brief Declares the Ampere unit
   * \see StandardUnit
   */
  using Ampere = StandardUnit<0, 0, 0, 1, 0, 0, 0>;
  /*!
   * \brief Declares the Temperature unit
   * \see StandardUnit
   */
  using Temperature = StandardUnit<0, 0, 0, 0, 1, 0, 0>;
  /*!
   * \brief Declares the Kelvin unit
   * \see StandardUnit
   */
  using Kelvin = StandardUnit<0, 0, 0, 0, 1, 0, 0>;
  /*!
   * \brief Declares the Candela unit
   * \see StandardUnit
   */
  using Candela = StandardUnit<0, 0, 0, 0, 0, 1, 0>;
  /*!
   * \brief Declares the Mole unit
   * \see StandardUnit
   */
  using Mole = StandardUnit<0, 0, 0, 0, 0, 0, 1>;

  // Additional units

  /*!
   * \brief Declares the InvLength unit
   * \see StandardUnit
   */
  using InvLength = StandardUnit<0, -1, 0, 0, 0, 0, 0>;  // m-1

  /*!
   * \brief Declares the InvTemperature unit
   * \see StandardUnit
   */
  using InvTemperature = StandardUnit<0, 0, 0, 0, -1, 0, 0>;

  /*!
   * \brief Declares the Frequency unit
   * \see StandardUnit
   */
  using Frequency = StandardUnit<0, 0, -1, 0, 0, 0, 0>;  // s-1

  /*!
   * \brief Declares the Speed unit
   * \see StandardUnit
   */
  using Speed = StandardUnit<0, 1, -1, 0, 0, 0, 0>;  // m.s-1

  /*!
   * \brief Declares the Acceleration unit
   * \see StandardUnit
   */
  using Acceleration = StandardUnit<0, 1, -2, 0, 0, 0, 0>;  // m.s-2

  /*!
   * \brief Declares the Momentum unit
   * \see StandardUnit
   */
  using Momentum = StandardUnit<1, 1, -1, 0, 0, 0, 0>;  // kg.m.s-1

  /*!
   * \brief Declares the Momentum unit
   * \see StandardUnit
   */
  using Force = StandardUnit<1, 1, -2, 0, 0, 0, 0>;  // kg.m.s-2

  /*!
   * \brief Declares the Newton unit
   * \see StandardUnit
   */
  using Newton = StandardUnit<1, 1, -2, 0, 0, 0, 0>;  // kg.m.s-2

  /*!
   * \brief Declares the Stress unit
   * \see StandardUnit
   */
  using Stress = StandardUnit<1, -1, -2, 0, 0, 0, 0>;  // kg.m-1.s-2

  /*!
   * \brief Declares the StressRate unit
   * \see StandardUnit
   */
  using StressRate = StandardUnit<1, -1, -3, 0, 0, 0, 0>;  // kg.m-1.s-3

  /*!
   * \brief Declares the Pressure unit
   * \see StandardUnit
   */
  using Pressure = StandardUnit<1, -1, -2, 0, 0, 0, 0>;  // kg.m-1.s-2

  /*!
   * \brief Declares the Energy unit
   * \see StandardUnit
   */
  using Energy = StandardUnit<1, 2, -2, 0, 0, 0, 0>;  // kg.m2.s-2

  /*!
   * \brief Declares the EnergyDensity unit
   * \see StandardUnit
   */
  using EnergyDensity = StandardUnit<1, -1, -2, 0, 0, 0, 0>;  // kg.m-1.s-2
  /*!
   * \brief Declares the Density unit
   * \see StandardUnit
   */
  using Density = StandardUnit<1, -3, 0, 0, 0, 0, 0>;  // k.m-3
  //! \brief Declares the TemperatureGradient unit
  using TemperatureGradient = StandardUnit<0, -1, 0, 0, 1, 0, 0>;  // K.m^{-1}
  //! \brief Declares the ThermalConductivity unit
  using ThermalConductivity =
      StandardUnit<1, 1, -3, 0, -1, 0, 0>;  // kg.m.s-3.K-1
  //! \brief Declares the HeatFluxDensity unit
  using HeatFluxDensity = StandardUnit<1, 0, -3, 0, 0, 0, 0>;  // kg.s-3

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

}  // end of namespace tfel::math::unit

namespace tfel::math {

  /*
   * \class Quantity
   * \brief This class describes numbers with unit.
   * qt must be has efficient and simple to use that standard numerical types.
   * \param UnitType: the unit of the qt.
   * \param ValueType: the underlying numerical type.
   * \param OwnershipPolicy: policy which tells if the .
   * \pre ValueType must be a fundamental numerical type (float, double, long
   * double).
   * \see unit_samples.hxx, IsFundamentalNumericType
   * \author Thomas Helfer
   * \date   06 Jun 2006
   */
  template <UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType,
            typename OwnershipPolicy>
  struct [[nodiscard]] Quantity;

  /*!
   * \brief an helper structure to retrieve the underlying numeric type and
   * unit from a type when this is meaningful.
   */
  template <typename>
  struct QuantityTraits {
    //! \brief unit of the given type
    using UnitType = tfel::meta::InvalidType;
    //! \brief underlying numeric type
    using ValueType = tfel::meta::InvalidType;
  };

  //! \brief partial specialisation for quantities.
  template <UnitConcept QuantityUnitType,
            StandardArithmeticTypeConcept QuantityValueType,
            typename QuantityOwnershipPolicy>
  struct QuantityTraits<
      Quantity<QuantityUnitType, QuantityValueType, QuantityOwnershipPolicy>> {
    //! \brief unit of the given type
    using UnitType = QuantityUnitType;
    //! \brief underlying numeric type
    using ValueType = QuantityValueType;
  };

  //! \brief a simple alias
  template <UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType = double>
  using qt = Quantity<UnitType,
                      ValueType,
                      tfel::math::internals::QuantityValueOwnershipPolicy<
                          ValueType,
                          std::is_same_v<UnitType, unit::NoUnit>>>;
  //! \brief a simple alias
  template <UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType = double>
  using qt_ref =
      Quantity<UnitType,
               ValueType,
               tfel::math::internals::QuantityReferenceOwnershipPolicy<
                   ValueType,
                   std::is_same_v<UnitType, unit::NoUnit>>>;
  //! \brief a simple alias
  template <UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType = double>
  using const_qt_ref =
      Quantity<UnitType,
               ValueType,
               tfel::math::internals::QuantityReferenceOwnershipPolicy<
                   const ValueType,
                   std::is_same_v<UnitType, unit::NoUnit>>>;
  //! \brief an helper metafunction which transforms a quantity type into the a
  //! reference
  template <typename QuantityType>
  struct MakeQuantityReferenceType {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };
  //! \brief partial specialisation for quantities
  template <UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  struct MakeQuantityReferenceType<qt<UnitType, ValueType>> {
    //! \brief result
    using type = qt_ref<UnitType, ValueType>;
  };  // end of MakeQuantityReferenceType<qt<UnitType,ValueType>>

  namespace internals {

    template <StandardArithmeticTypeConcept ValueType>
    struct MakeQuantityValueType {
      //! \brief result of the metafunction
      using type = ValueType;
    };  // end of struct MakeQuantityValueType

    template <StandardArithmeticTypeConcept ValueType>
    struct MakeQuantityValueType<qt<unit::NoUnit, ValueType>> {
      //! \brief result of the metafunction
      using type = ValueType;
    };  // end of struct MakeQuantityValueType

  }  // end of namespace internals

  //! \brief a simple alias
  template <StandardArithmeticTypeConcept ValueType,
            int N1 = 0,
            int N2 = 0,
            int N3 = 0,
            int N4 = 0,
            int N5 = 0,
            int N6 = 0,
            int N7 = 0,
            unsigned int D1 = 1,
            unsigned int D2 = 1,
            unsigned int D3 = 1,
            unsigned int D4 = 1,
            unsigned int D5 = 1,
            unsigned int D6 = 1,
            unsigned int D7 = 1>
  using quantity = qt<
      typename unit::UnitRebind<makeUnitExponents<N1,
                                                  N2,
                                                  N3,
                                                  N4,
                                                  N5,
                                                  N6,
                                                  N7,
                                                  D1,
                                                  D2,
                                                  D3,
                                                  D4,
                                                  D5,
                                                  D6,
                                                  D7>()>::type,
      typename tfel::math::internals::MakeQuantityValueType<ValueType>::type>;

  //! \brief a simple alias
  template <bool use_qt,
            StandardArithmeticTypeConcept ValueType,
            int N1 = 0,
            int N2 = 0,
            int N3 = 0,
            int N4 = 0,
            int N5 = 0,
            int N6 = 0,
            int N7 = 0,
            unsigned int D1 = 1,
            unsigned int D2 = 1,
            unsigned int D3 = 1,
            unsigned int D4 = 1,
            unsigned int D5 = 1,
            unsigned int D6 = 1,
            unsigned int D7 = 1>
  using quantity_or_base_type = std::conditional_t<use_qt,
                                                   quantity<ValueType,
                                                            N1,
                                                            N2,
                                                            N3,
                                                            N4,
                                                            N5,
                                                            N6,
                                                            N7,
                                                            D1,
                                                            D2,
                                                            D3,
                                                            D4,
                                                            D5,
                                                            D6,
                                                            D7>,
                                                   ValueType>;
  //! \brief cast the value to the base type
  template <UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType,
            typename OwnershipPolicy>
  TFEL_HOST_DEVICE constexpr ValueType& base_type_cast(
      Quantity<UnitType, ValueType, OwnershipPolicy>&) noexcept;
  //! \brief cast the value to the base type
  template <UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType,
            typename OwnershipPolicy>
  TFEL_HOST_DEVICE constexpr const ValueType& base_type_cast(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& v) noexcept;
  //
  template <typename T>
  concept QuantityConcept =
      ((UnitConcept<typename QuantityTraits<T>::UnitType>)&&  //
       (StandardArithmeticTypeConcept<
           typename QuantityTraits<T>::ValueType>)&&  //
       (requires(T & v) {
         {
           base_type_cast(v)
           } -> std::same_as<typename QuantityTraits<T>::ValueType&>;
       }) &&  //
       (requires(const T& v) {
         {
           base_type_cast(v)
           } -> std::same_as<const typename QuantityTraits<T>::ValueType&>;
       }));

  template <typename T>
  concept NoUnitQuantityConcept =
      ((QuantityConcept<T>)&&  //
       (areUnitsEqual<typename QuantityTraits<T>::UnitType,
                      tfel::math::unit::NoUnit>));

  /*!
   * \brief a function testing if a type is a quantity
   *
   * \note this function is kept for backward compatility with C++-17 when
   * concepts were not yet available
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr bool isQuantity() {
    return QuantityConcept<std::decay_t<T>>;
  }

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief partial specialisation for quantities
  template <tfel::math::QuantityConcept QuantityType>
  struct BaseType<QuantityType> {
    //! \brief result of the metafunction
    using type = typename tfel::math::QuantityTraits<QuantityType>::ValueType;
  };

}  // end of namespace tfel::typetraits

namespace tfel::math {

  template <QuantityConcept T>
  using quantity_unit = typename QuantityTraits<T>::UnitType;

  template <QuantityConcept T>
  using qt_rebind = qt<quantity_unit<T>, base_type<T>>;

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_QT_HXX */
