/*! 
 * \file  MFrontModelParserCommon.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jun 2010
 */

#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontModelParserCommon.hxx"
#include"MFront/MFrontModelInterfaceFactory.hxx"

namespace mfront{

  bool
  MFrontModelParserCommon::is(const MFrontModelData& data,
			      const VarContainer& vc,
			      const std::string& v)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,unsigned short>::const_iterator p2;
    unsigned short d;
    unsigned short i;
    for(p=vc.begin();p!=vc.end();++p){
      if(p->name==v){
	return true;
      }
      p2 = data.depths.find(p->name);
      if(p2==data.depths.end()){
	d = 0;
      } else {
	d = p2->second;
      }
      for(i=1;i!=d+1;++i){
	if(p->name+'_'+toString(i)==v){
	  return true;
	}
      }
    }
    return false;
  } // end of MFrontModelParserCommon::is(void)

  void MFrontModelParserCommon::treatModel(void)
  {
    using namespace std;
    if(!this->className.empty()){
      this->throwRuntimeError("MFrontModelParserCommon::treatModel",
			      "model name already defined");
    }
    this->className = this->readOnlyOneToken();
    if(!isValidIdentifier(this->className)){
      this->throwRuntimeError("MFrontModelParserCommon::treatModel",
			      "invalid model name");
    }
    if(!this->material.empty()){
      this->className = this->material+"_"+this->className;
    }
  } // end of MFrontModelParserCommon::treatModel

  void
  MFrontModelParserCommon::treatMaterial(void)
  {
    ParserBase::treatMaterial();
    if(!this->className.empty()){
      this->className = this->material+"_"+this->className;
    }
  } // end of MFrontModelParserCommon::treatMaterial

  void
  MFrontModelParserCommon::setInterfaces(const std::set<std::string>& i)
  {
    using namespace std;
    copy(i.begin(),i.end(),back_inserter(this->interfaces));
  } // end of MFrontModelParserCommon::setInterfaces

  void
  MFrontModelParserCommon::writeOutputFiles(void)
  {
    using namespace std;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    this->verboseMode = true;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      interface->writeOutputFiles(*this,*this);
    }
  } // end of MFrontModelParserCommon::writeOutputFiles

  void MFrontModelParserCommon::setVerboseMode(void)
  {
    using namespace std;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    this->verboseMode = true;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      interface->setVerboseMode();
    }
  }

  void MFrontModelParserCommon::setDebugMode(void)
  {
    using namespace std;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    this->debugMode = true;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      interface->setDebugMode();
    }
  }

  void MFrontModelParserCommon::setWarningMode(void)
  {
    using namespace std;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    this->warningMode = true;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      interface->setWarningMode();
    }
  }

  void
  MFrontModelParserCommon::treatUnknownKeyword(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    pair<bool,CxxTokenizer::TokensContainer::const_iterator> p;
    TokensContainer::const_iterator p2;
    vector<string>::const_iterator i;
    bool treated = false;
    string key;
    --(this->current);
    key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontModelParserCommon::treatUnknownKeyword");
    if(this->current->value=="["){
      set<string> s;
      while(this->current->value!="]"){
	++(this->current);
	this->checkNotEndOfFile("MFrontModelParserCommon::treatUnknownKeyword");
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
	  this->throwRuntimeError("MFrontModelParserCommon::treatUnknownKeyword",
				  "unexpected token '"+this->current->value+"' (expected ']' or ',').");
	}
      }
      ++(this->current);
      if(s.empty()){
	this->ignoreKeyWord(key);
      } else {
	for(set<string>::const_iterator pi  = s.begin();pi != s.end();++pi){
	  MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*pi);
	  p = interface->treatKeyword(key,this->current,
				      this->fileTokens.end());
	  if(!p.first){
	    string msg("MFrontModelParserCommon::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += " has not been treated by interface '"+*pi+"'";
	    throw(runtime_error(msg));
	  }
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontModelParserCommon::treatUnknownKeyword : the keyword '");
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
	MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
	p = interface->treatKeyword(key,this->current,
				    this->fileTokens.end());
	if(p.first){
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontModelParserCommon::treatUnknownKeyword : the keyword '");
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

  } // end of MFrontModelParserCommon::treatUnknownKeyword

  void
  MFrontModelParserCommon::treatDomain(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(!this->domains.empty()){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      }
      this->throwRuntimeError("MFrontModelParserCommon::treatDomains",msg);
    }
    this->checkNotEndOfFile("MFrontModelParserCommon::treatDomain");
    if(this->current->flag!=Token::String){
      this->throwRuntimeError("MFrontModelParserCommon::treatDomain",
			      "Expected to read a string (read '"+this->current->value+"').");
    }
    if(this->current->value.size()<2){
      this->throwRuntimeError("MFrontModelParserCommon::treatDomain : ",
			      "domain name too short.");
    }
    this->domains.insert(this->current->value.substr(1,this->current->value.size()-2));
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatDomain",";");
  } // end of MFrontModelParserCommon::treatDomain(void)

  void
  MFrontModelParserCommon::treatDomains(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    bool bend;
    if(!this->domains.empty()){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      }
      this->throwRuntimeError("MFrontModelParserCommon::treatDomains",msg);
    }
    this->checkNotEndOfFile("MFrontModelParserCommon::treatDomains");
    this->readSpecifiedToken("MFrontModelParserCommon::treatDomains","{");
    this->checkNotEndOfFile("MFrontModelParserCommon::treatDomains");
    bend = false;
    while((this->current!=this->fileTokens.end())&&(!bend)){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatDomains",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatDomains",
				"Domain name too short.");
      }
      if(!this->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatDomains",
				"domain "+this->current->value.substr(1,this->current->value.size()-2)+
				" already defined.");
      }
      ++(this->current);	
      this->checkNotEndOfFile("MFrontModelParserCommon::treatDomains",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++this->current;
      } else if(this->current->value=="}"){
	bend = true;
      } else {
	this->throwRuntimeError("MFrontModelParserCommon::treatDomains",
				"Expected ',' or ')' (read '"+this->current->value+"').");
      }
    }
    this->readSpecifiedToken("MFrontModelParserCommon::treatDomains","}");
    if(this->domains.empty()){
      this->throwRuntimeError("MFrontModelParserCommon::treatDomains",
			      "@Domains does not set any domain.");
    }
    this->checkNotEndOfFile("MFrontModelParserCommon::treatDomains");
    ++(this->current);
  } // end of MFrontModelParserCommon::treatDomains(void)

  std::pair<std::string,unsigned short>
  MFrontModelParserCommon::decomposeVariableName(const std::string& v) const
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,unsigned short>::const_iterator p2;
    unsigned short d;
    unsigned short i;
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      if(v==p->name){
	return pair<string,unsigned short>(v,0u);
      }
      p2 = this->depths.find(p->name);
      if(p2!=this->depths.end()){
	d = p2->second;
      } else {
	d = 0;
      }
      for(i=1;i!=d+1;++i){
	if(v==p->name+"_"+toString(i)){
	  return pair<string,unsigned short>(p->name,i);
	}
      }
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      if(v==p->name){
	return pair<string,unsigned short>(v,0);
      }
      p2 = this->depths.find(p->name);
      if(p2!=this->depths.end()){
	d = p2->second;
      } else {
	d = 0;
      }
      for(i=1;i!=d+1;++i){
	if(v==p->name+"_"+toString(i)){
	  return pair<string,unsigned short>(p->name,i);
	}
      }
    }
    string msg("MFrontModelParserCommon::decomposeVariableName : ");
    msg += "no decomposition found  for variable '"+v+"'";
    throw(runtime_error(msg));
    return pair<string,unsigned short>("",0u);
  } // end of MFrontModelParserCommon::getPleiadesVariableName(const std::string& v)

  bool
  MFrontModelParserCommon::isInputVariable(const std::string& v) const
  {
    return MFrontModelParserCommon::is(*this,this->inputs,v);
  } // end of MFrontModelParserCommon::isInputVariable(void)

  bool
  MFrontModelParserCommon::isOutputVariable(const std::string& v) const
  {
    return MFrontModelParserCommon::is(*this,this->outputs,v);
  } // end of MFrontModelParserCommon::isInputVariable(void)
  
  void
  MFrontModelParserCommon::treatFunction(void)
  {
    using namespace std;
    Function f;
    pair<string,unsigned short> dv;
    VarContainer::iterator p;
    set<string>::const_iterator p2;
    set<string>::iterator p3;
    FunctionContainer::iterator p4;
    StaticVarContainer::const_iterator p5;
    map<string,unsigned short>::const_iterator p6;
    unsigned short openedBrackets = 0;
    unsigned short openedParenthesis = 0;
    unsigned short currentLine;
    bool newLine;
    bool newInstruction;
    bool found = false;
    bool treated;
    f.useTimeIncrement = false;
    this->registerVariable("functor"+toString(static_cast<unsigned short>(this->functions.size())));
    this->checkNotEndOfFile("MFrontModelParserCommon::treatFunction");
    f.name = this->current->value;
    if(!this->isValidIdentifier(f.name)){
      string msg("MFrontModelParserCommon::treatFunction : ");
      msg += "function name '"+f.name+"' is not valid";
      throw(runtime_error(msg));
    }
    this->registerVariable(f.name);
    this->reserveName(f.name+".Domain");
    this->reserveName(f.name+".Domains");
    f.line = this->current->line;
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatFunction","{");
    ++openedBrackets;
    newInstruction = true;
    this->checkNotEndOfFile("MFrontModelParserCommon::treatFunction",
			    "Expected body of function '"+f.name+"'.");
    currentLine = this->current->line;
    newLine=true;
    if(this->debugMode){
      f.body  +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
    }
    for(;(this->current!=this->fileTokens.end())&&
	  (openedBrackets!=0);++(this->current)){
      if(this->current->line!=currentLine){
	currentLine=this->current->line;
	f.body  += "\n";
	if(this->debugMode){
	  f.body  +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
	}
	newLine = true;
      } 
      if(this->current->value=="{"){
	++openedBrackets;
	f.body  +="{";
	newInstruction = true;
      } else if(this->current->value=="}"){
	--openedBrackets;
	if(openedBrackets!=0){
	  f.body  +="}";
	}
      } else if(this->current->value=="("){
	++openedParenthesis;
	f.body  +="(";
      } else if(this->current->value==")"){
	if(openedParenthesis==0){
	  this->throwRuntimeError("MFrontModelParserCommon::treatFunction",
				  "unbalanced parenthesis");
	}
	--openedParenthesis;
	f.body  += ")";
      } else if(this->current->value==";"){
	f.body  += ";";
	newInstruction = true;
      } else {
	if(!newLine){
	  f.body  += " ";
	}
	if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
	  // treating the case of static variables
	  found=false;
	  for(p5=this->staticVars.begin();(p5!=this->staticVars.end())&&(!found);){
	    if(p5->name==this->current->value){
	      found = true;
	    } else {
	      ++p5;
	    }
	  }
	  if(!found){
	    this->throwRuntimeError("MFrontModelParserCommon::treatFunction",
				    "Internal error while treating static var '"+
				    this->current->value+"' in the body of function "+
				    f.name+"'");
	  }
	  treated = true;
	  f.body  += "(" + this->className + ":: "+ this->current->value + ")";
	} else if(this->varNames.find(this->current->value)!=this->varNames.end()){
	  treated = false;
	  if(newInstruction){
	    string var = this->current->value;
	    string op;
	    bool modifier;
	    ++(this->current);
	    if(this->current==fileTokens.end()){
	      string msg("MFrontModelParserCommon::treatFunction : ");
	      msg+="unexpected end of file while reading body of function " + f.name;
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
	      found = false;
	      for(p=this->outputs.begin();(p!=this->outputs.end())&&(!found);){
		if(p->name==var){
		  found = true;
		} else {
		  ++p;
		}
	      }
	      if(!found){
		this->throwRuntimeError("MFrontModelParserCommon::treatFunction",
					"trying to modify variable '"+var+
					"' in the body of function '"+f.name+"'");
	      }
	      f.body  += var + " " + op + " ";
	      f.modifiedVariables.insert(var);
	      p6 = f.depths.find(var);
	      if(p6==f.depths.end()){
		f.depths[var] = 0;
	      }	      
	      treated = true;
	    } else {
	      --(this->current);
	    }
	  }
	  if(!treated){
	    // treating the case of global parameters
	    for(p =this->globalParameters.begin();
		(p!=this->globalParameters.end())&&(!treated);++p){
	      if(p->name==this->current->value){
		treated = true;
		f.globalParameters.insert(this->current->value);
		f.body  += "(this->" + this->current->value + ")";
	      }
	    }
	    // treating the case of local parameters
	    for(p =this->localParameters.begin();
		(p!=this->localParameters.end())&&(!treated);++p){
	      if(p->name==this->current->value){
		treated = true;
		f.localParameters.insert(this->current->value);
		f.body  += "(this->" + this->current->value + ")";
	      }
	    }
	    // treating the case of local parameters
	    for(p =this->constantMaterialProperties.begin();
		(p!=this->constantMaterialProperties.end())&&(!treated);++p){
	      if(p->name==this->current->value){
		treated = true;
		f.constantMaterialProperties.insert(this->current->value);
		f.body  += "(this->" + this->current->value + ")";
	      }
	    }
	    if(!treated){
	      if(this->isInputVariable(this->current->value)){
		f.usedVariables.insert(this->current->value);
		dv = this->decomposeVariableName(this->current->value);
		p6 = f.depths.find(dv.first);
		if(p6!=f.depths.end()){
		  if(dv.second>p6->second){
		    f.depths[dv.first] = dv.second;
		  }
		} else {
		  f.depths[dv.first] = dv.second;
		}
		f.body  += this->current->value;
	      } else if(this->isOutputVariable(this->current->value)){
		f.usedVariables.insert(this->current->value);
		dv = this->decomposeVariableName(this->current->value);
		p6 = f.depths.find(dv.first);
		if(p6!=f.depths.end()){
		  if(dv.second>p6->second){
		    f.depths[dv.first] = dv.second;
		  }
		} else {
		  f.depths[dv.first] = dv.second;
		}
		f.body  += this->current->value;
	      }
	    }
	  }
	} else {
	  if(this->current->value=="dt"){
	    f.useTimeIncrement = true;
	    f.body  += "dt";
	  } else {
	    f.body  += this->current->value;
	  }
	}
	newInstruction = false;
      }
      newLine=false;
    }
    if((this->current==fileTokens.end())&&(openedBrackets!=0)){
      string msg("MFrontModelParserCommon::treatFunction : ");
      msg+="unexpected end of file while reading body of function " + f.name;
      throw(runtime_error(msg));
    }
    if(openedBrackets!=0){
      string msg("MFrontModelParserCommon::treatFunction : ");
      msg+="parenthesis still opened at the end of function " + f.name;
      throw(runtime_error(msg));
    }
    if(f.modifiedVariables.size()==0){
      string msg("MFrontModelParserCommon::treatFunction : ");
      msg+="function " + f.name + " does not change any variable.";
      throw(runtime_error(msg));
    }
    if(!this->functionNames.insert(f.name).second){
      string msg("MFrontModelParserCommon::treatFunction : ");
      msg+="function " + f.name + " already declared.";
      throw(runtime_error(msg));
    }

    for(p2=f.modifiedVariables.begin();p2!=f.modifiedVariables.end();++p2){
      if((p3=f.usedVariables.find(*p2))!=f.usedVariables.end()){
	f.usedVariables.erase(*p3);
      }
    }
    this->functions.push_back(f);
  } // end of MFrontModelParserCommon::treatFunction(void)

  void
  MFrontModelParserCommon::treatOutput(void)
  {
    using namespace std;
    if(!this->functions.empty()){
      string msg("MFrontModelParserCommon::treatInput : ");
      msg += "outputs must be declared before declaring functions";
      throw(runtime_error(msg));
    }
    VarContainer::const_iterator p;
    this->readVarList(this->outputs,"Field",false,false);
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      this->fieldNames.insert(p->name);
    }
  } // end of MFrontModelParserCommon::treatOutput(void)

  static void
  MFrontModelParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(const std::map<std::string,std::string>& g,
								 const std::map<std::string,std::string>& e,
								 const std::string& n)
  {
    using namespace std;
    map<string,string>::const_iterator p;
    for(p=g.begin();p!=g.end();++p){
      if(p->second==n){
	string msg("MFrontModelParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as a glossary name";
	throw(runtime_error(msg));
      }
    }
    for(p=e.begin();p!=e.end();++p){
      if(p->second==n){
	string msg("MFrontModelParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName : ");
	msg += "name '"+n+"' is already used as an entry name";
	throw(runtime_error(msg));
      }
    }
  }

  void
  MFrontModelParserCommon::treatOutputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    typedef map<string,string>::value_type MVType;
    typedef map<string,double>::value_type MVType2;
    typedef map<string,unsigned short>::value_type MVType3;
    unsigned short i;
    if(!this->functions.empty()){
      string msg("MFrontModelParserCommon::treatOutputMethod : ");
      msg += "output must be called before declaring functions";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MFrontModelParserCommon::treatOutputMethod",".");
    this->checkNotEndOfFile("MFrontModelParserCommon::treatOutputMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultInitialValue")&&
       (methodName!="setDepth")){
      this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
			      "Unknown method (valid methods for fields are "
			      "setGlossaryName, setEntryName, setDepth and "
			      "setDefaultInitialValue,"
			      "read '"+methodName+"').");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatOutputMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatOutputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontModelParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
								     this->entryNames,
								     glossaryName);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatOutputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar + "'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontModelParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
								     this->entryNames,
								     entryName);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Entry file name for field '"+this->currentVar+"' already defined.");
      }
    }  else if (methodName=="setDefaultInitialValue"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatOutputMethod",
			      "Expected intial value.");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Could not read initial value of field : '"+this->currentVar+"'");
      }
      if(!this->initialValues.insert(MVType2(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Initial value for field '"+this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatOutputMethod",
			      "Expected depth value.");
      unsigned short value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"Could not read depth value of field '"+this->currentVar+"'");
      }
      if(!this->depths.insert(MVType3(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				"depth value for field '"+this->currentVar+"' already defined.");
      }
      for(i=1;i<=value;++i){
	this->registerVariable(this->currentVar+"_"+toString(i));
	this->registerVariable("f_"+this->currentVar+"_"+toString(i));
	this->registerVariable("ff_"+this->currentVar+"_"+toString(i));
	if(!this->fieldNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
				  "Field '"+this->currentVar+"_"+toString(i)+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    } else {
      this->throwRuntimeError("MFrontModelParserCommon::treatOutputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatOutputMethod",")");
    this->readSpecifiedToken("MFrontModelParserCommon::treatOutputMethod",";");
  } // end of MFrontModelParserCommon::treatOutputMethod

  void
  MFrontModelParserCommon::treatInput(void)
  {
    using namespace std;
    if(!this->functions.empty()){
      string msg("MFrontModelParserCommon::treatInput : ");
      msg += "inputs must be declared before declaring functions";
      throw(runtime_error(msg));
    }
    VarContainer::const_iterator p;
    this->readVarList(this->inputs,"Field",false,false);
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      this->fieldNames.insert(p->name);
    }
  } // end of MFrontModelParserCommon::treatInput(void)

  void
  MFrontModelParserCommon::treatInputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    typedef map<string,string>::value_type MVType;
    typedef map<string,unsigned short>::value_type MVType2;
    unsigned short i;
    if(!this->functions.empty()){
      string msg("MFrontModelParserCommon::treatInputMethod : ");
      msg += "input method must be called before declaring functions";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MFrontModelParserCommon::treatInputMethod",".");
    this->checkNotEndOfFile("MFrontModelParserCommon::treatInputMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName") &&
       (methodName!="setDepth")){
      this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
			      "Unknown method (valid methods for input fields are "
			      "setGlossaryName, setEntryName, setDepth"
			      ", read '"+methodName+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatInputMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatInputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontModelParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
								     this->entryNames,
								     glossaryName);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Glossary name for field '"+this->currentVar+"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatInputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      MFrontModelParserCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
								     this->entryNames,
								     entryName);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatInputMethod",
			      "Expected depth value.");
      unsigned short value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Could not read initial value of field '"+this->currentVar+"'");
      }
      if(!this->depths.insert(MVType2(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				"Initial value for field '"+this->currentVar+"' already defined.");
      }
      for(i=1;i<=value;++i){
	this->registerVariable(this->currentVar+"_"+toString(i));
	this->registerVariable("f_"+this->currentVar+"_"+toString(i));
	this->registerVariable("ff_"+this->currentVar+"_"+toString(i));
	if(!this->fieldNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
				  "Field '"+this->currentVar+"_"+toString(i)+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    } else {
      this->throwRuntimeError("MFrontModelParserCommon::treatInputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatInputMethod",")");
    this->readSpecifiedToken("MFrontModelParserCommon::treatInputMethod",";");
  } // end of MFrontModelParserCommon::treatInputMethod

  void
  MFrontModelParserCommon::treatGlobalParameter(void)
  {
    this->readVarList(this->globalParameters,false,false);
  } // end of MFrontModelParserCommon::treatGlobalParameter(void)

  void
  MFrontModelParserCommon::treatGlobalParameterMethod(void) 
  {
    using namespace std; 
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontModelParserCommon::treatGlobalParameterMethod",".");
    this->checkNotEndOfFile("MFrontModelParserCommon::treatGlobalParameterMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
			      "Unknown method (valid methods for global parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read '"+methodName+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatGlobalParameterMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatGlobalParameterMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatGlobalParameterMethod"
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatGlobalParameterMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatGlobalParameterMethod",")");
    this->readSpecifiedToken("MFrontModelParserCommon::treatGlobalParameterMethod",";");
  } // end of MFrontModelParserCommon::treatGlobalParameterMethod

  void
  MFrontModelParserCommon::treatLocalParameter(void)
  {
    this->readVarList(this->localParameters,false,false);
  } // end of MFrontModelParserCommon::treatLocalParameter(void)

  void
  MFrontModelParserCommon::treatLocalParameterMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontModelParserCommon::treatLocalParameterMethod",".");
    this->checkNotEndOfFile("MFrontModelParserCommon::treatLocalParameterMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
			      "Unknown method (valid methods for local parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read "+methodName+").");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatLocalParameterMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatLocalParameterMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'.");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatLocalParameterMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatLocalParameterMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatLocalParameterMethod",")");
    this->readSpecifiedToken("MFrontModelParserCommon::treatLocalParameterMethod",";");
  } // end of MFrontModelParserCommon::treatLocalParameterMethod

  void
  MFrontModelParserCommon::treatConstantMaterialProperty(void)
  {
    this->readVarList(this->constantMaterialProperties,"real",false,false);
  } // end of MFrontModelParserCommon::treatConstantMaterialProperty(void)

  void
  MFrontModelParserCommon::treatConstantMaterialPropertyMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",".");
    this->checkNotEndOfFile("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
			      "Unknown method (valid methods for local parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read "+methodName+").");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatConstantMaterialPropertyMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'.");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",")");
    this->readSpecifiedToken("MFrontModelParserCommon::treatConstantMaterialPropertyMethod",";");
  } // end of MFrontModelParserCommon::treatConstantMaterialPropertyMethod

  void
  MFrontModelParserCommon::readDefaultValue(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    VarContainer::const_iterator p;
    unsigned short nbr;
    string res;
    bool found;
    bool bend;
    found=false;
    for(p=this->localParameters.begin();(p!=this->localParameters.end())&&(!found);){
      if(p->name==this->currentVar){
	found=true;
      } else {
	++p;
      }
    }    
    if(!found){
      for(p=this->globalParameters.begin();(p!=this->globalParameters.end())&&(!found);){
	if(p->name==this->currentVar){
	  found=true;
	} else {
	  ++p;
	}
      } 
    }
    if(!found){
      for(p=this->constantMaterialProperties.begin();
	  (p!=this->constantMaterialProperties.end())&&(!found);){
	if(p->name==this->currentVar){
	  found=true;
	} else {
	  ++p;
	}
      } 
    }
    if(!found){
      this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
			      "variable '"+this->currentVar+"' is neither a "
			      "constant material properties, "
			      "a local parameter or a global parameter");
    }
    this->checkNotEndOfFile("MFrontModelParserCommon::readDefaultValue",
			    "Expected default value.");
    if((p->type=="DoubleArray")||
       (p->type=="StringArray")){
      nbr=0u;
      bend = false;
      while(bend==false){
	if(p->type=="DoubleArray"){
	  istringstream converter(this->current->value);
	  double tmp;
	  converter >> tmp;
	  if(!converter&&(!converter.eof())){
	    this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
				    "Could not read default for variable '"+this->currentVar+"'");
	  }
	} else {
	  if(this->current->flag!=Token::String){
	    this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
				    "Expected to read a string (read '"+this->current->value+"').");
	  }
	}
	++nbr;
	res+=" "+this->current->value;
	++(this->current);
	this->checkNotEndOfFile("MFrontModelParserCommon::readDefaultValue",
				"Expected ',' or ')'.");
	if(this->current->value==")"){
	  bend = true;
	  --(this->current);
	} else if(this->current->value==","){
	  ++(this->current);
	  this->checkNotEndOfFile("MFrontModelParserCommon::readDefaultValue",
				  "Expected default value.");
	} else {
	  this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
				  "Unexpected token (expected ',' or ')', read '"+this->current->value+"').");
	}
      }
      res = toString(nbr)+res;
    } else if ((p->type=="double")||(p->type=="real")){
      istringstream converter(this->current->value);
      double tmp;
      converter >> tmp;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
				"Could not read default for variable '"+this->currentVar+"'");
      }
      res = this->current->value;
    } else if (p->type=="string"){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      res = this->current->value;
    } else if (p->type=="bool"){
      if((this->current->value!="true")&&
	 (this->current->value!="false")){
	this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
				"expected to read 'true' or 'false' for type 'bool', read '"+
				this->current->value+"'");
      }
      res = this->current->value;
    } else {
      this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
			      "type '"+p->type+"' is not supported.");
    }
    if(!this->defaultValues.insert(MVType(this->currentVar,res)).second){
      this->throwRuntimeError("MFrontModelParserCommon::readDefaultValue",
			      "default value for field '"+ this->currentVar +"' already defined.");
    }
  } // end of MFrontModelParserCommon::readDefaultValue

  void
  MFrontModelParserCommon::treatBounds(void)
  {
    this->registerBounds(this->boundsDescriptions);
  } // end of MFrontModelParserCommon::treatBounds

  void
  MFrontModelParserCommon::treatPhysicalBounds(void)
  {
    this->registerBounds(this->physicalBoundsDescriptions);
  } // end of MFrontModelParserCommon::treatPhysicalBounds

  void
  MFrontModelParserCommon::registerBounds(std::vector<VariableBoundsDescription>& container)
  {
    using namespace std;
    VarContainer::const_iterator p;
    VariableBoundsDescription boundsDescription;
    bool found;

    this->checkNotEndOfFile("MFrontModelParserCommon::registerBounds");

    boundsDescription.lineNumber = this->current->line;
    boundsDescription.varName = this->current->value;
    boundsDescription.varNbr  = 0u;
	
    found = false;
    for(p=this->outputs.begin();(p!=this->outputs.end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
      }
    }
    for(p=this->inputs.begin();(p!=this->inputs.end())&&(!found);++p){
      if(p->name==boundsDescription.varName){
	found=true;
      }
    }
    if(!found){
      this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
			      this->current->value+" is not a valid identifier.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::registerBounds","in");
    this->checkNotEndOfFile("MFrontModelParserCommon::registerBounds ",
			    "Expected ']' or '['.");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontModelParserCommon::registerBounds ",
			      "Expected '*'.");
      if(this->current->value!="*"){
	this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      boundsDescription.boundsType = VariableBoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("MFrontModelParserCommon::registerBounds ",
			      "Expected lower bound value for variable "+boundsDescription.varName);
      istringstream converter(this->current->value);
      converter >> boundsDescription.lowerBound;
      boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
    } else {
      this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::registerBounds",":");
    this->checkNotEndOfFile("MFrontModelParserCommon::registerBounds",
			    "Could not read upper bound value for variable "+boundsDescription.varName);
    if(this->current->value=="*"){
      if(boundsDescription.boundsType==VariableBoundsDescription::Upper){
	this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
				"Upper and lower values bounds are both infinity. This is inconsistent.");
      }
      boundsDescription.boundsType=VariableBoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("MFrontModelParserCommon::registerBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> boundsDescription.upperBound;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
      if(boundsDescription.boundsType==VariableBoundsDescription::LowerAndUpper){
	if(boundsDescription.lowerBound>boundsDescription.upperBound){
	  this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
				  "Lower bound value is greater than upper bound value for variable '"+
				  boundsDescription.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontModelParserCommon::registerBounds",
			      "Expected ']'");
      if(this->current->value!="]"){
	this->throwRuntimeError("MFrontModelParserCommon::registerBounds",
				"Expected ']' (read '"+this->current->value+"'");
      }      
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontModelParserCommon::registerBounds",";");
    container.push_back(boundsDescription);
  } // end of MFrontModelParserCommon::registerBounds

  std::map<std::string,std::vector<std::string> >
  MFrontModelParserCommon::getGlobalIncludes(void)
  {
    using namespace std;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    typedef map<string,vector<string> > Map;
    Map incs;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      const Map& iincs = interface->getGlobalIncludes(*this);
      for(p=iincs.begin();p!=iincs.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(incs[p->first]));
      }
    }
    return incs;
  } // end of MFrontModelParserCommon::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontModelParserCommon::getGlobalDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    Map deps;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      const Map& ideps = interface->getGlobalDependencies(*this);
      for(p=ideps.begin();p!=ideps.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(deps[p->first]));
      }
    }
    return deps;
  } // end of MFrontModelParserCommon::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontModelParserCommon::getGeneratedSources(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    Map osources;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      const Map& isources = interface->getGeneratedSources(*this);
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
  } // end of MFrontModelParserCommonCommon::getGeneratedSources

  std::vector<std::string>
  MFrontModelParserCommon::getGeneratedIncludes(void)
  {
    using namespace std;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    vector<string> incs;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      const vector<string>& iincs = interface->getGeneratedIncludes(*this);
      copy(iincs.begin(),iincs.end(),back_inserter(incs));
    }
    return incs;
  } // end of MFrontModelParserCommon::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontModelParserCommon::getLibrariesDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    vector<string>::const_iterator p3;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*i);
      const Map& ideps = interface->getLibrariesDependencies(*this);
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
  } // end of MFrontModelParserCommon::getLibrariesDependencies

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontModelParserCommon::getSpecificTargets(void)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontModelParserCommon::getSpecificTargets(void)

} // end of mfront
