/*!
 * \file   MFrontPleiadesModelParserArchitecture1_4.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<iterator>
#include<string>
#include<stdexcept>
#include<sstream>
#include<algorithm>
#include<cctype>
#include<cassert>

#include"ParserUtilities.hxx"
#include"MFrontHeader.hxx"
#include"MFrontParserFactory.hxx"
#include"MFrontPleiadesModelParserArchitecture1.4.hxx"
#include"System/System.hxx"

namespace mfront{

  template<typename Type>
  struct MFrontPleiadesModelParserArchitecture1_4::Property
  {
    std::string name;
    std::string varName;
    Type value;
    bool setValue;
  }; // end of struct MFrontPleiadesModelParserArchitecture1_4::Property

  struct MFrontPleiadesModelParserArchitecture1_4::Function
  {
    StaticVarContainer  staticVars;
    std::set<std::string> usedVariables;
    std::set<std::string> cusedVariables;
    std::set<std::string> modifiedVariables;
    std::set<std::string> domains;
    std::set<std::string> domainsParameters;
    std::vector<std::string> orderedVariables;
    std::string name;
    std::string body;
    std::string cbody;
    unsigned short line;
    bool isExportable;
  }; // end of struct MFrontPleiadesModelParserArchitecture1_4::Function

  struct MFrontPleiadesModelParserArchitecture1_4::LoadingVariable
  {
    std::string    name;
    std::string    tabName;
    unsigned short lineNumber;
    unsigned short depth;
  }; // end of struct MFrontPleiadesModelParserArchitecture1_4::LoadingVariable

  std::string 
  MFrontPleiadesModelParserArchitecture1_4::makeUpperCase(const std::string& n)
  {
    using namespace std;
    string s(n);
    string::const_iterator p;
    string::iterator p2;
    for(p=n.begin(),p2=s.begin();p!=n.end();++p,++p2){
      *p2 = static_cast<char>(toupper(*p));
    }
    return s;
  } // end of MFrontPleiadesModelParserArchitecture1_4::makeUpperCase

  MFrontPleiadesModelParserArchitecture1_4::MFrontPleiadesModelParserArchitecture1_4(void)
    : MFrontVirtualParser(),
      exportCastem(false),
      useTabStringToDefineDomain(false)
  {
    // Call Back
    this->registerNewCallBack("@MaterialLaw",&MFrontPleiadesModelParserArchitecture1_4::treatMaterialLaw);
    this->registerNewCallBack("@Output",&MFrontPleiadesModelParserArchitecture1_4::treatOutput);
    this->registerNewCallBack("@Input",&MFrontPleiadesModelParserArchitecture1_4::treatInput);
    this->registerNewCallBack("@GlobalParameter",
			      &MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameter);
    this->registerNewCallBack("@LocalParameter",
			      &MFrontPleiadesModelParserArchitecture1_4::treatLocalParameter);
    this->registerNewCallBack("@Execute",&MFrontPleiadesModelParserArchitecture1_4::treatExecute);
    this->registerNewCallBack("@Domain",&MFrontPleiadesModelParserArchitecture1_4::treatDomain);
    this->registerNewCallBack("@Domains",&MFrontPleiadesModelParserArchitecture1_4::treatDomains);
    this->registerNewCallBack("@Function",&MFrontPleiadesModelParserArchitecture1_4::treatFunction);
    this->registerNewCallBack("@LoadingVariable",
			      &MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable);
    this->registerNewCallBack("@InitializeInput",
			      &MFrontPleiadesModelParserArchitecture1_4::treatInitializeInput);
    this->registerNewCallBack("@InitializeOutput",
			      &MFrontPleiadesModelParserArchitecture1_4::treatInitializeOutput);
    this->registerNewCallBack("@InitializeParameters",
			      &MFrontPleiadesModelParserArchitecture1_4::treatInitializeParameters);
    this->registerNewCallBack("@ExportCastem",
			      &MFrontPleiadesModelParserArchitecture1_4::treatExportCastem);
    this->registerNewCallBack("@BindDomainToParameter",
			      &MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter);
    this->registerNewCallBack("@BindDomainsToParameters",
			      &MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters);
    this->localVarsHolder.push_back(VarHandler("unsigned short","period",1u));
    this->varNames.insert("period");
    this->varNames.insert("dt");
    this->varNames.insert("ptr");
    this->varNames.insert("ptr2");
    this->varNames.insert("index");
    this->varNames.insert("nbr");
    this->varNames.insert("nodes");
    this->varNames.insert("nbrOfPeriods");
  } // end of MFrontPleiadesModelParserArchitecture1_4::MFrontPleiadesModelParserArchitecture1_4()

  void
  MFrontPleiadesModelParserArchitecture1_4::setInterfaces(const std::set<std::string>&)
  {} // end of MFrontPleiadesModelParserArchitecture1_4::setInterfaces

  void
  MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable(void)
  {
    using namespace std;
    LoadingVariable var;
    var.depth = 0u;
    string tabName;
    bool endOfTreatement=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatement)){
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable",
				"Variable given is not valid (read '"+this->current->value+"'.");
      }
      var.name = this->current->value;
      var.lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable");
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatement=true;
	++(this->current);
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable",
				"',' or ';' expected after '"+var.name+"'");
      }
      if(!(this->varNames.insert(var.name)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable",
				var.name+" already declared.");
      }
      var.tabName = var.name;
      var.tabName[0] = static_cast<char>(toupper(var.tabName[0]));
      var.tabName = "tab"+var.tabName;
      if(!(this->varNames.insert(tabName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable",
				tabName+" already declared (mfront needs to use it for defining the '"+
				var.name + "' loading variable).");
      }
      this->loadingVariables.push_back(var);
    }
    if(!endOfTreatement){
      --(this->current);
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable",
			      "expected ';' before end of file.");
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariable

  void
  MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    string param;
    bool found;
    VarContainer::const_iterator p;
    if((!this->domains.empty())||(!this->domainsParameters.empty())){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      } else {
	msg+="with @BindDomainToParameter or @BindDomainsToParameters";
      }
      msg+=".";
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter",
			      msg);
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter");
    if(this->current->flag!=Token::String){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter",
			      "Expected to read a string (read '"+this->current->value+"').");
    }
    if(this->current->value.size()<3){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter",
			      "Parameter name too short.");
    }
    param = this->current->value.substr(1,this->current->value.size()-2);
    found=false;
    for(p=this->localParameters.begin();(p!=this->localParameters.end())&&(!found);){
      if(p->name==param){
	if((p->type!="TabString")&&(p->type!="string")){
	  --(this->current);
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter",
				  "Parameter '"+p->name+"' is neither a string nor an array of string.");
	}
	found = true;
	if(p->type=="TabString"){
	  this->useTabStringToDefineDomain = true;
	}
      } else {
	++p;
      }
    }
    if(!found){
      for(p=this->globalParameters.begin();(p!=this->globalParameters.end())&&(!found);){
	if(p->name==param){
	  if((p->type!="TabString")&&(p->type!="string")){
	    --(this->current);
	    this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter",
				    "parameter '"+p->name+"' is neither a string nor an array of string.");
	  }
	  found = true;
	  if(p->type=="TabString"){
	    this->useTabStringToDefineDomain = true;
	  }
	} else {
	  ++p;
	}
      }
    }
    if(!found){
      string msg("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter : ");
      msg += "no parameter " + param + " declared";
      throw(runtime_error(msg));
    }
    this->domainsParameters.insert(param);
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatBindDomainToParameter

  void
  MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    string param;
    bool found;
    bool bend;
    VarContainer::const_iterator p;
    if((!this->domains.empty())||(!this->domainsParameters.empty())){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      } else {
	msg+="with @BindDomainToParameter or @BindDomainsToParameters";
      }
      msg+=".";
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",msg);
    }
    bend = false;
    while((this->current!=this->fileTokens.end())&&(!bend)){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",
				"Parameter name too short.");
      }
      param = this->current->value.substr(1,this->current->value.size()-2);
      found=false;
      for(p=this->localParameters.begin();(p!=this->localParameters.end())&&(!found);){
	if(p->name==param){
	  if((p->type!="TabString")&&(p->type!="string")){
	    this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",
				    "Parameter '"+p->name+"' is neither a string nor an array of string.");
	  }
	  found = true;
	  if(p->type=="TabString"){
	    this->useTabStringToDefineDomain = true;
	  }
	} else {
	  ++p;
	}
      }
      if(!found){
	for(p=this->globalParameters.begin();(p!=this->globalParameters.end())&&(!found);){
	  if(p->name==param){
	    if((p->type!="TabString")&&(p->type!="string")){
	      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",
				      "Parameter '"+p->name+"' is neither a string nor an array of string.");
	    }
	    found = true;
	    if(p->type=="TabString"){
	      this->useTabStringToDefineDomain = true;
	    }
	  } else {
	    ++p;
	  }
	}
      }
      if(!found){
	string msg("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters : ");
	msg += "no parameter " + param + " declared";
	throw(runtime_error(msg));
      }
      if(!this->domainsParameters.insert(param).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",
				"Parameter '"+this->current->value+"' already defined.");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters");
      if(this->current->value==","){
	++this->current;
      } else if(this->current->value==";"){
	bend = true;
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",
				"Expected ',' or ')' (read '"+this->current->value+"').");
      }
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters");
    if(this->domainsParameters.empty()){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters",
			      "@BindDomainsToParameters does not set any parameters");
    }
    ++(this->current);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatBindDomainsToParameters

  std::string
  MFrontPleiadesModelParserArchitecture1_4::getName(void)
  {
    return "PleiadesModel1.4";
  } // end of MFrontPleiadesModelParserArchitecture1_4::getName(void)

  std::string
  MFrontPleiadesModelParserArchitecture1_4::getDescription()
  {
    return "this parser is used to define simple material models for pleiades";
  } // end of MFrontPleiadesModelParserArchitecture1_4::getDescription

  void
  MFrontPleiadesModelParserArchitecture1_4::setVerboseMode(void) 
  {
    this->verboseMode = true;
  } // MFrontPleiadesModelParserArchitecture1_4::setVerboseMode(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::setWarningMode(void) 
  {
    this->warningMode = true;
  } // MFrontPleiadesModelParserArchitecture1_4::setVerboseMode(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::setDebugMode(void) 
  {
    this->debugMode = true;
  }

  void
  MFrontPleiadesModelParserArchitecture1_4::treatExportCastem(void)
  {
    using namespace std;
    this->exportCastem = true;
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatExportCastem",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatExportCastem

  void
  MFrontPleiadesModelParserArchitecture1_4::treatFunction(void)
  {
    using namespace std;
    Function f;
    VarContainer::iterator p;
    set<string>::const_iterator p2;
    set<string>::iterator p3;
    FunctionContainer::iterator p4;
    StaticVarContainer::const_iterator p5;
    unsigned short openedBrackets = 0;
    unsigned short openedParenthesis = 0;
    unsigned short currentLine;
    bool newLine;
    bool newInstruction;
    bool found = false;
    bool treated;
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunction");
    f.name = this->current->value;
    f.line = this->current->line;
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatFunction","{");
    ++openedBrackets;
    newInstruction = true;
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunction",
			    "Expected body of function '"+f.name+"'.");
    currentLine = this->current->line;
    newLine=true;
    if(this->debugMode){
      f.body  +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
      f.cbody +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
    }
    for(;(this->current!=this->fileTokens.end())&&
	  (openedBrackets!=0);++(this->current)){
      if(this->current->line!=currentLine){
	currentLine=this->current->line;
	f.body  += "\n";
	f.cbody += "\n";
	if(this->debugMode){
	  f.body  +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
	  f.cbody +="#line " + toString(currentLine) + " \"" + this->fileName + "\"\n";
	}
	newLine = true;
      } 
      if(this->current->value=="{"){
	++openedBrackets;
	f.body  +="{";
	f.cbody +="{";
	newInstruction = true;
      } else if(this->current->value=="}"){
	--openedBrackets;
	if(openedBrackets!=0){
	  f.body  +="}";
	  f.cbody +="}";
	}
      } else if(this->current->value=="("){
	++openedParenthesis;
	f.body  +="(";
	f.cbody +="(";
      } else if(this->current->value==")"){
	if(openedParenthesis==0){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunction",
				  "unbalanced parenthesis");
	}
	--openedParenthesis;
	f.body  += ")";
	f.cbody += ")";
      } else if(this->current->value==";"){
	f.body  += ";";
	f.cbody += ";";
	newInstruction = true;
      } else {
	if(!newLine){
	  f.body  += " ";
	  f.cbody += " ";
	}
	if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
	  // treating the case of static variables
	  found=false;
	  for(p5=f.staticVars.begin();(p5!=f.staticVars.end())&&(!found);){
	    if(p5->name==this->current->value){
	      found = true;
	    } else {
	      ++p5;
	    }
	  }
	  if(!found){
	    for(p5=this->staticVars.begin();(p5!=this->staticVars.end())&&(!found);){
	      if(p5->name==this->current->value){
		f.staticVars.push_back(*p5);
		found = true;
	      } else {
		++p5;
	      }
	    }
	  }
	  if(!found){
	    this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunction",
				    "Internal error while treating static var '"+
				    this->current->value+"' in the body of function "+
				    f.name+"'");
	  }
	  treated = true;
	  f.body  += "(" + this->className + "::" + this->current->value + ")";
	  f.cbody += this->current->value;
	} else if(this->varNames.find(this->current->value)!=this->varNames.end()){
	  treated = false;
	  if(newInstruction){
	    string var = this->current->value;
	    string op;
	    bool modifier;
	    ++(this->current);
	    if(this->current==fileTokens.end()){
	      string msg("MFrontPleiadesModelParserArchitecture1_4::treatFunction : ");
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
		this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunction",
					"trying to modify variable '"+var+
					"' in the body of function '"+f.name+"'");
	      }
	      f.body  += var + " " + op + " ";
	      f.cbody += var + " " + op + " ";
	      f.modifiedVariables.insert(var);
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
		f.body  += "(this->" + this->current->value + ")";
		f.cbody += this->current->value;
		f.cusedVariables.insert(this->current->value);
	      }
	    }
	    // treating the case of local parameters
	    for(p =this->localParameters.begin();
		(p!=this->localParameters.end())&&(!treated);++p){
	      if(p->name==this->current->value){
		treated = true;
		f.body  += "(this->" + this->current->value + ")";
		f.cbody += this->current->value;
		f.cusedVariables.insert(this->current->value);
	      }
	    }
	    // treating the case of local variables
	    for(p =this->localVarsHolder.begin();
		(p!=this->localVarsHolder.end())&&(!treated);++p){
	      if(p->name==this->current->value){
		treated = true;
		f.body  += "(this->" + this->current->value + ")";
		f.cbody += this->current->value;
		f.cusedVariables.insert(this->current->value);
	      }
	    }
	    if(!treated){
	      f.usedVariables.insert(this->current->value);
	      f.cusedVariables.insert(this->current->value);
	      f.body  += this->current->value;
	      f.cbody += this->current->value;
	    }
	  }
	} else {
	  f.body  += this->current->value;
	  f.cbody += this->current->value;
	}
	newInstruction = false;
      }
      newLine=false;
    }
    if((this->current==fileTokens.end())&&(openedBrackets!=0)){
      string msg("MFrontPleiadesModelParserArchitecture1_4::treatFunction : ");
      msg+="unexpected end of file while reading body of function " + f.name;
      throw(runtime_error(msg));
    }
    if(openedBrackets!=0){
      string msg("MFrontPleiadesModelParserArchitecture1_4::treatFunction : ");
      msg+="parenthesis still opened at the end of function " + f.name;
      throw(runtime_error(msg));
    }
    if(f.modifiedVariables.size()==0){
      string msg("MFrontPleiadesModelParserArchitecture1_4::treatFunction : ");
      msg+="function " + f.name + " does not change any variable.";
      throw(runtime_error(msg));
    }
    if(!this->functionNames.insert(f.name).second){
      string msg("MFrontPleiadesModelParserArchitecture1_4::treatFunction : ");
      msg+="function " + f.name + " already declared.";
      throw(runtime_error(msg));
    }

    for(p2=f.modifiedVariables.begin();p2!=f.modifiedVariables.end();++p2){
      if((p3=f.usedVariables.find(*p2))!=f.usedVariables.end()){
	f.usedVariables.erase(*p3);
      }
      if((p3=f.cusedVariables.find(*p2))!=f.cusedVariables.end()){
	f.cusedVariables.erase(*p3);
      }
    }
    // we check here if this function is exportable
    if(f.modifiedVariables.size()==1){
      // OK, this function is a good candidate for export.
      // We now check that is parameters are all of type "double"
      f.isExportable = true;
      for(p3=f.cusedVariables.begin();(p3!=f.cusedVariables.end())&&(f.isExportable);++p3){
	if ((*p3=="period")||(*p3=="nbr")||(*p3=="nbrOfPeriods")){
	  f.isExportable = false;
	}
      }
      // treating the case of global parameters
      for(p =this->globalParameters.begin();
	  (p!=this->globalParameters.end())&&(f.isExportable);++p){
	if(p->type!="double"){
	  f.isExportable = false;
	}
      }
      // treating the case of local parameters
      for(p =this->localParameters.begin();
	  (p!=this->localParameters.end())&&(f.isExportable);++p){
	if(p->type!="double"){
	  f.isExportable = false;
	}
      }
      // treating the case of local variables
      for(p =this->localVarsHolder.begin();
	  (p!=this->localVarsHolder.end())&&(f.isExportable);++p){
	if(p->type!="double"){
	  f.isExportable = false;
	}
      }
    }
    this->functions.push_back(f);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatFunction(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatDomain(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if((!this->domains.empty())||(!this->domainsParameters.empty())){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      } else {
	msg+="with @BindDomainToParameter or @BindDomainsToParameters";
      }
      msg+=".";
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomains",msg);
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatDomain");
    if(this->current->flag!=Token::String){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomain",
			      "Expected to read a string (read '"+this->current->value+"').");
    }
    if(this->current->value.size()<3){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomain : ",
			      "domain name too short.");
    }
    this->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second;
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatDomain",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatDomain(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatDomains(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    bool bend;
    if((!this->domains.empty())||(!this->domainsParameters.empty())){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      } else {
	msg+="with @BindDomainToParameter or @BindDomainsToParameters";
      }
      msg+=".\n";
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomains",msg);
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatDomains");
    bend = false;
    while((this->current!=this->fileTokens.end())&&(!bend)){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomains",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomains",
				"Domain name too short.");
      }
      if(!this->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomains",
				"domain "+this->current->value.substr(1,this->current->value.size()-2)+
				" already defined.");
      }
      ++(this->current);	
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatDomains",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++this->current;
      } else if(this->current->value==";"){
	bend = true;
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomains",
				"Expected ',' or ')' (read '"+this->current->value+"').");
      }
    }
    if(this->domains.empty()){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatDomains",
			      "@Domains does not set any domain.");
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatDomains");
    ++(this->current);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatDomains(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatInitializeInput(void)
  {
    this->initializeInput = this->readNextBlock(true);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatInitializeInput(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatInitializeOutput(void)
  {
    this->initializeOutput = this->readNextBlock(true);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatInitializeOutput(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatInitializeParameters(void)
  {
    this->initializeParameters = this->readNextBlock(true);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatInitializeParameters(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    string var;
    FunctionContainer::iterator p;
    vector<string>::const_iterator p2;
    set<string>::const_iterator p3;
    VarContainer::const_iterator p4;
    string param;
    bool found;
    bool bend;
    found=false;
    for(p=this->functions.begin();(p!=this->functions.end())&&(!found);){
      if(p->name==this->currentVar){
	found = true;
      } else {
	++p;
      }
    }
    if(!found){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
			      "Internal error, no function named '"+this->currentVar+"'");
    }
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod");
    methodName = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod");
    if(this->current->value!="("){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
			      "Unexpected token (expected '(', read '"+this->current->value+"').");
    }
    if(methodName=="setDomain"){
      if((!p->domains.empty())||(!p->domainsParameters.empty())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"The domain of the function '"+this->currentVar+
				"' has already been defined.");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
			      "Expected domain name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Expected to read a string");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Domain name too short.");
      }
      p->domains.insert(this->current->value.substr(1,this->current->value.size()-2));
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
			      "Expected ')'.");
      if(this->current->value!=")"){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Unexpected token (expected ')', read '"+this->current->value+"').");
      }
    } else if(methodName=="setDomains"){
      if((!p->domains.empty())||(!p->domainsParameters.empty())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"The domain of the function '"+this->currentVar+"' has already been defined.");
      }
      ++(this->current);
      bend = false;
      while((this->current!=this->fileTokens.end())&&(!bend)){
	if(this->current->flag!=Token::String){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Expected to read a string (read '"+this->current->value+"').");
	}
	if(this->current->value.size()<3){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Domain name too short.");
	}
	if(!p->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Domain "+this->current->value.substr(1,this->current->value.size()-2)+
				  " already defined.");
	}
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod");
	if(this->current->value==","){
	  ++this->current;
	} else if(this->current->value==")"){
	  bend = true;
	} else {
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Expected ',' or ')' (read '"+this->current->value+"').");
	}
      }
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod");
      if(p->domains.empty()){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"setDomains does not set any domain for function '"+this->currentVar+"'");
      }
    } else if(methodName=="bindDomainToParameter"){
      if((!p->domains.empty())||(!p->domainsParameters.empty())){
	string msg("Domain has already been defined ");
	if(!p->domains.empty()){
	  msg+="with setDomain or setDomains";
	} else {
	  msg+="with bindDomainToParameter or bindDomainsToParameters";
	}
	msg+=".";
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				msg);
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
			      "Expected parameter name");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Variable name empty.");
      }
      param = this->current->value.substr(1,this->current->value.size()-2);
      found=false;
      for(p4=this->localParameters.begin();(p4!=this->localParameters.end())&&(!found);){
	if(p4->name==param){
	  if((p4->type!="TabString")&&(p4->type!="string")){
	    --(this->current);
	    this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				    "Parameter "+p4->name+" is neither a string nor an array of string.");
	  }
	  found = true;
	  if(p4->type=="TabString"){
	    this->useTabStringToDefineDomain = true;
	  }
	} else {
	  ++p4;
	}
      }
      if(!found){
	for(p4=this->globalParameters.begin();(p4!=this->globalParameters.end())&&(!found);){
	  if(p4->name==param){
	    if((p4->type!="TabString")&&(p4->type!="string")){
	      --(this->current);
	      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				      "Parameter "+p4->name+" is neither a string nor an array of string.");
	    }
	    found = true;
	    if(p4->type=="TabString"){
	      this->useTabStringToDefineDomain = true;
	    }
	  } else {
	    ++p4;
	  }
	}
      }
      if(!found){
	string msg("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod : ");
	msg += "no parameter " + param + " declared";
	throw(runtime_error(msg));
      }
      p->domainsParameters.insert(param);
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
			      "Expected ')'.");
      if(this->current->value!=")"){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Unexpected token (expected ')', read '"+this->current->value+"').");
      }
    } else if (methodName=="bindDomainsToParameters"){
      if((!p->domains.empty())||(!p->domainsParameters.empty())){
	string msg("Domain has already been defined ");
	if(!p->domains.empty()){
	  msg+="with setDomain or setDomains";
	} else {
	  msg+="with bindDomainToParameter or bindDomainsToParameters";
	}
	msg+=".";
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",msg);
      }
      ++(this->current);
      bend = false;
      while((this->current!=this->fileTokens.end())&&(!bend)){
	if(this->current->flag!=Token::String){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Expected to read a string (read '"+this->current->value+"').");
	}
	if(this->current->value.size()<3){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Domain name too short.");
	}
	param = this->current->value.substr(1,this->current->value.size()-2);
	found=false;
	for(p4=this->localParameters.begin();(p4!=this->localParameters.end())&&(!found);){
	  if(p4->name==param){
	    if((p4->type!="TabString")&&(p4->type!="string")){
	      --(this->current);
	      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				      "Parameter '"+p4->name+"' is neither a string nor an array of string.");
	    }
	    found = true;
	    if(p4->type=="TabString"){
	      this->useTabStringToDefineDomain = true;
	    }
	  } else {
	    ++p4;
	  }
	}
	if(!found){
	  for(p4=this->globalParameters.begin();(p4!=this->globalParameters.end())&&(!found);){
	    if(p4->name==param){
	      if((p4->type!="TabString")&&(p4->type!="string")){
		--(this->current);
		this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
					"Parameter '"+p4->name+"' is neither a string nor an array of string.");
	      }
	      found = true;
	      if(p4->type=="TabString"){
		this->useTabStringToDefineDomain = true;
	      }
	    } else {
	      ++p4;
	    }
	  }
	}
	if(!found){
	  string msg("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod : ");
	  msg += "no parameter " + param + " declared";
	  throw(runtime_error(msg));
	}
	if(!p->domainsParameters.insert(param).second){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Domain "+this->current->value.substr(1,this->current->value.size()-2)+
				  " already defined.");
	}
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Expected ',' or ')'.");
	if(this->current->value==","){
	  ++this->current;
	} else if(this->current->value==")"){
	  bend = true;
	} else {
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Expected ',' or ')' (read '"+this->current->value+"').");
	}
      }
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod");
      if(p->domainsParameters.empty()){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"bindDomainsToParameters does not set any parameter for function '"+
				this->currentVar+"'");
      }
    } else if(methodName=="setVariablesOrder"){
      if(p->modifiedVariables.size()!=1u){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"The method setVariableOrder is only applicable for exportable functions.");
      }
      if(!p->orderedVariables.empty()){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"The order of the variables of the function '"+
				p->name+"' has already been defined.");
      }
      ++(this->current);
      bend = false;
      while((this->current!=this->fileTokens.end())&&(!bend)){
	if(this->current->flag!=Token::String){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Expected to read a string (read '"+this->current->value+"').");
	}
	if(this->current->value.size()<3){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Cariable name empty.");
	}
	var = this->current->value.substr(1,this->current->value.size()-2);
	p2=find(p->orderedVariables.begin(),p->orderedVariables.end(),var);
	if(p2!=p->orderedVariables.end()){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Variable '"+var+"' multiply defined.");
	}
	if((p3=p->cusedVariables.find(var))==p->cusedVariables.end()){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Variable '"+var+"' is not a variable of function '"+p->name+"'.");
	}
	p->orderedVariables.push_back(var);
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				"Expected ',' or ')'.");
	if(this->current->value==","){
	  ++this->current;
	} else if(this->current->value==")"){
	  bend = true;
	} else {
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Expected ',' or ')' (read '"+this->current->value+"').");
	}
      }
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod");
      // Some sanity checks
      for(p3=p->cusedVariables.begin();p3!=p->cusedVariables.end();++p3){
	p2 = find(p->orderedVariables.begin(),p->orderedVariables.end(),*p3);
	if(p2==p->orderedVariables.end()){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
				  "Variable '"+*p3+"' is not defined in the ordered variable list.");
	}
      }
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",
			      "Unknown function method (read '"+methodName+"')."
			      "Valid methods are setDomain, setDomains and setVariablesOrder.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod(void) 

  void
  MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    typedef map<string,string>::value_type MVType;
    typedef map<string,double>::value_type MVType2;
    typedef map<string,unsigned short>::value_type MVType3;
    typedef map<string,vector<Property<double> > >::value_type MVType4;
    typedef map<string,vector<Property<string> > >::value_type MVType5;
    map<string,vector<Property<double> > >::iterator p;
    map<string,vector<Property<string> > >::iterator p2;
    unsigned short i;
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setInitialValue")&&
       (methodName!="setDepth")          &&
       (methodName!="setDoubleProperty") &&
       (methodName!="setStringProperty")){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Unknown method (valid methods for fields are "
			      "setGlossaryName, setEntryName, setDepth, setDoubleProperty, "
			      "setStringProperty and setInitialValue"
			      ", read '"+methodName+"').");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar + "'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Entry file name for field '"+this->currentVar+"' already defined.");
      }
    }  else if (methodName=="setInitialValue"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected intial value.");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Could not read initial value of field : '"+this->currentVar+"'");
      }
      if(!this->initialValues.insert(MVType2(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Initial value for field '"+this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected depth value.");
      unsigned short value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Could not read initial value of field '"+this->currentVar+"'");
      }
      if(!this->depth.insert(MVType3(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Initial value for field '"+this->currentVar+"' already defined.");
      }
      for(i=1;i<=value;++i){
	if(!this->varNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				  this->currentVar+"_"+toString(i)+" has already been declared.");
	}
	if(!this->fieldNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				  "Field '"+this->currentVar+"_"+toString(i)+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    } else if (methodName=="setDoubleProperty"){
      Property<double> property;
      property.setValue = true;
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected property name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Expected a string as property name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Glossary name too short.");
      }
      property.name     = this->current->value.substr(1,this->current->value.size()-2);
      property.varName  = this->currentVar + "_";
      property.varName += this->current->value.substr(1,this->current->value.size()-2);
      if(!this->varNames.insert(property.varName).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Variable name '"+property.varName+"' already exist.");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++(this->current);
	if(this->current==this->fileTokens.end()){
	  --(this->current);
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				  "Unexpected end of file (expected property value)");
	}
	istringstream converter(this->current->value);
	converter >> property.value;
	if(!converter&&(!converter.eof())){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				  "Could not read initial value of property '"+property.name+"'");
	}
      } else if (this->current->value==")"){
	property.value = 0.;
	--(this->current);
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Expected ',' or ')'.");
      }
      p=this->fieldDoubleProperties.find(this->currentVar);
      if(p!=this->fieldDoubleProperties.end()){
	p->second.push_back(property);
      } else {
	p = this->fieldDoubleProperties.insert(MVType4(this->currentVar,vector<Property<double> >())).first;
	p->second.push_back(property);
      }
    } else if (methodName=="setStringProperty"){
      Property<string> property;
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected property name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Glossary name too short.");
      }
      property.name  = this->current->value.substr(1,this->current->value.size()-2);
      property.varName  = this->currentVar + "_";
      property.varName += this->current->value.substr(1,this->current->value.size()-2);
      if(!this->varNames.insert(property.varName).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Variable name '"+property.varName+"' already exist.");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Expected property value.");
	if(this->current->flag!=Token::String){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				  "Expected a string as property name.");
	}
	if(this->current->value.size()<3){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				  "Glossary name too short.");
	}
	property.value = this->current->value.substr(1,this->current->value.size()-2);
      } else if (this->current->value==")"){
	property.value = "";
	--(this->current);
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
				"Expected ',' or ')'.");
      }
      p2=this->fieldStringProperties.find(this->currentVar);
      if(p2!=this->fieldStringProperties.end()){
	p2->second.push_back(property);
      } else {
	p2 = this->fieldStringProperties.insert(MVType5(this->currentVar,vector<Property<string> >())).first;
	p2->second.push_back(property);
      }
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod

  void
  MFrontPleiadesModelParserArchitecture1_4::treatInputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    typedef map<string,string>::value_type MVType;
    typedef map<string,unsigned short>::value_type MVType2;
    typedef map<string,vector<Property<double> > >::value_type MVType3;
    typedef map<string,vector<Property<string> > >::value_type MVType4;
    map<string,vector<Property<double> > >::iterator p;
    map<string,vector<Property<string> > >::iterator p2;
    unsigned short i;
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName") &&
       (methodName!="setDepth")&&
       (methodName!="setDoubleProperty")&&(methodName!="setStringProperty")){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Unknown method (valid methods for input fields are "
			      "setGlossaryName, setEntryName, setDepth"
			      ", setDoubleProperty and setStringProperty"
			      ", read '"+methodName+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Glossary name for field '"+this->currentVar+"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Expected depth value.");
      unsigned short value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Could not read initial value of field '"+this->currentVar+"'");
      }
      if(!this->depth.insert(MVType2(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Initial value for field '"+this->currentVar+"' already defined.");
      }
      for(i=1;i<=value;++i){
	if(!this->varNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				  this->currentVar+"_"+toString(i)+" has already been declared.");
	}
	if(!this->fieldNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				  "Field '"+this->currentVar+"_"+toString(i)+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    }  else if (methodName=="setDoubleProperty"){
      Property<double> property;
      property.setValue = true;
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Expected property name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected a string as property name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Glossary name too short.");
      }
      property.name  = this->current->value.substr(1,this->current->value.size()-2);
      property.varName  = this->currentVar + "_";
      property.varName += this->current->value.substr(1,this->current->value.size()-2);
      if(!this->varNames.insert(property.varName).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Variable name '"+property.varName+"' already exist.");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected property value");
	istringstream converter(this->current->value);
	converter >> property.value;
	if(!converter&&(!converter.eof())){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				  "Could not read initial value of property '"+property.name+"'");
	}
      } else if (this->current->value==")"){
	property.value = 0.;
	property.setValue = false;
	--(this->current);
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected ',' or ')'.");
      }
      p=this->fieldDoubleProperties.find(this->currentVar);
      if(p!=this->fieldDoubleProperties.end()){
	p->second.push_back(property);
      } else {
	p = this->fieldDoubleProperties.insert(MVType3(this->currentVar,vector<Property<double> >())).first;
	p->second.push_back(property);
      }
    } else if (methodName=="setStringProperty"){
      Property<string> property;
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Expected property name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected a string as property name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Glossary name too short.");
      }
      property.name  = this->current->value.substr(1,this->current->value.size()-2);
      property.varName  = this->currentVar + "_";
      property.varName += this->current->value.substr(1,this->current->value.size()-2);
      if(!this->varNames.insert(property.varName).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Variable name '" + property.varName + "' already exist.");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected property value");
	if(this->current->flag!=Token::String){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				  "Expected a string as glossary name.");
	}
	if(this->current->value.size()<3){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				  "Glossary name too short.");
	}
	property.value = this->current->value.substr(1,this->current->value.size()-2);
      } else if (this->current->value==")"){
	property.value = "";
	property.setValue = false;
	--(this->current);
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
				"Expected ',' or ')'.");
      }
      p2=this->fieldStringProperties.find(this->currentVar);
      if(p2!=this->fieldStringProperties.end()){
	p2->second.push_back(property);
      } else {
	p2 = this->fieldStringProperties.insert(MVType4(this->currentVar,vector<Property<string> >())).first;
	p2->second.push_back(property);
      } 
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatInputMethod",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatInputMethod

  void
  MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod(void) 
  {
    using namespace std; 
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
			      "Unknown method (valid methods for global parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read '"+methodName+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod"
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod

  void
  MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    vector<LoadingVariable>::iterator p;
    unsigned short i;
    bool found = false;
    for(p=this->loadingVariables.begin();(p!=this->loadingVariables.end())&&(!found);){
      if(p->name==currentVar){
	found = true;
      } else {
	++p;
      }
    }
    assert(p!=this->loadingVariables.end());
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setEntryName")&&
       (methodName!="setDepth")){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
			      "Unknown method (valid methods for loading variable are "
			      "setEntryName and setDepth"
			      ", read '"+methodName+"').");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod","(");
    if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setDepth"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
			      "Expected depth value.");
      unsigned short value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
				"Could not read depth value");
      }
      p->depth = value;
      for(i=1;i<=value;++i){
	if(!this->varNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",
				  this->currentVar+"_"+toString(i)+" has already been declared.");
	}
      }
    } else {
      assert(false);
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod

  void
  MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
			      "Unknown method (valid methods for local parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read "+methodName+").");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'.");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod",";");
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod

  void
  MFrontPleiadesModelParserArchitecture1_4::treatFile(const std::string& fileName_) 
  {
    using namespace std;
    this->fileName = fileName_;
    this->openFile(fileName_);
    CallBackContainer::const_iterator p;
    MemberFuncPtr handler = 0;
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->fileTokens.begin();
    while(this->current != this->fileTokens.end()){
      p = this->callBacks.find(this->current->value);
      if(p==this->callBacks.end()){
	VarContainer::const_iterator p2;
	FunctionContainer::const_iterator p3;
	vector<LoadingVariable>::const_iterator p4;
	bool found = false;
	for(p3=this->functions.begin();(p3!=this->functions.end())&&(!found);){
	  if(p3->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParserArchitecture1_4::treatFunctionMethod;
	  } else {
	    ++p3;
	  }
	}
	for(p2=this->outputs.begin();(p2!=this->outputs.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParserArchitecture1_4::treatOutputMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->inputs.begin();(p2!=this->inputs.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParserArchitecture1_4::treatInputMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->globalParameters.begin();(p2!=this->globalParameters.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->localParameters.begin();(p2!=this->localParameters.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParserArchitecture1_4::treatLocalParameterMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p4=this->loadingVariables.begin();(p4!=this->loadingVariables.end())&&(!found);){
	  if(p4->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParserArchitecture1_4::treatLoadingVariableMethod;
	  } else {
	    ++p4;
	  }
	}
	if(!found){
	  handler = &MFrontPleiadesModelParserArchitecture1_4::treatUnknownKeyword;
	}
      } else {
	handler = p->second;
      }
      ++(this->current);
      (this->*handler)();
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatFile

  void
  MFrontPleiadesModelParserArchitecture1_4::openOutputFiles(void)
  {
    using namespace std;
    using namespace tfel::system;
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/PMetier");
    systemCall::mkdir("include/Pleiades/PMetier/PModels");
    systemCall::mkdir("src");
    this->headerFile.open(this->headerFileName.c_str());
    if(!this->headerFile){
      string msg("MFrontPleiadesModelParserArchitecture1_4::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MFrontPleiadesModelParserArchitecture1_4::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    if(this->exportCastem){
      this->castemHeaderFile.open(this->castemHeaderFileName.c_str());
      if(!this->castemHeaderFile){
	string msg("MFrontPleiadesModelParserArchitecture1_4::openOutputFiles : ");
	msg += "unable to open ";
	msg += this->castemHeaderFileName;
	msg += " for writing output file.";
	throw(runtime_error(msg));
      }
      this->castemSrcFile.open(this->castemSrcFileName.c_str());
      if(!this->castemSrcFile){
	string msg("MFrontPleiadesModelParserArchitecture1_4::openOutputFiles : ");
	msg += "unable to open ";
	msg += this->castemSrcFileName;
	msg += " for writing output file.";
	throw(runtime_error(msg));
      }
      this->castemHeaderFile.exceptions(ios::badbit|ios::failbit);
      this->castemSrcFile.exceptions(ios::badbit|ios::failbit);
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::openOutputFiles()
  
  void
  MFrontPleiadesModelParserArchitecture1_4::closeOutputFiles(void)
  {
    this->headerFile.close();
    this->srcFile.close();
    if(this->exportCastem){
      this->castemHeaderFile.close();
      this->castemSrcFile.close();
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::closeOutputFiles()
  
  void
  MFrontPleiadesModelParserArchitecture1_4::generateOutputFiles(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    this->hasDefaultConstructor=true;
    if(!this->localParameters.empty()){
      for(p=this->localParameters.begin();
	  (p!=this->localParameters.end())&&(this->hasDefaultConstructor);++p){
	if(this->defaultValues.find(p->name)==this->defaultValues.end()){
	  this->hasDefaultConstructor = false;
	}
      }
    }
    this->writeHeaderFile();
    this->writeSrcFile();
    if(this->exportCastem){
      this->writeCastemHeaderFile();
      this->writeCastemSrcFile();
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::generateOutputFiles(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::writeCastemHeaderFile(void)
  {
    using namespace std;
    FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    vector<string>::const_iterator p3;
    string fName;
    string tmp;

    this->castemHeaderFile << "/*!" << endl;
    this->castemHeaderFile << "* \\file   " << this->castemHeaderFileName  << endl;
    this->castemHeaderFile << "* \\brief  " << "this file exports the functions of the " 
			   << this->className << " Pleiades model for use in castem.\n";
    this->castemHeaderFile << "*         File generated by ";
    this->castemHeaderFile << MFrontHeader::getVersionName() << " ";
    this->castemHeaderFile << "version " << MFrontHeader::getVersionNumber();
    this->castemHeaderFile << endl;
    if(!this->authorName.empty()){
      this->castemHeaderFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->castemHeaderFile << "* \\date   " << this->date       << endl;
    }
    this->castemHeaderFile << " */\n\n";
    
    this->castemHeaderFile << "#ifndef _" 
			   << MFrontPleiadesModelParserArchitecture1_4::makeUpperCase(this->className)
			   << "_H\n";
    this->castemHeaderFile << "#define _"
			   << MFrontPleiadesModelParserArchitecture1_4::makeUpperCase(this->className)
			   << "_H\n\n";
    if(!this->includes.empty()){
      this->castemHeaderFile << this->includes << endl << endl;
    }
    this->castemHeaderFile << "#ifdef __cplusplus\n";
    this->castemHeaderFile << "extern \"C\"{\n";
    this->castemHeaderFile << "#endif /* __cplusplus */\n\n";
    for(p=this->functions.begin();p!=this->functions.end();++p){
      if(p->isExportable){
	if(this->debugMode){
	  this->castemHeaderFile << "#line " << p->line << " \"" 
				 << this->fileName << "\"\n";
	}
	fName = this->className;
	fName[0] = static_cast<char>(tolower(fName[0]));
	tmp   = p->name;
	tmp[0] = static_cast<char>(toupper(tmp[0]));
	fName += tmp;
	this->castemHeaderFile << "double\n";
	this->castemHeaderFile << fName << "(";
	this->castemHeaderFile << "const double * const);\n\n";
      }
    }
    this->castemHeaderFile << "#ifdef __cplusplus\n";
    this->castemHeaderFile << "} // end of extern \"C\"\n";
    this->castemHeaderFile << "#endif /* __cplusplus */\n\n";
    this->castemHeaderFile << "#endif /* _"
			   << MFrontPleiadesModelParserArchitecture1_4::makeUpperCase(this->className)
			   << "_H */\n";

  } // end of MFrontPleiadesModelParserArchitecture1_4::writeCastemHeaderFile()

  void
  MFrontPleiadesModelParserArchitecture1_4::writeCastemSrcFile(void)
  {
    using namespace std;
    FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    StaticVarContainer::const_iterator p3;
    vector<string>::const_iterator p4;
    string fName;
    string tmp;
    unsigned short i;

    this->castemSrcFile << "/*!" << endl;
    this->castemSrcFile << "* \\file   " << this->castemSrcFileName  << endl;
    this->castemSrcFile << "* \\brief  " << "this file exports the function of the " 
			<< this->className << " Pleiades Model for use in castem.\n";
    this->castemSrcFile << "*         File generated by ";
    this->castemSrcFile << MFrontHeader::getVersionName() << " ";
    this->castemSrcFile << "version " << MFrontHeader::getVersionNumber();
    this->castemSrcFile << endl;
    if(!this->authorName.empty()){
      this->castemSrcFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->castemSrcFile << "* \\date   " << this->date       << endl;
    }
    this->castemSrcFile << " */\n\n";
    // include
    this->castemSrcFile << "#include<cmath>\n";
    this->castemSrcFile << "#include\"" << this->className << "-castem.hh\"\n\n";
    this->castemSrcFile << "#ifdef __cplusplus\n";
    this->castemSrcFile << "extern \"C\"{\n";
    this->castemSrcFile << "#endif /* __cplusplus */\n\n";
    // Functions
    for(p=this->functions.begin();p!=this->functions.end();++p){
      fName = this->className;
      fName[0] = static_cast<char>(tolower(fName[0]));
      tmp   = p->name;
      tmp[0] = static_cast<char>(toupper(tmp[0]));
      fName += tmp;
      if(p->isExportable){
	this->castemSrcFile << "// number of argument of the function\n";
	this->castemSrcFile << "unsigned short " << fName 
			    << "_nargs = " << p->cusedVariables.size() << "u;\n\n";
	this->castemSrcFile << "double\n";
	this->castemSrcFile << fName << "(";
	this->castemSrcFile << "const double * const castem_params)\n";
	this->castemSrcFile << "{\n";
	// static Variables
	if(!p->staticVars.empty()){
	  for(p3=p->staticVars.begin();p3!=p->staticVars.end();++p3){
	    if(this->debugMode){
	      this->castemSrcFile << "#line " << p3->lineNumber << " \"" 
				  << this->fileName << "\"\n";
	    }
	    if(p3->type=="short"){
	      this->castemSrcFile << "const short "  << p3->name 
				  << " = " << static_cast<short>(p3->value) << ";" << endl;
	    } else if(p3->type=="ushort"){
	      this->castemSrcFile << "const unsigned short "  << p3->name 
				  << " = " << static_cast<unsigned short>(p3->value) 
				  << ";" << endl;
	    } else if(p3->type=="int"){
	      this->castemSrcFile << "const int "  << p3->name 
				  << " = " << static_cast<int>(p3->value) << ";" << endl;
	    } else if(p3->type=="uint"){
	      this->castemSrcFile << "const unsigned int "  << p3->name 
				  << " = " << static_cast<unsigned int>(p3->value) << ";" << endl;
	    } else if(p3->type=="long"){
	      this->castemSrcFile << "const long "  << p3->name 
				  << " = " << static_cast<long>(p3->value) << ";" << endl;
	    } else if(p3->type=="ulong"){
	      this->castemSrcFile << "const unsigned long "  << p3->name 
				  << " = " << static_cast<unsigned long>(p3->value) << ";" << endl;
	    } else if(p3->type=="float"){
	      this->castemSrcFile << "const float "  << p3->name 
				  << " = " << static_cast<float>(p3->value) << ";" << endl;
	    } else if(p3->type=="double"){
	      this->castemSrcFile << "const double "  << p3->name 
				  << " = " << static_cast<double>(p3->value) << ";" << endl;
	    } else if(p3->type=="ldouble"){
	      this->castemSrcFile << "const long double " << p3->name 
				  << " = " << static_cast<long double>(p3->value) << ";" << endl;
	    } else {
	      string msg("MFrontPleiadesModelParserArchitecture1_4::writeCastemSrcFile : ");
	      msg += "type " + p3->type + " is not a supported type for a static variable.";
	      msg += "Supported types are short, ushort, int, uint, long, ulong,";
	      msg += "float, double and ldouble.\n";
	      msg += "Error at line " + toString(p3->lineNumber);
	      throw(runtime_error(msg));
	    }
	  }
	  this->castemSrcFile << endl;
	}
	if(p->cusedVariables.size()==0){
	  this->castemSrcFile << "(void) castem_params;\n";
	} else {
	  if(!p->orderedVariables.empty()){
	    i=0;
	    for(p4=p->orderedVariables.begin(),i=0;p4!=p->orderedVariables.end();++i,++p4){
	      this->castemSrcFile << "const double " << *p4 << " = ";
	      if(i==0){
		this->castemSrcFile << "*castem_params;\n";
	      } else {
		this->castemSrcFile << "*(castem_params+"+toString(i)+");\n";
	      }
	    }
	  } else {
	    for(p2=p->cusedVariables.begin(),i=0;p2!=p->cusedVariables.end();++i,++p2){
	      this->castemSrcFile << "const double " << *p2 << " = ";
	      if(i==0){
		this->castemSrcFile << "*castem_params;\n";
	      } else {
		this->castemSrcFile << "*(castem_params+"+toString(i)+");\n";
	      }
	    }
	  }
	}
	this->castemSrcFile << "double " << *(p->modifiedVariables.begin()) << ";\n";
	this->castemSrcFile << p->cbody;
	this->castemSrcFile << "return " << *(p->modifiedVariables.begin()) << ";\n";
	this->castemSrcFile << "} // end of " << fName << "\n\n";
      }
    }
    this->castemSrcFile << "#ifdef __cplusplus\n";
    this->castemSrcFile << "} // end of extern \"C\"\n";
    this->castemSrcFile << "#endif /* __cplusplus */\n\n";
  } // end of MFrontPleiadesModelParserArchitecture1_4::writeCastemSrcFile()

  void
  MFrontPleiadesModelParserArchitecture1_4::writeHeaderFile(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    StaticVarContainer::const_iterator p2;
    FunctionContainer::const_iterator p3;
    set<string>::iterator p4;
    set<string>::iterator p5;
    vector<LoadingVariable>::const_iterator p6;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << this->className << " PleiadesModel.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!this->authorName.empty()){
      this->headerFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->headerFile << "* \\date   " << this->date       << endl;
    }
    if(!this->description.empty()){
      this->headerFile << this->description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _PLEIADES_" 
		     << MFrontPleiadesModelParserArchitecture1_4::makeUpperCase(this->className)
		     << "_HH\n";
    this->headerFile << "#define _PLEIADES_"
		     << MFrontPleiadesModelParserArchitecture1_4::makeUpperCase(this->className)
		     << "_HH\n\n";

    this->headerFile << "#include<string>\n\n";
    this->headerFile << "#include\"Pleiades/PMetier/IncPMetier.hh\"\n";
    this->headerFile << "#include\"Pleiades/PMetier/PModels/MEDModel.hh\"\n\n";
    if(!this->includes.empty()){
      this->headerFile << this->includes << endl << endl;
    }
    this->headerFile << "namespace Pleiades\n{\n";
    this->headerFile << "namespace PMetier\n{\n";
    this->headerFile << "namespace PModels\n{\n\n";
    this->headerFile << "struct " << this->className << endl;
    this->headerFile << ": public IModel,\n";
    this->headerFile << "public MEDModel\n";
    this->headerFile << "{\n\n";
    if(!this->localParameters.empty()){
      this->headerFile << "//! Constructor to initialize local parameters\n";
      this->headerFile << this->className
		       << "(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    }
    if(this->hasDefaultConstructor){
      this->headerFile << "//! Default constructor\n";
      this->headerFile << this->className << "();\n\n";
    }
    this->headerFile << "bool\n"
		     << "initializeOutput(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    this->headerFile << "bool\n"
		     << "initializeInput(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    this->headerFile << "bool\n"
		     << "initializeParameters(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    this->headerFile << "bool\n"
		     << "initialize(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    this->headerFile << "bool\n"
		     << "execute(void);\n\n";
    this->headerFile << "void\n"
		     << "executeInitialPostProcessingTasks(const bool);\n\n";
    this->headerFile << "void\n"
		     << "executePostProcessingTasks(const bool);\n\n";
    this->headerFile << "private:\n\n";
    this->headerFile << "typedef std::string string;\n";
    this->headerFile << "typedef Pleiades::PUtilitaires::TabString TabString;\n";
    this->headerFile << "typedef Pleiades::PUtilitaires::TabDouble TabDouble;\n";
    this->headerFile << "typedef Pleiades::PMetier::PField::PtrIFieldDouble PleiadesField;\n\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "//! Copy constructor (disabled)\n";
    this->headerFile << this->className << "(const " 
		     << this->className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << this->className << "&\n"
		     << "operator=(const " 
		     << this->className << "&);\n\n";
    // Treat functions
    for(p3=this->functions.begin();p3!=this->functions.end();++p3){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelParserArchitecture1_4::writeHeaderFile : ");
	msg+="function " + p3->name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << this->fileName << "\"\n";
      }
      if(p3->modifiedVariables.size()==1){
	this->headerFile << "inline\n";
	this->headerFile << "const double\n";
	this->headerFile << p3->name << "(";
      } else {
	this->headerFile << "inline\n";
	this->headerFile << "void\n";
	this->headerFile << p3->name << "(";
	for(p4=p3->modifiedVariables.begin();p4!=p3->modifiedVariables.end();){
	  this->headerFile << "double&";
	  if((++p4)!=p3->modifiedVariables.end()){
	    this->headerFile << ",";
	  }
	}
      }
      if(p3->usedVariables.size()==0){
	if(p3->modifiedVariables.size()==1){
	  this->headerFile << "void";
	}
      } else {
	if(p3->modifiedVariables.size()!=1){
	  this->headerFile << ",";
	}
	for(p4=p3->usedVariables.begin();p4!=p3->usedVariables.end();){
	  this->headerFile << "const double";
	  if((++p4)!=p3->usedVariables.end()){
	    this->headerFile << ",";
	  }
	}
      }
      this->headerFile << ");\n\n";
    }

    if(!this->staticVars.empty()){
      for(p2=this->staticVars.begin();p2!=this->staticVars.end();++p2){
	if(this->debugMode){
	  this->headerFile << "#line " << p2->lineNumber << " \"" 
			   << this->fileName << "\"\n";
	}
	this->headerFile << "static const " << p2->type << " " << p2->name << ";" << endl;
      }
      this->headerFile << endl;
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << "PleiadesField ple" << p->name << ";\n\n";
    }
    this->headerFile << endl;
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << "PleiadesField ple" << p->name << ";\n\n";
    }
    this->headerFile << endl;
    for(p=this->globalParameters.begin();p!=this->globalParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for(p=this->localParameters.begin();p!=this->localParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for(p=this->localVarsHolder.begin();p!=this->localVarsHolder.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for(p6=this->loadingVariables.begin();p6!=this->loadingVariables.end();++p6){
      if(this->debugMode){
	this->headerFile << "#line " << p6->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << "TabDouble " << p6->tabName << ";\n\n";
    }
    this->headerFile << endl;
    if(this->useTabStringToDefineDomain){
      this->headerFile << "unsigned short nbr;\n\n";
      this->headerFile << "unsigned short nbrOfPeriods;\n\n";
    }
    this->headerFile << "}; // end of class " << this->className << endl << endl;
    this->headerFile << "} // end of namespace PModels\n\n";
    this->headerFile << "} // end of namespace PMetier\n\n";
    this->headerFile << "} // end of namespace Pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << MFrontPleiadesModelParserArchitecture1_4::makeUpperCase(this->className)
		     << "_HH */\n";
  } // end of MFrontPleiadesModelParserArchitecture1_4::writeHeaderFile(void)

  std::string
  MFrontPleiadesModelParserArchitecture1_4::getGenTypeMethod(const std::string& type) const
  {
    using namespace std;
    if(type=="double"){
      return "getDouble";
    } else if(type=="string"){
      return "getString";
    } else if(type=="TabString"){
      return "getTabString";
    } else if(type=="TabDouble"){
      return "getTabDouble";
    } else if(type=="TabTabDouble"){
      return "getTabTabDouble";
    } else {
      string msg("MFrontPleiadesModelParserArchitecture1_4::getGenTypeMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontPleiadesModelParserArchitecture1_4::getGenTypeMethod

  void
  MFrontPleiadesModelParserArchitecture1_4::writeSrcFile(void)
  {
    using namespace std;
    map<string,vector<Function> > mfunctions;
    map<string,vector<Function> > mfunctionsII;
    set<string> sdomains;
    set<string> sdomainsParameters;
    set<string> usedVariables;
    set<string> modifiedVariables;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    map<string,string>::const_iterator p4;
    map<string,unsigned short>::const_iterator p5;
    map<string,vector<Property<double> > >::const_iterator p6;
    map<string,vector<Property<string> > >::const_iterator p7;
    std::vector<Property<double> >::const_iterator p8;
    std::vector<Property<string> >::const_iterator p9;
    StaticVarContainer::const_iterator p10;
    FunctionContainer::const_iterator p11;
    set<string>::const_iterator p12;
    std::map<std::string,std::vector<Function> >::iterator p13;
    std::vector<Function>::iterator p14;
    set<string>::const_iterator p15;
    set<string>::const_iterator p16;
    vector<LoadingVariable>::const_iterator p17;
    set<string>::size_type nbr;
    unsigned short j;
    bool found;

    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << this->className << " PleiadesModel.\n";
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
    this->srcFile << " */\n\n";
    this->srcFile << "#include<iostream>\n";
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include\"Pleiades/PMetier/pglossaire.hh\"\n";
    this->srcFile << "#include\"Pleiades/PExamplars/ClassProxy.hh\"\n";
    this->srcFile << "#include\"Pleiades/PMetier/PModels/" 
		  << this->className << "-pleiades.hh\"\n\n";
    this->srcFile << "namespace Pleiades\n{\n";
    this->srcFile << "namespace PMetier\n{\n";
    this->srcFile << "namespace PModels\n{\n\n";
    // static variables
    if(!this->staticVars.empty()){
      for(p10=this->staticVars.begin();p10!=this->staticVars.end();++p10){
	if(this->debugMode){
	  this->srcFile << "#line " << p10->lineNumber << " \"" 
			<< this->fileName << "\"\n";
	}
	if(p10->type=="short"){
	  this->srcFile << "const short " << this->className << "::" << p10->name 
			<< " = " << static_cast<short>(p10->value) << ";" << endl;
	} else if(p10->type=="ushort"){
	  this->srcFile << "const unsigned short " << this->className << "::" << p10->name 
			<< " = " << static_cast<unsigned short>(p10->value) << ";" << endl;
	} else if(p10->type=="int"){
	  this->srcFile << "const int " << this->className << "::" << p10->name 
			<< " = " << static_cast<int>(p10->value) << ";" << endl;
	} else if(p10->type=="uint"){
	  this->srcFile << "const unsigned int " << this->className << "::" << p10->name 
			<< " = " << static_cast<unsigned int>(p10->value) << ";" << endl;
	} else if(p10->type=="long"){
	  this->srcFile << "const long " << this->className << "::" << p10->name 
			<< " = " << static_cast<long>(p10->value) << ";" << endl;
	} else if(p10->type=="ulong"){
	  this->srcFile << "const unsigned long " << this->className << "::" << p10->name 
			<< " = " << static_cast<unsigned long>(p10->value) << ";" << endl;
	} else if(p10->type=="float"){
	  this->srcFile << "const float " << this->className << "::" << p10->name 
			<< " = " << static_cast<float>(p10->value) << ";" << endl;
	} else if(p10->type=="double"){
	  this->srcFile << "const double " << this->className << "::" << p10->name 
			<< " = " << static_cast<double>(p10->value) << ";" << endl;
	} else if(p10->type=="ldouble"){
	  this->srcFile << "const long double " << this->className << "::" << p10->name 
			<< " = " << static_cast<long double>(p10->value) << ";" << endl;
	} else {
	  string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	  msg += "type " + p10->type + " is not a supported type for a static variable.";
	  msg += "Supported types are short, ushort, int, uint, long, ulong,";
	  msg += "float, double and ldouble.\n";
	  msg += "Error at line " + toString(p10->lineNumber);
	  throw(runtime_error(msg));
	}
      }
      this->srcFile << endl;
    }
    // Constructors
    if(this->hasDefaultConstructor){
      this->srcFile << this->className << "::" 
		    << this->className << "()\n{\n";
      for(p=this->localParameters.begin();p!=this->localParameters.end();++p){
	string name;
	if((p4=this->glossaryNames.find(p->name))!=this->glossaryNames.end()){
	  name = "GlossaireParam::" + p4->second;
	} else if((p4=this->entryNames.find(p->name))!=this->entryNames.end()){
	  name = "\""+p4->second+"\"";
	} else {
	  name = "\""+p->name+"\"";
	}
	p4=this->defaultValues.find(p->name);
	assert(p4!=this->defaultValues.end());
	this->writeAssignDefaultValue(p,p4);
      }
      this->srcFile << "} // end of " 
		    << this->className << "::" 
		    << this->className <<"\n\n";
    }
    if(!this->localParameters.empty()){
      this->srcFile << "//! Constructor to initialize local parameters\n";
      this->srcFile << this->className << "::" 
		    << this->className 
		    << "(const Pleiades::PMetier::IArgumentMetier& arg)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
      this->srcFile << "using namespace Pleiades::PExceptions;\n";
      for(p=this->localParameters.begin();p!=this->localParameters.end();++p){
	string name;
	if((p4=this->glossaryNames.find(p->name))!=this->glossaryNames.end()){
	  name = "GlossaireParam::" + p4->second;
	} else if((p4=this->entryNames.find(p->name))!=this->entryNames.end()){
	  name = "\""+p4->second+"\"";
	} else {
	  name = "\""+p->name+"\"";
	}
	this->srcFile << "if(!arg.contains(" << name << ")){\n";
	if((p4=this->defaultValues.find(p->name))!=this->defaultValues.end()){
	  this->writeAssignDefaultValue(p,p4);
	  this->srcFile << "} else {\n";
	} else {
	  this->srcFile << "string msg(\"" << className << "::" << className << " : \");\n";
	  this->srcFile << "msg += \"can't initialize parameter " << p->name << "\";\n";
	  this->srcFile << "throw(PleiadesError(msg));\n";
	  this->srcFile << "} else {\n";
	}
	this->srcFile << "this->" << p->name << " = arg[" << name << "]." 
		      << this->getGenTypeMethod(p->type) << "();\n";
	this->srcFile << "}\n";
      }
      this->srcFile <<"} // end of "
		    << this->className << "::" 
		    << this->className <<"\n\n";
    }
    // Functions
    for(p11=this->functions.begin();p11!=this->functions.end();++p11){
      if(p11->modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	msg+="function " + p11->name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->srcFile << "#line " << p11->line << " \"" 
		      << this->fileName << "\"\n";
      }
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "const double\n";
	this->srcFile << this->className << "::" << p11->name << "(";
      } else {
	this->srcFile << "void\n";
	this->srcFile << this->className << "::" << p11->name << "(";
	for(p12=p11->modifiedVariables.begin();p12!=p11->modifiedVariables.end();){
	  this->srcFile << "double& " << *p12;
	  if((++p12)!=p11->modifiedVariables.end()){
	    this->srcFile << ",";
	  }
	}
      }
      if(p11->usedVariables.size()==0){
	if(p11->modifiedVariables.size()==1){
	  this->srcFile << "void";
	}
      } else {
	if(p11->modifiedVariables.size()!=1){
	  this->srcFile << ",";
	}
	for(p12=p11->usedVariables.begin();p12!=p11->usedVariables.end();){
	  this->srcFile << "const double " << *p12;
	  if((++p12)!=p11->usedVariables.end()){
	    this->srcFile << ",";
	  }
	}
      }
      this->srcFile << ")\n";
      this->srcFile << "{\n";
      this->srcFile << "using namespace std;\n";
      writeMaterialLaws("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile",
			this->srcFile,this->materialLaws);		      
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "double " << *(p11->modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << p11->body;
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "return " << *(p11->modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << "} // end of " << this->className << "::" << p11->name << "\n\n";
    }
    // initializeOutput
    this->srcFile << "bool\n"
		  << this->className
		  << "::initializeOutput(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      string name;
      if((p2=this->glossaryNames.find(p->name))!=this->glossaryNames.end()){
	name = "GlossaireField::"+p2->second;
      } else if((p2=this->entryNames.find(p->name))!=this->entryNames.end()){
	name = "\""+p2->second+"\"";
      } else {
	name = "\""+p->name+"\"";
      }
      this->srcFile << "if(!this->initializeOutputIFieldDouble(arg," << name;
      this->srcFile << ",\nthis->ple" << p->name << ",\"FieldMedDouble\",";
      if((p3=this->initialValues.find(p->name))!=this->initialValues.end()){
	this->srcFile << p3->second;
      } else {
	this->srcFile << "0.";
      }
      this->srcFile << ")){\n";
      this->srcFile << "string msg(\"" << this->className << "::initializeOutput : \");\n";
      this->srcFile << "msg += \"can't initialize output field " << p->name << "\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "}\n";
    }
    if(!this->outputs.empty()){
      this->srcFile << "MEDModel::initializeMEDModel(arg,this->ple"
		    << this->outputs.front().name <<"->getMesh());\n";
    } else {
      string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : no output field (FIXE ME!!!!)");
      throw(runtime_error(msg));
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      p7 = this->fieldStringProperties.find(p->name);
      if(p7!=this->fieldStringProperties.end()){
	for(p9=p7->second.begin();p9!=p7->second.end();++p9){
	  this->srcFile << "this->ple" 
			<< p7->first    <<  "->setProperty(\"" 
			<< p9->name     <<  "\",GenType(" 
			<< p9->value    << "));\n";
	}
      }
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      p6 = this->fieldDoubleProperties.find(p->name);
      if(p6!=this->fieldDoubleProperties.end()){
	for(p8=p6->second.begin();p8!=p6->second.end();++p8){
	  this->srcFile << endl;
	  this->srcFile << "this->ple" 
			<< p6->first    <<  "->setProperty(\""
			<< p8->name     <<  ",\"GenType(" 
			<< p8->value    << "));\n";
	}
      }
    }
    if(!this->initializeOutput.empty()){
      this->srcFile << this->initializeOutput;
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << this->className << "::initializeOutput \n\n";
    // initializeInput
    this->srcFile << "bool\n"
		  << this->className 
		  << "::initializeInput(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      string name;
      if((p2=this->glossaryNames.find(p->name))!=this->glossaryNames.end()){
	name = "GlossaireField::"+p2->second;
      } else if((p2=this->entryNames.find(p->name))!=this->entryNames.end()){
	name = "\""+p2->second+"\"";
      } else {
	name = "\""+p->name+"\"";
      }
      this->srcFile << "if(!this->initializeInputIFieldDouble(this->ple"
		    << p->name << "," << name;
      this->srcFile << ",\"FieldMedDouble\",arg)){\n";
      this->srcFile << "string msg(\"" << this->className << "::initializeInput : \");\n";
      this->srcFile << "msg += \"can't initialize input field " << p->name << "\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "}\n";
    }
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      p7 = this->fieldStringProperties.find(p->name);
      if(p7!=this->fieldStringProperties.end()){
	for(p9=p7->second.begin();p9!=p7->second.end();++p9){
	  if(p9->setValue){
	    this->srcFile << "this->ple"  << p7->first   <<  "->setProperty(\"" 
			  << p9->name  <<  "\",GenType(" 
			  << p9->value << "));\n";
	  }
	}
      }
    }
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      p6 = this->fieldDoubleProperties.find(p->name);
      if(p6!=this->fieldDoubleProperties.end()){
	for(p8=p6->second.begin();p8!=p6->second.end();++p8){
	  if(p8->setValue){
	    this->srcFile << "this->ple" << p6->first    <<  "->setProperty(\""
			  << p8->name    <<  "\",GenType(" 
			  << p8->value   << "));\n";
	  }
	}
      }
    }
    this->srcFile << endl;
    if(!this->initializeInput.empty()){
      this->srcFile << this->initializeInput;
      this->srcFile << endl;
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << this->className << "::initializeInput\n\n";
    // initializeParamaters
    this->srcFile << "bool\n"
		  << this->className 
		  << "::initializeParameters(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=this->globalParameters.begin();p!=this->globalParameters.end();++p){
      string name;
      if((p4=this->glossaryNames.find(p->name))!=this->glossaryNames.end()){
	name = "GlossaireParam::" + p4->second;
      } else if((p4=this->entryNames.find(p->name))!=this->entryNames.end()){
	name = "\""+p4->second+"\"";
      } else {
	name = "\""+p->name+"\"";
      }
      this->srcFile << "if(!arg.contains(" << name << ")){\n";
      if((p4=this->defaultValues.find(p->name))!=this->defaultValues.end()){
	this->srcFile << "this->" << p->name << " = " << p4->second << ";" << endl;
	this->srcFile << "} else {\n";
      } else {
	this->srcFile << "string msg(\"" << this->className << "::initializeParameters : \");\n";
	this->srcFile << "msg += \"can't initialize parameter " << p->name << "\";\n";
	this->srcFile << "throw(PleiadesError(msg));\n";
	this->srcFile << "} else {\n";
      }
      this->srcFile << "this->" << p->name << " = arg[" << name << "]." 
		    << this->getGenTypeMethod(p->type) << "();\n";
      this->srcFile << "}\n";
    }
    for(p17=this->loadingVariables.begin();p17!=this->loadingVariables.end();++p17){
      string entryName;
      if((p4=this->entryNames.find(p17->name))!=this->entryNames.end()){
	entryName = p4->second+"Evolution";
      } else {
	entryName = p17->name+"Evolution";
      }
      this->srcFile << "if(!arg.contains(\"" << entryName << "\")){\n";
      this->srcFile << "string msg(\"" << this->className << "::initializeParameters : \");\n";
      this->srcFile << "msg += \"can't initialize loading variable " << p17->name << "\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "} else {\n";
      this->srcFile << "this->" << p17->tabName << " = arg[\"" << entryName << "\"]." 
		    <<  "getTabDouble();\n";
      this->srcFile << "}\n";
    }
    this->srcFile << "if(!arg.contains(\"ListeTempsReel\")){\n";
    this->srcFile << "string msg(\"" << this->className << "::initializeParameters : \");\n";
    this->srcFile << "msg += \"internal error (can't find list of times).\";\n";
    this->srcFile << "throw(PleiadesError(msg));\n}\n";
    this->srcFile << "this->nbrOfPeriods = static_cast<unsigned short>(arg[\"ListeTempsReel\"].getTabDouble().getTaille())-1;\n";
    if(!this->initializeParameters.empty()){
      this->srcFile << "// initialize parameters\n";
      this->srcFile << this->initializeParameters;
      this->srcFile << endl;
    }
    if(!this->initLocalVars.empty()){
      this->srcFile << "// initialize local variables\n";
      this->srcFile << this->initLocalVars;
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << this->className << "::initializeParameters\n\n";
    // initialize
    this->srcFile << "bool\n"
		  << this->className 
		  << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "this->initializeOutput(arg);\n";
    this->srcFile << "this->initializeInput(arg);\n";
    this->srcFile << "this->initializeParameters(arg);\n";
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << this->className << "::initialize\n\n";
    // execute
    this->srcFile << "bool\n"
		  << this->className
		  << "::execute(void){\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PTime;\n\n";
    this->srcFile << "typedef MEDMEM::FIELD<double>* MEDFieldPtr;\n";
    this->srcFile << "typedef MEDMEM::FIELD<double>& MEDField;\n\n";
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      this->srcFile << "MEDField " << p->name 
		    << " = *(static_cast<MEDFieldPtr>(this->ple" << p->name << "->getMed()));\n";
      if((p5=this->depth.find(p->name))!=this->depth.end()){
	for(j=1;j<=p5->second;++j){
	  ostringstream converter;
	  converter << j;
	  string varName = p->name+"_"+converter.str();
	  this->srcFile << "MEDField " << varName 
			<< " = *(static_cast<MEDFieldPtr>(this->ple" 
			<< p->name << "->getFieldPreceding("<< j <<")->getMed()));\n";
	}
      }
    }
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      this->srcFile << "MEDField " << p->name 
		    << " = *(static_cast<MEDFieldPtr>(this->ple" << p->name << "->getMed()));\n";
      if((p5=this->depth.find(p->name))!=this->depth.end()){
	for(j=1;j<=p5->second;++j){
	  this->srcFile << "MEDField " << p->name << "_" << j
			<< " = *(static_cast<MEDFieldPtr>(this->ple" 
			<< p->name << "->getFieldPreceding("<< j <<")->getMed()));\n";
	}
      }
    }
    this->srcFile << "this->period = static_cast<unsigned short>(ITime::GlobalTime->get(\"position\"));\n";
    this->srcFile << "const double t  = ITime::GlobalTime->get(\"time\");\n";
    this->srcFile << "const double dt = ITime::GlobalTime->get(\"time increment\");\n";
    this->srcFile << "static_cast<void>(t);  // suppress unused variable warning\n";
    this->srcFile << "static_cast<void>(dt); // suppress unused variable warning\n\n";
    for(p17=this->loadingVariables.begin();p17!=this->loadingVariables.end();++p17){
      this->srcFile << "const double " << p17->name << " = this->" 
		    << p17->tabName << "[period];\n";
      for(j=1;j!=p17->depth+1;++j){
	this->srcFile << "const double " << p17->name << "_" << j << " = this->" 
		      << p17->tabName << "[period-" << j << "];\n";
      }
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      p7 = this->fieldStringProperties.find(p->name);
      if(p7!=this->fieldStringProperties.end()){
	for(p9=p7->second.begin();p9!=p7->second.end();++p9){
	  this->srcFile << "string " << p9->varName  <<  " = this->ple" 
			<< p7->first    <<  "->getProperty(\"" 
			<< p9->name     <<  "\")."
			<< getGenTypeMethod("string") << "();\n";
	}
      }
    }
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      p7 = this->fieldStringProperties.find(p->name);
      if(p7!=this->fieldStringProperties.end()){
	for(p9=p7->second.begin();p9!=p7->second.end();++p9){
	  this->srcFile << "string " << p9->varName  <<  " = this->ple" 
			<< p7->first    <<  "->getProperty(\"" 
			<< p9->name     <<  "\")."
			<< getGenTypeMethod("string") << "();\n";
	}
      }
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      p6 = this->fieldDoubleProperties.find(p->name);
      if(p6!=this->fieldDoubleProperties.end()){
	for(p8=p6->second.begin();p8!=p6->second.end();++p8){
	  this->srcFile << "double " << p8->varName  <<  " = this->ple" 
			<< p6->first <<  "->getProperty(\""
			<< p8->name  <<  "\")."
			<< getGenTypeMethod("double") << "();\n";
	}
      }
    }
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      p6 = this->fieldDoubleProperties.find(p->name);
      if(p6!=this->fieldDoubleProperties.end()){
	for(p8=p6->second.begin();p8!=p6->second.end();++p8){
	  this->srcFile << "double " << p8->varName  <<  " = this->ple" 
			<< p6->first <<  "->getProperty(\""
			<< p8->name  <<  "\")."
			<< getGenTypeMethod("double") << "();\n";
	}
      }
    }

    this->srcFile << "cout << \"***" << this->className 
		  << "::execute (period \" << period << \"/\" << this->nbrOfPeriods << \")\\n\";\n\n";

    // calling functions
    // we first sort functions by domains
    for(p11=this->functions.begin();p11!=this->functions.end();++p11){
      if(p11->domains.empty()){
	if(p11->domainsParameters.empty()){
	  if(!this->domains.empty()){
	    // function domains are defined though the model's
	    // domains
	    sdomains.insert(this->domains.begin(),this->domains.end());
	    for(p16=this->domains.begin();p16!=this->domains.end();++p16){
	      if((p13=mfunctions.find(*p16))==mfunctions.end()){
		vector<Function> tmp;
		tmp.push_back(*p11);
		mfunctions.insert(make_pair(*p16,tmp));
	      } else {
		p13->second.push_back(*p11);
	      }
	    }
	  } else if(!this->domainsParameters.empty()){
	    // function domains are defined though the model's
	    // parameter
	    sdomainsParameters.insert(this->domainsParameters.begin(),
				      this->domainsParameters.end());
	    for(p16=this->domainsParameters.begin();p16!=this->domainsParameters.end();++p16){
	      if((p13=mfunctionsII.find(*p16))==mfunctionsII.end()){
		vector<Function> tmp;
		tmp.push_back(*p11);
		mfunctionsII.insert(make_pair(*p16,tmp));
	      } else {
		p13->second.push_back(*p11);
	      }
	    }
	  } else {
	    string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	    msg += "function "+p11->name+" has no domain and no global is domain defined";
	    throw(runtime_error(msg));
	  }
	} else {
	  // function whose domains are defined through parameters
	  for(p12=p11->domainsParameters.begin();p12!=p11->domainsParameters.end();++p12){
	    sdomainsParameters.insert(*p12);
	    if((p13=mfunctionsII.find(*p12))==mfunctionsII.end()){
	      vector<Function> tmp;
	      tmp.push_back(*p11);
	      mfunctionsII.insert(make_pair(*p12,tmp));
	    } else {
	      p13->second.push_back(*p11);
	    }
	  }
	}
      } else {
	// function domains are explicitly named
	for(p12=p11->domains.begin();p12!=p11->domains.end();++p12){
	  sdomains.insert(*p12);
	  if((p13=mfunctions.find(*p12))==mfunctions.end()){
	    vector<Function> tmp;
	    tmp.push_back(*p11);
	    mfunctions.insert(make_pair(*p12,tmp));
	  } else {
	    p13->second.push_back(*p11);
	  }
	}
      }
    }
    if((!mfunctions.empty())||(!mfunctionsII.empty())){
      this->srcFile << "map<string,vector<int> >::iterator ptr;\n";
      this->srcFile << "vector<int>::const_iterator ptr2;\n";
    }
    if(this->useTabStringToDefineDomain){
      this->srcFile << "this->nbr = 0u;\n\n";
    }
    // we now declare a nodes set on each domain defined through a
    // string parameter or explicitely named
    nbr = 0u;
    for(p12=sdomainsParameters.begin();p12!=sdomainsParameters.end();++p12){
      found = false;
      p = this->localParameters.begin();
      while((p!=this->localParameters.end())&&(!found)){
	if(p->name==*p12){
	  found = true;
	} else {
	  ++p;
	}
      }
      if(!found){
	p = this->globalParameters.begin();
	while((p!=this->globalParameters.end())&&(!found)){
	  if(p->name==*p12){
	    found = true;
	  } else {
	    ++p;
	  }
	}
      }
      if(!found){
	string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	msg += "internal error while treating parameter "+*p12;
	msg += " (no local nor gloabal parameter associated)";
	throw(runtime_error(msg));
      }
      if(p->type=="string"){
	this->srcFile << "ptr = this->nodesOfZones.find(this->" << *p12<< ");\n";
	this->srcFile << "if(ptr==this->nodesOfZones.end()){\n";
	this->srcFile << "string msg(\"" << this->className << "::execute : \");\n";
	this->srcFile << "msg += \"internal error (no group named \" + this->"
		      << *p12 << " + \" defined).\";\n";
	this->srcFile << "throw(PleiadesError(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const vector<int>& nodes" << nbr << "= ptr->second;\n";
	++nbr;
      }
    }
    for(p12=sdomains.begin();p12!=sdomains.end();++p12,++nbr){
      this->srcFile << "ptr = this->nodesOfZones.find(\"" << *p12<< "\");\n";
      this->srcFile << "if(ptr==this->nodesOfZones.end()){\n";
      this->srcFile << "string msg(\"" << this->className << "::execute : \");\n";
      this->srcFile << "msg += \"internal error (no group named "<< *p12 << " defined).\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "const vector<int>& nodes" << nbr << "= ptr->second;\n";
    }
    // Treating functions whose domains are given by parameters
    nbr = 0u;
    for(p12=sdomainsParameters.begin();p12!=sdomainsParameters.end();++p12){
      found = false;
      p = this->localParameters.begin();
      while((p!=this->localParameters.end())&&(!found)){
	if(p->name==*p12){
	  found = true;
	} else {
	  ++p;
	}
      }
      if(!found){
	p = this->globalParameters.begin();
	while((p!=this->globalParameters.end())&&(!found)){
	  if(p->name==*p12){
	    found = true;
	  } else {
	    ++p;
	  }
	}
      }
      if(!found){
	string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	msg += "internal error while treating parameter "+*p12;
	msg += " (no local nor gloabal parameter associated)";
	throw(runtime_error(msg));
      }
      if(p->type=="string"){
	this->srcFile << "// Calling functions whose domain is defined through parameter "
		      << *p12 << endl;
	this->srcFile << "for(ptr2=n" << nbr << ".begin();";
	this->srcFile << "ptr2!=n" << nbr << ".end();++ptr2){\n";
	p13 = mfunctionsII.find(*p12);
	if(p13==mfunctionsII.end()){
	  string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	  msg += "internal error while treating parameter "+*p12+" (no function associated)";
	  throw(runtime_error(msg));
	}
	usedVariables.clear();
	modifiedVariables.clear();
	// generating temporaries for variables of thoses fields
	for(p14=p13->second.begin();p14!= p13->second.end();++p14){
	  for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();++p15){
	    usedVariables.insert(*p15);	  
	  }
	  for(p15=p14->modifiedVariables.begin();p15!=p14->modifiedVariables.end();++p15){
	    modifiedVariables.insert(*p15);	  
	  }
	}
	for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	  if((p16=usedVariables.find(*p15))!=usedVariables.end()){
	    usedVariables.erase(*p16);
	  }
	}
	// declaring temporaries
	for(p15=usedVariables.begin();p15!=usedVariables.end();++p15){
	  if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	    this->srcFile << "const double tmp_" << *p15 << " = " 
			  << *p15 << ".getValueIJ(*ptr2,1);\n";
	  }
	}
	for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	  if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	    this->srcFile << "double tmp_" << *p15 << " = " << *p15 << ".getValueIJ(*ptr2,1);\n";
	  }
	}
	for(p14=p13->second.begin();p14!= p13->second.end();++p14){
	  if(p14->modifiedVariables.size()==1){
	    this->srcFile << "tmp_" << *(p14->modifiedVariables.begin()) << " = ";
	    this->srcFile << p14->name << "(";
	    for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();){
	      if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
		this->srcFile << "tmp_" << *p15;
	      } else {
		this->srcFile << *p15;
	      }
	      if((++p15)!=p14->usedVariables.end()){
		this->srcFile << ",";
	      }
	    }
	  } else{
	    this->srcFile << "this->" << p14->name << "(";
	    for(p15=p14->modifiedVariables.begin();p15!=p14->modifiedVariables.end();){
	      this->srcFile << "tmp_" << *p15;
	      if((++p15)!=p14->modifiedVariables.end()){
		this->srcFile << ",";
	      }
	    }
	    if(!p14->usedVariables.empty()){
	      this->srcFile << ",";
	    }
	    for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();){
	      if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
		this->srcFile << "tmp_" << *p15;
	      } else {
		this->srcFile << *p15;
	      }
	      if((++p15)!=p14->usedVariables.end()){
		this->srcFile << ",";
	      }
	    }
	  }
	  this->srcFile << ");\n";
	}
	for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	  this->srcFile << *p15 << ".setValueIJ(*ptr2,1,tmp_" << *p15 << ");\n";
	}
	this->srcFile << "}\n";
	++nbr;
      } else if (p->type=="TabString"){
	this->srcFile << "// Calling functions whose domains are defined through parameter " 
		      << *p12 << endl;
	this->srcFile << "for(unsigned int index=0;index!=static_cast<unsigned short>(" << *p12 << ".getTaille());++index,++nbr){\n";
	this->srcFile << "ptr = this->nodesOfZones.find(this->" << *p12<< "[index]);\n";
	this->srcFile << "if(ptr==this->nodesOfZones.end()){\n";
	this->srcFile << "string msg(\"" << this->className << "::execute : \");\n";
	this->srcFile << "msg += \"internal error (no group named \" + this->" << *p12 << "[index] +\" defined).\";\n";
	this->srcFile << "throw(PleiadesError(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const vector<int>& nodes = ptr->second;\n";
	this->srcFile << "for(ptr2=nodes.begin();ptr2!=nodes.end();++ptr2){\n";
	p13 = mfunctionsII.find(*p12);
	if(p13==mfunctionsII.end()){
	  string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	  msg += "internal error while treating parameter "+*p12+" (no function associated)";
	  throw(runtime_error(msg));
	}
	usedVariables.clear();
	modifiedVariables.clear();
	// generating temporaries for variables of thoses fields
	for(p14=p13->second.begin();p14!= p13->second.end();++p14){
	  for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();++p15){
	    usedVariables.insert(*p15);	  
	  }
	  for(p15=p14->modifiedVariables.begin();p15!=p14->modifiedVariables.end();++p15){
	    modifiedVariables.insert(*p15);	  
	  }
	}
	for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	  if((p16=usedVariables.find(*p15))!=usedVariables.end()){
	    usedVariables.erase(*p16);
	  }
	}
	// declaring temporaries
	for(p15=usedVariables.begin();p15!=usedVariables.end();++p15){
	  if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	    this->srcFile << "const double tmp_" << *p15 << " = " 
			  << *p15 << ".getValueIJ(*ptr2,1);\n";
	  }
	}
	for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	  if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	    this->srcFile << "double tmp_" << *p15 << " = " << *p15 << ".getValueIJ(*ptr2,1);\n";
	  }
	}
	for(p14=p13->second.begin();p14!= p13->second.end();++p14){
	  if(p14->modifiedVariables.size()==1){
	    this->srcFile << "tmp_" << *(p14->modifiedVariables.begin()) << " = ";
	    this->srcFile << p14->name << "(";
	    for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();){
	      if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
		this->srcFile << "tmp_" << *p15;
	      } else {
		this->srcFile << *p15;
	      }
	      if((++p15)!=p14->usedVariables.end()){
		this->srcFile << ",";
	      }
	    }
	  } else{
	    this->srcFile << "this->" << p14->name << "(";
	    for(p15=p14->modifiedVariables.begin();p15!=p14->modifiedVariables.end();){
	      this->srcFile << "tmp_" << *p15;
	      if((++p15)!=p14->modifiedVariables.end()){
		this->srcFile << ",";
	      }
	    }
	    if(!p14->usedVariables.empty()){
	      this->srcFile << ",";
	    }
	    for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();){
	      if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
		this->srcFile << "tmp_" << *p15;
	      } else {
		this->srcFile << *p15;
	      }
	      if((++p15)!=p14->usedVariables.end()){
		this->srcFile << ",";
	      }
	    }
	  }
	  this->srcFile << ");\n";
	}
	for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	  this->srcFile << *p15 << ".setValueIJ(*ptr2,1,tmp_" << *p15 << ");\n";
	}
	this->srcFile << "}\n";
	this->srcFile << "} // end of treatment of functions whose domains are defined through parameter "<< *p12 << "\n";
      } else {
	string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	msg += "internal error while treating parameter "+*p12+" (wrong type " + p->type  +")";
	throw(runtime_error(msg));
      }
    }
    // Treating functions whose domains are explicitly given
    for(p12=sdomains.begin();p12!=sdomains.end();++p12,++nbr){
      this->srcFile << "// Calling functions on domain " << *p12 << endl;
      this->srcFile << "for(p2=n" << nbr << ".begin();";
      this->srcFile << "p2!=n" << nbr << ".end();++p2){\n";
      p13 = mfunctions.find(*p12);
      if(p13==mfunctions.end()){
	string msg("MFrontPleiadesModelParserArchitecture1_4::writeSrcFile : ");
	msg += "internal error while treating domain "+*p12+" (no function associated)";
	throw(runtime_error(msg));
      }
      usedVariables.clear();
      modifiedVariables.clear();
      // generating temporaries for variables of thoses fields
      for(p14=p13->second.begin();p14!= p13->second.end();++p14){
	for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();++p15){
	  usedVariables.insert(*p15);	  
	}
	for(p15=p14->modifiedVariables.begin();p15!=p14->modifiedVariables.end();++p15){
	  modifiedVariables.insert(*p15);	  
	}
      }
      for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	if((p16=usedVariables.find(*p15))!=usedVariables.end()){
	  usedVariables.erase(*p16);
	}
      }
      // declaring temporaries
      for(p15=usedVariables.begin();p15!=usedVariables.end();++p15){
	if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	  this->srcFile << "const double tmp_" << *p15 << " = " << *p15 << ".getValueIJ(*ptr2,1);\n";
	}
      }
      for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	  this->srcFile << "double tmp_" << *p15 << " = " << *p15 << ".getValueIJ(*ptr2,1);\n";
	}
      }
      for(p14=p13->second.begin();p14!= p13->second.end();++p14){
	if(p14->modifiedVariables.size()==1){
	  this->srcFile << "tmp_" << *(p14->modifiedVariables.begin()) << " = ";
	  this->srcFile << p14->name << "(";
	  for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();){
	    if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	      this->srcFile << "tmp_" << *p15;
	    } else {
	      this->srcFile << *p15;
	    }
	    if((++p15)!=p14->usedVariables.end()){
	      this->srcFile << ",";
	    }
	  }
	} else{
	  this->srcFile << "this->" << p14->name << "(";
	  for(p15=p14->modifiedVariables.begin();p15!=p14->modifiedVariables.end();){
	    this->srcFile << "tmp_" << *p15;
	    if((++p15)!=p14->modifiedVariables.end()){
	      this->srcFile << ",";
	    }
	  }
	  if(!p14->usedVariables.empty()){
	    this->srcFile << ",";
	  }
	  for(p15=p14->usedVariables.begin();p15!=p14->usedVariables.end();){
	    if(this->fieldNames.find(*p15)!=this->fieldNames.end()){
	      this->srcFile << "tmp_" << *p15;
	    } else {
	      this->srcFile << *p15;
	    }
	    if((++p15)!=p14->usedVariables.end()){
	      this->srcFile << ",";
	    }
	  }
	}
	this->srcFile << ");\n";
      }
      for(p15=modifiedVariables.begin();p15!=modifiedVariables.end();++p15){
	this->srcFile << *p15 << ".setValueIJ(*ptr2,1,tmp_" << *p15 << ");\n";
      }
      this->srcFile << "}\n";
    }
    this->srcFile << this->execute;
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      p7 = this->fieldStringProperties.find(p->name);
      if(p7!=this->fieldStringProperties.end()){
	for(p9=p7->second.begin();p9!=p7->second.end();++p9){
	  this->srcFile << "this->ple" << p7->first     << "->setProperty(\"" 
			<< p9->name    << "\",GenType(" << p9->varName << "));\n";
	}
      }
    }
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      p7 = this->fieldStringProperties.find(p->name);
      if(p7!=this->fieldStringProperties.end()){
	for(p9=p7->second.begin();p9!=p7->second.end();++p9){
	  this->srcFile << "this->ple" << p7->first     << "->setProperty(\"" 
			<< p9->name    << "\",GenType(" << p9->varName << "));\n";
	}
      }
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      p6 = this->fieldDoubleProperties.find(p->name);
      if(p6!=this->fieldDoubleProperties.end()){
	for(p8=p6->second.begin();p8!=p6->second.end();++p8){
	  this->srcFile << "this->ple" << p6->first     << "->setProperty(\"" 
			<< p8->name    << "\",GenType(" << p8->varName << "));\n";
	}
      }
    }
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      p6 = this->fieldDoubleProperties.find(p->name);
      if(p6!=this->fieldDoubleProperties.end()){
	for(p8=p6->second.begin();p8!=p6->second.end();++p8){
	  this->srcFile << "this->ple" << p6->first     << "->setProperty(\"" 
			<< p8->name    << "\",GenType(" << p8->varName << "));\n";
	}
      }
    }    
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << this->className << "::execute\n\n";
    
    this->srcFile << "void\n" << this->className 
		  << "::executeInitialPostProcessingTasks(const bool)\n{} // end of " << this->className 
		  << "::executeInitialPostProcessingTasks\n\n";

    this->srcFile << "void\n" << this->className 
		  << "::executePostProcessingTasks(const bool)\n{} // end of " << this->className 
		  << "::executePostProcessingTasks\n\n";

    if(!this->localParameters.empty()){
      this->srcFile << "using Pleiades::PExamplars::TemplateModelClassProxy;\n";
      this->srcFile << "TemplateModelClassProxy<"
		    << this->className
		    << "> proxy" << this->className << "(\""
		    << this->className << "\");\n";
    }
    if(this->hasDefaultConstructor){
      this->srcFile << "GENERATE_PROXY(IModel," << this->className << ");\n\n";
    }
    this->srcFile << "} // end of namespace PModels\n\n";
    this->srcFile << "} // end of namespace PMetier\n\n";
    this->srcFile << "} // end of namespace Pleiades\n";
  } // end of MFrontPleiadesModelParserArchitecture1_4::writeSrcFile(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::writeOutputFiles(void)
  {
    using namespace std;
    if(this->className.empty()){
      string msg("MFrontPleiadesModelParserArchitecture1_4::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    if(!this->material.empty()){
      this->className = this->material+'_'+this->className;
    }
    this->headerFileName  = "include/Pleiades/PMetier/PModels/"+this->className;
    this->headerFileName += "-pleiades.hh";
    this->srcFileName  = "src/"+this->className;
    this->srcFileName += "-pleiades.cpp";
    this->castemHeaderFileName  = "include/"+this->className;
    this->castemHeaderFileName += "-castem.hh";
    this->castemSrcFileName  = "src/"+this->className;
    this->castemSrcFileName += "-castem.cpp";
    this->openOutputFiles();
    this->generateOutputFiles();
  } // end of class MFrontPleiadesModelParserArchitecture1_4::writeOutputFiles()

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParserArchitecture1_4::getGeneratedSources(void)
  {
    using namespace std;
    map<string,vector<string> > src;
    if(this->exportCastem){
      src["libCastemMaterialModels"].push_back(this->className+"-castem.cpp");
    }
    src["libPleiadesMaterialModels"].push_back(this->className+"-pleiades.cpp");
    return src;
  } // end of MFrontPleiadesModelParserArchitecture1_4::getGeneratedSources
  
  std::vector<std::string>
  MFrontPleiadesModelParserArchitecture1_4::getGeneratedIncludes(void)
  {
    using namespace std;
    vector<string> inc;
    inc.push_back("Pleiades/PMetier/PModels/"+this->className+"-pleiades.hh");
    if(this->exportCastem){
      inc.push_back(this->className+"-castem.hh");
    }
    return inc;
  } // end of MFrontPleiadesModelParserArchitecture1_4::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParserArchitecture1_4::getGlobalIncludes(void)
  {
    using namespace std;
    map<string,vector<string> > incs;
    incs["libPleiadesMaterialModels"].push_back("`pleiades-config --includes`\n");
    return incs;
  } // end of MFrontPleiadesModelParserArchitecture1_4::getGlobalIncludes
  
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParserArchitecture1_4::getGlobalDependencies(void)
  {
    using namespace std;
    map<string,vector<string> > libs;
    if(this->exportCastem){
      libs["libCastemMaterialModels"].push_back("-lm");
    }
    libs["libPleiadesMaterialModels"].push_back("`pleiades-config --libs` -lm");
    return libs;
  } // end of MFrontPleiadesModelParserArchitecture1_4::getGlobalDependencies
    
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParserArchitecture1_4::getLibrariesDependencies(void)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontPleiadesModelParserArchitecture1_4::getLibrariesDependencies

  void
  MFrontPleiadesModelParserArchitecture1_4::treatExecute(void)
  {
    this->execute = this->readNextBlock(true);
  }

  void
  MFrontPleiadesModelParserArchitecture1_4::treatOutput(void)
  {
    VarContainer::const_iterator p;
    this->readVarList("PleiadesField",this->outputs,false);
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      this->fieldNames.insert(p->name);
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatOutput(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatInput(void)
  {
    VarContainer::const_iterator p;
    this->readVarList("PleiadesField",this->inputs,false);
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      this->fieldNames.insert(p->name);
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatInputs(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameter(void)
  {
    this->readVarList(this->globalParameters,false);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatInputs(void)

  void
  MFrontPleiadesModelParserArchitecture1_4::treatLocalParameter(void)
  {
    this->readVarList(this->localParameters,false);
  } // end of MFrontPleiadesModelParserArchitecture1_4::treatInput(void)
  
  void
  MFrontPleiadesModelParserArchitecture1_4::writeAssignDefaultValue(const VarContainer::const_iterator p,
						     const std::map<std::string,std::string>::const_iterator p4)
  {
    using namespace std;
    if((p->type=="TabDouble")||(p->type=="TabString")){
      unsigned short i;
      unsigned short size;
      vector<string> res;
      string tmpName;
      istringstream c(p4->second);
      copy(istream_iterator<string>(c),istream_iterator<string>(),back_inserter(res));
      istringstream c2(res.front());
      c2 >> size;
      i=0;
      tmpName="tmp"+p->name;
      while(this->varNames.find(tmpName)!=this->varNames.end()){
	tmpName="tmp"+p->name+toString(i);
	++i;
      }
      this->srcFile << p->type << " " << tmpName << "(" << size << ");\n";
      for(i=0;i!=size;++i){
	this->srcFile << tmpName << "[" << i << "] = " << res.at(i+1) << ";" << endl;
      }
      this->srcFile << "this->" << p->name << " = " << tmpName << ";" << endl;
    } else if((p->type=="string")||(p->type=="double")){
      this->srcFile << "this->" << p->name << " = "  << p4->second << ";" << endl;
    } else {
      string msg("MFrontPleiadesModelParserArchitecture1_4::writeAssignDefaultValue : ");
      msg+="type "+p->type+"is not supported.\n";
      throw(runtime_error(msg));
    }   
  } // end of MFrontPleiadesModelParserArchitecture1_4::writeAssignDefaultValue

  void
  MFrontPleiadesModelParserArchitecture1_4::readDefaultValue(void)
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
      assert(found);
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
			    "Expected default value.");
    if((p->type=="TabDouble")||
       (p->type=="TabString")){
      nbr=0u;
      bend = false;
      while(bend==false){
	if(p->type=="TabDouble"){
	  istringstream converter(this->current->value);
	  double tmp;
	  converter >> tmp;
	  if(!converter&&(!converter.eof())){
	    this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
				    "Could not read default for variable '"+this->currentVar+"'");
	  }
	} else {
	  if(this->current->flag!=Token::String){
	    this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
				    "Expected to read a string (read '"+this->current->value+"').");
	  }
	}
	++nbr;
	res+=" "+this->current->value;
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
				"Expected ',' or ')'.");
	if(this->current->value==")"){
	  bend = true;
	  --(this->current);
	} else if(this->current->value==","){
	  ++(this->current);
	  this->checkNotEndOfFile("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
				  "Expected default value.");
	} else {
	  this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
				  "Unexpected token (expected ',' or ')', read '"+this->current->value+"').");
	}
      }
      res = toString(nbr)+res;
    } else if (p->type=="double"){
      istringstream converter(this->current->value);
      double tmp;
      converter >> tmp;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
				"Could not read default for variable '"+this->currentVar+"'");
      }
      res = this->current->value;
    } else if (p->type=="string"){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      res = this->current->value;
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::readDefaultValue",
			      "type '"+p->type+"' is not supported.");
    }
    if(!this->defaultValues.insert(MVType(this->currentVar,res)).second){
      this->throwRuntimeError("MFrontPleiadesModelParserArchitecture1_4::treatGlobalParameterMethod",
			      "Default value for field '"+ this->currentVar +"' already defined.");
    }
  } // end of MFrontPleiadesModelParserArchitecture1_4::writeReadDefaultValue

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontPleiadesModelParserArchitecture1_4::getSpecificTargets(void)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontPleiadesModelParserArchitecture1_4::getSpecificTargets(void)

} // end of namespace mfront  
