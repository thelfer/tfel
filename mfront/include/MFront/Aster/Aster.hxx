/*!
 * \file  mfront/include/MFront/Aster/Aster.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTER_H_
#define LIB_MFRONT_ASTER_H_

#ifndef TFEL_ASTER_INLINE
#define TFEL_ASTER_INLINE inline
#endif /* LIB_MFRONT_ASTER_H_ */

#ifndef TFEL_ASTER_INLINE2
#define TFEL_ASTER_INLINE2 inline
#endif /* LIB_MFRONT_ASTER_H_ */

namespace aster {

#if ASTER_ARCH == 64
#ifdef _WIN64
  typedef long long AsterInt;
#else
  typedef long AsterInt;
#endif /* LIB_MFRONT_ASTER_H_ */
#elif ASTER_ARCH == 32
  typedef int AsterInt;
#else
#error "aster : unsupported machine type"
#endif /* LIB_MFRONT_ASTER_H_ */

  typedef double AsterReal;

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
  typedef void (*StressFreeExpansionHandler)(AsterReal *const,
                                             AsterReal *const,
                                             const AsterReal *const,
                                             const AsterReal *const,
                                             const AsterInt);

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTER_H_ */
