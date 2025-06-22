/*!
 * \file  mfront/src/MaterialPropertyDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/MaterialPropertyDescription.hxx"

namespace mfront {

  std::vector<std::string>
  MaterialPropertyDescription::getInputVariablesExternalNames() {
    return this->inputs.getExternalNames(this->glossaryNames, this->entryNames);
  }  // end of MaterialPropertyDescription::getInputVariablesExternalNames

  MaterialPropertyDescription::MaterialPropertyDescription() = default;
  MaterialPropertyDescription::MaterialPropertyDescription(
      const MaterialPropertyDescription&) = default;
  MaterialPropertyDescription::MaterialPropertyDescription(
      MaterialPropertyDescription&&) = default;
  MaterialPropertyDescription& MaterialPropertyDescription::operator=(
      const MaterialPropertyDescription&) = default;
  MaterialPropertyDescription& MaterialPropertyDescription::operator=(
      MaterialPropertyDescription&&) = default;
  MaterialPropertyDescription::~MaterialPropertyDescription() = default;

}  // end of namespace mfront
