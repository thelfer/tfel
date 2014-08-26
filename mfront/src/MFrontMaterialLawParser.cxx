/*!
 * \file   MFrontMaterialLawParser.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<string>
#include<stdexcept>
#include<sstream>
#include<cctype>
#include<algorithm>
#include<iterator>
#include<cstdlib>

#include"TFEL/Utilities/Token.hxx"
#include"TFEL/System/System.hxx"
#include"TFEL/Glossary/Glossary.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontMaterialLawParser.hxx"
#include"MFront/MFrontLawInterfaceFactory.hxx"

namespace mfront{

  MFrontMaterialLawParser::MFrontMaterialLawParser(void)
    : MFrontVirtualParser(),
      useTemplate(false)
  {
    // Call Back
    this->registerNewCallBack("@Link",
			      &MFrontMaterialLawParser::treatLink);
    this->registerNewCallBack("@MaterialLaw",
			      &MFrontMaterialLawParser::treatMaterialLaw);
    this->registerNewCallBack("@MFront",
			      &MFrontMaterialLawParser::treatMFront);
    this->registerNewCallBack("@Material",&MFrontMaterialLawParser::treatMaterial);
    this->registerNewCallBack("@Library",&MFrontMaterialLawParser::treatLibrary);
    this->registerNewCallBack("@Parser",&MFrontMaterialLawParser::treatParser);
    this->registerNewCallBack("@Law",&MFrontMaterialLawParser::treatLaw);
    this->registerNewCallBack("@Author",&MFrontMaterialLawParser::treatAuthor);
    this->registerNewCallBack("@Date",&MFrontMaterialLawParser::treatDate);
    this->registerNewCallBack("@Includes",&MFrontMaterialLawParser::treatIncludes);
    this->registerNewCallBack("@StaticVar",&MFrontMaterialLawParser::treatStaticVar);
    this->registerNewCallBack("@StaticVariable",&MFrontMaterialLawParser::treatStaticVar);
    this->registerNewCallBack("@Parameter",&MFrontMaterialLawParser::treatParameter);
    this->registerNewCallBack("@Constant",&MFrontMaterialLawParser::treatConstant);
    this->registerNewCallBack("@Description",&MFrontMaterialLawParser::treatDescription);
    this->registerNewCallBack("@Input",&MFrontMaterialLawParser::treatInput);
    this->registerNewCallBack("@Output",&MFrontMaterialLawParser::treatOutput);
    this->registerNewCallBack("@Namespace",&MFrontMaterialLawParser::treatNamespace);
    this->registerNewCallBack("@Function",&MFrontMaterialLawParser::treatFunction);
    this->registerNewCallBack("@Import",&MFrontMaterialLawParser::treatImport);
    this->registerNewCallBack("@Interface",
			      &MFrontMaterialLawParser::treatInterface);
    this->registerNewCallBack("@Bounds",
			      &MFrontMaterialLawParser::treatBounds);
    this->registerNewCallBack("@PhysicalBounds",
			      &MFrontMaterialLawParser::treatPhysicalBounds);
    this->registerNewCallBack("@UseTemplate",
			      &MFrontMaterialLawParser::treatUseTemplate);
    this->reserveName("params",false);
  } // end of MFrontMaterialLawParser::MFrontMaterialLawParser()

  void
  MFrontMaterialLawParser::registerNewCallBack(const std::string& keyword,
					       const MemberFuncPtr ptr)
  {
    using namespace std;
    this->callBacks.insert(make_pair(keyword,ptr));
    this->registredKeyWords.insert(keyword);
  } // end of MFrontMaterialLawParser::registerNewCall

  void
  MFrontMaterialLawParser::getKeywordsList(std::vector<std::string>& k) const
  {
    CallBackContainer::const_iterator p;
    for(p=this->callBacks.begin();p!=this->callBacks.end();++p){
      k.push_back(p->first);
    }
  }

  std::string
  MFrontMaterialLawParser::getClassName(void) const
  {
    return this->className;
  } // end of MFrontMaterialLawParser::getClassName

  void
  MFrontMaterialLawParser::addStaticVariableDescription(const StaticVariableDescription& v)
  {
    this->staticVars.push_back(v);
  } // end of MFrontMaterialLawParser::addStaticVariableDescription

  std::string
  MFrontMaterialLawParser::getDescription()
  {
    using namespace std;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    const std::vector<std::string>& ai = mlif.getRegistredInterfaces();
    vector<string>::const_iterator p2  = ai.begin();
    vector<string>::const_iterator p2e = ai.end();
    string msg ("this parser is used to define material properties. ");
    msg += "Available interfaces are ";
    while(p2!=p2e){
      msg += "'"+*p2+"'";
      if(++p2!=p2e){
	msg+=",";
      } else {
	msg+=" and ";
      }
    }
    return msg;
  } // end of MFrontMaterialLawParser::getDescription

  std::string
  MFrontMaterialLawParser::getName(void)
  {
    return "MaterialLaw";
  } // end of MFrontMaterialLawParser::getName(void)

  void
  MFrontMaterialLawParser::treatMaterial(void)
  {
    using namespace std;
    if(!this->material.empty()){
      string msg("MFrontMaterialLawParser::treatMaterial : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->material = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(this->material,true)){
      string msg("MFrontMaterialLawParser::treatMaterial : ");
      msg += "invalid material name '"+this->material+"'";
      throw(runtime_error(msg));
    }
  } // end of MFrontMaterialLawParser::treatMaterial

  void
  MFrontMaterialLawParser::treatLibrary(void)
  {
    using namespace std;
    if(!this->material.empty()){
      string msg("MFrontMaterialLawParser::treatLibrary : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->library = this->readOnlyOneToken();
  } // end of MFrontLibraryLawParser::treatLibrary

  void
  MFrontMaterialLawParser::treatUseTemplate(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontMaterialLawParser::treatUseTemplate",";");
    this->useTemplate = true;
  } // end of MFrontMaterialLawParser::treatUseTemplate

  void
  MFrontMaterialLawParser::treatNamespace(void)
  {
    using namespace std;
    string name;
    bool bend;
    if(!this->namespaces.empty()){
      --(this->current);
      this->throwRuntimeError("MFrontMaterialLawParser::treatNamespace",
			      "Namespace already specified.");
    }
    bend = false;
    while(!bend){
      this->checkNotEndOfFile("MFrontMaterialLawParser::treatNamespace");
      name = this->current->value;
      if(!isValidIdentifier(name)){
	this->throwRuntimeError("MFrontMaterialLawParser::treatNamespace",
				"Namespace '"+name+"' is not valid.");
      }
      this->namespaces.push_back(name);
      ++(this->current);
      this->checkNotEndOfFile("MFrontMaterialLawParser::treatNamespace",
			      "Expected ';' or '::'.");
      if(this->current->value==";"){
	bend = true;
      } else if (this->current->value!="::"){
	this->throwRuntimeError("MFrontMaterialLawParser::treatNamespace",
				"Unexpected token '"+this->current->value+"' (expected ';' or '::').");
      }
      ++(this->current);
    }
  } // end of MFrontMaterialLawParser::treatNamespace(void)

  void
  MFrontMaterialLawParser::treatConstant(void)
  {
    using namespace std;
    string type;
    string name;
    unsigned short line;
    this->checkNotEndOfFile("MFrontMaterialLawParser::treatConstant",
			    "Cannot read variable name.");
    name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("ParserBase::treatConstant",
			      "constant name '"+name+"' is not valid.");
    }
    line = this->current->line;
    ++(this->current);
    const pair<bool,long double> value = this->readInitialisationValue<long double>(name,true);
    this->readSpecifiedToken("MFrontMaterialLawParser::treatConstant",";");
    this->registerStaticVariable(name);
    this->addStaticVariableDescription(StaticVariableDescription("real",name,line,value.second));
  } // end of MFrontMaterialLawParser::treatConstant

  void
  MFrontMaterialLawParser::treatParameter(void)
  {
    using namespace std;
    // note : we shall use the ParserBase::handleParameter method
    string parameter;
    this->checkNotEndOfFile("MFrontMaterialLawParser::treatParameter",
			    "Expected parameter name.");
    parameter = this->current->value;
    if(!isValidIdentifier(parameter)){
      this->throwRuntimeError("ParserBase::treatParameter",
			      "parameter name '"+parameter+"' is not valid.");
    }
    ++(this->current);
    const pair<bool,double> value = this->readInitialisationValue<double>(parameter,false);
    if(value.first){
      this->parametersValues.insert(make_pair(parameter,value.second));
    }
    this->readSpecifiedToken("MFrontMaterialLawParser::treatParameter",";");
    this->registerVariable(parameter,false);
    this->parameters.push_back(parameter);
  } // MFrontMaterialLawParser::treatParameter

  void
  MFrontMaterialLawParser::treatLaw(void)
  {
    using namespace std;
    if(!this->className.empty()){
      this->throwRuntimeError("MFrontMaterialLawParser::treatLaw",
			      "Law name has already been declared.");
    }
    if(!this->law.empty()){
      this->throwRuntimeError("MFrontMaterialLawParser::treatLaw",
			      "Law name has already been declared.");
    }
    this->law       = this->readOnlyOneToken();
    this->className = this->law;
    if(!isValidIdentifier(this->className)){
      --(this->current);
      this->throwRuntimeError("MFrontMaterialLawParser::treatLaw",
			      this->className+"is not a valid law name");
    }
  } // end of MFrontMaterialLawParser::treatLaw

  void
  MFrontMaterialLawParser::addInterface(const std::string& i)
  {
    using namespace std;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    vector<string>::const_iterator p2;
    vector<string>::const_iterator p2b;
    vector<string>::const_iterator p2e;
    const std::vector<std::string>& ai = mlif.getRegistredInterfaces();
    if(this->interfaces.find(i)!=this->interfaces.end()){
      string msg("MFrontMaterialLawParser::addInterface : ");
      msg += "interface '"+i+"' already declared";
      throw(runtime_error(msg));
    }
    if(find(ai.begin(),ai.end(),i)==ai.end()){
      string msg("MFrontMaterialLawParser::addInterface : ");
      msg += "unsupported interface '"+i+"'";
      if(ai.size()==1){
	msg += " (available interface is : '"+*(ai.begin())+"')";
      } else if(ai.size()!=0){
	msg += " (available interface are : ";
	p2   = ai.begin();
	p2b  = ai.begin();
	++p2b;
	p2e  = ai.end();
	while(p2b!=p2e){
	  msg += "'"+*p2+"'";
	  if(++p2b!=p2e){
	    msg+=",";
	  } else {
	    msg+=" and ";
	  }
	  ++p2;
	}
	msg += "'"+*p2+"')";
      }
      throw(runtime_error(msg));	
    }
    this->interfaces.insert(i);
  } // end of MFrontMaterialLawParser::addInterface

  void
  MFrontMaterialLawParser::setInterfaces(const std::set<std::string>& i) 
  {
    using namespace std;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    set<string>::const_iterator p;
    vector<string>::const_iterator p2;
    set<string> i2(i);
    // searching i2 depedencies
    for(p=i.begin();p!=i.end();++p){
      const vector<string>& dependencies = mlif.getInterfaceDependencies(*p);
      for(p2=dependencies.begin();p2!=dependencies.end();++p2){
	i2.insert(*p2);
      }
    }
    for(p=i2.begin();p!=i2.end();++p){
      this->addInterface(*p);
    }
  } // end of MFrontMaterialLawParser::setInterface

  void
  MFrontMaterialLawParser::setAnalysers(const std::set<std::string>&)
  {
    using namespace std;
    string msg("MFrontMaterialLawParser::setAnalysers : ");
    msg += "analysers are not supported for models";
    throw(runtime_error(msg));
  } // end of MFrontMaterialLawParser::setAnalysers

  void
  MFrontMaterialLawParser::treatInterface(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string interface;
    this->checkNotEndOfFile("MFrontMaterialLawParser::treatInterface",
			    "Expected interface name.");
    if(this->current->flag==Token::String){
      this->addInterface(this->current->value.substr(1,this->current->value.size()-2));
    } else {
      this->addInterface(this->current->value);
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontMaterialLawParser::treatInterface",";");
  } // end of MFrontMaterialLawParser::treatInterface

  void
  MFrontMaterialLawParser::treatFunction(void)
  {
    using namespace std;
    set<string>::const_iterator p2;
    set<string>::iterator p3;
    unsigned short openedBrackets = 0;
    unsigned short openedParenthesis = 0;
    unsigned short currentLine;
    bool newLine;
    bool newInstruction;
    bool treated;
    if(!this->f.body.empty()){
      string msg("MFrontMaterialLawParser::treatFunction : ");
      msg += "function already defined.";
      throw(runtime_error(msg));
    }
    if(this->output.empty()){
      this->registerVariable("res",false);
      this->output = "res";
    }
    this->f.modified = false;
    this->f.line = this->current->line;
    this->readSpecifiedToken("MFrontMaterialLawParser::treatFunction","{");
    // a simple check to see if this function is not empty
    this->checkNotEndOfFile("MFrontMaterialLawParser::treatFunction");
    if(this->current->value=="}"){
      this->throwRuntimeError("MFrontMaterialLawParser::treatFunction",
			      "Function is empty.");
    }
    --(this->current);
    ++openedBrackets;
    newInstruction = true;
    ++(this->current);
    this->checkNotEndOfFile("MFrontMaterialLawParser::treatFunction",
			    "Expected body of function.");
    currentLine = this->current->line;
    newLine=true;
    if(!getDebugMode()){
      this->f.body  +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
    }
    for(;(this->current!=this->fileTokens.end())&&
	  (openedBrackets!=0);++(this->current)){
      if(this->current->line!=currentLine){
	currentLine=this->current->line;
	f.body  += "\n";
	if(!getDebugMode()){
	  this->f.body  +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
	}
	newLine = true;
      } 
      if(this->current->value=="{"){
	++openedBrackets;
	this->f.body  +="{";
	newInstruction=true;
      } else if(this->current->value=="}"){
	--openedBrackets;
	if(openedBrackets!=0){
	  this->f.body  +="}";
	}
      } else if(this->current->value=="("){
	++openedParenthesis;
	this->f.body  +="(";
      } else if(this->current->value==")"){
	if(openedParenthesis==0){
	  this->throwRuntimeError("MFrontMaterialLawParser::treatFunction",
				  "unbalanced parenthesis");
	}
	--openedParenthesis;
	this->f.body  += ")";
      } else if(this->current->value==";"){
	this->f.body  += ";";
	newInstruction = true;
      } else {
	if(!newLine){
	  this->f.body  += " ";
	}
	if(this->varNames.find(this->current->value)!=this->varNames.end()){
	  treated = false;
	  if(newInstruction){
	    string var = this->current->value;
	    string op;
	    bool modifier;
	    ++(this->current);
	    if(this->current==fileTokens.end()){
	      string msg("MFrontMaterialLawParser::treatFunction : ");
	      msg+="unexpected end of file while reading body of function ";
	      throw(runtime_error(msg));
	    }
	    modifier = false;
	    if(this->current->value=="="){
	      op = "=";
	      modifier = true;
	    } else if(this->current->value=="+="){
	      op = "+=";
	      modifier = true;
	    } else if(this->current->value=="-="){
	      op = "-=";
	      modifier = true;
	    } else if(this->current->value=="*="){
	      op = "*=";
	      modifier = true;
	    } else if(this->current->value=="/="){
	      op = "/=";
	      modifier = true;
	    }
	    if(modifier){
	      if(var != this->output){
		this->throwRuntimeError("MFrontMaterialLawParser::treatFunction",
					"Trying to modify variable '"+var+"'.\n");
	      }
	      this->f.modified = true;
	      this->f.body  += var + " " + op + " ";
	      treated = true;
	    } else {
	      --(this->current);
	    }
	  }
	  if(!treated){
	    this->f.body  += this->current->value;
	  }
	} else {
	  this->f.body  += this->current->value;
	}
	newInstruction = false;
      }
      newLine=false;
    }
    if((this->current==fileTokens.end())&&(openedBrackets!=0)){
      string msg("MFrontMaterialLawParser::treatFunction : ");
      msg+="unexpected end of file while reading body of function";
      throw(runtime_error(msg));
    }
    if(openedBrackets!=0){
      string msg("MFrontMaterialLawParser::treatFunction : ");
      msg+="parenthesis still opened at the end of function";
      throw(runtime_error(msg));
    }
    if(this->f.body.empty()){
      string msg("MFrontMaterialLawParser::treatFunction : ");
      msg += "empty function.";
      throw(runtime_error(msg));      
    }
    if(!this->f.modified){
      this->throwRuntimeError("MFrontMaterialLawParser::treatFunction",
			      "Function does not modifiy output.");
    }
  } // end of MFrontMaterialLawParser::treatFunction(void)

  static void
  MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(const std::map<std::string,std::string>& g,
							 const std::map<std::string,std::string>& e,
							 const std::string& n)
  {
    using namespace std;
    map<string,string>::const_iterator p;
    for(p=g.begin();p!=g.end();++p){
      if(p->second==n){
	string msg("MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as a glossary name";
	throw(runtime_error(msg));
      }
    }
    for(p=e.begin();p!=e.end();++p){
      if(p->second==n){
	string msg("MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as an entry name";
	throw(runtime_error(msg));
      }
    }
  }

  void
  MFrontMaterialLawParser::treatMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    string methodName;
    typedef map<string,string>::value_type MVType;
    this->readSpecifiedToken("MFrontMaterialLawParser::treatMethod",".");
    this->checkNotEndOfFile("MFrontMaterialLawParser::treatMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
			      "unknown method '"+methodName+"' "
			      "valid methods are 'setGlossaryName', "
			      "'setEntryName' and 'setDefaultValue'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontMaterialLawParser::treatMethod","(");
    if(methodName=="setGlossaryName"){
      const Glossary& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("MFrontMaterialLawParser::treatMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar + "'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"Glossary name too short.");
      }
      const string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
							     this->entryNames,
							     glossaryName);
      if(!glossary.contains(glossaryName)){
	string msg("MFrontMaterialLawParser::treatMethod : "
		   "'"+glossaryName+"' is not a valid glossary name");
#warning "One day, we will throw"
	static_cast<void>(msg);
	//	throw(runtime_error(msg));
      }
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontMaterialLawParser::treatMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
							     this->entryNames,
							     entryName);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setDefaultValue"){
      if(find(this->parameters.begin(),this->parameters.end(),this->currentVar)==this->parameters.end()){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"method setDefaultValue is reserved for paramaters.");
      }
      double value;
      this->checkNotEndOfFile("MFrontMaterialLawParser::treatMethod",
			      "Expected to read value of variable '"+this->currentVar+"'");
      istringstream tmp(this->current->value);
      tmp >> value;
      if(!tmp&&(!tmp.eof())){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"Could not read value of variable '"+this->currentVar+"'.");
      }
      if(!this->parametersValues.insert(make_pair(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
				"default value already defined for variable '"+this->currentVar+"'.");
      }
    } else {
      this->throwRuntimeError("MFrontMaterialLawParser::treatMethod",
			      "Internal error (untreated method '"+ methodName +"'");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontMaterialLawParser::treatMethod",")");
    this->readSpecifiedToken("MFrontMaterialLawParser::treatMethod",";");
  } // end of MFrontMaterialLawParser::treatMethod
    
  void
  MFrontMaterialLawParser::treatFile(const std::string& fileName_,
				     const std::vector<std::string>& ecmds) 
  {
    this->analyseFile(fileName_,ecmds);
    this->writeOutputFiles();
  } // end of MFrontMaterialLawParser::treatFile

  void
  MFrontMaterialLawParser::analyseFile(const std::string& fileName_,
				       const std::vector<std::string>& ecmds) 
  {
    using namespace std;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    string key;
    CallBackContainer::const_iterator p;
    MemberFuncPtr handler = 0;
    this->fileName = fileName_;
    this->openFile(this->fileName,ecmds);
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->fileTokens.begin();
    while(this->current != this->fileTokens.end()){
      p = this->callBacks.find(this->current->value);
      if(p==this->callBacks.end()){
	VariableDescriptionContainer::const_iterator   p2;
	vector<string>::const_iterator p3;
	bool found = false;
	if(this->output==this->current->value){
	  found = true;
	  this->currentVar = this->output;
	  handler = &MFrontMaterialLawParser::treatMethod;
	}
	for(p2=this->inputs.begin();(p2!=this->inputs.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontMaterialLawParser::treatMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p3=this->parameters.begin();(p3!=this->parameters.end())&&(!found);){
	  if(*p3==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontMaterialLawParser::treatMethod;
	  } else {
	    ++p3;
	  }
	}
	if(!found){
	  handler = &MFrontMaterialLawParser::treatUnknownKeyword;
	}
	++(this->current);
	(this->*handler)();
      } else {
	pair<bool,CxxTokenizer::TokensContainer::const_iterator> p2;
	set<string>::const_iterator i;
	TokensContainer::const_iterator b;
	handler = p->second;
	key = this->current->value;
	this->currentComment = this->current->comment;
	++(this->current);
	b = this->current;
	try {
	  (this->*handler)();
       	} catch(const exception& e){
	  ostringstream msg;
	  msg << "MFrontMaterialLawParser::treatFile : error while treating keyword '" 
	      << key << "'.\n" ;
	  msg << e.what() << endl;
	  this->currentComment.clear();
	  throw(runtime_error(msg.str()));
	} catch (...){
	  ostringstream msg;
	  msg << "MFrontMaterialLawParser::treatFile : error while treating keyword '" 
	      << key << "'.\n" ;
	  this->currentComment.clear();
	  throw(runtime_error(msg.str()));
	}
	for(i  = this->interfaces.begin();
	    i != this->interfaces.end();++i){
	  MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
	  try{
	    p2 = interface->treatKeyword(key,b,
					 this->fileTokens.end());
	  }
	  catch(const runtime_error& e){
	    ostringstream msg;
	    msg << "MFrontMaterialLawParser::treatFile : error while treating keyword '" 
		 << key << "'.\n" ;
	    msg << e.what() << endl;
	    throw(runtime_error(msg.str()));
	  }
	  if(p2.first){
	    if(this->current!=p2.second){
	      string msg("MFrontMaterialLawParser::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by an interface but";
	      msg += " results were differents than from the parser.";
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->currentComment.clear();
      }
    }
  }

  void
  MFrontMaterialLawParser::writeOutputFiles(void)
  {
    using namespace std;
    using namespace tfel::system;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    set<string>::const_iterator p;
    if(this->className.empty()){
      string msg("MFrontMaterialLawParser::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    if(this->f.body.empty()){
      string msg("MFrontMaterialLawParser::writeOutputFiles : ");
      msg += "no function defined.";
      throw(runtime_error(msg));      
    }
    if(this->interfaces.empty()){
      string msg("MFrontMaterialLawParser::writeOutputFiles : ");
      msg += "no interface specified.";
      throw(runtime_error(msg));      
    }
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("src");
    // calling interfaces
    for(p=this->interfaces.begin();p!=this->interfaces.end();++p){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	getLogStream() << "calling interface " << *p << endl;
      }
      MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*p);
      interface->writeOutputFiles(this->fileName,
				  this->library,
				  this->material,
				  this->className,
				  this->authorName,this->date,
				  this->description,
				  this->includes,
				  this->output,this->inputs,
				  this->materialLaws,
				  this->glossaryNames,
				  this->entryNames,
				  MaterialPropertyDescription::staticVars,
				  this->parameters,
				  this->parametersValues,this->f,
				  this->boundsDescriptions,
				  this->physicalBoundsDescriptions,
				  this->useTemplate,
				  this->namespaces);
    }
  } // end of MFrontMaterialLawParser::writeOutputFiles

  void
  MFrontMaterialLawParser::treatInput(void)
  {
    this->readVarList(this->inputs,"real",
		      false,false,false);
  } // end of MFrontMaterialLawParser::treatInput

  void
  MFrontMaterialLawParser::treatOutput(void)
  {
    using namespace std;
    if(!this->output.empty()){
      this->throwRuntimeError("MFrontMaterialLawParser::treatOutput",
			      "Output already defined.");
    }
    this->output = this->readOnlyOneToken();
    this->registerVariable(this->output,false);
  } // end of MFrontMaterialLawParser::treatOutput

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialLawParser::getGeneratedSources(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    Map osources;
    set<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
      const Map& isources = interface->getGeneratedSources(this->library,
							   this->material,
							   this->className);
      for(p=isources.begin();p!=isources.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(osources[p->first]));
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
  } // end of MFrontMaterialLawParser::getGeneratedSources

  std::vector<std::string>
  MFrontMaterialLawParser::getGeneratedIncludes(void)
  {
    using namespace std;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    vector<string> incs;
    set<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
      const vector<string>& iincs = interface->getGeneratedIncludes(this->library,
								    this->material,
								    this->className);
      copy(iincs.begin(),iincs.end(),back_inserter(incs));
    }
    return incs;
  } // end of MFrontMaterialLawParser::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialLawParser::getGlobalIncludes(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    Map incs;
    set<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
      const Map& iincs = interface->getGlobalIncludes(this->library,
						      this->material,
						      this->className);
      for(p=iincs.begin();p!=iincs.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(incs[p->first]));
      }
    }
    return incs;
  } // end of MFrontMaterialLawParser::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialLawParser::getGlobalDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    Map deps;
    set<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
      const Map& ideps = interface->getGlobalDependencies(this->library,
							  this->material,
							  this->className);
      for(p=ideps.begin();p!=ideps.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(deps[p->first]));
      }
    }
    return deps;
  } // end of MFrontMaterialLawParser::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialLawParser::getLibrariesDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    Map deps;
    set<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
      const Map& ideps = interface->getLibrariesDependencies(this->library,
							     this->material,
							     this->className);
      for(p=ideps.begin();p!=ideps.end();++p){
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  if(find(this->sourcesLibrairiesDependencies[p->first].begin(),
		  this->sourcesLibrairiesDependencies[p->first].end(),
		  *p2)!=this->sourcesLibrairiesDependencies[p->first].end()){
	    this->sourcesLibrairiesDependencies[p->first].push_back(*p2);
	  }
	}
      }
    }
    return this->sourcesLibrairiesDependencies;
  } // end of MFrontMaterialLawParser::getLibrariesDependencies

  void
  MFrontMaterialLawParser::treatBounds(void)
  {
    this->registerBounds(this->boundsDescriptions);
  } // end of MFrontMaterialLawParser::treatBounds

  void
  MFrontMaterialLawParser::treatPhysicalBounds(void)
  {
    this->registerBounds(this->physicalBoundsDescriptions);
  } // end of MFrontMaterialLawParser::treatPhysicalBounds

  void
  MFrontMaterialLawParser::registerBounds(std::vector<VariableBoundsDescription>& container)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    vector<VariableBoundsDescription>::const_iterator p3;
    VariableBoundsDescription boundsDescription;
    unsigned short i;
    bool found;
    this->checkNotEndOfFile("MFrontMaterialLawParser::registerBounds");
    boundsDescription.lineNumber = this->current->line;
    boundsDescription.varName = this->current->value;
    found = false;
    for(i=1u,p=this->inputs.begin();
	(p!=this->inputs.end())&&(!found);++p,++i){
      if(p->name==boundsDescription.varName){
	found=true;
	boundsDescription.varNbr = i;
      }
    }
    if(!found){
      this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
			      "'"+this->current->value+"' is not a valid identifier.");
    }
    found = false;
    for(p3=container.begin();(p3!=container.end())&&(!found);++p3){
      if(p3->varName==boundsDescription.varName){
	found = true;
      }
    }
    if(found){
      this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
			      "bounds for variable '"+this->current->value+"' already declared.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontMaterialLawParser::registerBounds","in");
    this->checkNotEndOfFile("MFrontMaterialLawParser::registerBounds ",
			    "Expected ']' or '['.");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontMaterialLawParser::registerBounds ",
			      "Expected '*'.");
      if(this->current->value!="*"){
	this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      boundsDescription.boundsType = VariableBoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("MFrontMaterialLawParser::registerBounds ",
			      "Expected lower bound value for variable "+boundsDescription.varName);
      istringstream converter(this->current->value);
      converter >> boundsDescription.lowerBound;
      boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
    } else {
      this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontMaterialLawParser::registerBounds",":");
    this->checkNotEndOfFile("MFrontMaterialLawParser::registerBounds",
			    "Could not read upper bound value for variable "+boundsDescription.varName);
    if(this->current->value=="*"){
      if(boundsDescription.boundsType==VariableBoundsDescription::Upper){
	this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
				"Upper and lower values bounds are both infinity. This is inconsistent.");
      }
      boundsDescription.boundsType=VariableBoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("MFrontMaterialLawParser::registerBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> boundsDescription.upperBound;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
      if(boundsDescription.boundsType==VariableBoundsDescription::LowerAndUpper){
	if(boundsDescription.lowerBound>boundsDescription.upperBound){
	  this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
				  "Lower bound value is greater than upper bound value for variable '"+
				  boundsDescription.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontMaterialLawParser::registerBounds",
			      "Expected ']'");
      if(this->current->value!="]"){
	this->throwRuntimeError("MFrontMaterialLawParser::registerBounds",
				"Expected ']' (read '"+this->current->value+"'");
      }      
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontMaterialLawParser::registerBounds",";");
    container.push_back(boundsDescription);
  } // end of MFrontMaterialLawParser::registerBounds

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontMaterialLawParser::getSpecificTargets(void)
  {
    using namespace std;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    typedef map<string,pair<vector<string>,vector<string> > > Target;
    Target res;
    set<string>::const_iterator p;
    Target::const_iterator p2;
    for(p=this->interfaces.begin();p!=this->interfaces.end();++p){
      MFrontLawVirtualInterface *i = mlif.getInterfacePtr(*p);
      const Target& targets = i->getSpecificTargets(this->library,
						    this->material,
						    this->className,
						    this->librariesDependencies);
      for(p2=targets.begin();p2!=targets.end();++p2){
	copy(p2->second.first.begin(),
	     p2->second.first.end(),
	     back_inserter(res[p2->first].first));
	copy(p2->second.second.begin(),
	     p2->second.second.end(),
	     back_inserter(res[p2->first].second));
      }
    }
    return res;
  } // end of MFrontMaterialLawParser::getSpecificTargets(void)
  
  void
  MFrontMaterialLawParser::treatUnknownKeyword(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    pair<bool,CxxTokenizer::TokensContainer::const_iterator> p;
    TokensContainer::const_iterator p2;
    set<string>::const_iterator i;
    bool treated = false;
    string key;
    --(this->current);
    key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontMaterialLawParser::treatUnknownKeyword");
    if(this->current->value=="["){
      set<string> s;
      while(this->current->value!="]"){
	++(this->current);
	this->checkNotEndOfFile("MFrontMaterialLawParser::treatUnknownKeyword");
	string t;
	if(this->current->flag==Token::String){
	  t = this->current->value.substr(1,this->current->value.size()-2);
	} else {
	  t = this->current->value;
	}
	if(this->interfaces.find(t)!=this->interfaces.end()){
	  s.insert(t);
	}
	++(this->current);
	if((this->current->value!="]")&&(this->current->value!=",")){
	  this->throwRuntimeError("MFrontMaterialLawParser::treatUnknownKeyword",
				  "unexpected token '"+this->current->value+"' (expected ']' or ',').");
	}
      }
      ++(this->current);
      if(s.empty()){
	this->ignoreKeyWord(key);
      } else {
	for(i  = s.begin();i != s.end();++i){
	  MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
	  p = interface->treatKeyword(key,this->current,
				      this->fileTokens.end());
	  if(!p.first){
	    string msg("MFrontMaterialLawParser::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += " has not been treated by interface '"+*i+"'";
	    throw(runtime_error(msg));
	  }
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontMaterialLawParser::treatUnknownKeyword : the keyword '");
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
	MFrontLawVirtualInterface *interface = mlif.getInterfacePtr(*i);
	p = interface->treatKeyword(key,this->current,
				    this->fileTokens.end());
	if(p.first){
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontMaterialLawParser::treatUnknownKeyword : the keyword '");
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
  } // end of MFrontMaterialLawParser::treatUnknownKeyword

  const MaterialPropertyDescription&
  MFrontMaterialLawParser::getMaterialPropertyDescription(void) const
  {
    return *this;
  } // end of MFrontMaterialLawParser::getMaterialPropertyDescription

  void
  MFrontMaterialLawParser::appendToIncludes(const std::string& c)
  {
    this->includes+=c;
    if(!this->includes.empty()){
      if(*(this->includes.rbegin())!='\n'){
	this->includes+='\n';
      }
    }
  } // end of MFrontMaterialLawParser::appendToIncludes

  void
  MFrontMaterialLawParser::appendToMembers(const std::string& c)
  {
    this->members+=c;
    if(!this->members.empty()){
      if(*(this->members.rbegin())!='\n'){
	this->members+='\n';
      }
    }
  } // end of MFrontMaterialLawParser::appendToMembers

  void
  MFrontMaterialLawParser::appendToPrivateCode(const std::string& c)
  {
    this->privateCode+=c;
    if(!this->privateCode.empty()){
      if(*(this->privateCode.rbegin())!='\n'){
	this->privateCode+='\n';
      }
    }
  } // end of MFrontMaterialLawParser::appendToPrivateCode

  void
  MFrontMaterialLawParser::appendToSources(const std::string& c)
  {
    this->sources+=c;
    if(!this->sources.empty()){
      if(*(this->sources.rbegin())!='\n'){
	this->sources+='\n';
      }
    }
  } // end of MFrontMaterialLawParser::appendToSources

  void
  MFrontMaterialLawParser::addMaterialLaw(const std::string& m)
  {
    using namespace std;
    if(find(this->materialLaws.begin(),
	    this->materialLaws.end(),m)==this->materialLaws.end()){
      this->materialLaws.push_back(m);
    }
  } // end of MFrontMaterialLawParser::addMaterialLaw

} // end of namespace mfront  

