/*!
 * \file   include/TFEL/Math/qt.hxx
 * \brief  This file declares the qt class.
 * \author Thomas Helfer
 * \date   06 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QT_HXX
#define LIB_TFEL_MATH_QT_HXX

#include <cmath>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/TypeList.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/Promote.hxx"
#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/UnaryResultType.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel::math::internals {

  template <StandardArithmeticTypeConcept ValueType,
            bool AllowImplicitConversion>
  struct QuantityValueOwnershipPolicy {
    //
    static_assert(std::is_same_v<ValueType, std::decay_t<ValueType>>,
                  "invalid value type");
    //! \brief default constructor
    constexpr QuantityValueOwnershipPolicy() noexcept : value() {}
    //! \brief copy constructor.
    constexpr QuantityValueOwnershipPolicy(
        const QuantityValueOwnershipPolicy&) noexcept = default;
    //! \brief move constructor.
    constexpr QuantityValueOwnershipPolicy(
        QuantityValueOwnershipPolicy&&) noexcept = default;
    // NOLINTBEGIN(google-explicit-constructor)
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T>
    TFEL_HOST_DEVICE constexpr explicit QuantityValueOwnershipPolicy(
        const T& src) noexcept                              //
        requires((std::is_constructible_v<ValueType, T>)&&  //
                 (std::is_convertible_v<ValueType, T>)&&    //
                 (!AllowImplicitConversion))
        : value(src) {}
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T>
    TFEL_HOST_DEVICE constexpr QuantityValueOwnershipPolicy(
        const T& src) noexcept                              //
        requires((std::is_constructible_v<ValueType, T>)&&  //
                 (std::is_convertible_v<ValueType, T>)&&    //
                 (AllowImplicitConversion))
        : value(src) {}
    // NOLINTEND(google-explicit-constructor)
    //! \brief return the value
    TFEL_HOST_DEVICE [[nodiscard]] constexpr ValueType& getValue() noexcept {
      return this->value;
    }
    //! \brief return the value
    TFEL_HOST_DEVICE [[nodiscard]] constexpr const ValueType& getValue()
        const noexcept {
      return this->value;
    }
    //! \brief assignement operator
    constexpr QuantityValueOwnershipPolicy& operator=(
        const QuantityValueOwnershipPolicy&) noexcept = default;
    //! \brief move assignement operator
    constexpr QuantityValueOwnershipPolicy& operator=(
        QuantityValueOwnershipPolicy&&) noexcept = default;
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T>
    TFEL_HOST_DEVICE constexpr QuantityValueOwnershipPolicy& operator=(
        const T& src) noexcept
        requires((std::is_constructible_v<ValueType, T>)&&  //
                 (std::is_convertible_v<ValueType, T>)&&    //
                 (AllowImplicitConversion)) {
      this->value = src;
      return *this;
    }
    // NOLINTBEGIN(google-explicit-constructor)
    //! \brief conversion operator
    template <typename T>
    TFEL_HOST_DEVICE constexpr operator T&() noexcept
        requires((std::is_same_v<ValueType, T>)&&  //
                 (AllowImplicitConversion)) {
      return this->value;
    }
    //! \brief conversion operator
    template <typename T>
    TFEL_HOST_DEVICE constexpr operator T() const noexcept
        requires((std::is_same_v<ValueType, T>)&&  //
                 (AllowImplicitConversion)) {
      return this->value;
    }
    // NOLINTEND(google-explicit-constructor)
    //! \brief destructor
    ~QuantityValueOwnershipPolicy() noexcept = default;

   private:
    //! \brief the value of the quantity
    ValueType value;
  };

  template <StandardArithmeticTypeConcept ValueType,
            bool AllowImplicitConversion>
  struct QuantityReferenceOwnershipPolicy {
    //! \brief default constructor
    constexpr QuantityReferenceOwnershipPolicy() noexcept = default;
    //! \brief copy constructor.
    constexpr QuantityReferenceOwnershipPolicy(
        const QuantityReferenceOwnershipPolicy&) noexcept = default;
    //! \brief move constructor.
    constexpr QuantityReferenceOwnershipPolicy(
        QuantityReferenceOwnershipPolicy&&) noexcept = default;
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    TFEL_HOST_DEVICE constexpr explicit QuantityReferenceOwnershipPolicy(
        ValueType& src) noexcept
        : value(src) {}
    //! \brief return the value
    TFEL_HOST_DEVICE constexpr ValueType& getValue() noexcept {
      return this->value;
    }
    //! \brief return the value
    TFEL_HOST_DEVICE constexpr const ValueType& getValue() const noexcept {
      return this->value;
    }
    //! \brief assignement operator
    constexpr QuantityReferenceOwnershipPolicy& operator=(
        const QuantityReferenceOwnershipPolicy&) noexcept = default;
    //! \brief move assignement operator
    constexpr QuantityReferenceOwnershipPolicy& operator=(
        QuantityReferenceOwnershipPolicy&&) noexcept = default;
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T>
    TFEL_HOST_DEVICE constexpr QuantityReferenceOwnershipPolicy& operator=(
        const T& src) noexcept
        requires((std::is_constructible_v<ValueType, T>)&&  //
                 (std::is_convertible_v<ValueType, T>)&&    //
                 (AllowImplicitConversion)) {
      this->value = src;
      return *this;
    }
    // NOLINTBEGIN(google-explicit-constructor)
    //! \brief conversion operator
    template <typename T>
    TFEL_HOST_DEVICE constexpr operator T&() noexcept
        requires((std::is_same_v<ValueType, T>)&&  //
                 (AllowImplicitConversion)) {
      return this->value;
    }
    //! \brief conversion operator
    template <typename T>
    TFEL_HOST_DEVICE constexpr operator const T&() const noexcept
        requires((std::is_same_v<ValueType, T>)&&  //
                 (AllowImplicitConversion)) {
      return this->value;
    }
    // NOLINTEND(google-explicit-constructor)
    //! \brief destructor
    ~QuantityReferenceOwnershipPolicy() noexcept = default;

   private:
    //! \brief reference to the value of the quantity
    ValueType& value;
  };

}  // namespace tfel::math::internals

namespace tfel::math {

  template <unit::UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType,
            typename OwnershipPolicy>
  struct Quantity : OwnershipPolicy {
    /*!
     * An helper class to deal with some limitations of Visual Studio
     */
    template <typename T, typename T2>
    struct IsQtScalarOperationValid {
      static constexpr bool cond =
          tfel::typetraits::IsFundamentalNumericType<T2>::cond &&
          isScalar<T>() &&
          std::is_same_v<typename tfel::typetraits::Promote<T, T2>::type, T>;
    };
    //! \brief a simple alias
    template <StandardArithmeticTypeConcept ValueType1,
              StandardArithmeticTypeConcept ValueType2>
    using promote =
        typename tfel::typetraits::Promote<ValueType1, ValueType2>::type;
    //! \brief a simple alias
    using value_type = ValueType;
    //
    using OwnershipPolicy::OwnershipPolicy;
    //
    // NOLINTBEGIN(google-explicit-constructor)
    template <QuantityConcept OtherQuantityType>
    TFEL_HOST_DEVICE constexpr Quantity(const OtherQuantityType& src) noexcept
        requires(
            (unit::areUnitsEqual<UnitType, quantity_unit<OtherQuantityType>>)&&(
                std::is_same_v<promote<ValueType, base_type<OtherQuantityType>>,
                               ValueType>))
        : OwnershipPolicy(base_type_cast(src)) {}  // end of Quantity
    // NOLINTEND(google-explicit-constructor)
    //
    template <StandardArithmeticTypeConcept T>
    TFEL_HOST_DEVICE constexpr Quantity& operator=(const T& src) noexcept
        requires((std::is_constructible_v<ValueType, T>)&&  //
                 (std::is_convertible_v<ValueType, T>)&&    //
                 (unit::areUnitsEqual<UnitType, unit::NoUnit>)) {
      this->getValue() = src;
      return *this;
    }
    template <StandardArithmeticTypeConcept T>
    TFEL_HOST_DEVICE constexpr Quantity& operator+=(const T& src) noexcept
        requires((std::is_constructible_v<ValueType, T>)&&  //
                 (std::is_convertible_v<ValueType, T>)&&    //
                 (unit::areUnitsEqual<UnitType, unit::NoUnit>)) {
      this->getValue() += src;
      return *this;
    }
    template <StandardArithmeticTypeConcept T>
    TFEL_HOST_DEVICE constexpr Quantity& operator-=(const T& src) noexcept
        requires((std::is_constructible_v<ValueType, T>)&&  //
                 (std::is_convertible_v<ValueType, T>)&&    //
                 (unit::areUnitsEqual<UnitType, unit::NoUnit>)) {
      this->getValue() -= src;
      return *this;
    }
    //! \brief assignement operator
    template <QuantityConcept OtherQuantityType>
    TFEL_HOST_DEVICE constexpr Quantity& operator=(
        const OtherQuantityType& src) noexcept
        requires(
            (unit::areUnitsEqual<UnitType, quantity_unit<OtherQuantityType>>)&&(
                std::is_same_v<promote<ValueType, base_type<OtherQuantityType>>,
                               ValueType>)) {
      this->getValue() = base_type_cast(src);
      return *this;
    }
    //! \brief operator +=
    template <QuantityConcept OtherQuantityType>
    TFEL_HOST_DEVICE constexpr Quantity& operator+=(
        const OtherQuantityType& src) noexcept
        requires(
            (unit::areUnitsEqual<UnitType, quantity_unit<OtherQuantityType>>)&&(
                std::is_same_v<promote<ValueType, base_type<OtherQuantityType>>,
                               ValueType>)) {
      this->getValue() += base_type_cast(src);
      return *this;
    }
    //! \brief operator -=
    template <QuantityConcept OtherQuantityType>
    TFEL_HOST_DEVICE constexpr Quantity& operator-=(
        const OtherQuantityType& src) noexcept
        requires(
            (unit::areUnitsEqual<UnitType, quantity_unit<OtherQuantityType>>)&&(
                std::is_same_v<promote<ValueType, base_type<OtherQuantityType>>,
                               ValueType>)) {
      this->getValue() -= base_type_cast(src);
      return *this;
    }
    /*!
     * \brief multiplication by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <StandardArithmeticTypeConcept ValueType2>
    TFEL_HOST_DEVICE constexpr Quantity& operator*=(
        const ValueType2& a) noexcept
        requires(IsQtScalarOperationValid<ValueType, ValueType2>::cond) {
      this->getValue() *= a;
      return *this;
    }
    /*!
     * \brief multiplication by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <NoUnitQuantityConcept OtherQuantityType>
    TFEL_HOST_DEVICE constexpr Quantity& operator*=(
        const OtherQuantityType& a) noexcept
        requires(IsQtScalarOperationValid<ValueType,
                                          base_type<OtherQuantityType>>::cond) {
      this->getValue() *= base_type_cast(a);
      return *this;
    }
    /*!
     * \brief division by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <StandardArithmeticTypeConcept ValueType2>
    TFEL_HOST_DEVICE constexpr Quantity& operator/=(
        const ValueType2& a) noexcept
        requires(IsQtScalarOperationValid<ValueType, ValueType2>::cond) {
      this->getValue() /= a;
      return *this;
    }
    /*!
     * \brief division by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <NoUnitQuantityConcept OtherQuantityType>
    TFEL_HOST_DEVICE constexpr Quantity& operator/=(
        const OtherQuantityType& a) noexcept
        requires(IsQtScalarOperationValid<ValueType,
                                          base_type<OtherQuantityType>>::cond) {
      this->getValue() /= base_type_cast(a);
      return *this;
    }
    //! \brief negation operator
    TFEL_HOST_DEVICE constexpr qt<UnitType, ValueType> operator-()
        const noexcept {
      return qt<UnitType, ValueType>(-(this->getValue()));
    }
  };  // end of struct Quantity

  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  struct qt : Quantity<UnitType,
                       ValueType,
                       tfel::math::internals::QuantityValueOwnershipPolicy<
                           ValueType,
                           unit::areUnitsEqual<UnitType, unit::NoUnit>>> {
    //
    TFEL_HOST_DEVICE constexpr qt() noexcept
        : Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityValueOwnershipPolicy<
                       ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>() {}
    //
    TFEL_HOST_DEVICE constexpr qt(qt&&) noexcept = default;
    TFEL_HOST_DEVICE constexpr qt(const qt&) noexcept = default;
    TFEL_HOST_DEVICE constexpr qt& operator=(qt&&) noexcept = default;
    TFEL_HOST_DEVICE constexpr qt& operator=(const qt&) noexcept = default;
    //
    using Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityValueOwnershipPolicy<
                       ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>::Quantity;
    using Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityValueOwnershipPolicy<
                       ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>::operator=;
  };

  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  struct QuantityTraits<qt<UnitType, ValueType>>
      : QuantityTraits<
            Quantity<UnitType,
                     ValueType,
                     tfel::math::internals::QuantityValueOwnershipPolicy<
                         ValueType,
                         unit::areUnitsEqual<UnitType, unit::NoUnit>>>> {};

  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  struct qt_ref
      : Quantity<UnitType,
                 ValueType,
                 tfel::math::internals::QuantityReferenceOwnershipPolicy<
                     ValueType,
                     unit::areUnitsEqual<UnitType, unit::NoUnit>>> {
    //
    template <QuantityConcept QuantityType>
    requires(unit::areUnitsEqual<quantity_unit<QuantityType>,
                                 UnitType>)  //
        explicit constexpr qt_ref(QuantityType& src) noexcept
        : Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityReferenceOwnershipPolicy<
                       ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>(
              base_type_cast(src)) {}
    //
    explicit constexpr qt_ref(ValueType& src) noexcept
        : Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityReferenceOwnershipPolicy<
                       ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>(src) {}
    constexpr qt_ref(qt_ref&&) noexcept = default;
    constexpr qt_ref(const qt_ref&) noexcept = default;
    using Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityReferenceOwnershipPolicy<
                       ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>::operator=;
  };

  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  struct QuantityTraits<qt_ref<UnitType, ValueType>>
      : QuantityTraits<
            Quantity<UnitType,
                     ValueType,
                     tfel::math::internals::QuantityReferenceOwnershipPolicy<
                         ValueType,
                         unit::areUnitsEqual<UnitType, unit::NoUnit>>>> {};

  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  struct const_qt_ref
      : Quantity<UnitType,
                 ValueType,
                 tfel::math::internals::QuantityReferenceOwnershipPolicy<
                     const ValueType,
                     unit::areUnitsEqual<UnitType, unit::NoUnit>>> {
    //
    template <QuantityConcept QuantityType>
    requires(unit::areUnitsEqual<quantity_unit<QuantityType>,
                                 UnitType>)  //
        explicit constexpr const_qt_ref(const QuantityType& src) noexcept
        : Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityReferenceOwnershipPolicy<
                       const ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>(
              base_type_cast(src)) {}
    //
    explicit constexpr const_qt_ref(const ValueType& src) noexcept
        : Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityReferenceOwnershipPolicy<
                       const ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>(src) {}
    constexpr const_qt_ref(const_qt_ref&&) noexcept = default;
    constexpr const_qt_ref(const const_qt_ref&) noexcept = default;
    using Quantity<UnitType,
                   ValueType,
                   tfel::math::internals::QuantityReferenceOwnershipPolicy<
                       const ValueType,
                       unit::areUnitsEqual<UnitType, unit::NoUnit>>>::operator=;
  };

  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  struct QuantityTraits<const_qt_ref<UnitType, ValueType>>
      : QuantityTraits<
            Quantity<UnitType,
                     ValueType,
                     tfel::math::internals::QuantityReferenceOwnershipPolicy<
                         const ValueType,
                         unit::areUnitsEqual<UnitType, unit::NoUnit>>>> {};

  // class template argument deduction guidelines
  template <unit::UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType,
            typename OwnershipPolicy>
  Quantity(Quantity<UnitType, ValueType, OwnershipPolicy>)
      -> Quantity<UnitType, ValueType, OwnershipPolicy>;
  //
  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  qt(qt<UnitType, ValueType>) -> qt<UnitType, ValueType>;
  //
  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  qt_ref(qt_ref<UnitType, ValueType>) -> qt_ref<UnitType, ValueType>;
  //
  template <unit::UnitConcept UnitType, StandardArithmeticTypeConcept ValueType>
  const_qt_ref(const_qt_ref<UnitType, ValueType>)
      -> const_qt_ref<UnitType, ValueType>;

  template <unit::UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType,
            typename OwnershipPolicy>
  TFEL_HOST_DEVICE constexpr ValueType& base_type_cast(
      Quantity<UnitType, ValueType, OwnershipPolicy>& v) noexcept {
    return v.getValue();
  }

  template <unit::UnitConcept UnitType,
            StandardArithmeticTypeConcept ValueType,
            typename OwnershipPolicy>
  TFEL_HOST_DEVICE constexpr const ValueType& base_type_cast(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& v) noexcept {
    return v.getValue();
  }

  template <QuantityConcept T, int N, unsigned int D>
  class UnaryResultType<T, Power<N, D>> {
    using ResultUnit = typename tfel::math::unit::internals::
        PowerUnit<N, D, quantity_unit<T>>::type;
    using ResultValueType =
        typename UnaryResultType<base_type<T>, Power<N, D>>::type;

   public:
    //! \brief result of the metafunction
    using type = qt<ResultUnit, ResultValueType>;
  };

  template <int N, QuantityConcept T>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto power(const T& x) requires(
      std::is_floating_point_v<base_type<T>>) {
    using Result = typename UnaryResultType<T, Power<N, 1u>>::type;
    return Result{power<N>(base_type_cast(x))};
  }

  template <int N, unsigned int D, QuantityConcept T>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto power(const T& x) requires(
      std::is_floating_point_v<base_type<T>>) {
    using Result = typename UnaryResultType<T, Power<N, D>>::type;
    return Result{power<N, D>(base_type_cast(x))};
  }

  template <QuantityConcept T>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto square_root(
      const T& q) noexcept {
    return power<1, 2>(base_type_cast(q));
  }

}  // namespace tfel::math

namespace tfel::math::ieee754 {

  /*!
   * \return the class of the floating point value according to the
   * IEEE754 standard.
   * Possible return values are:
   * - FP_NORMAL: standard floating point value
   * - FP_SUBNORMAL: subnormal floating point value
   * - FP_NAN: NaN value
   * - FP_INFINITE: +Inf or -Inf value
   * \param[in] q: value to be tested
   */
  template <QuantityConcept QuantityType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr int fpclassify(
      const QuantityType& q) noexcept {
    static_assert(std::is_floating_point_v<base_type<QuantityType>>,
                  "base type must a floating point value");
    return fpclassify(base_type_cast(q));
  }  // end of fpclassify

  /*!
   * \return true if the given quantity is a not-a-number (NaN) value.
   * \param[in] q: value to be tested
   */
  template <QuantityConcept QuantityType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr bool isnan(
      const QuantityType& q) noexcept {
    static_assert(std::is_floating_point_v<base_type<QuantityType>>,
                  "base type must a floating point value");
    return isnan(base_type_cast(q));
  }

  /*!
   * \return true if the given quantity is finite.
   * \param[in] q: value to be tested
   */
  template <QuantityConcept QuantityType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr bool isfinite(
      const QuantityType& q) noexcept {
    static_assert(std::is_floating_point_v<base_type<QuantityType>>,
                  "base type must a floating point value");
    return isfinite(base_type_cast(q));
  }

}  // end of namespace tfel::math::ieee754

#include "TFEL/Math/Quantity/qtLimits.hxx"
#include "TFEL/Math/Quantity/qtSpecific.hxx"
#include "TFEL/Math/Quantity/qtOperations.hxx"

#endif /* LIB_TFEL_MATH_QT_HXX */
