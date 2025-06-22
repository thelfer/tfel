/*!
 * \file   mfront/src/CastemComputeThermalExpansionCoefficientTensor.cxx
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
#include "MFront/Castem/CastemComputeThermalExpansionCoefficientTensor.hxx"

namespace castem {

  static void CastemComputeIsotropicThermalExpansionCoefficientTensor2D(
      const CastemReal* const props,
      tfel::config::Types<2u, CastemReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const CastemReal a = props[3];
    A(0) = A(1) = A(2) = a;
    A(3) = 0.;
  }

  static void CastemComputeOrthotropicThermalExpansionCoefficientTensor2D(
      const CastemReal* const props,
      tfel::config::Types<2u, CastemReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const CastemReal a0 = props[10];
    const CastemReal a1 = props[11];
    const CastemReal a2 = props[12];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
    A(3) = 0.;
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
      ISOTROPIC>::exe(const CastemReal* const props,
                      tfel::config::Types<1u, CastemReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    const CastemReal a = props[3];
    A(0) = A(1) = A(2) = a;
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC>::exe(const CastemReal* const,
                      tfel::config::Types<2u, CastemReal, false>::
                          ThermalExpansionCoefficientTensor&) {
#pragma message("something needs to be done HERE")
    throw(
        std::runtime_error("CastemComputeThermalExpansionCoefficientTensor"
                           "<castem::SMALLSTRAINSTANDARDBEHAVIOUR,"
                           "tfel::material::ModellingHypothesis::PLANESTRESS,"
                           "ISOTROPIC>::exe : "
                           "unimplemented feature"));
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ISOTROPIC>::exe(const CastemReal* const props,
                      tfel::config::Types<2u, CastemReal, false>::
                          ThermalExpansionCoefficientTensor& C) {
    CastemComputeIsotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ISOTROPIC>::exe(const CastemReal* const props,
                      tfel::config::Types<2u, CastemReal, false>::
                          ThermalExpansionCoefficientTensor& C) {
    CastemComputeIsotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ISOTROPIC>::exe(const CastemReal* const props,
                      tfel::config::Types<2u, CastemReal, false>::
                          ThermalExpansionCoefficientTensor& C) {
    CastemComputeIsotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ISOTROPIC>::exe(const CastemReal* const props,
                      tfel::config::Types<3u, CastemReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    const CastemReal a = props[3];
    A(0) = A(1) = A(2) = a;
    A(3) = A(4) = A(5) = 0.;
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
      ORTHOTROPIC>::exe(const CastemReal* const props,
                        tfel::config::Types<1u, CastemReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    const CastemReal a0 = props[7];
    const CastemReal a1 = props[8];
    const CastemReal a2 = props[9];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC>::exe(const CastemReal* const,
                        tfel::config::Types<2u, CastemReal, false>::
                            ThermalExpansionCoefficientTensor&) {
#pragma message("something needs to be done HERE")
    throw(
        std::runtime_error("CastemComputeThermalExpansionCoefficientTensor"
                           "<castem::SMALLSTRAINSTANDARDBEHAVIOUR,"
                           "tfel::material::ModellingHypothesis::PLANESTRESS,"
                           "ORTHOTROPIC>::exe: "
                           "unimplemented feature"));
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ORTHOTROPIC>::exe(const CastemReal* const props,
                        tfel::config::Types<2u, CastemReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    CastemComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ORTHOTROPIC>::exe(const CastemReal* const props,
                        tfel::config::Types<2u, CastemReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    CastemComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ORTHOTROPIC>::exe(const CastemReal* const props,
                        tfel::config::Types<2u, CastemReal, false>::
                            ThermalExpansionCoefficientTensor& C) {
    CastemComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

  void CastemComputeThermalExpansionCoefficientTensor<
      castem::SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ORTHOTROPIC>::exe(const CastemReal* const props,
                        tfel::config::Types<3u, CastemReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    const CastemReal a0 = props[16];
    const CastemReal a1 = props[17];
    const CastemReal a2 = props[18];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
    A(3) = A(4) = A(5) = 0.;
  }  // end of struct CastemComputeThermalExpansionCoefficientTensor

}  // end of namespace castem
