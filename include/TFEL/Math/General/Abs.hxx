/*!
 * \file   include/TFEL/Math/General/Abs.hxx
 * \brief  This file declares the abs function
 * \author Thomas Helfer
 * \date   13 Feb 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ABS_HXX
#define LIB_TFEL_MATH_ABS_HXX

#include <cmath>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/AbsType.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"

namespace tfel::math {

  /*!
   * \return the absolute value of a scalar
   * \param[in] s: value
   */
  template <typename NumericType>
  TFEL_HOST_DEVICE constexpr auto abs(const NumericType& s) noexcept
      requires(tfel::typetraits::isFundamentalNumericType<NumericType>()) {
    return (s < NumericType(0)) ? -s : s;
  }
  /*!
   * \return the norm of a complex
   * \param[in] s: value
   */
  template <typename NumericType>
  TFEL_HOST_DEVICE constexpr auto abs(const Complex<NumericType>& s) requires(
      tfel::typetraits::IsFundamentalNumericType<NumericType>::cond) {
    return s.norm();
  }

  /*!
   * \brief a basic functor accumulating the absolute value of a container
   */
  template <typename T>
  struct AbsSum {
    //! \brief a simple alias
    using argument_type = T;
    //! \brief a simple alias
    using result_type = void;
    //! \param [in] v : initial value
    TFEL_HOST_DEVICE constexpr AbsSum(const T& v = T())
        : result(v) {}  // end of AbsSum
    /*!
     * \brief add the absolute value of the argument to the result member
     */
    TFEL_HOST_DEVICE constexpr void operator()(const T& v) noexcept {
      result += abs(v);
    }
    //! \brief result
    T result;
  };  // end of struct AbsSum

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ABS_HXX */
