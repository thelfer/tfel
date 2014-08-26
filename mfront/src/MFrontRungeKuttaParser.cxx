/*!
 * \file   MFrontRungeKuttaParser.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<string>
#include<sstream>
#include<stdexcept>

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontRungeKuttaParser.hxx"

namespace mfront{

  MFrontRungeKuttaParser::MFrontRungeKuttaParser()
    : MFrontBehaviourParserBase<MFrontRungeKuttaParser>()
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->useStateVarTimeDerivative=true;
    this->registerVariable("epsilon",false);
    this->registerVariable("dtmin",false);
    // main variables
    this->registerVariable("eto",false);
    this->registerVariable("deto",false);
    this->registerVariable("sig",false);
    this->mb.declareAsASmallStrainStandardBehaviour();
    // driving variables
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    for(pm=this->mb.getMainVariables().begin();
	pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      this->registerVariable(dv.name+"_",false);
      this->registerVariable("d"+dv.name+"_",false);
      this->mb.addLocalVariable(h,VariableDescription(dv.type,dv.name+"_",1u,0u));
      this->mb.addLocalVariable(h,VariableDescription(SupportedTypes::getTimeDerivativeType(dv.type),
						 "d"+dv.name+"_",1u,0u));
    }
    // Default state vars
    this->registerVariable("eel",false);
    this->registerVariable("deel",false);
    this->registerVariable("t",false);
    this->registerVariable("T_",false);
    this->reserveName("dt_",false);
    this->reserveName("corrector",false);
    this->reserveName("dtprec",false);
    this->reserveName("converged",false);
    this->reserveName("error",false);
    this->reserveName("failed",false);
    this->mb.addLocalVariable(h,VariableDescription("temperature","T_",1u,0u));
    this->mb.addStateVariable(h,VariableDescription("StrainStensor","eel",1u,0u));
    this->mb.setGlossaryName(h,"eel","ElasticStrain");
    // CallBacks
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &MFrontRungeKuttaParser::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&MFrontRungeKuttaParser::treatMaterialLaw);
    this->registerNewCallBack("@Algorithm",&MFrontRungeKuttaParser::treatAlgorithm);
    this->registerNewCallBack("@TangentOperator",
    			      &MFrontRungeKuttaParser::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
    			      &MFrontRungeKuttaParser::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@Derivative",&MFrontRungeKuttaParser::treatDerivative);
    this->registerNewCallBack("@ComputeStress",&MFrontRungeKuttaParser::treatComputeStress);
    this->registerNewCallBack("@Epsilon",&MFrontRungeKuttaParser::treatEpsilon);
    this->registerNewCallBack("@MinimalTimeStep",
			      &MFrontRungeKuttaParser::treatMinimalTimeStep);
    this->registerNewCallBack("@RequireStiffnessTensor",
			      &MFrontRungeKuttaParser::treatRequireStiffnessOperator);
    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
  }

  void MFrontRungeKuttaParser::writeBehaviourParserSpecificIncludes(void)
  {
    using namespace std;
    bool b1 = false;
    bool b2 = false;
    this->checkBehaviourFile();
    this->behaviourFile << "#include\"TFEL/Math/General/Abs.hxx\"" << endl << endl;
    this->mb.requiresTVectorOrVectorIncludes(b1,b2);
    if(b1){
      this->behaviourFile << "#include\"TFEL/Math/tvector.hxx\"\n";
    }
    if(b2){
      this->behaviourFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
  }

  void MFrontRungeKuttaParser::treatStateVariable(void)
  {
    using namespace std;
    VarContainer sv;
    set<Hypothesis> h;
    this->readVariableList(sv,h,&MechanicalBehaviourDescription::addStateVariables,true,true,false);
    if(!this->mb.hasAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			      MechanicalBehaviourData::numberOfEvaluations)){
      this->setDefaultAlgorithm();
    }
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      // On devrait le faire plus tard, pour ne traiter que les variables effectivement utilisées
#warning "HERE"
      CodeBlock ib;
      for(VariableDescriptionContainer::const_iterator p=sv.begin();p!=sv.end();++p){
	string currentVarName = p->name + "_";
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  ostream& log = getLogStream();
	  log << "registring variable '" << currentVarName << "'";
	  if(*ph==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	    log << " for default hypothesis" << endl;
	  } else {
	    log << " for the '" << ModellingHypothesis::toString(*ph)
		<< "' hypothesis" << endl;
	  }
	}
	this->registerVariable(currentVarName,false);
	this->mb.addLocalVariable(*ph,VariableDescription(p->type,currentVarName,p->arraySize,0u));
	if(this->useDynamicallyAllocatedVector(p->arraySize)){
	  ib.code += "this->"+currentVarName +".resize("+toString(p->arraySize)+");\n";
	}
	ib.code += "this->" +currentVarName + " = this->" + p->name + ";\n";
	const unsigned short n =
	  this->mb.getAttribute<unsigned short>(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
						MechanicalBehaviourData::numberOfEvaluations);
	for(unsigned short i=0u;i!=n;++i){
	  currentVarName = "d" + p->name + "_K"+toString(static_cast<unsigned short>(i+1u));
	  if(getVerboseMode()>=VERBOSE_DEBUG){
	    ostream& log = getLogStream();
	    log << "registring variable '" << currentVarName << "'";
	    if(*ph==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	      log << " for default hypothesis" << endl;
	    } else {
	      log << " for the '" << ModellingHypothesis::toString(*ph)
		  << "' hypothesis" << endl;
	    }
	  }
	  this->registerVariable(currentVarName,false);
	  this->mb.addLocalVariable(*ph,VariableDescription(p->type,currentVarName,p->arraySize,0u));
	}
      }
      this->mb.setCode(*ph,MechanicalBehaviourData::BeforeInitializeLocalVariables,ib,
		       MechanicalBehaviourData::CREATEORAPPEND,
		       MechanicalBehaviourData::AT_END);
    }
  } // end of MFrontRungeKuttaParser::treatStateVariable

  void MFrontRungeKuttaParser::treatExternalStateVariable(void)
  {
    using namespace std;
    VariableDescriptionContainer ev;
    set<Hypothesis> h;
    this->readVariableList(ev,h,&MechanicalBehaviourDescription::addExternalStateVariables,true,true,false);
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      // On devrait le faire plus tard, pour ne traiter que les variables effectivement utilisées
#warning "HERE"
      CodeBlock ib;
      for(VariableDescriptionContainer::const_iterator p=ev.begin();p!=ev.end();++p){
	string currentVarName = p->name + "_";
	this->registerVariable(currentVarName,false);
	this->mb.addLocalVariable(*ph,VariableDescription(p->type,currentVarName,p->arraySize,0u));
	if(this->useDynamicallyAllocatedVector(p->arraySize)){
	  ib.code += "this->"+currentVarName+".resize("+toString(p->arraySize)+");\n";
	}
	ib.code += "this->" + currentVarName + " = this->" + p->name + ";\n";
      }
      this->mb.setCode(*ph,MechanicalBehaviourData::BeforeInitializeLocalVariables,ib,
		       MechanicalBehaviourData::CREATEORAPPEND,
		       MechanicalBehaviourData::AT_END);
    }
  } // end of MFrontRungeKuttaParser::treatStateVariable

  void
  MFrontRungeKuttaParser::treatUpdateAuxiliaryStateVariables(void)
  {
    using tfel::utilities::shared_ptr;
    this->readCodeBlock(*this,MechanicalBehaviourData::UpdateAuxiliaryStateVariables,
			&MFrontRungeKuttaParser::standardModifier,true,true);
  } // end of MFrontRungeKuttaParser::treatUpdateAuxiliaryStateVarBase

  std::string
  MFrontRungeKuttaParser::computeStressVariableModifier1(const Hypothesis h,
							 const std::string& var,
							 const bool addThisPtr)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if((this->mb.isDrivingVariableName(var)) ||(var=="T") ||
       (this->mb.isDrivingVariableIncrementName(var))||
       (d.isIntegrationVariableName(var))||
       (d.isExternalStateVariableName(var))){
      if(addThisPtr){
	return "this->"+var+"_";
      } else {
	return var+"_";
      }
    }
    if(var=="dT"){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
      if(addThisPtr){
	return "(this->"+var+")/(this->dt)";
      } else {
	return "("+var+")/(this->dt)";
      }
    }
    if(this->mb.isExternalStateVariableIncrementName(h,var)){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
      const VariableDescription& v = d.getVariableHandler(d.getExternalStateVariables(),
							  var.substr(1));
      if(v.arraySize>1){
	if(addThisPtr){
	  return "(real(1)/(this->dt)) * (this->"+var+")";
	} else {
	  return "(real(1)/(this->dt)) * "+var;
	}
      } else {
	if(addThisPtr){
	  return "(this->"+var+")/(this->dt)";
	} else {
	  return "("+var+")/(this->dt)";
	}
      }
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontRungeKuttaParser::computeStressVariableModifier1

  std::string
  MFrontRungeKuttaParser::computeStressVariableModifier2(const Hypothesis h,
							 const std::string& var,
							 const bool addThisPtr)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if((this->mb.isDrivingVariableName(var))||(var=="T")||
       (d.isExternalStateVariableName(var))){
      if(addThisPtr){
	return "this->"+var+"+this->d"+var;
      } else {
	return var+"+d"+var;
      }
    }
    if((d.isExternalStateVariableIncrementName(var))||
       (var=="dT")||(this->mb.isDrivingVariableIncrementName(var))){
      if((d.isExternalStateVariableIncrementName(var))||(var=="dT")){
	this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
      }
      if(addThisPtr){
	return "(this->"+var+")/(this->dt)";
      } else {
	return "("+var+")/(this->dt)";
      }
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontRungeKuttaParser::computeStressVariableModifier2

  void
  MFrontRungeKuttaParser::treatComputeStress(void)
  {
    this->readCodeBlock(*this,MechanicalBehaviourData::ComputeStress,
			MechanicalBehaviourData::ComputeFinalStress,
			&MFrontRungeKuttaParser::computeStressVariableModifier1,
			&MFrontRungeKuttaParser::computeStressVariableModifier2,true,true);
  } // end of MFrontRungeKuttaParser::treatComputeStress

  void MFrontRungeKuttaParser::treatUnknownVariableMethod(const Hypothesis h,
							  const std::string& n)
  {
    using namespace std;
    if(this->mb.isIntegrationVariableName(h,n)){
      if(this->current->value=="setErrorNormalisationFactor"){
    	string var;
    	++(this->current);
    	this->checkNotEndOfFile("MFrontRungeKuttaParser::treatUnknowVariableMethod");
    	this->readSpecifiedToken("MFrontRungeKuttaParser::treatUnknowVariableMethod","(");
    	this->checkNotEndOfFile("MFrontRungeKuttaParser::treatUnknowVariableMethod");
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
    	    this->throwRuntimeError("MFrontRungeKuttaParser::treatUnknowVariableMethod",
    				    "Failed to read error normalisation factor.");
    	  }
    	  if(value<0.){
    	    this->throwRuntimeError("MFrontRungeKuttaParser::treatUnknowVariableMethod",
    				    "invalid error normalisation factor.");
    	  }
    	}
    	if(!this->enf.insert(make_pair(n,var)).second){
    	  this->throwRuntimeError("MFrontRungeKuttaParser::treatUnknowVariableMethod",
    				  "Error normalisation factor already defined for variable '"+n+"'.");
    	}
    	++(this->current);
    	this->checkNotEndOfFile("MFrontRungeKuttaParser::treatUnknowVariableMethod");
    	return;
      }
    }
    MFrontBehaviourParserCommon::treatUnknownVariableMethod(h,n);
  } // end of MFrontRungeKuttaParser::treatUnknowVariableMethod

  void MFrontRungeKuttaParser::treatDerivative(void)
  {
    this->readCodeBlock(*this,MechanicalBehaviourData::ComputeDerivative,
			&MFrontRungeKuttaParser::computeStressVariableModifier1,true,true);
  } // end of MFrontRungeKuttaParser::treatDerivative

  void
  MFrontRungeKuttaParser::treatEpsilon(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(this->mb.hasParameter(h,"epsilon")){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatEpsilon",
			      "value already specified.");
    }
    double epsilon;
    this->checkNotEndOfFile("MFrontRungeKuttaParser::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontRungeKuttaParser::treatEpsilon",";");
    this->mb.addParameter(h,VariableDescription("real","epsilon",1u,0u));
    this->mb.setParameterDefaultValue(h,"epsilon",epsilon);
  } // end of MFrontRungeKuttaParser::treatEpsilon

  void
  MFrontRungeKuttaParser::treatMinimalTimeStep(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(this->mb.hasParameter(h,"dtmin")){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatMinimalTimeStep",
			      "value already specified.");
    }
    double dtmin;
    this->checkNotEndOfFile("MFrontRungeKuttaParser::treatMinimalTimeStep",
			    "Cannot read dtmin value.");
    istringstream flux(current->value);
    flux >> dtmin;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatMinimalTimeStep",
			      "Failed to read dtmin value.");
    }
    if(dtmin<0){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatMinimalTimeStep",
			      "MinimalTimeStep value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontRungeKuttaParser::treatMinimalTimeStep",";");
    this->mb.addParameter(h,VariableDescription("real","dtmin",1u,0u));
    this->mb.setParameterDefaultValue(h,"dtmin",dtmin);
  } // end of MFrontRungeKuttaParser::treatEpsilon

  void MFrontRungeKuttaParser::setDefaultAlgorithm(void)
  {
    using namespace std;
    typedef unsigned short ushort;
    this->mb.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			  MechanicalBehaviourData::algorithm,
			  string("RungeKutta5/4"));
    this->mb.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
			  MechanicalBehaviourData::numberOfEvaluations,
			  ushort(6u));
  } // end of MFrontRungeKuttaParser::setDefaultAlgorithm

  void MFrontRungeKuttaParser::treatAlgorithm(void)
  {
    using namespace std;
    typedef unsigned short ushort;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("MFrontRungeKuttaParser::treatAlgorithm",
			    "Cannot read algorithm name.");
    if(this->mb.hasAttribute(h,MechanicalBehaviourData::algorithm)){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatAlgorithm",
			      "algorith already specified. This may be the second "
			      "time that the @Algorithm keyword is used, or the default "
			      "algorithm was selected when registring a state variable "
			      "(keyword @StateVariable)");
    }
    if(this->current->value=="euler"){
      this->mb.setAttribute(h,MechanicalBehaviourData::algorithm,string("Euler"));
      this->mb.setAttribute(h,MechanicalBehaviourData::numberOfEvaluations,ushort(0u));
    } else if(this->current->value=="rk2"){
      this->mb.setAttribute(h,MechanicalBehaviourData::algorithm,string("RungeKutta2"));
      this->mb.setAttribute(h,MechanicalBehaviourData::numberOfEvaluations,ushort(1u));
    } else if(this->current->value=="rk4"){
      this->mb.setAttribute(h,MechanicalBehaviourData::algorithm,string("RungeKutta4"));
      this->mb.setAttribute(h,MechanicalBehaviourData::numberOfEvaluations,ushort(4u));
    } else if(this->current->value=="rk42"){
      this->mb.setAttribute(h,MechanicalBehaviourData::algorithm,string("RungeKutta4/2"));
      this->mb.setAttribute(h,MechanicalBehaviourData::numberOfEvaluations,ushort(4u));
    } else if(this->current->value=="rk54"){
      this->setDefaultAlgorithm();
    } else if(this->current->value=="rkCastem"){
      this->reserveName("ra",false);
      this->reserveName("sqra",false);
      this->reserveName("errabs",false);
      this->reserveName("asig",false);
      this->reserveName("sigf",false);
      this->registerStaticVariable("rkcastem_div");
      this->registerStaticVariable("rkcastem_rmin");
      this->registerStaticVariable("rkcastem_rmax");
      this->registerStaticVariable("rkcastem_fac");
      this->registerStaticVariable("rkcastem_borne");
      this->mb.addStaticVariable(h,StaticVariableDescription("real","rkcastem_div",0u,7.));
      this->mb.addStaticVariable(h,StaticVariableDescription("real","rkcastem_rmin",0u,0.7));
      this->mb.addStaticVariable(h,StaticVariableDescription("real","rkcastem_rmax",0u,1.3));
      this->mb.addStaticVariable(h,StaticVariableDescription("real","rkcastem_fac",0u,3.));
      this->mb.addStaticVariable(h,StaticVariableDescription("real","rkcastem_borne",0u,2.));
      this->mb.setAttribute(h,MechanicalBehaviourData::algorithm,string("RungeKuttaCastem"));
      this->mb.setAttribute(h,MechanicalBehaviourData::numberOfEvaluations,ushort(5u));
    } else {
      this->throwRuntimeError("MFrontRungeKuttaParser::treatAlgorithm",
			      this->current->value+" is not a valid algorithm name"
			      "Supported algorithms are : 'euler', 'rk2',"
			      " 'rk4', 'rk42' , 'rk54' and 'rkCastem'");
    }
    ++this->current;
    this->readSpecifiedToken("MFrontRungeKuttaParser::treatAlgorithm",";");
  }

  std::string
  MFrontRungeKuttaParser::getName(void)
  {
    return "RungeKutta";
  }

  std::string
  MFrontRungeKuttaParser::getDescription(void)
  {
    return "this parser provides a generic integrator based on one of the many "
           "Runge-Kutta algorithm. Avalailable algorithms are 'euler', 'rk2', 'rk4' "
           "'r42', 'rk54' and 'rkCastem'";
  } // end of MFrontRungeKuttaParser::getDescription

  void
  MFrontRungeKuttaParser::endsInputFileProcessing(void)
  {
    using namespace std;
    MFrontBehaviourParserCommon::endsInputFileProcessing();
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    CodeBlock ib; // code inserted at the beginning of the local variable initialisation
    CodeBlock ie; // code inserted at the end of the local variable initialisation
    if(!this->mb.hasAttribute(h,MechanicalBehaviourData::algorithm)){
      this->setDefaultAlgorithm();
    }
    const string& algorithm =
      this->mb.getAttribute<string>(h,MechanicalBehaviourData::algorithm);
    const unsigned short n =
	  this->mb.getAttribute<unsigned short>(h,MechanicalBehaviourData::numberOfEvaluations);
    // registring the eel_ variable
    this->mb.addLocalVariable(h,VariableDescription("StrainStensor","eel_",1u,0u));
    ib.code += "this->eel_ = this->eel;\n";
    for(unsigned short i=0u;i!=n;++i){
      string currentVarName = "deel_K"+toString(static_cast<unsigned short>(i+1u));
      this->registerVariable(currentVarName,false);
      this->mb.addLocalVariable(h,VariableDescription("StrainStensor",currentVarName,1u,0u));
    }
    // some checks
    const set<Hypothesis>& bh = this->mb.getDistinctModellingHypotheses();
    for(set<Hypothesis>::const_iterator ph=bh.begin();ph!=bh.end();++ph){
      if(!this->mb.hasCode(*ph,MechanicalBehaviourData::ComputeStress)){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "@ComputeStress was not defined.";
	throw(runtime_error(msg));
      }
      if(!this->mb.hasCode(*ph,MechanicalBehaviourData::ComputeDerivative)){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "@Derivative was not defined.";
	throw(runtime_error(msg));
      }
    }
    // create the compute final stress code is necessary
    this->setComputeFinalStressFromComputeFinalStressCandidateIfNecessary();
    // declare the precision used by the algorithm
    if(!this->mb.hasParameter(h,"epsilon")){
      this->mb.addParameter(h,VariableDescription("real","epsilon",1u,0u));
      this->mb.setParameterDefaultValue(h,"epsilon",1.e-8);
    }
    // minimal time step
    if(this->mb.hasParameter(h,"dtmin")){
      ib.code += "if(this->dt<" + this->mb.getClassName() + "::dtmin){\n";
      ib.code += "this->dt=" + this->mb.getClassName() + "::dtmin;\n";
      ib.code += "}\n";
    } else {
      ib.code += "if(this->dt<100*numeric_limits<time>::min()){\n";
      ib.code += "string msg(\"" + this->mb.getClassName() + "::" + this->mb.getClassName() +"\");\n";
      ib.code += "msg += \"time step too small.\";\n";
      ib.code += "throw(runtime_error(msg));\n";
      ib.code += "}\n";
    }
    // driving variables
    if((algorithm!="RungeKutta4/2")&&
       (algorithm!="RungeKutta5/4")){
      for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
	const DrivingVariable& dv = pm->first;
	ib.code += "this->" + dv.name + "_ = this->" + dv.name + ";\n";
      }
    }
    // temperature
    if((algorithm!="RungeKutta4/2")&&
       (algorithm!="RungeKutta5/4")){
      ib.code += "this->T_ = this->T;\n";
    }
    this->mb.setCode(h,MechanicalBehaviourData::BeforeInitializeLocalVariables,ib,
		     MechanicalBehaviourData::CREATEORAPPEND,
		     MechanicalBehaviourData::AT_BEGINNING);
    // part introduced at the end of the initialize local variables
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(dv.increment_known){
	ie.code += "this->d" + dv.name + "_ = (this->d"+dv.name+")/(this->dt);\n";
      } else {
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
    }
    if((algorithm!="RungeKutta4/2")&&
       (algorithm!="RungeKutta5/4")){
      ie.code += "this->computeStress();\n";
    }
    this->mb.setCode(h,MechanicalBehaviourData::AfterInitializeLocalVariables,ie,
		     MechanicalBehaviourData::CREATEORAPPEND,
		     MechanicalBehaviourData::BODY);
    // minimal tangent operator if mandatory
    this->setMinimalTangentOperator();
  } // end of MFrontRungeKuttaParser::endsInputFileProcessing

  void
  MFrontRungeKuttaParser::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
  }

  void
  MFrontRungeKuttaParser::writeBehaviourParserSpecificMembers(const Hypothesis h)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "bool\ncomputeStress(void){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    this->writeStandardPerformanceProfiling(this->behaviourFile,
					    MechanicalBehaviourData::ComputeStress);
    this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputeStress).code << endl;
    this->behaviourFile << "return true;" << endl; 
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeStress" << endl << endl;
    this->behaviourFile << "bool\ncomputeFinalStress(void){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    writeMaterialLaws("MFrontRungeKuttaParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->mb.getMaterialLaws());		      
    this->writeStandardPerformanceProfiling(this->behaviourFile,
					    MechanicalBehaviourData::ComputeFinalStress);
    this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputeFinalStress).code << endl;
    this->behaviourFile << "return true;" << endl; 
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeFinalStress" << endl << endl;
    this->behaviourFile << "bool\ncomputeDerivative(void){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    this->writeStandardPerformanceProfiling(this->behaviourFile,
					    MechanicalBehaviourData::ComputeDerivative);
    writeMaterialLaws("MFrontRungeKuttaParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->mb.getMaterialLaws());		      
    this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputeDerivative).code << endl;
    this->behaviourFile << "return true;" << endl; 
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeDerivative" << endl << endl;
  } // end of writeBehaviourParserSpecificMembers

  void MFrontRungeKuttaParser::writeBehaviourUpdateStateVariables(const Hypothesis)
  {
    // Disabled (makes no sense for this parser)
  } // end of writeBehaviourUpdateStateVariables

  void
  MFrontRungeKuttaParser::writeBehaviourUpdateAuxiliaryStateVariables(const Hypothesis h) 
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    if(d.hasCode(MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Update auxiliary state variables at end of integration\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "updateAuxiliaryStateVariables(const real dt_)";
      this->behaviourFile << "{\n";
      this->behaviourFile << "static_cast<void>(dt_);\n";
      this->behaviourFile << "using namespace std;" << endl;
      this->behaviourFile << "using namespace tfel::math;" << endl;
      writeMaterialLaws("MFrontRungeKuttaParser::writeBehaviourUpdateAuxiliaryStateVariables",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->writeStandardPerformanceProfiling(this->behaviourFile,
					      MechanicalBehaviourData::UpdateAuxiliaryStateVariables);
      this->behaviourFile << d.getCode(MechanicalBehaviourData::UpdateAuxiliaryStateVariables).code << endl;
      this->behaviourFile << "}\n\n";
    }
  } // end of  MFrontRungeKuttaParser::writeBehaviourUpdateAuxiliaryStateVariables

  void MFrontRungeKuttaParser::writeBehaviourEulerIntegrator(const Hypothesis h)
  {
    using namespace std;
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << " += " 
			  << "this->dt*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(d.hasCode(MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);" << endl;
    }
  } // end of writeBehaviourEulerIntegrator

  void MFrontRungeKuttaParser::writeBehaviourRK2Integrator(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "// Compute K1's values" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (this->dt)*(this->d" << p->name << ");" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << "_K1);" << endl;
    }
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->"+dv.name+"_ += 0.5f*(this->d"+dv.name+");" << endl;
    }
    this->behaviourFile << "this->T_   += 0.5f*(this->dT);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeStress();" << endl << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    this->behaviourFile << "// Final Step" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << " += " 
			  << "this->dt*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(d.hasCode(MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);" << endl;
    }
  } // end of writeBehaviourRK2Integrator

  void MFrontRungeKuttaParser::writeBehaviourRK54Integrator(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    ErrorEvaluation eev;
    SupportedTypes::TypeSize svsize = this->getTotalSize(d.getStateVariables());
    if(svsize.getScalarSize()+svsize.getTVectorSize()+
       3u*svsize.getStensorSize()+3u*svsize.getTensorSize()>=20){
      eev = MAXIMUMVALUEERROREVALUATION;
    } else {
      eev = ERRORSUMMATIONEVALUATION;
    }
    SupportedTypes::TypeSize stateVarsSize = this->getTotalSize(d.getStateVariables());
    this->behaviourFile << "static const Type cste12_13       = Type(12)/Type(13);" << endl;
    this->behaviourFile << "static const Type cste1932_2197   = Type(1932)/Type(2197);" << endl;
    this->behaviourFile << "static const Type cste7200_2197   = Type(7200)/Type(2197);" << endl;
    this->behaviourFile << "static const Type cste7296_2197   = Type(7296)/Type(2197);" << endl;
    this->behaviourFile << "static const Type cste439_216     = Type(439)/Type(216);" << endl;
    this->behaviourFile << "static const Type cste3680_513    = Type(3680)/Type(513);" << endl;
    this->behaviourFile << "static const Type cste845_4104    = Type(845)/Type(4104);" << endl;
    this->behaviourFile << "static const Type cste8_27        = Type(8)/Type(27);" << endl;
    this->behaviourFile << "static const Type cste3544_2565   = Type(3544)/Type(2565);" << endl;
    this->behaviourFile << "static const Type cste1859_4104   = Type(1859)/Type(4104);" << endl;
    this->behaviourFile << "static const Type cste11_40       = Type(11)/Type(40);" << endl;
    this->behaviourFile << "static const Type cste16_135      = Type(16)/Type(135);" << endl;
    this->behaviourFile << "static const Type cste6656_12825  = Type(6656)/Type(12825);" << endl;
    this->behaviourFile << "static const Type cste28561_56430 = Type(28561)/Type(56430);" << endl;
    this->behaviourFile << "static const Type cste9_50        = Type(9)/Type(50);" << endl;
    this->behaviourFile << "static const Type cste2_55        = Type(2)/Type(55);" << endl;
    this->behaviourFile << "static const Type cste1_360       = Type(1)/Type(360);" << endl;
    this->behaviourFile << "static const Type cste128_4275    = Type(128)/Type(4275);" << endl;
    this->behaviourFile << "static const Type cste2197_75240  = Type(2197)/Type(75240);" << endl;
    this->behaviourFile << "static const Type cste1_50        = Type(1)/Type(50);" << endl;
    this->behaviourFile << "time t      = time(0);" << endl;
    this->behaviourFile << "time dt_    = this->dt;" << endl;
    this->behaviourFile << "time dtprec = 100*this->dt*numeric_limits<time>::epsilon();" << endl;
    this->behaviourFile << "Type error;" << endl;
    this->behaviourFile << "bool converged = false;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile << "while(!converged){" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : from \" << t <<  \" to \" << t+dt_ << \" with time step \" << dt_ << endl;\n";
    }
    this->behaviourFile << "bool failed = false;" << endl;
    this->behaviourFile << "// Compute K1's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->"+dv.name+"_ = this->"+dv.name+"+(this->d"+dv.name+")*(t/this->dt);\n";
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t/this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << "+(this->d" << p->name << ")*(t/this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
    }
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K1's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K1's derivative\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K2's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name 
			  << "+(this->d" << dv.name << ")*(t+0.25f*dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.25f*dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->d" << p->name << ")*(t+0.25f*dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.25f*(this->d" << p->name << "_K1);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K2's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K2's derivative\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K2 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K3's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name 
			  << "+(this->d" << dv.name << ")*(t+0.375f*dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.375f*dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->d" << p->name << ")*(t+0.375f*dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+0.09375f*(this->d" << p->name << "_K1+3*(this->d"
			  << p->name << "_K2));" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K3's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K3's derivative\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K3 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;

    this->behaviourFile << "if(!failed){" << endl;    
    this->behaviourFile << "// Compute K4's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name 
			  << "+(this->d" << dv.name << ")*(t+cste12_13*dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+cste12_13*dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->d" << p->name << ")*(t+cste12_13*dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+cste1932_2197*(this->d" << p->name << "_K1)"
			  << "-cste7200_2197*(this->d" << p->name << "_K2)"
			  << "+cste7296_2197*(this->d" << p->name << "_K3);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K4's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K4's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K4 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K5's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name 
			  << "+(this->d" << dv.name << ")*(t+dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->d" << p->name << ")*(t+dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+cste439_216*(this->d" << p->name << "_K1)"
			  << "-8*(this->d" << p->name << "_K2)"
			  << "+cste3680_513*(this->d" << p->name << "_K3)"
			  << "-cste845_4104*(this->d" << p->name << "_K4);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K5's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K5's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K5 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K6's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name 
			  << "+(this->d" << dv.name << ")*(t+0.5f*dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.5f*dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->d" << p->name << ")*(t+0.5f*dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "-cste8_27*(this->d" << p->name << "_K1)"
			  << "+2*(this->d" << p->name << "_K2)"
			  << "-cste3544_2565*(this->d" << p->name << "_K3)"
			  << "+cste1859_4104*(this->d" << p->name << "_K4)"
			  << "-cste11_40*(this->d" << p->name << "_K5);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K6's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K6's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K6 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Computing the error" << endl;
    if(eev==ERRORSUMMATIONEVALUATION){
      for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
	if(p->arraySize==1u){
	  if(p==d.getStateVariables().begin()){
	    this->behaviourFile << "error  = ";
	  } else {
	    this->behaviourFile << "error += ";
	  }
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << "(";
	  }
	  this->behaviourFile << "tfel::math::abs(";
	  this->behaviourFile << "cste1_360*(this->d" << p->name << "_K1)"
			      << "-cste128_4275*(this->d" << p->name << "_K3)"
			      << "-cste2197_75240*(this->d" << p->name << "_K4)"
			      << "+cste1_50*(this->d" << p->name << "_K5)"
			      << "+cste2_55*(this->d" << p->name << "_K6))";
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	  }
	  this->behaviourFile << ";"  << endl;
	} else {
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    if(p==d.getStateVariables().begin()){
	      this->behaviourFile << "error  = Type(0);" << endl;
	    }
	    this->behaviourFile << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){"  << endl;
	    this->behaviourFile << "error += ";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << "(";
	    }
	    this->behaviourFile << "tfel::math::abs(";
	    this->behaviourFile << "cste1_360*(this->d" << p->name       << "_K1[idx])"
				<< "-cste128_4275*(this->d" << p->name   << "_K3[idx])"
				<< "-cste2197_75240*(this->d" << p->name << "_K4[idx])"
				<< "+cste1_50*(this->d" << p->name       << "_K5[idx])"
				<< "+cste2_55*(this->d" << p->name       << "_K6[idx]))";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	    }
	    this->behaviourFile << ";"  << endl;
	    this->behaviourFile << "}" << endl;
	  } else {
	    for(unsigned short i=0;i!=p->arraySize;++i){
	      if((p==d.getStateVariables().begin())&&(i==0)){
		this->behaviourFile << "error  = ";
	      } else {
		this->behaviourFile << "error += ";
	      }
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << "(";
	      }
	      this->behaviourFile << "tfel::math::abs(";
	      this->behaviourFile << "cste1_360*(this->d" << p->name       << "_K1[" << i << "])"
				  << "-cste128_4275*(this->d" << p->name   << "_K3[" << i << "])"
				  << "-cste2197_75240*(this->d" << p->name << "_K4[" << i << "])"
				  << "+cste1_50*(this->d" << p->name       << "_K5[" << i << "])"
				  << "+cste2_55*(this->d" << p->name       << "_K6[" << i << "]))";
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	      }
	      this->behaviourFile << ";"  << endl;
	    }
	  }
	}
      }
      this->behaviourFile << "error/=" << stateVarsSize << ";" << endl;
    } else if(eev==MAXIMUMVALUEERROREVALUATION){
      this->behaviourFile << "error  = Type(0);" << endl;
      for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
	if(p->arraySize==1u){
	  this->behaviourFile << "error = std::max(error,";
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << "(";
	  }
	  this->behaviourFile << "tfel::math::abs(";
	  this->behaviourFile << "cste1_360*(this->d" << p->name << "_K1)"
			      << "-cste128_4275*(this->d" << p->name << "_K3)"
			      << "-cste2197_75240*(this->d" << p->name << "_K4)"
			      << "+cste1_50*(this->d" << p->name << "_K5)"
			      << "+cste2_55*(this->d" << p->name << "_K6))";
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	  }
	  this->behaviourFile << ");"  << endl;
	} else {
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    this->behaviourFile << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){"  << endl;
	    this->behaviourFile << "error = std::max(error,";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << "(";
	    }
	    this->behaviourFile << "tfel::math::abs(";
	    this->behaviourFile << "cste1_360*(this->d" << p->name       << "_K1[idx])"
				<< "-cste128_4275*(this->d" << p->name   << "_K3[idx])"
				<< "-cste2197_75240*(this->d" << p->name << "_K4[idx])"
				<< "+cste1_50*(this->d" << p->name       << "_K5[idx])"
				<< "+cste2_55*(this->d" << p->name       << "_K6[idx]))";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	    }
	    this->behaviourFile << ");"  << endl;
	    this->behaviourFile << "}" << endl;
	  } else {
	    for(unsigned short i=0;i!=p->arraySize;++i){
	      this->behaviourFile << "error  = std::max(error,";
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << "(";
	      }
	      this->behaviourFile << "tfel::math::abs(";
	      this->behaviourFile << "cste1_360*(this->d" << p->name       << "_K1[" << i << "])"
				  << "-cste128_4275*(this->d" << p->name   << "_K3[" << i << "])"
				  << "-cste2197_75240*(this->d" << p->name << "_K4[" << i << "])"
				  << "+cste1_50*(this->d" << p->name       << "_K5[" << i << "])"
				  << "+cste2_55*(this->d" << p->name       << "_K6[" << i << "]))";
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	      }
	      this->behaviourFile << ");"  << endl;
	    }
	  }
	}
      }
    } else {
      string msg("MFrontRungeKuttaParser::writeBehaviourRK54Integrator : ");
      msg += "internal error, unsupported error evaluation.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "if(::isnan(error)){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : nan decteted\");" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << msg << endl;" << endl;
    }
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : error \" << error << endl;"  << endl;
    }
    this->behaviourFile << "// test for convergence" << endl;
    this->behaviourFile << "if(error<this->epsilon){" << endl;
    this->behaviourFile << "// Final Step" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << " += cste16_135*(this->d" << p->name << "_K1)"
			  << "+cste6656_12825*(this->d" << p->name << "_K3)"
			  << "+cste28561_56430*(this->d" << p->name << "_K4)"
			  << "-cste9_50*(this->d" << p->name << "_K5)"
			  << "+cste2_55*(this->d" << p->name << "_K6);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(d.hasCode(MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(dt_);" << endl;
    }
    this->behaviourFile << "t += dt_;" << endl;
    this->behaviourFile << "if(abs(this->dt-t)<dtprec){" << endl;
    this->behaviourFile << "converged=true;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(!converged){" << endl;
    this->behaviourFile << "// time multiplier" << endl;
    this->behaviourFile << "real corrector;" << endl;
    this->behaviourFile << "if(error<100*std::numeric_limits<real>::min()){" << endl;
    this->behaviourFile << "corrector=real(10);" << endl;
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "corrector = 0.8*pow(this->epsilon/error,0.2);" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(corrector<real(0.1f)){" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : reducing time step by a factor 10\" << endl;" << endl;
    }
    this->behaviourFile << "dt_ *= real(0.1f);" << endl;
    this->behaviourFile << "} else if(corrector>real(10)){" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : increasing time step by a factor 10\" << endl;" << endl;
    }
    this->behaviourFile << "dt_ *= real(10);" << endl;
    this->behaviourFile << "} else {" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(corrector<1){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : reducing time step by a factor \"   << corrector << endl;" << endl;
      this->behaviourFile << "} else {" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : increasing time step by a factor \" << corrector << endl;" << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "dt_ *= corrector;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(dt_<dtprec){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : \");" << endl;
    this->behaviourFile << "msg += \"time step reduction has gone too far.\";" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){" << endl;
    this->behaviourFile << "dt_=this->dt-t;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "} else {" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failure detected, reducing time step by a factor 10\" << endl;";
    }
    this->behaviourFile << "// failed is true" << endl;
    this->behaviourFile << "dt_ *= real(0.1f);" << endl;
    this->behaviourFile << "if(dt_<dtprec){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : \");" << endl;
    this->behaviourFile << "msg += \"time step reduction has gone too far.\";" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
  } // end of MFrontRungeKuttaParser::writeBehaviourRK54Integrator

  void MFrontRungeKuttaParser::writeBehaviourRKCastemIntegrator(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize stateVarsSize;
    for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "static const Type cste1_6 = Type(1)/Type(6);" << endl; 
    this->behaviourFile << "time t   = time(0);" << endl;
    this->behaviourFile << "time dt_ = this->dt;" << endl;
    this->behaviourFile << "StressStensor sigf;" << endl;
    this->behaviourFile << "real errabs;" << endl;
    this->behaviourFile << "real asig;" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_  = this->" << dv.name << ";" << endl;
    }
    this->behaviourFile << "this->T_    = this->T;" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
    }
    this->behaviourFile << "bool failed;" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing initial stress\" << endl;" << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(failed){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : \");" << endl;
    this->behaviourFile << "msg += \"initial stress computations failed.\";" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "asig = sqrt((this->sig)|(this->sig));" << endl;
    this->behaviourFile << "if ((this->young)*Type(1.e-3)>asig){" << endl;
    this->behaviourFile << "  errabs = (this->young)*Type(1.e-3)*(this->epsilon);" << endl;
    this->behaviourFile << "}else{" << endl;
    this->behaviourFile << "  errabs = (this->epsilon)*asig;\n}\n" << endl;
    this->behaviourFile << "time dtprec = 100*this->dt*numeric_limits<time>::epsilon();" << endl;
    this->behaviourFile << "bool converged = false;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile << "while(!converged){" << endl;
    this->behaviourFile << "if(dt_< dtprec){" << endl; 
    this->behaviourFile << "cout<<\" dt \"<<this->dt<<\" t \"<<t<<\" dt_ \"<<dt_<<endl;" << endl;
    this->behaviourFile << "string msg(\"DDIF2::DDIF2\");" << endl;
    this->behaviourFile << "msg += \" time step too small \"; " << endl;
    this->behaviourFile << "throw(runtime_error(msg)); " << endl;
    this->behaviourFile << "} " << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : from \" << t <<  \" to \" << t+dt_ << \" with time step \" << dt_ << endl;\n";
    }
    this->behaviourFile << "// Compute K1's values => y in castem " << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_  = this->" << dv.name 
			  << "+(t/(this->dt))*(this->d" << dv.name << ");" << endl;
    }
    this->behaviourFile << "this->T_    = this->T+(t/(this->dt))*(this->dT);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name
			  << "+ (t/(this->dt))*(this->d" << p->name << ");" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
    }
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K1's stress\" << endl;" << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K1's derivatives\" << endl;" << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute y'1*dt=f(y)*dt in castem" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K2's values => y1 in castem" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name 
			  << "+(this->d" << dv.name << ")*(t+0.5f*dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.5f*dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name   << "_ = this->" << p->name 
			  <<"+(this->d" << p->name << ")*(t+0.5f*dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << "_K1);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K2's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K2's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute y'2*dt=f(y1)*dt in castem" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K2 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "// Compute K3's values => y12 in castem" << endl;
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+0.25f*(this->d" << p->name << "_K1 + this->d" << p->name << "_K2);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K3's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K3's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute y'3*dt=f(y12)*dt in castem" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K3 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K4's values => y13 = y12+y'3*dt/2 in castem" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name 
			  << "+(this->d" << dv.name << ")*(t+dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  <<"+(this->d" << p->name 
			  << ")*(t+dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ += 0.5f*(this->d" << p->name << "_K3);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K4's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K4's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute y'4*dt=f(y13)*dt in castem" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K4 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K5's values => yf = y12+0.5*(y'3+y'4)*dt/2 in castem" << endl;
    this->behaviourFile << "//                     => yf = y+0.5*(y'1+y'2+y'3+y'4)*dt/2 in castem" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+0.25f*(this->d" << p->name << "_K1 + this->d" << p->name 
			  << "_K2 + this->d" << p->name << "_K3 + this->d" << p->name << "_K4);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K5's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "// Saving stresses obtained with yf" << endl;
    this->behaviourFile << "sigf=this->sig;" << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K5's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute y'5*dt=f(yf)*dt in castem" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K5 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K6's values => y5 = y+1/6*(y'1+4*y'3+y'5)*dt in castem" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+cste1_6*(this->d" << p->name << "_K1 + Type(4)*this->d" << p->name 
			  << "_K3 + this->d" << p->name << "_K5);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing criterium stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "real ra;" << endl;
    this->behaviourFile << "real sqra;" << endl;
    this->behaviourFile << "// Computing the error" << endl;

    this->behaviourFile << "ra = sqrt(((sigf)-(this->sig))|((sigf)-(this->sig)))/errabs;" << endl;
    this->behaviourFile << "sqra = sqrt(ra);" << endl;
    this->behaviourFile << "// test for convergence" << endl;
    this->behaviourFile << "if ((sqra>"  << this->mb.getClassName() << "::rkcastem_div)||(::isnan(ra))){" << endl;
    this->behaviourFile << "dt_ /= "  << this->mb.getClassName() << "::rkcastem_div;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : multiplying time step by a factor \" << 1/("
			  << this->mb.getClassName() << "::rkcastem_div) << endl;" << endl;
    }
    this->behaviourFile << "} else if (ra> " << this->mb.getClassName() << "::rkcastem_borne){" << endl;
    this->behaviourFile << "dt_ /= sqra;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : multiplying time step by a factor \" << 1/sqra << endl;" << endl;
    }
    this->behaviourFile << "}else{" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << " += 0.25f*(this->d" << p->name << "_K1 + this->d" << p->name 
			  << "_K2 + this->d" << p->name << "_K3 + this->d" << p->name << "_K4);" << endl;
    }
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(this->mb.hasCode(h,MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(dt_);" << endl;
    }
    this->behaviourFile << "t += dt_;" << endl;
    this->behaviourFile << "if(abs(this->dt-t)<dtprec){" << endl;
    this->behaviourFile << "converged=true;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(!converged){" << endl;
    this->behaviourFile << "if (("  << this->mb.getClassName() << "::rkcastem_fac)*sqra<1.){" << endl;
    this->behaviourFile << "dt_ *= " << this->mb.getClassName() << "::rkcastem_fac;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : multiplying time step by a factor \" << "
			  << this->mb.getClassName() << "::rkcastem_fac << endl;" << endl;
    }
    this->behaviourFile << "}else if ((sqra< "<< this->mb.getClassName() << "::rkcastem_rmin)||" <<
      "(sqra>" << this->mb.getClassName() << "::rkcastem_rmax)){" << endl;
    this->behaviourFile << "dt_ /= sqra;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : multiplying time step by a factor \" << 1/sqra << endl;" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "} else { " << endl;
    this->behaviourFile << "dt_ /=  " << this->mb.getClassName() << "::rkcastem_div;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : multiplying time step by a factor \" << 1/("
			  << this->mb.getClassName() << "::rkcastem_fac) << endl;" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(dt_<dtprec){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : \");" << endl;
    this->behaviourFile << "msg += \"time step reduction has gone too far.\";" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(!converged){" << endl;
    this->behaviourFile << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){" << endl;
    this->behaviourFile << "dt_=this->dt-t;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
  } // end of MFrontRungeKuttaParser::writeBehaviourRKCastemIntegrator

  void MFrontRungeKuttaParser::writeBehaviourRK42Integrator(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    ErrorEvaluation eev;
    SupportedTypes::TypeSize svsize = this->getTotalSize(d.getStateVariables());
    if(svsize.getScalarSize()+svsize.getTVectorSize()+
       3u*svsize.getStensorSize()+3u*svsize.getTensorSize()>=20){
      eev = MAXIMUMVALUEERROREVALUATION;
    } else {
      eev = ERRORSUMMATIONEVALUATION;
    }
    SupportedTypes::TypeSize stateVarsSize;
    for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "static const Type cste1_6 = Type(1)/Type(6);" << endl;
    this->behaviourFile << "static const Type cste1_3 = Type(1)/Type(3);" << endl;

    this->behaviourFile << "time t   = time(0);" << endl;
    this->behaviourFile << "time dt_ = this->dt;" << endl;
    this->behaviourFile << "time dtprec = 100*this->dt*numeric_limits<time>::epsilon();" << endl;
    this->behaviourFile << "Type error;" << endl;
    this->behaviourFile << "bool converged = false;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << endl << \"" << this->mb.getClassName()
			  << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile << "while(!converged){" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : from \" << t <<  \" to \" << t+dt_ << \" with time step \" << dt_ << endl;\n";
    }
    this->behaviourFile << "bool failed = false;" << endl;
    this->behaviourFile << "// Compute K1's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_  = this->" << dv.name
			  << "+(t/(this->dt))*(this->d" << dv.name << ");" << endl;
    }
    this->behaviourFile << "this->T_    = this->T+(t/(this->dt))*(this->dT);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name
			  << "+ (t/(this->dt))*(this->d" << p->name << ");" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
    }
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K1's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K1's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K2's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name
			  << "+(this->d" << dv.name << ")*(t+0.5f*dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.5f*dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  <<"+(this->d" << p->name 
			  << ")*(t+0.5f*dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << "_K1);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K2's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K2's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K2 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "// Compute K3's values" << endl;
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+0.5f*(this->d" << p->name << "_K2);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K3's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K3's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K3 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Compute K4's values" << endl;
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ = this->" << dv.name
			  << "+(this->d" << dv.name << ")*(t+dt_)/(this->dt);" << endl;
    }
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+dt_)/(this->dt);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  <<"+(this->d" << p->name 
			  << ")*(t+dt_)/(this->dt);" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+this->d" << p->name << "_K3;" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K4's stress\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "failed = !this->computeDerivative();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing K4's derivatives\" << endl;"  << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(!failed){" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K4 = (dt_)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl << endl;
    this->behaviourFile << "if(!failed){" << endl;
    this->behaviourFile << "// Computing the error" << endl;
    if(eev==ERRORSUMMATIONEVALUATION){
      for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
	if(p->arraySize==1u){
	  if(p==d.getStateVariables().begin()){
	    this->behaviourFile << "error  = ";
	  } else {
	    this->behaviourFile << "error += ";
	  }
	  this->behaviourFile << "tfel::math::abs(";
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << "(";
	  }
	  this->behaviourFile << "cste1_6*(this->d" << p->name << "_K1+"
			      << "this->d" << p->name << "_K4-"
			      << "this->d" << p->name << "_K2-"
			      << "this->d" << p->name << "_K3))";
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	  }
	  this->behaviourFile << ";" << endl;
	} else {
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    if(p==d.getStateVariables().begin()){
	      this->behaviourFile << "error  = Type(0);" << endl;
	    }
	    this->behaviourFile << "for(unsigned short idx=0;idx!=" <<p->arraySize << ";++idx){" << endl;
	    this->behaviourFile << "error += tfel::math::abs(";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << "(";
	    }
	    this->behaviourFile << "cste1_6*(this->d" << p->name << "_K1[idx]+"
				<< "this->d" << p->name          << "_K4[idx]-"
				<< "this->d" << p->name          << "_K2[idx]-"
				<< "this->d" << p->name          << "_K3[idx]))";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	    }
	    this->behaviourFile << ";" << endl;
	    this->behaviourFile << "}" << endl;
	  } else {
	    for(unsigned short i=0;i!=p->arraySize;++i){
	      if((p==d.getStateVariables().begin())&&(i==0)){
		this->behaviourFile << "error  = ";
	      } else {
		this->behaviourFile << "error += ";
	      }
	      this->behaviourFile << "tfel::math::abs(";
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << "(";
	      }
	      this->behaviourFile << "cste1_6*(this->d" << p->name << "_K1[" << i << "]+"
				  << "this->d" << p->name          << "_K4[" << i << "]-"
				  << "this->d" << p->name          << "_K2[" << i << "]-"
				  << "this->d" << p->name          << "_K3[" << i << "]))";
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	      }
	      this->behaviourFile << ";" << endl;
	    }
	  }
	}
      }
      this->behaviourFile << "error/=" << stateVarsSize << ";" << endl;
    } else if(eev==MAXIMUMVALUEERROREVALUATION){
      this->behaviourFile << "error  = Type(0);" << endl;
      for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
	if(p->arraySize==1u){
	  this->behaviourFile << "error = std::max(error,tfel::math::abs(";
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << "(";
	  }
	  this->behaviourFile << "cste1_6*(this->d" << p->name << "_K1+"
			      << "this->d" << p->name << "_K4-"
			      << "this->d" << p->name << "_K2-"
			      << "this->d" << p->name << "_K3))";
	  if(enf.find(p->name)!=enf.end()){
	    this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	  }
	  this->behaviourFile << ");" << endl;
	} else {
	  if(this->useDynamicallyAllocatedVector(p->arraySize)){
	    this->behaviourFile << "for(unsigned short idx=0;idx!=" <<p->arraySize << ";++idx){" << endl;
	    this->behaviourFile << "error = std::max(error,tfel::math::abs(";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << "(";
	    }
	    this->behaviourFile << "cste1_6*(this->d" << p->name << "_K1[idx]+"
				<< "this->d" << p->name          << "_K4[idx]-"
				<< "this->d" << p->name          << "_K2[idx]-"
				<< "this->d" << p->name          << "_K3[idx]))";
	    if(enf.find(p->name)!=enf.end()){
	      this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	    }
	    this->behaviourFile << ");" << endl;
	    this->behaviourFile << "}" << endl;
	  } else {
	    for(unsigned short i=0;i!=p->arraySize;++i){
	      this->behaviourFile << "error = std::max(error,tfel::math::abs(";
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << "(";
	      }
	      this->behaviourFile << "cste1_6*(this->d" << p->name << "_K1[" << i << "]+"
				  << "this->d" << p->name          << "_K4[" << i << "]-"
				  << "this->d" << p->name          << "_K2[" << i << "]-"
				  << "this->d" << p->name          << "_K3[" << i << "]))";
	      if(enf.find(p->name)!=enf.end()){
		this->behaviourFile << ")/(" << enf.find(p->name)->second << ")";
	      }
	      this->behaviourFile << ");" << endl;
	    }
	  }
	}
      }
    } else {
      string msg("MFrontRungeKuttaParser::writeBehaviourRK42Integrator : ");
      msg += "internal error, unsupported error evaluation.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "if(::isnan(error)){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : nan decteted\");" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << msg << endl;" << endl;
    }
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : error \" << error << endl;"  << endl;
    }
    this->behaviourFile << "// test for convergence" << endl;
    this->behaviourFile << "if(error<this->epsilon){" << endl;
    this->behaviourFile << "// Final Step" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name 
			  << " += cste1_6*(this->d" << p->name << "_K1 + this->d" << p->name << "_K4)+"
			  << "    cste1_3*(this->d" << p->name << "_K3 + this->d" << p->name << "_K2);" << endl;
    }
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(this->mb.hasCode(h,MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(dt_);" << endl;
    }
    this->behaviourFile << "t += dt_;" << endl;
    this->behaviourFile << "if(abs(this->dt-t)<dtprec){" << endl;
    this->behaviourFile << "converged=true;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(!converged){";
    this->behaviourFile << "// time multiplier" << endl;
    this->behaviourFile << "real corrector;" << endl;
    this->behaviourFile << "if(error<100*std::numeric_limits<real>::min()){" << endl;
    this->behaviourFile << "corrector=real(10.);" << endl;
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "corrector = 0.8*pow((this->epsilon)/error,1./3.);" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(corrector<=real(0.1f)){" << endl;
    this->behaviourFile << "dt_ *= real(0.1f);" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : reducing time step by a factor 10\" << endl;" << endl;
    }
    this->behaviourFile << "} else if(corrector>real(10)){" << endl;
    this->behaviourFile << "dt_ *= real(10);" << endl;
    if(getDebugMode()){
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : multiplying time step by a factor 10\" << endl;" << endl;
    }
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "dt_ *= corrector;" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(corrector<1){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : reducing time step by a factor \"   << corrector << endl;" << endl;
      this->behaviourFile << "} else {" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : increasing time step by a factor \" << corrector << endl;" << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(dt_<dtprec){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : \");" << endl;
    this->behaviourFile << "msg += \"time step reduction has gone too far.\";" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){" << endl;
    this->behaviourFile << "dt_=this->dt-t;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "// failed is true" << endl;
    this->behaviourFile << "dt_ *= real(0.1f);" << endl;
    this->behaviourFile << "if(dt_<dtprec){" << endl;
    this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::integrate : \");" << endl;
    this->behaviourFile << "msg += \"time step reduction has gone too far.\";" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
  } // end of MFrontRungeKuttaParser::writeBehaviourRK42Integrator

  void MFrontRungeKuttaParser::writeBehaviourRK4Integrator(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    this->behaviourFile << "// Compute K1's values" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (this->dt)*(this->d" << p->name << ");" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << "_K1);" << endl;
    }
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ += 0.5f*(this->d" << dv.name << ");" << endl;
    }
    this->behaviourFile << "this->T_   += 0.5f*(this->dT);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeStress();" << endl << endl;
    this->behaviourFile << "// Compute K2's values" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K2 = (this->dt)*(this->d" << p->name << ");" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = " << "this->" << p->name 
			  << "+ 0.5f*(this->d" << p->name << "_K2);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeStress();" << endl << endl;
    this->behaviourFile << "// Compute K3's values" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K3 = (this->dt)*(this->d" << p->name << ");" << endl;
    }
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ = " << "this->" << p->name 
			  << "+ (this->d" << p->name << "_K3);" << endl;
    }
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(!dv.increment_known){
	string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      this->behaviourFile << "this->" << dv.name << "_ += 0.5f*(this->d" << dv.name << ");" << endl;
    }
    this->behaviourFile << "this->T_   += 0.5f*(this->dT);" << endl;
    for(p =d.getExternalStateVariables().begin();p!=d.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeStress();" << endl << endl;
    this->behaviourFile << "// Compute K4's values" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K4 = (this->dt)*(this->d" << p->name << ");" << endl;
    }
    this->behaviourFile << "// Final Step" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << " += " 
			  << "1.f/6.f*(this->d" << p->name 
			  << "_K1+this->d" << p->name << "_K4)+" << endl;
      this->behaviourFile << "1.f/3.f*(this->d" << p->name 
			  << "_K2+this->d" << p->name << "_K3);" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(this->mb.hasCode(h,MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);" << endl;
    }
  }  // end of MFrontRungeKuttaParser::writeBehaviourRK4Integrator

  void MFrontRungeKuttaParser::writeBehaviourIntegrator(const Hypothesis h)
  {
    using namespace std;
    const string btype = this->convertBehaviourTypeToString();
    const string& algorithm = this->mb.getAttribute<string>(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
							    MechanicalBehaviourData::algorithm);
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    std::vector<BoundsDescription>::const_iterator p2;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "IntegrationResult" << endl;
    if(this->mb.hasAttribute(h,MechanicalBehaviourData::hasConsistentTangentOperator)){
      this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt){" << endl;
    } else {
      if((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL)){
	this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt){" << endl;
      } else {
	this->behaviourFile << "integrate(const SMFlag,const SMType smt){" << endl;
      }
    }
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
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
    if(algorithm == "Euler"){
      this->writeBehaviourEulerIntegrator(h);
    } else if(algorithm == "RungeKutta2"){
      this->writeBehaviourRK2Integrator(h);
    } else if(algorithm == "RungeKutta4/2"){
      this->writeBehaviourRK42Integrator(h);
    } else if(algorithm == "RungeKutta5/4"){
      this->writeBehaviourRK54Integrator(h);
    } else if(algorithm == "RungeKuttaCastem"){
      this->writeBehaviourRKCastemIntegrator(h);
    } else if (algorithm == "RungeKutta4"){
      this->writeBehaviourRK4Integrator(h);
    } else {
      string msg("MFrontRungeKuttaParser::writeBehaviourIntegrator : ");
      msg += "internal error\n";
      msg += algorithm;
      msg += " is not a known algorithm. This shall not happen at this stage.";
      msg += " Please contact MFront developper to help them debug this.";
      throw(runtime_error(msg));
    }
    for(p2  = d.getBounds().begin();
	p2 != d.getBounds().end();++p2){
      if(p2->varCategory==BoundsDescription::StateVariable){
	p2->writeBoundsChecks(this->behaviourFile);
      }
    }
    this->writeStandardPerformanceProfilingEnd(this->behaviourFile);
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
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::integrate" << endl << endl;
  } // end of void MFrontRungeKuttaParser::writeBehaviourIntegrator(void)
  
  MFrontRungeKuttaParser::~MFrontRungeKuttaParser()
  {} // end of ~MFrontRungeKuttaParser

} // end of namespace mfront  
