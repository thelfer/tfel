/*!
 * \file   mfront/include/MFront/Castem/Castem.hxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   31 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CASTEM_MFRONT_CASTEM_H_
#define LIB_TFEL_CASTEM_MFRONT_CASTEM_H_

#include <castem.h>
#ifdef umat /* the umat symbol might be defined in the (standard) \
             * castem header, so remove it                        \
             */
#undef umat
#endif /* umat */

#ifndef TFEL_CASTEM_INLINE
#define TFEL_CASTEM_INLINE inline
#endif /* LIB_TFEL_CASTEM_MFRONT_CASTEM_H_ */

#ifndef TFEL_CASTEM_INLINE2
#define TFEL_CASTEM_INLINE2 inline
#endif /* LIB_TFEL_CASTEM_MFRONT_CASTEM_H_ */

namespace castem {

  typedef cast_int CastemInt;
  typedef cast_double CastemReal;

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
  typedef void (*StressFreeExpansionHandler)(CastemReal *const,
                                             CastemReal *const,
                                             const CastemReal *const,
                                             const CastemReal *const,
                                             const CastemInt);

}  // end of namespace castem

#endif /* LIB_TFEL_CASTEM_MFRONT_CASTEM_H_ */
