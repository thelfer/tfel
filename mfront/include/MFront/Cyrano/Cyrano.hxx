/*!
 * \file   mfront/include/MFront/Cyrano/Cyrano.hxx
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
   */
  typedef void (*StressFreeExpansionHandler)(CyranoReal * const,
					     CyranoReal * const,
					     const CyranoReal *const,
					     const CyranoReal *const);

} // end of namespace cyrano

#endif /* _LIB_TFEL_CYRANO_MFRONT_CYRANO_H */

