/*!
 * \file   mfront/src/RungeKuttaDSL.cxx
 * \brief
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include<string>
#include<sstream>
#include<stdexcept>

#include"MFront/DSLUtilities.hxx"
#include"MFront/DSLFactory.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/PerformanceProfiling.hxx"
#include"MFront/RungeKuttaDSL.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99)
#include<sstream>
namespace std{
  template<typename T>
  std::string to_string(const T& v){
    std::ostringstream s;
    s << v;
    return s.str();
  }
}
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront{

  static void
  writeExternalVariablesCurrentValues(std::ostream& f,
				      const BehaviourDescription& mb,
				      const RungeKuttaDSL::Hypothesis h,
				      const std::string& p)
  {
    using namespace std;
    const string t = ((p=="0") ? "(t/this->dt)" :
		      ((p=="1") ? "((t+dt_)/this->dt)" : "((t+"+p+"*dt_)/this->dt)"));
    const auto& d = mb.getBehaviourData(h);
    //! all registred variables used in this block
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
    const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
    uvs.insert(uvs2.begin(),uvs2.end());
    for(const auto& mv : mb.getMainVariables()){
      const auto& dv = mv.first;
      if(!dv.increment_known){
	string msg("writeExternalVariablesCurrentValues : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      if(uvs.find(dv.name)!=uvs.end()){
	f << "this->"+dv.name+"_ = this->"+dv.name+"+(this->d"+dv.name+")*" << t << ";\n";
      }
    }
    if(uvs.find("T")!=uvs.end()){
      f << "this->T_   = this->T+(this->dT)*" << t << ";" << endl;
    }
    for(const auto& v : d.getExternalStateVariables()){
      if(uvs.find(v.name)!=uvs.end()){
	f << "this->" << v.name << "_ = this->" << v.name
	  << "+(this->d" << v.name << ")*" << t << ";" << endl;
      }
    }
  }

  static void
  writeExternalVariableCurrentValue2(std::ostream& f,
				     const std::string& n,
				     const std::string& p)
  {
    if(p=="0"){
      f << "this->"  << n  << "_ = this->"  << n  << ";\n";
    } else if(p=="1"){
      f << "this->"  << n  << "_ = this->"  << n  << "+this->d" << n << ";\n";
    } else {
      f << "this->"  << n  << "_ = this->"  << n  << "+" << p << "*(this->d" << n << ");\n";
    }
  }

  static void
  writeExternalVariablesCurrentValues2(std::ostream& f,
				       const BehaviourDescription& mb,
				       const RungeKuttaDSL::Hypothesis h,
				       const std::string& p)
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    //! all registred variables used in this block
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
    const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
    uvs.insert(uvs2.begin(),uvs2.end());
    for(const auto& mv : mb.getMainVariables()){
      const auto& dv = mv.first;
      if(!dv.increment_known){
	string msg("writeExternalVariablesCurrentValues2 : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
      if(uvs.find(dv.name)!=uvs.end()){
	writeExternalVariableCurrentValue2(f,dv.name,p);
      }
    }
    if(uvs.find("T")!=uvs.end()){
      writeExternalVariableCurrentValue2(f,"T",p);
    }
    for(const auto& v : d.getExternalStateVariables()){
      if(uvs.find(v.name)!=uvs.end()){
	writeExternalVariableCurrentValue2(f,v.name,p);
      }
    }
  } // end of writeExternalVariablesCurrentValues2

  RungeKuttaDSL::RungeKuttaDSL()
    : BehaviourDSLBase<RungeKuttaDSL>()
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
      const auto& dv = pm->first;
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
    this->reserveName("cste1_2",false);
    this->reserveName("cste1_4",false);
    this->reserveName("cste3_8",false);
    this->reserveName("cste3_32",false);
    this->reserveName("cste12_13",false);
    this->reserveName("cste1932_2197",false);
    this->reserveName("cste7200_2197",false);
    this->reserveName("cste7296_2197",false);
    this->reserveName("cste439_216",false);
    this->reserveName("cste3680_513",false);
    this->reserveName("cste845_4104",false);
    this->reserveName("cste8_27",false);
    this->reserveName("cste3544_2565",false);
    this->reserveName("cste1859_4104",false);
    this->reserveName("cste11_40",false);
    this->reserveName("cste16_135",false);
    this->reserveName("cste6656_12825",false);
    this->reserveName("cste28561_56430",false);
    this->reserveName("cste9_50",false);
    this->reserveName("cste2_55",false);
    this->reserveName("cste1_360",false);
    this->reserveName("cste128_4275",false);
    this->reserveName("cste2197_75240",false);
    this->reserveName("cste1_50",false);
    this->mb.addLocalVariable(h,VariableDescription("temperature","T_",1u,0u));
    this->mb.addStateVariable(h,VariableDescription("StrainStensor","eel",1u,0u));
    this->mb.setGlossaryName(h,"eel","ElasticStrain");
    // CallBacks
    this->registerNewCallBack("@UsableInPurelyImplicitResolution",
			      &RungeKuttaDSL::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",&RungeKuttaDSL::treatMaterialLaw);
    this->registerNewCallBack("@Algorithm",&RungeKuttaDSL::treatAlgorithm);
    this->registerNewCallBack("@TangentOperator",
    			      &RungeKuttaDSL::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
    			      &RungeKuttaDSL::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@Derivative",&RungeKuttaDSL::treatDerivative);
    this->registerNewCallBack("@ComputeStress",&RungeKuttaDSL::treatComputeStress);
    this->registerNewCallBack("@Epsilon",&RungeKuttaDSL::treatEpsilon);
    this->registerNewCallBack("@MinimalTimeStep",
			      &RungeKuttaDSL::treatMinimalTimeStep);
    this->registerNewCallBack("@RequireStiffnessTensor",
			      &RungeKuttaDSL::treatRequireStiffnessOperator);
    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
    this->mb.setIntegrationScheme(BehaviourDescription::EXPLICITSCHEME);
  }

  void RungeKuttaDSL::writeBehaviourParserSpecificIncludes(void)
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

  void
  RungeKuttaDSL::treatUpdateAuxiliaryStateVariables(void)
  {
    this->readCodeBlock(*this,BehaviourData::UpdateAuxiliaryStateVariables,
			&RungeKuttaDSL::standardModifier,true,true);
  } // end of RungeKuttaDSL::treatUpdateAuxiliaryStateVarBase

  std::string
  RungeKuttaDSL::computeStressVariableModifier1(const Hypothesis h,
						const std::string& var,
						const bool addThisPtr)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
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
      const auto& v = d.getExternalStateVariables().getVariable(var.substr(1));
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
  } // end of RungeKuttaDSL::computeStressVariableModifier1

  std::string
  RungeKuttaDSL::computeStressVariableModifier2(const Hypothesis h,
							 const std::string& var,
							 const bool addThisPtr)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
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
  } // end of RungeKuttaDSL::computeStressVariableModifier2

  void
  RungeKuttaDSL::treatComputeStress(void)
  {
    this->readCodeBlock(*this,BehaviourData::ComputeStress,
			BehaviourData::ComputeFinalStress,
			&RungeKuttaDSL::computeStressVariableModifier1,
			&RungeKuttaDSL::computeStressVariableModifier2,true,true);
  } // end of RungeKuttaDSL::treatComputeStress

  void RungeKuttaDSL::treatUnknownVariableMethod(const Hypothesis h,
							  const std::string& n)
  {
    using namespace std;
    if(this->mb.isIntegrationVariableName(h,n)){
      if(this->current->value=="setErrorNormalisationFactor"){
    	string var;
    	++(this->current);
    	this->checkNotEndOfFile("RungeKuttaDSL::treatUnknowVariableMethod");
    	this->readSpecifiedToken("RungeKuttaDSL::treatUnknowVariableMethod","(");
    	this->checkNotEndOfFile("RungeKuttaDSL::treatUnknowVariableMethod");
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
    	    this->throwRuntimeError("RungeKuttaDSL::treatUnknowVariableMethod",
    				    "Failed to read error normalisation factor.");
    	  }
    	  if(value<0.){
    	    this->throwRuntimeError("RungeKuttaDSL::treatUnknowVariableMethod",
    				    "invalid error normalisation factor.");
    	  }
    	}
    	if(!this->enf.insert(make_pair(n,var)).second){
    	  this->throwRuntimeError("RungeKuttaDSL::treatUnknowVariableMethod",
    				  "Error normalisation factor already defined for variable '"+n+"'.");
    	}
    	++(this->current);
    	this->checkNotEndOfFile("RungeKuttaDSL::treatUnknowVariableMethod");
    	return;
      }
    }
    BehaviourDSLCommon::treatUnknownVariableMethod(h,n);
  } // end of RungeKuttaDSL::treatUnknowVariableMethod

  void RungeKuttaDSL::treatDerivative(void)
  {
    this->readCodeBlock(*this,BehaviourData::ComputeDerivative,
			&RungeKuttaDSL::computeStressVariableModifier1,true,true);
  } // end of RungeKuttaDSL::treatDerivative

  void
  RungeKuttaDSL::treatEpsilon(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(this->mb.hasParameter(h,"epsilon")){
      this->throwRuntimeError("RungeKuttaDSL::treatEpsilon",
			      "value already specified.");
    }
    double epsilon;
    this->checkNotEndOfFile("RungeKuttaDSL::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> epsilon;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("RungeKuttaDSL::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(epsilon<0){
      this->throwRuntimeError("RungeKuttaDSL::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("RungeKuttaDSL::treatEpsilon",";");
    this->mb.addParameter(h,VariableDescription("real","epsilon",1u,0u));
    this->mb.setParameterDefaultValue(h,"epsilon",epsilon);
  } // end of RungeKuttaDSL::treatEpsilon

  void
  RungeKuttaDSL::treatMinimalTimeStep(void)
  {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(this->mb.hasParameter(h,"dtmin")){
      this->throwRuntimeError("RungeKuttaDSL::treatMinimalTimeStep",
			      "value already specified.");
    }
    double dtmin;
    this->checkNotEndOfFile("RungeKuttaDSL::treatMinimalTimeStep",
			    "Cannot read dtmin value.");
    istringstream flux(current->value);
    flux >> dtmin;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("RungeKuttaDSL::treatMinimalTimeStep",
			      "Failed to read dtmin value.");
    }
    if(dtmin<0){
      this->throwRuntimeError("RungeKuttaDSL::treatMinimalTimeStep",
			      "MinimalTimeStep value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("RungeKuttaDSL::treatMinimalTimeStep",";");
    this->mb.addParameter(h,VariableDescription("real","dtmin",1u,0u));
    this->mb.setParameterDefaultValue(h,"dtmin",dtmin);
  } // end of RungeKuttaDSL::treatEpsilon

  void RungeKuttaDSL::setDefaultAlgorithm(void)
  {
    using namespace std;
    typedef unsigned short ushort;
    this->mb.setAttribute(BehaviourData::algorithm,
			  string("RungeKutta5/4"),false);
    this->mb.setAttribute(BehaviourData::numberOfEvaluations,
			  ushort(6u),false);
  } // end of RungeKuttaDSL::setDefaultAlgorithm

  void RungeKuttaDSL::treatAlgorithm(void)
  {
    using namespace std;
    typedef unsigned short ushort;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("RungeKuttaDSL::treatAlgorithm",
			    "Cannot read algorithm name.");
    if(this->mb.hasAttribute(BehaviourData::algorithm)){
      this->throwRuntimeError("RungeKuttaDSL::treatAlgorithm",
			      "algorith already specified. This may be the second "
			      "time that the @Algorithm keyword is used, or the default "
			      "algorithm was selected when registring a state variable "
			      "(keyword @StateVariable)");
    }
    if(this->current->value=="euler"){
      this->mb.setAttribute(BehaviourData::algorithm,string("Euler"),false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations,ushort(0u),false);
    } else if(this->current->value=="rk2"){
      this->mb.setAttribute(BehaviourData::algorithm,string("RungeKutta2"),false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations,ushort(1u),false);
    } else if(this->current->value=="rk4"){
      this->mb.setAttribute(BehaviourData::algorithm,string("RungeKutta4"),false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations,ushort(4u),false);
    } else if(this->current->value=="rk42"){
      this->mb.setAttribute(BehaviourData::algorithm,string("RungeKutta4/2"),false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations,ushort(4u),false);
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
      this->mb.setAttribute(BehaviourData::algorithm,string("RungeKuttaCastem"),false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations,ushort(5u),false);
    } else {
      this->throwRuntimeError("RungeKuttaDSL::treatAlgorithm",
			      this->current->value+" is not a valid algorithm name"
			      "Supported algorithms are : 'euler', 'rk2',"
			      " 'rk4', 'rk42' , 'rk54' and 'rkCastem'");
    }
    ++this->current;
    this->readSpecifiedToken("RungeKuttaDSL::treatAlgorithm",";");
  }

  std::string
  RungeKuttaDSL::getName(void)
  {
    return "RungeKutta";
  }

  std::string
  RungeKuttaDSL::getDescription(void)
  {
    return "this parser provides a generic integrator based on one of the many "
           "Runge-Kutta algorithm. Avalailable algorithms are 'euler', 'rk2', 'rk4' "
           "'r42', 'rk54' and 'rkCastem'";
  } // end of RungeKuttaDSL::getDescription

  void
  RungeKuttaDSL::endsInputFileProcessing(void)
  {
    using namespace std;
    BehaviourDSLCommon::endsInputFileProcessing();
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    CodeBlock ib; // code inserted at before of the local variable initialisation
    CodeBlock ie; // code inserted at the end of the local variable initialisation
    if(!this->mb.hasAttribute(BehaviourData::algorithm)){
      this->setDefaultAlgorithm();
    }
    const string& algorithm =
      this->mb.getAttribute<string>(BehaviourData::algorithm);
    const unsigned short n =
	  this->mb.getAttribute<unsigned short>(BehaviourData::numberOfEvaluations);
    // some checks
    const auto& bh = this->mb.getDistinctModellingHypotheses();
    for(const auto & elem : bh){
      const auto& d = this->mb.getBehaviourData(elem);
      if(!d.hasCode(BehaviourData::ComputeStress)){
	string msg("RungeKuttaDSL::endsInputFileProcessing : ");
	msg += "@ComputeStress was not defined.";
	throw(runtime_error(msg));
      }
      if(!d.hasCode(BehaviourData::ComputeDerivative)){
	string msg("RungeKuttaDSL::endsInputFileProcessing : ");
	msg += "@Derivative was not defined.";
	throw(runtime_error(msg));
      }
      auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
      const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
      uvs.insert(uvs2.begin(),uvs2.end());
      CodeBlock icb; // code inserted at the beginning of the local variable initialisation
      // creating local variables
      const auto& ivs = d.getStateVariables();
      const auto& evs = d.getExternalStateVariables();
      for(const auto& iv:ivs){
	for(unsigned short i=0u;i!=n;++i){
	  string currentVarName = "d" + iv.name + "_K"+to_string(i+1u);
	  if(getVerboseMode()>=VERBOSE_DEBUG){
	    auto& log = getLogStream();
	    log << "registring variable '" << currentVarName << "'";
	    if(elem==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	      log << " for default hypothesis" << endl;
	    } else {
	      log << " for the '" << ModellingHypothesis::toString(elem)
		  << "' hypothesis" << endl;
	    }
	  }
	  this->registerVariable(currentVarName,false);
	  this->mb.addLocalVariable(elem,VariableDescription(iv.type,currentVarName,iv.arraySize,0u));
	}
	if(uvs.find(iv.name)!=uvs.end()){
	  string currentVarName = iv.name + "_";
	  if(getVerboseMode()>=VERBOSE_DEBUG){
	    auto& log = getLogStream();
	    log << "registring variable '" << currentVarName << "'";
	    if(elem==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	      log << " for default hypothesis" << endl;
	    } else {
	      log << " for the '" << ModellingHypothesis::toString(elem)
		  << "' hypothesis" << endl;
	    }
	  }
	  this->registerVariable(currentVarName,false);
	  this->mb.addLocalVariable(elem,VariableDescription(iv.type,currentVarName,iv.arraySize,0u));
	  if(this->useDynamicallyAllocatedVector(iv.arraySize)){
	    icb.code += "this->"+currentVarName +".resize("+to_string(iv.arraySize)+");\n";
	  }
	  if((algorithm!="RungeKutta4/2")&&(algorithm!="RungeKutta5/4")){
	    icb.code += "this->" +currentVarName + " = this->" + iv.name + ";\n";
	  }
	}
      }
      // driving variables
      if((algorithm!="RungeKutta4/2")&&(algorithm!="RungeKutta5/4")){
	for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
	  const auto& dv = pm->first;
	  if(uvs.find(dv.name)!=uvs.end()){
	    ib.code += "this->" + dv.name + "_ = this->" + dv.name + ";\n";
	  }
	}
	if(uvs.find("T")!=uvs.end()){
	  ib.code += "this->T_ = this->T;\n";
	}
      }
      for(const auto& ev:evs){
	if(uvs.find(ev.name)!=uvs.end()){
	  string currentVarName = ev.name + "_";
	  if(getVerboseMode()>=VERBOSE_DEBUG){
	    auto& log = getLogStream();
	    log << "registring variable '" << currentVarName << "'";
	    if(elem==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	      log << " for default hypothesis" << endl;
	    } else {
	      log << " for the '" << ModellingHypothesis::toString(elem)
		  << "' hypothesis" << endl;
	    }
	  }
	  this->registerVariable(currentVarName,false);
	  this->mb.addLocalVariable(elem,VariableDescription(ev.type,currentVarName,ev.arraySize,0u));
	  if(this->useDynamicallyAllocatedVector(ev.arraySize)){
	    icb.code += "this->"+currentVarName +".resize("+to_string(ev.arraySize)+");\n";
	  }
	  if((algorithm!="RungeKutta4/2")&&(algorithm!="RungeKutta5/4")){
	    icb.code += "this->" +currentVarName + " = this->" + ev.name + ";\n";
	  }
	}
      }
      this->mb.setCode(elem,BehaviourData::BeforeInitializeLocalVariables,icb,
		       BehaviourData::CREATEORAPPEND,
		       BehaviourData::AT_END);
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
    this->mb.setCode(h,BehaviourData::BeforeInitializeLocalVariables,ib,
		     BehaviourData::CREATEORAPPEND,
		     BehaviourData::AT_BEGINNING);
    // part introduced at the end of the initialize local variables
    for(pm=this->mb.getMainVariables().begin();pm!=this->mb.getMainVariables().end();++pm){
      const auto& dv = pm->first;
      if(dv.increment_known){
	ie.code += "this->d" + dv.name + "_ = (this->d"+dv.name+")/(this->dt);\n";
      } else {
	string msg("RungeKuttaDSL::endsInputFileProcessing : ");
	msg += "unsupported driving variable '"+dv.name+"'";
	throw(runtime_error(msg));
      }
    }
    this->mb.setCode(h,BehaviourData::AfterInitializeLocalVariables,ie,
		     BehaviourData::CREATEORAPPEND,
		     BehaviourData::BODY);
    // minimal tangent operator if mandatory
    this->setMinimalTangentOperator();
  } // end of RungeKuttaDSL::endsInputFileProcessing

  void
  RungeKuttaDSL::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
  }

  void
  RungeKuttaDSL::writeBehaviourParserSpecificMembers(const Hypothesis h)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "bool\ncomputeStress(void){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputeStress) << endl;
    this->behaviourFile << "return true;" << endl;
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeStress" << endl << endl;
    this->behaviourFile << "bool\ncomputeFinalStress(void){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    writeMaterialLaws("RungeKuttaDSL::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->mb.getMaterialLaws());
    this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputeFinalStress) << endl;
    this->behaviourFile << "return true;" << endl;
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeFinalStress" << endl << endl;
    this->behaviourFile << "bool\ncomputeDerivative(void){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    writeMaterialLaws("RungeKuttaDSL::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->mb.getMaterialLaws());
    this->behaviourFile << this->mb.getCode(h,BehaviourData::ComputeDerivative) << endl;
    this->behaviourFile << "return true;" << endl;
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::computeDerivative" << endl << endl;
  } // end of writeBehaviourParserSpecificMembers

  void RungeKuttaDSL::writeBehaviourUpdateStateVariables(const Hypothesis)
  {
    // Disabled (makes no sense for this parser)
  } // end of writeBehaviourUpdateStateVariables

  void
  RungeKuttaDSL::writeBehaviourUpdateAuxiliaryStateVariables(const Hypothesis h)
  {
    using namespace std;
    if(this->mb.hasCode(h,BehaviourData::UpdateAuxiliaryStateVariables)){
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Update auxiliary state variables at end of integration\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "updateAuxiliaryStateVariables(const real dt_)";
      this->behaviourFile << "{\n";
      this->behaviourFile << "static_cast<void>(dt_);\n";
      this->behaviourFile << "using namespace std;" << endl;
      this->behaviourFile << "using namespace tfel::math;" << endl;
      writeMaterialLaws("RungeKuttaDSL::writeBehaviourUpdateAuxiliaryStateVariables",
			this->behaviourFile,this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,BehaviourData::UpdateAuxiliaryStateVariables) << endl;
      this->behaviourFile << "}\n\n";
    }
  } // end of  RungeKuttaDSL::writeBehaviourUpdateAuxiliaryStateVariables

  void RungeKuttaDSL::writeBehaviourEulerIntegrator(const Hypothesis h)
  {
    using std::endl;
    const auto& d = this->mb.getBehaviourData(h);
    this->behaviourFile << "this->computeStress();" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(const auto& v : d.getStateVariables()){
      this->behaviourFile << "this->" << v.name << " += "
			  << "this->dt*(this->d" << v.name << ");" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(d.hasCode(BehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);" << endl;
    }
  } // end of writeBehaviourEulerIntegrator

  void RungeKuttaDSL::writeBehaviourRK2Integrator(const Hypothesis h)
  {
    using std::endl;
    const auto& d = this->mb.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
    const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
    uvs.insert(uvs2.begin(),uvs2.end());
    this->behaviourFile << "constexpr real cste1_2 = real{1}/real{2};" << endl;
    this->behaviourFile << "// Compute K1's values" << endl;
    this->behaviourFile << "this->computeStress();" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(const auto& v : d.getStateVariables()){
      this->behaviourFile << "this->d" << v.name
			  << "_K1 = (this->dt)*(this->d" << v.name << ");" << endl;
    }
    writeExternalVariablesCurrentValues2(this->behaviourFile,this->mb,h,"cste1_2");
    for(const auto& iv : d.getStateVariables()){
      if(uvs.find(iv.name)!=uvs.end()){
	this->behaviourFile << "this->" << iv.name << "_ += cste1_2*(this->d" << iv.name << "_K1);" << endl;
      }
    }
    this->behaviourFile << "this->computeStress();" << endl << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    this->behaviourFile << "// Final Step" << endl;
    for(const auto& v : d.getStateVariables()){
      this->behaviourFile << "this->" << v.name << " += "
			  << "this->dt*(this->d" << v.name << ");" << endl;
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(d.hasCode(BehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);" << endl;
    }
  } // end of writeBehaviourRK2Integrator

  void RungeKuttaDSL::writeBehaviourRK54Integrator(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    //! all registred variables used in ComputeDerivatives and ComputeStress blocks
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
    const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
    uvs.insert(uvs2.begin(),uvs2.end());
    VariableDescriptionContainer::const_iterator p;
    ErrorEvaluation eev;
    auto svsize = this->getTotalSize(d.getStateVariables());
    if(svsize.getScalarSize()+svsize.getTVectorSize()+
       3u*svsize.getStensorSize()+3u*svsize.getTensorSize()>=20){
      eev = MAXIMUMVALUEERROREVALUATION;
    } else {
      eev = ERRORSUMMATIONEVALUATION;
    }
    this->behaviourFile << "constexpr real cste1_2         = real{1}/real{2};" << endl;
    this->behaviourFile << "constexpr real cste1_4         = real{1}/real{4};" << endl;
    this->behaviourFile << "constexpr real cste3_8         = real{3}/real{8};" << endl;
    this->behaviourFile << "constexpr real cste3_32        = real{3}/real{32};" << endl;
    this->behaviourFile << "constexpr real cste12_13       = Type(12)/Type(13);" << endl;
    this->behaviourFile << "constexpr real cste1932_2197   = Type(1932)/Type(2197);" << endl;
    this->behaviourFile << "constexpr real cste7200_2197   = Type(7200)/Type(2197);" << endl;
    this->behaviourFile << "constexpr real cste7296_2197   = Type(7296)/Type(2197);" << endl;
    this->behaviourFile << "constexpr real cste439_216     = Type(439)/Type(216);" << endl;
    this->behaviourFile << "constexpr real cste3680_513    = Type(3680)/Type(513);" << endl;
    this->behaviourFile << "constexpr real cste845_4104    = Type(845)/Type(4104);" << endl;
    this->behaviourFile << "constexpr real cste8_27        = Type(8)/Type(27);" << endl;
    this->behaviourFile << "constexpr real cste3544_2565   = Type(3544)/Type(2565);" << endl;
    this->behaviourFile << "constexpr real cste1859_4104   = Type(1859)/Type(4104);" << endl;
    this->behaviourFile << "constexpr real cste11_40       = Type(11)/Type(40);" << endl;
    this->behaviourFile << "constexpr real cste16_135      = Type(16)/Type(135);" << endl;
    this->behaviourFile << "constexpr real cste6656_12825  = Type(6656)/Type(12825);" << endl;
    this->behaviourFile << "constexpr real cste28561_56430 = Type(28561)/Type(56430);" << endl;
    this->behaviourFile << "constexpr real cste9_50        = Type(9)/Type(50);" << endl;
    this->behaviourFile << "constexpr real cste2_55        = Type(2)/Type(55);" << endl;
    this->behaviourFile << "constexpr real cste1_360       = Type(1)/Type(360);" << endl;
    this->behaviourFile << "constexpr real cste128_4275    = Type(128)/Type(4275);" << endl;
    this->behaviourFile << "constexpr real cste2197_75240  = Type(2197)/Type(75240);" << endl;
    this->behaviourFile << "constexpr real cste1_50        = Type(1)/Type(50);" << endl;
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"0");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"cste1_4");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name << "_ += cste1_4*(this->d" << p->name << "_K1);" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"cste3_8");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+cste3_32*(this->d" << p->name << "_K1+3*(this->d"
			    << p->name << "_K2));" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"cste12_13");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+cste1932_2197*(this->d" << p->name << "_K1)"
			    << "-cste7200_2197*(this->d" << p->name << "_K2)"
			    << "+cste7296_2197*(this->d" << p->name << "_K3);" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"1");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+cste439_216*(this->d" << p->name << "_K1)"
			    << "-8*(this->d" << p->name << "_K2)"
			    << "+cste3680_513*(this->d" << p->name << "_K3)"
			    << "-cste845_4104*(this->d" << p->name << "_K4);" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"cste1_2");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "-cste8_27*(this->d" << p->name << "_K1)"
			    << "+2*(this->d" << p->name << "_K2)"
			    << "-cste3544_2565*(this->d" << p->name << "_K3)"
			    << "+cste1859_4104*(this->d" << p->name << "_K4)"
			    << "-cste11_40*(this->d" << p->name << "_K5);" << endl;
      }
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
      this->behaviourFile << "error/=" << svsize << ";" << endl;
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
      string msg("RungeKuttaDSL::writeBehaviourRK54Integrator : ");
      msg += "internal error, unsupported error evaluation.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "if(std::isnan(error)){" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
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
    if(d.hasCode(BehaviourData::UpdateAuxiliaryStateVariables)){
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
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
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
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
  } // end of RungeKuttaDSL::writeBehaviourRK54Integrator

  void RungeKuttaDSL::writeBehaviourRKCastemIntegrator(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    //! all registred variables used in ComputeDerivatives and ComputeStress blocks
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
    const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
    uvs.insert(uvs2.begin(),uvs2.end());
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize stateVarsSize;
    for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "constexpr real cste1_2 = real{1}/real{2};" << endl;
    this->behaviourFile << "constexpr real cste1_4 = real{1}/real{4};" << endl;
    this->behaviourFile << "constexpr real cste1_6 = Type(1)/Type(6);" << endl;
    this->behaviourFile << "time t   = time(0);" << endl;
    this->behaviourFile << "time dt_ = this->dt;" << endl;
    this->behaviourFile << "StressStensor sigf;" << endl;
    this->behaviourFile << "real errabs;" << endl;
    this->behaviourFile << "real asig;" << endl;
    this->behaviourFile << "bool failed = !this->computeStress();" << endl;
    if(getDebugMode()){
      this->behaviourFile << "if(failed){" << endl;
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
			  << "::integrate() : failed while computing initial stress\" << endl;" << endl;
      this->behaviourFile << "}" << endl;
    }
    this->behaviourFile << "if(failed){" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"0");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"cste1_2");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name << "_ += cste1_2*(this->d" << p->name << "_K1);" << endl;
      }
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
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+cste1_4*(this->d" << p->name << "_K1 + this->d" << p->name << "_K2);" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"1");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ += cste1_2*(this->d" << p->name << "_K3);" << endl;
      }
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
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+cste1_4*(this->d" << p->name << "_K1 + this->d" << p->name
			    << "_K2 + this->d" << p->name << "_K3 + this->d" << p->name << "_K4);" << endl;
      }
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
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+cste1_6*(this->d" << p->name << "_K1 + Type(4)*this->d" << p->name
			    << "_K3 + this->d" << p->name << "_K5);" << endl;
      }
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
    this->behaviourFile << "if ((sqra>"  << this->mb.getClassName() << "::rkcastem_div)||(std::isnan(ra))){" << endl;
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
			  << " += cste1_4*(this->d" << p->name << "_K1 + this->d" << p->name
			  << "_K2 + this->d" << p->name << "_K3 + this->d" << p->name << "_K4);" << endl;
    }
    this->behaviourFile << "this->computeFinalStress();" << endl;
    if(this->mb.hasCode(h,BehaviourData::UpdateAuxiliaryStateVariables)){
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
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if(!converged){" << endl;
    this->behaviourFile << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){" << endl;
    this->behaviourFile << "dt_=this->dt-t;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
  } // end of RungeKuttaDSL::writeBehaviourRKCastemIntegrator

  void RungeKuttaDSL::writeBehaviourRK42Integrator(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
    const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
    uvs.insert(uvs2.begin(),uvs2.end());
    VariableDescriptionContainer::const_iterator p;
    ErrorEvaluation eev;
    auto svsize = this->getTotalSize(d.getStateVariables());
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
    this->behaviourFile << "constexpr real cste1_2 = real{1}/real{2};" << endl;
    this->behaviourFile << "constexpr real cste1_6  = Type(1)/Type(6);" << endl;
    this->behaviourFile << "constexpr real cste1_3  = Type(1)/Type(3);" << endl;
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"0");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"cste1_2");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name << "_ += cste1_2*(this->d" << p->name << "_K1);" << endl;
      }
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
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+cste1_2*(this->d" << p->name << "_K2);" << endl;
      }
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
    writeExternalVariablesCurrentValues(this->behaviourFile,this->mb,h,"1");
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name
			    << "_ = this->" << p->name
			    << "+this->d" << p->name << "_K3;" << endl;
      }
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
      string msg("RungeKuttaDSL::writeBehaviourRK42Integrator : ");
      msg += "internal error, unsupported error evaluation.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "if(std::isnan(error)){" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
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
    if(this->mb.hasCode(h,BehaviourData::UpdateAuxiliaryStateVariables)){
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
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){" << endl;
    this->behaviourFile << "dt_=this->dt-t;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "} else {" << endl;
    this->behaviourFile << "// failed is true" << endl;
    this->behaviourFile << "dt_ *= real(0.1f);" << endl;
    this->behaviourFile << "if(dt_<dtprec){" << endl;
    this->behaviourFile << "throw(tfel::material::DivergenceException());" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}" << endl;
  } // end of RungeKuttaDSL::writeBehaviourRK42Integrator

  void RungeKuttaDSL::writeBehaviourRK4Integrator(const Hypothesis h)
  {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).variables;
    const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).variables;
    uvs.insert(uvs2.begin(),uvs2.end());
    this->behaviourFile << "constexpr real cste1_2 = real{1}/real{2};" << endl;
    this->behaviourFile << "// Compute K1's values" << endl;
    this->behaviourFile << "this->computeStress();" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name
			  << "_K1 = (this->dt)*(this->d" << p->name << ");" << endl;
    }
    writeExternalVariablesCurrentValues2(this->behaviourFile,this->mb,h,"cste1_2");
    for(const auto& iv : d.getStateVariables()){
      if(uvs.find(iv.name)!=uvs.end()){
	this->behaviourFile << "this->" << iv.name << "_ += cste1_2*(this->d" << iv.name << "_K1);" << endl;
      }
    }
    this->behaviourFile << "// Update stress field" << endl;
    this->behaviourFile << "this->computeStress();" << endl << endl;
    this->behaviourFile << "// Compute K2's values" << endl;
    this->behaviourFile << "this->computeDerivative();" << endl;
    for(p =d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourFile << "this->d" << p->name
			  << "_K2 = (this->dt)*(this->d" << p->name << ");" << endl;
    }
    for(const auto& iv : d.getStateVariables()){
      if(uvs.find(iv.name)!=uvs.end()){
	this->behaviourFile << "this->" << iv.name << "_ = " << "this->" << iv.name
			    << "+ cste1_2*(this->d" << iv.name << "_K2);" << endl;
      }
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
      if(uvs.find(p->name)!=uvs.end()){
	this->behaviourFile << "this->" << p->name << "_ = " << "this->" << p->name
			    << "+ (this->d" << p->name << "_K3);" << endl;
      }
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
    if(this->mb.hasCode(h,BehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);" << endl;
    }
  }  // end of RungeKuttaDSL::writeBehaviourRK4Integrator

  void RungeKuttaDSL::writeBehaviourIntegrator(const Hypothesis h)
  {
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    const auto& algorithm = this->mb.getAttribute<string>(BehaviourData::algorithm);
    const auto& d = this->mb.getBehaviourData(h);
    std::vector<BoundsDescription>::const_iterator p2;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "IntegrationResult" << endl;
    if(this->mb.hasAttribute(h,BehaviourData::hasConsistentTangentOperator)){
      this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt) override{" << endl;
    } else {
      if((this->mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->mb.getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL)){
	this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt) override{" << endl;
      } else {
	this->behaviourFile << "integrate(const SMFlag,const SMType smt) override{" << endl;
      }
    }
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
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
      string msg("RungeKuttaDSL::writeBehaviourIntegrator : ");
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
    if(this->mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfilingEnd(this->behaviourFile);
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
    this->behaviourFile << "} // end of " << this->mb.getClassName() << "::integrate" << endl << endl;
  } // end of void RungeKuttaDSL::writeBehaviourIntegrator(void)

  RungeKuttaDSL::~RungeKuttaDSL()
  {} // end of ~RungeKuttaDSL

} // end of namespace mfront
