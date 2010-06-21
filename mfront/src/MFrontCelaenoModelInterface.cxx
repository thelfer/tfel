/*!
 * \file   MFrontCelaenoModelInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<stdexcept>
#include<iterator>
#include<sstream>
#include<algorithm>
#include<cassert>

#include"MFront/ParserUtilities.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/VarHandler.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/StaticVarHandler.hxx"
#include"MFront/MFrontModelParserCommon.hxx"
#include"MFront/MFrontCelaenoModelInterface.hxx"

static const unsigned short TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY = 8;

namespace mfront{

  static bool
  isInputVariable(const MFrontModelData& data,
		  const std::string& v)
  {
    return MFrontModelParserCommon::is(data,data.inputs,v);
  } // end of MFrontModelParserCommon::isInputVariable(void)

  static std::pair<std::string,unsigned short>
  decomposeVariableName(const MFrontModelData& data,
			const std::string& v)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,unsigned short>::const_iterator p2;
    unsigned short d;
    unsigned short i;
    for(p=data.inputs.begin();p!=data.inputs.end();++p){
      if(v==p->name){
	return pair<string,unsigned short>(v,0u);
      }
      p2 = data.depths.find(p->name);
      if(p2!=data.depths.end()){
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
    for(p=data.outputs.begin();p!=data.outputs.end();++p){
      if(v==p->name){
	return pair<string,unsigned short>(v,0);
      }
      p2 = data.depths.find(p->name);
      if(p2!=data.depths.end()){
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
  } // end of MFrontModelParserCommon::getCelaenoVariableName(const std::string& v)

  MFrontCelaenoModelInterface::MFrontCelaenoModelInterface(void)
  {
    this->hasDefaultConstructor     = false;
    this->verboseMode = false;
    this->debugMode   = false;
    this->warningMode = false;
  } // end of MFrontCelaenoModelInterface::MFrontCelaenoModelInterface

  void
  MFrontCelaenoModelInterface::declareReservedNames(std::set<std::string>& v)
  {
    v.insert("dt");
    v.insert("boost");
    v.insert("field");
    v.insert("support");
    v.insert("values");
    v.insert("results");
    v.insert("domains");
    v.insert("requirement");
    v.insert("requirementManager");
    v.insert("md");
    v.insert("ptr");
    v.insert("ptr2");
    v.insert("ptr3");
    v.insert("tmp");
    v.insert("outputsDepths");
    v.insert("outputsInitialValues");
    v.insert("inputsDepths");
    v.insert("ValidParametersNames");
    v.insert("computeMaterialProperties");
    v.insert("apply");
    v.insert("FieldHolder");
    v.insert("MTFieldManager");
    v.insert("mm");
    v.insert("initialize");
    v.insert("initializeOutput");
    v.insert("initializeOutputsVariablesDepths");
    v.insert("initializeOutputsVariablesInitialValues");
    v.insert("initializeInputsVariablesDepths");
    v.insert("initializeConstantMaterialProperties");
    v.insert("constantMaterialProperties");
    v.insert("declareRequirements");
    v.insert("resolveDependencies");
    v.insert("execute");
    v.insert("executeInitialPostProcessingTasks");
    v.insert("executePostProcessingTasks");
    v.insert("getName");
    v.insert("data");
  }

  bool
  MFrontCelaenoModelInterface::hasSpecializedConstructor(const MFrontModelData& data) const
  {
    MFrontModelData::FunctionContainer::const_iterator p;
    if(!data.localParameters.empty()){
      return true;
    }
    if(!data.domains.empty()){
      return true;
    }
    for(p=data.functions.begin();p!=data.functions.end();++p){
      if(!p->domains.empty()){
	return true;
      }
    }
    return false;
  } // end of MFrontCelaenoModelInterface::hasSpecializedConstructor

  void
  MFrontCelaenoModelInterface::openOutputFiles(void)
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
      string msg("MFrontCelaenoModelInterface::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MFrontCelaenoModelInterface::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.exceptions(ios::badbit|ios::failbit);
  } // end of MFrontCelaenoModelInterface::openOutputFiles()

  void
  MFrontCelaenoModelInterface::closeOutputFiles(void)
  {
    this->headerFile.close();
    this->srcFile.close();
  } // end of MFrontCelaenoModelInterface::closeOutputFiles()
  
  void
  MFrontCelaenoModelInterface::generateOutputFiles(const MFrontGenericData& pdata,
						   const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    MFrontModelData::FunctionContainer::const_iterator p2;
    bool found;
    this->hasDefaultConstructor=true;
    if(!data.localParameters.empty()){
      for(p=data.localParameters.begin();
	  (p!=data.localParameters.end())&&(this->hasDefaultConstructor);++p){
	if(data.defaultValues.find(p->name)==data.defaultValues.end()){
	  this->hasDefaultConstructor = false;
	}
      }
    }
    // sanity checks
    for(p2=data.functions.begin();p2!=data.functions.end();++p2){
      if(p2->modifiedVariables.size()==0){
	string msg("MFrontCelaenoModelInterface::generateOutputFiles : ");
	msg += "function '"+p2->name+"' does not modify any output";
	throw(runtime_error(msg));
      }
    }
    for(p=data.outputs.begin();p!=data.outputs.end();++p){
      found = false;
      for(p2=data.functions.begin();
	  (p2!=data.functions.end())&&(!found);++p2){
	if(p2->modifiedVariables.find(p->name)!=p2->modifiedVariables.end()){
	  found = true;
	}
      }
      if(!found){
	string msg("MFrontCelaenoModelInterface::generateOutputFiles : ");
	msg += "output '"+p->name+"' is not modified by any function";
	throw(runtime_error(msg));
      }
    }
    this->writeHeaderFile(pdata,data);
    this->writeSrcFile(pdata,data);
  } // end of MFrontCelaenoModelInterface::generateOutputFiles(void)

  void
  MFrontCelaenoModelInterface::writeHeaderFile(const MFrontGenericData& pdata,
						const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    StaticVarContainer::const_iterator p2;
    MFrontModelData::FunctionContainer::const_iterator p3;
    set<string>::iterator p4;
    set<string>::iterator p5;
    unsigned short i;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << pdata.className << " CelaenoModel.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!pdata.authorName.empty()){
      this->headerFile << "* \\author " << pdata.authorName << endl;
    }
    if(!pdata.date.empty()){
      this->headerFile << "* \\date   " << pdata.date       << endl;
    }
    if(!pdata.description.empty()){
      this->headerFile << pdata.description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _PLEIADES_" 
		     << makeUpperCase(pdata.className)
		     << "_HH\n";
    this->headerFile << "#define _PLEIADES_"
		     << makeUpperCase(pdata.className)
		     << "_HH\n\n";

    this->headerFile << "#include<string>\n\n";
    this->headerFile << "#include\"Pleiades/Model/ModelBase.hxx\"\n\n";
    if(!pdata.includes.empty()){
      this->headerFile << pdata.includes << endl << endl;
    }
    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace pleiades\n{\n\n";
    this->headerFile << "namespace model\n{\n\n";
    this->headerFile << "struct MFRONT_SHAREDOBJ " << pdata.className << endl;
    this->headerFile << ": public ModelBase\n";
    this->headerFile << "{\n\n";
    this->headerFile << "//! Default constructor\n";
    this->headerFile << pdata.className
		     << "(pleiades::mesh::SMeshManager&,\n"
		     << "pleiades::field::SFieldManager&,\n"
		     << "pleiades::time::SClock&,\n"
		     << "pleiades::loading::SLoadingManager&,\n"
		     << "pleiades::signal::SSignalManager&);\n\n";
    if(this->hasSpecializedConstructor(data)){
      this->headerFile << "//! Constructor to initialize local parameters\n";
      this->headerFile << pdata.className
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
    this->headerFile << "~" << pdata.className << "();\n\n";
    this->headerFile << "private:\n\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "//! Copy constructor (disabled)\n";
    this->headerFile << pdata.className << "(const " 
		     << pdata.className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << pdata.className << "&\n"
		     << "operator=(const " 
		     << pdata.className << "&);\n\n";
    for(p3=data.functions.begin();p3!=data.functions.end();++p3){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontCelaenoModelInterface::writeHeaderFile : ");
	msg+="function " + p3->name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << pdata.fileName << "\"\n";
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
      this->headerFile << "friend class " << pdata.className <<";\n\n";
      if(!p3->domains.empty()){
	this->headerFile << "std::vector<std::string> domains;" << endl;
      }
      for(p4=p3->globalParameters.begin();p4!=p3->globalParameters.end();++p4){
	p = MFrontCelaenoModelInterface::findVariableDescription(data.globalParameters,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << pdata.fileName << "\"\n";
	}
	this->headerFile << p->type << " " << p->name << ";\n";
      }
      for(p4=p3->localParameters.begin();p4!=p3->localParameters.end();++p4){
	p = MFrontCelaenoModelInterface::findVariableDescription(data.localParameters,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << pdata.fileName << "\"\n";
	}
	this->headerFile << p->type << " " << p->name << ";\n";
      }
      for(p4=p3->constantMaterialProperties.begin();p4!=p3->constantMaterialProperties.end();++p4){
	p = MFrontCelaenoModelInterface::findVariableDescription(data.constantMaterialProperties,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << pdata.fileName << "\"\n";
	}
	this->headerFile << p->type << " " << p->name << ";\n";
      }
      if(p3->useTimeIncrement){
	this->headerFile << "pleiades::field::real dt;\n";
      }
      this->headerFile << "}; // end of struct " << p3->name<< "\n\n";
    }
    if(!pdata.staticVars.empty()){
      for(p2=pdata.staticVars.begin();p2!=pdata.staticVars.end();++p2){
	if(this->debugMode){
	  this->headerFile << "#line " << p2->lineNumber << " \"" 
			   << pdata.fileName << "\"\n";
	}
	this->headerFile << "static const " << p2->type << " " << p2->name << ";" << endl;
      }
      this->headerFile << endl;
    }
    if(!data.domains.empty()){
      this->headerFile << "std::vector<std::string> domains;\n";
    }
    for(p=data.globalParameters.begin();p!=data.globalParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n";
    }
    for(p=data.localParameters.begin();p!=data.localParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n";
    }
    this->headerFile << "void\ninitializeOutputsVariablesDepths(void);\n";
    this->headerFile << "void\ninitializeOutputsVariablesInitialValues(const pleiades::parser::DataManager&);\n";
    this->headerFile << "void\ninitializeInputsVariablesDepths(void);";
    if(!data.constantMaterialProperties.empty()){
      this->headerFile << "\nvoid\ninitializeConstantMaterialProperties(const pleiades::parser::DataManager&);\n\n";
    } else {
      this->headerFile << "\n\n";
    }
    for(i=0,p3=data.functions.begin();p3!=data.functions.end();++p3,++i){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontCelaenoModelInterface::writeHeaderFile : ");
	msg += "function " + p3->name + " does not modify any variable ";
	msg += "(internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p3->name << " functor" << i <<  ";\n";
    }
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned short> > outputsDepths;\n";
    this->headerFile << "std::map<std::string,std::map<std::string,pleiades::field::real> > outputsInitialValues;\n";
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned short> > inputsDepths;\n";
    if(!data.constantMaterialProperties.empty()){
      this->headerFile << "std::map<std::string,std::map<std::string,pleiades::field::real> > constantMaterialProperties;\n";
    }
    this->headerFile << "}; // end of struct " << pdata.className << endl << endl;
    this->headerFile << "} // end of namespace model\n\n";
    this->headerFile << "} // end of namespace pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << makeUpperCase(pdata.className)
		     << "_HH */\n";
  } // end of MFrontCelaenoModelInterface::writeHeaderFile(void)

  std::string
  MFrontCelaenoModelInterface::getGenTypeGetMethod(const std::string& type) const
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
      string msg("MFrontCelaenoModelInterface::getGenTypeGetMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontCelaenoModelInterface::getGenTypeGetMethod

  std::string
  MFrontCelaenoModelInterface::getGenTypeIsMethod(const std::string& type) const
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
      string msg("MFrontCelaenoModelInterface::getGenTypeIsMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontCelaenoModelInterface::isGenTypeMethod

  VarContainer::const_iterator
  MFrontCelaenoModelInterface::findVariableDescription(const VarContainer& v,
							const std::string& n)
  {
    using namespace std;
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      if(p->name==n){
	return p;
      }
    }
    string msg("MFrontCelaenoModelInterface::findVariableDescription : ");
    msg += "no variable named '"+n+"' found";
    throw(runtime_error(msg));
    return v.end();
  } // MFrontCelaenoModelInterface::findVariableDescription

  void
  MFrontCelaenoModelInterface::writeSrcFile(const MFrontGenericData& pdata,
					     const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    map<string,string>::const_iterator p4;
    map<string,unsigned short>::const_iterator p5;
    StaticVarContainer::const_iterator p10;
    MFrontModelData::FunctionContainer::const_iterator p11;
    set<string>::const_iterator p12;
    std::map<std::string,std::vector<MFrontModelData::Function> >::iterator p13;
    std::vector<MFrontModelData::Function>::iterator p14;
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
		  << pdata.className << " celaeno model.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!pdata.authorName.empty()){
      this->srcFile << "* \\author " << pdata.authorName << endl;
    }
    if(!pdata.date.empty()){
      this->srcFile << "* \\date   " << pdata.date       << endl;
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
    for(p11=data.functions.begin();p11!=data.functions.end();++p11){
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
		  << pdata.className << "-celaeno.hxx\"\n\n";
    this->srcFile << "namespace pleiades\n{\n\n";
    this->srcFile << "namespace model\n{\n\n";
    // Functors
    for(p11=data.functions.begin();p11!=data.functions.end();++p11){
      // operator()
      if(this->debugMode){
	this->srcFile << "#line " << p11->line << " \"" 
		      << pdata.fileName << "\"\n";
      }
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	this->srcFile << "pleiades::field::real\n";
	this->srcFile << pdata.className << "::" << p11->name << "::operator()(";
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
	this->srcFile << pdata.className << "::" << p11->name << "::operator()(";
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
      writeMaterialLaws("MFrontCelaenoModelInterface::writeSrcFile",
			this->srcFile,pdata.materialLaws);		      
      for(i=0,p12=p11->usedVariables.begin();p12!=p11->usedVariables.end();++p12,++i){
	found = false;
	for(p19=data.physicalBoundsDescriptions.begin();
	    (p19!=data.physicalBoundsDescriptions.end())&&(!found);){
	  found = (p19->varName==*p12);
	  if(!found){
	    ++p19;
	  }
	}
	found = false;
	for(p20=data.boundsDescriptions.begin();
	    (p20!=data.boundsDescriptions.end())&&(!found);){
	  found = (p20->varName==*p12);
	  if(!found){
	    ++p20;
	  }
	}
	if((p19!=data.physicalBoundsDescriptions.end())||
	   (p20!=data.boundsDescriptions.end())){
	  this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
	}
	if(p19!=data.physicalBoundsDescriptions.end()){
	  this->srcFile << "// checking " << p19->varName<< " physical bounds\n";
	  if((p19->boundsType==VariableBoundsDescription::Lower)||
	     (p19->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p19->varName<< " < "<< p19->lowerBound << "){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << pdata.className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p19->varName << "' is below its physical lower bound (\" << "
			  << p19->varName << " << \"<" << p19->lowerBound << ").\\n\";\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n";
	    this->srcFile << "}\n";
	  }
	  if((p19->boundsType==VariableBoundsDescription::Upper)||
	     (p19->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p19->varName << " > " << p19->upperBound << "){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << pdata.className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p19->varName << "' is over its physical upper bound (\" << "
			  << p19->varName << " << \" > " << p19->upperBound << ").\\n\";\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n",
	    this->srcFile << "}\n";
	  }
	}
	if(p20!=data.boundsDescriptions.end()){
	  this->srcFile << "// checking " << p20->varName<< " bounds\n";
	  if((p20->boundsType==VariableBoundsDescription::Lower)||
	     (p20->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p20->varName<< " < "<< p20->lowerBound << "){\n";
	    this->srcFile << "if(pleiades::getOutOfBoundsPolicy()!= pleiades::NO_OUT_OF_BOUNDS_POLICY){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << pdata.className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p20->varName << "' is below its lower bound (\" << "
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
			  << "msg << \"" << pdata.className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p20->varName << "' is over its upper bound (\" << "
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
	if((p19!=data.physicalBoundsDescriptions.end())||
	   (p20!=data.boundsDescriptions.end())){
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
      this->srcFile << "} // end of " << pdata.className << "::" << p11->name << "::operator()\n\n";
    }
    // static variables
    if(!pdata.staticVars.empty()){
      for(p10=pdata.staticVars.begin();p10!=pdata.staticVars.end();++p10){
	this->writeStaticVariableInitialization(pdata,*p10);
      }
      this->srcFile << endl;
    }
    // Constructors
    if(this->hasDefaultConstructor){
      this->srcFile << pdata.className << "::" 
		    << pdata.className
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
      for(p=data.localParameters.begin();p!=data.localParameters.end();++p){
	string name;
	name = this->getCelaenoVariableName(p->name,data);
	p4=data.defaultValues.find(p->name);
	assert(p4!=data.defaultValues.end());
	this->writeAssignDefaultValue(pdata,p,p4);
      }
      if(!data.domains.empty()){
	for(p15=data.domains.begin();p15!=data.domains.end();++p15){
	  this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	  this->srcFile << "for(ptr=tmp.begin();ptr!=tmp.end();++ptr){\n";
	  this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr)!=this->domains.end()){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
	  this->srcFile << "msg += \"domain '\"+*ptr+\"' multiply defined\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "this->domains.push_back(*ptr);\n";
	  this->srcFile << "}\n";
	}
      }
      for(i=0,p11=data.functions.begin();p11!=data.functions.end();++p11,++i){
	if(!p11->domains.empty()){
	  string functor = "functor"+toString(i);
	  for(p15=p11->domains.begin();p15!=p11->domains.end();++p15){
	    this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	    this->srcFile << "if(find(this->" << functor << ".domains.begin(),"
			  << "this->" << functor << ".domains.end(),*ptr2)!=" 
			  << "this->" << functor << ".domains.end()){\n";
	    this->srcFile << "string msg(\"" << pdata.className << "::" 
			  << pdata.className << " : \");\n";
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
		    << pdata.className << "::" 
		    << pdata.className <<"\n\n";
    }
    if(this->hasSpecializedConstructor(data)){
      this->srcFile << "//! Constructor to initialize local parameters\n";
      this->srcFile << pdata.className << "::" 
		    << pdata.className 
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
      for(p=data.localParameters.begin();p!=data.localParameters.end();){
	this->srcFile << '"' << p->name << "\"";
	if(++p!=data.localParameters.end()){
	  this->srcFile << ",\n";
	}
	if(first){
	  first = false;
	}
	++(specializedParametersNumber);
      }
      if(!data.domains.empty()){
	if(first){
	  this->srcFile << "\"domains\"";
	  first = false;
	} else {
	  this->srcFile << ",\n\"domains\"";
	}
	++(specializedParametersNumber);
      }
      for(p11=data.functions.begin();p11!=data.functions.end();++p11){
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
		    << data.localParameters.size()
		    << ",ptr->first)==ValidParametersNames+"
		    << specializedParametersNumber << "){\n";
      this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
      this->srcFile << "msg += \"unknown parameter '\"+ptr->first+\"'\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      for(p=data.localParameters.begin();p!=data.localParameters.end();++p){
	string name;
	name = this->getCelaenoVariableName(p->name,data);
	this->srcFile << "ptr = data.find(" << name << ");\n";
	this->srcFile << "if(ptr==data.end()){\n";
	if((p4=data.defaultValues.find(p->name))!=data.defaultValues.end()){
	  this->writeAssignDefaultValue(pdata,p,p4);
	  this->srcFile << "} else {\n";
	} else {
	  this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
	  this->srcFile << "msg += \"can't initialize parameter " << p->name << "\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "} else {\n";
	}
	this->srcFile << "if(!ptr->second." << this->getGenTypeIsMethod(p->type) << "()){\n";
	this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
	this->srcFile << "msg += \"wrong type for parameter '" << p->name << "' (expected a '"+p->type+"')\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->" << p->name << " = ptr->second." 
		      << this->getGenTypeGetMethod(p->type) << "();\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "ptr = data.find(\"domains\");\n";
      this->srcFile << "if(ptr!=data.end()){\n";
      this->srcFile << "if(!ptr->second.is<vector<string> >()){\n";
      this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
      this->srcFile << "msg += \"invalid type for parameter 'domains'\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "for(ptr3=ptr->second.get<vector<string> >().begin();\n";
      this->srcFile << "ptr3!=ptr->second.get<vector<string> >().end();++ptr3){\n";
      this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,*ptr3);\n";
      this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
      this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
      this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << ": \");\n";
      this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "this->domains.push_back(*ptr2);\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      this->srcFile << "} else {\n";
      for(p15=data.domains.begin();p15!=data.domains.end();++p15){
	this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
	this->srcFile << "string msg(\"" << pdata.className << "::" 
		      << pdata.className << " : \");\n";
	this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->domains.push_back(*ptr2);\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "}\n";
      for(i=0,p11=data.functions.begin();p11!=data.functions.end();++p11,++i){
	if(!p11->domains.empty()){
	  string functor = "functor"+toString(i);
	  this->srcFile << "ptr = data.find(\"" << functor << ".domains\");\n";
	  this->srcFile << "if(ptr!=data.end()){\n";
	  this->srcFile << "if(!ptr->second.is<vector<string> >()){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
	  this->srcFile << "msg += \"invalid type for parameter '"+functor+".domains'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "for(ptr3=ptr->second.get<vector<string> >().begin();\n";
	  this->srcFile << "ptr3!=ptr->second.get<vector<string> >().end();++ptr3){\n";
	  this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,*ptr3);\n";
	  this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	  this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << ": \");\n";
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
	    this->srcFile << "string msg(\"" << pdata.className << "::" 
			  << pdata.className << " : \");\n";
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
		    << pdata.className << "::" 
		    << pdata.className <<"\n\n";
    }
    this->writeInitializeOutputsVariablesDepths(pdata,data);
    this->writeInitializeInputsVariablesDepths(pdata,data);
    // getName
    this->srcFile << "std::string\n"
		  << pdata.className
		  << "::getName(void) const\n"
		  << "{\n"
		  << "return \"" << pdata.className << "\";\n"
		  << "}\n\n";
    // initialize
    this->srcFile << "void\n"
		  << pdata.className
		  << "::initialize(const pleiades::parser::DataManager& data)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string> tmp;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(p=data.globalParameters.begin();p!=data.globalParameters.end();++p){
      string name = this->getCelaenoVariableName(p->name,data);
      this->srcFile << "if(!data.contains(" << name << ")){\n";
      if((p4=data.defaultValues.find(p->name))!=data.defaultValues.end()){
	this->srcFile << "this->" << p->name << " = " << p4->second << ";" << endl;
	this->srcFile << "} else {\n";
      } else {
	this->srcFile << "string msg(\"" << pdata.className << "::initialize : \");\n";
	this->srcFile << "msg += \"can't initialize parameter '" << p->name << "'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "} else {\n";
      }
      this->srcFile << "if(!data." << this->getGenTypeIsMethod(p->type) << "(" << name << ")){\n";
      this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
      this->srcFile << "msg += \"wrong type for parameter '" << p->name << "' (expected a '"+p->type+"')\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "this->" << p->name << " = data." 
		    << this->getGenTypeGetMethod(p->type) << "(" << name << ");\n";
      this->srcFile << "}\n";
    }
//     if(!data.initializeParameters.empty()){
//       this->srcFile << "// initialize parameters\n";
//       this->srcFile << data.initializeParameters;
//       this->srcFile << endl;
//     }
    this->srcFile << "this->initializeOutputsVariablesInitialValues(data);\n";
    if(!data.constantMaterialProperties.empty()){
      this->srcFile << "this->initializeConstantMaterialProperties(data);\n";
    }
    // initializing functor members
    for(i=0,p11=data.functions.begin();p11!=data.functions.end();++p11,++i){
      string functor = "functor"+toString(i);
      for(p15=p11->localParameters.begin();
	  p15!=p11->localParameters.end();++p15){
	this->srcFile << "this->" << functor << '.' << *p15 << " = this->" << *p15 << ";\n";
      }
      for(p15=p11->globalParameters.begin();p15!=p11->globalParameters.end();++p15){
	this->srcFile << "this->" << functor << '.' << *p15 << " = this->" << *p15 << ";\n";
      }
    }
    this->srcFile << "} // end of " << pdata.className << "::initialize\n\n";


    this->writeInitializeOutputsVariablesInitialValues(pdata,data);
    if(!data.constantMaterialProperties.empty()){
      this->writeInitializeConstantMaterialProperties(pdata,data);
    }
    // declareRequirements
    this->srcFile << "void\n";
    this->srcFile << pdata.className << "::declareRequirements(pleiades::coupling::SRequirementManager& requirementManager)\n";
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
    this->srcFile << "} // end of " << pdata.className << "::declareRequirements\n\n";
    // resolveDependencies
    this->srcFile << "void\n";
    this->srcFile << pdata.className << "::resolveRequirements(pleiades::coupling::SRequirementManager& requirementManager)\n";
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
    this->srcFile << "} // end of " << pdata.className << "::resolveRequirements\n\n";
    // initializeOutput
    this->srcFile << "void\n";
    this->srcFile << pdata.className << "::initializeOutput(pleiades::coupling::Requirement& requirement)\n";
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
    this->srcFile << "string msg(\"" << pdata.className << "::initializeOutput: \");\n";
    this->srcFile << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr4 = ptr3->second.find(requirement.getLocation());\n";
    this->srcFile << "if(ptr4==ptr3->second.end()){\n";
    this->srcFile << "string msg(\"" << pdata.className << "::initializeOutput: \");\n";
    this->srcFile << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "this->smanager.create<real>(requirement.getLocation(),requirement.getName(),\n";
    this->srcFile << "ptr4->second,max(requirement.getDepth(),ptr2->second));\n";
    this->srcFile << "} // end of " << pdata.className << "::initializeOutput\n\n";
    // execute
    this->srcFile << "void\n"
		  << pdata.className
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
    for(p11=data.functions.begin();(p11!=data.functions.end())&&(!found);++p11){
      if(p11->useTimeIncrement){
	this->srcFile << "const pleiades::field::real dt = convert_to_double(this->sclock.getCurrentTimeIncrement());\n";
	found = true;
      }
    }
    for(i=0,p11=data.functions.begin();p11!=data.functions.end();++p11,++i){
      string functor = "functor"+toString(i);
      if(p11->useTimeIncrement){
	this->srcFile << "this->" << functor << ".dt=dt;\n";
      }
      if(!(p11->domains.empty())){
	this->srcFile << "for(ptr=this->" << functor <<".domains.begin();ptr!=this->" << functor <<".domains.end();++ptr){\n";
      } else {
	if(data.domains.empty()){
	  string msg("MFrontCelaenoModelInterface::writeSrcFile : ");
	  msg += "no domain specified for function '"+p11->name+"'";
	  throw(runtime_error(msg));
	}
	this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
      }
      this->srcFile << "MTFieldManager& mm = this->smanager.getMTFieldManager(*ptr);\n";
      for(p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();++p15){
	pair<string,unsigned short> dv = decomposeVariableName(data,*p15);
	this->srcFile << "const FieldHolder& " << "f_" << *p15 
		      << " = mm.getFieldHolder(" << this->getCelaenoVariableName(dv.first,data)
		      << "," << dv.second << ");\n";
      }
      for(p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15){
	this->srcFile << "FieldHolder& " << "f_" << *p15 
		      << " = mm.getFieldHolder(" << this->getCelaenoVariableName(*p15,data) << ");\n";
      }
      for(p15=p11->constantMaterialProperties.begin();
	  p15!=p11->constantMaterialProperties.end();++p15){
	this->srcFile << "this->" << functor << "." << *p15 
		      << " = this->constantMaterialProperties[*ptr][\"" << *p15 << "\"];\n";
      }
      this->srcFile << "if(getVerboseMode()==VERBOSE_LEVEL1){\n";
      this->srcFile << "ostream& log = getLogStream();\n";
      this->srcFile << "log << \"**" << pdata.className << "::execute : \"" << "\n";
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
	  string msg("MFrontCelaenoModelInterface : untreated case");
	  throw(runtime_error(msg));
	}
      }
    }
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << pdata.className << "::execute\n\n";
    this->srcFile << "void\n" << pdata.className 
		  << "::executeInitialPostProcessingTasks(const bool)\n{} // end of " << pdata.className 
		  << "::executeInitialPostProcessingTasks\n\n";
    
    this->srcFile << "void\n" << pdata.className 
		  << "::executePostProcessingTasks(const bool)\n{} // end of " << pdata.className 
		  << "::executePostProcessingTasks\n\n";
    
    this->srcFile << pdata.className << "::~" << pdata.className << "()\n{}\n\n";
    
    if(this->hasSpecializedConstructor(data)){
      this->srcFile << "PLEIADES_GENERATE_PROXY2(IModel," << pdata.className << ")\n\n";
    } else {
      this->srcFile << "PLEIADES_GENERATE_PROXY(IModel," << pdata.className << ")\n\n";
    }
    this->srcFile << "} // end of namespace model\n\n";
    this->srcFile << "} // end of namespace pleiades\n";
  } // end of MFrontCelaenoModelInterface::writeSrcFile(void)
    
  void
  MFrontCelaenoModelInterface::writeInitializeOutputsVariablesDepths(const MFrontGenericData& pdata,
								      const MFrontModelData& data)
  {
    using namespace std;
    MFrontModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,unsigned short>::const_iterator p3;
    map<string,double>::const_iterator p4;
    unsigned short i;
    this->srcFile << "void\n" << pdata.className << "::initializeOutputsVariablesDepths(void)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=data.functions.begin();p!=data.functions.end();++p,++i){
      for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();++p2){
	string functor = "functor"+toString(i);
	p3 = p->depths.find(*p2);
	if(p3==p->depths.end()){
	  string msg("MFrontCelaenoModelInterface::writeInitializeOutputsVariablesDepths : ");
	  msg += "internal error, no depth found for variable '"+*p2+"' in function '"+p->name+"'";
	  throw(runtime_error(msg));
	}
	if(!p->domains.empty()){
	  this->srcFile << "for(ptr=this->" << functor << ".domains.begin();"
			<< "ptr!=this->" << functor << ".domains.end();++ptr){\n";
	} else {
	  this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	}
	this->srcFile << "if(!this->outputsDepths[" << this->getCelaenoVariableName(*p2,data)
		      << "].insert(make_pair(*ptr,";
	this->srcFile << p3->second;
	this->srcFile << ")).second){\n";
	this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
	this->srcFile << "msg += \"output '\";\n";
	this->srcFile << "msg += " << this->getCelaenoVariableName(*p2,data) << ";\n";
	this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << pdata.className << "::initializeOutputsVariablesDepths\n\n";
  } // end of MFrontCelaenoModelInterface::writeInitializeOutputsVariablesDepths()

  void
  MFrontCelaenoModelInterface::writeInitializeConstantMaterialProperties(const MFrontGenericData& pdata,
									  const MFrontModelData& data)
  {
    using namespace std;
    MFrontModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,string>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << pdata.className
		  << "::initializeConstantMaterialProperties(" 
		  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "typedef ConstantMaterialPropertyDescription CMPD;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=data.functions.begin();p!=data.functions.end();++p,++i){
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
	this->srcFile << "string msg(\"" << pdata.className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"no material description  on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const MaterialDescription& md = *(data.getMaterialDescription(*ptr));\n";
	this->srcFile << "if(md.containsMaterialProperty(" << this->getCelaenoVariableName(*p2,data) << ")){\n";
	this->srcFile << "if(!md.isMaterialPropertyAccesible(" << this->getCelaenoVariableName(*p2,data) << ")){\n";
	this->srcFile << "string msg(\"" << pdata.className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"material property '\";\n";
	this->srcFile << "msg += " <<  this->getCelaenoVariableName(*p2,data) << ";\n";
	this->srcFile << "msg += \"' is not accessible on material '\"+*ptr+\"'\";\n";
	this->srcFile << "msg += \"' (this means that this property is define within a behaviour)\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const MaterialPropertyDescription& mpd = *(md.getMaterialProperty(" 
		      << this->getCelaenoVariableName(*p2,data) << "));\n";
	this->srcFile << "if(!mpd.is<CMPD>()){\n";
	this->srcFile << "string msg(\"" << pdata.className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"material property '\";\n";
	this->srcFile << "msg += " <<  this->getCelaenoVariableName(*p2,data) << ";\n";
	this->srcFile << "msg += \"' is not constant on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->constantMaterialProperties[*ptr][\"" << *p2 << "\"]" 
		      << " = mpd.get<CMPD>().getValue();\n";
	this->srcFile << "} else {\n";
	if((p3=data.defaultValues.find(*p2))!=data.defaultValues.end()){
	  this->srcFile << "this->constantMaterialProperties[*ptr][\"" << *p2 
			<< "\"] = " << p3->second << ";\n";	  
	} else {
	  this->srcFile << "string msg(\"" << pdata.className << "::initializeConstantMaterialProperties : \");\n";
	  this->srcFile << "msg += \"material property '\";\n";
	  this->srcFile << "msg += " <<  this->getCelaenoVariableName(*p2,data) << ";\n";
	  this->srcFile << "msg += \"' is undefined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	}
	this->srcFile << "}\n";
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << pdata.className << "::initializeConstantMaterialProperties\n\n";
  } // end of MFrontCelaenoModelInterface::writeInitializeConstantMaterialProperties(void)

  void
  MFrontCelaenoModelInterface::writeInitializeOutputsVariablesInitialValues(const MFrontGenericData& pdata,
									     const MFrontModelData& data)
  {
    using namespace std;
    MFrontModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << pdata.className
		  << "::initializeOutputsVariablesInitialValues(" 
		  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=data.functions.begin();p!=data.functions.end();++p,++i){
      for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();++p2){
	string functor = "functor"+toString(i);
	p3=data.initialValues.find(*p2);
	if(!p->domains.empty()){
	  this->srcFile << "for(ptr=this->" << functor << ".domains.begin();"
			<< "ptr!=this->" << functor << ".domains.end();++ptr){\n";
	} else {
	  this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	}
	// getting material description
	if(p3!=data.initialValues.end()){
	  this->srcFile << "if(data.hasMaterialDescription(*ptr)){\n";
	  this->srcFile << "const MaterialDescription& md = *(data.getMaterialDescription(*ptr));\n";
	  this->srcFile << "if(md.containsStateVariable(" <<  this->getCelaenoVariableName(*p2,data) << ")){\n";
	  this->srcFile << "real tmp = md.getStateVariable("<<  this->getCelaenoVariableName(*p2,data) << ")->getInitialValue();\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getCelaenoVariableName(*p2,data)
			<< "].insert(make_pair(*ptr,tmp)).second){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getCelaenoVariableName(*p2,data) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getCelaenoVariableName(*p2,data)
			<< "].insert(make_pair(*ptr,";
	  this->srcFile << p3->second;
	  this->srcFile << ")).second){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getCelaenoVariableName(*p2,data) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getCelaenoVariableName(*p2,data)
			<< "].insert(make_pair(*ptr,";
	  this->srcFile << p3->second;
	  this->srcFile << ")).second){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getCelaenoVariableName(*p2,data) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	} else {
	  this->srcFile << "if(!data.hasMaterialDescription(*ptr)){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"no material description  on material '\"+*ptr+\"', \";\n";
	  this->srcFile << "msg += \"required to initialize output value '\";\n";
	  this->srcFile << "msg += " << this->getCelaenoVariableName(*p2,data) << ";\n";
	  this->srcFile << "msg += '\\\'';\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "const MaterialDescription& md = *(data.getMaterialDescription(*ptr));\n";
	  this->srcFile << "if(md.containsStateVariable(" <<  this->getCelaenoVariableName(*p2,data) << ")){\n";
	  this->srcFile << "real tmp = md.getStateVariable("<<  this->getCelaenoVariableName(*p2,data) << ")->getInitialValue();\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getCelaenoVariableName(*p2,data)
			<< "].insert(make_pair(*ptr,tmp)).second){\n";
	  this->srcFile << "string msg(\"" << pdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " << this->getCelaenoVariableName(*p2,data) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	}
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << pdata.className << "::initializeOutputsVariablesInitialValues\n\n";
  } // end of MFrontCelaenoModelInterface::writeInitializeOutputsVariablesInitialValues()

  void
  MFrontCelaenoModelInterface::writeInitializeInputsVariablesDepths(const MFrontGenericData& pdata,
								     const MFrontModelData& data)
  {
    using namespace std;
    set<string> treatedVars;
    MFrontModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,unsigned short>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << pdata.className << "::initializeInputsVariablesDepths(void)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=data.functions.begin();p!=data.functions.end();++p,++i){
      for(p2=p->usedVariables.begin();p2!=p->usedVariables.end();++p2){
	if(isInputVariable(data,*p2)){
	  const string& v = decomposeVariableName(data,*p2).first;
	  if(treatedVars.find(v)==treatedVars.end()){
	    string functor = "functor"+toString(i);
	    p3 = p->depths.find(v);
	    if(p3==p->depths.end()){
	      string msg("MFrontCelaenoModelInterface::writeInitializeInputsVariablesDepths : ");
	      msg += "internal error, no depth found for variable '"+v+"' in function '"+p->name+"'";
	      throw(runtime_error(msg));
	    }
	    if(!p->domains.empty()){
	      this->srcFile << "for(ptr=this->" << functor << ".domains.begin();"
			    << "ptr!=this->" << functor << ".domains.end();++ptr){\n";
	    } else {
	      this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	    }
	    this->srcFile << "map<string,unsigned short>& tmp = this->inputsDepths[" << this->getCelaenoVariableName(v,data) << "];\n";
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
    this->srcFile << "} // end of " << pdata.className << "::initializeInputsVariablesDepths\n\n";
  } // end of MFrontCelaenoModelInterface::writeInitializeInputsVariablesDepths()

  std::string
  MFrontCelaenoModelInterface::getCelaenoVariableName(const std::string& v,
							const MFrontModelData& data) const
  {
    using namespace std;
    map<string,string>::const_iterator p;
    if((p=data.glossaryNames.find(v))!=data.glossaryNames.end()){
      return "Glossary::" + p->second;
    } else if((p=data.entryNames.find(v))!=data.entryNames.end()){
      return "\""+p->second+"\"";
    }
    return "\""+v+"\"";
  } // end of MFrontCelaenoModelInterface::getCelaenoVariableName(const std::string& v)

  void
  MFrontCelaenoModelInterface::writeStaticVariableInitialization(const MFrontGenericData& pdata,
								  const StaticVarHandler& v)
  {
    using namespace std;
    if(this->debugMode){
      this->srcFile << "#line " << v.lineNumber << " \"" 
		    << pdata.fileName << "\"\n";
    }
    if(v.type=="short"){
      this->srcFile << "const short " << pdata.className << "::" << v.name 
		    << " = " << static_cast<short>(v.value) << ";" << endl;
    } else if(v.type=="ushort"){
      this->srcFile << "const unsigned short " << pdata.className << "::" << v.name 
		    << " = " << static_cast<unsigned short>(v.value) << ";" << endl;
    } else if(v.type=="int"){
      this->srcFile << "const int " << pdata.className << "::" << v.name 
		    << " = " << static_cast<int>(v.value) << ";" << endl;
    } else if(v.type=="uint"){
      this->srcFile << "const unsigned int " << pdata.className << "::" << v.name 
		    << " = " << static_cast<unsigned int>(v.value) << ";" << endl;
    } else if(v.type=="long"){
      this->srcFile << "const long " << pdata.className << "::" << v.name 
		    << " = " << static_cast<long>(v.value) << ";" << endl;
    } else if(v.type=="ulong"){
      this->srcFile << "const unsigned long " << pdata.className << "::" << v.name 
		    << " = " << static_cast<unsigned long>(v.value) << ";" << endl;
    } else if(v.type=="float"){
      this->srcFile << "const float " << pdata.className << "::" << v.name 
		    << " = " << static_cast<float>(v.value) << ";" << endl;
    } else if(v.type=="double"){
      this->srcFile << "const double " << pdata.className << "::" << v.name 
		    << " = " << static_cast<double>(v.value) << ";" << endl;
    } else if(v.type=="ldouble"){
      this->srcFile << "const long double " << pdata.className << "::" << v.name 
		    << " = " << static_cast<long double>(v.value) << ";" << endl;
    } else if(v.type=="real"){
      this->srcFile << "const pleiades::field::real " << pdata.className << "::" << v.name 
		    << " = " << static_cast<double>(v.value) << ";" << endl;
    } else {
      string msg("MFrontCelaenoModelInterface::writeStaticVariableInitialisation : ");
      msg += "type " + v.type + " is not a supported type for a static variable.";
      msg += "Supported types are short, ushort, int, uint, long, ulong,";
      msg += "float, double, ldouble and real.\n";
      msg += "Error at line " + toString(v.lineNumber);
      throw(runtime_error(msg));
    }
  } // end of MFrontCelaenoModelInterface::writeStaticVariableInitialisation
    
  void
  MFrontCelaenoModelInterface::writeOutputFiles(const MFrontGenericData& pdata,
						 const MFrontModelData& data)
  {
    using namespace std;
    if(pdata.className.empty()){
      string msg("MFrontCelaenoModelInterface::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->headerFileName  = "include/Pleiades/Model/"+pdata.className;
    this->headerFileName += "-celaeno.hxx";
    this->srcFileName  = "src/"+pdata.className;
    this->srcFileName += "-celaeno.cxx";
    this->openOutputFiles();
    this->generateOutputFiles(pdata,data);
    this->closeOutputFiles();
  } // end of class MFrontCelaenoModelInterface::writeOutputFiles()

  void
  MFrontCelaenoModelInterface::writeAssignDefaultValue(const MFrontGenericData& pdata,
							const VarContainer::const_iterator p,
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
      while(pdata.varNames.find(tmpName)!=pdata.varNames.end()){
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
      string msg("MFrontCelaenoModelInterface::writeAssignDefaultValue : ");
      msg+="type '"+p->type+"' is not supported.\n";
      throw(runtime_error(msg));
    }   
  } // end of MFrontCelaenoModelInterface::writeAssignDefaultValue

  void 
  MFrontCelaenoModelInterface::setVerboseMode()
  {
    this->verboseMode = true;
  } // end of MFrontCelaenoModelInterface::setVerboseMode

  void 
  MFrontCelaenoModelInterface::setDebugMode()
  {
    this->debugMode = true;
  } // end of MFrontCelaenoModelInterface::setDebugMode

  void 
  MFrontCelaenoModelInterface::setWarningMode()
  {
    this->warningMode = true;
  } // end of MFrontCelaenoModelInterface::setWarningMode

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontCelaenoModelInterface::treatKeyword(const std::string&,
					     tfel::utilities::CxxTokenizer::TokensContainer::const_iterator c,
					     const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef CxxTokenizer::TokensContainer::const_iterator TokenConstIterator;
    return pair<bool,TokenConstIterator>(false,c);
  } // end of MFrontCelaenoModelInterface::treatKeyword

  void
  MFrontCelaenoModelInterface::reset(void)
  {
    this->headerFileName.clear();
    this->srcFileName.clear();
    this->hasDefaultConstructor     = false;
    this->verboseMode = false;
    this->debugMode   = false;
    this->warningMode = false;
  } // end of MFrontCelaenoModelInterface::reset

  std::string
  MFrontCelaenoModelInterface::getName(void)
  {
    return "celaeno";
  } // end of MFrontCelaenoModelInterface::getName(void)

  std::map<std::string,std::vector<std::string> >
  MFrontCelaenoModelInterface::getGeneratedSources(const MFrontGenericData& pdata)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib;
    if(pdata.library.empty()){
      if(!pdata.material.empty()){
	lib = "libCelaeno"+pdata.material+"Models";
      } else {
	lib = "libCelaenoMaterialModels";
      }
    } else {
      lib = pdata.library;
    }
    src[lib].push_back(pdata.className+"-celaeno.cxx");
    return src;
  } // end of MFrontCelaenoModelInterface::getGeneratedSources
  
  std::vector<std::string>
  MFrontCelaenoModelInterface::getGeneratedIncludes(const MFrontGenericData& pdata)
  {
    using namespace std;
    vector<string> inc;
    inc.push_back("Pleiades/Model/"+pdata.className+"-celaeno.hxx");
    return inc;
  } // end of MFrontCelaenoModelInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontCelaenoModelInterface::getGlobalIncludes(const MFrontGenericData& pdata)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib;
    if(pdata.library.empty()){
      if(!pdata.material.empty()){
	lib = "libCelaeno"+pdata.material+"Models";
      } else {
	lib = "libCelaenoMaterialModels";
      }
    } else {
      lib = pdata.library;
    }
    incs[lib].push_back("`pleiades-config --includes`\n");
    return incs;
  } // end of MFrontCelaenoModelInterface::getGlobalIncludes
  
  std::map<std::string,std::vector<std::string> >
  MFrontCelaenoModelInterface::getGlobalDependencies(const MFrontGenericData& pdata)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string lib;
    if(pdata.library.empty()){
      if(!pdata.material.empty()){
	lib = "libCelaeno"+pdata.material+"Models";
      } else {
	lib = "libCelaenoMaterialModels";
      }
    } else {
      lib = pdata.library;
    }
    libs[lib].push_back("`pleiades-config --libs` -lm");
    return libs;
  } // end of MFrontCelaenoModelInterface::getGlobalDependencies
    
  std::map<std::string,std::vector<std::string> >
  MFrontCelaenoModelInterface::getLibrariesDependencies(const MFrontGenericData&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontCelaenoModelInterface::getLibrariesDependencies

} // end of namespace mfront  
