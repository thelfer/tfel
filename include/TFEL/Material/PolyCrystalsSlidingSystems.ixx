/*!
 * \file  include/TFEL/Material/PolyCrystalsSlidingSystems.ixx
 * \brief
 * \author Thomas Helfer
 * \brief 16 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATERIAL_POLYCRYSTALSSLIDINGSYSTEMS_IXX
#define TFEL_MATERIAL_POLYCRYSTALSSLIDINGSYSTEMS_IXX 1

#include <cstdlib>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Math/General/RandomRotationMatrix.hxx"

namespace tfel {

  namespace material {

    template <unsigned short Np, typename GS, typename NumType>
    const PolyCrystalsSlidingSystems<Np, GS, NumType>&
    PolyCrystalsSlidingSystems<Np, GS, NumType>::getPolyCrystalsSlidingSystems(
        const std::string& f) {
      static PolyCrystalsSlidingSystems gs(f);
      return gs;
    }

    template <unsigned short Np, typename GS, typename NumType>
    PolyCrystalsSlidingSystems<Np, GS, NumType>::PolyCrystalsSlidingSystems(
        const std::string& f)
        : mus(Np) {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::utilities;
      // the sliding systems of one phase
      const auto& gs = GS::getSlidingSystems();
      if (f.empty()) {
        for (unsigned short i = 0; i != Np; i++) {
          // allocting memory
          this->mus[i].resize(Nss);
          // rotation matrix
          tmatrix<3u, 3u, real> drot = getRandomRotationMatrix<real>();
          for (unsigned short j = 0; j != Nss; j++) {
            auto& mu = this->mus[i][j];
            // local tensor of directional sense
            mu = gs.mus[j];
            // change to the global direction
            mu.changeBasis(drot);
          }
        }
      } else {
        const real pi = real(4) * std::atan(real(1));
        // reading from the specified file
        CxxTokenizer file(f);
        file.stripComments();
        CxxTokenizer::const_iterator p = file.begin();
        const CxxTokenizer::const_iterator pe = file.end();
        for (unsigned short i = 0; i != Np; i++) {
          this->mus[i].resize(Nss);
          tmatrix<3u, 3u, real> drot;
          const real psi = CxxTokenizer::readDouble(p, pe) * pi / 180.0;
          const real the = CxxTokenizer::readDouble(p, pe) * pi / 180.0;
          const real phi = CxxTokenizer::readDouble(p, pe) * pi / 180.0;
          const real cospsi = std::cos(psi);
          const real costhe = std::cos(the);
          const real cosphi = std::cos(phi);
          const real sinpsi = std::sin(psi);
          const real sinthe = std::sin(the);
          const real sinphi = std::sin(phi);
          drot(0, 0) = cosphi * cospsi - sinphi * costhe * sinpsi;
          drot(0, 1) = cosphi * sinpsi + sinphi * costhe * cospsi;
          drot(0, 2) = sinphi * sinthe;
          drot(1, 0) = -sinphi * cospsi - cosphi * costhe * sinpsi;
          drot(1, 1) = -sinphi * sinpsi + cosphi * costhe * cospsi;
          drot(1, 2) = cosphi * sinthe;
          drot(2, 0) = sinthe * sinpsi;
          drot(2, 1) = -sinthe * cospsi;
          drot(2, 2) = costhe;
          for (unsigned short j = 0; j != Nss; j++) {
            auto& mu = this->mus[i][j];
            mu = gs.mus[j];
            mu.changeBasis(drot);
          }
        }
      }
    }
  }  // end of namespace material

}  // end of namespace tfel

#endif /* TFEL_MATERIAL_POLYCRYSTALSSLIDINGSYSTEMS_IXX */
