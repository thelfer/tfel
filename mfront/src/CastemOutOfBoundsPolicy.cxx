/*!
 * \file  mfront/src/CastemOutOfBoundsPolicy.cxx
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

#include <cstdlib>
#include <cstring>
#include "MFront/Castem/CastemOutOfBoundsPolicy.hxx"

namespace castem {

  const CastemOutOfBoundsPolicy&
  CastemOutOfBoundsPolicy::getCastemOutOfBoundsPolicy() {
    static CastemOutOfBoundsPolicy u;
    return u;
  }  // end of getCastemOutOfBoundsPolicy

  CastemOutOfBoundsPolicy::CastemOutOfBoundsPolicy() {
    const char* const pe = ::getenv("CASTEM_OUT_OF_BOUNDS_POLICY");
    if (pe != nullptr) {
      if (strcmp(pe, "STRICT") == 0) {
        this->policy = tfel::material::Strict;
      } else if (strcmp(pe, "WARNING") == 0) {
        this->policy = tfel::material::Warning;
      }
    }
  }  // end of CastemOutOfBoundsPolicy::CastemOutOfBoundsPolicy

  std::optional<tfel::material::OutOfBoundsPolicy>
  CastemOutOfBoundsPolicy::getOutOfBoundsPolicy() const {
    return this->policy;
  }  // end of CastemOutOfBoundsPolicy::getOutOfBoundsPolicy

}  // end of namespace castem
