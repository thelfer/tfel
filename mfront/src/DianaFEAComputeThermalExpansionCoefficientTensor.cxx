/*!
 * \file   mfront/src/DianaFEAComputeThermalExpansionCoefficientTensor.cxx
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
#include "MFront/DianaFEA/DianaFEAComputeThermalExpansionCoefficientTensor.hxx"

#define ANSYSCOMPUTEISOTROPICTHERMALEXPANSIONCOEFFICIENTTENSORIMP(real)        \
  static void DianaFEAComputeIsotropicThermalExpansionCoefficientTensor2D(     \
      const real* const props,                                                 \
      tfel::config::Types<2u, real, false>::ThermalExpansionCoefficientTensor& \
          A) {                                                                 \
    const real a = props[0];                                                   \
    A(0) = A(1) = A(2) = a;                                                    \
    A(3) = 0.;                                                                 \
  }                                                                            \
                                                                               \
  static void DianaFEAComputeOrthotropicThermalExpansionCoefficientTensor2D(   \
      const real* const props,                                                 \
      tfel::config::Types<2u, real, false>::ThermalExpansionCoefficientTensor& \
          A) {                                                                 \
    const real a0 = props[0];                                                  \
    const real a1 = props[1];                                                  \
    const real a2 = props[2];                                                  \
    A(0) = a0;                                                                 \
    A(1) = a1;                                                                 \
    A(2) = a2;                                                                 \
    A(3) = 0.;                                                                 \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::PLANESTRESS,                        \
      ISOTROPIC>::exe(const real* const props,                                 \
                      tfel::config::Types<2u, real, false>::                   \
                          ThermalExpansionCoefficientTensor& A) {              \
    DianaFEAComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);     \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,                     \
      ISOTROPIC>::exe(const real* const props,                                 \
                      tfel::config::Types<2u, real, false>::                   \
                          ThermalExpansionCoefficientTensor& A) {              \
    DianaFEAComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);     \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::PLANESTRAIN,                        \
      ISOTROPIC>::exe(const real* const props,                                 \
                      tfel::config::Types<2u, real, false>::                   \
                          ThermalExpansionCoefficientTensor& A) {              \
    DianaFEAComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);     \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,             \
      ISOTROPIC>::exe(const real* const props,                                 \
                      tfel::config::Types<2u, real, false>::                   \
                          ThermalExpansionCoefficientTensor& A) {              \
    DianaFEAComputeIsotropicThermalExpansionCoefficientTensor2D(props, A);     \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,                     \
      ISOTROPIC>::exe(const real* const props,                                 \
                      tfel::config::Types<3u, real, false>::                   \
                          ThermalExpansionCoefficientTensor& A) {              \
    const real a = props[0];                                                   \
    A(0) = A(1) = A(2) = a;                                                    \
    A(3) = A(4) = A(5) = 0.;                                                   \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::PLANESTRESS,                        \
      ORTHOTROPIC>::exe(const real* const props,                               \
                        tfel::config::Types<2u, real, false>::                 \
                            ThermalExpansionCoefficientTensor& A) {            \
    DianaFEAComputeOrthotropicThermalExpansionCoefficientTensor2D(props, A);   \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,                     \
      ORTHOTROPIC>::exe(const real* const props,                               \
                        tfel::config::Types<2u, real, false>::                 \
                            ThermalExpansionCoefficientTensor& C) {            \
    DianaFEAComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);   \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::PLANESTRAIN,                        \
      ORTHOTROPIC>::exe(const real* const props,                               \
                        tfel::config::Types<2u, real, false>::                 \
                            ThermalExpansionCoefficientTensor& C) {            \
    DianaFEAComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);   \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,             \
      ORTHOTROPIC>::exe(const real* const props,                               \
                        tfel::config::Types<2u, real, false>::                 \
                            ThermalExpansionCoefficientTensor& C) {            \
    DianaFEAComputeOrthotropicThermalExpansionCoefficientTensor2D(props, C);   \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */       \
                                                                               \
  void DianaFEAComputeThermalExpansionCoefficientTensor<                       \
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,                                  \
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,                     \
      ORTHOTROPIC>::exe(const real* const props,                               \
                        tfel::config::Types<3u, real, false>::                 \
                            ThermalExpansionCoefficientTensor& A) {            \
    const real a0 = props[0];                                                  \
    const real a1 = props[1];                                                  \
    const real a2 = props[2];                                                  \
    A(0) = a0;                                                                 \
    A(1) = a1;                                                                 \
    A(2) = a2;                                                                 \
    A(3) = A(4) = A(5) = 0.;                                                   \
  } /* end of struct DianaFEAComputeThermalExpansionCoefficientTensor */

namespace dianafea {

  ANSYSCOMPUTEISOTROPICTHERMALEXPANSIONCOEFFICIENTTENSORIMP(float)
  ANSYSCOMPUTEISOTROPICTHERMALEXPANSIONCOEFFICIENTTENSORIMP(double)

}  // end of namespace dianafea
