/*!
 * \file   UMAT.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   31 Jul 2006
 */

#ifndef _LIB_TFEL_UMAT_MFRONT_UMAT_H_
#define _LIB_TFEL_UMAT_MFRONT_UMAT_H_ 

#include<string>
#include<castem.h>
#ifdef umat /* the umat symbol might be defined 
	     * in the castem header, so remove it
	     */
#undef umat
#endif

#ifndef TFEL_UMAT_INLINE
#define TFEL_UMAT_INLINE inline
#endif  /* TFEL_UMAT_INLINE */

#ifndef TFEL_UMAT_INLINE2
#define TFEL_UMAT_INLINE2 inline
#endif  /* TFEL_UMAT_INLINE2 */

namespace umat{

  typedef cast_int UMATInt;
  typedef double   UMATReal;

} // end of namespace umat

#endif /* _LIB_TFEL_UMAT_MFRONT_UMAT_H */

