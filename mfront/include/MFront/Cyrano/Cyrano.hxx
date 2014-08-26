/*!
 * \file   Cyrano.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_TFEL_CYRANO_MFRONT_CYRANO_H_
#define _LIB_TFEL_CYRANO_MFRONT_CYRANO_H_ 

#ifndef TFEL_CYRANO_INLINE
#define TFEL_CYRANO_INLINE inline
#endif  /* TFEL_CYRANO_INLINE */

#ifndef TFEL_CYRANO_INLINE2
#define TFEL_CYRANO_INLINE2 inline
#endif  /* TFEL_CYRANO_INLINE2 */

namespace cyrano{

#if CYRANO_ARCH == 64
#ifdef WIN64
  typedef long long   CyranoInt;
#else
  typedef long   CyranoInt;
#endif
#elif CYRANO_ARCH == 32
  typedef int    CyranoInt;
#else
  #error "cyrano : unsupported machine type"
#endif

  typedef double   CyranoReal;

} // end of namespace cyrano

#endif /* _LIB_TFEL_CYRANO_MFRONT_CYRANO_H */

