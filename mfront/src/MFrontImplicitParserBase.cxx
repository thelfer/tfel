/*!
 * \file   MFrontImplicitParserBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2008
 */

#include<cmath>
#include<limits>
#include<cstdlib>
#include<sstream>

#include"TFEL/Utilities/ToString.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontImplicitParserBase.hxx"

namespace mfront{

  MFrontImplicitParserBase::MFrontImplicitParserBase()
    : MFrontBehaviourParserBase<MFrontImplicitParserBase>(),
      algorithm(MFrontImplicitParserBase::DEFAULT)
  {
    using namespace std;
    // dynamically allocated vectors are not yet allowed in implicit
    // parsers
    this->areDynamicallyAllocatedVectorsAllowed_ = false;
    // parameters
    this->registerVariable("theta",false);
    this->registerVariable("epsilon",false);
    this->registerVariable("iterMax",false);
    this->registerVariable("powell_dogleg_trust_region_size",false);
    this->registerVariable("numerical_jacobian_epsilon",false);
    this->registerVariable("maximum_increment_value_per_iteration",false);
    this->registerVariable("jacobianComparisonCriterium",false);
    this->registerVariable("relaxationTrigger",false);
    this->registerVariable("accelerationTrigger",false);
    this->registerVariable("accelerationPeriod",false);
    this->registerVariable("relaxationCoefficient",false);
    this->registerVariable("previous_zeros",false);
    this->registerVariable("vect_e",false);
    this->registerVariable("zeros",false);
    this->registerVariable("tzeros",false);
    this->registerVariable("zeros_1",false);
    this->registerVariable("fzeros",false);
    this->registerVariable("tfzeros",false);
    this->registerVariable("zeros2",false);
    this->registerVariable("fzeros2",false);
    this->registerVariable("Dzeros",false);
    this->registerVariable("Dfzeros",false);
    this->registerVariable("jacobian",false);
    this->registerVariable("tjacobian",false);
    this->registerVariable("njacobian",false);
    this->registerVariable("partial_jacobian",false);
    this->registerVariable("jacobian2",false);
    this->registerVariable("levmar_jacobian_1",false);
    this->registerVariable("levmar_fzeros_1",false);
    this->registerVariable("levmar_sm",false);
    this->registerVariable("levmar_mu",false);
    this->registerVariable("levmar_muF",false);
    this->registerVariable("levmar_r",false);
    this->registerVariable("levmar_p0",false);
    this->registerVariable("levmar_p1",false);
    this->registerVariable("levmar_p2",false);
    this->registerVariable("levmar_m",false);
    this->registerVariable("t",false);
    this->registerVariable("dt_",false);
    this->registerVariable("error",false);
    this->registerVariable("error_1",false);
    this->registerVariable("error_p",false);
    this->registerVariable("idx",false);
    this->registerVariable("idx2",false);
    this->registerVariable("idx3",false);
    this->reserveName("schmidt",false);
    this->reserveName("computeNumericalJacobian",false);
    this->reserveName("TinyMatrixSolve",false);
    this->reserveName("accelerate",false);
    this->reserveName("accelerate_k0",false);
    this->reserveName("accelerate_k1",false);
    this->reserveName("accelerate_k2",false);
    this->reserveName("accelerate_c0",false);
    this->reserveName("accelerate_c1",false);
    this->reserveName("accelerate_re0",false);
    this->reserveName("accelerate_re1",false);
    this->reserveName("accelerate_r0",false);
    this->reserveName("accelerate_r1",false);
    this->reserveName("accelerate_r2",false);
    this->reserveName("iter",false);
    this->reserveName("converged",false);
    this->reserveName("broyden_inv",false);
    // CallBacks
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &MFrontImplicitParserBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&MFrontImplicitParserBase::treatMaterialLaw);
    this->registerNewCallBack("@ComputeStress",&MFrontImplicitParserBase::treatComputeStress);
    this->registerNewCallBack("@ComputeFinalStress",&MFrontImplicitParserBase::treatComputeFinalStress);
    this->registerNewCallBack("@Predictor",&MFrontImplicitParserBase::treatPredictor);
    this->registerNewCallBack("@Theta",&MFrontImplicitParserBase::treatTheta);
    this->registerNewCallBack("@Epsilon",&MFrontImplicitParserBase::treatEpsilon);
    this->registerNewCallBack("@PerturbationValueForNumericalJacobianComputation",
			      &MFrontImplicitParserBase::treatPerturbationValueForNumericalJacobianComputation);
    this->registerNewCallBack("@IterMax",&MFrontImplicitParserBase::treatIterMax);
    this->registerNewCallBack("@PowellDogLegTrustRegionSize",
			      &MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize);
    this->registerNewCallBack("@MaximumNumberOfIterations",&MFrontImplicitParserBase::treatIterMax);
    this->registerNewCallBack("@Algorithm",&MFrontImplicitParserBase::treatAlgorithm);
    this->registerNewCallBack("@UseAcceleration",&MFrontImplicitParserBase::treatUseAcceleration);
    this->registerNewCallBack("@AccelerationTrigger",&MFrontImplicitParserBase::treatAccelerationTrigger);
    this->registerNewCallBack("@AccelerationPeriod",&MFrontImplicitParserBase::treatAccelerationPeriod);
    this->registerNewCallBack("@UseRelaxation",&MFrontImplicitParserBase::treatUseRelaxation);
    this->registerNewCallBack("@RelaxationTrigger",&MFrontImplicitParserBase::treatRelaxationTrigger);
    this->registerNewCallBack("@RelaxationCoefficient",&MFrontImplicitParserBase::treatRelaxationCoefficient);
    this->registerNewCallBack("@TangentOperator",
			      &MFrontImplicitParserBase::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
			      &MFrontImplicitParserBase::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@InitJacobian",
			      &MFrontImplicitParserBase::treatInitJacobian);
    this->registerNewCallBack("@CompareToNumericalJacobian",
			      &MFrontImplicitParserBase::treatCompareToNumericalJacobian);
    this->registerNewCallBack("@JacobianComparisonCriterium",
			      &MFrontImplicitParserBase::treatJacobianComparisonCriterium);
    this->registerNewCallBack("@RequireStiffnessTensor",
			      &MFrontImplicitParserBase::treatRequireStiffnessOperator);
    this->registerNewCallBack("@MaximumIncrementValuePerIteration",
			      &MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration);
    this->registerNewCallBack("@IntegrationVariable",
    			      &MFrontImplicitParserBase::treatIntegrationVariable);
    this->disableCallBack("@ComputedVar");
    this->disableCallBack("@UseQt");
  } // end of MFrontImplicitParserBase::MFrontImplicitParserBase

  void MFrontImplicitParserBase::treatStateVariable(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&MechanicalBehaviourDescription::addStateVariables,true,true,false);
  }

  void MFrontImplicitParserBase::treatIntegrationVariable(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&MechanicalBehaviourDescription::addIntegrationVariables,true,true,false);
  }

  void MFrontImplicitParserBase::treatInitJacobian(void)
  {
    using namespace std;
    if(this->algorithm!=BROYDEN){
      this->throwRuntimeError("MFrontImplicitParserBase::treatInitJacobian",
			      "@InitJacobian can only be used with "
			      "the broyden algorithm.");
    }
    this->readCodeBlock(*this,MechanicalBehaviourData::InitializeJacobian,
			&MFrontImplicitParserBase::standardModifier,true,true);
  } // end of MFrontImplicitParserBase::treatInitJacobian
  
  void MFrontImplicitParserBase::treatUnknownVariableMethod(const Hypothesis h,
							    const std::string& n)
  {
    using namespace std;
    if((this->mb.isIntegrationVariableName(h,n))||
       ((n[0]=='f')&&(this->mb.isIntegrationVariableName(h,n.substr(1))))){
      if(this->current->value=="setNormalisationFactor"){
    	string var;
    	string v;
    	if(this->mb.isIntegrationVariableName(h,n)){
    	  v = n;
    	} else {
    	  v = n.substr(1);
    	}
    	++(this->current);
    	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowVariableMethod");
    	this->readSpecifiedToken("MFrontImplicitParserBase::treatUnknowVariableMethod","(");
    	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowVariableMethod");
    	var = this->current->value;
    	if((this->mb.isMaterialPropertyName(h,var))||
    	   (this->mb.isLocalVariableName(h,var))){
    	  var = "this->" + var;
    	} else {
    	  // var shall be a number
    	  double value;
    	  istringstream flux(var);
    	  flux >> value;
    	  if(flux.fail()){
    	    this->throwRuntimeError("MFrontImplicitParserBase::treatUnknowVariableMethod",
    				    "Failed to read normalisation factor.");
    	  }
    	  if(value<=0.){
    	    this->throwRuntimeError("MFrontImplicitParserBase::treatUnknowVariableMethod",
    				    "invalid normalisation factor.");
    	  }
    	}
    	if(!this->nf.insert(make_pair(n,var)).second){
    	  this->throwRuntimeError("MFrontImplicitParserBase::treatUnknowVariableMethod",
    				  "Normalisation factor already defined for variable '"+n+"'.");
    	}
    	++(this->current);
    	return;
      } else if(this->current->value=="setMaximumIncrementValuePerIteration"){
    	string v;
    	if(this->mb.isIntegrationVariableName(h,n)){
    	  v = n;
    	} else {
    	  v = n.substr(1);
    	}
    	++(this->current);
    	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowVariableMethod");
    	this->readSpecifiedToken("MFrontImplicitParserBase::treatUnknowVariableMethod","(");
    	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowVariableMethod");
    	const string var =  this->current->value;
    	double value;
    	istringstream flux(var);
    	flux >> value;
    	if(flux.fail()){
    	  this->throwRuntimeError("MFrontImplicitParserBase::treatUnknowVariableMethod",
    				  "Failed to read maximum increment value per iteration from '"+var+"'.");
    	}
    	if(value<=0.){
    	  this->throwRuntimeError("MFrontImplicitParserBase::treatUnknowVariableMethod",
    				  "invalid maximum increment value per iteration.");
    	}
	this->registerVariable(n+"_maximum_increment_value_per_iteration",false);
	VariableDescription miv("real",n+"_maximum_increment_value_per_iteration",1u,0u);
	miv.description = "maximum increment allowed per iteration for variable '"+n+"'";
    	this->mb.addParameter(h,miv);
	this->mb.setParameterDefaultValue(h,n+"_maximum_increment_value_per_iteration",value);
    	++(this->current);
    	return;
      }
    }
    MFrontBehaviourParserCommon::treatUnknownVariableMethod(h,n);
  } // end of MFrontImplicitParserBase::treatUnknowVariableMethod
  
  bool
  MFrontImplicitParserBase::isCallableVariable(const Hypothesis h,
					       const std::string& n) const

  {
     using namespace std;
     if(n.empty()){
       string msg("MFrontImplicitParserBase::isCallableVariable : "
		  "empty variable name '"+n+"'");
       throw(runtime_error(msg));
     }
     if((n[0]=='f')&&(this->mb.isIntegrationVariableName(h,n.substr(1)))){
       return true;
     }
     return MFrontBehaviourParserCommon::isCallableVariable(h,n);
  } // end of MFrontImplicitParserBase::isCallableVariable
  
  void
  MFrontImplicitParserBase::treatUseAcceleration(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatUseAcceleration : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->mb.setAttribute(h,MechanicalBehaviourData::useCast3MAccelerationAlgorithm,true);
    } else if(this->current->value=="false"){
      this->mb.setAttribute(h,MechanicalBehaviourData::useCast3MAccelerationAlgorithm,false);
    } else {
      this->throwRuntimeError("MFrontImplicitParserBase::treatUseAcceleration",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatUseAcceleration",";");
  } // end of MFrontImplicitParserBase::treatUseAcceleration

  void
  MFrontImplicitParserBase::treatAccelerationTrigger(void)
  {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    unsigned short accelerationTrigger = this->readUnsignedShort("MFrontImplicitParserBase::treatAccelerationTrigger");
    if(accelerationTrigger<3){
      this->throwRuntimeError("MFrontImplicitParserBase::treatAccelerationTrigger",
			      "invalid acceleration trigger value.");
    }
    VariableDescription at("ushort","accelerationTrigger",1u,0u);
    at.description = "trigger period for the Cast3M acceleration algorithm";
    this->mb.addParameter(h,at);
    this->mb.setParameterDefaultValue(h,"accelerationTrigger",accelerationTrigger);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatAccelerationTrigger",";");
  } // end of MFrontImplicitParserBase::treatAccelerationTrigger

  void
  MFrontImplicitParserBase::treatAccelerationPeriod(void)
  {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    unsigned short accelerationPeriod;
    accelerationPeriod = this->readUnsignedShort("MFrontImplicitParserBase::treatAccelerationPeriod");
    if(accelerationPeriod==0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatAccelerationPeriod",
			      "invalid acceleration period value.");
    }
    this->mb.addParameter(h,VariableDescription("ushort","accelerationPeriod",1u,0u));
    this->mb.setParameterDefaultValue(h,"accelerationPeriod",accelerationPeriod);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatAccelerationPeriod",";");
  } // end of MFrontImplicitParserBase::treatAccelerationPeriod

  void
  MFrontImplicitParserBase::treatUseRelaxation(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatUseRelaxation : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->mb.setAttribute(h,MechanicalBehaviourData::useRelaxationAlgorithm,true);
    } else if(this->current->value=="false"){
      this->mb.setAttribute(h,MechanicalBehaviourData::useRelaxationAlgorithm,false);
    } else {
      this->throwRuntimeError("MFrontImplicitParserBase::treatUseRelaxation",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatUseRelaxation",";");
  } // end of MFrontImplicitParserBase::treatUseRelaxation

  void
  MFrontImplicitParserBase::treatCompareToNumericalJacobian(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatCompareToNumericalJacobian : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->mb.setAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,true);
    } else if(this->current->value=="false"){
      this->mb.setAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,false);
    } else {
      this->throwRuntimeError("MFrontImplicitParserBase::treatCompareToNumericalJacobian",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatCompareToNumericalJacobian",";");
  } // end of MFrontImplicitParserBase::treatCompareToNumericalJacobian
  
  void
  MFrontImplicitParserBase::treatJacobianComparisonCriterium(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!this->mb.getAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,false)){
      this->throwRuntimeError("MFrontImplicitParserBase::treatJacobianComparisonCriterium",
			      "must call '@CompareToNumericalJacobian' first");
    }
    double jacobianComparisonCriterium;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatJacobianComparisonCriterium",
			    "Cannot read jacobianComparisonCriterium value.");
    istringstream flux(current->value);
    flux >> jacobianComparisonCriterium;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatJacobianComparisonCriterium",
			      "Failed to read jacobianComparisonCriterium value.");
    }
    if(jacobianComparisonCriterium<0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatJacobianComparisonCriterium",
			      "JacobianComparisonCriterium value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatJacobianComparisonCriterium",";");
    this->mb.addParameter(h,VariableDescription("real","jacobianComparisonCriterium",1u,0u));
    this->mb.setParameterDefaultValue(h,"jacobianComparisonCriterium",
				      jacobianComparisonCriterium);
  } // MFrontImplicitParserBase::treatJacobianComparisonCriterium

  void
  MFrontImplicitParserBase::treatRelaxationTrigger(void)
  {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    unsigned short relaxationTrigger;
    relaxationTrigger = this->readUnsignedShort("MFrontImplicitParserBase::treatRelaxationTrigger");
    this->readSpecifiedToken("MFrontImplicitParserBase::treatRelaxationTrigger",";");
    this->mb.addParameter(h,VariableDescription("ushort","relaxationTrigger",1u,0u));
    this->mb.setParameterDefaultValue(h,"relaxationTrigger",relaxationTrigger);
  } // end of MFrontImplicitParserBase::treatRelaxationTrigger
  
  void
  MFrontImplicitParserBase::treatRelaxationCoefficient(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!this->mb.getAttribute(h,MechanicalBehaviourData::useRelaxationAlgorithm,false)){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationCoefficient",
			      "relaxation unused");
    }
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatRelaxationCoefficient",
			    "Cannot read relaxation coefficient value.");
    istringstream flux(current->value);
    double w;
    flux >> w;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationCoefficient",
			      "Failed to read relaxation coefficient value.");
    }
    if(w<0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationCoefficient",
			      "relaxation coefficient value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatRelaxationCoefficient",";");    
    this->mb.addParameter(h,VariableDescription("real","relaxationCoefficient",1u,0u));
    this->mb.setParameterDefaultValue(h,"relaxationCoefficient",w);
  } // end of MFrontImplicitParserBase::treatRelaxationCoefficient

  void MFrontImplicitParserBase::treatAlgorithm(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatAlgorithm",
			    "Cannot read algorithm name.");
    if(this->algorithm!=MFrontImplicitParserBase::DEFAULT){
      this->throwRuntimeError("MFrontImplicitParserBase::treatAlgorithm",
			      "@Algorithm has already been read.");
    }
    if(this->current->value=="NewtonRaphson"){
      this->algorithm = MFrontImplicitParserBase::NEWTONRAPHSON;
    } else if(this->current->value=="PowellDogLeg_NewtonRaphson"){
      this->algorithm = MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON;
    } else if(this->current->value=="NewtonRaphson_NumericalJacobian"){
      this->algorithm = MFrontImplicitParserBase::NEWTONRAPHSON_NJ;
    } else if(this->current->value=="PowellDogleg_NewtonRaphson_NumericalJacobian"){
      this->algorithm = MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ;
    } else if(this->current->value=="Broyden"){
      this->algorithm = MFrontImplicitParserBase::BROYDEN;
    } else if(this->current->value=="PowellDogLeg_Broyden"){
      this->algorithm = MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN;
    } else if(this->current->value=="Broyden2"){
      this->algorithm = MFrontImplicitParserBase::BROYDEN2;
    } else if(this->current->value=="LevenbergMarquardt"){
      this->algorithm = MFrontImplicitParserBase::LEVENBERGMARQUARDT;
    } else if(this->current->value=="LevenbergMarquardt_NumericalJacobian"){
      this->algorithm = MFrontImplicitParserBase::LEVENBERGMARQUARDT_NJ;
    } else {
      this->throwRuntimeError("MFrontImplicitParserBase::treatAlgorithm : ",
			      this->current->value+" is not a valid algorithm name\n"
			      "Supported algorithms are : NewtonRaphson, NewtonRaphson_NumericalJacobian, "
			      "Broyden, and Broyden2, "
			      "PowellDogLeg_NewtonRaphson, PowellDogLeg_NewtonRaphson_NumericalJacobian, "
			      "PowellDogLeg_NewtonRaphson, PowellDogLeg_NewtonRaphson_NumericalJacobian, "
			      "PowellDogLeg_Broyden, LevenbergMarquardt.");
    }
    ++this->current;
    this->readSpecifiedToken("MFrontImplicitParserBase::treatAlgorithm",";");
  } // end of MFrontImplicitParserBase::treatAlgorithm

  void
  MFrontImplicitParserBase::treatTheta(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double theta;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatTheta",
			    "Cannot read theta value.");
    istringstream flux(current->value);
    flux >> theta;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatTheta",
			      "Failed to read theta value.");
    }
    if((theta<0.)||(theta>1.)){
      this->throwRuntimeError("MFrontImplicitParserBase::treatTheta",
			      "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatTheta",";");
    VariableDescription tv("real","theta",1u,0u);
    tv.description = "theta value used by the implicit scheme";
    this->mb.addParameter(h,tv);
    this->mb.setParameterDefaultValue(h,"theta",theta);
    this->mb.setEntryName(h,"theta","theta");
  } // end of MFrontImplicitParserBase::treatTheta

  void
  MFrontImplicitParserBase::treatEpsilon(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatEpsilon",";");
    VariableDescription e("real","epsilon",1u,0u);
    e.description = "value used to stop the iteration of the implicit algorithm";
    this->mb.addParameter(h,e);
    this->mb.setParameterDefaultValue(h,"epsilon",epsilon);
    this->mb.setEntryName(h,"epsilon","epsilon");
  } // MFrontImplicitParserBase::treatEpsilon

  void
  MFrontImplicitParserBase::treatPerturbationValueForNumericalJacobianComputation(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatPerturbationValueForNumericalJacobianComputation",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatPerturbationValueForNumericalJacobianComputation",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatPerturbationValueForNumericalJacobianComputation",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatPerturbationValueForNumericalJacobianComputation",";");
    VariableDescription e("real","numerical_jacobian_epsilon",1u,0u);
    e.description = "perturbation value used to compute a finite difference approximation of the jacobian";
    this->mb.addParameter(h,e);
    this->mb.setParameterDefaultValue(h,"numerical_jacobian_epsilon",epsilon);
  } // MFrontImplicitParserBase::treatEpsilon

  void
  MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double pdl_trs;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize",
			    "Cannot read pdl_trs value.");
    istringstream flux(current->value);
    flux >> pdl_trs;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize",
			      "Failed to read region size.");
    }
    if(pdl_trs<0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize",
			      "Region size must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize",";");
    this->mb.addParameter(h,VariableDescription("real","powell_dogleg_trust_region_size",1u,0u));
    this->mb.setParameterDefaultValue(h,"powell_dogleg_trust_region_size",pdl_trs);
  } // MFrontImplicitParserBase::treatPowellDogLegTrustRegionSize

  void
  MFrontImplicitParserBase::treatIterMax(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    unsigned short iterMax;
    iterMax = this->readUnsignedShort("MFrontImplicitParserBase::treatIterMax");
    if(iterMax==0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatIterMax",
			      "invalid value for parameter 'iterMax'");
    }
    this->readSpecifiedToken("MFrontImplicitParserBase::treatIterMax",";");
    this->mb.addParameter(h,VariableDescription("ushort","iterMax",1u,0u));
    this->mb.setParameterDefaultValue(h,"iterMax",iterMax);
  } // end of MFrontImplicitParserBase::treatIterMax

  std::string
  MFrontImplicitParserBase::tangentOperatorVariableModifier(const Hypothesis h,
							    const std::string& var,
							    const bool addThisPtr)
  {
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if(d.isIntegrationVariableIncrementName(var)){
      if(nf.find(var.substr(1))!=nf.end()){
	const VariableDescription& s = d.getStateVariableHandler(var.substr(1));
	if(s.arraySize==1u){
	  if(addThisPtr){
	    return "(("+nf.find(var.substr(1))->second+")*(this->"+var+"))";
	  } else {
	    return "("+nf.find(var.substr(1))->second+")*var";
	  }
	} else {
	  if(addThisPtr){
	    return "("+nf.find(var.substr(1))->second+")*(this->"+var+")";
	  } else {
	    return "("+nf.find(var.substr(1))->second+")*var";
	  }
	}
      }
    }
    if(addThisPtr){
      return "(this->"+var+")";
    } else {
      return var;
    }
  } // end of MFrontImplicitParserBase::tangentOperatorVariableModifier

  std::string
  MFrontImplicitParserBase::integratorVariableModifier(const Hypothesis h,
						       const std::string& var,
						       const bool addThisPtr)
  {
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if(d.isIntegrationVariableIncrementName(var)){
      if(nf.find(var.substr(1))!=nf.end()){
	const VariableDescription& s = d.getStateVariableHandler(var.substr(1));
	if(s.arraySize==1u){
	  if(addThisPtr){
	    return "(("+nf.find(var.substr(1))->second+")*(this->"+var+"))";
	  } else {
	    return "("+nf.find(var.substr(1))->second+")*var";
	  }
	} else {
	  if(addThisPtr){
	    return "("+nf.find(var.substr(1))->second+")*(this->"+var+")";
	  } else {
	    return "("+nf.find(var.substr(1))->second+")*var";
	  }
	}
      }
    }
    if(addThisPtr){
      return "(this->"+var+")";
    } else {
      return var;
    }
  } // end of MFrontImplicitParserBase::integratorVariableModifier

  std::string
  MFrontImplicitParserBase::computeStressVariableModifier1(const Hypothesis h,
							   const std::string& var,
							   const bool addThisPtr)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if(this->mb.isDrivingVariableName(var)||(var=="T")|
       (d.isExternalStateVariableName(var))){
      if(addThisPtr){
	return "(this->"+var+"+(this->theta)*(this->d"+var+"))";
      } else {
	return "("+var+"+(this->theta)*d"+var+")";
      }
    }
    if(d.isIntegrationVariableName(var)){
      if(this->nf.find(var)!=nf.end()){
	const VariableDescription& s = d.getStateVariableHandler(var);
	if(s.arraySize==1u){
	  if(addThisPtr){
	    return "(this->"+var+"+(this->theta)*(("+this->nf.find(var)->second+")*(this->d"+var+")))";
	  } else {
	    return "("+var+"+("+this->nf.find(var)->second+")*(this->theta)*d"+var+")";
	  }
	} else {
	  if(addThisPtr){
	    return "(this->"+var+"+(this->theta)*(("+this->nf.find(var)->second+")*(this->d"+var+")))";
	  } else {
	    return "("+var+"+("+this->nf.find(var)->second+")*(this->theta)*d"+var+")";
	  }
	}
      } else {
	if(addThisPtr){
	  return "(this->"+var+"+(this->theta)*(this->d"+var+"))";
	} else {
	  return "("+var+"+(this->theta)*d"+var+")";
	}
      }
    }
    if((d.isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->mb.declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontImplicitParserBase::computeStressVariableModifier1

  std::string
  MFrontImplicitParserBase::computeStressVariableModifier2(const Hypothesis h,
							   const std::string& var,
							   const bool addThisPtr)
  {
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if((this->mb.isDrivingVariableName(var))||(var=="T")||
       (d.isExternalStateVariableName(var))){
      if(addThisPtr){
	return "(this->"+var+"+this->d"+var+")";
      } else {
	return "("+var+"+d"+var+")";
      }
    }
    if((d.isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->mb.declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontImplicitParserBase::computeStressVariableModifier2

  bool
  MFrontImplicitParserBase::isJacobianPart(const Hypothesis h,
					   const std::string& w)
  {
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    TokensContainer::const_iterator previous;
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	if(w=="df"+p->name+"_dd"+p2->name){
	  previous = this->current;
	  if(previous==this->begin()){
	    return true;
	  }
	  --previous;
	  if(previous->value=="."){
	    return false;
	  } else if(previous->value=="->"){
	    if(previous==this->begin()){
	      return false;
	    }
	    --previous;
	    if(previous->value=="this"){
	      return true;
	    } else {
	      return false;
	    }
	  } else if(previous->value=="::"){
	    if(previous==this->begin()){
	      return false;
	    }
	    --previous;
	    if(previous->value==this->mb.getClassName()){
	      return true;
	    } else {
	      return false;
	    }
	  }
	  return true;
	}
      }
    }
    return false;
  } // end of MFrontImplicitParserBase::isJacobianPart

  void
  MFrontImplicitParserBase::integratorAnalyser(const Hypothesis h,
					       const std::string& w)
  {
    if(this->isJacobianPart(h,w)){
      this->jacobianPartsUsedInIntegrator.insert(w);
    }
  } // end of MFrontImplicitParserBase::integratorAnalyser

  void
  MFrontImplicitParserBase::predictorAnalyser(const Hypothesis h,
					      const std::string& w)
  {
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if(d.isIntegrationVariableIncrementName(w)){
      this->integrationVariablesIncrementsUsedInPredictor.insert(w);
    }
  } // end of MFrontImplicitParserBase::predictorAnalyser

  void
  MFrontImplicitParserBase::treatIntegrator(void)
  {
    this->readCodeBlock(*this,MechanicalBehaviourData::Integrator,
			&MFrontImplicitParserBase::integratorVariableModifier,
			&MFrontImplicitParserBase::integratorAnalyser,true);
  } // end of MFrontImplicitParserBase::treatIntegrator

  void
  MFrontImplicitParserBase::treatPredictor(void)
  {
    using namespace std;
    this->readCodeBlock(*this,MechanicalBehaviourData::ComputePredictor,
			&MFrontImplicitParserBase::standardModifier,
			&MFrontImplicitParserBase::predictorAnalyser,true);
  } // end of MFrontImplicitParserBase::treatPredictor

  void
  MFrontImplicitParserBase::treatComputeStress(void)
  {
    /*
     * Most behaviours will only rely the @ComputeStress keyword to
     * estimate stresses at the middle of the time step and at the
     * end.  However, some finite strains behaviours must compute a
     * stress measure during the iterations which is not the Cauchy
     * stress. Thus, @ComputeStress also defines an
     * ComputeFinalStressCandidate code block which will be used if
     * the user does not provide an alternative through the
     * @ComputeFinalStress
     */
    this->readCodeBlock(*this,MechanicalBehaviourData::ComputeStress,
			MechanicalBehaviourData::ComputeFinalStressCandidate,
			&MFrontImplicitParserBase::computeStressVariableModifier1,
			&MFrontImplicitParserBase::computeStressVariableModifier2,true,true);
  } // end of MFrontImplicitParserBase::treatComputeStress

  void
  MFrontImplicitParserBase::treatComputeFinalStress(void)
  {
    this->readCodeBlock(*this,MechanicalBehaviourData::ComputeFinalStress,
			&MFrontImplicitParserBase::computeStressVariableModifier2,true,true);
  } // end of MFrontImplicitParserBase::treatComputeFinalStress

  void
  MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double value;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration",
			    "Cannot read value value.");
    istringstream flux(current->value);
    flux >> value;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration",
			      "Failed to read value.");
    }
    if(value<=0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration",
			      "Value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::MaximumIncrementValuePerIteration",";");
    this->mb.addParameter(h,VariableDescription("real","maximum_increment_value_per_iteration",1u,0u));
    this->mb.setParameterDefaultValue(h,"maximum_increment_value_per_iteration",value);
  } // end of MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration

  void MFrontImplicitParserBase::writeBehaviourParserSpecificIncludes(void)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    bool has_scalar  = false;
    bool has_scalar_array  = false;
    bool has_tvector = false;
    bool has_tvector_array = false;
    bool has_stensor = false;
    bool has_stensor_array = false;
    // checks
    const set<Hypothesis>& h = this->mb.getModellingHypotheses();
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(*ph);
      const VariableDescriptionContainer& sv = d.getIntegrationVariables();
      for(p=sv.begin();p!=sv.end();++p){
	SupportedTypes::TypeFlag flag  = this->getTypeFlag(p->type);
	if(flag==SupportedTypes::Scalar){
	  has_scalar = true;
	  if(p->arraySize>=1){
	    has_scalar_array = true;
	  }
	}
	if(flag==SupportedTypes::Stensor){
	  has_stensor = true;
	  if(p->arraySize>=1){
	    has_stensor_array = true;
	  }
	}
	if(flag==SupportedTypes::TVector){
	  has_tvector = true;
	  if(p->arraySize>=1){
	    has_tvector_array = true;
	  }
	}
      }
    }
    // output
    this->checkBehaviourFile();
    this->behaviourFile << "#include\"TFEL/Math/st2tost2.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Math/tmatrix.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Math/tvector.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Math/TinyMatrixSolve.hxx\"" << endl;
    if(has_scalar_array){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TinyVectorFromTinyVectorView.hxx\"" << endl;
    }
    // tiny vectors
    if(has_tvector){
      this->behaviourFile << "#include\"TFEL/Math/Matrix/tmatrix_submatrix_view.hxx\"\n" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Matrix/tmatrix_submatrix_view.hxx\"\n" << endl;
    }
    if((has_scalar)&&(has_tvector)){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixColumnView.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixRowView.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixColumnView2.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixRowView2.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyVectorView.hxx\"" << endl;
    }
    if(has_tvector_array){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TinyVectorOfTinyVectorFromTinyVectorView.hxx\"" << endl;
    }
    // symmetric tensors
    if(has_stensor){
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyVectorView.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/ST2toST2/ST2toST2FromTinyMatrixView.hxx\"\n" << endl;
      this->behaviourFile << "#include\"TFEL/Math/ST2toST2/ST2toST2FromTinyMatrixView2.hxx\"\n" << endl;
    }
    if((has_scalar)&&(has_stensor)){
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixColumnView.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixRowView.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixColumnView2.hxx\"" << endl;
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixRowView2.hxx\"" << endl;
    }
    if(has_stensor_array){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TinyVectorOfStensorFromTinyVectorView.hxx\"" << endl;
    }
  } // end of MFrontImplicitParserBase::writeBehaviourParserSpecificIncludes(void)
  
  void MFrontImplicitParserBase::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    MFrontBehaviourParserCommon::writeBehaviourParserSpecificTypedefs();
  } // end of MFrontImplicitParserBase::writeBehaviourParserSpecificTypedefs(void)

  void
  MFrontImplicitParserBase::writeBehaviourParserSpecificMembers(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n3;
    // size of linear system
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n3 += this->getTypeSize(p->type,p->arraySize);
    }
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      SupportedTypes::TypeSize n2;
      for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	SupportedTypes::TypeFlag flag  = this->getTypeFlag(p->type);
	SupportedTypes::TypeFlag flag2 = this->getTypeFlag(p2->type);
	if((p->arraySize!=1u)||(p2->arraySize!=1u)){
	  this->behaviourFile << "/*!" << endl
			      << " * \\return the part of the jacobian matrix "
			      << "corresponding to the derivative "
			      << "of variable " << p->name 
			      << " by variable " << p2->name << "" << endl
			      << " */" << endl;
	}
	if((p->arraySize!=1u)&&(p2->arraySize==1u)){
	  if(flag==SupportedTypes::Scalar){
	    if(flag2==SupportedTypes::Scalar){
	      this->behaviourFile << "real&" << endl
				  << "df" << p->name << "_dd" << p2->name
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian," << endl
				  << "const unsigned short idx){" << endl
				  << "return tjacobian(" << n << "+idx, " << n2 << ");" << endl
				  << "}\n" << endl;
	      this->behaviourFile << "real&" << endl
				  << "df" << p->name << "_dd" << p2->name << "(const unsigned short idx){\n"
				  << "return this->jacobian(" << n << "+idx, " << n2 << ");\n"
				  << "}\n\n";
	    } else if(flag2==SupportedTypes::TVector){
	      // Le résultat est un tenseur, une ligne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::TVectorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename TVectorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,0);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::TVectorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename TVectorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,0);\n"
				  << "}\n\n";
	    } else {
	      // Le résultat est un tenseur, une ligne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,0);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,0);\n"
				  << "}\n\n";
	    }
	  } else if(flag==SupportedTypes::TVector){
	    if(flag2==SupportedTypes::Scalar){
	      // Le résultat est un tenseur, une colonne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,0);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,0);\n"
				  << "}\n\n";
	    } else if(flag2==SupportedTypes::TVector){
	      // Le résultat est une sous-matrice
	      this->behaviourFile << "typename tfel::math::TMatrixFromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
	      			  << "df" << p->name << "_dd" << p2->name 
	      			  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
	      			  << "const unsigned short idx){\n"
	      			  << "using namespace tfel::math;\n"
	      			  << "return typename TMatrixFromTinyMatrixView2<N,"
	      			  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,0u);\n"
	      			  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::TMatrixFromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
	      			  << "df" << p->name << "_dd" << p2->name 
	      			  << "(const unsigned short idx){\n"
	      			  << "using namespace tfel::math;\n"
	      			  << "return typename TMatrixFromTinyMatrixView2<N,"
	      			  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,0u);\n"
	      			  << "}\n\n";
	    } else {
	      string msg("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers : ");
	      msg += "derivation of a vector by a tensor is not defined";
	      throw(runtime_error(msg));
	    }
	  } else {
	    if(flag2==SupportedTypes::Scalar){
	      // Le résultat est un tenseur, une colonne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,0);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,0);\n"
				  << "}\n\n";
	    } else if(flag2==SupportedTypes::Stensor){
	      // Le résultat est un tenseur d'ordre 4
	      this->behaviourFile << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename ST2toST2FromTinyMatrixView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,0u);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename ST2toST2FromTinyMatrixView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,0u);\n"
				  << "}\n\n";
	    } else {
	      string msg("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers : ");
	      msg += "derivation of a tensor by a vector is not defined";
	      throw(runtime_error(msg));
	    }
	  }
	} else if((p->arraySize==1u)&&(p2->arraySize!=1u)){
	  if(flag==SupportedTypes::Scalar){
	    if(flag2==SupportedTypes::Scalar){
	      this->behaviourFile << "real&\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "return tjacobian(" << n << ", " << n2 << "+idx);\n"
				  << "}\n\n";
	      this->behaviourFile << "real&\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "return this->jacobian(" << n << ", " << n2 << "+idx);\n"
				  << "}\n\n";
	    } else {
	      // Le résultat est un tenseur, une ligne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,0,idx);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,0,idx);\n"
				  << "}\n\n";
	    }
	  } else {
	    if(flag2==SupportedTypes::Scalar){
	      // Le résultat est un tenseur, une colonne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,0,idx);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,0,idx);\n"
				  << "}\n\n";
	    } else {
	      this->behaviourFile << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename ST2toST2FromTinyMatrixView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,0,idx);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename ST2toST2FromTinyMatrixView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,0,idx);\n"
				  << "}\n\n";
	    }
	  }
	} else if((p->arraySize!=1u)&&(p2->arraySize!=1u)){
	  if(flag==SupportedTypes::Scalar){
	    if(flag2==SupportedTypes::Scalar){
	      this->behaviourFile << "real&\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "return tjacobian(" << n << "+idx, " << n2 << "+idx2);\n"
				  << "}\n\n";
	      this->behaviourFile << "real&\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "return this->jacobian(" << n << "+idx, " << n2 << "+idx2);\n"
				  << "}\n\n";
	    } else {
	      // Le résultat est un tenseur, une ligne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,idx2);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixRowView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixRowView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,idx2);\n"
				  << "}\n\n";
	    }
	  } else {
	    if(flag2==SupportedTypes::Scalar){
	      // Le résultat est un tenseur, une colonne dans la matrice jacobienne
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,idx2);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::StensorFromTinyMatrixColumnView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename StensorFromTinyMatrixColumnView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,idx2);\n"
				  << "}\n\n";
	    } else {
	      this->behaviourFile << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename ST2toST2FromTinyMatrixView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(tjacobian,idx,idx2);\n"
				  << "}\n\n";
	      this->behaviourFile << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
				  << "df" << p->name << "_dd" << p2->name 
				  << "(const unsigned short idx,"
				  << " const unsigned short idx2){\n"
				  << "using namespace tfel::math;\n"
				  << "return typename ST2toST2FromTinyMatrixView2<N,"
				  << n3 << "," << n3 << "," << n << "," << n2 << ",real>::type(this->jacobian,idx,idx2);\n"
				  << "}\n\n";
	    }
	  }
	}
	n2 += this->getTypeSize(p2->type,p2->arraySize);
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
    // size of linear system
    n = n3;
    this->behaviourFile << "// Jacobian\n";
    this->behaviourFile << "tfel::math::tmatrix<" << n << "," << n << ",Type> jacobian;\n";
    this->behaviourFile << "// zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> zeros;\n\n";
    this->behaviourFile << "// previous zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> zeros_1;\n\n";
    this->behaviourFile << "// function\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> fzeros;\n\n";
    if(this->mb.getAttribute(h,MechanicalBehaviourData::useCast3MAccelerationAlgorithm,false)){
      this->behaviourFile << "// previous results (used for acceleration)\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type> previous_fzeros[3];\n\n";
      this->behaviourFile << "// previous zeros (used for acceleration)\n";
      this->behaviourFile << "tfel::math::tvector<" << n << ",Type> previous_zeros[3];\n\n";
    }
    this->behaviourFile << "// number of iterations\n";
    this->behaviourFile << "unsigned int iter;\n\n";
    // castem acceleration
    if(this->mb.getAttribute(h,MechanicalBehaviourData::useCast3MAccelerationAlgorithm,false)){
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
    if((this->mb.getAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,false))||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT_NJ)){
      this->writeComputeNumericalJacobian(h);
    }
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT_NJ)){
      this->writeGetPartialJacobianInvert(h);
    }
    // compute stress
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "void\ncomputeStress(void){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      this->writeStandardPerformanceProfiling(this->behaviourFile,
					      MechanicalBehaviourData::ComputeStress);
      writeMaterialLaws("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputeStress).code << endl;
      this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeStress\n\n";
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeFinalStress)){
      this->behaviourFile << "void\ncomputeFinalStress(void){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      this->writeStandardPerformanceProfiling(this->behaviourFile,
					      MechanicalBehaviourData::ComputeFinalStress);
      writeMaterialLaws("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputeFinalStress).code << endl;
      this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeStress\n\n";
      this->behaviourFile << endl;
    }
  } // end of MFrontImplicitParserBase::writeBehaviourParserSpecificMembers

  std::string
  MFrontImplicitParserBase::getJacobianPart(const VariableDescription&  v1,
					    const VariableDescription&  v2,
					    const SupportedTypes::TypeSize& n,
					    const SupportedTypes::TypeSize& n2,
					    const SupportedTypes::TypeSize& n3,
					    const std::string& j,
					    const std::string& p)
  {
    using namespace std;
    ostringstream d;
    if(this->varNames.find(p+"df"+v1.name+"_dd"+v2.name)!=this->varNames.end()){
      string msg("MFrontImplicitParserBase::getJacobianPart : ");
      msg += "variable name 'df"+v1.name+"_dd"+v2.name;
      msg += "' is reserved.\n";
      throw(runtime_error(msg));
    }
    if(this->getTypeFlag(v1.type)==SupportedTypes::Stensor){
      if(this->getTypeFlag(v2.type)==SupportedTypes::Stensor){
	d << "typename tfel::math::ST2toST2FromTinyMatrixView<N,"
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name << "_dd" << v2.name << "("+j+");\n";
      } else if(this->getTypeFlag(v2.type)==SupportedTypes::Scalar){
	d << "typename tfel::math::StensorFromTinyMatrixColumnView<N," 
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name << "_dd" << v2.name << "("+j+");\n";
      } else {
	string msg("MFrontImplicitParserBase::getJacobianPart : ");
	msg += "unsupported type for integration variable ";
	msg += v2.name;
	throw(runtime_error(msg));
      }
    } else if(this->getTypeFlag(v1.type)==SupportedTypes::Scalar){
      if(this->getTypeFlag(v2.type)==SupportedTypes::Stensor){
	d << "typename tfel::math::StensorFromTinyMatrixRowView<N,"
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name 
	  << "_dd" << v2.name << "("+j+");\n";
      } else if(this->getTypeFlag(v2.type)==SupportedTypes::Scalar){
	d << "real& "+p+"df" << v1.name << "_dd" << v2.name 
	  << " = "+j+"(" << n << "," << n3 << ");\n";
      } else {
	string msg("MFrontImplicitParserBase::getJacobianPart : ");
	msg += "unsupported type for integration variable ";
	msg += v2.name;
	throw(runtime_error(msg));
      }
    } else {
      string msg("MFrontImplicitParserBase::getJacobianPart : ");
      msg += "unsupported type for integration variable ";
      msg += v1.name;
      throw(runtime_error(msg));
    }
    return d.str();
  } // void MFrontImplicitParserBase::getJacobianPart

  void
  MFrontImplicitParserBase::writeGetPartialJacobianInvert(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    this->checkBehaviourFile();
    SupportedTypes::TypeSize n;
    VariableDescriptionContainer::size_type i;
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    p=d.getIntegrationVariables().begin();
    ++p;
    for(i=0;i!=d.getIntegrationVariables().size();++p,++i){
      this->behaviourFile << "void\ngetPartialJacobianInvert(";
      for(p2=d.getIntegrationVariables().begin();p2!=p;){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p2->type);
	if(p2->arraySize==1u){
	  switch(flag){
	  case SupportedTypes::Scalar : 
	    this->behaviourFile << "Stensor& ";
	    break;
	  case SupportedTypes::Stensor :
	    this->behaviourFile << "Stensor4& ";
	    break;
	  default :
	    string msg("MFrontImplicitParserBase::writeGetPartialJacobianInvert : ");
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	} else {
	  switch(flag){
	  case SupportedTypes::Scalar : 
	    this->behaviourFile << "tfel::math::tvector<" << p2->arraySize << "u,Stensor>& ";
	    break;
	  case SupportedTypes::Stensor :
	    this->behaviourFile << "tfel::math::tvector<" << p2->arraySize << "u,Stensor4>& ";
	    break;
	  default :
	    string msg("MFrontImplicitParserBase::writeGetPartialJacobianInvert : ");
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	}
	this->behaviourFile << "partial_jacobian_" << p2->name;
	if(++p2!=p){
	  this->behaviourFile << "," << endl;
	}	  
      }
      this->behaviourFile << ")" << endl;
      this->behaviourFile << "{\n";
      this->behaviourFile << "using namespace tfel::math;" << endl;
      this->behaviourFile << "TinyPermutation<" << n << "> permuation;" << endl;
      this->behaviourFile << "TinyMatrixSolve<" << n << ",real>::decomp(this->jacobian,permuation);" << endl;
      this->behaviourFile << "for(unsigned short idx=0;idx!=StensorSize;++idx){\n";
      this->behaviourFile << "tvector<" << n << ",real> vect_e(real(0));" << endl;
      this->behaviourFile << "vect_e(idx) = real(1);" << endl;
      this->behaviourFile << "TinyMatrixSolve<" << n << ",real>::back_substitute(this->jacobian,permuation,vect_e);" << endl;
      SupportedTypes::TypeSize n2;
      for(p2=d.getIntegrationVariables().begin();p2!=p;++p2){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p2->type);
	if(flag==SupportedTypes::Scalar){
	  if(p2->arraySize==1u){
	    this->behaviourFile << "partial_jacobian_" << p2->name << "(idx)=vect_e(" << n2 << ");\n";
	  } else {
	    this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				<< p2->arraySize << ";++idx2){" << endl;
	    this->behaviourFile << "partial_jacobian_" << p2->name << "(idx2)(idx)=vect_e(" << n2 << "+idx2);\n";
	    this->behaviourFile << "}\n";
	  }
	  n2 += this->getTypeSize(p2->type,p2->arraySize);
	} else if(flag==SupportedTypes::TVector){
	  if(p2->arraySize==1u){
	    this->behaviourFile << "for(unsigned short idx2=" << n2;
	    this->behaviourFile << ";idx2!=";
	    n2 += this->getTypeSize(p2->type,p2->arraySize);
	    this->behaviourFile << n2 << ";++idx2){" << endl;
	    this->behaviourFile << "partial_jacobian_" << p2->name << "(idx2,idx)=vect_e(idx2);\n";
	    this->behaviourFile << "}\n";
	  } else {
	    this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				<< p2->arraySize << ";++idx2){" << endl;
	    this->behaviourFile << "for(unsigned short idx3=" << n2;
	    this->behaviourFile << ";idx3!=TVectorSize;++idx3){" << endl;
	    this->behaviourFile << "partial_jacobian_" << p2->name 
				<< "(idx2)(idx3,idx)=vect_e(" << n2 << "+idx3+idx2*TVectorSize);\n";
	    this->behaviourFile << "}\n";
	    this->behaviourFile << "}\n";
	    n2 += this->getTypeSize(p2->type,p2->arraySize);
	  }
	} else if(flag==SupportedTypes::TVector){
	  if(p2->arraySize==1u){
	    this->behaviourFile << "for(unsigned short idx2=" << n2;
	    this->behaviourFile << ";idx2!=";
	    n2 += this->getTypeSize(p2->type,p2->arraySize);
	    this->behaviourFile << n2 << ";++idx2){" << endl;
	    this->behaviourFile << "partial_jacobian_" << p2->name << "(idx2,idx)=vect_e(idx2);\n";
	    this->behaviourFile << "}\n";
	  } else {
	    this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				<< p2->arraySize << ";++idx2){" << endl;
	    this->behaviourFile << "for(unsigned short idx3=" << n2;
	    this->behaviourFile << ";idx3!=TVectorSize;++idx3){" << endl;
	    this->behaviourFile << "partial_jacobian_" << p2->name 
				<< "(idx2)(idx3,idx)=vect_e(" << n2 << "+idx3+idx2*TVectorSize);\n";
	    this->behaviourFile << "}\n";
	    this->behaviourFile << "}\n";
	    n2 += this->getTypeSize(p2->type,p2->arraySize);
	  }
	} else if(flag==SupportedTypes::Stensor){
	  if(p2->arraySize==1u){
	    this->behaviourFile << "for(unsigned short idx2=" << n2;
	    this->behaviourFile << ";idx2!=";
	    n2 += this->getTypeSize(p2->type,p2->arraySize);
	    this->behaviourFile << n2 << ";++idx2){" << endl;
	    this->behaviourFile << "partial_jacobian_" << p2->name << "(idx2,idx)=vect_e(idx2);\n";
	    this->behaviourFile << "}\n";
	  } else {
	    this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				<< p2->arraySize << ";++idx2){" << endl;
	    this->behaviourFile << "for(unsigned short idx3=" << n2;
	    this->behaviourFile << ";idx3!=StensorSize;++idx3){" << endl;
	    this->behaviourFile << "partial_jacobian_" << p2->name 
				<< "(idx2)(idx3,idx)=vect_e(" << n2 << "+idx3+idx2*StensorSize);\n";
	    this->behaviourFile << "}\n";
	    this->behaviourFile << "}\n";
	    n2 += this->getTypeSize(p2->type,p2->arraySize);
	  }
	} else {
	  string msg("MFrontImplicitParserBase::writeGetPartialJacobianInvert : ");
	  msg += "internal error, tag unsupported";
	  throw(runtime_error(msg));
	}
      }
      this->behaviourFile << "}\n";
      for(p2=d.getIntegrationVariables().begin();p2!=p;++p2){
	if(nf.find(p2->name)!=nf.end()){
	  this->behaviourFile << "partial_jacobian_" << p2->name << " /= " << nf.find(p2->name)->second << ";\n";
	}
      }
      this->behaviourFile << "}\n\n";
    }
  } // end of MFrontImplicitParserBase::writeGetPartialJacobianInvert
  
  void
  MFrontImplicitParserBase::writeLimitsOnIncrementValues(const Hypothesis h,
							 const std::string& v)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    this->checkBehaviourFile();
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      SupportedTypes::TypeSize nv = this->getTypeSize(p->type,p->arraySize);
      if(this->mb.hasParameter(h,p->name+"_maximum_increment_value_per_iteration")){
	this->behaviourFile << "for(unsigned short idx = 0; idx!=" << nv << ";++idx){\n";
	if(this->nf.find(p->name)!=this->nf.end()){
	  this->behaviourFile << "if(std::abs(" << v << "[" << n 
			      << "+idx])>" << this->nf[p->name] << "*(this->" << p->name << "_maximum_increment_value_per_iteration)){\n";
	} else {
	  this->behaviourFile << "if(std::abs(" << v << "[" << n 
			      << "+idx])>this->" << p->name << "_maximum_increment_value_per_iteration){\n";
	}
	this->behaviourFile << "if("<< v << "[" << n << "+idx]<0){\n";
	if(this->nf.find(p->name)!=this->nf.end()){
	  this->behaviourFile << "" << v << "[" << n
			      << "+idx] = -"  << this->nf[p->name] << "*(this->" << p->name << "_maximum_increment_value_per_iteration);\n";
	} else {
	  this->behaviourFile << "" << v << "[" << n
			      << "+idx] = -this->" << p->name << "_maximum_increment_value_per_iteration;\n";
	}
	this->behaviourFile << "} else {\n";
	if(this->nf.find(p->name)!=this->nf.end()){
	  this->behaviourFile << "" << v << "[" << n
			      << "+idx] = " << this->nf[p->name] << "*(this->" << p->name << "_maximum_increment_value_per_iteration);\n";
	} else {
	  this->behaviourFile << "" << v << "["
			      << n << "+idx] =  this->" << p->name << "_maximum_increment_value_per_iteration;\n";
	}
	this->behaviourFile << "}\n";
	this->behaviourFile << "}\n";
	this->behaviourFile << "}\n";
      } else if(this->mb.hasParameter(h,"maximum_increment_value_per_iteration")){
	this->behaviourFile << "for(unsigned short idx = 0; idx!=" << nv << ";++idx){\n";
	this->behaviourFile << "if(std::abs(" << v << "[" << n 
			    << "+idx])>this->maximum_increment_value_per_iteration){\n";
	this->behaviourFile << "if("<< v << "[" << n << "+idx]<0){\n";
	this->behaviourFile << "" << v << "[" << n
			    << "+idx] = -this->maximum_increment_value_per_iteration;\n";
	this->behaviourFile << "} else {\n";
	this->behaviourFile << "" << v << "["
			    << n << "+idx] =  this->maximum_increment_value_per_iteration;\n";
	this->behaviourFile << "}\n";
	this->behaviourFile << "}\n";
	this->behaviourFile << "}\n";
      }
      n += nv;
    }
  }

  void MFrontImplicitParserBase::writeComputeNumericalJacobian(const Hypothesis h){
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
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
    this->behaviourFile << "this->zeros(idx) -= this->numerical_jacobian_epsilon;\n";
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "this->computeFdF();\n";
    this->behaviourFile << "this->zeros = tzeros;\n";
    this->behaviourFile << "tvector<" << n << ",real> tfzeros2(this->fzeros);\n";
    // if(this->algorithm!=MFrontImplicitParserBase::NEWTONRAPHSON){
    //   this->behaviourFile << "if(&njacobian!=&(this->jacobian)){\n";
    //   this->behaviourFile << "this->jacobian = tjacobian;\n";
    //   this->behaviourFile << "}\n";
    // }
    this->behaviourFile << "this->zeros(idx) += this->numerical_jacobian_epsilon;\n";
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "this->computeFdF();\n";
    this->behaviourFile << "this->fzeros = (this->fzeros-tfzeros2)/(real(2)*(this->numerical_jacobian_epsilon));\n";
    this->behaviourFile << "for(unsigned short idx2 = 0; idx2!= "<< n <<  ";++idx2){\n";
    this->behaviourFile << "njacobian(idx2,idx) = this->fzeros(idx2);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->zeros    = tzeros;\n";
    this->behaviourFile << "this->fzeros   = tfzeros;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(&njacobian!=&(this->jacobian)){\n";
    this->behaviourFile << "this->jacobian = tjacobian;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
  }

  std::string
  MFrontImplicitParserBase::getVectorMappingClass(const VariableDescription& v) const
  {
    using namespace std;
    const SupportedTypes::TypeFlag f = this->getTypeFlag(v.type);
    if(f==SupportedTypes::Stensor){
      if(v.arraySize==1u){
	return "StensorFromTinyVectorView";
      } else {
	return "TinyVectorOfStensorFromTinyVectorView";
      }
    }
    string msg("MFrontImplicitParserBase::getVectorMappingClass : "
	       "unsupported type for variable '"+v.name+"'");
    throw(runtime_error(msg));
    return "";
  } // end of MFrontImplicitParserBase::getVectorMappingClass

  void MFrontImplicitParserBase::writeBehaviourIntegrator(const Hypothesis h){
    using namespace std;
    const string btype = this->convertBehaviourTypeToString();
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    vector<BoundsDescription>::const_iterator p3;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult" << endl;
    if(this->mb.hasAttribute(h,MechanicalBehaviourData::hasConsistentTangentOperator)){
      this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt){\n";
    } else {
      if((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL)){
	this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt){\n";
      } else {
	this->behaviourFile << "integrate(const SMFlag,const SMType smt){\n";
      }
    }
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    if((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL)){
      if(this->mb.useQt()){
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){" << endl
			    << "throw(runtime_error(\"invalid tangent operator flag\"));" << endl
			    << "}" << endl;
      } else {
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){" << endl
			    << "throw(runtime_error(\"invalid tangent operator flag\"));" << endl
			    << "}" << endl;
      }
    }
    this->writeStandardPerformanceProfilingBegin(this->behaviourFile,
						 MechanicalBehaviourData::Integrator);
    if((this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT_NJ)){
      this->writeLevenbergMarquardtAlgorithm(h);
    } else {
      this->writeStandardImplicitAlgorithm(h);
    }
    this->behaviourFile << "if(this->iter==this->iterMax){\n";
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : no convergence after \" "
			  << "<< this->iter << \" iterations\"<< endl << endl;\n";
      this->behaviourFile << "cout << *this << endl;\n";
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(h);
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : convergence after \" "
			  << "<< this->iter << \" iterations\"<< endl << endl;\n";
    }
    this->writeStandardPerformanceProfilingEnd(this->behaviourFile);
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if(this->nf.find(p->name)!=this->nf.end()){
	this->behaviourFile << "this->d" << p->name << " *= " << this->nf.find(p->name)->second << ";\n";
      }
    }
    this->behaviourFile << "this->updateIntegrationVariables();\n";
    this->behaviourFile << "this->updateStateVariables();\n";
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeFinalStress)){
      this->behaviourFile << "this->computeFinalStress();\n";
    }
    this->behaviourFile << "this->updateAuxiliaryStateVariables();\n";
    for(p3  = d.getBounds().begin();
	p3 != d.getBounds().end();++p3){
      if(p3->varCategory==BoundsDescription::StateVariable){
	p3->writeBoundsChecks(this->behaviourFile);
      }
    }
    this->behaviourFile << "if(smt!=NOSTIFFNESSREQUESTED){\n";
    if(this->mb.hasAttribute(h,MechanicalBehaviourData::hasConsistentTangentOperator)){
      if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	this->behaviourFile << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
      } else {
	this->behaviourFile << "if(!this->computeConsistentTangentOperator(smt)){\n";
      }
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}\n";
    } else {
      this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : \");\n";
      this->behaviourFile << "msg +=\"unimplemented feature\";\n";
      this->behaviourFile << "throw(runtime_error(msg));\n";
    }
    this->behaviourFile << "}\n";
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::integrate\n\n";
    this->writeComputeFdF(h);
  }

  void
  MFrontImplicitParserBase::writeLevenbergMarquardtAlgorithm(const Hypothesis h)
  {
    using namespace std;
    const string btype = this->convertBehaviourTypeToString();
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "// dumping parameter" << endl;
    this->behaviourFile << "real levmar_mu = this->levmar_mu0;" << endl;
    this->behaviourFile << "real error;" << endl;
    this->behaviourFile << "bool converged=false;" << endl;
    this->behaviourFile << "this->iter=0;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : beginning of resolution\" << endl;" << endl;
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();" << endl;
    }
    this->behaviourFile << "if(!this->computeFdF()){" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : computFdF returned false on first call, abording...\" << endl;" << endl;
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;" << endl;
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "error=norm(this->fzeros);" << endl;
    this->behaviourFile << "while((converged==false)&&" << endl;
    this->behaviourFile << "(this->iter<" << this->mb.getClassName() << "::iterMax)){" << endl;
    this->behaviourFile << "++(this->iter);" << endl;
    this->behaviourFile << "this->zeros_1  = this->zeros;" << endl;
    this->writeComparisonToNumericalJacobian(h);
    this->behaviourFile << "converged = ((error)/(real(" << n2 << "))<";
    this->behaviourFile << "(this->epsilon));" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : iteration \" "
			  << "<< this->iter << \" : \" << (error)/(real(" << n2 << ")) << \", dumping parameter : \" << levmar_mu << endl;" << endl;
    }
    this->behaviourFile << "if(!converged){" << endl;
    this->behaviourFile << "// matrix containing tJJ+levmar_mu*I" << endl;
    this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> levmar_m;" << endl;
    this->behaviourFile << "// vector containing tJ*F" << endl;
    this->behaviourFile << "tvector<" << n2 << ",real> levmar_sm;" << endl;
    this->behaviourFile << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl
			<< "levmar_sm(idx)=real(0);"
			<< "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){" << endl
			<< "levmar_sm(idx)+=(this->jacobian(idx2,idx))*(this->fzeros(idx2));"
			<< "levmar_m(idx,idx2)=real(0);" << endl
			<< "for(unsigned short idx3=0;idx3!=" << n2 << ";++idx3){" << endl
			<< "levmar_m(idx,idx2)+=(this->jacobian(idx3,idx))*(this->jacobian(idx3,idx2));" << endl
			<< "}" << endl
			<< "}" << endl
			<< "}" << endl
			<< "const real levmar_muF = (levmar_mu)*norm(this->fzeros);" << endl
			<< "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl
			<< "levmar_m(idx,idx)+=levmar_muF;" << endl
			<< "}" << endl;
    this->behaviourFile << "try{" << endl;
    this->writeStandardPerformanceProfilingBegin(this->behaviourFile,
						 "TinyMatrixSolve","lu");
    this->behaviourFile << "TinyMatrixSolve<" << n2
			<< "," << "real>::exe(levmar_m,levmar_sm);" << endl;
    this->writeStandardPerformanceProfilingEnd(this->behaviourFile);
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "catch(LUException&){" << endl;
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;" << endl;
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->writeLimitsOnIncrementValues(h,"levmar_sm");
    // levmar_sm contains the step
    this->behaviourFile << "this->zeros -= levmar_sm;" << endl; 
    this->behaviourFile << "tvector<" << n2 <<  ",real> levmar_fzeros_1 = this->fzeros;" << endl; 
    this->behaviourFile << "tmatrix<" << n2 << "," << n2 <<  ",real> levmar_jacobian_1 = this->jacobian;" << endl; 
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();" << endl;
    }
    this->behaviourFile << "if(!this->computeFdF()){" << endl;
    this->behaviourFile << "// rejecting the step" << endl;
    this->behaviourFile << "this->zeros     = this->zeros_1;" << endl; 
    this->behaviourFile << "this->fzeros    = levmar_fzeros_1;" << endl; 
    this->behaviourFile << "this->jacobian  = levmar_jacobian_1;" << endl; 
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();" << endl;
    }
    this->behaviourFile << "// updating mu" << endl;
    this->behaviourFile << "levmar_mu *= 4;" << endl;
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "const real error_1=error;" << endl;
    this->behaviourFile << "const real error_p=norm(levmar_fzeros_1-levmar_jacobian_1*levmar_sm);" << endl;
    this->behaviourFile << "error=norm(this->fzeros);" << endl;
    this->behaviourFile << "const real levmar_r = (error*error-error_1*error_1)/(error_p*error_p-error_1*error_1);" << endl;
    this->behaviourFile << "if(levmar_r<this->levmar_p0){" << endl;
    this->behaviourFile << "// rejecting the step" << endl;
    this->behaviourFile << "this->zeros     = this->zeros_1;" << endl; 
    this->behaviourFile << "this->fzeros    = levmar_fzeros_1;" << endl; 
    this->behaviourFile << "this->jacobian  = levmar_jacobian_1;" << endl; 
    this->behaviourFile << "error = error_1;" << endl;
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();" << endl;
    }
    this->behaviourFile << "levmar_mu *= 4;" << endl;
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "// accepting the step and updating mu" << endl;
    this->behaviourFile << "if(levmar_r<this->levmar_p1){" << endl;
    this->behaviourFile << "levmar_mu *= 4;" << endl;
    this->behaviourFile << "} else if(levmar_r>this->levmar_p2){" << endl;
    this->behaviourFile << "levmar_mu  = max(levmar_mu/4,this->levmar_m);" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(h);
    this->writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(h);
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
  } // end of MFrontImplicitParserBase::writeLevenbergMarquardtAlgorithm(const Hypothesis h)

  void
  MFrontImplicitParserBase::writeStandardImplicitAlgorithm(const Hypothesis h)
  {
    using namespace std;
    const string btype = this->convertBehaviourTypeToString();
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    if(this->mb.hasAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian)){
      this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> njacobian;\n";
    }
    if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
      this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> jacobian2;\n";
      this->behaviourFile << "tvector<" << n2 << ",real> fzeros2;\n";
      this->behaviourFile << "tvector<" << n2 << ",real> Dzeros;\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::BROYDEN2){
      this->behaviourFile << "tvector<" << n2 << ",real> Dfzeros;\n";
    }
    if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
      this->behaviourFile << "real broyden_inv;\n";
    }
    this->behaviourFile << "real error;\n";
    this->behaviourFile << "bool converged=false;\n";
    this->behaviourFile << "this->iter=0;\n";
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile << "while((converged==false)&&\n";
    this->behaviourFile << "(this->iter<" << this->mb.getClassName() << "::iterMax)){\n";
    this->behaviourFile << "++(this->iter);\n";
    if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
      this->behaviourFile << "fzeros2 = this->fzeros;\n";
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "const bool computeFdF_ok = this->computeFdF();\n";
    this->behaviourFile << "if(!computeFdF_ok){\n";
    this->behaviourFile << "if(this->iter==1){\n";
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : computFdF returned false on first iteration, abording...\" << endl;\n";
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "} else {\n";
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : computFdF returned false, dividing increment by two...\" << endl;\n";
    }
    this->behaviourFile << "const real integrate_one_half = real(1)/real(2);\n";
    this->behaviourFile << "this->zeros -= (this->zeros-this->zeros_1)*integrate_one_half;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "this->zeros_1  = this->zeros;\n";
    this->writeComparisonToNumericalJacobian(h);
    this->behaviourFile << "error=norm(this->fzeros);\n";
    this->behaviourFile << "converged = ((error)/(real(" << n2 << "))<";
    this->behaviourFile << "(this->epsilon));\n";
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)){
      // We compute the numerical jacobian even if we converged since
      // most of the time, this tangent operator will be computed
      // using the partial jacobian invert. We consider very unlikely
      // that a user may use a numerical jacobian and provide an
      // analytic definition of the tangent operator
      this->behaviourFile << "if((!converged)||(smt!=NOSTIFFNESSREQUESTED)){" << endl
			  << "this->computeNumericalJacobian(this->jacobian);" << endl
			  << "}" << endl;
    }
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : iteration \" "
			  << "<< this->iter << \" : \" << (error)/(real(" << n2 << ")) << endl;\n";
    }
    this->behaviourFile << "if(!converged){\n";
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)){
      if((this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
	 (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)){
	this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> tjacobian(this->jacobian);\n";
	this->behaviourFile << "tvector<" << n2 << ",real> tfzeros(this->fzeros);\n";
      }
      this->behaviourFile << "try{" << endl;
      this->writeStandardPerformanceProfilingBegin(this->behaviourFile,
						   "TinyMatrixSolve","lu");
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(this->jacobian,this->fzeros);\n";
      this->writeStandardPerformanceProfilingEnd(this->behaviourFile);
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "catch(LUException&){" << endl;
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}" << endl;
      if((this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
	 (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)){
	this->writePowellDogLegStep(h,"tjacobian","tfzeros","fzeros");
      } else {
	this->writeLimitsOnIncrementValues(h,"fzeros");
	this->behaviourFile << "this->zeros -= this->fzeros;\n";
      }
    }
    if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)){
      this->behaviourFile << "Dzeros = this->fzeros;\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "try{" << endl;
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(jacobian2,Dzeros);\n";
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "catch(LUException&){" << endl;
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      if(this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN){
	this->writePowellDogLegStep(h,"this->jacobian","fzeros","Dzeros");
      } else {
	this->writeLimitsOnIncrementValues(h,"Dzeros");
	this->behaviourFile << "this->zeros -= Dzeros;\n";
      }
      this->behaviourFile << "if(this->iter>1){\n";
      this->behaviourFile << "broyden_inv = (Dzeros|Dzeros);\n";
      this->behaviourFile << "if(broyden_inv>100*std::numeric_limits<real>::epsilon()){\n";
      this->behaviourFile << "this->jacobian += "
			  << "(((this->fzeros-fzeros2)-(jacobian2)*(Dzeros))^Dzeros)/broyden_inv;\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "}\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::BROYDEN2){
      this->behaviourFile << "Dzeros   = -(this->jacobian)*(this->fzeros);\n";
      this->writeLimitsOnIncrementValues(h,"Dzeros");
      this->behaviourFile << "this->zeros  += Dzeros;\n";
      this->behaviourFile << "if(this->iter>1){\n";
      this->behaviourFile << "Dfzeros   = (this->fzeros)-fzeros2;\n";
      this->behaviourFile << "broyden_inv = Dzeros|((this->jacobian)*Dfzeros);\n";
      this->behaviourFile << "if(broyden_inv>100*std::numeric_limits<real>::epsilon()){\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "this->jacobian += "
			  << "((Dzeros-jacobian2*Dfzeros)^(Dzeros*jacobian2))/(broyden_inv);\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "}\n";
    }
    if(this->mb.getAttribute(h,MechanicalBehaviourData::useCast3MAccelerationAlgorithm,false)){
      this->behaviourFile << "this->previous_fzeros[this->iter%3] = this->fzeros;\n";
      this->behaviourFile << "this->previous_zeros[this->iter%3]  = this->zeros;\n";
      this->behaviourFile << "if((this->iter>this->accelerationTrigger" << ")&&\n"
			  <<  "((this->iter-this->accelerationTrigger)%"
			  << "(this->accelerationPeriod)==0)){\n";
      this->behaviourFile << "this->accelerate(this->zeros);\n";
      this->behaviourFile << "}\n";
    }
    if(this->mb.getAttribute(h,MechanicalBehaviourData::useRelaxationAlgorithm,false)){
      this->behaviourFile << "if(this->iter>=this->relaxationTrigger" << "){\n";
      this->behaviourFile << "this->zeros   -= (1-this->relaxationCoefficient) * (this->zeros-this->zeros_1);\n";
      this->behaviourFile << "}\n";
    }
    this->writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(h);
    this->writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(h);
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
  } // end of MFrontImplicitParserBase::writeStandardImplicitAlgorithm

  void
  MFrontImplicitParserBase::writeComparisonToNumericalJacobian(const Hypothesis h)
  {
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    if(this->mb.getAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,false)){
      this->behaviourFile << "this->computeNumericalJacobian(njacobian);\n";
      n = SupportedTypes::TypeSize();
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	if(p->arraySize==1){
	  n3 = SupportedTypes::TypeSize();
	  for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	    if((p->arraySize==1)&&(p2->arraySize==1)){
	      this->behaviourFile << "// derivative of variable f" << p->name 
				  << " by variable " << p2->name << "\n";
	      this->behaviourFile << this->getJacobianPart(*p,*p2,n,n2,n3);
	      this->behaviourFile << "// numerical derivative of variable f" << p->name 
				  << " by variable " << p2->name << "\n";
	      this->behaviourFile << this->getJacobianPart(*p,*p2,n,n2,n3,
							   "njacobian","n");
	      n3 += this->getTypeSize(p2->type,p2->arraySize);
	    }
	  }
	}
	n += this->getTypeSize(p->type,p->arraySize);
      }
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	  const VariableDescription& v1 = *p;
	  const VariableDescription& v2 = *p2;
	  SupportedTypes::TypeSize nv1 = this->getTypeSize(v1.type,1u);
	  SupportedTypes::TypeSize nv2 = this->getTypeSize(v2.type,1u);
	  this->behaviourFile << "error=" << nv1 << "*" << nv2 << "*"
			      << "(this->jacobianComparisonCriterium)" <<";\n";
	  if((v1.arraySize==1u)&&(v2.arraySize==1u)){
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
	  } else if(((v1.arraySize!=1u)&&(v2.arraySize==1u))||
		    ((v2.arraySize!=1u)&&(v1.arraySize==1u))){
	    unsigned short asize;
	    if(v1.arraySize!=1u){
	      asize = v1.arraySize;
	    } else {
	      asize = v2.arraySize;
	    }
	    this->behaviourFile << "for(unsigned short idx=0;idx!=" << asize << ";++idx){\n";
	    this->behaviourFile << "if(abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx)-"
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx)) > error)\n" 
				<< "{\n";
	    this->behaviourFile << "cout << abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx)-"
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx)) << \" \" << error << endl;\n";
	    this->behaviourFile << "cout << \"df" << v1.name
				<< "_dd" << v2.name << "(\" << idx << \") :\\n\" << " 
				<< "df" << v1.name  << "_dd" << v2.name << "(idx) << endl;\n";
	    this->behaviourFile << "cout << \"ndf" << v1.name
				<< "_dd" << v2.name << "(\" << idx << \") :\\n\" << " 
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx) << endl;\n";
	    this->behaviourFile << "cout << \"df" << v1.name << "_dd" << v2.name 
				<< " - ndf" << v1.name << "_dd" << v2.name << "(\" << idx << \") :\\n\" << "
				<< "df" << v1.name  << "_dd" << v2.name << "(idx) -"
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx) << endl;\n";
	    this->behaviourFile << "cout << endl;\n";
	    this->behaviourFile << "}\n";
	    this->behaviourFile << "}\n";
	  } else {
	    this->behaviourFile << "for(unsigned short idx=0;idx!=" << v1.arraySize << ";++idx){\n";
	    this->behaviourFile << "for(unsigned short idx2=0;idx2!=" << v2.arraySize << ";++idx2){\n";
	    this->behaviourFile << "if(abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx,idx2)-"
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx,idx2)) > error)\n" 
				<< "{\n";
	    this->behaviourFile << "cout << abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx,idx2)-"
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx,idx2)) << \" \" << error << endl;\n";
	    this->behaviourFile << "cout << \"df" << v1.name
				<< "_dd" << v2.name << "(\" << idx << \",\" << idx2 << \") :\\n\" << " 
				<< "df" << v1.name  << "_dd" << v2.name << "(idx,idx2) << endl;\n";
	    this->behaviourFile << "cout << \"ndf" << v1.name
				<< "_dd" << v2.name << "(\" << idx << \",\" << idx2 << \") :\\n\" << " 
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx,idx2) << endl;\n";
	    this->behaviourFile << "cout << \"df" << v1.name << "_dd" << v2.name 
				<< " - ndf" << v1.name << "_dd" << v2.name << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
				<< "df" << v1.name  << "_dd" << v2.name << "(idx,idx2) -"
				<< "df" << v1.name  << "_dd" << v2.name << "(njacobian,idx,idx2) << endl;\n";
	    this->behaviourFile << "cout << endl;\n";
	    this->behaviourFile << "}\n";
	    this->behaviourFile << "}\n";
	    this->behaviourFile << "}\n";
	  }
	}
      }
    }
  }

  void
  MFrontImplicitParserBase::writeComputeFdF(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief compute fzeros and jacobian\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "bool\n";
    this->behaviourFile << "computeFdF(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using std::vector;\n";
    this->writeStandardPerformanceProfiling(this->behaviourFile,
					    "ComputeFdF");
    writeMaterialLaws("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->mb.getMaterialLaws());
    n = SupportedTypes::TypeSize();
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if(this->varNames.find("f"+p->name)!=this->varNames.end()){
	string msg("MFrontImplicitParserBase::writeBehaviourIntegrator : ");
	msg += "variable name 'f"+p->name;
	msg += "' is reserved.\n";
	throw(runtime_error(msg));
      }
      if(this->getTypeFlag(p->type)==SupportedTypes::Scalar){
	if(p->arraySize==1u){
	  this->behaviourFile << "real& f" << p->name << "(this->fzeros(" << n << "));\n";
	} else {
	  this->behaviourFile << "typename tfel::math::TinyVectorFromTinyVectorView<" 
			      << p->arraySize << "," << n2 << "," << n
			      << ",real,false>::type"
			      << " f" << p->name << "(this->fzeros);\n";
	}
      } else {
	if(p->arraySize==1u){
	  this->behaviourFile << "typename tfel::math::"
			      << this->getVectorMappingClass(*p)
			      << "<N," << n2 << "," << n << ",real>::type"
			      << " f" << p->name << "(this->fzeros);\n";
	} else {
	  this->behaviourFile << "typename tfel::math::"
			      << this->getVectorMappingClass(*p)
			      << "<N," << n2 << "," << n << "," << p-> arraySize << ",real>::type"
			      << " f" << p->name << "(this->fzeros);\n";
	}
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT_NJ)){
      n = SupportedTypes::TypeSize();
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	n3 = SupportedTypes::TypeSize();
	for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	  if((p->arraySize==1u)&&
	     (p2->arraySize==1u)){
	    this->behaviourFile << "// derivative of variable f" << p->name 
				<< " by variable " << p2->name << "\n";
	    this->behaviourFile << this->getJacobianPart(*p,*p2,n,n2,n3);
	  }
	  n3 += this->getTypeSize(p2->type,p2->arraySize);
	}
	n += this->getTypeSize(p->type,p->arraySize);
      }
    }
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT)){
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	this->jacobianPartsUsedInIntegrator.insert("df"+p->name+"_dd"+p->name);
      }
      this->behaviourFile << "// setting jacobian to identity\n";
      this->behaviourFile << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n";
      this->behaviourFile << "for(unsigned short idx = 0; idx!=" << n << ";++idx){\n"
			  << "this->jacobian(idx,idx)= real(1);\n"
			  << "}\n";
    }
    this->behaviourFile << "// setting f values to zeros\n";
    this->behaviourFile << "this->fzeros = this->zeros;\n";
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if(this->nf.find(p->name)!=this->nf.end()){
    	this->behaviourFile << "f" << p->name << " *= " << this->nf.find(p->name)->second << ";" << endl;
      }
    }
    this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::Integrator).code << "\n";
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if(this->nf.find('f'+p->name)!=this->nf.end()){
    	this->behaviourFile << "f" << p->name << "*= real(1)/(" << this->nf.find('f'+p->name)->second << ");" << endl;
      }
    }
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT)){
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	  if((p->arraySize==1u)&&(p2->arraySize==1u)){
	    this->behaviourFile << "static_cast<void>(df"
				<< p->name << "_dd" << p2->name
				<< "); /* suppress potential warnings */\n";
	  }
	  if(this->jacobianPartsUsedInIntegrator.find("df"+p->name+"_dd"+p2->name)!=
	     this->jacobianPartsUsedInIntegrator.end()){
	    map<string,string>::const_iterator pjf = this->nf.find('f'+p->name);
	    map<string,string>::const_iterator pjv = this->nf.find(p2->name);
	    if(pjf!=this->nf.end()){
	      if(pjv!=this->nf.end()){
		if((p->arraySize!=1u)&&
		   (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				      << p2->arraySize << ";++idx2){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx,idx2) *= " 
				      << pjv->second << "/(" << pjf->second << ");" << endl;
		  this->behaviourFile << "}\n";
		  this->behaviourFile << "}\n";
		} else if((p->arraySize!=1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << pjv->second << "/(" << pjf->second << ");" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p2->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << pjv->second << "/(" << pjf->second << ");" << endl;		  
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << " *= " 
				      << pjv->second << "/(" << pjf->second << ");" << endl;
		}
	      } else {
		if((p->arraySize!=1u)&&
		   (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				      << p2->arraySize << ";++idx2){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx,idx2) *= " 
				      << "real(1)/(" << pjf->second << ");" << endl;
		  this->behaviourFile << "}\n";
		  this->behaviourFile << "}\n";
		} else if((p->arraySize!=1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << "real(1)/(" << pjf->second << ");" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p2->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << "real(1)/(" << pjf->second << ");" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << " *= " 
				      << "real(1)/(" << pjf->second << ");" << endl;
		}
	      }
	    } else{
	      if(pjv!=this->nf.end()){
		if((p->arraySize!=1u)&&
		   (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				      << p2->arraySize << ";++idx2){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx,idx2) *= " 
				      << pjv->second << ";" << endl;
		  this->behaviourFile << "}\n";
		  this->behaviourFile << "}\n";
		} else if((p->arraySize!=1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << pjv->second << ";" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p2->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << pjv->second << ";" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << " *= " 
				      << pjv->second << ";" << endl;
		}
	      }
	    }
	  }
	}
      }
    }
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  } // end of MFrontImplicitParserBase::writeBehaviourIntegrator

  void
  MFrontImplicitParserBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    SupportedTypes::TypeSize n;
    VariableDescriptionContainer::const_iterator p;
    vector<BoundsDescription>::const_iterator p2;
    for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      for(p2  = d.getBounds().begin();p2 != d.getBounds().end();++p2){
	if(p2->name==p->name){
	  // treating lower bounds
	  if(((p2->boundsType==BoundsDescription::Lower)||(p2->boundsType==BoundsDescription::LowerAndUpper))&&
	     (p2->category==BoundsDescription::Physical)){
	    if((this->getTypeFlag(p->type)==SupportedTypes::Scalar)&&(p->arraySize==1u)){
	      if(this->nf.find(p->name)!=this->nf.end()){
		this->behaviourFile << "if(this->" << p->name << "+ " << nf[p->name ]<< "*(this->zeros[" << n << "]) <" << p2->lowerBound << "){\n";
	      } else {
		this->behaviourFile << "if(this->" << p->name << "+this->zeros[" << n << "]<" << p2->lowerBound << "){\n";
	      }
	      if(abs(p2->lowerBound)>numeric_limits<long double>::min()){
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "] = (" << p2->lowerBound << "- (this->" << p->name << "))/(" << nf[p->name ]<< ");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "] = " << p2->lowerBound << "- (this->" << p->name << ");\n";
		}
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "] = - (this->" << p->name << ")/(" << nf[p->name] << ");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "] = - (this->" << p->name << ");\n";
		}
	      }
	      this->behaviourFile << "}\n";
	    }
	    if((this->getTypeFlag(p->type)==SupportedTypes::Scalar)&&(p->arraySize!=1u)){
	      this->behaviourFile << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	      if(this->nf.find(p->name)!=this->nf.end()){
		this->behaviourFile << "if(this->" << p->name << "[idx]+(" << nf[p->name] << ")*(this->zeros[" << n << "+idx])<" << p2->lowerBound << "){\n";
	      } else {
		this->behaviourFile << "if(this->" << p->name << "[idx]+this->zeros[" << n << "+idx]<" << p2->lowerBound << "){\n";
	      }
	      if(abs(p2->lowerBound)>numeric_limits<long double>::min()){
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "+idx] = (" << p2->lowerBound << "- (this->" << p->name << "[idx]))/(" << nf[p->name] <<");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "+idx] = " << p2->lowerBound << "- (this->" << p->name << "[idx]);\n";
		}
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx])/(" << nf[p->name] << ");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx]);\n";
		}
	      }
	      this->behaviourFile << "}\n";
	      this->behaviourFile << "}\n";
	    }
	    if((this->getTypeFlag(p->type)!=SupportedTypes::Scalar)&&(p->arraySize==1u)){
	      if(p2->component==-1){
		SupportedTypes::TypeSize n2 =  this->getTypeSize(p->type,1u);
		this->behaviourFile << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl;
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[idx]+(" << nf[p->name] << ")*(this->zeros[" << n << "+idx])<" << p2->lowerBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[idx]+this->zeros[" << n << "+idx]<" << p2->lowerBound << "){\n";
		}
		if(abs(p2->lowerBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+idx] = (" << p2->lowerBound << "- (this->" << p->name << "[idx]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+idx] = " << p2->lowerBound << "- (this->" << p->name << "[idx]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx]);\n";
		  }
		}
		this->behaviourFile << "}\n";
		this->behaviourFile << "}\n";
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[" << p2->component << "]+(" << nf[p->name] << ")*(this->zeros[" << n << "+" << p2->component << "])<" << p2->lowerBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[" << p2->component << "]+this->zeros[" << n << "+" << p2->component << "]<" << p2->lowerBound << "){\n";
		}
		if(abs(p2->lowerBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = (" << p2->lowerBound << "- (this->" << p->name << "[" << p2->component << "]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = " << p2->lowerBound << "- (this->" << p->name << "[" << p2->component << "]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = - (this->" << p->name << "[" << p2->component << "])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = - (this->" << p->name << "[" << p2->component << "]);\n";
		  }
		}
		this->behaviourFile << "}\n";
	      }
	    }
	    if((this->getTypeFlag(p->type)!=SupportedTypes::Scalar)&&(p->arraySize!=1u)){
	      SupportedTypes::TypeSize n2 =  this->getTypeSize(p->type,1u);
	      this->behaviourFile << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	      if(p2->component==-1){
		this->behaviourFile << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){" << endl;
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[idx][idx2]+(" << nf[p->name] << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])<" << p2->lowerBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[idx][idx2]+this->zeros[" << n << "+" << n2 << "*idx+idx2]<" << p2->lowerBound << "){\n";
		}
		if(abs(p2->lowerBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = (" << p2->lowerBound << "- (this->" << p->name << "[idx][idx2]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = " << p2->lowerBound << "- (this->" << p->name << "[idx][idx2]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << p->name << "[idx][idx2])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << p->name << "[idx][idx2]);\n";
		  }
		}
		this->behaviourFile << "}\n";
		this->behaviourFile << "}\n";
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[idx][" << p2->component << "]+(" << nf[p->name] << ")*(this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "])<" << p2->lowerBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[idx][" << p2->component << "]+this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "]<" << p2->lowerBound << "){\n";
		}
		if(abs(p2->lowerBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = (" << p2->lowerBound << "- (this->" << p->name << "[idx][" << p2->component << "]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = " << p2->lowerBound << "- (this->" << p->name << "[idx][" << p2->component << "]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = - (this->" << p->name << "[idx][" << p2->component << "])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = - (this->" << p->name << "[idx][" << p2->component << "]);\n";
		  }
		}
		this->behaviourFile << "}\n";
	      }
	      this->behaviourFile << "}\n";
	    }
	  }
	  // treating upper bounds
	  if(((p2->boundsType==BoundsDescription::Upper)||(p2->boundsType==BoundsDescription::LowerAndUpper))&&
	     (p2->category==BoundsDescription::Physical)){
	    if((this->getTypeFlag(p->type)==SupportedTypes::Scalar)&&(p->arraySize==1u)){
	      if(this->nf.find(p->name)!=this->nf.end()){
		this->behaviourFile << "if(this->" << p->name << "+ " << nf[p->name] << "*(this->zeros[" << n << "]) >" << p2->upperBound << "){\n";
	      } else {
		this->behaviourFile << "if(this->" << p->name << "+this->zeros[" << n << "]>" << p2->upperBound << "){\n";
	      }
	      if(abs(p2->upperBound)>numeric_limits<long double>::min()){
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "] = (" << p2->upperBound << "- (this->" << p->name << "))/(" << nf[p->name] << ");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "] = " << p2->upperBound << "- (this->" << p->name << ");\n";
		}
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "] = - (this->" << p->name << ")/(" << nf[p->name] << ");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "] = - (this->" << p->name << ");\n";
		}
	      }
	      this->behaviourFile << "}\n";
	    }
	    if((this->getTypeFlag(p->type)==SupportedTypes::Scalar)&&(p->arraySize!=1u)){
	      this->behaviourFile << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	      if(this->nf.find(p->name)!=this->nf.end()){
		this->behaviourFile << "if(this->" << p->name << "[idx]+(" << nf[p->name] << ")*(this->zeros[" << n << "+idx])>" << p2->upperBound << "){\n";
	      } else {
		this->behaviourFile << "if(this->" << p->name << "[idx]+this->zeros[" << n << "+idx]>" << p2->upperBound << "){\n";
	      }
	      if(abs(p2->upperBound)>numeric_limits<long double>::min()){
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "+idx] = (" << p2->upperBound << "- (this->" << p->name << "[idx]))/(" << nf[p->name] <<");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "+idx] = " << p2->upperBound << "- (this->" << p->name << "[idx]);\n";
		}
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx])/(" << nf[p->name] << ");\n";
		} else {
		  this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx]);\n";
		}
	      }
	      this->behaviourFile << "}\n";
	      this->behaviourFile << "}\n";
	    }
	    if((this->getTypeFlag(p->type)!=SupportedTypes::Scalar)&&(p->arraySize==1u)){
	      if(p2->component==-1){
		SupportedTypes::TypeSize n2 =  this->getTypeSize(p->type,1u);
		this->behaviourFile << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl;
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[idx]+(" << nf[p->name] << ")*(this->zeros[" << n << "+idx])>" << p2->upperBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[idx]+this->zeros[" << n << "+idx]>" << p2->upperBound << "){\n";
		}
		if(abs(p2->upperBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+idx] = (" << p2->upperBound << "- (this->" << p->name << "[idx]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+idx] = " << p2->upperBound << "- (this->" << p->name << "[idx]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+idx] = - (this->" << p->name << "[idx]);\n";
		  }
		}
		this->behaviourFile << "}\n";
		this->behaviourFile << "}\n";
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[" << p2->component << "]+(" << nf[p->name] << ")*(this->zeros[" << n << "+" << p2->component << "])>" << p2->upperBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[" << p2->component << "]+this->zeros[" << n << "+" << p2->component << "]>" << p2->upperBound << "){\n";
		}
		if(abs(p2->upperBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = (" << p2->upperBound << "- (this->" << p->name << "[" << p2->component << "]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = " << p2->upperBound << "- (this->" << p->name << "[" << p2->component << "]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = - (this->" << p->name << "[" << p2->component << "])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << p2->component << "] = - (this->" << p->name << "[" << p2->component << "]);\n";
		  }
		}
		this->behaviourFile << "}\n";
	      }
	    }
	    if((this->getTypeFlag(p->type)!=SupportedTypes::Scalar)&&(p->arraySize!=1u)){
	      SupportedTypes::TypeSize n2 =  this->getTypeSize(p->type,1u);
	      this->behaviourFile << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){" << endl;
	      if(p2->component==-1){
		this->behaviourFile << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){" << endl;
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[idx][idx2]+(" << nf[p->name] << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])>" << p2->upperBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[idx][idx2]+this->zeros[" << n << "+" << n2 << "*idx+idx2]>" << p2->upperBound << "){\n";
		}
		if(abs(p2->upperBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = (" << p2->upperBound << "- (this->" << p->name << "[idx][idx2]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = " << p2->upperBound << "- (this->" << p->name << "[idx][idx2]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << p->name << "[idx][idx2])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << p->name << "[idx][idx2]);\n";
		  }
		}
		this->behaviourFile << "}\n";
		this->behaviourFile << "}\n";
	      } else {
		if(this->nf.find(p->name)!=this->nf.end()){
		  this->behaviourFile << "if(this->" << p->name << "[idx][" << p2->component << "]+(" << nf[p->name] << ")*(this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "])>" << p2->upperBound << "){\n";
		} else {
		  this->behaviourFile << "if(this->" << p->name << "[idx][" << p2->component << "]+this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "]>" << p2->upperBound << "){\n";
		}
		if(abs(p2->upperBound)>numeric_limits<long double>::min()){
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = (" << p2->upperBound << "- (this->" << p->name << "[idx][" << p2->component << "]))/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = " << p2->upperBound << "- (this->" << p->name << "[idx][" << p2->component << "]);\n";
		  }
		} else {
		  if(this->nf.find(p->name)!=this->nf.end()){
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = - (this->" << p->name << "[idx][" << p2->component << "])/(" << nf[p->name] << ");\n";
		  } else {
		    this->behaviourFile << "this->zeros[" << n << "+" << n2 << "*idx+" << p2->component << "] = - (this->" << p->name << "[idx][" << p2->component << "]);\n";
		  }
		}
		this->behaviourFile << "}\n";
	      }
	      this->behaviourFile << "}\n";
	    }
	  }
	}
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
  } // end of MFrontImplicitParserBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds

  void
  MFrontImplicitParserBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(const Hypothesis)
  {
    

  } // end of MFrontImplicitParserBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds

  void
  MFrontImplicitParserBase::writePowellDogLegStep(const Hypothesis   h,
						  const std::string& B,
						  const std::string& f,
						  const std::string& pn)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "if(abs(" << pn<< ")<(" << n << ")*(this->powell_dogleg_trust_region_size)){" << endl;
    this->behaviourFile << "// using the newton method only" << endl;
    this->writeLimitsOnIncrementValues(h,pn);
    this->behaviourFile << "this->zeros -= " << pn<< ";\n";
    this->behaviourFile << "} else { " << endl;
    this->behaviourFile << "// computing the steepest descent step\n";
    this->behaviourFile << "tvector<" << n << ",real> pdl_g;\n";
    this->behaviourFile << "tvector<" << n << ",real> pdl_g2;\n";
    this->behaviourFile << "for(unsigned short idx=0;idx!=" << n << ";++idx){" << endl;
    this->behaviourFile << "pdl_g[idx]=real(0);\n";
    this->behaviourFile << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){" << endl;
    this->behaviourFile << "pdl_g[idx] += (" << B << "(idx2,idx)) * (" << f << "(idx2));\n";
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "for(unsigned short idx=0;idx!=" << n << ";++idx){" << endl;
    this->behaviourFile << "pdl_g2[idx]=real(0);\n";
    this->behaviourFile << "for(unsigned short idx2=0;idx2!=" << n << ";++idx2){" << endl;
    this->behaviourFile << "pdl_g2[idx] += (" << B << "(idx,idx2)) * pdl_g(idx2);\n";
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "const real pdl_cste = (pdl_g|pdl_g)/(pdl_g2|pdl_g2);" << endl;
    this->behaviourFile << "pdl_g *= pdl_cste;" << endl;
    this->behaviourFile << "if(abs(pdl_g)<(" << n << ")*(this->powell_dogleg_trust_region_size)){" << endl;
    this->behaviourFile << "const real pdl_0 = (this->powell_dogleg_trust_region_size)*(this->powell_dogleg_trust_region_size);" << endl;
    this->behaviourFile << "const real pdl_1 = (pdl_g|pdl_g);" << endl;
    this->behaviourFile << "const real pdl_2 = ((" << pn << ")|pdl_g);" << endl;
    this->behaviourFile << "const real pdl_3 = ((" << pn << ")|(" << pn << "));" << endl;
    this->behaviourFile << "const real pdl_alpha = "
			<< "(pdl_0-pdl_1)/((pdl_2-pdl_1)+sqrt(max((pdl_2-pdl_0)*(pdl_2-pdl_0)+(pdl_3-pdl_0)*(pdl_0-pdl_1),real(0))));" << endl;
    this->behaviourFile << "pdl_g = pdl_alpha*(" << pn<< ") + (1-pdl_alpha)*pdl_g;" << endl;
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "const real pdl_alpha = (this->powell_dogleg_trust_region_size)/(norm(pdl_g));" << endl;
    this->behaviourFile << "pdl_g *= pdl_alpha;" << endl;
    this->behaviourFile << "}" << endl;
    this->writeLimitsOnIncrementValues(h,"pdl_g");
    this->behaviourFile << "this->zeros -= pdl_g;\n";
    this->behaviourFile << "}" << endl;
  } // end of MFrontImplicitParserBase::writePowellDogLegStep

  std::string
  MFrontImplicitParserBase::getIntegrationVariablesIncrementsInitializers(const VariableDescriptionContainer& v,
									  const bool) const
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    ostringstream init;
    for(p=v.begin();p!=v.end();++p){
      SupportedTypes::TypeFlag flag = getTypeFlag(p->type);
      if(p!=v.begin()){
	init << ",\n";
      }
      if((flag!=SupportedTypes::Scalar)&&
	 (flag!=SupportedTypes::Stensor)){
	string msg("MFrontImplicitParserBase::writeBehaviourConstructors : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if((flag==SupportedTypes::Scalar)&&(p->arraySize==1u)){
	init << "d" << p->name << "(this->zeros(" << n << "))";
      } else {
	init << "d" << p->name 
		      << "(this->zeros)";
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
    return init.str();
  } // end of MFrontImplicitParserBase::getIntegrationVariableIncrementsInitializers

  std::string
  MFrontImplicitParserBase::getBehaviourConstructorsInitializers(const Hypothesis h)
  {    
    using namespace std;
    string init = MFrontBehaviourParserCommon::getBehaviourConstructorsInitializers(h);
    if(!init.empty()){
      init += ",\n";
    }
    init += "zeros(real(0)),\nfzeros(real(0))";
    return init;
  }

  void MFrontImplicitParserBase::writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis h)
  {
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    this->checkBehaviourFile();
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::InitializeJacobian)){
      this->writeStandardPerformanceProfilingBegin(this->behaviourFile,
						   MechanicalBehaviourData::InitializeJacobian);
      this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::InitializeJacobian).code;
      this->writeStandardPerformanceProfilingEnd(this->behaviourFile);
    } else {
      if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
	 (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
	 (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
	this->behaviourFile << "// setting jacobian to identity\n";
	this->behaviourFile << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n";
	this->behaviourFile << "for(unsigned short idx = 0; idx!= "<< n << ";++idx){\n";
	this->behaviourFile << "this->jacobian(idx,idx)= real(1);\n";
	this->behaviourFile << "}\n";
      }
    }
  }

  void MFrontImplicitParserBase::writeBehaviourIntegrationVariablesIncrements(const Hypothesis h)
  {    
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    this->checkBehaviourFile();
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if((!getDebugMode())&&(p->lineNumber!=0u)){
	this->behaviourFile << "#line " << p->lineNumber << " \"" 
			    << this->fileName << "\"\n";
      }
      if(this->getTypeFlag(p->type)==SupportedTypes::Scalar){
	if(p->arraySize==1u){
	  this->behaviourFile << "real& d" << p->name << ";\n";
	} else {
	  this->behaviourFile << "typename tfel::math::TinyVectorFromTinyVectorView<" 
			      <<  p->arraySize << "," << n2 << "," << n
			      << ",real,false>::type"
			      << " d" << p->name << ";\n";
	}
      } else {
	if(p->arraySize==1u){
	  this->behaviourFile << "typename tfel::math::"
			      << this->getVectorMappingClass(*p)    
			      << "<N," << n2 << "," << n << ",real>::type"
			      << " d" << p->name << ";\n";
	} else {
	  this->behaviourFile << "typename tfel::math::"
			      << this->getVectorMappingClass(*p)    
			      << "<N," << n2 << "," << n << "," << p-> arraySize << ",real>::type"
			      << " d" << p->name << ";\n";
	}
      }
      n += this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << endl;
  }

  void
  MFrontImplicitParserBase::endsInputFileProcessing(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef unsigned short ushort;
    MFrontBehaviourParserCommon::endsInputFileProcessing();
    // create the compute final stress code is necessary
    this->setComputeFinalStressFromComputeFinalStressCandidateIfNecessary();
    // creating default parameters if not explicitely specified by the user
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!this->mb.hasParameter(h,"epsilon")){
      VariableDescription e("real","epsilon",1u,0u);
      e.description = "value used to stop the iteration of the implicit algorithm";
      this->mb.addParameter(h,e);
      this->mb.setEntryName(h,"epsilon","epsilon");
      this->mb.setParameterDefaultValue(h,"epsilon",1.e-8);
    }
    if(!this->mb.hasParameter(h,"theta")){
      VariableDescription tv("real","theta",1u,0u);
      tv.description = "theta value used by the implicit scheme";
      this->mb.addParameter(h,tv);
      this->mb.setEntryName(h,"theta","theta");
      this->mb.setParameterDefaultValue(h,"theta",0.5);
    }
    if((this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)){
      if(!this->mb.hasParameter(h,"powell_dogleg_trust_region_size")){
	this->mb.addParameter(h,VariableDescription("real","powell_dogleg_trust_region_size",1u,0u));
	this->mb.setParameterDefaultValue(h,"powell_dogleg_trust_region_size",1.e-4);
      }
    }
    if((this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT_NJ)){
      if(!this->mb.hasParameter(h,"levmar_mu0")){
	this->mb.addParameter(h,VariableDescription("real","levmar_mu0",1u,0u));
	this->mb.setParameterDefaultValue(h,"levmar_mu0",1.e-6);
      }
      if(!this->mb.hasParameter(h,"levmar_p0")){
	this->mb.addParameter(h,VariableDescription("real","levmar_p0",1u,0u));
	this->mb.setParameterDefaultValue(h,"levmar_p0",1.e-4);
      }
      if(!this->mb.hasParameter(h,"levmar_p1")){
	this->mb.addParameter(h,VariableDescription("real","levmar_p1",1u,0u));
	this->mb.setParameterDefaultValue(h,"levmar_p1",0.25);
      }
      if(!this->mb.hasParameter(h,"levmar_p2")){
	this->mb.addParameter(h,VariableDescription("real","levmar_p2",1u,0u));
	this->mb.setParameterDefaultValue(h,"levmar_p2",0.75);
      }
      if(!this->mb.hasParameter(h,"levmar_m")){
	this->mb.addParameter(h,VariableDescription("real","levmar_m",1u,0u));
	this->mb.setParameterDefaultValue(h,"levmar_m",1.e-8);
      }
    }
    if((this->mb.getAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,false))||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON_NJ)||
       (this->algorithm==MFrontImplicitParserBase::LEVENBERGMARQUARDT_NJ)){
      const string nje = "numerical_jacobian_epsilon";
      if(!this->mb.hasParameter(h,nje)){
	const double eps = 0.1*this->mb.getFloattingPointParameterDefaultValue(h,"epsilon");
	this->mb.addParameter(h,VariableDescription("real",nje,1u,0u));
	this->mb.setParameterDefaultValue(h,nje,eps);
      }
    }
    if(!this->mb.hasParameter(h,"iterMax")){
      unsigned short iterMax = 100u;
      this->mb.addParameter(h,VariableDescription("ushort","iterMax",1u,0u));
      this->mb.setParameterDefaultValue(h,"iterMax",iterMax);
    }
    if(this->mb.getAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,false)){
      if(!this->mb.hasParameter(h,"jacobianComparisonCriterium")){
	this->mb.addParameter(h,VariableDescription("real","jacobianComparisonCriterium",1u,0u));
	this->mb.setParameterDefaultValue(h,"jacobianComparisonCriterium",
					  this->mb.getFloattingPointParameterDefaultValue(h,"epsilon"));
      }
    }
    if(this->mb.getAttribute(h,MechanicalBehaviourData::useRelaxationAlgorithm,false)){
      if(!this->mb.hasParameter(h,"relaxationCoefficient")){
	this->mb.addParameter(h,VariableDescription("real","relaxationCoefficient",1u,0u));
	this->mb.setParameterDefaultValue(h,"relaxationCoefficient",0.5);
      }
      if(!this->mb.hasParameter(h,"relaxationTrigger")){
	this->mb.addParameter(h,VariableDescription("ushort","relaxationTrigger",1u,0u));
	this->mb.setParameterDefaultValue(h,"relaxationTrigger",ushort(10u));
      }
      const unsigned short iterMax           = this->mb.getUnsignedShortParameterDefaultValue(h,"iterMax");
      const unsigned short relaxationTrigger = this->mb.getUnsignedShortParameterDefaultValue(h,"relaxationTrigger");
      if(relaxationTrigger+1>=iterMax){
	string msg("MFrontImplicitParserBase::endsInputFileProcessing :");
	msg += "relaxation can never take place (relaxationTrigger>=iterMax-1)'";
	throw(runtime_error(msg));
      }
    }
    if(this->mb.getAttribute(h,MechanicalBehaviourData::useCast3MAccelerationAlgorithm,false)){
      if(!this->mb.hasParameter(h,"accelerationTrigger")){
	this->mb.addParameter(h,VariableDescription("ushort","accelerationTrigger",1u,0u));
	this->mb.setParameterDefaultValue(h,"accelerationTrigger",ushort(10u));
      }
      if(!this->mb.hasParameter(h,"accelerationPeriod")){
	this->mb.addParameter(h,VariableDescription("ushort","accelerationPeriod",1u,0u));
	this->mb.setParameterDefaultValue(h,"accelerationPeriod",ushort(3u));
      }
      const unsigned short iterMax             =
	this->mb.getUnsignedShortParameterDefaultValue(h,"iterMax");
      const unsigned short accelerationTrigger =
	this->mb.getUnsignedShortParameterDefaultValue(h,"accelerationTrigger");
      if(accelerationTrigger+1>=iterMax){
	string msg("MFrontImplicitParserBase::endsInputFileProcessing :");
	msg += "acceleration can never take place (accelerationTrigger>=iterMax-1)'";
	throw(runtime_error(msg));
      }
    }
    if(this->algorithm==MFrontImplicitParserBase::DEFAULT){
      this->algorithm=MFrontImplicitParserBase::NEWTONRAPHSON;
    }
    if(this->mb.getAttribute(h,MechanicalBehaviourData::compareToNumericalJacobian,false)){
      if((this->algorithm!=MFrontImplicitParserBase::NEWTONRAPHSON)&&
	 (this->algorithm!=MFrontImplicitParserBase::POWELLDOGLEG_NEWTONRAPHSON)&&
	 (this->algorithm!=MFrontImplicitParserBase::LEVENBERGMARQUARDT)){
	string msg("MFrontImplicitParserBase::endsInputFileProcessing :");
	msg += "@CompareToNumericalJacobian can only be used with the NewtonRaphson algorithm";
	throw(runtime_error(msg));

      }
    }
    // correct prediction to take into account normalisation factors
    const set<Hypothesis> mh(this->mb.getDistinctModellingHypotheses());
    for(set<Hypothesis>::const_iterator ph=mh.begin();ph!=mh.end();++ph){
      if(this->mb.hasCode(*ph,MechanicalBehaviourData::ComputePredictor)){
	CodeBlock predictor;
	VariableDescriptionContainer::const_iterator p;
	const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(*ph);
	const VariableDescriptionContainer& sv = d.getIntegrationVariables();
	for(p=sv.begin();p!=sv.end();++p){
	  if(this->integrationVariablesIncrementsUsedInPredictor.find('d'+p->name)!=
	     this->integrationVariablesIncrementsUsedInPredictor.end()){
	    if(nf.find(p->name)!=nf.end()){
	      predictor.code += "this->d"+p->name + " /= " + nf.find(p->name)->second + ";\n";
	    }
	  }
	}
	this->mb.setCode(*ph,MechanicalBehaviourData::ComputePredictor,predictor,
			 MechanicalBehaviourData::CREATEORAPPEND,
			 MechanicalBehaviourData::AT_END);
      }
    }
    if(!this->mb.areAllMechanicalDataSpecialised()){
      if(this->mb.hasCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			  MechanicalBehaviourData::ComputePredictor)){
	CodeBlock predictor;
	VariableDescriptionContainer::const_iterator p;
	const MechanicalBehaviourData& d =
	  this->mb.getMechanicalBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	const VariableDescriptionContainer& sv = d.getIntegrationVariables();
	for(p=sv.begin();p!=sv.end();++p){
	  if(this->integrationVariablesIncrementsUsedInPredictor.find('d'+p->name)!=
	     this->integrationVariablesIncrementsUsedInPredictor.end()){
	    if(nf.find(p->name)!=nf.end()){
	      predictor.code += "this->d"+p->name + " /= " + nf.find(p->name)->second + ";\n";
	    }
	  }
	}
	this->mb.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			 MechanicalBehaviourData::ComputePredictor,predictor,
			 MechanicalBehaviourData::CREATEORAPPEND,
			 MechanicalBehaviourData::AT_END);
      }
    }
    // minimal tangent operator
    this->setMinimalTangentOperator();
  } // end of MFrontImplicitParserBase::endsInputFileProcessing(void)

  MFrontImplicitParserBase::~MFrontImplicitParserBase()
  {} // end of ~MFrontImplicitParserBase

} // end of namespace mfront  

