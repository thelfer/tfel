/*!
 * \file  mfront/src/AbaqusStressFreeExpansionHandler.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 04 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx"

namespace abaqus {

  template <typename real>
  static void AbaqusStandardSmallStrainStressFreeExpansionHandlerImpl(
      real *const e,
      real *const de,
      const real *const s0,
      const real *const s1,
      const AbaqusInt d) {
    constexpr auto cste = tfel::math::Cste<real>::sqrt2;
    e[0] -= s0[0];
    e[1] -= s0[1];
    e[2] -= s0[2];
    de[0] -= (s1[0] - s0[0]);
    de[1] -= (s1[1] - s0[1]);
    de[2] -= (s1[2] - s0[2]);
    if (d == 2) {
      e[3] -= s0[3] * cste;
      de[3] -= (s1[3] - s0[3]) * cste;
    }
    if (d == 3) {
      e[3] -= s0[3] * cste;
      e[4] -= s0[3] * cste;
      e[5] -= s0[5] * cste;
      de[3] -= (s1[3] - s0[3]) * cste;
      de[4] -= (s1[4] - s0[4]) * cste;
      de[5] -= (s1[5] - s0[5]) * cste;
    }
  }  // end of AbaqusStandardSmallStrainStressFreeExpansionHandler

  void AbaqusStandardSmallStrainStressFreeExpansionHandler(
      float *const e,
      float *const de,
      const float *const s0,
      const float *const s1,
      const AbaqusInt d) {
    AbaqusStandardSmallStrainStressFreeExpansionHandlerImpl(e, de, s0, s1, d);
  }

  void AbaqusStandardSmallStrainStressFreeExpansionHandler(
      double *const e,
      double *const de,
      const double *const s0,
      const double *const s1,
      const AbaqusInt d) {
    AbaqusStandardSmallStrainStressFreeExpansionHandlerImpl(e, de, s0, s1, d);
  }

  template <typename real>
  static void AbaqusLogarithmicStrainStressFreeExpansionHandlerImpl(
      real *const e,
      real *const de,
      const real *const s0,
      const real *const s1,
      const AbaqusInt d) {
    constexpr auto cste = tfel::math::Cste<real>::sqrt2;
    if (d == 2) {
      // s0 and s1 are given using TFEL conventions
      tfel::math::stensor<2u, real> e0(s0);
      tfel::math::stensor<2u, real> e1(s1);
      const auto ln_e0 = tfel::math::logarithm(e0);
      const auto ln_e1 = tfel::math::logarithm(e1);
      e[0] -= ln_e0[0];
      e[1] -= ln_e0[1];
      e[2] -= ln_e0[2];
      e[3] -= ln_e0[3] * cste;
      de[0] -= (ln_e1[0] - ln_e0[0]);
      de[1] -= (ln_e1[1] - ln_e0[1]);
      de[2] -= (ln_e1[2] - ln_e0[2]);
      de[3] -= (ln_e1[3] - ln_e0[3]) * cste;
    } else {
      tfel::math::stensor<3u, real> e0(s0);
      tfel::math::stensor<3u, real> e1(s1);
      const auto ln_e0 = tfel::math::logarithm(e0);
      const auto ln_e1 = tfel::math::logarithm(e1);
      e[0] -= ln_e0[0];
      e[1] -= ln_e0[1];
      e[2] -= ln_e0[2];
      e[3] -= ln_e0[3] * cste;
      e[4] -= ln_e0[3] * cste;
      e[5] -= ln_e0[5] * cste;
      de[0] -= (ln_e1[0] - ln_e0[0]);
      de[1] -= (ln_e1[1] - ln_e0[1]);
      de[2] -= (ln_e1[2] - ln_e0[2]);
      de[3] -= (ln_e1[3] - ln_e0[3]) * cste;
      de[4] -= (ln_e1[4] - ln_e0[4]) * cste;
      de[5] -= (ln_e1[5] - ln_e0[5]) * cste;
    }
  }  // end of AbaqusLogarithmicStrainStressFreeExpansionHandler

  void AbaqusLogarithmicStrainStressFreeExpansionHandler(float *const e,
                                                         float *const de,
                                                         const float *const s0,
                                                         const float *const s1,
                                                         const AbaqusInt d) {
    AbaqusLogarithmicStrainStressFreeExpansionHandlerImpl(e, de, s0, s1, d);
  }

  void AbaqusLogarithmicStrainStressFreeExpansionHandler(double *const e,
                                                         double *const de,
                                                         const double *const s0,
                                                         const double *const s1,
                                                         const AbaqusInt d) {
    AbaqusLogarithmicStrainStressFreeExpansionHandlerImpl(e, de, s0, s1, d);
  }

}  // end of namespace abaqus
