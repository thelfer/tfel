/*!
 * \file   mfront/src/CyranoComputeThermalExpansionCoefficientTensor.cxx
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

#include "TFEL/Math/stensor.hxx"
#include "MFront/Cyrano/CyranoComputeThermalExpansionCoefficientTensor.hxx"

namespace cyrano {
  void CyranoComputeThermalExpansionCoefficientTensor<
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
      ISOTROPIC>::exe(const CyranoReal* const props,
                      tfel::config::Types<1u, CyranoReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    const CyranoReal a = props[0];
    A(0) = A(1) = A(2) = a;
  }  // end of struct CyranoComputeThermalExpansionCoefficientTensor

  void CyranoComputeThermalExpansionCoefficientTensor<
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
      ORTHOTROPIC>::exe(const CyranoReal* const props,
                        tfel::config::Types<1u, CyranoReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    const CyranoReal a0 = props[0];
    const CyranoReal a1 = props[1];
    const CyranoReal a2 = props[2];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
  }  // end of struct CyranoComputeThermalExpansionCoefficientTensor

  void CyranoComputeThermalExpansionCoefficientTensor<
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
      ISOTROPIC>::exe(const CyranoReal* const props,
                      tfel::config::Types<1u, CyranoReal, false>::
                          ThermalExpansionCoefficientTensor& A) {
    const CyranoReal a = props[0];
    A(0) = A(1) = A(2) = a;
  }  // end of struct CyranoComputeThermalExpansionCoefficientTensor

  void CyranoComputeThermalExpansionCoefficientTensor<
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
      ORTHOTROPIC>::exe(const CyranoReal* const props,
                        tfel::config::Types<1u, CyranoReal, false>::
                            ThermalExpansionCoefficientTensor& A) {
    const CyranoReal a0 = props[0];
    const CyranoReal a1 = props[1];
    const CyranoReal a2 = props[2];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
  }  // end of struct CyranoComputeThermalExpansionCoefficientTensor

}  // end of namespace cyrano
