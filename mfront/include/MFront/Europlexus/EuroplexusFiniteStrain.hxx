/*!
 * \file   EuroplexusFiniteStrain.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_EUROPLEXUS_EUROPLEXUSFINITESTRAIN_HXX
#define LIB_MFRONT_EUROPLEXUS_EUROPLEXUSFINITESTRAIN_HXX

#include "MFront/Europlexus/Europlexus.hxx"
#include "MFront/Europlexus/EuroplexusConfig.hxx"

namespace epx {

  /*!
   * \brief compute the Green-Lagrange strain at the beginning of the
   * time step and its increment over the time step.
   * \param[out] eto: Green-Lagrange strain at the beginning of the
   * time step
   * \param[out] deto: Green-Lagrange strain increment
   * \param[in]  F0: deformation gradient at the beginning of the time step
   * \param[in]  F1: deformation gradient at the end of the time step
   * \param[in]  h: hypothesis
   */
  MFRONT_EPX_VISIBILITY_EXPORT void computeGreenLagrangeStrain(
      EuroplexusReal* const,
      EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusInt);
  /*!
   * \brief compute the second Piola-Kirchhoff stress from the Cauchy stress
   * \param[out] sig: second Piola-Kirchhoff stress
   * \param[in]  STRESS: Cauchy stress
   * \param[in]  F0: deformation gradient at the beginning of the time step
   * \param[in]  h: hypothesis
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  computeSecondPiolaKirchhoffStressFromCauchyStress(EuroplexusReal* const,
                                                    const EuroplexusReal* const,
                                                    const EuroplexusReal* const,
                                                    const EuroplexusInt);
  /*!
   * \brief compute the Cauchy stress from the second Piola-Kirchhoff stress
   * \param[out] STRESS: Cauchy stress
   * \param[in]  sig: second Piola-Kirchhoff stress
   * \param[in]  F1: deformation gradient at the end of the time step
   * \param[in]  h: hypothesis
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  computeCauchyStressFromSecondPiolaKirchhoffStress(EuroplexusReal* const,
                                                    const EuroplexusReal* const,
                                                    const EuroplexusReal* const,
                                                    const EuroplexusInt);
  /*!
   * \param[in,out] D:  second Piola-Kirchhoff stress derivative
   * \param[in]     h: hypothesis
   */
  MFRONT_EPX_VISIBILITY_EXPORT void computeElasticModuli(EuroplexusReal* const,
                                                         const EuroplexusInt);

  MFRONT_EPX_VISIBILITY_EXPORT void computeLogarithmicStrainAndDerivative(
      EuroplexusReal* const,
      EuroplexusReal* const,
      EuroplexusReal* const,
      EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusInt);

  MFRONT_EPX_VISIBILITY_EXPORT void
  computeDualStressOfLogarithmicStrainFromCauchyStress(
      EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusInt);

  MFRONT_EPX_VISIBILITY_EXPORT void
  computeCauchyStressFromDualStressOfLogarithmicStrain(
      EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusReal* const,
      const EuroplexusInt);
}  // end of namespace epx

#endif /* LIB_MFRONT_EUROPLEXUS_EUROPLEXUSFINITESTRAIN_HXX */
