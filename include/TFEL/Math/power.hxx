/*!
 * \file  include/TFEL/Math/power.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 13/01/2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_POWER_HXX
#define LIB_TFEL_MATH_POWER_HXX

#include <concepts>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"

namespace tfel::math {

  //! \brief an help structure used to specialize the UnaryResultType class
  template <int N, unsigned int D = 1>
  struct Power;
  /*!
   * \brief partial specialisation for the UnaryResultType class
   * for exponentation of floating point number using rational
   * exponent
   * \tparam N:
   * \tparam D:
   */
  template <int N, unsigned int D>
  requires(D != 0) struct UnaryResultType<float, Power<N, D>> {
    using type = float;
  };
  /*!
   * \brief partial specialisation for the UnaryResultType class
   * for exponentation of floating point number using rational
   * exponent
   * \tparam N:
   * \tparam D:
   */
  template <int N, unsigned int D>
  requires(D != 0) struct UnaryResultType<double, Power<N, D>> {
    using type = double;
  };
  /*!
   * \brief partial specialisation for the UnaryResultType class
   * for exponentation of floating point number using rational
   * exponent
   * \tparam N:
   * \tparam D:
   */
  template <int N, unsigned int D>
  requires(D != 0) struct UnaryResultType<long double, Power<N, D>> {
    using type = long double;
  };
  /*!
   * \brief computes x to the power N
   * \tparam N: exponent
   * \param x: value
   */
  template <int N>
  TFEL_HOST_DEVICE auto power(const std::floating_point auto) noexcept;
  /*!
   * \brief computes x to the power N/D
   * \tparam N: exponent numerator
   * \tparam M: exponent denumerator
   * \param x: value
   */
  template <int N, unsigned int D, typename T>
  TFEL_HOST_DEVICE auto power(const std::floating_point auto) noexcept  //
      requires(D != 0);

}  // end of namespace tfel::math

#include "TFEL/Math/power.ixx"

#endif /* LIB_TFEL_MATH_POWER_HXX */
