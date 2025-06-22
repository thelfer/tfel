/*!
 * \file
 * mfront/include/MFront/Europlexus/EuroplexusStressFreeExpansionHandler.hxx
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

#ifndef LIB_MFRONT_EPX_EPXSTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_EPX_EPXSTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/Europlexus/Europlexus.hxx"
#include "MFront/Europlexus/EuroplexusConfig.hxx"

namespace epx {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Europlexus conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Europlexus conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  EuroplexusStandardSmallStrainStressFreeExpansionHandler(
      EuroplexusReal *const,
      EuroplexusReal *const,
      const EuroplexusReal *const,
      const EuroplexusReal *const,
      const EuroplexusInt);
  /*!
   * \brief handler of stress free expansion handler for behaviours
   * based on logarithmic strains
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Europlexus conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Europlexus conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  EuroplexusLogarithmicStrainStressFreeExpansionHandler(
      EuroplexusReal *const,
      EuroplexusReal *const,
      const EuroplexusReal *const,
      const EuroplexusReal *const,
      const EuroplexusInt);

}  // end of namespace epx

#endif /* LIB_MFRONT_EPX_EPXSTRESSFREEEXPANSIONHANDLER_HXX */
