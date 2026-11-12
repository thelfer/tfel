/*!
 * \file
 * include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegNewtonRaphsonSolver-5.2.0.ixx
 * \brief  This file implements the TinyPowellDogLegNewtonMatrix class
 * \author Thomas Helfer
 * \date   02/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_5_2_0_IXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_5_2_0_IXX

#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase-5.2.0.hxx"

namespace tfel::math::v5_2_0 {

  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename>
            typename ExternalWorkSpace>
  bool TinyPowellDogLegNewtonRaphsonSolver<
      N,
      NumericType,
      Child,
      ExternalWorkSpace>::computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    if (!this->callUpdateOrCheckJacobian()) {
      return false;
    }
    const auto tjacobian = this->jacobian;
    const auto tfzeros = this->fzeros;
    if (!child.solveLinearSystem(this->jacobian, this->fzeros)) {
      return false;
    }
    this->delta_zeros = -this->fzeros;
    // qualified call: an unqualified one may be ambiguous through ADL
    tfel::math::v5_2_0::applyPowellDogLegAlgorithm(
        this->delta_zeros, tjacobian, tfzeros,
        this->powell_dogleg_trust_region_size);
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math::v5_2_0

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_5_2_0_IXX */
