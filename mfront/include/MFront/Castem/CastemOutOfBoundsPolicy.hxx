/*!
 * \file  mfront/include/MFront/Castem/CastemOutOfBoundsPolicy.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 jan 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MFRONT_CASTEMOUTOFBOUNDSPOLICY_HXX
#define LIB_TFEL_MFRONT_CASTEMOUTOFBOUNDSPOLICY_HXX

#include "MFront/Castem/CastemConfig.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"

namespace castem {

  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemOutOfBoundsPolicy {
    static const CastemOutOfBoundsPolicy& getCastemOutOfBoundsPolicy();
    tfel::material::OutOfBoundsPolicy getOutOfBoundsPolicy() const;

   private:
    CastemOutOfBoundsPolicy();
    CastemOutOfBoundsPolicy(const CastemOutOfBoundsPolicy&);
    CastemOutOfBoundsPolicy& operator=(const CastemOutOfBoundsPolicy&);
    tfel::material::OutOfBoundsPolicy policy;
  };  // end of struct CastemOutOfBounds

}  // end of namespace castem

#endif /* LIB_TFEL_MFRONT_CASTEMOUTOFBOUNDSPOLICY_HXX */
