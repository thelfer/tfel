/*!
 * \file  mfront/src/AsterStressFreeExpansionHandler.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include "MFront/Aster/AsterStressFreeExpansionHandler.hxx"

namespace aster {

  void AsterStandardSmallStrainStressFreeExpansionHandler(
      AsterReal *const e,
      AsterReal *const de,
      const AsterReal *const s0,
      const AsterReal *const s1,
      const AsterInt d) {
    using namespace std;
    static const AsterReal cste = AsterReal(2) / sqrt(AsterReal(2));
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
  }  // end of AsterStandardSmallStrainStressFreeExpansionHandler

}  // end of namespace aster
