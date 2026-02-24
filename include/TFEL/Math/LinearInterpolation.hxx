/*!
 * \file   include/TFEL/Math/LinearInterpolation.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27/07/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LINEARINTERPOLATION_HXX
#define LIB_TFEL_MATH_LINEARINTERPOLATION_HXX

namespace tfel::math {

  /*!
   * \brief compute a linear interpolation based on the given abscissae and
   * values
   * \param[in] abscissae: container containing the abscissae
   * \param[in] values: container containing the values
   * \param[in] a: point where the interpolation is computed
   *
   * \tparam extrapolate: boolean stating if extrapolation must be performed
   *
   * \note the abscissae are assumed ordered from lower to greater values.
   */
  template <bool extrapolate,
            typename AbscissaContainer,
            typename ValueContainer,
            typename AbscissaType>
  constexpr auto computeLinearInterpolation(const AbscissaContainer&,
                                            const ValueContainer&,
                                            const AbscissaType);

  /*!
   * \brief compute a linear interpolation based on the given abscissae and
   * values and the associated derivative
   * \param[in] abscissae: container containing the abscissae
   * \param[in] values: container containing the values
   * \param[in] a: point where the interpolation is computed
   *
   * \tparam extrapolate: boolean stating if extrapolation must be performed
   *
   * \note the abscissae are assumed ordered from lower to greater values.
   */
  template <bool extrapolate,
            typename AbscissaContainer,
            typename ValueContainer,
            typename AbscissaType>
  constexpr auto computeLinearInterpolationAndDerivative(
      const AbscissaContainer&, const ValueContainer&, const AbscissaType);

}  // end of namespace tfel::math

#include "TFEL/Math/LinearInterpolation.ixx"

#endif /* LIB_TFEL_MATH_LINEARINTERPOLATION_HXX */
