/*!
 * \file   MFrontImplicitParser.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2008
 */

#include<cstdlib>
#include<sstream>

#include"TFEL/System/System.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontImplicitParser.hxx"

namespace mfront{

  MFrontImplicitParser::MFrontImplicitParser()
    : MFrontVirtualParser(), 
      MFrontBehaviourParserBase<MFrontImplicitParser>(),
      algorithm(MFrontImplicitParser::DEFAULT),
      compareToNumericalJacobian(false),
      useRelaxation(false),
      useAcceleration(false)
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    // parameters
    this->registerVariable("theta");
    this->parametersHolder.push_back(VarHandler("real","theta",1u,0u));
    this->registerVariable("epsilon");
    this->parametersHolder.push_back(VarHandler("real","epsilon",1u,0u));
    this->registerVariable("iterMax");
    this->parametersHolder.push_back(VarHandler("ushort","iterMax",1u,0u));
    this->registerVariable("jacobianComparisonCriterium");
    this->registerVariable("relaxationTrigger");
    this->registerVariable("accelerationTrigger");
    this->registerVariable("accelerationPeriod");
    this->registerVariable("relaxationCoefficient");
    // Default state vars
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->registerVariable("previous_zeros");
    this->registerVariable("vect_e");
    this->registerVariable("zeros");
    this->registerVariable("tzeros");
    this->registerVariable("zeros_1");
    this->registerVariable("fzeros");
    this->registerVariable("tfzeros");
    this->registerVariable("zeros2");
    this->registerVariable("fzeros2");
    this->registerVariable("Dzeros");
    this->registerVariable("Dfzeros");
    this->registerVariable("jacobian");
    this->registerVariable("tjacobian");
    this->registerVariable("njacobian");
    this->registerVariable("partial_jacobian");
    this->registerVariable("jacobian2");
    this->registerVariable("t");
    this->registerVariable("dt_");
    this->registerVariable("error");
    this->registerVariable("idx");
    this->registerVariable("idx2");
    this->reserveName("schmidt");
    this->reserveName("computeNumericalJacobian");
    this->reserveName("TinyMatrixSolve");
    this->reserveName("accelerate");
    this->reserveName("accelerate_k0");
    this->reserveName("accelerate_k1");
    this->reserveName("accelerate_k2");
    this->reserveName("accelerate_c0");
    this->reserveName("accelerate_c1");
    this->reserveName("accelerate_re0");
    this->reserveName("accelerate_re1");
    this->reserveName("accelerate_r0");
    this->reserveName("accelerate_r1");
    this->reserveName("accelerate_r2");
    this->reserveName("iter");
    this->reserveName("converge");
    this->reserveName("broyden_inv");
    this->stateVarsHolder.push_back(VarHandler("StrainStensor","eel",1u,0u));
    this->glossaryNames.insert(MVType("eel","ElasticStrain"));
    // CallBacks
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &MFrontImplicitParser::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&MFrontImplicitParser::treatMaterialLaw);
    this->registerNewCallBack("@ComputeStress",&MFrontImplicitParser::treatComputeStress);
    this->registerNewCallBack("@Predictor",&MFrontImplicitParser::treatPredictor);
    this->registerNewCallBack("@Theta",&MFrontImplicitParser::treatTheta);
    this->registerNewCallBack("@Epsilon",&MFrontImplicitParser::treatEpsilon);
    this->registerNewCallBack("@IterMax",&MFrontImplicitParser::treatIterMax);
    this->registerNewCallBack("@MaximumNumberOfIterations",&MFrontImplicitParser::treatIterMax);
    this->registerNewCallBack("@Algorithm",&MFrontImplicitParser::treatAlgorithm);
    this->registerNewCallBack("@UseAcceleration",&MFrontImplicitParser::treatUseAcceleration);
    this->registerNewCallBack("@AccelerationTrigger",&MFrontImplicitParser::treatAccelerationTrigger);
    this->registerNewCallBack("@AccelerationPeriod",&MFrontImplicitParser::treatAccelerationPeriod);
    this->registerNewCallBack("@UseRelaxation",&MFrontImplicitParser::treatUseRelaxation);
    this->registerNewCallBack("@RelaxationTrigger",&MFrontImplicitParser::treatRelaxationTrigger);
    this->registerNewCallBack("@RelaxationCoefficient",&MFrontImplicitParser::treatRelaxationCoefficient);
    this->registerNewCallBack("@TangentOperator",
			      &MFrontImplicitParser::treatTangentOperator);
    this->registerNewCallBack("@InitJacobian",
			      &MFrontImplicitParser::treatInitJacobian);
    this->registerNewCallBack("@CompareToNumericalJacobian",
			      &MFrontImplicitParser::treatCompareToNumericalJacobian);
    this->registerNewCallBack("@JacobianComparisonCriterium",
			      &MFrontImplicitParser::treatJacobianComparisonCriterium);
    //    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
    this->disableCallBack("@UseQt");
  } // end of MFrontImplicitParser::MFrontImplicitParser

  void MFrontImplicitParser::treatInitJacobian(void)
  {
    using namespace std;
    if(this->algorithm!=BROYDEN){
      this->throwRuntimeError("MFrontImplicitParser::treatInitJacobian",
			      "@InitJacobian can only be used with "
			      "the broyden algorithm.");
    }
    if(!this->initJacobian.empty()){
      this->throwRuntimeError("MFrontImplicitParser::treatInitJacobian",
			      "@InitJacobian already used.");
    }
    this->initJacobian += this->readNextBlock(&ParserBase::variableModifier3,
					       true);
    this->initJacobian += "\n";
  } // end of MFrontImplicitParser::treatInitJacobian

  void MFrontImplicitParser::treatTangentOperator(void)
  {
    using namespace std;
    if(!this->tangentOperator.empty()){
      this->throwRuntimeError("MFrontImplicitParser::treatTangentOperator",
			      "@TangentOperator already used.");
    }
    this->tangentOperator = this->readNextBlock(&ParserBase::variableModifier3,
						true);
    this->tangentOperator += "\n";
    this->hasConsistantTangentOperator = true;
    this->isConsistantTangentOperatorSymmetric = false;
  } // end of MFrontImplicitParser::treatTangentOperator
  
  void MFrontImplicitParser::treatUnknownVariableMethod(const std::string& n)
  {
    using namespace std;
    if(this->isInternalStateVariableName(n)){
      if(this->current->value=="setErrorNormalisationFactor"){
	string var;
	++(this->current);
	this->checkNotEndOfFile("MFrontImplicitParser::treatUnknowVariableMethod");
	this->readSpecifiedToken("MFrontImplicitParser::treatUnknowVariableMethod","(");
	this->checkNotEndOfFile("MFrontImplicitParser::treatUnknowVariableMethod");
	var = this->current->value;
	if((this->isMaterialPropertyName(var))||
	   (this->isLocalVariableName(var))){
	  var = "this->" + var;
	} else {
	  // var shall be a number
	  double value;
	  istringstream flux(var);
	  flux >> value;
	  if(flux.fail()){
	    this->throwRuntimeError("MFrontImplicitParser::treatUnknowVariableMethod",
				    "Failed to read error normalisation factor.");
	  }
	  if(value<0.){
	    this->throwRuntimeError("MFrontImplicitParser::treatUnknowVariableMethod",
				    "invalid error normalisation factor.");
	  }
	}
	if(!this->enf.insert(make_pair(n,var)).second){
	  this->throwRuntimeError("MFrontImplicitParser::treatUnknowVariableMethod",
				  "Error normalisation factor already defined for variable '"+n+"'.");
	}
	++(this->current);
	this->checkNotEndOfFile("MFrontImplicitParser::treatUnknowVariableMethod");
	return;
      }
    }
    MFrontBehaviourParserCommon::treatUnknownVariableMethod(n);
  } // end of MFrontImplicitParser::treatUnknowVariableMethod

  void
  MFrontImplicitParser::treatUseAcceleration(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUseAcceleration : ",
			    "Expected 'true' or 'false'.");
    if(this->useAcceleration){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUseAcceleration",
			      "@UseAcceleration already specified");
    }
    if(this->current->value=="true"){
      this->useAcceleration = true;
      this->parametersHolder.push_back(VarHandler("ushort","accelerationTrigger",1u,0u));
      this->parametersHolder.push_back(VarHandler("ushort","accelerationPeriod",1u,0u));
    } else if(this->current->value=="false"){
      this->useAcceleration = false;
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUseAcceleration",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUseAcceleration",";");
  } // end of MFrontImplicitParser::treatUseAcceleration

  void
  MFrontImplicitParser::treatAccelerationTrigger(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short accelerationTrigger;
    accelerationTrigger = this->readUnsignedShort("MFrontImplicitParser::treatAccelerationTrigger");
    if(accelerationTrigger<3){
      this->throwRuntimeError("MFrontImplicitParser::treatAccelerationTrigger",
			      "invalid acceleration trigger value.");
    }
    if(!this->uParametersDefaultValues.insert(MVType("accelerationTrigger",accelerationTrigger)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatAccelerationTrigger",
			      "default value already defined for parameter 'accelerationTrigger'");
    }
    this->readSpecifiedToken("MFrontImplicitParser::treatAccelerationTrigger",";");
  } // end of MFrontImplicitParser::treatAccelerationTrigger

  void
  MFrontImplicitParser::treatAccelerationPeriod(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short accelerationPeriod;
    accelerationPeriod = this->readUnsignedShort("MFrontImplicitParser::treatAccelerationPeriod");
    if(accelerationPeriod==0){
      this->throwRuntimeError("MFrontImplicitParser::treatAccelerationPeriod",
			      "invalid acceleration period value.");
    }
    if(!this->uParametersDefaultValues.insert(MVType("accelerationPeriod",accelerationPeriod)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatAccelerationPeriod",
			      "default value already defined for parameter 'accelerationPeriod'");
    }
    this->readSpecifiedToken("MFrontImplicitParser::treatAccelerationPeriod",";");
  } // end of MFrontImplicitParser::treatAccelerationPeriod

  void
  MFrontImplicitParser::treatUseRelaxation(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUseRelaxation : ",
			    "Expected 'true' or 'false'.");
    if(this->useRelaxation){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUseRelaxation",
			      "@UseRelaxation already specified");
    }
    if(this->current->value=="true"){
      this->useRelaxation = true;
      this->parametersHolder.push_back(VarHandler("real","relaxationCoefficient",1u,0u));
      this->parametersHolder.push_back(VarHandler("ushort","relaxationTrigger",1u,0u));
    } else if(this->current->value=="false"){
      this->useRelaxation = false;
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUseRelaxation",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUseRelaxation",";");
  } // end of MFrontImplicitParser::treatUseRelaxation

  void
  MFrontImplicitParser::treatCompareToNumericalJacobian(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatCompareToNumericalJacobian : ",
			    "Expected 'true' or 'false'.");
    if(this->compareToNumericalJacobian){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatCompareToNumericalJacobian",
			      "@CompareToNumericalJacobian already specified");
    }
    if(this->current->value=="true"){
      this->compareToNumericalJacobian = true;
      this->parametersHolder.push_back(VarHandler("real","jacobianComparisonCriterium",1u,0u));
    } else if(this->current->value=="false"){
      this->compareToNumericalJacobian = false;
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatCompareToNumericalJacobian",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatCompareToNumericalJacobian",";");
  } // end of MFrontImplicitParser::treatCompareToNumericalJacobian
  
  void
  MFrontImplicitParser::treatJacobianComparisonCriterium(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    if(!this->compareToNumericalJacobian){
      this->throwRuntimeError("MFrontImplicitParser::treatJacobianComparisonCriterium",
			      "must call '@CompareToNumericalJacobian' first");
    }
    double jacobianComparisonCriterium;
    this->checkNotEndOfFile("MFrontImplicitParser::treatJacobianComparisonCriterium",
			    "Cannot read jacobianComparisonCriterium value.");
    istringstream flux(current->value);
    flux >> jacobianComparisonCriterium;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParser::treatJacobianComparisonCriterium",
			      "Failed to read jacobianComparisonCriterium value.");
    }
    if(jacobianComparisonCriterium<0){
      this->throwRuntimeError("MFrontImplicitParser::treatJacobianComparisonCriterium",
			      "JacobianComparisonCriterium value must be positive.");
    }
    if(!this->parametersDefaultValues.insert(MVType("jacobianComparisonCriterium",
						    jacobianComparisonCriterium)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatJacobianComparisonCriterium",
			      "default value already defined for parameter 'jacobianComparisonCriterium'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParser::treatJacobianComparisonCriterium",";");
  } // MFrontImplicitParser::treatJacobianComparisonCriterium

  void
  MFrontImplicitParser::treatRelaxationTrigger(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short relaxationTrigger;
    relaxationTrigger = this->readUnsignedShort("MFrontImplicitParser::treatRelaxationTrigger");
    if(!this->uParametersDefaultValues.insert(MVType("relaxationTrigger",relaxationTrigger)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatRelaxationTrigger",
			      "default value already defined for parameter 'relaxationTrigger'");
    }
    this->readSpecifiedToken("MFrontImplicitParser::treatRelaxationTrigger",";");
  } // end of MFrontImplicitParser::treatRelaxationTrigger
  
  void
  MFrontImplicitParser::treatRelaxationCoefficient(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    this->checkNotEndOfFile("MFrontImplicitParser::treatRelaxationCoefficient",
			    "Cannot read relaxation coefficient value.");
    if(!this->useRelaxation){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatRelaxationCoefficient",
			      "relaxation unused");
    }
    istringstream flux(current->value);
    double relaxationCoefficient;
    flux >> relaxationCoefficient;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParser::treatRelaxationCoefficient",
			      "Failed to read relaxation coefficient value.");
    }
    if(relaxationCoefficient<0){
      this->throwRuntimeError("MFrontImplicitParser::treatRelaxationCoefficient",
			      "relaxation coefficient value must be positive.");
    }
    if(!this->parametersDefaultValues.insert(MVType("relaxationCoefficient",
						    relaxationCoefficient)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatRelaxationCoefficient",
			      "default value already defined for parameter 'relaxationCoefficient'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParser::treatRelaxationCoefficient",";");    
  } // end of MFrontImplicitParser::treatRelaxationCoefficient

  void MFrontImplicitParser::treatAlgorithm(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParser::treatAlgorithm",
			    "Cannot read algorithm name.");
    if(this->algorithm!=MFrontImplicitParser::DEFAULT){
      this->throwRuntimeError("MFrontImplicitParser::treatAlgorithm",
			      "@Algorithm has already been read.");
    }
    if(this->current->value=="NewtonRaphson"){
      this->algorithm = MFrontImplicitParser::NEWTONRAPHSON;
    } else if(this->current->value=="NewtonRaphson_NumericalJacobian"){
      this->algorithm = MFrontImplicitParser::NEWTONRAPHSON_NR;
    } else if(this->current->value=="Broyden"){
      this->algorithm = MFrontImplicitParser::BROYDEN;
    } else if(this->current->value=="Broyden2"){
      this->algorithm = MFrontImplicitParser::BROYDEN2;
    } else {
      this->throwRuntimeError("MFrontImplicitParser::treatAlgorithm : ",
			      this->current->value+" is not a valid algorithm name\n"
			      "Supported algorithms are : NewtonRaphson, NewtonRaphson_NumericalJacobian, Broyden, and Broyden2.");
    }
    ++this->current;
    this->readSpecifiedToken("MFrontImplicitParser::treatAlgorithm",";");
  } // end of MFrontImplicitParser::treatAlgorithm

  void
  MFrontImplicitParser::treatTheta(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    double theta;
    this->checkNotEndOfFile("MFrontImplicitParser::treatTheta",
			    "Cannot read theta value.");
    istringstream flux(current->value);
    flux >> theta;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParser::treatTheta",
			      "Failed to read theta value.");
    }
    if((theta<0.)||(theta>1.)){
      this->throwRuntimeError("MFrontImplicitParser::treatTheta",
			      "Theta value must be positive and smaller than 1.");
    }
    if(!this->parametersDefaultValues.insert(MVType("theta",theta)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatTheta",
			      "default value already defined for parameter 'theta'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParser::treatTheta",";");
  } // end of MFrontImplicitParser::treatTheta

  void
  MFrontImplicitParser::treatEpsilon(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    double epsilon;
    this->checkNotEndOfFile("MFrontImplicitParser::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParser::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("MFrontImplicitParser::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    if(!this->parametersDefaultValues.insert(MVType("epsilon",epsilon)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatEpsilon",
			      "default value already defined for parameter 'epsilon'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParser::treatEpsilon",";");
  } // MFrontImplicitParser::treatEpsilon

  void
  MFrontImplicitParser::treatIterMax(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short iterMax;
    iterMax = this->readUnsignedShort("MFrontImplicitParser::treatIterMax");
    if(iterMax==0){
      this->throwRuntimeError("MFrontImplicitParser::treatIterMax",
			      "invalid value for parameter 'iterMax'");
    }
    if(!this->uParametersDefaultValues.insert(MVType("iterMax",iterMax)).second){
      this->throwRuntimeError("MFrontImplicitParser::treatIterMax",
			      "default value already defined for parameter 'iterMax'");
    }
    this->readSpecifiedToken("MFrontImplicitParser::treatIterMax",";");
  } // end of MFrontImplicitParser::treatIterMax

  void
  MFrontImplicitParser::setInterfaces(const std::set<std::string>& i)
  {
    MFrontBehaviourParserBase<MFrontImplicitParser>::setInterfaces(i);
  } // end of MFrontImplicitParser::setInterfaces

  void
  MFrontImplicitParser::treatStateVariables(void)
  {
    using namespace std;
    if((!this->integrator.empty())||
       (!this->computeStress.empty())){
      string msg("MFrontImplicitParser::treatStateVariables : ");
      msg += "state variables shall be defined before the @Integrator and @ComputeStress blocks";
      throw(runtime_error(msg));
    }
    this->readVarList(this->stateVarsHolder,false,true);
  } // end of MFrontImplicitParser::treatStateVariables

  void
  MFrontImplicitParser::treatAuxiliaryStateVariables(void)
  {
    using namespace std;
    if((!this->integrator.empty())||
       (!this->computeStress.empty())){
      string msg("MFrontImplicitParser::treatStateVariables : ");
      msg += "state variables shall be defined before the @Integrator and @ComputeStress blocks";
      throw(runtime_error(msg));
    }
    this->readVarList(this->auxiliaryStateVarsHolder,true,true);
  } // end of MFrontImplicitParser::treatAuxiliaryStateVariables

  std::string
  MFrontImplicitParser::variableModifier1(const std::string& var,
					  const bool addThisPtr)
  {
    if((var=="eto")||(var=="T")||
       (this->isInternalStateVariable(var))||
       (this->isExternalStateVariable(var))){
      if(addThisPtr){
	return "(this->"+var+"+(this->theta)*(this->d"+var+"))";
      } else {
	return "("+var+"+(this->theta)*d"+var+")";
      }
    }
    if((this->isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontImplicitParser::variableModifier1

  std::string
  MFrontImplicitParser::variableModifier2(const std::string& var,
					  const bool addThisPtr)
  {
    if((var=="eto")||(var=="T")||(this->isExternalStateVariable(var))){
      if(addThisPtr){
	return "(this->"+var+"+this->d"+var+")";
      } else {
	return "("+var+"+d"+var+")";
      }
    }
    if((this->isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontImplicitParser::variableModifier2

  void
  MFrontImplicitParser::treatPredictor(void)
  {
    using namespace std;
    if(!this->predictor.empty()){
      string msg("MFrontImplicitParser::treatPredictor : ");
      msg += "@Predictor already called";
      throw(runtime_error(msg));
    }
    this->predictor  = this->readNextBlock(true);
    this->predictor += "\n";
  } // end of MFrontImplicitParser::treatPredictor

  void
  MFrontImplicitParser::treatComputeStress(void)
  {
    using namespace std;
    if(!this->computeStress.empty()){
      string msg("MFrontImplicitParser::treatComputeStress : ");
      msg += "computeStress already defined";
      throw(runtime_error(msg));
    }
    this->readNextBlock(this->computeStress,
			this->computeFinalStress,
			&ParserBase::variableModifier1,
			&ParserBase::variableModifier2,
			true);
  } // end of MFrontImplicitParser::treatStateVariables

  std::string
  MFrontImplicitParser::getName(void)
  {
    return "Implicit";
  }

  std::string
  MFrontImplicitParser::getDescription(void)
  {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of MFrontImplicitParser::getDescription

  void
  MFrontImplicitParser::setVerboseMode(void) 
  {
    this->verboseMode = true;
  }

  void
  MFrontImplicitParser::setDebugMode(void) 
  {
    this->debugMode = true;
  }

  void
  MFrontImplicitParser::setWarningMode(void) 
  {
    this->warningMode = true;
  }

  void
  MFrontImplicitParser::treatFile(const std::string& fileName_) 
  {
    this->fileName = fileName_;
    MFrontBehaviourParserBase<MFrontImplicitParser>::treatFile(fileName_);
  }

  void MFrontImplicitParser::writeBehaviourParserSpecificIncludes(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "#include<limits>\n";
    this->behaviourFile << "#include<algorithm>\n\n";
    this->behaviourFile << "#include\"TFEL/Math/st2tost2.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/tmatrix.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/tvector.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/TinyMatrixSolve.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/Stensor/SFTMCV.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/Stensor/SFTMRV.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/Stensor/SFTVV.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/ST2toST2/ST2toST2FTMV.hxx\"\n\n";
  } // end of MFrontImplicitParser::writeBehaviourParserSpecificIncludes(void)

  void MFrontImplicitParser::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    MFrontBehaviourParserCommon::writeBehaviourParserSpecificTypedefs();
  } // end of MFrontImplicitParser::writeBehaviourParserSpecificTypedefs(void)

  void
  MFrontImplicitParser::writeBehaviourParserSpecificMembers(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "\n// Jacobian\n";
    this->behaviourFile << "tfel::math::tmatrix<" << n << "," << n << ",Type> jacobian;\n";
    this->behaviourFile << "// zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> zeros;\n\n";
    this->behaviourFile << "// previous zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> zeros_1;\n\n";
    this->behaviourFile << "// function\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> fzeros;\n\n";
    if(this->useAcceleration){
      this->behaviourFile << "// previous results (used for acceleration)\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type> previous_fzeros[3];\n\n";
      this->behaviourFile << "// previous zeros (used for acceleration)\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type> previous_zeros[3];\n\n";
    }
    this->behaviourFile << "// number of iterations\n";
    this->behaviourFile << "unsigned int iter;\n\n";
    // castem acceleration
    if(this->useAcceleration){
      this->behaviourFile << "static bool\n";
      this->behaviourFile << "schmidt(";
      this->behaviourFile << "Type& k0,\n";
      this->behaviourFile << "Type& k1,\n";
      this->behaviourFile << "Type& k2,\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type>& e0,\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type>& e1,\n";
      this->behaviourFile << "const tfel::math::tvector<" << n << ",Type>& v0,\n";
      this->behaviourFile << "const tfel::math::tvector<" << n << ",Type>& v1,\n";
      this->behaviourFile << "const tfel::math::tvector<" << n << ",Type>& v2)\n";
      this->behaviourFile << "{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "Type n; // temporary used for norm computation\n";
      this->behaviourFile << "Type m; // temporary\n";
      this->behaviourFile << "typename tvector<" << n << ",Type>::size_type i;\n";
      this->behaviourFile << "for(i=0;i!=N;++i){\n";
      this->behaviourFile << "m = abs(v0(i))+abs(v1(i))+abs(v2(i));\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "// base du plan\n";
      this->behaviourFile << "e0 = v1-v0;\n";
      this->behaviourFile << "n = norm(e0);\n";
      this->behaviourFile << "if((n<m*numeric_limits<Type>::epsilon())||\n";
      this->behaviourFile << "(n<10*numeric_limits<Type>::min())){\n";
      this->behaviourFile << "return false;\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "k0  = Type(1)/n;\n";
      this->behaviourFile << "e0 *= k0;\n";
      this->behaviourFile << "k1  = (v2-v0)|e0;\n";
      this->behaviourFile << "e1  = (v2-v0)-k1*e0;\n";
      this->behaviourFile << "n = norm(e1);\n";
      this->behaviourFile << "if((n<m*numeric_limits<Type>::epsilon())||\n";
      this->behaviourFile << "(n<10*numeric_limits<Type>::min())){\n";
      this->behaviourFile << "return false;\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "k2  = Type(1)/n;\n";
      this->behaviourFile << "e1 *= k2;\n";
      this->behaviourFile << "k1 *= -k2*k0;\n";
      this->behaviourFile << "return true;\n";
      this->behaviourFile << "} // end of schmidt\n\n";
    
      this->behaviourFile << "void\n";
      this->behaviourFile << "accelerate(tfel::math::tvector<" << n << ",Type>& r)";
      this->behaviourFile << "{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type> accelearate_re0;\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type> accelearate_re1;\n";
      this->behaviourFile << "Type accelearate_k0;\n";
      this->behaviourFile << "Type accelearate_k1;\n";
      this->behaviourFile << "Type accelearate_k2;\n";
      this->behaviourFile << "if(!schmidt(accelearate_k0,\n"
			  << "accelearate_k1,\n" 
			  << "accelearate_k2,\n"
			  << "accelearate_re0,accelearate_re1,previous_fzeros[0],\n"
			  << "previous_fzeros[1],previous_fzeros[2])){\n";
      this->behaviourFile << "return;\n"; 
      this->behaviourFile << "}\n";
      this->behaviourFile << "const Type accelearate_c0 = -(previous_fzeros[0]|accelearate_re0);\n";
      this->behaviourFile << "const Type accelearate_c1 = -(previous_fzeros[0]|accelearate_re1);\n";
      this->behaviourFile << "const Type accelearate_r1 = (accelearate_c0*accelearate_k0+accelearate_c1*accelearate_k1);\n";
      this->behaviourFile << "const Type accelearate_r2 = accelearate_c1*accelearate_k2;\n";
      this->behaviourFile << "const Type accelearate_r0 = (1-accelearate_r1-accelearate_r2)\n;";
      this->behaviourFile << "r = accelearate_r0*previous_zeros[0]+accelearate_r1*previous_zeros[1]+accelearate_r2*previous_zeros[2];\n";
      this->behaviourFile << "} // end of accelate\n\n";
    }
    // compute the numerical part of the jacobian
    if((this->compareToNumericalJacobian)||(this->algorithm==BROYDEN)||
       (this->algorithm==MFrontImplicitParser::NEWTONRAPHSON_NR)){
      this->writeComputeNumericalJacobian();
    }
    if((this->algorithm==MFrontImplicitParser::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParser::NEWTONRAPHSON_NR)){
      this->writeGetPartialJacobianInvert();
    }
    // compute stress
    this->behaviourFile << "void\ncomputeStress(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("MFrontImplicitParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);
    this->behaviourFile << this->computeStress << endl;
    this->behaviourFile << "} // end of " << this->className << "::computeStress\n\n";
    this->behaviourFile << "void\ncomputeFinalStress(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("MFrontImplicitParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);
    this->behaviourFile << this->computeFinalStress << endl;
    this->behaviourFile << "} // end of " << this->className << "::computeStress\n\n";
    this->behaviourFile << endl;
  } // end of MFrontImplicitParser::writeBehaviourParserSpecificMembers

  std::string
  MFrontImplicitParser::getJacobianPart(const VarHandler&  v1,
					const VarHandler&  v2,
					const SupportedTypes::TypeSize& n,
					const SupportedTypes::TypeSize& n2,
					const SupportedTypes::TypeSize& n3,
					const std::string& j,
					const std::string& p)
  {
    using namespace std;
    ostringstream d;
    if(this->varNames.find(p+"df"+v1.name+"_dd"+v2.name)!=this->varNames.end()){
      string msg("MFrontImplicitParser::writeBehaviourIntegrator : ");
      msg += "variable name 'df"+v1.name+"_dd"+v2.name;
      msg += "' is reserved.\n";
      throw(runtime_error(msg));
    }
    if(this->getTypeFlag(v1.type)==SupportedTypes::Stensor){
      if(this->getTypeFlag(v2.type)==SupportedTypes::Stensor){
	d << "typename tfel::math::ST2toST2FTMV<N,"
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name << "_dd" << v2.name << "("+j+");\n";
      } else if(this->getTypeFlag(v2.type)==SupportedTypes::Scalar){
	d << "typename tfel::math::SFTMCV<N," 
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name << "_dd" << v2.name << "("+j+");\n";
      } else {
	string msg("MFrontImplicitParser::writeOutputFiles : ");
	msg += "unsupported type for state variable ";
	msg += v2.name;
	throw(runtime_error(msg));
      }
    } else if(this->getTypeFlag(v1.type)==SupportedTypes::Scalar){
      if(this->getTypeFlag(v2.type)==SupportedTypes::Stensor){
	d << "typename tfel::math::SFTMRV<N,"
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name 
	  << "_dd" << v2.name << "("+j+");\n";
      } else if(this->getTypeFlag(v2.type)==SupportedTypes::Scalar){
	d << "real& "+p+"df" << v1.name << "_dd" << v2.name 
	  << " = "+j+"(" << n << "," << n3 << ");\n";
      } else {
	string msg("MFrontImplicitParser::writeOutputFiles : ");
	msg += "unsupported type for state variable ";
	msg += v2.name;
	throw(runtime_error(msg));
      }
    } else {
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "unsupported type for state variable ";
      msg += v1.name;
      throw(runtime_error(msg));
    }
    return d.str();
  } // void MFrontImplicitParser::getJacobianPart

  void
  MFrontImplicitParser::writeGetPartialJacobianInvert(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    SupportedTypes::TypeSize n;
    VarContainer::const_iterator p;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "void\ngetPartialJacobianInvert(tfel::math::tmatrix<StensorSize,StensorSize,real>& partial_jacobian)\n"
			<< "{\n";
    this->behaviourFile << "using namespace tfel::math;" << endl;
    this->behaviourFile << "TinyPermutation<" << n << "> permuation;" << endl;
    if(this->algorithm==MFrontImplicitParser::NEWTONRAPHSON_NR){
      this->behaviourFile << "this->computeNumericalJacobian(this->jacobian);\n";
    }
    this->behaviourFile << "TinyMatrixSolve<" << n << ",real>::decomp(this->jacobian,permuation);" << endl;
    this->behaviourFile << "for(unsigned short idx=0;idx!=StensorSize;++idx){\n";
    this->behaviourFile << "tvector<" << n << ",real> vect_e(real(0));" << endl;
    this->behaviourFile << "vect_e(idx) = real(1);" << endl;
    this->behaviourFile << "TinyMatrixSolve<" << n << ",real>::back_substitute(this->jacobian,permuation,vect_e);" << endl;
    this->behaviourFile << "for(unsigned short idx2=0;idx2!=StensorSize;++idx2){\n";
    this->behaviourFile << "partial_jacobian(idx2,idx)=vect_e(idx2);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
  } // end of MFrontImplicitParser::writeGetPartialJacobianInvert
  
  void MFrontImplicitParser::writeComputeNumericalJacobian(){
    using namespace std;
    VarContainer::const_iterator p;
    VarContainer::const_iterator p2;
    SupportedTypes::TypeSize n;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    this->checkBehaviourFile();
    this->behaviourFile << "void\ncomputeNumericalJacobian(tfel::math::tmatrix<" << n << "," << n << ",real>& njacobian)\n"
			<< "{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "tvector<" << n << ",real> tzeros(this->zeros);\n";
    this->behaviourFile << "tvector<" << n << ",real> tfzeros(this->fzeros);\n";
    this->behaviourFile << "tmatrix<" << n << "," << n << ",real> tjacobian(this->jacobian);\n";
    this->behaviourFile << "for(unsigned short idx = 0; idx!= "<< n<<  ";++idx){\n";
    this->behaviourFile << "this->zeros(idx) -= 10.*(this->epsilon);\n";
    this->behaviourFile << "this->computeStress();\n";
    this->behaviourFile << "this->computeFdF();\n";
    this->behaviourFile << "this->zeros = tzeros;\n";
    this->behaviourFile << "tvector<" << n << ",real> tfzeros2(this->fzeros);\n";
    // if(this->algorithm!=MFrontImplicitParser::NEWTONRAPHSON){
    //   this->behaviourFile << "if(&njacobian!=&(this->jacobian)){\n";
    //   this->behaviourFile << "this->jacobian = tjacobian;\n";
    //   this->behaviourFile << "}\n";
    // }
    this->behaviourFile << "this->zeros(idx) += 10.*(this->epsilon);\n";
    this->behaviourFile << "this->computeStress();\n";
    this->behaviourFile << "this->computeFdF();\n";
    this->behaviourFile << "this->fzeros = (this->fzeros-tfzeros2)/(20.*(this->epsilon));\n";
    this->behaviourFile << "for(unsigned short idx2 = 0; idx2!= "<< n <<  ";++idx2){\n";
    this->behaviourFile << "njacobian(idx2,idx) = this->fzeros(idx2);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->zeros    = tzeros;\n";
    this->behaviourFile << "this->fzeros   = tfzeros;\n";
    //    if(this->algorithm!=MFrontImplicitParser::NEWTONRAPHSON){
    // this->behaviourFile << "if(&njacobian!=&(this->jacobian)){\n";
    // this->behaviourFile << "this->jacobian = tjacobian;\n";
    // this->behaviourFile << "}\n";
    //}
    this->behaviourFile << "}\n";
    //    if(this->algorithm!=MFrontImplicitParser::NEWTONRAPHSON){
    this->behaviourFile << "if(&njacobian!=&(this->jacobian)){\n";
    this->behaviourFile << "this->jacobian = tjacobian;\n";
    this->behaviourFile << "}\n";
    //    }
    this->behaviourFile << "}\n\n";
  }

  void MFrontImplicitParser::writeBehaviourIntegrator(){
    using namespace std;
    VarContainer::const_iterator p;
    VarContainer::const_iterator p2;
    vector<BoundsDescription>::const_iterator p3;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "bool\nintegrate(const bool computeTangentOperator_){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    if(this->compareToNumericalJacobian){
      this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> njacobian;\n";
    }
    if((this->algorithm==MFrontImplicitParser::BROYDEN)||
       (this->algorithm==MFrontImplicitParser::BROYDEN2)){
      this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> jacobian2;\n";
      this->behaviourFile << "tvector<" << n2 << ",real> fzeros2;\n";
      this->behaviourFile << "tvector<" << n2 << ",real> Dzeros;\n";
    }
    if(this->algorithm==MFrontImplicitParser::BROYDEN2){
      this->behaviourFile << "tvector<" << n2 << ",real> Dfzeros;\n";
    }
    if((this->algorithm==MFrontImplicitParser::BROYDEN)||
       (this->algorithm==MFrontImplicitParser::BROYDEN2)){
      this->behaviourFile << "real broyden_inv;\n";
    }
    this->behaviourFile << "real error;\n";
    this->behaviourFile << "bool converge=false;\n";
    if((this->algorithm==MFrontImplicitParser::BROYDEN)||
       (this->algorithm==MFrontImplicitParser::BROYDEN2)){
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
    }
    this->behaviourFile << "this->iter=0;\n";
    if(this->debugMode){
      this->behaviourFile << "cout << endl << \"" << this->className
			  << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile << "while((converge==false)&&\n";
    this->behaviourFile << "(this->iter<" << this->className << "::iterMax)){\n";
    this->behaviourFile << "++(this->iter);\n";
    this->behaviourFile << "this->zeros_1  = this->zeros;\n";
    if(this->algorithm==MFrontImplicitParser::NEWTONRAPHSON){
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->algorithm==MFrontImplicitParser::NEWTONRAPHSON_NR){
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->algorithm==MFrontImplicitParser::BROYDEN){
      this->behaviourFile << "Dzeros = -this->fzeros;\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "try{" << endl;
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(jacobian2,Dzeros);\n";
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "catch(LUException&){" << endl;
      this->behaviourFile << "return false;\n";
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "this->zeros  += Dzeros;\n";
      this->behaviourFile << "fzeros2 = this->fzeros;\n";
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->algorithm==MFrontImplicitParser::BROYDEN2){
      this->behaviourFile << "Dzeros   = -(this->jacobian)*(this->fzeros);\n";
      this->behaviourFile << "this->zeros  += Dzeros;\n";
      this->behaviourFile << "fzeros2 = this->fzeros;\n";
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->compareToNumericalJacobian){
      this->behaviourFile << "this->computeNumericalJacobian(njacobian);\n";
      n = SupportedTypes::TypeSize();
      for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
	n3 = SupportedTypes::TypeSize();
	for(p2=this->stateVarsHolder.begin();p2!=this->stateVarsHolder.end();++p2){
	  this->behaviourFile << "// derivative of variable f" << p->name 
			      << " by variable " << p2->name << "\n";
	  this->behaviourFile << this->getJacobianPart(*p,*p2,n,n2,n3);
	  this->behaviourFile << "// numerical derivative of variable f" << p->name 
			      << " by variable " << p2->name << "\n";
	  this->behaviourFile << this->getJacobianPart(*p,*p2,n,n2,n3,
						       "njacobian","n");
	  n3 += this->getTypeSize(p2->type,p2->arraySize);
	}
	n += this->getTypeSize(p->type,p->arraySize);
      }
      for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
	for(p2=this->stateVarsHolder.begin();p2!=this->stateVarsHolder.end();++p2){
	  const VarHandler& v1 = *p;
	  const VarHandler& v2 = *p2;
	  SupportedTypes::TypeSize nv1 = this->getTypeSize(v1.type,v1.arraySize);
	  SupportedTypes::TypeSize nv2 = this->getTypeSize(v2.type,v2.arraySize);
	  this->behaviourFile << "error=" << nv1 << "*" << nv2 << "*"
			      << "(this->jacobianComparisonCriterium)" <<";\n";
	  this->behaviourFile << "if(abs(" << "df" << v1.name  << "_dd" << v2.name << "-"
			      << "ndf" << v1.name  << "_dd" << v2.name << ") > error)\n" 
			      << "{\n";
	  this->behaviourFile << "cout << abs(" << "df" << v1.name  << "_dd" << v2.name << "-"
			      << "ndf" << v1.name  << "_dd" << v2.name << ") << \" \" << error << endl;\n";
	  this->behaviourFile << "cout << \"df" << v1.name
			      << "_dd" << v2.name << " :\\n\" << " 
			      << "df" << v1.name  << "_dd" << v2.name << " << endl;\n";
	  this->behaviourFile << "cout << \"ndf" << v1.name
			      << "_dd" << v2.name << " :\\n\" << " 
			      << "ndf" << v1.name  << "_dd" << v2.name << " << endl;\n";
	  this->behaviourFile << "cout << \"df" << v1.name << "_dd" << v2.name 
			      << " - ndf" << v1.name << "_dd" << v2.name << " :\\n\" << "
			      << "df" << v1.name  << "_dd" << v2.name << "-"
			      << "ndf" << v1.name  << "_dd" << v2.name << " << endl;\n";
	  this->behaviourFile << "cout << endl;\n";
	  this->behaviourFile << "}\n";
	}
      }
    }
    this->behaviourFile << "error=norm(this->fzeros);\n";
    this->behaviourFile << "converge = ((error)/(real(" << n2 << "))<";
    this->behaviourFile << "(this->epsilon));\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::integrate() : iteration \" "
			  << "<< this->iter << \" : \" << (error)/(real(" << n2 << ")) << endl;\n";
    }
    this->behaviourFile << "if(!converge){\n";
    if((this->algorithm==MFrontImplicitParser::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParser::NEWTONRAPHSON_NR)){
      this->behaviourFile << "try{" << endl;
      if(this->algorithm==MFrontImplicitParser::NEWTONRAPHSON_NR){
	this->behaviourFile << "this->computeNumericalJacobian(this->jacobian);\n";
      }
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(this->jacobian,this->fzeros);\n";
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "catch(LUException&){" << endl;
      this->behaviourFile << "return false;\n";
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "this->zeros -= this->fzeros;\n";
    }
    if(this->algorithm==MFrontImplicitParser::BROYDEN){
      this->behaviourFile << "broyden_inv = (Dzeros|Dzeros);\n";
      this->behaviourFile << "if(broyden_inv<100*std::numeric_limits<real>::epsilon()){\n";
      this->behaviourFile << "return false;\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "this->jacobian += "
			  << "(((this->fzeros-fzeros2)-(jacobian2)*(Dzeros))^Dzeros)/broyden_inv;\n";
    }
    if(this->algorithm==MFrontImplicitParser::BROYDEN2){
      this->behaviourFile << "Dfzeros   = (this->fzeros)-fzeros2;\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "broyden_inv = Dzeros|jacobian2*Dfzeros;\n";
      this->behaviourFile << "if(broyden_inv<100*std::numeric_limits<real>::epsilon()){\n";
      this->behaviourFile << "return false;\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "this->jacobian += "
			  << "((Dzeros-jacobian2*Dfzeros)^(Dzeros*jacobian2))/(broyden_inv);\n";
    }
    if(this->useAcceleration){
      this->behaviourFile << "this->previous_fzeros[this->iter%3] = this->fzeros;\n";
      this->behaviourFile << "this->previous_zeros[this->iter%3]  = this->zeros;\n";
      this->behaviourFile << "if((this->iter>this->accelerationTrigger" << ")&&\n"
			  <<  "((this->iter-this->accelerationTrigger)%"
			  << "(this->accelerationPeriod)==0)){\n";
      this->behaviourFile << "this->accelerate(this->zeros);\n";
      this->behaviourFile << "}\n";
    }
    if(this->useRelaxation){
      this->behaviourFile << "if(this->iter>=this->relaxationTrigger" << "){\n";
      this->behaviourFile << "this->zeros   -= (1-this->relaxationCoefficient) * (this->zeros-this->zeros_1);\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(this->iter==this->iterMax){\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::integrate() : no convergence after \" "
			  << "<< this->iter << \" iterations\"<< endl << endl;\n";
      this->behaviourFile << "cout << *this << endl;\n";
    }
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::integrate() : convergence after \" "
			  << "<< this->iter << \" iterations\"<< endl << endl;\n";
    }
    this->behaviourFile << "if(computeTangentOperator_){\n";
    this->behaviourFile << "if(!this->computeConsistantTangentOperator()){\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->updateStateVars();\n";
    this->behaviourFile << "this->computeFinalStress();\n";
    this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    for(p3  = this->boundsDescriptions.begin();
	p3 != this->boundsDescriptions.end();++p3){
      if(p3->varCategory==BoundsDescription::StateVar){
	p3->writeBoundsChecks(this->behaviourFile);
      }
    }
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "} // end of " << this->className << "::integrate\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief compute fzeros and jacobian\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "computeFdF(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("MFrontImplicitParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);
    n = SupportedTypes::TypeSize();
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      if(this->varNames.find("f"+p->name)!=this->varNames.end()){
	string msg("MFrontImplicitParser::writeBehaviourIntegrator : ");
	msg += "variable name 'f"+p->name;
	msg += "' is reserved.\n";
	throw(runtime_error(msg));
      }
      if(this->getTypeFlag(p->type)==SupportedTypes::Stensor){
	this->behaviourFile << "typename tfel::math::SFTVV<N," << n2 
			    << "," << n << ",real>::type f" << p->name << "(this->fzeros);\n";
      } else if(this->getTypeFlag(p->type)==SupportedTypes::Scalar){
	this->behaviourFile << "real& f" << p->name << "= this->fzeros(" << n << ");\n";
      } else {
	string msg("MFrontImplicitParser::writeOutputFiles : ");
	msg += "unsupported type for state variable ";
	msg += p->name;
	throw(runtime_error(msg));
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
    if((this->algorithm==MFrontImplicitParser::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParser::BROYDEN)){
      n = SupportedTypes::TypeSize();
      for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
	n3 = SupportedTypes::TypeSize();
	for(p2=this->stateVarsHolder.begin();p2!=this->stateVarsHolder.end();++p2){
	  this->behaviourFile << "// derivative of variable f" << p->name 
			      << " by variable " << p2->name << "\n";
	  this->behaviourFile << this->getJacobianPart(*p,*p2,n,n2,n3);
	  n3 += this->getTypeSize(p2->type,p2->arraySize);
	}
	n += this->getTypeSize(p->type,p->arraySize);
      }
    }
    if(this->algorithm==MFrontImplicitParser::NEWTONRAPHSON){
      this->behaviourFile << "// setting jacobian to identity\n";
      this->behaviourFile << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n";
      this->behaviourFile << "for(unsigned short idx = 0; idx!=" << n << ";++idx){\n"
			  << "this->jacobian(idx,idx)= real(1);\n"
			  << "}\n";
    }
    this->behaviourFile << "// setting f values to zeros\n";
    this->behaviourFile << "this->fzeros = this->zeros;\n";
    this->behaviourFile << this->integrator;
    this->behaviourFile << "\n";
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      if(enf.find(p->name)!=enf.end()){
    	this->behaviourFile << "f" << p->name << "*= real(1)/(" << enf.find(p->name)->second << ");" << endl;
	if(this->algorithm==MFrontImplicitParser::NEWTONRAPHSON){
#warning "check for broyden !"
	  for(p2=this->stateVarsHolder.begin();p2!=this->stateVarsHolder.end();++p2){
	    this->behaviourFile << "df" << p->name << "_dd" << p2->name
				<< "*= real(1)/(" << enf.find(p->name)->second << ");" << endl;
	  }
	}
      }
    }
    if((this->algorithm==MFrontImplicitParser::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParser::BROYDEN)){
      for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
	for(p2=this->stateVarsHolder.begin();p2!=this->stateVarsHolder.end();++p2){
	  this->behaviourFile << "static_cast<void>(df"
			      << p->name << "_dd" << p2->name
			      << "); /* suppress potential warnings */\n";
	}
      }
    }
    this->behaviourFile << "}\n\n";
  } // end of MFrontImplicitParser::writeBehaviourIntegrator

  void MFrontImplicitParser::writeBehaviourComputeTangentOperator(void)
  {
    if(this->hasConsistantTangentOperator){
      this->behaviourFile << "bool computeConsistantTangentOperator(){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      writeMaterialLaws("MFrontImplicitParser::writeBehaviourIntegrator",
			this->behaviourFile,this->materialLaws);
      this->behaviourFile << this->tangentOperator;
      this->behaviourFile << "return true;\n";
      this->behaviourFile << "}\n\n";
    } else {
      MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator();
    }
  }

  void MFrontImplicitParser::writeBehaviourConstructors(void)
  {    
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    ostringstream initStateVars;
    ostringstream initComputedVars;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = getTypeFlag(p->type);
      initStateVars << ",\n";
      switch(flag){
      case SupportedTypes::Scalar : 
	initStateVars << "d" << p->name << "(this->zeros(" << n << "))";
	break;
      case SupportedTypes::Stensor :
	initStateVars << "d" << p->name 
		      << "(this->zeros)";
	break;
      default :
	string msg("MFrontImplicitParser::writeBehaviourConstructors : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
    initStateVars << ",\nzeros(real(0)),\nfzeros(real(0))";
    if(!this->computedVars.empty()){
      ComputedVarContainer::const_iterator p2;
      for(p2=this->computedVars.begin();p2!=this->computedVars.end();++p2){
	initComputedVars << ",\n";
	initComputedVars << p2->name << "(" << p2->description << ")" ;
      }
    }
    MFrontBehaviourParserCommon::writeBehaviourConstructors(initStateVars.str(),
							    initComputedVars.str(),
							    this->predictor);
  }

  void MFrontImplicitParser::writeBehaviourParserSpecificConstructorPart(void)
  {
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    this->checkBehaviourFile();
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    if(!this->initJacobian.empty()){
      this->behaviourFile << this->initJacobian;
    } else {
      if((this->algorithm==MFrontImplicitParser::BROYDEN)||
	 (this->algorithm==MFrontImplicitParser::BROYDEN2)){
	this->behaviourFile << "// setting jacobian to identity\n";
	this->behaviourFile << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n";
	this->behaviourFile << "for(unsigned short idx = 0; idx!= "<< n << ";++idx){\n";
	this->behaviourFile << "this->jacobian(idx,idx)= real(1);\n";
	this->behaviourFile << "}\n";
      }
    }
  }

  void MFrontImplicitParser::writeBehaviourStateVarsIncrements(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      if((!this->debugMode)&&(p->lineNumber!=0u)){
	this->behaviourFile << "#line " << p->lineNumber << " \"" 
			    << this->fileName << "\"\n";
      }
      if(this->getTypeFlag(p->type)==SupportedTypes::Stensor){
	this->behaviourFile << "typename tfel::math::SFTVV<N," 
			    << n2 << "," << n << ",real>::type"
			    << " d" << p->name << ";\n";
      } else if(this->getTypeFlag(p->type)==SupportedTypes::Scalar){
	this->behaviourFile << "real& d" << p->name << ";\n";
      } else {
	string msg("MFrontImplicitParser::writeBehaviourStateVarsIncrements :");
	msg += "unsupported type '"+p->type+"'";
	throw(runtime_error(msg));
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << endl;
  }

  void
  MFrontImplicitParser::endsInputFileProcessing(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    typedef map<string,unsigned short>::value_type MVType2;
    if(this->parametersDefaultValues.find("theta")==this->parametersDefaultValues.end()){
      this->parametersDefaultValues.insert(MVType("theta",0.5));
    }
    if(this->parametersDefaultValues.find("epsilon")==this->parametersDefaultValues.end()){
      this->parametersDefaultValues.insert(MVType("epsilon",1.e-8));
    }
    if(this->uParametersDefaultValues.find("iterMax")==this->uParametersDefaultValues.end()){
      this->uParametersDefaultValues.insert(MVType2("iterMax",100u));
    }
    unsigned short iterMax = this->uParametersDefaultValues["iterMax"];
    if(this->compareToNumericalJacobian){
      
      if(this->parametersDefaultValues.find("jacobianComparisonCriterium")==this->parametersDefaultValues.end()){
	double epsilon = this->parametersDefaultValues["epsilon"];
	this->parametersDefaultValues.insert(MVType("jacobianComparisonCriterium",epsilon));
      }
    }
    if(this->useRelaxation){
      if(this->parametersDefaultValues.find("relaxationCoefficient")==this->parametersDefaultValues.end()){
	this->parametersDefaultValues.insert(MVType("relaxationCoefficient",0.5));
      }
      if(this->uParametersDefaultValues.find("relaxationTrigger")==this->uParametersDefaultValues.end()){
	this->uParametersDefaultValues.insert(MVType2("relaxationTrigger",10u));
      }
      unsigned short relaxationTrigger = this->uParametersDefaultValues["relaxationTrigger"];
      if(relaxationTrigger+1>=iterMax){
	string msg("MFrontImplicitParser::endsInputFileProcessing :");
	msg += "relaxation can never take place (relaxationTrigger>=iterMax-1)'";
	throw(runtime_error(msg));
      }
    }
    if(this->useAcceleration){
      if(this->uParametersDefaultValues.find("accelerationTrigger")==this->uParametersDefaultValues.end()){
	this->uParametersDefaultValues.insert(MVType2("accelerationTrigger",10u));
      }
      if(this->uParametersDefaultValues.find("accelerationPeriod")==this->uParametersDefaultValues.end()){
	this->uParametersDefaultValues.insert(MVType2("accelerationPeriod",3u));
      }
      unsigned short accelerationTrigger = this->uParametersDefaultValues["accelerationTrigger"];
      if(accelerationTrigger+1>=iterMax){
	string msg("MFrontImplicitParser::endsInputFileProcessing :");
	msg += "acceleration can never take place (accelerationTrigger>=iterMax-1)'";
	throw(runtime_error(msg));
      }
    }
    if(this->algorithm==MFrontImplicitParser::DEFAULT){
      this->algorithm=MFrontImplicitParser::NEWTONRAPHSON;
    }
    if(this->compareToNumericalJacobian){
      if(this->algorithm!=MFrontImplicitParser::NEWTONRAPHSON){
	string msg("MFrontImplicitParser::endsInputFileProcessing :");
	msg += "@CompareToNumericalJacobian can only be used with the NewtonRaphson algorithm";
	throw(runtime_error(msg));

      }
    }
  } // end of MFrontImplicitParser::endsInputFileProcessing(void)

  void 
  MFrontImplicitParser::writeBehaviourStaticVars(void)
  {
    MFrontBehaviourParserBase<MFrontImplicitParser>::writeBehaviourStaticVars();
  } // end of MFrontImplicitParser::writeBehaviourStaticVars

  void 
  MFrontImplicitParser::generateOutputFiles(void){
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    if(this->integrator.empty()){
      string msg("MFrontImplicitParser::generateOutputFiles : ");
      msg += "definining the @Integrator block is required";
      throw(runtime_error(msg));
    }
    if(this->computeStress.empty()){
      string msg("MFrontImplicitParser::generateOutputFiles : ");
      msg += "definining the @ComputeStress block is required";
      throw(runtime_error(msg));
    }
    // Adds some stuff
    this->endsInputFileProcessing();
    // Generating BehaviourData's outputFile
    this->writeBehaviourDataFile();
    // Generating IntegrationData's outputFile
    this->writeIntegrationDataFile();
    // Generating Behaviour's outputFile
    this->writeBehaviourFile();
    // Generating Behaviour's outputFile
    this->writeSrcFile();
    // Generating Behaviour's unary loading tests
    this->writeUnaryLoadingTestFiles();

    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin(); i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->endTreatement(this->fileName,
			       this->library,
			       this->material,
			       this->className,
    			       this->authorName,
    			       this->date,
    			       this->coefsHolder,
    			       this->stateVarsHolder,
    			       this->auxiliaryStateVarsHolder,
    			       this->externalStateVarsHolder,
			       this->parametersHolder,
			       this->glossaryNames,
			       this->entryNames,
    			       this->behaviourCharacteristic);
    }
  }

  void
  MFrontImplicitParser::writeOutputFiles() 
  {    
    using namespace std;
    using namespace tfel::system;
    systemCall::mkdir("include");
    systemCall::mkdir("include/TFEL/");
    systemCall::mkdir("include/TFEL/Material/");
    systemCall::mkdir("src");
    if(this->className.empty()){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->behaviourFileName  = this->className;
    this->behaviourFileName += ".hxx";
    this->behaviourFile.open(("include/TFEL/Material/"+this->behaviourFileName).c_str());
    if(!this->behaviourFile){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->behaviourDataFileName  = this->className;
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->behaviourDataFile.open(("include/TFEL/Material/"+this->behaviourDataFileName).c_str());
    if(!this->behaviourDataFile){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->integrationDataFileName  = this->className;
    this->integrationDataFileName += "IntegrationData.hxx";
    this->integrationDataFile.open(("include/TFEL/Material/"+this->integrationDataFileName).c_str());
    if(!this->integrationDataFile){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->integrationDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFileName  = this->className;
    this->srcFileName += ".cxx";
    this->srcFile.open(("src/"+this->srcFileName).c_str());
    if(!this->srcFile){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->generateOutputFiles();
  }

  MFrontImplicitParser::~MFrontImplicitParser()
  {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();    
  } // end of ~MFrontImplicitParser

  std::map<std::string,std::vector<std::string> >
  MFrontImplicitParser::getGlobalIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGlobalIncludes();
  } // end of MFrontMFrontImplicitParser::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontImplicitParser::getGlobalDependencies(void)
  {
    return MFrontBehaviourParserCommon::getGlobalDependencies();
  } // end of MFrontMFrontImplicitParser::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontImplicitParser::getGeneratedSources(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedSources();
  } // end of MFrontImplicitParser::getGeneratedSources

  std::vector<std::string>
  MFrontImplicitParser::getGeneratedIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedIncludes();
  } // end of MFrontImplicitParser::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontImplicitParser::getLibrariesDependencies(void)
  {
    return MFrontBehaviourParserCommon::getLibrariesDependencies();
  } // end of MFrontImplicitParser::getLibrariesDependencies

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontImplicitParser::getSpecificTargets(void)
  {
    return MFrontBehaviourParserCommon::getSpecificTargets();
  } // end of MFrontImplicitParser::getSpecificTargets(void)

} // end of namespace mfront  

