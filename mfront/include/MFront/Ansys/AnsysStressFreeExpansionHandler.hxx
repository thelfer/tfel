/*!
 * \file   mfront/include/MFront/Ansys/AnsysStressFreeExpansionHandler.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  05 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ANSYS_ANSYSSTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_ANSYS_ANSYSSTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/Ansys/Ansys.hxx"
#include "MFront/Ansys/AnsysConfig.hxx"

namespace ansys {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Ansys conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Ansys conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysStandardSmallStrainStressFreeExpansionHandler(float *const,
                                                     float *const,
                                                     const float *const,
                                                     const float *const,
                                                     const AnsysInt);
  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Ansys conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Ansys conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysStandardSmallStrainStressFreeExpansionHandler(double *const,
                                                     double *const,
                                                     const double *const,
                                                     const double *const,
                                                     const AnsysInt);
  /*!
   * standard handler of stress free expansion handler for logarithmic
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Ansys conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Ansys conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysLogarithmicStrainStressFreeExpansionHandler(float *const,
                                                   float *const,
                                                   const float *const,
                                                   const float *const,
                                                   const AnsysInt);
  /*!
   * standard handler of stress free expansion handler for logarithmic
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Ansys conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Ansys conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysLogarithmicStrainStressFreeExpansionHandler(double *const,
                                                   double *const,
                                                   const double *const,
                                                   const double *const,
                                                   const AnsysInt);

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYS_ANSYSSTRESSFREEEXPANSIONHANDLER_HXX */
