/*!
 * \file  mfront/src/BehaviourDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/ModelDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront {

  std::vector<std::string>
  BehaviourDescription::AnalyticMaterialProperty::getVariablesNames() const {
    return tfel::math::Evaluator(this->f).getVariablesNames();
  }  // end of AnalyticMaterialProperty::getVariablesNames

  template <typename Arg1>
  void BehaviourDescription::callBehaviourData(
      const Hypothesis h,
      void (BehaviourData::*m)(const Arg1&),
      const Arg1& a,
      const bool b) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      (this->d.*m)(a);
      if (b) {
        for (auto md : this->sd) {
          (md.second.get()->*m)(a);
        }
      }
    } else {
      (this->getBehaviourData2(h).*m)(a);
    }
  }  // end of BehaviourDescription::callBehaviourData

  template <typename Arg1>
  void BehaviourDescription::callBehaviourData(
      const Hypothesis h,
      void (BehaviourData::*m)(const Arg1),
      const Arg1 a,
      const bool b) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      (this->d.*m)(a);
      if (b) {
        for (const auto& ptr : this->sd) {
          auto& bdata = *(ptr.second);
          (bdata.*m)(a);
        }
      }
    } else {
      (this->getBehaviourData2(h).*m)(a);
    }
  }  // end of BehaviourDescription::callBehaviourData

  template <typename Arg1, typename Arg2>
  void BehaviourDescription::callBehaviourData(
      const Hypothesis h,
      void (BehaviourData::*m)(const Arg1&, const Arg2),
      const Arg1& a1,
      const Arg2 a2,
      const bool b) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      (this->d.*m)(a1, a2);
      if (b) {
        for (const auto& ptr : this->sd) {
          auto& bdata = *(ptr.second);
          (bdata.*m)(a1, a2);
        }
      }
    } else {
      (this->getBehaviourData2(h).*m)(a1, a2);
    }
  }  // end of BehaviourDescription::callBehaviourData

  template <typename Arg1, typename Arg2>
  void BehaviourDescription::callBehaviourData(
      const Hypothesis h,
      void (BehaviourData::*m)(const Arg1&, const Arg2&),
      const Arg1& a1,
      const Arg2& a2,
      const bool b) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      (this->d.*m)(a1, a2);
      if (b) {
        for (const auto& ptr : this->sd) {
          auto& bdata = *(ptr.second);
          (bdata.*m)(a1, a2);
        }
      }
    } else {
      (this->getBehaviourData2(h).*m)(a1, a2);
    }
  }  // end of BehaviourDescription::callBehaviourData

  template <typename Arg1, typename Arg2, typename Arg3>
  void BehaviourDescription::callBehaviourData(
      const Hypothesis h,
      void (BehaviourData::*m)(const Arg1&, const Arg2&, const Arg3&),
      const Arg1& a1,
      const Arg2& a2,
      const Arg3& a3,
      const bool b) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      (this->d.*m)(a1, a2, a3);
      if (b) {
        for (const auto& ptr : this->sd) {
          auto& bdata = *(ptr.second);
          (bdata.*m)(a1, a2, a3);
        }
      }
    } else {
      (this->getBehaviourData2(h).*m)(a1, a2, a3);
    }
  }  // end of BehaviourDescription::callBehaviourData

  template <typename Res, typename Arg1>
  Res BehaviourDescription::getData(const Hypothesis h,
                                    Res (BehaviourData::*m)(const Arg1&) const,
                                    const Arg1& a) const {
    return (this->getBehaviourData(h).*m)(a);
  }  // end of BehaviourDescription::getData

  static void declareParameter(BehaviourDescription& bd,
                               BehaviourDescription::MaterialProperty& mp,
                               const tfel::glossary::GlossaryEntry& e,
                               const std::string& n) {
    const auto h = tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
      auto& cmp = mp.get<BehaviourDescription::ConstantMaterialProperty>();
      cmp.name = n;
      // declare associated parameter
      VariableDescription m("real", n, 1u, 0u);
      bd.addParameter(h, m);
      bd.setParameterDefaultValue(h, n, cmp.value);
      bd.setGlossaryName(h, n, e.getKey());
    }
  }  // end of declareParameter

  static void checkElasticMaterialProperty(
      BehaviourDescription& bd,
      BehaviourDescription::MaterialProperty& emp,
      const tfel::glossary::GlossaryEntry& e,
      const std::string& n2) {
    if (emp.is<BehaviourDescription::ExternalMFrontMaterialProperty>()) {
      const auto& mpd = *(
          emp.get<BehaviourDescription::ExternalMFrontMaterialProperty>().mpd);
      const auto& ename = mpd.output.getExternalName();
      if (ename != e) {
        getLogStream()
            << "checkElasticMaterialProperty: inconsistent external name for "
            << "material property '" + e.getKey() +
                   "': external name of mfront file "
            << "output  is '" << ename << "'\n";
      }
    }
    declareParameter(bd, emp, e, n2);
  }

  static void checkThermalExpansionCoefficientArgument(
      BehaviourDescription& bd,
      BehaviourDescription::MaterialProperty& a,
      const tfel::glossary::GlossaryEntry& e,
      const std::string& n) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "checkThermalExpansionCoefficientArgument: " + m);
    };
    declareParameter(bd, a, e, n);
    if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
      return;
    }
    if (a.is<BehaviourDescription::ExternalMFrontMaterialProperty>()) {
      const auto& mpd =
          *(a.get<BehaviourDescription::ExternalMFrontMaterialProperty>().mpd);
      for (const auto& i : bd.getMaterialPropertyInputs(mpd, false)) {
        const auto c = i.category;
        throw_if(
            (c != BehaviourDescription::MaterialPropertyInput::TEMPERATURE) &&
                (c != BehaviourDescription::MaterialPropertyInput::
                          MATERIALPROPERTY) &&
                (c != BehaviourDescription::MaterialPropertyInput::PARAMETER) &&
                (c !=
                 BehaviourDescription::MaterialPropertyInput::STATICVARIABLE),
            "invalid input '" + i.ename + "' (" + i.name +
                ") for thermal expansion");
      }
    } else if (a.is<BehaviourDescription::AnalyticMaterialProperty>()) {
      const auto& vn = a.get<BehaviourDescription::AnalyticMaterialProperty>()
                           .getVariablesNames();
      for (const auto& i : bd.getMaterialPropertyInputs(vn, false)) {
        const auto c = i.category;
        throw_if(
            (c != BehaviourDescription::MaterialPropertyInput::TEMPERATURE) &&
                (c != BehaviourDescription::MaterialPropertyInput::
                          MATERIALPROPERTY) &&
                (c != BehaviourDescription::MaterialPropertyInput::PARAMETER) &&
                (c !=
                 BehaviourDescription::MaterialPropertyInput::STATICVARIABLE),
            "invalid input '" + i.ename + "' (" + i.name +
                ") for thermal expansion");
      }
    } else {
      throw_if(true, "unsupported material property type");
    }
  }  // end of checkThermalExpansionCoefficientArgument

  const char* const BehaviourDescription::requiresStiffnessTensor =
      "requiresStiffnessTensor";

  const char* const BehaviourDescription::computesStiffnessTensor =
      "computesStiffnessTensor";

  const char* const BehaviourDescription::requiresUnAlteredStiffnessTensor =
      "requiresUnAlteredStiffnessTensor";

  const char* const
      BehaviourDescription::requiresThermalExpansionCoefficientTensor =
          "requiresThermalExpansionCoefficientTensor";

  BehaviourDescription::BehaviourDescription() = default;

  BehaviourDescription::BehaviourDescription(const BehaviourDescription&) =
      default;

  bool BehaviourDescription::allowsNewUserDefinedVariables() const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    return this->getAttribute(h, BehaviourData::allowsNewUserDefinedVariables,
                              true);
  }  // end of BehaviourDescription::allowNewsUserDefinedVariables

  void BehaviourDescription::disallowNewUserDefinedVariables() {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->setAttribute(uh, BehaviourData::allowsNewUserDefinedVariables, false);
  }  // end of BehaviourDescription::disallowNewUserDefinedVariables

  void BehaviourDescription::throwUndefinedAttribute(const std::string& n) {
    tfel::raise(
        "BehaviourDescription::getAttribute: "
        "no attribute named '" +
        n + "'");
  }  // end of BehaviourDescription::throwUndefinedAttribute

  const BehaviourData& BehaviourDescription::getBehaviourData(
      const Hypothesis& h) const {
    // check that the given hypothesis is supported
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return this->d;
    }
    this->checkModellingHypothesis(h);
    this->requestedHypotheses.insert(h);
    // check if a specialised version of the behaviour
    // description has been defined
    const auto p = this->sd.find(h);
    if (p != this->sd.end()) {
      return *(p->second);
    }
    // return the default...
    return this->d;
  }  // end of BehaviourDescription::getBehaviourData

  BehaviourData& BehaviourDescription::getBehaviourData2(
      const ModellingHypothesis::Hypothesis& h) {
    // check that the given hypothesis is supported
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return this->d;
    }
    if (this->areModellingHypothesesDefined()) {
      this->checkModellingHypothesis(h);
    }
    this->requestedHypotheses.insert(h);
    auto p = this->sd.find(h);
    if (p == this->sd.end()) {
      // copy of the default description
      p = this->sd.insert({h, std::make_shared<BehaviourData>(this->d)}).first;
    }
    return *(p->second);
  }  // end of BehaviourDescription::getBehaviourData2

  void BehaviourDescription::setBehaviourName(const std::string& m) {
    tfel::raise_if(!this->behaviour.empty(),
                   "BehaviourDescription::setBehaviourName: "
                   "behaviour name already defined");
    this->behaviour = m;
    this->updateClassName();
  }  // end of BehaviourDescription::setBehaviourName

  const std::string& BehaviourDescription::getBehaviourName() const {
    tfel::raise_if(this->behaviour.empty(),
                   "BehaviourDescription::getBehaviourName: "
                   "behaviour name not defined");
    return this->behaviour;
  }  // end of BehaviourDescription::getBehaviourName

  void BehaviourDescription::setDSLName(const std::string& m) {
    tfel::raise_if(!this->dsl.empty(),
                   "BehaviourDescription::setDSLName: "
                   "dsl name already defined");
    this->dsl = m;
    this->updateClassName();
  }  // end of BehaviourDescription::setDSLName

  const std::string& BehaviourDescription::getDSLName() const {
    tfel::raise_if(this->dsl.empty(),
                   "BehaviourDescription::getDSLName: "
                   "dsl name not defined");
    return this->dsl;
  }  // end of BehaviourDescription::getDSLName

  BehaviourDescription::MaterialPropertyInput::Category
  BehaviourDescription::getMaterialPropertyInputCategory(
      const Hypothesis h, const std::string& v) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c, "BehaviourDescription::getMaterialPropertyInputCategory: " + m);
    };
    if (this->isStateVariableName(h, v)) {
      return MaterialPropertyInput::STATEVARIABLE;
    } else if (this->isExternalStateVariableName(h, v)) {
      return MaterialPropertyInput::EXTERNALSTATEVARIABLE;
    } else if (this->isAuxiliaryStateVariableName(h, v)) {
      const auto& bd = this->getBehaviourData(h);
      const auto& av = bd.getAuxiliaryStateVariableDescription(v);
      throw_if(
          !av.getAttribute<bool>("ComputedByExternalModel", false),
          "only auxiliary state variable computed by a model are allowed here");
      return MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL;
    } else if (this->isMaterialPropertyName(h, v)) {
      return MaterialPropertyInput::MATERIALPROPERTY;
    } else if (this->isParameterName(h, v)) {
      return MaterialPropertyInput::PARAMETER;
    } else if (this->isStaticVariableName(h, v)) {
      return MaterialPropertyInput::STATICVARIABLE;
    }
    throw_if(true, "unsupported variable: variable '" + v +
                       "' is "
                       "neither an external state variable, a material "
                       "property nor a parameter nor an auxiliary "
                       "state variable evaluated by an external model, "
                       "nor a static variable");
  }  // end of BehaviourDescription::getMaterialPropertyInputCategory

  std::vector<BehaviourDescription::MaterialPropertyInput>
  BehaviourDescription::getMaterialPropertyInputs(
      const std::vector<std::string>& i, const bool b) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "BehaviourDescription::getMaterialPropertyInputs: " + m);
    };
    auto inputs = std::vector<MaterialPropertyInput>{};
    const auto hs = [this, b,
                     throw_if]() -> std::set<BehaviourDescription::Hypothesis> {
      if (this->hypotheses.empty()) {
        // modelling hypotheses are not set yet
        throw_if(b, "modelling hypothesis must be defined");
        return {ModellingHypothesis::UNDEFINEDHYPOTHESIS};
      }
      return this->getDistinctModellingHypotheses();
    }();
    for (const auto& v : i) {
      if (v == "T") {
        inputs.push_back({"T", tfel::glossary::Glossary::Temperature,
                          MaterialPropertyInput::TEMPERATURE});
      } else {
        const auto rh = *(hs.begin());
        const auto t = this->getMaterialPropertyInputCategory(rh, v);
        if (t == MaterialPropertyInput::STATICVARIABLE) {
          for (const auto h : hs) {
            throw_if(this->getMaterialPropertyInputCategory(h, v) != t,
                     "the variable '" + v +
                         "' belongs to two different "
                         "categories in two distinct modelling hypotheses. "
                         "This is not supported.");
          }
          inputs.push_back({v, v, t});
        } else {
          const auto vd = this->getBehaviourData(rh).getVariableDescription(v);
          const auto en = vd.getExternalName();
          for (const auto h : hs) {
            const auto vd2 =
                this->getBehaviourData(h).getVariableDescription(v);
            throw_if(vd2.getExternalName() != en,
                     "the variable '" + v +
                         "' has two different "
                         "external names in two distinct modelling hypotheses."
                         "This is not supported.");
            throw_if(this->getMaterialPropertyInputCategory(h, v) != t,
                     "the variable '" + v +
                         "' belongs to two different "
                         "categories in two distinct modelling hypotheses. "
                         "This is not supported.");
          }
          inputs.push_back({v, en, t});
        }
      }
    }
    return inputs;
  }  // end of BehaviourDescription::getMaterialPropertyInputs

  std::vector<BehaviourDescription::MaterialPropertyInput>
  BehaviourDescription::getMaterialPropertyInputs(
      const MaterialPropertyDescription& mpd, const bool b) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "BehaviourDescription::getMaterialPropertyInputs: " + m);
    };
    auto inputs = std::vector<MaterialPropertyInput>{};
    const auto hs = [this, b,
                     throw_if]() -> std::set<BehaviourDescription::Hypothesis> {
      if (this->hypotheses.empty()) {
        // modelling hypotheses are not set yet
        throw_if(b, "modelling hypothesis must be defined");
        return {ModellingHypothesis::UNDEFINEDHYPOTHESIS};
      }
      return this->getDistinctModellingHypotheses();
    }();
    for (const auto& v : mpd.inputs) {
      if ((getPedanticMode()) &&
          (!(v.hasGlossaryName()) && (!v.hasEntryName()))) {
        getLogStream() << "BehaviourDescription::getMaterialPropertyInputs: "
                          "no glossary nor entry name declared for variable "
                          "'" +
                              v.name + "' used by the material property '" +
                              mpd.law + "'\n";
      }
      const auto& vn = v.getExternalName();
      if (vn == tfel::glossary::Glossary::Temperature) {
        inputs.push_back({"T", tfel::glossary::Glossary::Temperature,
                          MaterialPropertyInput::TEMPERATURE});
      } else {
        const auto n =
            this->getVariableDescriptionByExternalName(*(hs.begin()), vn).name;
        const auto t = this->getMaterialPropertyInputCategory(*(hs.begin()), n);
        for (const auto h : hs) {
          throw_if(this->getVariableDescriptionByExternalName(h, vn).name != n,
                   "the external name '" + vn +
                       "' is associated with "
                       "two differents variables in two distinct "
                       "modelling hypotheses. This is not supported.");
          throw_if(this->getMaterialPropertyInputCategory(h, n) != t,
                   "the external name '" + vn +
                       "' has two different "
                       "types in two distinct modelling hypotheses. "
                       "This is not supported.");
        }
        inputs.push_back({n, vn, t});
      }
    }
    return inputs;
  }  // end of BehaviourDescription::getMaterialPropertyInputs

  void BehaviourDescription::setIntegrationScheme(
      const BehaviourDescription::IntegrationScheme s) {
    tfel::raise_if(this->ischeme != UNDEFINEDINTEGRATIONSCHEME,
                   "BehaviourDescription::setIntegrationScheme: "
                   "integration scheme already defined");
    this->ischeme = s;
  }  // end of BehaviourDescription::setIntegrationScheme

  BehaviourDescription::IntegrationScheme
  BehaviourDescription::getIntegrationScheme() const {
    tfel::raise_if(this->ischeme == UNDEFINEDINTEGRATIONSCHEME,
                   "BehaviourDescription::getIntegrationScheme: "
                   "the integration scheme is undefined");
    return this->ischeme;
  }  // end of BehaviourDescription::getIntegrationScheme

  void BehaviourDescription::setLibrary(const std::string& l) {
    tfel::raise_if(!this->library.empty(),
                   "BehaviourDescription::setLibrary: "
                   "library alreay defined");
    this->library = l;
  }  // end of BehaviourDescription::setLibrary

  const std::string& BehaviourDescription::getLibrary() const {
    return this->library;
  }  // end of BehaviourDescription::getLibrary

  void BehaviourDescription::setMaterialName(const std::string& m) {
    tfel::raise_if(!this->material.empty(),
                   "BehaviourDescription::setMaterialName: "
                   "material name alreay defined");
    this->material = m;
    this->updateClassName();
  }  // end of BehaviourDescription::setMaterialName

  const std::string& BehaviourDescription::getMaterialName() const {
    return this->material;
  }  // end of BehaviourDescription::getMaterialName

  void BehaviourDescription::setClassName(const std::string& n) {
    tfel::raise_if(!this->className.empty(),
                   "BehaviourDescription::setClassName: "
                   "class name alreay defined");
    this->className = n;
  }  // end of BehaviourDescription::setClassName

  const std::string& BehaviourDescription::getClassName() const {
    tfel::raise_if(this->className.empty(),
                   "BehaviourDescription::getClassName: "
                   "class name not defined");
    return this->className;
  }  // end of BehaviourDescription::getClassName

  void BehaviourDescription::appendToIncludes(const std::string& c) {
    this->includes += c;
    if (!this->includes.empty()) {
      if (*(this->includes.rbegin()) != '\n') {
        this->includes += '\n';
      }
    }
  }  // end of BehaviourDescription::appendToIncludes

  const std::string& BehaviourDescription::getIncludes() const {
    return this->includes;
  }  // end of BehaviourDescription::getIncludes

  void BehaviourDescription::appendToMembers(const Hypothesis h,
                                             const std::string& c,
                                             const bool b) {
    this->callBehaviourData(h, &BehaviourData::appendToMembers, c, b);
  }  // end of BehaviourDescription::appendToMembers

  std::string BehaviourDescription::getMembers(const Hypothesis h) const {
    return this->getBehaviourData(h).getMembers();
  }  // end of BehaviourDescription::getMembers

  void BehaviourDescription::appendToPrivateCode(const Hypothesis h,
                                                 const std::string& c,
                                                 const bool b) {
    this->callBehaviourData(h, &BehaviourData::appendToPrivateCode, c, b);
  }  // end of BehaviourDescription::appendToPrivateCode

  std::string BehaviourDescription::getPrivateCode(const Hypothesis h) const {
    return this->getBehaviourData(h).getPrivateCode();
  }  // end of BehaviourDescription::getPrivateCode

  void BehaviourDescription::appendToSources(const std::string& c) {
    this->sources += c;
    if (!this->sources.empty()) {
      if (*(this->sources.rbegin()) != '\n') {
        this->sources += '\n';
      }
    }
  }  // end of BehaviourDescription::appendToSources

  const std::string& BehaviourDescription::getSources() const {
    return this->sources;
  }  // end of BehaviourDescription::getSources

  BehaviourDescription::BehaviourType BehaviourDescription::getBehaviourType()
      const {
    return this->type;
  }  // end of BehaviourDescription::getBehaviourType

  std::string BehaviourDescription::getBehaviourTypeFlag() const {
    std::string btype;
    if (this->getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      btype = "MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR";
    } else if (this->getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      btype = "MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR";
    } else if (this->getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      btype = "MechanicalBehaviourBase::COHESIVEZONEMODEL";
    } else {
      tfel::raise(
          "BehaviourDescription::getBehaviourTypeFlag: "
          "unsupported behaviour type");
    }
    return btype;
  }  // end of BehaviourDescription::getBehaviourTypeFlag

  bool BehaviourDescription::areElasticMaterialPropertiesDefined() const {
    return !this->elasticMaterialProperties.empty();
  }  // end of BehaviourDescription::areElasticMaterialPropertiesDefined

  bool BehaviourDescription::isMaterialPropertyConstantDuringTheTimeStep(
      const MaterialProperty& mp) const {
    if (mp.is<ExternalMFrontMaterialProperty>()) {
      const auto& cmp = mp.get<ExternalMFrontMaterialProperty>();
      for (const auto& i : this->getMaterialPropertyInputs(*(cmp.mpd))) {
        if (!((i.category ==
               BehaviourDescription::MaterialPropertyInput::MATERIALPROPERTY) ||
              (i.category ==
               BehaviourDescription::MaterialPropertyInput::PARAMETER))) {
          return false;
        }
      }
      return true;
    } else if (mp.is<AnalyticMaterialProperty>()) {
      const auto& amp = mp.get<AnalyticMaterialProperty>();
      for (const auto& i :
           this->getMaterialPropertyInputs(amp.getVariablesNames())) {
        if (!((i.category ==
               BehaviourDescription::MaterialPropertyInput::MATERIALPROPERTY) ||
              (i.category ==
               BehaviourDescription::MaterialPropertyInput::PARAMETER))) {
          return false;
        }
      }
      return true;
    }
    return true;
  }  // end of BehaviourDescription::isMaterialPropertyConstantDuringTheTimeStep

  bool
  BehaviourDescription::areElasticMaterialPropertiesConstantDuringTheTimeStep()
      const {
    tfel::raise_if(!this->areElasticMaterialPropertiesDefined(),
                   "BehaviourDescription::"
                   "areElasticMaterialPropertiesConstantDuringTheTimeStep: "
                   "no elastic material property defined");
    return this->areMaterialPropertiesConstantDuringTheTimeStep(
        this->elasticMaterialProperties);
  }  // end of
     // BehaviourDescription::areElasticMaterialPropertiesConstantDuringTheTimeStep

  bool BehaviourDescription::areMaterialPropertiesConstantDuringTheTimeStep(
      const std::vector<MaterialProperty>& mps) const {
    for (const auto& mp : mps) {
      if (!this->isMaterialPropertyConstantDuringTheTimeStep(mp)) {
        return false;
      }
    }
    return true;
  }  // end of
     // BehaviourDescription::areMaterialPropertiesConstantDuringTheTimeStep

  bool BehaviourDescription::isMaterialPropertyDependantOnStateVariables(
      const MaterialProperty& mp) const {
    if (mp.is<ExternalMFrontMaterialProperty>()) {
      const auto& cmp = mp.get<ExternalMFrontMaterialProperty>();
      for (const auto& i : this->getMaterialPropertyInputs(*(cmp.mpd))) {
        if (i.category ==
            BehaviourDescription::MaterialPropertyInput::STATEVARIABLE) {
          return true;
        }
      }
      return false;
    }
    if (mp.is<AnalyticMaterialProperty>()) {
      const auto& amp = mp.get<AnalyticMaterialProperty>();
      for (const auto& i :
           this->getMaterialPropertyInputs(amp.getVariablesNames())) {
        if (i.category ==
            BehaviourDescription::MaterialPropertyInput::STATEVARIABLE) {
          return true;
        }
      }
      return false;
    }
    tfel::raise_if(
        !mp.is<ConstantMaterialProperty>(),
        "BehaviourDescription::isMaterialPropertyDependantOnStateVariables: "
        "unsupported material property");
    return false;
  }  // end of BehaviourDescription::isMaterialPropertyDependantOnStateVariables

  bool
  BehaviourDescription::areElasticMaterialPropertiesDependantOnStateVariables()
      const {
    tfel::raise_if(!this->areElasticMaterialPropertiesDefined(),
                   "BehaviourDescription::"
                   "areElasticMaterialPropertiesDependantOnStateVariables: "
                   "no elastic material property defined");
    return this->areMaterialPropertiesDependantOnStateVariables(
        this->elasticMaterialProperties);
  }  // end of
     // BehaviourDescription::areElasticMaterialPropertiesDependantOnStateVariables

  bool BehaviourDescription::areMaterialPropertiesDependantOnStateVariables(
      const std::vector<MaterialProperty>& mps) const {
    for (const auto& mp : mps) {
      if (this->isMaterialPropertyDependantOnStateVariables(mp)) {
        return true;
      }
    }
    return false;
  }  // end of
     // BehaviourDescription::areMaterialPropertiesDependantOnStateVariables

  const std::vector<BehaviourDescription::MaterialProperty>&
  BehaviourDescription::getElasticMaterialProperties() const {
    tfel::raise_if(!this->areElasticMaterialPropertiesDefined(),
                   "BehaviourDescription::getElasticMaterialProperties: "
                   "no elastic material property defined");
    return this->elasticMaterialProperties;
  }

  void BehaviourDescription::setElasticMaterialProperties(
      const std::vector<MaterialProperty>& emps) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "BehaviourDescription::setElasticMaterialProperties: " + m);
    };
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    throw_if((this->getBehaviourType() !=
              BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) &&
                 (this->getBehaviourType() !=
                  BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR),
             "only finite and strain behaviour are supported");
    this->setAttribute(BehaviourDescription::requiresStiffnessTensor, false,
                       false);
    throw_if(!this->elasticMaterialProperties.empty(),
             "elastic material property already declared");
    auto lemps =
        emps;  // local copy, swap to data member if no exceptions is thrown
    if (emps.size() == 2u) {
      if (this->isElasticSymmetryTypeDefined()) {
        throw_if(this->getElasticSymmetryType() != mfront::ISOTROPIC,
                 "inconsistent elastic symmetry type");
      } else {
        this->setElasticSymmetryType(mfront::ISOTROPIC);
      }
      checkElasticMaterialProperty(
          *this, lemps[0], tfel::glossary::Glossary::YoungModulus, "young");
      checkElasticMaterialProperty(
          *this, lemps[1], tfel::glossary::Glossary::PoissonRatio, "nu");
    } else if (emps.size() == 9u) {
      throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
               "the behaviour is not orthotropic.");
      if (this->isElasticSymmetryTypeDefined()) {
        throw_if(this->getElasticSymmetryType() != mfront::ORTHOTROPIC,
                 "inconsistent elastic symmetry type");
      } else {
        this->setElasticSymmetryType(mfront::ORTHOTROPIC);
      }
      checkElasticMaterialProperty(
          *this, lemps[0], tfel::glossary::Glossary::YoungModulus1, "young1");
      checkElasticMaterialProperty(
          *this, lemps[1], tfel::glossary::Glossary::YoungModulus2, "young2");
      checkElasticMaterialProperty(
          *this, lemps[2], tfel::glossary::Glossary::YoungModulus3, "young3");
      checkElasticMaterialProperty(
          *this, lemps[3], tfel::glossary::Glossary::PoissonRatio12, "nu12");
      checkElasticMaterialProperty(
          *this, lemps[4], tfel::glossary::Glossary::PoissonRatio23, "nu23");
      checkElasticMaterialProperty(
          *this, lemps[5], tfel::glossary::Glossary::PoissonRatio13, "nu13");
      checkElasticMaterialProperty(
          *this, lemps[6], tfel::glossary::Glossary::ShearModulus12, "mu12");
      checkElasticMaterialProperty(
          *this, lemps[7], tfel::glossary::Glossary::ShearModulus23, "mu23");
      checkElasticMaterialProperty(
          *this, lemps[8], tfel::glossary::Glossary::ShearModulus13, "mu13");
    } else {
      throw_if(true, "unsupported behaviour type");
    }
    this->elasticMaterialProperties.swap(lemps);
  }  // end of BehaviourDescription::setElasticMaterialProperties

  BehaviourSymmetryType BehaviourDescription::getElasticSymmetryType() const {
    if (!this->estypeIsDefined) {
      this->estype = this->getSymmetryType();
      this->estypeIsDefined = true;
    }
    return this->estype;
  }  // end of BehaviourDescription::getElasticSymmetryType

  void BehaviourDescription::setElasticSymmetryType(
      const BehaviourSymmetryType t) {
    tfel::raise_if(this->estypeIsDefined,
                   "BehaviourDescription::setElasticSymmetryType: "
                   "elastic symmetry type already declared");
    const auto s = this->getSymmetryType();
    tfel::raise_if((s == mfront::ISOTROPIC) && (t == mfront::ORTHOTROPIC),
                   "BehaviourDescription::setElasticSymmetryType: "
                   "can't define an orthotropic elastic symmetry for "
                   "an isotropic material");
    this->estype = t;
    this->estypeIsDefined = true;
  }  // end of BehaviourDescription::setElasticSymmetryType

  bool BehaviourDescription::isElasticSymmetryTypeDefined() const {
    return this->estypeIsDefined;
  }  // end of BehaviourDescription::isElasticSymmetryTypeDefined

  BehaviourSymmetryType BehaviourDescription::getSymmetryType() const {
    if (!this->stypeIsDefined) {
      this->stype = mfront::ISOTROPIC;
      this->stypeIsDefined = true;
    }
    return this->stype;
  }  // end of BehaviourDescription::getSymmetryType

  void BehaviourDescription::setSymmetryType(const BehaviourSymmetryType t) {
    tfel::raise_if(this->stypeIsDefined,
                   "BehaviourDescription::setSymmetryType: "
                   "symmetry type already declared");
    this->stype = t;
    this->stypeIsDefined = true;
  }  // end of BehaviourDescription::setSymmetryType

  bool BehaviourDescription::isSymmetryTypeDefined() const {
    return this->stypeIsDefined;
  }  // end of BehaviourDescription::setSymmetryType

  void BehaviourDescription::setCrystalStructure(const CrystalStructure s) {
    tfel::raise_if(this->hasCrystalStructure(),
                   "BehaviourDescription::setCrystalStructure: "
                   "crystal structure already declared");
    this->gs = SlipSystemsDescription(s);
  }  // end of BehaviourDescription::setCrystalStructure

  bool BehaviourDescription::hasCrystalStructure() const {
    return !this->gs.empty();
  }  // end of BehaviourDescription::hasCrystalStructure

  BehaviourDescription::CrystalStructure
  BehaviourDescription::getCrystalStructure() const {
    tfel::raise_if(!this->hasCrystalStructure(),
                   "BehaviourDescription::setCrystalStructure: "
                   "no crystal structure declared");
    return this->gs.get<SlipSystemsDescription>().getCrystalStructure();
  }  // end of BehaviourDescription::getCrystalStructure

  void BehaviourDescription::addHillTensor(
      const VariableDescription& v, const std::vector<MaterialProperty>& hcs) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::addHillTensor: " + m);
    };
    throw_if(
        (this->type != BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) &&
            (this->type != BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR),
        "this method is only valid for small and finite strain behaviours");
    throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
             "the behaviour is not orthotropic.");
    throw_if(hcs.size() != 6u, "invalid number of Hill coefficients");
    throw_if(v.arraySize != 1u, "invalid array size");
    throw_if(v.type != "tfel::math::st2tost2<N,stress>", "invalid type");
    this->addLocalVariable(ModellingHypothesis::UNDEFINEDHYPOTHESIS, v);
    HillTensor h;
    h.name = v.name;
    std::copy(hcs.begin(), hcs.end(), std::back_inserter(h.c));
    this->hillTensors.push_back(std::move(h));
  }  // end of BehaviourDescription::addHillTensor

  const std::vector<BehaviourDescription::HillTensor>&
  BehaviourDescription::getHillTensors() const {
    return this->hillTensors;
  }  // end of BehaviourDescription::getHillTensors

  void BehaviourDescription::declareAsASmallStrainStandardBehaviour() {
    tfel::raise_if(
        !this->mvariables.empty(),
        "BehaviourDescription::declareAsASmallStrainStandardBehaviour: "
        "some driving variables are already declared");
    Gradient eto("StrainStensor", "eto");
    eto.increment_known = true;
    eto.setGlossaryName("Strain");
    ThermodynamicForce sig("StressStensor", "sig");
    sig.setGlossaryName("Stress");
    this->mvariables.push_back({eto, sig});
    this->type = BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "eto");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "deto");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "sig");
  }

  void BehaviourDescription::declareAsAFiniteStrainStandardBehaviour(
      const bool b) {
    tfel::raise_if(
        !this->mvariables.empty(),
        "BehaviourDescription::declareAsAFiniteStrainStandardBehaviour: "
        "some driving variables are already declared");
    Gradient F("DeformationGradientTensor", "F");
    F.setGlossaryName("DeformationGradient");
    F.increment_known = false;
    ThermodynamicForce sig("StressStensor", "sig");
    sig.setGlossaryName("Stress");
    this->mvariables.push_back({F, sig});
    this->type = BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR;
    if (b) {
      this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "F");
      this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "dF");
    }
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "F0");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "F1");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "sig");
  }

  void BehaviourDescription::declareAsACohesiveZoneModel() {
    tfel::raise_if(!this->mvariables.empty(),
                   "BehaviourDescription::declareAsACohesiveZoneModel: "
                   "some driving variables are already declared");
    Gradient u("DisplacementTVector", "u");
    u.setGlossaryName("OpeningDisplacement");
    u.increment_known = true;
    ThermodynamicForce t("ForceTVector", "t");
    t.setGlossaryName("CohesiveForce");
    this->mvariables.push_back({u, t});
    this->type = BehaviourDescription::COHESIVEZONEMODEL;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "u");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "du");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "t");
  }

  void BehaviourDescription::addLocalDataStructure(
      const LocalDataStructure& lds,
      const BehaviourData::RegistrationStatus s) {
    auto structify = [](const std::vector<LocalDataStructure::Variable>& vars) {
      auto r = std::string("struct{\n");
      for (const auto& v : vars) {
        if (v.asize == 1u) {
          r += v.type + ' ' + v.name + ";\n";
        } else {
          r += v.type + ' ' + v.name + "[" + std::to_string(v.asize) + "];\n";
        }
      }
      r += "}";
      return r;
    };
    tfel::raise_if(
        !tfel::utilities::CxxTokenizer::isValidIdentifier(lds.name, true),
        "BehaviourDSLCommon::addLocalDataStructure: "
        "invalid local structure name '" +
            lds.name + "'");
    const auto mh = lds.getSpecialisedHypotheses();
    for (const auto h : mh) {
      if (!lds.get(h).empty()) {  // paranoiac checks, this can't occur
        this->addLocalVariable(h, {structify(lds.get(h)), lds.name, 1u, 0u}, s);
      }
    }
    const auto v = lds.get(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    if (v.empty()) {
      return;
    }
    auto vd = VariableDescription{structify(v), lds.name, 1u, 0u};
    if (!this->areAllMechanicalDataSpecialised()) {
      this->d.addLocalVariable(vd, s);
    }
    for (auto& ld : this->sd) {
      if (std::find(mh.begin(), mh.end(), ld.first) == mh.end()) {
        ld.second->addLocalVariable(vd, s);
      }
    }
  }

  void BehaviourDescription::addMainVariable(const Gradient& dv,
                                             const ThermodynamicForce& f) {
    tfel::raise_if(this->type != BehaviourDescription::GENERALBEHAVIOUR,
                   "BehaviourDescription::addMainVariables: "
                   "one can not add a main variable if the behaviour "
                   "don't have a general behaviour type");
    for (const auto& v : this->mvariables) {
      tfel::raise_if(dv.name == v.first.name,
                     "BehaviourDescription::addMainVariables: "
                     "a driving variable '" +
                         dv.name +
                         "' has "
                         "already been declared");
      tfel::raise_if(f.name == v.second.name,
                     "BehaviourDescription::addMainVariables: "
                     "a driving variable '" +
                         f.name +
                         "' has "
                         "already been declared");
    }
    this->mvariables.push_back({dv, f});
  }  // end of BehaviourDescription::addMainVariables

  const std::vector<std::pair<Gradient, ThermodynamicForce>>&
  BehaviourDescription::getMainVariables() const {
    return this->mvariables;
  }  // end of BehaviourDescription::getMainVariables

  Gradient& BehaviourDescription::getGradient(const std::string& n) {
    using value_type = std::pair<Gradient, ThermodynamicForce>;
    const auto p =
        std::find_if(this->mvariables.begin(), this->mvariables.end(),
                     [&n](const value_type& v) { return v.first.name == n; });
    tfel::raise_if(p == this->mvariables.end(),
                   "BehaviourDescription::getGradient: "
                   "unknown driving variable '" +
                       n + "'");
    return p->first;
  }  // end of BehaviourDescription::getGradient

  const Gradient& BehaviourDescription::getGradient(
      const std::string& n) const {
    using value_type = std::pair<Gradient, ThermodynamicForce>;
    const auto p =
        std::find_if(this->mvariables.begin(), this->mvariables.end(),
                     [&n](const value_type& v) { return v.first.name == n; });
    tfel::raise_if(p == this->mvariables.end(),
                   "BehaviourDescription::getGradient: "
                   "unknown driving variable '" +
                       n + "'");
    return p->first;
  }  // end of BehaviourDescription::getGradient

  ThermodynamicForce& BehaviourDescription::getThermodynamicForce(
      const std::string& n) {
    using value_type = std::pair<Gradient, ThermodynamicForce>;
    const auto p =
        std::find_if(this->mvariables.begin(), this->mvariables.end(),
                     [&n](const value_type& v) { return v.second.name == n; });
    tfel::raise_if(p == this->mvariables.end(),
                   "BehaviourDescription::getGradient: "
                   "unknown driving variable '" +
                       n + "'");
    return p->second;
  }  // end of BehaviourDescription::getThermodynamicForce

  const ThermodynamicForce& BehaviourDescription::getThermodynamicForce(
      const std::string& n) const {
    using value_type = std::pair<Gradient, ThermodynamicForce>;
    const auto p =
        std::find_if(this->mvariables.begin(), this->mvariables.end(),
                     [&n](const value_type& v) { return v.second.name == n; });
    tfel::raise_if(p == this->mvariables.end(),
                   "BehaviourDescription::getGradient: "
                   "unknown driving variable '" +
                       n + "'");
    return p->second;
  }  // end of BehaviourDescription::getThermodynamicForce

  bool BehaviourDescription::isGradientName(const std::string& n) const {
    for (const auto& v : this->getMainVariables()) {
      if (v.first.name == n) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourDescription::isGradientName

  bool BehaviourDescription::isGradientIncrementName(
      const std::string& n) const {
    for (const auto& v : this->getMainVariables()) {
      const auto& dv = v.first;
      if ("d" + dv.name == n) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourDescription::isGradientIncrementName

  bool BehaviourDescription::isThermodynamicForceName(
      const std::string& n) const {
    for (const auto& v : this->getMainVariables()) {
      const auto& tf = v.second;
      if (tf.name == n) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourDescription::isThermodynamicForceName

  std::pair<SupportedTypes::TypeSize, SupportedTypes::TypeSize>
  BehaviourDescription::getMainVariablesSize() const {
    auto ov = SupportedTypes::TypeSize{};
    auto of = SupportedTypes::TypeSize{};
    for (const auto& v : this->getMainVariables()) {
      ov += this->getTypeSize(v.first.type, 1u);
      of += this->getTypeSize(v.second.type, 1u);
    }
    return {ov, of};
  }  // end of BehaviourDescription::getMainVariablesSize

  void BehaviourDescription::setThermalExpansionCoefficient(
      MaterialProperty a) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c, "BehaviourDescription::setThermalExpansionCoefficient: " + m);
    };
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    tfel::raise_if(this->areThermalExpansionCoefficientsDefined(),
                   "BehaviourDescription::setThermalExpansionCoefficient: "
                   "thermal expansion coefficient already defined");
    this->setAttribute(
        ModellingHypothesis::UNDEFINEDHYPOTHESIS,
        BehaviourDescription::requiresThermalExpansionCoefficientTensor, false);
    checkThermalExpansionCoefficientArgument(
        *this, a, Glossary::ThermalExpansion, "alpha");
    this->thermalExpansionCoefficients.push_back(a);
  }  // end of BehaviourDescription::setThermalExpansionCoefficient

  void BehaviourDescription::setThermalExpansionCoefficients(
      MaterialProperty a1, MaterialProperty a2, MaterialProperty a3) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c, "BehaviourDescription::setThermalExpansionCoefficients: " + m);
    };
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    throw_if(this->areThermalExpansionCoefficientsDefined(),
             "thermal expansion coefficient already defined");
    throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
             "the behaviour is not orthotropic.");
    this->setAttribute(
        ModellingHypothesis::UNDEFINEDHYPOTHESIS,
        BehaviourDescription::requiresThermalExpansionCoefficientTensor, false);
    checkThermalExpansionCoefficientArgument(
        *this, a1, Glossary::ThermalExpansion1, "alpha1");
    checkThermalExpansionCoefficientArgument(
        *this, a2, Glossary::ThermalExpansion2, "alpha2");
    checkThermalExpansionCoefficientArgument(
        *this, a3, Glossary::ThermalExpansion3, "alpha3");
    this->thermalExpansionCoefficients.push_back(a1);
    this->thermalExpansionCoefficients.push_back(a2);
    this->thermalExpansionCoefficients.push_back(a3);
  }  // end of BehaviourDescription::setThermalExpansionCoefficients

  void BehaviourDescription::addStressFreeExpansion(
      const Hypothesis h, const StressFreeExpansionDescription& sfed) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::addStressFreeExpansion: " + m);
    };
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    if ((sfed.is<BehaviourData::AxialGrowth>()) ||
        (sfed.is<BehaviourData::OrthotropicStressFreeExpansion>()) ||
        (sfed.is<BehaviourData::OrthotropicStressFreeExpansionII>())) {
      throw_if((this->getBehaviourType() !=
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                   (this->getBehaviourType() !=
                    BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
               "AxialGrowth or OrthotropicStressFreeExpansion "
               "are only valid for small or "
               "finite strain behaviours");
      throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
               "axial growth is only valid for orthotropic behaviour");
    } else {
      throw_if((!sfed.is<BehaviourData::VolumeSwellingStressFreeExpansion>()) &&
                   (!sfed.is<BehaviourData::Relocation>()) &&
                   (!sfed.is<BehaviourData::IsotropicStressFreeExpansion>()),
               "internal error, unsupported stress free expansion type");
      throw_if((this->getBehaviourType() !=
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                   (this->getBehaviourType() !=
                    BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
               "Isotropic, Relocation or VolumeSwelling "
               "are only valid for small or "
               "finite strain behaviours");
    }
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->d.addStressFreeExpansion(sfed);
      for (auto& md : this->sd) {
        md.second->addStressFreeExpansion(sfed);
      }
    } else {
      this->getBehaviourData2(h).addStressFreeExpansion(sfed);
    }
  }  // end of BehaviourDescription::addStressFreeExpansion

  bool BehaviourDescription::requiresStressFreeExpansionTreatment(
      const Hypothesis h) const {
    return ((this->areThermalExpansionCoefficientsDefined()) ||
            (!this->getBehaviourData(h)
                  .getStressFreeExpansionDescriptions()
                  .empty()) ||
            (this->hasCode(h, BehaviourData::ComputeStressFreeExpansion)));
  }  // end of BehaviourDescription::requiresStressFreeExpansionTreatment

  bool BehaviourDescription::areThermalExpansionCoefficientsDefined() const {
    return !this->thermalExpansionCoefficients.empty();
  }  // end of BehaviourDescription::areThermalExpansionCoefficientsDefined

  const std::vector<BehaviourDescription::MaterialProperty>&
  BehaviourDescription::getThermalExpansionCoefficients() const {
    tfel::raise_if(!this->areThermalExpansionCoefficientsDefined(),
                   "BehaviourDescription::getThermalExpansionCoefficients: "
                   "no thermal expansion coefficients defined");
    return this->thermalExpansionCoefficients;
  }

  void BehaviourDescription::setSlipSystems(const std::vector<SlipSystem>& ss) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::setSlipSystems: " + m);
    };
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    throw_if(ss.empty(), "empty number of slip systems specified");
    throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
             "the behaviour is not orthotropic");
    throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
             "the behaviour is not orthotropic");
    throw_if(!this->hasCrystalStructure(),
             "crystal structure is not defined yet");
    for (const auto& s : ss) {
      auto& ssd = this->gs.get<SlipSystemsDescription>();
      const auto nb = ssd.getNumberOfSlipSystemsFamilies();
      if (s.is<SlipSystemsDescription::system3d>()) {
        const auto& s3d = s.get<SlipSystemsDescription::system3d>();
        ssd.addSlipSystemsFamily(s3d.burgers, s3d.plane);
      } else {
        throw_if(!s.is<SlipSystemsDescription::system4d>(),
                 "internal error (unsupported slip system definition)");
        const auto& s4d = s.get<SlipSystemsDescription::system4d>();
        ssd.addSlipSystemsFamily(s4d.burgers, s4d.plane);
      }
      const auto css = ssd.getSlipSystems(nb);
      StaticVariableDescription v("int", "Nss" + std::to_string(nb), 0u,
                                  static_cast<int>(css.size()));
      this->addStaticVariable(uh, v, BehaviourData::UNREGISTRED);
    }
    const auto& ssd = this->gs.get<SlipSystemsDescription>();
    auto n = int{};
    for (SlipSystemsDescription::size_type i = 0;
         i != ssd.getNumberOfSlipSystemsFamilies(); ++i) {
      const auto css = ssd.getSlipSystems(i);
      n += static_cast<int>(css.size());
    }
    StaticVariableDescription v("int", "Nss", 0u, n);
    this->addStaticVariable(uh, v, BehaviourData::UNREGISTRED);
  }

  bool BehaviourDescription::areSlipSystemsDefined() const {
    if (this->gs.empty()) {
      return false;
    }
    auto& ssd = this->gs.get<SlipSystemsDescription>();
    return ssd.getNumberOfSlipSystemsFamilies() != 0;
  }  // end of BehaviourDescription::areSlipSystemsDefined

  const tfel::material::SlipSystemsDescription&
  BehaviourDescription::getSlipSystems() const {
    tfel::raise_if(!this->areSlipSystemsDefined(),
                   "BehaviourDescription::getSlipSystems: "
                   "no slip systems defined");
    return this->gs;
  }  // end of BehaviourDescription::getSlipSystems

  BehaviourDescription::InteractionMatrixStructure
  BehaviourDescription::getInteractionMatrixStructure() const {
    tfel::raise_if(!this->areSlipSystemsDefined(),
                   "BehaviourDescription::getInteractionMatrixStructure: "
                   "no slip system defined");
    return this->gs.get<SlipSystemsDescription>()
        .getInteractionMatrixStructure();
  }  // end of BehaviourDescription::getInteractionMatrix

  bool BehaviourDescription::hasInteractionMatrix() const {
    if (!this->gs.is<SlipSystemsDescription>()) {
      return false;
    }
    return this->gs.get<SlipSystemsDescription>().hasInteractionMatrix();
  }  // end of BehaviourDescription::hasInteractionMatrix

  void BehaviourDescription::setInteractionMatrix(
      const std::vector<long double>& m) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "BehaviourDescription::setInteractionMatrix: " + msg);
    };
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    throw_if(!this->areSlipSystemsDefined(), "no slip system defined");
    this->gs.get<SlipSystemsDescription>().setInteractionMatrix(m);
  }  // end of BehaviourDescription::setInteractionMatrix

  bool BehaviourDescription::hasDislocationsMeanFreePathInteractionMatrix()
      const {
    if (!this->gs.is<SlipSystemsDescription>()) {
      return false;
    }
    return this->gs.get<SlipSystemsDescription>()
        .hasDislocationsMeanFreePathInteractionMatrix();
  }  // end of
     // BehaviourDescription::hasDislocationsMeanFreePathInteractionMatrix

  void BehaviourDescription::setDislocationsMeanFreePathInteractionMatrix(
      const std::vector<long double>& m) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "BehaviourDescription::"
                     "setDislocationsMeanFreePathInteractionMatrix: " +
                         msg);
    };
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    throw_if(!this->areSlipSystemsDefined(), "no slip system defined");
    this->gs.get<SlipSystemsDescription>()
        .setDislocationsMeanFreePathInteractionMatrix(m);
  }  // end of
     // BehaviourDescription::setDislocationsMeanFreePathInteractionMatrix

  void BehaviourDescription::setUseQt(const bool b) {
    tfel::raise_if(this->use_qt,
                   "BehaviourDescription::setUseQt: "
                   "setUseQt already called");
    this->use_qt = b;
  }  // end of BehaviourDescription::setUseQt

  bool BehaviourDescription::useQt() const {
    return this->use_qt;
  }  // end of BehaviourDescription::useQt

  std::string BehaviourDescription::getTangentOperatorType() const {
    if (this->type == GENERALBEHAVIOUR) {
      auto msizes = this->getMainVariablesSize();
      std::ostringstream t;
      t << "tfel::math::tmatrix<" << msizes.first << "," << msizes.second
        << ",real>";
      return t.str();
    } else if (this->type == STANDARDSTRAINBASEDBEHAVIOUR) {
      return "StiffnessTensor";
    } else if (this->type == STANDARDFINITESTRAINBEHAVIOUR) {
      return "FiniteStrainBehaviourTangentOperator<N,stress>";
    } else if (this->type == COHESIVEZONEMODEL) {
      return "tfel::math::tmatrix<N,N,stress>";
    }
    tfel::raise(
        "BehaviourDescription::getStiffnessOperatorType: "
        "internal error (unsupported behaviour type)");
  }  // end of BehaviourDescription::getStiffnessOperatorType

  const std::vector<BehaviourData::StressFreeExpansionDescription>&
  BehaviourDescription::getStressFreeExpansionDescriptions(
      const Hypothesis h) const {
    return this->getBehaviourData(h).getStressFreeExpansionDescriptions();
  }  // end of BehaviourDescription::getStressFreeExpansionDescriptions

  std::string BehaviourDescription::getStressFreeExpansionType() const {
    if ((this->type == STANDARDSTRAINBASEDBEHAVIOUR) ||
        (this->type == STANDARDFINITESTRAINBEHAVIOUR)) {
      return "StrainStensor";
    }
    tfel::raise(
        "BehaviourDescription::getStressFreeExpansionType: "
        "internal error (unsupported behaviour type)");
  }  // end of BehaviourDescription::getStressFreeExpansionType

  bool BehaviourDescription::isStressFreeExansionAnisotropic(
      const Hypothesis h) const {
    return this->getBehaviourData(h).isStressFreeExansionAnisotropic();
  }  // end of BehaviourDescription::isStressFreeExansionAnisotropic

  void BehaviourDescription::checkModellingHypothesis(
      const Hypothesis& h) const {
    if (this->getModellingHypotheses().find(h) ==
        this->getModellingHypotheses().end()) {
      std::ostringstream msg;
      msg << "BehaviourDescription::checkModellingHypothesis: "
          << "modelling hypothesis '" << ModellingHypothesis::toString(h)
          << "' is not supported. Refer to the documentation of "
          << "the '@ModellingHypothesis' or "
          << "the '@ModellingHypotheses' keywords for details.\n";
      msg << "Supported modelling hypotheses are :";
      for (const auto& lh : this->hypotheses) {
        msg << "\n- '" << ModellingHypothesis::toString(lh) << "'";
      }
      tfel::raise(msg.str());
    }
  }  // end of BehaviourDescription::checkModellingHypothesis

  bool BehaviourDescription::areModellingHypothesesDefined() const {
    return !this->hypotheses.empty();
  }  // end of BehaviourDescription::areModellingHypothesesDefined

  const std::set<BehaviourDescription::Hypothesis>&
  BehaviourDescription::getModellingHypotheses() const {
    tfel::raise_if(this->hypotheses.empty(),
                   "BehaviourDescription::getModellingHypotheses: "
                   "hypothesis undefined yet");
    return this->hypotheses;
  }  // end of BehaviourDescription::getModellingHypotheses

  std::set<BehaviourDescription::Hypothesis>
  BehaviourDescription::getDistinctModellingHypotheses() const {
    const auto& mh = this->getModellingHypotheses();
    if (mh.size() == 1u) {
      // if only one modelling hypothesis is supported, it is not
      // considered as specialised, so we return it.
      return mh;
    }
    std::set<Hypothesis> dh;
    if (!this->areAllMechanicalDataSpecialised()) {
      // We return UNDEFINEDHYPOTHESIS to take into account all the
      // modelling hypotheses that were not specialised
      dh.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mh) {
      if (this->hasSpecialisedMechanicalData(h)) {
        dh.insert(h);
      }
    }
    return dh;
  }  // end of BehaviourDescription::getDistinctModellingHypotheses

  bool BehaviourDescription::isModellingHypothesisSupported(
      const Hypothesis h) const {
    return this->getModellingHypotheses().count(h) != 0u;
  }

  void BehaviourDescription::setModellingHypotheses(
      const std::set<Hypothesis>& mh, const bool b) {
    constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::setHypotheses: " + m);
    };
    // never ever trust a user
    throw_if(mh.empty(), "empty set of modelling hypotheses specificied");
    // never ever trust a user
    throw_if(mh.find(uh) != mh.end(),
             "undefined modelling hypothesis specified");
    // check that the user did not already set the modelling hypotheses
    throw_if(!this->hypotheses.empty(),
             "supported modelling hypotheses have already been declared");
    // check that if a specialised version of the behaviour
    // is defined, it is present in the set of hypotheses defined here
    for (const auto& ld : this->sd) {
      throw_if(mh.find(ld.first) == mh.end(),
               "partial specialisation of the behaviour exists for "
               "the hypothesis '" +
                   ModellingHypothesis::toString(ld.first) +
                   "' "
                   "which is not in the set of hypotheses which have to be "
                   "supported by the behaviour.");
    }
    for (const auto h : this->requestedHypotheses) {
      throw_if(
          mh.find(h) == mh.end(),
          "a description of the behaviour for "
          "the hypothesis '" +
              ModellingHypothesis::toString(h) +
              "' "
              "has been requested earlier, but this hypothesis is not "
              "in the set of hypotheses which will to be "
              "supported by the behaviour. This may lead to inconsistencies. "
              "Cowardly aborting.");
    }
    if (this->hypotheses.empty()) {
      if ((this->stypeIsDefined) &&
          (this->getSymmetryType() == mfront::ORTHOTROPIC) &&
          (this->oacIsDefined) &&
          (this->getOrthotropicAxesConvention() ==
           OrthotropicAxesConvention::PLATE)) {
        for (const auto h : mh) {
          throw_if((h != ModellingHypothesis::TRIDIMENSIONAL) &&
                       (h != ModellingHypothesis::PLANESTRESS) &&
                       (h != ModellingHypothesis::PLANESTRAIN) &&
                       (h != ModellingHypothesis::GENERALISEDPLANESTRAIN),
                   "Modelling hypothesis '" + ModellingHypothesis::toString(h) +
                       "' is not compatible "
                       "with the `Plate` orthotropic axes convention");
        }
      }
      this->hypotheses.insert(mh.begin(), mh.end());
    } else {
      if (b) {
        // find the intersection of the given hypotheses and the
        // existing one
        std::set<Hypothesis> nh;
        for (const auto h : this->hypotheses) {
          if (mh.find(h) != mh.end()) {
            nh.insert(h);
          }
        }
        throw_if(nh.empty(),
                 "intersection of previously modelling hypotheses "
                 "with the new ones is empty");
        // as this is the intersection with previously defined
        // hyppotheses, restrictions related to the orthotropic axes
        // conditions does not have to be checked.
        this->hypotheses.swap(nh);
      } else {
        throw_if(true,
                 "supported modelling hypotheses have already been declared");
      }
    }
  }  // end of BehaviourDescription::setModellingHypotheses

  const std::vector<ModelDescription>&
  BehaviourDescription::getModelsDescriptions() const {
    return this->models;
  }  // end of BehaviourDescription::getModelsDescriptions

  void BehaviourDescription::addModelDescription(const ModelDescription& md) {
    constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    for (auto ov : md.outputs) {
      VariableDescription dov{ov.type, "d" + ov.name, ov.arraySize,
                              ov.lineNumber};
      ov.setAttribute("ComputedByExternalModel", true, false);
      this->addAuxiliaryStateVariable(uh, ov, BehaviourData::UNREGISTRED);
      this->addLocalVariable(uh, dov, BehaviourData::UNREGISTRED);
    }
    this->models.push_back(md);
  }  // end of BehaviourDescription::addModelDescription

  void BehaviourDescription::addMaterialProperties(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addMaterialProperty;
    this->addVariables(h, v, s, f);
  }  // end of BehaviourDescription::addMaterialProperties

  void BehaviourDescription::addMaterialProperty(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addMaterialProperty;
    this->addVariable(h, v, s, f);
  }

  void BehaviourDescription::addIntegrationVariables(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addIntegrationVariable;
    this->addVariables(h, v, s, f);
  }

  void BehaviourDescription::addStateVariables(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addStateVariable;
    this->addVariables(h, v, s, f);
  }

  void BehaviourDescription::addIntegrationVariable(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addIntegrationVariable;
    this->addVariable(h, v, s, f);
  }

  void BehaviourDescription::addStateVariable(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addStateVariable;
    this->addVariable(h, v, s, f);
  }

  void BehaviourDescription::addAuxiliaryStateVariables(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addAuxiliaryStateVariable;
    this->addVariables(h, v, s, f);
  }

  void BehaviourDescription::addAuxiliaryStateVariable(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addAuxiliaryStateVariable;
    this->addVariable(h, v, s, f);
  }

  void BehaviourDescription::addExternalStateVariables(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addExternalStateVariable;
    this->addVariables(h, v, s, f);
  }

  void BehaviourDescription::addExternalStateVariable(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addExternalStateVariable;
    this->addVariable(h, v, s, f);
  }

  void BehaviourDescription::addLocalVariables(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addLocalVariable;
    this->addVariables(h, v, s, f);
  }

  void BehaviourDescription::addLocalVariable(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addLocalVariable;
    this->addVariable(h, v, s, f);
  }

  void BehaviourDescription::addParameter(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addParameter;
    this->addVariable(h, v, s, f);
  }

  void BehaviourDescription::setVariableAttribute(const Hypothesis h,
                                                  const std::string& v,
                                                  const std::string& n,
                                                  const VariableAttribute& a,
                                                  const bool b) {
    auto set = [&v, &n, &a, b](BehaviourData& bd) {
      bd.setVariableAttribute(v, n, a, b);
    };
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      set(this->d);
      for (auto md : this->sd) {
        set(*(md.second));
      }
    } else {
      set(this->getBehaviourData2(h));
    }
  }  // end of BehaviourDescription::setVariableAttribute

  bool BehaviourDescription::hasGlossaryName(const Hypothesis h,
                                             const std::string& v) const {
    return this->getData(h, &BehaviourData::hasGlossaryName, v);
  }  // end of BehaviourDescription::hasGlossaryName

  bool BehaviourDescription::hasEntryName(const Hypothesis h,
                                          const std::string& v) const {
    return this->getData(h, &BehaviourData::hasEntryName, v);
  }  // end of BehaviourDescription::hasEntryName

  bool BehaviourDescription::hasParameter(const Hypothesis h,
                                          const std::string& v) const {
    return this->getData(h, &BehaviourData::hasParameter, v);
  }  // end of BehaviourDescription::hasParameter

  bool BehaviourDescription::hasParameters(const Hypothesis h) const {
    return this->getBehaviourData(h).hasParameters();
  }  // end of BehaviourDescription::hasParameters

  bool BehaviourDescription::hasParameters() const {
    if (this->d.hasParameters()) {
      return true;
    }
    for (const auto& ld : this->sd) {
      if (ld.second->hasParameters()) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourDescription::hasParameters

  void BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
                                                      const std::string& n,
                                                      const double v) {
    void (BehaviourData::*mptr)(const std::string&, const double);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h, mptr, n, v, true);
  }

  void BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
                                                      const std::string& n,
                                                      const unsigned short i,
                                                      const double v) {
    void (BehaviourData::*m)(const std::string&, const unsigned short,
                             const double) =
        &BehaviourData::setParameterDefaultValue;
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      (this->d.*m)(n, i, v);
      for (auto md : this->sd) {
        (md.second.get()->*m)(n, i, v);
      }
    } else {
      (this->getBehaviourData2(h).*m)(n, i, v);
    }
  }

  void BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
                                                      const std::string& n,
                                                      const int v) {
    void (BehaviourData::*mptr)(const std::string&, const int v);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h, mptr, n, v, true);
  }

  void BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
                                                      const std::string& n,
                                                      const unsigned short v) {
    void (BehaviourData::*mptr)(const std::string&, const unsigned short v);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h, mptr, n, v, true);
  }

  unsigned short BehaviourDescription::getUnsignedShortParameterDefaultValue(
      const Hypothesis h, const std::string& n) const {
    return this->getData(
        h, &BehaviourData::getUnsignedShortParameterDefaultValue, n);
  }  // end of BehaviourDescription::getUnsignedShortParameterDefaultValue

  int BehaviourDescription::getIntegerParameterDefaultValue(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::getIntegerParameterDefaultValue, n);
  }  // end of BehaviourDescription::getIntegerParameterDefaultValue

  double BehaviourDescription::getFloattingPointParameterDefaultValue(
      const Hypothesis h, const std::string& n) const {
    return this->getData(
        h, &BehaviourData::getFloattingPointParameterDefaultValue, n);
  }  // end of BehaviourDescription::getFloattingPointParameterDefaultValue

  double BehaviourDescription::getFloattingPointParameterDefaultValue(
      const Hypothesis h, const std::string& n, const unsigned short i) const {
    return this->getBehaviourData(h).getFloattingPointParameterDefaultValue(n,
                                                                            i);
  }  // end of BehaviourDescription::getFloattingPointParameterDefaultValue

  void BehaviourDescription::addStaticVariable(
      const Hypothesis h,
      const StaticVariableDescription& v,
      const BehaviourData::RegistrationStatus s) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->d.addStaticVariable(v, s);
      for (const auto& md : this->sd) {
        md.second->addStaticVariable(v, s);
      }
    } else {
      this->getBehaviourData2(h).addStaticVariable(v, s);
    }
  }  // end of BehaviourDescription::addStaticVariable

  int BehaviourDescription::getIntegerConstant(const Hypothesis h,
                                               const std::string& n) const {
    return this->getData(h, &BehaviourData::getIntegerConstant, n);
  }  // end of BehaviourDescription::getIntegerConstant

  void BehaviourDescription::addVariables(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s,
      void (BehaviourData::*m)(const VariableDescription&,
                               const BehaviourData::RegistrationStatus)) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->addVariables(this->d, v, s, m);
      for (auto& md : this->sd) {
        this->addVariables(*(md.second), v, s, m);
      }
    } else {
      this->addVariables(this->getBehaviourData2(h), v, s, m);
    }
  }

  void BehaviourDescription::addVariable(
      const Hypothesis h,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s,
      void (BehaviourData::*m)(const VariableDescription&,
                               const BehaviourData::RegistrationStatus)) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->addVariable(this->d, v, s, m);
      for (auto& md : this->sd) {
        this->addVariable(*(md.second), v, s, m);
      }
    } else {
      this->addVariable(this->getBehaviourData2(h), v, s, m);
    }
  }

  void BehaviourDescription::addVariables(
      BehaviourData& b,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s,
      void (BehaviourData::*m)(const VariableDescription&,
                               const BehaviourData::RegistrationStatus)) {
    for (const auto& e : v) {
      this->addVariable(b, e, s, m);
    }
  }

  void BehaviourDescription::addVariable(
      BehaviourData& b,
      const VariableDescription& v,
      const BehaviourData::RegistrationStatus s,
      void (BehaviourData::*m)(const VariableDescription&,
                               const BehaviourData::RegistrationStatus)) {
    (b.*m)(v, s);
  }

  bool BehaviourDescription::areAllMechanicalDataSpecialised() const {
    return this->getModellingHypotheses().size() == this->sd.size();
  }  // end of BehaviourDescription::areAllMechanicalDataSpecialised

  bool BehaviourDescription::areAllMechanicalDataSpecialised(
      const std::set<Hypothesis>& h) const {
    for (const auto& mh : h) {
      if (!this->hasSpecialisedMechanicalData(mh)) {
        return false;
      }
    }
    return true;
  }  // end of BehaviourDescription::areAllMechanicalDataSpecialised

  bool BehaviourDescription::hasSpecialisedMechanicalData(
      const Hypothesis h) const {
    tfel::raise_if(this->getModellingHypotheses().find(h) ==
                       this->getModellingHypotheses().end(),
                   "BehaviourDescription::areAllMechanicalDataSpecialised: "
                   "hypothesis '" +
                       ModellingHypothesis::toString(h) + "' is not supported");
    return this->sd.find(h) != this->sd.end();
  }

  void BehaviourDescription::requiresTVectorOrVectorIncludes(
      bool& b1, bool& b2, const BehaviourData& bd) const {
    for (const auto& v : bd.getMaterialProperties()) {
      if (v.arraySize > 1) {
        if (this->useDynamicallyAllocatedVector(v.arraySize)) {
          b2 = true;
        } else {
          b1 = true;
        }
      }
      if (b1 && b2) {
        return;
      }
    }
    for (const auto& v : bd.getIntegrationVariables()) {
      if (v.arraySize > 1) {
        if (this->useDynamicallyAllocatedVector(v.arraySize)) {
          b2 = true;
        } else {
          b1 = true;
        }
      }
      if (b1 && b2) {
        return;
      }
    }
    for (const auto& v : bd.getStateVariables()) {
      if (v.arraySize > 1) {
        if (this->useDynamicallyAllocatedVector(v.arraySize)) {
          b2 = true;
        } else {
          b1 = true;
        }
      }
      if (b1 && b2) {
        return;
      }
    }
    for (const auto& v : bd.getAuxiliaryStateVariables()) {
      if (v.arraySize > 1) {
        if (this->useDynamicallyAllocatedVector(v.arraySize)) {
          b2 = true;
        } else {
          b1 = true;
        }
      }
      if (b1 && b2) {
        return;
      }
    }
    for (const auto& v : bd.getLocalVariables()) {
      if (v.arraySize > 1) {
        if (this->useDynamicallyAllocatedVector(v.arraySize)) {
          b2 = true;
        } else {
          b1 = true;
        }
      }
      if (b1 && b2) {
        return;
      }
    }
    for (const auto& v : bd.getExternalStateVariables()) {
      if (v.arraySize > 1) {
        if (this->useDynamicallyAllocatedVector(v.arraySize)) {
          b2 = true;
        } else {
          b1 = true;
        }
      }
      if (b1 && b2) {
        return;
      }
    }
  }  // end of BehaviourData::requiresTVectorOrVectorIncludes

  void BehaviourDescription::requiresTVectorOrVectorIncludes(bool& b1,
                                                             bool& b2) const {
    b1 = b2 = false;
    tfel::raise_if(this->hypotheses.empty(),
                   "BehaviourDescription::areAllMechanicalDataSpecialised: "
                   "no hypothesis defined");
    if (!this->areAllMechanicalDataSpecialised()) {
      this->requiresTVectorOrVectorIncludes(b1, b2, d);
    }
    for (const auto& md : this->sd) {
      this->requiresTVectorOrVectorIncludes(b1, b2, *(md.second));
    }
  }  // end of BehaviourDescription::requiresTVectorOrVectorIncludes

  void BehaviourDescription::
      declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          const Hypothesis h, const std::string& n) {
    void (BehaviourData::*m)(const std::string&) =
        &BehaviourData::
            declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution;
    this->callBehaviourData(h, m, n, true);
  }  // end of
     // BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  void BehaviourDescription::setUsableInPurelyImplicitResolution(
      const Hypothesis h, const bool b) {
    void (BehaviourData::*m)(const bool) =
        &BehaviourData::setUsableInPurelyImplicitResolution;
    this->callBehaviourData(h, m, b, true);
  }  // end of
     // BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  bool BehaviourDescription::isMemberUsedInCodeBlocks(
      const Hypothesis h, const std::string& v) const {
    return this->getData(h, &BehaviourData::isMemberUsedInCodeBlocks, v);
  }  // end of BehaviourDescription::isMaterialPropertyName

  bool BehaviourDescription::isMaterialPropertyName(
      const Hypothesis h, const std::string& v) const {
    return this->getData(h, &BehaviourData::isMaterialPropertyName, v);
  }  // end of BehaviourDescription::isMaterialPropertyName

  bool BehaviourDescription::isLocalVariableName(const Hypothesis h,
                                                 const std::string& n) const {
    return this->getData(h, &BehaviourData::isLocalVariableName, n);
  }  // end of BehaviourDescription::isLocalVariableName

  bool BehaviourDescription::isPersistentVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isPersistentVariableName, n);
  }  // end of BehaviourDescription::isPersistentVariableName

  bool BehaviourDescription::isIntegrationVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isIntegrationVariableName, n);
  }  // end of BehaviourDescription::isIntegrationVariableName

  bool BehaviourDescription::isIntegrationVariableIncrementName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isIntegrationVariableIncrementName,
                         n);
  }  // end of BehaviourDescription::isIntegrationVariableIncrementName

  bool BehaviourDescription::isStateVariableName(const Hypothesis h,
                                                 const std::string& n) const {
    return this->getData(h, &BehaviourData::isStateVariableName, n);
  }  // end of BehaviourDescription::isStateVariableName

  bool BehaviourDescription::isStateVariableIncrementName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isStateVariableIncrementName, n);
  }  // end of BehaviourDescription::isStateVariableIncrementName

  bool BehaviourDescription::isAuxiliaryStateVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isAuxiliaryStateVariableName, n);
  }  // end of BehaviourDescription::isAuxiliaryStateVariableName

  bool BehaviourDescription::isExternalStateVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isExternalStateVariableName, n);
  }  // end of BehaviourDescription::isExternalStateVariableName

  bool BehaviourDescription::isExternalStateVariableIncrementName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(
        h, &BehaviourData::isExternalStateVariableIncrementName, n);
  }  // end of BehaviourDescription::isExternalStateVariableIncrementName

  bool BehaviourDescription::isParameterName(const Hypothesis h,
                                             const std::string& v) const {
    return this->getData(h, &BehaviourData::isParameterName, v);
  }  // end of BehaviourDescription::isParameterName

  bool BehaviourDescription::isStaticVariableName(const Hypothesis h,
                                                  const std::string& n) const {
    return this->getData(h, &BehaviourData::isStaticVariableName, n);
  }  // end of BehaviourDescription::isStaticVariableName

  void BehaviourDescription::updateClassName() {
    if ((!this->behaviour.empty()) || (!this->material.empty())) {
      this->className = this->material + this->behaviour;
    }
  }  // end of BehaviourDescription::updateClassName

  void BehaviourDescription::setCode(const Hypothesis h,
                                     const std::string& n,
                                     const CodeBlock& c,
                                     const Mode m,
                                     const Position p,
                                     const bool b) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourDescription::setCode : setting '" << n
            << "' on default hypothesis" << std::endl;
      }
      this->d.setCode(n, c, m, p, b);
      for (const auto& pd : sd) {
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "BehaviourDescription::setCode : setting '" << n
              << "' on hypothesis '" << ModellingHypothesis::toString(pd.first)
              << "'" << std::endl;
        }
        pd.second->setCode(n, c, m, p, b);
      }
    } else {
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourDescription::setCode : setting '" << n
            << "' on hypothesis '" << ModellingHypothesis::toString(h) << "'"
            << std::endl;
      }
      this->getBehaviourData2(h).setCode(n, c, m, p, b);
    }
  }  // end of BehaviourDescription::setCode

  const CodeBlock& BehaviourDescription::getCodeBlock(
      const Hypothesis h, const std::string& n) const {
    return this->getBehaviourData(h).getCodeBlock(n);
  }  // end of BehaviourDescription::getCode

  std::string BehaviourDescription::getCode(const Hypothesis h,
                                            const std::string& n) const {
    const auto b = this->getAttribute(BehaviourData::profiling, false);
    return this->getBehaviourData(h).getCode(n, this->getClassName(), b);
  }  // end of BehaviourDescription::getCode

  bool BehaviourDescription::hasCode(const Hypothesis h,
                                     const std::string& n) const {
    return this->getBehaviourData(h).hasCode(n);
  }  // end of BehaviourDescription::getCode

  void BehaviourDescription::setAttribute(const Hypothesis h,
                                          const std::string& n,
                                          const BehaviourAttribute& a,
                                          const bool b) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->d.setAttribute(n, a, b);
      for (const auto& md : this->sd) {
        BehaviourData& bdata = *(md.second);
        bdata.setAttribute(n, a, b);
      }
    } else {
      this->getBehaviourData2(h).setAttribute(n, a, b);
    }
  }  // end of BehaviourDescription::setAttribute

  void BehaviourDescription::updateAttribute(const Hypothesis h,
                                             const std::string& n,
                                             const BehaviourAttribute& a) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->d.updateAttribute(n, a);
      for (const auto& md : this->sd) {
        md.second->updateAttribute(n, a);
      }
    } else {
      this->getBehaviourData2(h).updateAttribute(n, a);
    }
  }  // end of BehaviourDescription::updateAttribute

  bool BehaviourDescription::hasAttribute(const Hypothesis h,
                                          const std::string& n) const {
    return this->getBehaviourData(h).hasAttribute(n);
  }  // end of BehaviourDescription::hasAttribute

  std::vector<std::string> BehaviourDescription::getCodeBlockNames(
      const Hypothesis h) const {
    return this->getBehaviourData(h).getCodeBlockNames();
  }

  std::vector<std::string> BehaviourDescription::getExternalNames(
      const Hypothesis h, const VarContainer& v) const {
    return this->getBehaviourData(h).getExternalNames(v);
  }

  void BehaviourDescription::getExternalNames(std::vector<std::string>& n,
                                              const Hypothesis h,
                                              const VarContainer& v) const {
    return this->getBehaviourData(h).getExternalNames(n, v);
  }  // end of BehaviourDescription::getExternalNames

  void BehaviourDescription::appendExternalNames(std::vector<std::string>& n,
                                                 const Hypothesis h,
                                                 const VarContainer& v) const {
    return this->getBehaviourData(h).appendExternalNames(n, v);
  }  // end of BehaviourDescription::appendExternalNames

  void BehaviourDescription::setGlossaryName(const Hypothesis h,
                                             const std::string& n,
                                             const std::string& g) {
    this->callBehaviourData(h, &BehaviourData::setGlossaryName, n, g, true);
  }  // end of BehaviourDescription::setGlossaryName

  bool BehaviourDescription::isGlossaryNameUsed(const Hypothesis h,
                                                const std::string& n) const {
    return this->getBehaviourData(h).isGlossaryNameUsed(n);
  }  // end of BehaviourDescription::isGlossaryName

  void BehaviourDescription::setEntryName(const Hypothesis h,
                                          const std::string& n,
                                          const std::string& g) {
    this->callBehaviourData(h, &BehaviourData::setEntryName, n, g, true);
  }  // end of BehaviourDescription::setEntryName

  bool BehaviourDescription::isUsedAsEntryName(const Hypothesis h,
                                               const std::string& n) const {
    return this->getBehaviourData(h).isUsedAsEntryName(n);
  }  // end of BehaviourDescription::isEntryName

  std::string BehaviourDescription::getExternalName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::getExternalName, n);
  }  // end of BehaviourDescription::getGlossaryName

  std::string BehaviourDescription::getVariableNameFromGlossaryNameOrEntryName(
      const Hypothesis h, const std::string& n) const {
    return this->getBehaviourData(h).getVariableNameFromGlossaryNameOrEntryName(
        n);
  }  // end of BehaviourDescription::getVariableNameFromGlossaryNameOrEntryName

  const VariableDescription&
  BehaviourDescription::getVariableDescriptionByExternalName(
      const Hypothesis h, const std::string& n) const {
    return this->getBehaviourData(h).getVariableDescriptionByExternalName(n);
  }  // end of BehaviourDescription::getVariableDescriptionByExternalName

  void BehaviourDescription::setBounds(const Hypothesis h,
                                       const std::string& n,
                                       const VariableBoundsDescription& b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::setBounds: " + m);
    };
    if (this->isGradientName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getGradient(n).setBounds(b);
    } else if (this->isThermodynamicForceName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getThermodynamicForce(n).setBounds(b);
    } else {
      this->callBehaviourData(h, &BehaviourData::setBounds, n, b, true);
    }
  }  // end of BehaviourDescription::setBounds

  void BehaviourDescription::setBounds(const Hypothesis h,
                                       const std::string& n,
                                       const unsigned short i,
                                       const VariableBoundsDescription& b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::setBounds: " + m);
    };
    if (this->isGradientName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getGradient(n).setBounds(b, i);
    } else if (this->isThermodynamicForceName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getThermodynamicForce(n).setBounds(b, i);
    } else {
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        this->d.setBounds(n, i, b);
        for (auto md : this->sd) {
          md.second.get()->setBounds(n, i, b);
        }
      } else {
        this->getBehaviourData2(h).setBounds(n, i, b);
      }
    }
  }  // end of BehaviourDescription::setBounds

  void BehaviourDescription::setPhysicalBounds(
      const Hypothesis h,
      const std::string& n,
      const VariableBoundsDescription& b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::setPhysicalBounds: " + m);
    };
    if (this->isGradientName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getGradient(n).setBounds(b);
    } else if (this->isThermodynamicForceName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getThermodynamicForce(n).setBounds(b);
    } else {
      this->callBehaviourData(h, &BehaviourData::setPhysicalBounds, n, b, true);
    }
  }  // end of BehaviourDescription::setPhysicalBounds

  void BehaviourDescription::setPhysicalBounds(
      const Hypothesis h,
      const std::string& n,
      const unsigned short i,
      const VariableBoundsDescription& b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::setPhysicalBounds: " + m);
    };
    if (this->isGradientName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getGradient(n).setBounds(b, i);
    } else if (this->isThermodynamicForceName(n)) {
      throw_if(h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               "invalid modelling hypothesis");
      this->getThermodynamicForce(n).setBounds(b, i);
    } else {
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        this->d.setPhysicalBounds(n, i, b);
        for (auto md : this->sd) {
          md.second.get()->setPhysicalBounds(n, i, b);
        }
      } else {
        this->getBehaviourData2(h).setPhysicalBounds(n, i, b);
      }
    }
  }  // end of BehaviourDescription::setPhysicalBounds

  void BehaviourDescription::setAttribute(const std::string& n,
                                          const BehaviourAttribute& a,
                                          const bool b) {
    if (b) {
      auto p = this->attributes.find(n);
      if (p != this->attributes.end()) {
        tfel::raise_if(a.getTypeIndex() != p->second.getTypeIndex(),
                       "BehaviourDescription::setAttribute: "
                       "attribute already exists with a different type");
        return;
      }
    }
    tfel::raise_if(!this->attributes.insert({n, a}).second,
                   "BehaviourDescription::setAttribute: "
                   "attribute '" +
                       n + "' already declared");
  }  // end of BehaviourDescription::setAttribute

  bool BehaviourDescription::hasAttribute(const std::string& n) const {
    return this->attributes.count(n) != 0u;
  }  // end of BehaviourDescription::hasAttribute

  const std::map<std::string, BehaviourAttribute>&
  BehaviourDescription::getAttributes() const {
    return this->attributes;
  }  // end of BehaviourDescription::getAttributes

  void BehaviourDescription::reserveName(const Hypothesis h,
                                         const std::string& n) {
    this->callBehaviourData(h, &BehaviourData::reserveName, n, true);
  }

  bool BehaviourDescription::isNameReserved(const std::string& n) const {
    if (this->d.isNameReserved(n)) {
      return true;
    }
    for (auto md : this->sd) {
      if (md.second->isNameReserved(n)) {
        return true;
      }
    }
    return false;
  }  // end of BehaviourDescription::isNameReserved

  void BehaviourDescription::registerMemberName(const Hypothesis h,
                                                const std::string& n) {
    this->callBehaviourData(h, &BehaviourData::registerMemberName, n, true);
  }  // end of BehaviourDescription::registerMemberName

  void BehaviourDescription::registerStaticMemberName(const Hypothesis h,
                                                      const std::string& n) {
    this->callBehaviourData(h, &BehaviourData::registerStaticMemberName, n,
                            true);
  }  // end of BehaviourDescription::registerMemberName

  void BehaviourDescription::addMaterialLaw(const std::string& m) {
    if (std::find(this->materialLaws.begin(), this->materialLaws.end(), m) ==
        this->materialLaws.end()) {
      this->materialLaws.push_back(m);
    }
  }  // end of BehaviourDescription::getMaterialLaws

  const std::vector<std::string>& BehaviourDescription::getMaterialLaws()
      const {
    return this->materialLaws;
  }

  std::pair<bool, bool> BehaviourDescription::checkVariableExistence(
      const std::string& v) const {
    const auto& mh = this->getDistinctModellingHypotheses();
    std::pair<bool, bool> r{true, false};
    for (const auto& h : mh) {
      const auto& bdata = this->getBehaviourData(h);
      const auto& vn = bdata.getVariablesNames();
      const bool b = vn.find(v) != vn.end();
      r.first = r.first && b;
      r.second = r.second || b;
    }
    if (!r.second) {
      r.first = false;
    }
    return r;
  }  // end of checkVariableExistence

  std::pair<bool, bool> BehaviourDescription::checkVariableExistence(
      const std::string& n, const std::string& c, const bool b) const {
    const auto& mh = this->getDistinctModellingHypotheses();
    std::pair<bool, bool> r = {true, false};
    for (const auto h : mh) {
      const auto& bd = this->getBehaviourData(h);
      const auto f = bd.getVariables(c).contains(n);
      tfel::raise_if(!f && b,
                     "BehaviourDescription::checkVariableExistence: "
                     "no '" +
                         c + "' named '" + n +
                         "' found for at "
                         "least one modelling hypothesis");
      r.first = r.first && f;
      r.second = r.second || f;
    }
    if (!r.second) {
      r.first = false;
    }
    return r;
  }

  void BehaviourDescription::checkVariableGlossaryName(
      const std::string& n, const std::string& g) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "BehaviourDescription::"
                     "checkVariableGlossaryName: " +
                         m);
    };
    for (const auto& h : this->getDistinctModellingHypotheses()) {
      const auto& bdata = this->getBehaviourData(h);
      throw_if(!bdata.hasGlossaryName(n),
               "no glossary name associated with variable '" + n + "'");
      const auto& en = bdata.getExternalName(n);
      throw_if(en != g,
               "the glossary name associated with "
               "variable '" +
                   n + "' is not '" + g + "', but '" + en + "'");
    }
  }  // end of BehaviourDescription::checkVariableGlossaryName

  void BehaviourDescription::checkVariableEntryName(
      const std::string& n, const std::string& e) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "BehaviourDescription::"
                     "checkVariableEntryName: " +
                         m);
    };
    for (const auto& h : this->getDistinctModellingHypotheses()) {
      const auto& bdata = this->getBehaviourData(h);
      throw_if(!bdata.hasEntryName(n),
               "no entry name associated with variable '" + n + "'");
      const auto& en = bdata.getExternalName(n);
      throw_if(en != e,
               "the entry name associated with "
               "variable '" +
                   n + "' is not '" + e + "', but '" + en + "'");
    }
  }  // end of BehaviourDescription::checkVariableEntryName

  void BehaviourDescription::checkVariablePosition(const std::string& n,
                                                   const std::string& c,
                                                   const size_t p) {
    for (const auto& h : this->getDistinctModellingHypotheses()) {
      const auto& bdata = this->getBehaviourData(h);
      const auto& vc = bdata.getVariables(c);
      tfel::raise_if(p >= vc.size(),
                     "BehaviourDescription::checkVariablePosition: "
                     "position given is greater than the number "
                     "of variables of category '" +
                         c + "'");
      const auto& v = vc[p];
      tfel::raise_if(v.name != n,
                     "BehaviourDescription::checkVariablePosition: "
                     "variable at the given position is not named '" +
                         n + "' but '" + v.name + "'");
    }
  }  // end of BehaviourDescription::checkVariablePosition

  void BehaviourDescription::setOrthotropicAxesConvention(
      const OrthotropicAxesConvention c) {
    tfel::raise_if(this->oacIsDefined,
                   "BehaviourDescription::setOrthotropicAxesConvention: "
                   "orthotropic axes convention already defined");
    tfel::raise_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
                   "BehaviourDescription::setOrthotropicAxesConvention: "
                   "the behaviour is not orthotropic.");
    if (c == OrthotropicAxesConvention::PLATE) {
      if (this->areModellingHypothesesDefined()) {
        for (const auto h : this->getModellingHypotheses()) {
          tfel::raise_if((h != ModellingHypothesis::TRIDIMENSIONAL) &&
                             (h != ModellingHypothesis::PLANESTRESS) &&
                             (h != ModellingHypothesis::PLANESTRAIN) &&
                             (h != ModellingHypothesis::GENERALISEDPLANESTRAIN),
                         "Modelling hypothesis '" +
                             ModellingHypothesis::toString(h) +
                             "' is not compatible "
                             "with the `Plate` orthotropic axes convention");
        }
      }
    }
    this->oacIsDefined = true;
    this->oac = c;
  }

  tfel::material::OrthotropicAxesConvention
  BehaviourDescription::getOrthotropicAxesConvention() const {
    tfel::raise_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
                   "BehaviourDescription::getOrthotropicAxesConvention: "
                   "the behaviour is not orthotropic.");
    if (!this->oacIsDefined) {
      this->oacIsDefined = true;
    }
    return this->oac;
  }

  bool BehaviourDescription::useDynamicallyAllocatedVector(
      const unsigned short s) const {
    return (s >= SupportedTypes::ArraySizeLimit) &&
           (this->areDynamicallyAllocatedVectorsAllowed());
  }  // end of SupportedTypes::useDynamicallyAllocatedVector

  bool BehaviourDescription::areDynamicallyAllocatedVectorsAllowed() const {
    if (this->areDynamicallyAllocatedVectorsAllowed_.is<bool>()) {
      return this->areDynamicallyAllocatedVectorsAllowed_.get<bool>();
    }
    return true;
  }  // end of SupportedTypes::areDynamicallyAllocatedVectorsAllowed

  void BehaviourDescription::areDynamicallyAllocatedVectorsAllowed(
      const bool b) {
    if (this->areDynamicallyAllocatedVectorsAllowed_.is<bool>()) {
      tfel::raise_if(
          this->areDynamicallyAllocatedVectorsAllowed_.get<bool>() != b,
          "BehaviourDescription::areDynamicallyAllocatedVectorsAllowed: "
          "inconsistent policy for dynamically allocated vectors");
      return;
    }
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  }  // end of SupportedTypes::areDynamicallyAllocatedVectorsAllowed

  void BehaviourDescription::setStrainMeasure(const StrainMeasure sm) {
    tfel::raise_if(
        !this->allowsNewUserDefinedVariables(),
        "BehaviourDescription::setStrainMeasure: "
        "the strain measure must be set before the first code block");
    tfel::raise_if(this->getBehaviourType() !=
                       BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                   "BehaviourDescription::setStrainMeasure: "
                   "invalid behaviour type");
    tfel::raise_if(this->isStrainMeasureDefined(),
                   "BehaviourDescription::setStrainMeasure: "
                   "strain measure already defined");
    this->strainMeasure = sm;
  }

  BehaviourDescription::StrainMeasure BehaviourDescription::getStrainMeasure()
      const {
    tfel::raise_if(!this->isStrainMeasureDefined(),
                   "BehaviourDescription::getStrainMeasure: "
                   "no strain measure defined");
    return this->strainMeasure.get<StrainMeasure>();
  }  // end of BehaviourDescription::setStrainMeasure()

  bool BehaviourDescription::isStrainMeasureDefined() const {
    return this->strainMeasure.is<StrainMeasure>();
  }  // end of BehaviourDescription::isStrainMeasureDefined()

  BehaviourDescription::~BehaviourDescription() = default;

  void setElasticSymmetryType(BehaviourDescription& bd,
                              const BehaviourSymmetryType s) {
    if (bd.isElasticSymmetryTypeDefined()) {
      tfel::raise_if(bd.getElasticSymmetryType() != s,
                     "setElasticSymmetryType: "
                     "the elastic symmetry type defined for "
                     "the behaviour is inconsistent.");
    } else {
      bd.setElasticSymmetryType(s);
    }
  }  // end of setElasticSymmetryType

  void checkIsStrictlyPositive(
      const BehaviourDescription::MaterialProperty& mp) {
    if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
      auto& cmp = mp.get<BehaviourDescription::ConstantMaterialProperty>();
      if (!(cmp.value > 0)) {
        tfel::raise("checkIsStrictlyPositive: material property '" + cmp.name +
                    "' is not strictly positive");
      }
    }
  }  // end of checkIsStrictlyPositive

  void checkIsStrictlyNegative(
      const BehaviourDescription::MaterialProperty& mp) {
    if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
      auto& cmp = mp.get<BehaviourDescription::ConstantMaterialProperty>();
      if (!(cmp.value < 0)) {
        tfel::raise("checkIsStrictlyNegative: material property '" + cmp.name +
                    "' is not strictly negative");
      }
    }
  }  // end of checkIsStrictlyNegative

}  // end of namespace mfront
