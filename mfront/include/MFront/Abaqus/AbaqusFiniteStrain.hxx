/*!
 * \file  mfront/include/MFront/Abaqus/AbaqusFiniteStrain.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  21/08/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSFINITESTRAIN_H_
#define LIB_MFRONT_ABAQUS_ABAQUSFINITESTRAIN_H_

#include "MFront/Abaqus/AbaqusConfig.hxx"
#include "MFront/Abaqus/Abaqus.hxx"

namespace abaqus {

  /*!
   * An helper structure providing utilities to implement various
   * finite strain strategies.
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusFiniteStrain {
    /*!
     * \brief compute the Green-Lagrange strain from the material
     * deformation gradient tensor F.
     * \param[in] e     : Green-Lagrange strain 0.5*(tFF-1) expressed using umat
     * conventions \param[in] F     : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in] NTENS : number of components of
     * the strain tensor \param[in] NDI   : modelling hypothesis
     */
    static void computeGreenLagrangeStrain(AbaqusReal* const,
                                           const AbaqusReal* const,
                                           const AbaqusInt,
                                           const bool);
    /*!
     * \brief compute the Cauchy stress tensor from the second Piola
     * Kirchhoff stress.
     * \param[in,out] s     : stress expressed using umat conventions
     * \param[in]     F     : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]     NTENS : number of components of the strain tensor
     * \param[in]     NDI   : modelling hypothesis
     * \param[in]     Fzz   : axial deformation gradient if mandatory
     */
    static void computeCauchyStressFromSecondPiolaKirchhoffStress(
        AbaqusReal* const,
        const AbaqusReal* const,
        const AbaqusInt,
        const bool,
        const AbaqusReal);
    /*!
     * \brief compute the second Piola Kirchhoff stress from the Cauchy stress.
     * \param[out] sk2    : second Piola Kirchhoff stress expressed using umat
     * conventions \param[in]  STRESS : cauchy stress expressed using umat
     * conventions \param[in]  F      : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in]  NTENS  : number of components
     * of the strain tensor \param[in]  ps: if true, plane stress \param[in] Fzz
     * : axial deformation gradient if mandatory
     */
    static void computeSecondPiolaKirchhoffStressFromCauchyStress(
        AbaqusReal* const,
        const AbaqusReal* const,
        const AbaqusReal* const,
        const AbaqusInt,
        const bool,
        const AbaqusReal);
    /*!
     * \brief compute the second Piola Kirchhoff stress from the Cauchy stress.
     * \param[out] STRESS : second Piola Kirchhoff stress expressed using umat
     * conventions \param[in]  F      : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in]  NTENS  : number of components
     * of the strain tensor \param[in]  ps: if true, plane stress \param[in] Fzz
     * : axial deformation gradient if mandatory
     */
    static void computeSecondPiolaKirchhoffStressFromCauchyStress(
        AbaqusReal* const,
        const AbaqusReal* const,
        const AbaqusInt,
        const bool,
        const AbaqusReal);
    /*!
     * \brief convert the tangent operator to the abaqus one
     * \param[out] D:     abaqus tangent operator
     * \param[in]  CSE:   tangent operator
     * \param[in]  F1:    deformation gradient
     * \param[in]  s:     stress tensor
     * \param[in]  NTENS: number of components of the strain tensor
     * \param[in]  ps:    plane stress
     */
    static void computeAbaqusTangentOperatorFromCSE(AbaqusReal* const,
                                                    const AbaqusReal* const,
                                                    const AbaqusReal* const,
                                                    const AbaqusReal* const,
                                                    const AbaqusInt,
                                                    const bool);
    /*!
     * \brief apply a correction to the consistent tangent operator to
     * take into account that the real operator is not the derivative
     * of the Cauchy stress with respect to the deformation increment
     * but the derivative of the Kirchhoff stress divided but the
     * determinant of the jacobian;
     *
     * \param[out/in] DDSDDE: on input, the derivative of the Cauchy
     *                        stress with respect to the deformation
     *                        increment. On output, the expected
     *                        tangent operator
     * \param[in] F1:    deformation gradient at the end of the time step
     * \param[in] s:     Cauchy stress at the end of the time step
     * \param[in] NTENS: number of components of the stresses
     */
    static void applyNativeFiniteStrainCorrection(AbaqusReal*,
                                                  const AbaqusReal* const,
                                                  const AbaqusReal* const,
                                                  const AbaqusInt);
  };  // end of struct AbaqusFiniteStrain

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSFINITESTRAIN_H_ */
