/*!
 * \file   include/TFEL/Math/General/Compare.hxx
 * \brief
 * \author Antoine Martin
 * \date   05/09/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_COMPARE_HXX
#define LIB_TFEL_MATH_COMPARE_HXX

#include<cmath>
#include<concepts>
#include"TFEL/Math/qt.hxx"

namespace tfel::math {

  /*
   * \brief compare two values with a precision
   */
  template <std::floating_point ScalarType>
  TFEL_HOST_DEVICE constexpr bool areAlmostEqual(const ScalarType& eps,
						 const ScalarType& a,
						 const ScalarType& b) {
    return std::abs(a - b) < eps;
  }

   /*
   * \brief compare two quantities with a precision
   */
  template <std::floating_point ScalarType, UnitConcept UnitType>
  TFEL_HOST_DEVICE constexpr bool areAlmostEqual(
      const ScalarType& eps,
      const qt<UnitType, ScalarType>& a,
      const qt<UnitType, ScalarType>& b) {
    return std::abs(a.getValue() - b.getValue()) < eps;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_COMPARE_HXX */
