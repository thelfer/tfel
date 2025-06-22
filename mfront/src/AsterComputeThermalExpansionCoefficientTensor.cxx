/*!
 * \file   mfront/src/AsterComputeThermalExpansionCoefficientTensor.cxx
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

#include "TFEL/Math/stensor.hxx"
#include "MFront/Aster/AsterComputeThermalExpansionCoefficientTensor.hxx"

namespace aster {

  static void AsterComputeIsotropicThermalExpansionCoefficientTensor2D(
      const AsterReal* const props,
      tfel::config::Types<2u, AsterReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const AsterReal a = props[0];
    A(0) = A(1) = A(2) = a;
    A(3) = 0.;
  }

  static void AsterComputeOrthotropicThermalExpansionCoefficientTensor2D(
      const AsterReal* const props,
      tfel::config::Types<2u, AsterReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const AsterReal a0 = props[0];
    const AsterReal a1 = props[1];
    const AsterReal a2 = props[2];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
    A(3) = 0.;
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC>::exe(const AsterReal* const props,
                      tfel::config::Types<2u, AsterReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    AsterComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ISOTROPIC>::exe(const AsterReal* const props,
                      tfel::config::Types<2u, AsterReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    AsterComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ISOTROPIC>::exe(const AsterReal* const props,
                      tfel::config::Types<2u, AsterReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    AsterComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ISOTROPIC>::exe(const AsterReal* const props,
                      tfel::config::Types<2u, AsterReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    AsterComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ISOTROPIC>::exe(const AsterReal* const props,
                      tfel::config::Types<3u, AsterReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    const AsterReal a = props[0];
    A(0) = A(1) = A(2) = a;
    A(3) = A(4) = A(5) = 0.;
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC>::exe(const AsterReal* const props,
                        tfel::config::Types<2u, AsterReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    AsterComputeOrthotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ORTHOTROPIC>::exe(const AsterReal* const props,
                        tfel::config::Types<2u, AsterReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    AsterComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ORTHOTROPIC>::exe(const AsterReal* const props,
                        tfel::config::Types<2u, AsterReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    AsterComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ORTHOTROPIC>::exe(const AsterReal* const props,
                        tfel::config::Types<2u, AsterReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    AsterComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

  void AsterComputeThermalExpansionCoefficientTensor<
      aster::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ORTHOTROPIC>::exe(const AsterReal* const props,
                        tfel::config::Types<3u, AsterReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    const AsterReal a0 = props[0];
    const AsterReal a1 = props[1];
    const AsterReal a2 = props[2];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
    A(3) = A(4) = A(5) = 0.;
  }  // end of struct AsterComputeThermalExpansionCoefficientTensor

}  // end of namespace aster
