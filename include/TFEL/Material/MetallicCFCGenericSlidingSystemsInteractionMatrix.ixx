/*!
 * \file
 * include/TFEL/Material/MetallicCFCGenericSlidingSystemsInteractionMatrix.ixx
 * \brief
 * \author Thomas Helfer/Proix Jean-Michel
 * \brief 25 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

namespace tfel {

  namespace material {

    template <typename NumType>
    MetallicCFCGenericSlidingSystemsInteractionMatrix<NumType>::
        MetallicCFCGenericSlidingSystemsInteractionMatrix(const NumType h1,
                                                          const NumType h2,
                                                          const NumType h3,
                                                          const NumType h4,
                                                          const NumType h5,
                                                          const NumType h6) {
      using namespace tfel::math;
      for (typename tmatrix<12u, 12u, real>::size_type i = 0; i != 12; ++i) {
        this->m(i, i) = h1;
      }
      this->m(1, 0) = this->m(0, 1) = h2;
      this->m(2, 0) = this->m(0, 2) = h2;
      this->m(2, 1) = this->m(1, 2) = h2;
      this->m(3, 0) = this->m(0, 3) = h4;
      this->m(3, 1) = this->m(1, 3) = h5;
      this->m(3, 2) = this->m(2, 3) = h5;
      this->m(4, 0) = this->m(0, 4) = h5;
      this->m(4, 1) = this->m(1, 4) = h3;
      this->m(4, 2) = this->m(2, 4) = h6;
      this->m(4, 3) = this->m(3, 4) = h2;
      this->m(5, 0) = this->m(0, 5) = h5;
      this->m(5, 1) = this->m(1, 5) = h6;
      this->m(5, 2) = this->m(2, 5) = h3;
      this->m(5, 3) = this->m(3, 5) = h2;
      this->m(5, 4) = this->m(4, 5) = h2;
      this->m(6, 0) = this->m(0, 6) = h5;
      this->m(6, 1) = this->m(1, 6) = h4;
      this->m(6, 2) = this->m(2, 6) = h5;
      this->m(6, 3) = this->m(3, 6) = h6;
      this->m(6, 4) = this->m(4, 6) = h3;
      this->m(6, 5) = this->m(5, 6) = h5;
      this->m(7, 0) = this->m(0, 7) = h6;
      this->m(7, 1) = this->m(1, 7) = h5;
      this->m(7, 2) = this->m(2, 7) = h3;
      this->m(7, 3) = this->m(3, 7) = h5;
      this->m(7, 4) = this->m(4, 7) = h5;
      this->m(7, 5) = this->m(5, 7) = h4;
      this->m(7, 6) = this->m(6, 7) = h2;
      this->m(8, 0) = this->m(0, 8) = h3;
      this->m(8, 1) = this->m(1, 8) = h5;
      this->m(8, 2) = this->m(2, 8) = h6;
      this->m(8, 3) = this->m(3, 8) = h3;
      this->m(8, 4) = this->m(4, 8) = h6;
      this->m(8, 5) = this->m(5, 8) = h5;
      this->m(8, 6) = this->m(6, 8) = h2;
      this->m(8, 7) = this->m(7, 8) = h2;
      this->m(9, 0) = this->m(0, 9) = h5;
      this->m(9, 1) = this->m(1, 9) = h5;
      this->m(9, 2) = this->m(2, 9) = h4;
      this->m(9, 3) = this->m(3, 9) = h6;
      this->m(9, 4) = this->m(4, 9) = h5;
      this->m(9, 5) = this->m(5, 9) = h3;
      this->m(9, 6) = this->m(6, 9) = h6;
      this->m(9, 7) = this->m(7, 9) = h3;
      this->m(9, 8) = this->m(8, 9) = h5;
      this->m(10, 0) = this->m(0, 10) = h3;
      this->m(10, 1) = this->m(1, 10) = h6;
      this->m(10, 2) = this->m(2, 10) = h5;
      this->m(10, 3) = this->m(3, 10) = h3;
      this->m(10, 4) = this->m(4, 10) = h5;
      this->m(10, 5) = this->m(5, 10) = h6;
      this->m(10, 6) = this->m(6, 10) = h5;
      this->m(10, 7) = this->m(7, 10) = h5;
      this->m(10, 8) = this->m(8, 10) = h4;
      this->m(10, 9) = this->m(9, 10) = h2;
      this->m(11, 0) = this->m(0, 11) = h6;
      this->m(11, 1) = this->m(1, 11) = h3;
      this->m(11, 2) = this->m(2, 11) = h5;
      this->m(11, 3) = this->m(3, 11) = h5;
      this->m(11, 4) = this->m(4, 11) = h4;
      this->m(11, 5) = this->m(5, 11) = h5;
      this->m(11, 6) = this->m(6, 11) = h3;
      this->m(11, 7) = this->m(7, 11) = h6;
      this->m(11, 8) = this->m(8, 11) = h5;
      this->m(11, 9) = this->m(9, 11) = h2;
      this->m(11, 10) = this->m(10, 11) = h2;
    }  // end of
       // MetallicCFCGenericSlidingSystemsInteractionMatrix::MetallicCFCGenericSlidingSystemsInteractionMatrix

    template <typename NumType>
    const tfel::math::tmatrix<12, 12, NumType>&
    MetallicCFCGenericSlidingSystemsInteractionMatrix<
        NumType>::getInteractionMatrix(const NumType h1,
                                       const NumType h2,
                                       const NumType h3,
                                       const NumType h4,
                                       const NumType h5,
                                       const NumType h6) {
      static MetallicCFCGenericSlidingSystemsInteractionMatrix s(h1, h2, h3, h4,
                                                                 h5, h6);
      return s.m;
    }  // end of
       // MetallicCFCGenericSlidingSystemsInteractionMatrix::getInteractionMatrix

    template <typename NumType>
    const tfel::math::tmatrix<12, 12, NumType>&
    MetallicCFCGenericSlidingSystemsInteractionMatrix<
        NumType>::getSecondInteractionMatrix(const NumType h1,
                                             const NumType h2,
                                             const NumType h3,
                                             const NumType h4,
                                             const NumType h5,
                                             const NumType h6) {
      static MetallicCFCGenericSlidingSystemsInteractionMatrix s2(h1, h2, h3,
                                                                  h4, h5, h6);
      return s2.m;
    }  // end of
       // MetallicCFCGenericSlidingSystemsInteractionMatrix::getInteractionMatrix

  }  // end of namespace material

}  // end of namespace tfel
