/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyNonLinearSolverBase.ixx
 * \brief    
 * \author Thomas Helfer
 * \date   02/07/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_IXX
#define LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_IXX

#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyNonLinearSolverBase<N, NumericType, Child>::solveNonLinearSystem() {
    auto& child = static_cast<Child&>(*this);
    // newton correction
    auto converged = false;
    this->iter =
      typename TinyNonLinearSolverBase<N, NumericType,Child>::size_type{};
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
      const auto successful_evaluation = child.computeResidual();
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
        if (!child.computeNewCorrection()) {
          treat_invalid_residual();
          continue;
        }
        child.processNewCorrection();
        this->zeros += this->delta_zeros;
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


#endif /* LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_IXX */
