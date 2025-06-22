/*!
 * \file   mfront/src/EuroplexusComputeThermalExpansionCoefficientTensor.cxx
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

#include "TFEL/Math/stensor.hxx"
#include "MFront/Europlexus/EuroplexusComputeThermalExpansionCoefficientTensor.hxx"

namespace epx {

  static void EuroplexusComputeIsotropicThermalExpansionCoefficientTensor2D(
      const EuroplexusReal* const props,
      tfel::config::Types<2u, EuroplexusReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const EuroplexusReal a = props[0];
    A(0) = A(1) = A(2) = a;
    A(3) = 0.;
  }

  static void EuroplexusComputeOrthotropicThermalExpansionCoefficientTensor2D(
      const EuroplexusReal* const props,
      tfel::config::Types<2u, EuroplexusReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const EuroplexusReal a0 = props[0];
    const EuroplexusReal a1 = props[1];
    const EuroplexusReal a2 = props[2];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
    A(3) = 0.;
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC>::exe(const EuroplexusReal* const props,
                      tfel::config::Types<2u, EuroplexusReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    EuroplexusComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ISOTROPIC>::exe(const EuroplexusReal* const props,
                      tfel::config::Types<2u, EuroplexusReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    EuroplexusComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ISOTROPIC>::exe(const EuroplexusReal* const props,
                      tfel::config::Types<2u, EuroplexusReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    EuroplexusComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ISOTROPIC>::exe(const EuroplexusReal* const props,
                      tfel::config::Types<2u, EuroplexusReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    EuroplexusComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ISOTROPIC>::exe(const EuroplexusReal* const props,
                      tfel::config::Types<3u, EuroplexusReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    const EuroplexusReal a = props[0];
    A(0) = A(1) = A(2) = a;
    A(3) = A(4) = A(5) = 0.;
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC>::exe(const EuroplexusReal* const props,
                        tfel::config::Types<2u, EuroplexusReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    EuroplexusComputeOrthotropicThermalExpansionCoefficientTensor2D(props, A);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ORTHOTROPIC>::exe(const EuroplexusReal* const props,
                        tfel::config::Types<2u, EuroplexusReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    EuroplexusComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ORTHOTROPIC>::exe(const EuroplexusReal* const props,
                        tfel::config::Types<2u, EuroplexusReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    EuroplexusComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ORTHOTROPIC>::exe(const EuroplexusReal* const props,
                        tfel::config::Types<2u, EuroplexusReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    EuroplexusComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  void EuroplexusComputeThermalExpansionCoefficientTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ORTHOTROPIC>::exe(const EuroplexusReal* const props,
                        tfel::config::Types<3u, EuroplexusReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    const EuroplexusReal a0 = props[0];
    const EuroplexusReal a1 = props[1];
    const EuroplexusReal a2 = props[2];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
    A(3) = A(4) = A(5) = 0.;
  }  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

}  // end of namespace epx
