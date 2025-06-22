/*!
 * \file  ModelDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/ModelDescription.hxx"

namespace mfront {

  ModelDescription::Function::Function() = default;
  ModelDescription::Function::Function(const ModelDescription::Function&) =
      default;
  ModelDescription::Function::Function(ModelDescription::Function&&) = default;
  ModelDescription::Function& ModelDescription::Function::operator=(
      const ModelDescription::Function&) = default;
  ModelDescription::Function& ModelDescription::Function::operator=(
      ModelDescription::Function&&) = default;
  ModelDescription::Function::~Function() = default;

  std::pair<std::string, unsigned short>
  ModelDescription::decomposeVariableName(const std::string& n) const {
    auto get = [&n](const VariableDescriptionContainer& vc)
        -> std::pair<std::string, unsigned short> {
      using size_type = unsigned short;
      for (const auto& v : vc) {
        if (v.name == n) {
          return {v.name, 0u};
        }
        const auto d = v.getAttribute<size_type>(VariableDescription::depth, 0);
        for (size_type j = 1; j <= d; ++j) {
          auto fn = v.name + "_" + std::to_string(j);
          if (fn == n) {
            return {v.name, j};
          }
        }
      }
      return {};
    };
    auto r = get(this->outputs);
    if (!r.first.empty()) {
      return r;
    }
    r = get(this->inputs);
    if (r.first.empty()) {
      throw(
          std::runtime_error("decomposeVariableName: "
                             "field name '" +
                             n + "' has not been found"));
    }
    return r;
  }  // end of ModelDescription::decomposeVariableName

  ModelDescription::ModelDescription() = default;
  ModelDescription::ModelDescription(const ModelDescription&) = default;
  ModelDescription::ModelDescription(ModelDescription&&) = default;
  ModelDescription& ModelDescription::operator=(const ModelDescription&) =
      default;
  ModelDescription& ModelDescription::operator=(ModelDescription&&) = default;

  void ModelDescription::reserveName(const std::string& n) {
    if (!this->reservedNames.insert(n).second) {
      throw(
          std::runtime_error("ModelDescription::reserveName: "
                             "name '" +
                             n + "' already reserved"));
    }
  }

  bool ModelDescription::isNameReserved(const std::string& n) const {
    return this->reservedNames.count(n) != 0;
  }

  void ModelDescription::registerMemberName(const std::string& n) {
    this->reserveName(n);
    if (!this->memberNames.insert(n).second) {
      throw(
          std::runtime_error("ModelDescription::registerMemberName: "
                             "name '" +
                             n + "' already reserved"));
    }
  }  // end of ModelDescription::registerMemberName

  void ModelDescription::registerStaticMemberName(const std::string& n) {
    this->reserveName(n);
    if (!this->staticMemberNames.insert(n).second) {
      throw(
          std::runtime_error("ModelDescription::registerStaticMemberName: "
                             "name '" +
                             n + "' already reserved"));
    }
  }  // end of ModelDescription::registerStaticMemberName

  const std::string& ModelDescription::getExternalName(
      const std::string& n) const {
    this->checkVariableExistence(n);
    auto p = this->glossaryNames.find(n);
    if (p != this->glossaryNames.end()) {
      return p->second;
    }
    p = this->entryNames.find(n);
    if (p != this->entryNames.end()) {
      return p->second;
    }
    return n;
  }  // end of ModelDescription::getExternalName

  void ModelDescription::checkVariableExistence(const std::string& v) const {
    if ((!this->inputs.contains(v)) && (!this->outputs.contains(v)) &&
        (!this->parameters.contains(v)) && (!this->staticVars.contains(v)) &&
        (!this->constantMaterialProperties.contains(v))) {
      throw(
          std::runtime_error("ModelDescription::checkVariableExistence: "
                             "no variable named '" +
                             v + "'"));
    }
  }  // end of ModelDescription::checkVariableExistence

  bool ModelDescription::hasGlossaryName(const std::string& v) const {
    this->checkVariableExistence(v);
    return this->glossaryNames.count(v) != 0;
  }

  std::string ModelDescription::getGlossaryName(const std::string& v) const {
    this->checkVariableExistence(v);
    const auto p = this->glossaryNames.find(v);
    if (p == this->glossaryNames.end()) {
      throw(
          std::runtime_error("ModelDescription::getGlossaryName: "
                             "no glossary named defined for variable '" +
                             v + "'"));
    }
    return p->second;
  }  // end of ModelDescription::getGlossaryName

  void ModelDescription::setGlossaryName(const std::string& v,
                                         const std::string& g) {
    this->checkVariableExistence(v);
    const auto& glossary = tfel::glossary::Glossary::getGlossary();
    if (!glossary.contains(g)) {
      throw(
          std::runtime_error("ModelDescription::setGlossaryName: "
                             "no glossary name '" +
                             g + "'"));
    }
    if ((this->glossaryNames.find(v) != this->glossaryNames.end()) ||
        (this->entryNames.find(v) != this->entryNames.end())) {
      throw(
          std::runtime_error("ModelDescription::setGlossaryName: "
                             "an external name has already been set "
                             "for variable '" +
                             v + "'"));
    }
    if (v != g) {
      this->reserveName(g);
    }
    this->glossaryNames.insert({v, glossary.getGlossaryEntry(g).getKey()});
  }

  void ModelDescription::setEntryName(const std::string& v,
                                      const std::string& e) {
    this->checkVariableExistence(v);
    if (!tfel::utilities::CxxTokenizer::isValidIdentifier(e, false)) {
      throw(
          std::runtime_error("ModelDescription::setEntryName: "
                             "'" +
                             e + "' is a not a valid entry name"));
    }
    if (tfel::glossary::Glossary::getGlossary().contains(e)) {
      throw(
          std::runtime_error("ModelDescription::setEntryName: "
                             "'" +
                             e + "' is a glossary name"));
    }
    if ((this->glossaryNames.find(v) != this->glossaryNames.end()) ||
        (this->entryNames.find(v) != this->entryNames.end())) {
      throw(
          std::runtime_error("ModelDescription::setEntryName: "
                             "an external name has already been set "
                             "for variable '" +
                             v + "'"));
    }
    if (v != e) {
      this->reserveName(e);
    }
    this->entryNames.insert({v, e});
  }

  std::set<std::string>& ModelDescription::getReservedNames() {
    return this->reservedNames;
  }

  const std::set<std::string>& ModelDescription::getReservedNames() const {
    return this->reservedNames;
  }

  ModelDescription::~ModelDescription() = default;

}  // end of namespace mfront
