/*!
 * \file   mfront/include/MFront/LSDYNA/LSDYNAStressFreeExpansionHandler.hxx
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

#ifndef LIB_MFRONT_LSDYNA_LSDYNASTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_LSDYNA_LSDYNASTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/LSDYNA/LSDYNA.hxx"
#include "MFront/LSDYNA/LSDYNAConfig.hxx"

namespace lsdyna {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (LSDYNA conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (LSDYNA conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAStandardSmallStrainStressFreeExpansionHandler(float *const,
                                                      float *const,
                                                      const float *const,
                                                      const float *const,
                                                      const LSDYNAInt);
  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (LSDYNA conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (LSDYNA conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAStandardSmallStrainStressFreeExpansionHandler(double *const,
                                                      double *const,
                                                      const double *const,
                                                      const double *const,
                                                      const LSDYNAInt);
  /*!
   * standard handler of stress free expansion handler for logarithmic
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (LSDYNA conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (LSDYNA conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNALogarithmicStrainStressFreeExpansionHandler(float *const,
                                                    float *const,
                                                    const float *const,
                                                    const float *const,
                                                    const LSDYNAInt);
  /*!
   * standard handler of stress free expansion handler for logarithmic
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (LSDYNA conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (LSDYNA conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNALogarithmicStrainStressFreeExpansionHandler(double *const,
                                                    double *const,
                                                    const double *const,
                                                    const double *const,
                                                    const LSDYNAInt);

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNA_LSDYNASTRESSFREEEXPANSIONHANDLER_HXX */
