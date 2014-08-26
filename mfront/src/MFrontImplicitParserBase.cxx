/*!
 * \file   MFrontImplicitParserBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2008
 */

#include<cstdlib>
#include<sstream>

#include"TFEL/Utilities/ToString.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontImplicitParserBase.hxx"

namespace mfront{

  MFrontImplicitParserBase::MFrontImplicitParserBase()
    : MFrontBehaviourParserBase<MFrontImplicitParserBase>(),
      algorithm(MFrontImplicitParserBase::DEFAULT),
      hasUserDefinedComputeFinalStress(false),
      compareToNumericalJacobian(false),
      isConsistantTangentOperatorSymmetricDefined(false),
      useRelaxation(false),
      useAcceleration(false)
  {
    using namespace std;
    // dynamically allocated vectors are not yet allowed in implicit
    // parsers
    this->areDynamicallyAllocatedVectorsAllowed_ = false;
    // parameters
    this->registerVariable("theta");
    this->mb.getParameters().push_back(VarHandler("real","theta",1u,0u));
    this->registerVariable("epsilon");
    this->mb.getParameters().push_back(VarHandler("real","epsilon",1u,0u));
    this->registerVariable("numerical_jacobian_epsilon");
    this->registerVariable("iterMax");
    this->mb.getParameters().push_back(VarHandler("ushort","iterMax",1u,0u));
    this->registerVariable("maximum_increment_value_per_iteration");
    this->registerVariable("jacobianComparisonCriterium");
    this->registerVariable("relaxationTrigger");
    this->registerVariable("accelerationTrigger");
    this->registerVariable("accelerationPeriod");
    this->registerVariable("relaxationCoefficient");
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
    this->registerVariable("idx3");
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
    // CallBacks
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &MFrontImplicitParserBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&MFrontImplicitParserBase::treatMaterialLaw);
    this->registerNewCallBack("@ComputeStress",&MFrontImplicitParserBase::treatComputeStress);
    this->registerNewCallBack("@ComputeFinalStress",&MFrontImplicitParserBase::treatComputeFinalStress);
    this->registerNewCallBack("@Predictor",&MFrontImplicitParserBase::treatPredictor);
    this->registerNewCallBack("@Theta",&MFrontImplicitParserBase::treatTheta);
    this->registerNewCallBack("@Epsilon",&MFrontImplicitParserBase::treatEpsilon);
    this->registerNewCallBack("@PertubationValueForNumericalJacobianComputation",
			      &MFrontImplicitParserBase::treatPertubationValueForNumericalJacobianComputation);
    this->registerNewCallBack("@IterMax",&MFrontImplicitParserBase::treatIterMax);
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
    //    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
    this->disableCallBack("@UseQt");
  } // end of MFrontImplicitParserBase::MFrontImplicitParserBase

  void MFrontImplicitParserBase::treatInitJacobian(void)
  {
    using namespace std;
    if(this->algorithm!=BROYDEN){
      this->throwRuntimeError("MFrontImplicitParserBase::treatInitJacobian",
			      "@InitJacobian can only be used with "
			      "the broyden algorithm.");
    }
    if(!this->initJacobian.empty()){
      this->throwRuntimeError("MFrontImplicitParserBase::treatInitJacobian",
			      "@InitJacobian already used.");
    }
    this->initJacobian += this->readNextBlock(makeVariableModifier(*this,&MFrontImplicitParserBase::standardModifier),
					      true);
    this->initJacobian += "\n";
  } // end of MFrontImplicitParserBase::treatInitJacobian

  void MFrontImplicitParserBase::treatTangentOperator(void)
  {
    using namespace std;
    if(!this->tangentOperator.empty()){
      this->throwRuntimeError("MFrontImplicitParserBase::treatTangentOperator",
			      "@TangentOperator already used.");
    }
    this->tangentOperator = this->readNextBlock(makeVariableModifier(*this,&MFrontImplicitParserBase::tangentOperatorVariableModifier),
						true);
    this->tangentOperator += "\n";
    this->hasConsistantTangentOperator = true;
  } // end of MFrontImplicitParserBase::treatTangentOperator

  void MFrontImplicitParserBase::treatIsTangentOperatorSymmetric(void)
  {
    using namespace std;
    if(this->isConsistantTangentOperatorSymmetricDefined){
      this->throwRuntimeError("MFrontImplicitParserBase::treatIsTangentOperatorSymmetric",
			      "@IsTangentOperatorSymmetric already used.");
    }
    this->isConsistantTangentOperatorSymmetricDefined = true;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatIsTangentOperatorSymmetric : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->isConsistantTangentOperatorSymmetric = true;
    } else if(this->current->value=="false"){
      this->isConsistantTangentOperatorSymmetric = false;
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatIsTangentOperatorSymmetric",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatIsTangentOperatorSymmetric",";");
  } // end of MFrontImplicitParserBase::treatTangentOperator
  
  void MFrontImplicitParserBase::treatUnknownVariableMethod(const std::string& n)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    if((this->mb.isInternalStateVariableName(n))||
       ((n[0]=='f')&&(this->mb.isInternalStateVariableName(n.substr(1))))){
      if(this->current->value=="setNormalisationFactor"){
	string var;
	string v;
	if(this->mb.isInternalStateVariableName(n)){
	  v = n;
	} else {
	  v = n.substr(1);
	}
	++(this->current);
	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowVariableMethod");
	this->readSpecifiedToken("MFrontImplicitParserBase::treatUnknowVariableMethod","(");
	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowVariableMethod");
	var = this->current->value;
	if((this->mb.isMaterialPropertyName(var))||
	   (this->mb.isLocalVariableName(var))){
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
	if(this->mb.isInternalStateVariableName(n)){
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
	if(!this->mb.getParametersDefaultValues().insert(MVType(n+"_maximum_increment_value_per_iteration",value)).second){
	  this->throwRuntimeError("MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration",
				  "default value already defined for parameter '"+n+"'_maximum_increment_value_per_iteration'");
	}
	this->mb.getParameters().push_back(VarHandler("real",n+"_maximum_increment_value_per_iteration",1u,0u));
	++(this->current);
	return;
      }
    }
    MFrontBehaviourParserCommon::treatUnknownVariableMethod(n);
  } // end of MFrontImplicitParserBase::treatUnknowVariableMethod
  
  void
  MFrontImplicitParserBase::treatUnknownKeyword(void)
  {
    using namespace std;
    --(this->current);
    const string& n = this->current->value;
    if((n[0]=='f')&&(this->mb.isInternalStateVariableName(n.substr(1)))){
      ++(this->current);
      this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknownKeyword");
      if(this->current->value=="."){
	++(this->current);
	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknownKeyword");
	this->treatUnknownVariableMethod(n);
	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowKeyword");
	this->readSpecifiedToken("MFrontImplicitParserBase::treatUnknowKeyword",")");
	this->checkNotEndOfFile("MFrontImplicitParserBase::treatUnknowKeyword");
	this->readSpecifiedToken("MFrontImplicitParserBase::treatUnknowKeyword",";");
	return;
      } else {
	--(this->current);
      }
    }
    ++(this->current);
    MFrontBehaviourParserCommon::treatUnknownKeyword();
  } // end of MFrontImplicitParserBase::treatUnknownKeyword

  void
  MFrontImplicitParserBase::treatUseAcceleration(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatUseAcceleration : ",
			    "Expected 'true' or 'false'.");
    if(this->useAcceleration){
      this->throwRuntimeError("MFrontImplicitParserBase::treatUseAcceleration",
			      "@UseAcceleration already specified");
    }
    if(this->current->value=="true"){
      this->useAcceleration = true;
      this->mb.getParameters().push_back(VarHandler("ushort","accelerationTrigger",1u,0u));
      this->mb.getParameters().push_back(VarHandler("ushort","accelerationPeriod",1u,0u));
    } else if(this->current->value=="false"){
      this->useAcceleration = false;
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
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short accelerationTrigger;
    accelerationTrigger = this->readUnsignedShort("MFrontImplicitParserBase::treatAccelerationTrigger");
    if(accelerationTrigger<3){
      this->throwRuntimeError("MFrontImplicitParserBase::treatAccelerationTrigger",
			      "invalid acceleration trigger value.");
    }
    if(!this->mb.getUnsignedShortParametersDefaultValues().insert(MVType("accelerationTrigger",accelerationTrigger)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatAccelerationTrigger",
			      "default value already defined for parameter 'accelerationTrigger'");
    }
    this->readSpecifiedToken("MFrontImplicitParserBase::treatAccelerationTrigger",";");
  } // end of MFrontImplicitParserBase::treatAccelerationTrigger

  void
  MFrontImplicitParserBase::treatAccelerationPeriod(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short accelerationPeriod;
    accelerationPeriod = this->readUnsignedShort("MFrontImplicitParserBase::treatAccelerationPeriod");
    if(accelerationPeriod==0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatAccelerationPeriod",
			      "invalid acceleration period value.");
    }
    if(!this->mb.getUnsignedShortParametersDefaultValues().insert(MVType("accelerationPeriod",accelerationPeriod)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatAccelerationPeriod",
			      "default value already defined for parameter 'accelerationPeriod'");
    }
    this->readSpecifiedToken("MFrontImplicitParserBase::treatAccelerationPeriod",";");
  } // end of MFrontImplicitParserBase::treatAccelerationPeriod

  void
  MFrontImplicitParserBase::treatUseRelaxation(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatUseRelaxation : ",
			    "Expected 'true' or 'false'.");
    if(this->useRelaxation){
      this->throwRuntimeError("MFrontImplicitParserBase::treatUseRelaxation",
			      "@UseRelaxation already specified");
    }
    if(this->current->value=="true"){
      this->useRelaxation = true;
      this->mb.getParameters().push_back(VarHandler("real","relaxationCoefficient",1u,0u));
      this->mb.getParameters().push_back(VarHandler("ushort","relaxationTrigger",1u,0u));
    } else if(this->current->value=="false"){
      this->useRelaxation = false;
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
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatCompareToNumericalJacobian : ",
			    "Expected 'true' or 'false'.");
    if(this->compareToNumericalJacobian){
      this->throwRuntimeError("MFrontImplicitParserBase::treatCompareToNumericalJacobian",
			      "@CompareToNumericalJacobian already specified");
    }
    if(this->current->value=="true"){
      this->compareToNumericalJacobian = true;
      this->mb.getParameters().push_back(VarHandler("real","jacobianComparisonCriterium",1u,0u));
    } else if(this->current->value=="false"){
      this->compareToNumericalJacobian = false;
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
    typedef map<string,double>::value_type MVType;
    if(!this->compareToNumericalJacobian){
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
    if(!this->mb.getParametersDefaultValues().insert(MVType("jacobianComparisonCriterium",
						    jacobianComparisonCriterium)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatJacobianComparisonCriterium",
			      "default value already defined for parameter 'jacobianComparisonCriterium'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatJacobianComparisonCriterium",";");
  } // MFrontImplicitParserBase::treatJacobianComparisonCriterium

  void
  MFrontImplicitParserBase::treatRelaxationTrigger(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short relaxationTrigger;
    relaxationTrigger = this->readUnsignedShort("MFrontImplicitParserBase::treatRelaxationTrigger");
    if(!this->mb.getUnsignedShortParametersDefaultValues().insert(MVType("relaxationTrigger",relaxationTrigger)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationTrigger",
			      "default value already defined for parameter 'relaxationTrigger'");
    }
    this->readSpecifiedToken("MFrontImplicitParserBase::treatRelaxationTrigger",";");
  } // end of MFrontImplicitParserBase::treatRelaxationTrigger
  
  void
  MFrontImplicitParserBase::treatRelaxationCoefficient(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatRelaxationCoefficient",
			    "Cannot read relaxation coefficient value.");
    if(!this->useRelaxation){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationCoefficient",
			      "relaxation unused");
    }
    istringstream flux(current->value);
    double relaxationCoefficient;
    flux >> relaxationCoefficient;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationCoefficient",
			      "Failed to read relaxation coefficient value.");
    }
    if(relaxationCoefficient<0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationCoefficient",
			      "relaxation coefficient value must be positive.");
    }
    if(!this->mb.getParametersDefaultValues().insert(MVType("relaxationCoefficient",
						    relaxationCoefficient)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatRelaxationCoefficient",
			      "default value already defined for parameter 'relaxationCoefficient'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatRelaxationCoefficient",";");    
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
    } else if(this->current->value=="NewtonRaphson_NumericalJacobian"){
      this->algorithm = MFrontImplicitParserBase::NEWTONRAPHSON_NR;
    } else if(this->current->value=="Broyden"){
      this->algorithm = MFrontImplicitParserBase::BROYDEN;
    } else if(this->current->value=="Broyden2"){
      this->algorithm = MFrontImplicitParserBase::BROYDEN2;
    } else {
      this->throwRuntimeError("MFrontImplicitParserBase::treatAlgorithm : ",
			      this->current->value+" is not a valid algorithm name\n"
			      "Supported algorithms are : NewtonRaphson, NewtonRaphson_NumericalJacobian, Broyden, and Broyden2.");
    }
    ++this->current;
    this->readSpecifiedToken("MFrontImplicitParserBase::treatAlgorithm",";");
  } // end of MFrontImplicitParserBase::treatAlgorithm

  void
  MFrontImplicitParserBase::treatTheta(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
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
    if(!this->mb.getParametersDefaultValues().insert(MVType("theta",theta)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatTheta",
			      "default value already defined for parameter 'theta'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatTheta",";");
  } // end of MFrontImplicitParserBase::treatTheta

  void
  MFrontImplicitParserBase::treatEpsilon(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
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
    if(!this->mb.getParametersDefaultValues().insert(MVType("epsilon",epsilon)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatEpsilon",
			      "default value already defined for parameter 'epsilon'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatEpsilon",";");
  } // MFrontImplicitParserBase::treatEpsilon

  void
  MFrontImplicitParserBase::treatPertubationValueForNumericalJacobianComputation(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    double epsilon;
    this->checkNotEndOfFile("MFrontImplicitParserBase::treatPertubationValueForNumericalJacobianComputation",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontImplicitParserBase::treatPertubationValueForNumericalJacobianComputation",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatPertubationValueForNumericalJacobianComputation",
			      "Epsilon value must be positive.");
    }
    if(!this->mb.getParametersDefaultValues().insert(MVType("numerical_jacobian_epsilon",epsilon)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatEpsilon",
			      "default value already defined for parameter 'epsilon'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::treatEpsilon",";");
  } // MFrontImplicitParserBase::treatEpsilon

  void
  MFrontImplicitParserBase::treatIterMax(void)
  {
    using namespace std;
    typedef map<string,unsigned short>::value_type MVType;
    unsigned short iterMax;
    iterMax = this->readUnsignedShort("MFrontImplicitParserBase::treatIterMax");
    if(iterMax==0){
      this->throwRuntimeError("MFrontImplicitParserBase::treatIterMax",
			      "invalid value for parameter 'iterMax'");
    }
    if(!this->mb.getUnsignedShortParametersDefaultValues().insert(MVType("iterMax",iterMax)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatIterMax",
			      "default value already defined for parameter 'iterMax'");
    }
    this->readSpecifiedToken("MFrontImplicitParserBase::treatIterMax",";");
  } // end of MFrontImplicitParserBase::treatIterMax

  void
  MFrontImplicitParserBase::treatStateVariables(void)
  {
    using namespace std;
    if((!this->integrator.empty())||
       (!this->computeStress.empty())||
       (!this->predictor.empty())||
       (!this->initJacobian.empty())||
       (!this->tangentOperator.empty())){
      string msg("MFrontImplicitParserBase::treatStateVariables : ");
      msg += "state variables shall be defined before any of the @Integrator "
	"@ComputeStress, @InitJacobian and @TangentOperator blocks";
      throw(runtime_error(msg));
    }
    this->readVarList(this->mb.getStateVariables(),true,true);
  } // end of MFrontImplicitParserBase::treatStateVariables

  void
  MFrontImplicitParserBase::treatAuxiliaryStateVariables(void)
  {
    using namespace std;
    if((!this->integrator.empty())||
       (!this->computeStress.empty())||
       (!this->predictor.empty())||
       (!this->initJacobian.empty())||
       (!this->tangentOperator.empty())){
      string msg("MFrontImplicitParserBase::treatStateVariables : ");
      msg += "state variables shall be defined before any of the @Integrator "
	"@ComputeStress, @InitJacobian and @TangentOperator blocks";
      throw(runtime_error(msg));
    }
    this->readVarList(this->mb.getAuxiliaryStateVariables(),true,true);
  } // end of MFrontImplicitParserBase::treatAuxiliaryStateVariables

  std::string
  MFrontImplicitParserBase::tangentOperatorVariableModifier(const std::string& var,
						   const bool addThisPtr)
  {
    if(this->mb.isInternalStateVariableIncrementName(var)){
      if(nf.find(var.substr(1))!=nf.end()){
	const VarHandler& s = this->mb.getStateVariableHandler(var.substr(1));
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
  MFrontImplicitParserBase::integratorVariableModifier(const std::string& var,
						       const bool addThisPtr)
  {
    if(this->mb.isInternalStateVariableIncrementName(var)){
      if(nf.find(var.substr(1))!=nf.end()){
	const VarHandler& s = this->mb.getStateVariableHandler(var.substr(1));
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
  MFrontImplicitParserBase::computeStressVariableModifier1(const std::string& var,
						       const bool addThisPtr)
  {
    using namespace std;
    if(this->mb.isDrivingVariableName(var)||(var=="T")|
       (this->mb.isExternalStateVariableName(var))){
      if(addThisPtr){
	return "(this->"+var+"+(this->theta)*(this->d"+var+"))";
      } else {
	return "("+var+"+(this->theta)*d"+var+")";
      }
    }
    if(this->mb.isInternalStateVariableName(var)){
      if(this->nf.find(var)!=nf.end()){
	const VarHandler& s = this->mb.getStateVariableHandler(var);
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
    if((this->mb.isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontImplicitParserBase::computeStressVariableModifier1

  std::string
  MFrontImplicitParserBase::computeStressVariableModifier2(const std::string& var,
						       const bool addThisPtr)
  {
    if((this->mb.isDrivingVariableName(var))||(var=="T")||
       (this->mb.isExternalStateVariableName(var))){
      if(addThisPtr){
	return "(this->"+var+"+this->d"+var+")";
      } else {
	return "("+var+"+d"+var+")";
      }
    }
    if((this->mb.isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontImplicitParserBase::computeStressVariableModifier2

  bool
  MFrontImplicitParserBase::isJacobianPart(const std::string& w)
  {
    TokensContainer::const_iterator previous;
    VarContainer::const_iterator p;
    VarContainer::const_iterator p2;
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      for(p2=this->mb.getStateVariables().begin();p2!=this->mb.getStateVariables().end();++p2){
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
	    if(previous->value==this->className){
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
  MFrontImplicitParserBase::integratorAnalyser(const std::string& w)
  {
    if(this->isJacobianPart(w)){
      this->jacobianPartsUsedInIntegrator.insert(w);
    }
  } // end of MFrontImplicitParserBase::integratorAnalyser

  void
  MFrontImplicitParserBase::predictorAnalyser(const std::string& w)
  {
    if(this->mb.isInternalStateVariableIncrementName(w)){
      this->internalStateVariableIncrementsUsedInPredictor.insert(w);
    }
  } // end of MFrontImplicitParserBase::predictorAnalyser

  void
  MFrontImplicitParserBase::treatIntegrator(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(!this->integrator.empty()){
      string msg("MFrontImplicitParserBase::treatIntegrator : ");
      msg += "integrator already defined";
      throw(runtime_error(msg));
    }
    this->integrator = this->readNextBlock(true,"{","}",true,true,
					   makeVariableModifier(*this,&MFrontImplicitParserBase::integratorVariableModifier),
					   makeWordAnalyser(*this,&MFrontImplicitParserBase::integratorAnalyser));
  } // end of MFrontImplicitParserBase::treatIntegrator

  void
  MFrontImplicitParserBase::treatPredictor(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(!this->predictor.empty()){
      string msg("MFrontImplicitParserBase::treatPredictor : ");
      msg += "@Predictor already called";
      throw(runtime_error(msg));
    }
    this->predictor  = this->readNextBlock(true,"{","}",true,true,
					   shared_ptr<VariableModifier>(),
					   makeWordAnalyser(*this,&MFrontImplicitParserBase::predictorAnalyser));
    this->predictor += "\n";
    VarContainer::const_iterator p;
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      if(this->internalStateVariableIncrementsUsedInPredictor.find('d'+p->name)!=
	 this->internalStateVariableIncrementsUsedInPredictor.end()){
	if(nf.find(p->name)!=nf.end()){
	  this->predictor += "this->d"+p->name + " /= " + nf.find(p->name)->second + ";\n";
	}
      }
    }
  } // end of MFrontImplicitParserBase::treatPredictor

  void
  MFrontImplicitParserBase::treatComputeStress(void)
  {
    using namespace std;
    if(!this->computeStress.empty()){
      string msg("MFrontImplicitParserBase::treatComputeStress : ");
      msg += "computeStress already defined";
      throw(runtime_error(msg));
    }
    string fs;
    this->readNextBlock(this->computeStress,fs,
			makeVariableModifier(*this,&MFrontImplicitParserBase::computeStressVariableModifier1),
			makeVariableModifier(*this,&MFrontImplicitParserBase::computeStressVariableModifier2),
			true);
    if(this->computeFinalStress.empty()){
      this->computeFinalStress = fs;
    }
  } // end of MFrontImplicitParserBase::treatComputeStress

  void
  MFrontImplicitParserBase::treatComputeFinalStress(void)
  {
    using namespace std;
    if(this->hasUserDefinedComputeFinalStress){
      string msg("MFrontImplicitParserBase::treatComputeFinalStress : ");
      msg += "final stress computation already defined";
      throw(runtime_error(msg));
    }
    this->computeFinalStress = this->readNextBlock(makeVariableModifier(*this,&MFrontImplicitParserBase::computeStressVariableModifier2),true);
    this->hasUserDefinedComputeFinalStress = true;
  } // end of MFrontImplicitParserBase::treatComputeFinalStress

  void
  MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration(void)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
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
    if(!this->mb.getParametersDefaultValues().insert(MVType("maximum_increment_value_per_iteration",value)).second){
      this->throwRuntimeError("MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration",
			      "default value already defined for parameter 'maximum_increment_value_per_iteration'");
    }
    this->mb.getParameters().push_back(VarHandler("real","maximum_increment_value_per_iteration",1u,0u));
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParserBase::MaximumIncrementValuePerIteration",";");
  } // end of MFrontImplicitParserBase::treatMaximumIncrementValuePerIteration

  void MFrontImplicitParserBase::writeBehaviourParserSpecificIncludes(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    bool has_scalar  = false;
    bool has_scalar_array  = false;
    bool has_tvector = false;
    bool has_tvector_array = false;
    bool has_stensor = false;
    bool has_stensor_array = false;
    // checks
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
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
    // output
    this->checkBehaviourFile();
    this->behaviourFile << "#include<limits>\n";
    this->behaviourFile << "#include<algorithm>\n\n";
    this->behaviourFile << "#include\"TFEL/Math/st2tost2.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/tmatrix.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/tvector.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/TinyMatrixSolve.hxx\"\n";
    if(has_scalar_array){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TinyVectorFromTinyVectorView.hxx\"\n";
    }
    // tiny vectors
    if(has_tvector){
      this->behaviourFile << "#include\"TFEL/Math/Matrix/tmatrix_submatrix_view.hxx\"\n\n";
      this->behaviourFile << "#include\"TFEL/Math/Matrix/tmatrix_submatrix_view.hxx\"\n\n";
    }
    if((has_scalar)&&(has_tvector)){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixColumnView.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixRowView.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixColumnView2.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixRowView2.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/Vector/TVectorFromTinyVectorView.hxx\"\n";
    }
    if(has_tvector_array){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TinyVectorOfTinyVectorFromTinyVectorView.hxx\"\n";
    }
    // symmetric tensors
    if(has_stensor){
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyVectorView.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/ST2toST2/ST2toST2FromTinyMatrixView.hxx\"\n\n";
      this->behaviourFile << "#include\"TFEL/Math/ST2toST2/ST2toST2FromTinyMatrixView2.hxx\"\n\n";
    }
    if((has_scalar)&&(has_stensor)){
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixColumnView.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixRowView.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixColumnView2.hxx\"\n";
      this->behaviourFile << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixRowView2.hxx\"\n";
    }
    if(has_stensor_array){
      this->behaviourFile << "#include\"TFEL/Math/Vector/TinyVectorOfStensorFromTinyVectorView.hxx\"\n";
    }
  } // end of MFrontImplicitParserBase::writeBehaviourParserSpecificIncludes(void)

  void MFrontImplicitParserBase::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    MFrontBehaviourParserCommon::writeBehaviourParserSpecificTypedefs();
  } // end of MFrontImplicitParserBase::writeBehaviourParserSpecificTypedefs(void)

  void
  MFrontImplicitParserBase::writeBehaviourParserSpecificMembers(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    VarContainer::const_iterator p2;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n3;
    // size of linear system
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      n3 += this->getTypeSize(p->type,p->arraySize);
    }
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      SupportedTypes::TypeSize n2;
      for(p2=this->mb.getStateVariables().begin();p2!=this->mb.getStateVariables().end();++p2){
	SupportedTypes::TypeFlag flag  = this->getTypeFlag(p->type);
	SupportedTypes::TypeFlag flag2 = this->getTypeFlag(p2->type);
	if((p->arraySize!=1u)||(p2->arraySize!=1u)){
	  this->behaviourFile << "/*!\n"
			      << " * \\return the part of the jacobian matrix "
			      << "corresponding to the derivative "
			      << "of variable " << p->name 
			      << " by variable " << p2->name << "\n"
			      << " */\n";
	}
	if((p->arraySize!=1u)&&(p2->arraySize==1u)){
	  if(flag==SupportedTypes::Scalar){
	    if(flag2==SupportedTypes::Scalar){
	      this->behaviourFile << "real&\n"
				  << "df" << p->name << "_dd" << p2->name
				  << "(tfel::math::tmatrix<" << n3 << "," << n3 << ">& tjacobian,\n"
				  << "const unsigned short idx){\n"
				  << "return tjacobian(" << n << "+idx, " << n2 << ");\n"
				  << "}\n\n";
	      this->behaviourFile << "real&\n"
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
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NR)){
      this->writeComputeNumericalJacobian();
    }
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NR)||
       (this->algorithm==BROYDEN)){
      this->writeGetPartialJacobianInvert();
    }
    // compute stress
    if(!this->computeStress.empty()){
      this->behaviourFile << "void\ncomputeStress(void){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers",
			this->behaviourFile,this->materialLaws);
      this->behaviourFile << this->computeStress << endl;
      this->behaviourFile << "} // end of " << this->className << "::computeStress\n\n";
    }
    if(!this->computeFinalStress.empty()){
      this->behaviourFile << "void\ncomputeFinalStress(void){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers",
			this->behaviourFile,this->materialLaws);
      this->behaviourFile << this->computeFinalStress << endl;
      this->behaviourFile << "} // end of " << this->className << "::computeStress\n\n";
      this->behaviourFile << endl;
    }
  } // end of MFrontImplicitParserBase::writeBehaviourParserSpecificMembers

  std::string
  MFrontImplicitParserBase::getJacobianPart(const VarHandler&  v1,
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
	msg += "unsupported type for state variable ";
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
	msg += "unsupported type for state variable ";
	msg += v2.name;
	throw(runtime_error(msg));
      }
    } else {
      string msg("MFrontImplicitParserBase::getJacobianPart : ");
      msg += "unsupported type for state variable ";
      msg += v1.name;
      throw(runtime_error(msg));
    }
    return d.str();
  } // void MFrontImplicitParserBase::getJacobianPart

  void
  MFrontImplicitParserBase::writeGetPartialJacobianInvert(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    SupportedTypes::TypeSize n;
    VarContainer::size_type i;
    VarContainer::const_iterator p;
    VarContainer::const_iterator p2;
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    p=this->mb.getStateVariables().begin();
    ++p;
    for(i=0;i!=this->mb.getStateVariables().size();++p,++i){
      this->behaviourFile << "void\ngetPartialJacobianInvert(";
      for(p2=this->mb.getStateVariables().begin();p2!=p;){
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
      if(this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NR){
	this->behaviourFile << "this->computeNumericalJacobian(this->jacobian);\n";
      }
      this->behaviourFile << "TinyMatrixSolve<" << n << ",real>::decomp(this->jacobian,permuation);" << endl;
      this->behaviourFile << "for(unsigned short idx=0;idx!=StensorSize;++idx){\n";
      this->behaviourFile << "tvector<" << n << ",real> vect_e(real(0));" << endl;
      this->behaviourFile << "vect_e(idx) = real(1);" << endl;
      this->behaviourFile << "TinyMatrixSolve<" << n << ",real>::back_substitute(this->jacobian,permuation,vect_e);" << endl;
      SupportedTypes::TypeSize n2;
      for(p2=this->mb.getStateVariables().begin();p2!=p;++p2){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p2->type);
	if(flag==SupportedTypes::Scalar){
	  if(p2->arraySize==1u){
	    this->behaviourFile << "partial_jacobian_" << p2->name << "(idx)=vect_e(" << n2 << ");\n";
	  } else {
	    this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				<< p->arraySize << ";++idx2){" << endl;
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
				<< p->arraySize << ";++idx2){" << endl;
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
				<< p->arraySize << ";++idx2){" << endl;
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
				<< p->arraySize << ";++idx2){" << endl;
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
      for(p2=this->mb.getStateVariables().begin();p2!=p;++p2){
	if(nf.find(p2->name)!=nf.end()){
	  this->behaviourFile << "partial_jacobian_" << p2->name << " /= " << nf.find(p2->name)->second << ";\n";
	}
      }
      this->behaviourFile << "}\n\n";
    }
  } // end of MFrontImplicitParserBase::writeGetPartialJacobianInvert
  
  void
  MFrontImplicitParserBase::writeLimitsOnIncrementValues(const std::string& v)
  {
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      SupportedTypes::TypeSize nv = this->getTypeSize(p->type,p->arraySize);
      if(this->mb.getParametersDefaultValues().find(p->name+"_maximum_increment_value_per_iteration")!=
	 this->mb.getParametersDefaultValues().end()){
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
      } else if(this->mb.getParametersDefaultValues().find("maximum_increment_value_per_iteration")!=
		this->mb.getParametersDefaultValues().end()){
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

  void MFrontImplicitParserBase::writeComputeNumericalJacobian(){
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
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
    if(!this->computeStress.empty()){
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
    if(!this->computeStress.empty()){
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
  MFrontImplicitParserBase::getVectorMappingClass(const VarHandler& v) const
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

  void MFrontImplicitParserBase::writeBehaviourIntegrator(){
    using namespace std;
    VarContainer::const_iterator p;
    VarContainer::const_iterator p2;
    vector<BoundsDescription>::const_iterator p3;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult" << endl;
    this->behaviourFile << "integrate(const SMType smt){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    if(this->compareToNumericalJacobian){
      this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> njacobian;\n";
    }
    if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
      this->behaviourFile << "tmatrix<" << n2 << "," << n2 << ",real> jacobian2;\n";
      this->behaviourFile << "tvector<" << n2 << ",real> fzeros2;\n";
      this->behaviourFile << "tvector<" << n2 << ",real> Dzeros;\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::BROYDEN2){
      this->behaviourFile << "tvector<" << n2 << ",real> Dfzeros;\n";
    }
    if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
      this->behaviourFile << "real broyden_inv;\n";
    }
    this->behaviourFile << "real error;\n";
    this->behaviourFile << "bool converge=false;\n";
    if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
      if(!this->computeStress.empty()){
	this->behaviourFile << "this->computeStress();\n";
      }
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
    if(this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON){
      if(!this->computeStress.empty()){
	this->behaviourFile << "this->computeStress();\n";
      }
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NR){
      if(!this->computeStress.empty()){
	this->behaviourFile << "this->computeStress();\n";
      }
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::BROYDEN){
      this->behaviourFile << "Dzeros = -this->fzeros;\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "try{" << endl;
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(jacobian2,Dzeros);\n";
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "catch(LUException&){" << endl;
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->writeLimitsOnIncrementValues("Dzeros");
      this->behaviourFile << "this->zeros  += Dzeros;\n";
      this->behaviourFile << "fzeros2 = this->fzeros;\n";
      if(!this->computeStress.empty()){
	this->behaviourFile << "this->computeStress();\n";
      }
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::BROYDEN2){
      this->behaviourFile << "Dzeros   = -(this->jacobian)*(this->fzeros);\n";
      this->writeLimitsOnIncrementValues("Dzeros");
      this->behaviourFile << "this->zeros  += Dzeros;\n";
      this->behaviourFile << "fzeros2 = this->fzeros;\n";
      if(!this->computeStress.empty()){
	this->behaviourFile << "this->computeStress();\n";
      }
      this->behaviourFile << "this->computeFdF();\n";
    }
    if(this->compareToNumericalJacobian){
      this->behaviourFile << "this->computeNumericalJacobian(njacobian);\n";
      n = SupportedTypes::TypeSize();
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
	if(p->arraySize==1){
	  n3 = SupportedTypes::TypeSize();
	  for(p2=this->mb.getStateVariables().begin();p2!=this->mb.getStateVariables().end();++p2){
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
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
	for(p2=this->mb.getStateVariables().begin();p2!=this->mb.getStateVariables().end();++p2){
	  const VarHandler& v1 = *p;
	  const VarHandler& v2 = *p2;
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
    this->behaviourFile << "error=norm(this->fzeros);\n";
    this->behaviourFile << "converge = ((error)/(real(" << n2 << "))<";
    this->behaviourFile << "(this->epsilon));\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::integrate() : iteration \" "
			  << "<< this->iter << \" : \" << (error)/(real(" << n2 << ")) << endl;\n";
    }
    this->behaviourFile << "if(!converge){\n";
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NR)){
      this->behaviourFile << "try{" << endl;
      if(this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NR){
	this->behaviourFile << "this->computeNumericalJacobian(this->jacobian);\n";
      }
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(this->jacobian,this->fzeros);\n";
      this->behaviourFile << "}" << endl;
      this->behaviourFile << "catch(LUException&){" << endl;
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}" << endl;
      this->writeLimitsOnIncrementValues("fzeros");
      this->behaviourFile << "this->zeros -= this->fzeros;\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::BROYDEN){
      this->behaviourFile << "broyden_inv = (Dzeros|Dzeros);\n";
      this->behaviourFile << "if(broyden_inv<100*std::numeric_limits<real>::epsilon()){\n";
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}\n";
      this->behaviourFile << "this->jacobian += "
			  << "(((this->fzeros-fzeros2)-(jacobian2)*(Dzeros))^Dzeros)/broyden_inv;\n";
    }
    if(this->algorithm==MFrontImplicitParserBase::BROYDEN2){
      this->behaviourFile << "Dfzeros   = (this->fzeros)-fzeros2;\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "broyden_inv = Dzeros|jacobian2*Dfzeros;\n";
      this->behaviourFile << "if(broyden_inv<100*std::numeric_limits<real>::epsilon()){\n";
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
      }
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
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::integrate() : convergence after \" "
			  << "<< this->iter << \" iterations\"<< endl << endl;\n";
    }
    this->behaviourFile << "if(smt!=NOSTIFFNESSREQUESTED){\n";
    this->behaviourFile << "if(!this->computeConsistantTangentOperator(smt)){\n";
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      if(this->nf.find(p->name)!=this->nf.end()){
	this->behaviourFile << "this->d" << p->name << " *= " << this->nf.find(p->name)->second << ";\n";
      }
    }
    this->behaviourFile << "this->updateStateVars();\n";
    if(!this->computeFinalStress.empty()){
      this->behaviourFile << "this->computeFinalStress();\n";
    }
    this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    for(p3  = this->mb.getBounds().begin();
	p3 != this->mb.getBounds().end();++p3){
      if(p3->varCategory==BoundsDescription::StateVar){
	p3->writeBoundsChecks(this->behaviourFile);
      }
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "} // end of " << this->className << "::integrate\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief compute fzeros and jacobian\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "computeFdF(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws("MFrontImplicitParserBase::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);
    n = SupportedTypes::TypeSize();
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
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
       (this->algorithm==MFrontImplicitParserBase::BROYDEN)){
      n = SupportedTypes::TypeSize();
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
	n3 = SupportedTypes::TypeSize();
	for(p2=this->mb.getStateVariables().begin();p2!=this->mb.getStateVariables().end();++p2){
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
    if(this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON){
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
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
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      if(this->nf.find(p->name)!=this->nf.end()){
    	this->behaviourFile << "f" << p->name << " *= " << this->nf.find(p->name)->second << ";" << endl;
      }
    }
    this->behaviourFile << this->integrator;
    this->behaviourFile << "\n";
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      if(this->nf.find('f'+p->name)!=this->nf.end()){
    	this->behaviourFile << "f" << p->name << "*= real(1)/(" << this->nf.find('f'+p->name)->second << ");" << endl;
      }
    }
    if((this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON)||
       (this->algorithm==MFrontImplicitParserBase::BROYDEN)){
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
	for(p2=this->mb.getStateVariables().begin();p2!=this->mb.getStateVariables().end();++p2){
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
    this->behaviourFile << "}\n\n";
  } // end of MFrontImplicitParserBase::writeBehaviourIntegrator

  void MFrontImplicitParserBase::writeBehaviourComputeTangentOperator(void)
  {
    if(this->hasConsistantTangentOperator){
      this->behaviourFile << "bool computeConsistantTangentOperator(const SMType smt){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("MFrontImplicitParserBase::writeBehaviourIntegrator",
			this->behaviourFile,this->materialLaws);
      this->behaviourFile << this->tangentOperator;
      this->behaviourFile << "return true;\n";
      this->behaviourFile << "}\n\n";
    } else {
      MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator();
    }
  }

  void MFrontImplicitParserBase::writeBehaviourConstructors(void)
  {    
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    ostringstream init;
    if(!this->localVariablesInitializers.empty()){
      init << ",\n" << this->localVariablesInitializers;
    }
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      SupportedTypes::TypeFlag flag = getTypeFlag(p->type);
      init << ",\n";
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
    init << ",\nzeros(real(0)),\nfzeros(real(0))";
    MFrontBehaviourParserCommon::writeBehaviourConstructors(init.str(),
							    this->predictor);
  }

  void MFrontImplicitParserBase::writeBehaviourParserSpecificConstructorPart(void)
  {
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    this->checkBehaviourFile();
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    if(!this->initJacobian.empty()){
      this->behaviourFile << this->initJacobian;
    } else {
      if((this->algorithm==MFrontImplicitParserBase::BROYDEN)||
	 (this->algorithm==MFrontImplicitParserBase::BROYDEN2)){
	this->behaviourFile << "// setting jacobian to identity\n";
	this->behaviourFile << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n";
	this->behaviourFile << "for(unsigned short idx = 0; idx!= "<< n << ";++idx){\n";
	this->behaviourFile << "this->jacobian(idx,idx)= real(1);\n";
	this->behaviourFile << "}\n";
      }
    }
  }

  void MFrontImplicitParserBase::writeBehaviourStateVarsIncrements(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      n2 += this->getTypeSize(p->type,p->arraySize);
    }
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      if((!this->debugMode)&&(p->lineNumber!=0u)){
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
    MFrontBehaviourParserCommon::endsInputFileProcessing();
    if(this->integrator.empty()){
      string msg("MFrontImplicitParserBase::endsInputFileProcessing : ");
      msg += "definining the @Integrator block is required";
      throw(runtime_error(msg));
    }
    // if(this->computeStress.empty()){
    //   string msg("MFrontImplicitParserBase::endsInputFileProcessing : ");
    //   msg += "definining the @ComputeStress block is required";
    //   throw(runtime_error(msg));
    // }
    typedef map<string,double>::value_type MVType;
    typedef map<string,unsigned short>::value_type MVType2;
    if(this->mb.getParametersDefaultValues().find("theta")==this->mb.getParametersDefaultValues().end()){
      this->mb.getParametersDefaultValues().insert(MVType("theta",0.5));
    }
    if(this->mb.getParametersDefaultValues().find("epsilon")==
       this->mb.getParametersDefaultValues().end()){
      this->mb.getParametersDefaultValues().insert(MVType("epsilon",1.e-8));
    }
    if((this->compareToNumericalJacobian)||(this->algorithm==BROYDEN)||
       (this->algorithm==MFrontImplicitParserBase::NEWTONRAPHSON_NR)){
      const string nje = "numerical_jacobian_epsilon";
      this->mb.getParameters().push_back(VarHandler("real",nje,1u,0u));
      if(this->mb.getParametersDefaultValues().find(nje)==
	 this->mb.getParametersDefaultValues().end()){
	const double eps = 0.1*this->mb.getParametersDefaultValues()["epsilon"];
	this->mb.getParametersDefaultValues().insert(MVType(nje,eps));
      }
    }
    if(this->mb.getUnsignedShortParametersDefaultValues().find("iterMax")==this->mb.getUnsignedShortParametersDefaultValues().end()){
      this->mb.getUnsignedShortParametersDefaultValues().insert(MVType2("iterMax",100u));
    }
    unsigned short iterMax = this->mb.getUnsignedShortParametersDefaultValues()["iterMax"];
    if(this->compareToNumericalJacobian){
      if(this->mb.getParametersDefaultValues().find("jacobianComparisonCriterium")==this->mb.getParametersDefaultValues().end()){
	double epsilon = this->mb.getParametersDefaultValues()["epsilon"];
	this->mb.getParametersDefaultValues().insert(MVType("jacobianComparisonCriterium",epsilon));
      }
    }
    if(this->useRelaxation){
      if(this->mb.getParametersDefaultValues().find("relaxationCoefficient")==this->mb.getParametersDefaultValues().end()){
	this->mb.getParametersDefaultValues().insert(MVType("relaxationCoefficient",0.5));
      }
      if(this->mb.getUnsignedShortParametersDefaultValues().find("relaxationTrigger")==this->mb.getUnsignedShortParametersDefaultValues().end()){
	this->mb.getUnsignedShortParametersDefaultValues().insert(MVType2("relaxationTrigger",10u));
      }
      unsigned short relaxationTrigger = this->mb.getUnsignedShortParametersDefaultValues()["relaxationTrigger"];
      if(relaxationTrigger+1>=iterMax){
	string msg("MFrontImplicitParserBase::endsInputFileProcessing :");
	msg += "relaxation can never take place (relaxationTrigger>=iterMax-1)'";
	throw(runtime_error(msg));
      }
    }
    if(this->useAcceleration){
      if(this->mb.getUnsignedShortParametersDefaultValues().find("accelerationTrigger")==this->mb.getUnsignedShortParametersDefaultValues().end()){
	this->mb.getUnsignedShortParametersDefaultValues().insert(MVType2("accelerationTrigger",10u));
      }
      if(this->mb.getUnsignedShortParametersDefaultValues().find("accelerationPeriod")==this->mb.getUnsignedShortParametersDefaultValues().end()){
	this->mb.getUnsignedShortParametersDefaultValues().insert(MVType2("accelerationPeriod",3u));
      }
      unsigned short accelerationTrigger = this->mb.getUnsignedShortParametersDefaultValues()["accelerationTrigger"];
      if(accelerationTrigger+1>=iterMax){
	string msg("MFrontImplicitParserBase::endsInputFileProcessing :");
	msg += "acceleration can never take place (accelerationTrigger>=iterMax-1)'";
	throw(runtime_error(msg));
      }
    }
    if(this->algorithm==MFrontImplicitParserBase::DEFAULT){
      this->algorithm=MFrontImplicitParserBase::NEWTONRAPHSON;
    }
    if(this->compareToNumericalJacobian){
      if(this->algorithm!=MFrontImplicitParserBase::NEWTONRAPHSON){
	string msg("MFrontImplicitParserBase::endsInputFileProcessing :");
	msg += "@CompareToNumericalJacobian can only be used with the NewtonRaphson algorithm";
	throw(runtime_error(msg));

      }
    }
    // minimal tangent operator
    if(!this->hasConsistantTangentOperator){
      if(this->mb.requiresStiffnessOperator()){
	this->hasConsistantTangentOperator = true;
	this->tangentOperator = "if(smt==ELASTIC){\n"
	                        "this->Dt = this->D;"
 	                        "} else {\n"
	                        "return false;\n"
	                        "}\n";
      }
    }
  } // end of MFrontImplicitParserBase::endsInputFileProcessing(void)

  void 
  MFrontImplicitParserBase::writeBehaviourStaticVars(void)
  {
    MFrontBehaviourParserBase<MFrontImplicitParserBase>::writeBehaviourStaticVars();
  } // end of MFrontImplicitParserBase::writeBehaviourStaticVars

  MFrontImplicitParserBase::~MFrontImplicitParserBase()
  {} // end of ~MFrontImplicitParserBase

} // end of namespace mfront  

