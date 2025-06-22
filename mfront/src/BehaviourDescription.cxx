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
#include "MFront/DSLBase.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront {

  const char* const BehaviourDescription::
      automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable =
          "automatic_declaration_of_the_temperature_as_first_external_state_"
          "variable";
  const char* const BehaviourDescription::modellingHypothesis =
      "modelling_hypothesis";
  const char* const BehaviourDescription::modellingHypotheses =
      "modelling_hypotheses";

  static MaterialPropertyDescription buildMaterialPropertyDescription(
      const BehaviourDescription::ConstantMaterialProperty& mp,
      const BehaviourDescription& bd,
      const std::string& n) {
    const auto prefix =
        bd.isBehaviourNameDefined() ? bd.getBehaviourName() + "_" : "";
    auto mpd = MaterialPropertyDescription{};
    mpd.output = VariableDescription{"real", "res", 1u, 0u};
    std::ostringstream body;
    body << "res = " << mp.value << ";\n";
    mpd.law = prefix + n;
    mpd.className = prefix + n;
    mpd.material = bd.getMaterialName();
    mpd.f.modified = true;
    mpd.f.body = body.str();
    return mpd;
  }  // end of buildMaterialPropertyDescription

  static MaterialPropertyDescription buildMaterialPropertyDescription(
      const BehaviourDescription::ExternalMFrontMaterialProperty& mp,
      const BehaviourDescription& bd,
      const std::string& n) {
    const auto prefix =
        bd.isBehaviourNameDefined() ? bd.getBehaviourName() + "_" : "";
    auto mpd = *(mp.mpd);
    mpd.law = prefix + n;
    mpd.className = prefix + n;
    mpd.material = bd.getMaterialName();
    return mpd;
  }  // end of buildMaterialPropertyDescription

  static MaterialPropertyDescription buildMaterialPropertyDescription(
      const BehaviourDescription::AnalyticMaterialProperty& mp,
      const BehaviourDescription& bd,
      const std::string& n) {
    const auto prefix =
        bd.isBehaviourNameDefined() ? bd.getBehaviourName() + "_" : "";
    auto mpd = MaterialPropertyDescription{};
    mpd.output = VariableDescription{"real", "res", 1u, 0u};
    mpd.law = prefix + n;
    mpd.className = prefix + n;
    mpd.material = bd.getMaterialName();
    const auto h = *(bd.getDistinctModellingHypotheses().begin());
    const auto& d = bd.getBehaviourData(h);
    for (const auto& i : bd.getMaterialPropertyInputs(mp.getVariablesNames())) {
      if (i.category ==
          BehaviourDescription::MaterialPropertyInput::STATICVARIABLE) {
        mpd.staticVars.push_back(d.getStaticVariables().get(i.name));
      } else {
        const auto& v = d.getVariableDescriptionByExternalName(i.ename);
        mpd.inputs.push_back(v);
      }
    }
    mpd.f.modified = true;
    tfel::math::Evaluator e(mp.f);
    mpd.f.body = "res = " + e.getCxxFormula() + ";\n";
    return mpd;
  }  // end of buildMaterialPropertyDescription

  static MaterialPropertyDescription buildMaterialPropertyDescription(
      const BehaviourDescription::MaterialProperty& mp,
      const BehaviourDescription& bd,
      const std::string& n) {
    if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
      return buildMaterialPropertyDescription(
          mp.get<BehaviourDescription::ConstantMaterialProperty>(), bd, n);
    } else if (mp.is<BehaviourDescription::ExternalMFrontMaterialProperty>()) {
      return buildMaterialPropertyDescription(
          mp.get<BehaviourDescription::ExternalMFrontMaterialProperty>(), bd,
          n);
    } else if (mp.is<BehaviourDescription::AnalyticMaterialProperty>()) {
      return buildMaterialPropertyDescription(
          mp.get<BehaviourDescription::AnalyticMaterialProperty>(), bd, n);
    } else {
      tfel::raise(
          "buildMaterialPropertyDescription: unsupported material property "
          "type");
    }
  }  // end of buildMaterialPropertyDescription

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
  }  // end of callBehaviourData

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
  }  // end of callBehaviourData

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
  }  // end of callBehaviourData

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
  }  // end of callBehaviourData

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
  }  // end of callBehaviourData

  template <typename Res>
  Res BehaviourDescription::getData(const Hypothesis h,
                                    Res (BehaviourData::*m)() const) const {
    return (this->getBehaviourData(h).*m)();
  }  // end of getData

  template <typename Res, typename Arg1>
  Res BehaviourDescription::getData(const Hypothesis h,
                                    Res (BehaviourData::*m)(const Arg1&) const,
                                    const Arg1& a) const {
    return (this->getBehaviourData(h).*m)(a);
  }  // end of getData

  static void declareParameter(BehaviourDescription& bd,
                               BehaviourDescription::MaterialProperty& mp,
                               const tfel::glossary::GlossaryEntry& e,
                               const std::string& t,
                               const std::string& n) {
    const auto h = tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
      auto& cmp = mp.get<BehaviourDescription::ConstantMaterialProperty>();
      cmp.name = n;
      // declare associated parameter
      VariableDescription m(t, n, 1u, 0u);
      bd.addParameter(h, m);
      bd.setParameterDefaultValue(h, n, cmp.value);
      bd.setGlossaryName(h, n, e.getKey());
    }
  }  // end of declareParameter

  static void checkElasticMaterialProperty(
      BehaviourDescription& bd,
      BehaviourDescription::MaterialProperty& emp,
      const tfel::glossary::GlossaryEntry& e,
      const std::string& t,
      const std::string& n) {
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
    declareParameter(bd, emp, e, t, n);
  }

  static void checkThermalExpansionCoefficientArgument(
      BehaviourDescription& bd,
      BehaviourDescription::MaterialProperty& a,
      const tfel::glossary::GlossaryEntry& e,
      const std::string& n) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "checkThermalExpansionCoefficientArgument: " + m);
    };
    declareParameter(bd, a, e, "thermalexpansion", n);
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

  static std::pair<VariableDescription, VariableDescription>
  decomposeAdditionalTangentOperatorBlock(const BehaviourDescription& bd,
                                          const std::string& bn) {
    const auto h = [&bd] {
      if (bd.areModellingHypothesesDefined()) {
        const auto mhs = bd.getModellingHypotheses();
        if (mhs.size() == 1u) {
          return *(mhs.begin());
        }
      }
      return BehaviourDescription::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    }();
    auto a = VariableDescription{};
    auto b = VariableDescription{};
    auto found = false;
    auto check = [&found, &bn] {
      if (found) {
        tfel::raise(
            "decomposeAdditionalTangentOperatorBlock: "
            "multiple match for block '" +
            bn + "'");
      }
    };
    auto assign_if = [&check, &a, &b, &found, &bn](
                         const VariableDescription& v1,
                         const VariableDescription& v2, const bool no_inc) {
      const auto block_name = [&v1, &v2, &no_inc] {
        if (no_inc) {
          return "d" + v1.name + "_d" + v2.name + "1";
        }
        return "d" + v1.name + "_dd" + v2.name;
      }();
      const auto symbolic_block_name = [&v1, &v2, &no_inc] {
        const auto tmp = "\u2202" + displayName(v1) + "\u2215\u2202";
        if (no_inc) {
          return tmp + displayName(v2) + "1";
        }
        return tmp + "\u0394" + displayName(v2);
      }();
      if ((bn == block_name) || (bn == symbolic_block_name)) {
        check();
        a = v1;
        b = v2;
        found = true;
      }
    };
    const auto& d = bd.getBehaviourData(h);
    for (const auto& mv : bd.getMainVariables()) {
      for (const auto& mv2 : bd.getMainVariables()) {
        assign_if(mv.second, mv2.first, !Gradient::isIncrementKnown(mv2.first));
      }
      for (const auto& e : d.getExternalStateVariables()) {
        assign_if(mv.second, e, false);
      }
    }
    for (const auto& s : d.getPersistentVariables()) {
      for (const auto& mv : bd.getMainVariables()) {
        assign_if(s, mv.first, !Gradient::isIncrementKnown(mv.first));
      }
      for (const auto& e : d.getExternalStateVariables()) {
        assign_if(s, e, false);
      }
    }
    if (!found) {
      tfel::raise(
          "decomposeAdditionalTangentOperatorBlock: "
          "no match for '" +
          bn + "'");
    }
    return {a, b};
  }  // end of decomposeTangentOperatorBlock

  const char* const BehaviourDescription::requiresStiffnessTensor =
      "requiresStiffnessTensor";

  const char* const BehaviourDescription::computesStiffnessTensor =
      "computesStiffnessTensor";

  const char* const BehaviourDescription::requiresUnAlteredStiffnessTensor =
      "requiresUnAlteredStiffnessTensor";

  const char* const
      BehaviourDescription::requiresThermalExpansionCoefficientTensor =
          "requiresThermalExpansionCoefficientTensor";

  BehaviourDescription::BehaviourDescription() {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto* const Topt = BehaviourDescription::
        automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable;
    auto T = VariableDescription{"temperature", "T", 1u, 0u};
    T.setGlossaryName("Temperature");
    this->addExternalStateVariable(h, T, BehaviourData::UNREGISTRED);
    this->setAttribute(Topt, true, false);
  }  // end of BehaviourDescription

  BehaviourDescription::BehaviourDescription(
      const tfel::utilities::DataMap& opts) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //
    if ((opts.count(BehaviourDescription::modellingHypothesis) != 0) &&
        (opts.count(BehaviourDescription::modellingHypotheses) != 0)) {
      tfel::raise(
          "BehaviourDescription::BehaviourDescription: "
          "can't specify both options '" +
          std::string{BehaviourDescription::modellingHypothesis} + "' and '" +
          std::string{BehaviourDescription::modellingHypotheses} + "'");
    }
    if (opts.count(BehaviourDescription::modellingHypothesis) != 0) {
      this->overriden_hypotheses.insert(ModellingHypothesis::fromString(
          opts.at(BehaviourDescription::modellingHypothesis)
              .get<std::string>()));
    }
    if (opts.count(BehaviourDescription::modellingHypotheses) != 0) {
      const auto& mhs = opts.at(BehaviourDescription::modellingHypotheses)
                            .get<std::vector<tfel::utilities::Data>>();
      for (const auto& h : mhs) {
        if (!h.is<std::string>()) {
          tfel::raise(
              "BehaviourDescription::BehaviourDescription: "
              "invalid data type in option '" +
              std::string{BehaviourDescription::modellingHypotheses} + "'");
        }
        this->overriden_hypotheses.insert(
            ModellingHypothesis::fromString(h.get<std::string>()));
      }
    }
    // override parameters
    const auto oparameters =
        tfel::utilities::convert<std::map<std::string, double>>(
            tfel::utilities::get_if<tfel::utilities::DataMap>(
                opts, DSLBase::overridingParameters,
                tfel::utilities::DataMap{}));
    for (const auto& op : oparameters) {
      this->overrideByAParameter(op.first, op.second);
    }
    // treating the temperature
    const auto* const Topt = BehaviourDescription::
        automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable;
    const auto bT = tfel::utilities::get_if<bool>(opts, Topt, true);
    this->setAttribute(Topt, bT, false);
    if (bT) {
      auto T = VariableDescription{"temperature", "T", 1u, 0u};
      T.setGlossaryName("Temperature");
      this->addExternalStateVariable(uh, T, BehaviourData::UNREGISTRED);
    }
  }  // end of BehaviourDescription

  BehaviourDescription::BehaviourDescription(const BehaviourDescription&) =
      default;

  bool BehaviourDescription::isModel() const noexcept {
    if (this->getBehaviourType() != BehaviourDescription::GENERALBEHAVIOUR) {
      return false;
    }
    return this->mvariables.empty();
  }  // end of isModel

  bool BehaviourDescription::allowsNewUserDefinedVariables() const {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    return this->getAttribute(h, BehaviourData::allowsNewUserDefinedVariables,
                              true);
  }  // end of allowNewsUserDefinedVariables

  void BehaviourDescription::disallowNewUserDefinedVariables() {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->d.finalizeVariablesDeclaration();
    for (const auto& bd : this->sd) {
      bd.second->finalizeVariablesDeclaration();
    }
    this->setAttribute(uh, BehaviourData::allowsNewUserDefinedVariables, false);
  }  // end of disallowNewUserDefinedVariables

  void BehaviourDescription::checkAndCompletePhysicalBoundsDeclaration() {
    if (this->hasUnitSystem()) {
      this->d.checkAndCompletePhysicalBoundsDeclaration(this->getUnitSystem());
      for (const auto& bd : this->sd) {
        bd.second->checkAndCompletePhysicalBoundsDeclaration(
            this->getUnitSystem());
      }
    }
  }  // end of checkAndCompletePhysicalBoundsDeclaration

  void BehaviourDescription::throwUndefinedAttribute(const std::string& n) {
    tfel::raise(
        "BehaviourDescription::getAttribute: "
        "no attribute named '" +
        n + "'");
  }  // end of throwUndefinedAttribute

  const BehaviourData& BehaviourDescription::getBehaviourData(
      const Hypothesis h) const {
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
  }  // end of getBehaviourData

  void BehaviourDescription::specialize(const Hypothesis h) {
    if (this->areModellingHypothesesDefined()) {
      this->checkModellingHypothesis(h);
    }
    auto p = this->sd.find(h);
    if (p == this->sd.end()) {
      // copy of the default description
      this->sd.insert({h, std::make_shared<BehaviourData>(this->d)});
    }
  }  // end of specialize

  BehaviourData& BehaviourDescription::getBehaviourData2(const Hypothesis h) {
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
  }  // end of getBehaviourData2

  bool BehaviourDescription::isBehaviourNameDefined() const {
    return !this->behaviour.empty();
  }  // end of isBehaviourNameDefined

  void BehaviourDescription::setBehaviourName(const std::string& m) {
    tfel::raise_if(!this->behaviour.empty(),
                   "BehaviourDescription::setBehaviourName: "
                   "behaviour name already defined");
    this->behaviour = m;
    this->updateClassName();
  }  // end of setBehaviourName

  const std::string& BehaviourDescription::getBehaviourName() const {
    tfel::raise_if(this->behaviour.empty(),
                   "BehaviourDescription::getBehaviourName: "
                   "behaviour name not defined");
    return this->behaviour;
  }  // end of getBehaviourName

  void BehaviourDescription::setDSLName(const std::string& m) {
    tfel::raise_if(!this->dsl.empty(),
                   "BehaviourDescription::setDSLName: "
                   "dsl name already defined");
    this->dsl = m;
    this->updateClassName();
  }  // end of setDSLName

  const std::string& BehaviourDescription::getDSLName() const {
    tfel::raise_if(this->dsl.empty(),
                   "BehaviourDescription::getDSLName: "
                   "dsl name not defined");
    return this->dsl;
  }  // end of getDSLName

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
    }
    throw_if(!this->isStaticVariableName(h, v),
             "unsupported variable: variable '" + v +
                 "' is neither an external state variable, a material "
                 "property nor a parameter nor an auxiliary "
                 "state variable evaluated by an external model, "
                 "nor a static variable");
    return MaterialPropertyInput::STATICVARIABLE;
  }  // end of getMaterialPropertyInputCategory

  BehaviourDescription::MaterialPropertyInput
  BehaviourDescription::getMaterialPropertyInput(const std::string& v,
                                                 const bool b) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::getMaterialPropertyInput: " + m);
    };
    const auto hs = [this, b,
                     throw_if]() -> std::set<BehaviourDescription::Hypothesis> {
      if (this->hypotheses.empty()) {
        // modelling hypotheses are not set yet
        throw_if(b, "modelling hypothesis must be defined");
        return {ModellingHypothesis::UNDEFINEDHYPOTHESIS};
      }
      return this->getDistinctModellingHypotheses();
    }();
    if (v == "T") {
      return {"T", tfel::glossary::Glossary::Temperature,
              MaterialPropertyInput::TEMPERATURE};
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
        return {v, v, t};
      } else {
        const auto vd = this->getBehaviourData(rh).getVariableDescription(v);
        const auto en = vd.getExternalName();
        for (const auto h : hs) {
          const auto vd2 = this->getBehaviourData(h).getVariableDescription(v);
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
        return {v, en, t};
      }
    }
  }  // end of getMaterialPropertyInput

  std::vector<BehaviourDescription::MaterialPropertyInput>
  BehaviourDescription::getMaterialPropertyInputs(
      const std::vector<std::string>& i, const bool b) const {
    auto inputs = std::vector<MaterialPropertyInput>{};
    for (const auto& v : i) {
      inputs.push_back(this->getMaterialPropertyInput(v, b));
    }
    return inputs;
  }  // end of getMaterialPropertyInputs

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
  }  // end of getMaterialPropertyInputs

  void BehaviourDescription::setIntegrationScheme(
      const BehaviourDescription::IntegrationScheme s) {
    tfel::raise_if(this->ischeme != UNDEFINEDINTEGRATIONSCHEME,
                   "BehaviourDescription::setIntegrationScheme: "
                   "integration scheme already defined");
    this->ischeme = s;
  }  // end of setIntegrationScheme

  BehaviourDescription::IntegrationScheme
  BehaviourDescription::getIntegrationScheme() const {
    tfel::raise_if(this->ischeme == UNDEFINEDINTEGRATIONSCHEME,
                   "BehaviourDescription::getIntegrationScheme: "
                   "the integration scheme is undefined");
    return this->ischeme;
  }  // end of getIntegrationScheme

  void BehaviourDescription::setLibrary(const std::string& l) {
    tfel::raise_if(!this->library.empty(),
                   "BehaviourDescription::setLibrary: "
                   "library alreay defined");
    this->library = l;
  }  // end of setLibrary

  const std::string& BehaviourDescription::getLibrary() const {
    return this->library;
  }  // end of getLibrary

  void BehaviourDescription::setMaterialName(const std::string& m) {
    tfel::raise_if(!this->material.empty(),
                   "BehaviourDescription::setMaterialName: "
                   "material name alreay defined");
    this->material = m;
    this->updateClassName();
  }  // end of setMaterialName

  const std::string& BehaviourDescription::getMaterialName() const {
    return this->material;
  }  // end of getMaterialName

  void BehaviourDescription::setClassName(const std::string& n) {
    tfel::raise_if(!this->className.empty(),
                   "BehaviourDescription::setClassName: "
                   "class name alreay defined");
    this->className = n;
  }  // end of setClassName

  const std::string& BehaviourDescription::getClassName() const {
    tfel::raise_if(this->className.empty(),
                   "BehaviourDescription::getClassName: "
                   "class name not defined");
    return this->className;
  }  // end of getClassName

  void BehaviourDescription::appendToIncludes(const std::string& c) {
    this->includes += c;
    if (!this->includes.empty()) {
      if (*(this->includes.rbegin()) != '\n') {
        this->includes += '\n';
      }
    }
  }  // end of appendToIncludes

  const std::string& BehaviourDescription::getIncludes() const {
    return this->includes;
  }  // end of getIncludes

  void BehaviourDescription::appendToMembers(const Hypothesis h,
                                             const std::string& c,
                                             const bool b) {
    this->callBehaviourData(h, &BehaviourData::appendToMembers, c, b);
  }  // end of appendToMembers

  std::string BehaviourDescription::getMembers(const Hypothesis h) const {
    return this->getBehaviourData(h).getMembers();
  }  // end of getMembers

  void BehaviourDescription::appendToPrivateCode(const Hypothesis h,
                                                 const std::string& c,
                                                 const bool b) {
    this->callBehaviourData(h, &BehaviourData::appendToPrivateCode, c, b);
  }  // end of appendToPrivateCode

  void BehaviourDescription::addInitializeFunction(const Hypothesis h,
                                                   const std::string& n,
                                                   const CodeBlock& c) {
    this->callBehaviourData(h, &BehaviourData::addInitializeFunction, n, c,
                            true);
  }  // end of addInitializeFunction

  void BehaviourDescription::addPostProcessing(const Hypothesis h,
                                               const std::string& n,
                                               const CodeBlock& c) {
    this->callBehaviourData(h, &BehaviourData::addPostProcessing, n, c, true);
  }  // end of addPostProcessing

  std::string BehaviourDescription::getPrivateCode(const Hypothesis h) const {
    return this->getBehaviourData(h).getPrivateCode();
  }  // end of getPrivateCode

  void BehaviourDescription::appendToSources(const std::string& c) {
    this->sources += c;
    if (!this->sources.empty()) {
      if (*(this->sources.rbegin()) != '\n') {
        this->sources += '\n';
      }
    }
  }  // end of appendToSources

  const std::string& BehaviourDescription::getSources() const {
    return this->sources;
  }  // end of getSources

  BehaviourDescription::BehaviourType BehaviourDescription::getBehaviourType()
      const {
    if (this->type.empty()) {
      tfel::raise(
          "BehaviourDescription::getBehaviourType: "
          "undefined behaviour type");
    }
    return this->type;
  }  // end of getBehaviourType

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
    } else if (this->getBehaviourType() ==
               BehaviourDescription::GENERALBEHAVIOUR) {
      btype = "MechanicalBehaviourBase::GENERALBEHAVIOUR";
    } else {
      tfel::raise(
          "BehaviourDescription::getBehaviourTypeFlag: "
          "unsupported behaviour type");
    }
    return btype;
  }  // end of getBehaviourTypeFlag

  bool BehaviourDescription::areElasticMaterialPropertiesDefined() const {
    return !this->elasticMaterialProperties.empty();
  }  // end of areElasticMaterialPropertiesDefined

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
  }  // end of isMaterialPropertyConstantDuringTheTimeStep

  bool
  BehaviourDescription::areElasticMaterialPropertiesConstantDuringTheTimeStep()
      const {
    tfel::raise_if(!this->areElasticMaterialPropertiesDefined(),
                   "BehaviourDescription::"
                   "areElasticMaterialPropertiesConstantDuringTheTimeStep: "
                   "no elastic material property defined");
    return this->areMaterialPropertiesConstantDuringTheTimeStep(
        this->elasticMaterialProperties);
  }  // end of areElasticMaterialPropertiesConstantDuringTheTimeStep

  bool BehaviourDescription::areMaterialPropertiesConstantDuringTheTimeStep(
      const std::vector<MaterialProperty>& mps) const {
    for (const auto& mp : mps) {
      if (!this->isMaterialPropertyConstantDuringTheTimeStep(mp)) {
        return false;
      }
    }
    return true;
  }  // end of areMaterialPropertiesConstantDuringTheTimeStep

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
  }  // end of isMaterialPropertyDependantOnStateVariables

  bool
  BehaviourDescription::areElasticMaterialPropertiesDependantOnStateVariables()
      const {
    tfel::raise_if(!this->areElasticMaterialPropertiesDefined(),
                   "BehaviourDescription::"
                   "areElasticMaterialPropertiesDependantOnStateVariables: "
                   "no elastic material property defined");
    return this->areMaterialPropertiesDependantOnStateVariables(
        this->elasticMaterialProperties);
  }  // end of areElasticMaterialPropertiesDependantOnStateVariables

  bool BehaviourDescription::areMaterialPropertiesDependantOnStateVariables(
      const std::vector<MaterialProperty>& mps) const {
    for (const auto& mp : mps) {
      if (this->isMaterialPropertyDependantOnStateVariables(mp)) {
        return true;
      }
    }
    return false;
  }  // end of areMaterialPropertiesDependantOnStateVariables

  const std::vector<BehaviourDescription::MaterialProperty>&
  BehaviourDescription::getElasticMaterialProperties() const {
    tfel::raise_if(!this->areElasticMaterialPropertiesDefined(),
                   "BehaviourDescription::getElasticMaterialProperties: "
                   "no elastic material property defined");
    return this->elasticMaterialProperties;
  }  // end of getElasticMaterialProperties

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
      checkElasticMaterialProperty(*this, lemps[0],
                                   tfel::glossary::Glossary::YoungModulus,
                                   "stress", "young");
      checkElasticMaterialProperty(*this, lemps[1],
                                   tfel::glossary::Glossary::PoissonRatio,
                                   "real", "nu");
    } else if (emps.size() == 9u) {
      throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
               "the behaviour is not orthotropic.");
      if (this->isElasticSymmetryTypeDefined()) {
        throw_if(this->getElasticSymmetryType() != mfront::ORTHOTROPIC,
                 "inconsistent elastic symmetry type");
      } else {
        this->setElasticSymmetryType(mfront::ORTHOTROPIC);
      }
      checkElasticMaterialProperty(*this, lemps[0],
                                   tfel::glossary::Glossary::YoungModulus1,
                                   "stress", "young1");
      checkElasticMaterialProperty(*this, lemps[1],
                                   tfel::glossary::Glossary::YoungModulus2,
                                   "stress", "young2");
      checkElasticMaterialProperty(*this, lemps[2],
                                   tfel::glossary::Glossary::YoungModulus3,
                                   "stress", "young3");
      checkElasticMaterialProperty(*this, lemps[3],
                                   tfel::glossary::Glossary::PoissonRatio12,
                                   "real", "nu12");
      checkElasticMaterialProperty(*this, lemps[4],
                                   tfel::glossary::Glossary::PoissonRatio23,
                                   "real", "nu23");
      checkElasticMaterialProperty(*this, lemps[5],
                                   tfel::glossary::Glossary::PoissonRatio13,
                                   "real", "nu13");
      checkElasticMaterialProperty(*this, lemps[6],
                                   tfel::glossary::Glossary::ShearModulus12,
                                   "stress", "mu12");
      checkElasticMaterialProperty(*this, lemps[7],
                                   tfel::glossary::Glossary::ShearModulus23,
                                   "stress", "mu23");
      checkElasticMaterialProperty(*this, lemps[8],
                                   tfel::glossary::Glossary::ShearModulus13,
                                   "stress", "mu13");
    } else {
      throw_if(true, "unsupported behaviour type");
    }
    this->elasticMaterialProperties.swap(lemps);
  }  // end of setElasticMaterialProperties

  BehaviourSymmetryType BehaviourDescription::getElasticSymmetryType() const {
    if (!this->estypeIsDefined) {
      this->estype = this->getSymmetryType();
      this->estypeIsDefined = true;
    }
    return this->estype;
  }  // end of getElasticSymmetryType

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
  }  // end of setElasticSymmetryType

  bool BehaviourDescription::isElasticSymmetryTypeDefined() const {
    return this->estypeIsDefined;
  }  // end of isElasticSymmetryTypeDefined

  BehaviourSymmetryType BehaviourDescription::getSymmetryType() const {
    if (!this->stypeIsDefined) {
      this->stype = mfront::ISOTROPIC;
      this->stypeIsDefined = true;
    }
    return this->stype;
  }  // end of getSymmetryType

  void BehaviourDescription::setSymmetryType(const BehaviourSymmetryType t) {
    tfel::raise_if(this->stypeIsDefined,
                   "BehaviourDescription::setSymmetryType: "
                   "symmetry type already declared");
    this->stype = t;
    this->stypeIsDefined = true;
  }  // end of setSymmetryType

  bool BehaviourDescription::isSymmetryTypeDefined() const {
    return this->stypeIsDefined;
  }  // end of setSymmetryType

  void BehaviourDescription::setCrystalStructure(const CrystalStructure s) {
    tfel::raise_if(this->hasCrystalStructure(),
                   "BehaviourDescription::setCrystalStructure: "
                   "crystal structure already declared");
    this->gs.emplace(s);
  }  // end of setCrystalStructure

  bool BehaviourDescription::hasCrystalStructure() const {
    return this->gs.has_value();
  }  // end of hasCrystalStructure

  BehaviourDescription::CrystalStructure
  BehaviourDescription::getCrystalStructure() const {
    tfel::raise_if(!this->hasCrystalStructure(),
                   "BehaviourDescription::setCrystalStructure: "
                   "no crystal structure declared");
    return this->gs.value().getCrystalStructure();
  }  // end of getCrystalStructure

  void BehaviourDescription::addHillTensor(
      const VariableDescription& v, const std::vector<MaterialProperty>& hcs) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::addHillTensor: " + m);
    };
    throw_if(
        (this->getBehaviourType() !=
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) &&
            (this->getBehaviourType() !=
             BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR),
        "this method is only valid for small and finite strain behaviours");
    throw_if(this->getSymmetryType() != mfront::ORTHOTROPIC,
             "the behaviour is not orthotropic.");
    throw_if(hcs.size() != 6u, "invalid number of Hill coefficients");
    throw_if(v.arraySize != 1u, "invalid array size");
    throw_if(v.type != "tfel::math::st2tost2<N,real>", "invalid type");
    this->addLocalVariable(ModellingHypothesis::UNDEFINEDHYPOTHESIS, v);
    HillTensor h;
    h.name = v.name;
    h.symbolic_form = v.symbolic_form;
    std::copy(hcs.begin(), hcs.end(), std::back_inserter(h.c));
    this->hillTensors.push_back(std::move(h));
  }  // end of addHillTensor

  const std::vector<BehaviourDescription::HillTensor>&
  BehaviourDescription::getHillTensors() const {
    return this->hillTensors;
  }  // end of getHillTensors

  void BehaviourDescription::declareAsGenericBehaviour() {
    tfel::raise_if(!this->type.empty(),
                   "BehaviourDescription::declareAsGenericBehaviour: "
                   "behaviour type has already been defined");
    this->type = GENERALBEHAVIOUR;
  }  // end of declareAsGenericBehaviour

  void BehaviourDescription::declareAsASmallStrainStandardBehaviour() {
    tfel::raise_if(
        !this->type.empty(),
        "BehaviourDescription::declareAsASmallStrainStandardBehaviour: "
        "behaviour type has already been defined");
    tfel::raise_if(
        !this->mvariables.empty(),
        "BehaviourDescription::declareAsASmallStrainStandardBehaviour: "
        "some driving variables are already declared");
    Gradient eto("StrainStensor", "\u03B5\u1D57\u1D52", "eto");
    Gradient::setIsIncrementKnownAttribute(eto, true);
    eto.setGlossaryName("Strain");
    ThermodynamicForce sig("StressStensor", "\u03C3", "sig");
    sig.setGlossaryName("Stress");
    this->addMainVariable2(eto, sig, true, true);
    this->type = BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR;
  }  // end of declareAsASmallStrainStandardBehaviour

  void BehaviourDescription::declareAsAFiniteStrainStandardBehaviour(
      const bool b) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    tfel::raise_if(
        !this->type.empty(),
        "BehaviourDescription::declareAsAFiniteStrainStandardBehaviour: "
        "behaviour type has already been defined");
    tfel::raise_if(
        !this->mvariables.empty(),
        "BehaviourDescription::declareAsAFiniteStrainStandardBehaviour: "
        "some driving variables are already declared");
    Gradient F("DeformationGradientTensor", "F");
    F.setGlossaryName("DeformationGradient");
    Gradient::setIsIncrementKnownAttribute(F, false);
    ThermodynamicForce sig("StressStensor", "\u03C3", "sig");
    sig.setGlossaryName("Stress");
    if (b) {
      constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->registerMemberName(uh, "F");
      this->registerMemberName(uh, "dF");
    }
    this->addMainVariable2(F, sig, false, false);
    this->registerGlossaryName(uh, "F1", "DeformationGradient");
    this->type = BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR;
  }

  void BehaviourDescription::declareAsACohesiveZoneModel() {
    tfel::raise_if(!this->type.empty(),
                   "BehaviourDescription::declareAsACohesiveZoneModel: "
                   "behaviour type has already been defined");
    tfel::raise_if(!this->mvariables.empty(),
                   "BehaviourDescription::declareAsACohesiveZoneModel: "
                   "some driving variables are already declared");
    Gradient u("DisplacementTVector", "u");
    u.setGlossaryName("OpeningDisplacement");
    Gradient::setIsIncrementKnownAttribute(u, true);
    ThermodynamicForce t("ForceTVector", "t");
    t.setGlossaryName("CohesiveForce");
    this->addMainVariable2(u, t, true, true);
    this->type = BehaviourDescription::COHESIVEZONEMODEL;
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

  void BehaviourDescription::addMainVariable(const Gradient& g,
                                             const ThermodynamicForce& f) {
    tfel::raise_if(
        this->getBehaviourType() != BehaviourDescription::GENERALBEHAVIOUR,
        "BehaviourDescription::addMainVariable: "
        "one can not add a main variable if the behaviour "
        "don't have a general behaviour type");
    tfel::raise_if(
        !this->allowsNewUserDefinedVariables(),
        "BehaviourDescription::addMainVariable: "
        "new variables are can't be defined after the first code block.");
    this->addMainVariable2(g, f, true, true);
  }

  void BehaviourDescription::addMainVariable2(
      const Gradient& g,
      const ThermodynamicForce& f,
      const bool registerGradientGlossaryName,
      const bool registerTangentOperatorBlock) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (g.arraySize != f.arraySize) {
      tfel::raise(
          "BehaviourDescription::addMainVariable2: "
          "the gradient '" +
          g.name + "' and the thermodynamic force '" + f.name +
          "' must have the same array size (" + std::to_string(g.arraySize) +
          " vs " + std::to_string(f.arraySize) + ")");
    }
    for (const auto& v : this->mvariables) {
      tfel::raise_if(g.name == v.first.name,
                     "BehaviourDescription::addMainVariable2: "
                     "a gradient named '" +
                         g.name +
                         "' has "
                         "already been declared");
      tfel::raise_if(f.name == v.second.name,
                     "BehaviourDescription::addMainVariable2: "
                     "a thermodynamic force named '" +
                         f.name +
                         "' has "
                         "already been declared");
    }
    if (Gradient::isIncrementKnown(g)) {
      this->registerMemberName(h, g.name);
      this->registerMemberName(h, "d" + g.name);
    } else {
      this->registerMemberName(h, g.name + "0");
      this->registerMemberName(h, g.name + "1");
    }
    if ((g.hasGlossaryName()) && (registerGradientGlossaryName)) {
      this->registerGlossaryName(h, g.name, g.getExternalName());
    }
    if (g.hasEntryName()) {
      this->registerEntryName(h, g.name, g.getExternalName());
    }
    this->registerMemberName(h, f.name);
    if (f.hasGlossaryName()) {
      this->registerGlossaryName(h, f.name, f.getExternalName());
    }
    if (f.hasEntryName()) {
      this->registerEntryName(h, f.name, f.getExternalName());
    }
    if (registerTangentOperatorBlock) {
      this->registerMemberName(h, this->getTangentOperatorBlockName({f, g}));
    }
    this->mvariables.push_back({g, f});
  }  // end of addMainVariables

  const std::vector<std::pair<Gradient, ThermodynamicForce>>&
  BehaviourDescription::getMainVariables() const {
    return this->mvariables;
  }  // end of getMainVariables

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
  }  // end of getGradient

  const Gradient& BehaviourDescription::getGradient(
      const std::string& n) const {
    using value_type = std::pair<Gradient, ThermodynamicForce>;
    const auto p =
        std::find_if(this->mvariables.begin(), this->mvariables.end(),
                     [&n](const value_type& v) { return v.first.name == n; });
    tfel::raise_if(p == this->mvariables.end(),
                   "BehaviourDescription::getGradient: "
                   "unknown gradient '" +
                       n + "'");
    return p->first;
  }  // end of getGradient

  ThermodynamicForce& BehaviourDescription::getThermodynamicForce(
      const std::string& n) {
    using value_type = std::pair<Gradient, ThermodynamicForce>;
    const auto p =
        std::find_if(this->mvariables.begin(), this->mvariables.end(),
                     [&n](const value_type& v) { return v.second.name == n; });
    tfel::raise_if(p == this->mvariables.end(),
                   "BehaviourDescription::getGradient: "
                   "unknown thermodynamic force '" +
                       n + "'");
    return p->second;
  }  // end of getThermodynamicForce

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
  }  // end of getThermodynamicForce

  bool BehaviourDescription::isGradientName(const std::string& n) const {
    for (const auto& v : this->getMainVariables()) {
      if (v.first.name == n) {
        return true;
      }
    }
    return false;
  }  // end of isGradientName

  bool BehaviourDescription::isGradientIncrementName(
      const std::string& n) const {
    for (const auto& v : this->getMainVariables()) {
      const auto& g = v.first;
      if ((Gradient::isIncrementKnown(g)) && ("d" + g.name == n)) {
        return true;
      }
    }
    return false;
  }  // end of isGradientIncrementName

  bool BehaviourDescription::isThermodynamicForceName(
      const std::string& n) const {
    for (const auto& v : this->getMainVariables()) {
      const auto& tf = v.second;
      if (tf.name == n) {
        return true;
      }
    }
    return false;
  }  // end of isThermodynamicForceName

  std::pair<SupportedTypes::TypeSize, SupportedTypes::TypeSize>
  BehaviourDescription::getMainVariablesSize() const {
    auto ov = SupportedTypes::TypeSize{};
    auto of = SupportedTypes::TypeSize{};
    for (const auto& v : this->getMainVariables()) {
      ov += this->getTypeSize(v.first.type, 1u);
      of += this->getTypeSize(v.second.type, 1u);
    }
    return {ov, of};
  }  // end of getMainVariablesSize

  std::string BehaviourDescription::getTangentOperatorBlockName(
      const std::pair<VariableDescription, VariableDescription>& block) const {
    const auto& a = block.first;
    const auto& b = block.second;
    if (this->isGradientName(b.name)) {
      if (Gradient::isIncrementKnown(b)) {
        return "d" + a.name + "_dd" + b.name;
      } else {
        return "d" + a.name + "_d" + b.name + "1";
      }
    }
    return "d" + a.name + "_dd" + b.name;
  }  // end of getTangentOperatorBlockName

  std::string BehaviourDescription::getTangentOperatorBlockSymbolicName(
      const std::pair<VariableDescription, VariableDescription>& block) const {
    const auto& a = block.first;
    const auto& b = block.second;
    const auto tmp = "\u2202" + displayName(a) + "\u2215\u2202";
    if ((this->isGradientName(b.name)) && (!Gradient::isIncrementKnown(b))) {
      return tmp + displayName(b) + "1";
    }
    return tmp + "\u0394" + displayName(b);
  }  // end of getTangentOperatorBlockSymbolicName

  void BehaviourDescription::addTangentOperatorBlock(const std::string& bn) {
    tfel::raise_if(
        this->getBehaviourType() != BehaviourDescription::GENERALBEHAVIOUR,
        "BehaviourDescription::addTangentOperatorBlock: "
        "the behaviour must be generic "
        "to add a tangent operator block");
    // check that the operator blocks exists
    const auto b = decomposeAdditionalTangentOperatorBlock(*this, bn);
    const auto block_name = this->getTangentOperatorBlockName(b);
    for (const auto& eb : this->getTangentOperatorBlocks()) {
      if (block_name == this->getTangentOperatorBlockName(eb)) {
        tfel::raise(
            "BehaviourDescription::addTangentOperatorBlock: "
            "tangent operator block '" +
            bn + "' already defined");
      }
    }
    this->additionalTangentOperatorBlocks.push_back(b);
  }  // end of addTangentOperatorBlock

  void BehaviourDescription::addTangentOperatorBlocks(
      const std::vector<std::string>& blocks) {
    for (const auto& b : blocks) {
      this->addTangentOperatorBlock(b);
    }
  }  // end of addTangentOperatorBlocks

  void BehaviourDescription::setTangentOperatorBlocks(
      const std::vector<std::string>& blocks) {
    if (!this->additionalTangentOperatorBlocks.empty()) {
      if (this->useDefaultTangentOperatorBlocks) {
        tfel::raise(
            "BehaviourDescription::setTangentOperatorBlocks: "
            "invalid call, must be called before adding tangent operator "
            "blocks");
      } else {
        tfel::raise(
            "BehaviourDescription::setTangentOperatorBlocks: "
            "invalid call, this method has already been called");
      }
    }
    if (blocks.empty()) {
      tfel::raise(
          "BehaviourDescription::setTangentOperatorBlocks: "
          "empty list of tangent operator blocks");
    }
    this->useDefaultTangentOperatorBlocks = false;
    for (const auto& b : blocks) {
      this->addTangentOperatorBlock(b);
    }
  }  // end of setTangentOperatorBlocks

  std::vector<std::pair<VariableDescription, VariableDescription>>
  BehaviourDescription::getTangentOperatorBlocks() const {
    auto blocks =
        std::vector<std::pair<VariableDescription, VariableDescription>>{};
    if (this->useDefaultTangentOperatorBlocks) {
      for (const auto& f : this->getMainVariables()) {
        for (const auto& g : this->getMainVariables()) {
          blocks.push_back({f.second, g.first});
        }
      }
    }
    blocks.insert(blocks.end(), this->additionalTangentOperatorBlocks.begin(),
                  this->additionalTangentOperatorBlocks.end());
    return blocks;
  }  // end of getTangentOperatorBlocks

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
  }  // end of setThermalExpansionCoefficient

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
  }  // end of setThermalExpansionCoefficients

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
  }  // end of addStressFreeExpansion

  bool BehaviourDescription::requiresStressFreeExpansionTreatment(
      const Hypothesis h) const {
    return ((this->areThermalExpansionCoefficientsDefined()) ||
            (!this->getBehaviourData(h)
                  .getStressFreeExpansionDescriptions()
                  .empty()) ||
            (this->hasCode(h, BehaviourData::ComputeStressFreeExpansion)));
  }  // end of requiresStressFreeExpansionTreatment

  bool BehaviourDescription::areThermalExpansionCoefficientsDefined() const {
    return !this->thermalExpansionCoefficients.empty();
  }  // end of areThermalExpansionCoefficientsDefined

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
      auto& ssd = this->gs.value();
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
    const auto& ssd = this->gs.value();
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
    if (!this->gs.has_value()) {
      return false;
    }
    auto& ssd = this->gs.value();
    return ssd.getNumberOfSlipSystemsFamilies() != 0;
  }  // end of areSlipSystemsDefined

  const tfel::material::SlipSystemsDescription&
  BehaviourDescription::getSlipSystems() const {
    tfel::raise_if(!this->areSlipSystemsDefined(),
                   "BehaviourDescription::getSlipSystems: "
                   "no slip systems defined");
    return this->gs.value();
  }  // end of getSlipSystems

  BehaviourDescription::InteractionMatrixStructure
  BehaviourDescription::getInteractionMatrixStructure() const {
    tfel::raise_if(!this->areSlipSystemsDefined(),
                   "BehaviourDescription::getInteractionMatrixStructure: "
                   "no slip system defined");
    return this->gs.value().getInteractionMatrixStructure();
  }  // end of getInteractionMatrix

  bool BehaviourDescription::hasInteractionMatrix() const {
    if (!this->gs.has_value()) {
      return false;
    }
    return this->gs.value().hasInteractionMatrix();
  }  // end of hasInteractionMatrix

  void BehaviourDescription::setInteractionMatrix(
      const std::vector<long double>& m) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "BehaviourDescription::setInteractionMatrix: " + msg);
    };
    throw_if(!this->allowsNewUserDefinedVariables(),
             "new variables are can't be defined after the first code block.");
    throw_if(!this->areSlipSystemsDefined(), "no slip system defined");
    this->gs.value().setInteractionMatrix(m);
  }  // end of setInteractionMatrix

  bool BehaviourDescription::hasDislocationsMeanFreePathInteractionMatrix()
      const {
    if (!this->gs.has_value()) {
      return false;
    }
    return this->gs.value().hasDislocationsMeanFreePathInteractionMatrix();
  }  // end of hasDislocationsMeanFreePathInteractionMatrix

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
    this->gs.value().setDislocationsMeanFreePathInteractionMatrix(m);
  }  // end of setDislocationsMeanFreePathInteractionMatrix

  void BehaviourDescription::setUseQt(const bool b) {
    tfel::raise_if(this->use_qt.has_value(),
                   "BehaviourDescription::setUseQt: "
                   "setUseQt already called");
    this->use_qt = b;
  }  // end of setUseQt

  bool BehaviourDescription::useQt() const {
    if (!this->use_qt.has_value()) {
      this->disableQuantitiesUsageIfNotAlreadySet();
    }
    return *(this->use_qt);
  }  // end of useQt

  bool BehaviourDescription::hasTangentOperator() const {
    const auto& mvs = this->getMainVariables();
    return (!mvs.empty()) || (!this->additionalTangentOperatorBlocks.empty());
  }  // end of hasTangentOperator

  bool BehaviourDescription::hasTrivialTangentOperatorStructure() const {
    const auto& mvs = this->getMainVariables();
    return (mvs.size() == 1) && (mvs.front().first.arraySize == 1u) &&
           (mvs.front().second.arraySize == 1u) &&
           (this->additionalTangentOperatorBlocks.empty());
  }  // end of hasTrivialTangentOperatorStructure

  std::string BehaviourDescription::computeTangentOperatorSize() const {
    const auto& blocks = this->getTangentOperatorBlocks();
    if (blocks.empty()) {
      return "0";
    }
    std::ostringstream t;
    auto p = blocks.begin();
    const auto pe = blocks.end();
    t << "(" << p->first.getTypeSize() << ")"
      << "*(" << p->second.getTypeSize() << ")";
    ++p;
    while (p != pe) {
      t << "+"
        << "(" << p->first.getTypeSize() << ")"
        << "*(" << p->second.getTypeSize() << ")";
      ++p;
    }
    return t.str();
  }  // end of computeTangentOperatorSize

  std::string BehaviourDescription::getTangentOperatorType() const {
    if (this->getBehaviourType() == GENERALBEHAVIOUR) {
      const auto& mvs = this->getMainVariables();
      if (this->hasTrivialTangentOperatorStructure()) {
        const auto& mv = mvs.front();
        const auto gflags = mv.first.getTypeFlag();
        const auto fflags = mv.second.getTypeFlag();
        auto throw_unmatch = [&mv] {
          tfel::raise(
              "BehaviourDescription::getTangentOperatorType: "
              "gradient '" +
              mv.first.name + "' is not conjugated with flux '" +
              mv.second.name + "'");
        };
        if (gflags == SupportedTypes::SCALAR) {
          if (fflags != SupportedTypes::SCALAR) {
            throw_unmatch();
          }
          return "real";
        } else if (gflags == SupportedTypes::TVECTOR) {
          if (fflags != SupportedTypes::TVECTOR) {
            throw_unmatch();
          }
          return "tfel::math::tmatrix<N,N,real>";
        } else if (gflags == SupportedTypes::STENSOR) {
          if (fflags != SupportedTypes::STENSOR) {
            throw_unmatch();
          }
          return "tfel::math::st2tost2<N,real>";
        } else if (gflags == SupportedTypes::TENSOR) {
          if (fflags == SupportedTypes::STENSOR) {
            return "tfel::math::t2tost2<N,real>";
          } else if (fflags == SupportedTypes::TENSOR) {
            return "tfel::math::t2tot2<N,real>";
          } else {
            throw_unmatch();
          }
        } else {
          tfel::raise(
              "BehaviourDescription::getTangentOperatorType: "
              "unsupported type for gradient '" +
              mv.first.name + "'");
        }
      }
      return "tfel::math::tvector<" + this->computeTangentOperatorSize() +
             ",real>";
    } else if (this->getBehaviourType() == STANDARDSTRAINBASEDBEHAVIOUR) {
      return "StiffnessTensor";
    } else if (this->getBehaviourType() == STANDARDFINITESTRAINBEHAVIOUR) {
      return "FiniteStrainBehaviourTangentOperator<N,stress>";
    } else if (this->getBehaviourType() == COHESIVEZONEMODEL) {
      return "tfel::math::tmatrix<N,N,stress>";
    }
    tfel::raise(
        "BehaviourDescription::getStiffnessOperatorType: "
        "internal error (unsupported behaviour type)");
  }  // end of getStiffnessOperatorType

  const std::vector<BehaviourData::StressFreeExpansionDescription>&
  BehaviourDescription::getStressFreeExpansionDescriptions(
      const Hypothesis h) const {
    return this->getBehaviourData(h).getStressFreeExpansionDescriptions();
  }  // end of getStressFreeExpansionDescriptions

  std::string BehaviourDescription::getStressFreeExpansionType() const {
    if ((this->getBehaviourType() == STANDARDSTRAINBASEDBEHAVIOUR) ||
        (this->getBehaviourType() == STANDARDFINITESTRAINBEHAVIOUR)) {
      return "StrainStensor";
    }
    tfel::raise(
        "BehaviourDescription::getStressFreeExpansionType: "
        "internal error (unsupported behaviour type)");
  }  // end of getStressFreeExpansionType

  bool BehaviourDescription::isStressFreeExansionAnisotropic(
      const Hypothesis h) const {
    return this->getBehaviourData(h).isStressFreeExansionAnisotropic();
  }  // end of isStressFreeExansionAnisotropic

  void BehaviourDescription::checkModellingHypothesis(
      const Hypothesis h) const {
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
  }  // end of checkModellingHypothesis

  bool BehaviourDescription::areModellingHypothesesDefined() const {
    return !this->hypotheses.empty();
  }  // end of areModellingHypothesesDefined

  const std::set<BehaviourDescription::Hypothesis>&
  BehaviourDescription::getModellingHypotheses() const {
    tfel::raise_if(this->hypotheses.empty(),
                   "BehaviourDescription::getModellingHypotheses: "
                   "hypothesis undefined yet");
    return this->hypotheses;
  }  // end of getModellingHypotheses

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
  }  // end of getDistinctModellingHypotheses

  bool BehaviourDescription::isModellingHypothesisSupported(
      const Hypothesis h) const {
    return this->getModellingHypotheses().count(h) != 0u;
  }

  void BehaviourDescription::setModellingHypotheses(
      const std::set<Hypothesis>& mhs, const bool b) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourDescription::setHypotheses: " + m);
    };
    auto update_hypotheses = [this,
                              throw_if](const std::set<Hypothesis>& nmhs) {
      if (this->overriden_hypotheses.empty()) {
        this->hypotheses = nmhs;
      } else {
        for (const auto& h : this->overriden_hypotheses) {
          if (nmhs.find(h) == nmhs.end()) {
            tfel::raise(
                "BehaviourDescription::setHypotheses: "
                "overriden modelling hypothesis '" +
                ModellingHypothesis::toString(h) +
                "' is not part of the supported modelling hypothesis");
          }
        }
        this->hypotheses = this->overriden_hypotheses;
      }
    };
    // never ever trust a user
    throw_if(mhs.empty(), "empty set of modelling hypotheses specificied");
    // never ever trust a user
    throw_if(mhs.find(uh) != mhs.end(),
             "undefined modelling hypothesis specified");
    // check that the user did not already set the modelling hypotheses
    throw_if(!this->hypotheses.empty(),
             "supported modelling hypotheses have already been declared");
    // check that if a specialised version of the behaviour
    // is defined, it is present in the set of hypotheses defined here
    for (const auto& ld : this->sd) {
      throw_if(mhs.find(ld.first) == mhs.end(),
               "partial specialisation of the behaviour exists for "
               "the hypothesis '" +
                   ModellingHypothesis::toString(ld.first) +
                   "' "
                   "which is not in the set of hypotheses which have to be "
                   "supported by the behaviour.");
    }
    for (const auto h : this->requestedHypotheses) {
      throw_if(mhs.find(h) == mhs.end(),
               "a description of the behaviour for "
               "the hypothesis '" +
                   ModellingHypothesis::toString(h) +
                   "' "
                   "has been requested earlier, but this hypothesis is not "
                   "in the set of hypotheses which will to be "
                   "supported by the behaviour. This may lead to "
                   "inconsistencies. "
                   "Cowardly aborting.");
    }
    if (this->hypotheses.empty()) {
      if ((this->stypeIsDefined) &&
          (this->getSymmetryType() == mfront::ORTHOTROPIC) &&
          (this->oacIsDefined) &&
          (this->getOrthotropicAxesConvention() ==
           OrthotropicAxesConvention::PLATE)) {
        for (const auto h : mhs) {
          throw_if(((h != ModellingHypothesis::TRIDIMENSIONAL) &&
                    (h != ModellingHypothesis::PLANESTRESS) &&
                    (h != ModellingHypothesis::PLANESTRAIN) &&
                    (h != ModellingHypothesis::GENERALISEDPLANESTRAIN)),
                   "Modelling hypothesis '" + ModellingHypothesis::toString(h) +
                       "' is not compatible with the `Plate` orthotropic axes "
                       "convention");
        }
      }
      update_hypotheses(mhs);
    } else {
      if (b) {
        // find the intersection of the given hypotheses and the
        // existing one
        std::set<Hypothesis> nmhs;
        for (const auto h : this->hypotheses) {
          if (mhs.find(h) != mhs.end()) {
            nmhs.insert(h);
          }
        }
        throw_if(nmhs.empty(),
                 "intersection of previously modelling hypotheses "
                 "with the new ones is empty");
        // as this is the intersection with previously defined
        // hyppotheses, restrictions related to the orthotropic axes
        // conditions does not have to be checked.
        update_hypotheses(nmhs);
      } else {
        throw_if(true,
                 "supported modelling hypotheses have already been declared");
      }
    }
  }  // end of setModellingHypotheses

  const std::vector<ModelDescription>&
  BehaviourDescription::getModelsDescriptions() const {
    return this->models;
  }  // end of getModelsDescriptions

  void BehaviourDescription::addModelDescription(const ModelDescription& md) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    for (auto ov : md.outputs) {
      VariableDescription dov{ov.type, "d" + ov.name, ov.arraySize,
                              ov.lineNumber};
      ov.setAttribute("ComputedByExternalModel", true, false);
      this->addAuxiliaryStateVariable(uh, ov, BehaviourData::UNREGISTRED);
      this->addLocalVariable(uh, dov, BehaviourData::UNREGISTRED);
    }
    this->models.push_back(md);
  }  // end of addModelDescription

  void BehaviourDescription::addMaterialProperties(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addMaterialProperty;
    this->addVariables(h, v, s, f);
  }  // end of addMaterialProperties

  void BehaviourDescription::addParameters(
      const Hypothesis h,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s) {
    using mptr = void (mfront::BehaviourData::*)(
        const mfront::VariableDescription&,
        const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addParameter;
    this->addVariables(h, v, s, f);
  }  // end of addParameters

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

  bool
  BehaviourDescription::isTemperatureDefinedAsTheFirstExternalStateVariable()
      const {
    const auto mhs = this->getDistinctModellingHypotheses();
    for (const auto h : mhs) {
      const auto& esvs = this->getBehaviourData(h).getExternalStateVariables();
      if (esvs.empty()) {
        return false;
      }
      if (esvs[0].getExternalName() != tfel::glossary::Glossary::Temperature) {
        return false;
      }
      if (SupportedTypes::getTypeFlag(esvs[0].type) != SupportedTypes::SCALAR) {
        return false;
      }
      if ((esvs[0].type != "temperature") && (esvs[0].type != "real")) {
        return false;
      }
    }
    return true;
  }  // end of isTemperatureDefinedAsTheFirstExternalStateVariable

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

  void BehaviourDescription::addInitializeFunctionVariables(
      const Hypothesis h, const VariableDescriptionContainer& v) {
    for (const auto& ppv : v) {
      this->addInitializeFunctionVariable(h, ppv);
    }
  }  // end of addInitializeFunctionVariables

  void BehaviourDescription::addInitializeFunctionVariable(
      const Hypothesis h, const VariableDescription& v) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->d.addInitializeFunctionVariable(v);
      for (auto& md : this->sd) {
        md.second->addInitializeFunctionVariable(v);
      }
    } else {
      this->getBehaviourData2(h).addInitializeFunctionVariable(v);
    }
  }  // end of addInitializeFunctionVariable

  void BehaviourDescription::addPostProcessingVariables(
      const Hypothesis h, const VariableDescriptionContainer& v) {
    for (const auto& ppv : v) {
      this->addPostProcessingVariable(h, ppv);
    }
  }  // end of addPostProcessingVariables

  void BehaviourDescription::addPostProcessingVariable(
      const Hypothesis h, const VariableDescription& v) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->d.addPostProcessingVariable(v);
      for (auto& md : this->sd) {
        md.second->addPostProcessingVariable(v);
      }
    } else {
      this->getBehaviourData2(h).addPostProcessingVariable(v);
    }
  }  // end of addPostProcessingVariable

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
  }  // end of setVariableAttribute

  bool BehaviourDescription::hasGlossaryName(const Hypothesis h,
                                             const std::string& v) const {
    return this->getData(h, &BehaviourData::hasGlossaryName, v);
  }  // end of hasGlossaryName

  bool BehaviourDescription::hasEntryName(const Hypothesis h,
                                          const std::string& v) const {
    return this->getData(h, &BehaviourData::hasEntryName, v);
  }  // end of hasEntryName

  bool BehaviourDescription::hasParameter(const Hypothesis h,
                                          const std::string& v) const {
    return this->getData(h, &BehaviourData::hasParameter, v);
  }  // end of hasParameter

  bool BehaviourDescription::hasParameters(const Hypothesis h) const {
    return this->getBehaviourData(h).hasParameters();
  }  // end of hasParameters

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
  }  // end of hasParameters

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
  }  // end of getUnsignedShortParameterDefaultValue

  int BehaviourDescription::getIntegerParameterDefaultValue(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::getIntegerParameterDefaultValue, n);
  }  // end of getIntegerParameterDefaultValue

  double BehaviourDescription::getFloattingPointParameterDefaultValue(
      const Hypothesis h, const std::string& n) const {
    return this->getData(
        h, &BehaviourData::getFloattingPointParameterDefaultValue, n);
  }  // end of getFloattingPointParameterDefaultValue

  double BehaviourDescription::getFloattingPointParameterDefaultValue(
      const Hypothesis h, const std::string& n, const unsigned short i) const {
    return this->getBehaviourData(h).getFloattingPointParameterDefaultValue(n,
                                                                            i);
  }  // end of getFloattingPointParameterDefaultValue

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
  }  // end of addStaticVariable

  std::map<std::string, int> BehaviourDescription::getIntegerConstants(
      const Hypothesis h) const {
    return this->getData(h, &BehaviourData::getIntegerConstants);
  }  // end of getIntegerConstant

  int BehaviourDescription::getIntegerConstant(const Hypothesis h,
                                               const std::string& n) const {
    return this->getData(h, &BehaviourData::getIntegerConstant, n);
  }  // end of getIntegerConstant

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
  }  // end of addVariables

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
  }  // end of addVariable

  void BehaviourDescription::addVariables(
      BehaviourData& b,
      const VariableDescriptionContainer& v,
      const BehaviourData::RegistrationStatus s,
      void (BehaviourData::*m)(const VariableDescription&,
                               const BehaviourData::RegistrationStatus)) {
    for (const auto& e : v) {
      this->addVariable(b, e, s, m);
    }
  }  // end of addVariables

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
  }  // end of areAllMechanicalDataSpecialised

  bool BehaviourDescription::areAllMechanicalDataSpecialised(
      const std::set<Hypothesis>& h) const {
    for (const auto& mh : h) {
      if (!this->hasSpecialisedMechanicalData(mh)) {
        return false;
      }
    }
    return true;
  }  // end of areAllMechanicalDataSpecialised

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
    auto update = [this, &b1, &b2](const auto& variables) {
      const auto& flags = SupportedTypes::getTypeFlags();
      for (const auto& v : variables) {
        const auto pf = flags.find(v.type);
        if (pf != flags.end()) {
          if (pf->second == SupportedTypes::TVECTOR) {
            b1 = true;
          }
        }
        if (v.arraySize > 1) {
          if (this->useDynamicallyAllocatedVector(v.arraySize)) {
            b2 = true;
          } else {
            b1 = true;
          }
        }
      }
    };
    update(bd.getMaterialProperties());
    update(bd.getIntegrationVariables());
    update(bd.getStateVariables());
    update(bd.getAuxiliaryStateVariables());
    update(bd.getLocalVariables());
    update(bd.getExternalStateVariables());
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
  }  // end of requiresTVectorOrVectorIncludes

  void BehaviourDescription::
      declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          const Hypothesis h, const std::string& n) {
    void (BehaviourData::*m)(const std::string&) =
        &BehaviourData::
            declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution;
    this->callBehaviourData(h, m, n, true);
  }  // end of
     // declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  void BehaviourDescription::setUsableInPurelyImplicitResolution(
      const Hypothesis h, const bool b) {
    void (BehaviourData::*m)(const bool) =
        &BehaviourData::setUsableInPurelyImplicitResolution;
    this->callBehaviourData(h, m, b, true);
  }  // end of
     // declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  bool BehaviourDescription::isMemberUsedInCodeBlocks(
      const Hypothesis h, const std::string& v) const {
    return this->getData(h, &BehaviourData::isMemberUsedInCodeBlocks, v);
  }  // end of isMaterialPropertyName

  bool BehaviourDescription::isMaterialPropertyName(
      const Hypothesis h, const std::string& v) const {
    return this->getData(h, &BehaviourData::isMaterialPropertyName, v);
  }  // end of isMaterialPropertyName

  bool BehaviourDescription::isLocalVariableName(const Hypothesis h,
                                                 const std::string& n) const {
    return this->getData(h, &BehaviourData::isLocalVariableName, n);
  }  // end of isLocalVariableName

  bool BehaviourDescription::isPersistentVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isPersistentVariableName, n);
  }  // end of isPersistentVariableName

  bool BehaviourDescription::isIntegrationVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isIntegrationVariableName, n);
  }  // end of isIntegrationVariableName

  bool BehaviourDescription::isIntegrationVariableIncrementName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isIntegrationVariableIncrementName,
                         n);
  }  // end of isIntegrationVariableIncrementName

  bool BehaviourDescription::isStateVariableName(const Hypothesis h,
                                                 const std::string& n) const {
    return this->getData(h, &BehaviourData::isStateVariableName, n);
  }  // end of isStateVariableName

  bool BehaviourDescription::isStateVariableIncrementName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isStateVariableIncrementName, n);
  }  // end of isStateVariableIncrementName

  bool BehaviourDescription::isAuxiliaryStateVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isAuxiliaryStateVariableName, n);
  }  // end of isAuxiliaryStateVariableName

  bool BehaviourDescription::isExternalStateVariableName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::isExternalStateVariableName, n);
  }  // end of isExternalStateVariableName

  bool BehaviourDescription::isExternalStateVariableIncrementName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(
        h, &BehaviourData::isExternalStateVariableIncrementName, n);
  }  // end of isExternalStateVariableIncrementName

  bool BehaviourDescription::isParameterName(const Hypothesis h,
                                             const std::string& v) const {
    return this->getData(h, &BehaviourData::isParameterName, v);
  }  // end of isParameterName

  bool BehaviourDescription::isInitializeFunctionVariableName(
      const Hypothesis h, const std::string& v) const {
    return this->getData(h, &BehaviourData::isInitializeFunctionVariableName,
                         v);
  }  // end of isInitializeFunctionVariableName

  bool BehaviourDescription::isPostProcessingVariableName(
      const Hypothesis h, const std::string& v) const {
    return this->getData(h, &BehaviourData::isPostProcessingVariableName, v);
  }  // end of isPostProcessingVariableName

  bool BehaviourDescription::isStaticVariableName(const Hypothesis h,
                                                  const std::string& n) const {
    return this->getData(h, &BehaviourData::isStaticVariableName, n);
  }  // end of isStaticVariableName

  void BehaviourDescription::updateClassName() {
    if ((!this->behaviour.empty()) || (!this->material.empty())) {
      this->className = this->material + this->behaviour;
    }
  }  // end of updateClassName

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
  }  // end of setCode

  const CodeBlock& BehaviourDescription::getCodeBlock(
      const Hypothesis h, const std::string& n) const {
    return this->getBehaviourData(h).getCodeBlock(n);
  }  // end of getCode

  std::string BehaviourDescription::getCode(const Hypothesis h,
                                            const std::string& n) const {
    const auto b = this->getAttribute(BehaviourData::profiling, false);
    return this->getBehaviourData(h).getCode(n, this->getClassName(), b);
  }  // end of getCode

  bool BehaviourDescription::hasCode(const Hypothesis h,
                                     const std::string& n) const {
    return this->getBehaviourData(h).hasCode(n);
  }  // end of getCode

  void BehaviourDescription::setAttribute(const Hypothesis h,
                                          const std::string& n,
                                          const MaterialKnowledgeAttribute& a,
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
  }  // end of setAttribute

  void BehaviourDescription::updateAttribute(
      const Hypothesis h,
      const std::string& n,
      const MaterialKnowledgeAttribute& a) {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      this->d.updateAttribute(n, a);
      for (const auto& md : this->sd) {
        md.second->updateAttribute(n, a);
      }
    } else {
      this->getBehaviourData2(h).updateAttribute(n, a);
    }
  }  // end of updateAttribute

  bool BehaviourDescription::hasAttribute(const Hypothesis h,
                                          const std::string& n) const {
    return this->getBehaviourData(h).hasAttribute(n);
  }  // end of hasAttribute

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
  }  // end of getExternalNames

  void BehaviourDescription::appendExternalNames(std::vector<std::string>& n,
                                                 const Hypothesis h,
                                                 const VarContainer& v) const {
    return this->getBehaviourData(h).appendExternalNames(n, v);
  }  // end of appendExternalNames

  void BehaviourDescription::setGlossaryName(const std::string& n,
                                             const std::string& gn) {
    using tfel::glossary::Glossary;
    const auto& glossary = Glossary::getGlossary();
    tfel::raise_if(!glossary.contains(gn),
                   "BehaviourDescription::setGlossaryName: "
                   "'" +
                       gn + "' is not a glossary name");
    for (auto& v : this->mvariables) {
      constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (v.first.name == n) {
        v.first.setGlossaryName(gn);
        this->registerGlossaryName(h, n, gn);
      }
      if (v.second.name == n) {
        v.second.setGlossaryName(gn);
        this->registerGlossaryName(h, n, gn);
      }
    }
  }  // end of setGlossaryName

  void BehaviourDescription::setEntryName(const std::string& n,
                                          const std::string& e) {
    using tfel::glossary::Glossary;
    using MainVariable = std::pair<Gradient, ThermodynamicForce>;
    const auto& glossary = Glossary::getGlossary();
    tfel::raise_if(glossary.contains(e),
                   "BehaviourDescription::setEntryName: "
                   "'" +
                       e + "' is a glossary name");
    std::for_each(this->mvariables.begin(), this->mvariables.end(),
                  [this, &n, &e](MainVariable& v) {
                    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
                    if (v.first.name == n) {
                      v.first.setEntryName(e);
                      this->registerEntryName(h, n, e);
                    }
                    if (v.second.name == n) {
                      v.second.setEntryName(e);
                      this->registerEntryName(h, n, e);
                    }
                  });
  }  // end of setEntryName

  void BehaviourDescription::setGlossaryName(const Hypothesis h,
                                             const std::string& n,
                                             const std::string& g) {
    this->callBehaviourData(h, &BehaviourData::setGlossaryName, n, g, true);
  }  // end of setGlossaryName

  bool BehaviourDescription::isGlossaryNameUsed(const Hypothesis h,
                                                const std::string& n) const {
    return this->getBehaviourData(h).isGlossaryNameUsed(n);
  }  // end of isGlossaryName

  void BehaviourDescription::setEntryName(const Hypothesis h,
                                          const std::string& n,
                                          const std::string& g) {
    this->callBehaviourData(h, &BehaviourData::setEntryName, n, g, true);
  }  // end of setEntryName

  bool BehaviourDescription::isUsedAsEntryName(const Hypothesis h,
                                               const std::string& n) const {
    return this->getBehaviourData(h).isUsedAsEntryName(n);
  }  // end of isEntryName

  std::string BehaviourDescription::getExternalName(
      const Hypothesis h, const std::string& n) const {
    return this->getData(h, &BehaviourData::getExternalName, n);
  }  // end of getGlossaryName

  std::string BehaviourDescription::getVariableNameFromGlossaryNameOrEntryName(
      const Hypothesis h, const std::string& n) const {
    return this->getBehaviourData(h).getVariableNameFromGlossaryNameOrEntryName(
        n);
  }  // end of getVariableNameFromGlossaryNameOrEntryName

  const VariableDescription&
  BehaviourDescription::getVariableDescriptionByExternalName(
      const Hypothesis h, const std::string& n) const {
    return this->getBehaviourData(h).getVariableDescriptionByExternalName(n);
  }  // end of getVariableDescriptionByExternalName

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
  }  // end of setBounds

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
  }  // end of setBounds

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
  }  // end of setPhysicalBounds

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
  }  // end of setPhysicalBounds

  void BehaviourDescription::disableQuantitiesUsageIfNotAlreadySet() const {
    if (!this->use_qt.has_value()) {
      this->use_qt = false;
    }
  }  // end of disableQuantitiesUsageIfNotAlreadySet

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
  }  // end of isNameReserved

  void BehaviourDescription::registerGlossaryName(const Hypothesis h,
                                                  const std::string& n,
                                                  const std::string& g) {
    this->callBehaviourData(h, &BehaviourData::registerGlossaryName, n, g,
                            true);
  }  // end of registerGlossaryName

  void BehaviourDescription::registerEntryName(const Hypothesis h,
                                               const std::string& n,
                                               const std::string& e) {
    this->callBehaviourData(h, &BehaviourData::registerEntryName, n, e, true);
  }  // end of registerEntryName

  void BehaviourDescription::registerMemberName(const Hypothesis h,
                                                const std::string& n) {
    this->callBehaviourData(h, &BehaviourData::registerMemberName, n, true);
  }  // end of registerMemberName

  void BehaviourDescription::registerStaticMemberName(const Hypothesis h,
                                                      const std::string& n) {
    this->callBehaviourData(h, &BehaviourData::registerStaticMemberName, n,
                            true);
  }  // end of registerMemberName

  void BehaviourDescription::addMaterialLaw(const std::string& m) {
    if (std::find(this->materialLaws.begin(), this->materialLaws.end(), m) ==
        this->materialLaws.end()) {
      this->materialLaws.push_back(m);
    }
  }  // end of getMaterialLaws

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
  }  // end of checkVariableGlossaryName

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
  }  // end of checkVariableEntryName

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
  }  // end of checkVariablePosition

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
    tfel::raise_if(this->getBehaviourType() !=
                       BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                   "BehaviourDescription::setStrainMeasure: "
                   "invalid behaviour type");
    tfel::raise_if(this->isStrainMeasureDefined(),
                   "BehaviourDescription::setStrainMeasure: "
                   "strain measure already defined");
    this->strainMeasure = sm;
  }  // end of setStrainMeasure

  void BehaviourDescription::setSaveStrainMeasure(const bool b) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    tfel::raise_if(this->getBehaviourType() !=
                       BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                   "BehaviourDescription::setSaveStrainMeasure: "
                   "invalid behaviour type");
    tfel::raise_if(this->saveStrainMeasureValue.has_value(),
                   "BehaviourDescription::setSaveStrainMeasure: "
                   "this method has already been called");
    this->saveStrainMeasureValue = b;
    auto v =
        VariableDescription("StrainStensor", "saved_strain_measure", 1u, 0u);
    v.setGlossaryName(tfel::glossary::Glossary::StrainMeasure);
    this->addAuxiliaryStateVariable(uh, v, BehaviourData::UNREGISTRED);
    CodeBlock save_strain;
    save_strain.code +=
        "this->saved_strain_measure = this->eto + this->deto;\n";
    this->setCode(uh, BehaviourData::UpdateAuxiliaryStateVariables, save_strain,
                  BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }  // end of setSaveStrainMeasure

  void BehaviourDescription::setSaveDualStress(const bool b) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    tfel::raise_if(this->getBehaviourType() !=
                       BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                   "BehaviourDescription::setSaveStrainMeasure: "
                   "invalid behaviour type");
    tfel::raise_if(this->saveDualStressValue.has_value(),
                   "BehaviourDescription::setSaveDualStress: "
                   "this method has already been called");
    this->saveDualStressValue = b;
    auto v = VariableDescription("StressStensor", "saved_dual_stress", 1u, 0u);
    v.setGlossaryName(tfel::glossary::Glossary::DualStress);
    this->addAuxiliaryStateVariable(uh, v, BehaviourData::UNREGISTRED);
    CodeBlock save_stress;
    save_stress.code += "this->saved_dual_stress = this->sig;\n";
    this->setCode(uh, BehaviourData::UpdateAuxiliaryStateVariables, save_stress,
                  BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }  // end of setSaveDualStress

  BehaviourDescription::StrainMeasure BehaviourDescription::getStrainMeasure()
      const {
    tfel::raise_if(!this->isStrainMeasureDefined(),
                   "BehaviourDescription::getStrainMeasure: "
                   "no strain measure defined");
    return this->strainMeasure.value();
  }  // end of setStrainMeasure()

  bool BehaviourDescription::isStrainMeasureDefined() const {
    return this->strainMeasure.has_value();
  }  // end of isStrainMeasureDefined()

  bool BehaviourDescription::saveStrainMeasure() const {
    if (!this->saveStrainMeasureValue.has_value()) {
      return false;
    }
    return this->saveStrainMeasureValue.value();
  }  // end of saveStrainMeasure

  bool BehaviourDescription::saveDualStress() const {
    if (!this->saveDualStressValue.has_value()) {
      return false;
    }
    return this->saveDualStressValue.value();
  }  // end of saveDualStress

  void BehaviourDescription::getSymbols(
      std::map<std::string, std::string>& symbols, const Hypothesis h) const {
    for (const auto& mv : this->mvariables) {
      getSymbol(symbols, mv.first);
      getSymbol(symbols, mv.second);
    }
    for (const auto& b : this->getTangentOperatorBlocks()) {
      if ((Gradient::isIncrementKnown(b.second)) ||
          (this->isExternalStateVariableName(h, b.second.name))) {
        symbols.insert({"\u2202" + displayName(b.first) + "\u2215\u2202\u0394" +
                            displayName(b.second),
                        "d" + b.first.name + "_dd" + b.second.name});
      } else {
        symbols.insert({"\u2202" + displayName(b.first) + "\u2215\u2202" +
                            displayName(b.second) + "\u2081",
                        "d" + b.first.name + "_d" + b.second.name + "1"});
      }
    }
    this->getBehaviourData(h).getSymbols(symbols);
  }  // end of getSymbols

  std::vector<MaterialPropertyDescription>
  BehaviourDescription::getElasticMaterialPropertiesDescriptions() const {
    const auto& emps = this->getElasticMaterialProperties();
    auto empds = std::vector<MaterialPropertyDescription>{emps.size()};
    if (emps.size() == 2u) {
      empds[0] =
          buildMaterialPropertyDescription(emps[0], *this, "YoungModulus");
      empds[1] =
          buildMaterialPropertyDescription(emps[1], *this, "PoissonRatio");
    } else if (emps.size() == 9u) {
      empds[0] =
          buildMaterialPropertyDescription(emps[0], *this, "YoungModulus1");
      empds[1] =
          buildMaterialPropertyDescription(emps[1], *this, "YoungModulus2");
      empds[2] =
          buildMaterialPropertyDescription(emps[2], *this, "YoungModulus3");
      empds[3] =
          buildMaterialPropertyDescription(emps[3], *this, "PoissonRatio12");
      empds[4] =
          buildMaterialPropertyDescription(emps[4], *this, "PoissonRatio23");
      empds[5] =
          buildMaterialPropertyDescription(emps[5], *this, "PoissonRatio13");
      empds[6] =
          buildMaterialPropertyDescription(emps[6], *this, "ShearModulus12");
      empds[7] =
          buildMaterialPropertyDescription(emps[7], *this, "ShearModulus23");
      empds[8] =
          buildMaterialPropertyDescription(emps[8], *this, "ShearModulus13");
    } else {
      tfel::raise(
          "BehaviourDescription::getElasticMaterialPropertiesDescriptions: "
          "invalid number of material properties");
    }
    return empds;
  }  // end of getElasticMaterialPropertiesDescriptions

  std::vector<MaterialPropertyDescription>
  BehaviourDescription::getThermalExpansionCoefficientsDescriptions() const {
    const auto& themps = this->getThermalExpansionCoefficients();
    auto thempds = std::vector<MaterialPropertyDescription>{themps.size()};
    if (themps.size() == 1u) {
      thempds[0] = buildMaterialPropertyDescription(themps[0], *this,
                                                    "ThermalExpansion");
    } else if (themps.size() == 3u) {
      thempds[0] = buildMaterialPropertyDescription(themps[0], *this,
                                                    "ThermalExpansion1");
      thempds[1] = buildMaterialPropertyDescription(themps[1], *this,
                                                    "ThermalExpansion2");
      thempds[2] = buildMaterialPropertyDescription(themps[2], *this,
                                                    "ThermalExpansion3");
    } else {
      tfel::raise(
          "BehaviourDescription::getThermalExpansionCoefficientsDescriptions:"
          " "
          "invalid number of material properties");
    }
    return thempds;
  }  // end of getThermalExpansionCoefficientsDescriptions

  void BehaviourDescription::overrideByAParameter(const std::string& n,
                                                  const double v) {
    this->d.overrideByAParameter(n, v);
    for (auto& md : this->sd) {
      md.second->overrideByAParameter(n, v);
    }
  }  // end of overrideByAParameter

  std::map<std::string, double> BehaviourDescription::getOverridenParameters()
      const {
    auto parameters = this->d.getOverridenParameters();
    for (auto& md : this->sd) {
      const auto params = md.second->getOverridenParameters();
      parameters.insert(params.begin(), params.end());
    }
    return parameters;
  }  // end of getOverridenParameters

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

  std::string getParametersFileName(const BehaviourDescription& bd) {
    return bd.getClassName() + "-parameters.txt";
  }  // end of getParametersFileName

  std::string getParametersFileName(const BehaviourDescription& bd,
                                    const BehaviourDescription::Hypothesis h) {
    const auto hn = BehaviourDescription::ModellingHypothesis::toString(h);
    return bd.getClassName() + hn + "-parameters.txt";
  }  // end of getParametersFileName

}  // end of namespace mfront
