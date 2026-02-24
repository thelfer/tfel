/*!
 * \file
 * mfront/include/MFront/LSDYNA/LSDYNAComputeThermalExpansionCoefficientTensor.hxx
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

#ifndef LIB_MFRONT_LSDYNACOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_LSDYNACOMPUTETHERMALEXPANSIONTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/LSDYNA/LSDYNA.hxx"
#include "MFront/LSDYNA/LSDYNAConfig.hxx"
#include "MFront/LSDYNA/LSDYNATraits.hxx"

namespace lsdyna {

  template <LSDYNABehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis H,
            LSDYNASymmetryType>
  struct LSDYNAComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAComputeThermalExpansionCoefficientTensor<
          lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct LSDYNAComputeThermalExpansionCoefficientTensor

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNACOMPUTETHERMALEXPANSIONTENSOR_HXX */
