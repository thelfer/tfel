/*!
 * \file   mfront/src/CastemComputeStiffnessTensor.cxx
 * \brief
 * \author Helfer Thomas
 * \date   18 mar 2008
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
#include "MFront/Castem/CastemComputeStiffnessTensor.hxx"

namespace castem {

  void CastemComputeIsotropicStiffnessTensor1D(
      tfel::config::Types<1u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<1u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }  // end of CastemComputeIsotropicStiffnessTensor1D

  void CastemComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }

  void CastemComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::ALTERED>(C, props[0], props[1]);
  }  // end of struct CastemComputeIsotropicPlaneStressAlteredStiffnessTensor

  void CastemComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<3u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }  // end of struct CastemComputeStiffnessTensor

  void CastemComputeOrthotropicStiffnessTensor1D(
      tfel::config::Types<1u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<1u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[0],
        props[1], props[2]);
  }  // end of struct CastemComputeStiffnessTensor

  void CastemComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct CastemComputeStiffnessTensor

  void CastemComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props) {
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'NU12'
    // props[3] :'G12'
    // props[6] :'YG3'
    // props[7] :'NU23'
    // props[8] :'NU13'
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::ALTERED>(
        C, props[0], props[1], props[6], props[2], props[7], props[8], props[3],
        props[3], props[3]);
  }  // end of struct
     // CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor

  void CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props) {
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'NU12'
    // props[3] :'G12'
    // props[6] :'YG3'
    // props[7] :'NU23'
    // props[8] :'NU13'
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::UNALTERED>(
        C, props[0], props[1], props[6], props[2], props[7], props[8], props[3],
        props[3], props[3]);
  }  // end of struct
     // CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor

  void CastemComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, CastemReal, false>::StiffnessTensor& C,
      const CastemReal* const props) {
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'YG3'
    // props[3] :'NU12'
    // props[4] :'NU23'
    // props[5] :'NU13'
    // props[6] :'G12'
    // props[7] :'G23'
    // props[8] :'G13'
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<3u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[7], props[8]);
  }  // end of struct CastemComputeStiffnessTensor

}  // end of namespace castem
