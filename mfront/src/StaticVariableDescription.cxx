/*!
 * \file   mfront/src/StaticVariableDescription.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   19 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/StaticVariableDescription.hxx"

namespace mfront {

  StaticVariableDescription::StaticVariableDescription(
      const std::string& type_,
      const std::string& name_,
      const unsigned int line_,
      const StaticVariableValueType value_)
      : VariableDescriptionBase(type_, name_, 1u, line_), value(value_) {}

  StaticVariableDescription::StaticVariableDescription(
      StaticVariableDescription&&) = default;
  StaticVariableDescription::StaticVariableDescription(
      const StaticVariableDescription&) = default;
  StaticVariableDescription& StaticVariableDescription::operator=(
      StaticVariableDescription&&) = default;
  StaticVariableDescription& StaticVariableDescription::operator=(
      const StaticVariableDescription&) = default;
  StaticVariableDescription::~StaticVariableDescription() = default;

  bool StaticVariableDescriptionContainer::contains(
      const std::string& n) const {
    for (const auto& v : *this) {
      if (v.name == n) {
        return true;
      }
    }
    return false;
  }  // end of StaticVariableDescriptionContainer::contains

  const StaticVariableDescription& StaticVariableDescriptionContainer::get(
      const std::string& n) const {
    for (const auto& v : *this) {
      if (v.name == n) {
        return v;
      }
    }
    tfel::raise(
        "StaticVariableDescriptionContainer::get: "
        "no static variable named '" +
        n + "'");
  }  // end of StaticVariableDescriptionContainer::get

}  // end of namespace mfront
