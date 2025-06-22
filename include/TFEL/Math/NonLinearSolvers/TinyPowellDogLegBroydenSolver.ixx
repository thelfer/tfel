/*!
 * \file
 * include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegBroydenSolver.ixx
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

#ifndef LIB_TFEL_MATH_TINYBROYDENSOLVER_IXX
#define LIB_TFEL_MATH_TINYBROYDENSOLVER_IXX

#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase.hxx"

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  void TinyPowellDogLegBroydenSolver<N, NumericType, Child>::
      updateOrCheckJacobian() {
    if (this->iter == 0) {
      return;
    }
    const auto n = (this->delta_zeros | this->delta_zeros);
    if (tfel::math::ieee754::fpclassify(n) != FP_ZERO) {
      const tvector<N, NumericType> fzeros2 =
          this->jacobian * this->delta_zeros;
      this->jacobian +=
          ((this->fzeros - this->fzeros_1 - fzeros2) ^ (this->delta_zeros)) / n;
    }
  }  // end of updateOrCheckJacobian

  template <unsigned short N, typename NumericType, typename Child>
  bool
  TinyPowellDogLegBroydenSolver<N, NumericType, Child>::computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    child.updateOrCheckJacobian();
    auto tmp_jacobian = this->jacobian;
    auto tmp_fzeros = this->fzeros;
    if (!child.solveLinearSystem(tmp_jacobian, tmp_fzeros)) {
      return false;
    }
    this->delta_zeros = -tmp_fzeros;
    this->fzeros_1 = this->fzeros;
    applyPowellDogLegAlgorithm(this->delta_zeros, tmp_jacobian, tmp_fzeros,
                               this->powell_dogleg_trust_region_size);
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYBROYDENSOLVER_IXX */
