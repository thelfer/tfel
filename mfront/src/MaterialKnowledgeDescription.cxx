/*!
 * \file   mfront/src/MaterialKnowledgeDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <iterator>
#include "TFEL/Raise.hxx"
#include "MFront/MaterialKnowledgeDescription.hxx"

namespace mfront {

  const char* const MaterialKnowledgeDescription::disableRuntimeChecks =
      "disable_runtime_checks";
  const char* const MaterialKnowledgeDescription::defaultOutOfBoundsPolicy =
      "default_out_of_bounds_policy";
  const char* const
      MaterialKnowledgeDescription::runtimeModificationOfTheOutOfBoundsPolicy =
          "out_of_bounds_policy_runtime_modification";
  const char* const MaterialKnowledgeDescription::parametersAsStaticVariables =
      "parameters_as_static_variables";
  const char* const MaterialKnowledgeDescription::initializeParametersFromFile =
      "parameters_initialization_from_file";
  const char* const MaterialKnowledgeDescription::validator = "validator";
  const char* const MaterialKnowledgeDescription::buildIdentifier =
      "build_identifier";

  MaterialKnowledgeDescription::MaterialKnowledgeDescription() = default;
  MaterialKnowledgeDescription::MaterialKnowledgeDescription(
      MaterialKnowledgeDescription&&) = default;
  MaterialKnowledgeDescription::MaterialKnowledgeDescription(
      const MaterialKnowledgeDescription&) = default;
  MaterialKnowledgeDescription& MaterialKnowledgeDescription::operator=(
      const MaterialKnowledgeDescription&) = default;
  MaterialKnowledgeDescription& MaterialKnowledgeDescription::operator=(
      MaterialKnowledgeDescription&&) = default;

  void MaterialKnowledgeDescription::throwUndefinedAttribute(
      const std::string_view n) {
    tfel::raise(
        "MaterialKnowledgeDescription::getAttribute: "
        "no attribute named '" +
        std::string{n} + "'");
  }  // end of throwUndefinedAttribute

  void MaterialKnowledgeDescription::setAttribute(
      const std::string& n, const MaterialKnowledgeAttribute& a, const bool b) {
    auto throw_if = [](const bool c, const std::string_view m) {
      if (c) {
        tfel::raise("MaterialKnowledgeDescription::setAttribute: " +
                    std::string{m});
      }
    };
    auto p = this->attributes.find(n);
    if (p != this->attributes.end()) {
      throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
               "attribute already exists with a different type");
    }
    if (!this->attributes.insert({n, a}).second) {
      throw_if(!b, "attribute '" + n + "' already declared");
    }
  }  // end of setAttribute

  void MaterialKnowledgeDescription::updateAttribute(
      const std::string_view n, const MaterialKnowledgeAttribute& a) {
    auto throw_if = [](const bool c, const std::string_view m) {
      if (c) {
        tfel::raise("MaterialKnowledgeDescription::updateAttribute: " +
                    std::string{m});
      }
    };
    auto p = this->attributes.find(n);
    throw_if(p == this->attributes.end(),
             "unknown attribute '" + std::string{n} + "'");
    throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
             "attribute already exists with a different type");
    p->second = a;
  }  // end of setMaterialKnowledgeAttribute

  bool MaterialKnowledgeDescription::hasAttribute(
      const std::string_view n) const {
    return this->attributes.count(n) != 0u;
  }  // end of hasAttribute

  const std::map<std::string, MaterialKnowledgeAttribute, std::less<>>&
  MaterialKnowledgeDescription::getAttributes() const {
    return this->attributes;
  }  // end of getAttributes

  void MaterialKnowledgeDescription::addExternalMFrontFile(
      const std::string_view f,
      const std::vector<std::string>& interfaces,
      const tfel::utilities::DataMap& dsl_options) {
    const auto p = this->externalMFrontFiles.find(f);
    if (p == this->externalMFrontFiles.end()) {
      this->externalMFrontFiles.insert(
          {std::string{f}, {interfaces, dsl_options}});
    } else {
#pragma message("treat DSL options")
      std::copy_if(interfaces.begin(), interfaces.end(),
                   std::back_inserter(std::get<0>(p->second)),
                   [&interfaces](const std::string& i) {
                     return std::find(interfaces.begin(), interfaces.end(),
                                      i) == interfaces.end();
                   });
    }
  }  // end of addExternalMFrontFile

  const std::map<std::string,
                 std::tuple<std::vector<std::string>, tfel::utilities::DataMap>,
                 std::less<>>&
  MaterialKnowledgeDescription::getExternalMFrontFiles() const {
    return this->externalMFrontFiles;
  }  // end of getExternalMFrontFiles

  bool MaterialKnowledgeDescription::hasUnitSystem() const {
    return this->unit_system.has_value();
  }

  void MaterialKnowledgeDescription::setUnitSystem(const std::string_view s) {
    if (this->hasUnitSystem()) {
      tfel::raise(
          "MaterialKnowledgeDescription::setUnitSystem: "
          "an unit system has already been defined");
    }
    if (s != "SI") {
      tfel::raise(
          "MaterialKnowledgeDescription::setUnitSystem: "
          "unsupported unit system '" +
          std::string{s} + "'");
    }
    this->unit_system = s;
  }  // end of setUnitSystem

  const std::string& MaterialKnowledgeDescription::getUnitSystem() const {
    if (!this->hasUnitSystem()) {
      tfel::raise(
          "MaterialKnowledgeDescription::getUnitSystem: "
          "an unit system has not been defined");
    }
    return *(this->unit_system);
  }  // end of setUnitSystem

  MaterialKnowledgeDescription::~MaterialKnowledgeDescription() = default;

  void setDefaultOutOfBoundsPolicy(MaterialKnowledgeDescription& d,
                                   const std::string& policy) {
    if ((policy != "None") && (policy != "Warning") && (policy != "Strict")) {
      tfel::raise("setDefaultOutOfBoundsPolicy: invalid default policy '" +
                  std::string{policy} + "'");
    }
    d.setAttribute(MaterialKnowledgeDescription::defaultOutOfBoundsPolicy,
                   policy, false);
  }  // end of setDefaultOutOfBoundsPolicy

  void setDisableRuntimeChecks(MaterialKnowledgeDescription& d, const bool b) {
    d.setAttribute(MaterialKnowledgeDescription::disableRuntimeChecks, b,
                   false);
  }  // end of setDisableRuntimeChecks

  tfel::material::OutOfBoundsPolicy getDefaultOutOfBoundsPolicy(
      const MaterialKnowledgeDescription& d) {
    const auto policy = d.getAttribute<std::string>(
        MaterialKnowledgeDescription::defaultOutOfBoundsPolicy, "None");
    if (policy == "Strict") {
      return tfel::material::Strict;
    } else if (policy == "Warning") {
      return tfel::material::Warning;
    }
    if (policy != "None") {
      tfel::raise("getDefaultOutOfBoundsPolicy: invalid default policy '" +
                  policy + "'");
    }
    return tfel::material::None;
  }  // end of getDefaultOutOfBoundsPolicy

  bool areRuntimeChecksDisabled(const MaterialKnowledgeDescription& d) {
    return d.getAttribute<bool>(
        MaterialKnowledgeDescription::disableRuntimeChecks, false);
  }  // end of getDisableRuntimeChecks

  std::string getDefaultOutOfBoundsPolicyAsString(
      const MaterialKnowledgeDescription& d) {
    const auto& policy = d.getAttribute<std::string>(
        MaterialKnowledgeDescription::defaultOutOfBoundsPolicy, "None");
    if ((policy != "None") && (policy != "Warning") && (policy != "Strict")) {
      tfel::raise(
          "getDefaultOutOfBoundsPolicyAsString:"
          " invalid default policy '" +
          policy + "'");
    }
    return policy;
  }  // end of getDefaultOutOfBoundsPolicyAsString

  std::string getDefaultOutOfBoundsPolicyAsUpperCaseString(
      const MaterialKnowledgeDescription& d) {
    const auto& policy = d.getAttribute<std::string>(
        MaterialKnowledgeDescription::defaultOutOfBoundsPolicy, "None");
    if (policy == "None") {
      return "NONE";
    } else if (policy == "Warning") {
      return "WARNING";
    } else if (policy == "Strict") {
      return "STRICT";
    } else {
      tfel::raise(
          "getDefaultOutOfBoundsPolicyAsUpperCaseString:"
          " invalid default policy '" +
          policy + "'");
    }
  }  // end of getDefaultOutOfBoundsPolicyAsString

  bool allowRuntimeModificationOfTheOutOfBoundsPolicy(
      const MaterialKnowledgeDescription& d) {
    return d.getAttribute<bool>(
        MaterialKnowledgeDescription::runtimeModificationOfTheOutOfBoundsPolicy,
        true);
  }  // end of allowRuntimeModificationOfTheOutOfBoundsPolicy

  bool areParametersTreatedAsStaticVariables(
      const MaterialKnowledgeDescription& d) {
    return d.getAttribute<bool>(
        MaterialKnowledgeDescription::parametersAsStaticVariables, false);
  }  // end of areParametersTreatedAsStaticVariables

  bool allowsParametersInitializationFromFile(
      const MaterialKnowledgeDescription& d) {
    return d.getAttribute<bool>(
        MaterialKnowledgeDescription::initializeParametersFromFile, true);
  }  // end of allowsParametersInitializationFromFile

}  // end of namespace mfront
