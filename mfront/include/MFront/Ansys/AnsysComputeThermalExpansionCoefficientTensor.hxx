/*!
 * \file
 * mfront/include/MFront/Ansys/AnsysComputeThermalExpansionCoefficientTensor.hxx
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

#ifndef LIB_MFRONT_ANSYSCOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_ANSYSCOMPUTETHERMALEXPANSIONTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Ansys/Ansys.hxx"
#include "MFront/Ansys/AnsysConfig.hxx"
#include "MFront/Ansys/AnsysTraits.hxx"

namespace ansys {

  template <AnsysBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis H,
            AnsysSymmetryType>
  struct AnsysComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysComputeThermalExpansionCoefficientTensor<
          ansys::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct AnsysComputeThermalExpansionCoefficientTensor

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYSCOMPUTETHERMALEXPANSIONTENSOR_HXX */
