/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.ixx
 * \brief  This file implements usual operations on quantities.
 * \author Thomas Helfer
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTOPERATIONS_IXX
#define LIB_TFEL_MATH_QTOPERATIONS_IXX

namespace tfel::math {

#define TFEL_MATH_QT_SCALAR_OPERATIONS_IMPL1(Op)                             \
                                                                             \
  template <QuantityConcept QuantityType,                                    \
            StandardArithmeticTypeConcept ScalarType>                        \
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator Op(                 \
      const QuantityType& lhs, const ScalarType& rhs) noexcept {             \
    static_assert(NoUnitQuantityConcept<QuantityType>, "invalid operation"); \
    return qt<unit::NoUnit, typename tfel::typetraits::Promote<              \
                                base_type<QuantityType>, ScalarType>::type>{ \
        base_type_cast(lhs) Op rhs};                                         \
  }                                                                          \
                                                                             \
  template <StandardArithmeticTypeConcept ScalarType,                        \
            QuantityConcept QuantityType>                                    \
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator Op(                 \
      const ScalarType& lhs, const QuantityType& rhs) noexcept {             \
    static_assert(NoUnitQuantityConcept<QuantityType>, "invalid operation"); \
    return qt<unit::NoUnit, typename tfel::typetraits::Promote<              \
                                base_type<QuantityType>, ScalarType>::type>{ \
        lhs Op base_type_cast(rhs)};                                         \
  }

  TFEL_MATH_QT_SCALAR_OPERATIONS_IMPL1(+)
  TFEL_MATH_QT_SCALAR_OPERATIONS_IMPL1(-)

  template <QuantityConcept QuantityType,
            StandardArithmeticTypeConcept ScalarType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator*(
      const QuantityType& lhs, const ScalarType& rhs) noexcept {
    return qt<quantity_unit<QuantityType>,                                
              typename tfel::typetraits::Promote<base_type<QuantityType>, 
                                                 ScalarType>::type>{      
        base_type_cast(lhs) * rhs};
  }

  template <StandardArithmeticTypeConcept ScalarType,                     
            QuantityConcept QuantityType>                                 
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator *(              
      const ScalarType& lhs, const QuantityType& rhs) noexcept {
    return qt<quantity_unit<QuantityType>,
              typename tfel::typetraits::Promote<base_type<QuantityType>,
                                                 ScalarType>::type>{
        lhs * base_type_cast(rhs)};
  }

  template <QuantityConcept QuantityType,
            StandardArithmeticTypeConcept ScalarType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator/(
      const QuantityType& lhs, const ScalarType& rhs) noexcept {
    return qt<quantity_unit<QuantityType>,                                
              typename tfel::typetraits::Promote<base_type<QuantityType>, 
                                                 ScalarType>::type>{      
        base_type_cast(lhs) / rhs};
  }

  template <StandardArithmeticTypeConcept ScalarType,                     
            QuantityConcept QuantityType>                                 
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto operator /(              
      const ScalarType& lhs, const QuantityType& rhs) noexcept {
    return qt<typename internals::SubtractUnit<
                  unit::NoUnit, quantity_unit<QuantityType>>::type,
              typename tfel::typetraits::Promote<base_type<QuantityType>,
                                                 ScalarType>::type>{
        lhs / base_type_cast(rhs)};
  }

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_QTOPERATIONS_IXX */
