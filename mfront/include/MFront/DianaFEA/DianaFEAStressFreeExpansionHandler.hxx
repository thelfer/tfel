/*!
 * \file   mfront/include/MFront/DianaFEA/DianaFEAStressFreeExpansionHandler.hxx
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

#ifndef LIB_MFRONT_DIANAFEA_DIANAFEASTRESSFREEEXPANSIONHANDLER_HXX
#define LIB_MFRONT_DIANAFEA_DIANAFEASTRESSFREEEXPANSIONHANDLER_HXX

#include "MFront/DianaFEA/DianaFEA.hxx"
#include "MFront/DianaFEA/DianaFEAConfig.hxx"

namespace dianafea {

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (DianaFEA conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (DianaFEA conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAStandardSmallStrainStressFreeExpansionHandler(double *const,
                                                        double *const,
                                                        const double *const,
                                                        const double *const,
                                                        const DianaFEAInt);

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEA_DIANAFEASTRESSFREEEXPANSIONHANDLER_HXX */
