/*!
 * \file
 * mfront/include/MFront/Aster/AsterComputeThermalExpansionCoefficientTensor.hxx
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

#ifndef LIB_MFRONT_ASTERCOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_ASTERCOMPUTETHERMALEXPANSIONTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterConfig.hxx"
#include "MFront/Aster/AsterTraits.hxx"

namespace aster {

  template <AsterBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis H,
            AsterSymmetryType>
  struct AsterComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<1u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ISOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ISOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ISOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ISOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<3u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<1u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ORTHOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ORTHOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<2u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterComputeThermalExpansionCoefficientTensor<
          aster::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ORTHOTROPIC> {
    static void exe(const AsterReal* const,
                    tfel::config::Types<3u, AsterReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AsterComputeThermalExpansionCoefficientTensor

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTERCOMPUTETHERMALEXPANSIONTENSOR_HXX */
