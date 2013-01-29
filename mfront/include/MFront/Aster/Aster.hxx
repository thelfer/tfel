/*! 
 * \file  Aster.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 janv. 2013
 */

#ifndef _LIB_MFRONT_ASTER_H_
#define _LIB_MFRONT_ASTER_H_ 

#ifndef TFEL_ASTER_INLINE
#define TFEL_ASTER_INLINE inline
#endif  /* TFEL_ASTER_INLINE */

#ifndef TFEL_ASTER_INLINE2
#define TFEL_ASTER_INLINE2 inline
#endif  /* TFEL_ASTER_INLINE2 */

namespace aster{

#if ASTER_ARCH == 64
  typedef long   AsterInt;
  typedef double AsterReal;
#elif ASTER_ARCH == 32
  typedef int    AsterInt;
  typedef double AsterReal;
#else
  #error "aster : unsupported machine type"
#endif

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_H */

