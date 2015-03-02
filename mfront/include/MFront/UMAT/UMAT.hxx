/*!
 * \file   mfront/include/MFront/UMAT/UMAT.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   31 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_UMAT_MFRONT_UMAT_H_
#define LIB_TFEL_UMAT_MFRONT_UMAT_H_ 

#include<castem.h>

#ifdef umat /* the umat symbol might be defined 
	     * in the castem header, so remove it
	     */
#undef umat
#endif /* LIB_TFEL_UMAT_MFRONT_UMAT_H_ */

#ifndef TFEL_UMAT_INLINE
#define TFEL_UMAT_INLINE inline
#endif /* LIB_TFEL_UMAT_MFRONT_UMAT_H_ */

#ifndef TFEL_UMAT_INLINE2
#define TFEL_UMAT_INLINE2 inline
#endif /* LIB_TFEL_UMAT_MFRONT_UMAT_H_ */

namespace umat{

  typedef cast_int UMATInt;
  typedef double   UMATReal;

  /*!
   * prototype of a function handling stress-free expansion at the
   * beginning of the time step
   *
   * - first  paramater : driving variable
   * - second paramater : driving variable at the end of the time step
   *   or driving variable increment
   * - third paramater  : expansion value at the beginning of the time
   *   step
   * - fourth paramater : expansion value at the end of the time step
   * - third  parameter : spatial dimension
   */
  typedef void (*StressFreeExpansionHandler)(UMATReal * const,
					     UMATReal * const,
					     const UMATReal *const,
					     const UMATReal *const,
					     const UMATInt);

} // end of namespace umat

#endif /* LIB_TFEL_UMAT_MFRONT_UMAT_H_ */

