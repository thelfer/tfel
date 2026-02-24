/*!
 * \file   mfront/include/MFront/Castem/CastemStressFreeExpansionHandler.hxx
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

#ifndef LIB_MFRONT_CASTEM_CASTEMSTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_CASTEM_CASTEMSTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/Castem/Castem.hxx"
#include "MFront/Castem/CastemConfig.hxx"

namespace castem {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Castem conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Castem conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemStandardSmallStrainStressFreeExpansionHandler(CastemReal *const,
                                                      CastemReal *const,
                                                      const CastemReal *const,
                                                      const CastemReal *const,
                                                      const CastemInt);
  /*!
   * \brief handler of stress free expansion handler for logarithmic strains
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Castem conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Castem conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemLogarithmicStrainStressFreeExpansionHandler(CastemReal *const,
                                                    CastemReal *const,
                                                    const CastemReal *const,
                                                    const CastemReal *const,
                                                    const CastemInt);

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMSTRESSFREEEXPANSIONHANDLER_HXX */
