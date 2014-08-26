/*! 
 * \file   AsterStressFreeExpansionHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  05 mars 2014
 */

#ifndef _LIB_MFRONT_ASTER_ASTERSTRESSFREEEXPANSIONHANDLER_H_
#define _LIB_MFRONT_ASTER_ASTERSTRESSFREEEXPANSIONHANDLER_H_ 

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"

namespace aster
{

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
  AsterStandardSmallStrainStressFreeExpansionHandler(AsterReal * const,
						     AsterReal * const,
						     const AsterReal *const,
						     const AsterReal *const,
						     const AsterInt);

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_ASTERSTRESSFREEEXPANSIONHANDLER_H */

