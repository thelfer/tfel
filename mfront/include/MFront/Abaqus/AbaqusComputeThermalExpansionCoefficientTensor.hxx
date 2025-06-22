/*!
 * \file
 * mfront/include/MFront/Abaqus/AbaqusComputeThermalExpansionCoefficientTensor.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   01 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUSCOMPUTETHERMALEXPANSIONTENSOR_HXX_
#define LIB_MFRONT_ABAQUSCOMPUTETHERMALEXPANSIONTENSOR_HXX_

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusConfig.hxx"
#include "MFront/Abaqus/AbaqusTraits.hxx"

namespace abaqus {

  template <AbaqusBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis H,
            AbaqusSymmetryType>
  struct AbaqusComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusComputeThermalExpansionCoefficientTensor<
          abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AbaqusComputeThermalExpansionCoefficientTensor

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUSCOMPUTETHERMALEXPANSIONTENSOR_HXX_ */
