/*!
 * \file   include/TFEL/Material/FiniteStrainBehaviourTangentOperatorBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  11 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORBASE_H_
#define LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORBASE_H_

namespace tfel {

  namespace material {

    /*!
     * a dummy structure which encapsulate the Flag enum.
     */
    struct FiniteStrainBehaviourTangentOperatorBase {
      /*!
       * list all the supported type for tangent operator in finite
       * strain
       */
      enum Flag {
        DSIG_DF,        /*!< derivative of the Cauchy stress with respect to
                          the deformation gradient.
                          This is the tangent operator expected by mtest */
        DSIG_DDF,       /*!< derivative of the Cauchy stress with respect to
                          the deformation gradient increment
                          \f$\Delta\,F = F_{1}\,F_{0}^{-1}\f$ */
        C_TRUESDELL,    /*!< moduli associated with Truesdell rate of the Cauchy
                          stress    This is one of the tangent operator usable in
                          the ZeBuLoN finite element solver */
        SPATIAL_MODULI, /*!< moduli associated with the Lie derivative
                         *   of the Kirchhoff stress
                         */
        C_TAU_JAUMANN,  /*!< moduli associated with the jaumann
                         *   objective stress rate of the Kirchhoff
                         *   stress
                         */
        ABAQUS,         /*!< tangent moduli associated with the Jaumann rate
                          of the Kirchhoff stress divided by J. This is the
                          tangent operator for the Abaqus and Ansys finite
                          element solvers */
        DSIG_DDE,       /*!< derivative of the Cauchy stress with respect to
                          the small strain deformation increment*/
        DTAU_DF,        /*!< derivative of the Kirchhoff stress with respect to
                          the deformation gradient increment */
        DTAU_DDF,       /*!< derivative of the Kirchhoff stress with respect
                          to the deformation gradient increment
                          \f$\Delta\,F\f$ This is the deformation gradient
                          used by the Code-Aster finite element solver */
        DS_DF,          /*!< derivative of the second Piola-Kirchhoff stress
                          with respect to the deformation gradient */
        DS_DDF,         /*!< derivative of the second Piola-Kirchhoff stress
                          with respect to the deformation gradient
                          increment */
        DS_DC,          /*!< derivative of the second Piola-Kirchhoff stress
                          with respect to the right Cauchy-Green tensor */
        DS_DEGL         /*!< derivative of the Cauchy stress with respect to
                          the Green-Lagrange strain deformation */
      };                // end of Flag
    };

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORBASE_H_ */
