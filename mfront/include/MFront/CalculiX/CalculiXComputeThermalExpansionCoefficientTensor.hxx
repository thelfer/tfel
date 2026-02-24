/*!
 * \file
 * mfront/include/MFront/CalculiX/CalculiXComputeThermalExpansionCoefficientTensor.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CALCULIXCOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_CALCULIXCOMPUTETHERMALEXPANSIONTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXConfig.hxx"
#include "MFront/CalculiX/CalculiXTraits.hxx"

namespace calculix {

  template <CalculiXSymmetryType>
  struct CalculiXComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
      CalculiXComputeThermalExpansionCoefficientTensor<ISOTROPIC> {
    static void exe(const CalculiXReal* const,
                    tfel::config::Types<3u, CalculiXReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
      CalculiXComputeThermalExpansionCoefficientTensor<ORTHOTROPIC> {
    static void exe(const CalculiXReal* const,
                    tfel::config::Types<3u, CalculiXReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CalculiXComputeThermalExpansionCoefficientTensor

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIXCOMPUTETHERMALEXPANSIONTENSOR_HXX */
