/*!
 * \file  mfront/src/EuroplexusStressFreeExpansionHandler.cxx
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
#include "MFront/Europlexus/EuroplexusStressFreeExpansionHandler.hxx"

namespace epx {

  void EuroplexusStandardSmallStrainStressFreeExpansionHandler(
      EuroplexusReal *const e,
      EuroplexusReal *const de,
      const EuroplexusReal *const s0,
      const EuroplexusReal *const s1,
      const EuroplexusInt d) {
    if (d == 0) {
      tfel::math::StensorView<3u, EuroplexusReal> se(e);
      tfel::math::StensorView<3u, EuroplexusReal> sde(de);
      tfel::math::ConstStensorView<3u, EuroplexusReal> se0(s0);
      tfel::math::ConstStensorView<3u, EuroplexusReal> se1(s1);
      se -= se0;
      sde -= se1 - se0;
    } else {
      tfel::math::StensorView<2u, EuroplexusReal> se(e);
      tfel::math::StensorView<2u, EuroplexusReal> sde(de);
      tfel::math::ConstStensorView<2u, EuroplexusReal> se0(s0);
      tfel::math::ConstStensorView<2u, EuroplexusReal> se1(s1);
      se -= se0;
      sde -= se1 - se0;
    }
  }  // end of EuroplexusStandardSmallStrainStressFreeExpansionHandler

  void EuroplexusLogarithmicStrainStressFreeExpansionHandler(
      EuroplexusReal *const e,
      EuroplexusReal *const de,
      const EuroplexusReal *const s0,
      const EuroplexusReal *const s1,
      const EuroplexusInt d) {
    if (d == 0) {
      tfel::math::StensorView<3u, EuroplexusReal> se(e);
      tfel::math::StensorView<3u, EuroplexusReal> sde(de);
      tfel::math::stensor<3u, EuroplexusReal> se0(s0);
      tfel::math::stensor<3u, EuroplexusReal> se1(s1);
      tfel::raise_if(
          (std::abs(s0[3]) > 10 * std::numeric_limits<EuroplexusReal>::min()) ||
              (std::abs(s1[3]) >
               10 * std::numeric_limits<EuroplexusReal>::min()) ||
              (std::abs(s0[4]) >
               10 * std::numeric_limits<EuroplexusReal>::min()) ||
              (std::abs(s1[4]) >
               10 * std::numeric_limits<EuroplexusReal>::min()) ||
              (std::abs(s0[5]) >
               10 * std::numeric_limits<EuroplexusReal>::min()) ||
              (std::abs(s1[5]) >
               10 * std::numeric_limits<EuroplexusReal>::min()),
          "EuroplexusLogarithmicStrainStressFreeExpansionHandler: "
          "stress free expansion is assumed diagonal");
      se[0] -= std::log1p(se0[0]);
      se[1] -= std::log1p(se0[1]);
      se[2] -= std::log1p(se0[2]);
      sde[0] -= std::log1p((se1[0] - se0[0]) / (1 + se0[0]));
      sde[1] -= std::log1p((se1[1] - se0[1]) / (1 + se0[1]));
      sde[2] -= std::log1p((se1[2] - se0[2]) / (1 + se0[2]));
    } else {
      tfel::math::StensorView<2u, EuroplexusReal> se(e);
      tfel::math::StensorView<2u, EuroplexusReal> sde(de);
      tfel::math::stensor<2u, EuroplexusReal> se0(s0);
      tfel::math::stensor<2u, EuroplexusReal> se1(s1);
      tfel::raise_if(
          (std::abs(s0[3]) > 10 * std::numeric_limits<EuroplexusReal>::min()) ||
              (std::abs(s1[3]) >
               10 * std::numeric_limits<EuroplexusReal>::min()),
          "EuroplexusLogarithmicStrainStressFreeExpansionHandler: "
          "stress free expansion is assumed diagonal");
      se[0] -= std::log1p(se0[0]);
      se[1] -= std::log1p(se0[1]);
      se[2] -= std::log1p(se0[2]);
      sde[0] -= std::log1p((se1[0] - se0[0]) / (1 + se0[0]));
      sde[1] -= std::log1p((se1[1] - se0[1]) / (1 + se0[1]));
      sde[2] -= std::log1p((se1[2] - se0[2]) / (1 + se0[2]));
    }
  }  // end of EuroplexusLogarithmicStrainStrainStressFreeExpansionHandler

}  // end of namespace epx
