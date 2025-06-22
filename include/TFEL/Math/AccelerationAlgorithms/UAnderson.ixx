/*!
 * \file   UAnderson.ixx
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

#ifndef LIB_TFEL_MATH_UANDERSON_IXX
#define LIB_TFEL_MATH_UANDERSON_IXX

#include <utility>

namespace tfel::math {

  //! constructor
  template <typename Field, typename real>
  UAnderson<Field, real>::UAnderson(
      const typename AndersonBase<Field, real>::Allocator a)
      : AndersonBase<Field, real>(std::move(a)) {
  }  // end of UAnderson<Field,real>::UAnderson

  // Displacement fields for a new iteration
  // uO,uN: Old and new displacement field
  template <typename Field, typename real>
  void UAnderson<Field, real>::newIter(Field*& uO, Field*& uN) {
    using size_type = typename AndersonBase<Field, real>::size_type;
    // Matrice upgrade
    if (this->n < this->size()) {
      ++(this->n);
    }
    *uO -= *uN;
    this->reset();
    // Projection
    if (this->n < this->size()) {
      uO = this->D[this->n];
    } else {
      uO = this->D[0];
    }
    if ((1 < this->n) && (this->alt == this->alMax)) {
      this->cM.weightsGSchmidtD(this->w);
      anderson::linear_combinaison(*uO, this->u, this->w, this->n);
      this->alt = 1;
    } else {
      *uO = *uN;
      if (this->alt < this->alMax) {
        ++(this->alt);
      }
    }
    // Shift
    if (this->n == this->size()) {
      this->cM.shift();
      uN = this->u[0];
      for (size_type i = 0; i < this->n - 1; ++i) {
        this->u[i] = this->u[i + 1];
      }
      this->u[this->n - 1] = uN;
      for (size_type i = 0; i < this->n - 1; ++i) {
        this->D[i] = this->D[i + 1];
      }
      this->D[this->n - 1] = uO;
    } else {
      uN = this->u[this->n];
    }
  }

  // First Iteration of a new time step
  // uO,uN: Old and new displacement field
  template <typename Field, typename real>
  void UAnderson<Field, real>::restart(Field*& uO, Field*& uN) {
    this->alloc();
    uO = this->D[0];
    uN = this->u[0];
    this->alt = this->alMax;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_UANDERSON_IXX */
