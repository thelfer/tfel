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
#include "TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyNonLinearSolverBase<N, NumericType, Child>::solveNonLinearSystem2() {
    auto& child = static_cast<Child&>(*this);
    auto converged = false;
    child.executeInitialisationTaskBeforeBeginningOfCoreAlgorithm();
    while (true) {
      if (!child.computeResidual()) {
        child.reportInvalidResidualEvaluation();
        return false;
      }
      const auto error = child.computeResidualNorm();
      const auto finite_error = ieee754::isfinite(error);
      if (!finite_error) {
        child.reportInvalidResidualEvaluation();
        return false;
      }
      child.reportStandardIteration(error);
      converged = child.checkConvergence(error);
      if (converged) {
        return true;
      }
      if (!child.computeNewCorrection()) {
        child.reportNewCorrectionComputationFailure();
        break;
      }
      this->is_delta_zeros_defined = true;
      child.processNewCorrection();
      this->zeros += this->delta_zeros;
      child.processNewEstimate();
      ++(this->iter);
      if (this->iter == this->iterMax) {
        break;
      }
    }
    return false;
  }  // end of solveNonLinearSystem2

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyNonLinearSolverBase<N, NumericType, Child>::solveNonLinearSystem() {
    constexpr auto one_half = NumericType(1) / 2;
    auto& child = static_cast<Child&>(*this);
    child.reportBeginningOfResolution();
    this->iter =
      typename TinyNonLinearSolverBase<N, NumericType,Child>::size_type{};
    this->is_delta_zeros_defined = false;
    child.executeInitialisationTaskBeforeResolution();
    while (this->iter != this->iterMax) {
      child.processNewEstimate();
      if (child.solveNonLinearSystem2()) {
        child.reportSuccess();
        return true;
      }
      if (this->iter == this->iterMax) {
        break;
      }
      if (this->is_delta_zeros_defined) {
        this->delta_zeros *= one_half;
        this->zeros -= this->delta_zeros;
      } else {
        this->zeros *= one_half;
      }
      ++(this->iter);
    }
    child.reportFailure();
    return false;
  }  // end of solve

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyNonLinearSolverBase<N, NumericType, Child>::solveLinearSystem(
      tfel::math::tmatrix<N, N, NumericType>& m,
      tfel::math::tvector<N, NumericType>& v) const noexcept {
    return TinyMatrixSolve<N, NumericType, false>::exe(m, v);
  }  // end of solveLinearSystem
 
}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_NONLINEARSOLVERS_TINYNONLINEARSOLVERBASE_IXX */