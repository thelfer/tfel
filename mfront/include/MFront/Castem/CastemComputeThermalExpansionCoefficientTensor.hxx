/*!
 * \file
 * mfront/include/MFront/Castem/CastemComputeThermalExpansionCoefficientTensor.hxx
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

#ifndef LIB_MFRONT_CASTEMCOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_CASTEMCOMPUTETHERMALEXPANSIONTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Castem/Castem.hxx"
#include "MFront/Castem/CastemConfig.hxx"
#include "MFront/Castem/CastemTraits.hxx"

namespace castem {

  template <CastemBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis H,
            CastemSymmetryType>
  struct CastemComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<1u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ISOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<2u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ISOTROPIC> {
    TFEL_NORETURN static void exe(const CastemReal* const,
                                  tfel::config::Types<2u, CastemReal, false>::
                                      ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ISOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<2u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<2u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ISOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<3u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<1u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ORTHOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<2u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ORTHOTROPIC> {
    TFEL_NORETURN static void exe(const CastemReal* const,
                                  tfel::config::Types<2u, CastemReal, false>::
                                      ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<2u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<2u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT
      CastemComputeThermalExpansionCoefficientTensor<
          castem::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ORTHOTROPIC> {
    static void exe(const CastemReal* const,
                    tfel::config::Types<3u, CastemReal, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct CastemComputeThermalExpansionCoefficientTensor

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEMCOMPUTETHERMALEXPANSIONTENSOR_HXX */
