/*!
 * \file
 * mfront/include/MFront/Europlexus/EuroplexusComputeThermalExpansionCoefficientTensor.hxx
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

#ifndef LIB_MFRONT_EPXCOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_EPXCOMPUTETHERMALEXPANSIONTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Europlexus/Europlexus.hxx"
#include "MFront/Europlexus/EuroplexusConfig.hxx"
#include "MFront/Europlexus/EuroplexusTraits.hxx"

namespace epx {

  template <EuroplexusBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis H,
            EuroplexusSymmetryType>
  struct EuroplexusComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<1u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ISOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ISOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ISOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ISOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<3u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<1u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ORTHOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ORTHOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<2u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusComputeThermalExpansionCoefficientTensor<
          epx::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ORTHOTROPIC> {
    static void exe(const EuroplexusReal* const,
                    tfel::config::Types<3u, EuroplexusReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct EuroplexusComputeThermalExpansionCoefficientTensor

}  // end of namespace epx

#endif /* LIB_MFRONT_EPXCOMPUTETHERMALEXPANSIONTENSOR_HXX */
