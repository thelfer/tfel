/*!
 * \file   mfront/src/AbaqusComputeStiffnessTensor.cxx
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
#include "MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"

namespace abaqus {

  template <typename real>
  static inline void AbaqusComputeIsotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }

  template <typename real>
  static inline void
  AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::ALTERED>(C, props[0], props[1]);
  }  // end of struct AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void AbaqusComputeIsotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<3u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }  // end of struct AbaqusComputeStiffnessTensor

  template <typename real>
  static inline void AbaqusComputeOrthotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct AbaqusComputeStiffnessTensor

  template <typename real>
  static inline void
  AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::ALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void AbaqusComputeOrthotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<3u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[7], props[8]);
  }  // end of struct AbaqusComputeStiffnessTensor

  void AbaqusComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AbaqusComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props)

  {
    AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor

  void AbaqusComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AbaqusComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AbaqusComputeStiffnessTensor

  void AbaqusComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AbaqusComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct AbaqusComputeStiffnessTensor

  void AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void AbaqusComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    AbaqusComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AbaqusComputeStiffnessTensor

  void AbaqusComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AbaqusComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props)

  {
    AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor

  void AbaqusComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AbaqusComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AbaqusComputeStiffnessTensor

  void AbaqusComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AbaqusComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct AbaqusComputeStiffnessTensor

  void AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void AbaqusComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    AbaqusComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct AbaqusComputeStiffnessTensor

}  // end of namespace abaqus
