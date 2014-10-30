/*!
 * \file   mfront/src/IsotropicBehaviourDSLBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>

#include"MFront/MFrontLogStream.hxx"
#include"MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront{

  IsotropicBehaviourDSLBase::IsotropicBehaviourDSLBase()
    : BehaviourDSLBase<IsotropicBehaviourDSLBase>(),
      theta(0.5f)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->reserveName("NewtonIntegration",false);
    // main variables
    this->registerVariable("eto",false);
    this->registerVariable("deto",false);
    this->registerVariable("sig",false);
    this->mb.declareAsASmallStrainStandardBehaviour();
    // parameters
    this->registerVariable("theta",false);
    this->registerVariable("epsilon",false);
    this->registerVariable("iterMax",false);
    this->registerVariable("young",false);
    this->registerVariable("nu",false);
    this->registerVariable("rho",false);
    this->registerVariable("alpha",false);
    this->registerVariable("lambda",false);
    this->registerVariable("mu",false);
    this->mb.addMaterialProperty(h,VariableDescription("stress","young",1u,0u));
    this->mb.setGlossaryName(h,"young","YoungModulus");
    this->mb.addMaterialProperty(h,VariableDescription("real","nu",1u,0u));
    this->mb.setGlossaryName(h,"nu","PoissonRatio");
    // Lame coefficients
    this->mb.addLocalVariable(h,VariableDescription("stress","lambda",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("stress","mu",1u,0u));
    // intermediate temperature
    this->registerVariable("T_",false);
    this->mb.addLocalVariable(h,VariableDescription("temperature","T_",1u,0u));
    // local variable initialisation
    CodeBlock initLocalVars;
    initLocalVars.code = "// initialisation Lame's coefficient\n"
      "this->lambda=tfel::material::lame::computeLambda(this->young,this->nu);\n"
      "this->mu=tfel::material::lame::computeMu(this->young,this->nu);\n";
    this->mb.setCode(h,BehaviourData::BeforeInitializeLocalVariables,
		     initLocalVars,BehaviourData::CREATEORAPPEND,
		     BehaviourData::AT_BEGINNING);
    // Call Back
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &IsotropicBehaviourDSLBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&IsotropicBehaviourDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@FlowRule",&IsotropicBehaviourDSLBase::treatFlowRule);
    this->registerNewCallBack("@Theta",&IsotropicBehaviourDSLBase::treatTheta);
    this->registerNewCallBack("@Epsilon",&IsotropicBehaviourDSLBase::treatEpsilon);
    this->registerNewCallBack("@IterMax",&IsotropicBehaviourDSLBase::treatIterMax);
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
    this->mb.setAttribute(h,BehaviourData::hasPredictionOperator,true);
    this->mb.setIntegrationScheme(BehaviourDescription::SPECIFICSCHEME);
  } // end of IsotropicBehaviourDSLBase::IsotropicBehaviourDSLBase

  bool
  IsotropicBehaviourDSLBase::isModellingHypothesisSupported(const Hypothesis h) const
  {
    if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
       (h==ModellingHypothesis::AXISYMMETRICAL)||
       (h==ModellingHypothesis::PLANESTRAIN)||
       (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
       (h==ModellingHypothesis::TRIDIMENSIONAL)){
      return true;
    }
    return false;
  } // end of IsotropicBehaviourDSLBase::isModellingHypothesisSupported


  IsotropicBehaviourDSLBase::~IsotropicBehaviourDSLBase()
  {} // end of IsotropicBehaviourDSLBase::~IsotropicBehaviourDSLBase()

  void
  IsotropicBehaviourDSLBase::treatTheta(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double v;
    this->checkNotEndOfFile("IsotropicBehaviourDSLBase::treatTheta",
			    "Cannot read theta value.");
    istringstream flux(current->value);
    flux >> v;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatTheta",
			      "Failed to read theta value.");
    }
    if((v<0.)||(v>1.)){
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatTheta",
			      "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatTheta",";");
    this->mb.addParameter(h,VariableDescription("real","theta",1u,0u));
    this->mb.setParameterDefaultValue(h,"theta",v);
  } // end of IsotropicBehaviourDSLBase::treatTheta

  void
  IsotropicBehaviourDSLBase::treatEpsilon(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile("IsotropicBehaviourDSLBase::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatEpsilon",";");
    this->mb.addParameter(h,VariableDescription("real","epsilon",1u,0u));
    this->mb.setParameterDefaultValue(h,"epsilon",epsilon);
  } // IsotropicBehaviourDSLBase::treatEpsilon

  void
  IsotropicBehaviourDSLBase::treatIterMax(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    unsigned short iterMax;
    iterMax = this->readUnsignedShort("IsotropicBehaviourDSLBase::treatIterMax");
    if(iterMax==0){
      this->throwRuntimeError("IsotropicBehaviourDSLBase::treatIterMax",
			      "invalid value for parameter 'iterMax'");
    }
    this->readSpecifiedToken("IsotropicBehaviourDSLBase::treatIterMax",";");
    this->mb.addParameter(h,VariableDescription("ushort","iterMax",1u,0u));
    this->mb.setParameterDefaultValue(h,"iterMax",iterMax);
  } // end of IsotropicBehaviourDSLBase::treatIterMax

  void
  IsotropicBehaviourDSLBase::writeBehaviourParserSpecificIncludes(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "#include\"TFEL/Math/General/BaseCast.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/Lame.hxx\"\n\n";
  } // end of IsotropicBehaviourDSLBase::writeBehaviourParserSpecificIncludes

  std::string
  IsotropicBehaviourDSLBase::flowRuleVariableModifier(const Hypothesis h,
							       const std::string& var,
							       const bool addThisPtr)
  {
    if((var=="T")||(this->mb.isExternalStateVariableName(h,var))||
       (this->mb.isStateVariableName(h,var))){
      if(addThisPtr){
	return "this->"+var+"_";
      } else {
	return var+"_";
      }
    }
    if((this->mb.isExternalStateVariableIncrementName(h,var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of IsotropicBehaviourDSLBase::flowRuleVariableModifier

  void
  IsotropicBehaviourDSLBase::treatFlowRule(void)
  {
    using namespace std;
    this->readCodeBlock(*this,BehaviourData::FlowRule,
			&IsotropicBehaviourDSLBase::flowRuleVariableModifier,true,false);
  } // end of IsotropicBehaviourDSLBase::treatFlowRule

  void
  IsotropicBehaviourDSLBase::endsInputFileProcessing(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing : begin" << endl;
    }
    BehaviourDSLCommon::endsInputFileProcessing();
    string currentVarName;
    if(!this->mb.hasParameter(h,"theta")){
      this->mb.addParameter(h,VariableDescription("real","theta",1u,0u));
      this->mb.setParameterDefaultValue(h,"theta",this->theta);
    }
    if(!this->mb.hasParameter(h,"epsilon")){
      this->mb.addParameter(h,VariableDescription("real","epsilon",1u,0u));
      this->mb.setParameterDefaultValue(h,"epsilon",1.e-8);
    }
    if(!this->mb.hasParameter(h,"iterMax")){
      unsigned short iterMax = 100u;
      this->mb.addParameter(h,VariableDescription("ushort","iterMax",1u,0u));
      this->mb.setParameterDefaultValue(h,"iterMax",iterMax);
    }
    // temperature at the midle of the time step
    CodeBlock initLocalVars;
    initLocalVars.code = "this->T_ = this->T+(" + this->getClassName() + "::theta)*(this->dT);\n";
    this->mb.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
		     BehaviourData::BeforeInitializeLocalVariables,
    		     initLocalVars,BehaviourData::CREATEORAPPEND,
    		     BehaviourData::BODY);
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "IsotropicBehaviourDSLBase::endsInputFileProcessing : end" << endl;
    }
  } // end of IsotropicBehaviourDSLBase::endsInputFileProcessing

  void IsotropicBehaviourDSLBase::treatExternalStateVariable(void)
  {
    using namespace std;
    VariableDescriptionContainer ev;
    set<Hypothesis> h;
    this->readVariableList(ev,h,&BehaviourDescription::addExternalStateVariables,true,true,false);
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      CodeBlock ib;
      for(VariableDescriptionContainer::const_iterator p=ev.begin();p!=ev.end();++p){
	string currentVarName = p->name + "_";
	this->registerVariable(currentVarName,false);
	this->mb.addLocalVariable(*ph,VariableDescription(p->type,currentVarName,p->arraySize,0u));
	ib.code = "this->" + currentVarName + " = this->" + p->name +
	  "+(" + this->mb.getClassName() + "::theta)*(this->d" + p->name + ");\n";
      }
      this->mb.setCode(*ph,BehaviourData::BeforeInitializeLocalVariables,ib,
		       BehaviourData::CREATEORAPPEND,
		       BehaviourData::AT_END);
    }
  } // end of IsotropicBehaviourDSLBase::treatExternalStateVariable

  void
  IsotropicBehaviourDSLBase::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "typedef typename tfel::math::ComputeBinaryResult<"
			<< "strain,time,tfel::math::OpDiv>::Result DstrainDt;\n";
    this->behaviourFile << "typedef typename tfel::math::ComputeBinaryResult<"
			<< "DstrainDt,stress,tfel::math::OpDiv>::Result DF_DSEQ_TYPE;\n\n";
  } // end of IsotropicBehaviourDSLBase::writeBehaviourParserSpecificTypedefs

  void IsotropicBehaviourDSLBase::writeBehaviourComputePredictionOperator(const Hypothesis h)
  {
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    if(!this->mb.hasCode(h,BehaviourData::ComputePredictionOperator)){
      this->behaviourFile << "IntegrationResult" << endl
			  << "computePredictionOperator(const SMFlag smflag, const SMType smt){\n";
      this->behaviourFile << "using namespace std;\n";
      if(this->mb.useQt()){
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){" << endl;
      } else {
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){" << endl;
      }
      this->behaviourFile << "throw(runtime_error(\"" << this->mb.getClassName()
			  << "::computePredictionOperator : "
			  << "invalid tangent operator flag\"));" << endl
			  << "}" << endl;
      this->behaviourFile << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n";
      this->behaviourFile << "Dt = (this->lambda)*Stensor4::IxI()+2*(this->mu)*Stensor4::Id();\n";
      this->behaviourFile << "} else {\n";
      this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::computePredictionOperator : \");\n";
      this->behaviourFile << "msg +=\"unimplemented feature\";\n";
      this->behaviourFile << "throw(runtime_error(msg));\n";
      this->behaviourFile << "}\n\n";
      this->behaviourFile << "return SUCCESS;\n";
      this->behaviourFile << "}\n\n";
    } else {
      BehaviourDSLCommon::writeBehaviourComputePredictionOperator(h);
    }
  } // end of IsotropicBehaviourDSLBase::writeBehaviourComputePredictionOperator(void)

  void IsotropicBehaviourDSLBase::writeBehaviourComputeTangentOperator(const Hypothesis)
  {} // end of IsotropicBehaviourDSLBase::writeBehaviourComputeTangentOperator

} // end of namespace mfront
