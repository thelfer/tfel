/*!
 * \file  mfront/src/CastemStressFreeExpansionHandler.cxx
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
#include <limits>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "MFront/Castem/CastemStressFreeExpansionHandler.hxx"

namespace castem {

  void CastemStandardSmallStrainStressFreeExpansionHandler(
      CastemReal *const e,
      CastemReal *const de,
      const CastemReal *const s0,
      const CastemReal *const s1,
      const CastemInt d) {
    constexpr const auto cste = tfel::math::Cste<CastemReal>::sqrt2;
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
  }  // end of CastemStandardSmallStrainStressFreeExpansionHandler

  void CastemLogarithmicStrainStressFreeExpansionHandler(
      CastemReal *const e,
      CastemReal *const de,
      const CastemReal *const s0,
      const CastemReal *const s1,
      const CastemInt d) {
    constexpr const auto cste = tfel::math::Cste<CastemReal>::sqrt2;
    CastemReal log_s0[6];
    CastemReal log_s1[6];
    if (d == 1) {
      log_s0[0] = std::log1p(s0[0]);
      log_s0[1] = std::log1p(s0[1]);
      log_s0[2] = std::log1p(s0[2]);
      log_s1[0] = std::log1p(s1[0]);
      log_s1[1] = std::log1p(s1[1]);
      log_s1[2] = std::log1p(s1[2]);
    } else if (d == 2) {
      tfel::raise_if(
          (std::abs(s0[3]) > 10 * std::numeric_limits<CastemReal>::min()) ||
              (std::abs(s1[3]) > 10 * std::numeric_limits<CastemReal>::min()),
          "CastemLogarithmicStrainStressFreeExpansionHandler: "
          "stress free expansion is assumed diagonal");
      log_s0[0] = std::log1p(s0[0]);
      log_s0[1] = std::log1p(s0[1]);
      log_s0[2] = std::log1p(s0[2]);
      log_s0[3] = CastemReal(0);
      log_s1[0] = std::log1p(s1[0]);
      log_s1[1] = std::log1p(s1[1]);
      log_s1[2] = std::log1p(s1[2]);
      log_s1[3] = CastemReal(0);
    } else if (d == 3) {
      tfel::raise_if(
          (std::abs(s0[3]) > 10 * std::numeric_limits<CastemReal>::min()) ||
              (std::abs(s1[3]) > 10 * std::numeric_limits<CastemReal>::min()) ||
              (std::abs(s0[4]) > 10 * std::numeric_limits<CastemReal>::min()) ||
              (std::abs(s1[4]) > 10 * std::numeric_limits<CastemReal>::min()) ||
              (std::abs(s0[5]) > 10 * std::numeric_limits<CastemReal>::min()) ||
              (std::abs(s1[5]) > 10 * std::numeric_limits<CastemReal>::min()),
          "CastemLogarithmicStrainStressFreeExpansionHandler: "
          "stress free expansion is assumed diagonal");
      log_s0[0] = std::log1p(s0[0]);
      log_s0[1] = std::log1p(s0[1]);
      log_s0[2] = std::log1p(s0[2]);
      log_s0[3] = CastemReal(0);
      log_s0[4] = CastemReal(0);
      log_s0[5] = CastemReal(0);
      log_s1[0] = std::log1p(s1[0]);
      log_s1[1] = std::log1p(s1[1]);
      log_s1[2] = std::log1p(s1[2]);
      log_s1[3] = CastemReal(0);
      log_s1[4] = CastemReal(0);
      log_s1[5] = CastemReal(0);
    } else {
      tfel::raise(
          "CastemLogarithmicStrainStressFreeExpansionHandler: "
          "invalid dimension");
    }
    e[0] -= log_s0[0];
    e[1] -= log_s0[1];
    e[2] -= log_s0[2];
    de[0] -= (log_s1[0] - log_s0[0]);
    de[1] -= (log_s1[1] - log_s0[1]);
    de[2] -= (log_s1[2] - log_s0[2]);
    if (d == 2) {
      e[3] -= log_s0[3] * cste;
      de[3] -= (log_s1[3] - log_s0[3]) * cste;
    }
    if (d == 3) {
      e[3] -= log_s0[3] * cste;
      e[4] -= log_s0[3] * cste;
      e[5] -= log_s0[5] * cste;
      de[3] -= (log_s1[3] - log_s0[3]) * cste;
      de[4] -= (log_s1[4] - log_s0[4]) * cste;
      de[5] -= (log_s1[5] - log_s0[5]) * cste;
    }
  }  // end of CastemLogarithmicStrainStressFreeExpansionHandler

}  // end of namespace castem
