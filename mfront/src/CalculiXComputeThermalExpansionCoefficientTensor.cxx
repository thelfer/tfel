/*!
 * \file   mfront/src/CalculiXComputeThermalExpansionCoefficientTensor.cxx
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
#include "MFront/CalculiX/CalculiXComputeThermalExpansionCoefficientTensor.hxx"

namespace calculix {

  void CalculiXComputeThermalExpansionCoefficientTensor<ISOTROPIC>::exe(
      const CalculiXReal* const props,
      tfel::config::Types<3u, CalculiXReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const auto a = props[0];
    A(0) = A(1) = A(2) = a;
    A(3) = A(4) = A(5) = CalculiXReal(0);
  }  // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  void CalculiXComputeThermalExpansionCoefficientTensor<ORTHOTROPIC>::exe(
      const CalculiXReal* const props,
      tfel::config::Types<3u, CalculiXReal, false>::
          ThermalExpansionCoefficientTensor& A) {
    const auto a0 = props[0];
    const auto a1 = props[1];
    const auto a2 = props[2];
    A(0) = a0;
    A(1) = a1;
    A(2) = a2;
    A(3) = A(4) = A(5) = CalculiXReal(0);
  }  // end of struct CalculiXComputeThermalExpansionCoefficientTensor

}  // end of namespace calculix
