/*!
 * \file   mfront/src/BehaviourData.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   18 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/ModelDescription.hxx"
#include "MFront/BehaviourData.hxx"

namespace mfront {

  const char* const BehaviourData::FlowRule = "FlowRule";
  const char* const BehaviourData::BeforeInitializeLocalVariables =
      "BeforeInitializeLocalVariables";
  const char* const BehaviourData::InitializeLocalVariables =
      "InitializeLocalVariables";
  const char* const BehaviourData::AfterInitializeLocalVariables =
      "AfterInitializeLocalVariables";
  const char* const BehaviourData::ComputePredictor = "ComputePredictor";
  const char* const BehaviourData::ComputeStressFreeExpansion =
      "ComputeStressFreeExpansion";
  const char* const BehaviourData::ComputeThermodynamicForces =
      "ComputeThermodynamicForces";
  const char* const BehaviourData::ComputeFinalThermodynamicForces =
      "ComputeFinalThermodynamicForces";
  const char* const BehaviourData::ComputeFinalThermodynamicForcesCandidate =
      "ComputeFinalThermodynamicForcesCandidate";
  const char* const BehaviourData::ComputeInternalEnergy =
      "ComputeInternalEnergy";
  const char* const BehaviourData::ComputeDissipatedEnergy =
      "ComputeDissipatedEnergy";
  const char* const BehaviourData::APrioriTimeStepScalingFactor =
      "APrioriTimeStepScalingFactor";
  const char* const BehaviourData::Integrator = "Integrator";
  const char* const BehaviourData::APosterioriTimeStepScalingFactor =
      "APosterioriTimeStepScalingFactor";
  const char* const BehaviourData::ComputeDerivative = "ComputeDerivative";
  const char* const BehaviourData::UpdateAuxiliaryStateVariables =
      "UpdateAuxiliaryStateVariables";
  const char* const BehaviourData::ComputePredictionOperator =
      "ComputePredictionOperator";
  const char* const BehaviourData::ComputeTangentOperator =
      "ComputeTangentOperator";
  const char* const BehaviourData::InitializeJacobian = "InitializeJacobian";
  const char* const BehaviourData::InitializeJacobianInvert =
      "InitializeJacobianInvert";
  const char* const BehaviourData::AdditionalConvergenceChecks =
      "AdditionalConvergenceChecks";

  const char* const BehaviourData::profiling = "profiling";
  const char* const BehaviourData::hasAPrioriTimeStepScalingFactor =
      "hasAPrioriTimeStepScalingFactor";
  const char* const BehaviourData::hasAPosterioriTimeStepScalingFactor =
      "hasAPosterioriTimeStepScalingFactor";
  const char* const BehaviourData::hasConsistentTangentOperator =
      "hasConsistentTangentOperator";
  const char* const BehaviourData::isConsistentTangentOperatorSymmetric =
      "isConsistentTangentOperatorSymmetric";
  const char* const BehaviourData::hasPredictionOperator =
      "hasPredictionOperator";
  const char* const BehaviourData::compareToNumericalJacobian =
      "compareToNumericalJacobian";
  const char* const BehaviourData::numericallyComputedJacobianBlocks =
      "numericallyComputedJacobianBlocks";
  const char* const BehaviourData::allowsNewUserDefinedVariables =
      "allowsNewUserDefinedVariables";
  const char* const BehaviourData::algorithm = "algorithm";
  const char* const BehaviourData::numberOfEvaluations = "numberOfEvaluations";

  /*!
   * check if the given name has already been used as value in the
   * glossary map or in the entry map
   * \param[in] gm : glossray names
   * \param[in] em : entry names
   * \param[in] n  : glossary name
   */
  static void BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName(
      const std::map<std::string, std::string>& gm,
      const std::map<std::string, std::string>& em,
      const std::string& n) {
    auto check = [&n](const std::map<std::string, std::string>& m,
                      const char* const t) {
      for (const auto& v : m) {
        tfel::raise_if(v.second == n,
                       "BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName: "
                       "name '" +
                           n + "' is already used as a " + std::string(t));
      }
    };
    check(gm, "glossary name");
    check(em, "entry name");
  }  // end of BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName

  /*!
   * \brief associate a glossary name or an entry name to a variable
   * \param[in] m  : map to be updated
   * \param[in] gn : glossary names
   * \param[in] en : entry names
   * \param[in] vn  : variable name
   * \param[in] n  : variable name
   * \param[in] g  : glossray name or entry name
   */
  static void BehaviourDataAddToGlossaryOrEntryNames(
      std::map<std::string, std::string>& m,
      const std::map<std::string, std::string>& gn,
      const std::map<std::string, std::string>& en,
      const std::set<std::string>& vn,
      const std::string& n,
      const std::string& g) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "BehaviourDataAddToGlossaryOrEntryNames: " + msg);
    };
    BehaviourDataCheckIfNameIsAnEntryNameOrAGlossaryName(gn, en, g);
    throw_if(en.find(n) != en.end(),
             "an entry name has already been specified for "
             "variable '" +
                 n + "'");
    throw_if(gn.find(n) != gn.end(),
             "a glossary name has already been specified for "
             "variable '" +
                 n + "'");
    if (n != g) {
      throw_if(vn.find(g) != vn.end(), "a member with the '" + g +
                                           "' name has already "
                                           "been declared");
    }
    throw_if(!m.insert({n, g}).second, "glossary name for variable '" + n +
                                           "' "
                                           "already specified");
  }

  void BehaviourData::throwUndefinedAttribute(const std::string& n) {
    tfel::raise(
        "BehaviourData::getAttribute: "
        "no attribute named '" +
        n + "'");
  }  // end of BehaviourData::throwUndefinedAttribute

  BehaviourData::CodeBlocksAggregator::CodeBlocksAggregator() = default;

  bool BehaviourData::CodeBlocksAggregator::isMutable() const {
    return this->is_mutable;
  }  // end of BehaviourData::CodeBlocksAggregator::isMutable

  void BehaviourData::CodeBlocksAggregator::update() {
    // updating code
    this->cblock.code = cblock_begin;
    if (!this->cblock_body.empty()) {
      if (!this->cblock.code.empty()) {
        if (*(this->cblock.code.rbegin()) != '\n') {
          this->cblock.code.push_back('\n');
        }
      }
    }
    this->cblock.code += cblock_body;
    if (!this->cblock_end.empty()) {
      if (!this->cblock.code.empty()) {
        if (*(this->cblock.code.rbegin()) != '\n') {
          this->cblock.code.push_back('\n');
        }
      }
    }
    this->cblock.code += cblock_end;
    // updating description
    this->cblock.description = cdoc_begin;
    if (!this->cdoc_body.empty()) {
      if (!this->cblock.description.empty()) {
        if (*(this->cblock.description.rbegin()) != '\n') {
          this->cblock.description.push_back('\n');
        }
      }
    }
    this->cblock.description += cdoc_body;
    if (!this->cdoc_end.empty()) {
      if (!this->cblock.description.empty()) {
        if (*(this->cblock.description.rbegin()) != '\n') {
          this->cblock.description.push_back('\n');
        }
      }
    }
    this->cblock.description += cdoc_end;
  }  // end of BehaviourData::update

  void BehaviourData::CodeBlocksAggregator::set(const CodeBlock& c,
                                                const Position p,
                                                const bool b) {
    this->check();
    this->cblock.staticMembers.insert(c.staticMembers.begin(),
                                      c.staticMembers.end());
    this->cblock.members.insert(c.members.begin(), c.members.end());
    switch (p) {
      case AT_BEGINNING:
        if (!this->cblock_begin.empty()) {
          this->cblock_begin += '\n';
        }
        this->cblock_begin += c.code;
        if (!c.description.empty()) {
          if (!this->cdoc_begin.empty()) {
            this->cdoc_begin += '\n';
          }
          this->cdoc_begin += c.description;
        }
        break;
      case BODY:
        if (!this->cblock_body.empty()) {
          this->cblock_body += '\n';
        }
        this->cblock_body += c.code;
        if (!c.description.empty()) {
          if (!this->cdoc_body.empty()) {
            this->cdoc_body += '\n';
          }
          this->cdoc_body += c.description;
        }
        break;
      case AT_END:
        if (!this->cblock_end.empty()) {
          this->cblock_end += '\n';
        }
        this->cblock_end += c.code;
        if (!c.description.empty()) {
          if (!this->cdoc_end.empty()) {
            this->cdoc_end += '\n';
          }
          this->cdoc_end += c.description;
        }
        break;
    }
    this->update();
    this->is_mutable = b;
  }  // end of BehaviourData::CodeBlocksAggregator::set

  void BehaviourData::CodeBlocksAggregator::replace(const CodeBlock& c,
                                                    const Position p,
                                                    const bool b) {
    this->check();
    this->cblock_begin.clear();
    this->cblock_body.clear();
    this->cblock_end.clear();
    this->cblock.code.clear();
    this->cblock.members.clear();
    this->cblock.staticMembers.clear();
    this->set(c, p, b);
  }  // end of BehaviourData::CodeBlocksAggregator::set

  void BehaviourData::CodeBlocksAggregator::check() const {
    tfel::raise_if(!this->is_mutable,
                   "BehaviourData::CodeBlocksAggregator::set: "
                   "can't modifiy a code block");
  }  // end of BehaviourData::CodeBlocksAggregator::check

  const CodeBlock& BehaviourData::CodeBlocksAggregator::get() const {
    this->is_mutable = false;
    return this->cblock;
  }  // end of BehaviourData::CodeBlocksAggregator::get

  BehaviourData::CodeBlocksAggregator::~CodeBlocksAggregator() = default;

  BehaviourData::BehaviourData() {
    this->registerMemberName("dt");
    this->reserveName("\u0394t");  // symbolic value
    // treating the temperature
    auto T = VariableDescription{"temperature", "T", 1u, 0u};
    T.setGlossaryName("Temperature");
    this->addExternalStateVariable(T, UNREGISTRED);
  }  // end of BehaviourData::BehaviourData()

  BehaviourData::BehaviourData(const BehaviourData&) = default;

  static void checkAlreadyRegistred(const std::set<std::string>& r,
                                    const std::string& n) {
    if (r.find(n) == r.end()) {
      tfel::raise(
          "checkAlreadyRegistred: "
          "variable '" +
          n + "' was not registred");
    }
  }

  void BehaviourData::addStaticVariable(const StaticVariableDescription& v,
                                        const RegistrationStatus s) {
    if ((this->hasAttribute(BehaviourData::allowsNewUserDefinedVariables)) &&
        (!this->getAttribute<bool>(
            BehaviourData::allowsNewUserDefinedVariables))) {
      const auto cbn = this->getCodeBlockNames();
      tfel::raise_if(cbn.empty(),
                     "BehaviourData::addStaticVariable: "
                     "no more variable can be defined. This may mean that "
                     "the parser does not expect you to add variables");
      auto cbs = std::string{};
      for (const auto& n : cbn) {
        cbs += "\n- " + n;
      }
      tfel::raise(
          "BehaviourData::addStaticVariable: "
          "no more variable can be defined. This may mean that "
          "you already declared a block of code (or that the dsl "
          "does not expect you to add variables for whatever reason). "
          "This is the list of "
          "code blocks defined :" +
          cbs);
    }
    if (s == UNREGISTRED) {
      this->registerStaticMemberName(v.name);
    } else {
      checkAlreadyRegistred(this->reservedNames, v.name);
    }
    this->staticVariables.push_back(v);
  }  // end of BehaviourData::addStaticVariable

  int BehaviourData::getIntegerConstant(const std::string& n) const {
    for (const auto& v : this->staticVariables) {
      if (v.name == n) {
        tfel::raise_if(v.type != "int",
                       "MaterialPropertyDSL::getIntegerConstant: "
                       "invalid type for variable '" +
                           n + "'");
        return v.value;
      }
    }
    tfel::raise(
        "MaterialPropertyDSL::getIntegerConstant: "
        "unknown variable '" +
        n + "'");
  }  // end of BehaviourData::getIntegerConstant

  const StaticVariableDescriptionContainer& BehaviourData::getStaticVariables()
      const {
    return this->staticVariables;
  }  // end of BehaviourData::getStaticVariables

  const VariableDescription& BehaviourData::getPersistentVariableDescription(
      const std::string& v) const {
    return this->getPersistentVariables().getVariable(v);
  }  // end of BehaviourData::getPersistentVariableDescription

  const VariableDescription&
  BehaviourData::getPersistentVariableDescriptionByExternalName(
      const std::string& v) const {
    return this->getPersistentVariables().getVariableByExternalName(v);
  }  // end of BehaviourData::getPersistentVariableDescriptionByExternalName

  const VariableDescription& BehaviourData::getIntegrationVariableDescription(
      const std::string& v) const {
    return this->getIntegrationVariables().getVariable(v);
  }  // end of BehaviourData::getIntegrationVariableDescription

  const VariableDescription&
  BehaviourData::getIntegrationVariableDescriptionByExternalName(
      const std::string& v) const {
    return this->getIntegrationVariables().getVariableByExternalName(v);
  }  // end of BehaviourData::getIntegrationVariableDescriptionByExternalName

  const VariableDescription& BehaviourData::getStateVariableDescription(
      const std::string& v) const {
    return this->getStateVariables().getVariable(v);
  }  // end of BehaviourData::getStateVariableDescription

  const VariableDescription&
  BehaviourData::getStateVariableDescriptionByExternalName(
      const std::string& v) const {
    return this->getStateVariables().getVariableByExternalName(v);
  }  // end of BehaviourData::getStateVariableDescriptionByExternalName

  const VariableDescription& BehaviourData::getExternalStateVariableDescription(
      const std::string& v) const {
    return this->getExternalStateVariables().getVariable(v);
  }  // end of BehaviourData::getExternalStateVariableDescription

  const VariableDescription&
  BehaviourData::getExternalStateVariableDescriptionByExternalName(
      const std::string& v) const {
    return this->getExternalStateVariables().getVariableByExternalName(v);
  }  // end of BehaviourData::getExternalStateVariableDescriptionByExternalName

  const VariableDescription&
  BehaviourData::getAuxiliaryStateVariableDescription(
      const std::string& v) const {
    return this->getAuxiliaryStateVariables().getVariable(v);
  }  // end of BehaviourData::getAuxiliaryStateVariableDescription

  const VariableDescription&
  BehaviourData::getAuxiliaryStateVariableDescriptionByExternalName(
      const std::string& v) const {
    return this->getAuxiliaryStateVariables().getVariableByExternalName(v);
  }  // end of BehaviourData::getAuxiliaryStateVariableDescriptionByExternalName

  const VariableDescription& BehaviourData::getParameterDescription(
      const std::string& v) const {
    return this->getParameters().getVariable(v);
  }  // end of BehaviourData::getParameterDescription

  const VariableDescription&
  BehaviourData::getParameterDescriptionByExternalName(
      const std::string& v) const {
    return this->getParameters().getVariableByExternalName(v);
  }  // end of BehaviourData::getParameterDescriptionByExternalName

  void BehaviourData::addMaterialProperty(const VariableDescription& v,
                                          const RegistrationStatus s) {
    this->addVariable(this->materialProperties, v, s, false);
  }  // end of BehaviourData::addMaterialProperty

  void BehaviourData::addIntegrationVariable(const VariableDescription& v,
                                             const RegistrationStatus s) {
    this->addVariable(this->integrationVariables, v, s, true);
  }  // end of BehaviourData::addIntegrationVariable

  void BehaviourData::addStateVariable(const VariableDescription& v,
                                       const RegistrationStatus s) {
    this->addVariable(this->stateVariables, v, s, true);
    if (s == FORCEREGISTRATION) {
      this->addVariable(this->integrationVariables, v, ALREADYREGISTRED, true,
                        true);
    } else {
      this->addVariable(this->integrationVariables, v, ALREADYREGISTRED, true);
    }
    /*!
     * for compatibility reasons with previous mfront versions
     * (<2.0), auxiliary state variables shall be put after
     * state variables.
     */
    bool found = false;
    auto p = this->persistentVariables.begin();
    while (p != this->persistentVariables.end()) {
      if (this->isAuxiliaryStateVariableName(p->name)) {
        this->persistentVariables.insert(p, v);
        found = true;
        break;
      } else {
        ++p;
      }
    }
    if (!found) {
      this->persistentVariables.push_back(v);
    }
  }  // end of BehaviourData::addStateVariable

  void BehaviourData::addAuxiliaryStateVariable(const VariableDescription& v,
                                                const RegistrationStatus s) {
    this->addVariable(this->auxiliaryStateVariables, v, s, false);
    if (s == FORCEREGISTRATION) {
      this->addVariable(this->persistentVariables, v, ALREADYREGISTRED, false,
                        true);
    } else {
      this->addVariable(this->persistentVariables, v, ALREADYREGISTRED, false);
    }
  }  // end of BehaviourData::addAuxiliaryStateVariable

  void BehaviourData::addExternalStateVariable(const VariableDescription& v,
                                               const RegistrationStatus s) {
    this->addVariable(this->externalStateVariables, v, s, true);
  }  // end of BehaviourData::addExternalStateVariable

  void BehaviourData::addLocalVariable(const VariableDescription& v,
                                       const RegistrationStatus s) {
    this->addVariable(this->localVariables, v, s, false, true);
  }  // end of BehaviourData::addLocalVariable

  void BehaviourData::addParameter(const VariableDescription& v,
                                   const RegistrationStatus s) {
    this->addVariable(this->parameters, v, s, false);
  }  // end of BehaviourData::addParameter

  bool BehaviourData::hasParameter(const std::string& n) const {
    return this->parameters.contains(n);
  }

  bool BehaviourData::hasParameters() const {
    return !this->parameters.empty();
  }

  bool BehaviourData::isMemberUsedInCodeBlocks(const std::string& v) const {
    for (const auto& c : this->cblocks) {
      const auto& m = c.second.get().members;
      if (m.find(v) != m.end()) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourData::isMemberUsedInCodeBlocks

  bool BehaviourData::isMaterialPropertyName(const std::string& n) const {
    return this->getMaterialProperties().contains(n);
  }  // end of BehaviourData::isMaterialPropertyName

  bool BehaviourData::isStaticVariableName(const std::string& n) const {
    return this->getStaticVariables().contains(n);
  }  // end of BehaviourData::isStaticVariableName

  bool BehaviourData::isLocalVariableName(const std::string& n) const {
    return this->getLocalVariables().contains(n);
  }  // end of BehaviourData::isLocalVariableName

  bool BehaviourData::isPersistentVariableName(const std::string& n) const {
    return this->getPersistentVariables().contains(n);
  }  // end of BehaviourData::isPersistentVariableName

  bool BehaviourData::isIntegrationVariableName(const std::string& n) const {
    return this->getIntegrationVariables().contains(n);
  }  // end of BehaviourData::isIntegrationVariableName

  bool BehaviourData::isIntegrationVariableIncrementName(
      const std::string& n) const {
    if (n.size() < 2) {
      return false;
    }
    if (n[0] != 'd') {
      return false;
    }
    return this->getIntegrationVariables().contains(n.substr(1));
  }  // end of BehaviourData::isIntegrationVariableName

  bool BehaviourData::isStateVariableName(const std::string& n) const {
    return this->getStateVariables().contains(n);
  }  // end of BehaviourData::isStateVariableName

  bool BehaviourData::isStateVariableIncrementName(const std::string& n) const {
    if (n.size() < 2) {
      return false;
    }
    if (n[0] != 'd') {
      return false;
    }
    return this->getStateVariables().contains(n.substr(1));
  }  // end of BehaviourData::isStateVariableName

  bool BehaviourData::isAuxiliaryStateVariableName(const std::string& n) const {
    return this->getAuxiliaryStateVariables().contains(n);
  }  // end of BehaviourData::isStateVariableName

  bool BehaviourData::isExternalStateVariableName(const std::string& n) const {
    return this->getExternalStateVariables().contains(n);
  }  // end of BehaviourData::isExternalStateVariableName

  bool BehaviourData::isExternalStateVariableIncrementName(
      const std::string& n) const {
    if (n.size() < 2) {
      return false;
    }
    if (n[0] != 'd') {
      return false;
    }
    return this->getExternalStateVariables().contains(n.substr(1));
  }  // end of BehaviourData::isExternalStateVariableName

  bool BehaviourData::isParameterName(const std::string& n) const {
    return this->getParameters().contains(n);
  }  // end of BehaviourData::isParameterName

  const VariableDescriptionContainer& BehaviourData::getMaterialProperties()
      const {
    return this->materialProperties;
  }  // end of BehaviourData::getMaterialProperties

  const VariableDescriptionContainer& BehaviourData::getPersistentVariables()
      const {
    return this->persistentVariables;
  }  // end of BehaviourData::getPersistentVariables

  std::set<std::string> BehaviourData::getVariablesNames() const {
    auto getNames = [](std::set<std::string>& r,
                       const VariableDescriptionContainer& c) {
      for (const auto& v : c) {
        tfel::raise_if(!r.insert(v.name).second,
                       "BehaviourData::getVariablesNames: "
                       "internal error, variable name "
                       "'" +
                           v.name + "' multiply defined");
      }
    };
    auto n = std::set<std::string>{};
    getNames(n, this->getMaterialProperties());
    getNames(n, this->getStateVariables());
    getNames(n, this->getAuxiliaryStateVariables());
    getNames(n, this->getExternalStateVariables());
    getNames(n, this->getParameters());
    return n;
  }  // end of BehaviourData::getVariablesNames

  const VariableDescriptionContainer& BehaviourData::getVariables(
      const std::string& t) const {
    const VariableDescriptionContainer& (BehaviourData::*m)() const;
    if (t == "MaterialProperty") {
      m = &BehaviourData::getMaterialProperties;
    } else if (t == "PersistentVariable") {
      m = &BehaviourData::getPersistentVariables;
    } else if (t == "IntegrationVariable") {
      m = &BehaviourData::getIntegrationVariables;
    } else if (t == "StateVariable") {
      m = &BehaviourData::getStateVariables;
    } else if (t == "AuxiliaryStateVariable") {
      m = &BehaviourData::getAuxiliaryStateVariables;
    } else if (t == "ExternalStateVariable") {
      m = &BehaviourData::getExternalStateVariables;
    } else if (t == "Parameter") {
      m = &BehaviourData::getParameters;
    } else {
      tfel::raise(
          "BehaviourData::getVariables: "
          "invalid variables type '" +
          t + "'");
    }
    return (this->*m)();
  }  // end of BehaviourData::getIntegrationVariables

  const VariableDescriptionContainer& BehaviourData::getIntegrationVariables()
      const {
    return this->integrationVariables;
  }  // end of BehaviourData::getIntegrationVariables

  const VariableDescriptionContainer& BehaviourData::getStateVariables() const {
    return this->stateVariables;
  }  // end of BehaviourData::getStateVariables

  const VariableDescriptionContainer&
  BehaviourData::getAuxiliaryStateVariables() const {
    return this->auxiliaryStateVariables;
  }  // end of BehaviourData::getAuxiliaryStateVariables

  const VariableDescriptionContainer& BehaviourData::getExternalStateVariables()
      const {
    return this->externalStateVariables;
  }  // end of BehaviourData::getExternalStateVariables

  const VariableDescriptionContainer& BehaviourData::getLocalVariables() const {
    return this->localVariables;
  }  // end of BehaviourData::getLocalVariables

  bool BehaviourData::isUsableInPurelyImplicitResolution() const {
    return this->usableInPurelyImplicitResolution;
  }  // end of BehaviourData::isUsableInPurelyImplicitResolution

  void BehaviourData::setBounds(const std::string& n,
                                const VariableBoundsDescription& b) {
    bool treated = false;
    auto set_bounds = [&n, &b, &treated](VariableDescriptionContainer& c) {
      if (c.contains(n)) {
        c.getVariable(n).setBounds(b);
        treated = true;
      }
    };
    set_bounds(this->materialProperties);
    set_bounds(this->localVariables);
    set_bounds(this->stateVariables);
    set_bounds(this->auxiliaryStateVariables);
    set_bounds(this->integrationVariables);
    set_bounds(this->persistentVariables);
    set_bounds(this->externalStateVariables);
    set_bounds(this->parameters);
    tfel::raise_if(!treated,
                   "BehaviourData::setBounds: "
                   "no variable named '" +
                       n + "'");
  }  // end of BehaviourData::getBounds

  void BehaviourData::setBounds(const std::string& n,
                                const unsigned short i,
                                const VariableBoundsDescription& b) {
    bool treated = false;
    auto set_bounds = [&n, i, &b, &treated](VariableDescriptionContainer& c) {
      if (c.contains(n)) {
        c.getVariable(n).setBounds(b, i);
        treated = true;
      }
    };
    set_bounds(this->materialProperties);
    set_bounds(this->localVariables);
    set_bounds(this->stateVariables);
    set_bounds(this->auxiliaryStateVariables);
    set_bounds(this->integrationVariables);
    set_bounds(this->persistentVariables);
    set_bounds(this->externalStateVariables);
    set_bounds(this->parameters);
    tfel::raise_if(!treated,
                   "BehaviourData::setBounds: "
                   "no variable named '" +
                       n + "'");
  }

  const VariableDescription&
  BehaviourData::getVariableDescriptionByExternalName(
      const std::string& n) const {
    auto contains = [&n](const VariableDescriptionContainer& vc) {
      for (const auto& v : vc) {
        if (v.getExternalName() == n) {
          return true;
        }
      }
      return false;
    };
    if (contains(this->materialProperties)) {
      return this->materialProperties.getVariableByExternalName(n);
    }
    if (contains(this->localVariables)) {
      return this->localVariables.getVariableByExternalName(n);
    }
    if (contains(this->stateVariables)) {
      return this->stateVariables.getVariableByExternalName(n);
    }
    if (contains(this->auxiliaryStateVariables)) {
      return this->auxiliaryStateVariables.getVariableByExternalName(n);
    }
    if (contains(this->integrationVariables)) {
      return this->integrationVariables.getVariableByExternalName(n);
    }
    if (contains(this->externalStateVariables)) {
      return this->externalStateVariables.getVariableByExternalName(n);
    }
    if (contains(this->parameters)) {
      return this->parameters.getVariableByExternalName(n);
    }
    tfel::raise(
        "BehaviourData::getVariableDescriptionByExternalName: "
        "no variable with external name '" +
        n +
        "' found. "
        "Such variable is *not*:\n"
        "- a material property\n"
        "- a local variable\n"
        "- a state variable\n"
        "- an auxiliary state variable\n"
        "- an integration variable\n"
        "- an external state variable\n"
        "- a parameter");
  }  // end of BehaviourData::getVariableDescriptionByExternalName

  const VariableDescription& BehaviourData::getVariableDescription(
      const std::string& n) const {
    if (this->materialProperties.contains(n)) {
      return this->materialProperties.getVariable(n);
    }
    if (this->localVariables.contains(n)) {
      return this->localVariables.getVariable(n);
    }
    if (this->stateVariables.contains(n)) {
      return this->stateVariables.getVariable(n);
    }
    if (this->auxiliaryStateVariables.contains(n)) {
      return this->auxiliaryStateVariables.getVariable(n);
    }
    if (this->integrationVariables.contains(n)) {
      return this->integrationVariables.getVariable(n);
    }
    if (this->externalStateVariables.contains(n)) {
      return this->externalStateVariables.getVariable(n);
    }
    if (this->parameters.contains(n)) {
      return this->parameters.getVariable(n);
    }
    tfel::raise(
        "BehaviourData::getVariableDescription: "
        "no variable named '" +
        n +
        "' found. "
        "This variable is *not*:\n"
        "- a material property\n"
        "- a local variable\n"
        "- a state variable\n"
        "- an auxiliary state variable\n"
        "- an integration variable\n"
        "- an external state variable\n"
        "- a parameter");
  }  // end of BehaviourData::getVariableDescription

  void BehaviourData::setVariableAttribute(const std::string& v,
                                           const std::string& n,
                                           const VariableAttribute& a,
                                           const bool b) {
    bool treated = false;
    auto set_if = [&v, &n, &a, b, &treated](VariableDescriptionContainer& vc) {
      if (vc.contains(v)) {
        vc.getVariable(v).setAttribute(n, a, b);
        treated = true;
      }
    };
    set_if(this->materialProperties);
    set_if(this->localVariables);
    set_if(this->integrationVariables);
    set_if(this->stateVariables);
    set_if(this->auxiliaryStateVariables);
    set_if(this->persistentVariables);
    set_if(this->externalStateVariables);
    set_if(this->parameters);
    tfel::raise_if(!treated,
                   "BehaviourData::setVariableAttribute: "
                   "no variable named '" +
                       n +
                       "' found."
                       "This variable is *not*:\n"
                       "- a material property\n"
                       "- a local variable\n"
                       "- a state variable\n"
                       "- an auxiliary state variable\n"
                       "- an integration variable\n"
                       "- a  persistent variable\n"
                       "- an external state variable\n"
                       "- a parameter");
  }  // end of BehaviourData::setVariableAttribute

  VariableDescription& BehaviourData::getVariableDescription(
      const std::string& n) {
    if (this->materialProperties.contains(n)) {
      return this->materialProperties.getVariable(n);
    }
    if (this->localVariables.contains(n)) {
      return this->localVariables.getVariable(n);
    }
    if (this->stateVariables.contains(n)) {
      return this->stateVariables.getVariable(n);
    }
    if (this->auxiliaryStateVariables.contains(n)) {
      return this->auxiliaryStateVariables.getVariable(n);
    }
    if (this->integrationVariables.contains(n)) {
      return this->integrationVariables.getVariable(n);
    }
    if (this->externalStateVariables.contains(n)) {
      return this->externalStateVariables.getVariable(n);
    }
    if (this->parameters.contains(n)) {
      return this->parameters.getVariable(n);
    }
    tfel::raise(
        "BehaviourData::getVariableDescription: "
        "no variable named '" +
        n +
        "' found."
        "This variable is *not*:\n"
        "- a material property\n"
        "- a local variable\n"
        "- a state variable\n"
        "- an auxiliary state variable\n"
        "- an integration variable\n"
        "- an external state variable\n"
        "- a parameter");
  }  // end of BehaviourData::getVariableDescription

  void BehaviourData::setPhysicalBounds(const std::string& n,
                                        const VariableBoundsDescription& b) {
    bool treated = false;
    auto set_bounds = [&n, &b, &treated](VariableDescriptionContainer& c) {
      if (c.contains(n)) {
        c.getVariable(n).setPhysicalBounds(b);
        treated = true;
      }
    };
    set_bounds(this->materialProperties);
    set_bounds(this->localVariables);
    set_bounds(this->stateVariables);
    set_bounds(this->auxiliaryStateVariables);
    set_bounds(this->integrationVariables);
    set_bounds(this->persistentVariables);
    set_bounds(this->externalStateVariables);
    set_bounds(this->parameters);
    tfel::raise_if(!treated,
                   "BehaviourData::setPhysicalBounds: "
                   "no variable named '" +
                       n + "'");
  }  // end of BehaviourData::setPhysicalBounds

  void BehaviourData::setPhysicalBounds(const std::string& n,
                                        const unsigned short i,
                                        const VariableBoundsDescription& b) {
    bool treated = false;
    auto set_bounds = [&n, &i, &b, &treated](VariableDescriptionContainer& c) {
      if (c.contains(n)) {
        c.getVariable(n).setPhysicalBounds(b, i);
        treated = true;
      }
    };
    set_bounds(this->materialProperties);
    set_bounds(this->localVariables);
    set_bounds(this->stateVariables);
    set_bounds(this->auxiliaryStateVariables);
    set_bounds(this->integrationVariables);
    set_bounds(this->persistentVariables);
    set_bounds(this->externalStateVariables);
    set_bounds(this->parameters);
    tfel::raise_if(!treated,
                   "BehaviourData::setPhysicalBounds: "
                   "no variable named '" +
                       n + "'");
  }  // end of BehaviourData::setPhysicalBounds

  void BehaviourData::setUsableInPurelyImplicitResolution(const bool b) {
    this->usableInPurelyImplicitResolution = b;
  }  // end of BehaviourData::setUsableInPurelyImplicitResolution

  void BehaviourData::
      declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          const std::string& n) {
    this->pupirv.insert(n);
  }  // end of
     // BehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  const std::set<std::string>& BehaviourData::
      getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution()
          const {
    return this->pupirv;
  }  // end of
     // BehaviourData::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution

  void BehaviourData::addVariable(VariableDescriptionContainer& c,
                                  const VariableDescription& v,
                                  const RegistrationStatus s,
                                  const bool bi,
                                  const bool b) {
    if ((v.hasGlossaryName()) && (v.hasEntryName())) {
      tfel::raise(
          "BehaviourData::addVariable: "
          "variable '" +
          v.name + "' declares a glossary name and an entry name.");
    }
    if ((!b) && (s != FORCEREGISTRATION)) {
      if ((this->hasAttribute(BehaviourData::allowsNewUserDefinedVariables)) &&
          (!this->getAttribute<bool>(
              BehaviourData::allowsNewUserDefinedVariables))) {
        const auto cbn = this->getCodeBlockNames();
        tfel::raise_if(
            cbn.empty(),
            "BehaviourData::addVariable: can't add variable '" + v.name +
                "', "
                "no more variable can be defined. This may mean that "
                "the parser does not expect you to add variables");
        auto cbs = std::string{};
        for (const auto& n : cbn) {
          cbs += "\n- " + n;
        }
        tfel::raise(
            "BehaviourData::addVariable: can't add variable '" + v.name +
            "', "
            "no more variable can be defined. This may mean that "
            "you already declared a block of code (or that the dsl "
            "does not expect you to add variables for whatever reason). "
            "This is the list of "
            "code blocks defined :" +
            cbs);
      }
    }
    if (s == ALREADYREGISTRED) {
      checkAlreadyRegistred(this->reservedNames, v.name);
      if (!v.symbolic_form.empty()) {
        checkAlreadyRegistred(this->reservedNames, v.symbolic_form);
      }
      if (bi) {
        checkAlreadyRegistred(this->membersNames, "d" + v.name);
        if (!v.symbolic_form.empty()) {
          checkAlreadyRegistred(this->reservedNames,
                                "\u0394" + v.symbolic_form);
        } else {
          checkAlreadyRegistred(this->reservedNames, "\u0394" + v.name);
        }
      }
      if (v.hasGlossaryName()) {
        const auto pe = this->entryNames.find(v.name);
        if (pe != this->entryNames.end()) {
          tfel::raise(
              "BehaviourData::addVariable: "
              "already registred variable '" +
              v.name + "' with entry name '" + pe->second + "' now declares '" +
              v.getExternalName() + "' as glossary name");
        }
        const auto p = this->glossaryNames.find(v.name);
        if (p == this->glossaryNames.end()) {
          this->registerGlossaryName(v.name, v.getExternalName());
        } else {
          if (p->second != v.getExternalName()) {
            tfel::raise(
                "BehaviourData::addVariable: "
                "unmatched glossary names for already registred variable '" +
                v.name + "' (" + p->second + " vs " + v.getExternalName() +
                ")");
          }
        }
      }
      if (v.hasEntryName()) {
        const auto pg = this->glossaryNames.find(v.name);
        if (pg != this->glossaryNames.end()) {
          tfel::raise(
              "BehaviourData::addVariable: "
              "already registred variable '" +
              v.name + "' with glossary name '" + pg->second +
              "' now declares '" + v.getExternalName() + "' as entry name");
        }
        const auto p = this->entryNames.find(v.name);
        if (p == this->entryNames.end()) {
          this->registerEntryName(v.name, v.getExternalName());
        } else {
          if (p->second != v.getExternalName()) {
            tfel::raise(
                "BehaviourData::addVariable: "
                "unmatched entry names for already registred variable '" +
                v.name + "' (" + p->second + " vs " + v.getExternalName() +
                ")");
          }
        }
      }
    } else {
      // Unregistred variable
      this->registerMemberName(v.name);
      if (!v.symbolic_form.empty()) {
        this->reserveName(v.symbolic_form);
      }
      if (bi) {
        this->registerMemberName("d" + v.name);
        if (!v.symbolic_form.empty()) {
          this->reserveName("\u0394" + v.symbolic_form);
        } else {
          this->reserveName("\u0394" + v.name);
        }
      }
      if (v.hasGlossaryName()) {
        this->registerGlossaryName(v.name, v.getExternalName());
      }
      if (v.hasEntryName()) {
        this->registerEntryName(v.name, v.getExternalName());
      }
    }
    c.push_back(v);
  }  // end of BehaviourData::addVariable

  void BehaviourData::reserveName(const std::string& n) {
    tfel::raise_if(!this->reservedNames.insert(n).second,
                   "BehaviourData::reserveName: "
                   "name '" +
                       n + "' already registred");
  }  // end of BehaviourData::reserveName

  bool BehaviourData::isNameReserved(const std::string& n) const {
    return this->reservedNames.count(n) != 0;
  }

  void BehaviourData::registerGlossaryName(const std::string& n,
                                           const std::string& g) {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    if (glossary.contains(n)) {
      std::ostringstream msg;
      msg << "BehaviourData::registerEntryName: "
          << "the name '" << n << "' is a registred as a glossary name.\n";
      displayGlossaryEntryCompleteDescription(msg,
                                              glossary.getGlossaryEntry(n));
      tfel::raise(msg.str());
    }
    if (!glossary.contains(g)) {
      tfel::raise(
          "BehaviourData::registerGlossaryName: "
          "the name '" +
          g + "' is a not known as a glossary name");
    }
    if (!this->isNameReserved(n)) {
      tfel::raise(
          "BehaviourData::registerGlossaryName: "
          "the variable name '" +
          n + "' not is registred");
    }
    /*
     * Special exceptions for glossary name which are also supported type name:
     * those names are already registred and no clash ispossible.
     */
    const auto& flags = SupportedTypes::getTypeFlags();
    if (flags.find(g) == flags.end()) {
      this->reserveName(g);
    }
    tfel::raise_if(!this->glossaryNames.insert({n, g}).second,
                   "BehaviourData::registerGlossaryName: "
                   "a variable named '" +
                       n + "' has already been registred");
  }  // end of BehaviourData::registerGlossaryName

  void BehaviourData::registerEntryName(const std::string& n,
                                        const std::string& e) {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    if (glossary.contains(e)) {
      std::ostringstream msg;
      msg << "BehaviourData::registerEntryName: "
          << "the name '" << e << "' is a registred as a glossary name.\n";
      displayGlossaryEntryCompleteDescription(msg,
                                              glossary.getGlossaryEntry(e));
      tfel::raise(msg.str());
    }
    if (!this->isNameReserved(n)) {
      tfel::raise(
          "BehaviourData::registerGlossaryName: "
          "the variable name '" +
          n + "' is registred");
    }
    /*
     * Special exceptions for glossary name which are also supported type name:
     * those names are already registred and no clash ispossible.
     */
    const auto& flags = SupportedTypes::getTypeFlags();
    if (flags.find(e) == flags.end()) {
      this->reserveName(e);
    }
    tfel::raise_if(!this->entryNames.insert({n, e}).second,
                   "BehaviourData::registerEntryName: "
                   "a variable named '" +
                       n + "' has already been registred");
  }  // end of BehaviourData::registerEntryName

  void BehaviourData::registerMemberName(const std::string& n) {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    for (auto& e : this->entryNames) {
      tfel::raise_if(e.second == n,
                     "BehaviourData::registerMemberName: "
                     "the name '" +
                         n +
                         "' is already been used "
                         "for an entry name");
    }
    if (glossary.contains(n)) {
      std::ostringstream msg;
      msg << "BehaviourData::registerMemberName: "
          << "the name '" << n << "' is a registred as a glossary name.\n";
      displayGlossaryEntryCompleteDescription(msg,
                                              glossary.getGlossaryEntry(n));
      tfel::raise(msg.str());
    }
    this->reserveName(n);
    tfel::raise_if(!this->membersNames.insert(n).second,
                   "BehaviourData::registerMemberName: "
                   "a variable named '" +
                       n + "' has already been registred");
  }  // end of BehaviourData::registerMemberName

  void BehaviourData::registerStaticMemberName(const std::string& n) {
    const auto& g = tfel::glossary::Glossary::getGlossary();
    for (auto& e : this->entryNames) {
      tfel::raise_if(e.second == n,
                     "BehaviourData::registerStaticMemberName: "
                     "the name '" +
                         n +
                         "' is already been used "
                         "for an entry name");
    }
    if (g.contains(n)) {
      std::ostringstream msg;
      msg << "BehaviourData::registerStaticMemberName: "
          << "the name '" << n << "' is a registred as a glossary name.\n";
      displayGlossaryEntryCompleteDescription(msg, g.getGlossaryEntry(n));
      tfel::raise(msg.str());
    }
    this->reserveName(n);
    tfel::raise_if(!this->staticMembersNames.insert(n).second,
                   "BehaviourData::registerStaticMemberName: "
                   "a variable named '" +
                       n + "' has already been registred");
  }  // end of BehaviourData::registerStaticMemberName

  const std::set<std::string>& BehaviourData::getRegistredMembersNames() const {
    return this->membersNames;
  }  // end of BehaviourData::getRegistredMemberNames

  const std::set<std::string>& BehaviourData::getRegistredStaticMembersNames()
      const {
    return this->staticMembersNames;
  }  // end of BehaviourData::getRegistredStaticMemberNames

  void BehaviourData::checkVariableName(const std::string& n) const {
    if ((this->materialProperties.contains(n)) ||
        (this->persistentVariables.contains(n)) ||
        (this->integrationVariables.contains(n)) ||
        (this->stateVariables.contains(n)) ||
        (this->auxiliaryStateVariables.contains(n)) ||
        (this->externalStateVariables.contains(n)) ||
        (this->localVariables.contains(n)) || (this->parameters.contains(n)) ||
        (this->staticVariables.contains(n))) {
      return;
    }
    tfel::raise(
        "BehaviourData::checkVariableName: "
        "no variable named '" +
        n + "'");
  }  // end of BehaviourData::checkVariableName

  void BehaviourData::setCode(const std::string& n,
                              const CodeBlock& c,
                              const Mode m,
                              const Position p,
                              const bool b) {
    auto pc = this->cblocks.find(n);
    if (pc == this->cblocks.end()) {
      pc = this->cblocks.insert({n, CodeBlocksAggregator{}}).first;
    } else {
      if (m == CREATE) {
        tfel::raise(
            "BehaviourData::setCode: "
            "a code block named '" +
            n +
            "' already exists.\n"
            "If you wanted to append this new code to the "
            "existing one, you shall use the 'Append' option.\n"
            "You can also replace it with 'Replace' option "
            "(assuming you know what you are doing).\n");
      } else if (m == CREATEORREPLACE) {
        tfel::raise_if(!pc->second.isMutable(),
                       "BehaviourData::setCode: "
                       "the code block named '" +
                           n +
                           "' "
                           "is not modifiable");
        this->cblocks.erase(pc);
        pc = this->cblocks.insert({n, CodeBlocksAggregator{}}).first;
      } else if (m == CREATEBUTDONTREPLACE) {
        return;
      }
    }
    pc->second.set(c, p, b);
  }  // end of BehaviourData::setCode

  const CodeBlock& BehaviourData::getCodeBlock(const std::string& n) const {
    auto p = this->cblocks.find(n);
    tfel::raise_if(p == this->cblocks.end(),
                   "BehaviourData::getCode: "
                   "no code block associated with '" +
                       n + "'");
    return p->second.get();
  }  // end of BehaviourData::getCodeBlock

  std::string BehaviourData::getCode(const std::string& n,
                                     const std::string& cn,
                                     const bool b) const {
    if (!b) {
      return this->getCodeBlock(n).code;
    }
    std::ostringstream out;
    writeStandardPerformanceProfilingBegin(out, cn, n);
    out << this->getCodeBlock(n).code;
    writeStandardPerformanceProfilingEnd(out);
    return out.str();
  }  // end of BehaviourData::getCode

  bool BehaviourData::hasCode(const std::string& n) const {
    return this->cblocks.find(n) != this->cblocks.end();
  }

  void BehaviourData::setParameterDefaultValue(const std::string& n,
                                               const double v) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "BehaviourData::setParameterDefaultValue: " + m);
    };
    throw_if(!this->parameters.contains(n), "no parameter '" + n + "' defined");
    const auto& p = this->parameters.getVariable(n);
    const auto f = SupportedTypes::getTypeFlag(p.type);
    throw_if(f != SupportedTypes::SCALAR,
             "parameter '" + n + "' is not a scalar");
    throw_if(!this->parametersDefaultValues.insert({n, v}).second,
             "default value for parameter '" + n + "' already defined");
  }

  void BehaviourData::setParameterDefaultValue(const std::string& n,
                                               const unsigned short i,
                                               const double v) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "BehaviourData::setParameterDefaultValue: " + m);
    };
    throw_if(!this->parameters.contains(n), "no parameter '" + n + "' defined");
    const auto& p = this->parameters.getVariable(n);
    const auto f = SupportedTypes::getTypeFlag(p.type);
    throw_if(f != SupportedTypes::SCALAR,
             "parameter '" + n + "' is not a floatting point");
    throw_if(p.arraySize == 1,
             "parameter '" + n + "' has not been declared as an array");
    const auto idx = std::to_string(i);
    const auto n2 = n + '[' + idx + ']';
    throw_if(i >= p.arraySize, "index " + idx +
                                   " is greater "
                                   "than parameter '" +
                                   n + "' array size");
    throw_if(!this->parametersDefaultValues.insert({n2, v}).second,
             "default value for parameter '" + n2 + "' already defined");
  }

  void BehaviourData::setParameterDefaultValue(const std::string& n,
                                               const int v) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "BehaviourData::setParameterDefaultValue: " + m);
    };
    throw_if(!this->parameters.contains(n), "no parameter '" + n + "' defined");
    const auto& p = this->parameters.getVariable(n);
    throw_if(p.type != "int", "parameter '" + n + "' is not a floatting point");
    throw_if(!this->iParametersDefaultValues.insert({n, v}).second,
             "default value for parameter '" + n + "' already defined");
  }

  void BehaviourData::setParameterDefaultValue(const std::string& n,
                                               const unsigned short v) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "BehaviourData::setParameterDefaultValue: " + m);
    };
    throw_if(!this->parameters.contains(n), "no parameter '" + n + "' defined");
    const auto& p = this->parameters.getVariable(n);
    throw_if(p.type != "ushort",
             "parameter '" + n + "' is not a floatting point");
    throw_if(!this->uParametersDefaultValues.insert({n, v}).second,
             "default value for parameter '" + n + "' already defined");
  }

  double BehaviourData::getFloattingPointParameterDefaultValue(
      const std::string& n) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "BehaviourData::getFloattingPointParameterDefaultValue: " + m);
    };
    throw_if(!this->parameters.contains(n), "no parameter '" + n + "' defined");
    const auto p = this->parametersDefaultValues.find(n);
    throw_if(p == this->parametersDefaultValues.end(),
             "no default value defined for parameter '" + n + "'");
    return p->second;
  }  // end of BehaviourData::getFloattingPointParameterDefaultValue

  double BehaviourData::getFloattingPointParameterDefaultValue(
      const std::string& n, const unsigned short i) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "BehaviourData::getFloattingPointParameterDefaultValue: " + m);
    };
    throw_if(!this->parameters.contains(n), "no parameter '" + n + "' defined");
    const auto& v = this->parameters.getVariable(n);
    throw_if(v.arraySize == 1u, "parameter '" + n + "' is not an array");
    throw_if(i >= v.arraySize, "invalid index for parameter '" + n + "'");
    const auto n2 = n + '[' + std::to_string(i) + ']';
    const auto p = this->parametersDefaultValues.find(n2);
    throw_if(p == this->parametersDefaultValues.end(),
             "no default value defined for parameter '" + n2 + "'");
    return p->second;
  }  // end of BehaviourData::getFloattingPointParameterDefaultValue

  int BehaviourData::getIntegerParameterDefaultValue(
      const std::string& n) const {
    tfel::raise_if(!this->parameters.contains(n),
                   "BehaviourData::getIntegerParameterDefaultValue: "
                   "no parameter '" +
                       n + "' defined");
    auto p = this->iParametersDefaultValues.find(n);
    tfel::raise_if(p == this->iParametersDefaultValues.end(),
                   "BehaviourData::getIntegerParameterDefaultValue: "
                   "no default value defined for parameter '" +
                       n + "'");
    return p->second;
  }  // end of BehaviourData::getIntegerParameterDefaultValue

  unsigned short BehaviourData::getUnsignedShortParameterDefaultValue(
      const std::string& n) const {
    tfel::raise_if(!this->parameters.contains(n),
                   "BehaviourData::getUnsignedShortParameterDefaultValue: "
                   "no parameter '" +
                       n + "' defined");
    auto p = this->uParametersDefaultValues.find(n);
    tfel::raise_if(p == this->uParametersDefaultValues.end(),
                   "BehaviourData::getUnsignedShortParameterDefaultValue: "
                   "no default value defined for parameter '" +
                       n + "'");
    return p->second;
  }  // end of BehaviourData::getUnsignedShortParameterDefaultValue

  void BehaviourData::setAttribute(const std::string& n,
                                   const BehaviourAttribute& a,
                                   const bool b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourData::setAttribute: " + m);
    };
    auto p = this->attributes.find(n);
    if (p != this->attributes.end()) {
      throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
               "attribute already exists with a different type");
    }
    if (!this->attributes.insert({n, a}).second) {
      throw_if(!b, "attribute '" + n + "' already declared");
    }
  }  // end of BehaviourData::setAttribute

  void BehaviourData::updateAttribute(const std::string& n,
                                      const BehaviourAttribute& a) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourData::updateAttribute: " + m);
    };
    auto p = this->attributes.find(n);
    throw_if(p == this->attributes.end(), "unknown attribute '" + n + "'");
    throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
             "attribute already exists with a different type");
    p->second = a;
  }  // end of BehaviourData::setAttribute

  bool BehaviourData::hasAttribute(const std::string& n) const {
    return this->attributes.count(n) != 0u;
  }  // end of BehaviourData::hasAttribute

  const std::map<std::string, BehaviourAttribute>&
  BehaviourData::getAttributes() const {
    return this->attributes;
  }  // end of BehaviourData::getAttributes

  std::vector<std::string> BehaviourData::getCodeBlockNames() const {
    auto names = std::vector<std::string>{};
    for (const auto& c : this->cblocks) {
      names.push_back(c.first);
    }
    return names;
  }  // end of BehaviourData::getCodeBlockNames

  bool BehaviourData::hasGlossaryName(const std::string& n) const {
    this->checkVariableName(n);
    return this->glossaryNames.find(n) != this->glossaryNames.end();
  }  // end of BehaviourData::hasGlossaryName

  bool BehaviourData::hasEntryName(const std::string& n) const {
    this->checkVariableName(n);
    return this->entryNames.find(n) != this->entryNames.end();
  }  // end of BehaviourData::hasEntryName

  std::string BehaviourData::getExternalName(const std::string& n) const {
    this->checkVariableName(n);
    auto p = this->glossaryNames.find(n);
    if (p != this->glossaryNames.end()) {
      return p->second;
    }
    p = this->entryNames.find(n);
    if (p != this->entryNames.end()) {
      return p->second;
    }
    return n;
  }  // end of BehaviourData::getExternalName

  std::vector<std::string> BehaviourData::getExternalNames(
      const VarContainer& v) const {
    return v.getExternalNames();
  }  // end of BehaviourData::getExternalNames

  void BehaviourData::getExternalNames(std::vector<std::string>& names,
                                       const VarContainer& v) const {
    v.getExternalNames(names);
  }  // end of BehaviourData::getExternalNames

  void BehaviourData::appendExternalNames(std::vector<std::string>& names,
                                          const VarContainer& v) const {
    v.appendExternalNames(names);
  }  // end of BehaviourData::appendExternalNames

  void BehaviourData::setGlossaryName(const std::string& n,
                                      const std::string& g) {
    using tfel::glossary::Glossary;
    const auto& glossary = Glossary::getGlossary();
    tfel::raise_if(!glossary.contains(g),
                   "BehaviourData::setGlossaryName: "
                   "'" +
                       g + "' is not a glossary name");
    bool treated = false;
    auto set_glossary_name = [&n, &g,
                              &treated](VariableDescriptionContainer& c) {
      if (c.contains(n)) {
        c.getVariable(n).setGlossaryName(g);
        treated = true;
      }
    };
    this->checkVariableName(n);
    BehaviourDataAddToGlossaryOrEntryNames(
        this->glossaryNames, this->glossaryNames, this->entryNames,
        this->reservedNames, n, glossary.getGlossaryEntry(g).getKey());
    set_glossary_name(this->materialProperties);
    set_glossary_name(this->localVariables);
    set_glossary_name(this->stateVariables);
    set_glossary_name(this->auxiliaryStateVariables);
    set_glossary_name(this->integrationVariables);
    set_glossary_name(this->persistentVariables);
    set_glossary_name(this->externalStateVariables);
    set_glossary_name(this->parameters);
    tfel::raise_if(!treated,
                   "BehaviourData::setGlossaryName: "
                   "no variable named '" +
                       n + "'");
  }  // end of BehaviourData::addGlossaryName

  bool BehaviourData::isGlossaryNameUsed(const std::string& n) const {
    using namespace tfel::glossary;
    const auto& g = Glossary::getGlossary();
    tfel::raise_if(!g.contains(n),
                   "BehaviourData::isGlossaryNameUsed: "
                   "'" +
                       n + "' is not a glossary name");
    for (const auto& gn : this->glossaryNames) {
      if (gn.second == n) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourData::isGlossaryName

  void BehaviourData::setEntryName(const std::string& n, const std::string& e) {
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    bool treated = false;
    auto set_entry_name = [&n, &e, &treated](VariableDescriptionContainer& c) {
      if (c.contains(n)) {
        c.getVariable(n).setEntryName(e);
        treated = true;
      }
    };
    if (glossary.contains(e)) {
      std::ostringstream msg;
      msg << "BehaviourData::setEntryName: "
          << "'" << e << "' is a glossary name. " << std::endl
          << "Please use 'setGlossaryName' method instead or choose another "
             "entry name.";
      displayGlossaryEntryCompleteDescription(msg,
                                              glossary.getGlossaryEntry(e));
      tfel::raise(msg.str());
    }
    tfel::raise_if(!tfel::utilities::CxxTokenizer::isValidIdentifier(e, false),
                   "BehaviourData::setEntryName: "
                   "'" +
                       e + "' is a not a valid entry name");
    this->checkVariableName(n);
    BehaviourDataAddToGlossaryOrEntryNames(
        this->entryNames, this->glossaryNames, this->entryNames,
        this->reservedNames, n, e);
    set_entry_name(this->materialProperties);
    set_entry_name(this->localVariables);
    set_entry_name(this->stateVariables);
    set_entry_name(this->auxiliaryStateVariables);
    set_entry_name(this->integrationVariables);
    set_entry_name(this->persistentVariables);
    set_entry_name(this->externalStateVariables);
    set_entry_name(this->parameters);
    tfel::raise_if(!treated,
                   "BehaviourData::setEntryName: "
                   "no variable named '" +
                       n + "'");
  }  // end of BehaviourData::addEntryName

  bool BehaviourData::isUsedAsEntryName(const std::string& n) const {
    for (const auto& en : this->entryNames) {
      if (en.second == n) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourData::isEntryName

  std::string BehaviourData::getVariableNameFromGlossaryNameOrEntryName(
      const std::string& n) const {
    for (const auto& e : this->glossaryNames) {
      if (e.second == n) {
        return e.first;
      }
    }
    for (const auto& e : this->entryNames) {
      if (e.second == n) {
        return e.first;
      }
    }
    tfel::raise(
        "BehaviourData::getVariableNameFromGlossaryNameOrEntryName: "
        "no variable with glossary or entry name '" +
        n + "'");
  }  // end of BehaviourData::getVariableNameFromGlossaryNameOrEntryName

  const VariableDescriptionContainer& BehaviourData::getParameters() const {
    return this->parameters;
  }  // end of BehaviourData::getParameters

  void BehaviourData::appendToMembers(const std::string& c) {
    this->members += c;
    if (!this->members.empty()) {
      if (*(this->members.rbegin()) != '\n') {
        this->members += '\n';
      }
    }
  }  // end of BehaviourData::appendToMembers

  std::string BehaviourData::getMembers() const {
    return this->members;
  }  // end of BehaviourData::getMembers

  void BehaviourData::appendToPrivateCode(const std::string& c) {
    this->privateCode += c;
    if (!this->privateCode.empty()) {
      if (*(this->privateCode.rbegin()) != '\n') {
        this->privateCode += '\n';
      }
    }
  }  // end of BehaviourData::appendToPrivateCode

  std::string BehaviourData::getPrivateCode() const {
    return this->privateCode;
  }  // end of BehaviourData::getPrivateCode

  void BehaviourData::addStressFreeExpansion(
      const StressFreeExpansionDescription& sfed) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourData::addStressFreeExpansion: " + m);
    };
    auto check_esv = [this, &throw_if](const SFED_ESV& h) {
      throw_if(!this->isExternalStateVariableName(h.vname),
               "'" + h.vname + "' is not an external state variable name");
      const auto& ev = this->getExternalStateVariables().getVariable(h.vname);
      throw_if(ev.arraySize != 1u,
               "invalid arrary size of variable '" + h.vname + "'");
    };
    auto check_esv2 = [this, &throw_if](const SFED_ESV& h) {
      throw_if(!this->isExternalStateVariableName(h.vname),
               "'" + h.vname + "' is not an external state variable name");
      const auto& ev = this->getExternalStateVariables().getVariable(h.vname);
      throw_if(ev.arraySize != 3u,
               "invalid arrary size of variable '" + h.vname + "'");
    };
    auto treat = [&check_esv, &throw_if,
                  this](const StressFreeExpansionHandler& h) {
      if (h.is<SFED_ESV>()) {
        check_esv(h.get<SFED_ESV>());
      } else if (h.is<std::shared_ptr<ModelDescription>>()) {
        const auto& md = *(h.get<std::shared_ptr<ModelDescription>>());
        throw_if(!md.constantMaterialProperties.empty(),
                 "constant material properties are not supported yet");
        throw_if(md.functions.size() != 1u,
                 "invalid number of functions in model '" + md.className + "'");
        throw_if(md.functions[0].name.empty(), "invalid function name");
        throw_if(md.functions[0].modifiedVariables.size() != 1u,
                 "invalid number of modified variables in function '" +
                     md.functions[0].name + "' in model '" + md.className +
                     "'");
        // checking output
        throw_if(md.outputs.size() != 1u,
                 "external model must have one and only output");
        // checking inputs
        for (const auto& i : md.inputs) {
          throw_if(i.arraySize != 1u,
                   "invalid array size for model variable '" + i.name + "'");
          // external name
          const auto& n = i.getExternalName();
          // check that an external state variable with the same
          // external state variable exists
          const auto& evnames =
              this->getExternalNames(this->getExternalStateVariables());
          if (n == tfel::glossary::Glossary::Temperature) {
            continue;
          }
          if ((std::find(std::begin(evnames), std::end(evnames), n) ==
               std::end(evnames))) {
            // adding a new state variable with given external name
            auto v = i;
            v.name = md.className + "_" + v.name;
            this->addExternalStateVariable(v, UNREGISTRED);
          }
        }
        // declaring the output has an auxiliary state variable
        auto o = md.outputs[0];
        o.name = md.className + "_" + o.name;
        this->addAuxiliaryStateVariable(o, UNREGISTRED);
      } else if (h.is<NullExpansion>()) {
        // do nothing
      } else {
        throw_if(true, "unsupported stress free expansion handler");
      }
    };
    if (sfed.is<VolumeSwellingStressFreeExpansion>()) {
      const auto& s = sfed.get<VolumeSwellingStressFreeExpansion>();
      throw_if(s.sfe.is<NullExpansion>(), "null swelling is not allowed");
      treat(s.sfe);
    } else if (sfed.is<IsotropicStressFreeExpansion>()) {
      const auto& s = sfed.get<IsotropicStressFreeExpansion>();
      throw_if(s.sfe.is<NullExpansion>(), "null swelling is not allowed");
      treat(s.sfe);
    } else if (sfed.is<AxialGrowth>()) {
      const auto& s = sfed.get<AxialGrowth>();
      throw_if(s.sfe.is<NullExpansion>(), "null swelling is not allowed");
      treat(s.sfe);
    } else if (sfed.is<Relocation>()) {
      const auto& s = sfed.get<Relocation>();
      throw_if(s.sfe.is<NullExpansion>(), "null swelling is not allowed");
      treat(s.sfe);
    } else if (sfed.is<OrthotropicStressFreeExpansion>()) {
      const auto& s = sfed.get<OrthotropicStressFreeExpansion>();
      throw_if(s.sfe0.is<NullExpansion>() && s.sfe1.is<NullExpansion>() &&
                   s.sfe2.is<NullExpansion>(),
               "null swelling is not allowed");
      treat(s.sfe0);
      treat(s.sfe1);
      treat(s.sfe2);
    } else if (sfed.is<OrthotropicStressFreeExpansionII>()) {
      const auto& s = sfed.get<OrthotropicStressFreeExpansionII>();
      check_esv2(s.esv);
    } else {
      throw_if(true, "internal error, unsupported stress free expansion type");
    }
    this->sfeds.push_back(sfed);
  }  // end of BehaviourData::addStressFreeExpansion

  const std::vector<BehaviourData::StressFreeExpansionDescription>&
  BehaviourData::getStressFreeExpansionDescriptions() const {
    return this->sfeds;
  }  // end of BehaviourData::getStressFreeExpansionDescriptions

  bool BehaviourData::isStressFreeExansionAnisotropic() const {
    for (const auto& sfed : this->sfeds) {
      if ((sfed.is<BehaviourData::AxialGrowth>()) ||
          (sfed.is<BehaviourData::Relocation>()) ||
          (sfed.is<BehaviourData::OrthotropicStressFreeExpansion>()) ||
          (sfed.is<BehaviourData::OrthotropicStressFreeExpansionII>())) {
        return true;
      } else {
        tfel::raise_if(
            (!sfed.is<BehaviourData::IsotropicStressFreeExpansion>()) &&
                (!sfed.is<BehaviourData::VolumeSwellingStressFreeExpansion>()),
            "BehaviourData::isStressFreeExansionAnisotropic: "
            "internal error, unsupported stress "
            "free expansion type");
      }
    }
    return false;
  }  // end of BehaviourData::isStressFreeExansionAnisotropic

  void BehaviourData::getSymbols(
      std::map<std::string, std::string>& symbols) const {
    mfront::getSymbols(symbols, this->materialProperties);
    mfront::getSymbols(symbols, this->persistentVariables);
    mfront::getSymbols(symbols, this->integrationVariables);
    mfront::getSymbols(symbols, this->stateVariables);
    mfront::getSymbols(symbols, this->auxiliaryStateVariables);
    mfront::getSymbols(symbols, this->externalStateVariables);
    mfront::getSymbols(symbols, this->localVariables);
    mfront::getSymbols(symbols, this->parameters);
  }  // end of BehaviourData::getSymbols

  BehaviourData::~BehaviourData() = default;

}  // end of namespace mfront
