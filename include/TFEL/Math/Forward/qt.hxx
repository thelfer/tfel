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
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/Math/Forward/General.hxx"
#include "TFEL/Math/Forward/Unit.hxx"

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

  /*!
   * \brief Partial specialisation for qt
   * \see   IsFundamentalNumericType
   */
  template <tfel::math::NoUnitQuantityConcept QuantityType>
  struct IsFundamentalNumericType<QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsFundamentalNumericType
   */
  template <tfel::math::NoUnitQuantityConcept QuantityType>
  struct IsFundamentalNumericType<const QuantityType> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

}  // end of namespace tfel::typetraits

namespace tfel::math {

  template <QuantityConcept T>
  using quantity_unit = typename QuantityTraits<T>::UnitType;

  template <QuantityConcept T>
  using qt_rebind = qt<quantity_unit<T>, base_type<T>>;

  //! \brief a simple alias
  template <FundamentalNumericTypeConcept ValueType,
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
  using quantity = qt<typename unit::UnitRebind<makeUnitExponents<N1,
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
                      base_type<ValueType>>;

  //! \brief a simple alias
  template <bool use_qt,
            FundamentalNumericTypeConcept ValueType,
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

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_QT_HXX */
