/*!
 * \file   mfront/src/ImplicitDSLBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<iomanip>

#include<cmath>
#include<limits>
#include<cstdlib>
#include<sstream>

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/NonLinearSystemSolver.hxx"
#include"MFront/NonLinearSystemSolverBase.hxx"
#include"MFront/NonLinearSystemSolverFactory.hxx"
#include"MFront/PerformanceProfiling.hxx"
#include"MFront/ImplicitDSLBase.hxx"

#warning"HERE"
  // if(this->varNames.find(p+"df"+v1.name+"_dd"+v2.name)!=this->varNames.end()){
  //   string msg("NonLinearSystemSolverBase::getJacobianPart : ");
  //   msg += "variable name 'df"+v1.name+"_dd"+v2.name;
  //   msg += "' is reserved.\n";
  //   throw(runtime_error(msg));
  // }
  
namespace mfront{

  ImplicitDSLBase::ImplicitDSLBase()
    : BehaviourDSLBase<ImplicitDSLBase>()
  {
    using namespace std;
    // dynamically allocated vectors are not yet allowed in implicit
    // parsers
    this->areDynamicallyAllocatedVectorsAllowed_ = false;
    // parameters
    this->registerVariable("theta",false);
    this->registerVariable("numerical_jacobian_epsilon",false);
    this->registerVariable("maximum_increment_value_per_iteration",false);
    this->registerVariable("jacobianComparisonCriterium",false);
    this->registerVariable("epsilon",false);
    this->registerVariable("vect_e",false);
    this->registerVariable("zeros",false);
    this->registerVariable("tzeros",false);
    this->registerVariable("zeros_1",false);
    this->registerVariable("fzeros",false);
    this->registerVariable("jacobian",false);
    this->registerVariable("njacobian",false);
    this->registerVariable("partial_jacobian",false);
    this->registerVariable("t",false);
    this->registerVariable("error_1",false);
    this->registerVariable("error_p",false);
    this->registerVariable("idx",false);
    this->registerVariable("idx2",false);
    this->registerVariable("idx3",false);
    this->reserveName("computeNumericalJacobian",false);
    this->reserveName("TinyMatrixSolve",false);
    // CallBacks
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &ImplicitDSLBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&ImplicitDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@ComputeStress",&ImplicitDSLBase::treatComputeStress);
    this->registerNewCallBack("@ComputeFinalStress",&ImplicitDSLBase::treatComputeFinalStress);
    this->registerNewCallBack("@Predictor",&ImplicitDSLBase::treatPredictor);
    this->registerNewCallBack("@Theta",&ImplicitDSLBase::treatTheta);
    this->registerNewCallBack("@Epsilon",&ImplicitDSLBase::treatEpsilon);
    this->registerNewCallBack("@PerturbationValueForNumericalJacobianComputation",
			      &ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation);
    this->registerNewCallBack("@IterMax",&ImplicitDSLBase::treatIterMax);
    this->registerNewCallBack("@MaximumNumberOfIterations",&ImplicitDSLBase::treatIterMax);
    this->registerNewCallBack("@Algorithm",&ImplicitDSLBase::treatAlgorithm);
    this->registerNewCallBack("@TangentOperator",
			      &ImplicitDSLBase::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
			      &ImplicitDSLBase::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@InitJacobian",
			      &ImplicitDSLBase::treatInitJacobian);
    this->registerNewCallBack("@InitializeJacobian",
			      &ImplicitDSLBase::treatInitJacobian);
    this->registerNewCallBack("@InitJacobianInvert",
			      &ImplicitDSLBase::treatInitJacobianInvert);
    this->registerNewCallBack("@InitializeJacobianInvert",
			      &ImplicitDSLBase::treatInitJacobianInvert);
    this->registerNewCallBack("@CompareToNumericalJacobian",
			      &ImplicitDSLBase::treatCompareToNumericalJacobian);
    this->registerNewCallBack("@JacobianComparisonCriterium",
			      &ImplicitDSLBase::treatJacobianComparisonCriterium);
    this->registerNewCallBack("@RequireStiffnessTensor",
			      &ImplicitDSLBase::treatRequireStiffnessOperator);
    this->registerNewCallBack("@MaximumIncrementValuePerIteration",
			      &ImplicitDSLBase::treatMaximumIncrementValuePerIteration);
    this->registerNewCallBack("@IntegrationVariable",
    			      &ImplicitDSLBase::treatIntegrationVariable);
    this->disableCallBack("@ComputedVar");
    this->disableCallBack("@UseQt");
    this->mb.setIntegrationScheme(BehaviourDescription::IMPLICITSCHEME);
  } // end of ImplicitDSLBase::ImplicitDSLBase

  void
  ImplicitDSLBase::treatUnknownKeyword()
  {
    --(this->current);
    const auto& key = this->current->value;
    ++(this->current);
    if(this->solver.get()!=nullptr){
      const auto r =  this->solver->treatSpecificKeywords(this->mb,key,this->current,
							  this->fileTokens.end());
      if(r.first){
	this->current = r.second;
	return;
      }
    }
    BehaviourDSLCommon::treatUnknownKeyword();
  } // end of ImplicitDSLBase::treatUnknownKeyword

  void ImplicitDSLBase::treatStateVariable(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&BehaviourDescription::addStateVariables,true,true,false);
  }

  void ImplicitDSLBase::treatIntegrationVariable(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&BehaviourDescription::addIntegrationVariables,true,true,false);
  }

  void ImplicitDSLBase::treatInitJacobian(void)
  {
    using namespace std;
    if(this->solver.get()==nullptr){
      this->throwRuntimeError("ImplicitDSLBase::treatInitJacobian",
			      "undefined algorithm. Please use the '@Algorithm' "
			      "keyword to define one.");
    }
    if(!this->solver->allowsJacobianInitialisation()){
      this->throwRuntimeError("ImplicitDSLBase::treatInitJacobian",
			      "@InitJacobian can not be used with "
			      "the current algorithm.");
    }
    this->readCodeBlock(*this,BehaviourData::InitializeJacobian,
			&ImplicitDSLBase::standardModifier,true,true);
  } // end of ImplicitDSLBase::treatInitJacobian

  void ImplicitDSLBase::treatInitJacobianInvert(void)
  {
    using namespace std;
    if(this->solver.get()==nullptr){
      this->throwRuntimeError("ImplicitDSLBase::treatInitJacobianInvert",
			      "undefined algorithm. Please use the '@Algorithm' "
			      "keyword to define one.");
    }
    if(!this->solver->allowsJacobianInvertInitialisation()){
      this->throwRuntimeError("ImplicitDSLBase::treatInitJacobianInvert",
			      "@InitJacobianInvert can not be used with "
			      "the current algorithm.");
    }
    this->readCodeBlock(*this,BehaviourData::InitializeJacobianInvert,
			&ImplicitDSLBase::standardModifier,true,true);
  } // end of ImplicitDSLBase::treatInitJacobianInvert
  
  void ImplicitDSLBase::treatUnknownVariableMethod(const Hypothesis h,
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
    	this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
    	this->readSpecifiedToken("ImplicitDSLBase::treatUnknowVariableMethod","(");
    	this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
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
    	    this->throwRuntimeError("ImplicitDSLBase::treatUnknowVariableMethod",
    				    "Failed to read normalisation factor.");
    	  }
    	  if(value<=0.){
    	    this->throwRuntimeError("ImplicitDSLBase::treatUnknowVariableMethod",
    				    "invalid normalisation factor.");
    	  }
    	}
	this->mb.setAttribute(h,n+"_normalisation_factor",var);
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
    	this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
    	this->readSpecifiedToken("ImplicitDSLBase::treatUnknowVariableMethod","(");
    	this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
    	const string var =  this->current->value;
    	double value;
    	istringstream flux(var);
    	flux >> value;
    	if(flux.fail()){
    	  this->throwRuntimeError("ImplicitDSLBase::treatUnknowVariableMethod",
    				  "Failed to read maximum increment value per iteration from '"+var+"'.");
    	}
    	if(value<=0.){
    	  this->throwRuntimeError("ImplicitDSLBase::treatUnknowVariableMethod",
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
    BehaviourDSLCommon::treatUnknownVariableMethod(h,n);
  } // end of ImplicitDSLBase::treatUnknowVariableMethod
  
  bool
  ImplicitDSLBase::isCallableVariable(const Hypothesis h,
					       const std::string& n) const

  {
     using namespace std;
     if(n.empty()){
       string msg("ImplicitDSLBase::isCallableVariable : "
		  "empty variable name '"+n+"'");
       throw(runtime_error(msg));
     }
     if((n[0]=='f')&&(this->mb.isIntegrationVariableName(h,n.substr(1)))){
       return true;
     }
     return BehaviourDSLCommon::isCallableVariable(h,n);
  } // end of ImplicitDSLBase::isCallableVariable

  void
  ImplicitDSLBase::treatCompareToNumericalJacobian(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("ImplicitDSLBase::treatCompareToNumericalJacobian : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->mb.setAttribute(h,BehaviourData::compareToNumericalJacobian,true);
    } else if(this->current->value=="false"){
      this->mb.setAttribute(h,BehaviourData::compareToNumericalJacobian,false);
    } else {
      this->throwRuntimeError("ImplicitDSLBase::treatCompareToNumericalJacobian",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatCompareToNumericalJacobian",";");
  } // end of ImplicitDSLBase::treatCompareToNumericalJacobian
  
  void
  ImplicitDSLBase::treatJacobianComparisonCriterium(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!this->mb.getAttribute(h,BehaviourData::compareToNumericalJacobian,false)){
      this->throwRuntimeError("ImplicitDSLBase::treatJacobianComparisonCriterium",
			      "must call '@CompareToNumericalJacobian' first");
    }
    double jacobianComparisonCriterium;
    this->checkNotEndOfFile("ImplicitDSLBase::treatJacobianComparisonCriterium",
			    "Cannot read jacobianComparisonCriterium value.");
    istringstream flux(current->value);
    flux >> jacobianComparisonCriterium;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ImplicitDSLBase::treatJacobianComparisonCriterium",
			      "Failed to read jacobianComparisonCriterium value.");
    }
    if(jacobianComparisonCriterium<0){
      this->throwRuntimeError("ImplicitDSLBase::treatJacobianComparisonCriterium",
			      "JacobianComparisonCriterium value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatJacobianComparisonCriterium",";");
    this->mb.addParameter(h,VariableDescription("real","jacobianComparisonCriterium",1u,0u));
    this->mb.setParameterDefaultValue(h,"jacobianComparisonCriterium",
				      jacobianComparisonCriterium);
  } // ImplicitDSLBase::treatJacobianComparisonCriterium

  void ImplicitDSLBase::treatAlgorithm(void)
  {
    using namespace std;
    const NonLinearSystemSolverFactory& f =
      NonLinearSystemSolverFactory::getNonLinearSystemSolverFactory();
    if(this->solver.get()!=nullptr){
      this->throwRuntimeError("ImplicitDSLBase::treatAlgorithm",
			      "an algorithm has already been defined.");
    }
    this->checkNotEndOfFile("ImplicitDSLBase::treatAlgorithm",
			    "Cannot read algorithm name.");
    const auto& s = this->current->value;
    ++this->current;
    this->readSpecifiedToken("ImplicitDSLBase::treatAlgorithm",";");
    this->solver = f.getSolver(s);
    this->mb.setAttribute(BehaviourData::algorithm,s,false);
  } // end of ImplicitDSLBase::treatAlgorithm

  void
  ImplicitDSLBase::treatTheta(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double theta;
    this->checkNotEndOfFile("ImplicitDSLBase::treatTheta",
			    "Cannot read theta value.");
    istringstream flux(current->value);
    flux >> theta;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ImplicitDSLBase::treatTheta",
			      "Failed to read theta value.");
    }
    if((theta<0.)||(theta>1.)){
      this->throwRuntimeError("ImplicitDSLBase::treatTheta",
			      "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatTheta",";");
    VariableDescription tv("real","theta",1u,0u);
    tv.description = "theta value used by the implicit scheme";
    this->mb.addParameter(h,tv);
    this->mb.setParameterDefaultValue(h,"theta",theta);
    this->mb.setEntryName(h,"theta","theta");
  } // end of ImplicitDSLBase::treatTheta

  void
  ImplicitDSLBase::treatEpsilon(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile("ImplicitDSLBase::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ImplicitDSLBase::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("ImplicitDSLBase::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatEpsilon",";");
    VariableDescription e("real","epsilon",1u,0u);
    e.description = "value used to stop the iteration of the implicit algorithm";
    this->mb.addParameter(h,e);
    this->mb.setParameterDefaultValue(h,"epsilon",epsilon);
    this->mb.setEntryName(h,"epsilon","epsilon");
  } // ImplicitDSLBase::treatEpsilon

  void
  ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile("ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation",";");
    VariableDescription e("real","numerical_jacobian_epsilon",1u,0u);
    e.description = "perturbation value used to compute a finite difference approximation of the jacobian";
    this->mb.addParameter(h,e);
    this->mb.setParameterDefaultValue(h,"numerical_jacobian_epsilon",epsilon);
  } // ImplicitDSLBase::treatEpsilon

  void
  ImplicitDSLBase::treatIterMax(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    unsigned short iterMax;
    iterMax = this->readUnsignedShort("ImplicitDSLBase::treatIterMax");
    if(iterMax==0){
      this->throwRuntimeError("ImplicitDSLBase::treatIterMax",
			      "invalid value for parameter 'iterMax'");
    }
    this->readSpecifiedToken("ImplicitDSLBase::treatIterMax",";");
    this->mb.addParameter(h,VariableDescription("ushort","iterMax",1u,0u));
    this->mb.setParameterDefaultValue(h,"iterMax",iterMax);
  } // end of ImplicitDSLBase::treatIterMax

  std::string
  ImplicitDSLBase::tangentOperatorVariableModifier(const Hypothesis h,
							    const std::string& var,
							    const bool addThisPtr)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    if(d.isIntegrationVariableIncrementName(var)){
      if(this->mb.hasAttribute(h,var.substr(1)+"_normalisation_factor")){
	const auto& s = d.getStateVariableHandler(var.substr(1));
	const auto& nf = this->mb.getAttribute<string>(h,var.substr(1)+"_normalisation_factor");
	if(s.arraySize==1u){
	  if(addThisPtr){
	    return "(("+nf+")*(this->"+var+"))";
	  } else {
	    return "("+nf+")*var";
	  }
	} else {
	  if(addThisPtr){
	    return "("+nf+")*(this->"+var+")";
	  } else {
	    return "("+nf+")*var";
	  }
	}
      }
    }
    if(addThisPtr){
      return "(this->"+var+")";
    } else {
      return var;
    }
  } // end of ImplicitDSLBase::tangentOperatorVariableModifier

  std::string
  ImplicitDSLBase::integratorVariableModifier(const Hypothesis h,
						       const std::string& var,
						       const bool addThisPtr)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    if(d.isIntegrationVariableIncrementName(var)){
      if(this->mb.hasAttribute(h,var.substr(1)+"_normalisation_factor")){
	const auto& s = d.getStateVariableHandler(var.substr(1));
	const auto& nf = this->mb.getAttribute<string>(h,var.substr(1)+"_normalisation_factor");
	if(s.arraySize==1u){
	  if(addThisPtr){
	    return "(("+nf+")*(this->"+var+"))";
	  } else {
	    return "("+nf+")*var";
	  }
	} else {
	  if(addThisPtr){
	    return "("+nf+")*(this->"+var+")";
	  } else {
	    return "("+nf+")*var";
	  }
	}
      }
    }
    if(addThisPtr){
      return "(this->"+var+")";
    } else {
      return var;
    }
  } // end of ImplicitDSLBase::integratorVariableModifier

  std::string
  ImplicitDSLBase::computeStressVariableModifier1(const Hypothesis h,
							   const std::string& var,
							   const bool addThisPtr)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    if(this->mb.isDrivingVariableName(var)||(var=="T")|
       (d.isExternalStateVariableName(var))){
      if(addThisPtr){
	return "(this->"+var+"+(this->theta)*(this->d"+var+"))";
      } else {
	return "("+var+"+(this->theta)*d"+var+")";
      }
    }
    if(d.isIntegrationVariableName(var)){
      if(this->mb.hasAttribute(h,var+"_normalisation_factor")){
	const auto& s = d.getStateVariableHandler(var);
	const auto& nf = this->mb.getAttribute<string>(h,var+"_normalisation_factor");
	if(s.arraySize==1u){
	  if(addThisPtr){
	    return "(this->"+var+"+(this->theta)*(("+nf+")*(this->d"+var+")))";
	  } else {
	    return "("+var+"+("+nf+")*(this->theta)*d"+var+")";
	  }
	} else {
	  if(addThisPtr){
	    return "(this->"+var+"+(this->theta)*(("+nf+")*(this->d"+var+")))";
	  } else {
	    return "("+var+"+("+nf+")*(this->theta)*d"+var+")";
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
  } // end of ImplicitDSLBase::computeStressVariableModifier1

  std::string
  ImplicitDSLBase::computeStressVariableModifier2(const Hypothesis h,
							   const std::string& var,
							   const bool addThisPtr)
  {
    const auto& d = this->mb.getBehaviourData(h);
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
  } // end of ImplicitDSLBase::computeStressVariableModifier2

  bool
  ImplicitDSLBase::isJacobianPart(const Hypothesis h,
					   const std::string& w)
  {
    const auto& d = this->mb.getBehaviourData(h);
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
  } // end of ImplicitDSLBase::isJacobianPart

  void
  ImplicitDSLBase::integratorAnalyser(const Hypothesis h,
					       const std::string& w)
  {
    if(this->isJacobianPart(h,w)){
      this->jacobianPartsUsedInIntegrator.insert(w);
    }
  } // end of ImplicitDSLBase::integratorAnalyser

  void
  ImplicitDSLBase::predictorAnalyser(const Hypothesis h,
					      const std::string& w)
  {
    const auto& d = this->mb.getBehaviourData(h);
    if(d.isIntegrationVariableIncrementName(w)){
      this->integrationVariablesIncrementsUsedInPredictor.insert(w);
    }
  } // end of ImplicitDSLBase::predictorAnalyser

  void
  ImplicitDSLBase::treatIntegrator(void)
  {
    this->readCodeBlock(*this,BehaviourData::Integrator,
			&ImplicitDSLBase::integratorVariableModifier,
			&ImplicitDSLBase::integratorAnalyser,true);
  } // end of ImplicitDSLBase::treatIntegrator

  void
  ImplicitDSLBase::treatPredictor(void)
  {
    using namespace std;
    this->readCodeBlock(*this,BehaviourData::ComputePredictor,
			&ImplicitDSLBase::standardModifier,
			&ImplicitDSLBase::predictorAnalyser,true);
  } // end of ImplicitDSLBase::treatPredictor

  void
  ImplicitDSLBase::treatComputeStress(void)
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
    this->readCodeBlock(*this,BehaviourData::ComputeStress,
			BehaviourData::ComputeFinalStressCandidate,
			&ImplicitDSLBase::computeStressVariableModifier1,
			&ImplicitDSLBase::computeStressVariableModifier2,true,true);
  } // end of ImplicitDSLBase::treatComputeStress

  void
  ImplicitDSLBase::treatComputeFinalStress(void)
  {
    this->readCodeBlock(*this,BehaviourData::ComputeFinalStress,
			&ImplicitDSLBase::computeStressVariableModifier2,true,true);
  } // end of ImplicitDSLBase::treatComputeFinalStress

  void
  ImplicitDSLBase::treatMaximumIncrementValuePerIteration(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double value;
    this->checkNotEndOfFile("ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
			    "Cannot read value value.");
    istringstream flux(current->value);
    flux >> value;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
			      "Failed to read value.");
    }
    if(value<=0){
      this->throwRuntimeError("ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
			      "Value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::MaximumIncrementValuePerIteration",";");
    this->mb.addParameter(h,VariableDescription("real","maximum_increment_value_per_iteration",1u,0u));
    this->mb.setParameterDefaultValue(h,"maximum_increment_value_per_iteration",value);
  } // end of ImplicitDSLBase::treatMaximumIncrementValuePerIteration

  void ImplicitDSLBase::writeBehaviourParserSpecificIncludes(void)
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
    const auto& h = this->mb.getModellingHypotheses();
    for(const auto & elem : h){
      const auto& d = this->mb.getBehaviourData(elem);
      const auto& sv = d.getIntegrationVariables();
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
  } // end of ImplicitDSLBase::writeBehaviourParserSpecificIncludes(void)
  
  void ImplicitDSLBase::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    BehaviourDSLCommon::writeBehaviourParserSpecificTypedefs();
  } // end of ImplicitDSLBase::writeBehaviourParserSpecificTypedefs(void)

  void
  ImplicitDSLBase::writeBehaviourParserSpecificMembers(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
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
	      string msg("ImplicitDSLBase::writeBehaviourParserSpecificMembers : ");
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
	      string msg("ImplicitDSLBase::writeBehaviourParserSpecificMembers : ");
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
    if(this->solver->usesJacobian()){
      this->behaviourFile << "// Jacobian\n";
      this->behaviourFile << "tfel::math::tmatrix<" << n << "," << n << ",Type> jacobian;\n";
    }
    if(this->solver->usesJacobianInvert()){
      this->behaviourFile << "// Jacobian\n";
      this->behaviourFile << "tfel::math::tmatrix<" << n << "," << n << ",Type> inv_jacobian;\n";
    }
    this->solver->writeSpecificMembers(this->behaviourFile,this->mb,h);
    this->behaviourFile << "// zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> zeros;\n\n";
    this->behaviourFile << "// previous zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> zeros_1;\n\n";
    this->behaviourFile << "// function\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",Type> fzeros;\n\n";
    this->behaviourFile << "// number of iterations\n";
    this->behaviourFile << "unsigned int iter;\n\n";
    if(this->solver->usesJacobian()){
      // compute the numerical part of the jacobian.  This method is
      // used to compute a numerical approximation of the jacobian for
      // the solvers based on it. For solvers providing an analytical
      // jacobian, or an approximation of it, this method is used to
      // compare the jacobain to its numerical approximation
      this->writeComputeNumericalJacobian(h);
    }
    if(this->solver->usesJacobian()){
      // for solver based on the jacobian matrix, the get partial
      // jacobian invert method can be used to compute the tangent
      // operator.
      this->writeGetPartialJacobianInvert(h);
    }
    // compute stress
    if(this->mb.hasCode(h,BehaviourData::ComputeStress)){
      this->behaviourFile << "void\ncomputeStress(void){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("ImplicitDSLBase::writeBehaviourParserSpecificMembers",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputeStress) << endl;
      this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeStress\n\n";
    }
    if(this->mb.hasCode(h,BehaviourData::ComputeFinalStress)){
      this->behaviourFile << "void\ncomputeFinalStress(void){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("ImplicitDSLBase::writeBehaviourParserSpecificMembers",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputeFinalStress) << endl;
      this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeStress\n\n";
      this->behaviourFile << endl;
    }
  } // end of ImplicitDSLBase::writeBehaviourParserSpecificMembers

  void
  ImplicitDSLBase::writeGetPartialJacobianInvert(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
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
	    string msg("ImplicitDSLBase::writeGetPartialJacobianInvert : ");
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
	    string msg("ImplicitDSLBase::writeGetPartialJacobianInvert : ");
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
	  string msg("ImplicitDSLBase::writeGetPartialJacobianInvert : ");
	  msg += "internal error, tag unsupported";
	  throw(runtime_error(msg));
	}
      }
      this->behaviourFile << "}\n";
      for(p2=d.getIntegrationVariables().begin();p2!=p;++p2){
	if(this->mb.hasAttribute(h,p2->name+"_normalisation_factor")){
	  const auto& nf = this->mb.getAttribute<string>(h,p2->name+"_normalisation_factor");
	  this->behaviourFile << "partial_jacobian_" << p2->name << " /= " << nf << ";\n";
	}
      }
      this->behaviourFile << "}\n\n";
    }
  } // end of ImplicitDSLBase::writeGetPartialJacobianInvert
  
  void ImplicitDSLBase::writeComputeNumericalJacobian(const Hypothesis h){
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
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
    if(this->mb.hasCode(h,BehaviourData::ComputeStress)){
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "this->computeFdF();\n";
    this->behaviourFile << "this->zeros = tzeros;\n";
    this->behaviourFile << "tvector<" << n << ",real> tfzeros2(this->fzeros);\n";
    this->behaviourFile << "this->zeros(idx) += this->numerical_jacobian_epsilon;\n";
    if(this->mb.hasCode(h,BehaviourData::ComputeStress)){
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
  ImplicitDSLBase::getVectorMappingClass(const VariableDescription& v) const
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
    string msg("ImplicitDSLBase::getVectorMappingClass : "
	       "unsupported type for variable '"+v.name+"'");
    throw(runtime_error(msg));
    return "";
  } // end of ImplicitDSLBase::getVectorMappingClass

  void ImplicitDSLBase::writeBehaviourIntegrator(const Hypothesis h){
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
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
    if(this->mb.hasAttribute(h,BehaviourData::hasConsistentTangentOperator)){
      this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt){\n";
    } else {
      if((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL)){
	this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt){\n";
      } else {
	this->behaviourFile << "integrate(const SMFlag,const SMType smt){\n";
      }
    }
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    if((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL)){
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
    if(this->mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfilingBegin(this->behaviourFile,
					     mb.getClassName(),
					     BehaviourData::Integrator);
    }
    this->solver->writeResolutionAlgorithm(this->behaviourFile,this->mb,h);
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
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : convergence after \" "
			  << "<< this->iter << \" iterations\"<< endl << endl;\n";
    }
    if(this->mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfilingEnd(this->behaviourFile);
    }
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if(this->mb.hasAttribute(h,p->name+"_normalisation_factor")){
	const auto& nf = this->mb.getAttribute<string>(h,p->name+"_normalisation_factor");
	this->behaviourFile << "this->d" << p->name << " *= " << nf << ";\n";
      }
    }
    this->behaviourFile << "this->updateIntegrationVariables();\n";
    this->behaviourFile << "this->updateStateVariables();\n";
    if(this->mb.hasCode(h,BehaviourData::ComputeFinalStress)){
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
    if(this->mb.hasAttribute(h,BehaviourData::hasConsistentTangentOperator)){
      if(this->mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
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
  ImplicitDSLBase::writeComputeFdF(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
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
    if(this->mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfiling(this->behaviourFile,
					mb.getClassName(),
					"ComputeFdF");
    }
    writeMaterialLaws("ImplicitDSLBase::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->mb.getMaterialLaws());
    n = SupportedTypes::TypeSize();
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if(this->varNames.find("f"+p->name)!=this->varNames.end()){
	string msg("ImplicitDSLBase::writeBehaviourIntegrator : ");
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
    if((this->solver->usesJacobian())&&
       (!(this->solver->requiresNumericalJacobian()))){
      n = SupportedTypes::TypeSize();
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	n3 = SupportedTypes::TypeSize();
	for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	  if((p->arraySize==1u)&&
	     (p2->arraySize==1u)){
	    this->behaviourFile << "// derivative of variable f" << p->name 
				<< " by variable " << p2->name << "\n";
	    this->behaviourFile << NonLinearSystemSolverBase::getJacobianPart(this->mb,*p,*p2,n,n2,n3);
	  }
	  n3 += this->getTypeSize(p2->type,p2->arraySize);
	}
	n += this->getTypeSize(p->type,p->arraySize);
      }
    }
    if(this->solver->requiresJacobianToBeReinitialisedToIdentityAtEachIterations()){
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
      if(this->mb.hasAttribute(h,p->name+"_normalisation_factor")){
	const auto& nf = this->mb.getAttribute<string>(h,p->name+"_normalisation_factor");
    	this->behaviourFile << "f" << p->name << " *= " << nf << ";" << endl;
      }
    }
    this->behaviourFile << this->mb.getCodeBlock(h,BehaviourData::Integrator).code << "\n";
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      if(this->mb.hasAttribute(h,'f'+p->name+"_normalisation_factor")){
	const auto& nf = this->mb.getAttribute<string>(h,'f'+p->name+"_normalisation_factor");
    	this->behaviourFile << "f" << p->name << "*= real(1)/(" << nf << ");" << endl;
      }
    }
    if((this->solver->usesJacobian())&&
       (!(this->solver->requiresNumericalJacobian()))){
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	for(p2=d.getIntegrationVariables().begin();p2!=d.getIntegrationVariables().end();++p2){
	  if((p->arraySize==1u)&&(p2->arraySize==1u)){
	    this->behaviourFile << "static_cast<void>(df"
				<< p->name << "_dd" << p2->name
				<< "); /* suppress potential warnings */\n";
	  }
	  if(this->jacobianPartsUsedInIntegrator.find("df"+p->name+"_dd"+p2->name)!=
	     this->jacobianPartsUsedInIntegrator.end()){
	    const bool bfnf = this->mb.hasAttribute(h,'f'+p->name+"_normalisation_factor");
	    const bool bvnf = this->mb.hasAttribute(h,p2->name+"_normalisation_factor");
	    if(bfnf){
	      const auto& fnf = this->mb.getAttribute<string>(h,'f'+p->name+"_normalisation_factor");	      
	      if(bvnf){
		const auto& vnf = this->mb.getAttribute<string>(h,p2->name+"_normalisation_factor");
		if((p->arraySize!=1u)&&
		   (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				      << p2->arraySize << ";++idx2){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx,idx2) *= " 
				      << vnf << "/(" << fnf << ");" << endl;
		  this->behaviourFile << "}\n";
		  this->behaviourFile << "}\n";
		} else if((p->arraySize!=1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << vnf << "/(" << fnf << ");" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p2->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << vnf << "/(" << fnf << ");" << endl;		  
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << " *= " 
				      << vnf << "/(" << fnf << ");" << endl;
		}
	      } else {
		if((p->arraySize!=1u)&&
		   (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				      << p2->arraySize << ";++idx2){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx,idx2) *= " 
				      << "real(1)/(" << fnf << ");" << endl;
		  this->behaviourFile << "}\n";
		  this->behaviourFile << "}\n";
		} else if((p->arraySize!=1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << "real(1)/(" << fnf << ");" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p2->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << "real(1)/(" << fnf << ");" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << " *= " 
				      << "real(1)/(" << fnf << ");" << endl;
		}
	      }
	    } else{
	      if(bvnf){
		const auto& vnf = this->mb.getAttribute<string>(h,p2->name+"_normalisation_factor");
		if((p->arraySize!=1u)&&
		   (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "for(unsigned short idx2=0;idx2!="
				      << p2->arraySize << ";++idx2){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx,idx2) *= " 
				      << vnf << ";" << endl;
		  this->behaviourFile << "}\n";
		  this->behaviourFile << "}\n";
		} else if((p->arraySize!=1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << vnf << ";" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize!=1u)){
		  this->behaviourFile << "for(unsigned short idx=0;idx!="
				      << p2->arraySize << ";++idx){" << endl;
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << "(idx) *= " 
				      << vnf << ";" << endl;
		  this->behaviourFile << "}\n";
		} else if((p->arraySize==1u)&&
			  (p2->arraySize==1u)){
		  this->behaviourFile << "df" << p->name << "_dd" << p2->name << " *= " 
				      << vnf << ";" << endl;
		}
	      }
	    }
	  }
	}
      }
    }
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  } // end of ImplicitDSLBase::writeBehaviourIntegrator

  std::string
  ImplicitDSLBase::getIntegrationVariablesIncrementsInitializers(const VariableDescriptionContainer& v,
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
	string msg("ImplicitDSLBase::writeBehaviourConstructors : ");
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
  } // end of ImplicitDSLBase::getIntegrationVariableIncrementsInitializers

  std::string
  ImplicitDSLBase::getBehaviourConstructorsInitializers(const Hypothesis h)
  {    
    using namespace std;
    string init = BehaviourDSLCommon::getBehaviourConstructorsInitializers(h);
    if(!init.empty()){
      init += ",\n";
    }
    init += "zeros(real(0)),\nfzeros(real(0))";
    return init;
  }

  void ImplicitDSLBase::writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis h)
  {
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    this->checkBehaviourFile();
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n += this->getTypeSize(p->type,p->arraySize);
    }
    this->solver->writeSpecificInitializeMethodPart(this->behaviourFile,mb,h);
  }

  void ImplicitDSLBase::writeBehaviourIntegrationVariablesIncrements(const Hypothesis h)
  {    
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
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
  ImplicitDSLBase::endsInputFileProcessing(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    BehaviourDSLCommon::endsInputFileProcessing();
    if(this->solver.get()==nullptr){
      const NonLinearSystemSolverFactory& f =
	NonLinearSystemSolverFactory::getNonLinearSystemSolverFactory();
      this->solver = f.getSolver("NewtonRaphson");
    }
#warning "shall be done earlier"
    // reserved names
    const auto& n = this->solver->getReservedNames();
    for(const auto & elem : n){
      this->reserveName(elem,false);
    }
    if(this->mb.getAttribute(h,BehaviourData::compareToNumericalJacobian,false)){
      if((!this->solver->usesJacobian())||(this->solver->requiresNumericalJacobian())){
	string msg("ImplicitDSLBase::endsInputFileProcessing :");
	msg += "@CompareToNumericalJacobian can only be used with solver using ";
	msg += "an analytical jacobian (or an approximation of it";
	throw(runtime_error(msg));

      }
    }
    // create the compute final stress code is necessary
    this->setComputeFinalStressFromComputeFinalStressCandidateIfNecessary();
    // creating default parameters if not explicitely specified by the user
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
    this->solver->endsInputFileProcessing(this->mb);
    if((this->mb.getAttribute(h,BehaviourData::compareToNumericalJacobian,false))||
       (this->solver->usesJacobian())){
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
    if(this->mb.getAttribute(h,BehaviourData::compareToNumericalJacobian,false)){
      if(!this->mb.hasParameter(h,"jacobianComparisonCriterium")){
	this->mb.addParameter(h,VariableDescription("real","jacobianComparisonCriterium",1u,0u));
	this->mb.setParameterDefaultValue(h,"jacobianComparisonCriterium",
					  this->mb.getFloattingPointParameterDefaultValue(h,"epsilon"));
      }
    }
    // correct prediction to take into account normalisation factors
    const set<Hypothesis> mh(this->mb.getDistinctModellingHypotheses());
    for(const auto & elem : mh){
      if(this->mb.hasCode(elem,BehaviourData::ComputePredictor)){
	CodeBlock predictor;
	VariableDescriptionContainer::const_iterator p;
	const auto& d = this->mb.getBehaviourData(elem);
	const auto& sv = d.getIntegrationVariables();
	for(p=sv.begin();p!=sv.end();++p){
	  if(this->integrationVariablesIncrementsUsedInPredictor.find('d'+p->name)!=
	     this->integrationVariablesIncrementsUsedInPredictor.end()){
	    if(this->mb.hasAttribute(h,p->name+"_normalisation_factor")){
	      const auto& nf = this->mb.getAttribute<string>(h,p->name+"_normalisation_factor");
	      predictor.code += "this->d"+p->name + " /= " + nf + ";\n";
	    }
	  }
	}
	this->mb.setCode(elem,BehaviourData::ComputePredictor,predictor,
			 BehaviourData::CREATEORAPPEND,
			 BehaviourData::AT_END);
      }
    }
    if(!this->mb.areAllMechanicalDataSpecialised()){
      if(this->mb.hasCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			  BehaviourData::ComputePredictor)){
	CodeBlock predictor;
	VariableDescriptionContainer::const_iterator p;
	const BehaviourData& d =
	  this->mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	const auto& sv = d.getIntegrationVariables();
	for(p=sv.begin();p!=sv.end();++p){
	  if(this->integrationVariablesIncrementsUsedInPredictor.find('d'+p->name)!=
	     this->integrationVariablesIncrementsUsedInPredictor.end()){
	    if(this->mb.hasAttribute(h,p->name+"_normalisation_factor")){
	      const auto& nf = this->mb.getAttribute<string>(h,p->name+"_normalisation_factor");
	      predictor.code += "this->d"+p->name + " /= " + nf + ";\n";
	    }
	  }
	}
	this->mb.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			 BehaviourData::ComputePredictor,predictor,
			 BehaviourData::CREATEORAPPEND,
			 BehaviourData::AT_END);
      }
    }
    // minimal tangent operator
    this->setMinimalTangentOperator();
  } // end of ImplicitDSLBase::endsInputFileProcessing(void)

  ImplicitDSLBase::~ImplicitDSLBase()
  {} // end of ~ImplicitDSLBase

} // end of namespace mfront  

