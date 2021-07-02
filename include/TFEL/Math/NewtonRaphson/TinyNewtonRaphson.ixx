/*!
 * \file   include/TFEL/Math/Newton-Raphson/TinyNewtonRaphson.ixx
 * \brief  This file implements the TinyNewtonMatrix
 * \author Thomas Helfer
 * \date   09 Aug 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYNEWTONRAPHSON_IXX
#define LIB_TFEL_MATH_TINYNEWTONRAPHSON_IXX

#include <cmath>
#include <limits>
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyNewtonRaphson<N, NumericType, Child>::solveNonLinearSystem() {
    auto& child = static_cast<Child&>(*this);
    // newton correction
    auto converged = false;
    this->iter = size_type{};
    child.reportBeginningOfResolution();
    child.processNewEstimate();
    auto treat_invalid_residual = [this, &child] {
      constexpr auto one_half = NumericType(1) / 2;
      child.reportInvalidResidualEvaluation();
      this->delta_zeros *= one_half;
      this->zeros -= this->delta_zeros;
      child.processNewEstimate();
      ++(this->iter);
    };
    while ((!converged) && (this->iter != this->iterMax)) {
      const auto successful_evaluation = child.computeResidualAndJacobian();
      const auto error = [&successful_evaluation, &child] {
        if (successful_evaluation) {
          return child.computeResidualNorm();
        }
        return NumericType{};
      }();
      const auto finite_error = ieee754::isfinite(error);
      if ((!successful_evaluation) || (!finite_error)) {
        treat_invalid_residual();
        continue;
      }
      child.reportStandardNewtonIteration(error);
      converged = child.checkConvergence(error);
      if (!converged) {
        child.updateOrCheckJacobian();
        const auto linear_solver_success =
            TinyMatrixSolve<N, NumericType, false>::exe(this->jacobian,
                                                        this->fzeros);
        if (!linear_solver_success) {
          treat_invalid_residual();
          continue;
        }
        delta_zeros = -(this->fzeros);
        child.processNewCorrection();
        this->zeros += delta_zeros;
        child.processNewEstimate();
        ++(this->iter);
      }
    }
    if (converged) {
      child.reportSuccess();
    } else {
      child.reportFailure();
    }
    return converged;
  }  // end of solve

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYNEWTONRAPHSON_IXX */
