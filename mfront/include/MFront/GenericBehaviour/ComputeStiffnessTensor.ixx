/*!
 * \file   mfront/include/MFront/GenericBehaviour/ComputeStiffnessTensor.ixx
 * \brief
 * \author Thomas Helfer
 * \date   25/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERIC_COMPUTESTIFFNESSTENSOR_IXX
#define LIB_MFRONT_GENERIC_COMPUTESTIFFNESSTENSOR_IXX

#include "TFEL/Material/StiffnessTensor.hxx"

namespace mfront::gb {

  template <tfel::material::ModellingHypothesis::Hypothesis H,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE constexpr void
  computeOrthotropicUnAlteredElasticStiffnessTensor(
      tfel::math::st2tost2<1u, stress>& C,
      const real* const mps) noexcept  //
      requires(tfel::material::ModellingHypothesisToSpaceDimension<H>::value ==
               1u) {
    using namespace tfel::material;
    constexpr StiffnessTensorAlterationCharacteristic stac =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    // last three parameters are unused
    computeOrthotropicStiffnessTensor<H, stac, stress, real>(
        C, stress(mps[0]), stress(mps[1]), stress(mps[2]), mps[3], mps[4],
        mps[5], stress(mps[0]), stress(mps[1]), stress(mps[2]));
  }  // end of computeOrthotropicUnAlteredElasticStiffnessTensor

  template <tfel::material::ModellingHypothesis::Hypothesis H,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE constexpr void
  computeOrthotropicUnAlteredElasticStiffnessTensor(
      tfel::math::st2tost2<2u, stress>& C,
      const real* const mps) noexcept  //
      requires(tfel::material::ModellingHypothesisToSpaceDimension<H>::value ==
               2u) {
    using namespace tfel::material;
    constexpr StiffnessTensorAlterationCharacteristic stac =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    // last two parameters are unused
    computeOrthotropicStiffnessTensor<H, stac, stress, real>(
        C, stress(mps[0]), stress(mps[1]), stress(mps[2]), mps[3], mps[4],
        mps[5], stress(mps[6]), stress(mps[1]), stress(mps[2]));
  }  // end of computeOrthotropicUnAlteredElasticStiffnessTensor

  template <tfel::material::ModellingHypothesis::Hypothesis H,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE constexpr void
  computeOrthotropicUnAlteredElasticStiffnessTensor(
      tfel::math::st2tost2<3u, stress>& C, const real* const mps) noexcept
      requires(tfel::material::ModellingHypothesisToSpaceDimension<H>::value ==
               3u) {
    using namespace tfel::material;
    constexpr StiffnessTensorAlterationCharacteristic stac =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeOrthotropicStiffnessTensor<H, stac, stress, real>(
        C, stress(mps[0]), stress(mps[1]), stress(mps[2]), mps[3], mps[4],
        mps[5], stress(mps[6]), stress(mps[7]), stress(mps[8]));
  }  // end of computeOrthotropicUnAlteredElasticStiffnessTensor

  template <tfel::material::ModellingHypothesis::Hypothesis H,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE constexpr void
  computeOrthotropicAlteredElasticStiffnessTensor(
      tfel::math::st2tost2<1u, stress>& C,
      const real* const mps) noexcept  //
      requires(tfel::material::ModellingHypothesisToSpaceDimension<H>::value ==
               1u) {
    using namespace tfel::material;
    constexpr StiffnessTensorAlterationCharacteristic stac =
        StiffnessTensorAlterationCharacteristic::ALTERED;
    // last three parameters are unused
    computeOrthotropicStiffnessTensor<H, stac, stress, real>(
        C, stress(mps[0]), stress(mps[1]), stress(mps[2]), mps[3], mps[4],
        mps[5], stress(mps[0]), stress(mps[1]), stress(mps[2]));
  }  // end of computeOrthotropicAlteredElasticStiffnessTensor

  template <tfel::material::ModellingHypothesis::Hypothesis H,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE constexpr void
  computeOrthotropicAlteredElasticStiffnessTensor(
      tfel::math::st2tost2<2u, stress>& C,
      const real* const mps) noexcept  //
      requires(tfel::material::ModellingHypothesisToSpaceDimension<H>::value ==
               2u) {
    using namespace tfel::material;
    constexpr StiffnessTensorAlterationCharacteristic stac =
        StiffnessTensorAlterationCharacteristic::ALTERED;
    // last two parameters are unused
    computeOrthotropicStiffnessTensor<H, stac, stress, real>(
        C, stress(mps[0]), stress(mps[1]), stress(mps[2]), mps[3], mps[4],
        mps[5], stress(mps[6]), stress(mps[1]), stress(mps[2]));
  }  // end of computeOrthotropicAlteredElasticStiffnessTensor

  template <tfel::material::ModellingHypothesis::Hypothesis H,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE constexpr void
  computeOrthotropicAlteredElasticStiffnessTensor(
      tfel::math::st2tost2<3u, stress>& C,
      const real* const mps) noexcept  //
      requires(tfel::material::ModellingHypothesisToSpaceDimension<H>::value ==
               3u) {
    using namespace tfel::material;
    constexpr StiffnessTensorAlterationCharacteristic stac =
        StiffnessTensorAlterationCharacteristic::ALTERED;
    computeOrthotropicStiffnessTensor<H, stac, stress, real>(
        C, stress(mps[0]), stress(mps[1]), stress(mps[2]), mps[3], mps[4],
        mps[5], stress(mps[6]), stress(mps[7]), stress(mps[8]));
  }  // end of computeOrthotropicAlteredElasticStiffnessTensor

}  // end of namespace mfront::gb

#endif /* LIB_MFRONT_GENERIC_COMPUTESTIFFNESSTENSOR_HXX */
