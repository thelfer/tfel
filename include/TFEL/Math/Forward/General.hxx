/*!
 * \file   TFEL/Math/Forward/General.hxx
 * \brief
 * \author Thomas Helfer
 * \date 29/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_GENERAL_HXX
#define LIB_TFEL_MATH_FORWARD_GENERAL_HXX

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"

namespace tfel::math {

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

  template <typename T>
  concept StandardArithmeticTypeConcept =
      tfel::typetraits::StandardArithmeticTypeConcept<T>;

  template <typename T>
  concept FundamentalNumericTypeConcept =
      tfel::typetraits::FundamentalNumericTypeConcept<T>;

  template <typename T>
  concept ScalarConcept = tfel::typetraits::IsScalar<std::decay_t<T>>::cond;

  template <typename T>
  concept MutableScalarConcept = ScalarConcept<T> &&(!std::is_const_v<T>);

  template <typename T>
  struct BaseType {
    //! \brief result of the metafunction
    using type = tfel::meta::InvalidType;
  };

  template <ScalarConcept T>
  struct BaseType<T> {
    //! \brief result of the metafunction
    using type = tfel::typetraits::base_type<T>;
  };

  template <typename T>
  using base_type = typename BaseType<T>::type;

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_GENERAL_HXX */
