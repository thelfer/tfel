/*!
 * \file   FAnderson.ixx
 * \brief
 * \author Étienne Castelier
 * \date   10 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FANDERSON_IXX
#define LIB_TFEL_MATH_FANDERSON_IXX

#include <utility>

namespace tfel::math {

  template <typename Field, typename real>
  FAnderson<Field, real>::FAnderson(
      const typename AndersonBase<Field, real>::Allocator a)
      : AndersonBase<Field, real>(std::move(a)) {
  }  // end of FAnderson<Field,real>::FAnderson

  // Displacement fields for a new iteration
  // uO,uN: Old and new displacement field
  // Df: Rsidual force field
  template <typename Field, typename real>
  void FAnderson<Field, real>::newIter(Field*& uO, Field*& uN, Field*& Df) {
    // Reseting the Covariance matrix with a new vector
    if (this->n < this->size()) {
      ++(this->n);
    }
    this->reset();
    uO = uN;
    // Projection
    if (this->size() == this->n) {
      uN = this->u[0];
    } else {
      uN = this->u[this->n];
    }
    if (1 < this->n) {
      if (this->alt == this->alMax) {
        this->cM.weightsGSchmidtD(this->w);
        anderson::linear_combinaison(*uN, this->u, this->w, this->n);
        uO = uN;
        this->alt = 1;
      } else {
        if (this->alMax > 1) {
          ++(this->alt);
        }
      }
    }
    if (this->size() == this->n) {
      // Shift
      this->cM.shift();
      std::rotate(this->u.begin(), std::next(this->u.begin()), this->u.end());
      this->u[this->n - 1] = uN;
      Df = this->D[0];
      std::rotate(this->D.begin(), std::next(this->D.begin()), this->D.end());
      this->D[this->n - 1] = Df;
    } else {
      Df = this->D[this->n];
    }
  }

  // First Iteration of a new time step
  // uN: Old and new displacement field
  // Df: Residual force field
  template <typename Field, typename real>
  void FAnderson<Field, real>::restart(Field*& uO, Field*& uN, Field*& Df) {
    this->alloc();
    uO = uN = this->u[0];
    Df = this->D[0];
    this->alt = this->alMax;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FANDERSON_IXX */
