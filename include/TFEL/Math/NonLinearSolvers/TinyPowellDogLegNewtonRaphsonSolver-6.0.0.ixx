/*!
 * \file
 * include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegNewtonRaphsonSolver-6.0.0.ixx
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

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_6_0_0_IXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_6_0_0_IXX

#include "TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase-6.0.0.hxx"

namespace tfel::math::v6_0_0 {

  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename>
            typename ExternalWorkSpace>
  TFEL_HOST_DEVICE bool TinyPowellDogLegNewtonRaphsonSolver<
      N,
      NumericType,
      Child,
      ExternalWorkSpace>::computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    // qualified call: an unqualified one may be ambiguous through ADL
    if (this->powell_dogleg_accept_correction ||
        tfel::math::v6_0_0::updatePowellDogLegTrustRegionRadius(
            this->powell_dogleg_radius, this->delta_zeros,
            this->powell_dogleg_jacobian_1, this->powell_dogleg_fzeros_1,
            this->fzeros)) {
      // accepting the step
      if (!this->callUpdateOrCheckJacobian()) {
        return false;
      }
      // if the convergence criterion is met, additional checks made by the
      // derived class changed the system: the next residual can't be compared
      // to the current one
      this->powell_dogleg_accept_correction =
          child.computeResidualNorm() < this->epsilon;
      this->powell_dogleg_jacobian_1 = this->jacobian;
      this->powell_dogleg_fzeros_1 = this->fzeros;
    } else {
      // rejecting the step
      child.rejectCurrentCorrection();
      this->zeros -= this->delta_zeros;
      this->jacobian = this->powell_dogleg_jacobian_1;
      this->fzeros = this->powell_dogleg_fzeros_1;
    }
    if (!child.solveLinearSystem(this->jacobian, this->fzeros)) {
      return false;
    }
    this->delta_zeros = -this->fzeros;
    // qualified call: an unqualified one may be ambiguous through ADL
    tfel::math::v6_0_0::applyPowellDogLegAlgorithm(
        this->delta_zeros, this->powell_dogleg_jacobian_1,
        this->powell_dogleg_fzeros_1, this->powell_dogleg_radius);
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math::v6_0_0

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGNEWTONRAPHSONSOLVER_6_0_0_IXX */
