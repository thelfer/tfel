/*!
 * \file   EuroplexusFiniteStrain.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   15 mars 2016
 */

#ifndef LIB_MFRONT_EUROPLEXUS_EUROPLEXUSFINITESTRAIN_HXX
#define LIB_MFRONT_EUROPLEXUS_EUROPLEXUSFINITESTRAIN_HXX

#include"MFront/Europlexus/Europlexus.hxx"
#include"MFront/Europlexus/EuroplexusConfig.hxx"

namespace epx{

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
  MFRONT_EPX_VISIBILITY_EXPORT void
  computeGreenLagrangeStrain(EuroplexusReal * const,
			     EuroplexusReal * const,
			     const EuroplexusReal * const,
			     const EuroplexusReal * const,
			     const EuroplexusInt);
  /*!
   * \brief compute the second Piola-Kirchoff stress from the Cauchy stress
   * \param[out] sig: second Piola-Kirchoff stress
   * \param[in]  STRESS: Cauchy stress
   * \param[in]  F0: deformation gradient at the beginning of the time step
   * \param[in]  h: hypothesis
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  computeSecondPiolaKirchhoffStressFromCauchyStress(EuroplexusReal * const,
						    const EuroplexusReal * const,
						    const EuroplexusReal * const,
						    const EuroplexusInt);
  /*!
   * \brief compute the Cauchy stress from the second Piola-Kirchoff stress
   * \param[out] STRESS: Cauchy stress
   * \param[in]  sig: second Piola-Kirchoff stress
   * \param[in]  F1: deformation gradient at the end of the time step
   * \param[in]  h: hypothesis
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  computeCauchyStressFromSecondPiolaKirchhoffStress(EuroplexusReal * const,
						    const EuroplexusReal * const,
						    const EuroplexusReal * const,
						    const EuroplexusInt);
  /*!
   * \param[in/out] D:  second Piola-Kirchoff stress derivative
   * \param[in]     s:  Cauchy stress
   * \param[in]     F1: deformation gradient at the end of the time step
   * \param[in]     h: hypothesis
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  computeCauchyStressDerivativeFromSecondPiolaKirchhoffStressDerivative(EuroplexusReal * const,
									const EuroplexusReal * const,
									const EuroplexusReal * const,
									const EuroplexusInt);

  MFRONT_EPX_VISIBILITY_EXPORT void
  computeLogarithmicStrainAndDerivative(EuroplexusReal * const,
					EuroplexusReal * const,
					EuroplexusReal * const,
					EuroplexusReal * const,
					const EuroplexusReal * const,
					const EuroplexusReal * const,
					const EuroplexusInt);
  
  MFRONT_EPX_VISIBILITY_EXPORT void
  computeDualStressOfLogarithmicStrainFromCauchyStress(EuroplexusReal * const,
						       const EuroplexusReal * const,
						       const EuroplexusReal * const,
						       const EuroplexusReal * const,
						       const EuroplexusInt);

  MFRONT_EPX_VISIBILITY_EXPORT void
  computeCauchyStressFromDualStressOfLogarithmicStrain(EuroplexusReal * const,
						       const EuroplexusReal * const,
						       const EuroplexusReal * const,
						       const EuroplexusReal * const,
						       const EuroplexusInt);
} // end of namespace epx

#endif /* LIB_MFRONT_EUROPLEXUS_EUROPLEXUSFINITESTRAIN_HXX_ */
