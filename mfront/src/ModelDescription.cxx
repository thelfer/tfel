/*!
 * \file  ModelDescription.cxx
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

#include <algorithm>
#include "TFEL/Raise.hxx"
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
    tfel::raise_if(r.first.empty(),
                   "decomposeVariableName: "
                   "field name '" +
                       n + "' has not been found");
    return r;
  }  // end of ModelDescription::decomposeVariableName

  ModelDescription::ModelDescription() = default;
  ModelDescription::ModelDescription(const ModelDescription&) = default;
  ModelDescription::ModelDescription(ModelDescription&&) = default;
  ModelDescription& ModelDescription::operator=(const ModelDescription&) =
      default;
  ModelDescription& ModelDescription::operator=(ModelDescription&&) = default;

  void ModelDescription::reserveName(const std::string& n) {
    tfel::raise_if(!this->reservedNames.insert(n).second,
                   "ModelDescription::reserveName: "
                   "name '" +
                       n + "' already reserved");
  }

  bool ModelDescription::isNameReserved(const std::string& n) const {
    return this->reservedNames.count(n) != 0;
  }

  void ModelDescription::registerMemberName(const std::string& n) {
    this->reserveName(n);
    tfel::raise_if(!this->memberNames.insert(n).second,
                   "ModelDescription::registerMemberName: "
                   "name '" +
                       n + "' already reserved");
  }  // end of ModelDescription::registerMemberName

  void ModelDescription::registerStaticMemberName(const std::string& n) {
    this->reserveName(n);
    tfel::raise_if(!this->staticMemberNames.insert(n).second,
                   "ModelDescription::registerStaticMemberName: "
                   "name '" +
                       n + "' already reserved");
  }  // end of ModelDescription::registerStaticMemberName

  VariableDescription& ModelDescription::getVariableDescription(
      const std::string& n) {
    if (this->outputs.contains(n)) {
      return this->outputs.getVariable(n);
    }
    if (this->inputs.contains(n)) {
      return this->inputs.getVariable(n);
    }
    if (this->parameters.contains(n)) {
      return this->parameters.getVariable(n);
    }
    if (this->constantMaterialProperties.contains(n)) {
      return this->constantMaterialProperties.getVariable(n);
    }
    tfel::raise(
        "ModelDescription::getVariableDescription: "
        "No variable named '" +
        n +
        "'.\n"
        "'" +
        n +
        "' is neither:\n"
        "- An output.\n"
        "- An input.\n"
        "- A parameter.\n"
        "- A constant material properties.");
  }  // end of ModelDescription::getVariableDescription

  const VariableDescription& ModelDescription::getVariableDescription(
      const std::string& n) const {
    if (this->outputs.contains(n)) {
      return this->outputs.getVariable(n);
    }
    if (this->inputs.contains(n)) {
      return this->inputs.getVariable(n);
    }
    if (this->parameters.contains(n)) {
      return this->parameters.getVariable(n);
    }
    if (this->constantMaterialProperties.contains(n)) {
      return this->constantMaterialProperties.getVariable(n);
    }
    tfel::raise(
        "ModelDescription::getVariableDescription: "
        "No variable named '" +
        n +
        "'.\n"
        "'" +
        n +
        "' is neither:\n"
        "- An output.\n"
        "- An input.\n"
        "- A parameter.\n"
        "- A constant material properties.");
  }  // end of ModelDescription::getVariableDescription

  void ModelDescription::checkVariableExistence(const std::string& v) const {
    tfel::raise_if((!this->inputs.contains(v)) &&
                       (!this->outputs.contains(v)) &&
                       (!this->parameters.contains(v)) &&
                       (!this->staticVars.contains(v)) &&
                       (!this->constantMaterialProperties.contains(v)),
                   "ModelDescription::checkVariableExistence: "
                   "no variable named '" +
                       v + "'");
  }  // end of ModelDescription::checkVariableExistence

  void ModelDescription::setGlossaryName(const std::string& v,
                                         const std::string& g) {
    this->checkVariableExistence(v);
    const auto& glossary = tfel::glossary::Glossary::getGlossary();
    tfel::raise_if(!glossary.contains(g),
                   "ModelDescription::setGlossaryName: "
                   "no glossary name '" +
                       g + "'");
    tfel::raise_if((this->glossaryNames.find(v) != this->glossaryNames.end()) ||
                       (this->entryNames.find(v) != this->entryNames.end()),
                   "ModelDescription::setGlossaryName: "
                   "an external name has already been set "
                   "for variable '" +
                       v + "'");
    if (v != g) {
      this->reserveName(g);
    }
    const auto gn = glossary.getGlossaryEntry(g).getKey();
    this->getVariableDescription(v).setGlossaryName(g);
    this->glossaryNames.insert({v, gn});
  }

  void ModelDescription::setEntryName(const std::string& v,
                                      const std::string& e) {
    this->checkVariableExistence(v);
    tfel::raise_if(!tfel::utilities::CxxTokenizer::isValidIdentifier(e, false),
                   "ModelDescription::setEntryName: "
                   "'" +
                       e + "' is a not a valid entry name");
    tfel::raise_if(tfel::glossary::Glossary::getGlossary().contains(e),
                   "ModelDescription::setEntryName: "
                   "'" +
                       e + "' is a glossary name");
    tfel::raise_if((this->glossaryNames.find(v) != this->glossaryNames.end()) ||
                       (this->entryNames.find(v) != this->entryNames.end()),
                   "ModelDescription::setEntryName: "
                   "an external name has already been set "
                   "for variable '" +
                       v + "'");
    if (v != e) {
      this->reserveName(e);
    }
    this->getVariableDescription(v).setEntryName(e);
    this->entryNames.insert({v, e});
  }

  std::set<std::string>& ModelDescription::getReservedNames() {
    return this->reservedNames;
  }

  const std::set<std::string>& ModelDescription::getReservedNames() const {
    return this->reservedNames;
  }

  void ModelDescription::addMaterialLaw(const std::string& m) {
    if (std::find(this->materialLaws.begin(), this->materialLaws.end(), m) ==
        this->materialLaws.end()) {
      this->materialLaws.push_back(m);
    }
  }  // end of ModelDescription::addMaterialLaw

  void ModelDescription::appendToIncludes(const std::string& c) {
    this->includes += c;
    if (!this->includes.empty()) {
      if (*(this->includes.rbegin()) != '\n') {
        this->includes += '\n';
      }
    }
  }  // end of ModelDescription::appendToIncludes

  void ModelDescription::appendToMembers(const std::string& c) {
    this->members += c;
    if (!this->members.empty()) {
      if (*(this->members.rbegin()) != '\n') {
        this->members += '\n';
      }
    }
  }  // end of ModelDescription::appendToMembers

  void ModelDescription::appendToPrivateCode(const std::string& c) {
    this->privateCode += c;
    if (!this->privateCode.empty()) {
      if (*(this->privateCode.rbegin()) != '\n') {
        this->privateCode += '\n';
      }
    }
  }  // end of ModelDescription::appendToPrivateCode

  void ModelDescription::appendToSources(const std::string& c) {
    this->sources += c;
    if (!this->sources.empty()) {
      if (*(this->sources.rbegin()) != '\n') {
        this->sources += '\n';
      }
    }
  }  // end of ModelDescription::appendToSources

  void ModelDescription::checkAndCompletePhysicalBoundsDeclaration() {
    if (this->hasUnitSystem()) {
      const auto& s = this->getUnitSystem();
      mfront::checkAndCompletePhysicalBoundsDeclaration(this->outputs, s);
      mfront::checkAndCompletePhysicalBoundsDeclaration(this->inputs, s);
      mfront::checkAndCompletePhysicalBoundsDeclaration(this->parameters, s);
    }
  }  // end of checkAndCompletePhysicalBoundsDeclaration

  ModelDescription::~ModelDescription() = default;

  BehaviourDescription convertToBehaviourDescription(
      const ModelDescription& md) {
    using tfel::material::ModellingHypothesis;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto* const Topt = BehaviourDescription::
        automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable;
    const auto opts = tfel::utilities::DataMap{{Topt, false}};
    auto bd = BehaviourDescription{opts};
    bd.setDSLName("Model");
    bd.declareAsGenericBehaviour();
    //
    if (md.use_qt.has_value()) {
      bd.setUseQt(*(md.use_qt));
    }
    // global options
    bd.setAttribute(MaterialKnowledgeDescription::buildIdentifier,
                    md.getAttribute<std::string>(
                        MaterialKnowledgeDescription::buildIdentifier, ""),
                    false);
    bd.setAttribute(MaterialKnowledgeDescription::defaultOutOfBoundsPolicy,
                    getDefaultOutOfBoundsPolicyAsString(md), false);
    bd.setAttribute(
        MaterialKnowledgeDescription::runtimeModificationOfTheOutOfBoundsPolicy,
        allowRuntimeModificationOfTheOutOfBoundsPolicy(md), false);
    bd.setAttribute(MaterialKnowledgeDescription::parametersAsStaticVariables,
                    areParametersTreatedAsStaticVariables(md), false);
    bd.setAttribute(MaterialKnowledgeDescription::initializeParametersFromFile,
                    allowsParametersInitializationFromFile(md), false);
    // exernal files
    for (const auto& efiles : md.getExternalMFrontFiles()) {
      bd.addExternalMFrontFile(efiles.first, std::get<0>(efiles.second),
                               std::get<1>(efiles.second));
    }
    //
    bd.setBehaviourName(md.modelName);
    if (!md.material.empty()) {
      bd.setMaterialName(md.material);
    }
    if (!md.library.empty()) {
      bd.setLibrary(md.library);
    }
    //
    const auto mhs = ModellingHypothesis::getModellingHypotheses();
    bd.setModellingHypotheses(
        std::set<ModellingHypothesis::Hypothesis>{mhs.begin(), mhs.end()});
    bd.addMaterialProperties(uh, md.constantMaterialProperties);
    bd.addStateVariables(uh, md.outputs);
    bd.addExternalStateVariables(uh, md.inputs);
    //
    for (const auto& v : md.staticVars) {
      bd.addStaticVariable(uh, v);
    }
    //
    for (const auto& m : md.materialLaws) {
      bd.addMaterialLaw(m);
    }
    //
    auto parameters = VariableDescriptionContainer{};
    for (const auto& p : md.parameters) {
      if (!((p.type == "double") || (p.isScalar()))) {
        continue;
      }
      bd.addParameter(uh, p);
      if (md.overriding_parameters.count(p.name) != 0) {
        bd.setParameterDefaultValue(uh, p.name,
                                    md.overriding_parameters.at(p.name));
      } else {
        const auto v =
            p.getAttribute<double>(VariableDescription::defaultValue);
        bd.setParameterDefaultValue(uh, p.name, v);
      }
    }
    return bd;
  }  // end of convertToBehaviourDescription

}  // end of namespace mfront
