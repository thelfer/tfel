/*!
 * \file  include/TFEL/Material/MetallicCFCSlidingSystems.ixx
 * \brief
 * \author Thomas Helfer
 * \brief 21 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMSIXX
#define LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMSIXX

#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel::material {

  template <typename NumType>
  const MetallicCFCSlidingSystems<NumType>&
  MetallicCFCSlidingSystems<NumType>::getSlidingSystems() {
    static MetallicCFCSlidingSystems gs;
    return gs;
  }  // end of MetallicCFCSlidingSystems::getSlidingSystems

  template <typename NumType>
  MetallicCFCSlidingSystems<NumType>::MetallicCFCSlidingSystems() {
    using namespace tfel::math;
    const auto coefm = Cste<real>::isqrt2;
    const auto coefn = Cste<real>::isqrt3;
    // sliding systems
    const real nx[Nss] = {1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
                          -1.0, -1.0, -1.0, -1.0, -1.0, -1.0};
    const real ny[Nss] = {1.0, 1.0, 1.0, -1.0, -1.0, -1.0,
                          1.0, 1.0, 1.0, -1.0, -1.0, -1.0};
    const real nz[Nss] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    const real mx[Nss] = {-1.0, 0.0, -1.0, -1.0, 0.0, 1.0,
                          0.0,  1.0, 1.0,  -1.0, 1.0, 0.0};
    const real my[Nss] = {0.0,  -1.0, 1.0, 0.0, 1.0, 1.0,
                          -1.0, 1.0,  0.0, 1.0, 0.0, 1.0};
    const real mz[Nss] = {1.0, 1.0, 0.0, 1.0, 1.0, 0.0,
                          1.0, 0.0, 1.0, 0.0, 1.0, 1.0};
    for (unsigned short i = 0; i != Nss; ++i) {
      tvector<3u, real> ns(real(0));
      tvector<3u, real> ms(real(0));
      ns[0] = nx[i] * coefn;
      ns[1] = ny[i] * coefn;
      ns[2] = nz[i] * coefn;
      ms[0] = mx[i] * coefm;
      ms[1] = my[i] * coefm;
      ms[2] = mz[i] * coefm;
      this->mu[i][0] = ns[0] * ms[0];  // XX
      this->mu[i][1] = ns[1] * ms[1];  // YY
      this->mu[i][2] = ns[2] * ms[2];  // ZZ
      this->mu[i][3] = ns[1] * ms[0];  // XY
      this->mu[i][4] = ns[0] * ms[1];  // YX
      this->mu[i][5] = ns[2] * ms[0];  // XZ
      this->mu[i][6] = ns[0] * ms[2];  // ZX
      this->mu[i][7] = ns[2] * ms[1];  // YZ
      this->mu[i][8] = ns[1] * ms[2];  // ZY
      this->mus[i] = syme(this->mu[i]);
    }
  }  // end of MetallicCFCSlidingSystems::MetallicCFCSlidingSystems

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMSIXX */
