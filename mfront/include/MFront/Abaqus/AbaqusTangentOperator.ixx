/*!
 * \file   AbaqusTangentOperator.ixx
 * \brief
 * \author THOMAS HELFER
 * \date   04 avril 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_ABAQUSTANGENTOPERATOR_IXX_
#define LIB_ABAQUSTANGENTOPERATOR_IXX_

#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace abaqus {

  template <typename real>
  void AbaqusTangentOperator<real>::normalize(
      tfel::math::ST2toST2View<1u, real>&) {
  }  // end of AbaqusTangentOperator<real>::normalize

  template <typename real>
  void AbaqusTangentOperator<real>::normalize(
      tfel::math::ST2toST2View<2u, real>& Dt) {
    constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
    Dt(0, 3) *= icste;
    Dt(1, 3) *= icste;
    Dt(2, 3) *= icste;
    Dt(3, 0) *= icste;
    Dt(3, 1) *= icste;
    Dt(3, 2) *= icste;
    Dt(3, 3) /= real(2);
  }  // end of AbaqusTangentOperator<real>::normalize

  template <typename real>
  void AbaqusTangentOperator<real>::normalize(
      tfel::math::ST2toST2View<3u, real>& Dt) {
    constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
    TFEL_CONSTEXPR const auto one_half = real(1) / real(2);
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
    Dt(3, 3) *= one_half;
    Dt(3, 4) *= one_half;
    Dt(3, 5) *= one_half;
    Dt(4, 3) *= one_half;
    Dt(4, 4) *= one_half;
    Dt(4, 5) *= one_half;
    Dt(5, 3) *= one_half;
    Dt(5, 4) *= one_half;
    Dt(5, 5) *= one_half;
  }  // end of struct AbaqusTangentOperator

  template <typename real>
  void AbaqusTangentOperator<real>::transpose(
      tfel::math::ST2toST2View<1u, real>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
  }

  template <typename real>
  void AbaqusTangentOperator<real>::transpose(
      tfel::math::ST2toST2View<2u, real>& Dt) {
    std::swap(Dt(0, 1), Dt(1, 0));
    std::swap(Dt(0, 2), Dt(2, 0));
    std::swap(Dt(0, 3), Dt(3, 0));
    std::swap(Dt(1, 2), Dt(2, 1));
    std::swap(Dt(1, 3), Dt(3, 1));
    std::swap(Dt(2, 3), Dt(3, 2));
  }

  template <typename real>
  void AbaqusTangentOperator<real>::transpose(
      tfel::math::ST2toST2View<3u, real>& Dt) {
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

}  // end of namespace abaqus

#endif /* LIB_ABAQUSTANGENTOPERATOR_IXX_ */
