/*!
 * \file   MFrontPleiadesModelParser.cxx
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

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontPleiadesModelParser.hxx"
#include"TFEL/System/System.hxx"

static const unsigned short TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY = 8;

namespace mfront{

  struct MFrontPleiadesModelParser::Function
  {
    StaticVarContainer  staticVars;
    std::set<std::string> usedVariables;
    std::set<std::string> modifiedVariables;
    std::set<std::string> domains;
    std::set<std::string> localVars;
    std::set<std::string> constantMaterialProperties;
    std::set<std::string> localParameters;
    std::set<std::string> globalParameters;
    std::map<std::string,unsigned short> depth;
    std::string name;
    std::string body;
    unsigned short line;
    bool useTimeIncrement;
  }; // end of struct MFrontPleiadesModelParser::Function

  std::string 
  MFrontPleiadesModelParser::makeUpperCase(const std::string& n)
  {
    using namespace std;
    string s(n);
    string::const_iterator p;
    string::iterator p2;
    for(p=n.begin(),p2=s.begin();p!=n.end();++p,++p2){
      *p2 = static_cast<char>(toupper(*p));
    }
    return s;
  } // end of MFrontPleiadesModelParser::makeUpperCase

  MFrontPleiadesModelParser::MFrontPleiadesModelParser(void)
    : MFrontVirtualParser()
  {
    // Call Back
    this->registerNewCallBack("@MaterialLaw",&MFrontPleiadesModelParser::treatMaterialLaw);
    this->registerNewCallBack("@Material",&MFrontPleiadesModelParser::treatMaterial);
    this->registerNewCallBack("@Output",&MFrontPleiadesModelParser::treatOutput);
    this->registerNewCallBack("@Input",&MFrontPleiadesModelParser::treatInput);
    this->registerNewCallBack("@GlobalParameter",
			      &MFrontPleiadesModelParser::treatGlobalParameter);
    this->registerNewCallBack("@LocalParameter",
			      &MFrontPleiadesModelParser::treatLocalParameter);
    this->registerNewCallBack("@ConstantMaterialProperty",
			      &MFrontPleiadesModelParser::treatConstantMaterialProperty);
    this->registerNewCallBack("@Execute",&MFrontPleiadesModelParser::treatExecute);
    this->registerNewCallBack("@Domain",&MFrontPleiadesModelParser::treatDomain);
    this->registerNewCallBack("@Domains",&MFrontPleiadesModelParser::treatDomains);
    this->registerNewCallBack("@Function",&MFrontPleiadesModelParser::treatFunction);
    this->registerNewCallBack("@InitializeParameters",
			      &MFrontPleiadesModelParser::treatInitializeParameters);
    this->registerNewCallBack("@Bounds",
			      &MFrontPleiadesModelParser::treatBounds);
    this->registerNewCallBack("@PhysicalBounds",
			      &MFrontPleiadesModelParser::treatPhysicalBounds);
    this->reserveName("dt");
    this->reserveName("boost");
    this->reserveName("field");
    this->reserveName("support");
    this->reserveName("values");
    this->reserveName("results");
    this->reserveName("domains");
    this->reserveName("requirement");
    this->reserveName("requirementManager");
    this->reserveName("md");
    this->reserveName("ptr");
    this->reserveName("ptr2");
    this->reserveName("ptr3");
    this->reserveName("tmp");
    this->reserveName("outputsDepths");
    this->reserveName("outputsInitialValues");
    this->reserveName("inputsDepths");
    this->reserveName("ValidParametersNames");
    this->reserveName("computeMaterialProperties");
    this->reserveName("apply");
    this->reserveName("FieldHolder");
    this->reserveName("MTFieldManager");
    this->reserveName("mm");
    this->reserveName("initialize");
    this->reserveName("initializeOutput");
    this->reserveName("initializeOutputsVariablesDepths");
    this->reserveName("initializeOutputsVariablesInitialValues");
    this->reserveName("initializeInputsVariablesDepths");
    this->reserveName("initializeConstantMaterialProperties");
    this->reserveName("constantMaterialProperties");
    this->reserveName("declareRequirements");
    this->reserveName("resolveDependencies");
    this->reserveName("execute");
    this->reserveName("executeInitialPostProcessingTasks");
    this->reserveName("executePostProcessingTasks");
    this->reserveName("getName");
    this->reserveName("data");
  } // end of MFrontPleiadesModelParser::MFrontPleiadesModelParser()

  void
  MFrontPleiadesModelParser::treatBounds(void)
  {
    this->registerBounds(this->boundsDescriptions);
  } // end of MFrontPleiadesModelParser::treatBounds

  void
  MFrontPleiadesModelParser::treatPhysicalBounds(void)
  {
    this->registerBounds(this->physicalBoundsDescriptions);
  } // end of MFrontPleiadesModelParser::treatPhysicalBounds

  void
  MFrontPleiadesModelParser::registerBounds(std::vector<VariableBoundsDescription>& container)
  {
    using namespace std;
    VarContainer::const_iterator p;
    ComputedVarContainer::const_iterator p2;
    VariableBoundsDescription boundsDescription;
    unsigned short i;
    bool found;

    this->checkNotEndOfFile("MFrontPleiadesModelParser::registerBounds");

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
      this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
			      this->current->value+" is not a valid identifier.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::registerBounds","in");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::registerBounds ",
			    "Expected ']' or '['.");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParser::registerBounds ",
			      "Expected '*'.");
      if(this->current->value!="*"){
	this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      boundsDescription.boundsType = VariableBoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParser::registerBounds ",
			      "Expected lower bound value for variable "+boundsDescription.varName);
      istringstream converter(this->current->value);
      converter >> boundsDescription.lowerBound;
      boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::registerBounds",":");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::registerBounds",
			    "Could not read upper bound value for variable "+boundsDescription.varName);
    if(this->current->value=="*"){
      if(boundsDescription.boundsType==VariableBoundsDescription::Upper){
	this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
				"Upper and lower values bounds are both infinity. This is inconsistent.");
      }
      boundsDescription.boundsType=VariableBoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParser::registerBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> boundsDescription.upperBound;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
      if(boundsDescription.boundsType==VariableBoundsDescription::LowerAndUpper){
	if(boundsDescription.lowerBound>boundsDescription.upperBound){
	  this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
				  "Lower bound value is greater than upper bound value for variable '"+
				  boundsDescription.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParser::registerBounds",
			      "Expected ']'");
      if(this->current->value!="]"){
	this->throwRuntimeError("MFrontPleiadesModelParser::registerBounds",
				"Expected ']' (read '"+this->current->value+"'");
      }      
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::registerBounds",";");
    container.push_back(boundsDescription);
  } // end of MFrontPleiadesModelParser::registerBounds

  void
  MFrontPleiadesModelParser::treatMaterial(void)
  {
    ParserBase::treatMaterial();
    this->className = this->material+"_"+this->className;
  } // end of MFrontPleiadesModelParser::treatMaterial

  void
  MFrontPleiadesModelParser::treatModel(void)
  {
    MFrontModelParserBase<MFrontPleiadesModelParser>::treatModel();
    if(!this->material.empty()){
      this->className = this->material+"_"+this->className;
    }
  } // end of MFrontPleiadesModelParser::treatModel

  void
  MFrontPleiadesModelParser::setInterfaces(const std::set<std::string>&)
  {} // end of MFrontPleiadesModelParser::setInterfaces

  std::string
  MFrontPleiadesModelParser::getName(void)
  {
    return "PleiadesModel";
  } // end of MFrontPleiadesModelParser::getName(void)

  std::string
  MFrontPleiadesModelParser::getDescription()
  {
    return "this parser is used to define simple material models for pleiades";
  } // end of MFrontPleiadesModelParser::getDescription

  void
  MFrontPleiadesModelParser::setVerboseMode(void) 
  {
    this->verboseMode = true;
  } // MFrontPleiadesModelParser::setVerboseMode(void)

  void
  MFrontPleiadesModelParser::setWarningMode(void) 
  {
    this->warningMode = true;
  } // MFrontPleiadesModelParser::setVerboseMode(void)

  void
  MFrontPleiadesModelParser::setDebugMode(void) 
  {
    this->debugMode = true;
  }
  
  bool
  MFrontPleiadesModelParser::hasSpecializedConstructor(void) const
  {
    FunctionContainer::const_iterator p;
    if(!this->localParameters.empty()){
      return true;
    }
    if(!this->domains.empty()){
      return true;
    }
    for(p=this->functions.begin();p!=this->functions.end();++p){
      if(!p->domains.empty()){
	return true;
      }
    }
    return false;
  } // end of MFrontPleiadesModelParser::hasSpecializedConstructor

  void
  MFrontPleiadesModelParser::treatFunction(void)
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
    this->registerVariable("functor"+toString(this->functions.size()));
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunction");
    f.name = this->current->value;
    if(!this->isValidIdentifier(f.name)){
      string msg("MFrontPleiadesModelParser::treatFunction : ");
      msg += "function name '"+f.name+"' is not valid";
      throw(runtime_error(msg));
    }
    this->registerVariable(f.name);
    this->reserveName(f.name+".Domain");
    this->reserveName(f.name+".Domains");
    f.line = this->current->line;
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatFunction","{");
    ++openedBrackets;
    newInstruction = true;
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunction",
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
	  this->throwRuntimeError("MFrontPleiadesModelParser::treatFunction",
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
	    this->throwRuntimeError("MFrontPleiadesModelParser::treatFunction",
				    "Internal error while treating static var '"+
				    this->current->value+"' in the body of function "+
				    f.name+"'");
	  }
	  treated = true;
	  f.body  += "(" + this->className + "::" +f.name+"::"+ this->current->value + ")";
	} else if(this->varNames.find(this->current->value)!=this->varNames.end()){
	  treated = false;
	  if(newInstruction){
	    string var = this->current->value;
	    string op;
	    bool modifier;
	    ++(this->current);
	    if(this->current==fileTokens.end()){
	      string msg("MFrontPleiadesModelParser::treatFunction : ");
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
		this->throwRuntimeError("MFrontPleiadesModelParser::treatFunction",
					"trying to modify variable '"+var+
					"' in the body of function '"+f.name+"'");
	      }
	      f.body  += var + " " + op + " ";
	      f.modifiedVariables.insert(var);
	      p6 = f.depth.find(var);
	      if(p6==f.depth.end()){
		f.depth[var] = 0;
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
	    // treating the case of local variables
	    for(p =this->localVarsHolder.begin();
		(p!=this->localVarsHolder.end())&&(!treated);++p){
	      if(p->name==this->current->value){
		treated = true;
		f.localVars.insert(this->current->value);
		f.body  += "(this->" + this->current->value + ")";
	      }
	    }
	    if(!treated){
	      if(this->isInputVariable(this->current->value)){
		f.usedVariables.insert(this->current->value);
		dv = this->decomposeVariableName(this->current->value);
		p6 = f.depth.find(dv.first);
		if(p6!=f.depth.end()){
		  if(dv.second>p6->second){
		    f.depth[dv.first] = dv.second;
		  }
		} else {
		  f.depth[dv.first] = dv.second;
		}
		f.body  += this->current->value;
	      } else if(this->isOutputVariable(this->current->value)){
		f.usedVariables.insert(this->current->value);
		dv = this->decomposeVariableName(this->current->value);
		p6 = f.depth.find(dv.first);
		if(p6!=f.depth.end()){
		  if(dv.second>p6->second){
		    f.depth[dv.first] = dv.second;
		  }
		} else {
		  f.depth[dv.first] = dv.second;
		}
		f.body  += this->current->value;
	      }
	    }
	  }
	} else {
	  if(this->current->value=="dt"){
	    f.useTimeIncrement = true;
	    f.body  += "(this->dt)";
	  } else {
	    f.body  += this->current->value;
	  }
	}
	newInstruction = false;
      }
      newLine=false;
    }
    if((this->current==fileTokens.end())&&(openedBrackets!=0)){
      string msg("MFrontPleiadesModelParser::treatFunction : ");
      msg+="unexpected end of file while reading body of function " + f.name;
      throw(runtime_error(msg));
    }
    if(openedBrackets!=0){
      string msg("MFrontPleiadesModelParser::treatFunction : ");
      msg+="parenthesis still opened at the end of function " + f.name;
      throw(runtime_error(msg));
    }
    if(f.modifiedVariables.size()==0){
      string msg("MFrontPleiadesModelParser::treatFunction : ");
      msg+="function " + f.name + " does not change any variable.";
      throw(runtime_error(msg));
    }
    if(!this->functionNames.insert(f.name).second){
      string msg("MFrontPleiadesModelParser::treatFunction : ");
      msg+="function " + f.name + " already declared.";
      throw(runtime_error(msg));
    }

    for(p2=f.modifiedVariables.begin();p2!=f.modifiedVariables.end();++p2){
      if((p3=f.usedVariables.find(*p2))!=f.usedVariables.end()){
	f.usedVariables.erase(*p3);
      }
    }
    this->functions.push_back(f);
  } // end of MFrontPleiadesModelParser::treatFunction(void)

  bool
  MFrontPleiadesModelParser::is(const VarContainer& vc,
				const std::string& v) const
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
      p2 = this->depth.find(p->name);
      if(p2==this->depth.end()){
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
  } // end of MFrontPleiadesModelParser::isVariable(void)

  bool
  MFrontPleiadesModelParser::isInputVariable(const std::string& v) const
  {
    return this->is(this->inputs,v);
  } // end of MFrontPleiadesModelParser::isInputVariable(void)

  bool
  MFrontPleiadesModelParser::isOutputVariable(const std::string& v) const
  {
    return this->is(this->outputs,v);
  } // end of MFrontPleiadesModelParser::isInputVariable(void)
  
  void
  MFrontPleiadesModelParser::treatDomain(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(!this->domains.empty()){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      }
      this->throwRuntimeError("MFrontPleiadesModelParser::treatDomains",msg);
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatDomain");
    if(this->current->flag!=Token::String){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatDomain",
			      "Expected to read a string (read '"+this->current->value+"').");
    }
    if(this->current->value.size()<3){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatDomain : ",
			      "domain name too short.");
    }
    this->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second;
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatDomain",";");
  } // end of MFrontPleiadesModelParser::treatDomain(void)

  void
  MFrontPleiadesModelParser::treatDomains(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    bool bend;
    if(!this->domains.empty()){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      }
      this->throwRuntimeError("MFrontPleiadesModelParser::treatDomains",msg);
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatDomains");
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatDomains","{");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatDomains");
    bend = false;
    while((this->current!=this->fileTokens.end())&&(!bend)){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatDomains",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatDomains",
				"Domain name too short.");
      }
      if(!this->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatDomains",
				"domain "+this->current->value.substr(1,this->current->value.size()-2)+
				" already defined.");
      }
      ++(this->current);	
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatDomains",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++this->current;
      } else if(this->current->value=="}"){
	bend = true;
      } else {
	this->throwRuntimeError("MFrontPleiadesModelParser::treatDomains",
				"Expected ',' or ')' (read '"+this->current->value+"').");
      }
    }
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatDomains","}");
    if(this->domains.empty()){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatDomains",
			      "@Domains does not set any domain.");
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatDomains");
    ++(this->current);
  } // end of MFrontPleiadesModelParser::treatDomains(void)

  void
  MFrontPleiadesModelParser::treatInitializeParameters(void)
  {
    this->initializeParameters = this->readNextBlock(true);
  } // end of MFrontPleiadesModelParser::treatInitializeParameters(void)

  void
  MFrontPleiadesModelParser::treatFunctionMethod(void) 
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
      this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
			      "Internal error, no function named '"+this->currentVar+"'");
    }
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatFunctionMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunctionMethod");
    methodName = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunctionMethod");
    if(this->current->value!="("){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
			      "Unexpected token (expected '(', read '"+this->current->value+"').");
    }
    if(methodName=="setDomain"){
      if(!p->domains.empty()){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				"The domain of the function '"+this->currentVar+
				"' has already been defined.");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunctionMethod",
			      "Expected domain name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				"Expected to read a string");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				"Domain name too short.");
      }
      p->domains.insert(this->current->value.substr(1,this->current->value.size()-2));
      ++(this->current);
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunctionMethod",
			      "Expected ')'.");
      if(this->current->value!=")"){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				"Unexpected token (expected ')', read '"+this->current->value+"').");
      }
    } else if(methodName=="setDomains"){
      if(!p->domains.empty()){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				"The domain of the function '"+this->currentVar+"' has already been defined.");
      }
      ++(this->current);
      bend = false;
      while((this->current!=this->fileTokens.end())&&(!bend)){
	if(this->current->flag!=Token::String){
	  this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				  "Expected to read a string (read '"+this->current->value+"').");
	}
	if(this->current->value.size()<3){
	  this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				  "Domain name too short.");
	}
	if(!p->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				  "Domain "+this->current->value.substr(1,this->current->value.size()-2)+
				  " already defined.");
	}
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunctionMethod");
	if(this->current->value==","){
	  ++this->current;
	} else if(this->current->value==")"){
	  bend = true;
	} else {
	  this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				  "Expected ',' or ')' (read '"+this->current->value+"').");
	}
      }
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatFunctionMethod");
      if(p->domains.empty()){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
				"setDomains does not set any domain for function '"+this->currentVar+"'");
      }
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParser::treatFunctionMethod",
			      "Unknown function method (read '"+methodName+"')."
			      "Valid methods are setDomain, setDomains and setVariablesOrder.");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatFunctionMethod",";");
  } // end of MFrontPleiadesModelParser::treatFunctionMethod(void) 

  void
  MFrontPleiadesModelParser::treatOutputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    typedef map<string,string>::value_type MVType;
    typedef map<string,double>::value_type MVType2;
    typedef map<string,unsigned short>::value_type MVType3;
    unsigned short i;
    if(!this->functions.empty()){
      string msg("MFrontPleiadesModelParser::treatOutputMethod : ");
      msg += "output must be called before declaring functions";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatOutputMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatOutputMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultInitialValue")&&
       (methodName!="setDepth")){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
			      "Unknown method (valid methods for fields are "
			      "setGlossaryName, setEntryName, setDepth and "
			      "setDefaultInitialValue,"
			      "read '"+methodName+"').");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatOutputMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatOutputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatOutputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar + "'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Entry file name for field '"+this->currentVar+"' already defined.");
      }
    }  else if (methodName=="setDefaultInitialValue"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatOutputMethod",
			      "Expected intial value.");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Could not read initial value of field : '"+this->currentVar+"'");
      }
      if(!this->initialValues.insert(MVType2(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Initial value for field '"+this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatOutputMethod",
			      "Expected depth value.");
      unsigned short value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"Could not read depth value of field '"+this->currentVar+"'");
      }
      if(!this->depth.insert(MVType3(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				"depth value for field '"+this->currentVar+"' already defined.");
      }
      for(i=1;i<=value;++i){
	this->registerVariable(this->currentVar+"_"+toString(i));
	this->registerVariable("f_"+this->currentVar+"_"+toString(i));
	this->registerVariable("ff_"+this->currentVar+"_"+toString(i));
	if(!this->fieldNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
				  "Field '"+this->currentVar+"_"+toString(i)+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParser::treatOutputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatOutputMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatOutputMethod",";");
  } // end of MFrontPleiadesModelParser::treatOutputMethod

  void
  MFrontPleiadesModelParser::treatInputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    typedef map<string,string>::value_type MVType;
    typedef map<string,unsigned short>::value_type MVType2;
    unsigned short i;
    if(!this->functions.empty()){
      string msg("MFrontPleiadesModelParser::treatInputMethod : ");
      msg += "input method must be called before declaring functions";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatInputMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatInputMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName") &&
       (methodName!="setDepth")){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
			      "Unknown method (valid methods for input fields are "
			      "setGlossaryName, setEntryName, setDepth"
			      ", read '"+methodName+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatInputMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatInputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Glossary name for field '"+this->currentVar+"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatInputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatInputMethod",
			      "Expected depth value.");
      unsigned short value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Could not read initial value of field '"+this->currentVar+"'");
      }
      if(!this->depth.insert(MVType2(this->currentVar,value)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				"Initial value for field '"+this->currentVar+"' already defined.");
      }
      for(i=1;i<=value;++i){
	this->registerVariable(this->currentVar+"_"+toString(i));
	this->registerVariable("f_"+this->currentVar+"_"+toString(i));
	this->registerVariable("ff_"+this->currentVar+"_"+toString(i));
	if(!this->fieldNames.insert(this->currentVar+"_"+toString(i)).second){
	  this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
				  "Field '"+this->currentVar+"_"+toString(i)+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParser::treatInputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatInputMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatInputMethod",";");
  } // end of MFrontPleiadesModelParser::treatInputMethod

  void
  MFrontPleiadesModelParser::treatGlobalParameterMethod(void) 
  {
    using namespace std; 
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatGlobalParameterMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatGlobalParameterMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
			      "Unknown method (valid methods for global parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read '"+methodName+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatGlobalParameterMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatGlobalParameterMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatGlobalParameterMethod"
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatGlobalParameterMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatGlobalParameterMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatGlobalParameterMethod",";");
  } // end of MFrontPleiadesModelParser::treatGlobalParameterMethod

  void
  MFrontPleiadesModelParser::treatLocalParameterMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatLocalParameterMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatLocalParameterMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
			      "Unknown method (valid methods for local parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read "+methodName+").");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatLocalParameterMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatLocalParameterMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'.");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatLocalParameterMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatLocalParameterMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatLocalParameterMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatLocalParameterMethod",";");
  } // end of MFrontPleiadesModelParser::treatLocalParameterMethod

  void
  MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    string methodName;
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",".");
    this->checkNotEndOfFile("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
			      "Unknown method (valid methods for local parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read "+methodName+").");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'.");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert(MVType(this->currentVar,glossaryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert(MVType(this->currentVar,entryName)).second){
	this->throwRuntimeError("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",")");
    this->readSpecifiedToken("MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod",";");
  } // end of MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod

  void
  MFrontPleiadesModelParser::treatFile(const std::string& fileName_) 
  {
    using namespace std;
    this->fileName = fileName_;
    this->openFile(fileName_);
    CallBackContainer::const_iterator p;
    VarContainer::const_iterator p2;
    FunctionContainer::const_iterator p3;
    MemberFuncPtr handler = 0;
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->fileTokens.begin();
    while(this->current != this->fileTokens.end()){
      p = this->callBacks.find(this->current->value);
      if(p==this->callBacks.end()){
	bool found = false;
	for(p3=this->functions.begin();(p3!=this->functions.end())&&(!found);){
	  if(p3->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParser::treatFunctionMethod;
	  } else {
	    ++p3;
	  }
	}
	for(p2=this->outputs.begin();(p2!=this->outputs.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParser::treatOutputMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->inputs.begin();(p2!=this->inputs.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParser::treatInputMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->globalParameters.begin();(p2!=this->globalParameters.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParser::treatGlobalParameterMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->localParameters.begin();(p2!=this->localParameters.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParser::treatLocalParameterMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->constantMaterialProperties.begin();(p2!=this->constantMaterialProperties.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &MFrontPleiadesModelParser::treatConstantMaterialPropertyMethod;
	  } else {
	    ++p2;
	  }
	}
	if(!found){
	  handler = &MFrontPleiadesModelParser::treatUnknownKeyword;
	}
      } else {
	handler = p->second;
      }
      ++(this->current);
      (this->*handler)();
    }
  } // end of MFrontPleiadesModelParser::treatFile

  void
  MFrontPleiadesModelParser::openOutputFiles(void)
  {
    using namespace std;
    using namespace tfel::system;
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/Model");
    systemCall::mkdir("src");
    this->headerFile.open(this->headerFileName.c_str());
    if(!this->headerFile){
      string msg("MFrontPleiadesModelParser::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MFrontPleiadesModelParser::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.exceptions(ios::badbit|ios::failbit);
  } // end of MFrontPleiadesModelParser::openOutputFiles()

  void
  MFrontPleiadesModelParser::closeOutputFiles(void)
  {
    this->headerFile.close();
    this->srcFile.close();
  } // end of MFrontPleiadesModelParser::closeOutputFiles()
  
  void
  MFrontPleiadesModelParser::generateOutputFiles(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    FunctionContainer::const_iterator p2;
    bool found;
    this->hasDefaultConstructor=true;
    if(!this->localParameters.empty()){
      for(p=this->localParameters.begin();
	  (p!=this->localParameters.end())&&(this->hasDefaultConstructor);++p){
	if(this->defaultValues.find(p->name)==this->defaultValues.end()){
	  this->hasDefaultConstructor = false;
	}
      }
    }
    // sanity checks
    for(p2=this->functions.begin();p2!=this->functions.end();++p2){
      if(p2->modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelParser::generateOutputFiles : ");
	msg += "function '"+p2->name+"' does not modify any output";
	throw(runtime_error(msg));
      }
    }
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      found = false;
      for(p2=this->functions.begin();
	  (p2!=this->functions.end())&&(!found);++p2){
	if(p2->modifiedVariables.find(p->name)!=p2->modifiedVariables.end()){
	  found = true;
	}
      }
      if(!found){
	string msg("MFrontPleiadesModelParser::generateOutputFiles : ");
	msg += "output '"+p->name+"' is not modified by any function";
	throw(runtime_error(msg));
      }
    }
    this->writeHeaderFile();
    this->writeSrcFile();
  } // end of MFrontPleiadesModelParser::generateOutputFiles(void)

  void
  MFrontPleiadesModelParser::writeHeaderFile(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    StaticVarContainer::const_iterator p2;
    FunctionContainer::const_iterator p3;
    set<string>::iterator p4;
    set<string>::iterator p5;
    unsigned short i;
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
		     << MFrontPleiadesModelParser::makeUpperCase(this->className)
		     << "_HH\n";
    this->headerFile << "#define _PLEIADES_"
		     << MFrontPleiadesModelParser::makeUpperCase(this->className)
		     << "_HH\n\n";

    this->headerFile << "#include<string>\n\n";
    this->headerFile << "#include\"Pleiades/Model/ModelBase.hxx\"\n\n";
    if(!this->includes.empty()){
      this->headerFile << this->includes << endl << endl;
    }
    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace pleiades\n{\n\n";
    this->headerFile << "namespace model\n{\n\n";
    this->headerFile << "struct MFRONT_SHAREDOBJ " << this->className << endl;
    this->headerFile << ": public ModelBase\n";
    this->headerFile << "{\n\n";
    this->headerFile << "//! Default constructor\n";
    this->headerFile << this->className
		     << "(pleiades::mesh::SMeshManager&,\n"
		     << "pleiades::field::SFieldManager&,\n"
		     << "pleiades::time::SClock&,\n"
		     << "pleiades::loading::SLoadingManager&,\n"
		     << "pleiades::signal::SSignalManager&);\n\n";
    if(this->hasSpecializedConstructor()){
      this->headerFile << "//! Constructor to initialize local parameters\n";
      this->headerFile << this->className
		       << "(const std::map<std::string,pleiades::parser::Data>&,\n"
		       << "pleiades::mesh::SMeshManager&,\n"
		       << "pleiades::field::SFieldManager&,\n"
		       << "pleiades::time::SClock&,\n"
		       << "pleiades::loading::SLoadingManager&,\n"
		       << "pleiades::signal::SSignalManager&);\n\n";
    }
    this->headerFile << "std::string\ngetName(void) const;\n\n"
		     << "void\ninitialize(const pleiades::parser::DataManager&);\n\n"
		     << "void\ndeclareRequirements(pleiades::coupling::SRequirementManager&);\n\n"
		     << "void\nresolveRequirements(pleiades::coupling::SRequirementManager&);\n\n"
		     << "void\ninitializeOutput(pleiades::coupling::Requirement&);\n\n"
		     << "void\nexecute(void);\n\n";
    this->headerFile << "void\nexecuteInitialPostProcessingTasks(const bool);\n\n";
    this->headerFile << "void\nexecutePostProcessingTasks(const bool);\n\n";
    this->headerFile << "~" << this->className << "();\n\n";
    this->headerFile << "private:\n\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "//! Copy constructor (disabled)\n";
    this->headerFile << this->className << "(const " 
		     << this->className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << this->className << "&\n"
		     << "operator=(const " 
		     << this->className << "&);\n\n";
    for(p3=this->functions.begin();p3!=this->functions.end();++p3){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelParser::writeHeaderFile : ");
	msg+="function " + p3->name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << "struct " << p3->name<< "\n";
      this->headerFile << "{\n\n";
      if((p3->modifiedVariables.size()==1)&&
	 (p3->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	this->headerFile << "typedef pleiades::field::real Result;\n";
	for(i=1,p4=p3->usedVariables.begin();p4!=p3->usedVariables.end();++i,++p4){
	  this->headerFile << "typedef pleiades::field::real Arg" << i << ";\n";
	}
	this->headerFile << "\nResult\n";
	this->headerFile << "operator()(";
	if(p3->usedVariables.size()==0){
	  this->headerFile << "void";
	} else {
	  for(i=1,p4=p3->usedVariables.begin();p4!=p3->usedVariables.end();++i){
	    this->headerFile << "const Arg" << i;
	    if((++p4)!=p3->usedVariables.end()){
	      this->headerFile << ",\n";
	    }
	  }
	}
      } else {
	if(p3->usedVariables.size()==0){
	  this->headerFile << "void\n";
	  this->headerFile << "operator()(std::vector<pleiades::field::real>&";
	} else {
	  this->headerFile << "void\n";
	  this->headerFile << "operator()(std::vector<pleiades::field::real>&,\n"
			   << "const std::vector<pleiades::field::real>&";
	}
      }
      this->headerFile << ") const;\n\n";
      this->headerFile << "private:\n\n";
      this->headerFile << "friend class " << this->className <<";\n\n";
      for(p2=p3->staticVars.begin();p2!=p3->staticVars.end();++p2){
	if(this->debugMode){
	  this->headerFile << "#line " << p2->lineNumber << " \"" 
			   << this->fileName << "\"\n";
	}
	this->headerFile << "static const " << p2->type << " " << p2->name << ";" << endl;
      }
      if(!p3->domains.empty()){
	this->headerFile << "std::vector<std::string> domains;" << endl;
      }
      for(p4=p3->globalParameters.begin();p4!=p3->globalParameters.end();++p4){
	p = MFrontPleiadesModelParser::findVariableDescription(this->globalParameters,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << this->fileName << "\"\n";
	}
	this->headerFile << p->type << " " << p->name << ";\n";
      }
      for(p4=p3->localParameters.begin();p4!=p3->localParameters.end();++p4){
	p = MFrontPleiadesModelParser::findVariableDescription(this->localParameters,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << this->fileName << "\"\n";
	}
	this->headerFile << p->type << " " << p->name << ";\n";
      }
      for(p4=p3->constantMaterialProperties.begin();p4!=p3->constantMaterialProperties.end();++p4){
	p = MFrontPleiadesModelParser::findVariableDescription(this->constantMaterialProperties,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << this->fileName << "\"\n";
	}
	this->headerFile << p->type << " " << p->name << ";\n";
      }
      for(p4=p3->localVars.begin();p4!=p3->localVars.end();++p4){
	p = MFrontPleiadesModelParser::findVariableDescription(this->localVarsHolder,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << this->fileName << "\"\n";
	}
	this->headerFile << p->type << " " << p->name << ";\n";
      }
      if(p3->useTimeIncrement){
	this->headerFile << "pleiades::field::real dt;\n";
      }
      this->headerFile << "}; // end of struct " << p3->name<< "\n\n";
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
    if(!this->domains.empty()){
      this->headerFile << "std::vector<std::string> domains;\n";
    }
    for(p=this->globalParameters.begin();p!=this->globalParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n";
    }
    for(p=this->localParameters.begin();p!=this->localParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n";
    }
    for(p=this->localVarsHolder.begin();p!=this->localVarsHolder.end();++p,++i){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n";
    }
    this->headerFile << "void\ninitializeOutputsVariablesDepths(void);\n";
    this->headerFile << "void\ninitializeOutputsVariablesInitialValues(const pleiades::parser::DataManager&);\n";
    this->headerFile << "void\ninitializeInputsVariablesDepths(void);";
    if(!this->constantMaterialProperties.empty()){
      this->headerFile << "\nvoid\ninitializeConstantMaterialProperties(const pleiades::parser::DataManager&);\n\n";
    } else {
      this->headerFile << "\n\n";
    }
    for(i=0,p3=this->functions.begin();p3!=this->functions.end();++p3,++i){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelParser::writeHeaderFile : ");
	msg += "function " + p3->name + " does not modify any variable ";
	msg += "(internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << this->fileName << "\"\n";
      }
      this->headerFile << p3->name << " functor" << i <<  ";\n";
    }
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned short> > outputsDepths;\n";
    this->headerFile << "std::map<std::string,std::map<std::string,pleiades::field::real> > outputsInitialValues;\n";
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned short> > inputsDepths;\n";
    if(!this->constantMaterialProperties.empty()){
      this->headerFile << "std::map<std::string,std::map<std::string,pleiades::field::real> > constantMaterialProperties;\n";
    }
    this->headerFile << "}; // end of struct " << this->className << endl << endl;
    this->headerFile << "} // end of namespace model\n\n";
    this->headerFile << "} // end of namespace pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << MFrontPleiadesModelParser::makeUpperCase(this->className)
		     << "_HH */\n";
  } // end of MFrontPleiadesModelParser::writeHeaderFile(void)

  std::string
  MFrontPleiadesModelParser::getGenTypeMethod(const std::string& type) const
  {
    using namespace std;
    if(type=="real"){
      return "get<real>";
    } else if(type=="double"){
      return "get<double>";
    } else if(type=="string"){
      return "get<string>";
    } else if(type=="StringArray"){
      return "get<vector<string> >";
    } else if(type=="DoubleArray"){
      return "get<vector<double> >";
    } else {
      string msg("MFrontPleiadesModelParser::getGenTypeMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontPleiadesModelParser::getGenTypeMethod

  std::string
  MFrontPleiadesModelParser::isGenTypeMethod(const std::string& type) const
  {
    using namespace std;
    if(type=="real"){
      return "is<real>";
    } else if(type=="double"){
      return "is<double>";
    } else if(type=="string"){
      return "is<string>";
    } else if(type=="StringArray"){
      return "is<vector<string> >";
    } else if(type=="DoubleArray"){
      return "is<vector<double> >";
    } else {
      string msg("MFrontPleiadesModelParser::isGenTypeMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontPleiadesModelParser::isGenTypeMethod

  VarContainer::const_iterator
  MFrontPleiadesModelParser::findVariableDescription(const VarContainer& v,
						     const std::string& n)
  {
    using namespace std;
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      if(p->name==n){
	return p;
      }
    }
    string msg("MFrontPleiadesModelParser::findVariableDescription : ");
    msg += "no variable named '"+n+"' found";
    throw(runtime_error(msg));
    return v.end();
  } // MFrontPleiadesModelParser::findVariableDescription

  void
  MFrontPleiadesModelParser::writeSrcFile(void)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    map<string,string>::const_iterator p4;
    map<string,unsigned short>::const_iterator p5;
    StaticVarContainer::const_iterator p10;
    FunctionContainer::const_iterator p11;
    set<string>::const_iterator p12;
    std::map<std::string,std::vector<Function> >::iterator p13;
    std::vector<Function>::iterator p14;
    set<string>::const_iterator p15;
    set<string>::const_iterator p16;
    set<unsigned short> applyHeaders;
    set<unsigned short>::const_iterator p18;
    vector<VariableBoundsDescription>::const_iterator p19;
    vector<VariableBoundsDescription>::const_iterator p20;
    unsigned short specializedParametersNumber;
    unsigned short i;
    bool first;
    bool found;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << this->className << " pleiades model.\n";
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
    this->srcFile << "#include<stdexcept>\n";
    this->srcFile << "#include<sstream>\n";
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include\"Pleiades/Global.hxx\"\n";
    this->srcFile << "#include\"Pleiades/OutOfBoundsPolicy.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Parser/DataManager.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Glossary/Glossary.hxx\"\n";
    // Functions
    for(p11=this->functions.begin();p11!=this->functions.end();++p11){
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	applyHeaders.insert(static_cast<unsigned short>(p11->usedVariables.size()));
      }
    }
    for(p18=applyHeaders.begin();p18!=applyHeaders.end();++p18){
      this->srcFile << "#include\"Pleiades/Field/FieldApply" << *p18 << ".hxx\"\n";
    }
    this->srcFile << "#include\"Pleiades/Model/MaterialDescription.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/IModelFactory.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/" 
		  << this->className << "-pleiades.hxx\"\n\n";
    this->srcFile << "namespace pleiades\n{\n\n";
    this->srcFile << "namespace model\n{\n\n";
    // Functors
    for(p11=this->functions.begin();p11!=this->functions.end();++p11){
      // static variables
      if(!p11->staticVars.empty()){
	for(p10=p11->staticVars.begin();p10!=p11->staticVars.end();++p10){
	  if(this->debugMode){
	    this->srcFile << "#line " << p10->lineNumber << " \"" 
			  << this->fileName << "\"\n";
	  }
	  this->writeStaticVariableInitialization(this->className + "::" + p11->name,
						  *p10);
	}
	this->srcFile << endl;
      }
      // operator()
      if(this->debugMode){
	this->srcFile << "#line " << p11->line << " \"" 
		      << this->fileName << "\"\n";
      }
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	this->srcFile << "pleiades::field::real\n";
	this->srcFile << this->className << "::" << p11->name << "::operator()(";
	if(p11->usedVariables.size()==0){
	  if(p11->modifiedVariables.size()==1){
	    this->srcFile << "void";
	  }
	} else {
	  if(p11->modifiedVariables.size()!=1){
	    this->srcFile << ",\n";
	  }
	  for(p12=p11->usedVariables.begin();p12!=p11->usedVariables.end();){
	    this->srcFile << "const pleiades::field::real " << *p12;
	    if((++p12)!=p11->usedVariables.end()){
	      this->srcFile << ",\n";
	    }
	  }
	}
      } else {
	this->srcFile << "void\n";
	this->srcFile << this->className << "::" << p11->name << "::operator()(";
	this->srcFile << "std::vector<pleiades::field::real>& results";
	if(p11->usedVariables.size()!=0){
	  this->srcFile << ",\nconst std::vector<pleiades::field::real>& values";
	}
      }
      this->srcFile << ") const\n";
      this->srcFile << "{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using pleiades::field::real;\n";
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	this->srcFile << "real " << *(p11->modifiedVariables.begin()) << ";\n";	
      } else {
	for(i=0,p12=p11->usedVariables.begin();p12!=p11->usedVariables.end();++p12,++i){
	  this->srcFile << "const real " << *p12 << " = values[" << i<< "];\n";
	}
	for(i=0,p12=p11->modifiedVariables.begin();p12!=p11->modifiedVariables.end();++p12,++i){
	  this->srcFile << "real " << *p12 << " = results[" << i << "];\n";
	}
      }
      writeMaterialLaws("MFrontPleiadesModelParser::writeSrcFile",
			this->srcFile,this->materialLaws);		      
      for(i=0,p12=p11->usedVariables.begin();p12!=p11->usedVariables.end();++p12,++i){
	found = false;
	for(p19=this->physicalBoundsDescriptions.begin();
	    (p19!=this->physicalBoundsDescriptions.end())&&(!found);){
	  found = (p19->varName==*p12);
	  if(!found){
	    ++p19;
	  }
	}
	found = false;
	for(p20=this->boundsDescriptions.begin();
	    (p20!=this->boundsDescriptions.end())&&(!found);){
	  found = (p20->varName==*p12);
	  if(!found){
	    ++p20;
	  }
	}
	if((p19!=this->physicalBoundsDescriptions.end())||
	   (p20!=this->boundsDescriptions.end())){
	  this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
	}
	if(p19!=this->physicalBoundsDescriptions.end()){
	  this->srcFile << "// checking " << p19->varName<< " physical bounds\n";
	  if((p19->boundsType==VariableBoundsDescription::Lower)||
	     (p19->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p19->varName<< " < "<< p19->lowerBound << "){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << this->className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p19->varName << "' is below its physical lower bound (\" << "
			  << p19->varName << " << \"<" << p19->lowerBound << ").\\n\";\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n";
	    this->srcFile << "}\n";
	  }
	  if((p19->boundsType==VariableBoundsDescription::Upper)||
	     (p19->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p19->varName << " > " << p19->upperBound << "){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << this->className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p19->varName << "' is over its physical upper bound (\" << "
			  << p19->varName << " << \" > " << p19->upperBound << ").\\n\";\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n",
	    this->srcFile << "}\n";
	  }
	}
	if(p20!=this->physicalBoundsDescriptions.end()){
	  this->srcFile << "// checking " << p20->varName<< " bounds\n";
	  if((p20->boundsType==VariableBoundsDescription::Lower)||
	     (p20->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p20->varName<< " < "<< p20->lowerBound << "){\n";
	    this->srcFile << "if(pleiades::getOutOfBoundsPolicy()!= pleiades::NO_OUT_OF_BOUNDS_POLICY){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << this->className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p20->varName << "' is below its physical lower bound (\" << "
			  << p20->varName << " << \"<" << p20->lowerBound << ").\\n\";\n";
	    this->srcFile << "switch(pleiades::getOutOfBoundsPolicy()){\n";
	    this->srcFile << "case pleiades::STRICT_OUT_OF_BOUNDS_POLICY :\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n";
	    this->srcFile << "break;\n";
	    this->srcFile << "case pleiades::WARNING_OUT_OF_BOUNDS_POLICY :\n";
	    this->srcFile << "pleiades::printWarning(msg.str());\n";
	    this->srcFile << "break;\n";
	    this->srcFile << "case pleiades::NO_OUT_OF_BOUNDS_POLICY :\n";
	    this->srcFile << "break;\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	  }
	  if((p20->boundsType==VariableBoundsDescription::Upper)||
	     (p20->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p20->varName<< " > "<< p20->upperBound << "){\n";
	    this->srcFile << "if(pleiades::getOutOfBoundsPolicy()!= pleiades::NO_OUT_OF_BOUNDS_POLICY){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << this->className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p20->varName << "' is over its physical lower bound (\" << "
			  << p20->varName << " << \" > " << p20->upperBound << ").\\n\";\n";
	    this->srcFile << "switch(pleiades::getOutOfBoundsPolicy()){\n";
	    this->srcFile << "case pleiades::STRICT_OUT_OF_BOUNDS_POLICY :\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n";
	    this->srcFile << "break;\n";
	    this->srcFile << "case pleiades::WARNING_OUT_OF_BOUNDS_POLICY :\n";
	    this->srcFile << "pleiades::printWarning(msg.str());\n";
	    this->srcFile << "break;\n";
	    this->srcFile << "case pleiades::NO_OUT_OF_BOUNDS_POLICY :\n";
	    this->srcFile << "break;\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	    this->srcFile << "}\n";
	  }
	}
	if((p19!=this->physicalBoundsDescriptions.end())||
	   (p20!=this->boundsDescriptions.end())){
	  this->srcFile << "#endif /* NO_PLEIADES_BOUNDS_CHECK */\n";
	}
      }
      this->srcFile << p11->body;
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	this->srcFile << "return " << *(p11->modifiedVariables.begin()) << ";\n";
      } else {
	for(i=0,p12=p11->modifiedVariables.begin();p12!=p11->modifiedVariables.end();++p12,++i){
	  this->srcFile << "results[" << i << "] = " <<  *p12 << ";\n";
	}
      }
      this->srcFile << "} // end of " << this->className << "::" << p11->name << "::operator()\n\n";
    }
    // static variables
    if(!this->staticVars.empty()){
      for(p10=this->staticVars.begin();p10!=this->staticVars.end();++p10){
	this->writeStaticVariableInitialization(this->className,*p10);
      }
      this->srcFile << endl;
    }
    // Constructors
    if(this->hasDefaultConstructor){
      this->srcFile << this->className << "::" 
		    << this->className
		    << "(pleiades::mesh::SMeshManager& m,\n"
		    << "pleiades::field::SFieldManager& mf,\n"
		    << "pleiades::time::SClock& c,\n"
		    << "pleiades::loading::SLoadingManager& l,\n"
		    << "pleiades::signal::SSignalManager& s)\n"
		    << ": ModelBase(m,mf,c,l,s)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace pleiades::glossary;\n";
      this->srcFile << "vector<string> tmp;\n";
      this->srcFile << "vector<string>::const_iterator ptr;\n";
      this->srcFile << "vector<string>::const_iterator ptr2;\n";
      for(p=this->localParameters.begin();p!=this->localParameters.end();++p){
	string name;
	name = this->getPleiadesVariableName(p->name);
	p4=this->defaultValues.find(p->name);
	assert(p4!=this->defaultValues.end());
	this->writeAssignDefaultValue(p,p4);
      }
      if(!this->domains.empty()){
	for(p15=this->domains.begin();p15!=this->domains.end();++p15){
	  this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	  this->srcFile << "for(ptr=tmp.begin();ptr!=tmp.end();++ptr){\n";
	  this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr)!=this->domains.end()){\n";
	  this->srcFile << "string msg(\"" << this->className << "::initialize : \");\n";
	  this->srcFile << "msg += \"domain '\"+*ptr+\"' multiply defined\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "this->domains.push_back(*ptr);\n";
	  this->srcFile << "}\n";
	}
      }
      for(i=0,p11=this->functions.begin();p11!=this->functions.end();++p11,++i){
	if(!p11->domains.empty()){
	  string functor = "functor"+toString(i);
	  for(p15=p11->domains.begin();p15!=p11->domains.end();++p15){
	    this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	    this->srcFile << "if(find(this->" << functor << ".domains.begin(),"
			  << "this->" << functor << ".domains.end(),*ptr2)!=" 
			  << "this->" << functor << ".domains.end()){\n";
	    this->srcFile << "string msg(\"" << this->className << "::" 
			  << this->className << " : \");\n";
	    this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
	    this->srcFile << "throw(runtime_error(msg));\n";
	    this->srcFile << "}\n";
	    this->srcFile << "this->" << functor << ".domains.push_back(*ptr2);\n";
	    this->srcFile << "}\n";
	  }
	}
      }
      this->srcFile << "this->initializeOutputsVariablesDepths();\n";
      this->srcFile << "this->initializeInputsVariablesDepths();\n";
      this->srcFile << "} // end of " 
		    << this->className << "::" 
		    << this->className <<"\n\n";
    }
    if(this->hasSpecializedConstructor()){
      this->srcFile << "//! Constructor to initialize local parameters\n";
      this->srcFile << this->className << "::" 
		    << this->className 
		    << "(const std::map<std::string,pleiades::parser::Data>& data,\n"
		    << "pleiades::mesh::SMeshManager& m,\n"
		    << "pleiades::field::SFieldManager& mf,\n"
		    << "pleiades::time::SClock& c,\n"
		    << "pleiades::loading::SLoadingManager& l,\n"
		    << "pleiades::signal::SSignalManager& s)\n"
		    << ": ModelBase(m,mf,c,l,s)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace pleiades::field;\n";
      this->srcFile << "using namespace pleiades::glossary;\n";
      this->srcFile << "using namespace pleiades::parser;\n";
      this->srcFile << "static const string ValidParametersNames[] = {";
      specializedParametersNumber=0u;
      first = true;
      for(p=this->localParameters.begin();p!=this->localParameters.end();){
	this->srcFile << '"' << p->name << "\"";
	if(++p!=this->localParameters.end()){
	  this->srcFile << ",\n";
	}
	if(first){
	  first = false;
	}
	++(specializedParametersNumber);
      }
      if(!this->domains.empty()){
	if(first){
	  this->srcFile << "\"domains\"";
	  first = false;
	} else {
	  this->srcFile << ",\n\"domains\"";
	}
	++(specializedParametersNumber);
      }
      for(p11=this->functions.begin();p11!=this->functions.end();++p11){
	if(!p11->domains.empty()){
	  if(first){
	    this->srcFile << "\"" << p11->name << ".domains\"";
	    first = false;
	  } else {
	    this->srcFile << ",\n\"" << p11->name << ".domains\"";
	  }
	  ++(specializedParametersNumber);
	}
      }
      this->srcFile << "};\n";
      this->srcFile << "vector<string> tmp;\n";
      this->srcFile << "map<string,Data>::const_iterator ptr;\n";
      this->srcFile << "vector<string>::const_iterator ptr2;\n";
      this->srcFile << "vector<string>::const_iterator ptr3;\n";
      this->srcFile << "for(ptr=data.begin();ptr!=data.end();++ptr){\n";
      this->srcFile << "if(find(ValidParametersNames,ValidParametersNames+" 
		    << this->localParameters.size()
		    << ",ptr->first)==ValidParametersNames+"
		    << specializedParametersNumber << "){\n";
      this->srcFile << "string msg(\"" << this->className << "::" << this->className << " : \");\n";
      this->srcFile << "msg += \"unknown parameter '\"+ptr->first+\"'\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      for(p=this->localParameters.begin();p!=this->localParameters.end();++p){
	string name;
	name = this->getPleiadesVariableName(p->name);
	this->srcFile << "ptr = data.find(" << name << ");\n";
	this->srcFile << "if(ptr==data.end()){\n";
	if((p4=this->defaultValues.find(p->name))!=this->defaultValues.end()){
	  this->writeAssignDefaultValue(p,p4);
	  this->srcFile << "} else {\n";
	} else {
	  this->srcFile << "string msg(\"" << className << "::" << className << " : \");\n";
	  this->srcFile << "msg += \"can't initialize parameter " << p->name << "\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "} else {\n";
	}
	this->srcFile << "if(!ptr->second." << this->isGenTypeMethod(p->type) << "()){\n";
	this->srcFile << "string msg(\"" << className << "::" << className << " : \");\n";
	this->srcFile << "msg += \"wrong type for parameter '" << p->name << "' (expected a '"+p->type+"')\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->" << p->name << " = ptr->second." 
		      << this->getGenTypeMethod(p->type) << "();\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "ptr = data.find(\"domains\");\n";
      this->srcFile << "if(ptr!=data.end()){\n";
      this->srcFile << "if(!ptr->second.is<vector<string> >()){\n";
      this->srcFile << "string msg(\"" << this->className << "::" << this->className << " : \");\n";
      this->srcFile << "msg += \"invalid type for parameter 'domains'\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "for(ptr3=ptr->second.get<vector<string> >().begin();\n";
      this->srcFile << "ptr3!=ptr->second.get<vector<string> >().end();++ptr3){\n";
      this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,*ptr3);\n";
      this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
      this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
      this->srcFile << "string msg(\"" << this->className << "::" << this->className << ": \");\n";
      this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "this->domains.push_back(*ptr2);\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      this->srcFile << "} else {\n";
      for(p15=this->domains.begin();p15!=this->domains.end();++p15){
	this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
	this->srcFile << "string msg(\"" << this->className << "::" 
		      << this->className << " : \");\n";
	this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->domains.push_back(*ptr2);\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "}\n";
      for(i=0,p11=this->functions.begin();p11!=this->functions.end();++p11,++i){
	if(!p11->domains.empty()){
	  string functor = "functor"+toString(i);
	  this->srcFile << "ptr = data.find(\"" << functor << ".domains\");\n";
	  this->srcFile << "if(ptr!=data.end()){\n";
	  this->srcFile << "if(!ptr->second.is<vector<string> >()){\n";
	  this->srcFile << "string msg(\"" << this->className << "::" << this->className << " : \");\n";
	  this->srcFile << "msg += \"invalid type for parameter '"+functor+".domains'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "for(ptr3=ptr->second.get<vector<string> >().begin();\n";
	  this->srcFile << "ptr3!=ptr->second.get<vector<string> >().end();++ptr3){\n";
	  this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,*ptr3);\n";
	  this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	  this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
	  this->srcFile << "string msg(\"" << this->className << "::" << this->className << ": \");\n";
	  this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "this->domains.push_back(*ptr2);\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  for(p15=p11->domains.begin();p15!=p11->domains.end();++p15){
	    this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	    this->srcFile << "if(find(this->" << functor << ".domains.begin(),"
			  << "this->" << functor << ".domains.end(),*ptr2)!=" 
			  << "this->" << functor << ".domains.end()){\n";
	    this->srcFile << "string msg(\"" << this->className << "::" 
			  << this->className << " : \");\n";
	    this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
	    this->srcFile << "throw(runtime_error(msg));\n";
	    this->srcFile << "}\n";
	    this->srcFile << "this->" << functor << ".domains.push_back(*ptr2);\n";
	    this->srcFile << "}\n";
	  }
	  this->srcFile << "}\n";
	}
      }
      this->srcFile << "this->initializeOutputsVariablesDepths();\n";
      this->srcFile << "this->initializeInputsVariablesDepths();\n";
      this->srcFile <<"} // end of "
		    << this->className << "::" 
		    << this->className <<"\n\n";
    }
    this->writeInitializeOutputsVariablesDepths();
    this->writeInitializeInputsVariablesDepths();
    // getName
    this->srcFile << "std::string\n"
		  << this->className
		  << "::getName(void) const\n"
		  << "{\n"
		  << "return \"" << this->className << "\";\n"
		  << "}\n\n";
    // initialize
    this->srcFile << "void\n"
		  << this->className
		  << "::initialize(const pleiades::parser::DataManager& data)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string> tmp;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(p=this->globalParameters.begin();p!=this->globalParameters.end();++p){
      string name = this->getPleiadesVariableName(p->name);
      this->srcFile << "if(!data.contains(" << name << ")){\n";
      if((p4=this->defaultValues.find(p->name))!=this->defaultValues.end()){
	this->srcFile << "this->" << p->name << " = " << p4->second << ";" << endl;
	this->srcFile << "} else {\n";
      } else {
	this->srcFile << "string msg(\"" << this->className << "::initialize : \");\n";
	this->srcFile << "msg += \"can't initialize parameter '" << p->name << "'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "} else {\n";
      }
      this->srcFile << "if(!data." << this->isGenTypeMethod(p->type) << "(" << name << ")){\n";
      this->srcFile << "string msg(\"" << className << "::" << className << " : \");\n";
      this->srcFile << "msg += \"wrong type for parameter '" << p->name << "' (expected a '"+p->type+"')\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "this->" << p->name << " = data." 
		    << this->getGenTypeMethod(p->type) << "(" << name << ");\n";
      this->srcFile << "}\n";
    }
    if(!this->initializeParameters.empty()){
      this->srcFile << "// initialize parameters\n";
      this->srcFile << this->initializeParameters;
      this->srcFile << endl;
    }
    if(!this->initLocalVars.empty()){
      this->srcFile << "// initialize local variables\n";
      this->srcFile << this->initLocalVars;
    }
    this->srcFile << "this->initializeOutputsVariablesInitialValues(data);\n";
    if(!this->constantMaterialProperties.empty()){
      this->srcFile << "this->initializeConstantMaterialProperties(data);\n";
    }
    // initializing functor members
    for(i=0,p11=this->functions.begin();p11!=this->functions.end();++p11,++i){
      string functor = "functor"+toString(i);
      for(p15=p11->localParameters.begin();
	  p15!=p11->localParameters.end();++p15){
	this->srcFile << "this->" << functor << '.' << *p15 << " = this->" << *p15 << ";\n";
      }
      for(p15=p11->globalParameters.begin();p15!=p11->globalParameters.end();++p15){
	this->srcFile << "this->" << functor << '.' << *p15 << " = this->" << *p15 << ";\n";
      }
      for(p15=p11->localVars.begin();p15!=p11->localVars.end();++p15){
	this->srcFile << "this->" << functor << '.' << *p15 << " = this->" << *p15 << ";\n";
      }
    }
    this->srcFile << "} // end of " << this->className << "::initialize\n\n";
    this->writeInitializeOutputsVariablesInitialValues();
    if(!this->constantMaterialProperties.empty()){
      this->writeInitializeConstantMaterialProperties();
    }
    // declareRequirements
    this->srcFile << "void\n";
    this->srcFile << this->className << "::declareRequirements(pleiades::coupling::SRequirementManager& requirementManager)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::coupling;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "map<string,map<string,unsigned short> >::const_iterator ptr;\n";
    this->srcFile << "map<string,unsigned short>::const_iterator ptr2;\n";
    this->srcFile << "for(ptr=this->inputsDepths.begin();ptr!=this->inputsDepths.end();++ptr){\n";
    this->srcFile << "const map<string,unsigned short>& tmp = ptr->second;\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile << "requirementManager.addRequirement(static_cast<IModel&>(*this),\n";
    this->srcFile << "ptr->first,ptr2->first,\n";
    this->srcFile << "Requirement::SCALAR,\n";
    this->srcFile << "Requirement::MANDATORY,";
    this->srcFile << "ptr2->second);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << this->className << "::declareRequirements\n\n";
    // resolveDependencies
    this->srcFile << "void\n";
    this->srcFile << this->className << "::resolveRequirements(pleiades::coupling::SRequirementManager& requirementManager)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::coupling;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "map<string,map<string,unsigned short> >::const_iterator ptr;\n";
    this->srcFile << "map<string,unsigned short>::const_iterator ptr2;\n";
    this->srcFile << "for(ptr=this->outputsDepths.begin();ptr!=this->outputsDepths.end();++ptr){\n";
    this->srcFile << "const map<string,unsigned short>& tmp = ptr->second;\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile << "if(this->outputsInitialValues.find(ptr->first)==this->outputsInitialValues.end()){\n";
    this->srcFile << "requirementManager.setRequirementProvider(static_cast<IModel&>(*this),\n"
		  << "ptr->first,ptr2->first,Requirement::SCALAR,false);\n";
    this->srcFile << "} else {\n";
    this->srcFile << "if(this->outputsInitialValues[ptr->first].find(ptr2->first)!=this->outputsInitialValues[ptr->first].end()){\n";
    this->srcFile << "requirementManager.setRequirementProvider(static_cast<IModel&>(*this),\n"
		  << "ptr->first,ptr2->first,Requirement::SCALAR,true);\n";
    this->srcFile << "} else {\n";
    this->srcFile << "requirementManager.setRequirementProvider(static_cast<IModel&>(*this),\n"
		  << "ptr->first,ptr2->first,Requirement::SCALAR,false);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << this->className << "::resolveRequirements\n\n";
    // initializeOutput
    this->srcFile << "void\n";
    this->srcFile << this->className << "::initializeOutput(pleiades::coupling::Requirement& requirement)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "map<string,map<string,unsigned short> >::const_iterator ptr;\n";
    this->srcFile << "map<string,unsigned short>::const_iterator ptr2;\n";
    this->srcFile << "map<string,map<string,real> >::const_iterator ptr3;\n";
    this->srcFile << "map<string,real>::const_iterator ptr4;\n";
    this->srcFile << "ptr = this->outputsDepths.find(requirement.getName());\n";
    this->srcFile << "if(ptr==this->outputsDepths.end()){\n";
    this->srcFile << "return ModelBase::initializeOutput(requirement);\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr2 = ptr->second.find(requirement.getLocation());\n";
    this->srcFile << "if(ptr2==ptr->second.end()){\n";
    this->srcFile << "return ModelBase::initializeOutput(requirement);\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr3 = this->outputsInitialValues.find(requirement.getName());\n";
    this->srcFile << "if(ptr3==this->outputsInitialValues.end()){\n";
    this->srcFile << "string msg(\"" << this->className << "::initializeOutput: \");\n";
    this->srcFile << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr4 = ptr3->second.find(requirement.getLocation());\n";
    this->srcFile << "if(ptr4==ptr3->second.end()){\n";
    this->srcFile << "string msg(\"" << this->className << "::initializeOutput: \");\n";
    this->srcFile << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "this->smanager.create<real>(requirement.getLocation(),requirement.getName(),\n";
    this->srcFile << "ptr4->second,max(requirement.getDepth(),ptr2->second));\n";
    this->srcFile << "} // end of " << this->className << "::initializeOutput\n\n";
    // execute
    this->srcFile << "void\n"
		  << this->className
		  << "::execute(void){\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace boost;\n";
    this->srcFile << "using namespace pleiades;\n";
    this->srcFile << "using namespace pleiades::time;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    this->srcFile << "this->computeMaterialProperties();\n";
    // do we need time increment ?
    found = false;
    for(p11=this->functions.begin();(p11!=this->functions.end())&&(!found);++p11){
      if(p11->useTimeIncrement){
	this->srcFile << "const pleiades::field::real dt = convert_to_double(this->sclock.getCurrentTimeIncrement());\n";
	found = true;
      }
    }
    for(i=0,p11=this->functions.begin();p11!=this->functions.end();++p11,++i){
      string functor = "functor"+toString(i);
      if(p11->useTimeIncrement){
	this->srcFile << "this->" << functor << ".dt=dt;\n";
      }
      if(!(p11->domains.empty())){
	this->srcFile << "for(ptr=this->" << functor <<".domains.begin();ptr!=this->" << functor <<".domains.end();++ptr){\n";
      } else {
	if(domains.empty()){
	  string msg("MFrontPleiadesModelParser::writeSrcFile : ");
	  msg += "no domain specified for function '"+p11->name+"'";
	  throw(runtime_error(msg));
	}
	this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
      }
      this->srcFile << "MTFieldManager& mm = this->smanager.getMTFieldManager(*ptr);\n";
      for(p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();++p15){
	pair<string,unsigned short> dv = this->decomposeVariableName(*p15);
	this->srcFile << "const FieldHolder& " << "f_" << *p15 
		      << " = mm.getFieldHolder(" << this->getPleiadesVariableName(dv.first)
		      << "," << dv.second << ");\n";
      }
      for(p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15){
	this->srcFile << "FieldHolder& " << "f_" << *p15 
		      << " = mm.getFieldHolder(" << this->getPleiadesVariableName(*p15) << ");\n";
      }
      for(p15=p11->constantMaterialProperties.begin();
	  p15!=p11->constantMaterialProperties.end();++p15){
	this->srcFile << "this->" << functor << "." << *p15 
		      << " = this->constantMaterialProperties[*ptr][\"" << *p15 << "\"];\n";
      }
      this->srcFile << "if(getVerboseMode()==VERBOSE_LEVEL1){\n";
      this->srcFile << "ostream& log = getLogStream();\n";
      this->srcFile << "log << \"**" << this->className << "::execute : \"" << "\n";
      this->srcFile << "<< \"executing function '" << p11->name << "' on domain '\" << *ptr << \"'\" << endl;\n";
      this->srcFile << "}\n";
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	string output = *(p11->modifiedVariables.begin());
	this->srcFile << "apply(this->" << functor << ",f_" << output;
	if(p11->usedVariables.size()!=0){
	  this->srcFile << ",\n";
	  for(p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();){
	    this->srcFile << "f_" << *p15;
	    if(++p15!=p11->usedVariables.end()){
	      this->srcFile << ",";
	    }
	  }
	}
	this->srcFile << ");\n";
      } else {
	if(p11->usedVariables.size()!=0){
	  this->srcFile << "vector<real> results;\n";
	  this->srcFile << "vector<real> values;\n";
	  this->srcFile << "results.resize(" << p11->modifiedVariables.size() << ");\n";
	  this->srcFile << "values.resize("  << p11->usedVariables.size()     << ");\n";
	  this->srcFile << "if(";
	  for(p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();){
	    this->srcFile << "(f_" << *p15 << ".is<Field<real> >())";
	    if(++p15!=p11->usedVariables.end()){
	      this->srcFile << "||\n";
	    }
	  }
	  this->srcFile << "){\n";
	  // a least one input is a field
	  this->srcFile << "const MEDMEM::SUPPORT& support = mm.getSupport();\n";
	  this->srcFile << "map<unsigned short,const Field<real> *> fields;\n";
	  this->srcFile << "Field<real>::size_type ptr2;\n";
	  this->srcFile << "map<unsigned short,const Field<real> *>::const_iterator ptr3;\n";
	  // converting the output
	  for(p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15){
	    this->srcFile << "if(!f_" << *p15<< ".is<Field<real> >()){\n";
	    this->srcFile << "f_" << *p15 << " = FieldHolder(shared_ptr<Field<real> >(new Field<real>(support,f_"
			  << *p15 << ".get<real>())));\n";
	    this->srcFile << "}\n";
	  }
	  // getting output field
	  for(p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15){
	    this->srcFile << "Field<real>& ff_" << *p15 << " = f_" << *p15 << ".get<Field<real> >();\n";  
	  }
	  for(i=0,p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();++p15,++i){
	    this->srcFile << "if(f_" << *p15<< ".is<Field<real> >()){\n";
	    this->srcFile << "fields[" << i << "] = &(f_" << *p15<< ".get<Field<real> >());\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "values[" << i << "] = f_" << *p15<< ".get<real>();\n";
	    this->srcFile << "}\n";
	  }
	  this->srcFile << "for(ptr2=0;ptr2!=ff_" << *(p11->modifiedVariables.begin()) << ".size();++ptr2){\n";
	  this->srcFile << "for(ptr3=fields.begin();ptr3!=fields.end();++ptr3){\n";
	  this->srcFile << "values[ptr3->first]=ptr3->second->operator()(ptr2);\n";
	  this->srcFile << "}\n";
	  for(i=0,p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15,++i){
	    this->srcFile << "results[" << i << "] = ff_" << *p15 << "[ptr2];\n";
	  }
	  this->srcFile << "this->" << functor << "(results,values);\n";
	  for(i=0,p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15,++i){
	    this->srcFile << "ff_" << *p15 << "[ptr2] = " << "results[" << i << "];\n";  
	  }
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  // all fields are uniform
	  for(i=0,p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();++p15,++i){
	    this->srcFile << "values[" << i << "] = f_" << *p15<< ".get<real>();\n";
	  }
	  for(i=0,p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15,++i){
	    this->srcFile << "results[" << i << "] = f_" << *p15<< ".get<real>();\n";
	  }
	  this->srcFile << "this->" << functor << "(results,values);\n";
	  for(i=0,p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15,++i){
	    this->srcFile << "f_" << *p15 << " = FieldHolder(UniformField<real>(results[" << i <<"]));\n";
	  }
	  this->srcFile << "}\n";
	} else {
	  // no input variables
	  string msg("MFrontPleiadesModelParser : untreated case");
	  throw(runtime_error(msg));
	}
      }
    }
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << this->className << "::execute\n\n";
    this->srcFile << "void\n" << this->className 
		  << "::executeInitialPostProcessingTasks(const bool)\n{} // end of " << this->className 
		  << "::executeInitialPostProcessingTasks\n\n";
    
    this->srcFile << "void\n" << this->className 
		  << "::executePostProcessingTasks(const bool)\n{} // end of " << this->className 
		  << "::executePostProcessingTasks\n\n";
    
    this->srcFile << this->className << "::~" << this->className << "()\n{}\n\n";
    
    if(this->hasSpecializedConstructor()){
      this->srcFile << "PLEIADES_GENERATE_PROXY2(IModel," << this->className << ")\n\n";
    } else {
      this->srcFile << "PLEIADES_GENERATE_PROXY(IModel," << this->className << ")\n\n";
    }
    this->srcFile << "} // end of namespace model\n\n";
    this->srcFile << "} // end of namespace pleiades\n";
  } // end of MFrontPleiadesModelParser::writeSrcFile(void)
    
  void
  MFrontPleiadesModelParser::writeInitializeOutputsVariablesDepths(void)
  {
    using namespace std;
    FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,unsigned short>::const_iterator p3;
    map<string,double>::const_iterator p4;
    unsigned short i;
    this->srcFile << "void\n" << this->className << "::initializeOutputsVariablesDepths(void)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=this->functions.begin();p!=this->functions.end();++p,++i){
      for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();++p2){
	string functor = "functor"+toString(i);
	p3 = p->depth.find(*p2);
	if(p3==p->depth.end()){
	  string msg("MFrontPleiadesModelParser::writeInitializeOutputsVariablesDepths : ");
	  msg += "internal error, no depth found for variable '"+*p2+"' in function '"+p->name+"'";
	  throw(runtime_error(msg));
	}
	if(!p->domains.empty()){
	  this->srcFile << "for(ptr=this->" << functor << ".domains.begin();"
			<< "ptr!=this->" << functor << ".domains.end();++ptr){\n";
	} else {
	  this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	}
	this->srcFile << "if(!this->outputsDepths[" << this->getPleiadesVariableName(*p2)
		      << "].insert(make_pair(*ptr,";
	this->srcFile << p3->second;
	this->srcFile << ")).second){\n";
	this->srcFile << "string msg(\"" << this->className << "::" << this->className << " : \");\n";
	this->srcFile << "msg += \"output '\";\n";
	this->srcFile << "msg += " << this->getPleiadesVariableName(*p2) << ";\n";
	this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << this->className << "::initializeOutputsVariablesDepths\n\n";
  } // end of MFrontPleiadesModelParser::writeInitializeOutputsVariablesDepths()

  void
  MFrontPleiadesModelParser::writeInitializeConstantMaterialProperties(void)
  {
    using namespace std;
    FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,string>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << this->className
		  << "::initializeConstantMaterialProperties(" 
		  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "typedef ConstantMaterialPropertyDescription CMPD;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=this->functions.begin();p!=this->functions.end();++p,++i){
      for(p2=p->constantMaterialProperties.begin();p2!=p->constantMaterialProperties.end();++p2){
	string functor = "functor"+toString(i);
	if(!p->domains.empty()){
	  this->srcFile << "for(ptr=this->" << functor << ".domains.begin();"
			<< "ptr!=this->" << functor << ".domains.end();++ptr){\n";
	} else {
	  this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	}
	// getting material description
	this->srcFile << "if(!data.hasMaterialDescription(*ptr)){\n";
	this->srcFile << "string msg(\"" << this->className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"no material description  on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const MaterialDescription& md = *(data.getMaterialDescription(*ptr));\n";
	this->srcFile << "if(md.containsMaterialProperty(" << this->getPleiadesVariableName(*p2) << ")){\n";
	this->srcFile << "if(!md.isMaterialPropertyAccesible(" << this->getPleiadesVariableName(*p2) << ")){\n";
	this->srcFile << "string msg(\"" << this->className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"material property '\";\n";
	this->srcFile << "msg += " <<  this->getPleiadesVariableName(*p2) << ";\n";
	this->srcFile << "msg += \"' is not accessible on material '\"+*ptr+\"'\";\n";
	this->srcFile << "msg += \"' (this means that this property is define within a behaviour)\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const MaterialPropertyDescription& mpd = *(md.getMaterialProperty(" 
		      << this->getPleiadesVariableName(*p2) << "));\n";
	this->srcFile << "if(!mpd.is<CMPD>()){\n";
	this->srcFile << "string msg(\"" << this->className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"material property '\";\n";
	this->srcFile << "msg += " <<  this->getPleiadesVariableName(*p2) << ";\n";
	this->srcFile << "msg += \"' is not constant on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->constantMaterialProperties[*ptr][\"" << *p2 << "\"]" 
		      << " = mpd.get<CMPD>().getValue();\n";
	this->srcFile << "} else {\n";
	if((p3=this->defaultValues.find(*p2))!=this->defaultValues.end()){
	  this->srcFile << "this->constantMaterialProperties[*ptr][\"" << *p2 
			<< "\"] = " << p3->second << ";\n";	  
	} else {
	  this->srcFile << "string msg(\"" << this->className << "::initializeConstantMaterialProperties : \");\n";
	  this->srcFile << "msg += \"material property '\";\n";
	  this->srcFile << "msg += " <<  this->getPleiadesVariableName(*p2) << ";\n";
	  this->srcFile << "msg += \"' is undefined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	}
	this->srcFile << "}\n";
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << this->className << "::initializeConstantMaterialProperties\n\n";
  } // end of MFrontPleiadesModelParser::writeInitializeConstantMaterialProperties(void)

  void
  MFrontPleiadesModelParser::writeInitializeOutputsVariablesInitialValues(void)
  {
    using namespace std;
    FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << this->className
		  << "::initializeOutputsVariablesInitialValues(" 
		  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=this->functions.begin();p!=this->functions.end();++p,++i){
      for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();++p2){
	string functor = "functor"+toString(i);
	p3=this->initialValues.find(*p2);
	if(!p->domains.empty()){
	  this->srcFile << "for(ptr=this->" << functor << ".domains.begin();"
			<< "ptr!=this->" << functor << ".domains.end();++ptr){\n";
	} else {
	  this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	}
	// getting material description
	if(p3!=this->initialValues.end()){
	  this->srcFile << "if(data.hasMaterialDescription(*ptr)){\n";
	  this->srcFile << "const MaterialDescription& md = *(data.getMaterialDescription(*ptr));\n";
	  this->srcFile << "if(md.containsStateVariable(" <<  this->getPleiadesVariableName(*p2) << ")){\n";
	  this->srcFile << "real tmp = md.getStateVariable("<<  this->getPleiadesVariableName(*p2) << ")->getInitialValue();\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getPleiadesVariableName(*p2)
			<< "].insert(make_pair(*ptr,tmp)).second){\n";
	  this->srcFile << "string msg(\"" << this->className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getPleiadesVariableName(*p2) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getPleiadesVariableName(*p2)
			<< "].insert(make_pair(*ptr,";
	  this->srcFile << p3->second;
	  this->srcFile << ")).second){\n";
	  this->srcFile << "string msg(\"" << this->className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getPleiadesVariableName(*p2) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getPleiadesVariableName(*p2)
			<< "].insert(make_pair(*ptr,";
	  this->srcFile << p3->second;
	  this->srcFile << ")).second){\n";
	  this->srcFile << "string msg(\"" << this->className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getPleiadesVariableName(*p2) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	} else {
	  this->srcFile << "if(!data.hasMaterialDescription(*ptr)){\n";
	  this->srcFile << "string msg(\"" << this->className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"no material description  on material '\"+*ptr+\"', \";\n";
	  this->srcFile << "msg += \"required to initialize output value '"+this->getPleiadesVariableName(*p2)+"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "const MaterialDescription& md = *(data.getMaterialDescription(*ptr));\n";
	  this->srcFile << "if(md.containsStateVariable(" <<  this->getPleiadesVariableName(*p2) << ")){\n";
	  this->srcFile << "real tmp = md.getStateVariable("<<  this->getPleiadesVariableName(*p2) << ")->getInitialValue();\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getPleiadesVariableName(*p2)
			<< "].insert(make_pair(*ptr,tmp)).second){\n";
	  this->srcFile << "string msg(\"" << this->className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\"\n;";
	  this->srcFile << "msg += " << this->getPleiadesVariableName(*p2) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	}
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << this->className << "::initializeOutputsVariablesInitialValues\n\n";
  } // end of MFrontPleiadesModelParser::writeInitializeOutputsVariablesInitialValues()

  void
  MFrontPleiadesModelParser::writeInitializeInputsVariablesDepths(void)
  {
    using namespace std;
    set<string> treatedVars;
    FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,unsigned short>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << this->className << "::initializeInputsVariablesDepths(void)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=this->functions.begin();p!=this->functions.end();++p,++i){
      for(p2=p->usedVariables.begin();p2!=p->usedVariables.end();++p2){
	if(this->isInputVariable(*p2)){
	  const string& v = this->decomposeVariableName(*p2).first;
	  if(treatedVars.find(v)==treatedVars.end()){
	    string functor = "functor"+toString(i);
	    p3 = p->depth.find(v);
	    if(p3==p->depth.end()){
	      string msg("MFrontPleiadesModelParser::writeInitializeInputsVariablesDepths : ");
	      msg += "internal error, no depth found for variable '"+v+"' in function '"+p->name+"'";
	      throw(runtime_error(msg));
	    }
	    if(!p->domains.empty()){
	      this->srcFile << "for(ptr=this->" << functor << ".domains.begin();"
			    << "ptr!=this->" << functor << ".domains.end();++ptr){\n";
	    } else {
	      this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	    }
	    this->srcFile << "map<string,unsigned short>& tmp = this->inputsDepths[" << this->getPleiadesVariableName(v) << "];\n";
	    if(p3->second==0){
	      this->srcFile << "if(tmp.find(*ptr)==tmp.end()){\n";
	      this->srcFile << "tmp[*ptr]=0;\n";
	      this->srcFile << "}\n";
	    } else {
	      this->srcFile << "if(tmp.find(*ptr)!=tmp.end()){\n";
	      this->srcFile << "if(tmp[*ptr]<" << p3->second << "){\n";
	      this->srcFile << "tmp[*ptr]=" << p3->second << ";\n";
	      this->srcFile << "}\n";
	      this->srcFile << "} else {\n";
	      this->srcFile << "tmp[*ptr]=" << p3->second << ";\n";
	      this->srcFile << "}\n";
	    }
	    this->srcFile << "}\n";
	    treatedVars.insert(v);
	  }
	}
      }
    }
    this->srcFile << "} // end of " << this->className << "::initializeInputsVariablesDepths\n\n";
  } // end of MFrontPleiadesModelParser::writeInitializeInputsVariablesDepths()

  std::string
  MFrontPleiadesModelParser::getPleiadesVariableName(const std::string& v) const
  {
    using namespace std;
    map<string,string>::const_iterator p;
    if((p=this->glossaryNames.find(v))!=this->glossaryNames.end()){
      return "Glossary::" + p->second;
    } else if((p=this->entryNames.find(v))!=this->entryNames.end()){
      return "\""+p->second+"\"";
    }
    return "\""+v+"\"";
  } // end of MFrontPleiadesModelParser::getPleiadesVariableName(const std::string& v)

  std::pair<std::string,unsigned short>
  MFrontPleiadesModelParser::decomposeVariableName(const std::string& v) const
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
      p2 = this->depth.find(p->name);
      if(p2!=this->depth.end()){
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
      p2 = this->depth.find(p->name);
      if(p2!=this->depth.end()){
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
    string msg("MFrontPleiadesModelParser::decomposeVariableName : ");
    msg += "no decomposition found  for variable '"+v+"'";
    throw(runtime_error(msg));
    return pair<string,unsigned short>("",0u);
  } // end of MFrontPleiadesModelParser::getPleiadesVariableName(const std::string& v)


  void
  MFrontPleiadesModelParser::writeStaticVariableInitialization(const std::string& cname,
							       const StaticVarHandler& v)
  {
    using namespace std;
    if(this->debugMode){
      this->srcFile << "#line " << v.lineNumber << " \"" 
		    << this->fileName << "\"\n";
    }
    if(v.type=="short"){
      this->srcFile << "const short " << cname << "::" << v.name 
		    << " = " << static_cast<short>(v.value) << ";" << endl;
    } else if(v.type=="ushort"){
      this->srcFile << "const unsigned short " << cname << "::" << v.name 
		    << " = " << static_cast<unsigned short>(v.value) << ";" << endl;
    } else if(v.type=="int"){
      this->srcFile << "const int " << cname << "::" << v.name 
		    << " = " << static_cast<int>(v.value) << ";" << endl;
    } else if(v.type=="uint"){
      this->srcFile << "const unsigned int " << cname << "::" << v.name 
		    << " = " << static_cast<unsigned int>(v.value) << ";" << endl;
    } else if(v.type=="long"){
      this->srcFile << "const long " << cname << "::" << v.name 
		    << " = " << static_cast<long>(v.value) << ";" << endl;
    } else if(v.type=="ulong"){
      this->srcFile << "const unsigned long " << cname << "::" << v.name 
		    << " = " << static_cast<unsigned long>(v.value) << ";" << endl;
    } else if(v.type=="float"){
      this->srcFile << "const float " << cname << "::" << v.name 
		    << " = " << static_cast<float>(v.value) << ";" << endl;
    } else if(v.type=="double"){
      this->srcFile << "const double " << cname << "::" << v.name 
		    << " = " << static_cast<double>(v.value) << ";" << endl;
    } else if(v.type=="ldouble"){
      this->srcFile << "const long double " << cname << "::" << v.name 
		    << " = " << static_cast<long double>(v.value) << ";" << endl;
    } else if(v.type=="real"){
      this->srcFile << "const pleiades::field::real " << cname << "::" << v.name 
		    << " = " << static_cast<double>(v.value) << ";" << endl;
    } else {
      string msg("MFrontPleiadesModelParser::writeStaticVariableInitialisation : ");
      msg += "type " + v.type + " is not a supported type for a static variable.";
      msg += "Supported types are short, ushort, int, uint, long, ulong,";
      msg += "float, double, ldouble and real.\n";
      msg += "Error at line " + toString(v.lineNumber);
      throw(runtime_error(msg));
    }
  } // end of MFrontPleiadesModelParser::writeStaticVariableInitialisation
    
  void
  MFrontPleiadesModelParser::writeOutputFiles(void)
  {
    using namespace std;
    if(this->className.empty()){
      string msg("MFrontPleiadesModelParser::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->headerFileName  = "include/Pleiades/Model/"+this->className;
    this->headerFileName += "-pleiades.hxx";
    this->srcFileName  = "src/"+this->className;
    this->srcFileName += "-pleiades.cxx";
    this->openOutputFiles();
    this->generateOutputFiles();
    this->closeOutputFiles();
  } // end of class MFrontPleiadesModelParser::writeOutputFiles()

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParser::getGeneratedSources(void)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib;
    if(!this->material.empty()){
      lib = "libPleiades"+this->material+"Models";
    } else {
      lib = "libPleiadesMaterialModels";
    }
    src[lib].push_back(this->className+"-pleiades.cxx");
    return src;
  } // end of MFrontPleiadesModelParser::getGeneratedSources
  
  std::vector<std::string>
  MFrontPleiadesModelParser::getGeneratedIncludes(void)
  {
    using namespace std;
    vector<string> inc;
    inc.push_back("Pleiades/Model/"+this->className+"-pleiades.hxx");
    return inc;
  } // end of MFrontPleiadesModelParser::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParser::getGlobalIncludes(void)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib;
    if(!this->material.empty()){
      lib = "libPleiades"+this->material+"Models";
    } else {
      lib = "libPleiadesMaterialModels";
    }
    incs[lib].push_back("`pleiades-config --includes`\n");
    return incs;
  } // end of MFrontPleiadesModelParser::getGlobalIncludes
  
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParser::getGlobalDependencies(void)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string lib;
    if(!this->material.empty()){
      lib = "libPleiades"+this->material+"Models";
    } else {
      lib = "libPleiadesMaterialModels";
    }
    libs[lib].push_back("`pleiades-config --libs` -lm");
    return libs;
  } // end of MFrontPleiadesModelParser::getGlobalDependencies
    
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelParser::getLibrariesDependencies(void)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontPleiadesModelParser::getLibrariesDependencies

  void
  MFrontPleiadesModelParser::treatExecute(void)
  {
    this->execute = this->readNextBlock(true);
  }

  void
  MFrontPleiadesModelParser::treatOutput(void)
  {
    using namespace std;
    if(!this->functions.empty()){
      string msg("MFrontPleiadesModelParser::treatInput : ");
      msg += "outputs must be declared before declaring functions";
      throw(runtime_error(msg));
    }
    VarContainer::const_iterator p;
    this->readVarList("PleiadesField",this->outputs,false);
    for(p=this->outputs.begin();p!=this->outputs.end();++p){
      this->fieldNames.insert(p->name);
    }
  } // end of MFrontPleiadesModelParser::treatOutput(void)

  void
  MFrontPleiadesModelParser::treatInput(void)
  {
    using namespace std;
    if(!this->functions.empty()){
      string msg("MFrontPleiadesModelParser::treatInput : ");
      msg += "inputs must be declared before declaring functions";
      throw(runtime_error(msg));
    }
    VarContainer::const_iterator p;
    this->readVarList("PleiadesField",this->inputs,false);
    for(p=this->inputs.begin();p!=this->inputs.end();++p){
      this->fieldNames.insert(p->name);
    }
  } // end of MFrontPleiadesModelParser::treatInput(void)

  void
  MFrontPleiadesModelParser::treatGlobalParameter(void)
  {
    this->readVarList(this->globalParameters,false);
  } // end of MFrontPleiadesModelParser::treatGlobalParameter(void)

  void
  MFrontPleiadesModelParser::treatLocalParameter(void)
  {
    this->readVarList(this->localParameters,false);
  } // end of MFrontPleiadesModelParser::treatLocalParameter(void)

  void
  MFrontPleiadesModelParser::treatConstantMaterialProperty(void)
  {
    this->readVarList("real",this->constantMaterialProperties,false);
  } // end of MFrontPleiadesModelParser::treatConstantMaterialProperty(void)
  
  void
  MFrontPleiadesModelParser::writeAssignDefaultValue(const VarContainer::const_iterator p,
						     const std::map<std::string,std::string>::const_iterator p4)
  {
    using namespace std;
    if((p->type=="DoubleArray")||(p->type=="StringArray")){
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
    } else if((p->type=="string")||(p->type=="double")||(p->type=="real")){
      this->srcFile << "this->" << p->name << " = "  << p4->second << ";" << endl;
    } else {
      string msg("MFrontPleiadesModelParser::writeAssignDefaultValue : ");
      msg+="type '"+p->type+"' is not supported.\n";
      throw(runtime_error(msg));
    }   
  } // end of MFrontPleiadesModelParser::writeAssignDefaultValue

  void
  MFrontPleiadesModelParser::readDefaultValue(void)
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
      this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
			      "variable '"+this->currentVar+"' is neither a "
			      "constant material properties, "
			      "a local parameter or a global parameter");
    }
    this->checkNotEndOfFile("MFrontPleiadesModelParser::readDefaultValue",
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
	    this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
				    "Could not read default for variable '"+this->currentVar+"'");
	  }
	} else {
	  if(this->current->flag!=Token::String){
	    this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
				    "Expected to read a string (read '"+this->current->value+"').");
	  }
	}
	++nbr;
	res+=" "+this->current->value;
	++(this->current);
	this->checkNotEndOfFile("MFrontPleiadesModelParser::readDefaultValue",
				"Expected ',' or ')'.");
	if(this->current->value==")"){
	  bend = true;
	  --(this->current);
	} else if(this->current->value==","){
	  ++(this->current);
	  this->checkNotEndOfFile("MFrontPleiadesModelParser::readDefaultValue",
				  "Expected default value.");
	} else {
	  this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
				  "Unexpected token (expected ',' or ')', read '"+this->current->value+"').");
	}
      }
      res = toString(nbr)+res;
    } else if ((p->type=="double")||(p->type=="real")){
      istringstream converter(this->current->value);
      double tmp;
      converter >> tmp;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
				"Could not read default for variable '"+this->currentVar+"'");
      }
      res = this->current->value;
    } else if (p->type=="string"){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      res = this->current->value;
    } else {
      this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
			      "type '"+p->type+"' is not supported.");
    }
    if(!this->defaultValues.insert(MVType(this->currentVar,res)).second){
      this->throwRuntimeError("MFrontPleiadesModelParser::readDefaultValue",
			      "default value for field '"+ this->currentVar +"' already defined.");
    }
  } // end of MFrontPleiadesModelParser::writeReadDefaultValue

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontPleiadesModelParser::getSpecificTargets(void)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontPleiadesModelParser::getSpecificTargets(void)

} // end of namespace mfront  
