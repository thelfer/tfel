/*!
 * \file
 * mfront/include/MFront/DianaFEA/DianaFEAComputeThermalExpansionCoefficientTensor.hxx
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

#ifndef LIB_MFRONT_DIANAFEACOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_DIANAFEACOMPUTETHERMALEXPANSIONTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/DianaFEA/DianaFEA.hxx"
#include "MFront/DianaFEA/DianaFEAConfig.hxx"
#include "MFront/DianaFEA/DianaFEATraits.hxx"

namespace dianafea {

  template <DianaFEABehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis H,
            DianaFEASymmetryType>
  struct DianaFEAComputeThermalExpansionCoefficientTensor;

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ISOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::
              AXISYMMETRICALGENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<1u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<1u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::AXISYMMETRICAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRESS,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::PLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<2u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<2u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

  template <>
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT
      DianaFEAComputeThermalExpansionCoefficientTensor<
          dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          ORTHOTROPIC> {
    static void exe(const float* const,
                    tfel::config::Types<3u, float, false>::
                        ThermalExpansionCoefficientTensor&);
    static void exe(const double* const,
                    tfel::config::Types<3u, double, false>::
                        ThermalExpansionCoefficientTensor&);
  };  // end of struct DianaFEAComputeThermalExpansionCoefficientTensor

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEACOMPUTETHERMALEXPANSIONTENSOR_HXX */
