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

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"

namespace tfel::math::internals {

  /*!
   * \brief an helper structure which holds the value internally.
   * \param ValueType: the underlying numerical type.
   */
  template <typename ValueType, bool AllowImplicitConversion>
  struct QuantityValueOwnershipPolicy;
  /*!
   * \brief an helper structure which wraps a reference to an external value.
   * \param ValueType: the underlying numerical type.
   */
  template <typename ValueType, bool AllowImplicitConversion>
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
  typedef GenerateUnit<1, 0, 0, 0, 0, 0, 0>::type Mass;
  /*!
   * \brief Declares the Length unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 1, 0, 0, 0, 0, 0>::type Length;
  /*!
   * \brief Declares the Time unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, 1, 0, 0, 0, 0>::type Time;
  /*!
   * \brief Declares the Ampere unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, 0, 1, 0, 0, 0>::type Ampere;
  /*!
   * \brief Declares the Temperature unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, 0, 0, 1, 0, 0>::type Temperature;
  /*!
   * \brief Declares the Kelvin unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, 0, 0, 1, 0, 0>::type Kelvin;
  /*!
   * \brief Declares the Candela unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, 0, 0, 0, 1, 0>::type Candela;
  /*!
   * \brief Declares the Mole unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, 0, 0, 0, 0, 1>::type Mole;

  // Additional units

  /*!
   * \brief Declares the InvLength unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, -1, 0, 0, 0, 0, 0>::type InvLength;  // m-1

  /*!
   * \brief Declares the InvTemperature unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, 0, 0, -1, 0, 0>::type InvTemperature;

  /*!
   * \brief Declares the Frequency unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 0, -1, 0, 0, 0, 0>::type Frequency;  // s-1

  /*!
   * \brief Declares the Speed unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 1, -1, 0, 0, 0, 0>::type Speed;  // m.s-1

  /*!
   * \brief Declares the Acceleration unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<0, 1, -2, 0, 0, 0, 0>::type Acceleration;  // m.s-2

  /*!
   * \brief Declares the Momentum unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, 1, -1, 0, 0, 0, 0>::type Momentum;  // kg.m.s-1

  /*!
   * \brief Declares the Momentum unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, 1, -2, 0, 0, 0, 0>::type Force;  // kg.m.s-2

  /*!
   * \brief Declares the Newton unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, 1, -2, 0, 0, 0, 0>::type Newton;  // kg.m.s-2

  /*!
   * \brief Declares the Stress unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type Stress;  // kg.m-1.s-2

  /*!
   * \brief Declares the StressRate unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, -1, -3, 0, 0, 0, 0>::type StressRate;  // kg.m-1.s-3

  /*!
   * \brief Declares the Pressure unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type Pressure;  // kg.m-1.s-2

  /*!
   * \brief Declares the Energy unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, 2, -2, 0, 0, 0, 0>::type Energy;  // kg.m2.s-2

  /*!
   * \brief Declares the EnergyDensity unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type
      EnergyDensity;  // kg.m-1.s-2
  /*!
   * \brief Declares the Density unit
   * \see GenerateUnit
   */
  typedef GenerateUnit<1, -3, 0, 0, 0, 0, 0>::type Density;  // k.m-3

  /*!
   * \brief Declares the TemperatureGradient unit
   */
  using TemperatureGradient =
      GenerateUnit<0, -1, 0, 0, 1, 0, 0>::type;  // K.m^{-1}
  /*!
   * \brief Declares the ThermalConductivity
   * unit
   */
  typedef GenerateUnit<1, 1, -3, 0, -1, 0, 0>::type
      ThermalConductivity;  // kg.m.s-3.K-1

  /*!
   * \brief Declares the HeatFluxDensity unit
   */
  typedef GenerateUnit<1, 0, -3, 0, 0, 0, 0>::type HeatFluxDensity;  // kg.s-3

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
  template <UnitConcept UnitType, typename ValueType, typename OwnershipPolicy>
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
            typename QuantityValueType,
            typename QuantityOwnershipPolicy>
  struct QuantityTraits<
      Quantity<QuantityUnitType, QuantityValueType, QuantityOwnershipPolicy>> {
    //! \brief unit of the given type
    using UnitType = QuantityUnitType;
    //! \brief underlying numeric type
    using ValueType = QuantityValueType;
  };

  //! \brief a simple alias
  template <UnitConcept UnitType, typename ValueType = double>
  using qt = Quantity<UnitType,
                      ValueType,
                      tfel::math::internals::QuantityValueOwnershipPolicy<
                          ValueType,
                          std::is_same_v<UnitType, NoUnit>>>;
  //! \brief a simple alias
  template <UnitConcept UnitType, typename ValueType = double>
  using qt_ref =
      Quantity<UnitType,
               ValueType,
               tfel::math::internals::QuantityReferenceOwnershipPolicy<
                   ValueType,
                   std::is_same_v<UnitType, NoUnit>>>;
  //! \brief a simple alias
  template <UnitConcept UnitType, typename ValueType = double>
  using const_qt_ref =
      Quantity<UnitType,
               ValueType,
               tfel::math::internals::QuantityReferenceOwnershipPolicy<
                   const ValueType,
                   std::is_same_v<UnitType, NoUnit>>>;
  //! \brief an helper metafunction which transforms a quantity type into the a
  //! reference
  template <typename QuantityType>
  struct MakeQuantityReferenceType {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };
  //! \brief partial specialisation for quantities
  template <UnitConcept UnitType, typename ValueType>
  struct MakeQuantityReferenceType<qt<UnitType, ValueType>> {
    //! \brief result
    using type = qt_ref<UnitType, ValueType>;
  };  // end of MakeQuantityReferenceType<qt<UnitType,ValueType>>

  namespace internals {

    template <typename ValueType>
    struct MakeQuantityValueType {
      //! \brief result of the metafunction
      using type = ValueType;
    };  // end of struct MakeQuantityValueType

    template <typename ValueType>
    struct MakeQuantityValueType<qt<NoUnit, ValueType>> {
      //! \brief result of the metafunction
      using type = ValueType;
    };  // end of struct MakeQuantityValueType

  }  // end of namespace internals

  //! \brief a simple alias
  template <typename ValueType,
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

  namespace internals {

    template <typename T>
    struct IsQuantity : std::false_type {};

    template <UnitConcept UnitType,
              typename ValueType,
              typename OwnershipPolicy>
    struct IsQuantity<Quantity<UnitType, ValueType, OwnershipPolicy>>
        : std::true_type {};

  }  // end of namespace internals

  template <typename T>
  TFEL_HOST_DEVICE constexpr auto isQuantity() {
    return tfel::math::internals::IsQuantity<std::decay_t<T>>::value;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_QT_HXX */
