/*!
 * \file   src/System/ExternalMaterialPropertyDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 June 2015
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
#include "TFEL/System/ExternalMaterialPropertyDescription.hxx"

namespace tfel::system {

  ExternalMaterialPropertyDescription::ExternalMaterialPropertyDescription(
      const std::string& l, const std::string& f)
      : ExternalMaterialKnowledgeDescription(l, f) {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    this->material_property = f;
    this->law = elm.getLaw(l, f);
    this->output = elm.getMaterialPropertyOutput(l, f);
    this->arguments = elm.getMaterialPropertyVariables(l, f);
    try {
      this->parameters = elm.getMaterialPropertyParameters(l, f);
    } catch (...) {
    }
  }  // end of ExternalMaterialPropertyDescription

  double ExternalMaterialPropertyDescription::getParameterDefaultValue(
      const std::string& p) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getRealParameterDefaultValue(this->library,
                                            this->material_property, "", p);
  }  // end of getParameterDefaultValue

  bool ExternalMaterialPropertyDescription::hasBounds(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasBounds(this->library, this->material_property, v);
  }  // end of hasBounds

  bool ExternalMaterialPropertyDescription::hasLowerBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerBound(this->library, this->material_property, v);
  }  // end of hasLowerBound

  bool ExternalMaterialPropertyDescription::hasUpperBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperBound(this->library, this->material_property, v);
  }  // end of hasUpperBound

  long double ExternalMaterialPropertyDescription::getLowerBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerBound(this->library, this->material_property, v);
  }  // end of getLowerBound

  long double ExternalMaterialPropertyDescription::getUpperBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperBound(this->library, this->material_property, v);
  }  // end of getUpperBound

  bool ExternalMaterialPropertyDescription::hasPhysicalBounds(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasPhysicalBounds(this->library, this->material_property, v);
  }  // end of hasPhysicalBounds

  bool ExternalMaterialPropertyDescription::hasLowerPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerPhysicalBound(this->library, this->material_property, v);
  }  // end of hasLowerPhysicalBound

  bool ExternalMaterialPropertyDescription::hasUpperPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperPhysicalBound(this->library, this->material_property, v);
  }  // end of hasUpperPhysicalBound

  long double ExternalMaterialPropertyDescription::getLowerPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerPhysicalBound(this->library, this->material_property, v);
  }  // end of getLowerPhysicalBound

  long double ExternalMaterialPropertyDescription::getUpperPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperPhysicalBound(this->library, this->material_property, v);
  }  // end of getUpperPhysicalBound

  ExternalMaterialPropertyDescription::ExternalMaterialPropertyDescription() =
      default;
  ExternalMaterialPropertyDescription::ExternalMaterialPropertyDescription(
      ExternalMaterialPropertyDescription&&) = default;
  ExternalMaterialPropertyDescription::ExternalMaterialPropertyDescription(
      const ExternalMaterialPropertyDescription&) = default;
  ExternalMaterialPropertyDescription&
  ExternalMaterialPropertyDescription::operator=(
      const ExternalMaterialPropertyDescription&) = default;
  ExternalMaterialPropertyDescription&
  ExternalMaterialPropertyDescription::operator=(
      ExternalMaterialPropertyDescription&&) = default;
  ExternalMaterialPropertyDescription::~ExternalMaterialPropertyDescription() =
      default;

}  // end of namespace tfel::system

extern "C" {

char* getExternalMaterialPropertyData(
    tfel::system::ExternalMaterialPropertyData* const d,
    const char* const l,
    const char* const f) {
  auto report = [](const char* e) {
    auto strndup = [](const char* s, size_t n) -> char* {
      if (s == nullptr) {
        return nullptr;
      }
      auto length = ::strlen(s);
      if (length > n) {
        length = n;
      }
      auto copy = static_cast<char*>(::malloc(length + 1));
      if (copy == nullptr) {
        return nullptr;
      }
      ::memcpy(copy, s, length);
      copy[length] = '\0';
      return copy;
    };
    const auto msg = strndup(e, 255);
    if (msg == nullptr) {
      ::fprintf(stderr,
                "getExternalMaterialPropertyData: generating "
                "error message failed, aborting\n");
      ::exit(EXIT_FAILURE);
    }
    return msg;
  };
  try {
    *d = tfel::system::ExternalMaterialPropertyDescription(l, f);
  } catch (std::exception& e) {
    return report(e.what());
  } catch (...) {
    return report("getExternalMaterialPropertyData: unknown exception thrown");
  }
  return nullptr;
}

tfel::system::ExternalMaterialPropertyData* newExternalMaterialPropertyData() {
  return new tfel::system::ExternalMaterialPropertyData;
}

void freeExternalMaterialPropertyData(
    tfel::system::ExternalMaterialPropertyData* ptr) {
  delete ptr;
}

}  // end of extern "C"
