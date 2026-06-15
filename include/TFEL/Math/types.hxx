/*!
 * \file   TFEL/Math/types.hxx
 * \brief  This file implements useful features for handling types, in
 * particular when quantity are involved. \author Thomas Helfer \date 11/11/2024
 */

#ifndef LIB_TFEL_MATH_TYPES_HXX
#define LIB_TFEL_MATH_TYPES_HXX

#include <type_traits>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*!
   * \brief this struct exposes standard types compatible with its argument.
   * - the exposed types have the same space dimension at the argument
   * - the exposed types use quantities if the argument does.
   *
   * A typical usage of this class is to deduce the types of the arguments of
   * a method from the type of one of the argument.
   */
  template <MathObjectConcept Type>
  struct TypeRebind : tfel::config::Types<MathObjectTraits<Type>::dime,
                                          base_type<numeric_type<Type>>,
                                          isQuantity<numeric_type<Type>>()> {};
  /*!
   * \brief this struct exposes standard scalar types compatible with its
   * argument. The exposed types use quantities if the argument does.
   *
   * Let us consider the following function:
   *
   * \code
   * template <tfel::math::ScalarConcept StressType>
   * StressType computeLambda(
   *  const StressType& young,
   *  const typename tfel::math::ScalarTypeRebind<StressType>::real& nu);
   * \endcode
   *
   * This code takes one template parameter, `StressType`, which must
   * satisfies the `ScalarConcept` concept. Here, the type of the second
   * parameter is deduced for `StressType`:
   *
   * - if `StressType` matches `qt<Stress, NumericType>`, `nu` must have the
   * type `qt<NoUnit, NumericType>`, where `NumericType` is a plain old
   * floatting point type such as `double`, `float` or `long double`.
   * - if `StressType` matches `double`, `nu` must have the type `double`.
   */
  template <ScalarConcept Type>
  struct ScalarTypeRebind
      : tfel::config::ScalarTypes<base_type<numeric_type<Type>>,
                                  isQuantity<numeric_type<Type>>()> {};

  /*!
   * \brief this function checks if the given type matches the given unit if it
   * built on top of a quantity. If the given type is built on a plain old
   * scalar type, this function always returns true.
   *
   * This function allows to add requirements on a template parameter of a
   * function. Let us consider the following function:
   *
   * \code
   * template <tfel::math::ScalarConcept StressType>
   * StressType computeLambda(
   *  const StressType& young,
   *  const typename tfel::math::ScalarTypeRebind<StressType>::real& nu)
   *  requires(
   *      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
   * StressType>()); \endcode
   *
   * Thanks to the `checkUnitCompatibility` function, this function only
   * compiles if `StressType` either:
   *
   * - maches `qt<Stress, NumericType>`, where `NumericType` is a plain old
   * floatting point type such as `double`, `float` or `long double`.
   * - is a is a plain old floatting point type.
   *
   * \tparam Unit: unit used for the test
   * \tparam T: tested type
   */
  template <UnitConcept Unit, typename T>
  requires(ScalarConcept<T> || MathObjectConcept<T>)  //
      constexpr bool checkUnitCompatibility();

}  // end of namespace tfel::math

namespace tfel::types {

  template <tfel::math::ScalarConcept ScalarType>
  using real = typename tfel::math::ScalarTypeRebind<ScalarType>::real;

  template <tfel::math::ScalarConcept ScalarType>
  using time = typename tfel::math::ScalarTypeRebind<ScalarType>::time;

  template <tfel::math::ScalarConcept ScalarType>
  using frequency =
      typename tfel::math::ScalarTypeRebind<ScalarType>::frequency;

  template <tfel::math::ScalarConcept ScalarType>
  using length = typename tfel::math::ScalarTypeRebind<ScalarType>::length;

  template <tfel::math::ScalarConcept ScalarType>
  using inv_length =
      typename tfel::math::ScalarTypeRebind<ScalarType>::inv_length;

  template <tfel::math::ScalarConcept ScalarType>
  using displacement =
      typename tfel::math::ScalarTypeRebind<ScalarType>::displacement;

  template <tfel::math::ScalarConcept ScalarType>
  using force = typename tfel::math::ScalarTypeRebind<ScalarType>::force;

  template <tfel::math::ScalarConcept ScalarType>
  using strain = typename tfel::math::ScalarTypeRebind<ScalarType>::strain;

  template <tfel::math::ScalarConcept ScalarType>
  using strainrate =
      typename tfel::math::ScalarTypeRebind<ScalarType>::strainrate;

  template <tfel::math::ScalarConcept ScalarType>
  using stress = typename tfel::math::ScalarTypeRebind<ScalarType>::stress;

  template <tfel::math::ScalarConcept ScalarType>
  using compliance =
      typename tfel::math::ScalarTypeRebind<ScalarType>::compliance;

  template <tfel::math::ScalarConcept ScalarType>
  using viscosity =
      typename tfel::math::ScalarTypeRebind<ScalarType>::viscosity;

  template <tfel::math::ScalarConcept ScalarType>
  using viscouscompliance =
      typename tfel::math::ScalarTypeRebind<ScalarType>::viscouscompliance;

  template <tfel::math::ScalarConcept ScalarType>
  using stressrate =
      typename tfel::math::ScalarTypeRebind<ScalarType>::stressrate;

  template <tfel::math::ScalarConcept ScalarType>
  using temperature =
      typename tfel::math::ScalarTypeRebind<ScalarType>::temperature;

  template <tfel::math::ScalarConcept ScalarType>
  using thermalexpansion =
      typename tfel::math::ScalarTypeRebind<ScalarType>::thermalexpansion;

  template <tfel::math::ScalarConcept ScalarType>
  using massdensity =
      typename tfel::math::ScalarTypeRebind<ScalarType>::massdensity;

  template <tfel::math::ScalarConcept ScalarType>
  using energydensity =
      typename tfel::math::ScalarTypeRebind<ScalarType>::energydensity;

  template <tfel::math::ScalarConcept ScalarType>
  using speed = typename tfel::math::ScalarTypeRebind<ScalarType>::speed;

  template <tfel::math::ScalarConcept ScalarType>
  using thermalconductivity =
      typename tfel::math::ScalarTypeRebind<ScalarType>::thermalconductivity;

}  // end of namespace tfel::types

#include "TFEL/Math/types.ixx"

#endif /* LIB_TFEL_MATH_TYPES_HXX */
