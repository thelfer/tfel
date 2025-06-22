/*!
 * \file   mfront/include/MFront/Aster/AsterStressFreeExpansionHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  05 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTER_ASTERSTRESSFREEEXPANSIONHANDLER_H_
#define LIB_MFRONT_ASTER_ASTERSTRESSFREEEXPANSIONHANDLER_H_

#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterConfig.hxx"

namespace aster {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (Aster conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (Aster conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_ASTER_VISIBILITY_EXPORT void
  AsterStandardSmallStrainStressFreeExpansionHandler(AsterReal *const,
                                                     AsterReal *const,
                                                     const AsterReal *const,
                                                     const AsterReal *const,
                                                     const AsterInt);

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTER_ASTERSTRESSFREEEXPANSIONHANDLER_H_ */
