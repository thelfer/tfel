/*!
 * \file   mfront/src/IsotropicBehaviourDSLBase.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>

#include "MFront/MFrontLogStream.hxx"
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  IsotropicBehaviourDSLBase::IsotropicBehaviourDSLBase()
      : BehaviourDSLBase<IsotropicBehaviourDSLBase>(), theta(0.5) {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->reserveName("NewtonIntegration");
    // main variables
    this->mb.declareAsASmallStrainStandardBehaviour();
    // material symmetry
    this->mb.setSymmetryType(mfront::ISOTROPIC);
    this->mb.setElasticSymmetryType(mfront::ISOTROPIC);
    // parameters
    this->reserveName("theta");
    this->reserveName("epsilon");
    this->reserveName("iterMax");
    this->mb.addMaterialProperty(
        h, VariableDescription("stress", "young", 1u, 0u));
    this->mb.setGlossaryName(h, "young", "YoungModulus");
    this->mb.addMaterialProperty(h, VariableDescription("real", "nu", 1u, 0u));
    this->mb.setGlossaryName(h, "nu", "PoissonRatio");
    // Lame coefficients
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "lambda", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("stress", "mu", 1u, 0u));
    // intermediate temperature
    this->mb.addLocalVariable(h,
                              VariableDescription("temperature", "T_", 1u, 0u));
    // local variable initialisation
    CodeBlock initLocalVars;
    initLocalVars.code =
        "// initialisation Lame's coefficient\n"
        "this->lambda=tfel::material::computeLambda(this->young,this->nu);\n"
        "this->mu=tfel::material::computeMu(this->young,this->nu);\n";
    this->mb.setCode(h, BehaviourData::BeforeInitializeLocalVariables,
                     initLocalVars, BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
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
    this->disableCallBack("@Brick");
    this->disableCallBack("@StateVar");
    this->disableCallBack("@StateVariable");
    this->disableCallBack("@Integrator");
    this->disableCallBack("@OrthotropicBehaviour");
    this->disableCallBack("@IsotropicElasticBehaviour");
    this->disableCallBack("@IsotropicBehaviour");
    this->disableCallBack("@RequireStiffnessOperator");
    this->disableCallBack("@RequireThermalExpansionCoefficientTensor");
    // a defaut version of the prediction operator is always provided
    this->mb.setAttribute(h, BehaviourData::hasPredictionOperator, true);
    this->mb.setIntegrationScheme(BehaviourDescription::SPECIFICSCHEME);
  }  // end of IsotropicBehaviourDSLBase::IsotropicBehaviourDSLBase

  bool IsotropicBehaviourDSLBase::isModellingHypothesisSupported(
      const Hypothesis h) const {
    return ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICAL) ||
            (h == ModellingHypothesis::PLANESTRAIN) ||
            (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
            (h == ModellingHypothesis::TRIDIMENSIONAL));
  }  // end of IsotropicBehaviourDSLBase::isModellingHypothesisSupported

  void IsotropicBehaviourDSLBase::treatTheta() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double v;
    this->checkNotEndOfFile("IsotropicBehaviourDSLBase::treatTheta",
                            "Cannot read theta value.");
    std::istringstream flux(current->value);
    flux >> v;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatTheta",
                              "Failed to read theta value.");
    }
    if ((v < 0.) || (v > 1.)) {
      this->throwRuntimeError(
          "IsotropicBehaviourDSLBase::treatTheta",
          "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatTheta", ";");
    this->mb.addParameter(h, VariableDescription("real", "theta", 1u, 0u),
                          BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "theta", v);
  }  // end of IsotropicBehaviourDSLBase::treatTheta

  void IsotropicBehaviourDSLBase::treatEpsilon() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile("IsotropicBehaviourDSLBase::treatEpsilon",
                            "Cannot read epsilon value.");
    std::istringstream flux(current->value);
    flux >> epsilon;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatEpsilon",
                              "Failed to read epsilon value.");
    }
    if (epsilon < 0) {
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatEpsilon",
                              "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatEpsilon", ";");
    this->mb.addParameter(h, VariableDescription("real", "epsilon", 1u, 0u),
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

  void IsotropicBehaviourDSLBase::writeBehaviourParserSpecificIncludes() {
    this->checkBehaviourFile();
    this->behaviourFile << "#include\"TFEL/Math/General/BaseCast.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/Lame.hxx\"\n\n";
  }  // end of IsotropicBehaviourDSLBase::writeBehaviourParserSpecificIncludes

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
  }  // end of IsotropicBehaviourDSLBase::flowRuleVariableModifier

  void IsotropicBehaviourDSLBase::treatFlowRule() {
    this->readCodeBlock(*this, BehaviourData::FlowRule,
                        &IsotropicBehaviourDSLBase::flowRuleVariableModifier,
                        true, false);
  }  // end of IsotropicBehaviourDSLBase::treatFlowRule

  void IsotropicBehaviourDSLBase::endsInputFileProcessing() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      auto& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing : begin\n";
    }
    BehaviourDSLCommon::endsInputFileProcessing();
    if (!this->mb.hasParameter(h, "theta")) {
      this->mb.addParameter(h, VariableDescription("real", "theta", 1u, 0u),
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "theta", this->theta);
    }
    if (!this->mb.hasParameter(h, "epsilon")) {
      this->mb.addParameter(h, VariableDescription("real", "epsilon", 1u, 0u),
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "epsilon", 1.e-8);
    }
    if (!this->mb.hasParameter(h, "iterMax")) {
      unsigned short iterMax = 100u;
      this->mb.addParameter(h, VariableDescription("ushort", "iterMax", 1u, 0u),
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "iterMax", iterMax);
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
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing : end\n";
    }
  }  // end of IsotropicBehaviourDSLBase::endsInputFileProcessing

  void IsotropicBehaviourDSLBase::treatExternalStateVariable() {
    VariableDescriptionContainer ev;
    std::set<Hypothesis> h;
    this->readVariableList(
        ev, h, &BehaviourDescription::addExternalStateVariables, true, false);
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

  void IsotropicBehaviourDSLBase::writeBehaviourParserSpecificTypedefs() {
    this->checkBehaviourFile();
    this->behaviourFile
        << "typedef typename tfel::math::ComputeBinaryResult<"
        << "strain,time,tfel::math::OpDiv>::Result DstrainDt;\n"
        << "typedef typename tfel::math::ComputeBinaryResult<"
        << "DstrainDt,stress,tfel::math::OpDiv>::Result DF_DSEQ_TYPE;\n\n";
  }  // end of IsotropicBehaviourDSLBase::writeBehaviourParserSpecificTypedefs

  void IsotropicBehaviourDSLBase::writeBehaviourComputePredictionOperator(
      const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
    if (!this->mb.hasCode(h, BehaviourData::ComputePredictionOperator)) {
      this->behaviourFile << "IntegrationResult\n"
                          << "computePredictionOperator(const SMFlag smflag, "
                             "const SMType smt) override{\n";
      this->behaviourFile << "using namespace std;\n";
      if (this->mb.useQt()) {
        this->behaviourFile
            << "if(smflag!=MechanicalBehaviour<" << btype
            << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n";
      } else {
        this->behaviourFile
            << "if(smflag!=MechanicalBehaviour<" << btype
            << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n";
      }
      this->behaviourFile
          << "throw(runtime_error(\"" << this->mb.getClassName()
          << "::computePredictionOperator : "
          << "invalid tangent operator flag\"));\n"
          << "}\n"
          << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
          << "Dt = "
             "(this->lambda)*Stensor4::IxI()+2*(this->mu)*Stensor4::Id();\n"
          << "} else {\n"
          << "string msg(\"" << this->mb.getClassName()
          << "::computePredictionOperator : \");\n"
          << "msg +=\"unimplemented feature\";\n"
          << "throw(runtime_error(msg));\n"
          << "}\n\n"
          << "return SUCCESS;\n"
          << "}\n\n";
    } else {
      BehaviourDSLCommon::writeBehaviourComputePredictionOperator(h);
    }
  }  // end of
     // IsotropicBehaviourDSLBase::writeBehaviourComputePredictionOperator(void)

  void IsotropicBehaviourDSLBase::writeBehaviourComputeTangentOperator(
      const Hypothesis) {
  }  // end of IsotropicBehaviourDSLBase::writeBehaviourComputeTangentOperator

  IsotropicBehaviourDSLBase::~IsotropicBehaviourDSLBase() = default;

}  // end of namespace mfront
