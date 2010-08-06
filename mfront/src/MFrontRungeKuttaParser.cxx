/*!
 * \file   MFrontRungeKuttaParser.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<string>
#include<sstream>
#include<stdexcept>

#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontRungeKuttaParser.hxx"

namespace mfront{

  MFrontRungeKuttaParser::MFrontRungeKuttaParser()
    : MFrontVirtualParser(), 
      MFrontBehaviourParserBase<MFrontRungeKuttaParser>(),
      algorithm("RungeKutta5/4"),
      epsilon(1.e-8),
      dtmin(0.),
      nbrOfEvaluation(6)
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    this->useStateVarTimeDerivative=true;
    // static variable
    this->registerStaticVariable("epsilon");
    this->registerStaticVariable("dtmin");
    // Default state vars
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->registerVariable("t");
    this->registerVariable("dt_");
    this->registerVariable("T_");
    this->registerVariable("eto_");
    this->registerVariable("deto_");
    this->reserveName("error");
    this->reserveName("failed");
    this->localVarsHolder.push_back(VarHandler("StrainStensor","eto_",0u));
    this->localVarsHolder.push_back(VarHandler("StrainRateStensor","deto_",0u));
    this->localVarsHolder.push_back(VarHandler("temperature","T_",0u));
    this->stateVarsHolder.push_back(VarHandler("StrainStensor","eel",0u));
    this->glossaryNames.insert(MVType("eel","ElasticStrain"));
    // CallBacks
    this->registerNewCallBack("@MaterialLaw",&MFrontRungeKuttaParser::treatMaterialLaw);
    this->registerNewCallBack("@Algorithm",&MFrontRungeKuttaParser::treatAlgorithm);
    this->registerNewCallBack("@Derivative",&MFrontRungeKuttaParser::treatDerivative);
    this->registerNewCallBack("@ComputeStress",&MFrontRungeKuttaParser::treatComputeStress);
    this->registerNewCallBack("@Epsilon",&MFrontRungeKuttaParser::treatEpsilon);
    this->registerNewCallBack("@MinimalTimeStep",
			      &MFrontRungeKuttaParser::treatMinimalTimeStep);
    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
  }

  void
  MFrontRungeKuttaParser::setInterfaces(const std::set<std::string>& i)
  {
     MFrontBehaviourParserBase<MFrontRungeKuttaParser>::setInterfaces(i);
  } // end of MFrontRungeKuttaParser::setInterfaces

  void MFrontRungeKuttaParser::writeBehaviourParserSpecificIncludes(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "#include<limits>\n\n";
    this->behaviourFile << "#include<stdexcept>\n\n";
    this->behaviourFile << "#include\"TFEL/Math/General/Abs.hxx\"\n\n";
  }

  void MFrontRungeKuttaParser::treatStateVariables(void)
  {
    this->readVarList(this->stateVarsHolder,true);
  } // end of MFrontRungeKuttaParser::treatStateVariables

  std::string
  MFrontRungeKuttaParser::variableModifier1(const std::string& var,
					    const bool addThisPtr)
  {
    if((var=="eto")||(var=="T")||(var=="deto")||
       (this->isInternalStateVariable(var))||
       (this->isExternalStateVariable(var))){
      if(addThisPtr){
	return "this->"+var+"_";
      } else {
	return var+"_";
      }
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontRungeKuttaParser::variableModifier1

  std::string
  MFrontRungeKuttaParser::variableModifier2(const std::string& var,
					    const bool addThisPtr)
  {
    if((var=="eto")||(var=="T")||
       (this->isExternalStateVariable(var))){
      if(addThisPtr){
	return "this->"+var+"+this->d"+var;
      } else {
	return var+"+d"+var;
      }
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontRungeKuttaParser::variableModifier2

  void
  MFrontRungeKuttaParser::treatComputeStress(void)
  {
    if((!this->computeStress.empty())||
       (!this->computeFinalStress.empty())){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatComputeStress",
			      "@ComputeStress has already been called");
    }
    this->readNextBlock(this->computeStress,
			this->computeFinalStress,
			&ParserBase::variableModifier1,
			&ParserBase::variableModifier2,true);
  } // end of MFrontRungeKuttaParser::treatComputeStress

  void MFrontRungeKuttaParser::treatDerivative(void)
  {
    if(!this->derivative.empty()){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatDerivative",
			      "@Derivative has already been called");
    }
    this->derivative = this->readNextBlock(&ParserBase::variableModifier1,true);
  } // end of MFrontRungeKuttaParser::treatDerivative

  void
  MFrontRungeKuttaParser::treatUpdateAuxiliaryStateVars(void)
  {
    this->updateAuxiliaryStateVars = this->readNextBlock(0,true);
  } // end of MFrontRungeKuttaParser::treatUpdateAuxiliaryStateVarBase

  void
  MFrontRungeKuttaParser::treatEpsilon(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontRungeKuttaParser::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(this->current->value);
    flux >> this->epsilon;
    if(flux.fail()){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(this->epsilon<0.){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatEpsilon",
			      "Epsilon value can't be negative.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontRungeKuttaParser::treatEpsilon : ",";");
  } // end of MFrontRungeKuttaParser::treatEpsilon

  void
  MFrontRungeKuttaParser::treatMinimalTimeStep(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontRungeKuttaParser::treatMinimalTimeStep",
			    "Cannot read minimal time step value.");
    istringstream flux(this->current->value);
    flux >> this->dtmin;
    if(flux.fail()){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatMinimalTimeStep",
			      "Failed to read minimal time step value.");
    }
    if(this->dtmin<0.){
      this->throwRuntimeError("MFrontRungeKuttaParser::treatMinimalTimeStep",
			      "minimal time step value can't be negative.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontRungeKuttaParser::treatMinimalTimeStep : ",";");
  } // end of MFrontRungeKuttaParser::treatEpsilon

  void MFrontRungeKuttaParser::treatAlgorithm(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontRungeKuttaParser::treatAlgorithm",
			    "Cannot read algorithm name.");
    if(this->current->value=="euler"){
      this->algorithm = "Euler";
      this->nbrOfEvaluation = 0u;
    } else if(this->current->value=="rk2"){
      this->algorithm = "RungeKutta2";
      this->nbrOfEvaluation = 1u;
    } else if(this->current->value=="rk4"){
      this->algorithm = "RungeKutta4";
      this->nbrOfEvaluation = 4u;
    } else if(this->current->value=="rk42"){
      this->algorithm = "RungeKutta4/2";
      this->nbrOfEvaluation = 4u;
    } else if(this->current->value=="rk54"){
      this->algorithm = "RungeKutta5/4";
      this->nbrOfEvaluation = 6u;
    } else {
      this->throwRuntimeError("MFrontRungeKuttaParser::treatAlgorithm",
			      this->current->value+" is not a valid algorithm name"
			      "Supported algorithms are : 'euler', 'rk2',"
			      " 'rk4', 'rk42' and 'rk54'");
    }
    ++this->current;
    this->readSpecifiedToken("MFrontRungeKuttaParser::treatAlgorithm",";");
  }

  void MFrontRungeKuttaParser::writeBehaviourParserSpecificConstructorPart(void)
    {
    this->checkBehaviourFile();
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
           "Runge-Kutta algorithm. Avalailable algorithms are 'euleur', 'rk2', 'rk4' "
           "'r42' and 'rk54'";
  } // end of MFrontRungeKuttaParser::getDescription

  void
  MFrontRungeKuttaParser::setVerboseMode(void) 
  {
    this->verboseMode = true;
  }

  void
  MFrontRungeKuttaParser::setDebugMode(void) 
  {
    this->debugMode = true;
  }

  void
  MFrontRungeKuttaParser::setWarningMode(void) 
  {
    this->warningMode = true;
  }

  void
  MFrontRungeKuttaParser::treatFile(const std::string& fileName_) 
  {
    this->fileName = fileName_;
    MFrontBehaviourParserBase<MFrontRungeKuttaParser>::treatFile(fileName_);
  }

  void 
  MFrontRungeKuttaParser::writeBehaviourStaticVars(void)
  {
    this->checkBehaviourFile();
    MFrontBehaviourParserBase<MFrontRungeKuttaParser>::writeBehaviourStaticVars();
  }

  void
  MFrontRungeKuttaParser::endsInputFileProcessing(void)
  {
    using namespace std;
    VarContainer::iterator p;
    string currentVarName;
    string parserInitLocalVars;

    this->staticVars.push_back(StaticVarHandler("real","epsilon",0u,this->epsilon));
    this->staticVars.push_back(StaticVarHandler("real","dtmin",0u,this->dtmin));

    if(this->computeStress.empty()){
      string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
      msg += "@ComputeStress was not defined.";
      throw(runtime_error(msg));
    }
    if(this->derivative.empty()){
      string msg("MFrontRungeKuttaParser::endsInputFileProcessing : ");
      msg += "@Derivative was not defined.";
      throw(runtime_error(msg));
    }
    if(this->dtmin>0.){
      parserInitLocalVars += "if(this->dt<" + this->className + "::dtmin){\n";
      parserInitLocalVars += "this->dt=" + this->className + "::dtmin;\n";
      parserInitLocalVars += "}\n";
    }
    parserInitLocalVars += "if(this->dt<100*numeric_limits<real>::min()){\n";
    parserInitLocalVars += "string msg(\"" + this->className + "::" + this->className +"\");\n";
    parserInitLocalVars += "msg += \"time step too small.\";\n";
    parserInitLocalVars += "throw(runtime_error(msg));\n";
    parserInitLocalVars += "}\n";
    parserInitLocalVars += "this->deto_ = (this->deto)/(this->dt);\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      currentVarName = p->name + "_";
      this->registerVariable(currentVarName);
      this->localVarsHolder.push_back(VarHandler(p->type,currentVarName,0u));
      if((this->algorithm!="RungeKutta4/2")&&
	 (this->algorithm!="RungeKutta5/4")){
	parserInitLocalVars += "this->" +currentVarName + " = this->" + p->name + ";\n";
      }
      for(unsigned short i=0u;i!=this->nbrOfEvaluation;++i){
	currentVarName = "d" + p->name + "_K"+toString(static_cast<unsigned short>(i+1u));
	this->registerVariable(currentVarName);
	this->localVarsHolder.push_back(VarHandler(p->type,currentVarName,0u));
      }
    }
    if((this->algorithm!="RungeKutta4/2")&&
       (this->algorithm!="RungeKutta5/4")){
      parserInitLocalVars += "this->eto_ = this->eto;\n";
    }
    if((this->algorithm!="RungeKutta4/2")&&
       (this->algorithm!="RungeKutta5/4")){
      parserInitLocalVars += "this->T_ = this->T;\n";
    }
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      currentVarName = p->name + "_";
      this->registerVariable(currentVarName);
      this->localVarsHolder.push_back(VarHandler(p->type,currentVarName,0u));
      if((this->algorithm!="RungeKutta4/2")&&
	 (this->algorithm!="RungeKutta5/4")){
	parserInitLocalVars += "this->" + currentVarName + " = this->" + p->name + ";\n";
      }
    }
    if((this->algorithm!="RungeKutta4/2")&&
       (this->algorithm!="RungeKutta5/4")){
      parserInitLocalVars += "this->computeStress();\n";
    }
    this->initLocalVars = parserInitLocalVars + this->initLocalVars;
  }

  void 
  MFrontRungeKuttaParser::generateOutputFiles(void){
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
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
			       this->auxiliaryStateVarsHolder,
			       this->externalStateVarsHolder,
			       this->glossaryNames,
			       this->entryNames,
			       this->behaviourCharacteristic);
    }
  }

  void
  MFrontRungeKuttaParser::writeOutputFiles() 
  {    
    using namespace std;
    using namespace tfel::system;
    systemCall::mkdir("include");
    systemCall::mkdir("include/TFEL/");
    systemCall::mkdir("include/TFEL/Material/");
    systemCall::mkdir("src");
    if(this->className.empty()){
      string msg("MFrontRungeKuttaParser::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->behaviourFileName  = this->className;
    this->behaviourFileName += ".hxx";
    this->behaviourFile.open(("include/TFEL/Material/"+this->behaviourFileName).c_str());
    if(!this->behaviourFile){
      string msg("MFrontRungeKuttaParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->behaviourDataFileName  = this->className;
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->behaviourDataFile.open(("include/TFEL/Material/"+this->behaviourDataFileName).c_str());
    if(!this->behaviourDataFile){
      string msg("MFrontRungeKuttaParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->integrationDataFileName  = this->className;
    this->integrationDataFileName += "IntegrationData.hxx";
    this->integrationDataFile.open(("include/TFEL/Material/"+this->integrationDataFileName).c_str());
    if(!this->integrationDataFile){
      string msg("MFrontRungeKuttaParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->integrationDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFileName  = this->className;
    this->srcFileName += ".cxx";
    this->srcFile.open(("src/"+this->srcFileName).c_str());
    if(!this->srcFile){
      string msg("MFrontRungeKuttaParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->generateOutputFiles();
  }

  void
  MFrontRungeKuttaParser::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
  }

  void
  MFrontRungeKuttaParser::writeBehaviourParserSpecificMembers(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "bool\ncomputeStress(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << this->computeStress << endl;
    this->behaviourFile << "return true;\n"; 
    this->behaviourFile << "} // end of " << this->className << "::computeStress\n\n";
    this->behaviourFile << "bool\ncomputeFinalStress(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("MFrontRungeKuttaParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);		      
    this->behaviourFile << this->computeFinalStress << endl;
    this->behaviourFile << "return true;\n"; 
    this->behaviourFile << "} // end of " << this->className << "::computeFinalStress\n\n";
    this->behaviourFile << "bool\ncomputeDerivative(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("MFrontRungeKuttaParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);		      
    this->behaviourFile << this->derivative << endl; 
    this->behaviourFile << "return true;\n"; 
    this->behaviourFile << "} // end of " << this->className << "::computeDerivative\n\n";
  } // end of writeBehaviourParserSpecificMembers

  void MFrontRungeKuttaParser::writeBehaviourUpdateStateVars(void)
  {
    // Disabled (makes no sense for this parser)
  } // end of writeBehaviourUpdateStateVars

  void MFrontRungeKuttaParser::writeBehaviourEulerIntegrator(void)
  {
    using namespace std;
    using namespace std;
    VarContainer::iterator p;
    this->behaviourFile << "this->computeDerivative();\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << " += " 
			  << "this->dt*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeFinalStress();\n\n";
    if(!this->updateAuxiliaryStateVars.empty()){
      this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    }
  } // end of writeBehaviourEulerIntegrator

  void MFrontRungeKuttaParser::writeBehaviourRK2Integrator(void)
  {
    using namespace std;
    using namespace std;
    VarContainer::iterator p;
    this->behaviourFile << "// Compute K1's values\n";
    this->behaviourFile << "this->computeDerivative();\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (this->dt)*(this->d" << p->name << ");\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << "_K1);\n";
    }
    this->behaviourFile << "this->eto_ += 0.5f*(this->deto);\n";
    this->behaviourFile << "this->T_   += 0.5f*(this->dT);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeStress();\n\n";
    this->behaviourFile << "this->computeDerivative();\n";
    this->behaviourFile << "// Final Step\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << " += " 
			  << "this->dt*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeFinalStress();\n\n";
    if(!this->updateAuxiliaryStateVars.empty()){
      this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    }
  } // end of writeBehaviourRK2Integrator

  void MFrontRungeKuttaParser::writeBehaviourRK54Integrator(void)
  {
    using namespace std;
    VarContainer::iterator p;
    SupportedTypes::TypeSize stateVarsSize;

    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      stateVarsSize+=this->getTypeSize(p->type);
    }
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourFile << "static const unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;\n";
    this->behaviourFile << "static const Type cste12_13       = Type(12)/Type(13);\n";
    this->behaviourFile << "static const Type cste1932_2197   = Type(1932)/Type(2197);\n";
    this->behaviourFile << "static const Type cste7200_2197   = Type(7200)/Type(2197);\n";
    this->behaviourFile << "static const Type cste7296_2197   = Type(7296)/Type(2197);\n";
    this->behaviourFile << "static const Type cste439_216     = Type(439)/Type(216);\n";
    this->behaviourFile << "static const Type cste3680_513    = Type(3680)/Type(513);\n";
    this->behaviourFile << "static const Type cste845_4104    = Type(845)/Type(4104);\n";
    this->behaviourFile << "static const Type cste8_27        = Type(8)/Type(27);\n";
    this->behaviourFile << "static const Type cste3544_2565   = Type(3544)/Type(2565);\n";
    this->behaviourFile << "static const Type cste1859_4104   = Type(1859)/Type(4104);\n";
    this->behaviourFile << "static const Type cste11_40       = Type(11)/Type(40);\n";
    this->behaviourFile << "static const Type cste16_135      = Type(16)/Type(135);\n";
    this->behaviourFile << "static const Type cste6656_12825  = Type(6656)/Type(12825);\n";
    this->behaviourFile << "static const Type cste28561_56430 = Type(28561)/Type(56430);\n";
    this->behaviourFile << "static const Type cste9_50        = Type(9)/Type(50);\n";
    this->behaviourFile << "static const Type cste2_55        = Type(2)/Type(55);\n";
    this->behaviourFile << "static const Type cste1_360       = Type(1)/Type(360);\n";
    this->behaviourFile << "static const Type cste128_4275    = Type(128)/Type(4275);\n";
    this->behaviourFile << "static const Type cste2197_75240  = Type(2197)/Type(75240);\n";
    this->behaviourFile << "static const Type cste1_50        = Type(1)/Type(50);\n";

    this->behaviourFile << "time t   = time(0);\n";
    this->behaviourFile << "time dt_ = this->dt;\n";
    this->behaviourFile << "Type error;\n";
    this->behaviourFile << "while((this->dt)-t>Type(0.25)*dt_){\n";
    this->behaviourFile << "bool failed = false;\n";
    this->behaviourFile << "// Compute K1's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t/this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t/this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << "+(this->d" << p->name << ")*(t/this->dt);\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";\n";
    }
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K2's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t+0.25f*dt_)/(this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.25f*dt_)/(this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->" << p->name << ")*(t+0.25f*dt_)/(this->dt);\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.25f*(this->d" << p->name << "_K1);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K2 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K3's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t+0.375f*dt_)/(this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.375f*dt_)/(this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->" << p->name << ")*(t+0.375f*dt_)/(this->dt);\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+0.09375f*(this->d" << p->name << "_K1+3*(this->d"
			  << p->name << "_K2));\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K3 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";

    this->behaviourFile << "if(!failed){\n";    
    this->behaviourFile << "// Compute K4's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t+cste12_13*dt_)/(this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+cste12_13*dt_)/(this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->" << p->name << ")*(t+cste12_13*dt_)/(this->dt);\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+cste1932_2197*(this->d" << p->name << "_K1)"
			  << "-cste7200_2197*(this->d" << p->name << "_K2)"
			  << "+cste7296_2197*(this->d" << p->name << "_K3);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K4 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K5's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t+dt_)/(this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+dt_)/(this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->" << p->name << ")*(t+dt_)/(this->dt);\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+cste439_216*(this->d" << p->name << "_K1)"
			  << "-8*(this->d" << p->name << "_K2)"
			  << "+cste3680_513*(this->d" << p->name << "_K3)"
			  << "-cste845_4104*(this->d" << p->name << "_K4);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K5 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K6's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t+0.5f*dt_)/(this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.5f*dt_)/(this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  << "+(this->" << p->name << ")*(t+0.5f*dt_)/(this->dt);\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "-cste8_27*(this->d" << p->name << "_K1)"
			  << "+2*(this->d" << p->name << "_K2)"
			  << "-cste3544_2565*(this->d" << p->name << "_K3)"
			  << "+cste1859_4104*(this->d" << p->name << "_K4)"
			  << "-cste11_40*(this->d" << p->name << "_K5);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K6 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Computing the error\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      if(p==this->stateVarsHolder.begin()){
	this->behaviourFile << "error = tfel::math::abs(";
      } else {
	this->behaviourFile << "error += tfel::math::abs(";
      }
      this->behaviourFile << "cste1_360*(this->d" << p->name << "_K1)"
			  << "-cste128_4275*(this->d" << p->name << "_K3)"
			  << "-cste2197_75240*(this->d" << p->name << "_K4)"
			  << "+cste1_50*(this->d" << p->name << "_K5)"
			  << "+cste2_55*(this->d" << p->name << "_K6));\n";
    }
    this->behaviourFile << "error/=" << stateVarsSize << ";\n";
    this->behaviourFile << "// test for convergence\n";
    this->behaviourFile << "if(error<" << this->className << "::epsilon){\n";
    this->behaviourFile << "// Final Step\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << " += cste16_135*(this->d" << p->name << "_K1)"
			  << "+cste6656_12825*(this->d" << p->name << "_K3)"
			  << "+cste28561_56430*(this->d" << p->name << "_K4)"
			  << "-cste9_50*(this->d" << p->name << "_K5)"
			  << "+cste2_55*(this->d" << p->name << "_K6);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeFinalStress();\n";
    if(!this->updateAuxiliaryStateVars.empty()){
      this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    }
    this->behaviourFile << "t += dt_;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "// time multiplier\n";
    this->behaviourFile << "if((this->dt)-t>Type(0.25)*dt_){\n";
    this->behaviourFile << "real corrector;\n";
    this->behaviourFile << "if(error<100*std::numeric_limits<real>::min()){\n";
    this->behaviourFile << "corrector=real(1);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "corrector = 0.8*pow(" << this->className << "::epsilon/error,0.2);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(corrector<real(0.1f)){\n";
    this->behaviourFile << "dt_ *= real(0.1f);\n";
    this->behaviourFile << "} else if(corrector>real(10)){\n";
    this->behaviourFile << "dt_ *= real(10);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "dt_ *= corrector;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(dt_>this->dt-t){\n";
    this->behaviourFile << "dt_=this->dt-t;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "// failed is true\n";
    this->behaviourFile << "dt_ *= real(0.1f);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(dt_<(this->dt)*real(0.0000001f)){\n";
    this->behaviourFile << "string msg(\"" << this->className << "::integrate : \");\n";
    this->behaviourFile << "msg += \"time step reduction has gone too far.\";\n";
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
  } // end of MFrontRungeKuttaParser::writeBehaviourRK54Integrator

  void MFrontRungeKuttaParser::writeBehaviourRK42Integrator(void)
  {
    using namespace std;
    VarContainer::iterator p;
    SupportedTypes::TypeSize stateVarsSize;
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      stateVarsSize+=this->getTypeSize(p->type);
    }
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourFile << "static const unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;\n";
    this->behaviourFile << "static const Type cste1_6 = Type(1)/Type(6);\n";
    this->behaviourFile << "static const Type cste1_3 = Type(1)/Type(3);\n";

    this->behaviourFile << "time t   = time(0);\n";
    this->behaviourFile << "time dt_ = this->dt;\n";
    this->behaviourFile << "Type error;\n";
    this->behaviourFile << "while((this->dt)-t>Type(0.25)*dt_){\n";
    this->behaviourFile << "bool failed = false;\n";
    this->behaviourFile << "// Compute K1's values\n";
    this->behaviourFile << "this->eto_  = this->eto+t*(this->deto);\n";
    this->behaviourFile << "this->T_    = this->T+t*(this->dT);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name
			  << "+ t*(this->d" << p->name << ");\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name << ";\n";
    }
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K2's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t+0.5f*dt_)/(this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+0.5f*dt_)/(this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  <<"+(this->d" << p->name 
			  << ")*(t+0.5f*dt_)*(this->d" << p->name << ");\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << "_K1);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K2 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "// Compute K3's values\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+0.5f*(this->d" << p->name << "_K2);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K3 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K4's values\n";
    this->behaviourFile << "this->eto_ = this->eto+(this->deto)*(t+dt_)/(this->dt);\n";
    this->behaviourFile << "this->T_   = this->T+(this->dT)*(t+dt_)/(this->dt);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = this->" << p->name 
			  <<"+(this->d" << p->name 
			  << ")*(t+dt_)*(this->d" << p->name << ");\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << "_ = this->" << p->name
			  << "+this->d" << p->name << "_K3;\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "failed = !this->computeStress();\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    this->behaviourFile << "if(!failed){\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K4 = (dt_)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Computing the error\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      if(p==this->stateVarsHolder.begin()){
	this->behaviourFile << "error = tfel::math::abs(";
      } else {
	this->behaviourFile << "error += tfel::math::abs(";
      }
      this->behaviourFile << "cste1_6*(this->d" << p->name << "_K1+"
			  << "this->d" << p->name << "_K4-"
			  << "this->d" << p->name << "_K2-"
			  << "this->d" << p->name << "_K3));\n";
    }
    this->behaviourFile << "error/=" << stateVarsSize << ";\n";
    this->behaviourFile << "// test for convergence\n";
    this->behaviourFile << "if(error<" << this->className << "::epsilon){\n";
    this->behaviourFile << "// Final Step\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name 
			  << " += cste1_6*(this->d" << p->name << "_K1 + this->d" << p->name << "_K4)+"
			  << "    cste1_3*(this->d" << p->name << "_K3 + this->d" << p->name << "_K2);\n";
    }
    this->behaviourFile << "this->computeFinalStress();\n";
    if(!this->updateAuxiliaryStateVars.empty()){
      this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    }
    this->behaviourFile << "t += dt_;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "// time multiplier\n";
    this->behaviourFile << "if(this->dt-t>Type(0.25)*dt_){\n";
    this->behaviourFile << "real corrector;\n";
    this->behaviourFile << "if(error<100*std::numeric_limits<real>::min()){\n";
    this->behaviourFile << "corrector=real(1.);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "corrector = 0.8*pow((" << this->className << "::epsilon)/error,1./3.);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(corrector<=real(0.1f)){\n";
    this->behaviourFile << "dt_ *= real(0.1f);\n";
    this->behaviourFile << "} else if(corrector>real(10)){\n";
    this->behaviourFile << "dt_ *= real(10);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "dt_ *= corrector;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(dt_>this->dt-t){\n";
    this->behaviourFile << "dt_=this->dt-t;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "// failed is true\n";
    this->behaviourFile << "dt_ *= real(0.1f);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(dt_<(this->dt)*real(0.0000001f)){\n";
    this->behaviourFile << "string msg(\"" << this->className << "::integrate : \");\n";
    this->behaviourFile << "msg += \"time step reduction has gone too far.\";\n";
    this->behaviourFile << "throw(tfel::material::DivergenceException(msg));\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
  } // end of MFrontRungeKuttaParser::writeBehaviourRK42Integrator

  void MFrontRungeKuttaParser::writeBehaviourRK4Integrator(void)
  {
    using namespace std;
    VarContainer::iterator p;
    this->behaviourFile << "// Compute K1's values\n";
    this->behaviourFile << "this->computeDerivative();\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K1 = (this->dt)*(this->d" << p->name << ");\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << "_K1);\n";
    }
    this->behaviourFile << "this->eto_ += 0.5f*(this->deto);\n";
    this->behaviourFile << "this->T_   += 0.5f*(this->dT);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeStress();\n\n";
    this->behaviourFile << "// Compute K2's values\n";
    this->behaviourFile << "this->computeDerivative();\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K2 = (this->dt)*(this->d" << p->name << ");\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = " << "this->" << p->name 
			  << "+ 0.5f*(this->d" << p->name << "_K2);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeStress();\n\n";
    this->behaviourFile << "// Compute K3's values\n";
    this->behaviourFile << "this->computeDerivative();\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K3 = (this->dt)*(this->d" << p->name << ");\n";
    }
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ = " << "this->" << p->name 
			  << "+ (this->d" << p->name << "_K3);\n";
    }
    this->behaviourFile << "this->eto_ += 0.5f*(this->deto);\n";
    this->behaviourFile << "this->T_   += 0.5f*(this->dT);\n";
    for(p =this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << "_ += 0.5f*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeStress();\n\n";
    this->behaviourFile << "// Compute K4's values\n";
    this->behaviourFile << "this->computeDerivative();\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->d" << p->name 
			  << "_K4 = (this->dt)*(this->d" << p->name << ");\n";
    }
    this->behaviourFile << "// Final Step\n";
    for(p =this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "this->" << p->name << " += " 
			  << "1.f/6.f*(this->d" << p->name 
			  << "_K1+this->d" << p->name << "_K4)+\n";
      this->behaviourFile << "1.f/3.f*(this->d" << p->name 
			  << "_K2+this->d" << p->name << "_K3);\n";
    }
    this->behaviourFile << "// Update stress field\n";
    this->behaviourFile << "this->computeFinalStress();\n\n";
  }  // end of MFrontRungeKuttaParser::writeBehaviourRK4Integrator

  void MFrontRungeKuttaParser::writeBehaviourIntegrator(void)
  {
    using namespace std;
    std::vector<BoundsDescription>::const_iterator p2;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "integrate(void){\n";
    if(this->algorithm == "Euler"){
      this->writeBehaviourEulerIntegrator();
    } else if(this->algorithm == "RungeKutta2"){
      this->writeBehaviourRK2Integrator();
    } else if(this->algorithm == "RungeKutta4/2"){
      this->writeBehaviourRK42Integrator();
    } else if(this->algorithm == "RungeKutta5/4"){
      this->writeBehaviourRK54Integrator();
    } else if (this->algorithm == "RungeKutta4"){
      this->writeBehaviourRK4Integrator();
    } else {
      string msg("MFrontRungeKuttaParser::writeBehaviourIntegrator : ");
      msg += "internal error\n";
      msg += this->algorithm;
      msg += " is not a known algorithm. This shall not happen at this stage.";
      msg += " Please contact MFront developper to help them debug this.";
      throw(runtime_error(msg));
    }
    for(p2  = this->boundsDescriptions.begin();
	p2 != this->boundsDescriptions.end();++p2){
      if(p2->varCategory==BoundsDescription::StateVar){
	p2->writeBoundsChecks(this->behaviourFile);
      }
    }
    this->behaviourFile << "} // end of " << this->className << "::integrate\n\n";
  } // end of void MFrontRungeKuttaParser::writeBehaviourIntegrator(void)
  
    MFrontRungeKuttaParser::~MFrontRungeKuttaParser()
  {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();    
  } // end of ~MFrontRungeKuttaParser

  std::map<std::string,std::vector<std::string> >
  MFrontRungeKuttaParser::getGlobalIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGlobalIncludes();
  } // end of MFrontMFrontRungeKuttaParser::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontRungeKuttaParser::getGlobalDependencies(void)
  {
    return MFrontBehaviourParserCommon::getGlobalDependencies();
  } // end of MFrontMFrontRungeKuttaParser::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontRungeKuttaParser::getGeneratedSources(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedSources();
  } // end of MFrontRungeKuttaParser::getGeneratedSources

  std::vector<std::string>
  MFrontRungeKuttaParser::getGeneratedIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedIncludes();
  } // end of MFrontRungeKuttaParser::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontRungeKuttaParser::getLibrariesDependencies(void)
  {
    return MFrontBehaviourParserCommon::getLibrariesDependencies();
  } // end of MFrontRungeKuttaParser::getLibrariesDependencies

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontRungeKuttaParser::getSpecificTargets(void)
  {
    return MFrontBehaviourParserCommon::getSpecificTargets();
  } // end of MFrontRungeKuttaParser::getSpecificTargets(void)

} // end of namespace mfront  
