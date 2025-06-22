/*!
 * \file   mfront/src/CyranoOutOfBoundsPolicy.cxx
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

#include <cstdlib>
#include <cstring>
#include "MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx"

namespace cyrano {

  const CyranoOutOfBoundsPolicy&
  CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy() {
    static CyranoOutOfBoundsPolicy u;
    return u;
  }  // end of getCyranoOutOfBoundsPolicy

  CyranoOutOfBoundsPolicy::CyranoOutOfBoundsPolicy() {
    const char* const pe = ::getenv("CYRANO_OUT_OF_BOUNDS_POLICY");
    if (pe != nullptr) {
      if (strcmp(pe, "STRICT") == 0) {
        this->policy = tfel::material::Strict;
      } else if (strcmp(pe, "WARNING") == 0) {
        this->policy = tfel::material::Warning;
      }
    }
  }  // end of CyranoOutOfBoundsPolicy::CyranoOutOfBoundsPolicy

  std::optional<tfel::material::OutOfBoundsPolicy>
  CyranoOutOfBoundsPolicy::getOutOfBoundsPolicy() const {
    return this->policy;
  }  // end of CyranoOutOfBoundsPolicy::getOutOfBoundsPolicy

}  // end of namespace cyrano
