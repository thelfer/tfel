/*! 
 * \file   UMATStressFreeExpansionHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  04 mars 2014
 */

#ifndef _LIB_MFRONT_UMAT_UMATSTRESSFREEEXPANSIONHANDLER_H_
#define _LIB_MFRONT_UMAT_UMATSTRESSFREEEXPANSIONHANDLER_H_ 

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"

namespace umat
{

  /*!
   * standard handler of stress free expansion handler for small
   * strain behaviours
   * \param[in,out] e  : total deformation at the beginning of the time
   *                     step (UMAT conventions)
   * \param[in,out] de : total deformation increment over the time
   *                     step (UMAT conventions)
   * \param[in] s0     : stress free expansion at beginning of the time
   *                     step (TFEL conventions)
   * \param[in] s1     : stress free expansion at end of the time
   *                     step (TFEL conventions)
   * \param[in] d      : spatial dimension
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATStandardSmallStrainStressFreeExpansionHandler(UMATReal * const,
						    UMATReal * const,
						    const UMATReal *const,
						    const UMATReal *const,
						    const UMATInt);

} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATSTRESSFREEEXPANSIONHANDLER_H */

