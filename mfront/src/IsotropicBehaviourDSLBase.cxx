/*!
 * \file   mfront/src/IsotropicBehaviourDSLBase.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  IsotropicBehaviourDSLBase::IsotropicBehaviourDSLBase() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->reserveName("NewtonIntegration");
    // main variables
    this->mb.declareAsASmallStrainStandardBehaviour();
    // material symmetry
    this->mb.setSymmetryType(mfront::ISOTROPIC);
    this->mb.setElasticSymmetryType(mfront::ISOTROPIC);
    // parameters
    this->reserveName("epsilon");
    this->reserveName("\u03B5");
    this->reserveName("theta");
    this->reserveName("\u03B8");
    this->reserveName("iterMax");
    // intermediate temperature
    this->mb.addLocalVariable(h,
                              VariableDescription("temperature", "T_", 1u, 0u));
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
    // a defaut version of the prediction operator is always provided
    this->mb.setAttribute(h, BehaviourData::hasPredictionOperator, true);
    this->mb.setIntegrationScheme(BehaviourDescription::SPECIFICSCHEME);
  }  // end of IsotropicBehaviourDSLBase::IsotropicBehaviourDSLBase

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
  }  // end of IsotropicBehaviourDSLBase::getBehaviourDSLDescription

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
    if (n != BehaviourData::FlowRule) {
      mfront::getIncrementSymbols(symbols, d.getExternalStateVariables());
      mfront::addSymbol(symbols, "\u0394t", "dt");
    }
  }  // end of IsotropicBehaviourDSLBase::getSymbols

  double IsotropicBehaviourDSLBase::getDefaultThetaValue() const { return 0.5; }

  void IsotropicBehaviourDSLBase::treatTheta() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
  }  // end of IsotropicBehaviourDSLBase::treatTheta

  void IsotropicBehaviourDSLBase::treatEpsilon() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("IsotropicBehaviourDSLBase::treatEpsilon",
                            "cannot read epsilon value");
    const auto epsilon = tfel::utilities::convert<double>(this->current->value);
    if (epsilon < 0) {
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatEpsilon",
                              "Epsilon value must be positive");
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
  }  // end of IsotropicBehaviourDSLBase::treatIterMax

  std::string IsotropicBehaviourDSLBase::flowRuleVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((d.isExternalStateVariableName(var)) ||
        (d.isStateVariableName(var))) {
      if (addThisPtr) {
        return "this->" + var + "_";
      } else {
        return var + "_";
      }
    }
    if (d.isAuxiliaryStateVariableName(var)){
      const auto& v = d.getAuxiliaryStateVariables().getVariable(var);
      if (v.getAttribute<bool>("ComputedByExternalModel", false)) {
        if (addThisPtr) {
          return "(this->" + var + " + (this->theta) * (this->d" + var + "))";
        } else {
          return "(" + var + " + (this->theta) * d" + var + ")";
        }
      }
    }
    if ((d.isExternalStateVariableIncrementName(var)) ||
        (var == "dT")) {
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          h, var.substr(1));
    }
    if (addThisPtr) {
      return "this->" + var;
    }
    return var;
  }  // end of IsotropicBehaviourDSLBase::flowRuleVariableModifier

  void IsotropicBehaviourDSLBase::treatFlowRule() {
    this->readCodeBlock(*this, BehaviourData::FlowRule,
                        &IsotropicBehaviourDSLBase::flowRuleVariableModifier,
                        true, false);
  }  // end of IsotropicBehaviourDSLBase::treatFlowRule

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
  }  // end of IsotropicBehaviourDSLBase::treatExternalStateVariable

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
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::completeVariableDeclaration : begin\n";
    }
    BehaviourDSLCommon::completeVariableDeclaration();
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
    } else {
      this->mb.addMaterialProperty(
          h, VariableDescription("stress", "young", 1u, 0u));
      this->mb.setGlossaryName(h, "young", "YoungModulus");
      this->mb.addMaterialProperty(
          h, VariableDescription("real", "\u03BD", "nu", 1u, 0u));
      this->mb.setGlossaryName(h, "nu", "PoissonRatio");
    }
    if (!this->mb.hasParameter(h, "theta")) {
      this->mb.addParameter(
          h, VariableDescription("real", "\u03B8", "theta", 1u, 0u),
          BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "theta",
                                        this->getDefaultThetaValue());
    }
    if (!this->mb.hasParameter(h, "epsilon")) {
      this->mb.addParameter(
          h, VariableDescription("real", "\u03B5", "epsilon", 1u, 0u),
          BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "epsilon", 1.e-8);
    }
    if (!this->mb.hasParameter(h, "iterMax")) {
      unsigned short iterMax = 100u;
      this->mb.addParameter(h, VariableDescription("ushort", "iterMax", 1u, 0u),
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "iterMax", iterMax);
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::completeVariableDeclaration: end\n";
    }
  }  // end of IsotropicBehaviourDSLBase::completeVariableDeclaration

  void IsotropicBehaviourDSLBase::endsInputFileProcessing() {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing: begin\n";
    }
    // temperature at the midle of the time step
    CodeBlock initLocalVars;
    initLocalVars.code = "this->T_ = this->T+(" + this->getClassName() +
                         "::theta)*(this->dT);\n";
    this->mb.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                     BehaviourData::BeforeInitializeLocalVariables,
                     initLocalVars, BehaviourData::CREATEORAPPEND,
                     BehaviourData::BODY);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing: end\n";
    }
  }  // end of IsotropicBehaviourDSLBase::endsInputFileProcessing

  void IsotropicBehaviourDSLBase::writeBehaviourLocalVariablesInitialisation(
      std::ostream& os, const Hypothesis h) const {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier mts = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+(this->theta)*(this->d" + i.name + ')';
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->mb.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    Modifier ets = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->mb.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    if (this->mb.areElasticMaterialPropertiesDefined()) {
      const auto& emps = this->mb.getElasticMaterialProperties();
      if (emps.size() != 2u) {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "invalid number of material properties");
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], mts);
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], mts);
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        os << "this->young=";
        this->writeMaterialPropertyEvaluation(os, emps[0], mts);
        os << ";\n";
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        os << "this->nu=";
        this->writeMaterialPropertyEvaluation(os, emps[1], mts);
        os << ";\n";
      }
      os << "this->lambda=computeLambda(young,nu);\n";
      os << "this->mu=computeMu(young,nu);\n";
      if (!this->mb.isMaterialPropertyConstantDuringTheTimeStep(emps[0])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], ets);
        os << "this->young_tdt=";
        this->writeMaterialPropertyEvaluation(os, emps[0], ets);
        os << ";\n";
      } else {
        os << "this->young_tdt  = this->young;\n";
      }
      if (!this->mb.isMaterialPropertyConstantDuringTheTimeStep(emps[1])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], ets);
        os << "this->nu_tdt=";
        this->writeMaterialPropertyEvaluation(os, emps[1], ets);
        os << ";\n";
      } else {
        os << "this->nu_tdt     = this->nu;\n";
      }
      if (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        os << "this->lambda_tdt = computeLambda(young_tdt,nu_tdt);\n";
        os << "this->mu_tdt     = computeMu(young_tdt,nu_tdt);\n";
      } else {
        os << "this->lambda_tdt = this->lambda;\n"
           << "this->mu_tdt     = this->mu;\n";
      }
    } else {
      os << "this->lambda=tfel::material::computeLambda(this->young,this->nu);"
            "\n"
         << "this->mu=tfel::material::computeMu(this->young,this->nu);\n"
         << "this->lambda_tdt = this->lambda;\n"
         << "this->mu_tdt     = this->mu;\n";
    }
    BehaviourDSLCommon::writeBehaviourLocalVariablesInitialisation(os, h);
  }  // end of
     // IsotropicBehaviourDSLBase::writeBehaviourLocalVariablesInitialisation

  void IsotropicBehaviourDSLBase::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "#include\"TFEL/Math/General/BaseCast.hxx\"\n\n";
  }  // end of IsotropicBehaviourDSLBase::writeBehaviourParserSpecificIncludes

  void IsotropicBehaviourDSLBase::writeBehaviourParserSpecificTypedefs(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "typedef typename tfel::math::ComputeBinaryResult<"
       << "strain,time,tfel::math::OpDiv>::Result DstrainDt;\n"
       << "typedef typename tfel::math::ComputeBinaryResult<"
       << "DstrainDt,stress,tfel::math::OpDiv>::Result DF_DSEQ_TYPE;\n\n";
  }  // end of IsotropicBehaviourDSLBase::writeBehaviourParserSpecificTypedefs

  void IsotropicBehaviourDSLBase::writeBehaviourComputePredictionOperator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->mb.getBehaviourTypeFlag();
    if (!this->mb.hasCode(h, BehaviourData::ComputePredictionOperator)) {
      os << "IntegrationResult\n"
         << "computePredictionOperator(const SMFlag smflag, const SMType smt) "
            "override{\n";
      os << "using namespace std;\n";
      if (this->mb.useQt()) {
        os << "if(smflag!=MechanicalBehaviour<" << btype
           << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n";
      } else {
        os << "if(smflag!=MechanicalBehaviour<" << btype
           << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n";
      }
      os << "throw(runtime_error(\"" << this->mb.getClassName()
         << "::computePredictionOperator : "
         << "invalid tangent operator flag\"));\n"
         << "}\n"
         << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
         << "Dt = "
            "(this->lambda_tdt)*Stensor4::IxI()+2*(this->mu_tdt)*Stensor4::Id()"
            ";\n"
         << "} else {\n"
         << "string msg(\"" << this->mb.getClassName()
         << "::computePredictionOperator : \");\n"
         << "msg +=\"unimplemented feature\";\n"
         << "throw(runtime_error(msg));\n"
         << "}\n\n"
         << "return SUCCESS;\n"
         << "}\n\n";
    } else {
      BehaviourDSLCommon::writeBehaviourComputePredictionOperator(os, h);
    }
  }  // end of
     // IsotropicBehaviourDSLBase::writeBehaviourComputePredictionOperator()

  void IsotropicBehaviourDSLBase::writeBehaviourComputeTangentOperator(
      std::ostream&, const Hypothesis) const {
  }  // end of IsotropicBehaviourDSLBase::writeBehaviourComputeTangentOperator

  IsotropicBehaviourDSLBase::~IsotropicBehaviourDSLBase() = default;

}  // end of namespace mfront
