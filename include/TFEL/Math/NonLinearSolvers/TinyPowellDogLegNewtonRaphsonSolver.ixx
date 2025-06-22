/*!
 * \file
 * include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegNewtonRaphsonSolver.ixx
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

#ifndef LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_IXX
#define LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_IXX

#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase.hxx"

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyPowellDogLegNewtonRaphsonSolver<N, NumericType, Child>::
      computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    child.updateOrCheckJacobian();
    const auto tjacobian = this->jacobian;
    const auto tfzeros = this->fzeros;
    if (!child.solveLinearSystem(this->jacobian, this->fzeros)) {
      return false;
    }
    this->delta_zeros = -this->fzeros;
    applyPowellDogLegAlgorithm(this->delta_zeros, tjacobian, tfzeros,
                               this->powell_dogleg_trust_region_size);
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_IXX */
