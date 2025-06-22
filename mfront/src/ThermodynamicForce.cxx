/*!
 * \file  ThermodynamicForce.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/ThermodynamicForce.hxx"

namespace mfront {

  ThermodynamicForce::ThermodynamicForce(const std::string& t,
                                         const std::string& n)
      : VariableDescription(t, n, 1u, 0u) {
  }  // end of ThermodynamicForce::ThermodynamicForce

  ThermodynamicForce::ThermodynamicForce(ThermodynamicForce&&) = default;
  ThermodynamicForce::ThermodynamicForce(const ThermodynamicForce&) = default;
  ThermodynamicForce& ThermodynamicForce::operator=(ThermodynamicForce&&) =
      default;
  ThermodynamicForce& ThermodynamicForce::operator=(const ThermodynamicForce&) =
      default;

  ThermodynamicForce::~ThermodynamicForce() noexcept = default;

}  // end of namespace mfront
