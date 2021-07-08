/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyLevenbergMarquardtSolver.ixx
 * \brief
 * \author Thomas Helfer
 * \date   06/07/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_IXX
#define LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_IXX

#include <cmath>
#include "TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  bool
  TinyLevenbergMarquardtSolver<N, NumericType, Child>::computeNewCorrection() {
    // matrix containing tJJ+levmar_mu*I
    tmatrix<N, N, NumericType> levmar_tJJ;
    // vector containing tJ*F
    tvector<N, NumericType> levmar_sm;
    for (unsigned short idx = 0; idx != N; ++idx) {
      levmar_sm(idx) = NumericType(0);
      for (unsigned short idx2 = 0; idx2 != N; ++idx2) {
        levmar_sm(idx) += (this->jacobian(idx2, idx)) * (this->fzeros(idx2));
        levmar_tJJ(idx, idx2) = NumericType(0);
        for (unsigned short idx3 = 0; idx3 != N; ++idx3) {
          levmar_tJJ(idx, idx2) +=
              (this->jacobian(idx3, idx)) * (this->jacobian(idx3, idx2));
        }
      }
    }
    const NumericType levmar_muF = (this->levmar_mu)*norm(this->fzeros);
    for (unsigned short idx = 0; idx != N; ++idx) {
      levmar_tJJ(idx, idx) += levmar_muF;
    }
    if (!TinyMatrixSolve<N, NumericType, false>::exe(levmar_tJJ, levmar_sm)) {
      return false;
    }
    this->delta_zeros = -levmar_sm;
    return true;
  }

  template <unsigned short N, typename NumericType, typename Child>
  void TinyLevenbergMarquardtSolver<N, NumericType, Child>::rejectCorrection() {
    this->fzeros = this->levmar_fzeros_1;
    this->jacobian = this->levmar_jacobian_1;
    this->levmar_error = this->levmar_error_1;
    this->zeros -= this->delta_zeros;
    // updating mu
    this->levmar_mu *= 4;
  }  // end of rejectCorrection

  template <unsigned short N, typename NumericType, typename Child>
  bool
  TinyLevenbergMarquardtSolver<N, NumericType, Child>::solveNonLinearSystem() {
    auto& child = static_cast<Child&>(*this);
    // dumping parameter
    this->iter = 0;
    if (!child.computeResidual()) {
      return false;
    }
    this->levmar_mu = this->levmar_mu0;
    this->levmar_error = norm(this->fzeros);
    bool converged = false;
    while ((converged == false) && (this->iter < this->iterMax)) {
      ++(this->iter);
      auto error = child.computeResidualNorm();
      const auto finite_error = ieee754::isfinite(error);
      if (!finite_error) {
        child.rejectCorrection();
        continue;
      }
      child.reportStandardNewtonIteration(error);
      converged = child.checkConvergence(error);
      if (!converged) {
        child.updateOrCheckJacobian();
        if (!child.computeNewCorrection()) {
          child.rejectCorrection();
          continue;
        }
        child.processNewCorrection();
        this->zeros += this->delta_zeros;
        child.processNewEstimate();
        this->levmar_fzeros_1 = this->fzeros;
        this->levmar_jacobian_1 = this->jacobian;
        if (!child.computeResidual()) {
          child.rejectCorrection();
          continue;
        }
        this->levmar_error_1 = levmar_error;
        const auto levmar_error2 = eval(levmar_jacobian_1 * this->delta_zeros);
        const auto error_p = norm(levmar_fzeros_1 + levmar_error2);
        this->levmar_error = norm(this->fzeros);
        const auto levmar_r =
            (levmar_error * levmar_error - levmar_error_1 * levmar_error_1) /
            (error_p * error_p - levmar_error_1 * levmar_error_1);
        if (levmar_r < this->levmar_p0) {
          // rejecting the step
          child.rejectCorrection();
          continue;
        }
        // accepting the step and updating mu
        error = levmar_error / (NumericType(N));
        if (levmar_r < this->levmar_p1) {
          levmar_mu *= 4;
        } else if (levmar_r > this->levmar_p2) {
          levmar_mu = std::max(levmar_mu / 4, this->levmar_m);
        }
      } // end of if(!converged)
    }
    if (this->iter == this->iterMax) {
      return false;
    }
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_IXX */
