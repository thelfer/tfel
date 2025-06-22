/*!
 * \file   mfront/src/CyranoComputeStiffnessTensor.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include "MFront/Cyrano/CyranoComputeStiffnessTensor.hxx"

namespace cyrano {

  void CyranoComputeStiffnessTensor<ISOTROPIC>::exe(
      tfel::config::Types<1u, CyranoReal, false>::StiffnessTensor& C,
      const CyranoReal* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<1u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }  // end of struct CyranoComputeStiffnessTensor

  void CyranoComputeStiffnessTensor<ORTHOTROPIC>::exe(
      tfel::config::Types<1u, CyranoReal, false>::StiffnessTensor& C,
      const CyranoReal* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<1u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[0],
        props[1], props[2]);
  }  // end of struct CyranoComputeStiffnessTensor

}  // end of namespace cyrano
