/*!
 * \file   mfront/src/DSLBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iterator>
#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"TFEL/Math/IntegerEvaluator.hxx"

#include"MFront/MFront.hxx"
#include"MFront/DSLBase.hxx"
#include"MFront/SearchFile.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontMaterialPropertyInterface.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/MaterialPropertyDSL.hxx"

namespace mfront
{

  static bool
  isInteger(const std::string& s){
    using namespace std;
    bool b = true;
    for(auto p=s.begin();(p!=s.end()&&(b));++p){
      b = isdigit(*p)!=0;
    }
    return b;
  }

  DSLBase::VariableModifier::~VariableModifier()
  {} // end of DSLBase::VariableModifier::~VariableModifier

  DSLBase::WordAnalyser::~WordAnalyser()
  {} // end of DSLBase::WordAnalyser::~WordAnalyser

  DSLBase::CodeBlockParserOptions::CodeBlockParserOptions()
    : modifier(nullptr),
      analyser(nullptr),
      delim1("{"),
      delim2("}"),
      qualifyStaticVariables(false),
      qualifyMemberVariables(false),
      allowSemiColon(true),
      registerLine(true)
  {}

  DSLBase::CodeBlockParserOptions::~CodeBlockParserOptions() noexcept
  {} // end of DSLBase::CodeBlockParserOptions::~CodeBlockParserOptions()

  DSLBase::DSLBase()
	  : AbstractDSL()
  {
    this->reserveName("std",false);
    this->reserveName("tfel",false);
    this->reserveName("math",false);
    this->reserveName("material",false);
    this->reserveName("utilities",false);
    this->reserveName("real",false);
    this->reserveName("policy",false);
  }

  void
  DSLBase::openFile(const std::string& f,
		    const std::vector<std::string>& ecmds)
  {
    using namespace std;
    CxxTokenizer::openFile(f);
    for(const auto& c : ecmds){
      CxxTokenizer t;
      try{
	t.parseString(c);
      } catch(exception& e){
	string msg("DSLBase::openFile : ");
	msg += "error while parsing external command "
	  "'"+c+"' ('"+e.what()+"')";
	throw(runtime_error(msg));
      }
      this->fileTokens.insert(this->fileTokens.begin(),
			      t.begin(),t.end());
    }
  } // end of DSLBase::openFile

  const FileDescription&
  DSLBase::getFileDescription(void) const
  {
    return *this;
  } // end of DSLBase::getFileDescription

  const TargetsDescription&
  DSLBase::getTargetsDescription(void) const{
    return this->td;
  } // end of DSLBase::getTargetsDescription

  DSLBase::~DSLBase()
  {} // end of DSLBase::~DSLBase

  void
  DSLBase::readNextBlock(CodeBlock& res1,
			    CodeBlock& res2,
			    const CodeBlockParserOptions& o1,
			    const CodeBlockParserOptions& o2)
  {
    TokensContainer::const_iterator pb = this->current;
    res1 = this->readNextBlock(o1);
    this->current = pb;
    res2 = this->readNextBlock(o2);
  } // end of DSLBase::readNextBlock

  CodeBlock
  DSLBase::readNextBlock(const CodeBlockParserOptions& options)
  {
    using namespace std;
    using namespace tfel::utilities;
    const bool addThisPtr     = options.qualifyMemberVariables;
    const bool addClassName   = options.qualifyStaticVariables;
    const bool allowSemiColon = options.allowSemiColon;
    const bool registerLine   = options.registerLine;
    const auto& delim1 = options.delim1;
    const auto& delim2 = options.delim2;
    shared_ptr<VariableModifier> modifier = options.modifier;
    shared_ptr<WordAnalyser>     analyser = options.analyser;
    CodeBlock b;
    if(!this->currentComment.empty()){
      b.description += this->currentComment;
    }
    auto& res = b.code;
    unsigned int currentLine;
    unsigned int openedBlock;
    TokensContainer::const_iterator previous;
    openedBlock=0;
    this->readSpecifiedToken("DSLBase::readNextBlock",delim1);
    this->checkNotEndOfFile("DSLBase::readNextBlock",
			    "Expected a '"+delim2+"'.");
    if((this->current->value==";")&&(!allowSemiColon)){
      string msg("DSLBase::readNextBlock : ");
      msg += "read ';' before the end of block.\n";
      msg += "Number of block opened : ";
      msg += to_string(openedBlock);
      throw(runtime_error(msg));
    }
    if(this->current->value==delim1){
      ++openedBlock;
    }
    if(this->current->value==delim2){
      ++(this->current);
      return b;
    }
    currentLine = this->current->line;
    if((registerLine)&&(!getDebugMode())){
      res  = "#line ";
      res += to_string(currentLine);
      res += " \"";
      res += this->fileName;
      res += "\"\n";
    }
    if(!this->current->comment.empty()){
      if(!b.description.empty()){
	b.description += '\n';
      }
      b.description += this->current->comment;
    }
    if(analyser.get()!=nullptr){
      analyser->exe(this->current->value);
    }
    if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
      b.static_variables.insert(this->current->value);
      previous = this->current;
      --previous;
      if((previous->value!="->")&&
	 (previous->value!=".")&&
	 (previous->value!="::")){
	if(addClassName){
	  res += this->getClassName();
	  res += "::";
	}
      }
      res += this->current->value;
    } else if(this->varNames.find(this->current->value)!=this->varNames.end()){
      b.variables.insert(this->current->value);
      string currentValue;
      previous = this->current;
      --previous;
      if((previous->value=="->")||
	 (previous->value==".")||
	 (previous->value=="::")){
	currentValue = this->current->value;
      } else {
	if(modifier.get()!=nullptr){
	  currentValue = modifier->exe(this->current->value,addThisPtr);
	} else {
	  if(addThisPtr){
	    currentValue = "this->"+this->current->value;
	  } else {
	    currentValue = this->current->value;
	  }
	}
      }
      previous = this->current;
      --previous;
      if(previous->value=="*"){
	res += "("+currentValue+")";
      } else {
	res += currentValue;
      }
    } else {
      res += this->current->value;
    }
    res += " ";
    ++(this->current);
    while((this->current!=this->fileTokens.end())&&
	  (!((this->current->value==delim2)&&(openedBlock==0)))){
      if(currentLine!=this->current->line){
	currentLine = this->current->line;
	if((registerLine)&&(!getDebugMode())){
	  res += "\n";
	  res += "#line ";
	  res += to_string(currentLine);
	  res += " \"";
	  res += this->fileName;
	  res += "\"\n";
	}  else {
	  res += "\n";
	}
      }
      if((this->current->value==";")&&(!allowSemiColon)){
	string msg("DSLBase::readNextBlock : ");
	msg += "read ';' before the end of block.\n";
	msg += "Number of block opened : ";
	msg += to_string(openedBlock);
	throw(runtime_error(msg));
      }
      if(!this->current->comment.empty()){
	if(!b.description.empty()){
	  b.description += '\n';
	}
	b.description += this->current->comment;
      }
      if(analyser.get()!=nullptr){
	analyser->exe(this->current->value);
      }
      if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
	b.static_variables.insert(this->current->value);
	previous = this->current;
	--previous;
	if((previous->value!="->")&&
	   (previous->value!=".")&&
	   (previous->value!="::")){
	  if(addClassName){
	    res += this->getClassName();
	    res += "::";
	  }
	}
	res += this->current->value;
      } else if(this->varNames.find(this->current->value)!=this->varNames.end()){
	b.variables.insert(this->current->value);
	string currentValue;
	previous = this->current;
	--previous;
	if((previous->value=="->")||
	   (previous->value==".")||
	   (previous->value=="::")){
	  currentValue = this->current->value;
	} else {
	  if(modifier.get()!=nullptr){
	    currentValue = modifier->exe(this->current->value,addThisPtr);	    
	  } else {
	    if(addThisPtr){
	      if(previous->value=="*"){
		currentValue = "(this->"+this->current->value+')';
	      } else {
		currentValue = "this->"+this->current->value;
	      }
	    } else {
	      currentValue = this->current->value;
	    }
	  }
	}
	res += currentValue;
      } else {
	res += this->current->value;
      }
      res+=" ";
      if(this->current->value==delim1){
	++openedBlock;
      }
      if(this->current->value==delim2){
	--openedBlock;
      }
      ++(this->current);
    }
    if(this->current==this->fileTokens.end()){
      --(this->current);
      string msg("DSLBase::readNextBlock : ");
      msg += "Expected the end of a block.\n";
      msg += "Number of block opened : ";
      msg += to_string(openedBlock);
      throw(runtime_error(msg));
    }
    ++(this->current);
    return b;
  } // end of DSLBase::readNextBlock

  void
  DSLBase::throwRuntimeError(const std::string& method,
				const std::string& m) const
  {
    using namespace std;
    TokensContainer::const_iterator t = this->current;
    if(t==this->fileTokens.end()){
      --t;
    }
    string msg(method);
    if(!m.empty()){
      msg +=" : " + m;
    }
    if(!this->fileTokens.empty()){
      msg += "\nError at line " + to_string(t->line);
    }
    throw(runtime_error(msg));
  } // end of DSLBase::throwRuntimeError

  void DSLBase::treatImport()
  {
    using namespace std;
    const string m = "DSLBase::treatImport";
    const string oFileName = this->fileName;
    this->checkNotEndOfFile(m);
    const auto& files = this->readStringOrArrayOfString(m);
    this->checkNotEndOfFile(m);
    this->readSpecifiedToken(m,";");
    vector<string>::const_iterator p;
    TokensContainer oFileTokens;
    oFileTokens.swap(this->fileTokens);
    TokensContainer::const_iterator ocurrent = this->current;
    for(p=files.begin();p!=files.end();++p){
      this->importFile(SearchFile::search(*p),
		       vector<string>());
    }
    this->fileName = oFileName;
    this->fileTokens.swap(oFileTokens);
    this->current = ocurrent;
  }

  void
  DSLBase::checkNotEndOfFile(const std::string& method,
				const std::string& error) const{
    using namespace std;
    if(this->current==this->fileTokens.end()){
      TokensContainer::const_iterator previous = this->current;
      --previous;
      string msg(method+" : ");
      msg += "unexpected end of file.";
      if(!error.empty()){
	msg += "\n"+error;
      }
      if(!this->fileTokens.empty()){
	msg += "\nError at line " + to_string(this->current->line);
      }
      throw(runtime_error(msg));
    }
  } // end of DSLBase::checkNotEndOfFile

  unsigned short
  DSLBase::readUnsignedShort(const std::string& m)
  {
    using namespace std;
    this->checkNotEndOfFile(m,"Cannot read unsigned short value.");
    unsigned short value;
    istringstream flux(current->value);
    flux >> value;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError(m,"Failed to read unsigned short value.");
    }
    ++(this->current);
    return value;
  } // end of DSLBase::readUnsignedShort

  void
  DSLBase::readSpecifiedToken(const std::string& method,
				 const std::string& token)
  {
    using namespace std;
    this->checkNotEndOfFile(method,"Expected '"+token+"'.");
    if(this->current->value!=token){
      string msg(method+" : ");
      msg += "expected '"+token+"', read ";
      msg += this->current->value;
      msg += ".\n";
      msg += "Error at line : ";
      msg += to_string(this->current->line);
      throw(runtime_error(msg));
    }
    ++(this->current);
  } // end of DSLBase::readSpecifiedToken

  std::string
  DSLBase::readUntilEndOfInstruction(void)
  {
    using namespace std;
    string res;
    while((this->current!=this->fileTokens.end())&&
	  (this->current->value != ";")){
      if(!this->current->value.empty()){
	if(this->current->value[0]=='@'){
	  string msg("DSLBase::readUntilEndOfInstruction : ");
	  msg += "no word beginning with '@' are allowed here";
	  throw(runtime_error(msg));
	}
	res+=this->current->value;
	res+=" ";
      }
      ++(this->current);
    }
    this->checkNotEndOfFile("DSLBase::readUntilEndOfInstruction",
			    "Missing ';' delimiter.");
    if(!res.empty()){
      res.erase(res.length()-1);
    }
    ++(this->current);
    return res;
  }

  std::string
  DSLBase::readOnlyOneToken(void)
  {
    using namespace std;
    string res;
    this->checkNotEndOfFile("DSLBase::readOnlyOneToken",
			    "Expected a word.");
    if(this->current->value==";"){
      string msg("DSLBase::readOnlyOneToken : ");
      msg += "No word read";
      throw(runtime_error(msg));
    }
    res = this->current->value;
    ++(this->current);
    this->readSpecifiedToken("DSLBase::readOnlyOneToken",";");
    return res;
  } // end of DSLBase::readOnlyOneToken

  void
  DSLBase::treatIntegerConstant()
  {
    using namespace std;
    string name;
    pair<bool,int> value;
    this->checkNotEndOfFile("DSLBase::treatIntegerConstant",
			    "Cannot read type of static variable.");
    name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("DSLBase::treatIntegerConstant",
			      "Variable name '"+name+"' is not valid.");
    }
    ++(this->current);
    value = this->readInitialisationValue<int>(name,true);
    this->readSpecifiedToken("DSLBase::treatIntegerConstant",";");
    this->registerStaticVariable(name);
    if(!this->integerConstants.insert({name,value.second}).second){
      this->throwRuntimeError("DSLBase::treatIntegerConstant",
			      "variable '"+name+"' already declared");
    }
  } // end of DSLBase::treatIntegerConstant

  void DSLBase::readVarList(VariableDescriptionContainer& cont,
			       const std::string& type,
			       const bool allowArray,
			       const bool addIncrementVar,
			       const bool b)
  {
    using namespace std;
    using namespace tfel::math;
    using namespace tfel::utilities;
    string endComment;
    auto endOfTreatment=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatment)){
      const auto& varName = this->current->value;
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("DSLBase::readVarList : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      auto lineNumber = this->current->line;
      unsigned int asize = 1u;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readVarList");
      if(this->current->value=="["){
	if(!allowArray){
	  this->throwRuntimeError("DSLBase::readVarList : ",
				  "variable '"+varName+"' can't be declared an array");
	}
	string array_size;
	++(this->current);
	this->checkNotEndOfFile("DSLBase::readVarList");
	while(this->current->value!="]"){
	  if((this->current->flag!=Token::Standard)||
	     (this->current->value==";")){
	    this->throwRuntimeError("DSLBase::readVarList : ",
				    "invalid array size for '"+varName+"'");
	  }
	  array_size += this->current->value;
	  ++(this->current);
	  this->checkNotEndOfFile("DSLBase::readVarList");
	}
	if(array_size.empty()){
	  this->throwRuntimeError("DSLBase::readVarList : ",
				  "empty array size for '"+varName+"'");
	}
	IntegerEvaluator ev(array_size);
	const auto& vars = ev.getVariablesNames();
	vector<string>::const_iterator pv;
	for(pv=vars.begin();pv!=vars.end();++pv){
	  map<string,int>::const_iterator pvv = this->integerConstants.find(*pv);
	  if(pvv==this->integerConstants.end()){
	    this->throwRuntimeError("DSLBase::readVarList : ",
				    "unknown constant '"+*pv+"'");
	  }
	  ev.setVariableValue(*pv,pvv->second);
	}
	const auto iv = ev.getValue();
	if(iv<=0){
	  this->throwRuntimeError("DSLBase::readVarList : ",
				  "invalid array size for '"+varName+"'");
	}
	asize = static_cast<unsigned int>(iv);
	this->readSpecifiedToken("DSLBase::readVarList","]");
	this->checkNotEndOfFile("DSLBase::readVarList");
      }
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatment=true;
	endComment = this->current->comment;
	++(this->current);
      } else {
	this->throwRuntimeError("DSLBase::readVarList",
				", or ; expected afer '"+varName+"'");
      }
      this->registerVariable(varName,b);
      cont.push_back(VariableDescription(type,varName,asize,lineNumber));
      if(!this->currentComment.empty()){
	cont.back().description = this->currentComment;
      }
      if(addIncrementVar){
	string incrVarName("d");
	incrVarName += varName;	
	this->registerVariable(incrVarName,b);
      }
    }
    if(!endComment.empty()){
      for(auto & elem : cont){
	if(!elem.description.empty()){
	  elem.description += ' ';
	}
	elem.description += endComment;
      }
    }
    if(!endOfTreatment){
      --(this->current);
      this->throwRuntimeError("DSLBase::readVarList",
			      "Expected ';' before end of file");
    }
  }

  void DSLBase::readVarList(VariableDescriptionContainer& cont,
			       const bool allowArray,
			       const bool addIncrementVar,
			       const bool b)
  {
    this->checkNotEndOfFile("DSLBase::readVarList",
			    "Cannot read type of varName.\n");
    auto type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("DSLBase::readVarList",
			      "given type "+type+"is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::readVarList");
    while(this->current->value=="::"){
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readVarList");
      const auto t = this->current->value;
      if(!isValidIdentifier(t,false)){
	--(this->current);
	this->throwRuntimeError("DSLBase::readVarList",
				"given type '"+t+"' is not valid.");
      }
      type += "::"+t;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readVarList");
    }
    if(this->current->value=="<"){
      unsigned short openBrackets = 1u;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::readVarList");
      type += "<";
      while(openBrackets!=0){
	const auto t = this->current->value;
	if((!isValidIdentifier(t,false))&&
	   (!isInteger(t))){
	  this->throwRuntimeError("DSLBase::readVarList",
				  "given type '"+t+"' is not valid.");
	}
	++(this->current);
	this->checkNotEndOfFile("DSLBase::readVarList");
	type += t;
	if(this->current->value==","){
	  ++(this->current);
	  this->checkNotEndOfFile("DSLBase::readVarList");
	  const auto t2 = this->current->value;
	  if((!isValidIdentifier(t2,false))&&
	     (!isInteger(t2))){
	    --(this->current);
	    this->throwRuntimeError("DSLBase::readVarList",
				    "given type '"+t+"' is not valid.");
	  }
	  type+=",";
	} else if(this->current->value=="<"){
	  if(isInteger(t)){
	    this->throwRuntimeError("DSLBase::readVarList",
				    "given type '"+t+"'is not valid.");
	  }
	  ++openBrackets;
	  ++(this->current);
	  this->checkNotEndOfFile("DSLBase::readVarList");
	  type += "<";      
	} else if(this->current->value==">"){
	  --openBrackets;
	  ++(this->current);
	  this->checkNotEndOfFile("DSLBase::readVarList");
	  type += ">";      
	}
      }
    }
    this->readVarList(cont,type,allowArray,addIncrementVar,b);
  } // end of DSLBase::readVarList

  void
  DSLBase::readList(std::vector<tfel::utilities::Token>& l,
		       const std::string& m,
		       const std::string& db,
		       const std::string& de,
		       const bool b)
  {
    using namespace std;
    l.clear();
    if(this->current==this->fileTokens.end()){
      if(b){
	return;
      }
    }
    this->checkNotEndOfFile(m,"Expected '"+db+"'");
    if(this->current->value!=db){
      return;
    }
    this->readSpecifiedToken(m,db);
    this->checkNotEndOfFile(m,"Expected '"+de+"'");
    while(this->current->value!=de){
      l.push_back(*(this->current));
      ++(this->current);
      this->checkNotEndOfFile(m,"Expected '"+de+"'");
      if(!((this->current->value==de)||
	   (this->current->value==","))){
	this->throwRuntimeError(m,"Expected ',' or '"+de+"',"
				" read '"+this->current->value+"'");
      }
      if(this->current->value==","){
	++(this->current);
	this->checkNotEndOfFile(m,"Expected '"+de+"'");
	if(this->current->value==de){
	  this->throwRuntimeError(m,"Expected a new item");
	}
      }
    }
    ++(this->current);
  } // end of DSLBase::readList  

  std::vector<std::string>
  DSLBase::readArrayOfString(const std::string& m)
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    vector<Token> tokens;
    this->readList(tokens,m,"{","}",false);
    for(vector<Token>::const_iterator p=tokens.begin();p!=tokens.end();++p){
      if(p->flag!=Token::String){
	this->throwRuntimeError(m,"Expected a string");
      }
      res.push_back(p->value.substr(1,p->value.size()-2));
    }
    return res;
  } // end of DSLBase::readArrayOfString

  bool
  DSLBase::readBooleanValue(const std::string& m)
  {
    bool b = false;
    this->checkNotEndOfFile(m,"Expected a boolean value");
    if(this->current->value=="true"){
      b = true;
    } else if(this->current->value=="false"){
      b = false;
    } else {
      this->throwRuntimeError(m,"Expected to read 'true' or 'false' "
			      "(read '"+this->current->value+"')");
    }
    ++(this->current);
    return b;
  } // end of DSLBase::readBooleanValue

  std::string
  DSLBase::readString(const std::string& m)
  {
    using namespace std;
    using namespace tfel::utilities;
    this->checkNotEndOfFile(m,"Expected a string or '{'");
    if(this->current->flag!=Token::String){
      this->throwRuntimeError(m,"Expected a string");
    }
    const auto& r = this->current->value.substr(1,this->current->value.size()-2);
    ++(this->current);
    return r;
  } // end of DSLBase::readString

  std::vector<std::string>
  DSLBase::readStringOrArrayOfString(const std::string& m)
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    this->checkNotEndOfFile(m,"Expected a string or '{'");
    if(this->current->value=="{"){
      return this->readArrayOfString(m);
    }
    res.push_back(this->readString(m));
    return res;
  } // end of DSLBase::readStringOrArrayOfString

  void
  DSLBase::treatLink(void){
    using namespace std;
    vector<string> nlink;
    vector<string>::const_iterator p;
    nlink = readStringOrArrayOfString("DSLBase::treatLink");
    this->readSpecifiedToken("DSLBase::treatLink",";");
    for(p=nlink.begin();p!=nlink.end();++p){
      if(find(this->librariesDependencies.begin(),
	      this->librariesDependencies.end(),*p)==this->librariesDependencies.end()){
	this->librariesDependencies.push_back(*p);
      }
    }
  } // end of DSLBase::treatLink

  void
  DSLBase::callMFront(const std::vector<std::string>& interfaces,
		      const std::vector<std::string>& files)
  {
    MFront m;
    for(const auto& i : interfaces){
      m.setInterface(i);
    }
    for(const auto& f : files){
      mergeTargetsDescription(this->td,m.treatFile(f),true);
    }
  } // end of DSLBase::callMFront

  void DSLBase::treatMFront(void){
    using namespace std;
    vector<string> vfiles;
    vector<string> vinterfaces;
    this->readSpecifiedToken("DSLBase::treatMfront","{");
    vfiles = this->readStringOrArrayOfString("DSLBase::treatMfront");
    this->checkNotEndOfFile("DSLBase::treatMfront","Expected '}'");
    if(!((this->current->value=="}")||
	 (this->current->value==","))){
      this->throwRuntimeError("DSLBase::treatMfront",
			      "Expected ',' or '}', read '"+this->current->value+"'");
    }
    if(this->current->value==","){
      ++(this->current);
     vinterfaces = this->readStringOrArrayOfString("DSLBase::treatMfront");
    }
    this->readSpecifiedToken("DSLBase::treatMfront","}");
    this->readSpecifiedToken("DSLBase::treatMfront",";");
    this->callMFront(vinterfaces,vfiles);
  } // end of DSLBase::treatMfront

  std::string
  DSLBase::readSpecifiedValue(const std::string& file,
				 const std::string& value)
  {
    using namespace std;
    vector<string> values(1,value);
    return this->readSpecifiedValues(file,values)[1];
  } // end of DSLBase::readSpecifiedValue

  std::vector<std::string>
  DSLBase::readSpecifiedValues(const std::string& file,
				  const std::string& value1,
				  const std::string& value2)
  {
    using namespace std;
    vector<string> values(2);
    values[0] = value1;
    values[1] = value2;
    return this->readSpecifiedValues(file,values);
  } // end of DSLBase::readSpecifiedValues

  std::vector<std::string>
  DSLBase::readSpecifiedValues(const std::string& file,
				  const std::vector<std::string>& values)
  {
    using namespace std;
    using namespace tfel::utilities;
    CxxTokenizer cfile;
    vector<string> res(values.size());
    string value;
    CxxTokenizer::TokensContainer::const_iterator pt;
    CxxTokenizer::TokensContainer::const_iterator pte;
    std::vector<std::string>::const_iterator p;
    cfile.openFile(file);
    cfile.stripComments();
    pt  = cfile.begin();
    pte = cfile.end();
    while(pt!=pte){
      if((p=find(values.begin(),values.end(),pt->value))!=values.end()){
	if(pt!=cfile.begin()){
	  CxxTokenizer::TokensContainer::const_iterator ptp = pt;
	  --ptp;
	  if(ptp->value!=";"){
	    string msg("DSLBase::readSpecifiedValues : ");
	    msg += "the keyword @Parser does not begin a new instruction.";
	    throw(runtime_error(msg));
	  }
	}
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "DSLBase::readSpecifiedValues : ";
	  msg << "unexepected end of file '"+file+"' (exepected "+*p+").\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value==";"){
	  ostringstream msg;
	  msg << "DSLBase::readSpecifiedValues : ";
	  msg << "unexepected end of line '"+file+"' (exepected "+*p+").\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	value = pt->value;
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "DSLBase::readSpecifiedValues : ";
	  msg << "unexepected end of file '"+file+"' (exepected library name or ';').\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value!=";"){
	  ostringstream msg;
	  msg << "DSLBase::readSpecifiedValues : ";
	  msg << "unexepected token '" << pt->value << "' (exepected ';').\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	res[static_cast<vector<string>::size_type>(p-values.begin())]=value;
      }
      ++pt;
    }
    return res;
  } // end of DSLBase::readSpecifiedValues

  void
  DSLBase::registerVariable(const std::string& v,
			       const bool b)
  {
    using namespace std;
    if(!CxxTokenizer::isValidIdentifier(v)){
      string msg("DSLBase::registerVariable : ");
      msg += "variable name '"+v+"' is invalid";
      throw(runtime_error(msg));
    }
    if(b){
      this->varNames.insert(v);
    } else {
      if(!this->varNames.insert(v).second){
	string msg("DSLBase::registerVariable : ");
	msg += "variable '"+v+"' already registred";
	throw(runtime_error(msg));
      }
    }
    this->reserveName(v,b);
  } // end of DSLBase::registerVariable

  void
  DSLBase::registerStaticVariable(const std::string& v)
  {
    using namespace std;
    if(!CxxTokenizer::isValidIdentifier(v)){
      string msg("DSLBase::registerStaticVariable : ");
      msg += "variable name '"+v+"' is invalid";
      throw(runtime_error(msg));
    }
    if(!this->staticVarNames.insert(v).second){
      string msg("DSLBase::registerStaticVariable : ");
      msg += "static variable '"+v+"' already registred";
      throw(runtime_error(msg));
    }
    this->reserveName(v,false);
  } // end of DSLBase::registerStaticVariable

  void
  DSLBase::reserveName(const std::string& w,
			  const bool b)
  {
    using namespace std;
    if(b){
      this->reservedNames.insert(w);
    } else {
      if(!this->reservedNames.insert(w).second){
	string msg("DSLBase::reserveName : ");
	msg += "name '"+w+"' already reserved";
	throw(runtime_error(msg));
      }
    }
  } // end of DSLBase::reserveName

  bool
  DSLBase::isNameReserved(const std::string& n) const
  {
    return this->reservedNames.find(n)!=this->reservedNames.end();
  } // end of DSLBase::isNameReserved

  MaterialPropertyDescription
  DSLBase::handleMaterialLaw(const std::string& f)
  {
    using namespace std;
    // getting informations the source files
    MaterialPropertyDSL mp;
    try{
      MFrontMaterialPropertyInterface minterface;
      const auto& path = SearchFile::search(f);
      mp.analyseFile(path);
      const auto& mpd = mp.getMaterialPropertyDescription();
      const auto& mname = minterface.getFunctionName(mpd.material,
						       mpd.law);
      this->reserveName(mname,false);
      this->appendToIncludes("#include\""+minterface.getHeaderFileName(mpd.material,
								       mpd.law)+".hxx\"");
      this->addMaterialLaw(mname);
      // generating the source files and adds them to the various
      // files used to generate the final Makefile
      this->callMFront(vector<string>(1u,"mfront"),
		       vector<string>(1u,path));
    } catch(exception& e){
      string msg("Error while treating file '"+f+"'\n");
      msg += e.what();
      msg += "\n\n";
      throw(runtime_error(msg));
    } catch(...){
      string msg("Error while treating file '"+f+"'\n");
      msg += "Unknown exception\n\n";
      throw(runtime_error(msg));
    }
    if(find(this->librariesDependencies.begin(),
	    this->librariesDependencies.end(),"-lMFrontMaterialLaw")==this->librariesDependencies.end()){
      this->librariesDependencies.push_back("-lMFrontMaterialLaw");
    }
    return mp.getMaterialPropertyDescription();
  } // end of DSLBase::handleMaterialLaw

  void
  DSLBase::treatMaterialLaw(void){
    const auto vfiles = this->readStringOrArrayOfString("DSLBase::treatMfront");
    this->readSpecifiedToken("DSLBase::treatMfront",";");
    for(const auto& f : vfiles){
      this->handleMaterialLaw(f);
    }
  } // end of DSLBase::treatMaterialLaw

  void
  DSLBase::treatDescription(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    unsigned int currentLine;
    unsigned int openedBrackets;
    this->readSpecifiedToken("DSLBase::treatDescription","{");
    this->checkNotEndOfFile("DSLBase::treatDescription");
    currentLine = this->current->line;
    this->description += "* ";
    openedBrackets = 1u;
    while((this->current!=this->fileTokens.end())&&
	  (!((this->current->value=="}")&&
	     (openedBrackets==1u)))){
      if(this->current->value=="{"){
	TokensContainer::const_iterator previous = this->current;
	--previous;
	if((previous->value.size()>0)&&
	   (previous->value[previous->value.size()-1]!='\\')){
	  ++openedBrackets;
	}
      }
      if(this->current->value=="}"){
	TokensContainer::const_iterator previous = this->current;
	--previous;
	if((previous->value.size()>0)&&
	   (previous->value[previous->value.size()-1]!='\\')){
	  --openedBrackets;
	}
      }
      if(currentLine!=this->current->line){
	while(currentLine!=this->current->line){
	  this->description+="\n* ";
	  ++currentLine;
	}
      }
      if(this->current->flag==Token::String){
	this->description+=this->current->value.substr(1,this->current->value.size()-2u);
      } else {
	this->description+=this->current->value;
      }
      this->description+=" ";
      ++(this->current);
    }
    if(this->current==this->fileTokens.end()){
      --(this->current);
      this->throwRuntimeError("DSLBase::treatDescription",
			      "File ended before the end of description.");
    }
    ++(this->current);
  } // end of DSLBase::treatDescription

  void DSLBase::treatAuthor(void)
  {
    this->authorName = this->readUntilEndOfInstruction();
  } // end of DSLBase::treatAuthor

  void DSLBase::treatDate(void)
  {
    this->date = this->readUntilEndOfInstruction();
  } // end of DSLBase::treatDate

  void DSLBase::treatUnknownKeyword(void)
  {
    using namespace std;
    --(this->current);
    this->throwRuntimeError("DSLBase::treatUnknownKeyword",
			    "unknown keyword (read '"+this->current->value+"')");
  } // end of DSLBase::treatUnknownKeyword

  void DSLBase::treatIncludes(void)
  {
    CodeBlockParserOptions options;
    this->appendToIncludes(this->readNextBlock(options).code);
  }

  void
  DSLBase::treatSources(void)
  {
    CodeBlockParserOptions options;
    this->appendToSources(this->readNextBlock(options).code);
  } // end of DSLBase::treatSources(void)

  void
  DSLBase::treatMembers(void)
  {
    CodeBlockParserOptions options;
    options.qualifyStaticVariables = true;
    options.qualifyMemberVariables = true;
    this->appendToMembers(this->readNextBlock(options).code);
  }

  void
  DSLBase::treatPrivate(void)
  {
    CodeBlockParserOptions options;
    options.qualifyStaticVariables = true;
    options.qualifyMemberVariables = true;
    this->appendToPrivateCode(this->readNextBlock(options).code);
  } // end of DSLBase::treatSources(void)

  void
  DSLBase::treatParser(void)
  {
    this->readUntilEndOfInstruction();
  } // end of DSLBase::treatParser

  void
  DSLBase::treatStaticVar(void)
  {
    using namespace std;
    string type;
    string name;
    unsigned int line;
    pair<bool,long double> value;
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
			    "Cannot read type of static variable.");
    type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("DSLBase::treatStaticVar",
			      "type given is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
			    "Cannot read variable name.");
    name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("DSLBase::treatStaticVar",
			      "Variable name '"+name+"' is not valid.");
    }
    line = this->current->line;
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
			    "Expected to read value of variable.");
    value = this->readInitialisationValue<long double>(name,true);
    this->readSpecifiedToken("DSLBase::treatStaticVar",";");
    this->registerStaticVariable(name);
    this->addStaticVariableDescription(StaticVariableDescription(type,name,line,value.second));
  } // end of DSLBase::treatStaticVar

  void
  DSLBase::ignoreKeyWord(const std::string& key)
  {
    using namespace std;
    this->checkNotEndOfFile("DSLBase::ignoreKeyWord",
			    "error while treating keyword '"+key+"' ");
    while((this->current->value!="{")&&(this->current->value!=";")){
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::ignoreKeyWord",
			      "error while treating keyword '"+key+"' ");
    }
    if(this->current->value=="{"){
      unsigned short openedBrackets = 1;
      while(!((this->current->value=="}")&&
	      (openedBrackets==0))){
	++(this->current);
	this->checkNotEndOfFile("DSLBase::ignoreKeyWord",
				"error while treating keyword '"+key+"' ");
	if(this->current->value=="{"){
	  ++openedBrackets;
	}
	if(this->current->value=="}"){
	  --openedBrackets;
	}
      }
      TokensContainer::const_iterator next = this->current;
      ++next;
      if(next!=this->fileTokens.end()){
	if(next->value==";"){
	  current = next;
	}
      }
    }
    ++(this->current);
  } // end of DSLBase::ignoreKeyWord

  double
  DSLBase::readDouble(void)
  {
    using namespace std;
    double value;
    this->checkNotEndOfFile("DSLBase::readDouble");
    istringstream flux(this->current->value);
    flux >> value;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("DSLBase::readDouble",
			      "Failed to read double value from token '"+
			      this->current->value+"'.");
    }
    ++(this->current);
    return value;
  } // end of DSLBase::readDouble

  void
  DSLBase::handleParameter(VariableDescriptionContainer& c,
			      std::map<std::string,double>& v)
  {
    using namespace std;
    bool endOfTreatment=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatment)){
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("DSLBase::handleParameter : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      const string n = this->current->value;
      const auto lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::handleParameter");
      if((this->current->value=="=")||
	 (this->current->value=="{")||
	 (this->current->value=="(")){
	string ci; // closing initializer
	if(this->current->value=="{"){
	  ci="}";
	}
	if(this->current->value=="("){
	  ci=")";
	}
	++(this->current);
	this->checkNotEndOfFile("DSLBase::handleParameter");
	istringstream converter(this->current->value);
	double value;
	converter >> value;
	if(!converter||(!converter.eof())){
	  this->throwRuntimeError("DSLBase::handleParameter",
				  "could not read default value for parameter '"+n+"'");
	}
	++(this->current);
	this->checkNotEndOfFile("DSLBase::handleParameter");
	if(!v.insert({n,value}).second){
	  this->throwRuntimeError("DSLBase::handleParameter",
				  "default value already defined for parameter '"+n+"'");
	}
	if(!ci.empty()){
	  this->readSpecifiedToken("DSLBase::handleParameter",ci);
	}
      }
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatment=true;
	++(this->current);
      } else {
	this->throwRuntimeError("DSLBase::handleParameter",
				", or ; expected afer '"+n+"'");
      }
      this->registerVariable(n,false);
      c.push_back(VariableDescription("real",n,1u,lineNumber));
    }
    if(!endOfTreatment){
      --(this->current);
      this->throwRuntimeError("DSLBase::handleParameter",
			      "Expected ';' before end of file");
    }
  } // end of DSLBase::handleParameter

  

} // end of namespace mfront
