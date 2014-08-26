/*! 
 * \file  UMATFiniteStrain.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 févr. 2013
 */

#ifndef _LIB_MFRONT_UMAT_UMATFINITESTRAIN_H_
#define _LIB_MFRONT_UMAT_UMATFINITESTRAIN_H_ 

#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMAT.hxx"

namespace umat
{

  /*!
   * An helper structure providing utilities to implement various
   * finite strain strategies.
   */
  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATFiniteStrain
  {
    /*!
     * \brief compute the Green-Lagrange strain from the material
     * deformation gradient tensor F.
     * \param[in] e     : Green-Lagrange strain 0.5*(tFF-1) expressed using umat conventions
     * \param[in] F     : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in] NTENS : number of components of the strain tensor
     * \param[in] NDI   : modelling hypothesis
     */    
    static void
    computeGreenLagrangeStrain(UMATReal* const,
			       const UMATReal* const,
			       const UMATInt,
			       const UMATInt);
    /*!
     * \brief compute the Cauchy stress tensor from the second Piola
     * Kirchoff stress.
     * \param[in,out] s     : stress expressed using umat conventions
     * \param[in]     F     : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]     NTENS : number of components of the strain tensor
     * \param[in]     NDI   : modelling hypothesis
     */    
    static void
    computeCauchyStressFromSecondPiolaKirchhoffStress(UMATReal* const,
						      const UMATReal* const,
						      const UMATInt,
						      const UMATInt);
    /*!
     * \brief compoute the logarithmic strain and its derivatives
     * \param[out] P     : derivative of the logarithmic strain with
     * respect to the right cauchy tensor, multiplied by two
     * \param[out] E     : logarithmic strain expressed using umat conventions
     * \param[in]  F     : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS : number of components of the strain tensor
     * \param[in]  NDI   : modelling hypothesis
     */
    static void
    computeLogarithmicStrainAndDerivative(UMATReal* const,
					  UMATReal* const,
					  const UMATReal* const,
					  const UMATInt,
					  const UMATInt);

    static void
    computeDualStressOfLogarithmicStrainFromCauchyStress(UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATInt,
							 const UMATInt);
    
    static void
    computeCauchyStressFromDualStressOfLogarithmicStrain(UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATInt,
							 const UMATInt);

  }; // end of struct UMATFiniteStrain

} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATFINITESTRAIN_H */

