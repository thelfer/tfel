/*!
 * \file   src/System/ExternalMaterialKnowledgeDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   11/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "TFEL/System/ExternalMaterialKnowledgeDescription.hxx"

namespace tfel::system {

  ExternalMaterialKnowledgeDescription::ExternalMaterialKnowledgeDescription(
      const std::string& l, const std::string& f) {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    this->library = l;
    this->entry_point = f;
    this->tfel_version = elm.getTFELVersion(l, f);
    this->unit_system = elm.getUnitSystem(l, f);
    this->build_id = elm.getBuildId(l, f);
    this->source = elm.getSource(l, f);
    this->material = elm.getMaterial(l, f);
    this->mfront_interface = elm.getInterface(l, f);
    this->author = elm.getAuthor(l, f);
    this->date = elm.getDate(l, f);
    this->description = elm.getDescription(l, f);
  }  // end of ExternalMaterialKnowledgeDescription

  ExternalMaterialKnowledgeDescription::ExternalMaterialKnowledgeDescription() =
      default;
  ExternalMaterialKnowledgeDescription::ExternalMaterialKnowledgeDescription(
      ExternalMaterialKnowledgeDescription&&) = default;
  ExternalMaterialKnowledgeDescription::ExternalMaterialKnowledgeDescription(
      const ExternalMaterialKnowledgeDescription&) = default;
  ExternalMaterialKnowledgeDescription&
  ExternalMaterialKnowledgeDescription::operator=(
      const ExternalMaterialKnowledgeDescription&) = default;
  ExternalMaterialKnowledgeDescription&
  ExternalMaterialKnowledgeDescription::operator=(
      ExternalMaterialKnowledgeDescription&&) = default;
  ExternalMaterialKnowledgeDescription::
      ~ExternalMaterialKnowledgeDescription() = default;

}  // end of namespace tfel::system
