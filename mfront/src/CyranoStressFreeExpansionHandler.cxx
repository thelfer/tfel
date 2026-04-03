/*!
 * \file  mfront/src/CyranoStressFreeExpansionHandler.cxx
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
#include "MFront/Cyrano/CyranoStressFreeExpansionHandler.hxx"

namespace cyrano {

  void CyranoStandardSmallStrainStressFreeExpansionHandler(
      CyranoReal *const em,
      CyranoReal *const dem,
      const CyranoReal *const e,
      const CyranoReal *const de,
      const CyranoReal *const s0,
      const CyranoReal *const s1) {
    em[0] = e[0] - s0[0];
    em[1] = e[2] - s0[1];
    em[2] = e[1] - s0[2];
    dem[0] = de[0] - (s1[0] - s0[0]);
    dem[1] = de[2] - (s1[1] - s0[1]);
    dem[2] = de[1] - (s1[2] - s0[2]);
  }  // end of CyranoStandardSmallStrainStressFreeExpansionHandler

  void CyranoLogarithmicStrainStressFreeExpansionHandler(
      CyranoReal *const em,
      CyranoReal *const dem,
      const CyranoReal *const e,
      const CyranoReal *const de,
      const CyranoReal *const s0,
      const CyranoReal *const s1) {
    CyranoReal log_s0[3];
    CyranoReal log_s1[3];
    log_s0[0] = std::log1p(s0[0]);
    log_s0[1] = std::log1p(s0[1]);
    log_s0[2] = std::log1p(s0[2]);
    log_s1[0] = std::log1p(s1[0]);
    log_s1[1] = std::log1p(s1[1]);
    log_s1[2] = std::log1p(s1[2]);
    em[0] = e[0] - log_s0[0];
    em[1] = e[2] - log_s0[1];
    em[2] = e[1] - log_s0[2];
    dem[0] = de[0] - (log_s1[0] - log_s0[0]);
    dem[1] = de[2] - (log_s1[1] - log_s0[1]);
    dem[2] = de[1] - (log_s1[2] - log_s0[2]);
  }  // end of CyranoLogarithmicStrainStressFreeExpansionHandler

}  // end of namespace cyrano
