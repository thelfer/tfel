/*!
 * \file   mfront/src/DianaFEAComputeStiffnessTensor.cxx
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
#include "MFront/DianaFEA/DianaFEAComputeStiffnessTensor.hxx"

namespace dianafea {

  template <typename real>
  static inline void DianaFEAComputeIsotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }

  template <typename real>
  static inline void
  DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props)

  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u, STAC::ALTERED>(C, props[0], props[1]);
  }  // end of struct DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void DianaFEAComputeIsotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<3u, STAC::UNALTERED>(C, props[0],
                                                           props[1]);
  }  // end of struct DianaFEAComputeStiffnessTensor

  template <typename real>
  static inline void DianaFEAComputeOrthotropicStiffnessTensor2DImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct DianaFEAComputeStiffnessTensor

  template <typename real>
  static inline void
  DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(
      typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u, STAC::ALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[6], props[6]);
  }  // end of struct
     // DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor

  template <typename real>
  static inline void DianaFEAComputeOrthotropicStiffnessTensor3DImpl(
      typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
      const real* const props) {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<3u, STAC::UNALTERED>(
        C, props[0], props[1], props[2], props[3], props[4], props[5], props[6],
        props[7], props[8]);
  }  // end of struct DianaFEAComputeStiffnessTensor

  void DianaFEAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    DianaFEAComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props)

  {
    DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor

  void DianaFEAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    DianaFEAComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct DianaFEAComputeStiffnessTensor

  void DianaFEAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    DianaFEAComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct DianaFEAComputeStiffnessTensor

  void DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor& C,
      const float* const props) {
    DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct
     // DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void DianaFEAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor& C,
      const float* const props) {
    DianaFEAComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct DianaFEAComputeStiffnessTensor

  void DianaFEAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    DianaFEAComputeIsotropicStiffnessTensor2DImpl(C, props);
  }

  void DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props)

  {
    DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor

  void DianaFEAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    DianaFEAComputeIsotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct DianaFEAComputeStiffnessTensor

  void DianaFEAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    DianaFEAComputeOrthotropicStiffnessTensor2DImpl(C, props);
  }  // end of struct DianaFEAComputeStiffnessTensor

  void DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor& C,
      const double* const props) {
    DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C, props);
  }  // end of struct
     // DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void DianaFEAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor& C,
      const double* const props) {
    DianaFEAComputeOrthotropicStiffnessTensor3DImpl(C, props);
  }  // end of struct DianaFEAComputeStiffnessTensor

}  // end of namespace dianafea
