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

#include <concepts>
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

  /*!
   * \brief structure describing an unit
   */
  template <typename N1,
            typename N2,
            typename N3,
            typename N4,
            typename N5,
            typename N6,
            typename N7,
            typename D1,
            typename D2,
            typename D3,
            typename D4,
            typename D5,
            typename D6,
            typename D7>
  struct Unit;

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
  struct GenerateUnit {
    //! \brief result of the metafunction
    using type = Unit<std::integral_constant<int, N1>,
                      std::integral_constant<int, N2>,
                      std::integral_constant<int, N3>,
                      std::integral_constant<int, N4>,
                      std::integral_constant<int, N5>,
                      std::integral_constant<int, N6>,
                      std::integral_constant<int, N7>,
                      std::integral_constant<unsigned int, D1>,
                      std::integral_constant<unsigned int, D2>,
                      std::integral_constant<unsigned int, D3>,
                      std::integral_constant<unsigned int, D4>,
                      std::integral_constant<unsigned int, D5>,
                      std::integral_constant<unsigned int, D6>,
                      std::integral_constant<unsigned int, D7>>;
  };

  namespace internal {

    template <typename T>
    struct UnitConceptImplementation : std::false_type {};

    template <int N1,
              int N2,
              int N3,
              int N4,
              int N5,
              int N6,
              int N7,
              unsigned int D1,
              unsigned int D2,
              unsigned int D3,
              unsigned int D4,
              unsigned int D5,
              unsigned int D6,
              unsigned int D7>
    struct UnitConceptImplementation<
        Unit<std::integral_constant<int, N1>,
             std::integral_constant<int, N2>,
             std::integral_constant<int, N3>,
             std::integral_constant<int, N4>,
             std::integral_constant<int, N5>,
             std::integral_constant<int, N6>,
             std::integral_constant<int, N7>,
             std::integral_constant<unsigned int, D1>,
             std::integral_constant<unsigned int, D2>,
             std::integral_constant<unsigned int, D3>,
             std::integral_constant<unsigned int, D4>,
             std::integral_constant<unsigned int, D5>,
             std::integral_constant<unsigned int, D6>,
             std::integral_constant<unsigned int, D7>>> : std::true_type {};

  }  // namespace internal

  template <typename T>
  concept UnitConcept = internal::UnitConceptImplementation<T>::value;

}  // end of namespace tfel::math

namespace tfel::math::unit {

  //! \brief a simple alias
  using NoUnit = Unit<std::integral_constant<int, 0>,
                      std::integral_constant<int, 0>,
                      std::integral_constant<int, 0>,
                      std::integral_constant<int, 0>,
                      std::integral_constant<int, 0>,
                      std::integral_constant<int, 0>,
                      std::integral_constant<int, 0>,
                      std::integral_constant<unsigned int, 1u>,
                      std::integral_constant<unsigned int, 1u>,
                      std::integral_constant<unsigned int, 1u>,
                      std::integral_constant<unsigned int, 1u>,
                      std::integral_constant<unsigned int, 1u>,
                      std::integral_constant<unsigned int, 1u>,
                      std::integral_constant<unsigned int, 1u>>;

  /*!
   * \brief Declares the Mass unit
   * \see GenerateUnit
   */
  using Mass = GenerateUnit<1, 0, 0, 0, 0, 0, 0>::type;
  /*!
   * \brief Declares the Length unit
   * \see GenerateUnit
   */
  using Length = GenerateUnit<0, 1, 0, 0, 0, 0, 0>::type;
  /*!
   * \brief Declares the Time unit
   * \see GenerateUnit
   */
  using Time = GenerateUnit<0, 0, 1, 0, 0, 0, 0>::type;
  /*!
   * \brief Declares the Ampere unit
   * \see GenerateUnit
   */
  using Ampere = GenerateUnit<0, 0, 0, 1, 0, 0, 0>::type;
  /*!
   * \brief Declares the Temperature unit
   * \see GenerateUnit
   */
  using Temperature = GenerateUnit<0, 0, 0, 0, 1, 0, 0>::type;
  /*!
   * \brief Declares the Kelvin unit
   * \see GenerateUnit
   */
  using Kelvin = GenerateUnit<0, 0, 0, 0, 1, 0, 0>::type;
  /*!
   * \brief Declares the Candela unit
   * \see GenerateUnit
   */
  using Candela = GenerateUnit<0, 0, 0, 0, 0, 1, 0>::type;
  /*!
   * \brief Declares the Mole unit
   * \see GenerateUnit
   */
  using Mole = GenerateUnit<0, 0, 0, 0, 0, 0, 1>::type;

  // Additional units

  /*!
   * \brief Declares the InvLength unit
   * \see GenerateUnit
   */
  using InvLength = GenerateUnit<0, -1, 0, 0, 0, 0, 0>::type;  // m-1

  /*!
   * \brief Declares the InvTemperature unit
   * \see GenerateUnit
   */
  using InvTemperature = GenerateUnit<0, 0, 0, 0, -1, 0, 0>::type;

  /*!
   * \brief Declares the Frequency unit
   * \see GenerateUnit
   */
  using Frequency = GenerateUnit<0, 0, -1, 0, 0, 0, 0>::type;  // s-1

  /*!
   * \brief Declares the Speed unit
   * \see GenerateUnit
   */
  using Speed = GenerateUnit<0, 1, -1, 0, 0, 0, 0>::type;  // m.s-1

  /*!
   * \brief Declares the Acceleration unit
   * \see GenerateUnit
   */
  using Acceleration = GenerateUnit<0, 1, -2, 0, 0, 0, 0>::type;  // m.s-2

  /*!
   * \brief Declares the Momentum unit
   * \see GenerateUnit
   */
  using Momentum = GenerateUnit<1, 1, -1, 0, 0, 0, 0>::type;  // kg.m.s-1

  /*!
   * \brief Declares the Momentum unit
   * \see GenerateUnit
   */
  using Force = GenerateUnit<1, 1, -2, 0, 0, 0, 0>::type;  // kg.m.s-2

  /*!
   * \brief Declares the Newton unit
   * \see GenerateUnit
   */
  using Newton = GenerateUnit<1, 1, -2, 0, 0, 0, 0>::type;  // kg.m.s-2

  /*!
   * \brief Declares the Stress unit
   * \see GenerateUnit
   */
  using Stress = GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type;  // kg.m-1.s-2

  /*!
   * \brief Declares the StressRate unit
   * \see GenerateUnit
   */
  using StressRate = GenerateUnit<1, -1, -3, 0, 0, 0, 0>::type;  // kg.m-1.s-3

  /*!
   * \brief Declares the Pressure unit
   * \see GenerateUnit
   */
  using Pressure = GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type;  // kg.m-1.s-2

  /*!
   * \brief Declares the Energy unit
   * \see GenerateUnit
   */
  using Energy = GenerateUnit<1, 2, -2, 0, 0, 0, 0>::type;  // kg.m2.s-2

  /*!
   * \brief Declares the EnergyDensity unit
   * \see GenerateUnit
   */
  using EnergyDensity =
      GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type;  // kg.m-1.s-2
  /*!
   * \brief Declares the Density unit
   * \see GenerateUnit
   */
  using Density = GenerateUnit<1, -3, 0, 0, 0, 0, 0>::type;  // k.m-3
  //! \brief Declares the TemperatureGradient unit
  using TemperatureGradient =
      GenerateUnit<0, -1, 0, 0, 1, 0, 0>::type;  // K.m^{-1}
  //! \brief Declares the ThermalConductivity unit
  using ThermalConductivity =
      GenerateUnit<1, 1, -3, 0, -1, 0, 0>::type;  // kg.m.s-3.K-1
  //! \brief Declares the HeatFluxDensity unit
  using HeatFluxDensity = GenerateUnit<1, 0, -3, 0, 0, 0, 0>::type;  // kg.s-3

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
      typename GenerateUnit<N1,
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
                            D7>::type,
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
       (std::same_as<typename QuantityTraits<T>::UnitType,
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
