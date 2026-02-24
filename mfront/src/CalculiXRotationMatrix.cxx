/*!
 * \file   CalculiXRotationMatrix.cxx
 * \brief
 * \author Thomas Helfer
 * \date   18 sept. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include "MFront/CalculiX/CalculiXRotationMatrix.hxx"

namespace calculix {

  tfel::math::tmatrix<3u, 3u, CalculiXReal> getRotationMatrix(
      const CalculiXReal* const c, const CalculiXReal* const p) {
    using namespace tfel::math;
    constexpr CalculiXReal eps = CalculiXReal(1.e-10);
    tmatrix<3u, 3u, CalculiXReal> r;
    tvector<3u, CalculiXReal> e1, e2, e3;
    if (c[6] >= 0) {
      // carthesian transformation
      e1 = {c[0], c[1], c[2]};
      e2 = {c[3], c[4], c[5]};
      //
      e1 /= std::sqrt(e1[0] * e1[0] + e1[1] * e1[1] + e1[2] * e1[2]);
      e2 /= std::sqrt(e2[0] * e2[0] + e2[1] * e2[1] + e2[2] * e2[2]);
      e3 = {e1[1] * e2[2] - e2[1] * e1[2], e1[2] * e2[0] - e1[0] * e2[2],
            e1[0] * e2[1] - e2[0] * e1[1]};
    } else {
      //
      // cylindrical coordinate system in point p
      //
      e1[0] = p[0] - c[0];
      e1[1] = p[1] - c[1];
      e1[2] = p[2] - c[2];
      //
      e3[0] = c[3] - c[0];
      e3[1] = c[4] - c[1];
      e3[2] = c[5] - c[2];
      e3 /= std::sqrt(e3[0] * e3[0] + e3[1] * e3[1] + e3[2] * e3[2]);
      const auto dd = e1[0] * e3[0] + e1[1] * e3[1] + e1[2] * e3[2];
      e1 -= dd * e3;
      auto dd2 = std::sqrt(e1[0] * e1[0] + e1[1] * e1[1] + e1[2] * e1[2]);
      // check whether p belongs to the cylindrical coordinate axis
      // if so, an arbitrary vector perpendicular to the axis can
      // be taken
      if (dd2 <= 1.e-10) {
        if (std::abs(e3[0]) >= eps) {
          e1[1] = 1;
          e1[2] = 0;
          e1[0] = -e3[1] / e3[0];
        } else if (std::abs(e3[1]) >= eps) {
          e1[2] = 1;
          e1[0] = 0;
          e1[1] = -e3[2] / e3[1];
        } else {
          e1[0] = 1;
          e1[1] = 0;
          e1[2] = -e3[0] / e3[2];
        }
        dd2 = std::sqrt(e1[0] * e1[0] + e1[1] * e1[1] + e1[2] * e1[2]);
      }
      e1 /= dd2;
      e2[0] = e3[1] * e1[2] - e1[1] * e3[2];
      e2[1] = e3[2] * e1(1) - e1[2] * e3[0];
      e2[2] = e3[0] * e1[1] - e1(1) * e3[1];
      //
    }
    r.column_view<0>() = e1;
    r.column_view<1>() = e2;
    r.column_view<2>() = e3;
    return r;
  }  // end of getRotationMatrix(

}  // end of namespace calculix
