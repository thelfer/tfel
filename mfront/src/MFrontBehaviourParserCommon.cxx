/*!
 * \file   MFrontBehaviourParserCommon.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 mai 2008
 */

#include<iostream>
#include<iterator>
#include<algorithm>
#include<vector>
#include<sstream>
#include<utility>
#include<stdexcept>
#include<limits>
#include<cctype>
#include<cmath>

#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"

#include"MFront/MFrontBehaviourParserCommon.hxx"

namespace mfront{

  tfel::material::ModellingHypothesis::Hypothesis
  MFrontBehaviourParserCommon::getModellingHypothesisFromString(const std::string& h)
  {
    using namespace std;
    using namespace tfel::material;
    if(h=="AxisymmetricalGeneralisedPlaneStrain"){
      return ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
    } else if(h=="Axisymmetrical"){
      return ModellingHypothesis::AXISYMMETRICAL;
    } else if(h=="PlaneStress"){
      return ModellingHypothesis::PLANESTRESS;
    } else if(h=="PlaneStrain"){
      return ModellingHypothesis::PLANESTRAIN;
    } else if(h=="GeneralisedPlaneStrain"){
      return ModellingHypothesis::GENERALISEDPLANESTRAIN;
    } else if(h=="Tridimensional"){
      return ModellingHypothesis::TRIDIMENSIONAL;
    } else {
      string msg("MFrontBehaviourParserCommon::getModellingHypothesisFromString : "
		 "invalid or unsupported hypothesis '"+h+"'. The following "
		 "hypotheses are supported:\n"
		 "- AxisymmetricalGeneralisedPlaneStrain\n"
		 "- Axisymmetrical\n"
		 "- PlaneStress\n"
		 "- PlaneStrain\n"
		 "- GeneralisedPlaneStrain\n"
		 "- Tridimensional");
      throw(runtime_error(msg));
    }
    return ModellingHypothesis::UNDEFINEDHYPOTHESIS;
  }

  void
  MFrontBehaviourParserCommon::requiresTVectorOrVectorIncludes(bool& b1,
							       bool& b2) const
  {
    VarContainer::const_iterator ps;
    b1 = false;
    b2 = false;
    for(ps=this->mb.getMaterialProperties().begin();
	(ps!=this->mb.getMaterialProperties().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->mb.getStateVariables().begin();
	(ps!=this->mb.getStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->mb.getAuxiliaryStateVariables().begin();
	(ps!=this->mb.getAuxiliaryStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->mb.getLocalVariables().begin();
	(ps!=this->mb.getLocalVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
    for(ps=this->mb.getExternalStateVariables().begin();
	(ps!=this->mb.getExternalStateVariables().end())&&(!(b1&&b2));++ps){
      if(ps->arraySize>1){
	if(this->useDynamicallyAllocatedVector(ps->arraySize)){
	  b2 = true;
	} else {
	  b1 = true;
	}
      }
    }
  } // end of MFrontBehaviourParserCommon::requiresTVectorOrVectorIncludes

  void MFrontBehaviourParserCommon::setVerboseMode(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->verboseMode = true;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->setVerboseMode();
    }
  }

  void MFrontBehaviourParserCommon::setDebugMode(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->debugMode = true;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->setDebugMode();
    }
  }

  void MFrontBehaviourParserCommon::setWarningMode(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->warningMode = true;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->setWarningMode();
    }
  }
  
  void
  MFrontBehaviourParserCommon::endsInputFileProcessing()
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string>::const_iterator i;
    if(this->hypotheses.empty()){
      this->setDefaultHypotheses();
    }
    if(this->hypotheses.empty()){
      string msg("MFrontBehaviourParserCommon::endsInputFileProcessing : "
		 "no modelling hypothesis defined by the user and "
		 "no default modelling hypothesis");
      throw(runtime_error(msg));
    }
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->allowDynamicallyAllocatedArrays(this->areDynamicallyAllocatedVectorsAllowed());
    }
  } // end of MFrontBehaviourParserCommon::endsInputFileProcessing

  void
  MFrontBehaviourParserCommon::setDefaultHypotheses(void)
  {
    using namespace tfel::material;
    typedef ModellingHypothesis MH;
    static const MH::Hypothesis h[6u] = {MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
					 MH::AXISYMMETRICAL,
					 MH::PLANESTRAIN,
					 MH::GENERALISEDPLANESTRAIN,
					 MH::TRIDIMENSIONAL};
    this->hypotheses.clear();
    this->hypotheses.insert(h,h+6u);
  } // end of MFrontBehaviourParserCommon::setDefaultHypotheses  

  void 
  MFrontBehaviourParserCommon::writeOutputFiles(){
    using namespace std;
    using namespace tfel::system;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    systemCall::mkdir("src");
    systemCall::mkdir("include");
    systemCall::mkdir("include/TFEL/");
    systemCall::mkdir("include/TFEL/Material");
    if(this->className.empty()){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->behaviourFileName  = this->className;
    this->behaviourFileName += ".hxx";
    this->behaviourFile.open(("include/TFEL/Material/"+this->behaviourFileName).c_str());
    if(!this->behaviourFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->behaviourDataFileName  = this->className;
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->behaviourDataFile.open(("include/TFEL/Material/"+this->behaviourDataFileName).c_str());
    if(!this->behaviourDataFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->integrationDataFileName  = this->className;
    this->integrationDataFileName += "IntegrationData.hxx";
    this->integrationDataFile.open(("include/TFEL/Material/"+this->integrationDataFileName).c_str());
    if(!this->integrationDataFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->integrationDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFileName  = this->className;
    this->srcFileName += ".cxx";
    this->srcFile.open(("src/"+this->srcFileName).c_str());
    if(!this->srcFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
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
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->endTreatement(this->fileName,
			       this->library,
			       this->material,
			       this->className,
			       this->authorName,
			       this->date,
			       this->glossaryNames,
			       this->entryNames,
			       this->mb);
    }
  }

  void
  MFrontBehaviourParserCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string& n)
  {
    if(!this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->mb.setUsableInPurelyImplicitResolution(false);
    }
    this->mb.declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(n);
  } // end of MFrontBehaviourParserCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  std::string
  MFrontBehaviourParserCommon::standardModifier(const std::string& var,
						 const bool addThisPtr)
  {
    if((this->mb.isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontBehaviourParserCommon::standardModifier
    
  static void
  MFrontBehaviourParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(const std::map<std::string,std::string>& g,
								     const std::map<std::string,std::string>& e,
								     const std::string& n)
  {
    using namespace std;
    map<string,string>::const_iterator p;
    for(p=g.begin();p!=g.end();++p){
      if(p->second==n){
	string msg("MFrontBehaviourParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as a glossary name";
	throw(runtime_error(msg));
      }
    }
    for(p=e.begin();p!=e.end();++p){
      if(p->second==n){
	string msg("MFrontBehaviourParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as an entry name";
	throw(runtime_error(msg));
      }
    }
  }

  void
  MFrontBehaviourParserCommon::treatModellingHypothesis(void)
  {
    using namespace std;
    if(!this->hypotheses.empty()){
      string msg("MFrontBehaviourParserCommon::treatModellingHypothesis : "
		 "one or more modelling hypotheses have already been declared");
      throw(runtime_error(msg));
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypothesis");
    const string& h = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypothesis");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatModellingHypothesis",";");
    this->hypotheses.insert(MFrontBehaviourParserCommon::getModellingHypothesisFromString(h));
  } // end of MFrontBehaviourParserCommon::treatModellingHypothesis

  void
  MFrontBehaviourParserCommon::treatModellingHypotheses(void)
  {
    using namespace std;
    if(!this->hypotheses.empty()){
      string msg("MFrontBehaviourParserCommon::treatModellingHypotheses : "
		 "one or more modelling hypotheses have already been declared");
      throw(runtime_error(msg));
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypotheses");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatModellingHypotheses","{");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypotheses");
    while(this->current->value!="}"){
      const string& h = this->current->value;
      this->hypotheses.insert(MFrontBehaviourParserCommon::getModellingHypothesisFromString(h));
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypotheses");
      if(this->current->value!="}"){
	this->readSpecifiedToken("MFrontBehaviourParserCommon::treatModellingHypotheses",",");
	this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypotheses");
	if(this->current->value=="}"){
	  string msg("MFrontBehaviourParserCommon::treatModellingHypotheses : ");
	  msg += "unexpected token '}'";
	  throw(runtime_error(msg));
	}
      }
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypotheses");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatModellingHypotheses",";");
  } // end of MFrontBehaviourParserCommon::treatModellingHypotheses

  void
  MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution(void)
  {
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution",";");
    if(this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution",
			      "keyword '@UsableInPurelyImplicitResolution' already called");
    }
    this->explicitlyDeclaredUsableInPurelyImplicitResolution = true;
    this->mb.setUsableInPurelyImplicitResolution(true);
  } // end of MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution

  void
  MFrontBehaviourParserCommon::treatParameterMethod(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,double>::value_type MVType;
    const string& n = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod",".");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
    if(this->current->value=="setDefaultValue"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatParameterMethod",
				"could not read default value for parameter '"+n+"'");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod",")");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod",";");
      if(!this->mb.getParametersDefaultValues().insert(MVType(n,value)).second){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatParameterMethod",
				"default value already defined for parameter '"+n+"'");
      }
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatParameterMethod",
			      "could not read default value for parameter '"+n+"'");
    }
  } // end of MFrontBehaviourParserCommon::treatParameterMethod

  void
  MFrontBehaviourParserCommon::treatVariableMethod(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    map<string,string>::const_iterator p;
    const string& n = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",".");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    if(this->current->value=="setGlossaryName"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      if(this->current->flag!=Token::String){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "expected to read a string";
	throw(runtime_error(msg));
      }
      const string& g = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->isValidIdentifier(g)){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "invalid glossary name";
	throw(runtime_error(msg));
      }
      MFrontBehaviourParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
									 this->entryNames,g);
      ++(this->current);
      if(this->entryNames.find(n)!=this->entryNames.end()){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "an entry name has already been specified for variable '"+n+"'";
	throw(runtime_error(msg));
      }
      if(!this->glossaryNames.insert(MVType(n,g)).second){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "glossary name for variable '"+n+"' already specified";
	throw(runtime_error(msg));
      }
    } else if(this->current->value=="setEntryName"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      if(this->current->flag!=Token::String){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "expected to read a string";
	throw(runtime_error(msg));
      }
      const string& e = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->isValidIdentifier(e)){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "invalid glossary name";
	throw(runtime_error(msg));
      }
      MFrontBehaviourParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
									 this->entryNames,e);
      ++(this->current);
      if(this->glossaryNames.find(n)!=this->glossaryNames.end()){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "a glossary name has already been specified for variable '"+n+"'";
	throw(runtime_error(msg));
      }
      if(!this->entryNames.insert(MVType(n,e)).second){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "entry name for variable '"+n+"' already specified";
	throw(runtime_error(msg));
      }
    } else {
      this->treatUnknownVariableMethod(n);
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",")");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",";");
  } // end of MFrontBehaviourParserCommon::treatVariableMethod

  void
  MFrontBehaviourParserCommon::treatUnknownVariableMethod(const std::string& n)
  {
    using namespace std;
    string msg("MFrontBehaviourParserCommon::treatUnknownVariableMethod : ");
    msg += "unknown method '"+this->current->value+"' for variable '"+n+"', ";
    msg += "valid methods are 'setGlossaryName' or 'setEntryName'";
    throw(runtime_error(msg));
  } // end of MFrontBehaviourParserCommon::treatUnknownVariableMethod

  void
  MFrontBehaviourParserCommon::treatUnknownKeyword(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    pair<bool,CxxTokenizer::TokensContainer::const_iterator> p;
    TokensContainer::const_iterator p2;
    vector<string>::const_iterator i;
    bool treated = false;
    string key;
    --(this->current);
    key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnknownKeyword");
    if(this->current->value=="["){
      set<string> s;
      while(this->current->value!="]"){
	++(this->current);
	this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnknownKeyword");
	string t;
	if(this->current->flag==Token::String){
	  t = this->current->value.substr(1,this->current->value.size()-2);
	} else {
	  t = this->current->value;
	}
	if(find(this->interfaces.begin(),this->interfaces.end(),t)!=this->interfaces.end()){
	  s.insert(t);
	}
	++(this->current);
	if((this->current->value!="]")&&(this->current->value!=",")){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnknownKeyword",
				  "unexpected token '"+this->current->value+"' (expected ']' or ',').");
	}
      }
      ++(this->current);
      if(s.empty()){
	this->ignoreKeyWord(key);
      } else {
	for(set<string>::const_iterator pi  = s.begin();pi != s.end();++pi){
	  MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*pi);
	  p = interface->treatKeyword(key,this->current,
				      this->fileTokens.end());
	  if(!p.first){
	    string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += " has not been treated by interface '"+*pi+"'";
	    throw(runtime_error(msg));
	  }
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by two interfaces but";
	      msg += " results were differents";
	      throw(runtime_error(msg));
	    }
	  }
	  p2 = p.second;
	  treated = true;
	}
	this->current = p2;
      }
    } else {
      for(i  = this->interfaces.begin();
	  i != this->interfaces.end();++i){
	MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
	p = interface->treatKeyword(key,this->current,
				    this->fileTokens.end());
	if(p.first){
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by two interfaces but";
	      msg += " results were differents";
	      throw(runtime_error(msg));
	    }
	  }
	  p2 = p.second;
	  treated = true;
	}
      }
      if(!treated){
	ParserBase::treatUnknownKeyword();
      }
      this->current = p2;
    }

  } // end of MFrontBehaviourParserCommon::treatUnknownKeyword

  void
  MFrontBehaviourParserCommon::treatUseQt(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUseQt : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->mb.setUseQt(true);
    } else if(this->current->value=="false"){
      this->mb.setUseQt(false);
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUseQt",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUseQt",";");
  } // end of MFrontBehaviourParserCommon::treatUseQt

  void
  MFrontBehaviourParserCommon::treatIsotropicBehaviour(void)
  {
    using namespace std;
    if(this->mb.getBehaviourType()!=mfront::ISOTROPIC){
      string msg("MFrontBehaviourParserCommon::treatIsotropicBehaviour  : ");
      msg += "this behaviour has been declared orthotropic";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatIsotropicBehaviour",";");
  } // end of MFrontBehaviourParserCommon::treatIsotropicBehaviour

  void
  MFrontBehaviourParserCommon::treatOrthotropicBehaviour(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatOrthotropicBehaviour",";");
    this->mb.setBehaviourType(mfront::ORTHOTROPIC);
    // by defaut the elastic behaviour is also orthotropic
    this->mb.setElasticBehaviourType(mfront::ORTHOTROPIC);
  } // end of MFrontBehaviourParserCommon::treatOrthotropicBehaviour

  void
  MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour",";");
    if(this->mb.getBehaviourType()!=mfront::ORTHOTROPIC){
      string msg("MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour  : ");
      msg += "this behaviour has not been declared orthotropic";
      throw(runtime_error(msg));
    }
    if(this->mb.getElasticBehaviourType()!=mfront::ORTHOTROPIC){
      string msg("MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour  : ");
      msg += "the elastic behaviour has already been declared isotropic";
      throw(runtime_error(msg));
    }
    this->mb.setElasticBehaviourType(mfront::ISOTROPIC);
  } // end of MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour

  void
  MFrontBehaviourParserCommon::treatRequireStiffnessTensor(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireStiffnessTensor",";");
    this->mb.setRequireStiffnessTensor(true);
  } // end of MFrontBehaviourParserCommon::treatRequireStiffnessTensor

  void
  MFrontBehaviourParserCommon::treatRequireThermalExpansionTensor(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireThermalExpansionTensor",";");
    this->mb.setRequireThermalExpansionTensor(true);
  } // end of MFrontBehaviourParserCommon::treatRequireThermalExpansionTensor

  void
  MFrontBehaviourParserCommon::updateClassName(void)
  {
    if((!this->behaviour.empty())||
       (!this->material.empty())){
      this->className = this->material+this->behaviour;
    }
    if(!isValidIdentifier(this->className)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::updateClassName",
			      "resulting class name is not valid (read '"+this->className+"')");
    }
  } // end of MFrontBehaviourParserCommon::updateClassName

  void
  MFrontBehaviourParserCommon::treatMaterial(void)
  {
    ParserBase::treatMaterial();
    this->updateClassName();
  } // end of MFrontBehaviourParserCommon::treatMaterial

  void MFrontBehaviourParserCommon::treatBehaviour(void)
  {
    using namespace std;
    if(!this->behaviour.empty()){
      string msg("ParserBase::treatBehaviour : ");
      msg += "behaviour name alreay defined";
      throw(runtime_error(msg));
    }
    this->behaviour = this->readOnlyOneToken();
    if(!isValidIdentifier(this->behaviour)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatBehaviour",
			      "Given behaviour name is not valid (read '"+this->behaviour+"')");
    }
    this->updateClassName();
  } // end of MFrontBehaviourParserCommon::treatMaterial

  void 
  MFrontBehaviourParserCommon::readStringList(std::vector<std::string>& cont)
  {
    using namespace std;
    string type;
    string s;
    bool endOfTreatement;
    vector<string>::iterator p;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::readStringList",
			    "Cannot read interface name.");
    endOfTreatement=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatement)){
      s = this->current->value;
      if(!isValidIdentifier(s)){
	--(this->current);
	this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
				"interface name is not valid (read '"+s+"')");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::readStringList");
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatement=true;
	++(this->current);
      } else {
	this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
				"',' or ';' expected afer '"+s+"'");
      }
      if((p = std::find(cont.begin(),cont.end(),s))!=cont.end()){
	this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
				"'"+s+"' has already been registred.\n");
      } 
      cont.push_back(s);
    }
    if(!endOfTreatement){
      --(this->current);
      this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
			      "Expected ';' before end of file.");
    }
  }

  void MFrontBehaviourParserCommon::treatCoef(void)
  {
    this->readVarList(this->mb.getMaterialProperties(),true,false);
  } // end of MFrontBehaviourParserCommon::treatCoef

  void MFrontBehaviourParserCommon::treatInterface(void)
  {
    this->readStringList(this->interfaces);
  } // end of MFrontBehaviourParserCommon::treatInterface

  void
  MFrontBehaviourParserCommon::setInterfaces(const std::set<std::string>& i)
  {
    using namespace std;
    copy(i.begin(),i.end(),back_inserter(this->interfaces));
  } // end of MFrontBehaviourParserCommon::setInterfaces

  void
  MFrontBehaviourParserCommon::treatIntegrator(void)
  {
    using namespace std;
    if(!this->integrator.empty()){
      string msg("MFrontBehaviourParserCommon::treatIntegrator : ");
      msg += "integrator already defined";
      throw(runtime_error(msg));
    }
    this->integrator = this->readNextBlock(true);
  } // end of MFrontBehaviourParserCommon::treatIntegrator

  void MFrontBehaviourParserCommon::treatStateVariables(void)
  {
    this->readVarList(this->mb.getStateVariables(),true,false);
  }

  void MFrontBehaviourParserCommon::treatAuxiliaryStateVariables(void)
  {
    this->readVarList(this->mb.getAuxiliaryStateVariables(),true,false);
  }

  void MFrontBehaviourParserCommon::treatExternalStateVariables(void)
  {
    this->readVarList(this->mb.getExternalStateVariables(),true,true);
  }

  void
  MFrontBehaviourParserCommon::treatBounds(void)
  {
    BoundsDescription boundsDescription;
    this->mb.getBounds().push_back(boundsDescription);
    this->mb.getBounds().back().category = BoundsDescription::Standard;
    this->treatBounds(this->mb.getBounds().back());
  } // end of MFrontBehaviourParserCommon::treatBounds

  void
  MFrontBehaviourParserCommon::treatPhysicalBounds(void)
  {
    BoundsDescription boundsDescription;
    this->mb.getBounds().push_back(BoundsDescription());
    this->mb.getBounds().back().category = BoundsDescription::Physical;
    this->treatBounds(this->mb.getBounds().back());
  } // end of MFrontBehaviourParserCommon::treatPhysicalBounds

  void MFrontBehaviourParserCommon::treatBounds(BoundsDescription& boundsDescription)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;

    bool found;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");

    boundsDescription.lineNumber = this->current->line;
    boundsDescription.varName = this->current->value;
    
    found = false;
    boundsDescription.arraySize = 1u;
    for(p3=this->mb.getMainVariables().begin();
	p3!=this->mb.getMainVariables().end();++p3){
      // check if the driving variable match
      if(boundsDescription.varName==p3->first.name){
	found=true;
	if(p3->first.increment_known){
	  boundsDescription.varCategory = BoundsDescription::ExternalStateVar;
	} else {
	  boundsDescription.varCategory = BoundsDescription::ExternalStateVarII;
	}
	boundsDescription.varType     = this->getTypeFlag(p3->first.type);
      }
      // check if the associated thermodynamic force match
      if(boundsDescription.varName==p3->second.name){
	found=true;
	boundsDescription.varCategory = BoundsDescription::StateVar;
	boundsDescription.varType     = this->getTypeFlag(p3->second.type);
      }
    }
    // temperature
    if(boundsDescription.varName=="T"){
      found=true;
      boundsDescription.varCategory = BoundsDescription::StateVar;
      boundsDescription.varType     = Scalar;
    }
    for(p   = this->mb.getMaterialProperties().begin();
	(p != this->mb.getMaterialProperties().end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::Coef;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->mb.getLocalVariables().begin();
	(p != this->mb.getLocalVariables().end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::LocalVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->mb.getStateVariables().begin();
	(p != this->mb.getStateVariables().end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::StateVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->mb.getAuxiliaryStateVariables().begin();
	(p != this->mb.getAuxiliaryStateVariables().end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::StateVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->mb.getExternalStateVariables().begin();
	(p != this->mb.getExternalStateVariables().end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::ExternalStateVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    if(!found){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
			      this->current->value+" is not a valid identifier.");
    }
    if ((boundsDescription.varType==SupportedTypes::TVector)||
	(boundsDescription.varType==SupportedTypes::Stensor)||
	(boundsDescription.varType==SupportedTypes::Tensor)){
      ++(this->current);
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds : ","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds : ");
      if(this->current->value!="*"){
	unsigned short component;
	istringstream converter(this->current->value);
	converter >> component;
	if(!converter&&(!converter.eof())){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				  "could not read component number for variable '"+boundsDescription.varName+"'");
	}
	boundsDescription.varType=Scalar;
	boundsDescription.varName+="(";
	boundsDescription.varName+=toString(component);
	boundsDescription.varName+=")";
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");
      if(this->current->value!=")"){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected ')' (read '"+this->current->value+"')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds","in");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");
      if(this->current->value!="*"){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      boundsDescription.boundsType = BoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			      "expected to read lower bound value for variable "+boundsDescription.varName);
      istringstream converter(this->current->value);
      converter >> boundsDescription.lowerBound;
      boundsDescription.boundsType = BoundsDescription::LowerAndUpper;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"could not read lower bound value for variable '"+boundsDescription.varName+"'");
      }
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds : ",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds",":");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			    "expected to read upper bound value for variable "+boundsDescription.varName);
    if(this->current->value=="*"){
      if(boundsDescription.boundsType==BoundsDescription::Upper){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"upper and lower values bounds are both infinity. This is inconsistent.");
      }
      boundsDescription.boundsType=BoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> boundsDescription.upperBound;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"could not read upper bound value for variable '"+boundsDescription.varName+"'");
      }
      if(boundsDescription.boundsType==BoundsDescription::LowerAndUpper){
	if(boundsDescription.lowerBound>boundsDescription.upperBound){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				  "lower bound value is greater than upper bound value for variable '"+
				  boundsDescription.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			      "Expected ']'.");
      if(this->current->value!="]"){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected ']' (read '"+this->current->value+"')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds",";");
  } // end of MFrontBehaviourParserCommon::treatBounds

  void
  MFrontBehaviourParserCommon::registerDefaultVarNames(void)
  {
    using namespace std;
    this->registerVariable("D");
    this->registerVariable("Dt");
    this->registerVariable("T");
    this->registerVariable("dT");
    this->registerVariable("dt");
    this->reserveName("N");
    this->reserveName("Type");
    this->reserveName("use_qt");
    this->reserveName("src1");
    this->reserveName("src2");
    this->reserveName("policy_value");
    this->reserveName("integrate");
    this->reserveName("computeStress");
    this->reserveName("computeFinalStress");
    this->reserveName("computeFdF");
    this->reserveName("updateStateVars");
    this->reserveName("updateAuxiliaryStateVars");
    this->reserveName("getTangentOperator");
    this->reserveName("getTimeStepScalingFactor");
    this->reserveName("computeConsistantTangentOperator");
    this->reserveName("computePredictionOperator");
    this->reserveName("computeTangentOperator_");
    this->reserveName("smt");
    this->reserveName("hypothesis");
  } // end of MFrontBehaviourParserCommon::registerDefaultVarNames

  MFrontBehaviourParserCommon::MFrontBehaviourParserCommon()
    : MFrontVirtualParser(),
      ParserBase(),
      useStateVarTimeDerivative(false),
      explicitlyDeclaredUsableInPurelyImplicitResolution(false),
      hasConsistantTangentOperator(false),
      isConsistantTangentOperatorSymmetric(false),
      hasTimeStepScalingFactor(false)
  {
    // Register var names
    this->registerDefaultVarNames();
    // By default disable use of quantities
    this->mb.setUseQt(false);
    // By default, a behaviour is isotropic 
    this->mb.setBehaviourType(mfront::ISOTROPIC);
    // By default, a behaviour is isotropic 
    this->mb.setElasticBehaviourType(mfront::ISOTROPIC);
    // By default, a behaviour can be used in a purely implicit resolution
    this->mb.setUsableInPurelyImplicitResolution(true);
  } // end of MFrontBehaviourParserCommon::MFrontParserCommon

  void MFrontBehaviourParserCommon::writeIncludes(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("MFrontBehaviourParserCommon::writeIncludes : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << this->includes << endl;
    file << endl;
  }

  void MFrontBehaviourParserCommon::writeNamespaceBegin(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("MFrontBehaviourParserCommon::writeNamespaceBegin : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "namespace tfel{" << endl;
    file << endl;
    file << "namespace material{" << endl;
    file << endl;
  }

  void MFrontBehaviourParserCommon::writeNamespaceEnd(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("MFrontBehaviourParserCommon::writeNamespaceEnd : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "} // end of namespace material" << endl;
    file << endl;
    file << "} // end of namespace tfel" << endl;
    file << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegerConstants(std::ofstream& file)
  {
    using namespace std;
    map<string,int>::const_iterator p;
    if((!file)||(!file.good())){
      string msg("MFrontBehaviourParserCommon::writeIntegerConstants : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    if(!this->integerConstants.empty()){
      file << endl;
    }
    for(p=this->integerConstants.begin();p!=this->integerConstants.end();++p){
      file << "static const int " << p->first << " = " << p->second << ";" << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeIntegerConstants

  void MFrontBehaviourParserCommon::writeStandardTFELTypedefs(std::ofstream& file) 
  {
    using namespace std;
    if((!file)||(!file.good())){
      string msg("MFrontBehaviourParserCommon::writeStandardTFELTypedefs : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "typedef unsigned short ushort;" << endl;
    if(this->mb.useQt()){        
      file << "typedef tfel::config::Types<N,Type,use_qt> Types;\n";
    } else {
      file << "typedef tfel::config::Types<N,Type,false> Types;\n";
    }
    file << "typedef typename Types::real                   real;\n";
    file << "typedef typename Types::time                   time;\n";
    file << "typedef typename Types::frequency              frequency;\n";
    file << "typedef typename Types::stress                 stress;\n";
    file << "typedef typename Types::strain                 strain;\n";
    file << "typedef typename Types::strainrate             strainrate;\n";
    file << "typedef typename Types::stressrate             stressrate;\n";
    file << "typedef typename Types::temperature            temperature;\n";
    file << "typedef typename Types::thermalexpansion       thermalexpansion;\n";
    file << "typedef typename Types::density                density;\n";
    file << "typedef typename Types::Stensor                Stensor;\n";
    file << "typedef typename Types::Stensor4               Stensor4;\n";
    file << "typedef typename Types::FrequencyStensor       FrequencyStensor;\n";
    file << "typedef typename Types::StressStensor          StressStensor;\n";
    file << "typedef typename Types::StrainStensor          StrainStensor;\n";
    file << "typedef typename Types::StrainRateStensor      StrainRateStensor;\n";
    file << "typedef typename Types::StrainRateStensor      StressRateStensor;\n";
    file << "typedef typename Types::StiffnessTensor        StiffnessTensor;\n";
    file << "typedef typename Types::ThermalExpansionTensor ThermalExpansionTensor;\n";
    file << "typedef typename Types::DisplacementTVector    DisplacementTVector;\n";
  } // end of MFrontBehaviourParserCommon::writeStandardTFELTypedefs

  MFrontBehaviourParserCommon::~MFrontBehaviourParserCommon()
  {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();    
  } // end of MFrontBehaviourParserCommon::~MFrontParserCommon

  void MFrontBehaviourParserCommon::checkBehaviourDataFile() const {
    using namespace std;
    if((!this->behaviourDataFile)||
       (!this->behaviourDataFile.good())){
      string msg("MFrontBehaviourParserCommon::checkBehaviourDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataFileHeader(){
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "/*!" << endl;
    this->behaviourDataFile << "* \\file   " << this->behaviourDataFileName << endl;
    this->behaviourDataFile << "* \\brief  " << "this file implements the " 
			    << this->className << "BehaviourData" << " class." << endl;
    this->behaviourDataFile << "*         File generated by ";
    this->behaviourDataFile << MFrontHeader::getVersionName() << " ";
    this->behaviourDataFile << "version " << MFrontHeader::getVersionNumber();
    this->behaviourDataFile << endl;
    if(!this->authorName.empty()){
      this->behaviourDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourDataFile << "* \\date   " << this->date       << endl;
    }
    this->behaviourDataFile << " */" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataFileHeaderBegin(){
    using namespace std;
    this->checkBehaviourDataFile();

    this->behaviourDataFile << "#ifndef _LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->className);
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX_" << endl;
    this->behaviourDataFile << "#define _LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->className);
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX_" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataFileHeaderEnd()
  {
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "#endif /* _LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->className);
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX_ */" << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStandardTFELIncludes(void)
  {
    using namespace std;
    bool b1 = false;
    bool b2 = false;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#include<iostream>\n";
    this->behaviourDataFile << "#include<algorithm>\n";
    this->behaviourDataFile << "#include<string>\n\n";
    this->behaviourDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/Config/TFELTypes.hxx\""  << endl;
    this->behaviourDataFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"" << endl;
    if(this->mb.useQt()){
      this->behaviourDataFile << "#include\"TFEL/Math/General/BaseCast.hxx\"" << endl;
    }
    this->requiresTVectorOrVectorIncludes(b1,b2);
    if(b1){
      this->behaviourDataFile << "#include\"TFEL/Math/tvector.hxx\"\n";
    }
    if(b2){
      this->behaviourDataFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    this->behaviourDataFile << "#include\"TFEL/Material/MechanicalBehaviourData.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/Material/ModellingHypothesis.hxx\"" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataDefaultMembers(void)
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "protected: \n\n";
    if(this->mb.requiresStiffnessTensor()){
      this->behaviourDataFile << "StiffnessTensor D;\n";
    }
    if(this->mb.requiresThermalExpansionTensor()){
      this->behaviourDataFile << "ThermalExpansionTensor A;\n";
    }
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << p3->first.type  << " " << p3->first.name << ";\n\n";
      } else {
	this->behaviourDataFile << p3->first.type  << " " << p3->first.name << "0;\n\n";
      }
      this->behaviourDataFile << p3->second.type << " " << p3->second.name << ";\n\n";
    }
    this->behaviourDataFile << "temperature T;\n";
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeStandardTFELTypedefs(this->behaviourDataFile);
    this->behaviourDataFile << endl;
    this->behaviourDataFile << "static const unsigned short TVectorSize = N;\n";
    this->behaviourDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourDataFile << "static const unsigned short StensorSize = ";
    this->behaviourDataFile << "StensorDimeToSize::value;\n";
    this->behaviourDataFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
    this->behaviourDataFile << "static const unsigned short TensorSize = ";
    this->behaviourDataFile << "TensorDimeToSize::value;\n";
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataGetName(void){    
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "public:\n";
    this->behaviourDataFile << endl;
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\brief  Return the name of the class.\n";
    this->behaviourDataFile << "* \\param  void.\n";
    this->behaviourDataFile << "* \\return const std::string, the name of the class.\n";
    this->behaviourDataFile << "* \\see    Name.\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << "static std::string\n";
    this->behaviourDataFile << "getName(void){\n";
    this->behaviourDataFile << "return std::string(\"";
    this->behaviourDataFile << this->className << "BehaviourData\");\n";
    this->behaviourDataFile << "}\n\n";
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataDisabledConstructors(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataConstructors(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    VarContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\brief Default constructor\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->className << "BehaviourData()\n";
    this->behaviourDataFile << "{}\n\n";
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\brief Copy constructor\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->className << "BehaviourData(const ";
    this->behaviourDataFile << this->className << "BehaviourData& src)\n";
    this->behaviourDataFile << ": " << endl;
    if(this->mb.requiresStiffnessTensor()){
      this->behaviourDataFile << "D(src.D),\n";
    }
    if(this->mb.requiresThermalExpansionTensor()){
      this->behaviourDataFile << "A(src.A),\n";
    }
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << p3->first.name  << "(src." << p3->first.name << "),\n";
      } else {
	this->behaviourDataFile << p3->first.name  << "0(src." << p3->first.name << "0),\n";
      }
      this->behaviourDataFile << p3->second.name << "(src." << p3->second.name << "),\n";
    }
    this->behaviourDataFile << "T(src.T)";
    if(!this->mb.getMaterialProperties().empty()){
      for(p=this->mb.getMaterialProperties().begin();p!=this->mb.getMaterialProperties().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!this->mb.getStateVariables().empty()){
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!this->mb.getAuxiliaryStateVariables().empty()){
      for(p=this->mb.getAuxiliaryStateVariables().begin();p!=this->mb.getAuxiliaryStateVariables().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!this->mb.getExternalStateVariables().empty()){
      for(p =this->mb.getExternalStateVariables().begin();
	  p!=this->mb.getExternalStateVariables().end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    this->behaviourDataFile << "\n";
    this->behaviourDataFile << "{}\n\n" << endl;
    this->behaviourDataFile << "/*!\n";
    this->behaviourDataFile << "* \\brief Constructor\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->className 
			    << "BehaviourData(const ";
    if(this->mb.useQt()){
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,use_qt>& src)\n";
    } else {
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,false>& src)\n";
    }
    this->behaviourDataFile << ":";
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << p3->first.name  << "(src." << p3->first.name << "),\n";
      } else {
	this->behaviourDataFile << p3->first.name  << "0(src." << p3->first.name << "0),\n";
      }
      this->behaviourDataFile << p3->second.name << "(src." << p3->second.name << "),\n";
    }
    this->behaviourDataFile << "T(src.T)";
    SupportedTypes::TypeSize o;
    this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
							       this->mb.getMaterialProperties(),
							       "src.material_properties","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
								   this->mb.getStateVariables(),
								   "src.internal_variables","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
							       this->mb.getAuxiliaryStateVariables(),
							       "src.internal_variables","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
							       this->mb.getExternalStateVariables(),
							       "src.external_variables","","");
    this->behaviourDataFile << "\n";
    this->behaviourDataFile << "{\n";
    this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								this->mb.getMaterialProperties(),
								"src.material_properties","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								    this->mb.getStateVariables(),
								    "src.internal_variables","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								this->mb.getAuxiliaryStateVariables(),
								"src.internal_variables","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								this->mb.getExternalStateVariables(),
								"src.external_variables","","");
    this->behaviourDataFile << "}\n\n";
    // Creating constructor for external interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin(); i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeBehaviourDataConstructor(this->behaviourDataFile,
					       this->className,
					       this->mb);
    }
  } // end of MFrontBehaviourParserCommon::WriteBehaviourDataConstructors

  void MFrontBehaviourParserCommon::writeBehaviourDataAssignementOperator(void)
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;
    VarContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*\n";
    this->behaviourDataFile << "* \\brief Assignement operator\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->className << "BehaviourData&\n";
    this->behaviourDataFile << "operator=(const " 
			    << this->className << "BehaviourData& src){\n";
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << "this->" << p3->first.name  << " = src." << p3->first.name << ";\n\n";
      } else {
	this->behaviourDataFile << "this->" << p3->first.name  << "0 = src." << p3->first.name << "0;\n\n";
      }
      this->behaviourDataFile << "this->" << p3->second.name << " = src." << p3->second.name << ";\n\n";
    }
    if(!this->mb.getMaterialProperties().empty()){
      for(p=this->mb.getMaterialProperties().begin();p!=this->mb.getMaterialProperties().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!this->mb.getStateVariables().empty()){
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!this->mb.getAuxiliaryStateVariables().empty()){
      for(p=this->mb.getAuxiliaryStateVariables().begin();p!=this->mb.getAuxiliaryStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!this->mb.getExternalStateVariables().empty()){
      for(p =this->mb.getExternalStateVariables().begin();
	  p!=this->mb.getExternalStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    this->behaviourDataFile << "return *this;" << endl;
    this->behaviourDataFile << "}\n\n" << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourAssignementOperator

  void MFrontBehaviourParserCommon::writeBehaviourDataExport(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "void\n"
			    << "exportBehaviourData(const ";
    if(this->mb.useQt()){
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,use_qt>& res) const\n";
    } else {
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,false>& res) const\n";
    }
    this->behaviourDataFile << "{\n";
    this->behaviourDataFile << "using namespace tfel::math;\n";
    for(p=this->mb.getMainVariables().begin();p!=this->mb.getMainVariables().end();++p){
      if(p->first.increment_known){
	this->behaviourDataFile << "res." << p->first.name  << " = this->" << p->first.name << ";\n\n";
      } else {
	this->behaviourDataFile << "res." << p->first.name  << "0 = this->" << p->first.name << "0;\n\n";
      }
      this->behaviourDataFile << "res." << p->second.name << " = this->" << p->second.name << ";\n\n";
    }
    if(!this->mb.getMaterialProperties().empty()){
      this->writeResultsArrayResize(this->behaviourDataFile,
				    "res.material_properties",
				    this->mb.getMaterialProperties());
      this->exportResults(this->behaviourDataFile,
			  this->mb.getMaterialProperties(),
			  "res.material_properties",
			  this->mb.useQt());
    }
    if((!this->mb.getStateVariables().empty())||
       (!this->mb.getAuxiliaryStateVariables().empty())){
      SupportedTypes::TypeSize s = this->getTotalSize(this->mb.getStateVariables());
      s += this->getTotalSize(this->mb.getAuxiliaryStateVariables());
      this->writeResultsArrayResize(this->behaviourDataFile,
				    "res.internal_variables",s);
      SupportedTypes::TypeSize o;
      o = this->exportResults(this->behaviourDataFile,
			  this->mb.getStateVariables(),
			  "res.internal_variables",
			  this->mb.useQt());
      this->exportResults(this->behaviourDataFile,
			  this->mb.getAuxiliaryStateVariables(),
			  "res.internal_variables",
			  this->mb.useQt(),o);
    }
    if(!this->mb.getExternalStateVariables().empty()){
      this->writeResultsArrayResize(this->behaviourDataFile,
				    "res.external_variables",
				    this->mb.getExternalStateVariables());
      this->exportResults(this->behaviourDataFile,
			  this->mb.getExternalStateVariables(),
			  "res.external_variables",
			  this->mb.useQt());
    }
    this->behaviourDataFile << "} // end of exportBehaviourData\n";
    this->behaviourDataFile << endl;
    // Creating constructor for external interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->exportMechanicalData(this->behaviourDataFile,
				      this->className,
				      this->mb);
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataPublicMembers(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    if(this->mb.requiresStiffnessTensor()){
      this->behaviourDataFile << "StiffnessTensor&\n";
      this->behaviourDataFile << "getStiffnessTensor(void)\n";
      this->behaviourDataFile << "{\nreturn this->D;\n}\n\n";
      this->behaviourDataFile << "const StiffnessTensor&\n";
      this->behaviourDataFile << "getStiffnessTensor(void) const\n";
      this->behaviourDataFile << "{\nreturn this->D;\n}\n\n";
    }
    if(this->mb.requiresThermalExpansionTensor()){
      this->behaviourDataFile << "ThermalExpansionTensor&\n";
      this->behaviourDataFile << "getThermalExpansionTensor(void)\n";
      this->behaviourDataFile << "{\nreturn this->A;\n}\n\n";
      this->behaviourDataFile << "const ThermalExpansionTensor&\n";
      this->behaviourDataFile << "getThermalExpansionTensor(void) const\n";
      this->behaviourDataFile << "{\nreturn this->A;\n}\n\n";
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourDataPublicMembers

  void MFrontBehaviourParserCommon::writeBehaviourDataClassHeader(void) 
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!" << endl;
    this->behaviourDataFile << "* \\class " << this->className << "BehaviourData"  << endl;
    this->behaviourDataFile << "* \\brief This class implements the " 
			    << this->className << "BehaviourData" << " ." << endl;
    this->behaviourDataFile << "* \\param unsigned short N, space dimension." << endl;
    this->behaviourDataFile << "* \\param typename Type, numerical type." << endl;
    this->behaviourDataFile << "* \\param bool use_qt, conditional saying if quantities are use." << endl;
    if(!this->authorName.empty()){
      this->behaviourDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourDataFile << "* \\date   " << this->date << endl;
    }
    this->behaviourDataFile << "*/" << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataClassBegin(void) {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "// Forward Declaration" << endl;
    this->behaviourDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
    this->behaviourDataFile << "class " << this->className << "BehaviourData;\n\n";
    
    if(this->mb.useQt()){
      this->behaviourDataFile << "// Forward Declaration" << endl;
      this->behaviourDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->className 
			      << "BehaviourData<N,Type,use_qt>&);\n\n";
    } else {
      this->behaviourDataFile << "// Forward Declaration" << endl;
      this->behaviourDataFile << "template<unsigned short N,typename Type>" << endl;
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->className 
			      << "BehaviourData<N,Type,false>&);\n\n";
    }
    
    if(this->mb.useQt()){
      this->behaviourDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
      this->behaviourDataFile << "class " << this->className << "BehaviourData\n";
    } else {
      this->behaviourDataFile << "template<unsigned short N,typename Type>\n";
      this->behaviourDataFile << "class " << this->className 
			      << "BehaviourData<N,Type,false>" << endl;
    }
    
    this->behaviourDataFile << "{" << endl;
    this->behaviourDataFile << endl;
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);" << endl;
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			    << "IsFundamentalNumericType<Type>::cond);\n";
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->behaviourDataFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourDataFile << this->className << "BehaviourData&);\n\n";
    this->writeIntegerConstants(this->behaviourDataFile);    
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataClassEnd() {    
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "}; // end of " << this->className << "BehaviourData" 
			    << "class" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataCoefs() {
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->mb.getMaterialProperties(),
				     "","",this->fileName,
				     false,this->debugMode);

    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStateVars() {
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->mb.getStateVariables(),
				     "","",this->fileName,
				     false,this->debugMode);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->mb.getAuxiliaryStateVariables(),
				     "","",this->fileName,
				     false,this->debugMode);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->mb.getExternalStateVariables(),
				     "","",this->fileName,
				     false,this->debugMode);
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataOutputOperator(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    if(this->mb.useQt()){        
      this->behaviourDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
      this->behaviourDataFile << "std::ostream&\n";
      this->behaviourDataFile << "operator <<(std::ostream& os,";
      this->behaviourDataFile << "const " << this->className 
			      << "BehaviourData<N,Type,use_qt>& b)\n";
    } else {
      this->behaviourDataFile << "template<unsigned short N,typename Type>" << endl;
      this->behaviourDataFile << "std::ostream&\n";
      this->behaviourDataFile << "operator <<(std::ostream& os,";
      this->behaviourDataFile << "const " << this->className 
			      << "BehaviourData<N,Type,false>& b)\n";
    }
    this->behaviourDataFile << "{" << endl;
    this->behaviourDataFile << "using namespace std;" << endl;
    if(this->mb.useQt()){        
      this->behaviourDataFile << "os << " << this->className 
			      << "BehaviourData<N,Type,use_qt>::getName() << endl;\n";
    } else {
      this->behaviourDataFile << "os << " << this->className 
			      << "BehaviourData<N,Type,false>::getName() << endl;\n";
    }
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourDataFile << "os << \"" << p2->first.name  << " : \" << b." << p2->first.name  << " << endl;\n";
      } else {
	this->behaviourDataFile << "os << \"" << p2->first.name  << "0 : \" << b." << p2->first.name  << "0 << endl;\n";
      }
      this->behaviourDataFile << "os << \"" << p2->second.name << " : \" << b." << p2->second.name << " << endl;\n";
    }
    this->behaviourDataFile << "os << \"T : \" << b.T << endl;\n";
    for(p=this->mb.getMaterialProperties().begin();p!=this->mb.getMaterialProperties().end();++p){
      this->behaviourDataFile <<  "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;\n";  
    }
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;\n";  
    }    
    for(p=this->mb.getAuxiliaryStateVariables().begin();
	p!=this->mb.getAuxiliaryStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;\n";  
    }    
    for(p=this->mb.getExternalStateVariables().begin();
	p!=this->mb.getExternalStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name << " << endl;\n";
    }
    this->behaviourDataFile << "return os;\n";
    this->behaviourDataFile << "}\n\n";
  } //  MFrontBehaviourParserCommon::writeBehaviourDataOutputOperator

  void MFrontBehaviourParserCommon::writeBehaviourDataFile(void){
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeBehaviourDataFileHeader();
    this->writeBehaviourDataFileHeaderBegin();
    this->writeBehaviourDataStandardTFELIncludes();
    if(!this->includes.empty()){
      this->writeIncludes(this->behaviourDataFile);
    }
    this->writeNamespaceBegin(this->behaviourDataFile);
    this->writeBehaviourDataClassBegin();
    this->writeBehaviourDataStandardTFELTypedefs();
    this->writeBehaviourDataDefaultMembers();
    this->writeBehaviourDataCoefs();
    this->writeBehaviourDataStateVars();
    this->writeBehaviourDataDisabledConstructors();
    this->writeBehaviourDataGetName();
    this->writeBehaviourDataConstructors();
    this->writeBehaviourDataPublicMembers();
    this->writeBehaviourDataAssignementOperator();
    this->writeBehaviourDataExport();
    this->writeBehaviourDataClassEnd();
    this->writeBehaviourDataOutputOperator();
    this->writeNamespaceEnd(this->behaviourDataFile);
    this->writeBehaviourDataFileHeaderEnd();
  }
  void MFrontBehaviourParserCommon::checkBehaviourFile() const {
    using namespace std;
    if((!this->behaviourFile)||
       (!this->behaviourFile.good())){
      string msg("MFrontBehaviourParserCommon::checkBehaviourDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourClassBegin() {
    using namespace std;

    this->checkBehaviourFile();

    this->behaviourFile << "// Forward Declaration" << endl;
    this->behaviourFile << "template<ModellingHypothesis::Hypothesis,typename Type,bool use_qt>" << endl;
    this->behaviourFile << "class " << this->className << ";\n\n";

    if(this->mb.useQt()){
      this->behaviourFile << "// Forward Declaration" << endl;
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourFile << "const " << this->className << "<hypothesis,Type,use_qt>&);\n\n";
    } else {
      this->behaviourFile << "// Forward Declaration" << endl;
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourFile << "const " << this->className << "<hypothesis,Type,false>&);\n\n";
    }

    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\class " << this->className    << endl;
    this->behaviourFile << "* \\brief This class implements the " 
			<< this->className << " behaviour." << endl;
    this->behaviourFile << "* \\param hypothesis, modelling hypothesis." << endl;
    this->behaviourFile << "* \\param Type, numerical type." << endl;
    if(this->mb.useQt()){    
      this->behaviourFile << "* \\param use_qt, conditional "
			  << "saying if quantities are use." << endl;
    }
    if(!this->authorName.empty()){
      this->behaviourFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourFile << "* \\date   " << this->date << endl;
    }
    if(!this->description.empty()){
      this->behaviourFile << this->description << endl;
    }
    this->behaviourFile << "*/" << endl;

    if(this->mb.useQt()){        
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->behaviourFile << "class " << this->className << endl;
      this->behaviourFile << ": public MechanicalBehaviour<hypothesis,Type,use_qt>,\n";
      this->behaviourFile << "public "
			  << this->className << "BehaviourData<ModellingHypothesisToSpaceDimension<hypothesis>::value,Type,use_qt>," << endl;
      this->behaviourFile << "public "
			  << this->className << "IntegrationData<ModellingHypothesisToSpaceDimension<hypothesis>::value,Type,use_qt>";
      this->writeBehaviourParserSpecificInheritanceRelationship();
    } else {
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->behaviourFile << "class " << this->className << "<hypothesis,Type,false>" << endl;
      this->behaviourFile << ": public MechanicalBehaviour<hypothesis,Type,false>,\n";
      this->behaviourFile << "public "
			  << this->className << "BehaviourData<ModellingHypothesisToSpaceDimension<hypothesis>::value,Type,false>," << endl;
      this->behaviourFile << "public "
			  << this->className << "IntegrationData<ModellingHypothesisToSpaceDimension<hypothesis>::value,Type,false>";
      this->writeBehaviourParserSpecificInheritanceRelationship();
    }
    this->behaviourFile << "{" << endl;
    this->behaviourFile << endl;
    this->behaviourFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    this->behaviourFile << endl;
    this->behaviourFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);" << endl;
    this->behaviourFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			<< "IsFundamentalNumericType<Type>::cond);\n";
    this->behaviourFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->behaviourFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourFile << this->className << "&);\n\n";
    this->writeIntegerConstants(this->behaviourFile);
  }

  void MFrontBehaviourParserCommon::writeBehaviourFileHeader(){
    using namespace std;

    this->checkBehaviourFile();

    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\file   " << this->behaviourFileName  << endl;
    this->behaviourFile << "* \\brief  " << "this file implements the " 
			<< this->className << " Behaviour." << endl;
    this->behaviourFile << "*         File generated by ";
    this->behaviourFile << MFrontHeader::getVersionName() << " ";
    this->behaviourFile << "version " << MFrontHeader::getVersionNumber();
    this->behaviourFile << endl;
    if(!this->authorName.empty()){
      this->behaviourFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourFile << "* \\date   " << this->date       << endl;
    }
    this->behaviourFile << " */" << endl;
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourFileHeaderBegin(){
    using namespace std;
    this->checkBehaviourFile();

    this->behaviourFile << "#ifndef _LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->className);
    this->behaviourFile << "_HXX_" << endl;
    this->behaviourFile << "#define _LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->className);
    this->behaviourFile << "_HXX_" << endl;
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourFileHeaderEnd() {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "#endif /* _LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->className);
    this->behaviourFile << "_HXX_ */" << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourClassEnd() {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "}; // end of " << this->className 
			<< " class" << endl;
    this->behaviourFile << endl;
  }
  
  void
  MFrontBehaviourParserCommon::treatUpdateAuxiliaryStateVars(void)
  {
    this->updateAuxiliaryStateVars = this->readNextBlock(true);
  } // end of MFrontBehaviourParserCommon::treatUpdateAuxiliaryStateVarBase

  void
  MFrontBehaviourParserCommon::writeBehaviourUpdateStateVars(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Update internal variables at end of integration\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "updateStateVars(void)";
    if(!this->mb.getStateVariables().empty()){
      this->behaviourFile << "{\n";
      for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
	this->behaviourFile << "this->"  << p->name << " += ";
	this->behaviourFile << "this->d" << p->name << ";\n";
      }
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourUpdateStateVars

  void
  MFrontBehaviourParserCommon::writeBehaviourUpdateAuxiliaryStateVars() 
  {
    using namespace std;
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Update auxiliary state variables at end of integration\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\n";
    this->behaviourFile << "updateAuxiliaryStateVars(void)";
    if(!this->updateAuxiliaryStateVars.empty()){
      this->behaviourFile << "{\n";
      this->behaviourFile << "using namespace std;" << endl;
      this->behaviourFile << "using namespace tfel::math;" << endl;
      writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourUpdateAuxiliaryStateVars",
			this->behaviourFile,this->materialLaws);		      
      this->behaviourFile << this->updateAuxiliaryStateVars << endl;
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "\n{}\n\n";
    }
  } // end of  MFrontBehaviourParserCommon::writeBehaviourUpdateAuxiliaryStateVars

  void MFrontBehaviourParserCommon::writeBehaviourIntegrator() {
    using namespace std;
    vector<BoundsDescription>::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour  over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult\n";
    this->behaviourFile << "integrate(const SMType smt){\n";
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    this->behaviourFile << "bool computeTangentOperator_ = smt!=NOSTIFFNESSREQUESTED;\n";
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourIntegrator",
		      this->behaviourFile,this->materialLaws);
    if(!this->integrator.empty()){
      this->behaviourFile << this->integrator;
      this->behaviourFile << "\n";
    }
    this->behaviourFile << "this->updateStateVars();\n";
    this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    for(p  = this->mb.getBounds().begin();
	p != this->mb.getBounds().end();++p){
      if(p->varCategory==BoundsDescription::StateVar){
	p->writeBoundsChecks(this->behaviourFile);
      }
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void MFrontBehaviourParserCommon::writeBehaviourDisabledConstructors(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Default constructor (disabled)\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << this->className << "();\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Copy constructor (disabled)\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << this->className << "(const ";
    this->behaviourFile << this->className<< "&);\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Assignement operator (disabled)\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << this->className << "& operator = (const ";
    this->behaviourFile << this->className<< "&);\n\n";
  }

  void MFrontBehaviourParserCommon::writeBehaviourSetOutOfBoundsPolicy(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief set the policy for \"out of bounds\" conditions\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\nsetOutOfBoundsPolicy(const OutOfBoundsPolicy policy_value){\n";
    this->behaviourFile << "this->policy = policy_value;\n";
    this->behaviourFile << "} // end of setOutOfBoundsPolicy\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourOutOfBoundsEnumeration(void)

  void MFrontBehaviourParserCommon::writeBehaviourCheckBounds(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief check bounds\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "void\ncheckBounds(void) const{\n";
    vector<BoundsDescription>::const_iterator b;
    for(b  = this->mb.getBounds().begin();
	b != this->mb.getBounds().end();++b){
      b->writeBoundsChecks(this->behaviourFile);
    }      
    this->behaviourFile << "} // end of checkBounds\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourCheckBounds(void)

  void MFrontBehaviourParserCommon::writeBehaviourConstructors(void)
  {    
    using namespace std;
    ostringstream initStateVarsIncrements;
    this->writeStateVariableIncrementsInitializers(initStateVarsIncrements,
						   this->mb.getStateVariables(),
						   this->useStateVarTimeDerivative);
    this->writeBehaviourConstructors(initStateVarsIncrements.str());
  }

  void MFrontBehaviourParserCommon::writeBehaviourConstructors(const std::string& initStateVarsIncrements,
							       const std::string& predictor)
  {    
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Constructor\n";
    this->behaviourFile << "*/\n";
    if(this->mb.useQt()){        
      this->behaviourFile << this->className << "("
			  << "const " << this->className 
			  << "BehaviourData<N,Type,use_qt>& src1,\n"
			  << "const " << this->className 
			  << "IntegrationData<N,Type,use_qt>& src2)"
			  << "\n";
      this->behaviourFile << ": " << this->className 
			  << "BehaviourData<N,Type,use_qt>(src1),\n";
      this->behaviourFile << this->className 
			  << "IntegrationData<N,Type,use_qt>(src2)";
    } else {
      this->behaviourFile << this->className << "("
			  << "const " << this->className 
			  << "BehaviourData<N,Type,false>& src1,\n"
			  << "const " << this->className 
			  << "IntegrationData<N,Type,false>& src2)"
			  << "\n"; 
      this->behaviourFile << ": " << this->className 
			  << "BehaviourData<N,Type,false>(src1),\n";
      this->behaviourFile << this->className 
			  << "IntegrationData<N,Type,false>(src2)";
    }
    this->behaviourFile << initStateVarsIncrements;
    this->behaviourFile << "\n{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
		      this->behaviourFile,this->materialLaws);
    this->writeBehaviourParameterInitialisation();
    this->writeBehaviourLocalVariablesInitialisation();
    if(!this->initLocalVars.empty()){
      this->behaviourFile << this->initLocalVars;
    }
    if(!predictor.empty()){
      this->behaviourFile << predictor;
    }
    this->writeBehaviourParserSpecificConstructorPart();
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Constructor\n";
    this->behaviourFile << "*/\n";
    if(this->mb.useQt()){        
      this->behaviourFile << this->className << "("
			  << "const MechanicalBehaviourData<N,Type,use_qt>& src1,\n"
			  << "const MechanicalIntegrationData<N,Type,use_qt>& src2)\n";
      this->behaviourFile << ": " << this->className 
			  << "BehaviourData<N,Type,use_qt>(src1),\n";
      this->behaviourFile << this->className 
			  << "IntegrationData<N,Type,use_qt>(src2)";
    } else {
      this->behaviourFile << this->className << "("
			  << "const MechanicalBehaviourData<N,Type,false>& src1,\n"
			  << "const MechanicalIntegrationData<N,Type,false>& src2)\n";
      this->behaviourFile << ": " << this->className 
			  << "BehaviourData<N,Type,false>(src1),\n";
      this->behaviourFile << this->className 
			  << "IntegrationData<N,Type,false>(src2)";
    }
    this->behaviourFile << initStateVarsIncrements;
    this->behaviourFile << "\n{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
		      this->behaviourFile,this->materialLaws);		      
    this->writeBehaviourParameterInitialisation();
    this->writeBehaviourLocalVariablesInitialisation();
    if(!this->initLocalVars.empty()){
      this->behaviourFile << this->initLocalVars;
    } 
    if(!predictor.empty()){
      this->behaviourFile << predictor;
    }
    this->writeBehaviourParserSpecificConstructorPart();
    this->behaviourFile << "}\n\n";
    // Creating constructor for external interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeBehaviourConstructor(this->behaviourFile,
					   this->className,
					   this->mb,
					   initStateVarsIncrements);
      this->behaviourFile << "\n{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using std::vector;\n";
      writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
			this->behaviourFile,this->materialLaws);		      
      this->writeBehaviourParameterInitialisation();
      this->writeBehaviourLocalVariablesInitialisation();
      if(!this->initLocalVars.empty()){
	this->behaviourFile << this->initLocalVars;
      }
      if(!predictor.empty()){
	this->behaviourFile << predictor;
      }
      this->writeBehaviourParserSpecificConstructorPart();
      this->behaviourFile << "}\n\n";
    }
  }

  void
  MFrontBehaviourParserCommon::writeBehaviourLocalVariablesInitialisation()
  {
    using namespace std;
    VarContainer::const_iterator p;
    this->checkBehaviourFile();
    for(p=this->mb.getLocalVariables().begin();p!=this->mb.getLocalVariables().end();++p){
      if(this->useDynamicallyAllocatedVector(p->arraySize)){
	this->behaviourFile << "this->" << p->name << ".resize(" << p->arraySize << ");" << endl;
      }
    }  
  }
  
  void MFrontBehaviourParserCommon::writeBehaviourParameterInitialisation(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    if(!this->mb.getParameters().empty()){
      for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
	this->behaviourFile << "this->" << p->name << " = " << this->className 
			    << "ParametersInitializer::get()." << p->name << ";\n";  
      }
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourParameterInitialisation

  void MFrontBehaviourParserCommon::writeBehaviourGetModellingHypothesis()
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief set the policy for \"out of bounds\" conditions\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "tfel::material::ModellingHypothesis::Hypothesis\ngetModellingHypothesis(void) const{\n";
    this->behaviourFile << "return hypothesis;\n";
    this->behaviourFile << "} // end of getModellingHypothesis\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourGetModellingHypothesis();

  void MFrontBehaviourParserCommon::writeBehaviourLocalVars(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->writeVariablesDeclarations(this->behaviourFile,
				     this->mb.getLocalVariables(),
				     "","",this->fileName,
				     false,this->debugMode);
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourParameters(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
      if(!this->debugMode){
	if(p->lineNumber!=0u){
	  this->behaviourFile << "#line " << p->lineNumber << " \"" 
			      << this->fileName << "\"\n";
	}
      }
      this->behaviourFile << p->type << " " << p->name << ";\n";  
    }
    this->behaviourFile << endl;
  }

  
  void MFrontBehaviourParserCommon::writeBehaviourPolicyVariable(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "//! policy for treating out of bounds conditions\n";
    this->behaviourFile << "OutOfBoundsPolicy policy;\n";  
  } // end of MFrontBehaviourParserCommon::writeBehaviourPolicyVariable

  void MFrontBehaviourParserCommon::writeBehaviourStaticVars(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    StaticVarContainer::const_iterator p;
    for(p=this->staticVars.begin();p!=this->staticVars.end();++p){
      if(!this->debugMode){
	if(p->lineNumber!=0u){
	  this->behaviourFile << "#line " << p->lineNumber << " \"" 
			      << this->fileName << "\"\n";
	}
      }
      this->behaviourFile << "static " << p->type << " " << p->name << ";\n";  
    }
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourStateVarsIncrements(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->writeVariablesDeclarations(this->behaviourFile,
				     this->mb.getStateVariables(),
				     "d","",this->fileName,
				     this->useStateVarTimeDerivative,
				     this->debugMode);
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourOutputOperator(void)
  {    
    using namespace std;
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    if(this->mb.useQt()){        
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->behaviourFile << "std::ostream&\n";
      this->behaviourFile << "operator <<(std::ostream& os,";
      this->behaviourFile << "const " << this->className << "<hypothesis,Type,use_qt>& b)\n";
    } else {
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->behaviourFile << "std::ostream&\n";
      this->behaviourFile << "operator <<(std::ostream& os,";
      this->behaviourFile << "const " << this->className << "<hypothesis,Type,false>& b)\n";
    }
    this->behaviourFile << "{" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    if(this->mb.useQt()){        
      this->behaviourFile << "os << " << this->className 
			  << "<hypothesis,Type,use_qt>::getName() << endl;\n";
    } else {
      this->behaviourFile << "os << " << this->className 
			  << "<hypothesis,Type,false>::getName() << endl;\n";
    }
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourFile << "os << \""  << p2->first.name << " : \" << b." << p2->first.name << " << endl;\n";
	this->behaviourFile << "os << \"d" << p2->first.name << " : \" << b.d" << p2->first.name << " << endl;\n";
      } else {
	this->behaviourFile << "os << \"" << p2->first.name << "0 : \" << b." << p2->first.name << "0 << endl;\n";
	this->behaviourFile << "os << \"" << p2->first.name << "1 : \" << b." << p2->first.name << "1 << endl;\n";
      }
      this->behaviourFile << "os << \"" << p2->second.name << " : \" << b." << p2->second.name << " << endl;\n";
    }
    this->behaviourFile << "os << \"dt : \" << b.dt << endl;\n";
    this->behaviourFile << "os << \"T : \" << b.T << endl;\n";
    this->behaviourFile << "os << \"dT : \" << b.dT << endl;\n";
    for(p=this->mb.getMaterialProperties().begin();p!=this->mb.getMaterialProperties().end();++p){
      this->behaviourFile <<  "os << \"" << p->name << " : \" << b." << p->name <<  " << endl;\n";  
    }
    for(p=this->mb.getStateVariables().begin();
	p!=this->mb.getStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << endl;\n";  
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name <<  " << endl;\n";  
    }    
    for(p=this->mb.getAuxiliaryStateVariables().begin();
	p!=this->mb.getAuxiliaryStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << endl;\n";  
    }
    for(p=this->mb.getExternalStateVariables().begin();
	p!=this->mb.getExternalStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name << " << endl;\n";
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name << " << endl;\n";  
    }
    for(p=this->mb.getLocalVariables().begin();p!=this->mb.getLocalVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name;
      if((p+1)!=this->mb.getLocalVariables().end()){
	this->behaviourFile <<  " << endl;\n";  
      } else {
	this->behaviourFile <<  ";\n";  
      }
    }
    for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name;
      if((p+1)!=this->mb.getParameters().end()){
	this->behaviourFile <<  " << endl;\n";  
      } else {
	this->behaviourFile <<  ";\n";  
      }
    }
    this->behaviourFile << "return os;\n";
    this->behaviourFile << "}\n\n";
  }

  void MFrontBehaviourParserCommon::writeBehaviourDestructor(void) 
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Destructor\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "~" << this->className << "()\n{}\n\n";
  }

  void MFrontBehaviourParserCommon::writeBehaviourUpdateExternalStateVariables(void) 
  {    
    using namespace std;
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    this->behaviourFile << "void updateExternalStateVariables(void){\n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourFile << "this->" << p2->first.name  << "  += this->d" << p2->first.name << ";\n\n";
      } else {
	this->behaviourFile << "this->" << p2->first.name  << "0  = this->"  << p2->first.name << "1;\n\n";
      }
    }
    this->behaviourFile << "this->T   += this->dT;\n";
    for(p=this->mb.getExternalStateVariables().begin();p!=this->mb.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << " += this->d" << p->name << ";\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void MFrontBehaviourParserCommon::writeBehaviourIncludeBehaviourData(void) {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "#include<iostream>\n";
    this->behaviourFile << "#include<algorithm>\n";
    this->behaviourFile << "#include<string>\n\n";
    this->behaviourFile << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Config/TFELTypes.hxx\""  << endl;
    this->behaviourFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/MechanicalBehaviour.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/MechanicalBehaviourTraits.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/BoundsCheck.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/";
    this->behaviourFile << this->behaviourDataFileName << "\"\n";
    this->behaviourFile << "#include\"TFEL/Material/";
    this->behaviourFile << this->integrationDataFileName << "\"\n";
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourMembersFunc(void) {    
    using namespace std;
    this->checkBehaviourFile();
    if(!this->members.empty()){
      this->behaviourFile << this->members << "\n\n";
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourPrivate(void) {    
    using namespace std;
    this->checkBehaviourFile();
    if(!this->privateCode.empty()){
      this->behaviourFile << this->privateCode << "\n\n";
    }
  } // end of void MFrontBehaviourParserCommon::writeBehaviourPrivate

  void MFrontBehaviourParserCommon::writeBehaviourGetName(void){    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "public:\n\n";
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief  Return the name of the class.\n";
    this->behaviourFile << "* \\param  void.\n";
    this->behaviourFile << "* \\return const std::string, the name of the class.\n";
    this->behaviourFile << "* \\see    Name.\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "static std::string\n";
    this->behaviourFile << "getName(void){\n";
    this->behaviourFile << "return std::string(\"";
    this->behaviourFile << this->className << "\");\n";
    this->behaviourFile << "}\n\n";
  }

  void MFrontBehaviourParserCommon::writeBehaviourStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->writeStandardTFELTypedefs(this->behaviourFile);
    this->behaviourFile << "\nstatic const unsigned short TVectorSize = N;\n";
    this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourFile << "static const unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;\n\n";
    this->behaviourFile << "public :\n\n";
    if(this->mb.useQt()){        
      this->behaviourFile << "typedef " << this->className 
			  << "BehaviourData<N,Type,use_qt> BehaviourData;\n";
      this->behaviourFile << "typedef " << this->className 
			  << "IntegrationData<N,Type,use_qt> IntegrationData;\n";
      this->behaviourFile << "typedef typename MechanicalBehaviour<hypothesis,Type,use_qt>::SMType            SMType;\n\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::ELASTIC;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::SECANTOPERATOR;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::TANGENTOPERATOR;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::CONSISTANTTANGENTOPERATOR;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::NOSTIFFNESSREQUESTED;\n";
      this->behaviourFile << "typedef typename MechanicalBehaviour<hypothesis,Type,use_qt>::IntegrationResult IntegrationResult;\n\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::SUCCESS;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,use_qt>::UNRELIABLE_RESULTS;\n\n";
    } else {
      this->behaviourFile << "typedef " << this->className 
			  << "BehaviourData<N,Type,false> BehaviourData;\n";
      this->behaviourFile << "typedef " << this->className 
			  << "IntegrationData<N,Type,false> IntegrationData;\n";
      this->behaviourFile << "typedef typename MechanicalBehaviour<hypothesis,Type,false>::SMType            SMType;\n\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::ELASTIC;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::SECANTOPERATOR;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::TANGENTOPERATOR;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::CONSISTANTTANGENTOPERATOR;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::NOSTIFFNESSREQUESTED;\n";
      this->behaviourFile << "typedef typename MechanicalBehaviour<hypothesis,Type,false>::IntegrationResult IntegrationResult;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::SUCCESS;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
      this->behaviourFile << "using MechanicalBehaviour<hypothesis,Type,false>::UNRELIABLE_RESULTS;\n\n";
    }
    
    this->behaviourFile << "private :\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourStandardTFELTypedefs

  void MFrontBehaviourParserCommon::writeBehaviourTraits(void)
  {
    using namespace std;
    using namespace tfel::material;
    typedef ModellingHypothesis MH;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize coefSize;
    SupportedTypes::TypeSize stateVarsSize;
    SupportedTypes::TypeSize externalStateVarsSize;
    const vector<MH::Hypothesis>& ah = MH::getModellingHypotheses();
    vector<MH::Hypothesis>::const_iterator ph;
    set<MH::Hypothesis>::const_iterator ph2;
    for(p=this->mb.getMaterialProperties().begin();p!=this->mb.getMaterialProperties().end();++p){
      coefSize+=this->getTypeSize(p->type,p->arraySize);
    }
    for(p=this->mb.getStateVariables().begin();p!=this->mb.getStateVariables().end();++p){
      stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    for(p=this->mb.getAuxiliaryStateVariables().begin();
	p!=this->mb.getAuxiliaryStateVariables().end();++p){
      stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    for(p  = this->mb.getExternalStateVariables().begin();
	p != this->mb.getExternalStateVariables().end();++p){
      externalStateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    // writing partial specialisations
    if(this->hypotheses.size()>=4u){
      // on définit toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(MH::UNDEFINEDHYPOTHESIS,
					       coefSize,stateVarsSize,
					       externalStateVarsSize,true);
      // unsupported hypothesis
      for(ph=ah.begin();ph!=ah.end();++ph){
	if(this->hypotheses.find(*ph)==this->hypotheses.end()){
	  this->writeBehaviourTraitsSpecialisation(*ph,coefSize,stateVarsSize,
						   externalStateVarsSize,false);
	}
      }
    } else {
      // on définit toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(MH::UNDEFINEDHYPOTHESIS,
					       coefSize,stateVarsSize,
					       externalStateVarsSize,false);
      // unsupported hypothesis
      for(ph2=this->hypotheses.begin();ph2!=this->hypotheses.end();++ph2){
	this->writeBehaviourTraitsSpecialisation(*ph2,coefSize,stateVarsSize,
						 externalStateVarsSize,true);
      }
    }
  }

  void
  MFrontBehaviourParserCommon::writeBehaviourTraitsSpecialisation(const tfel::material::ModellingHypothesis::Hypothesis h,
								  const SupportedTypes::TypeSize coefSize,
								  const SupportedTypes::TypeSize stateVarsSize,
								  const SupportedTypes::TypeSize externalStateVarsSize,
								  const bool b)
  {
    using namespace tfel::material;
    typedef ModellingHypothesis MH;
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* Partial specialisation for ";
    this->behaviourFile << this->className << ".\n";
    this->behaviourFile << "*/\n";
    if(h==MH::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->className << "<hypothesis,Type,use_qt> >\n";
      } else {
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->className << "<hypothesis,Type,false> >\n";
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourFile << "template<typename Type,bool use_qt>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->className << "<ModellingHypothesis::" << MH::HypothesisToString(h) << ",Type,use_qt> >\n";
      } else {
	this->behaviourFile << "template<typename Type>\n";
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->className << "<ModellingHypothesis::" << MH::HypothesisToString(h) << ",Type,false> >\n";
      }
    }
    this->behaviourFile << "{\n";
    if(h==MH::UNDEFINEDHYPOTHESIS){
      this->behaviourFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    } else {
      this->behaviourFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<"
			  << "ModellingHypothesis::" << MH::HypothesisToString(h) << ">::value;\n";
    }
    this->behaviourFile << "static const unsigned short TVectorSize = N;\n";
    this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourFile << "static const unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;\n";
    this->behaviourFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
    this->behaviourFile << "static const unsigned short TensorSize = ";
    this->behaviourFile << "TensorDimeToSize::value;\n";
    this->behaviourFile << "public:\n";
    if(b){
      this->behaviourFile << "static const bool is_defined = true;\n";
    } else {
      this->behaviourFile << "static const bool is_defined = false;\n";
    }
    if(this->mb.useQt()){
      this->behaviourFile << "static const bool use_quantities = use_qt;\n";
    } else {
      this->behaviourFile << "static const bool use_quantities = false;\n";
    }
    this->behaviourFile << "static const unsigned short dimension = N;\n";
    this->behaviourFile << "typedef Type NumType;\n";
    this->behaviourFile << "static const unsigned short material_properties_nb = ";
    this->behaviourFile << coefSize << ";\n";
    this->behaviourFile << "static const unsigned short internal_variables_nb  = ";
    this->behaviourFile << stateVarsSize << ";\n";
    this->behaviourFile << "static const unsigned short external_variables_nb  = ";
    this->behaviourFile << externalStateVarsSize << ";\n";
    this->behaviourFile << "static const bool hasConsistantTangentOperator = ";
    if(this->hasConsistantTangentOperator){
      this->behaviourFile << "true;\n";
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static const bool isConsistantTangentOperatorSymmetric = ";
    if(this->isConsistantTangentOperatorSymmetric){
      this->behaviourFile << "true;\n";
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static const bool hasPredictionOperator = ";
    if(this->hasPredictionOperator){
      this->behaviourFile << "true;\n";
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "static const bool hasTimeStepScalingFactor = ";
    if(this->hasTimeStepScalingFactor){
      this->behaviourFile << "true;\n";
    } else {
      this->behaviourFile << "false;\n";
    }
    this->behaviourFile << "};\n\n";
  }

  void 
  MFrontBehaviourParserCommon::writeBehaviourParserSpecificInheritanceRelationship(void)
  {
    using namespace std;
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificTypedefs(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificMembers(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificIncludes(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void MFrontBehaviourParserCommon::writeBehaviourParametersInitializer()
  {
    using namespace std;
    if(!this->mb.getParameters().empty()){
      VarContainer::const_iterator p;
      bool rp = false;
      bool ip = false;
      bool up = false;
      this->checkBehaviourFile();
      this->behaviourFile << "struct " << this->className << "ParametersInitializer\n"
			  << "{\n"
			  << "static " << this->className << "ParametersInitializer&\n"
			  << "get();\n\n";
      for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
	if(p->type=="real"){
	  rp = true;
	  this->behaviourFile << "double " << p->name << ";\n"; 
	} else 	if(p->type=="int"){
	  ip = true;
	  this->behaviourFile << "double " << p->name << ";\n"; 
	} else 	if(p->type=="ushort"){
	  up = true;
	  this->behaviourFile << "unsigned short " << p->name << ";\n"; 
	} else {
	  string msg("MFrontBehaviourParserCommon::writeBehaviourParametersInitializer : ");
	  msg += "invalid type for parameter '"+p->name+"' ('"+p->type+"')";
	  throw(runtime_error(msg));
	}
      }
      this->behaviourFile << "\n"; 
      if(rp){
	this->behaviourFile << "void set(const char* const,const double);\n\n";
      }
      if(ip){
	this->behaviourFile << "void set(const char* const,const int);\n\n";
      }
      if(up){
	this->behaviourFile << "void set(const char* const,const unsigned short);\n\n";
      }
      this->behaviourFile << "private :\n\n"
			  << this->className << "ParametersInitializer();\n\n"
			  << this->className << "ParametersInitializer(const " << this->className << "ParametersInitializer&);\n\n"
			  << this->className << "ParametersInitializer&\n"
			  << "operator=(const " << this->className << "ParametersInitializer&);\n\n";
      this->behaviourFile << "};\n\n";
    }
    
  } // end of MFrontBehaviourParserCommon::writeBehaviourParametersInitializer

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificConstructorPart(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void MFrontBehaviourParserCommon::writeBehaviourFile(void){
    using namespace std;
    this->checkBehaviourFile();
    this->writeBehaviourFileHeader();
    this->writeBehaviourFileHeaderBegin();
    this->writeBehaviourIncludeBehaviourData();
    this->writeBehaviourParserSpecificIncludes();
    if(!this->includes.empty()){
      this->writeIncludes(this->behaviourFile);
    }
    this->writeNamespaceBegin(this->behaviourFile);
    this->writeBehaviourParametersInitializer();
    this->writeBehaviourClassBegin();
    this->writeBehaviourStandardTFELTypedefs();
    this->writeBehaviourParserSpecificTypedefs();
    this->writeBehaviourStaticVars();
    this->writeBehaviourStateVarsIncrements();
    this->writeBehaviourLocalVars();
    this->writeBehaviourParameters();
    this->writeBehaviourParserSpecificMembers();
    this->writeBehaviourUpdateStateVars();
    this->writeBehaviourUpdateAuxiliaryStateVars();
    this->writeBehaviourMembersFunc();
    this->writeBehaviourPrivate();
    this->writeBehaviourDisabledConstructors();
    // from this point, all is public
    this->writeBehaviourGetName();
    this->writeBehaviourConstructors();
    this->writeBehaviourSetOutOfBoundsPolicy();
    this->writeBehaviourGetModellingHypothesis();
    this->writeBehaviourCheckBounds();
    this->writeBehaviourComputePredictionOperator();
    this->writeBehaviourIntegrator();
    this->writeBehaviourComputeTangentOperator();
    this->writeBehaviourGetTangentOperator();
    this->writeBehaviourGetTimeStepScalingFactor();
    this->writeBehaviourUpdateExternalStateVariables();
    this->writeBehaviourDestructor();
    this->checkBehaviourFile();
    this->behaviourFile << "private:" << endl << endl;
    this->writeBehaviourTangentStiffnessOperator();
    this->writeBehaviourPolicyVariable();
    this->writeBehaviourClassEnd();
    this->writeBehaviourOutputOperator();
    this->writeBehaviourTraits();
    this->writeNamespaceEnd(this->behaviourFile);
    this->writeBehaviourFileHeaderEnd();
  }

  void MFrontBehaviourParserCommon::writeBehaviourComputePredictionOperator(void)
  {
    if(!this->predictionOperator.empty()){
      this->behaviourFile << "IntegrationResult computePredictionOperator(const SMType smt){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << this->predictionOperator;
      this->behaviourFile << "return SUCCESS;\n";
      this->behaviourFile << "}\n\n";
    } else {
      this->behaviourFile << "IntegrationResult computePredictionOperator(const SMType){\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "string msg(\"" << this->className<< "::computePredictionOperator : \");\n";
      this->behaviourFile << "msg +=\"unimplemented feature\";\n";
      this->behaviourFile << "throw(runtime_error(msg));\n";
      this->behaviourFile << "return FAILURE;\n";
      this->behaviourFile << "}\n\n";
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputePredictionOperator(void)

  void MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator(void)
  {
    this->behaviourFile << "bool computeConsistantTangentOperator(const SMType){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "string msg(\"" << this->className<< "::computeConsistantTangentOperator : \");\n";
    this->behaviourFile << "msg +=\"unimplemented feature\";\n";
    this->behaviourFile << "throw(runtime_error(msg));\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator(void)

  void MFrontBehaviourParserCommon::writeBehaviourGetTangentOperator()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "const StiffnessTensor&\n";
    this->behaviourFile << "getTangentOperator(void) const{\n";
    this->behaviourFile << "return this->Dt;\n";
    this->behaviourFile << "}\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator(void)

  void MFrontBehaviourParserCommon::writeBehaviourGetTimeStepScalingFactor()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "real\n";
    this->behaviourFile << "getTimeStepScalingFactor(void) const{\n";
    this->behaviourFile << "return real(1);\n";
    this->behaviourFile << "}\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator(void)

  void MFrontBehaviourParserCommon::writeBehaviourTangentStiffnessOperator()
  {
    this->checkBehaviourFile();
    this->behaviourFile << "//! Tangent operator;\n";
    this->behaviourFile << "StiffnessTensor Dt;\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourTangentStiffnessOperator()

  void MFrontBehaviourParserCommon::checkIntegrationDataFile() const {
    using namespace std;
    if((!this->integrationDataFile)||
       (!this->integrationDataFile.good())){
      string msg("MFrontBehaviourParserCommon::checkIntegrationDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileHeader(){
    using namespace std;

    this->checkIntegrationDataFile();

    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << "* \\file   " << this->integrationDataFileName << endl;
    this->integrationDataFile << "* \\brief  " << "this file implements the " 
			      << this->className << "IntegrationData" << " class." << endl;
    this->integrationDataFile << "*         File generated by ";
    this->integrationDataFile << MFrontHeader::getVersionName() << " ";
    this->integrationDataFile << "version " << MFrontHeader::getVersionNumber();
    this->integrationDataFile << endl;
    if(!this->authorName.empty()){
      this->integrationDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->integrationDataFile << "* \\date   " << this->date       << endl;
    }
    this->integrationDataFile << " */" << endl;
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileHeaderBegin(){
    using namespace std;
    this->checkIntegrationDataFile();

    this->integrationDataFile << "#ifndef _LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->className);
    this->integrationDataFile << "_INTEGRATION_DATA_HXX_" << endl;
    this->integrationDataFile << "#define _LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->className);
    this->integrationDataFile << "_INTEGRATION_DATA_HXX_" << endl;
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileHeaderEnd()
  {
    using namespace std;

    this->checkIntegrationDataFile();

    this->integrationDataFile << "#endif /* _LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->className);
    this->integrationDataFile << "_INTEGRATION_DATA_HXX_ */" << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataStandardTFELIncludes(void)
  {
    using namespace std;
    bool b1 = false;
    bool b2 = false;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "#include<iostream>\n";
    this->integrationDataFile << "#include<algorithm>\n";
    this->integrationDataFile << "#include<string>\n\n";
    this->integrationDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/Config/TFELTypes.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/Metaprogramming/IsSameType.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/Metaprogramming/EnableIf.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsScalar.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n";
    this->integrationDataFile << "#include\"TFEL/TypeTraits/Promote.hxx\"\n";
    this->requiresTVectorOrVectorIncludes(b1,b2);
    if(b1){
      this->integrationDataFile << "#include\"TFEL/Math/tvector.hxx\"\n";
    }
    if(b2){
      this->integrationDataFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    this->integrationDataFile << "#include\"TFEL/Material/MechanicalIntegrationData.hxx\"\n\n";
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataDefaultMembers(void)
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "protected: \n\n";
    for(p=this->mb.getMainVariables().begin();p!=this->mb.getMainVariables().end();++p){
      if(p->first.increment_known){
	this->integrationDataFile << "/*!\n";
	this->integrationDataFile << " * \\brief " << p->first.name << " increment\n";
	this->integrationDataFile << " */\n";
	this->integrationDataFile << p->first.type  << " d" << p->first.name << ";\n\n";
      } else {
	this->integrationDataFile << "/*!\n";
	this->integrationDataFile << " * \\brief " << p->first.name << " at the end of the time step\n";
	this->integrationDataFile << " */\n";
	this->integrationDataFile << p->first.type  << " " << p->first.name << "1;\n\n";
      }
    }
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << " * \\brief time increment\n";
    this->integrationDataFile << " */\n";
    this->integrationDataFile << "time dt;\n";
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << " * \\brief temperature increment\n";
    this->integrationDataFile << " */\n";
    this->integrationDataFile << "temperature dT;\n";
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->writeStandardTFELTypedefs(this->integrationDataFile);
    this->integrationDataFile << endl;
    this->integrationDataFile << "static const unsigned short TVectorSize = N;\n";
    this->integrationDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->integrationDataFile << "static const unsigned short StensorSize = ";
    this->integrationDataFile << "StensorDimeToSize::value;\n";
    this->integrationDataFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n";
    this->integrationDataFile << "static const unsigned short TensorSize = ";
    this->integrationDataFile << "TensorDimeToSize::value;\n";
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataGetName(void){    
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "public:\n";
    this->integrationDataFile << endl;
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\brief  Return the name of the class.\n";
    this->integrationDataFile << "* \\param  void.\n";
    this->integrationDataFile << "* \\return const std::string, the name of the class.\n";
    this->integrationDataFile << "* \\see    Name.\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << "static std::string\n";
    this->integrationDataFile << "getName(void){\n";
    this->integrationDataFile << "return std::string(\"";
    this->integrationDataFile << this->className << "IntegrationData\");\n";
    this->integrationDataFile << "}\n\n";
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataDisabledConstructors(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataConstructors(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\brief Default constructor\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << this->className << "IntegrationData()\n";
    this->integrationDataFile << "{}\n\n";
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\brief Copy constructor\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << this->className << "IntegrationData(const ";
    this->integrationDataFile << this->className << "IntegrationData& src)\n";
    this->integrationDataFile << ": ";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "d" <<p2->first.name  << "(src.d" << p2->first.name << "),\n";
      } else {
	this->integrationDataFile << p2->first.name  << "1(src." << p2->first.name << "1),\n";
      }
    }
    this->integrationDataFile << "dt(src.dt),\n";
    this->integrationDataFile << "dT(src.dT)";
    if(!this->mb.getExternalStateVariables().empty()){
      for(p =this->mb.getExternalStateVariables().begin();
	  p!=this->mb.getExternalStateVariables().end();++p){
	this->integrationDataFile << ",\n";
	this->integrationDataFile << "d" << p->name << "(src.d" << p->name << ")";
      }
    }
    this->integrationDataFile << "\n{}\n\n";
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << "* \\brief Constructor\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << this->className 
			      << "IntegrationData(const ";
    if(this->mb.useQt()){
      this->integrationDataFile << "MechanicalIntegrationData<N,Type,use_qt>& src)\n";
    } else {
      this->integrationDataFile << "MechanicalIntegrationData<N,Type,false>& src)\n";
    }
    this->integrationDataFile << ": \n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "d" <<p2->first.name  << "(src.d" << p2->first.name << "),\n";
      } else {
	this->integrationDataFile << p2->first.name  << "1(src." << p2->first.name << "1),\n";
      }
    }
    this->integrationDataFile << "dt(src.dt),\n";
    this->integrationDataFile << "dT(src.dT)";
    if(!this->mb.getExternalStateVariables().empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(this->integrationDataFile,
								 this->mb.getExternalStateVariables(),
								 "src.dexternal_variables","d","");
    }
    this->integrationDataFile << "\n{\n";
    this->writeVariableInitializersInBehaviourDataConstructorII(this->integrationDataFile,
								this->mb.getExternalStateVariables(),
								"src.dexternal_variables","d","");
    this->integrationDataFile << "}\n\n";
    // Creating constructor for external interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin(); i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeIntegrationDataConstructor(this->integrationDataFile,
						 this->className,
						 this->mb);
    }
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataScaleOperators(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*\n";
    this->integrationDataFile << "* Multiplication by a scalar.\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << "template<typename Scal>\n";
    this->integrationDataFile << "typename tfel::meta::EnableIf<\n";
    this->integrationDataFile << "tfel::typetraits::IsFundamentalNumericType<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::typetraits::IsScalar<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::typetraits::IsReal<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::meta::IsSameType<Type," 
			      << "typename tfel::typetraits::Promote"
			      << "<Type,Scal>::type>::cond,\n";
    this->integrationDataFile << this->className << "IntegrationData&\n"
			      << ">::type\n";
    this->integrationDataFile << "operator *= (const Scal s){\n";
    this->integrationDataFile << "this->dt   *= s;\n";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "this->d" <<p2->first.name  << " *= s;\n";
      } else {
	string msg("MFrontBehaviourParserCommon::writeIntegrationDataScaleOperators : ");
	msg += "unimplemented feature.";
	throw(runtime_error(msg));
      }
    }
    this->integrationDataFile << "this->dT   *= s;\n";
    for(p=this->mb.getExternalStateVariables().begin();p!=this->mb.getExternalStateVariables().end();++p){
      this->integrationDataFile << "this->d" << p->name << " *= s;\n";
    }
    this->integrationDataFile << "return *this;\n";
    this->integrationDataFile << "}\n\n";
    this->integrationDataFile << "/*\n";
    this->integrationDataFile << "* Division by a scalar.\n";
    this->integrationDataFile << "*/\n";
    this->integrationDataFile << "template<typename Scal>\n";
    this->integrationDataFile << "typename tfel::meta::EnableIf<\n";
    this->integrationDataFile << "tfel::typetraits::IsFundamentalNumericType<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::typetraits::IsScalar<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::typetraits::IsReal<Scal>::cond&&\n";
    this->integrationDataFile << "tfel::meta::IsSameType<Type," 
			      << "typename tfel::typetraits::Promote"
			      << "<Type,Scal>::type>::cond,\n";
    this->integrationDataFile << this->className << "IntegrationData&\n"
			      << ">::type\n";
    this->integrationDataFile << "operator /= (const Scal s){\n";
    this->integrationDataFile << "return this->operator*=(1/s);\n";
    this->integrationDataFile << "}\n\n";
  } // end of MFrontBehaviourParserCommon::writeIntegrationDataScaleOpeartors

  void MFrontBehaviourParserCommon::writeIntegrationDataClassHeader(void) 
  {
    using namespace std;

    this->checkIntegrationDataFile();

    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << "* \\class " << this->className << "IntegrationData"  << endl;
    this->integrationDataFile << "* \\brief This class implements the " 
			      << this->className << "IntegrationData" << " behaviour." << endl;
    this->integrationDataFile << "* \\param unsigned short N, space dimension." << endl;
    this->integrationDataFile << "* \\param typename Type, numerical type." << endl;
    this->integrationDataFile << "* \\param bool use_qt, conditional saying if quantities are use." << endl;
    if(!this->authorName.empty()){
      this->integrationDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->integrationDataFile << "* \\date   " << this->date << endl;
    }
    this->integrationDataFile << "*/" << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataClassBegin(void) {
    using namespace std;

    this->checkIntegrationDataFile();
    this->integrationDataFile << "// Forward Declaration" << endl;
    this->integrationDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
    this->integrationDataFile << "class " << this->className << "IntegrationData;\n\n";

    if(this->mb.useQt()){
      this->integrationDataFile << "// Forward Declaration" << endl;
      this->integrationDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
      this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->integrationDataFile << "const " << this->className 
				<< "IntegrationData<N,Type,use_qt>&);\n\n";
    } else {
      this->integrationDataFile << "// Forward Declaration" << endl;
      this->integrationDataFile << "template<unsigned short N,typename Type>" << endl;
      this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->integrationDataFile << "const " << this->className 
				<< "IntegrationData<N,Type,false>&);\n\n";
    }

    if(this->mb.useQt()){
      this->integrationDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
      this->integrationDataFile << "class " << this->className << "IntegrationData\n";
    } else {
      this->integrationDataFile << "template<unsigned short N,typename Type>\n";
      this->integrationDataFile << "class " << this->className 
				<< "IntegrationData<N,Type,false>" << endl;
    }

    this->integrationDataFile << "{" << endl;
    this->integrationDataFile << endl;
    this->integrationDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);" << endl;
    this->integrationDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			      << "IsFundamentalNumericType<Type>::cond);\n";
    this->integrationDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);\n\n";
    this->integrationDataFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->integrationDataFile << this->className << "IntegrationData&);\n\n";
    this->writeIntegerConstants(this->integrationDataFile);
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataOutputOperator(void)
  {    
    using namespace std;
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    if(this->mb.useQt()){        
      this->integrationDataFile << "template<unsigned short N,typename Type,bool use_qt>" << endl;
      this->integrationDataFile << "std::ostream&\n";
      this->integrationDataFile << "operator <<(std::ostream& os,";
      this->integrationDataFile << "const " << this->className 
				<< "IntegrationData<N,Type,use_qt>& b)\n";
    } else {
      this->integrationDataFile << "template<unsigned short N,typename Type>" << endl;
      this->integrationDataFile << "std::ostream&\n";
      this->integrationDataFile << "operator <<(std::ostream& os,";
      this->integrationDataFile << "const " << this->className 
				<< "IntegrationData<N,Type,false>& b)\n";
    }
    this->integrationDataFile << "{" << endl;
    this->integrationDataFile << "using namespace std;" << endl;
    if(this->mb.useQt()){        
      this->integrationDataFile << "os << " << this->className 
				<< "IntegrationData<N,Type,use_qt>::getName() << endl;\n";
    } else {
      this->integrationDataFile << "os << " << this->className 
				<< "IntegrationData<N,Type,false>::getName() << endl;\n";
    }
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "os << \"d" << p2->first.name << " : \" << b.d" << p2->first.name << " << endl;\n";
      } else {
	this->integrationDataFile << "os << \"" << p2->first.name << "1 : \" << b." << p2->first.name << "1 << endl;\n";
      }
    }
    this->integrationDataFile << "os << \"dt : \" << b.dt << endl;\n";
    this->integrationDataFile << "os << \"dT : \" << b.dT << endl;\n";
    for(p=this->mb.getExternalStateVariables().begin();p!=this->mb.getExternalStateVariables().end();++p){
      this->integrationDataFile << "os << \"d" << p->name << " : \" << b.d" 
				<< p->name << " << endl;\n";  
    }
    this->integrationDataFile << "return os;\n";
    this->integrationDataFile << "}\n\n";
  } // end of MFrontBehaviourParserCommon::writeIntegrationDataOutputOperator()

  void MFrontBehaviourParserCommon::writeIntegrationDataClassEnd() {    
    using namespace std;

    this->checkIntegrationDataFile();

    this->integrationDataFile << "}; // end of " << this->className << "IntegrationData" 
			      << "class" << endl;
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataExternalStateVars() {
    using namespace std;
    this->checkIntegrationDataFile();
    this->writeVariablesDeclarations(this->integrationDataFile,
				     this->mb.getExternalStateVariables(),
				     "d","",this->fileName,
				     false,this->debugMode);
  }


  void MFrontBehaviourParserCommon::writeIntegrationDataFile(void){
    using namespace std;
    this->checkIntegrationDataFile();
    this->writeIntegrationDataFileHeader();
    this->writeIntegrationDataFileHeaderBegin();
    this->writeIntegrationDataStandardTFELIncludes();
    if(!this->includes.empty()){
      this->writeIncludes(this->integrationDataFile);
    }
    this->writeNamespaceBegin(this->integrationDataFile);
    this->writeIntegrationDataClassBegin();
    this->writeIntegrationDataStandardTFELTypedefs();
    this->writeIntegrationDataDefaultMembers();
    this->writeIntegrationDataExternalStateVars();
    this->writeIntegrationDataDisabledConstructors();
    this->writeIntegrationDataGetName();
    this->writeIntegrationDataConstructors();
    this->writeIntegrationDataScaleOperators();
    this->writeIntegrationDataClassEnd();
    this->writeIntegrationDataOutputOperator();
    this->writeNamespaceEnd(this->integrationDataFile);
    this->writeIntegrationDataFileHeaderEnd();
  }

  void MFrontBehaviourParserCommon::checkSrcFile(void) const {
    using namespace std;
    if((!this->integrationDataFile)||
       (!this->integrationDataFile.good())){
      string msg("MFrontBehaviourParserCommon::checkSrcOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeSrcFileHeader(void){
    using namespace std;
    this->checkSrcFile();
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << this->className << " Behaviour." << endl;
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!this->authorName.empty()){
      this->srcFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->srcFile << "* \\date   " << this->date       << endl;
    }
    this->srcFile << " */" << endl;
    this->srcFile << endl;
    if(!this->mb.getParameters().empty()){
      this->srcFile << "#include<cstring>\n";
    }
    this->srcFile << endl;
    this->srcFile << "#include\"TFEL/Material/" << this->behaviourDataFileName   << "\"\n";
    this->srcFile << "#include\"TFEL/Material/" << this->integrationDataFileName << "\"\n";
    this->srcFile << "#include\"TFEL/Material/" << this->behaviourFileName       << "\"\n";
    this->srcFile << endl;
  } // end of MFrontBehaviourParserCommon::writeSrcFileHeader()

  void MFrontBehaviourParserCommon::writeSrcFileStaticVars(void){
    using namespace std;
    this->checkSrcFile();
    vector<string> m;
    StaticVarContainer::const_iterator p;
    vector<string>::const_iterator pm;
    m.push_back("tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN");
    m.push_back("tfel::material::ModellingHypothesis::AXISYMMETRICAL");
    m.push_back("tfel::material::ModellingHypothesis::PLANESTRESS");
    m.push_back("tfel::material::ModellingHypothesis::PLANESTRAIN");
    m.push_back("tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN");
    m.push_back("tfel::material::ModellingHypothesis::TRIDIMENSIONAL");
    for(p=this->staticVars.begin();p!=this->staticVars.end();++p){
      for(pm=m.begin();pm!=m.end();++pm){
	if(this->mb.useQt()){
	  this->srcFile << "template<>\n";
	  this->srcFile << this->className << "<" << *pm << ",float,true>::" 
			<< p->type << "\n" << this->className 
			<< "<" << *pm << ",float,true>::" 
			<< p->name << " = " << this->className 
			<< "<" << *pm << ",float,true>::" << p->type 
			<< "(static_cast<float>(" << p->value <<"));\n\n";
	}
	this->srcFile << "template<>\n";
	this->srcFile << this->className << "<" << *pm << ",float,false>::" 
		      << p->type << "\n" << this->className 
		      << "<" << *pm << ",float,false>::" 
		      << p->name << " = " << this->className 
		      << "<" << *pm << ",float,false>::" << p->type 
		      << "(static_cast<float>(" << p->value <<"));\n\n";
	if(this->mb.useQt()){
	  this->srcFile << "template<>\n";
	  this->srcFile << this->className << "<" << *pm << ",double,true>::" 
			<< p->type << "\n" << this->className 
			<< "<" << *pm << ",double,true>::" 
			<< p->name << " = " << this->className 
			<< "<" << *pm << ",double,true>::" << p->type 
			<< "(static_cast<double>(" << p->value <<"));\n\n";
	}
	this->srcFile << "template<>\n";
	this->srcFile << this->className << "<" << *pm << ",double,false>::" 
		      << p->type << "\n" << this->className 
		      << "<" << *pm << ",double,false>::" 
		      << p->name << " = " << this->className 
		      << "<" << *pm << ",double,false>::" << p->type 
		      << "(static_cast<double>(" << p->value <<"));\n\n";
	if(this->mb.useQt()){
	  this->srcFile << "template<>\n";
	  this->srcFile << this->className << "<" << *pm << ",long double,true>::" 
			<< p->type << "\n" << this->className 
			<< "<" << *pm << ",long double,true>::" 
			<< p->name << " = " << this->className << "<" 
			<< *pm << ",long double,true>::" << p->type 
			<< "(static_cast<long double>(" << p->value <<"));\n\n";
	}
	this->srcFile << "template<>\n";
	this->srcFile << this->className << "<" << *pm << ",long double,false>::" 
		      << p->type << "\n" << this->className 
		      << "<" << *pm << ",long double,false>::" 
		      << p->name << " = " << this->className 
		      << "<" << *pm << ",long double,false>::" << p->type 
		      << "(static_cast<long double>(" << p->value <<"));\n\n";
      }
    }
  } // end of MFrontBehaviourParserCommon::writeSrcFileStaticVars
  
  void
  MFrontBehaviourParserCommon::writeSrcFileUserDefinedCode(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    if(!this->sources.empty()){
      this->srcFile << this->sources << "\n\n";
    }
  } // end of MFrontBehaviourParserCommon::writeSrcFileUserDefinedCode

  void
  MFrontBehaviourParserCommon::writeSrcFileParametersInitializer(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    if(!this->mb.getParameters().empty()){
      bool rp = false; // real    parameter found
      bool ip = false; // integer parameter found
      bool up = false; // unsigned short parameter found
      VarContainer::const_iterator p;
      std::map<std::string,double>::const_iterator p2;
      std::map<std::string,int>::const_iterator p3;
      std::map<std::string,unsigned short>::const_iterator p4;
      this->srcFile << this->className << "ParametersInitializer&\n"
		    << this->className << "ParametersInitializer::get()\n"
		    <<"{\n"
		    << "static " << this->className << "ParametersInitializer i;\n"
		    << "return i;\n"
		    << "}\n\n";
      this->srcFile << this->className << "ParametersInitializer::" 
		    << this->className << "ParametersInitializer()\n"
		    <<"{\n";
      for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
	if(p->type=="real"){
	  rp = true;
	  p2 = this->mb.getParametersDefaultValues().find(p->name);
	  if(p2==this->mb.getParametersDefaultValues().end()){
	    string msg("MFrontBehaviourParserCommon::writeSrcFileParametersInitializer : ");
	    msg += "no default value for parameter '"+p->name+"'";
	    throw(runtime_error(msg));
	  }
	  this->srcFile << "this->" << p->name << " = " << p2->second << ";\n"; 
	} else if(p->type=="int"){
	  ip = true;
	  p3 = this->mb.getIntegerParametersDefaultValues().find(p->name);
	  if(p3==this->mb.getIntegerParametersDefaultValues().end()){
	    string msg("MFrontBehaviourParserCommon::writeSrcFileParametersInitializer : ");
	    msg += "no default value for parameter '"+p->name+"'";
	    throw(runtime_error(msg));
	  }
	  this->srcFile << "this->" << p->name << " = " << p3->second << ";\n"; 
	} else if(p->type=="ushort"){
	  up = true;
	  p4 = this->mb.getUnsignedShortParametersDefaultValues().find(p->name);
	  if(p4==this->mb.getUnsignedShortParametersDefaultValues().end()){
	    string msg("MFrontBehaviourParserCommon::writeSrcFileParametersInitializer : ");
	    msg += "no default value for parameter '"+p->name+"'";
	    throw(runtime_error(msg));
	  }
	  this->srcFile << "this->" << p->name << " = " << p4->second << ";\n"; 
	}
      }
      this->srcFile <<"}\n\n";
      if(rp){
	this->srcFile <<"void\n"
		      << this->className << "ParametersInitializer::set(const char* const key,\nconst double v)" 
		      << "{\n"
		      << "using namespace std;\n";
	bool first = true;
	for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
	  if(p->type=="real"){
	    if(first){
	      this->srcFile << "if(";
	      first = false;
	    } else {
	      this->srcFile << "} else if(";
	    }
	    this->srcFile << "::strcmp(\""+p->name+"\",key)==0){\n"
			  << "this->" << p->name << " = v;\n";
	  }
	}
	this->srcFile << "} else {";
	this->srcFile << "string msg(\"" << this->className << "ParametersInitializer::set : \");\n"
		      << "msg += \" no paramater named '\";\n"
		      << "msg += key;\n"
		      << "msg += \"'\";\n"
		      << "throw(runtime_error(msg));\n"
		      << "}\n"
		      << "}\n\n";
      }
      if(ip){
	this->srcFile <<"void\n"
		      << this->className << "ParametersInitializer::set(const char* const key,\nconst int v)" 
		      << "{\n"
		      << "using namespace std;\n";
	bool first = true;
	for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
	  if(p->type=="int"){
	    if(first){
	      this->srcFile << "if(";
	      first = false;
	    } else {
	      this->srcFile << "} else if(";
	    }
	    this->srcFile << "::strcmp(\""+p->name+"\",key)==0){\n"
			  << "this->" << p->name << " = v;\n";
	  }
	}
	this->srcFile << "} else {";
	this->srcFile << "string msg(\"" << this->className << "ParametersInitializer::set : \");\n"
		      << "msg += \" no paramater named '\";\n"
		      << "msg += key;\n"
		      << "msg += \"'\";\n"
		      << "throw(runtime_error(msg));\n"
		      << "}\n"
		      << "}\n\n";
      }
      if(up){
	this->srcFile <<"void\n"
		      << this->className << "ParametersInitializer::set(const char* const key,\nconst unsigned short v)" 
		      << "{\n"
		      << "using namespace std;\n";
	bool first = true;
	for(p=this->mb.getParameters().begin();p!=this->mb.getParameters().end();++p){
	  if(p->type=="ushort"){
	    if(first){
	      this->srcFile << "if(";
	      first = false;
	    } else {
	      this->srcFile << "} else if(";
	    }
	    this->srcFile << "::strcmp(\""+p->name+"\",key)==0){\n"
			  << "this->" << p->name << " = v;\n";
	  }
	}
	this->srcFile << "} else {";
	this->srcFile << "string msg(\"" << this->className << "ParametersInitializer::set : \");\n"
		      << "msg += \" no paramater named '\";\n"
		      << "msg += key;\n"
		      << "msg += \"'\";\n"
		      << "throw(runtime_error(msg));\n"
		      << "}\n"
		      << "}\n\n";
      }
    }
  } // end of MFrontBehaviourParserCommon::writeSrcFileParametersInitializer

  void MFrontBehaviourParserCommon::writeSrcFile(void)
  {
    this->writeSrcFileHeader();
    this->writeSrcFileUserDefinedCode();
    this->writeNamespaceBegin(this->srcFile);
    this->writeSrcFileParametersInitializer();
    this->writeSrcFileStaticVars();
    this->writeNamespaceEnd(this->srcFile);
  } // end of MFrontBehaviourParserCommon::writeSrcFile(void)

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getGlobalIncludes(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    typedef map<string,vector<string> > Map;
    Map incs;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& iincs = interface->getGlobalIncludes(this->library,
						      this->material,
						      this->className);
      for(p=iincs.begin();p!=iincs.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(incs[p->first]));
      }
    }
    return incs;
  } // end of MFrontBehaviourParserCommon::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getGlobalDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    Map deps;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& ideps = interface->getGlobalDependencies(this->library,
							  this->material,
							  this->className);
      for(p=ideps.begin();p!=ideps.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(deps[p->first]));
      }
    }
    return deps;
  } // end of MFrontBehaviourParserCommon::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getGeneratedSources(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    Map osources;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& isources = interface->getGeneratedSources(this->library,
							   this->material,
							   this->className);
      for(p=isources.begin();p!=isources.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(osources[p->first]));
	osources[p->first].push_back(this->srcFileName);
      }
    }
    for(p=osources.begin();p!=osources.end();++p){
      for(p2=this->librariesDependencies.begin();
	  p2!=this->librariesDependencies.end();++p2){
	if("-l"+p->first!=*p2){
	  this->sourcesLibrairiesDependencies[p->first].push_back(*p2);
	}
      }
    }
    return osources;
  } // end of MFrontBehaviourParserCommonCommon::getGeneratedSources

  std::vector<std::string>
  MFrontBehaviourParserCommon::getGeneratedIncludes(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string> incs;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const vector<string>& iincs = interface->getGeneratedIncludes(this->library,
								    this->material,
								    this->className);
      copy(iincs.begin(),iincs.end(),back_inserter(incs));
    }
    incs.push_back(this->behaviourFileName);
    incs.push_back(this->behaviourDataFileName);
    incs.push_back(this->integrationDataFileName);
    return incs;
  } // end of MFrontBehaviourParserCommon::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getLibrariesDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& ideps = interface->getLibrariesDependencies(this->library,
							     this->material,
							     this->className);
      for(p=ideps.begin();p!=ideps.end();++p){
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  if(find(this->sourcesLibrairiesDependencies[p->first].begin(),
		  this->sourcesLibrairiesDependencies[p->first].end(),
		  *p2)==this->sourcesLibrairiesDependencies[p->first].end()){
	    this->sourcesLibrairiesDependencies[p->first].push_back(*p2);
	  }
	}
      }
    }
    return this->sourcesLibrairiesDependencies;
  } // end of MFrontBehaviourParserCommon::getLibrariesDependencies

  std::string
  MFrontBehaviourParserCommon::predictionOperatorVariableModifier(const std::string& var,
							       const bool addThisPtr)
  {
    if(this->mb.isInternalStateVariableIncrementName(var)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::predictionOperatorVariableModifier : ",
			      "state variable increment '"+var+"' can't be used in @PredictionOperator");
    }
    if(addThisPtr){
      return "(this->"+var+")";
    } else {
      return var;
    }
  } // end of MFrontBehaviourParserCommon::predictionOperatorVariableModifier

  void MFrontBehaviourParserCommon::treatPredictionOperator(void)
  {
    using namespace std;
    if(!this->predictionOperator.empty()){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatPredictionOperator",
			      "@PredictionOperator already used.");
    }
    this->predictionOperator = this->readNextBlock(makeVariableModifier(*this,&MFrontBehaviourParserCommon::predictionOperatorVariableModifier),
						   true);
    this->predictionOperator += "\n";
    this->hasPredictionOperator = true;
  } // end of MFrontBehaviourParserCommon::treatPredictionOperator

  void
  MFrontBehaviourParserCommon::treatLocalVar(void)
  {
    this->readVarList(this->mb.getLocalVariables(),true,false);
  } // end of MFrontBehaviourParserCommon::treatLocalVar

  void
  MFrontBehaviourParserCommon::treatParameter(void)
  {
    this->readVarList(this->mb.getParameters(),"real",false,false);
  } // end of MFrontBehaviourParserCommon::treatParameter

  void
  MFrontBehaviourParserCommon::treatInitLocalVars(void)
  {
    this->initLocalVars += this->readNextBlock(makeVariableModifier(*this,&MFrontBehaviourParserCommon::standardModifier),
					       true);
    this->initLocalVars += "\n";
  } // end of MFrontBehaviourParserCommon::treatInitLocalVars

  void
  MFrontBehaviourParserCommon::defineSmallStrainInputVariables(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    DrivingVariable eto;
    eto.name = "eto";
    eto.type = "StrainStensor";
    eto.increment_known = true;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->registerVariable("eto");
    this->registerVariable("deto");
    this->registerVariable("sig");
    this->mb.getMainVariables().insert(MVType(eto,sig));
  }

  void
  MFrontBehaviourParserCommon::defineCZMInputVariables(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    DrivingVariable u;
    u.name = "u";
    u.type = "DisplacementTVector";
    u.increment_known = true;
    ThermodynamicForce F;
    F.name = "F";
    F.type = "ForceTVector";
    this->registerVariable("u");
    this->registerVariable("du");
    this->registerVariable("F");
    this->mb.getMainVariables().insert(MVType(u,F));
  }

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontBehaviourParserCommon::getSpecificTargets(void)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontBehaviourParserCommon::getSpecificTargets(void)

} // end of namespace mfront
