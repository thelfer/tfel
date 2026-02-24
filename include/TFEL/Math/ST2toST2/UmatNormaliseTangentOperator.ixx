/*!
 * \file   UmatNormaliseTangentOperator.ixx
 * \brief
 * \author Thomas Helfer
 * \date   23 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_UMATNORMALISETANGENTOPERATORIXX
#define LIB_TFEL_MATH_UMATNORMALISETANGENTOPERATORIXX

#include <cmath>
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel::math {

  template <typename stress>
  void UmatNormaliseTangentOperatorBase<1u, stress>::exe(
      stress* const K, const stress* const D) {
    ST2toST2View<1u, stress> k(K);
    ConstST2toST2View<1u, stress> d(D);
    // transpose
    for (unsigned short i = 0; i != 3u; ++i) {
      for (unsigned short j = 0; j != 3u; ++j) {
        k(i, j) = d(i, j);
      }
    }
  }

  template <typename stress>
  void UmatNormaliseTangentOperatorBase<2u, stress>::exe(
      stress* const K, const stress* const D) {
    constexpr auto cste = Cste<stress>::sqrt2;
    ST2toST2View<2u, stress> k(K);
    ConstST2toST2View<2u, stress> d(D);
    // transpose
    for (unsigned short i = 0; i != 4u; ++i) {
      for (unsigned short j = 0; j != 4u; ++j) {
        k(i, j) = d(j, i);
      }
    }
    k(0, 3) *= cste;
    k(1, 3) *= cste;
    k(2, 3) *= cste;
    k(3, 0) *= cste;
    k(3, 1) *= cste;
    k(3, 2) *= cste;
    k(3, 3) *= 2;
  }

  template <typename stress>
  void UmatNormaliseTangentOperatorBase<3u, stress>::exe(
      stress* const K, const stress* const D) {
    constexpr auto cste = Cste<stress>::sqrt2;
    ST2toST2View<3u, stress> k(K);
    ConstST2toST2View<3u, stress> d(D);
    // transpose
    for (unsigned short i = 0; i != 6u; ++i) {
      for (unsigned short j = 0; j != 6u; ++j) {
        k(i, j) = d(j, i);
      }
    }
    k(0, 3) *= cste;
    k(1, 3) *= cste;
    k(2, 3) *= cste;
    k(0, 4) *= cste;
    k(1, 4) *= cste;
    k(2, 4) *= cste;
    k(0, 5) *= cste;
    k(1, 5) *= cste;
    k(2, 5) *= cste;
    k(3, 0) *= cste;
    k(3, 1) *= cste;
    k(3, 2) *= cste;
    k(4, 0) *= cste;
    k(4, 1) *= cste;
    k(4, 2) *= cste;
    k(5, 0) *= cste;
    k(5, 1) *= cste;
    k(5, 2) *= cste;
    k(3, 3) *= 2;
    k(3, 4) *= 2;
    k(3, 5) *= 2;
    k(4, 3) *= 2;
    k(4, 4) *= 2;
    k(4, 5) *= 2;
    k(5, 3) *= 2;
    k(5, 4) *= 2;
    k(5, 5) *= 2;
  }

  template <unsigned short N, typename stress>
  st2tost2<N, stress> UmatNormaliseTangentOperator<N, stress>::exe(
      const stress* const D) {
    st2tost2<N, stress> r;
    UmatNormaliseTangentOperatorBase<N, stress>::exe(&r(0, 0), D);
    return r;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_UMATNORMALISETANGENTOPERATORIXX */
