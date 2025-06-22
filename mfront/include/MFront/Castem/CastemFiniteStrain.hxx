/*!
 * \file  mfront/include/MFront/Castem/CastemFiniteStrain.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_CASTEMFINITESTRAIN_H_
#define LIB_MFRONT_CASTEM_CASTEMFINITESTRAIN_H_

#include "MFront/Castem/CastemConfig.hxx"
#include "MFront/Castem/Castem.hxx"

namespace castem {

  /*!
   * An helper structure providing utilities to implement various
   * finite strain strategies.
   */
  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemFiniteStrain {
    /*!
     * \brief compute the Green-Lagrange strain from the material
     * deformation gradient tensor F.
     * \param[in] e     : Green-Lagrange strain 0.5*(tFF-1) expressed using umat
     * conventions \param[in] F     : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in] NTENS : number of components of
     * the strain tensor \param[in] NDI   : modelling hypothesis
     */
    static void computeGreenLagrangeStrain(CastemReal* const,
                                           const CastemReal* const,
                                           const CastemInt,
                                           const CastemInt);
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
        CastemReal* const,
        const CastemReal* const,
        const CastemInt,
        const CastemInt,
        const CastemReal);
    /*!
     * \brief compute the second Piola Kirchhoff stress from the Cauchy stress.
     * \param[out] sk2    : second Piola Kirchhoff stress expressed using umat
     * conventions \param[in]  STRESS : cauchy stress expressed using umat
     * conventions \param[in]  F      : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in]  NTENS  : number of components
     * of the strain tensor \param[in]  NDI    : modelling hypothesis \param[in]
     * Fzz    : axial deformation gradient if mandatory
     */
    static void computeSecondPiolaKirchhoffStressFromCauchyStress(
        CastemReal* const,
        const CastemReal* const,
        const CastemReal* const,
        const CastemInt,
        const CastemInt,
        const CastemReal);
    /*!
     * \brief compute the second Piola Kirchhoff stress from the Cauchy stress.
     * \param[out] STRESS : second Piola Kirchhoff stress expressed using umat
     * conventions \param[in]  F      : material deformation gradient tensor F
     * expressed as a fortran matrix \param[in]  NTENS  : number of components
     * of the strain tensor \param[in]  NDI    : modelling hypothesis \param[in]
     * Fzz    : axial deformation gradient if mandatory
     */
    static void computeSecondPiolaKirchhoffStressFromCauchyStress(
        CastemReal* const,
        const CastemReal* const,
        const CastemInt,
        const CastemInt,
        const CastemReal);
    /*!
     * \brief compute the moduli associated to the Truesdell rate of
     *        the Cauchy stress
     * \param[in,out] DDSDDE: on input, CSE expressed using umat
     *                        conventions. On output, the moduli
     *                        associated to the Truesdell rate of the
     *                        Cauchy stress, expressed using umat
     *                        conventions.
     * \param[out] STRESS:    Cauchy stress expressed using umat
     *                        conventions
     * \param[in] F:          material deformation gradient tensor F
     *                        expressed as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  NDI    : modelling hypothesis
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */
    static void convertCSEtoCauchyTruesdellRateModuli(CastemReal* const,
                                                      const CastemReal* const,
                                                      const CastemReal* const,
                                                      const CastemInt,
                                                      const CastemInt,
                                                      const CastemReal);
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
    static void computeLogarithmicStrainAndDerivative(CastemReal* const,
                                                      CastemReal* const,
                                                      const CastemReal* const,
                                                      const CastemInt,
                                                      const CastemInt);
    /*!
     * \brief compute the stress dual to the logarithmic strain from the Cauchy
     * stress. \param[out] s      : stress dual to the logarithmic strain using
     * the umat conventions \param[in]  STRESS : second Piola Kirchhoff stress
     * expressed using umat conventions \param[in]  P      : derivative of the
     * logarithmic strain with respect to the right Cauchy tensor \param[in]  F
     * : material deformation gradient tensor F expressed as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  NDI    : modelling hypothesis
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */
    static void computeDualStressOfLogarithmicStrainFromCauchyStress(
        CastemReal* const,
        const CastemReal* const,
        const CastemReal* const,
        const CastemReal* const,
        const CastemInt,
        const CastemInt,
        const CastemReal);
    /*!
     * \brief compute the Cauchy stress from the stress dual to the logarithmic
     * strain. \param[out] STRESS : Cauchy stress expressed using the umat
     * conventions \param[in]  s      : stress dual to the logarithmic strain
     * using the umat conventions \param[in]  P      : derivative of the
     * logarithmic strain with respect to the right Cauchy tensor \param[in]  F
     * : material deformation gradient tensor F expressed as a fortran matrix
     * \param[in]  NTENS  : number of components of the strain tensor
     * \param[in]  NDI    : modelling hypothesis
     * \param[in]  Fzz    : axial deformation gradient if mandatory
     */
    static void computeCauchyStressFromDualStressOfLogarithmicStrain(
        CastemReal* const,
        const CastemReal* const,
        const CastemReal* const,
        const CastemReal* const,
        const CastemInt,
        const CastemInt,
        const CastemReal);

  };  // end of struct CastemFiniteStrain

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMFINITESTRAIN_H_ */
