/*!
 * \file   mfront/src/MaterialPropertyDSL.cxx
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
#include<stdexcept>
#include<sstream>
#include<cctype>
#include<algorithm>
#include<iterator>
#include<cstdlib>

#include"TFEL/Utilities/Token.hxx"
#include"TFEL/System/System.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/DSLFactory.hxx"
#include"MFront/MaterialPropertyDSL.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyInterfaceFactory.hxx"

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

  MaterialPropertyDSL::MaterialPropertyDSL(void)
  {
    // Call Back
    this->registerNewCallBack("@Link",
			      &MaterialPropertyDSL::treatLink);
    this->registerNewCallBack("@MaterialLaw",
			      &MaterialPropertyDSL::treatMaterialLaw);
    this->registerNewCallBack("@MFront",
			      &MaterialPropertyDSL::treatMFront);
    this->registerNewCallBack("@Material",&MaterialPropertyDSL::treatMaterial);
    this->registerNewCallBack("@Library",&MaterialPropertyDSL::treatLibrary);
    this->registerNewCallBack("@DSL",&MaterialPropertyDSL::treatParser);
    this->registerNewCallBack("@Parser",&MaterialPropertyDSL::treatParser);
    this->registerNewCallBack("@Law",&MaterialPropertyDSL::treatLaw);
    this->registerNewCallBack("@Author",&MaterialPropertyDSL::treatAuthor);
    this->registerNewCallBack("@Date",&MaterialPropertyDSL::treatDate);
    this->registerNewCallBack("@Includes",&MaterialPropertyDSL::treatIncludes);
    this->registerNewCallBack("@StaticVar",&MaterialPropertyDSL::treatStaticVar);
    this->registerNewCallBack("@StaticVariable",&MaterialPropertyDSL::treatStaticVar);
    this->registerNewCallBack("@Parameter",&MaterialPropertyDSL::treatParameter);
    this->registerNewCallBack("@Constant",&MaterialPropertyDSL::treatConstant);
    this->registerNewCallBack("@Description",&MaterialPropertyDSL::treatDescription);
    this->registerNewCallBack("@Input",&MaterialPropertyDSL::treatInput);
    this->registerNewCallBack("@Output",&MaterialPropertyDSL::treatOutput);
    this->registerNewCallBack("@Function",&MaterialPropertyDSL::treatFunction);
    this->registerNewCallBack("@Import",&MaterialPropertyDSL::treatImport);
    this->registerNewCallBack("@Interface",
			      &MaterialPropertyDSL::treatInterface);
    this->registerNewCallBack("@Bounds",
			      &MaterialPropertyDSL::treatBounds);
    this->registerNewCallBack("@PhysicalBounds",
			      &MaterialPropertyDSL::treatPhysicalBounds);
    // reverse names
    for(const auto& v : DSLBase::getDefaultReservedNames()){
      this->reserveName(v);
    }
    this->reserveName("params");
  } // end of MaterialPropertyDSL::MaterialPropertyDSL()

  
  AbstractDSL::DSLTarget
  MaterialPropertyDSL::getTargetType(void) const{
    return MATERIALPROPERTYDSL;
  }

  void
  MaterialPropertyDSL::registerNewCallBack(const std::string& keyword,
					       const MemberFuncPtr ptr)
  {
    using namespace std;
    this->callBacks.insert(make_pair(keyword,ptr));
    this->registredKeyWords.insert(keyword);
  } // end of MaterialPropertyDSL::registerNewCall

  void
  MaterialPropertyDSL::getKeywordsList(std::vector<std::string>& k) const
  {
    CallBackContainer::const_iterator p;
    for(p=this->callBacks.begin();p!=this->callBacks.end();++p){
      k.push_back(p->first);
    }
  }

  std::string
  MaterialPropertyDSL::getClassName(void) const
  {
    return this->className;
  } // end of MaterialPropertyDSL::getClassName

  void
  MaterialPropertyDSL::addStaticVariableDescription(const StaticVariableDescription& v)
  {
    this->reserveName(v.name);
    this->staticVars.push_back(v);
  } // end of MaterialPropertyDSL::addStaticVariableDescription

  std::string
  MaterialPropertyDSL::getDescription()
  {
    using namespace std;
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    const auto& ai = mlif.getRegistredInterfaces();
    auto p2  = ai.cbegin();
    const auto p2e = ai.cend();
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
  } // end of MaterialPropertyDSL::getDescription

  std::string
  MaterialPropertyDSL::getName(void)
  {
    return "MaterialLaw";
  } // end of MaterialPropertyDSL::getName(void)

  void
  MaterialPropertyDSL::treatMaterial(void)
  {
    if(!this->material.empty()){
      throw(std::runtime_error("MaterialPropertyDSL::treatMaterial: "
			       "material name alreay defined"));
    }
    this->material = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(this->material,true)){
      throw(std::runtime_error("MaterialPropertyDSL::treatMaterial : "
			       "invalid material name '"+this->material+"'"));
    }
  } // end of MaterialPropertyDSL::treatMaterial

  void
  MaterialPropertyDSL::treatLibrary(void)
  {
    using namespace std;
    if(!this->library.empty()){
      string msg("MaterialPropertyDSL::treatLibrary : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    const auto& l = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(l,true)){
      string msg("MaterialPropertyDSL::treatLibrary : ");
      msg += "invalid library name '"+l+"'";
      throw(runtime_error(msg));
    }
    this->library = l;
  } // end of MFrontLibraryLawParser::treatLibrary

  void
  MaterialPropertyDSL::treatConstant(void)
  {
    this->checkNotEndOfFile("MaterialPropertyDSL::treatConstant",
			    "Cannot read variable name.");
    const auto name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("DSLBase::treatConstant",
			      "constant name '"+name+"' is not valid.");
    }
    const auto line = this->current->line;
    ++(this->current);
    const auto value = this->readInitialisationValue<long double>(name,true);
    this->readSpecifiedToken("MaterialPropertyDSL::treatConstant",";");
    this->addStaticVariableDescription(StaticVariableDescription("real",name,line,value.second));
  } // end of MaterialPropertyDSL::treatConstant

  void
  MaterialPropertyDSL::treatParameter(void)
  {
    using namespace std;
    // note : we shall use the DSLBase::handleParameter method
    string parameter;
    this->checkNotEndOfFile("MaterialPropertyDSL::treatParameter",
			    "Expected parameter name.");
    parameter = this->current->value;
    if(!isValidIdentifier(parameter)){
      this->throwRuntimeError("DSLBase::treatParameter",
			      "parameter name '"+parameter+"' is not valid.");
    }
    ++(this->current);
    const pair<bool,double> value = this->readInitialisationValue<double>(parameter,false);
    if(value.first){
      this->parametersValues.insert(make_pair(parameter,value.second));
    }
    this->readSpecifiedToken("MaterialPropertyDSL::treatParameter",";");
    this->reserveName(parameter);
    this->parameters.push_back(parameter);
  } // MaterialPropertyDSL::treatParameter

  void
  MaterialPropertyDSL::treatLaw(void)
  {
    using namespace std;
    if(!this->className.empty()){
      this->throwRuntimeError("MaterialPropertyDSL::treatLaw",
			      "Law name has already been declared.");
    }
    if(!this->law.empty()){
      this->throwRuntimeError("MaterialPropertyDSL::treatLaw",
			      "Law name has already been declared.");
    }
    this->law       = this->readOnlyOneToken();
    this->className = this->law;
    if(!isValidIdentifier(this->className)){
      --(this->current);
      this->throwRuntimeError("MaterialPropertyDSL::treatLaw",
			      this->className+"is not a valid law name");
    }
  } // end of MaterialPropertyDSL::treatLaw

  void
  MaterialPropertyDSL::addInterface(const std::string& i)
  {
    using namespace std;
    typedef MaterialPropertyInterfaceFactory MLIF;
    if(this->interfaces.find(i)==this->interfaces.end()){
      auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
      this->interfaces.insert({i,mlif.getInterface(i)});
    }
  } // end of MaterialPropertyDSL::addInterface

  void
  MaterialPropertyDSL::setInterfaces(const std::set<std::string>& inames) 
  {
    using namespace std;
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    // searching i2 depedencies
    for(const auto i: inames){
      for(const auto& d : mlif.getInterfaceDependencies(i)){
	this->addInterface(d);
      }
      this->addInterface(i);
    }
  } // end of MaterialPropertyDSL::setInterface

  void
  MaterialPropertyDSL::treatInterface(void) 
  {
    using namespace tfel::utilities;
    this->checkNotEndOfFile("MaterialPropertyDSL::treatInterface",
			    "Expected interface name.");
    if(this->current->flag==Token::String){
      this->addInterface(this->current->value.substr(1,this->current->value.size()-2));
    } else {
      this->addInterface(this->current->value);
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatInterface",";");
  } // end of MaterialPropertyDSL::treatInterface

  void
  MaterialPropertyDSL::treatFunction(void)
  {
    using namespace std;
    unsigned int openedBrackets = 0;
    unsigned int openedParenthesis = 0;
    unsigned int currentLine;
    bool newLine;
    bool newInstruction;
    bool treated;
    if(!this->f.body.empty()){
      string msg("MaterialPropertyDSL::treatFunction : ");
      msg += "function already defined.";
      throw(runtime_error(msg));
    }
    if(this->output.empty()){
      this->reserveName("res");
      this->output = "res";
    }
    this->f.modified = false;
    this->f.line = this->current->line;
    this->readSpecifiedToken("MaterialPropertyDSL::treatFunction","{");
    // a simple check to see if this function is not empty
    this->checkNotEndOfFile("MaterialPropertyDSL::treatFunction");
    if(this->current->value=="}"){
      this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
			      "Function is empty.");
    }
    --(this->current);
    ++openedBrackets;
    newInstruction = true;
    ++(this->current);
    this->checkNotEndOfFile("MaterialPropertyDSL::treatFunction",
			    "Expected body of function.");
    currentLine = this->current->line;
    newLine=true;
    if(!getDebugMode()){
      this->f.body  +="#line " + to_string(currentLine) + " \"" + this->fileName + "\"\n";
    }
    for(;(this->current!=this->fileTokens.end())&&
	  (openedBrackets!=0);++(this->current)){
      if(this->current->line!=currentLine){
	currentLine=this->current->line;
	f.body  += "\n";
	if(!getDebugMode()){
	  this->f.body  +="#line " + to_string(currentLine) + " \"" + this->fileName + "\"\n";
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
	  this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
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
	if((this->current->value==output)||
	   (this->inputs.contains(this->current->value))){
	  treated = false;
	  if(newInstruction){
	    const auto var = this->current->value;
	    ++(this->current);
	    if(this->current==fileTokens.end()){
	      string msg("MaterialPropertyDSL::treatFunction : ");
	      msg+="unexpected end of file while reading body of function ";
	      throw(runtime_error(msg));
	    }
	    if((this->current->value=="=")||
	       (this->current->value=="+=")||
	       (this->current->value=="-=")||
	       (this->current->value=="*=")||
	       (this->current->value=="/=")){
	      if(var != this->output){
		this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
					"Trying to modify variable '"+var+"'.\n");
	      }
	      this->f.modified = true;
	      this->f.body  += var + " " + this->current->value + " ";
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
      string msg("MaterialPropertyDSL::treatFunction : ");
      msg+="unexpected end of file while reading body of function";
      throw(runtime_error(msg));
    }
    if(openedBrackets!=0){
      string msg("MaterialPropertyDSL::treatFunction : ");
      msg+="parenthesis still opened at the end of function";
      throw(runtime_error(msg));
    }
    if(this->f.body.empty()){
      string msg("MaterialPropertyDSL::treatFunction : ");
      msg += "empty function.";
      throw(runtime_error(msg));      
    }
    if(!this->f.modified){
      this->throwRuntimeError("MaterialPropertyDSL::treatFunction",
			      "Function does not modifiy output.");
    }
  } // end of MaterialPropertyDSL::treatFunction(void)

  static void
  MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(const std::map<std::string,std::string>& g,
							 const std::map<std::string,std::string>& e,
							 const std::string& n)
  {
    for(const auto& gn : g){
      if(gn.second==n){
	throw(std::runtime_error("MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName : "
				 "name '"+n+"' is already used as a glossary name"));
      }
    }
    for(const auto& en : e){
      if(en.second==n){
	throw(std::runtime_error("MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName : "
				 "name '"+n+"' is already used as a entry name"));
      }
    }
  }

  void
  MaterialPropertyDSL::treatMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod",".");
    this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
			    "Expected method name.");
    const auto methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
			      "unknown method '"+methodName+"' "
			      "valid methods are 'setGlossaryName', "
			      "'setEntryName' and 'setDefaultValue'");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod","(");
    if(methodName=="setGlossaryName"){
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"A glossary or an entry name has already "
				"been defined for field '"+this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Glossary name too short.");
      }
      const auto glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
							     this->entryNames,
							     glossaryName);
      if(!glossary.contains(glossaryName)){
	throw(runtime_error("MaterialPropertyDSL::treatMethod : "
			    "'"+glossaryName+"' is not a valid glossary name"));
      }
      const auto& k = glossary.getGlossaryEntry(glossaryName).getKey();
      if(!this->glossaryNames.insert({this->currentVar,k}).second){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"glossary name for field '"+this->currentVar+"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"A glossary or an entry name has already been "
				"defined for field '"+this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Entry file name too short.");
      }
      const auto entryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontLawParserCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
							     this->entryNames,
							     entryName);
      if(glossary.contains(entryName)){
	ostringstream msg;
	msg << "MaterialPropertyDSL::treatMethod : "
	    << "'" << entryName <<"' is a glossary name. Please use "
	    << "the 'setGlossaryName' method or choose another entry name.";
	displayGlossaryEntryCompleteDescription(msg,glossary.getGlossaryEntry(entryName));
	throw(runtime_error(msg.str()));
      }
      if(!CxxTokenizer::isValidIdentifier(entryName)){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Invalid entry name '"+entryName+"'");
      }
      if(entryName!=this->currentVar){
	this->reserveName(entryName);
      }
      if(!this->entryNames.insert({this->currentVar,entryName}).second){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Entry file name for field '"+this->currentVar+"' already defined.");
      }
    } else if(methodName=="setDefaultValue"){
      if(find(this->parameters.begin(),this->parameters.end(),this->currentVar)==this->parameters.end()){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"method setDefaultValue is reserved for paramaters.");
      }
      double value;
      this->checkNotEndOfFile("MaterialPropertyDSL::treatMethod",
			      "Expected to read value of variable '"+this->currentVar+"'");
      istringstream tmp(this->current->value);
      tmp >> value;
      if(!tmp&&(!tmp.eof())){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Could not read value of variable '"+this->currentVar+"'.");
      }
      if(!this->parametersValues.insert(make_pair(this->currentVar,value)).second){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"default value already defined for variable '"+this->currentVar+"'.");
      }
    } else {
      this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
			      "Internal error (untreated method '"+ methodName +"'");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod",")");
    this->readSpecifiedToken("MaterialPropertyDSL::treatMethod",";");
  } // end of MaterialPropertyDSL::treatMethod

  void
  MaterialPropertyDSL::importFile(const std::string& fileName_,
				  const std::vector<std::string>& ecmds) 
  {
    this->fileName = fileName_;
    this->openFile(this->fileName,ecmds);
    this->analyse();
  }

  void
  MaterialPropertyDSL::analyseString(const std::string& s)
  {
    this->fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  }
  
  void
  MaterialPropertyDSL::analyse(void)
  {
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->fileTokens.begin();
    while(this->current != this->fileTokens.end()){
      const auto p = this->callBacks.find(this->current->value);
      if(p==this->callBacks.end()){
	MemberFuncPtr handler = nullptr;
	if(this->output==this->current->value){
	  this->currentVar = this->output;
	  handler = &MaterialPropertyDSL::treatMethod;
	}
	for(auto p2=this->inputs.begin();
	    (p2!=this->inputs.end())&&(handler==nullptr);){
	  if(p2->name==this->current->value){
	    this->currentVar = this->current->value;
	    handler = &MaterialPropertyDSL::treatMethod;
	  } else {
	    ++p2;
	  }
	}
	for(auto p3=this->parameters.begin();
	    (p3!=this->parameters.end())&&(handler==nullptr);){
	  if(*p3==this->current->value){
	    this->currentVar = this->current->value;
	    handler = &MaterialPropertyDSL::treatMethod;
	  } else {
	    ++p3;
	  }
	}
	if(handler==nullptr){
	  handler = &MaterialPropertyDSL::treatUnknownKeyword;
	}
	++(this->current);
	(this->*handler)();
      } else {
	auto handler = p->second;
	auto key = this->current->value;
	this->currentComment = this->current->comment;
	++(this->current);
	auto b = this->current;
	try {
	  (this->*handler)();
       	} catch(const std::exception& e){
	  this->currentComment.clear();
	  throw(std::runtime_error("MaterialPropertyDSL::analyse: "
				   "error while treating keyword '"+key+"'.\n"
				   +std::string(e.what())));
	} catch (...){
	  this->currentComment.clear();
	  throw(std::runtime_error("MaterialPropertyDSL::analyse: "
				   "error while treating keyword '"+key+'\''));
	}
	for(const auto& i : this->interfaces){
	  std::pair<bool,CxxTokenizer::TokensContainer::const_iterator> p2;
	  try{
	    p2 = i.second->treatKeyword(key,b,this->fileTokens.end());
	  }
	  catch(const std::runtime_error& e){
	    throw(std::runtime_error("MaterialPropertyDSL::analyse: "
				     "error while treating keyword '"+key+"'.\n"
				     +std::string(e.what())));
	  }
	  if(p2.first){
	    if(this->current!=p2.second){
	      throw(std::runtime_error("MaterialPropertyDSL::treatUnknownKeyword: "
				       "the keyword '"+key+"' has been treated "
				       "by an interface but results were differents "
				       "than from the parser."));
	    }
	  }
	}
	this->currentComment.clear();
      }
    }
  }

  void
  MaterialPropertyDSL::analyseFile(const std::string& fileName_,
				   const std::vector<std::string>& ecmds) 
  {
    this->importFile(fileName_,ecmds);
    for(const auto & i : this->interfaces){
      i.second->getTargetsDescription(this->td,*this);
    }
    this->completeTargetsDescription();
  }

  void
  MaterialPropertyDSL::reserveName(const std::string& n){
    if(!this->reservedNames.insert(n).second){
      throw(std::runtime_error("MaterialPropertyDSL::reserveName: "
			       "name '"+n+"' already reserved"));
    }
  }
      
  void
  MaterialPropertyDSL::generateOutputFiles(void)
  {
    using namespace tfel::system;
    if(this->className.empty()){
      throw(std::runtime_error("MaterialPropertyDSL::generateOutputFiles: "
			       "no behaviour name defined."));      
    }
    if(this->f.body.empty()){
      throw(std::runtime_error("MaterialPropertyDSL::generateOutputFiles: "
			       "no function defined."));      
    }
    if(this->interfaces.empty()){
      throw(std::runtime_error("MaterialPropertyDSL::generateOutputFiles: "
			       "no interface defined."));      
    }
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("src");
    // calling interfaces
    for(const auto& i : this->interfaces){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	getLogStream() << "calling interface " << i.first << '\n';
      }
      i.second->writeOutputFiles(*this,*this);
    }
  } // end of MaterialPropertyDSL::generateOutputFiles

  void
  MaterialPropertyDSL::treatInput(void)
  {
    VariableDescriptionContainer ninputs;
    this->readVarList(ninputs,"real",false);
    for(const auto& i : ninputs){
      this->reserveName(i.name);
      this->inputs.push_back(i);
    }
  } // end of MaterialPropertyDSL::treatInput

  void
  MaterialPropertyDSL::treatOutput(void)
  {
    if(!this->output.empty()){
      this->throwRuntimeError("MaterialPropertyDSL::treatOutput",
			      "Output already defined.");
    }
    this->output = this->readOnlyOneToken();
    this->reserveName(this->output);
  } // end of MaterialPropertyDSL::treatOutput

  void
  MaterialPropertyDSL::treatBounds(void)
  {
    this->registerBounds(this->boundsDescriptions);
  } // end of MaterialPropertyDSL::treatBounds

  void
  MaterialPropertyDSL::treatPhysicalBounds(void)
  {
    this->registerBounds(this->physicalBoundsDescriptions);
  } // end of MaterialPropertyDSL::treatPhysicalBounds

  void
  MaterialPropertyDSL::registerBounds(std::vector<VariableBoundsDescription>& container)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    vector<VariableBoundsDescription>::const_iterator p3;
    VariableBoundsDescription boundsDescription;
    unsigned short i;
    bool found;
    this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds");
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
      this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
			      "'"+this->current->value+"' is not a valid identifier.");
    }
    found = false;
    for(p3=container.begin();(p3!=container.end())&&(!found);++p3){
      if(p3->varName==boundsDescription.varName){
	found = true;
      }
    }
    if(found){
      this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
			      "bounds for variable '"+this->current->value+"' already declared.");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::registerBounds","in");
    this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds ",
			    "Expected ']' or '['.");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds ",
			      "Expected '*'.");
      if(this->current->value!="*"){
	this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      boundsDescription.boundsType = VariableBoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds ",
			      "Expected lower bound value for variable "+boundsDescription.varName);
      istringstream converter(this->current->value);
      converter >> boundsDescription.lowerBound;
      boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
    } else {
      this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::registerBounds",":");
    this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds",
			    "Could not read upper bound value for variable "+boundsDescription.varName);
    if(this->current->value=="*"){
      if(boundsDescription.boundsType==VariableBoundsDescription::Upper){
	this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
				"Upper and lower values bounds are both infinity. This is inconsistent.");
      }
      boundsDescription.boundsType=VariableBoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> boundsDescription.upperBound;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
      if(boundsDescription.boundsType==VariableBoundsDescription::LowerAndUpper){
	if(boundsDescription.lowerBound>boundsDescription.upperBound){
	  this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
				  "Lower bound value is greater than upper bound value for variable '"+
				  boundsDescription.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("MaterialPropertyDSL::registerBounds",
			      "Expected ']'");
      if(this->current->value!="]"){
	this->throwRuntimeError("MaterialPropertyDSL::registerBounds",
				"Expected ']' (read '"+this->current->value+"'");
      }      
    }
    ++(this->current);
    this->readSpecifiedToken("MaterialPropertyDSL::registerBounds",";");
    container.push_back(boundsDescription);
  } // end of MaterialPropertyDSL::registerBounds
  
  void
  MaterialPropertyDSL::treatUnknownKeyword(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    pair<bool,CxxTokenizer::TokensContainer::const_iterator> p;
    TokensContainer::const_iterator p2;
    bool treated = false;
    string key;
    --(this->current);
    key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MaterialPropertyDSL::treatUnknownKeyword");
    if(this->current->value=="["){
      set<string> s;
      while(this->current->value!="]"){
	++(this->current);
	this->checkNotEndOfFile("MaterialPropertyDSL::treatUnknownKeyword");
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
	  this->throwRuntimeError("MaterialPropertyDSL::treatUnknownKeyword",
				  "unexpected token '"+this->current->value+"' (expected ']' or ',').");
	}
      }
      ++(this->current);
      if(s.empty()){
	this->ignoreKeyWord(key);
      } else {
	for(const auto &i : s){
	  p = this->interfaces.at(i)->treatKeyword(key,this->current,
						   this->fileTokens.end());
	  if(!p.first){
	    string msg("MaterialPropertyDSL::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += " has not been treated by interface '"+i+"'";
	    throw(runtime_error(msg));
	  }
	  if(treated){
	    if(p2!=p.second){
	      string msg("MaterialPropertyDSL::treatUnknownKeyword : the keyword '");
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
      for(const auto& i : this->interfaces){
	p = i.second->treatKeyword(key,this->current,
				   this->fileTokens.end());
	if(p.first){
	  if(treated){
	    if(p2!=p.second){
	      string msg("MaterialPropertyDSL::treatUnknownKeyword : the keyword '");
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
	DSLBase::treatUnknownKeyword();
      }
      this->current = p2;
    }
  } // end of MaterialPropertyDSL::treatUnknownKeyword

  const MaterialPropertyDescription&
  MaterialPropertyDSL::getMaterialPropertyDescription(void) const
  {
    return *this;
  } // end of MaterialPropertyDSL::getMaterialPropertyDescription

  void
  MaterialPropertyDSL::appendToIncludes(const std::string& c)
  {
    this->includes+=c;
    if(!this->includes.empty()){
      if(*(this->includes.rbegin())!='\n'){
	this->includes+='\n';
      }
    }
  } // end of MaterialPropertyDSL::appendToIncludes

  void
  MaterialPropertyDSL::appendToMembers(const std::string& c)
  {
    this->members+=c;
    if(!this->members.empty()){
      if(*(this->members.rbegin())!='\n'){
	this->members+='\n';
      }
    }
  } // end of MaterialPropertyDSL::appendToMembers

  void
  MaterialPropertyDSL::appendToPrivateCode(const std::string& c)
  {
    this->privateCode+=c;
    if(!this->privateCode.empty()){
      if(*(this->privateCode.rbegin())!='\n'){
	this->privateCode+='\n';
      }
    }
  } // end of MaterialPropertyDSL::appendToPrivateCode

  void
  MaterialPropertyDSL::appendToSources(const std::string& c)
  {
    this->sources+=c;
    if(!this->sources.empty()){
      if(*(this->sources.rbegin())!='\n'){
	this->sources+='\n';
      }
    }
  } // end of MaterialPropertyDSL::appendToSources

  void
  MaterialPropertyDSL::addMaterialLaw(const std::string& m)
  {
    using namespace std;
    if(find(this->materialLaws.begin(),
	    this->materialLaws.end(),m)==this->materialLaws.end()){
      this->materialLaws.push_back(m);
    }
  } // end of MaterialPropertyDSL::addMaterialLaw

  MaterialPropertyDSL::~MaterialPropertyDSL()
  {} // end of MaterialPropertyDSL::~MaterialPropertyDSL

} // end of namespace mfront  

