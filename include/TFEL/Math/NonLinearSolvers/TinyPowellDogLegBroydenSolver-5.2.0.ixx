/*!
 * \file
 * include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegBroydenSolver-5.2.0.ixx
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

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_5_2_0_IXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_5_2_0_IXX

#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase-5.2.0.hxx"

namespace tfel::math::v5_2_0 {

  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename>
            typename ExternalWorkSpace>
  void TinyPowellDogLegBroydenSolver<N, NumericType, Child, ExternalWorkSpace>::
      updateJacobian() {
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
  }  // end of updateJacobian

  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename>
            typename ExternalWorkSpace>
  bool TinyPowellDogLegBroydenSolver<N, NumericType, Child, ExternalWorkSpace>::
      computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    this->updateJacobian();
    if (!this->callUpdateOrCheckJacobian()) {
      return false;
    }
    auto tmp_jacobian = this->jacobian;
    auto tmp_fzeros = this->fzeros;
    if (!child.solveLinearSystem(tmp_jacobian, tmp_fzeros)) {
      return false;
    }
    this->delta_zeros = -tmp_fzeros;
    this->fzeros_1 = this->fzeros;
    // qualified call: an unqualified one may be ambiguous through ADL
    tfel::math::v5_2_0::applyPowellDogLegAlgorithm(
        this->delta_zeros, this->jacobian, this->fzeros,
        this->powell_dogleg_trust_region_size);
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math::v5_2_0

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_5_2_0_IXX */
