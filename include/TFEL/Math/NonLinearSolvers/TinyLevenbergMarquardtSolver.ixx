/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyLevenbergMarquardtSolver.ixx
 * \brief
 * \author Thomas Helfer
 * \date   06/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_IXX
#define LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_IXX

#include <cmath>

namespace tfel::math {

  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename>
            typename ExternalWorkSpace>
  bool TinyLevenbergMarquardtSolver<N, NumericType, Child, ExternalWorkSpace>::
      computeLevenbergMarquardtCorrection() {
    auto& child = static_cast<Child&>(*this);
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
    const NumericType levmar_muF = (this->levmar_mu) * norm(this->fzeros);
    for (unsigned short idx = 0; idx != N; ++idx) {
      levmar_tJJ(idx, idx) += levmar_muF;
    }
    if (!child.solveLinearSystem(levmar_tJJ, levmar_sm)) {
      return false;
    }
    this->delta_zeros = -levmar_sm;
    return true;
  }  // end of computeLevenbergMarquardtCorrection

  template <unsigned short N,
            typename NumericType,
            typename Child,
            template <unsigned short, typename>
            typename ExternalWorkSpace>
  bool TinyLevenbergMarquardtSolver<N, NumericType, Child, ExternalWorkSpace>::
      computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    this->levmar_error = norm(this->fzeros);
    if (!this->levmar_first) {
      const auto levmar_error2 =
          eval(this->levmar_jacobian_1 * this->delta_zeros);
      const auto error_p = norm(this->levmar_fzeros_1 + levmar_error2);
      const auto levmar_r =
          (this->levmar_error * this->levmar_error -
           this->levmar_error_1 * this->levmar_error_1) /
          (error_p * error_p - this->levmar_error_1 * this->levmar_error_1);
      if (levmar_r < this->levmar_p0) {
        // rejecting the step
        this->levmar_mu *= 4;
        this->zeros -= this->delta_zeros;
        this->fzeros = this->levmar_fzeros_1;
        this->jacobian = this->levmar_jacobian_1;
        this->levmar_error = this->levmar_error_1;
        if (!child.computeLevenbergMarquardtCorrection()) {
          return false;
        }
      } else {
        // accepting the step and updating mu
        if (levmar_r < this->levmar_p1) {
          this->levmar_mu *= 4;
        } else if (levmar_r > this->levmar_p2) {
          this->levmar_mu = std::max(this->levmar_mu / 4, this->levmar_m);
        }
        child.updateOrCheckJacobian();
        if (!child.computeLevenbergMarquardtCorrection()) {
          return false;
        }
      }
    } else {
      child.updateOrCheckJacobian();
      if (!child.computeLevenbergMarquardtCorrection()) {
        return false;
      }
      this->levmar_first = false;
    }
    this->levmar_error_1 = this->levmar_error;
    this->levmar_fzeros_1 = this->fzeros;
    this->levmar_jacobian_1 = this->jacobian;
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYLEVENBERGMARQUARDTSOLVER_IXX */
