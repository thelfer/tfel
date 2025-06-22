/*!
 * \file   mfront/include/MFront/Cyrano/Cyrano.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX
#define LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX

#ifndef TFEL_CYRANO_INLINE
#define TFEL_CYRANO_INLINE inline
#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */

#ifndef TFEL_CYRANO_INLINE2
#define TFEL_CYRANO_INLINE2 inline
#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */

namespace cyrano {

#if CYRANO_ARCH == 64
#ifdef _WIN64
  typedef long long CyranoInt;
#else
  typedef long CyranoInt;
#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */
#elif CYRANO_ARCH == 32
  typedef int CyranoInt;
#else
#error "cyrano : unsupported machine type"
#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */

  typedef double CyranoReal;

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
  typedef void (*StressFreeExpansionHandler)(CyranoReal *const,
                                             CyranoReal *const,
                                             const CyranoReal *const,
                                             const CyranoReal *const);

}  // end of namespace cyrano

#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */
