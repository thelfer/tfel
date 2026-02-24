/*!
 * \file  mfront/include/MFront/Ansys/AnsysFiniteStrain.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  21/08/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ANSYS_ANSYSFINITESTRAIN_HXX
#define LIB_MFRONT_ANSYS_ANSYSFINITESTRAIN_HXX

#include "MFront/Ansys/AnsysConfig.hxx"
#include "MFront/Ansys/Ansys.hxx"

namespace ansys {

  /*!
   * An helper structure providing utilities to implement various
   * finite strain strategies.
   */
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysFiniteStrain {
    /*!
     * \brief compute the Green-Lagrange strain from the material
     * deformation gradient tensor F.
     * \param[in] e     : Green-Lagrange strain 0.5*(tFF-1) expressed using umat
     * conventions \param[in] F     : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in] NTENS : number of components of
     * the strain tensor \param[in] NDI   : modelling hypothesis
     */
    static void computeGreenLagrangeStrain(AnsysReal* const,
                                           const AnsysReal* const,
                                           const AnsysInt,
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
        AnsysReal* const,
        const AnsysReal* const,
        const AnsysInt,
        const bool,
        const AnsysReal);
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
        AnsysReal* const,
        const AnsysReal* const,
        const AnsysReal* const,
        const AnsysInt,
        const bool,
        const AnsysReal);
    /*!
     * \brief compute the second Piola Kirchhoff stress from the Cauchy stress.
     * \param[out] STRESS : second Piola Kirchhoff stress expressed using umat
     * conventions \param[in]  F      : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in]  NTENS  : number of components
     * of the strain tensor \param[in]  ps: if true, plane stress \param[in] Fzz
     * : axial deformation gradient if mandatory
     */
    static void computeSecondPiolaKirchhoffStressFromCauchyStress(
        AnsysReal* const,
        const AnsysReal* const,
        const AnsysInt,
        const bool,
        const AnsysReal);
    /*!
     * \brief convert the tangent operator to the ansys one
     * \param[out] D:     ansys tangent operator
     * \param[in]  CSE:   tangent operator
     * \param[in]  F1:    deformation gradient
     * \param[in]  s:     stress tensor
     * \param[in]  NTENS: number of components of the strain tensor
     * \param[in]  ps:    plane stress
     */
    static void computeAnsysTangentOperatorFromCSE(AnsysReal* const,
                                                   const AnsysReal* const,
                                                   const AnsysReal* const,
                                                   const AnsysReal* const,
                                                   const AnsysInt,
                                                   const bool);
  };  // end of struct AnsysFiniteStrain

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYS_ANSYSFINITESTRAIN_HXX */
