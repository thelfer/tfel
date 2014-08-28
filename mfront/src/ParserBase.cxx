/*!
 * \file   mfront/src/ParserBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 */

#include<stdexcept>
#include<sstream>
#include<algorithm>
#include<iterator>

#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include"TFEL/System/ProcessManager.hxx"
#endif
#include"TFEL/Math/IntegerEvaluator.hxx"

#include"MFront/MFront.hxx"
#include"MFront/ParserBase.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontSearchFile.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontMFrontLawInterface.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/MFrontMaterialLawParser.hxx"

namespace mfront
{

  static bool
  isInteger(const std::string& s){
    using namespace std;
    bool b = true;
    string::const_iterator p;
    for(p=s.begin();(p!=s.end()&&(b));++p){
      b = static_cast<bool>(isdigit(*p));
    }
    return b;
  }

  ParserBase::VariableModifier::~VariableModifier()
  {} // end of ParserBase::VariableModifier::~VariableModifier

  ParserBase::WordAnalyser::~WordAnalyser()
  {} // end of ParserBase::WordAnalyser::~WordAnalyser

  ParserBase::ParserBase()
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
  ParserBase::openFile(const std::string& f,
		       const std::vector<std::string>& ecmds)
  {
    using namespace std;
    CxxTokenizer::openFile(f);
    vector<string>::const_reverse_iterator p;
    for(p=ecmds.rbegin();p!=ecmds.rend();++p){
      CxxTokenizer t;
      try{
	t.parseString(*p);
      } catch(exception& e){
	string msg("ParserBase::openFile : ");
	msg += "error while parsing external command "
	  "'"+*p+"' ('"+e.what()+"')";
	throw(runtime_error(msg));
      }
      this->fileTokens.insert(this->fileTokens.begin(),
			      t.begin(),t.end());
    }
  } // end of ParserBase::openFile

  const MFrontFileDescription&
  ParserBase::getMFrontFileDescription(void) const
  {
    return *this;
  } // end of ParserBase::getMFrontFileDescription

  ParserBase::~ParserBase()
  {} // end of ParserBase::~ParserBase

  void
  ParserBase::readNextBlock(CodeBlock& res1,
			    CodeBlock& res2,
			    tfel::utilities::shared_ptr<VariableModifier> modifier1,
			    tfel::utilities::shared_ptr<VariableModifier> modifier2,
			    const bool addThisPtr,
			    const std::string delim1,
			    const std::string delim2,
			    const bool allowSemiColon,
			    const bool registerLine)
  {
    TokensContainer::const_iterator pb = this->current;
    res1 = this->readNextBlock(addThisPtr,delim1,delim2,allowSemiColon,registerLine,modifier1);
    this->current = pb;
    res2 = this->readNextBlock(addThisPtr,delim1,delim2,allowSemiColon,registerLine,modifier2);
  } // end of ParserBase::readNextBlock

  CodeBlock
  ParserBase::readNextBlock(tfel::utilities::shared_ptr<VariableModifier> modifier,
			    const bool addThisPtr,
			    const std::string delim1,
			    const std::string delim2,
			    const bool allowSemiColon,
			    const bool registerLine)
  {
    return this->readNextBlock(addThisPtr,delim1,delim2,allowSemiColon,registerLine,modifier);
  } // end of ParserBase::readNextBlock

  CodeBlock
  ParserBase::readNextBlock(const bool addThisPtr,
			    const std::string delim1,
			    const std::string delim2,
			    const bool allowSemiColon,
			    const bool registerLine,
			    tfel::utilities::shared_ptr<VariableModifier> modifier,
			    tfel::utilities::shared_ptr<WordAnalyser> analyser)
  {
    using namespace std;
    CodeBlock b;
    if(!this->currentComment.empty()){
      b.description += this->currentComment;
    }
    string& res = b.code;
    unsigned short currentLine;
    unsigned short openedBlock;
    TokensContainer::const_iterator previous;
    openedBlock=0;
    this->readSpecifiedToken("ParserBase::readNextBlock",delim1);
    this->checkNotEndOfFile("ParserBase::readNextBlock",
			    "Expected a '"+delim2+"'.");
    if((this->current->value==";")&&(!allowSemiColon)){
      string msg("ParserBase::readNextBlock : ");
      msg += "read ';' before the end of block.\n";
      msg += "Number of block opened : ";
      msg += toString(openedBlock);
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
      res += toString(currentLine);
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
    if(analyser.get()!=0){
      analyser->exe(this->current->value);
    }
    if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
      b.static_variables.insert(this->current->value);
      previous = this->current;
      --previous;
      if((previous->value!="->")&&
	 (previous->value!=".")&&
	 (previous->value!="::")){
	res += this->getClassName();
	res += "::";
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
	if(modifier.get()!=0){
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
	  res += toString(currentLine);
	  res += " \"";
	  res += this->fileName;
	  res += "\"\n";
	}  else {
	  res += "\n";
	}
      }
      if((this->current->value==";")&&(!allowSemiColon)){
	string msg("ParserBase::readNextBlock : ");
	msg += "read ';' before the end of block.\n";
	msg += "Number of block opened : ";
	msg += toString(openedBlock);
	throw(runtime_error(msg));
      }
      if(!this->current->comment.empty()){
	if(!b.description.empty()){
	  b.description += '\n';
	}
	b.description += this->current->comment;
      }
      if(analyser.get()!=0){
	analyser->exe(this->current->value);
      }
      if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
	b.static_variables.insert(this->current->value);
	previous = this->current;
	--previous;
	if((previous->value!="->")&&
	   (previous->value!=".")&&
	   (previous->value!="::")){
	  res += this->getClassName();
	  res += "::";
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
	  if(modifier.get()!=0){
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
      string msg("ParserBase::readNextBlock : ");
      msg += "Expected the end of a block.\n";
      msg += "Number of block opened : ";
      msg += toString(openedBlock);
      throw(runtime_error(msg));
    }
    ++(this->current);
    return b;
  } // end of ParserBase::readNextBlock

  void
  ParserBase::throwRuntimeError(const std::string& method,
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
      msg += "\nError at line " + toString(t->line);
    }
    throw(runtime_error(msg));
  } // end of ParserBase::throwRuntimeError

  void ParserBase::treatImport()
  {
    using namespace std;
    const string m = "ParserBase::treatImport";
    const string oFileName = this->fileName;
    this->checkNotEndOfFile(m);
    const vector<string>& files = this->readStringOrArrayOfString(m);
    this->checkNotEndOfFile(m);
    this->readSpecifiedToken(m,";");
    vector<string>::const_iterator p;
    TokensContainer oFileTokens;
    oFileTokens.swap(this->fileTokens);
    TokensContainer::const_iterator ocurrent = this->current;
    for(p=files.begin();p!=files.end();++p){
      this->analyseFile(MFrontSearchFile::search(*p),
			vector<string>());
    }
    this->fileName = oFileName;
    this->fileTokens.swap(oFileTokens);
    this->current = ocurrent;
  }

  void
  ParserBase::checkNotEndOfFile(const std::string& method,
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
	msg += "\nError at line " + toString(this->current->line);
      }
      throw(runtime_error(msg));
    }
  } // end of ParserBase::checkNotEndOfFile

  unsigned short
  ParserBase::readUnsignedShort(const std::string& m)
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
  } // end of ParserBase::readUnsignedShort

  void
  ParserBase::readSpecifiedToken(const std::string& method,
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
      msg += toString(this->current->line);
      throw(runtime_error(msg));
    }
    ++(this->current);
  } // end of ParserBase::readSpecifiedToken

  std::string
  ParserBase::readUntilEndOfInstruction(void)
  {
    using namespace std;
    string res;
    while((this->current!=this->fileTokens.end())&&
	  (this->current->value != ";")){
      if(!this->current->value.empty()){
	if(this->current->value[0]=='@'){
	  string msg("ParserBase::readUntilEndOfInstruction : ");
	  msg += "no word beginning with '@' are allowed here";
	  throw(runtime_error(msg));
	}
	res+=this->current->value;
	res+=" ";
      }
      ++(this->current);
    }
    this->checkNotEndOfFile("ParserBase::readUntilEndOfInstruction",
			    "Missing ';' delimiter.");
    if(!res.empty()){
      res.erase(res.length()-1);
    }
    ++(this->current);
    return res;
  }

  std::string
  ParserBase::readOnlyOneToken(void)
  {
    using namespace std;
    string res;
    this->checkNotEndOfFile("ParserBase::readOnlyOneToken",
			    "Expected a word.");
    if(this->current->value==";"){
      string msg("ParserBase::readOnlyOneToken : ");
      msg += "No word read";
      throw(runtime_error(msg));
    }
    res = this->current->value;
    ++(this->current);
    this->readSpecifiedToken("ParserBase::readOnlyOneToken",";");
    return res;
  } // end of ParserBase::readOnlyOneToken

  void
  ParserBase::treatIntegerConstant()
  {
    using namespace std;
    typedef map<string,int>::value_type MVType;
    string name;
    pair<bool,int> value;
    this->checkNotEndOfFile("ParserBase::treatIntegerConstant",
			    "Cannot read type of static variable.");
    name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("ParserBase::treatIntegerConstant",
			      "Variable name '"+name+"' is not valid.");
    }
    ++(this->current);
    value = this->readInitialisationValue<int>(name,true);
    this->readSpecifiedToken("ParserBase::treatIntegerConstant",";");
    this->registerStaticVariable(name);
    if(!this->integerConstants.insert(MVType(name,value.second)).second){
      this->throwRuntimeError("ParserBase::treatIntegerConstant",
			      "variable '"+name+"' already declared");
    }
  } // end of ParserBase::treatIntegerConstant

  void ParserBase::readVarList(VariableDescriptionContainer& cont,
			       const std::string& type,
			       const bool allowArray,
			       const bool addIncrementVar,
			       const bool b)
  {
    using namespace std;
    using namespace tfel::math;
    using namespace tfel::utilities;
    string varName;
    unsigned short lineNumber;
    unsigned short asize;
    string endComment;
    bool endOfTreatement=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatement)){
      varName = this->current->value;
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("ParserBase::readVarList : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      lineNumber = this->current->line;
      asize = 1u;
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::readVarList");
      if(this->current->value=="["){
	if(!allowArray){
	  this->throwRuntimeError("ParserBase::readVarList : ",
				  "variable '"+varName+"' can't be declared an array");
	}
	string array_size;
	++(this->current);
	this->checkNotEndOfFile("ParserBase::readVarList");
	while(this->current->value!="]"){
	  if((this->current->flag!=Token::Standard)||
	     (this->current->value==";")){
	    this->throwRuntimeError("ParserBase::readVarList : ",
				    "invalid array size for '"+varName+"'");
	  }
	  array_size += this->current->value;
	  ++(this->current);
	  this->checkNotEndOfFile("ParserBase::readVarList");
	}
	if(array_size.empty()){
	  this->throwRuntimeError("ParserBase::readVarList : ",
				  "empty array size for '"+varName+"'");
	}
	IntegerEvaluator ev(array_size);
	const vector<string>& vars = ev.getVariablesNames();
	vector<string>::const_iterator pv;
	for(pv=vars.begin();pv!=vars.end();++pv){
	  map<string,int>::const_iterator pvv = this->integerConstants.find(*pv);
	  if(pvv==this->integerConstants.end()){
	    this->throwRuntimeError("ParserBase::readVarList : ",
				    "unknown constant '"+*pv+"'");
	  }
	  ev.setVariableValue(*pv,pvv->second);
	}
	asize = ev.getValue();
	if(asize<=0){
	  this->throwRuntimeError("ParserBase::readVarList : ",
				  "invalid array size for '"+varName+"'");
	}
	this->readSpecifiedToken("ParserBase::readVarList","]");
	this->checkNotEndOfFile("ParserBase::readVarList");
      }
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatement=true;
	endComment = this->current->comment;
	++(this->current);
      } else {
	this->throwRuntimeError("ParserBase::readVarList",
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
      for(VariableDescriptionContainer::iterator p=cont.begin();p!=cont.end();++p){
	if(!p->description.empty()){
	  p->description += ' ';
	}
	p->description += endComment;
      }
    }
    if(!endOfTreatement){
      --(this->current);
      this->throwRuntimeError("ParserBase::readVarList",
			      "Expected ';' before end of file");
    }
  }

  void ParserBase::readVarList(VariableDescriptionContainer& cont,
			       const bool allowArray,
			       const bool addIncrementVar,
			       const bool b)
  {
    using namespace std;
    string type;
    set<string>::iterator p;
    this->checkNotEndOfFile("ParserBase::readVarList",
			    "Cannot read type of varName.\n");
    type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("ParserBase::readVarList",
			      "given type "+type+"is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("ParserBase::readVarList");
    while(this->current->value=="::"){
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::readVarList");
      string t = this->current->value;
      if(!isValidIdentifier(t,false)){
	--(this->current);
	this->throwRuntimeError("ParserBase::readVarList",
				"given type '"+t+"' is not valid.");
      }
      type += "::"+t;
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::readVarList");
    }
    if(this->current->value=="<"){
      unsigned short openBrackets = 1u;
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::readVarList");
      type += "<";
      while(openBrackets!=0){
	string t = this->current->value;
	if((!isValidIdentifier(t,false))&&
	   (!isInteger(t))){
	  this->throwRuntimeError("ParserBase::readVarList",
				  "given type '"+t+"' is not valid.");
	}
	++(this->current);
	this->checkNotEndOfFile("ParserBase::readVarList");
	type += t;
	if(this->current->value==","){
	  ++(this->current);
	  this->checkNotEndOfFile("ParserBase::readVarList");
	  string t2 = this->current->value;
	  if((!isValidIdentifier(t2,false))&&
	     (!isInteger(t2))){
	    --(this->current);
	    this->throwRuntimeError("ParserBase::readVarList",
				    "given type '"+t+"' is not valid.");
	  }
	  type+=",";
	} else if(this->current->value=="<"){
	  if(isInteger(t)){
	    this->throwRuntimeError("ParserBase::readVarList",
				    "given type '"+t+"'is not valid.");
	  }
	  ++openBrackets;
	  ++(this->current);
	  this->checkNotEndOfFile("ParserBase::readVarList");
	  type += "<";      
	} else if(this->current->value==">"){
	  --openBrackets;
	  ++(this->current);
	  this->checkNotEndOfFile("ParserBase::readVarList");
	  type += ">";      
	}
      }
    }
    this->readVarList(cont,type,allowArray,addIncrementVar,b);
  } // end of ParserBase::readVarList

  void
  ParserBase::readList(std::vector<tfel::utilities::Token>& l,
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
  } // end of ParserBase::readList  

  std::vector<std::string>
  ParserBase::readArrayOfString(const std::string& m)
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
  } // end of ParserBase::readArrayOfString

  bool
  ParserBase::readBooleanValue(const std::string& m)
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
  } // end of ParserBase::readBooleanValue

  std::string
  ParserBase::readString(const std::string& m)
  {
    using namespace std;
    using namespace tfel::utilities;
    this->checkNotEndOfFile(m,"Expected a string or '{'");
    if(this->current->flag!=Token::String){
      this->throwRuntimeError(m,"Expected a string");
    }
    const string& r = this->current->value.substr(1,this->current->value.size()-2);
    ++(this->current);
    return r;
  } // end of ParserBase::readString

  std::vector<std::string>
  ParserBase::readStringOrArrayOfString(const std::string& m)
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
  } // end of ParserBase::readStringOrArrayOfString

  void
  ParserBase::treatLink(void){
    using namespace std;
    vector<string> nlink;
    vector<string>::const_iterator p;
    nlink = readStringOrArrayOfString("ParserBase::treatLink");
    this->readSpecifiedToken("ParserBase::treatLink",";");
    for(p=nlink.begin();p!=nlink.end();++p){
      if(find(this->librariesDependencies.begin(),
	      this->librariesDependencies.end(),*p)==this->librariesDependencies.end()){
	this->librariesDependencies.push_back(*p);
      }
    }
  } // end of ParserBase::treatLink

  void
  ParserBase::callMFront(const std::vector<std::string>& interfaces,
			 const std::vector<std::string>& files) const
  {
    using namespace std;
#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    using namespace tfel::system;
    ProcessManager m;
    ostringstream cmd;
    vector<string>::const_iterator p;
    cmd << MFront::getCallingName();
    cmd << " --interface=";
    for(p=interfaces.begin();p!=interfaces.end();){
      cmd << *p;
      if((++p)!=interfaces.end()){
	cmd << ",";
      }
    }
    cmd << " ";
    copy(files.begin(),files.end(),ostream_iterator<string>(cmd," "));
    m.execute(cmd.str());
#else
    string msg("ParserBase::callMFront : ");
    msg += "unsupported keyword on windows plate-form";
    throw(runtime_error(msg));
#endif
  } // end of ParserBase::callMFront

  void
  ParserBase::treatMFront(void){
    using namespace std;
    vector<string> vfiles;
    vector<string> vinterfaces;
    vector<string>::const_iterator p;
    this->readSpecifiedToken("ParserBase::treatMfront","{");
    vfiles = this->readStringOrArrayOfString("ParserBase::treatMfront");
    this->checkNotEndOfFile("ParserBase::treatMfront","Expected '}'");
    if(!((this->current->value=="}")||
	 (this->current->value==","))){
      this->throwRuntimeError("ParserBase::treatMfront",
			      "Expected ',' or '}', read '"+this->current->value+"'");
    }
    if(this->current->value==","){
      ++(this->current);
     vinterfaces = this->readStringOrArrayOfString("ParserBase::treatMfront");
    }
    this->readSpecifiedToken("ParserBase::treatMfront","}");
    this->readSpecifiedToken("ParserBase::treatMfront",";");
    this->callMFront(vinterfaces,vfiles);
  } // end of ParserBase::treatMfront

  std::string
  ParserBase::readSpecifiedValue(const std::string& file,
				 const std::string& value)
  {
    using namespace std;
    vector<string> values(1,value);
    return this->readSpecifiedValues(file,values)[1];
  } // end of ParserBase::readSpecifiedValue

  std::vector<std::string>
  ParserBase::readSpecifiedValues(const std::string& file,
				  const std::string& value1,
				  const std::string& value2)
  {
    using namespace std;
    vector<string> values(2);
    values[0] = value1;
    values[1] = value2;
    return this->readSpecifiedValues(file,values);
  } // end of ParserBase::readSpecifiedValues

  std::vector<std::string>
  ParserBase::readSpecifiedValues(const std::string& file,
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
	    string msg("ParserBase::readSpecifiedValues : ");
	    msg += "the keyword @Parser does not begin a new instruction.";
	    throw(runtime_error(msg));
	  }
	}
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected end of file '"+file+"' (exepected "+*p+").\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value==";"){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected end of line '"+file+"' (exepected "+*p+").\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	value = pt->value;
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected end of file '"+file+"' (exepected library name or ';').\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value!=";"){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected token '" << pt->value << "' (exepected ';').\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	res[static_cast<vector<string>::size_type>(p-values.begin())]=value;
      }
      ++pt;
    }
    return res;
  } // end of ParserBase::readSpecifiedValues

  void
  ParserBase::registerVariable(const std::string& v,
			       const bool b)
  {
    using namespace std;
    if(!CxxTokenizer::isValidIdentifier(v)){
      string msg("ParserBase::registerVariable : ");
      msg += "variable name '"+v+"' is invalid";
      throw(runtime_error(msg));
    }
    if(b){
      this->varNames.insert(v);
    } else {
      if(!this->varNames.insert(v).second){
	string msg("ParserBase::registerVariable : ");
	msg += "variable '"+v+"' already registred";
	throw(runtime_error(msg));
      }
    }
    this->reserveName(v,b);
  } // end of ParserBase::registerVariable

  void
  ParserBase::registerStaticVariable(const std::string& v)
  {
    using namespace std;
    if(!CxxTokenizer::isValidIdentifier(v)){
      string msg("ParserBase::registerStaticVariable : ");
      msg += "variable name '"+v+"' is invalid";
      throw(runtime_error(msg));
    }
    if(!this->staticVarNames.insert(v).second){
      string msg("ParserBase::registerStaticVariable : ");
      msg += "static variable '"+v+"' already registred";
      throw(runtime_error(msg));
    }
    this->reserveName(v,false);
  } // end of ParserBase::registerStaticVariable

  void
  ParserBase::reserveName(const std::string& w,
			  const bool b)
  {
    using namespace std;
    if(b){
      this->reservedNames.insert(w);
    } else {
      if(!this->reservedNames.insert(w).second){
	string msg("ParserBase::reserveName : ");
	msg += "name '"+w+"' already reserved";
	throw(runtime_error(msg));
      }
    }
  } // end of ParserBase::reserveName

  bool
  ParserBase::isNameReserved(const std::string& n) const
  {
    return this->reservedNames.find(n)!=this->reservedNames.end();
  } // end of ParserBase::isNameReserved

  MaterialPropertyDescription
  ParserBase::handleMaterialLaw(const std::string& f)
  {
    using namespace std;
#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    using namespace tfel::system;
    ProcessManager m;
    // getting informations the source files
    MFrontMaterialLawParser mp;
    try{
      MFrontMFrontLawInterface minterface;
      const string& path = MFrontSearchFile::search(f);
      mp.analyseFile(path);
      const MaterialPropertyDescription& mpd  = mp.getMaterialPropertyDescription();
      const string& mname = minterface.getFunctionName(mpd.material,
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
#else
    string msg("ParserBase::handleMaterialLaw : ");
    msg += "unsupported keyword on windows plate-form";
    throw(runtime_error(msg));
    return MaterialPropertyDescription();
#endif
  } // end of ParserBase::handleMaterialLaw

  void
  ParserBase::treatMaterialLaw(void){
    using namespace std;
    vector<string> vfiles;
    vector<string>::const_iterator p;
    vfiles = this->readStringOrArrayOfString("ParserBase::treatMfront");
    this->readSpecifiedToken("ParserBase::treatMfront",";");
    for(p=vfiles.begin();p!=vfiles.end();++p){
      this->handleMaterialLaw(*p);
    }
  } // end of ParserBase::treatMaterialLaw

  void
  ParserBase::treatDescription(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    unsigned short currentLine;
    unsigned short openedBrackets;
    this->readSpecifiedToken("ParserBase::treatDescription","{");
    this->checkNotEndOfFile("ParserBase::treatDescription");
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
      this->throwRuntimeError("ParserBase::treatDescription",
			      "File ended before the end of description.");
    }
    ++(this->current);
  } // end of ParserBase::treatDescription

  void ParserBase::treatAuthor(void)
  {
    this->authorName = this->readUntilEndOfInstruction();
  } // end of ParserBase::treatAuthor

  void ParserBase::treatDate(void)
  {
    this->date = this->readUntilEndOfInstruction();
  } // end of ParserBase::treatDate

  void ParserBase::treatUnknownKeyword(void)
  {
    using namespace std;
    --(this->current);
    this->throwRuntimeError("ParserBase::treatUnknownKeyword",
			    "unknown keyword (read '"+this->current->value+"')");
  } // end of ParserBase::treatUnknownKeyword

  void ParserBase::treatIncludes(void)
  {
    this->appendToIncludes(this->readNextBlock().code);
  }

  void
  ParserBase::treatMembers(void)
  {
    this->appendToMembers(this->readNextBlock().code);
  }

  void
  ParserBase::treatParser(void)
  {
    this->readUntilEndOfInstruction();
  } // end of ParserBase::treatParser

  void
  ParserBase::treatSources(void)
  {
    this->appendToSources(this->readNextBlock().code);
  } // end of ParserBase::treatSources(void)

  void
  ParserBase::treatPrivate(void)
  {
    this->appendToPrivateCode(this->readNextBlock().code);
  } // end of ParserBase::treatSources(void)

  void
  ParserBase::treatStaticVar(void)
  {
    using namespace std;
    string type;
    string name;
    unsigned short line;
    pair<bool,long double> value;
    this->checkNotEndOfFile("ParserBase::treatStaticVar",
			    "Cannot read type of static variable.");
    type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("ParserBase::treatStaticVar",
			      "type given is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("ParserBase::treatStaticVar",
			    "Cannot read variable name.");
    name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("ParserBase::treatStaticVar",
			      "Variable name '"+name+"' is not valid.");
    }
    line = this->current->line;
    ++(this->current);
    this->checkNotEndOfFile("ParserBase::treatStaticVar",
			    "Expected to read value of variable.");
    value = this->readInitialisationValue<long double>(name,true);
    this->readSpecifiedToken("ParserBase::treatStaticVar",";");
    this->registerStaticVariable(name);
    this->addStaticVariableDescription(StaticVariableDescription(type,name,line,value.second));
  } // end of ParserBase::treatStaticVar

  void
  ParserBase::ignoreKeyWord(const std::string& key)
  {
    using namespace std;
    this->checkNotEndOfFile("ParserBase::ignoreKeyWord",
			    "error while treating keyword '"+key+"' ");
    while((this->current->value!="{")&&(this->current->value!=";")){
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::ignoreKeyWord",
			      "error while treating keyword '"+key+"' ");
    }
    if(this->current->value=="{"){
      unsigned short openedBrackets = 1;
      while(!((this->current->value=="}")&&
	      (openedBrackets==0))){
	++(this->current);
	this->checkNotEndOfFile("ParserBase::ignoreKeyWord",
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
  } // end of ParserBase::ignoreKeyWord

  double
  ParserBase::readDouble(void)
  {
    using namespace std;
    double value;
    this->checkNotEndOfFile("ParserBase::readDouble");
    istringstream flux(this->current->value);
    flux >> value;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ParserBase::readDouble",
			      "Failed to read double value from token '"+
			      this->current->value+"'.");
    }
    ++(this->current);
    return value;
  } // end of ParserBase::readDouble

  void
  ParserBase::handleParameter(VariableDescriptionContainer& c,
			      std::map<std::string,double>& v)
  {
    using namespace std;
    typedef map<string,double>::value_type MVType;
    bool endOfTreatement=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatement)){
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("ParserBase::handleParameter : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      const string n = this->current->value;
      const unsigned short lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::handleParameter");
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
	this->checkNotEndOfFile("ParserBase::handleParameter");
	istringstream converter(this->current->value);
	double value;
	converter >> value;
	if(!converter&&(!converter.eof())){
	  this->throwRuntimeError("ParserBase::handleParameter",
				  "could not read default value for parameter '"+n+"'");
	}
	++(this->current);
	this->checkNotEndOfFile("ParserBase::handleParameter");
	if(!v.insert(MVType(n,value)).second){
	  this->throwRuntimeError("ParserBase::handleParameter",
				  "default value already defined for parameter '"+n+"'");
	}
	if(!ci.empty()){
	  this->readSpecifiedToken("ParserBase::handleParameter",ci);
	}
      }
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatement=true;
	++(this->current);
      } else {
	this->throwRuntimeError("ParserBase::handleParameter",
				", or ; expected afer '"+n+"'");
      }
      this->registerVariable(n,false);
      c.push_back(VariableDescription("real",n,1u,lineNumber));
    }
    if(!endOfTreatement){
      --(this->current);
      this->throwRuntimeError("ParserBase::handleParameter",
			      "Expected ';' before end of file");
    }
  } // end of ParserBase::handleParameter

} // end of namespace mfront
