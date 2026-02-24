/*!
 * \file  mfront/include/MFront/LSDYNA/LSDYNAFiniteStrain.hxx
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

#ifndef LIB_MFRONT_LSDYNA_LSDYNAFINITESTRAIN_HXX
#define LIB_MFRONT_LSDYNA_LSDYNAFINITESTRAIN_HXX

#include "MFront/LSDYNA/LSDYNAConfig.hxx"
#include "MFront/LSDYNA/LSDYNA.hxx"

namespace lsdyna {

  /*!
   * An helper structure providing utilities to implement various
   * finite strain strategies.
   */
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT LSDYNAFiniteStrain {
    /*!
     * \brief compute the Green-Lagrange strain from the material
     * deformation gradient tensor F.
     * \param[in] e     : Green-Lagrange strain 0.5*(tFF-1) expressed using umat
     * conventions
     * \param[in] F     : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in] NTENS : number of components of the strain tensor
     * \param[in] NDI   : modelling hypothesis
     */
    static void computeGreenLagrangeStrain(LSDYNAReal* const,
                                           const LSDYNAReal* const,
                                           const LSDYNAInt,
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
        LSDYNAReal* const,
        const LSDYNAReal* const,
        const LSDYNAInt,
        const bool,
        const LSDYNAReal);
    /*!
     * \brief compute the second Piola Kirchhoff stress from the Cauchy stress.
     * \param[out] sk2    : second Piola Kirchhoff stress expressed using umat
     * conventions
     * \param[in]  STRESS : cauchy stress expressed using umat conventions
     * \param[in]  F      : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  ps: if true, plane stress
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */
    static void computeSecondPiolaKirchhoffStressFromCauchyStress(
        LSDYNAReal* const,
        const LSDYNAReal* const,
        const LSDYNAReal* const,
        const LSDYNAInt,
        const bool,
        const LSDYNAReal);
    /*!
     * \brief compute the second Piola Kirchhoff stress from the Cauchy stress.
     * \param[out] STRESS : second Piola Kirchhoff stress expressed using umat
     * conventions
     * \param[in]  F      : material deformation gradient tensor F expressed
     * as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  ps: if true, plane stress
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */
    static void computeSecondPiolaKirchhoffStressFromCauchyStress(
        LSDYNAReal* const,
        const LSDYNAReal* const,
        const LSDYNAInt,
        const bool,
        const LSDYNAReal);
  };  // end of struct LSDYNAFiniteStrain

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNA_LSDYNAFINITESTRAIN_HXX */
