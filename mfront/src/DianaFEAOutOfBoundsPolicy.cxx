/*!
 * \file  mfront/src/DianaFEAOutOfBoundsPolicy.cxx
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
#include "MFront/DianaFEA/DianaFEAOutOfBoundsPolicy.hxx"

namespace dianafea {

  const DianaFEAOutOfBoundsPolicy&
  DianaFEAOutOfBoundsPolicy::getDianaFEAOutOfBoundsPolicy() {
    static DianaFEAOutOfBoundsPolicy u;
    return u;
  }  // end of getDianaFEAOutOfBoundsPolicy

  DianaFEAOutOfBoundsPolicy::DianaFEAOutOfBoundsPolicy() {
    const char* const pe = ::getenv("DIANAFEA_OUT_OF_BOUNDS_POLICY");
    if (pe != nullptr) {
      if (strcmp(pe, "STRICT") == 0) {
        this->policy = tfel::material::Strict;
      } else if (strcmp(pe, "WARNING") == 0) {
        this->policy = tfel::material::Warning;
      }
    }
  }  // end of DianaFEAOutOfBoundsPolicy::DianaFEAOutOfBoundsPolicy

  std::optional<tfel::material::OutOfBoundsPolicy>
  DianaFEAOutOfBoundsPolicy::getOutOfBoundsPolicy() const {
    return this->policy;
  }  // end of DianaFEAOutOfBoundsPolicy::getOutOfBoundsPolicy

}  // end of namespace dianafea
