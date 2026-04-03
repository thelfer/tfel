/*!
 * \file   mfront/src/LSDYNAComputeStiffnessTensor.cxx
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
#include "MFront/LSDYNA/LSDYNAComputeStiffnessTensor.hxx"

namespace lsdyna {

  template <typename real>
  static inline void LSDYNAComputeIsotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }

  template <typename real>
  static inline void
  LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::ALTERED>(C, props[0], props[1]);
  }  // end of struct LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void LSDYNAComputeIsotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<3u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }  // end of struct LSDYNAComputeStiffnessTensor

  template <typename real>
  static inline void LSDYNAComputeOrthotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct LSDYNAComputeStiffnessTensor

  template <typename real>
  static inline void
  LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::ALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void LSDYNAComputeOrthotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<3u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[7], props[8]);
  }  // end of struct LSDYNAComputeStiffnessTensor

  void LSDYNAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    LSDYNAComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props)

  {
    LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor

  void LSDYNAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    LSDYNAComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct LSDYNAComputeStiffnessTensor

  void LSDYNAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    LSDYNAComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct LSDYNAComputeStiffnessTensor

  void LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void LSDYNAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    LSDYNAComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct LSDYNAComputeStiffnessTensor

  void LSDYNAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    LSDYNAComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props)

  {
    LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor

  void LSDYNAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    LSDYNAComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct LSDYNAComputeStiffnessTensor

  void LSDYNAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    LSDYNAComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct LSDYNAComputeStiffnessTensor

  void LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void LSDYNAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    LSDYNAComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct LSDYNAComputeStiffnessTensor

}  // end of namespace lsdyna
