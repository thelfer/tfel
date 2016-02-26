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
#include"MFront/SupportedTypes.hxx"
#include"MFront/DSLBase.hxx"
#include"MFront/SearchFile.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontMaterialPropertyInterface.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/MaterialPropertyDSL.hxx"

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

namespace mfront
{

  static bool
  isInteger(const std::string& s){
    for(const auto c : s){
      if(!std::isdigit(c)){return false;}
    }
    return true;
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
  {}

  std::vector<std::string>
  DSLBase::getDefaultReservedNames(){
    auto names = std::vector<std::string>{};
    // names of the c++ standard
    names.insert(names.end(),{"std","cout","cerr","endl",
	  "cos","sin","tan","acos","asin","atan","atan2",
	  "cosh","sinh","tanh","acosh","asinh","atanh",
	  "exp","frexp","ldexp","log","log10","modf",
	  "exp2","expm1","ilogb","log1p","log2",
	  "logb","scalbn","scalbln","pow","sqrt",
	  "cbrt","hypot","erf","erfc","tgamma","lgamma","abs"});
    // tfel namespaces
    names.insert(names.end(),{"tfel","math","material","utilities",
	  "exception","glossary"});
    for(const auto& v : SupportedTypes::getTypeFlags()){
      names.push_back(v.first);
    }
    names.push_back("policy");
    return names;
  }
  
  void
  DSLBase::openFile(const std::string& f,
		    const std::vector<std::string>& ecmds)
  {
    CxxTokenizer::openFile(f);
    for(const auto& c : ecmds){
      CxxTokenizer t;
      try{
	t.parseString(c);
      } catch(std::exception& e){
	throw(std::runtime_error("DSLBase::openFile : "
				 "error while parsing external command "
				 "'"+c+"'\n"+std::string(e.what())));
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
    auto pb = this->current;
    res1 = this->readNextBlock(o1);
    this->current = pb;
    res2 = this->readNextBlock(o2);
  } // end of DSLBase::readNextBlock

  CodeBlock
  DSLBase::readNextBlock(const CodeBlockParserOptions& options)
  {
    using namespace std;
    using namespace tfel::utilities;
    const auto& smn = options.smn;
    const auto& mn  = options.mn;
    const auto& delim1 = options.delim1;
    const auto& delim2 = options.delim2;
    const auto addThisPtr     = options.qualifyMemberVariables;
    const auto addClassName   = options.qualifyStaticVariables;
    const auto allowSemiColon = options.allowSemiColon;
    const auto registerLine   = options.registerLine;
    auto modifier = options.modifier;
    auto analyser = options.analyser;
    CodeBlock b;
    if(!this->currentComment.empty()){
      b.description += this->currentComment;
    }
    auto& res = b.code;
    unsigned int openedBlock =0;
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
    auto currentLine = this->current->line;
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
    if(smn.find(this->current->value)!=smn.end()){
      b.staticMembers.insert(this->current->value);
      auto previous = this->current;
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
    } else if(mn.find(this->current->value)!=mn.end()){
      b.members.insert(this->current->value);
      string currentValue;
      auto previous = this->current;
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
      if(smn.find(this->current->value)!=smn.end()){
	b.staticMembers.insert(this->current->value);
	auto previous = this->current;
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
      } else if(mn.find(this->current->value)!=mn.end()){
	b.members.insert(this->current->value);
	string currentValue;
	auto previous = this->current;
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
    auto msg = method;
    if(!m.empty()){
      msg +=": " + m;
    }
    if(!this->fileTokens.empty()){
      auto t = this->current;
      if(t==this->fileTokens.end()){
	--t;
      }
      msg += "\nError at line " + std::to_string(t->line);
    }
    throw(std::runtime_error(msg));
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
    TokensContainer oFileTokens;
    oFileTokens.swap(this->fileTokens);
    TokensContainer::const_iterator ocurrent = this->current;
    for(const auto& f : files){
      this->importFile(SearchFile::search(f),
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
      auto previous = this->current;
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
    this->checkNotEndOfFile(m,"Cannot read unsigned short value.");
    unsigned short value;
    std::istringstream flux(current->value);
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
    this->checkNotEndOfFile("DSLBase::treatIntegerConstant",
			    "Cannot read type of static variable.");
    const auto name = this->current->value;
    const auto line = this->current->line;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("DSLBase::treatIntegerConstant",
			      "Variable name '"+name+"' is not valid.");
    }
    ++(this->current);
    const auto value = this->readInitialisationValue<int>(name,true);
    this->readSpecifiedToken("DSLBase::treatIntegerConstant",";");
    this->addStaticVariableDescription(StaticVariableDescription("int",name,line,
								 value.second));
    if(!this->integerConstants.insert({name,value.second}).second){
      this->throwRuntimeError("DSLBase::treatIntegerConstant",
			      "variable '"+name+"' already declared");
    }
  } // end of DSLBase::treatIntegerConstant

  void DSLBase::readVarList(VariableDescriptionContainer& cont,
			    const std::string& type,
			    const bool allowArray)
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
	for(auto pv=vars.begin();pv!=vars.end();++pv){
	  auto pvv = this->integerConstants.find(*pv);
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
      cont.push_back(VariableDescription(type,varName,asize,lineNumber));
      if(!this->currentComment.empty()){
	cont.back().description = this->currentComment;
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
			    const bool allowArray)
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
    this->readVarList(cont,type,allowArray);
  } // end of DSLBase::readVarList

  void
  DSLBase::readList(std::vector<tfel::utilities::Token>& l,
		    const std::string& m,
		    const std::string& db,
		    const std::string& de,
		    const bool b)
  {
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
    using namespace tfel::utilities;
    std::vector<std::string> res;
    std::vector<Token> tokens;
    this->readList(tokens,m,"{","}",false);
    for(const auto& t : tokens){
      if(t.flag!=Token::String){
	this->throwRuntimeError(m,"Expected a string");
      }
      res.push_back(t.value.substr(1,t.value.size()-2));
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
    std::vector<std::string> res;
    this->checkNotEndOfFile(m,"Expected a string or '{'");
    if(this->current->value=="{"){
      return this->readArrayOfString(m);
    }
    res.push_back(this->readString(m));
    return res;
  } // end of DSLBase::readStringOrArrayOfString

  void
  DSLBase::treatLink(void){
    const auto nlink = readStringOrArrayOfString("DSLBase::treatLink");
    this->readSpecifiedToken("DSLBase::treatLink",";");
#pragma message("DSLBase::treatLink: unimplemented feature")
    throw(std::runtime_error("DSLBase::treatLink: "
			     "unimplemented feature"));
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
      mergeTargetsDescription(this->td,m.treatFile(f),false);
    }
  } // end of DSLBase::callMFront

  void DSLBase::treatMFront(void){
    this->readSpecifiedToken("DSLBase::treatMfront","{");
    const auto vfiles = this->readStringOrArrayOfString("DSLBase::treatMfront");
    auto vinterfaces = std::vector<std::string>{};
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
    std::vector<std::string> values(1,value);
    return this->readSpecifiedValues(file,values)[1];
  } // end of DSLBase::readSpecifiedValue

  std::vector<std::string>
  DSLBase::readSpecifiedValues(const std::string& file,
			       const std::string& value1,
			       const std::string& value2)
  {
    std::vector<std::string> values(2);
    values[0] = value1;
    values[1] = value2;
    return this->readSpecifiedValues(file,values);
  } // end of DSLBase::readSpecifiedValues

  std::vector<std::string>
  DSLBase::readSpecifiedValues(const std::string& file,
			       const std::vector<std::string>& values)
  {
    auto throw_if = [](const bool b,const std::string& m,
		       const unsigned int l){
      if(b){throw(std::runtime_error("DSLBase::readSpecifiedValues : "+m+"\n"
				     "Error at line "+std::to_string(l)));}
    };
    tfel::utilities::CxxTokenizer cfile;
    auto res = std::vector<std::string>(values.size());
    cfile.openFile(file);
    cfile.stripComments();
    auto pt  = cfile.begin();
    const auto pte = cfile.end();
    while(pt!=pte){
      const auto p=find(values.begin(),values.end(),pt->value);
      if(p!=values.end()){
	if(pt!=cfile.begin()){
	  auto ptp = pt;
	  --ptp;
	  throw_if(ptp->value!=";","the keyword '"+*p+"' does not "
		   "begin a new instruction.",pt->line);
	}
	throw_if(++pt==pte,"unexepected end of file '"+file+
		 "' (expected "+*p+").\n",(--pt)->line);
	const auto value = pt->value;
	throw_if(pt->value==";","unexepected token ';' (exepected "+*p+")", pt->line);
	throw_if(++pt==pte,"unexepected end of file '"+file+
		 "' (expected ';').\n",(--pt)->line);
	throw_if(pt->value!=";","unexepected token '"+pt->value+"' (exepected ';')",
		 pt->line);
	res[static_cast<std::vector<std::string>::size_type>(p-values.begin())]=value;
      }
      ++pt;
    }
    return res;
  } // end of DSLBase::readSpecifiedValues

    /*!
   * \return the material property description generated from a list of mfront files
   * \param[in] files: list of files
   */
  std::vector<std::shared_ptr<MaterialPropertyDescription>>
  DSLBase::getMaterialPropertiesDescriptions(const std::vector<std::string>& files)
  {
    auto mpds = std::vector<std::shared_ptr<MaterialPropertyDescription>>{};
    for(const auto & file : files){
      const auto& f = SearchFile::search(file);
      mpds.push_back(std::make_shared<MaterialPropertyDescription>(this->handleMaterialLaw(f)));
    }
    return mpds;
  }
  
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
      this->reserveName(mname);
      this->appendToIncludes("#include\""+minterface.getHeaderFileName(mpd.material,
								       mpd.law)+".hxx\"");
      this->addMaterialLaw(mname);
      MFront m;
      m.setInterface("mfront");
      const auto t = m.treatFile(path);
      if(!t.specific_targets.empty()){
	throw(std::runtime_error("DSLBase::handleMaterialLaw: "
				 "error while treating file '"+f+"'\n"
				 "No specific targets are not supported"));
      }
      this->atds.push_back(t);
    } catch(exception& e){
      throw(std::runtime_error("DSLBase::handleMaterialLaw: "
			       "error while treating file '"+f+"'\n"+
			       std::string(e.what())));
    } catch(...){
      throw(std::runtime_error("DSLBase::handleMaterialLaw: "
			       "error while treating file '"+f+"'"));
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
    this->readSpecifiedToken("DSLBase::treatDescription","{");
    this->checkNotEndOfFile("DSLBase::treatDescription");
    this->description += "* ";
    auto currentLine = this->current->line;
    unsigned int openedBrackets = 1u;
    while((this->current!=this->fileTokens.end())&&
	  (!((this->current->value=="}")&&
	     (openedBrackets==1u)))){
      if(this->current->value=="{"){
	auto previous = this->current;
	--previous;
	if((previous->value.size()>0)&&
	   (previous->value[previous->value.size()-1]!='\\')){
	  ++openedBrackets;
	}
      }
      if(this->current->value=="}"){
	auto previous = this->current;
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
      if(this->current->flag==tfel::utilities::Token::String){
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
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
			    "Cannot read type of static variable.");
    const auto type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("DSLBase::treatStaticVar",
			      "type given is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
			    "Cannot read variable name.");
    const auto name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("DSLBase::treatStaticVar",
			      "Variable name '"+name+"' is not valid.");
    }
    const auto line = this->current->line;
    ++(this->current);
    this->checkNotEndOfFile("DSLBase::treatStaticVar",
			    "Expected to read value of variable.");
    const auto value = this->readInitialisationValue<long double>(name,true);
    this->readSpecifiedToken("DSLBase::treatStaticVar",";");
    this->addStaticVariableDescription(StaticVariableDescription(type,name,line,value.second));
  } // end of DSLBase::treatStaticVar

  void
  DSLBase::ignoreKeyWord(const std::string& key)
  {
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
      auto next = this->current;
      ++next;
      if(next!=this->fileTokens.end()){
	if(next->value==";"){
	  current = next;
	}
      }
    }
    ++(this->current);
  } // end of DSLBase::ignoreKeyWord

  double DSLBase::readDouble(void)
  {
    this->checkNotEndOfFile("DSLBase::readDouble");
    std::istringstream flux(this->current->value);
    double value;
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
    auto endOfTreatment=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatment)){
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("DSLBase::handleParameter : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      const auto n = this->current->value;
      const auto lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("DSLBase::handleParameter");
      if((this->current->value=="=")||
	 (this->current->value=="{")||
	 (this->current->value=="(")){
	std::string ci; // closing initializer
	if(this->current->value=="{"){
	  ci="}";
	}
	if(this->current->value=="("){
	  ci=")";
	}
	++(this->current);
	this->checkNotEndOfFile("DSLBase::handleParameter");
	std::istringstream converter(this->current->value);
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
      c.push_back(VariableDescription("real",n,1u,lineNumber));
    }
    if(!endOfTreatment){
      --(this->current);
      this->throwRuntimeError("DSLBase::handleParameter",
			      "Expected ';' before end of file");
    }
  } // end of DSLBase::handleParameter

  void DSLBase::completeTargetsDescription(){
    for(const auto& t : this->atds){
      for(const auto& al : t){
	for(auto& l : this->td){
	  if(l.name!=al.name){
	    insert_if(l.deps,al.name);
	  }
	}
      }
    }
    for(const auto& t : this->atds){
      mergeTargetsDescription(this->td,t,false);      
    }
    this->atds.clear();
  } // end of DSLBase::completeTargetsDescription()

} // end of namespace mfront
