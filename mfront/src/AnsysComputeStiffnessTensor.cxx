/*!
 * \file   mfront/src/AnsysComputeStiffnessTensor.cxx
 * \brief
 * \author Thomas Helfer
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
#include "MFront/Ansys/AnsysComputeStiffnessTensor.hxx"

namespace ansys {

  template <typename real>
  static inline void AnsysComputeIsotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }

  template <typename real>
  static inline void AnsysComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::ALTERED>(C, props[0], props[1]);
  }  // end of struct AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void AnsysComputeIsotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<3u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }  // end of struct AnsysComputeStiffnessTensor

  template <typename real>
  static inline void AnsysComputeOrthotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct AnsysComputeStiffnessTensor

  template <typename real>
  static inline void
  AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::ALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void AnsysComputeOrthotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<3u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[7], props[8]);
  }  // end of struct AnsysComputeStiffnessTensor

  void AnsysComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AnsysComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props)

  {
    AnsysComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor

  void AnsysComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AnsysComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AnsysComputeStiffnessTensor

  void AnsysComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AnsysComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct AnsysComputeStiffnessTensor

  void AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void AnsysComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AnsysComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AnsysComputeStiffnessTensor

  void AnsysComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AnsysComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props)

  {
    AnsysComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor

  void AnsysComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AnsysComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AnsysComputeStiffnessTensor

  void AnsysComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AnsysComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct AnsysComputeStiffnessTensor

  void AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void AnsysComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AnsysComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AnsysComputeStiffnessTensor

}  // end of namespace ansys
