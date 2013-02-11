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

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"

#include"MFront/MFrontBehaviourParserCommon.hxx"

namespace mfront{

  void MFrontBehaviourParserCommon::setVerboseMode(void)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->verboseMode = true;
    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->setVerboseMode();
    }
  }

  void MFrontBehaviourParserCommon::setDebugMode(void)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->debugMode = true;
    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->setDebugMode();
    }
  }

  void MFrontBehaviourParserCommon::setWarningMode(void)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->warningMode = true;
    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->setWarningMode();
    }
  }

  void
  MFrontBehaviourParserCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string& n)
  {
    if(!this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->behaviourCharacteristic.setUsableInPurelyImplicitResolution(false);
    }
    this->behaviourCharacteristic.declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(n);
  } // end of MFrontBehaviourParserCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  std::string
  MFrontBehaviourParserCommon::variableModifier3(const std::string& var,
						 const bool addThisPtr)
  {
    if((this->isExternalStateVariableIncrementName(var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontBehaviourParserCommon::variableModifier3

  bool
  MFrontBehaviourParserCommon::contains(const VarContainer& v,
					const std::string& n) const
  {
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      if(p->name==n){
	return true;
      }
    }
    return false;
  } // end of MFrontBehaviourParserCommon::contains

  bool
  MFrontBehaviourParserCommon::isMaterialPropertyName(const std::string& n) const
  {
    return this->contains(this->coefsHolder,n);
  } // end of MFrontBehaviourParserCommon::isMaterialPropertyName

  bool
  MFrontBehaviourParserCommon::isLocalVariableName(const std::string& n) const
  {
    return this->contains(this->localVarsHolder,n);
  } // end of MFrontBehaviourParserCommon::isLocalVariableName

  bool
  MFrontBehaviourParserCommon::isParameterName(const std::string& n) const
  {
    return this->contains(this->parametersHolder,n);
  } // end of MFrontBehaviourParserCommon::isParameterName
  
  bool
  MFrontBehaviourParserCommon::isInternalStateVariableName(const std::string& n) const
  {
    return this->contains(this->stateVarsHolder,n);
  } // end of MFrontBehaviourParserCommon::isInternalStateVariableName

  bool
  MFrontBehaviourParserCommon::isAuxiliaryInternalStateVariableName(const std::string& n) const
  {
    return this->contains(this->auxiliaryStateVarsHolder,n);
  } // end of MFrontBehaviourParserCommon::isInternalStateVariableName
  
  bool
  MFrontBehaviourParserCommon::isExternalStateVariableName(const std::string& n) const
  {
    return this->contains(this->externalStateVarsHolder,n);
  } // end of MFrontBehaviourParserCommon::isExternalStateVariableName

  bool
  MFrontBehaviourParserCommon::isExternalStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->contains(this->externalStateVarsHolder,n.substr(1));
  } // end of MFrontBehaviourParserCommon::isExternalStateVariableName
    
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
  MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution(void)
  {
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution",";");
    if(this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution",
			      "keyword '@UsableInPurelyImplicitResolution' already called");
    }
    this->explicitlyDeclaredUsableInPurelyImplicitResolution = true;
    this->behaviourCharacteristic.setUsableInPurelyImplicitResolution(true);
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
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatVariableMethod",
				"could not read default value for parameter '"+n+"'");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",")");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",";");
      if(!this->parametersDefaultValues.insert(MVType(n,value)).second){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatVariableMethod",
				"default value already defined for parameter '"+n+"'");
      }
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatVariableMethod",
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
    StringContainer::const_iterator i;
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
      this->behaviourCharacteristic.setUseQt(true);
    } else if(this->current->value=="false"){
      this->behaviourCharacteristic.setUseQt(false);
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
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatIsotropicBehaviour",";");
    this->behaviourCharacteristic.setBehaviourType(mfront::ISOTROPIC);
  } // end of MFrontBehaviourParserCommon::treatIsotropicBehaviour

  void
  MFrontBehaviourParserCommon::treatOrthotropicBehaviour(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatOrthotropicBehaviour",";");
    this->behaviourCharacteristic.setBehaviourType(mfront::ORTHOTROPIC);
  } // end of MFrontBehaviourParserCommon::treatOrthotropicBehaviour

  void
  MFrontBehaviourParserCommon::treatRequireStiffnessTensor(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireStiffnessTensor",";");
    this->behaviourCharacteristic.setRequireStiffnessTensor(true);
  } // end of MFrontBehaviourParserCommon::treatRequireStiffnessTensor

  void
  MFrontBehaviourParserCommon::treatRequireThermalExpansionTensor(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireThermalExpansionTensor",";");
    this->behaviourCharacteristic.setRequireThermalExpansionTensor(true);
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
  MFrontBehaviourParserCommon::readStringList(StringContainer& cont)
  {
    using namespace std;
    string type;
    string s;
    bool endOfTreatement;
    StringContainer::iterator p;
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
    this->readVarList(this->coefsHolder,true,false);
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

  void MFrontBehaviourParserCommon::treatComputedVar(void)
  {
    using namespace std;
    string type;
    string varName;
    string function;
    unsigned short lineNumber;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatComputedVar",
			    "Cannot read type of varName.");
    type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputedVar",
			      "type given is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatComputedVar",
			    "Cannot read var name.");
    if(!isValidIdentifier(this->current->value)){
      --(this->current);
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputedVar",
			      "type given is not valid.");
    }
    varName = this->current->value;
    lineNumber = this->current->line;
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatComputedVar","=");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatComputedVar",
			    "Expected function name.");
    if(!isValidFunctionIdentifier(this->current->value)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputedVar",
			      "function name is not valid (read '"+this->current->value+"')");
    }
    function  = this->current->value;
    ++(this->current);
    function += "(";
    function += this->readNextBlock(true,"(",")",false,false);
    function += ")";
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatComputedVar",";");
    this->computedVars.push_back(ComputedVarHandler(type,varName,lineNumber,function));
  } // end of MFrontBehaviourParserCommon::treatComputedVar

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
    this->readVarList(this->stateVarsHolder,true,false);
  }

  void MFrontBehaviourParserCommon::treatAuxiliaryStateVariables(void)
  {
    this->readVarList(this->auxiliaryStateVarsHolder,true,false);
  }

  void MFrontBehaviourParserCommon::treatExternalStateVariables(void)
  {
    this->readVarList(this->externalStateVarsHolder,true,true);
  }

  void MFrontBehaviourParserCommon::treatUnaryLoadingTest(void)
  {
    using namespace std;

    string test;
    string testVariable;
    unsigned short dimension;
    string numericType = "double";
    bool use_qt = false;
    bool isTimeDefined = false;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUnaryLoadingTest","<");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
			    "Expected loading name.");
    if((this->current->value!="\"sigxx\"")&&
       (this->current->value!="\"sigyy\"")&&
       (this->current->value!="\"sigzz\"")&&
       (this->current->value!="\"sigxy\"")&&
       (this->current->value!="\"sigxz\"")&&
       (this->current->value!="\"sigyz\"")&&
       (this->current->value!="\"etoxx\"")&&
       (this->current->value!="\"etoyy\"")&&
       (this->current->value!="\"etozz\"")&&
       (this->current->value!="\"etoxy\"")&&
       (this->current->value!="\"etoxz\"")&&       
       (this->current->value!="\"etoyz\"")&&
       (this->current->value!="\"Ux\"")&&
       (this->current->value!="\"Uy\"")&&       
       (this->current->value!="\"Uz\"")){
      string msg("Test description is not valid (read '");
      msg+= this->current->value+"').";
      msg+=".\nValid test descriptions are : \n";
      msg+="'sigxx' 'sigyy' 'sigzz' 'sigxy' 'sigxz' 'sigyz'\n" ;
      msg+="'etoxx' 'etoyy' 'etozz' 'etoxy' 'etoxz' 'etoyz'\n" ;
      msg+="'Ux' 'Uy' 'Uz'\n";
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",msg);
    } 
    test = this->current->value; 
    testVariable = test.substr(1,test.size()-2);
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUnaryLoadingTest",",");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
			    "Expected dimension.");
    istringstream tmp(this->current->value);
    tmp >> dimension;
    if(tmp.fail()){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
			      "Unable to treat the dimension (read '"+this->current->value+"')");
    }
    if(!tmp.eof()){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
			      "Error while reading the dimension. ('"+tmp.str()+
			      "' has not been parsed correctly).");
    }
    if((dimension!=1u)&&(dimension!=2u)&&(dimension!=3u)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
			      "Dimension must be either 1, 2 or 3 (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnaryLoadingTest");
    if(this->current->value!=">"){
      if(this->current->value!=","){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
				"Expected ',' or '>' (read '"+this->current->value+"')");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnaryLoadingTest");
      if((this->current->value!="float")&&
	 (this->current->value!="double")&&
	 (this->current->value!="long double")){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
				"Numeric type must be either a float, a double or a long double (read '"
				+this->current->value+"')");
      }
      numericType = this->current->value;
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnaryLoadingTest");
      if(this->current->value!=">"){
	if(this->current->value!=","){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
				  "Expected ',' or '>' (read '"+this->current->value+"')");
	}
	++(this->current);
	this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnaryLoadingTest");
	if((this->current->value!="true")&&
	   (this->current->value!="false")){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
				  "Expected 'true' or 'false'  (read '"+this->current->value+"')");
	}
	if(this->current->value=="true"){
	  if(!this->behaviourCharacteristic.useQt()){
	    this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
				    "This behaviour does not use quantities."
				    "Please have a look at the '@UseQt' directive.");
	  }
	  use_qt = true;
	} else {
	  use_qt = false;
	}
	++(this->current);
	this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUnaryLoadingTest",">");
      }
    }
    ++(this->current);
    UnaryLoadingDefinition definition(test,dimension,numericType,use_qt);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUnaryLoadingTest","{");
    while((this->current->value!="}")&&
	  (this->current!=this->fileTokens.end())){
      VarContainer::const_iterator p;
      bool found = false;
      if(this->current->value=="times"){
	if(isTimeDefined){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
				  "time already defined.");
	}
	isTimeDefined = true;
	++(this->current);
	this->current = definition.treatTimes(this->current,
					      this->fileTokens.end());
	found = true;
      }
      if(this->current->value=="T"){
	++(this->current);
	this->current = definition.treatParticularInput("T",
							this->current,
							this->fileTokens.end());
	found = true;
      }
      for(p =this->coefsHolder.begin();
	  (!found)&&(p!=this->coefsHolder.end());
	  ++p){
	if(p->name==this->current->value){
	  ++(this->current);
	  this->current = definition.treatCoef(p->name,
					       this->getTypeFlag(p->type),
					       this->current,
					       this->fileTokens.end());
	  found = true;
	}
      }
      for(p =this->stateVarsHolder.begin();
	  (!found)&&(p!=this->stateVarsHolder.end());
	  ++p){
	if(p->name==this->current->value){
	  ++(this->current);
	  this->current = definition.treatStateVar(p->name,
						   this->getTypeFlag(p->type),
						   this->current,
						   this->fileTokens.end());
	  found = true;
	}
      }
      for(p =this->auxiliaryStateVarsHolder.begin();
	  (!found)&&(p!=this->auxiliaryStateVarsHolder.end());
	  ++p){
	if(p->name==this->current->value){
	  ++(this->current);
	  this->current = definition.treatStateVar(p->name,
						   this->getTypeFlag(p->type),
						   this->current,
						   this->fileTokens.end());
	  found = true;
	}
      }
      for(p =this->externalStateVarsHolder.begin();
	  (!found)&&(p!=this->externalStateVarsHolder.end());
	  ++p){
	if(p->name==this->current->value){
	  ++(this->current);
	  this->current = definition.treatExternalStateVar(p->name,
							   this->getTypeFlag(p->type),
							   this->current,
							   this->fileTokens.end());
	  found = true;
	}
      }
      if(!found){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
				"unkwown keyword '"+this->current->value+"'");
      }
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnaryLoadingTest",
			    "Block defining the unary loading test is not finished.");
    ++(this->current);
    unaryLoadingDefinitions.push_back(definition);
  } // end of MFrontBehaviourParserCommon::treatUnaryLoadingTest

  void
  MFrontBehaviourParserCommon::treatBounds(void)
  {
    BoundsDescription boundsDescription;
    this->boundsDescriptions.push_back(boundsDescription);
    this->boundsDescriptions.back().category = BoundsDescription::Standard;
    this->treatBounds(this->boundsDescriptions.back());
  } // end of MFrontBehaviourParserCommon::treatBounds

  void
  MFrontBehaviourParserCommon::treatPhysicalBounds(void)
  {
    BoundsDescription boundsDescription;
    this->boundsDescriptions.push_back(BoundsDescription());
    this->boundsDescriptions.back().category = BoundsDescription::Physical;
    this->treatBounds(this->boundsDescriptions.back());
  } // end of MFrontBehaviourParserCommon::treatPhysicalBounds

  void MFrontBehaviourParserCommon::treatBounds(BoundsDescription& boundsDescription)
  {
    using namespace std;
    VarContainer::const_iterator p;
    ComputedVarContainer::const_iterator p2;
    bool found;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");

    boundsDescription.lineNumber = this->current->line;
    boundsDescription.varName = this->current->value;
    
    found = false;
    boundsDescription.arraySize = 1u;
    if(boundsDescription.varName=="sig"){
      found=true;
      boundsDescription.varCategory = BoundsDescription::StateVar;
      boundsDescription.varType     = Stensor;
    }
    if(boundsDescription.varName=="T"){
      found=true;
      boundsDescription.varCategory = BoundsDescription::StateVar;
      boundsDescription.varType     = Scalar;
    }
    if(boundsDescription.varName=="eto"){
      found=true;
      boundsDescription.varCategory = BoundsDescription::ExternalStateVar;
      boundsDescription.varType     = Stensor;
    }
    for(p   = this->coefsHolder.begin();
	(p != this->coefsHolder.end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::Coef;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->localVarsHolder.begin();
	(p != this->localVarsHolder.end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::LocalVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->stateVarsHolder.begin();
	(p != this->stateVarsHolder.end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::StateVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->auxiliaryStateVarsHolder.begin();
	(p != this->auxiliaryStateVarsHolder.end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::StateVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p   = this->externalStateVarsHolder.begin();
	(p != this->externalStateVarsHolder.end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::ExternalStateVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }
    for(p2   = this->computedVars.begin();
	(p2 != this->computedVars.end())&&(!found);++p2){
      if(p2->name==boundsDescription.varName){
	found=true;
	boundsDescription.varCategory = BoundsDescription::ComputedVar;
	boundsDescription.varType     = this->getTypeFlag(p->type);
	boundsDescription.arraySize   = p->arraySize;
      }
    }

    if(!found){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
			      this->current->value+" is not a valid identifier.");
    }
    
    if (boundsDescription.varType==Stensor){
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
    this->registerVariable("sig");
    this->registerVariable("F0");
    this->registerVariable("F1");
    this->registerVariable("eto");
    this->registerVariable("deto");
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
    this->reserveName("computeConsistantTangentOperator");
    this->reserveName("computeTangentOperator_");
    this->reserveName("hypothesis");
  } // end of MFrontBehaviourParserCommon::registerDefaultVarNames

  MFrontBehaviourParserCommon::MFrontBehaviourParserCommon()
    : ParserBase(),
      useStateVarTimeDerivative(false),
      explicitlyDeclaredUsableInPurelyImplicitResolution(false),
      hasConsistantTangentOperator(false),
      isConsistantTangentOperatorSymmetric(false)
  {
    // Register var names
    this->registerDefaultVarNames();
    // By default disable use of quantities
    this->behaviourCharacteristic.setUseQt(false);
    // By default, a behaviour is isotropic 
    this->behaviourCharacteristic.setBehaviourType(mfront::ISOTROPIC);
    // By default, a behaviour can be used in a purely implicit resolution
    this->behaviourCharacteristic.setUsableInPurelyImplicitResolution(true);
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

  void MFrontBehaviourParserCommon::writeStandardTFELTypedefs(std::ofstream& file) 
  {
    using namespace std;
    if((!file)||(!file.good())){
      string msg("MFrontBehaviourParserCommon::writeStandardTFELTypedefs : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    if(this->behaviourCharacteristic.useQt()){        
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
  } // end of MFrontBehaviourParserCommon::writeStandardTFELTypedefs

  MFrontBehaviourParserCommon::~MFrontBehaviourParserCommon()
  {} // end of MFrontBehaviourParserCommon::~MFrontParserCommon


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

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "#include<iostream>\n";
    this->behaviourDataFile << "#include<algorithm>\n";
    this->behaviourDataFile << "#include<string>\n\n";
    this->behaviourDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/Config/TFELTypes.hxx\""  << endl;
    this->behaviourDataFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"" << endl;
    if(this->behaviourCharacteristic.useQt()){
      this->behaviourDataFile << "#include\"TFEL/Math/General/BaseCast.hxx\"" << endl;
    }
    this->behaviourDataFile << "#include\"TFEL/Material/MechanicalBehaviourData.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/Material/ModellingHypothesis.hxx\"" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataDefaultMembers(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "protected: \n\n";
    if(this->behaviourCharacteristic.requiresStiffnessTensor()){
      this->behaviourDataFile << "StiffnessTensor D;\n";
    }
    if(this->behaviourCharacteristic.requiresThermalExpansionTensor()){
      this->behaviourDataFile << "ThermalExpansionTensor A;\n";
    }
    this->behaviourDataFile << "StressStensor sig;\n";
    this->behaviourDataFile << "StrainStensor eto;\n\n";
    this->behaviourDataFile << "temperature T;\n";
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeStandardTFELTypedefs(this->behaviourDataFile);
    this->behaviourDataFile << endl;
    this->behaviourDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourDataFile << "static const unsigned short StensorSize = ";
    this->behaviourDataFile << "StensorDimeToSize::value;\n";
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
    if(this->behaviourCharacteristic.requiresStiffnessTensor()){
      this->behaviourDataFile << "D(src.D),\n";
    }
    if(this->behaviourCharacteristic.requiresThermalExpansionTensor()){
      this->behaviourDataFile << "A(src.A),\n";
    }
    this->behaviourDataFile << "sig(src.sig),\n";
    this->behaviourDataFile << "eto(src.eto),\n";
    this->behaviourDataFile << "T(src.T)";
    if(!this->coefsHolder.empty()){
      for(p=this->coefsHolder.begin();p!=this->coefsHolder.end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!this->stateVarsHolder.empty()){
      for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!this->auxiliaryStateVarsHolder.empty()){
      for(p=this->auxiliaryStateVarsHolder.begin();p!=this->auxiliaryStateVarsHolder.end();++p){
	this->behaviourDataFile << ",\n";
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!this->externalStateVarsHolder.empty()){
      for(p =this->externalStateVarsHolder.begin();
	  p!=this->externalStateVarsHolder.end();++p){
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
    if(this->behaviourCharacteristic.useQt()){
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,use_qt>& src)\n";
    } else {
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,false>& src)\n";
    }
    this->behaviourDataFile << ": sig(src.sig),\n";
    this->behaviourDataFile << "eto(src.eto),\n";
    this->behaviourDataFile << "T(src.T)";
    SupportedTypes::TypeSize o;
    this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
							       this->coefsHolder,
							       "src.material_properties","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
								   this->stateVarsHolder,
								   "src.internal_variables","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
							       this->auxiliaryStateVarsHolder,
							       "src.internal_variables","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorI(this->behaviourDataFile,
							       this->externalStateVarsHolder,
							       "src.external_variables","","");
    this->behaviourDataFile << "\n";
    this->behaviourDataFile << "{\n";
    this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								this->coefsHolder,
								"src.material_properties","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								    this->stateVarsHolder,
								    "src.internal_variables","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								this->auxiliaryStateVarsHolder,
								"src.internal_variables","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorII(this->behaviourDataFile,
								this->externalStateVarsHolder,
								"src.external_variables","","");
    this->behaviourDataFile << "}\n\n";
    // Creating constructor for external interfaces
    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin(); i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeBehaviourDataConstructor(this->behaviourDataFile,
					       this->className,
					       this->coefsHolder,
					       this->stateVarsHolder,
					       this->auxiliaryStateVarsHolder,
					       this->externalStateVarsHolder,
					       this->behaviourCharacteristic);
    }
  } // end of MFrontBehaviourParserCommon::WriteBehaviourDataConstructors

  void MFrontBehaviourParserCommon::writeBehaviourDataAssignementOperator(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*\n";
    this->behaviourDataFile << "* \\brief Assignement operator\n";
    this->behaviourDataFile << "*/\n";
    this->behaviourDataFile << this->className << "BehaviourData&\n";
    this->behaviourDataFile << "operator=(const " 
			    << this->className << "BehaviourData& src){\n";
    this->behaviourDataFile << "this->sig = src.sig;\n";
    this->behaviourDataFile << "this->eto = src.eto;\n";
    if(!this->coefsHolder.empty()){
      for(p=this->coefsHolder.begin();p!=this->coefsHolder.end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!this->stateVarsHolder.empty()){
      for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!this->auxiliaryStateVarsHolder.empty()){
      for(p=this->auxiliaryStateVarsHolder.begin();p!=this->auxiliaryStateVarsHolder.end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";\n";  
      }
    }
    if(!this->externalStateVarsHolder.empty()){
      for(p =this->externalStateVarsHolder.begin();
	  p!=this->externalStateVarsHolder.end();++p){
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
    VarContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "void\n"
			    << "exportBehaviourData(const ";
    if(this->behaviourCharacteristic.useQt()){
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,use_qt>& res) const\n";
    } else {
      this->behaviourDataFile << "MechanicalBehaviourData<N,Type,false>& res) const\n";
    }
    this->behaviourDataFile << "{\n";
    this->behaviourDataFile << "using namespace tfel::math;\n";
    this->behaviourDataFile << "res.sig = this->sig;\n";
    this->behaviourDataFile << "res.eto = this->eto;\n";
    if(!this->coefsHolder.empty()){
      this->writeResultsArrayResize(this->behaviourDataFile,
				    "res.material_properties",
				    this->coefsHolder);
      this->exportResults(this->behaviourDataFile,
			  this->coefsHolder,
			  "res.material_properties",
			  this->behaviourCharacteristic.useQt());
    }
    if((!this->stateVarsHolder.empty())||
       (!this->auxiliaryStateVarsHolder.empty())){
      SupportedTypes::TypeSize s = this->getTotalSize(this->stateVarsHolder);
      s += this->getTotalSize(this->auxiliaryStateVarsHolder);
      this->writeResultsArrayResize(this->behaviourDataFile,
				    "res.internal_variables",s);
      SupportedTypes::TypeSize o;
      o = this->exportResults(this->behaviourDataFile,
			  this->stateVarsHolder,
			  "res.internal_variables",
			  this->behaviourCharacteristic.useQt());
      this->exportResults(this->behaviourDataFile,
			  this->auxiliaryStateVarsHolder,
			  "res.internal_variables",
			  this->behaviourCharacteristic.useQt(),o);
    }
    if(!this->externalStateVarsHolder.empty()){
      this->writeResultsArrayResize(this->behaviourDataFile,
				    "res.external_variables",
				    this->externalStateVarsHolder);
      this->exportResults(this->behaviourDataFile,
			  this->externalStateVarsHolder,
			  "res.external_variables",
			  this->behaviourCharacteristic.useQt());
    }
    this->behaviourDataFile << "} // end of exportBehaviourData\n";
    this->behaviourDataFile << endl;
    // Creating constructor for external interfaces
    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->exportMechanicalData(this->behaviourDataFile,
				      this->className,
				      this->coefsHolder,
				      this->stateVarsHolder,
				      this->auxiliaryStateVarsHolder,
				      this->externalStateVarsHolder,
				      this->behaviourCharacteristic);
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataPublicMembers(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    if(this->behaviourCharacteristic.requiresStiffnessTensor()){
      this->behaviourDataFile << "StiffnessTensor&\n";
      this->behaviourDataFile << "getStiffnessTensor(void)\n";
      this->behaviourDataFile << "{\nreturn this->D;\n}\n\n";
      this->behaviourDataFile << "const StiffnessTensor&\n";
      this->behaviourDataFile << "getStiffnessTensor(void) const\n";
      this->behaviourDataFile << "{\nreturn this->D;\n}\n\n";
    }
    if(this->behaviourCharacteristic.requiresThermalExpansionTensor()){
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
    
    if(this->behaviourCharacteristic.useQt()){
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
    
    if(this->behaviourCharacteristic.useQt()){
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
				     this->coefsHolder,
				     "","",this->fileName,
				     false,this->debugMode);

    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStateVars() {
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->stateVarsHolder,
				     "","",this->fileName,
				     false,this->debugMode);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->auxiliaryStateVarsHolder,
				     "","",this->fileName,
				     false,this->debugMode);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->externalStateVarsHolder,
				     "","",this->fileName,
				     false,this->debugMode);
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataOutputOperator(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    if(this->behaviourCharacteristic.useQt()){        
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
    if(this->behaviourCharacteristic.useQt()){        
      this->behaviourDataFile << "os << " << this->className 
			      << "BehaviourData<N,Type,use_qt>::getName() << endl;\n";
    } else {
      this->behaviourDataFile << "os << " << this->className 
			      << "BehaviourData<N,Type,false>::getName() << endl;\n";
    }
    this->behaviourDataFile << "os << \"sig : \" << b.sig << endl;\n";
    this->behaviourDataFile << "os << \"eto : \" << b.eto << endl;\n";
    this->behaviourDataFile << "os << \"T : \" << b.T << endl;\n";
    for(p=this->coefsHolder.begin();p!=this->coefsHolder.end();++p){
      this->behaviourDataFile <<  "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;\n";  
    }
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;\n";  
    }    
    for(p=this->auxiliaryStateVarsHolder.begin();
	p!=this->auxiliaryStateVarsHolder.end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;\n";  
    }    
    for(p=this->externalStateVarsHolder.begin();
	p!=this->externalStateVarsHolder.end();++p){
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
    this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
    this->behaviourFile << "class " << this->className << ";\n\n";

    if(this->behaviourCharacteristic.useQt()){
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
    this->behaviourFile << "* \\param H, modelling hypothesis." << endl;
    this->behaviourFile << "* \\param Type, numerical type." << endl;
    if(this->behaviourCharacteristic.useQt()){    
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

    if(this->behaviourCharacteristic.useQt()){        
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
    if(!this->stateVarsHolder.empty()){
      this->behaviourFile << "{\n";
      for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
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
    this->behaviourFile << "bool\n";
    this->behaviourFile << "integrate(const bool computeTangentOperator_){\n";
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourIntegrator",
		      this->behaviourFile,this->materialLaws);		      
    if(!this->integrator.empty()){
      this->behaviourFile << this->integrator;
      this->behaviourFile << "\n";
    }
    this->behaviourFile << "this->updateStateVars();\n";
    this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    for(p  = this->boundsDescriptions.begin();
	p != this->boundsDescriptions.end();++p){
      if(p->varCategory==BoundsDescription::StateVar){
	p->writeBoundsChecks(this->behaviourFile);
      }
    }
    this->behaviourFile << "return true;\n";
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
    this->behaviourFile << "}; // end of setOutOfBoundsPolicy\n\n";
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
    for(b  = this->boundsDescriptions.begin();
	b != this->boundsDescriptions.end();++b){
      b->writeBoundsChecks(this->behaviourFile);
    }      
    this->behaviourFile << "}; // end of checkBounds\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourCheckBounds(void)

  void MFrontBehaviourParserCommon::writeBehaviourConstructors(void)
  {    
    using namespace std;
    ostringstream initStateVarsIncrements;
    ostringstream initComputedVars;
    this->writeStateVariableIncrementsInitializers(initStateVarsIncrements,
						   this->stateVarsHolder,
						   this->useStateVarTimeDerivative);
    if(!this->computedVars.empty()){
      ComputedVarContainer::const_iterator p2;
      for(p2=this->computedVars.begin();p2!=this->computedVars.end();++p2){
	initComputedVars << ",\n";
	initComputedVars << p2->name << "(" << p2->description << ")" ;
      }
    }   
    this->writeBehaviourConstructors(initStateVarsIncrements.str(),
				     initComputedVars.str());
  }

  void MFrontBehaviourParserCommon::writeBehaviourConstructors(const std::string& initStateVarsIncrements,
							       const std::string& initComputedVars,
							       const std::string& predictor)
  {    
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    VarContainer::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Constructor\n";
    this->behaviourFile << "*/\n";
    if(this->behaviourCharacteristic.useQt()){        
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
    this->behaviourFile << initComputedVars;
    this->behaviourFile << "\n{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
		      this->behaviourFile,this->materialLaws);
    this->writeBehaviourParameterInitialisation();
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
    if(this->behaviourCharacteristic.useQt()){        
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
    this->behaviourFile << initComputedVars;
    this->behaviourFile << "\n{\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
		      this->behaviourFile,this->materialLaws);		      
    this->writeBehaviourParameterInitialisation();
    if(!this->initLocalVars.empty()){
      this->behaviourFile << this->initLocalVars;
    } 
    if(!predictor.empty()){
      this->behaviourFile << predictor;
    }
    this->writeBehaviourParserSpecificConstructorPart();
    this->behaviourFile << "}\n\n";
    // Creating constructor for external interfaces
    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeBehaviourConstructor(this->behaviourFile,
					   this->className,
					   this->coefsHolder,
					   this->stateVarsHolder,
					   this->auxiliaryStateVarsHolder,
					   this->externalStateVarsHolder,
					   this->behaviourCharacteristic,
					   initStateVarsIncrements,
					   initComputedVars);
      this->behaviourFile << "\n{\n";
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
			this->behaviourFile,this->materialLaws);		      
      this->writeBehaviourParameterInitialisation();
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

  void MFrontBehaviourParserCommon::writeBehaviourParameterInitialisation(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    if(!this->parametersHolder.empty()){
      for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
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
    this->behaviourFile << "}; // end of getModellingHypothesis\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourGetModellingHypothesis();

  void MFrontBehaviourParserCommon::writeBehaviourLocalVars(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->writeVariablesDeclarations(this->behaviourFile,
				     this->localVarsHolder,
				     "","",this->fileName,
				     false,this->debugMode);
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourParameters(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
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

  void MFrontBehaviourParserCommon::writeBehaviourComputedVars(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    ComputedVarContainer::const_iterator p;
    for(p=this->computedVars.begin();p!=this->computedVars.end();++p){
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
				     this->stateVarsHolder,
				     "d","",this->fileName,
				     this->useStateVarTimeDerivative,
				     this->debugMode);
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourOutputOperator(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    if(this->behaviourCharacteristic.useQt()){        
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
    if(this->behaviourCharacteristic.useQt()){        
      this->behaviourFile << "os << " << this->className 
			  << "<hypothesis,Type,use_qt>::getName() << endl;\n";
    } else {
      this->behaviourFile << "os << " << this->className 
			  << "<hypothesis,Type,false>::getName() << endl;\n";
    }
    this->behaviourFile << "os << \"sig : \" << b.sig << endl;\n";
    this->behaviourFile << "os << \"eto : \" << b.eto << endl;\n";
    this->behaviourFile << "os << \"deto : \" << b.deto << endl;\n";
    this->behaviourFile << "os << \"dt : \" << b.dt << endl;\n";
    this->behaviourFile << "os << \"T : \" << b.T << endl;\n";
    this->behaviourFile << "os << \"dT : \" << b.dT << endl;\n";
    for(p=this->coefsHolder.begin();p!=this->coefsHolder.end();++p){
      this->behaviourFile <<  "os << \"" << p->name << " : \" << b." << p->name <<  " << endl;\n";  
    }
    for(p=this->stateVarsHolder.begin();
	p!=this->stateVarsHolder.end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << endl;\n";  
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name <<  " << endl;\n";  
    }    
    for(p=this->auxiliaryStateVarsHolder.begin();
	p!=this->auxiliaryStateVarsHolder.end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << endl;\n";  
    }
    for(p=this->externalStateVarsHolder.begin();
	p!=this->externalStateVarsHolder.end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name << " << endl;\n";
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name << " << endl;\n";  
    }
    for(p=this->localVarsHolder.begin();p!=this->localVarsHolder.end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name;
      if((p+1)!=this->localVarsHolder.end()){
	this->behaviourFile <<  " << endl;\n";  
      } else {
	this->behaviourFile <<  ";\n";  
      }
    }
    for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name;
      if((p+1)!=this->parametersHolder.end()){
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
    this->checkBehaviourFile();
    this->behaviourFile << "void updateExternalStateVariables(void){\n";
    this->behaviourFile << "this->eto += this->deto;\n";
    this->behaviourFile << "this->T   += this->dT;\n";
    for(p=this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
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
    this->behaviourFile << "\ntypedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourFile << "static const unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;\n\n";
    this->behaviourFile << "public :\n\n";
    if(this->behaviourCharacteristic.useQt()){        
      this->behaviourFile << "typedef " << this->className 
			  << "BehaviourData<N,Type,use_qt> BehaviourData;\n";
      this->behaviourFile << "typedef " << this->className 
			  << "IntegrationData<N,Type,use_qt> IntegrationData;\n";
    } else {
      this->behaviourFile << "typedef " << this->className 
			  << "BehaviourData<N,Type,false> BehaviourData;\n";
      this->behaviourFile << "typedef " << this->className 
			  << "IntegrationData<N,Type,false> IntegrationData;\n\n";
    }
    this->behaviourFile << "private :\n\n";
  } // end of MFrontBehaviourParserCommon::writeBehaviourStandardTFELTypedefs

  void MFrontBehaviourParserCommon::writeBehaviourTraits(void)
  {
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    SupportedTypes::TypeSize coefSize;
    SupportedTypes::TypeSize stateVarsSize;
    SupportedTypes::TypeSize externalStateVarsSize;
    for(p=this->coefsHolder.begin();p!=this->coefsHolder.end();++p){
      coefSize+=this->getTypeSize(p->type,p->arraySize);
    }
    for(p=this->stateVarsHolder.begin();p!=this->stateVarsHolder.end();++p){
      stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    for(p=this->auxiliaryStateVarsHolder.begin();
	p!=this->auxiliaryStateVarsHolder.end();++p){
      stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    for(p  = this->externalStateVarsHolder.begin();
	p != this->externalStateVarsHolder.end();++p){
      externalStateVarsSize+=this->getTypeSize(p->type,p->arraySize);
    }
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* Partial specialisation for ";
    this->behaviourFile << this->className << ".\n";
    this->behaviourFile << "*/\n";

    if(this->behaviourCharacteristic.useQt()){
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>\n";
      this->behaviourFile << "class MechanicalBehaviourTraits<";
      this->behaviourFile << this->className << "<hypothesis,Type,use_qt> >\n";
    } else {
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>\n";
      this->behaviourFile << "class MechanicalBehaviourTraits<";
      this->behaviourFile << this->className << "<hypothesis,Type,false> >\n";
    }
    this->behaviourFile << "{\n";
    this->behaviourFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->behaviourFile << "static const unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;\n";
    this->behaviourFile << "public:\n";
    this->behaviourFile << "static const bool is_defined = true;\n";
    if(this->behaviourCharacteristic.useQt()){
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
    if(!this->parametersHolder.empty()){
      VarContainer::const_iterator p;
      bool rp = false;
      bool ip = false;
      bool up = false;
      this->checkBehaviourFile();
      this->behaviourFile << "struct " << this->className << "ParametersInitializer\n"
			  << "{\n"
			  << "static " << this->className << "ParametersInitializer&\n"
			  << "get();\n\n";
      for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
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
    this->writeBehaviourIntegrator();
    this->writeBehaviourComputeTangentOperator();
    this->writeBehaviourGetTangentOperator();
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

  void MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator(void)
  {
    this->behaviourFile << "bool computeConsistantTangentOperator(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "string msg(\"" << this->className<< "::writeBehaviourComputeTangentOperator : \");\n";
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
    this->behaviourFile << "};\n\n";
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
    this->integrationDataFile << "#include\"TFEL/Material/MechanicalIntegrationData.hxx\"\n\n";
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataDefaultMembers(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "protected: \n\n";
    this->integrationDataFile << "/*!\n";
    this->integrationDataFile << " * \\brief strain increment\n";
    this->integrationDataFile << " */\n";
    this->integrationDataFile << "StrainStensor deto;\n";
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
    this->integrationDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n";
    this->integrationDataFile << "static const unsigned short StensorSize = ";
    this->integrationDataFile << "StensorDimeToSize::value;\n";
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
    this->integrationDataFile << ": deto(src.deto),\n";
    this->integrationDataFile << "dt(src.dt),\n";
    this->integrationDataFile << "dT(src.dT)";
    if(!this->externalStateVarsHolder.empty()){
      for(p =this->externalStateVarsHolder.begin();
	  p!=this->externalStateVarsHolder.end();++p){
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
    if(this->behaviourCharacteristic.useQt()){
      this->integrationDataFile << "MechanicalIntegrationData<N,Type,use_qt>& src)\n";
    } else {
      this->integrationDataFile << "MechanicalIntegrationData<N,Type,false>& src)\n";
    }
    this->integrationDataFile << ": deto(src.deto),\n";
    this->integrationDataFile << "dt(src.dt),\n";
    this->integrationDataFile << "dT(src.dT)";
    if(!this->externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(this->integrationDataFile,
								 this->externalStateVarsHolder,
								 "src.dexternal_variables","d","");
    }
    this->integrationDataFile << "\n{\n";
    this->writeVariableInitializersInBehaviourDataConstructorII(this->integrationDataFile,
								this->externalStateVarsHolder,
								"src.dexternal_variables","d","");
    this->integrationDataFile << "}\n\n";
    // Creating constructor for external interfaces
    StringContainer::const_iterator i;
    for(i  = this->interfaces.begin(); i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeIntegrationDataConstructor(this->integrationDataFile,
						 this->className,
						 this->coefsHolder,
						 this->stateVarsHolder,
						 this->auxiliaryStateVarsHolder,
						 this->externalStateVarsHolder,
						 this->behaviourCharacteristic);
    }
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataScaleOperators(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
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
    this->integrationDataFile << "this->deto *= s;\n";
    this->integrationDataFile << "this->dT   *= s;\n";
    for(p=this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
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

    if(this->behaviourCharacteristic.useQt()){
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

    if(this->behaviourCharacteristic.useQt()){
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
    
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataOutputOperator(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    VarContainer::const_iterator p;
    if(this->behaviourCharacteristic.useQt()){        
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
    if(this->behaviourCharacteristic.useQt()){        
      this->integrationDataFile << "os << " << this->className 
				<< "IntegrationData<N,Type,use_qt>::getName() << endl;\n";
    } else {
      this->integrationDataFile << "os << " << this->className 
				<< "IntegrationData<N,Type,false>::getName() << endl;\n";
    }
    this->integrationDataFile << "os << \"deto : \" << b.deto << endl;\n";
    this->integrationDataFile << "os << \"dt : \" << b.dt << endl;\n";
    this->integrationDataFile << "os << \"dT : \" << b.dT << endl;\n";
    for(p=this->externalStateVarsHolder.begin();p!=this->externalStateVarsHolder.end();++p){
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
    VarContainer::const_iterator p;
    this->checkIntegrationDataFile();
    for(p =this->externalStateVarsHolder.begin();
	p!=this->externalStateVarsHolder.end();++p){
      if((!this->debugMode)&&(p->lineNumber!=0u)){
	this->integrationDataFile << "#line " << p->lineNumber << " \"" 
				  << this->fileName << "\"\n";
      }
      this->integrationDataFile << p->type << " d" << p->name << ";\n";  
    }
    this->integrationDataFile << endl;
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
    if(!this->parametersHolder.empty()){
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
    m.push_back("tfel::material::ModellingHypothesis::AXISYMETRICALGENERALISEDPLANESTRAIN");
    m.push_back("tfel::material::ModellingHypothesis::AXISYMETRICAL");
    m.push_back("tfel::material::ModellingHypothesis::PLANESTRESS");
    m.push_back("tfel::material::ModellingHypothesis::PLANESTRAIN");
    m.push_back("tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN");
    m.push_back("tfel::material::ModellingHypothesis::TRIDIMENSIONAL");
    for(p=this->staticVars.begin();p!=this->staticVars.end();++p){
      for(pm=m.begin();pm!=m.end();++pm){
	if(this->behaviourCharacteristic.useQt()){
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
	if(this->behaviourCharacteristic.useQt()){
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
	if(this->behaviourCharacteristic.useQt()){
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
    if(!this->parametersHolder.empty()){
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
      for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
	if(p->type=="real"){
	  rp = true;
	  p2 = this->parametersDefaultValues.find(p->name);
	  if(p2==this->parametersDefaultValues.end()){
	    string msg("MFrontBehaviourParserCommon::writeSrcFileParametersInitializer : ");
	    msg += "no default value for parameter '"+p->name+"'";
	    throw(runtime_error(msg));
	  }
	  this->srcFile << "this->" << p->name << " = " << p2->second << ";\n"; 
	} else if(p->type=="int"){
	  ip = true;
	  p3 = this->iParametersDefaultValues.find(p->name);
	  if(p3==this->iParametersDefaultValues.end()){
	    string msg("MFrontBehaviourParserCommon::writeSrcFileParametersInitializer : ");
	    msg += "no default value for parameter '"+p->name+"'";
	    throw(runtime_error(msg));
	  }
	  this->srcFile << "this->" << p->name << " = " << p3->second << ";\n"; 
	} else if(p->type=="ushort"){
	  up = true;
	  p4 = this->uParametersDefaultValues.find(p->name);
	  if(p4==this->uParametersDefaultValues.end()){
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
	for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
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
	for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
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
	for(p=this->parametersHolder.begin();p!=this->parametersHolder.end();++p){
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
    this->writeNamespaceBegin(this->srcFile);
    this->writeSrcFileParametersInitializer();
    this->writeSrcFileUserDefinedCode();
    this->writeSrcFileStaticVars();
    this->writeNamespaceEnd(this->srcFile);
  } // end of MFrontBehaviourParserCommon::writeSrcFile(void)

  void
  MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles(void)
  {
    using namespace std;
    vector<UnaryLoadingDefinition>::const_iterator p;
    vector<UnaryLoadingDefinition>::size_type nbr = 0u;
    vector<long double>::const_iterator p2;
    VarContainer::const_iterator p3;
    map<string,long double>::const_iterator p4;
    string test;
    ostringstream testFileName;
    ofstream testFile;
    string numType;
    unsigned short i;
    unsigned short stensorSize;
    unsigned short coefNbr;
    unsigned short statevNbr;
    unsigned short extStatNbr;
    unsigned short pos;
    
    for(p=unaryLoadingDefinitions.begin();
	p!=unaryLoadingDefinitions.end();++p,++nbr){
      testFileName.clear();
      testFileName << this->className;
      testFileName << "-";
      test = p->getTestType();
      test = test.substr(1,test.size()-2);
      testFileName << test;
      testFileName << "-";
      testFileName << p->getDimension();
      testFileName << "D-test";
      testFileName << nbr;
      testFileName << ".cxx";
      numType = p->getNumericType();
      testFile.open(("src/"+testFileName.str()).c_str());
      if(!testFile){
	string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
	msg+="cannot open file : ";
	msg+=testFileName.str();
	throw(runtime_error(msg));
      }
      testFile << "/*!" << endl;
      testFile << "* \\file  " << testFileName.str() << endl;
      testFile << "* \\brief this file implements unary loading test\n"
	       << "*        for the " << this->className << " Behaviour." << endl;
      testFile << "*        File generated by ";
      testFile << MFrontHeader::getVersionName() << " ";
      testFile << "version " << MFrontHeader::getVersionNumber();
      testFile << endl;
      if(!this->authorName.empty()){
	testFile << "* \\author " << this->authorName << endl;
      }
      if(!this->date.empty()){
	testFile << "* \\date   " << this->date       << endl;
      }
      testFile << "* Test type : " << p->getTestType()  << "\n";
      testFile << "* Dimension : " << p->getDimension() << "\n";
      if(p->getUseQt()){
	testFile << "* UseQt : true\n";
      } else {
	testFile << "* UseQt : false\n";
      }
      testFile << " */" << endl;
      testFile << endl;
      testFile << "#include<utility>\n\n";
      testFile << "#include\"TFEL/Config/TFELConfig.hxx\"\n";
      testFile << "#include\"TFEL/Config/TFELTypes.hxx\"\n";
      testFile << "#include\"TFEL/Material/MechanicalBehaviourData.hxx\"\n";
      testFile << "#include\"TFEL/Material/";
      testFile << this->behaviourFileName << "\"\n\n";
      testFile << "int\nmain(void)\n{\n\n";
      testFile << "using namespace std;\n";
      testFile << "using namespace tfel::material;\n\n";
      if(p->getUseQt()){
	testFile << "typedef tfel::config::Types<"
		 << p->getDimension() << "," << numType << ",true> Types;\n";
      } else {
	testFile << "typedef tfel::config::Types<" 
		 << p->getDimension() << "," << numType << ",false> Types;\n";
      }
      testFile << "typedef Types::real            real;\n";
      testFile << "typedef Types::time            time;\n";
      testFile << "typedef Types::stress          stress;\n";
      testFile << "typedef Types::strain          strain;\n";
      testFile << "typedef Types::temperature     temperature;\n";
      testFile << "typedef Types::StressStensor   StressStensor;\n";
      testFile << "typedef Types::StrainStensor   StrainStensor;\n";
      testFile << "typedef Types::StiffnessTensor StiffnessTensor;\n";
      if(p->getDimension()==1){
	stensorSize = 3u;
      } else if (p->getDimension()==2){
      	stensorSize = 4u;
      } else {
	stensorSize = 6u;
      }
      // writing times
      testFile << "// time values\n";
      const vector<long double>& times = p->getTimes();
      testFile << "const " << numType << " times [] = {";
      for(i=1u,p2=times.begin();p2!=times.end();++i){
	if(numType=="float"){
	  if(abs(*p2)>numeric_limits<float>::max()){
	    string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
	    msg += "cannot convert time to float (out of range)";
	    throw(runtime_error(msg));
	  }
	  testFile << static_cast<float>(*p2);
	} else if (numType=="double"){
	  if(abs(*p2)>numeric_limits<double>::max()){
	    string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
	    msg += "cannot convert time to double (out of range)";
	    throw(runtime_error(msg));
	  }
	  testFile << static_cast<double>(*p2);
	} else {
	  testFile << *p2;
	}
	if(++p2!=times.end()){
	  testFile << ",";
	  if(i%5==0){
	    testFile << "\n";
	  }
	}
      }
      testFile << "};\n";
      // we now treat coefficients values
      SupportedTypes::TypeSize s = this->getTotalSize(this->coefsHolder);
      coefNbr = static_cast<unsigned short>(s.getStensorSize()*stensorSize+s.getScalarSize());      
      testFile << "// coefficients values\n";
      testFile << "vector<real> coefficients("<< coefNbr << "u);\n";
      const map<string,long double>& coefficients = p->getCoefficientsValues();
      pos = 0u;
      for(p3 =this->coefsHolder.begin();
	  p3!=this->coefsHolder.end();++p3){
	testFile << "// treating coefficient " << p3->name << endl;
	if(this->getTypeFlag(p3->type)==SupportedTypes::Stensor){
	  for(i=0;i!=stensorSize;++i,++pos){
	    if((p4=coefficients.find(p3->name+"("+toString(i)+")"))==coefficients.end()){
	      string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
	      msg += "no value defined for "+toString(i);
	      msg += " component of the coefficient " + p3->name;
	      throw(runtime_error(msg));
	    }
	    testFile << "coefficients[" << pos << "] = ";
	    if(numType=="float"){
	      if(abs(p4->second)>numeric_limits<float>::max()){
		string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		msg += "cannot convert coefficient " + p4->first + " value to float";
		msg += "(out of range)";
		throw(runtime_error(msg));
	      }
	      testFile << "real(" << static_cast<float>(p4->second) << ");\n";
	    } else if(numType=="double"){
	      if(abs(p4->second)>numeric_limits<double>::max()){
		string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		msg += "cannot convert coefficient " + p4->first + " value to double";
		msg += "(out of range)";
		throw(runtime_error(msg));
	      }
	      testFile << "real(" << static_cast<double>(p4->second) << ");\n";
	    } else {
	      testFile << "real(" << p4->second << ");\n";
	    }	
	  }
	} else {
	  // scalar coefficient
	  if((p4=coefficients.find(p3->name))==coefficients.end()){
	    string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
	    msg += "no value defined for coefficient " + p3->name;
	    throw(runtime_error(msg));
	  }
	  if(numType=="float"){
	    if(abs(p4->second)>numeric_limits<float>::max()){
	      string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
	      msg += "cannot convert coefficient " + p4->first + " value to float";
	      msg += "(out of range)";
	      throw(runtime_error(msg));
	    }
	    testFile << "coefficients[" << pos << "] = "
		     << "real(" << static_cast<float>(p4->second) << ");\n";
	  } else if(numType=="double"){
	    if(abs(p4->second)>numeric_limits<double>::max()){
	      string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
	      msg += "cannot convert coefficient " + p4->first + " value to double";
	      msg += "(out of range)";
	      throw(runtime_error(msg));
	    }
	    testFile << "coefficients[" << pos << "] = "
		     << "real(" << static_cast<double>(p4->second) << ");\n";
	  } else {
	    testFile << "coefficients[" << pos << "] = "
		     << "real(" << p4->second << ");\n";
	  }
	}
      }
      // we now treat initial state variables values
      const map<string,long double>& stateVar = p->getInternalStateVariablesValues();
      s  = this->getTotalSize(this->stateVarsHolder);
      s += this->getTotalSize(this->auxiliaryStateVarsHolder);
      statevNbr = static_cast<unsigned short>(s.getStensorSize()*stensorSize+s.getScalarSize());
      testFile << "vector<real> internalStateVariables(" << statevNbr << "u);\n";
      pos = 0u;
      for(p3 =this->stateVarsHolder.begin();
	  p3!=this->stateVarsHolder.end();++p3){
	testFile << "// treating internal variable " << p3->name << endl;
	if(this->getTypeFlag(p3->type)==SupportedTypes::Stensor){
	  for(i=0;i!=stensorSize;++i,++pos){
	    if((p4=stateVar.find(p3->name+"("+toString(i)+")"))==stateVar.end()){
	      testFile << "internalStateVariables[" << pos << "] = 0;\n";
	    } else {
	      testFile << "internalStateVariables[" << pos << "] = real(";
	      if(numType=="float"){
		if(abs(p4->second)>numeric_limits<float>::max()){
		  string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		  msg += "cannot convert coefficient " + p4->first + " value to float";
		  msg += "(out of range)";
		  throw(runtime_error(msg));
		}
		testFile << static_cast<float>(p4->second) << ");\n";
	      } else if(numType=="double"){
		if(abs(p4->second)>numeric_limits<double>::max()){
		  string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		  msg += "cannot convert coefficient " + p4->first + " value to double";
		  msg += "(out of range)";
		  throw(runtime_error(msg));
		}
		testFile << static_cast<double>(p4->second) << ");\n";
	      } else {
		testFile << p4->second << ");\n";
	      }
	    }
	  }
	} else {
	  if((p4=stateVar.find(p3->name+"("+toString(i)+")"))==stateVar.end()){
	    testFile << "internalStateVariables[" << pos << "] = 0;\n";
	  } else {
	    testFile << "internalStateVariables[" << pos << "] = real(";
	    if(numType=="float"){
	      if(abs(p4->second)>numeric_limits<float>::max()){
		string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		msg += "cannot convert coefficient " + p4->first + " value to float";
		msg += "(out of range)";
		throw(runtime_error(msg));
	      }
	      testFile << static_cast<float>(p4->second) << ");\n";
	    } else if(numType=="double"){
	      if(abs(p4->second)>numeric_limits<double>::max()){
		string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		msg += "cannot convert coefficient " + p4->first + " value to double";
		msg += "(out of range)";
		throw(runtime_error(msg));
	      }
	      testFile << static_cast<double>(p4->second) << ");\n";
	    } else {
	      testFile << p4->second << ");\n";
	    }
	  }
	  ++pos;
	}
      }
      for(p3 =this->auxiliaryStateVarsHolder.begin();
	  p3!=this->auxiliaryStateVarsHolder.end();++p3){
	testFile << "// treating internal variable " << p3->name << endl;
	if(this->getTypeFlag(p3->type)==SupportedTypes::Stensor){
	  for(i=0;i!=stensorSize;++i,++pos){
	    if((p4=stateVar.find(p3->name+"("+toString(i)+")"))==stateVar.end()){
	      testFile << "internalStateVariables[" << pos << "] = 0;\n";
	    } else {
	      testFile << "internalStateVariables[" << pos << "] = real(";
	      if(numType=="float"){
		if(abs(p4->second)>numeric_limits<float>::max()){
		  string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		  msg += "cannot convert coefficient " + p4->first + " value to float";
		  msg += "(out of range)";
		  throw(runtime_error(msg));
		}
		testFile << static_cast<float>(p4->second) << ");\n";
	      } else if(numType=="double"){
		if(abs(p4->second)>numeric_limits<double>::max()){
		  string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		  msg += "cannot convert coefficient " + p4->first + " value to double";
		  msg += "(out of range)";
		  throw(runtime_error(msg));
		}
		testFile << static_cast<double>(p4->second) << ");\n";
	      } else {
		testFile << p4->second << ");\n";
	      }
	    }
	  }
	} else {
	  if((p4=stateVar.find(p3->name+"("+toString(i)+")"))==stateVar.end()){
	    testFile << "internalStateVariables[" << pos << "] = 0;\n";
	  } else {
	    testFile << "internalStateVariables[" << pos << "] = real(";
	    if(numType=="float"){
	      if(abs(p4->second)>numeric_limits<float>::max()){
		string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		msg += "cannot convert coefficient " + p4->first + " value to float";
		msg += "(out of range)";
		throw(runtime_error(msg));
	      }
	      testFile << static_cast<float>(p4->second) << ");\n";
	    } else if(numType=="double"){
	      if(abs(p4->second)>numeric_limits<double>::max()){
		string msg("MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles : ");
		msg += "cannot convert coefficient " + p4->first + " value to double";
		msg += "(out of range)";
		throw(runtime_error(msg));
	      }
	      testFile << static_cast<double>(p4->second) << ");\n";
	    } else {
	      testFile << p4->second << ");\n";
	    }
	  }
	  ++pos;
	}
      }
      s = this->getTotalSize(this->externalStateVarsHolder);
      extStatNbr = static_cast<unsigned short>(s.getStensorSize()*stensorSize+s.getScalarSize());
      if(p->getUseQt()){
	testFile << "MechanicalBehaviourData<"
		 << p->getDimension() << "," << numType << ",true> ";
      } else {
	testFile << "MechanicalBehaviourData<" 
		 << p->getDimension() << "," << numType << ",false> ";
      }
      testFile << "behaviourData(StressStensor(),\n";
      testFile << "StrainStensor(),\n";
      testFile << "StiffnessTensor(),\n";
      testFile << "coefficients,\n";
      testFile << "internalStateVariables,\n";
      testFile << "vector<real>(" << extStatNbr << "u),\n";
      testFile << "temperature());\n";
      testFile << "} // end of main\n";
      testFile.close();
    }

  } // end of namespace MFrontBehaviourParserCommon::writeUnaryLoadingTestFiles

  bool
  MFrontBehaviourParserCommon::isInternalStateVariable(const std::string& name)
  {
    VarContainer::const_iterator ps;
    for(ps=this->stateVarsHolder.begin();
	(ps!=this->stateVarsHolder.end())&&(ps->name!=name);++ps)
      {}
    return ps!=this->stateVarsHolder.end();
  } // end of MFrontBehaviourParserCommon::isInternalStateVariable

  bool
  MFrontBehaviourParserCommon::isInternalStateVariableIncrement(const std::string& name)
  {
    VarContainer::const_iterator ps;
    for(ps=this->stateVarsHolder.begin();
	(ps!=this->stateVarsHolder.end())&&("d"+ps->name!=name);++ps)
      {}
    return ps!=this->stateVarsHolder.end();
  } // end of MFrontBehaviourParserCommon::isInternalStateVariable

  bool
  MFrontBehaviourParserCommon::isExternalStateVariable(const std::string& name)
  {
    VarContainer::const_iterator ps;
    for(ps=this->externalStateVarsHolder.begin();
	(ps!=this->externalStateVarsHolder.end())&&(ps->name!=name);++ps)
      {}
    return ps!=this->externalStateVarsHolder.end();
  } // end of MFrontBehaviourParserCommon::isExternalStateVariable

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getGlobalIncludes(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    typedef map<string,vector<string> > Map;
    Map incs;
    StringContainer::const_iterator i;
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
    StringContainer::const_iterator i;
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
    StringContainer::const_iterator i;
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
    StringContainer::const_iterator i;
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
    StringContainer::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    vector<string>::const_iterator p3;
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

  void
  MFrontBehaviourParserCommon::treatLocalVar(void)
  {
    this->readVarList(this->localVarsHolder,true,false);
  } // end of MFrontBehaviourParserCommon::treatLocalVar

  void
  MFrontBehaviourParserCommon::treatParameter(void)
  {
    this->readVarList(this->parametersHolder,"real",false,false);
  } // end of MFrontBehaviourParserCommon::treatParameter

  void
  MFrontBehaviourParserCommon::treatInitLocalVars(void)
  {
    this->initLocalVars += this->readNextBlock(&ParserBase::variableModifier3,
					       true);
    this->initLocalVars += "\n";
  } // end of MFrontBehaviourParserCommon::treatInitLocalVars

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontBehaviourParserCommon::getSpecificTargets(void)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontBehaviourParserCommon::getSpecificTargets(void)

} // end of namespace mfront
