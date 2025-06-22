/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyBroyden2Solver.ixx
 * \brief  This file implements the TinyNewtonMatrix
 * \author Thomas Helfer
 * \date   05/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYBROYDEN2SOLVER_IXX
#define LIB_TFEL_MATH_TINYBROYDEN2SOLVER_IXX

#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel::math {

  template <unsigned short N, typename NumericType, typename Child>
  void TinyBroyden2Solver<N, NumericType, Child>::updateOrCheckJacobian() {
    if (this->iter == 0) {
      return;
    }
    // updating jacobian invert
    const auto delta_fzeros = eval(this->fzeros - this->fzeros_1);
    const tvector<N, NumericType> c2 = this->inv_jacobian * delta_fzeros;
    const tvector<N, NumericType> c3 =
        (this->delta_zeros) * (this->inv_jacobian);
    const auto nc = c3 | delta_fzeros;
    if (tfel::math::ieee754::fpclassify(nc) != FP_ZERO) {
      this->inv_jacobian += ((this->delta_zeros - c2) ^ (c3)) / nc;
    }
  }  // end of updateOrCheckJacobian

  template <unsigned short N, typename NumericType, typename Child>
  bool TinyBroyden2Solver<N, NumericType, Child>::computeNewCorrection() {
    auto& child = static_cast<Child&>(*this);
    child.updateOrCheckJacobian();
    this->delta_zeros = -(this->inv_jacobian) * (this->fzeros);
    this->fzeros_1 = this->fzeros;
    return true;
  }  // end of computeNewCorrection

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYBROYDEN2SOLVER_IXX */
