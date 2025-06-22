/*!
 * \file   mfront/include/MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx
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

#ifndef LIB_MFRONT_ABAQUS_ABAQUSSTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSSTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusConfig.hxx"

namespace abaqus {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Abaqus conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Abaqus conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusStandardSmallStrainStressFreeExpansionHandler(float *const,
                                                      float *const,
                                                      const float *const,
                                                      const float *const,
                                                      const AbaqusInt);
  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Abaqus conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Abaqus conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusStandardSmallStrainStressFreeExpansionHandler(double *const,
                                                      double *const,
                                                      const double *const,
                                                      const double *const,
                                                      const AbaqusInt);
  /*!
   * standard handler of stress free expansion handler for logarithmic
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Abaqus conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Abaqus conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusLogarithmicStrainStressFreeExpansionHandler(float *const,
                                                    float *const,
                                                    const float *const,
                                                    const float *const,
                                                    const AbaqusInt);
  /*!
   * standard handler of stress free expansion handler for logarithmic
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Abaqus conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Abaqus conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusLogarithmicStrainStressFreeExpansionHandler(double *const,
                                                    double *const,
                                                    const double *const,
                                                    const double *const,
                                                    const AbaqusInt);

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSSTRESSFREEEXPANSIONHANDLER_HXX */
