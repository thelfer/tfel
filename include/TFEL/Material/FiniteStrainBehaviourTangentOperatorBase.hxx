/*! 
 * \file   FiniteStrainBehaviourTangentOperatorBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  11 juin 2014
 */

#ifndef _LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORBASE_H_
#define _LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORBASE_H_ 

namespace tfel
{

  namespace material
  {

    /*!
     * a dummy structure which encapsulate the Flag enum.
     */
    struct FiniteStrainBehaviourTangentOperatorBase
    {
      /*!
       * list all the supported type for tangent operator in finite
       * strain
       */
      enum Flag{
	DSIG_DF,  /*!< derivative of the Cauchy stress with respect to
		    the deformation gradient.
		    This is the tangent operator expected by mtest */
	DSIG_DDF, /*!< derivative of the Cauchy stress with respect to
		    the deformation gradient increment
		    \(\Delta\,F = F_{1}\,F_{0}^{-1}\)  */
	DSIG_DD,  /*!< derivative of the Cauchy stress with respect to
		    the deformation rate 
		    \(D=\frac{1}{2}\left(L+L^{tr}\right)\)
		    with
		    \(L=\dot{F}\,F^{-1}\)
		    This is one of the tangenet operator usable in
		    the ZeBuLoN finite element solver */
	DSIG_DDE, /*!< derivative of the Cauchy stress with respect to
		    the small strain deformation increment*/
	DTAU_DF,  /*!< derivative of the Kirchhoff stress with respect to
		    the deformation gradient increment */
	DTAU_DDF, /*!< derivative of the Kirchhoff stress with respect
		    to the deformation gradient increment
		    \(\Delta\,F\) This is the deformation gradient
		    used by the Code-Aster finite element solver */
	DS_DF,    /*!< derivative of the second Piola-Kirchoff stress
		    with respect to the deformation gradient */
	DS_DDF,   /*!< derivative of the second Piola-Kirchoff stress
		    with respect to the deformation gradient 
		    increment */
	DS_DC,    /*!< derivative of the second Piola-Kirchoff stress
		    with respect to the right Cauchy-Green tensor */
	DS_DEGL   /*!< derivative of the Cauchy stress with respect to
		    the Green-Lagrange strain deformation */
      }; // end of Flag
    };

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_H */

