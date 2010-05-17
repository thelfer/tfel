/*!
 * \file   MFrontIsotropicBehaviourParserBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2007
 */

#include<sstream>

#include"TFEL/System/System.hxx"
#include"MFront/MFrontIsotropicBehaviourParserBase.hxx"

namespace mfront{

  MFrontIsotropicBehaviourParserBase::MFrontIsotropicBehaviourParserBase()
    : MFrontVirtualParser(),
      MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>(),
      theta(0.5f),
      epsilon(1.e-8),
      iterMax(100)
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    this->registerStaticVariable("theta");
    this->registerStaticVariable("epsilon");
    this->registerStaticVariable("iterMax");
    this->registerVariable("young");
    this->registerVariable("nu");
    this->registerVariable("rho");
    this->registerVariable("alpha");
    this->registerVariable("lambda");
    this->registerVariable("mu");
    this->coefsHolder.push_back(VarHandler("stress","young",0u));
    this->glossaryNames.insert(MVType("young","YoungModulus"));
    this->coefsHolder.push_back(VarHandler("real","nu",0u));
    this->glossaryNames.insert(MVType("nu","PoissonRatio"));
    this->coefsHolder.push_back(VarHandler("density","rho",0u));
    this->glossaryNames.insert(MVType("rho","MassDensity"));
    this->coefsHolder.push_back(VarHandler("thermalexpansion","alpha",0u));
    this->glossaryNames.insert(MVType("alpha","ThermalExpansion"));
    this->localVarsHolder.push_back(VarHandler("stress","lambda",0u));
    this->localVarsHolder.push_back(VarHandler("stress","mu",0u));
    // local var initialisation
    this->initLocalVars +="this->lambda=tfel::material::lame::computeLambda(this->young,this->nu);\n";
    this->initLocalVars +="this->mu=tfel::material::lame::computeMu(this->young,this->nu);\n";
    // Call Back
    this->registerNewCallBack("@MaterialLaw",&MFrontIsotropicBehaviourParserBase::treatMaterialLaw);
    this->registerNewCallBack("@FlowRule",&MFrontIsotropicBehaviourParserBase::treatFlowRule);
    this->registerNewCallBack("@Theta",&MFrontIsotropicBehaviourParserBase::treatTheta);
    this->registerNewCallBack("@Epsilon",&MFrontIsotropicBehaviourParserBase::treatEpsilon);
    this->registerNewCallBack("@IterMax",&MFrontIsotropicBehaviourParserBase::treatIterMax);
    this->disableCallBack("@StateVar");
    this->disableCallBack("@Integrator");
    this->disableCallBack("@OrthotropicBehaviour");
    this->disableCallBack("@IsotropicBehaviour");
  } // end of MFrontIsotropicBehaviourParserBase::MFrontIsotropicBehaviourParserBase

  MFrontIsotropicBehaviourParserBase::~MFrontIsotropicBehaviourParserBase()
  {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();    
  } // end of MFrontIsotropicBehaviourParserBase::~MFrontIsotropicBehaviourParserBase()

  void
  MFrontIsotropicBehaviourParserBase::setInterfaces(const std::set<std::string>& i)
  {
    MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>::setInterfaces(i);
  } // end of MFrontIsotropicBehaviourParserBase::setInterfaces

  void
  MFrontIsotropicBehaviourParserBase::treatTheta(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontIsotropicBehaviourParserBase::treatTheta",
			    "Cannot read theta value.");
    istringstream flux(current->value);
    flux >> this->theta;
    if(flux.fail()){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatTheta",
			      "Failed to read theta value.");
    }
    if((this->theta<0.)||(this->theta>1.)){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatTheta",
			      "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontIsotropicBehaviourParserBase::treatTheta",";");
  } // end of MFrontIsotropicBehaviourParserBase::treatTheta

  void
  MFrontIsotropicBehaviourParserBase::treatEpsilon(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontIsotropicBehaviourParserBase::treatEpsilon",
			    "Cannot read epsilon value.");
    istringstream flux(current->value);
    flux >> this->epsilon;
    if(flux.fail()){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatEpsilon",
			      "Failed to read epsilon value.");
    }
    if(this->epsilon<0){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatEpsilon",
			      "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontIsotropicBehaviourParserBase::treatEpsilon",";");
  } // MFrontIsotropicBehaviourParserBase::treatEpsilon

  void
  MFrontIsotropicBehaviourParserBase::treatIterMax(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontIsotropicBehaviourParserBase::treatIterMax",
			    "Cannot read iterMax value.");
    istringstream flux(current->value);
    flux >> this->iterMax;
    if(flux.fail()){
      this->throwRuntimeError("MFrontIsotropicBehaviourParserBase::treatIterMax",
			      "Failed to read iterMax value.");
    }
    ++(this->current);
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
  MFrontIsotropicBehaviourParserBase::variableModifier1(const std::string& var,
							const bool addThisPtr)
  {
    if((var=="T")||(this->isExternalStateVariable(var))||
       (this->isInternalStateVariable(var))){
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
  } // end of MFrontIsotropicBehaviourParserBase::variableModifier1

  std::string
  MFrontIsotropicBehaviourParserBase::variableModifier2(const std::string& var,
							const bool addThisPtr)
  {
    if((var=="T")||(this->isExternalStateVariable(var))||
       (this->isInternalStateVariable(var))||
       (this->isInternalStateVariableIncrement(var))){
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
  } // end of MFrontIsotropicBehaviourParserBase::variableModifier2

  void
  MFrontIsotropicBehaviourParserBase::treatFlowRuleBase(std::string &flow)
  {
    flow = this->readNextBlock(&ParserBase::variableModifier1,true);
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
    VarContainer::iterator p;
    string currentVarName;

    this->staticVars.push_back(StaticVarHandler("real","theta",0u,this->theta));
    this->staticVars.push_back(StaticVarHandler("real","epsilon",0u,this->epsilon));

    if(this->flowRule.empty()){
      string msg("MFrontIsotropicBehaviourParserBase::endsInputFileProcessing : ");
      msg += "@FlowRule was not defined.";
      throw(runtime_error(msg));
    }
    this->registerVariable("T_");
    this->localVarsHolder.push_back(VarHandler("temperature","T_",0u));
    this->initLocalVars = "this->T_ = this->T+(" + this->className + "::theta)*(this->dT);\n" + this->initLocalVars;

    for(p =this->externalStateVarsHolder.begin();
	p!=this->externalStateVarsHolder.end();++p){
      currentVarName = p->name + "_";
      this->registerVariable(currentVarName);
      this->localVarsHolder.push_back(VarHandler(p->type,currentVarName,0u));
      this->initLocalVars = "this->" + currentVarName + " = this->" + p->name +
	"+(" + this->className + "::theta)*(this->d" + p->name + ");\n" + this->initLocalVars;
    }
  } // end of MFrontIsotropicBehaviourParserBase::endsInputFileProcessing

  void 
  MFrontIsotropicBehaviourParserBase::generateOutputFiles(void){
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
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
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
  } // end of MFrontIsotropicBehaviourParserBase::generateOutputFiles

  void
  MFrontIsotropicBehaviourParserBase::writeOutputFiles() 
  {    
    using namespace std;
    using namespace tfel::system;
    systemCall::mkdir("include");
    systemCall::mkdir("include/TFEL/");
    systemCall::mkdir("include/TFEL/Material/");
    systemCall::mkdir("src");
    if(this->className.empty()){
      string msg("MFrontIsotropicBehaviourParserBase::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->behaviourFileName  = this->className;
    this->behaviourFileName += ".hxx";
    this->behaviourFile.open(("include/TFEL/Material/"+this->behaviourFileName).c_str());
    if(!this->behaviourFile){
      string msg("MFrontIsotropicBehaviourParserBase::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->behaviourDataFileName  = this->className;
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->behaviourDataFile.open(("include/TFEL/Material/"+this->behaviourDataFileName).c_str());
    if(!this->behaviourDataFile){
      string msg("MFrontIsotropicBehaviourParserBase::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->integrationDataFileName  = this->className;
    this->integrationDataFileName += "IntegrationData.hxx";
    this->integrationDataFile.open(("include/TFEL/Material/"+this->integrationDataFileName).c_str());
    if(!this->integrationDataFile){
      string msg("MFrontIsotropicBehaviourParserBase::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->integrationDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFileName  = this->className;
    this->srcFileName += ".cxx";
    this->srcFile.open(("src/"+this->srcFileName).c_str());
    if(!this->srcFile){
      string msg("MFrontIsotropicBehaviourParserBase::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->generateOutputFiles();
  } // end of MFrontIsotropicBehaviourParserBase::writeOutputFiles

  void
  MFrontIsotropicBehaviourParserBase::writeBehaviourParserSpecificTypedefs(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "typedef typename tfel::math::ComputeBinaryResult<"
			<< "strain,time,tfel::math::OpDiv>::Result DstrainDt;\n";
    this->behaviourFile << "typedef typename tfel::math::ComputeBinaryResult<"
			<< "DstrainDt,stress,tfel::math::OpDiv>::Result DF_DSEQ_TYPE;\n\n";
  } // end of MFrontIsotropicBehaviourParserBase::writeBehaviourParserSpecificTypedefs

  void
  MFrontIsotropicBehaviourParserBase::setVerboseMode(void) 
  {
    this->verboseMode = true;
  } // end of MFrontIsotropicBehaviourParserBase::setVerboseMode

  void
  MFrontIsotropicBehaviourParserBase::setDebugMode(void) 
  {
    this->debugMode = true;
  } // end of MFrontIsotropicBehaviourParserBase::setDebugMode

  void
  MFrontIsotropicBehaviourParserBase::setWarningMode(void) 
  {
    this->warningMode = true;
  } // end of MFrontIsotropicBehaviourParserBase::setWarningMode

  void
  MFrontIsotropicBehaviourParserBase::treatFile(const std::string& fileName_) 
  {
    this->fileName = fileName_;
    MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>::treatFile(fileName_);
  } // end of MFrontIsotropicBehaviourParserBase::treatFile

  void 
  MFrontIsotropicBehaviourParserBase::writeBehaviourStaticVars(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "static const unsigned short iterMax = ";
    this->behaviourFile << this->iterMax << ";\n";
    MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>::writeBehaviourStaticVars();
  } // end of MFrontIsotropicBehaviourParserBase::writeBehaviourStaticVars

  std::map<std::string,std::vector<std::string> >
  MFrontIsotropicBehaviourParserBase::getGlobalIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGlobalIncludes();
  } // end of MFrontMFrontIsotropicBehaviourParserBase::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontIsotropicBehaviourParserBase::getGlobalDependencies(void)
  {
    return MFrontBehaviourParserCommon::getGlobalDependencies();
  } // end of MFrontMFrontIsotropicBehaviourParserBase::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontIsotropicBehaviourParserBase::getGeneratedSources(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedSources();
  } // end of MFrontIsotropicBehaviourParserBase::getGeneratedSources

  std::vector<std::string>
  MFrontIsotropicBehaviourParserBase::getGeneratedIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedIncludes();
  } // end of MFrontIsotropicBehaviourParserBase::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontIsotropicBehaviourParserBase::getLibrariesDependencies(void)
  {
    return MFrontBehaviourParserCommon::getLibrariesDependencies();
  } // end of MFrontIsotropicBehaviourParserBase::getLibrariesDependencies

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontIsotropicBehaviourParserBase::getSpecificTargets(void)
  {
    return MFrontBehaviourParserCommon::getSpecificTargets();
  } // end of MFrontIsotropicBehaviourParserBase::getSpecificTargets(void)

} // end of namespace mfront
