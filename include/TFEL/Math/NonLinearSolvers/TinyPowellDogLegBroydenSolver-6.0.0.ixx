/*!
 * \file
 * include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegBroydenSolver-6.0.0.ixx
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

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_6_0_0_IXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_6_0_0_IXX

#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase-6.0.0.hxx"

namespace tfel::math::v6_0_0 {

  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename>
            typename ExternalWorkSpace>
  TFEL_HOST_DEVICE void
  TinyPowellDogLegBroydenSolver<N, NumericType, Child, ExternalWorkSpace>::
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
  TFEL_HOST_DEVICE bool
  TinyPowellDogLegBroydenSolver<N, NumericType, Child, ExternalWorkSpace>::
      computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    // a rejected step usually updates the jacobian, as in the `hybrd` routine
    // of MINPACK: otherwise, no step could be accepted if the approximation of
    // the jacobian does not give a descent direction. However, the secant
    // equation would spoil the jacobian for a much larger residual, obtained
    // too far from the current estimate. This can't last as the radius is
    // reduced.
    constexpr auto maximal_residual_growth = NumericType(10);
    // qualified call: an unqualified one may be ambiguous through ADL
    const auto accepted =
        this->powell_dogleg_accept_correction ||
        tfel::math::v6_0_0::updatePowellDogLegTrustRegionRadius(
            this->powell_dogleg_radius, this->delta_zeros, this->jacobian,
            this->fzeros_1, this->fzeros);
    if (accepted ||
        (norm(this->fzeros) < maximal_residual_growth * norm(this->fzeros_1))) {
      this->updateJacobian();
      if (!this->callUpdateOrCheckJacobian()) {
        return false;
      }
    }
    auto tmp_jacobian = this->jacobian;
    auto tmp_fzeros = accepted ? this->fzeros : this->fzeros_1;
    if (!child.solveLinearSystem(tmp_jacobian, tmp_fzeros)) {
      return false;
    }
    if (accepted) {
      // if the convergence criterion is met, additional checks made by the
      // derived class changed the system: the next residual can't be compared
      // to the current one
      this->powell_dogleg_accept_correction =
          child.computeResidualNorm() < this->epsilon;
      this->fzeros_1 = this->fzeros;
    } else {
      // rejecting the step. This is done after the resolution of the linear
      // system, as `solveNonLinearSystem` halves the last step on failure
      child.rejectCurrentCorrection();
      this->zeros -= this->delta_zeros;
      this->fzeros = this->fzeros_1;
    }
    this->delta_zeros = -tmp_fzeros;
    // qualified call: an unqualified one may be ambiguous through ADL
    tfel::math::v6_0_0::applyPowellDogLegAlgorithm(this->delta_zeros,
                                                   this->jacobian, this->fzeros,
                                                   this->powell_dogleg_radius);
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math::v6_0_0

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGBROYDENSOLVER_6_0_0_IXX */
