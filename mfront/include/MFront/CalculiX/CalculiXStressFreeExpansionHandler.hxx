/*!
 * \file   mfront/include/MFront/CalculiX/CalculiXStressFreeExpansionHandler.hxx
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

#ifndef LIB_MFRONT_CALCULIX_CALCULIXSTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXSTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXConfig.hxx"

namespace calculix {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (CalculiX conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (CalculiX conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_CALCULIX_VISIBILITY_EXPORT void
  CalculiXStandardSmallStrainStressFreeExpansionHandler(
      CalculiXReal *const,
      CalculiXReal *const,
      const CalculiXReal *const,
      const CalculiXReal *const,
      const CalculiXInt);
  /*!
   * standard handler of stress free expansion handler for logarithmic
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (CalculiX conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (CalculiX conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_CALCULIX_VISIBILITY_EXPORT void
  CalculiXLogarithmicStrainStressFreeExpansionHandler(CalculiXReal *const,
                                                      CalculiXReal *const,
                                                      const CalculiXReal *const,
                                                      const CalculiXReal *const,
                                                      const CalculiXInt);

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXSTRESSFREEEXPANSIONHANDLER_HXX */
