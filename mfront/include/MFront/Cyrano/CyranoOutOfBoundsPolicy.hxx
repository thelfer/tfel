/*!
 * \file  mfront/include/MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx
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

#ifndef LIB_TFEL_MFRONT_CYRANOOUTOFBOUNDSPOLICY_HXX
#define LIB_TFEL_MFRONT_CYRANOOUTOFBOUNDSPOLICY_HXX

#include "MFront/Cyrano/CyranoConfig.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"

namespace cyrano {

  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoOutOfBoundsPolicy {
    static const CyranoOutOfBoundsPolicy& getCyranoOutOfBoundsPolicy();
    tfel::material::OutOfBoundsPolicy getOutOfBoundsPolicy() const;

   private:
    CyranoOutOfBoundsPolicy();
    CyranoOutOfBoundsPolicy(const CyranoOutOfBoundsPolicy&);
    CyranoOutOfBoundsPolicy& operator=(const CyranoOutOfBoundsPolicy&);
    tfel::material::OutOfBoundsPolicy policy;
  };  // end of struct CyranoOutOfBounds

}  // end of namespace cyrano

#endif /* LIB_TFEL_MFRONT_CYRANOOUTOFBOUNDSPOLICY_HXX */
