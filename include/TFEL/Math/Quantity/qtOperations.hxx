/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.hxx
 * \brief  This file declares usual operations on quantities.
 * \author Thomas Helfer
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QT_OPERATIONS_HXX
#define LIB_TFEL_MATH_QT_OPERATIONS_HXX

#include <concepts>
#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"
#include "TFEL/Math/qt.hxx"

#define TFEL_MATH_QT_NOUNIT_COMPARISION_OPERATORS(Op)                         \
  template <ImmutableQuantityConcept QuantityType,                            \
            StandardArithmeticTypeConcept ScalarType>                         \
  TFEL_HOST_DEVICE constexpr bool operator Op(const QuantityType& a,          \
                                              const ScalarType& b) noexcept { \
    static_assert(NoUnitImmutableQuantityConcept<QuantityType>,               \
                  "invalid operation (unmatched unit)");                      \
    return base_type_cast(a) Op b;                                            \
  }                                                                           \
  template <StandardArithmeticTypeConcept ScalarType,                         \
            ImmutableQuantityConcept QuantityType>                            \
  TFEL_HOST_DEVICE constexpr bool operator Op(                                \
      const ScalarType& a, const QuantityType& b) noexcept {                  \
    static_assert(NoUnitImmutableQuantityConcept<QuantityType>,               \
                  "invalid operation (unmatched unit)");                      \
    return a Op base_type_cast(b);                                            \
  }

#define TFEL_MATH_QT_SCALAR_OPERATIONS(Op)                   \
  template <ImmutableQuantityConcept QuantityType,           \
            StandardArithmeticTypeConcept ScalarType>        \
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator Op( \
      const QuantityType&, const ScalarType&) noexcept;      \
  template <StandardArithmeticTypeConcept ScalarType,        \
            ImmutableQuantityConcept QuantityType>           \
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator Op( \
      const ScalarType&, const QuantityType&) noexcept;

namespace tfel::math {

  template <ImmutableQuantityConcept T>
  struct ComputeUnaryOperationResult<ScalarTag, UnaryOperatorTag, T, OpNeg> {
    //! \brief result
    using type = qt<quantity_unit<T>,
                    typename UnaryResultType<base_type<T>, OpNeg>::type>;
  };  // end of struct ComputeUnaryOperationResult

  //! \brief negation operator
  template <ImmutableQuantityConcept QuantityType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator-(
      const QuantityType& q) noexcept {
    return qt_rebind<QuantityType>{-base_type_cast(q)};
  }  // end of operator-

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr bool operator<(const T1& a, const T2& b) noexcept {
    static_assert(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>,
                  "invalid operation (unmatched unit)");
    return base_type_cast(a) < base_type_cast(b);
  }  // end of operator<

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr bool operator<=(const T1& a,
                                             const T2& b) noexcept {
    static_assert(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>,
                  "invalid operation (unmatched unit)");
    return base_type_cast(a) <= base_type_cast(b);
  }  // end of operator<=

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr bool operator>(const T1& a, const T2& b) noexcept {
    static_assert(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>,
                  "invalid operation (unmatched unit)");
    return base_type_cast(a) > base_type_cast(b);
  }  // end of operator>

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr bool operator>=(const T1& a,
                                             const T2& b) noexcept {
    static_assert(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>,
                  "invalid operation (unmatched unit)");
    return base_type_cast(a) >= base_type_cast(b);
  }  // end of operator>=

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr bool operator==(const T1& a,
                                             const T2& b) noexcept {
    static_assert(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>,
                  "invalid operation (unmatched unit)");
    if constexpr (std::floating_point<quantity_unit<T1>>) {
      return tfel::math::ieee754::fpclassify(base_type_cast(a) -
                                             base_type_cast(b)) == FP_ZERO;
    } else {
      return base_type_cast(a) == base_type_cast(b);
    }
  }  // end of operator==

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr bool operator!=(const T1& a,
                                             const T2& b) noexcept {
    static_assert(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>,
                  "invalid operation (unmatched unit)");
    return base_type_cast(a) != base_type_cast(b);
  }  // end of operator!=

  /*!
   *\brief partial specialisation for addition of two quantity objects having
   * the same unit
   */
  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  requires(unit::areUnitsEqual<quantity_unit<T1>,
                               quantity_unit<T2>>)  //
      struct ResultType<T1, T2, OpPlus> {
    using type = qt<quantity_unit<T1>,
                    result_type<base_type<T1>, base_type<T2>, OpPlus>>;
  };

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr auto operator+(const T1& a, const T2& b) noexcept
      requires(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>)
  {
    return result_type<T1, T2, OpPlus>{base_type_cast(a) + base_type_cast(b)};
  }

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr auto operator+(const T1& a, const T2& b) noexcept
      requires(!std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>) = delete;

#pragma message("all invalid operations must be deleted")

  /*!
   * \brief partial specialisation for subtraction of two quantity objects
   * having the same unit
   */
  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  requires(unit::areUnitsEqual<quantity_unit<T1>,
                               quantity_unit<T2>>)  //
      struct ResultType<T1, T2, OpMinus> {
    using type = qt<quantity_unit<T1>,
                    result_type<base_type<T1>, base_type<T2>, OpMinus>>;
  };

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  TFEL_HOST_DEVICE constexpr auto operator-(const T1& a, const T2& b) noexcept {
    static_assert(std::is_same_v<quantity_unit<T1>, quantity_unit<T2>>,
                  "invalid operation");
    return result_type<T1, T2, OpPlus>{base_type_cast(a) - base_type_cast(b)};
  }

  //! \brief partial specialisation for multiplication of two quantity objects
  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  struct ResultType<T1, T2, OpMult> {
    using type = qt<typename tfel::math::unit::internals::
                        AddUnit<quantity_unit<T1>, quantity_unit<T2>>::type,
                    result_type<base_type<T1>, base_type<T2>, OpMult>>;
  };

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  constexpr auto operator*(const T1& a, const T2& b) noexcept {
    return result_type<T1, T2, OpMult>{base_type_cast(a) * base_type_cast(b)};
  }  // end of operator*

  //! \brief partial specialisation for division of two quantity objects
  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  struct ResultType<T1, T2, OpDiv> {
    using type =
        qt<typename tfel::math::unit::internals::
               SubtractUnit<quantity_unit<T1>, quantity_unit<T2>>::type,
           result_type<base_type<T1>, base_type<T2>, OpDiv>>;
  };

  template <ImmutableQuantityConcept T1, ImmutableQuantityConcept T2>
  constexpr auto operator/(const T1& a, const T2& b) noexcept {
    return result_type<T1, T2, OpDiv>{base_type_cast(a) / base_type_cast(b)};
  }  // end of operator/

  /*!
   * \brief partial specialisation for fundamental numeric type and a quantity
   * with no unit
   */
  template <StandardArithmeticTypeConcept ScalarType,
            NoUnitImmutableQuantityConcept QuantityType>
  struct ResultType<ScalarType, QuantityType, OpPlus> {
    using type = qt<unit::NoUnit,
                    result_type<ScalarType, base_type<QuantityType>, OpPlus>>;
  };

  /*!
   * \brief partial specialisation for a quantity with no unit and a fundamental
   * numeric type
   */
  template <NoUnitImmutableQuantityConcept QuantityType,
            StandardArithmeticTypeConcept ScalarType>
  struct ResultType<QuantityType, ScalarType, OpPlus> {
    using type = qt<unit::NoUnit,
                    result_type<base_type<QuantityType>, ScalarType, OpPlus>>;
  };

  /*!
   * \brief partial specialisation for fundamental numeric type and a quantity
   * with no unit
   */
  template <StandardArithmeticTypeConcept ScalarType,
            NoUnitImmutableQuantityConcept QuantityType>
  struct ResultType<ScalarType, QuantityType, OpMinus> {
    using type = qt<unit::NoUnit,
                    result_type<ScalarType, base_type<QuantityType>, OpMinus>>;
  };

  /*!
   * \brief partial specialisation for a quantity with no unit and a fundamental
   * numeric type
   */
  template <NoUnitImmutableQuantityConcept QuantityType,
            StandardArithmeticTypeConcept ScalarType>
  struct ResultType<QuantityType, ScalarType, OpMinus> {
    using type = qt<unit::NoUnit,
                    result_type<base_type<QuantityType>, ScalarType, OpMinus>>;
  };

  /*!
   * \brief partial specialisation for fundamental numeric type and a quantity
   * with no unit
   */
  template <StandardArithmeticTypeConcept ScalarType,
            ImmutableQuantityConcept QuantityType>
  struct ResultType<ScalarType, QuantityType, OpMult> {
    using type = qt<quantity_unit<QuantityType>,
                    result_type<ScalarType, base_type<QuantityType>, OpMult>>;
  };

  /*!
   * \brief partial specialisation for a quantity with no unit and a fundamental
   * numeric type
   */
  template <ImmutableQuantityConcept QuantityType,
            StandardArithmeticTypeConcept ScalarType>
  struct ResultType<QuantityType, ScalarType, OpMult> {
    using type = qt<quantity_unit<QuantityType>,
                    result_type<base_type<QuantityType>, ScalarType, OpMult>>;
  };

  /*!
   * \brief partial specialisation for fundamental numeric type and a quantity
   * with no unit
   */
  template <StandardArithmeticTypeConcept ScalarType,
            ImmutableQuantityConcept QuantityType>
  struct ResultType<ScalarType, QuantityType, OpDiv> {
    using type =
        qt<typename tfel::math::unit::internals::
               SubtractUnit<unit::NoUnit, quantity_unit<QuantityType>>::type,
           result_type<ScalarType, base_type<QuantityType>, OpDiv>>;
  };

  /*!
   * \brief partial specialisation for a quantity with no unit and a fundamental
   * numeric type
   */
  template <ImmutableQuantityConcept QuantityType,
            StandardArithmeticTypeConcept ScalarType>
  struct ResultType<QuantityType, ScalarType, OpDiv> {
    using type = qt<quantity_unit<QuantityType>,
                    result_type<base_type<QuantityType>, ScalarType, OpDiv>>;
  };

  TFEL_MATH_QT_NOUNIT_COMPARISION_OPERATORS(<)
  TFEL_MATH_QT_NOUNIT_COMPARISION_OPERATORS(<=)
  TFEL_MATH_QT_NOUNIT_COMPARISION_OPERATORS(>)
  TFEL_MATH_QT_NOUNIT_COMPARISION_OPERATORS(>=)
  TFEL_MATH_QT_NOUNIT_COMPARISION_OPERATORS(==)
  TFEL_MATH_QT_NOUNIT_COMPARISION_OPERATORS(!=)

  TFEL_MATH_QT_SCALAR_OPERATIONS(+);
  TFEL_MATH_QT_SCALAR_OPERATIONS(-);
  TFEL_MATH_QT_SCALAR_OPERATIONS(*);
  TFEL_MATH_QT_SCALAR_OPERATIONS(/);

}  // namespace tfel::math

#include "TFEL/Math/Quantity/qtOperations.ixx"

#endif /* LIB_TFEL_MATH_QT_OPERATIONS_HXX */
