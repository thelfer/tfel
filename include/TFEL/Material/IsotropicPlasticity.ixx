/*!
 * \file   include/TFEL/Material/IsotropicPlasticity.ixx
 * \brief This files implements the function declared in
 * `include/TFEL/Material/IsotropicPlasticity.ixx`.
 * \author Thomas Helfer
 * \date   14/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_IXX
#define LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_IXX

namespace tfel::material {

  TFEL_HOST_DEVICE constexpr auto computeJ3Derivative(
      const tfel::math::StensorConcept auto& s) {
    return tfel::math::computeDeviatorDeterminantDerivative(s);
  }  // end of computeJ3Derivative

  TFEL_HOST_DEVICE constexpr auto computeJ3SecondDerivative(
      const tfel::math::StensorConcept auto& s) {
    return tfel::math::computeDeviatorDeterminantSecondDerivative(s);
  }  // end of computeJ3SecondDerivative

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_IXX */
