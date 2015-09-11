/*! 
 * \file  mfront/src/ModelDSLCommon.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jun 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/ModelDSLCommon.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/ModelInterfaceFactory.hxx"

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

  ModelDSLCommon::ModelDSLCommon(){
    for(const auto& v : DSLBase::getDefaultReservedNames()){
      this->reserveName(v);
    }
  }
  
  void
  ModelDSLCommon::addLibraryDependency(const std::string& l){
    if(std::find(this->librariesDependencies.begin(),
		 this->librariesDependencies.end(),l)==this->librariesDependencies.end()){
      this->librariesDependencies.push_back(l);
    }
  } // end of ModelDSLCommon::addLibraryDependency
  
  void
  ModelDSLCommon::reserveName(const std::string& n){
    if(!this->reservedNames.insert(n).second){
      throw(std::runtime_error("ModelDSLCommon::reserveName: "
			       "name '"+n+"' already reserved"));
    }
  }
  
  void
  ModelDSLCommon::registerMemberName(const std::string& n)
  {
    this->reserveName(n);
    if(!this->memberNames.insert(n).second){
      throw(std::runtime_error("ModelDSLCommon::registerMemberName: "
			       "name '"+n+"' already reserved"));
    }
  } // end of ModelDSLCommon::registerMemberName

  void
  ModelDSLCommon::registerStaticMemberName(const std::string& n)
  {
    this->reserveName(n);
    if(!this->staticMemberNames.insert(n).second){
      throw(std::runtime_error("ModelDSLCommon::registerStaticMemberName: "
			       "name '"+n+"' already reserved"));
    }
  } // end of ModelDSLCommon::registerStaticMemberName
  
  AbstractDSL::DSLTarget
  ModelDSLCommon::getTargetType(void) const{
    return MODELDSL;
  }

  bool
  ModelDSLCommon::is(const ModelData& data,
			      const VariableDescriptionContainer& vc,
			      const std::string& v)
  {
    using namespace std;
    unsigned int d,i;
    for(const auto & elem : vc){
      if(elem.name==v){
	return true;
      }
      auto p2 = data.depths.find(elem.name);
      if(p2==data.depths.end()){
	d = 0;
      } else {
	d = p2->second;
      }
      for(i=1;i!=d+1;++i){
	if(elem.name+'_'+to_string(i)==v){
	  return true;
	}
      }
    }
    return false;
  } // end of ModelDSLCommon::is(void)

  std::string
  ModelDSLCommon::getClassName(void) const
  {
    return this->className;
  } // end of ModelDSLCommon::getClassName

  void
  ModelDSLCommon::addStaticVariableDescription(const StaticVariableDescription& v)
  {
    this->staticVars.push_back(v);
  } // end of ModelDSLCommon::addStaticVariableDescription

  void
  ModelDSLCommon::treatMaterial(void)
  {
    using namespace std;
    if(!ModelData::material.empty()){
      string msg("ModelDSLCommon::treatMaterial : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->material       = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(ModelData::material,true)){
      string msg("ModelDSLCommon::treatMaterial : ");
      msg += "invalid material name '"+ModelData::material+"'";
      throw(runtime_error(msg));
    }
    if(!this->className.empty()){
      this->className = ModelData::material+"_"+this->className;
    }
  } // end of ModelDSLCommon::treatMaterial

  void
  ModelDSLCommon::treatLibrary(void)
  {
    using namespace std;
    const auto& l = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(l,true)){
      string msg("ModelDSLCommon::treatLibrary : ");
      msg += "invalid library name '"+l+"'";
      throw(runtime_error(msg));
    }
    if(!this->library.empty()){
      string msg("ModelDSLCommon::treatLibrary : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->library = l;
  } // end of ModelDSLCommon::treatLibrary

  void ModelDSLCommon::treatModel(void)
  {
    using namespace std;
    if(!this->className.empty()){
      this->throwRuntimeError("ModelDSLCommon::treatModel",
			      "model name already defined");
    }
    this->className = this->readOnlyOneToken();
    if(!isValidIdentifier(this->className)){
      this->throwRuntimeError("ModelDSLCommon::treatModel",
			      "invalid model name");
    }
    if(!ModelData::material.empty()){
      this->className = ModelData::material+"_"+this->className;
    }
  } // end of ModelDSLCommon::treatModel

  void
  ModelDSLCommon::setInterfaces(const std::set<std::string>& inames)
  {
    using namespace std;
    auto& mbif = ModelInterfaceFactory::getModelInterfaceFactory();
    for(const auto& i : inames){
      if(this->interfaces.count(i)==0){
	this->interfaces.insert({i,mbif.getInterface(i)});
      }
    }
  } // end of ModelDSLCommon::setInterfaces

  void
  ModelDSLCommon::generateOutputFiles(void)
  {
    using namespace std;
    if(this->interfaces.empty()){
      string msg("ModelDSLCommon::generateOutputFile : ");
      msg += "no interface defined";
      throw(runtime_error(msg));
    }
    for(const auto& i : this->interfaces){
      i.second->writeOutputFiles(*this,*this);
    }
  } // end of ModelDSLCommon::writeOutputFiles

  void
  ModelDSLCommon::treatUnknownKeyword(void)
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
    this->checkNotEndOfFile("ModelDSLCommon::treatUnknownKeyword");
    if(this->current->value=="["){
      set<string> s;
      while(this->current->value!="]"){
	++(this->current);
	this->checkNotEndOfFile("ModelDSLCommon::treatUnknownKeyword");
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
	  this->throwRuntimeError("ModelDSLCommon::treatUnknownKeyword",
				  "unexpected token '"+this->current->value+"' (expected ']' or ',').");
	}
      }
      ++(this->current);
      if(s.empty()){
	this->ignoreKeyWord(key);
      } else {
	for(const auto & elem : s){
	  p = interfaces.at(elem)->treatKeyword(key,this->current,
						this->fileTokens.end());
	  if(!p.first){
	    string msg("ModelDSLCommon::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += " has not been treated by interface '"+elem+"'";
	    throw(runtime_error(msg));
	  }
	  if(treated){
	    if(p2!=p.second){
	      string msg("ModelDSLCommon::treatUnknownKeyword : the keyword '");
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
	      string msg("ModelDSLCommon::treatUnknownKeyword : the keyword '");
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

  } // end of ModelDSLCommon::treatUnknownKeyword

  void
  ModelDSLCommon::treatDomain(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(!this->domains.empty()){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      }
      this->throwRuntimeError("ModelDSLCommon::treatDomains",msg);
    }
    this->checkNotEndOfFile("ModelDSLCommon::treatDomain");
    if(this->current->flag!=Token::String){
      this->throwRuntimeError("ModelDSLCommon::treatDomain",
			      "Expected to read a string (read '"+this->current->value+"').");
    }
    if(this->current->value.size()<2){
      this->throwRuntimeError("ModelDSLCommon::treatDomain : ",
			      "domain name too short.");
    }
    this->domains.insert(this->current->value.substr(1,this->current->value.size()-2));
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatDomain",";");
  } // end of ModelDSLCommon::treatDomain(void)

  void
  ModelDSLCommon::treatDomains(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    bool bend;
    if(!this->domains.empty()){
      string msg("domain has already been defined ");
      if(!this->domains.empty()){
	msg+="with @Domain or @Domains";
      }
      this->throwRuntimeError("ModelDSLCommon::treatDomains",msg);
    }
    this->checkNotEndOfFile("ModelDSLCommon::treatDomains");
    this->readSpecifiedToken("ModelDSLCommon::treatDomains","{");
    this->checkNotEndOfFile("ModelDSLCommon::treatDomains");
    bend = false;
    while((this->current!=this->fileTokens.end())&&(!bend)){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatDomains",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatDomains",
				"Domain name too short.");
      }
      if(!this->domains.insert(this->current->value.substr(1,this->current->value.size()-2)).second){
	this->throwRuntimeError("ModelDSLCommon::treatDomains",
				"domain "+this->current->value.substr(1,this->current->value.size()-2)+
				" already defined.");
      }
      ++(this->current);	
      this->checkNotEndOfFile("ModelDSLCommon::treatDomains",
			      "Expected ',' or ')'.");
      if(this->current->value==","){
	++this->current;
      } else if(this->current->value=="}"){
	bend = true;
      } else {
	this->throwRuntimeError("ModelDSLCommon::treatDomains",
				"Expected ',' or ')' (read '"+this->current->value+"').");
      }
    }
    this->readSpecifiedToken("ModelDSLCommon::treatDomains","}");
    if(this->domains.empty()){
      this->throwRuntimeError("ModelDSLCommon::treatDomains",
			      "@Domains does not set any domain.");
    }
    this->checkNotEndOfFile("ModelDSLCommon::treatDomains");
    ++(this->current);
  } // end of ModelDSLCommon::treatDomains(void)

  std::pair<std::string,unsigned short>
  ModelDSLCommon::decomposeVariableName(const std::string& v) const
  {
    using namespace std;
    unsigned int d,i;
    for(const auto & elem : this->inputs){
      if(v==elem.name){
	return pair<string,unsigned short>(v,0u);
      }
      auto p2 = this->depths.find(elem.name);
      if(p2!=this->depths.end()){
	d = p2->second;
      } else {
	d = 0;
      }
      for(i=1;i!=d+1;++i){
	if(v==elem.name+"_"+to_string(i)){
	  return pair<string,unsigned short>(elem.name,i);
	}
      }
    }
    for(const auto & elem : this->outputs){
      if(v==elem.name){
	return pair<string,unsigned short>(v,0);
      }
      auto p2 = this->depths.find(elem.name);
      if(p2!=this->depths.end()){
	d = p2->second;
      } else {
	d = 0;
      }
      for(i=1;i!=d+1;++i){
	if(v==elem.name+"_"+to_string(i)){
	  return pair<string,unsigned short>(elem.name,i);
	}
      }
    }
    throw(std::runtime_error("ModelDSLCommon::decomposeVariableName : "
			     "no decomposition found  for variable '"+v+"'"));
  } // end of ModelDSLCommon::getPleiadesVariableName(const std::string& v)

  bool
  ModelDSLCommon::isInputVariable(const std::string& v) const
  {
    return ModelDSLCommon::is(*this,this->inputs,v);
  } // end of ModelDSLCommon::isInputVariable(void)

  bool
  ModelDSLCommon::isOutputVariable(const std::string& v) const
  {
    return ModelDSLCommon::is(*this,this->outputs,v);
  } // end of ModelDSLCommon::isInputVariable(void)
  
  void
  ModelDSLCommon::treatFunction(void)
  {
    using namespace std;
    auto isStaticMemberName = [](const ModelData& d,
				 const std::string& n){
      for(const auto& v : d.staticMemberNames){
	if(v==n){
	  return true;
	}
      }
      return false;
    };
    auto isMemberName = [](const ModelData& d,
			   const std::string& n){
      for(const auto& v : d.memberNames){
	if(v==n){
	  return true;
	}
      }
      return false;
    };
    Function f;
    unsigned int openedBrackets = 0;
    unsigned int openedParenthesis = 0;
    f.useTimeIncrement = false;
    this->registerMemberName("functor"+to_string(this->functions.size()));
    this->checkNotEndOfFile("ModelDSLCommon::treatFunction");
    f.name = this->current->value;
    if(!this->isValidIdentifier(f.name)){
      string msg("ModelDSLCommon::treatFunction : ");
      msg += "function name '"+f.name+"' is not valid";
      throw(runtime_error(msg));
    }
    this->registerMemberName(f.name);
    this->reserveName(f.name+".Domain");
    this->reserveName(f.name+".Domains");
    f.line = this->current->line;
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatFunction","{");
    ++openedBrackets;
    this->checkNotEndOfFile("ModelDSLCommon::treatFunction",
			    "Expected body of function '"+f.name+"'.");
    auto currentLine    = this->current->line;
    auto newInstruction = true;
    auto newLine        = true;
    if(getDebugMode()){
      f.body  +="#line " + to_string(currentLine) + " \"" + this->fileName + "\"\n";
    }
    for(;(this->current!=this->fileTokens.end())&&
	  (openedBrackets!=0);++(this->current)){
      if(this->current->line!=currentLine){
	currentLine=this->current->line;
	f.body  += "\n";
	if(getDebugMode()){
	  f.body  +="#line " + to_string(currentLine) + " \"" + this->fileName + "\"\n";
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
	  this->throwRuntimeError("ModelDSLCommon::treatFunction",
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
	if(isStaticMemberName(*this,this->current->value)){
	  f.body  += "(" + this->className + ":: "+ this->current->value + ")";
	} else if(isMemberName(*this,this->current->value)){
	  bool treated = false;
	  if(newInstruction){
	    string var = this->current->value;
	    string op;
	    ++(this->current);
	    if(this->current==fileTokens.end()){
	      string msg("ModelDSLCommon::treatFunction : ");
	      msg+="unexpected end of file while reading body of function " + f.name;
	      throw(runtime_error(msg));
	    }
	    auto modifier = false;
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
	      bool found = false;
	      for(auto p=this->outputs.begin();(p!=this->outputs.end())&&(!found);){
		if(p->name==var){
		  found = true;
		} else {
		  ++p;
		}
	      }
	      if(!found){
		this->throwRuntimeError("ModelDSLCommon::treatFunction",
					"trying to modify variable '"+var+
					"' in the body of function '"+f.name+"'");
	      }
	      f.body  += var + " " + op + " ";
	      f.modifiedVariables.insert(var);
	      auto p6 = f.depths.find(var);
	      if(p6==f.depths.end()){
		f.depths[var] = 0;
	      }	      
	      treated = true;
	    } else {
	      --(this->current);
	    }
	  }
	  if(!treated){
	    // treating the case of local parameters
	    for(auto p =this->parameters.begin();
		(p!=this->parameters.end())&&(!treated);++p){
	      if(p->name==this->current->value){
		treated = true;
		f.parameters.insert(this->current->value);
		f.body  += "(this->" + this->current->value + ")";
	      }
	    }
	    // treating the case of local parameters
	    for(auto p =this->constantMaterialProperties.begin();
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
		auto dv = this->decomposeVariableName(this->current->value);
		auto p6 = f.depths.find(dv.first);
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
		auto dv = this->decomposeVariableName(this->current->value);
		auto p6 = f.depths.find(dv.first);
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
      string msg("ModelDSLCommon::treatFunction : ");
      msg+="unexpected end of file while reading body of function " + f.name;
      throw(runtime_error(msg));
    }
    if(openedBrackets!=0){
      string msg("ModelDSLCommon::treatFunction : ");
      msg+="parenthesis still opened at the end of function " + f.name;
      throw(runtime_error(msg));
    }
    if(f.modifiedVariables.size()==0){
      string msg("ModelDSLCommon::treatFunction : ");
      msg+="function " + f.name + " does not change any variable.";
      throw(runtime_error(msg));
    }
    if(!this->functionNames.insert(f.name).second){
      string msg("ModelDSLCommon::treatFunction : ");
      msg+="function " + f.name + " already declared.";
      throw(runtime_error(msg));
    }

    for(auto p2=f.modifiedVariables.begin();p2!=f.modifiedVariables.end();++p2){
      auto p3=f.usedVariables.find(*p2);
      if(p3!=f.usedVariables.end()){
	f.usedVariables.erase(*p3);
      }
    }
    this->functions.push_back(f);
  } // end of ModelDSLCommon::treatFunction(void)

  void
  ModelDSLCommon::treatOutput(void)
  {
    using namespace std;
    if(!this->functions.empty()){
      throw(std::runtime_error("ModelDSLCommon::treatInput : "
			       "outputs must be declared before "
			       "declaring functions"));
    }
    VariableDescriptionContainer noutputs;
    this->readVarList(noutputs,"Field",false);
    for(const auto& v : noutputs){
      this->registerMemberName(v.name);
      this->fieldNames.insert(v.name);
      this->outputs.push_back(v);
    }
  } // end of ModelDSLCommon::treatOutput(void)

  void
  ModelDSLCommon::treatInput(void)
  {
    if(!this->functions.empty()){
      throw(std::runtime_error("ModelDSLCommon::treatInput : "
			       "inputs must be declared before declaring functions"));
    }
    VariableDescriptionContainer ninputs;
    this->readVarList(ninputs,"Field",false);
    for(const auto& v : ninputs){
      this->registerMemberName(v.name);
      this->fieldNames.insert(v.name);
      this->inputs.push_back(v);
    }
  } // end of ModelDSLCommon::treatInput(void)

  static void
  ModelDSLCommonCheckIfNameIsAnEntryNameOrAGlossaryName(const std::map<std::string,std::string>& g,
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
  ModelDSLCommon::treatOutputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    string methodName;
    if(!this->functions.empty()){
      string msg("ModelDSLCommon::treatOutputMethod : ");
      msg += "output must be called before declaring functions";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod",".");
    this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultInitialValue")&&
       (methodName!="setDepth")){
      this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
			      "Unknown method (valid methods for fields are "
			      "setGlossaryName, setEntryName, setDepth and "
			      "setDefaultInitialValue,"
			      "read '"+methodName+"').");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod","(");
    if(methodName=="setGlossaryName"){
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Glossary name too short.");
      }
      const auto glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      ModelDSLCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
								     this->entryNames,
								     glossaryName);
      if(!glossary.contains(glossaryName)){
	throw(std::runtime_error("MaterialPropertyDSL::treatMethod : "
				 "'"+glossaryName+"' is not a valid glossary name"));
      }
      const auto& k = glossary.getGlossaryEntry(glossaryName).getKey();
      if(!this->glossaryNames.insert({this->currentVar,k}).second){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar + "'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Entry file name too short.");
      }
      const auto entryName = this->current->value.substr(1,this->current->value.size()-2);
      ModelDSLCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
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
      if(entryName!=this->currentVar){
	this->reserveName(entryName);
      }
      if(!CxxTokenizer::isValidIdentifier(entryName)){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Invalid entry name '"+entryName+"'");
      }
      if(!this->entryNames.insert({this->currentVar,entryName}).second){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Entry file name for field '"+this->currentVar+"' already defined.");
      }
    }  else if (methodName=="setDefaultInitialValue"){
      this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
			      "Expected intial value.");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Could not read initial value of field : '"+this->currentVar+"'");
      }
      if(!this->initialValues.insert({this->currentVar,value}).second){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Initial value for field '"+this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("ModelDSLCommon::treatOutputMethod",
			      "Expected depth value.");
      unsigned int value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"Could not read depth value of field '"+this->currentVar+"'");
      }
      if(!this->depths.insert({this->currentVar,value}).second){
	this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				"depth value for field '"+this->currentVar+"' already defined.");
      }
      for(unsigned int i=1;i<=value;++i){
	const auto vn = this->currentVar+"_"+to_string(i);
	this->registerMemberName(vn);
	this->registerMemberName("f_"+vn);
	this->registerMemberName("ff_"+vn);
	if(!this->fieldNames.insert(vn).second){
	  this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
				  "Field '"+vn+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    } else {
      this->throwRuntimeError("ModelDSLCommon::treatOutputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod",")");
    this->readSpecifiedToken("ModelDSLCommon::treatOutputMethod",";");
  } // end of ModelDSLCommon::treatOutputMethod

  void
  ModelDSLCommon::treatInputMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    if(!this->functions.empty()){
      string msg("ModelDSLCommon::treatInputMethod : ");
      msg += "input method must be called before declaring functions";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod",".");
    this->checkNotEndOfFile("ModelDSLCommon::treatInputMethod",
			    "Expected method name.");
    const auto methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName") &&
       (methodName!="setDepth")){
      this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
			      "Unknown method (valid methods for input fields are "
			      "setGlossaryName, setEntryName, setDepth"
			      ", read '"+methodName+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod","(");
    if(methodName=="setGlossaryName"){
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("ModelDSLCommon::treatInputMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Glossary name too short.");
      }
      const auto glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      ModelDSLCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
								     this->entryNames,
								     glossaryName);
      if(!glossary.contains(glossaryName)){
	string msg("MaterialPropertyDSL::treatMethod : "
		   "'"+glossaryName+"' is not a valid glossary name");
	throw(runtime_error(msg));
      }
      const auto& k = glossary.getGlossaryEntry(glossaryName).getKey();
      if(!this->glossaryNames.insert({this->currentVar,k}).second){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Glossary name for field '"+this->currentVar+"' already defined.");
      }
    } else if (methodName=="setEntryName"){
      const auto& glossary = Glossary::getGlossary();
      this->checkNotEndOfFile("ModelDSLCommon::treatInputMethod",
			      "Expected entry file name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"A glossary or an entry name has already been defined for field '"+
				this->currentVar+"'");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Entry file name too short.");
      }
      const auto entryName = this->current->value.substr(1,this->current->value.size()-2);
      ModelDSLCommonCheckIfNameIsAnEntryNameOrAGlossaryName(this->glossaryNames,
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
      if(entryName!=this->currentVar){
	this->reserveName(entryName);
      }
      if(!CxxTokenizer::isValidIdentifier(entryName)){
	this->throwRuntimeError("MaterialPropertyDSL::treatMethod",
				"Invalid entry name '"+entryName+"'");
      }
      if(!this->entryNames.insert({this->currentVar,entryName}).second){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDepth"){
      this->checkNotEndOfFile("ModelDSLCommon::treatInputMethod",
			      "Expected depth value.");
      unsigned int value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Could not read initial value of field '"+this->currentVar+"'");
      }
      if(!this->depths.insert({this->currentVar,value}).second){
	this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				"Initial value for field '"+this->currentVar+"' already defined.");
      }
      for(unsigned int i=1;i<=value;++i){
	const auto vn = this->currentVar+"_"+to_string(i);
	this->registerMemberName(vn);
	this->registerMemberName("f_"+vn);
	this->registerMemberName("ff_"+vn);
	if(!this->fieldNames.insert(this->currentVar+"_"+to_string(i)).second){
	  this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
				  "Field '"+vn+"' has already been declared "
				  "(internal error, this shall have been detected before).");
	}
      }
    } else {
      this->throwRuntimeError("ModelDSLCommon::treatInputMethod",
			      "Internal error (untreated method '"+ methodName +"')");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod",")");
    this->readSpecifiedToken("ModelDSLCommon::treatInputMethod",";");
  } // end of ModelDSLCommon::treatInputMethod

  void
  ModelDSLCommon::treatParameter(void)
  {
    VariableDescriptionContainer gp;
    this->readVarList(gp,false);
    for(const auto& p : gp){
      this->registerMemberName(p.name);
      this->parameters.push_back(p);
    }
  } // end of ModelDSLCommon::treatParameter(void)
  

  void
  ModelDSLCommon::treatParameterMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod",".");
    this->checkNotEndOfFile("ModelDSLCommon::treatParameterMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")&&
       (methodName!="setDefaultValue")){
      this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
			      "Unknown method (valid methods for local parameters are "
			      "setGlossaryName, setEntryName and setDefaultValue"
			      ", read "+methodName+").");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("ModelDSLCommon::treatParameterMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'.");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert({this->currentVar,glossaryName}).second){
	this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("ModelDSLCommon::treatParameterMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert({this->currentVar,entryName}).second){
	this->throwRuntimeError("ModelDSLCommon::treatParameterMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    } else if (methodName=="setDefaultValue"){
      this->readDefaultValue();
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod",")");
    this->readSpecifiedToken("ModelDSLCommon::treatParameterMethod",";");
  } // end of ModelDSLCommon::treatParameterMethod

  void
  ModelDSLCommon::treatConstantMaterialProperty(void)
  {
    VariableDescriptionContainer cmp;
    this->readVarList(cmp,"real",false);
    for(const auto& mp : cmp){
      this->registerMemberName(mp.name);
      this->constantMaterialProperties.push_back(mp);
    }
  } // end of ModelDSLCommon::treatConstantMaterialProperty(void)

  void
  ModelDSLCommon::treatConstantMaterialPropertyMethod(void) 
  {
    using namespace std;
    using namespace tfel::utilities;
    string methodName;
    this->readSpecifiedToken("ModelDSLCommon::treatConstantMaterialPropertyMethod",".");
    this->checkNotEndOfFile("ModelDSLCommon::treatConstantMaterialPropertyMethod",
			    "Expected method name.");
    methodName = this->current->value;
    if((methodName!="setGlossaryName")&&
       (methodName!="setEntryName")){
      this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
			      "Unknown method (valid methods for local parameters are "
			      "setGlossaryName and setEntryName"
			      ", read "+methodName+").");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatConstantMaterialPropertyMethod","(");
    if(methodName=="setGlossaryName"){
      this->checkNotEndOfFile("ModelDSLCommon::treatConstantMaterialPropertyMethod",
			      "Expected glossary name.");
      if((this->glossaryNames.find(this->currentVar)!=this->glossaryNames.end()) ||
	 (this->entryNames.find(this->currentVar)!=this->entryNames.end())){
	this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
				"A glossary or an entry name has already been defined for field '"
				+this->currentVar+"'.");
      }
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
				"Expected a string as glossary name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
				"Glossary name too short.");
      }
      string glossaryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->glossaryNames.insert({this->currentVar,glossaryName}).second){
	this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
				"Glossary name for field '"+ this->currentVar +"' already defined.");
      }
    } else if(methodName=="setEntryName"){
      this->checkNotEndOfFile("ModelDSLCommon::treatConstantMaterialPropertyMethod",
			      "Expected entry file name.");
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
				"Expected a string as entry file name.");
      }
      if(this->current->value.size()<3){
	this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
				"Entry file name too short.");
      }
      string entryName = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->entryNames.insert({this->currentVar,entryName}).second){
	this->throwRuntimeError("ModelDSLCommon::treatConstantMaterialPropertyMethod",
				"Entry file name for field '"+ this->currentVar +"' already defined.");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::treatConstantMaterialPropertyMethod",")");
    this->readSpecifiedToken("ModelDSLCommon::treatConstantMaterialPropertyMethod",";");
  } // end of ModelDSLCommon::treatConstantMaterialPropertyMethod

  void
  ModelDSLCommon::readDefaultValue(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    VariableDescriptionContainer::const_iterator p;
    string res;
    bool found;
    found=false;
    for(p=this->parameters.begin();(p!=this->parameters.end())&&(!found);){
      if(p->name==this->currentVar){
	found=true;
      } else {
	++p;
      }
    }    
    if(!found){
      this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
			      "variable '"+this->currentVar+"' is not "
			      "a parameter");
    }
    this->checkNotEndOfFile("ModelDSLCommon::readDefaultValue",
			    "Expected default value.");
    if((p->type=="DoubleArray")||
       (p->type=="StringArray")){
      unsigned int nbr=0u;
      auto bend = false;
      while(bend==false){
	if(p->type=="DoubleArray"){
	  istringstream converter(this->current->value);
	  double tmp;
	  converter >> tmp;
	  if(!converter||(!converter.eof())){
	    this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
				    "Could not read default for variable '"+this->currentVar+"'");
	  }
	} else {
	  if(this->current->flag!=Token::String){
	    this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
				    "Expected to read a string (read '"+this->current->value+"').");
	  }
	}
	++nbr;
	res+=" "+this->current->value;
	++(this->current);
	this->checkNotEndOfFile("ModelDSLCommon::readDefaultValue",
				"Expected ',' or ')'.");
	if(this->current->value==")"){
	  bend = true;
	  --(this->current);
	} else if(this->current->value==","){
	  ++(this->current);
	  this->checkNotEndOfFile("ModelDSLCommon::readDefaultValue",
				  "Expected default value.");
	} else {
	  this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
				  "Unexpected token (expected ',' or ')', read '"+this->current->value+"').");
	}
      }
      res = to_string(nbr)+res;
    } else if ((p->type=="double")||(p->type=="real")){
      istringstream converter(this->current->value);
      double tmp;
      converter >> tmp;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("MFrontMode⁹8lParserCommon::readDefaultValue",
				"Could not read default for variable '"+this->currentVar+"'");
      }
      res = this->current->value;
    } else if (p->type=="string"){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
				"Expected to read a string (read '"+this->current->value+"').");
      }
      res = this->current->value;
    } else if (p->type=="bool"){
      if((this->current->value!="true")&&
	 (this->current->value!="false")){
	this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
				"expected to read 'true' or 'false' for type 'bool', read '"+
				this->current->value+"'");
      }
      res = this->current->value;
    } else {
      this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
			      "type '"+p->type+"' is not supported.");
    }
    if(!this->defaultValues.insert({this->currentVar,res}).second){
      this->throwRuntimeError("ModelDSLCommon::readDefaultValue",
			      "default value for field '"+ this->currentVar +"' already defined.");
    }
  } // end of ModelDSLCommon::readDefaultValue

  void
  ModelDSLCommon::treatBounds(void)
  {
    this->registerBounds(this->boundsDescriptions);
  } // end of ModelDSLCommon::treatBounds

  void
  ModelDSLCommon::treatPhysicalBounds(void)
  {
    this->registerBounds(this->physicalBoundsDescriptions);
  } // end of ModelDSLCommon::treatPhysicalBounds

  void
  ModelDSLCommon::registerBounds(std::vector<VariableBoundsDescription>& container)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    VariableBoundsDescription boundsDescription;
    bool found;

    this->checkNotEndOfFile("ModelDSLCommon::registerBounds");

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
      this->throwRuntimeError("ModelDSLCommon::registerBounds",
			      this->current->value+" is not a valid identifier.");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::registerBounds","in");
    this->checkNotEndOfFile("ModelDSLCommon::registerBounds ",
			    "Expected ']' or '['.");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("ModelDSLCommon::registerBounds ",
			      "Expected '*'.");
      if(this->current->value!="*"){
	this->throwRuntimeError("ModelDSLCommon::registerBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      boundsDescription.boundsType = VariableBoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("ModelDSLCommon::registerBounds ",
			      "Expected lower bound value for variable "+boundsDescription.varName);
      istringstream converter(this->current->value);
      converter >> boundsDescription.lowerBound;
      boundsDescription.boundsType = VariableBoundsDescription::LowerAndUpper;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("ModelDSLCommon::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
    } else {
      this->throwRuntimeError("ModelDSLCommon::registerBounds",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::registerBounds",":");
    this->checkNotEndOfFile("ModelDSLCommon::registerBounds",
			    "Could not read upper bound value for variable "+boundsDescription.varName);
    if(this->current->value=="*"){
      if(boundsDescription.boundsType==VariableBoundsDescription::Upper){
	this->throwRuntimeError("ModelDSLCommon::registerBounds",
				"Upper and lower values bounds are both infinity. This is inconsistent.");
      }
      boundsDescription.boundsType=VariableBoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("ModelDSLCommon::registerBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("ModelDSLCommon::registerBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> boundsDescription.upperBound;
      if(!converter||(!converter.eof())){
	this->throwRuntimeError("ModelDSLCommon::registerBounds",
				"Could not read lower bound value for variable '"+
				boundsDescription.varName+"' (read '"+this->current->value+"')");
      }
      if(boundsDescription.boundsType==VariableBoundsDescription::LowerAndUpper){
	if(boundsDescription.lowerBound>boundsDescription.upperBound){
	  this->throwRuntimeError("ModelDSLCommon::registerBounds",
				  "Lower bound value is greater than upper bound value for variable '"+
				  boundsDescription.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("ModelDSLCommon::registerBounds",
			      "Expected ']'");
      if(this->current->value!="]"){
	this->throwRuntimeError("ModelDSLCommon::registerBounds",
				"Expected ']' (read '"+this->current->value+"'");
      }      
    }
    ++(this->current);
    this->readSpecifiedToken("ModelDSLCommon::registerBounds",";");
    container.push_back(boundsDescription);
  } // end of ModelDSLCommon::registerBounds

  void
  ModelDSLCommon::addMaterialLaw(const std::string& m)
  {
    using namespace std;
    if(find(this->materialLaws.begin(),
	    this->materialLaws.end(),m)==this->materialLaws.end()){
      this->materialLaws.push_back(m);
    }
  } // end of ModelDSLCommon::addMaterialLaw

  void
  ModelDSLCommon::appendToIncludes(const std::string& c)
  {
    this->includes+=c;
    if(!this->includes.empty()){
      if(*(this->includes.rbegin())!='\n'){
	this->includes+='\n';
      }
    }
  } // end of ModelDSLCommon::appendToIncludes

  void
  ModelDSLCommon::appendToMembers(const std::string& c)
  {
    this->members+=c;
    if(!this->members.empty()){
      if(*(this->members.rbegin())!='\n'){
	this->members+='\n';
      }
    }
  } // end of ModelDSLCommon::appendToMembers

  void
  ModelDSLCommon::appendToPrivateCode(const std::string& c)
  {
    this->privateCode+=c;
    if(!this->privateCode.empty()){
      if(*(this->privateCode.rbegin())!='\n'){
	this->privateCode+='\n';
      }
    }
  } // end of ModelDSLCommon::appendToPrivateCode

  void
  ModelDSLCommon::appendToSources(const std::string& c)
  {
    this->sources+=c;
    if(!this->sources.empty()){
      if(*(this->sources.rbegin())!='\n'){
	this->sources+='\n';
      }
    }
  } // end of ModelDSLCommon::appendToSources

  ModelDSLCommon::~ModelDSLCommon()
  {} // end of ModelDSLCommon::~ModelDSLCommon

} // end of mfront
