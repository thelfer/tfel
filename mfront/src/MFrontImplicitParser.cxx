/*!
 * \file   MFrontImplicitParser.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 sep 2008
 */

#include<cstdlib>
#include<sstream>

#include"System/System.hxx"
#include"ParserUtilities.hxx"
#include"MFrontImplicitParser.hxx"

namespace mfront{

  MFrontImplicitParser::MFrontImplicitParser()
    : MFrontVirtualParser(), 
      MFrontBehaviourParserBase<MFrontImplicitParser>(),
      theta(0.5f),
      epsilon(1.e-8),
      iterMax(100),
      algorithm(MFrontImplicitParser::DEFAULT)
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    // static variable
    this->registerStaticVariable("theta");
    this->registerStaticVariable("epsilon");
    this->registerStaticVariable("iterMax");
    // Default state vars
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->registerVariable("deto");
    this->registerVariable("zeros");
    this->registerVariable("fzeros");
    this->registerVariable("zeros2");
    this->registerVariable("fzeros2");
    this->registerVariable("Dzeros");
    this->registerVariable("Dfzeros");
    this->registerVariable("jacobian");
    this->registerVariable("jacobian2");
    this->registerVariable("deto");
    this->registerVariable("t");
    this->registerVariable("dt_");
    this->registerVariable("error");
    this->registerVariable("idx");
    this->reserveName("computeStress");
    this->reserveName("computeFinalStress");
    this->reserveName("computeFdF");
    this->reserveName("predicte");
    this->reserveName("integrate");
    this->reserveName("error");
    this->reserveName("iter");
    this->reserveName("iterMax");
    this->reserveName("converge");
    this->reserveName("broyden_inv");
    this->stateVarsHolder.push_back(VarHandler("StrainStensor","eel",0u));
    this->glossaryNames.insert(MVType("eel","ElasticStrain"));
    // CallBacks
    this->registerNewCallBack("@MaterialLaw",&MFrontImplicitParser::treatMaterialLaw);
    this->registerNewCallBack("@ComputeStress",&MFrontImplicitParser::treatComputeStress);
    this->registerNewCallBack("@Predictor",&MFrontImplicitParser::treatPredictor);
    this->registerNewCallBack("@Theta",&MFrontImplicitParser::treatTheta);
    this->registerNewCallBack("@Epsilon",&MFrontImplicitParser::treatEpsilon);
    this->registerNewCallBack("@IterMax",&MFrontImplicitParser::treatIterMax);
    this->registerNewCallBack("@Algorithm",&MFrontImplicitParser::treatAlgorithm);
    //    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
    this->disableCallBack("@UseQt");
  } // end of MFrontImplicitParser::MFrontImplicitParser

    void MFrontImplicitParser::treatAlgorithm(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParser::treatAlgorithm",
			    "Cannot read algorithm name.");
    if(this->algorithm!=MFrontImplicitParser::DEFAULT){
      this->throwRuntimeError("MFrontImplicitParser::treatAlgorithm",
			      "@Algorithm has already been read.");
    }
    if(this->current->value=="Newton-Raphson"){
      this->algorithm = MFrontImplicitParser::NEWTONRAPHSON;
    } else if(this->current->value=="Broyden"){
      this->algorithm = MFrontImplicitParser::BROYDEN;
    } else if(this->current->value=="Broyden2"){
      this->algorithm = MFrontImplicitParser::BROYDEN2;
    } else {
      this->throwRuntimeError("MFrontImplicitParser::treatAlgorithm : ",
			      this->current->value+" is not a valid algorithm name\n"
			      "Supported algorithms are : Newton-Raphson, Broyden, and Broyden2.");
    }
    ++this->current;
    this->readSpecifiedToken("MFrontImplicitParser::treatAlgorithm",";");
  } // end of MFrontImplicitParser::treatAlgorithm

  void
  MFrontImplicitParser::treatTheta(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParser::treatTheta",
			    "Cannot read theta value.");
    istringstream flux(current->value);
    flux >> this->theta;
    if(flux.fail()){
      this->throwRuntimeError("MFrontImplicitParser::treatTheta",
			      "Failed to read theta value.");
    }
    if((this->theta<0.)||(this->theta>1.)){
      this->throwRuntimeError("MFrontImplicitParser::treatTheta",
			      "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParser::treatTheta",";");
  } // end of MFrontImplicitParser::treatTheta

  void
  MFrontImplicitParser::treatEpsilon(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParser::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> this->epsilon;
    if(flux.fail()){
      this->throwRuntimeError("MFrontImplicitParser::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(this->epsilon<0){
      this->throwRuntimeError("MFrontImplicitParser::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontImplicitParser::treatEpsilon",";");
  } // MFrontImplicitParser::treatEpsilon

  void
  MFrontImplicitParser::treatIterMax(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontImplicitParser::treatIterMax",
			    "Cannot read iterMax value.");
    istringstream flux(current->value);
    flux >> this->iterMax;
    if(flux.fail()){
      this->throwRuntimeError("MFrontImplicitParser::treatIterMax",
			      "Failed to read iterMax value.");
    }
    ++(this->current);
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
    this->readVarList(this->stateVarsHolder,true);
  } // end of MFrontImplicitParser::treatStateVariables

  std::string
  MFrontImplicitParser::variableModifier1(const std::string& var,
					  const bool addThisPtr)
  {
    if((var=="eto")||(var=="T")||
       (this->isInternalStateVariable(var))||
       (this->isExternalStateVariable(var))){
      if(addThisPtr){
	return "(this->"+var+"+("+this->className+"::theta)*(this->d"+var+"))";
      } else {
	return "("+var+"+("+this->className+"::theta)*d"+var+")";
      }
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
    this->behaviourFile << "#include\"Math/st2tost2.hxx\"\n";
    this->behaviourFile << "#include\"Math/tmatrix.hxx\"\n";
    this->behaviourFile << "#include\"Math/tvector.hxx\"\n";
    this->behaviourFile << "#include\"Math/TinyMatrixSolve.hxx\"\n";
    this->behaviourFile << "#include\"Math/Stensor/SFTMCV.hxx\"\n";
    this->behaviourFile << "#include\"Math/Stensor/SFTMRV.hxx\"\n";
    this->behaviourFile << "#include\"Math/Stensor/SFTVV.hxx\"\n";
    this->behaviourFile << "#include\"Math/ST2toST2/ST2toST2FTMV.hxx\"\n\n";
  } // end of MFrontImplicitParser::writeBehaviourParserSpecificIncludes(void)

  void MFrontImplicitParser::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "typedef tfel::math::st2tost2<N,Type> Stensor4;\n\n";
  } // end of MFrontImplicitParser::writeBehaviourParserSpecificTypedefs(void)

  void
  MFrontImplicitParser::writeBehaviourParserSpecificMembers(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n += this->getTypeSize(p->type);
    }
    this->behaviourFile << "\n// Jacobian\n";
    this->behaviourFile << "tfel::math::tmatrix<" << n << "," << n << "> jacobian;\n";
    this->behaviourFile << "// zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << "> zeros;\n\n";
    this->behaviourFile << "// function\n";
    this->behaviourFile << "tfel::math::tvector<" << n << "> fzeros;\n\n";
    this->behaviourFile << "// number of iterations\n";
    this->behaviourFile << "unsigned int iter;\n\n";
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

  void MFrontImplicitParser::writeBehaviourIntegrator(){
    using namespace std;
    VarContainer::const_iterator p;
    VarContainer::const_iterator p2;
    vector<BoundsDescription>::const_iterator p3;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n2 += this->getTypeSize(p->type);
    }
    this->checkBehaviourFile();
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\nintegrate(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
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
      this->behaviourFile << "// setting jacobian to identity\n";
      this->behaviourFile << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n";
      this->behaviourFile << "for(unsigned short idx = 0; idx!= "<< n2<<  ";++idx){\n";
      this->behaviourFile << "this->jacobian(idx,idx)= real(1);\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
    }
    this->behaviourFile << "this->iter=0;\n";
    this->behaviourFile << "while((converge==false)&&\n";
    this->behaviourFile << "(this->iter<(" << this->className << "::iterMax))){\n";
    if(this->algorithm==MFrontImplicitParser::NEWTONRAPHSON){
    this->behaviourFile << "this->computeStress();\n";
    this->behaviourFile << "this->computeFdF();\n";
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(this->jacobian,this->fzeros);\n";
      this->behaviourFile << "this->zeros -= this->fzeros;\n";
    }
    if(this->algorithm==MFrontImplicitParser::BROYDEN){
      this->behaviourFile << "Dzeros = -this->fzeros;\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "TinyMatrixSolve<" << n2
			  << "," << "real>::exe(jacobian2,Dzeros);\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "this->zeros  += Dzeros;\n";
      this->behaviourFile << "fzeros2 = this->fzeros;\n";
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
      this->behaviourFile << "broyden_inv = (Dzeros|Dzeros);\n";
      this->behaviourFile << "if(broyden_inv<100*std::numeric_limits<real>::epsilon()){\n";
      this->behaviourFile << "throw(MaterialLawException(\"Broyden null increment\"));\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "this->jacobian += "
			  << "(((this->fzeros-fzeros2)-(jacobian2)*(Dzeros))^Dzeros)/broyden_inv;\n";
    }
    if(this->algorithm==MFrontImplicitParser::BROYDEN2){
      this->behaviourFile << "Dzeros   = -(this->jacobian)*(this->fzeros);\n";
      this->behaviourFile << "this->zeros += Dzeros;\n";
      this->behaviourFile << "fzeros2 = this->fzeros;\n";
      this->behaviourFile << "this->computeStress();\n";
      this->behaviourFile << "this->computeFdF();\n";
      this->behaviourFile << "Dfzeros   = (this->fzeros)-fzeros2;\n";
      this->behaviourFile << "jacobian2 = this->jacobian;\n";
      this->behaviourFile << "broyden_inv = Dzeros|jacobian2*Dfzeros;\n";
      this->behaviourFile << "if(broyden_inv<100*std::numeric_limits<real>::epsilon()){\n";
      this->behaviourFile << "throw(MaterialException(\"Broyden null denominator\"));\n";
      this->behaviourFile << "}\n";
      this->behaviourFile << "this->jacobian += "
			  << "((Dzeros-jacobian2*Dfzeros)^(Dzeros*jacobian2))/(broyden_inv);\n";
    }
    this->behaviourFile << "error=norm(this->fzeros);\n";
    this->behaviourFile << "this->iter+=1;\n";
    this->behaviourFile << "converge = ((error)/(real(" << n2 << "))<";
    this->behaviourFile << "(" << this->className << "::epsilon));\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(this->iter==" << this->className << "::iterMax){\n";
    this->behaviourFile << "throw(MaterialException(\"Newton-Raphson: no convergence\"));\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->updateStateVars();\n";
    this->behaviourFile << "this->computeFinalStress();\n";
    for(p3  = this->boundsDescriptions.begin();
	p3 != this->boundsDescriptions.end();++p3){
      if(p3->varCategory==BoundsDescription::StateVar){
	p3->writeBoundsChecks(this->behaviourFile);
      }
    }
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
      n += this->getTypeSize(p->type);
    }
    n = SupportedTypes::TypeSize();
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n3 = SupportedTypes::TypeSize();
      for(p2=this->stateVarsHolder.begin();p2!=this->stateVarsHolder.end();++p2){
	if(this->varNames.find("df"+p->name+"_dd"+p2->name)!=this->varNames.end()){
	  string msg("MFrontImplicitParser::writeBehaviourIntegrator : ");
	  msg += "variable name 'df"+p->name+"_dd"+p2->name;
	  msg += "' is reserved.\n";
	  throw(runtime_error(msg));
	}
	this->behaviourFile << "// derivative of variable f" << p->name 
			    << " by variable " << p2->name << "\n";
	if(this->getTypeFlag(p->type)==SupportedTypes::Stensor){
	  if(this->getTypeFlag(p2->type)==SupportedTypes::Stensor){
	    this->behaviourFile << "typename tfel::math::ST2toST2FTMV<N,"
				<< n2 << "," << n2 << ",\n"
				<< n  << "," << n3
				<< ",real>::type df" << p->name << "_dd" << p2->name << "(this->jacobian);\n";
	  } else if(this->getTypeFlag(p2->type)==SupportedTypes::Scalar){
	    this->behaviourFile << "typename tfel::math::SFTMCV<N," 
				<< n2 << "," << n2 << ",\n"
				<< n  << "," << n3
				<< ",real>::type df" << p->name << "_dd" << p2->name << "(this->jacobian);\n";
	  } else {
	    string msg("MFrontImplicitParser::writeOutputFiles : ");
	    msg += "unsupported type for state variable ";
	    msg += p2->name;
	    throw(runtime_error(msg));
	  }
	} else if(this->getTypeFlag(p->type)==SupportedTypes::Scalar){
	  if(this->getTypeFlag(p2->type)==SupportedTypes::Stensor){
	    this->behaviourFile << "typename tfel::math::SFTMRV<N,"
				<< n2 << "," << n2 << ",\n"
				<< n  << "," << n3
				<< ",real>::type df" << p->name 
				<< "_dd" << p2->name << "(this->jacobian);\n";
	  } else if(this->getTypeFlag(p2->type)==SupportedTypes::Scalar){
	    this->behaviourFile << "real& df" << p->name << "_dd" << p2->name 
				<< " = this->jacobian(" << n << "," << n3 << ");\n";
	  } else {
	    string msg("MFrontImplicitParser::writeOutputFiles : ");
	    msg += "unsupported type for state variable ";
	    msg += p2->name;
	    throw(runtime_error(msg));
	  }
	} else {
	  string msg("MFrontImplicitParser::writeOutputFiles : ");
	  msg += "unsupported type for state variable ";
	  msg += p->name;
	  throw(runtime_error(msg));
	}
	n3 += this->getTypeSize(p2->type);
      }
      n += this->getTypeSize(p->type);
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
      for(p2=this->stateVarsHolder.begin();p2!=this->stateVarsHolder.end();++p2){
	this->behaviourFile << "static_cast<void>(df"
			    << p->name << "_dd" << p2->name
			    << "); /* suppress potential warnings */\n";
      }
    }
    this->behaviourFile << "}\n\n";
  } // end of MFrontImplicitParser::writeBehaviourIntegrator

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
      n += this->getTypeSize(p->type);
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

  void MFrontImplicitParser::writeBehaviourStateVarsIncrements(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      n2 += this->getTypeSize(p->type);
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
      n += this->getTypeSize(p->type);
    }
    this->behaviourFile << endl;
  }

  void
  MFrontImplicitParser::endsInputFileProcessing(void)
  {
    this->staticVars.push_back(StaticVarHandler("real","theta",0u,this->theta));
    this->staticVars.push_back(StaticVarHandler("real","epsilon",0u,this->epsilon));
    if(this->algorithm==MFrontImplicitParser::DEFAULT){
      this->algorithm=MFrontImplicitParser::NEWTONRAPHSON;
    }
  } // end of MFrontImplicitParser::endsInputFileProcessing(void)

  void 
  MFrontImplicitParser::writeBehaviourStaticVars(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "static const unsigned short iterMax = ";
    this->behaviourFile << this->iterMax << ";\n";
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
      interface->endTreatement(this->library,
			       this->material,
			       this->className,
    			       this->authorName,
    			       this->date,
    			       this->coefsHolder,
    			       this->stateVarsHolder,
    			       this->externalStateVarsHolder,
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
    systemCall::mkdir("include/Material/");
    systemCall::mkdir("src");
    if(this->className.empty()){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->behaviourFileName  = this->className;
    this->behaviourFileName += ".hxx";
    this->behaviourFile.open(("include/Material/"+this->behaviourFileName).c_str());
    if(!this->behaviourFile){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->behaviourDataFileName  = this->className;
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->behaviourDataFile.open(("include/Material/"+this->behaviourDataFileName).c_str());
    if(!this->behaviourDataFile){
      string msg("MFrontImplicitParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->integrationDataFileName  = this->className;
    this->integrationDataFileName += "IntegrationData.hxx";
    this->integrationDataFile.open(("include/Material/"+this->integrationDataFileName).c_str());
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

