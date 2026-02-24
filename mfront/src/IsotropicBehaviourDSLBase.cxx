/*!
 * \file   mfront/src/IsotropicBehaviourDSLBase.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <vector>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontWarningMode.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRuleFactory.hxx"
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  const char* const IsotropicBehaviourDSLBase::useStressUpdateAlgorithm =
      "use_stress_update_algorithm";

  tfel::utilities::DataMapValidator
  IsotropicBehaviourDSLBase::getDSLOptionsValidator() {
    auto validator =
        BehaviourDSLBase<IsotropicBehaviourDSLBase>::getDSLOptionsValidator();
    validator.addDataTypeValidator<bool>(
        IsotropicBehaviourDSLBase::useStressUpdateAlgorithm);
    return validator;
  }

  IsotropicBehaviourDSLBase::IsotropicBehaviourDSLBase(const DSLOptions& opts)
      : BehaviourDSLBase<IsotropicBehaviourDSLBase>(tfel::utilities::remove(
            opts, {IsotropicBehaviourDSLBase::useStressUpdateAlgorithm})) {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->reserveName("NewtonIntegration");
    // main variables
    this->mb.declareAsASmallStrainStandardBehaviour();
    // intermediate temperature
    const auto* const Topt = BehaviourDescription::
        automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable;
    if (this->mb.getAttribute<bool>(Topt)) {
      // temperature at the midle of the time step
      const auto T_ = VariableDescription("temperature", "T_", 1u, 0u);
      this->mb.addLocalVariable(h, T_);
    }
    this->mb.setAttribute(
        IsotropicBehaviourDSLBase::useStressUpdateAlgorithm,
        get_if(opts, IsotropicBehaviourDSLBase::useStressUpdateAlgorithm,
               false),
        true);
    if (!this->mb.getAttribute(
            IsotropicBehaviourDSLBase::useStressUpdateAlgorithm, false)) {
      this->mb.addStateVariable(
          h, VariableDescription("StrainStensor", "εᵉˡ", "eel", 1u, 0u));
      this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    }
    // material symmetry
    this->mb.setSymmetryType(mfront::ISOTROPIC);
    this->mb.setElasticSymmetryType(mfront::ISOTROPIC);
    // parameters
    this->mb.registerMemberName(h, "epsilon");
    this->mb.registerMemberName(h, "\u03B5");
    this->mb.registerMemberName(h, "theta");
    this->mb.registerMemberName(h, "\u03B8");
    this->mb.registerMemberName(h, "iterMax");
    // Call Back
    this->registerNewCallBack(
        "@UsableInPurelyImplicitResolution",
        &IsotropicBehaviourDSLBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",
                              &IsotropicBehaviourDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@FlowRule",
                              &IsotropicBehaviourDSLBase::treatFlowRule);
    this->registerNewCallBack("@Theta", &IsotropicBehaviourDSLBase::treatTheta);
    this->registerNewCallBack("@Epsilon",
                              &IsotropicBehaviourDSLBase::treatEpsilon);
    this->registerNewCallBack("@IterMax",
                              &IsotropicBehaviourDSLBase::treatIterMax);
    this->registerNewCallBack(
        "@ElasticMaterialProperties",
        &IsotropicBehaviourDSLBase::treatElasticMaterialProperties);
    this->registerNewCallBack(
        "@IsotropicHardeningRule",
        &IsotropicBehaviourDSLBase::treatIsotropicHardeningRule);
    this->registerNewCallBack(
        "@IsotropicHardeningRules",
        &IsotropicBehaviourDSLBase::treatIsotropicHardeningRules);
    this->disableCallBack("@Brick");
    this->disableCallBack("@StateVar");
    this->disableCallBack("@StateVariable");
    this->disableCallBack("@Integrator");
    this->disableCallBack("@OrthotropicBehaviour");
    this->disableCallBack("@IsotropicElasticBehaviour");
    this->disableCallBack("@IsotropicBehaviour");
    this->disableCallBack("@RequireStiffnessOperator");
    this->disableCallBack("@RequireThermalExpansionCoefficientTensor");
    this->disableCallBack("@CrystalStructure");
    this->disableCallBack("@SlipSystem");
    this->disableCallBack("@GlidingSystem");
    this->disableCallBack("@SlidingSystem");
    this->disableCallBack("@SlipSystems");
    this->disableCallBack("@GlidingSystems");
    this->disableCallBack("@SlidingSystems");
    this->disableCallBack("@InteractionMatrix");
    this->registerNewCallBack("@Predictor",
                              &IsotropicBehaviourDSLBase::treatPredictor);
    // a defaut version of the prediction operator is always provided
    this->mb.setAttribute(h, BehaviourData::hasPredictionOperator, true);
    this->mb.setIntegrationScheme(BehaviourDescription::SPECIFICSCHEME);
  }  // end of IsotropicBehaviourDSLBase

  BehaviourDSLDescription
  IsotropicBehaviourDSLBase::getBehaviourDSLDescription() const {
    auto d = BehaviourDSLDescription{};
    d.behaviourType =
        tfel::material::MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR;
    d.integrationScheme = IntegrationScheme::SPECIFICSCHEME;
    d.typicalCodeBlocks = {BehaviourData::FlowRule};
    d.supportedModellingHypotheses = {
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
        ModellingHypothesis::AXISYMMETRICAL, ModellingHypothesis::PLANESTRAIN,
        ModellingHypothesis::GENERALISEDPLANESTRAIN,
        ModellingHypothesis::TRIDIMENSIONAL};
    d.supportedBehaviourSymmetries = {mfront::ISOTROPIC};
    d.allowUserDefinedStateVariables = false;
    d.allowUserDefinedIntegrationVariables = false;
    d.allowCrystalStructureDefinition = false;
    d.allowStiffnessTensorDefinition = false;
    d.minimalMFrontFileBody = "@FlowRule{}\n\n";
    return d;
  }  // end of getBehaviourDSLDescription

  void IsotropicBehaviourDSLBase::treatPredictor() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputePredictor, m, true, true);
  }  // end of treatPredictor

  bool IsotropicBehaviourDSLBase::handleStrainHardening() const { return true; }

  void IsotropicBehaviourDSLBase::getSymbols(
      std::map<std::string, std::string>& symbols,
      const Hypothesis h,
      const std::string& n) {
    BehaviourDSLCommon::getSymbols(symbols, h, n);
    const auto& d = this->mb.getBehaviourData(h);
    for (const auto& v : d.getIntegrationVariables()) {
      getIncrementSymbol(symbols, v);
    }
    for (const auto& mv : this->mb.getMainVariables()) {
      if (Gradient::isIncrementKnown(mv.first)) {
        getIncrementSymbol(symbols, mv.first);
      }
    }
    mfront::getIncrementSymbols(symbols, d.getExternalStateVariables());
    mfront::addSymbol(symbols, "\u0394t", "dt");
  }  // end of getSymbols

  double IsotropicBehaviourDSLBase::getDefaultThetaValue() const { return 0.5; }

  void IsotropicBehaviourDSLBase::treatTheta() {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("IsotropicBehaviourDSLBase::treatTheta",
                            "Cannot read theta value.");
    const auto v = tfel::utilities::convert<double>(this->current->value);
    if ((v < 0.) || (v > 1.)) {
      this->throwRuntimeError(
          "IsotropicBehaviourDSLBase::treatTheta",
          "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatTheta", ";");
    this->mb.addParameter(
        h, VariableDescription("real", "\u03B8", "theta", 1u, 0u),
        BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "theta", v);
  }  // end of treatTheta

  void IsotropicBehaviourDSLBase::treatEpsilon() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto safe = this->readSafeOptionTypeIfPresent();
    this->checkNotEndOfFile("IsotropicBehaviourDSLBase::treatEpsilon",
                            "cannot read epsilon value");
    const auto epsilon = tfel::utilities::convert<double>(this->current->value);
    if (epsilon < 0) {
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatEpsilon",
                              "Epsilon value must be positive");
    }
    if ((!safe) && (epsilon < 1e-18)) {
      auto converter = std::ostringstream{};
      converter << std::scientific << epsilon;
      reportWarning(
          "the choosen default value for the convergence threshold could be "
          "too stringent (" +
          converter.str() +
          "). You may want to consider a more stringent value (1e-14 "
          "is generally a good choice).");
    }
    if ((!safe) && (epsilon > 1e-12)) {
      auto converter = std::ostringstream{};
      converter << std::scientific << epsilon;
      reportWarning(
          "the choosen default value for the convergence threshold could be "
          "too loose (" +
          converter.str() +
          "). You may want to consider a more stringent value (1e-14 "
          "is generally a good choice).");
    }
    ++(this->current);
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatEpsilon", ";");
    this->mb.addParameter(
        h, VariableDescription("real", "\u03B5", "epsilon", 1u, 0u),
        BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "epsilon", epsilon);
  }  // IsotropicBehaviourDSLBase::treatEpsilon

  void IsotropicBehaviourDSLBase::treatIterMax() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto iterMax =
        this->readUnsignedShort("IsotropicBehaviourDSLBase::treatIterMax");
    if (iterMax == 0) {
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatIterMax",
                              "invalid value for parameter 'iterMax'");
    }
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatIterMax", ";");
    this->mb.addParameter(h, VariableDescription("ushort", "iterMax", 1u, 0u),
                          BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "iterMax", iterMax);
  }  // end of treatIterMax

  std::string IsotropicBehaviourDSLBase::flowRuleVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((d.isExternalStateVariableName(var)) || (d.isStateVariableName(var))) {
      if (addThisPtr) {
        return "this->" + var + "_";
      } else {
        return var + "_";
      }
    }
    if (d.isAuxiliaryStateVariableName(var)) {
      const auto& v = d.getAuxiliaryStateVariables().getVariable(var);
      if (v.getAttribute<bool>("ComputedByExternalModel", false)) {
        if (addThisPtr) {
          return "(this->" + var + " + (this->theta) * (this->d" + var + "))";
        } else {
          return "(" + var + " + (this->theta) * d" + var + ")";
        }
      }
    }
    if ((d.isExternalStateVariableIncrementName(var)) || (var == "dT")) {
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          h, var.substr(1));
    }
    if (addThisPtr) {
      return "this->" + var;
    }
    return var;
  }  // end of flowRuleVariableModifier

  void IsotropicBehaviourDSLBase::checkFlowRule(
      std::string_view n,
      const std::size_t id,
      const bool is_df_dp_required) const {
    auto warnings = std::vector<std::string>{};
    for (const auto h : this->mb.getDistinctModellingHypotheses()) {
      auto report = [&warnings](const std::string& msg) {
        warnings.push_back(
            msg + ". This warning can be disabled by using the <safe> option.");
      };
      auto report_unexpected = [&report, &n](std::string_view v) {
        report("using " + std::string{v} + " in the body of the '" +
               std::string{n} +
               "' code block is unexpected and can be a mistake");
      };
      const auto& d = this->mb.getBehaviourData(h);
      const auto& c = d.getCodeBlock(std::string{n});
      if (isSafe(c)) {
        continue;
      }
      for (const auto& m : c.members) {
        if (m == "theta") {
          report_unexpected("the 'theta' parameter");
        }
        if (m == "iterMax") {
          report_unexpected("the 'iterMax' parameter");
        }
        if (m == "dt") {
          report_unexpected("the time increment 'dt'");
        }
        if (d.isAuxiliaryStateVariableName(m)) {
          const auto& v = d.getAuxiliaryStateVariables().getVariable(m);
          if (!v.getAttribute<bool>("ComputedByExternalModel", false)) {
            report_unexpected("variable '" + m + "'");
          }
        }
        if ((this->mb.isGradientName(m)) ||
            (this->mb.isGradientIncrementName(m)) ||
            (this->mb.isThermodynamicForceName(m)) ||
            (d.isIntegrationVariableIncrementName(m))) {
          report_unexpected("variable '" + m + "'");
        }
      }  // end of for(const auto& m : c.members)

      if (!c.block_variables.contains("f")) {
        report("'f' is not used");
      }
      if (!c.block_variables.contains("df_dseq")) {
        report("'df_dseq' is not used");
      }
      if (is_df_dp_required) {
        if (!c.block_variables.contains("df_dp")) {
          report("'df_dp' is not used");
        }
      }
      if (this->ihrs.contains(id)) {
        if (!c.block_variables.contains("R")) {
          report(
              "the value of the yield surface 'R' "
              "defined by an isotropic hardening rule is not used");
        }
        if (!c.block_variables.contains("dR_dp")) {
          report(
              "the value of the derivative of the yield surface 'dR_dp' "
              "defined by an isotropic hardening rule is not used");
        }
      }
    }
    reportWarning(warnings);
  }  // end of checkFlowRule

  std::function<void(CodeBlock&,
                     const IsotropicBehaviourDSLBase::Hypothesis,
                     const std::string&)>
  IsotropicBehaviourDSLBase::getFlowRuleAnalyser(
      const std::size_t flow_id) const {
    const auto has_ihr = this->ihrs.contains(flow_id);
    return [this, has_ihr](CodeBlock& c, const Hypothesis,
                           const std::string& cv) {
      if ((cv == "f") || (cv == "df_dseq")) {
        c.block_variables.insert(cv);
      }
      if (cv == "df_dp") {
        if (!this->handleStrainHardening()) {
          this->throwRuntimeError("IsotropicBehaviourDSLBase::treatFlowRule",
                                  "the derivative 'df_dp' shall not be used.");
        } else {
          c.block_variables.insert(cv);
        }
      }
      if (has_ihr) {
        if ((cv == "R") || (cv == "dR_dp")) {
          c.block_variables.insert(cv);
        }
      }
    };
  }  // end of getFlowRuleAnalyser

  void IsotropicBehaviourDSLBase::treatFlowRule() {
    auto modifier = [this](const Hypothesis h, const std::string& sv,
                           const bool b) {
      return this->flowRuleVariableModifier(h, sv, b);
    };
    const auto flow_id = [this]() -> std::size_t {
      if (this->allowMultipleFlowRules()) {
        return this->getNumberOfFlowRules();
      }
      return 0;
    }();
    auto analyser = this->getFlowRuleAnalyser(flow_id);
    this->treatCodeBlock(BehaviourData::FlowRule, modifier, analyser, true,
                         false);
    this->checkFlowRule(BehaviourData::FlowRule, flow_id,
                        this->handleStrainHardening());
  }  // end of treatFlowRule

  bool IsotropicBehaviourDSLBase::allowMultipleFlowRules() const {
    return false;
  }  // end of allowMultipleFlowRules

  std::size_t IsotropicBehaviourDSLBase::getNumberOfFlowRules() const {
    return 1;
  }  // end of getNumberOfFlowRules

  static std::size_t extractFlowId(const tfel::utilities::DataMap& opts,
                                   const bool allowMultipleFlowRules) {
    auto extract = [&opts] {
      if (!is<int>(opts, "flow_id")) {
        tfel::raise("the 'flow_id' parameter is not an integer");
      }
      return opts.at("flow_id").get<int>();
    };
    if (!allowMultipleFlowRules) {
      if (opts.contains("flow_id")) {
        const auto id = extract();
        if (id != 0) {
          tfel::raise(
              "the (optional) 'flow_id' parameter shall be egal to zero when "
              "only one flow rule is allowed");
        }
      }
      return 0;
    }
    if (!opts.contains("flow_id")) {
      tfel::raise("no 'flow_id' parameter specified");
    }
    const auto id = extract();
    if (id < 0) {
      tfel::raise("invalid value for the 'flow_id' parameter ('" +
                  std::to_string(id) + "')");
    }
    return static_cast<std::size_t>(id);
  }  // end of extractFlowId

  void IsotropicBehaviourDSLBase::treatIsotropicHardeningRule() {
    const auto name = this->readString(
        "IsotropicBehaviourDSLBase::treatIsotropicHardeningRule");
    this->checkNotEndOfFile(
        "IsotropicBehaviourDSLBase::treatIsotropicHardeningRule");
    const auto opts =
        read<tfel::utilities::DataMap>(this->current, this->tokens.end());
    this->readSpecifiedToken(
        "IsotropicBehaviourDSLBase::treatIsotropicHardeningRule", ";");
    //
    const auto flow_id = extractFlowId(opts, this->allowMultipleFlowRules());
    if (!this->ihrs[flow_id].empty()) {
      this->throwRuntimeError(
          "IsotropicBehaviourDSLBase::treatIsotropicHardeningRule",
          "an isotropic hardening rule has already been defined for flow '" +
              std::to_string(flow_id) + "'");
    }
    //
    auto& rf = ::mfront::bbrick::IsotropicHardeningRuleFactory::getFactory();
    const auto ihr = rf.generate(name);
    if (this->allowMultipleFlowRules()) {
      ihr->initialize(this->mb, *this, std::to_string(flow_id), "",
                      tfel::utilities::remove(opts, {"flow_id"}));
    } else {
      ihr->initialize(this->mb, *this, "", "",
                      tfel::utilities::remove(opts, {"flow_id"}));
    }
    this->ihrs[flow_id].push_back(ihr);
  }  // end of treatIsotropicHardeningRule

  void IsotropicBehaviourDSLBase::treatIsotropicHardeningRules() {
    this->checkNotEndOfFile(
        "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules");
    const auto opts = [this] {
      auto o = tfel::utilities::DataParsingOptions{};
      o.allowMultipleKeysInMap = true;
      const auto data =
          tfel::utilities::Data::read(this->current, this->tokens.end(), o);
      if (data.empty()) {
        return tfel::utilities::DataMap{};
      }
      if (!data.is<tfel::utilities::DataMap>()) {
        this->throwRuntimeError(
            "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules",
            "expected to read a map");
      }
      return data.get<tfel::utilities::DataMap>();
    }();
    this->readSpecifiedToken(
        "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules", ";");
    for (const auto& [k, v] : opts) {
      static_cast<void>(v);
      if ((k != "flow_id") && (k != "isotropic_hardening")) {
        this->throwRuntimeError(
            "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules",
            "unexpected entry '" + k +
                "'. Valid entries are 'flow_id' and 'isotropic_hardening'.");
      }
    }
    if (!opts.contains("isotropic_hardening")) {
      this->throwRuntimeError(
          "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules",
          "no isotropic hardening rule defined.");
    }
    const auto flow_id = extractFlowId(opts, this->allowMultipleFlowRules());
    if (!this->ihrs[flow_id].empty()) {
      this->throwRuntimeError(
          "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules",
          "an isotropic hardening rule has already been defined for flow '" +
              std::to_string(flow_id) + "'");
    }
    auto getDataStructure = [this](const tfel::utilities::Data& ds) {
      if (ds.is<std::string>()) {
        tfel::utilities::DataStructure nds;
        nds.name = ds.get<std::string>();
        return nds;
      }
      if (!ds.is<tfel::utilities::DataStructure>()) {
        this->throwRuntimeError(
            "IsotropicBehaviourDSLBase::treatIsotropicHardeningRules",
            "invalid data type for entry 'isotropic_hardening'");
      }
      return ds.get<tfel::utilities::DataStructure>();
    };  // end of getDataStructure
    auto& rf = ::mfront::bbrick::IsotropicHardeningRuleFactory::getFactory();
    const auto& rules = opts.at("isotropic_hardening");
    if (rules.is<std::vector<tfel::utilities::Data>>()) {
      for (const auto& ird : rules.get<std::vector<tfel::utilities::Data>>()) {
        const auto ds = getDataStructure(ird);
        const auto ihr = rf.generate(ds.name);
        if (this->allowMultipleFlowRules()) {
          ihr->initialize(this->mb, *this, std::to_string(flow_id),
                          std::to_string(this->ihrs[flow_id].size()), ds.data);
        } else {
          ihr->initialize(this->mb, *this, "",
                          std::to_string(this->ihrs[flow_id].size()), ds.data);
        }
        this->ihrs[flow_id].push_back(ihr);
      }
    } else {
      const auto ds = getDataStructure(rules);
      const auto ihr = rf.generate(ds.name);
      if (this->allowMultipleFlowRules()) {
        ihr->initialize(this->mb, *this, std::to_string(flow_id), "", ds.data);
      } else {
        ihr->initialize(this->mb, *this, "", "", ds.data);
      }
      this->ihrs[flow_id].push_back(ihr);
    }
  }  // end of treatIsotropicHardeningRules

  void IsotropicBehaviourDSLBase::treatExternalStateVariable() {
    VariableDescriptionContainer ev;
    std::set<Hypothesis> h;
    this->readVariableList(
        ev, h, &BehaviourDescription::addExternalStateVariables, true);
    for (const auto& elem : h) {
      CodeBlock ib;
      for (const auto& v : ev) {
        const auto currentVarName = v.name + "_";
        this->mb.addLocalVariable(
            elem, VariableDescription(v.type, currentVarName, v.arraySize, 0u));
        ib.code = "this->" + currentVarName + " = this->" + v.name + "+(" +
                  this->mb.getClassName() + "::theta)*(this->d" + v.name +
                  ");\n";
      }
      this->mb.setCode(elem, BehaviourData::BeforeInitializeLocalVariables, ib,
                       BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    }
  }  // end of treatExternalStateVariable

  void IsotropicBehaviourDSLBase::completeVariableDeclaration() {
    using namespace tfel::glossary;
    auto add_lv = [](BehaviourDescription& bd, const std::string& t,
                     const std::string& sn, const std::string& n,
                     const std::string& g, const std::string d) {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto r = bd.checkVariableExistence(n, "Parameter", false);
      if (!r.first) {
        auto v = (!sn.empty()) ? VariableDescription(t, sn, n, 1u, 0u)
                               : VariableDescription(t, n, 1u, 0u);
        v.description = d;
        bd.addLocalVariable(h, v, BehaviourData::UNREGISTRED);
      } else {
        tfel::raise_if(!r.second,
                       "ImplicitDSLBase::completeVariableDeclaration: "
                       "Parameter '" +
                           n + "' is not defined for all hypotheses");
        if (!g.empty()) {
          bd.checkVariableGlossaryName(n, g);
        }
      }
    };
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::completeVariableDeclaration : begin\n";
    }
    BehaviourDSLCommon::completeVariableDeclaration();
    // intermediate temperature
    const auto* const Topt = BehaviourDescription::
        automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable;
    if (this->mb.getAttribute<bool>(Topt)) {
      CodeBlock initLocalVars;
      initLocalVars.code = "this->T_ = this->T + (" + this->getClassName() +
                           "::theta) * (this->dT);\n";
      this->mb.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                       BehaviourData::BeforeInitializeLocalVariables,
                       initLocalVars, BehaviourData::CREATEORAPPEND,
                       BehaviourData::BODY);
    }
    add_lv(this->mb, "stress", "\u03BB", "lambda",
           Glossary::FirstLameCoefficient,
           "first Lamé coefficient at t+theta*dt");
    add_lv(this->mb, "stress", "\u03BC", "mu", Glossary::ShearModulus,
           "shear modulus at t+theta*dt");
    add_lv(this->mb, "stress", "\u03BB\u2091\u209C\u209B", "lambda_tdt", "",
           "first Lamé coefficient at t+dt");
    add_lv(this->mb, "stress", "\u03BC\u2091\u209C\u209B", "mu_tdt", "",
           "shear modulus at t+dt");
    if (this->mb.areElasticMaterialPropertiesDefined()) {
      add_lv(this->mb, "stress", "", "young", Glossary::YoungModulus,
             "Young modulus at t+theta*dt");
      add_lv(this->mb, "stress", "", "young_tdt", "", "Young modulus at t+dt");
      add_lv(this->mb, "real", "\u03BD", "nu", Glossary::PoissonRatio,
             "Poisson ratio at t+theta*dt");
      add_lv(this->mb, "real", "\u03BD\u2091\u209C\u209B", "nu_tdt", "",
             "Poisson ratio at t+dt");
      if (this->mb.getAttribute(
              IsotropicBehaviourDSLBase::useStressUpdateAlgorithm, false)) {
        const auto& emps = this->mb.getElasticMaterialProperties();
        if (emps.size() != 2u) {
          this->throwRuntimeError(
              "IsotropicBehaviourCodeGenerator::"
              "writeBehaviourLocalVariablesInitialisation",
              "invalid number of material properties");
        }
        if (!((this->mb.isMaterialPropertyConstantDuringTheTimeStep(emps[0])) &&
              (this->mb.isMaterialPropertyConstantDuringTheTimeStep(
                  emps[1])))) {
          this->reserveName("mfront_eel_bts");
          this->reserveName("mfront_eel_ets");
          this->reserveName("mfront_young_bts");
          this->reserveName("mfront_nu_bts");
        }
      }
    } else {
      this->mb.addMaterialProperty(
          uh, VariableDescription("stress", "young", 1u, 0u));
      this->mb.setGlossaryName(uh, "young", "YoungModulus");
      this->mb.addMaterialProperty(
          uh, VariableDescription("real", "\u03BD", "nu", 1u, 0u));
      this->mb.setGlossaryName(uh, "nu", "PoissonRatio");
    }
    if (!this->mb.hasParameter(uh, "theta")) {
      this->mb.addParameter(
          uh, VariableDescription("real", "\u03B8", "theta", 1u, 0u),
          BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(uh, "theta",
                                        this->getDefaultThetaValue());
    }
    if (!this->mb.hasParameter(uh, "epsilon")) {
      this->mb.addParameter(
          uh, VariableDescription("real", "\u03B5", "epsilon", 1u, 0u),
          BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(uh, "epsilon", 1.e-8);
      reportWarning(
          "using the default value for the convergence threshold. "
          "This value is generally considered too loose. You may "
          "want to consider a more stringent value (1e-14 is a good choice). "
          "See the `@Epsilon` keyword for details");
    }
    if (!this->mb.hasParameter(uh, "iterMax")) {
      unsigned short iterMax = 100u;
      this->mb.addParameter(uh,
                            VariableDescription("ushort", "iterMax", 1u, 0u),
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(uh, "iterMax", iterMax);
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::completeVariableDeclaration: "
             "end\n";
    }
  }  // end of completeVariableDeclaration

  void IsotropicBehaviourDSLBase::endsInputFileProcessing() {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing: begin\n";
    }
    BehaviourDSLCommon::endsInputFileProcessing();
    // isotropic hardening rules
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      auto add_ihrs = [this, h](
                          const std::string& cname, const std::string& id,
                          const std::vector<std::shared_ptr<
                              mfront::bbrick::IsotropicHardeningRule>>& lihrs) {
        auto i = CodeBlock{};
        i.code = computeElasticLimitAndDerivative(this->mb, lihrs, id);
        if (!id.empty()) {
          i.code += "const auto R = R" + id + ";\n";
          i.code += "const auto dR_dp = dR" + id + "_dp" + id + ";\n";
        }
        this->mb.setCode(h, cname, i, BehaviourData::CREATEORAPPEND,
                         BehaviourData::AT_BEGINNING);
      };
      auto& d = this->mb.getBehaviourData(h);
      if (this->allowMultipleFlowRules()) {
        for (const auto& [id, lihrs] : this->ihrs) {
          if (id + 1 > IsotropicBehaviourDSLBase::getNumberOfFlowRules()) {
            this->throwRuntimeError(
                "IsotropicBehaviourDSLBase::completeVariableDeclaration",
                "no flow rule associated with flow id '" + std::to_string(id) +
                    "'");
          }
          if (lihrs.size() == 1) {
            lihrs[0]->endTreatment(this->mb, *this, "", "");
          } else {
            auto n = std::size_t{};
            for (auto& ihr : lihrs) {
              ihr->endTreatment(this->mb, *this, "", std::to_string(n));
              ++n;
            }
          }
          const auto cname = BehaviourData::BeforeFlowRule + std::to_string(id);
          add_ihrs(cname, std::to_string(id), lihrs);
        }
      } else {
        if (this->ihrs.size() != 0) {
          if ((this->ihrs.size() != 1) || (!this->ihrs.contains(0))) {
            this->throwRuntimeError(
                "IsotropicBehaviourDSLBase::completeVariableDeclaration",
                "internal error (inconsistent isotropic hardening");
          }
          if (this->ihrs.at(0).size() == 1) {
            this->ihrs.at(0)[0]->endTreatment(this->mb, *this, "", "");
          } else {
            auto n = std::size_t{};
            for (auto& ihr : this->ihrs.at(0)) {
              ihr->endTreatment(this->mb, *this, "", std::to_string(n));
              ++n;
            }
          }
          if (!d.hasCode(BehaviourData::FlowRule)) {
            this->throwRuntimeError(
                "IsotropicBehaviourDSLBase::completeVariableDeclaration",
                "no flow rule defined");
          }
          add_ihrs(BehaviourData::BeforeFlowRule, "", this->ihrs.at(0));
        }
      }
    }
    //
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing: end\n";
    }
  }  // end of endsInputFileProcessing

  IsotropicBehaviourDSLBase::~IsotropicBehaviourDSLBase() = default;

}  // end of namespace mfront
