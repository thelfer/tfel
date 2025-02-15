/*!
 * \file   TFEL/Math/types.ixx
 * \brief  This file implements the checkUnitCompatibility function
 * \author Thomas Helfer
 * \date   12/11/2024
 */

#ifndef LIB_TFEL_MATH_TYPES_IXX
#define LIB_TFEL_MATH_TYPES_IXX

namespace tfel::math {

  namespace internals {

    template <UnitConcept Unit, typename Type>
    struct CheckUnitCompatibilityImplementation;

    template <UnitConcept Unit, ScalarConcept NumericType>
    struct CheckUnitCompatibilityImplementation<Unit, NumericType>
        : std::conditional_t<
              isQuantity<NumericType>(),
              std::is_same<typename QuantityTraits<NumericType>::UnitType,
                           Unit>,
              std::true_type> {};

    template <UnitConcept Unit, MathObjectConcept MathObjectType>
    struct CheckUnitCompatibilityImplementation<Unit, MathObjectType>
        : CheckUnitCompatibilityImplementation<Unit,
                                               numeric_type<MathObjectType>> {};

  }  // namespace internals

  template <UnitConcept Unit, typename T>
  requires(ScalarConcept<T> || MathObjectConcept<T>)  //
      struct CheckUnitCompatibility
      : internals::CheckUnitCompatibilityImplementation<Unit, std::decay_t<T>> {
  };

  template <UnitConcept Unit, typename T>
  requires(ScalarConcept<T> || MathObjectConcept<T>)  //
      constexpr bool checkUnitCompatibility() {
    return CheckUnitCompatibility<Unit, std::decay_t<T>>::value;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TYPES_IXX */
