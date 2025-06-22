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

#include "Cyrano/Types.hxx"

namespace cyrano {

  using CyranoInt = ::CyranoIntegerType;
  using CyranoReal = ::CyranoRealType;

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
  using StressFreeExpansionHandler = void (*)(CyranoReal *const,
                                              CyranoReal *const,
                                              const CyranoReal *const,
                                              const CyranoReal *const,
                                              const CyranoReal *const,
                                              const CyranoReal *const);

}  // end of namespace cyrano

#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */
