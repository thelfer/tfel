/*!
 * \file  mfront/src/CastemTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <algorithm>

#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "MFront/Castem/CastemTangentOperator.hxx"

namespace castem {

  void CastemTangentOperator::normalize(
      tfel::math::ST2toST2View<1u, CastemReal>&) {}

  void CastemTangentOperator::normalize(
      tfel::math::ST2toST2View<2u, CastemReal>& Dt) {
    constexpr const auto icste = tfel::math::Cste<CastemReal>::isqrt2;
    Dt(0, 3) *= icste;
    Dt(1, 3) *= icste;
    Dt(2, 3) *= icste;
    Dt(3, 0) *= icste;
    Dt(3, 1) *= icste;
    Dt(3, 2) *= icste;
    Dt(3, 3) *= 0.5;
  }  // end of CastemTangentOperator::normalize

  /*!
   * \brief normalize the tangent operator in 3D
   */
  void CastemTangentOperator::normalize(
      tfel::math::ST2toST2View<3u, CastemReal>& Dt) {
    constexpr const auto icste = tfel::math::Cste<CastemReal>::isqrt2;
    Dt(0, 3) *= icste;
    Dt(1, 3) *= icste;
    Dt(2, 3) *= icste;
    Dt(0, 4) *= icste;
    Dt(1, 4) *= icste;
    Dt(2, 4) *= icste;
    Dt(0, 5) *= icste;
    Dt(1, 5) *= icste;
    Dt(2, 5) *= icste;
    Dt(3, 0) *= icste;
    Dt(3, 1) *= icste;
    Dt(3, 2) *= icste;
    Dt(4, 0) *= icste;
    Dt(4, 1) *= icste;
    Dt(4, 2) *= icste;
    Dt(5, 0) *= icste;
    Dt(5, 1) *= icste;
    Dt(5, 2) *= icste;
    Dt(3, 3) *= 0.5;
    Dt(3, 4) *= 0.5;
    Dt(3, 5) *= 0.5;
    Dt(4, 3) *= 0.5;
    Dt(4, 4) *= 0.5;
    Dt(4, 5) *= 0.5;
    Dt(5, 3) *= 0.5;
    Dt(5, 4) *= 0.5;
    Dt(5, 5) *= 0.5;
  }  // end of struct CastemTangentOperator

  void CastemTangentOperator::transpose(
      tfel::math::ST2toST2View<1u, CastemReal>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
  }

  void CastemTangentOperator::transpose(
      tfel::math::ST2toST2View<2u, CastemReal>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(0, 3), Dt(3, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
    std::swap(Dt(1, 3), Dt(3, 1));
    std::swap(Dt(2, 3), Dt(3, 2));
  }

  void CastemTangentOperator::transpose(
      tfel::math::ST2toST2View<3u, CastemReal>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(0, 3), Dt(3, 0));
    std::swap(Dt(0, 4), Dt(4, 0));
    std::swap(Dt(0, 5), Dt(5, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
    std::swap(Dt(1, 3), Dt(3, 1));
    std::swap(Dt(1, 4), Dt(4, 1));
    std::swap(Dt(1, 5), Dt(5, 1));
    std::swap(Dt(2, 3), Dt(3, 2));
    std::swap(Dt(2, 4), Dt(4, 2));
    std::swap(Dt(2, 5), Dt(5, 2));
    std::swap(Dt(3, 4), Dt(4, 3));
    std::swap(Dt(3, 5), Dt(5, 3));
    std::swap(Dt(4, 5), Dt(5, 4));
  }

  void CastemTangentOperator::normalize(
      tfel::math::TMatrixView<1u, 1u, CastemReal>&) {
  }  // end of CastemTangentOperator::normalize

  void CastemTangentOperator::normalize(
      tfel::math::TMatrixView<2u, 2u, CastemReal>&) {
  }  // end of CastemTangentOperator::normalize

  void CastemTangentOperator::normalize(
      tfel::math::TMatrixView<3u, 3u, CastemReal>&) {
  }  // end of struct CastemTangentOperator

  void CastemTangentOperator::transpose(
      tfel::math::TMatrixView<1u, 1u, CastemReal>&) {}

  void CastemTangentOperator::transpose(
      tfel::math::TMatrixView<2u, 2u, CastemReal>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
  }

  void CastemTangentOperator::transpose(
      tfel::math::TMatrixView<3u, 3u, CastemReal>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(0, 3), Dt(3, 0));
  }

}  // end of namespace castem
