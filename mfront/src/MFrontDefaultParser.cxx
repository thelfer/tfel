/*!
 * \file   MFrontBase.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#include<string>
#include<fstream>
#include<stdexcept>

#include"TFEL/System/System.hxx"

#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontDefaultParser.hxx"

namespace mfront{

  MFrontDefaultParser::MFrontDefaultParser()
    : MFrontVirtualParser(),
      MFrontBehaviourParserBase<MFrontDefaultParser>()
  {}

  std::string
  MFrontDefaultParser::getDescription(void)
  {
    return "this parser is the most generic one as it does not make any restriction "
           "on the behaviour or the integration method that may be used";
  } // end of MFrontDefaultParser::getDescription
  
  std::string
  MFrontDefaultParser::getName(void)
  {
    return "DefaultParser";
  }

  void
  MFrontDefaultParser::setInterfaces(const std::set<std::string>& i)
  {
    MFrontBehaviourParserBase<MFrontDefaultParser>::setInterfaces(i);
  } // end of MFrontDefaultParser::setInterfaces

  void 
  MFrontDefaultParser::setVerboseMode(void){
    this->verboseMode = true;
  }

  void 
  MFrontDefaultParser::setDebugMode(void){
    this->debugMode = true;
  }

  void 
  MFrontDefaultParser::setWarningMode(void){
    this->warningMode = true;
  }

  void 
  MFrontDefaultParser::treatFile(const std::string& fileName_){
    this->fileName = fileName_;
    MFrontBehaviourParserBase<MFrontDefaultParser>::treatFile(fileName_);
  }

  void 
  MFrontDefaultParser::generateOutputFiles(void){
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
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
			       this->parametersHolder,
			       this->glossaryNames,
			       this->entryNames,
			       this->behaviourCharacteristic);
    }
  } // end of MFrontDefaultParser::generateOutputFiles

  void 
  MFrontDefaultParser::writeOutputFiles(){
    using namespace std;
    using namespace tfel::system;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    systemCall::mkdir("src");
    systemCall::mkdir("include");
    systemCall::mkdir("include/TFEL/");
    systemCall::mkdir("include/TFEL/Material");
    if(this->className.empty()){
      string msg("MFrontDefaultParser::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->behaviourFileName  = this->className;
    this->behaviourFileName += ".hxx";
    this->behaviourFile.open(("include/TFEL/Material/"+this->behaviourFileName).c_str());
    if(!this->behaviourFile){
      string msg("MFrontDefaultParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }

    this->behaviourDataFileName  = this->className;
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->behaviourDataFile.open(("include/TFEL/Material/"+this->behaviourDataFileName).c_str());

    if(!this->behaviourDataFile){
      string msg("MFrontDefaultParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }

    this->integrationDataFileName  = this->className;
    this->integrationDataFileName += "IntegrationData.hxx";
    this->integrationDataFile.open(("include/TFEL/Material/"+this->integrationDataFileName).c_str());

    if(!this->integrationDataFile){
      string msg("MFrontDefaultParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->integrationDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }

    this->srcFileName  = this->className;
    this->srcFileName += ".cxx";
    this->srcFile.open(("src/"+this->srcFileName).c_str());

    if(!this->srcFile){
      string msg("MFrontDefaultParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }

    this->generateOutputFiles();

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
			       this->parametersHolder,
			       this->glossaryNames,
			       this->entryNames,
			       this->behaviourCharacteristic);
    }
  }

  MFrontDefaultParser::~MFrontDefaultParser()
  {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();
  } // end of MFrontDefaultParser::~MFrontDefaultParser

  std::map<std::string,std::vector<std::string> >
  MFrontDefaultParser::getGlobalIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGlobalIncludes();
  } // end of MFrontMFrontDefaultParser::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontDefaultParser::getGlobalDependencies(void)
  {
    return MFrontBehaviourParserCommon::getGlobalDependencies();
  } // end of MFrontMFrontDefaultParser::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontDefaultParser::getGeneratedSources(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedSources();
  } // end of MFrontDefaultParser::getGeneratedSources

  std::vector<std::string>
  MFrontDefaultParser::getGeneratedIncludes(void)
  {
    return MFrontBehaviourParserCommon::getGeneratedIncludes();
  } // end of MFrontDefaultParser::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontDefaultParser::getLibrariesDependencies(void)
  {
    return MFrontBehaviourParserCommon::getLibrariesDependencies();
  } // end of MFrontDefaultParser::getLibrariesDependencies

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontDefaultParser::getSpecificTargets(void)
  {
    return MFrontBehaviourParserCommon::getSpecificTargets();
  } // end of MFrontDefaultParser::getSpecificTargets(void)

} // end of namespace mfront  
