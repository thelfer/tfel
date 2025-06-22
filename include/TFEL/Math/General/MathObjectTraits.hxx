/*!
 * \file   include/TFEL/Math/General/MathObjectTraits.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/12/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_GENERAL_MATHOBJECTTRAITS_HXX
#define LIB_TFEL_MATH_GENERAL_MATHOBJECTTRAITS_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"

namespace tfel::math {

  /*!
   * \brief a helper structure to define the `MathObjectTraits`
   * more compactly
   */
  template <typename ValueType, typename SizeType>
  struct MathObjectTraitsBase {
    /*!
     * \brief numerical type on which the object is based
     */
    using value_type = ValueType;
    /*!
     * \brief index type
     */
    using size_type = SizeType;
    /*!
     * \brief numerical type on which the object is based
     * \deprecated This alias is deprecated and provided for backward
     * compatibility with previous versions of `TFEL`. Use `size_type`
     * instead.
     */
    using NumType = value_type;
    /*!
     * \brief index type
     * \deprecated This alias is deprecated and provided for backward
     * compatibility with previous versions of `TFEL`. Use `size_type` instead.
     */
    using IndexType = size_type;
  };  // end of struct MathObjectTraitsBase

  /*!
   * \brief a traits class describing a mathematical object
   * \tparam MathObjectType: object described by the traits
   */
  template <typename MathObjectType>
  struct MathObjectTraits
      : MathObjectTraitsBase<tfel::meta::InvalidType, tfel::meta::InvalidType> {
    /*!
     * \brief space dimension
     *
     * This value is set to 0 if the object is not related to the space
     * dimension
     */
    static constexpr unsigned short dime = 0u;
  };  // end of struct MathObjectTraits

  //! \brief a simple alias
  template <typename MathObjectType>
  using numeric_type = std::conditional_t<
      tfel::typetraits::isScalar<std::decay_t<MathObjectType>>(),
      std::decay_t<MathObjectType>,
      typename MathObjectTraits<std::decay_t<MathObjectType>>::value_type>;
  //! \brief a simple alias
  template <typename MathObjectType>
  using index_type =
      typename MathObjectTraits<std::decay_t<MathObjectType>>::size_type;
  //! \brief a simple alias
  template <typename T>
  using base_type =
      std::conditional_t<tfel::typetraits::isScalar<T>(),
                         tfel::typetraits::base_type<T>,
                         tfel::typetraits::base_type<numeric_type<T>>>;
  //! \brief cast the value to the base type
  template <typename T>
  TFEL_HOST_DEVICE constexpr std::
      enable_if_t<tfel::typetraits::IsFundamentalNumericType<T>::cond, T&>
      base_type_cast(T& v) noexcept {
    return v;
  }
  //! \brief cast the value to the base type
  template <typename T>
  TFEL_HOST_DEVICE constexpr std::
      enable_if_t<tfel::typetraits::IsFundamentalNumericType<T>::cond, const T&>
      base_type_cast(const T& v) noexcept {
    return v;
  }
  /*!
   * \brief an helper function to retrieve the space dimension associated
   * with a math object.
   * \tparam MathObjectType: math object
   */
  template <typename MathObjectType>
  TFEL_HOST_DEVICE constexpr unsigned short getSpaceDimension() {
    return MathObjectTraits<std::decay_t<MathObjectType>>::dime;
  }  // end of getSpaceDimension

  /*!
   * \brief an helper function around `isAssignableTo`
   * \tparam  A, first type
   * \tparam  B, second type
   */
  template <typename A, typename B>
  TFEL_HOST_DEVICE constexpr bool isAssignableTo() {
    return tfel::typetraits::isAssignableTo<A, B>();
  }  // end of isAssignableTo

  /*!
   * \brief a simple wrapper around `tfel::typetraits::IsInvalid`
   * \tparam T: tested type
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr auto isInvalid() {
    return tfel::typetraits::isInvalid<T>();
  }  // end of isInvalid

  /*!
   * \brief a simple wrapper around `tfel::typetraits::IsScalar`
   * \tparam T: tested type
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr auto isScalar() {
    return tfel::typetraits::isScalar<T>();
  }  // end of isScalar

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_GENERAL_MATHOBJECTTRAITS_HXX */
