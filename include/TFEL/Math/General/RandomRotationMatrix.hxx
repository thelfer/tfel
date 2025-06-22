/*!
 * \file  include/TFEL/Math/General/RandomRotationMatrix.hxx
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

#ifndef LIB_TFEL_MATH_RANDOMROTATIONMATRIX_HXX
#define LIB_TFEL_MATH_RANDOMROTATIONMATRIX_HXX

#include <cmath>
#include <cstdlib>
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  template <typename T>
  rotation_matrix<T> getRandomRotationMatrix() {
    using namespace std;
    const T rmax = static_cast<T>(RAND_MAX);
    const T pi = T(4) * atan(T(1));
    rotation_matrix<T> drot;
    T psi = T(2) * pi * (static_cast<T>(rand()) / rmax);
    T cthe = T(2) * ((static_cast<T>(rand()) / rmax) - T(1) / T(2));
    T the = acos(cthe);
    T phi = T(2) * pi * (static_cast<T>(rand()) / rmax);
    T cospsi = cos(psi);
    T costhe = cos(the);
    T cosphi = cos(phi);
    T sinpsi = sin(psi);
    T sinthe = sin(the);
    T sinphi = sin(phi);
    drot(0, 0) = cosphi * cospsi - sinphi * costhe * sinpsi;
    drot(0, 1) = cosphi * sinpsi + sinphi * costhe * cospsi;
    drot(0, 2) = sinphi * sinthe;
    drot(1, 0) = -sinphi * cospsi - cosphi * costhe * sinpsi;
    drot(1, 1) = -sinphi * sinpsi + cosphi * costhe * cospsi;
    drot(1, 2) = cosphi * sinthe;
    drot(2, 0) = sinthe * sinpsi;
    drot(2, 1) = -sinthe * cospsi;
    drot(2, 2) = costhe;
    return drot;
  }  // end of getRandomRotationMatrix

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_RANDOMROTATIONMATRIX_HXX */
