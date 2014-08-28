/*! 
 * \file  mfront/include/MFront/UMAT/UMATFiniteStrain.hxx
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
     * \param[in]     Fzz   : axial deformation gradient if mandatory
     */    
    static void
    computeCauchyStressFromSecondPiolaKirchhoffStress(UMATReal* const,
						      const UMATReal* const,
						      const UMATInt,
						      const UMATInt,
						      const UMATReal);
    /*!
     * \brief compute the second Piola Kirchoff stress from the Cauchy stress.
     * \param[out] sk2    : second Piola Kirchoff stress expressed using umat conventions
     * \param[in]  STRESS : cauchy stress expressed using umat conventions
     * \param[in]  F      : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  NDI    : modelling hypothesis
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */    
    static void
    computeSecondPiolaKirchhoffStressFromCauchyStress(UMATReal* const,
						      const UMATReal* const,
						      const UMATReal* const,
						      const UMATInt,
						      const UMATInt,
						      const UMATReal);
    /*!
     * \brief compute the second Piola Kirchoff stress from the Cauchy stress.
     * \param[out] STRESS : second Piola Kirchoff stress expressed using umat conventions
     * \param[in]  F      : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  NDI    : modelling hypothesis
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */    
    static void
    computeSecondPiolaKirchhoffStressFromCauchyStress(UMATReal* const,
						      const UMATReal* const,
						      const UMATInt,
						      const UMATInt,
						      const UMATReal);
    /*!
     * \brief compoute the logarithmic strain and its derivatives
     * \param[out] P     : derivative of the logarithmic strain with
     * respect to the right cauchy tensor, multiplied by two
     * \param[out] E     : logarithmic strain expressed using umat conventions
     * \param[in]  F     : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS : number of components of the strain tensor
     * \param[in]  NDI   : modelling hypothesis
     * \note this works in plane stress even though the axial
     * deformation gradient is not known
     */
    static void
    computeLogarithmicStrainAndDerivative(UMATReal* const,
					  UMATReal* const,
					  const UMATReal* const,
					  const UMATInt,
					  const UMATInt);
    /*!
     * \brief compute the stress dual to the logarithmic strain from the Cauchy stress.
     * \param[out] s      : stress dual to the logarithmic strain using the umat conventions
     * \param[in]  STRESS : second Piola Kirchoff stress expressed using umat conventions
     * \param[in]  P      : derivative of the logarithmic strain with respect to the right Cauchy tensor
     * \param[in]  F      : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  NDI    : modelling hypothesis
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */    
    static void
    computeDualStressOfLogarithmicStrainFromCauchyStress(UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATInt,
							 const UMATInt,
							 const UMATReal);
    /*!
     * \brief compute the Cauchy stress from the stress dual to the logarithmic strain.
     * \param[out] STRESS : Cauchy stress expressed using the umat conventions
     * \param[in]  s      : stress dual to the logarithmic strain using the umat conventions
     * \param[in]  P      : derivative of the logarithmic strain with respect to the right Cauchy tensor
     * \param[in]  F      : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  NDI    : modelling hypothesis
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */    
    static void
    computeCauchyStressFromDualStressOfLogarithmicStrain(UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATReal* const,
							 const UMATInt,
							 const UMATInt,
							 const UMATReal);

  }; // end of struct UMATFiniteStrain

} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATFINITESTRAIN_H */

