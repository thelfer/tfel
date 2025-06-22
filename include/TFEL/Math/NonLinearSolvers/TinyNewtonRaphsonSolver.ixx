/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyNewtonRaphsonSolver.ixx
 * \brief  This file implements the TinyNewtonMatrix
 * \author Thomas Helfer
 * \date   09 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_IXX
#define LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_IXX

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyNewtonRaphsonSolver<N, NumericType, Child>::computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    child.updateOrCheckJacobian();
    if (!child.solveLinearSystem(this->jacobian, this->fzeros)) {
      return false;
    }
    this->delta_zeros = -(this->fzeros);
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYNEWTONRAPHSONSOLVER_IXX */
