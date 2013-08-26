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
#if WIN64
  typedef long long   AsterInt;
#else
  typedef long   AsterInt;
#endif
#elif ASTER_ARCH == 32
  typedef int    AsterInt;
#else
  #error "aster : unsupported machine type"
#endif

  typedef double AsterReal;

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_H */

