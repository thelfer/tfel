/*!
 * \file   MFrontIsotropicBehaviourParserBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2007
 */

#include<sstream>

#include"MFront/MFrontIsotropicBehaviourParserBase.hxx"

namespace mfront{

  MFrontIsotropicBehaviourParserBase::MFrontIsotropicBehaviourParserBase()
    : MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>(),
      theta(0.5f)
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    this->reserveName("NewtonIntegration");
    // main variables
    this->registerVariable("eto");
    this->registerVariable("deto");
    this->registerVariable("sig");
    this->mb.declareAsASmallStrainStandardBehaviour();
    // parameters
    this->registerVariable("theta");
    this->mb.getParameters().push_back(VariableDescription("real","theta",1u,0u));
    this->registerVariable("epsilon");
    this->mb.getParameters().push_back(VariableDescription("real","epsilon",1u,0u));
    this->registerVariable("iterMax");
    this->mb.getParameters().push_back(VariableDescription("ushort","iterMax",
						1u,0u));
    this->registerVariable("young");
    this->registerVariable("nu");
    this->registerVariable("rho");
    this->registerVariable("alpha");
    this->registerVariable("lambda");
    this->registerVariable("mu");
    this->mb.getMaterialProperties().push_back(VariableDescription("stress","young",1u,0u));
    this->glossaryNames.insert(MVType("young","YoungModulus"));
    this->mb.getMaterialProperties().push_back(VariableDescription("real","nu",1u,0u));
    this->glossaryNames.insert(MVType("nu","PoissonRatio"));
    this->mb.getMaterialProperties().push_back(VariableDescription("density","rho",1u,0u));
    this->glossaryNames.insert(MVType("rho","MassDensity"));
    this->mb.getMaterialProperties().push_back(VariableDescription("thermalexpansion","alpha",1u,0u));
    this->glossaryNames.insert(MVType("alpha","ThermalExpansion"));
    this->mb.getLocalVariables().push_back(VariableDescription("stress","lambda",1u,0u));
    this->mb.getLocalVariables().push_back(VariableDescription("stress","mu",1u,0u));
    // local var initialisation
    this->initLocalVars +="this->lambda=tfel::material::lame::computeLambda(this->young,this->nu);\n";
    this->initLocalVars +="this->mu=tfel::material::lame::computeMu(this->young,this->nu);\n";
    // Call Back
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &MFrontIsotropicBehaviourParserBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&MFrontIsotropicBehaviourParserBase::treatMaterialLaw);
    this->registerNewCallBack("@FlowRule",&MFrontIsotropicBehaviourParserBase::treatFlowRule);
    this->registerNewCallBack("@Theta",&MFrontIsotropicBehaviourParserBase::treatTheta);
    this->registerNewCallBack("@Epsilon",&MFrontIsotropicBehaviourParserBase::treatEpsilon);
    this->registerNewCallBack("@IterMax",&MFrontIsotropicBehaviourParserBase::treatIterMax);
    this->disableCallBack("@StateVar");
    this->disableCallBack("@StateVariable");
    this->disableCallBack("@Integrator");
    this->disableCallBack("@OrthotropicBehaviour");
    this->disableCallBack("@IsotropicElasticBehaviour");
    this->disableCallBack("@IsotropicBehaviour");
    this->disableCallBack("@RequireStiffnessOperator");
    this->disableCallBack("@RequireThermalExpansionTensor");
    this->hasPredictionOperator = true;
  } // end of MFrontIsotropicBehaviourParserBase::MFrontIsotropicBehaviourParserBase

  MFrontIsotropicBehaviourParserBase::~MFrontIsotropicBehaviourParserBase()
  {} // end of MFrontIsotropicBehaviourParserBase::~MFrontIsotropicBehaviourParserBase()

  void
  MFrontIsotropicBehaviourParserBase::treatTheta(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    double v;
    this->checkNotEndOfFile("MFrontIsotropicBehaviourParserBase::treatTheta",
			    "Cannot read theta value.");
    istringstream flux(current->value);
    flux >> v;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatTheta",
			      "Failed to read theta value.");
    }
    if((v<0.)||(v>1.)){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatTheta",
			      "Theta value must be positive and smaller than 1.");
    }
    if(!this->mb.getParametersDefaultValues().insert(MVType("theta",v)).second){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatTheta",
			      "default value already defined for parameter 'theta'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontIsotropicBehaviourParserBase::treatTheta",";");
  } // end of MFrontIsotropicBehaviourParserBase::treatTheta

  void
  MFrontIsotropicBehaviourParserBase::treatEpsilon(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    double epsilon;
    this->checkNotEndOfFile("MFrontIsotropicBehaviourParserBase::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    if(!this->mb.getParametersDefaultValues().insert(MVType("epsilon",epsilon)).second){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatEpsilon",
			      "default value already defined for parameter 'epsilon'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontIsotropicBehaviourParserBase::treatEpsilon",";");
  } // MFrontIsotropicBehaviourParserBase::treatEpsilon

  void
  MFrontIsotropicBehaviourParserBase::treatIterMax(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short iterMax;
    iterMax = this->readUnsignedShort("MFrontIsotropicBehaviourParserBase::treatIterMax");
    if(iterMax==0){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatIterMax",
			      "invalid value for parameter 'iterMax'");
    }
    if(!this->mb.getUnsignedShortParametersDefaultValues().insert(MVType("iterMax",iterMax)).second){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatIterMax",
			      "default value already defined for parameter 'iterMax'");
    }
    this->readSpecifiedToken("MFrontIsotropicBehaviourParserBase::treatIterMax",";");
  } // end of MFrontIsotropicBehaviourParserBase::treatIterMax

  void
  MFrontIsotropicBehaviourParserBase::writeBehaviourParserSpecificIncludes(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "#include\"TFEL/Math/General/BaseCast.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/Lame.hxx\"\n\n";
  } // end of MFrontIsotropicBehaviourParserBase::writeBehaviourParserSpecificIncludes

  std::string
  MFrontIsotropicBehaviourParserBase::flowRuleVariableModifier(const std::string& var,
							const bool addThisPtr)
  {
    if((var=="T")||(this->mb.isExternalStateVariableName(var))||
       (this->mb.isInternalStateVariableName(var))){
      if(addThisPtr){
	return "this->"+var+"_";
      } else {
	return var+"_";
      }
    }
    if((this->mb.isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontIsotropicBehaviourParserBase::flowRuleVariableModifier

  void
  MFrontIsotropicBehaviourParserBase::treatFlowRuleBase(std::string &flow)
  {
    flow = this->readNextBlock(makeVariableModifier(*this,&MFrontIsotropicBehaviourParserBase::flowRuleVariableModifier),
			       true);
  } // end of MFrontIsotropicBehaviourParserBase::treatFlowRuleBase

  void
  MFrontIsotropicBehaviourParserBase::treatFlowRule(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    if(!this->flowRule.empty()){
      string msg("MFrontIsotropicBehaviourParserBase::treatFlowRule : ");
      msg += "flow already defined";
      throw(runtime_error(msg));
    }
    MFrontIsotropicBehaviourParserBase::treatFlowRuleBase(this->flowRule);
  } // end of MFrontIsotropicBehaviourParserBase::treatFlowRule

  void
  MFrontIsotropicBehaviourParserBase::endsInputFileProcessing(void)
  {
    using namespace std;
    MFrontBehaviourParserCommon::endsInputFileProcessing();
    typedef map<string,double>::value_type MVType;
    typedef map<string,unsigned short>::value_type MVType2;
    VariableDescriptionContainer::iterator p;
    string currentVarName;
    if(this->mb.getParametersDefaultValues().find("theta")==this->mb.getParametersDefaultValues().end()){
      this->mb.getParametersDefaultValues().insert(MVType("theta",this->theta));
    }
    if(this->mb.getParametersDefaultValues().find("epsilon")==this->mb.getParametersDefaultValues().end()){
      this->mb.getParametersDefaultValues().insert(MVType("epsilon",1.e-8));
    }
    if(this->mb.getUnsignedShortParametersDefaultValues().find("iterMax")==this->mb.getUnsignedShortParametersDefaultValues().end()){
      this->mb.getUnsignedShortParametersDefaultValues().insert(MVType2("iterMax",100u));
    }
    if(this->flowRule.empty()){
      string msg("MFrontIsotropicBehaviourParserBase::endsInputFileProcessing : ");
      msg += "@FlowRule was not defined.";
      throw(runtime_error(msg));
    }
    this->registerVariable("T_");
    this->mb.getLocalVariables().push_back(VariableDescription("temperature","T_",1u,0u));
    this->initLocalVars = "this->T_ = this->T+(" + this->className + "::theta)*(this->dT);\n" + this->initLocalVars;
    for(p =this->mb.getExternalStateVariables().begin();
	p!=this->mb.getExternalStateVariables().end();++p){
      currentVarName = p->name + "_";
      this->registerVariable(currentVarName);
      this->mb.getLocalVariables().push_back(VariableDescription(p->type,currentVarName,p->arraySize,0u));
      this->initLocalVars = "this->" + currentVarName + " = this->" + p->name +
	"+(" + this->className + "::theta)*(this->d" + p->name + ");\n" + this->initLocalVars;
    }
  } // end of MFrontIsotropicBehaviourParserBase::endsInputFileProcessing

  void
  MFrontIsotropicBehaviourParserBase::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "typedef typename tfel::math::ComputeBinaryResult<"
			<< "strain,time,tfel::math::OpDiv>::Result DstrainDt;\n";
    this->behaviourFile << "typedef typename tfel::math::ComputeBinaryResult<"
			<< "DstrainDt,stress,tfel::math::OpDiv>::Result DF_DSEQ_TYPE;\n\n";
  } // end of MFrontIsotropicBehaviourParserBase::writeBehaviourParserSpecificTypedefs

  void MFrontIsotropicBehaviourParserBase::writeBehaviourComputePredictionOperator(void)
  {
    if(this->predictionOperator.empty()){
      this->behaviourFile << "IntegrationResult computePredictionOperator(const SMType smt){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n";
      this->behaviourFile << "Dt = (this->lambda)*Stensor4::IxI()+2*(this->mu)*Stensor4::Id();\n";
      this->behaviourFile << "} else {\n";
      this->behaviourFile << "string msg(\"" << this->className<< "::computePredictionOperator : \");\n";
      this->behaviourFile << "msg +=\"unimplemented feature\";\n";
      this->behaviourFile << "throw(runtime_error(msg));\n";
      this->behaviourFile << "}\n\n";
      this->behaviourFile << "return SUCCESS;\n";
      this->behaviourFile << "}\n\n";
    } else {
      MFrontBehaviourParserCommon::writeBehaviourComputePredictionOperator();
    }
  } // end of MFrontIsotropicBehaviourParserBase::writeBehaviourComputePredictionOperator(void)

  void 
  MFrontIsotropicBehaviourParserBase::writeBehaviourStaticVars(void)
  {
    this->checkBehaviourFile();
    MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>::writeBehaviourStaticVars();
  } // end of MFrontIsotropicBehaviourParserBase::writeBehaviourStaticVars

} // end of namespace mfront
