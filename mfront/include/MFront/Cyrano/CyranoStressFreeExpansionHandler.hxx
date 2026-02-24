/*!
 * \file   mfront/include/MFront/Cyrano/CyranoStressFreeExpansionHandler.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  04 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANO_CYRANOSTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_CYRANO_CYRANOSTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/Cyrano/Cyrano.hxx"
#include "MFront/Cyrano/CyranoConfig.hxx"

namespace cyrano {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] em : mechanical deformation at the beginning of the time
   *                     step (TFEL conventions)
   * \param[in,out] dem: mechanical deformation increment over the time
   *                     step (TFEL conventions)
   * \param[in]     e : total deformation at the beginning of the time
   *                     step (Cyrano conventions)
   * \param[in]     de: total deformation increment over the time
   *                     step (Cyrano conventions)
   * \param[in] s0    : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1    : stress free expansion at end of the time
   *                     step (TFEL conventions)
   */
  MFRONT_CYRANO_VISIBILITY_EXPORT void
  CyranoStandardSmallStrainStressFreeExpansionHandler(CyranoReal *const,
                                                      CyranoReal *const,
                                                      const CyranoReal *const,
                                                      const CyranoReal *const,
                                                      const CyranoReal *const,
                                                      const CyranoReal *const);
  /*!
   * \brief handler of stress free expansion handler for logarithmic strains
   * \param[out] em : mechanical deformation at the beginning of the time
   *                     step (TFEL conventions)
   * \param[out] dem: mechanical deformation increment over the time
   *                     step (TFEL conventions)
   * \param[out] e : total deformation at the beginning of the time
   *                     step (Cyrano conventions)
   * \param[out] de: total deformation increment over the time
   *                     step (Cyrano conventions)
   * \param[in] s0    : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1    : stress free expansion at end of the time
   *                     step (TFEL conventions)
   */
  MFRONT_CYRANO_VISIBILITY_EXPORT void
  CyranoLogarithmicStrainStressFreeExpansionHandler(CyranoReal *const,
                                                    CyranoReal *const,
                                                    const CyranoReal *const,
                                                    const CyranoReal *const,
                                                    const CyranoReal *const,
                                                    const CyranoReal *const);

}  // end of namespace cyrano

#endif /* LIB_MFRONT_CYRANO_CYRANOSTRESSFREEEXPANSIONHANDLER_HXX */
