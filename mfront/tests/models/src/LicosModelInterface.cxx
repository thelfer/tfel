/*!
 * \file   mfront/tests/models/src/LicosModelInterface.cxx
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

#include<iostream>

#include<stdexcept>
#include<iterator>
#include<sstream>
#include<algorithm>
#include<cassert>
#include<cctype>

#include"MFront/DSLUtilities.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/ModelDSLCommon.hxx"
#include"MFront/ModelInterfaceProxy.hxx"
#include"MFront/LicosModelInterface.hxx"

static constexpr unsigned short TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY = 8;

namespace mfront{

  static std::string
  getVariableType(const std::string& v){
    if(v=="string"){
      return "std::string";
    }
    return v;
  }

  static std::string
  getLibraryName(const ModelData& mdata)
  {
    using namespace std;
    string a = "@Application@";
    if(mdata.library.empty()){
      if(!mdata.material.empty()){
	return "lib"+a+mdata.material+"Models";
      } else {
	return "lib"+a+"MaterialModels";
      }
    }
    return mdata.library;
  } // end of getLibraryName

  static bool
  isInputVariable(const ModelData& data,
		  const std::string& v)
  {
    return ModelDSLCommon::is(data,data.inputs,v);
  } // end of isInputVariable

  static std::pair<std::string,unsigned short>
  decomposeVariableName(const ModelData& data,
			const std::string& v)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
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
    string msg("ModelDSLCommon::decomposeVariableName : ");
    msg += "no decomposition found  for variable '"+v+"'";
    throw(runtime_error(msg));
    return pair<string,unsigned short>("",0u);
  } // end of MFrontModelInterface::getVariableName(const std::string& v)

  MFrontModelInterface::MFrontModelInterface(void)
  {
    this->reset();
  } // end of MFrontModelInterface::MFrontModelInterface

  void
  MFrontModelInterface::declareReservedNames(std::set<std::string>& v)
  {
    v.insert("first");
    v.insert("saveddata");
    v.insert("msaveddata");
    v.insert("activestate");
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
    v.insert("sapply");
    v.insert("UNField");
    v.insert("Field");
    v.insert("NFieldHolder");
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
    v.insert("save");
    v.insert("restore");
    v.insert("execute");
    v.insert("executeInitialPostProcessingTasks");
    v.insert("executePostProcessingTasks");
    v.insert("getName");
    v.insert("data");
  }

  void
  MFrontModelInterface::openOutputFiles(void)
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
      string msg("MFrontModelInterface::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MFrontModelInterface::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.exceptions(ios::badbit|ios::failbit);
  } // end of MFrontModelInterface::openOutputFiles()

  void
  MFrontModelInterface::closeOutputFiles(void)
  {
    if(this->headerFile.is_open()){
      this->headerFile.close();
    }
    if(this->srcFile.is_open()){
      this->srcFile.close();
    }
  } // end of MFrontModelInterface::closeOutputFiles()
  
  void
  MFrontModelInterface::generateOutputFiles(const FileDescription& fdata,
					    const ModelData& mdata)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    ModelData::FunctionContainer::const_iterator p2;
    bool found;
    this->hasDefaultConstructor=true;
    if(mdata.domains.empty()){
      this->hasDefaultConstructor=false;
    }
    if(!mdata.localParameters.empty()){
      for(p=mdata.localParameters.begin();
	  (p!=mdata.localParameters.end())&&(this->hasDefaultConstructor);++p){
	if(mdata.defaultValues.find(p->name)==mdata.defaultValues.end()){
	  this->hasDefaultConstructor = false;
	}
      }
    }
    // sanity checks
    for(p2=mdata.functions.begin();p2!=mdata.functions.end();++p2){
      if(p2->modifiedVariables.size()==0){
	string msg("MFrontModelInterface::generateOutputFiles : ");
	msg += "function '"+p2->name+"' does not modify any output";
	throw(runtime_error(msg));
      }
    }
    for(p=mdata.outputs.begin();p!=mdata.outputs.end();++p){
      found = false;
      for(p2=mdata.functions.begin();
	  (p2!=mdata.functions.end())&&(!found);++p2){
	if(p2->modifiedVariables.find(p->name)!=p2->modifiedVariables.end()){
	  found = true;
	}
      }
      if(!found){
	string msg("MFrontModelInterface::generateOutputFiles : ");
	msg += "output '"+p->name+"' is not modified by any function";
	throw(runtime_error(msg));
      }
    }
    this->writeHeaderFile(fdata,mdata);
    this->headerFile.close();
    this->writeSrcFile(fdata,mdata);
    this->srcFile.close();
  } // end of MFrontModelInterface::generateOutputFiles(void)

  void
  MFrontModelInterface::writeHeaderFile(const FileDescription& fdata,
					const ModelData& mdata)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    StaticVariableDescriptionContainer::const_iterator p2;
    ModelData::FunctionContainer::const_iterator p3;
    set<string>::iterator p4;
    set<string>::iterator p5;
    set<unsigned short> applyHeaders;
    set<unsigned short>::const_iterator p18;
    unsigned short i;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << mdata.className << " Model.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!fdata.authorName.empty()){
      this->headerFile << "* \\author " << fdata.authorName << endl;
    }
    if(!fdata.date.empty()){
      this->headerFile << "* \\date   " << fdata.date       << endl;
    }
    if(!fdata.description.empty()){
      this->headerFile << fdata.description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _PLEIADES_" 
		     << makeUpperCase(mdata.className)
		     << "_HH\n";
    this->headerFile << "#define _PLEIADES_"
		     << makeUpperCase(mdata.className)
		     << "_HH\n\n";

    this->headerFile << "#include<string>\n\n";
    for(p3=mdata.functions.begin();p3!=mdata.functions.end();++p3){
      if((p3->modifiedVariables.size()==1)&&
	 (p3->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	applyHeaders.insert(static_cast<unsigned short>(p3->usedVariables.size()));
      }
    }
    for(p18=applyHeaders.begin();p18!=applyHeaders.end();++p18){
      if(*p18<=5){
	this->headerFile << "#include\"Pleiades/Field/FieldSApply" << *p18 << ".hxx\"\n";
      }
    }
    this->headerFile << "#include\"Pleiades/Model/ModelBase.hxx\"\n";
    this->headerFile << "#include\"Pleiades/Model/StateVariableDescription.hxx\"\n\n";
    if(!mdata.includes.empty()){
      this->headerFile << mdata.includes << endl << endl;
    }
    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace pleiades\n{\n\n";
    this->headerFile << "namespace model\n{\n\n";
    this->headerFile << "struct MFRONT_SHAREDOBJ " << mdata.className << endl;
    this->headerFile << ": public ModelBase\n";
    this->headerFile << "{\n\n";
    this->headerFile << "//! Default constructor\n";
    this->headerFile << mdata.className
		     << "(pleiades::mesh::SMeshManager&,\n"
		     << "pleiades::field::SFieldManager&,\n"
		     << "pleiades::time::SClock&,\n"
		     << "pleiades::loading::SLoadingManager&,\n"
		     << "pleiades::signal::SSignalManager&);\n\n";
    this->headerFile << "//! Constructor to initialize local parameters\n";
    this->headerFile << mdata.className
		     << "(const std::map<std::string,pleiades::parser::Data>&,\n"
		     << "pleiades::mesh::SMeshManager&,\n"
		     << "pleiades::field::SFieldManager&,\n"
		     << "pleiades::time::SClock&,\n"
		     << "pleiades::loading::SLoadingManager&,\n"
		     << "pleiades::signal::SSignalManager&);\n\n";
    this->headerFile << "std::string\ngetName(void) const;\n\n"
		     << "void\ninitialize(const pleiades::parser::DataManager&);\n\n"
		     << "void\ndeclareRequirements(pleiades::coupling::SRequirementManager&);\n\n"
		     << "void\nresolveRequirements(pleiades::coupling::SRequirementManager&);\n\n"
		     << "void\ninitializeOutput(pleiades::coupling::Requirement&);\n\n"
		     << "void\nexecute(void);\n\n"
		     << "void\nsave(const std::string&,\n"
		     << "pleiades::parser::DataMap&);\n\n"
		     << "void\nrestore(const pleiades::time::ptime,\n"
		     << "const std::string&,\n" 
		     << "const pleiades::parser::DataMap&);\n\n";
    this->headerFile << "void\nexecuteInitialPostProcessingTasks(const bool);\n\n";
    this->headerFile << "void\nexecutePostProcessingTasks(const bool);\n\n";
    this->headerFile << "~" << mdata.className << "();\n\n";
    this->headerFile << "private:\n\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "//! Copy constructor (disabled)\n";
    this->headerFile << mdata.className << "(const " 
		     << mdata.className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << mdata.className << "&\n"
		     << "operator=(const " 
		     << mdata.className << "&);\n\n";
    for(p3=mdata.functions.begin();p3!=mdata.functions.end();++p3){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontModelInterface::writeHeaderFile : ");
	msg+="function " + p3->name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << fdata.fileName << "\"\n";
      }
      this->headerFile << "struct " << p3->name<< "\n";
      if((p3->modifiedVariables.size()==1)&&
	 (p3->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	if(p3->usedVariables.size()<=5){
	  this->headerFile << ": public pleiades::field::ScalarFieldFunctor" << p3->usedVariables.size() << endl;
	}
      }
      this->headerFile << "{\n\n";
      if((p3->modifiedVariables.size()==1)&&
	 (p3->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	if(p3->usedVariables.size()>5){
	  this->headerFile << "typedef pleiades::field::real Result;\n";
	  for(i=1,p4=p3->usedVariables.begin();p4!=p3->usedVariables.end();++i,++p4){
	    this->headerFile << "typedef pleiades::field::real Arg" << i << ";\n";
	  }
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
      this->headerFile << "friend class " << mdata.className <<";\n\n";
      for(p4=p3->globalParameters.begin();p4!=p3->globalParameters.end();++p4){
	p = MFrontModelInterface::findVariableDescription(mdata.globalParameters,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << fdata.fileName << "\"\n";
	}
	this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
      }
      for(p4=p3->localParameters.begin();p4!=p3->localParameters.end();++p4){
	p = MFrontModelInterface::findVariableDescription(mdata.localParameters,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << fdata.fileName << "\"\n";
	}
	this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
      }
      for(p4=p3->constantMaterialProperties.begin();p4!=p3->constantMaterialProperties.end();++p4){
	p = MFrontModelInterface::findVariableDescription(mdata.constantMaterialProperties,*p4);
	if(this->debugMode){
	  this->headerFile << "#line " << p->lineNumber << " \"" 
			   << fdata.fileName << "\"\n";
	}
	this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
      }
      if(p3->useTimeIncrement){
	this->headerFile << "pleiades::field::real dt;\n";
      }
      this->headerFile << "}; // end of struct " << p3->name<< "\n\n";
    }
    if(!mdata.staticVars.empty()){
      for(p2=mdata.staticVars.begin();p2!=mdata.staticVars.end();++p2){
	if(this->debugMode){
	  this->headerFile << "#line " << p2->lineNumber << " \"" 
			   << fdata.fileName << "\"\n";
	}
	this->headerFile << "static constexpr " << getVariableType(p2->type) << " " << p2->name << ";" << endl;
      }
      this->headerFile << endl;
    }
    this->headerFile << "std::vector<std::string> domains;\n";
    for(p=mdata.globalParameters.begin();p!=mdata.globalParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << fdata.fileName << "\"\n";
      }
      this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
    }
    for(p=mdata.localParameters.begin();p!=mdata.localParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << fdata.fileName << "\"\n";
      }
      this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
    }
    this->headerFile << "void\ninitializeOutputsVariablesDepths(void);\n";
    this->headerFile << "void\ninitializeOutputsVariablesInitialValues(const pleiades::parser::DataManager&);\n";
    this->headerFile << "void\ninitializeInputsVariablesDepths(void);";
    if(!mdata.constantMaterialProperties.empty()){
      this->headerFile << "\nvoid\ninitializeConstantMaterialProperties(const pleiades::parser::DataManager&);\n\n";
    } else {
      this->headerFile << "\n\n";
    }
    for(i=0,p3=mdata.functions.begin();p3!=mdata.functions.end();++p3,++i){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontModelInterface::writeHeaderFile : ");
	msg += "function " + p3->name + " does not modify any variable ";
	msg += "(internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << fdata.fileName << "\"\n";
      }
      this->headerFile << p3->name << " functor" << i <<  ";\n";
    }
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned short> > outputsDepths;\n";
    this->headerFile << "std::map<std::string,std::map<std::string,boost::shared_ptr<StateVariableDescription> > > outputsInitialValues;\n";
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned short> > inputsDepths;\n";
    if(!mdata.constantMaterialProperties.empty()){
      this->headerFile << "std::map<std::string,std::map<std::string,pleiades::field::real> > constantMaterialProperties;\n";
    }
    this->headerFile << "}; // end of struct " << mdata.className << endl << endl;
    this->headerFile << "} // end of namespace model\n\n";
    this->headerFile << "} // end of namespace pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << makeUpperCase(mdata.className)
		     << "_HH */\n";
  } // end of MFrontModelInterface::writeHeaderFile(void)

  std::string
  MFrontModelInterface::getGenTypeGetMethod(const std::string& type) const
  {
    using namespace std;
    if(type=="bool"){
      return "get<bool>";
    } else if(type=="real"){
      return "get<real>";
    } else if(type=="double"){
      return "get<double>";
    } else if(type=="string"){
      return "get<string>";
    } else {
      string msg("MFrontModelInterface::getGenTypeGetMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontModelInterface::getGenTypeGetMethod

  std::string
  MFrontModelInterface::getGenTypeIsMethod(const std::string& type) const
  {
    using namespace std;
    if(type=="real"){
      return "is<real>";
    } else if(type=="bool"){
      return "is<bool>";
    } else if(type=="double"){
      return "is<double>";
    } else if(type=="string"){
      return "is<string>";
    } else {
      string msg("MFrontModelInterface::getGenTypeIsMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontModelInterface::isGenTypeMethod

  VariableDescriptionContainer::const_iterator
  MFrontModelInterface::findVariableDescription(const VariableDescriptionContainer& v,
						const std::string& n)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      if(p->name==n){
	return p;
      }
    }
    string msg("MFrontModelInterface::findVariableDescription : ");
    msg += "no variable named '"+n+"' found";
    throw(runtime_error(msg));
    return v.end();
  } // MFrontModelInterface::findVariableDescription

  void
  MFrontModelInterface::writeSrcFile(const FileDescription& fdata,
				     const ModelData& mdata)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    map<string,string>::const_iterator p4;
    map<string,unsigned short>::const_iterator p5;
    StaticVariableDescriptionContainer::const_iterator p10;
    ModelData::FunctionContainer::const_iterator p11;
    set<string>::const_iterator p12;
    std::map<std::string,std::vector<ModelData::Function> >::iterator p13;
    set<string>::const_iterator p15;
    set<string>::const_iterator p16;
    set<unsigned short> applyHeaders;
    set<unsigned short>::const_iterator p18;
    vector<VariableBoundsDescription>::const_iterator p19;
    vector<VariableBoundsDescription>::const_iterator p20;
    unsigned short specializedParametersNumber;
    unsigned short i;
    bool found;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << mdata.className << "  model.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!fdata.authorName.empty()){
      this->srcFile << "* \\author " << fdata.authorName << endl;
    }
    if(!fdata.date.empty()){
      this->srcFile << "* \\date   " << fdata.date       << endl;
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<iostream>\n";
    this->srcFile << "#include<stdexcept>\n";
    this->srcFile << "#include<sstream>\n";
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include\"Pleiades/Global.hxx\"\n";
    this->srcFile << "#include\"Pleiades/OutOfBoundsPolicy.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Parser/Mdata.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Parser/DataManager.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Glossary/Glossary.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Time/SClock.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/MaterialPropertyDescription.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Coupling/Requirement.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Coupling/SRequirementManager.hxx\"\n";

    // Functions
    bool requiresNFieldHolder = false;
    for(p11=mdata.functions.begin();p11!=mdata.functions.end();++p11){
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	applyHeaders.insert(static_cast<unsigned short>(p11->usedVariables.size()));
      } else{
	requiresNFieldHolder = true;
      }
    }
    for(p18=applyHeaders.begin();p18!=applyHeaders.end();++p18){
      if(*p18>5){
	this->srcFile << "#include\"Pleiades/Field/FieldApply" << *p18 << ".hxx\"\n";
      }
    }
    if(requiresNFieldHolder){
      this->srcFile << "#include\"Pleiades/Field/VNField.hxx\"\n";
      this->srcFile << "#include\"Pleiades/Field/NFieldHolder.hxx\"\n";
    }
    this->srcFile << "#include\"Pleiades/Model/MaterialDescription.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/UniformScalarStateVariableDescription.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/IModelFactory.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/" 
		  << mdata.className << "-@application@.hxx\"\n\n";
    this->srcFile << "namespace pleiades\n{\n\n";
    this->srcFile << "namespace model\n{\n\n";
    // Functors
    for(p11=mdata.functions.begin();p11!=mdata.functions.end();++p11){
      // operator()
      if(this->debugMode){
	this->srcFile << "#line " << p11->line << " \"" 
		      << fdata.fileName << "\"\n";
      }
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	this->srcFile << "pleiades::field::real\n";
	this->srcFile << mdata.className << "::" << p11->name << "::operator()(";
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
	this->srcFile << mdata.className << "::" << p11->name << "::operator()(";
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
      writeMaterialLaws("MFrontModelInterface::writeSrcFile",
			this->srcFile,mdata.materialLaws);		      
      for(i=0,p12=p11->usedVariables.begin();p12!=p11->usedVariables.end();++p12,++i){
	found = false;
	for(p19=mdata.physicalBoundsDescriptions.begin();
	    (p19!=mdata.physicalBoundsDescriptions.end())&&(!found);){
	  found = (p19->varName==*p12);
	  if(!found){
	    ++p19;
	  }
	}
	found = false;
	for(p20=mdata.boundsDescriptions.begin();
	    (p20!=mdata.boundsDescriptions.end())&&(!found);){
	  found = (p20->varName==*p12);
	  if(!found){
	    ++p20;
	  }
	}
	if((p19!=mdata.physicalBoundsDescriptions.end())||
	   (p20!=mdata.boundsDescriptions.end())){
	  this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
	}
	if(p19!=mdata.physicalBoundsDescriptions.end()){
	  this->srcFile << "// checking " << p19->varName<< " physical bounds\n";
	  if((p19->boundsType==VariableBoundsDescription::Lower)||
	     (p19->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p19->varName<< " < "<< p19->lowerBound << "){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << mdata.className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p19->varName << "' is below its physical lower bound (\" << "
			  << p19->varName << " << \"<" << p19->lowerBound << ").\\n\";\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n";
	    this->srcFile << "}\n";
	  }
	  if((p19->boundsType==VariableBoundsDescription::Upper)||
	     (p19->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p19->varName << " > " << p19->upperBound << "){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << mdata.className << "::" << p11->name << "::operator() : \"\n"
			  << "<< \"input '" << p19->varName << "' is over its physical upper bound (\" << "
			  << p19->varName << " << \" > " << p19->upperBound << ").\\n\";\n";
	    this->srcFile << "throw(runtime_error(msg.str()));\n",
	      this->srcFile << "}\n";
	  }
	}
	if(p20!=mdata.boundsDescriptions.end()){
	  this->srcFile << "// checking " << p20->varName<< " bounds\n";
	  if((p20->boundsType==VariableBoundsDescription::Lower)||
	     (p20->boundsType==VariableBoundsDescription::LowerAndUpper)){
	    this->srcFile << "if(" << p20->varName<< " < "<< p20->lowerBound << "){\n";
	    this->srcFile << "if(pleiades::getOutOfBoundsPolicy()!= pleiades::NO_OUT_OF_BOUNDS_POLICY){\n";
	    this->srcFile << "ostringstream msg;\n"
			  << "msg << \"" << mdata.className << "::" << p11->name << "::operator() : \"\n"
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
			  << "msg << \"" << mdata.className << "::" << p11->name << "::operator() : \"\n"
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
	if((p19!=mdata.physicalBoundsDescriptions.end())||
	   (p20!=mdata.boundsDescriptions.end())){
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
      this->srcFile << "} // end of " << mdata.className << "::" << p11->name << "::operator()\n\n";
    }
    // static variables
    if(!mdata.staticVars.empty()){
      for(p10=mdata.staticVars.begin();p10!=mdata.staticVars.end();++p10){
	this->writeStaticVariableInitialization(fdata,mdata,*p10);
      }
      this->srcFile << endl;
    }
    // Constructors
    if(this->hasDefaultConstructor){
      this->srcFile << mdata.className << "::" 
		    << mdata.className
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
      for(p=mdata.localParameters.begin();p!=mdata.localParameters.end();++p){
	string name;
	name = this->getVariableName(p->name,mdata);
	p4=mdata.defaultValues.find(p->name);
	assert(p4!=mdata.defaultValues.end());
	this->writeAssignDefaultValue(p,p4);
      }
      if(mdata.domains.empty()){
	string msg("MFrontModelInterface::writeSrcFile : ");
	msg += "no domain defined (internal error, we shall not reach this point)";
	throw(runtime_error(msg));
      }
      for(p15=mdata.domains.begin();p15!=mdata.domains.end();++p15){
	this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	this->srcFile << "for(ptr=tmp.begin();ptr!=tmp.end();++ptr){\n";
	this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr)!=this->domains.end()){\n";
	this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
	this->srcFile << "msg += \"domain '\"+*ptr+\"' multiply defined\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->domains.push_back(*ptr);\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "this->initializeOutputsVariablesDepths();\n";
      this->srcFile << "this->initializeInputsVariablesDepths();\n";
      this->srcFile << "} // end of " 
		    << mdata.className << "::" 
		    << mdata.className <<"\n\n";
    } else {
      this->srcFile << mdata.className << "::" 
		    << mdata.className
		    << "(pleiades::mesh::SMeshManager& m,\n"
		    << "pleiades::field::SFieldManager& mf,\n"
		    << "pleiades::time::SClock& c,\n"
		    << "pleiades::loading::SLoadingManager& l,\n"
		    << "pleiades::signal::SSignalManager& s)\n"
		    << ": ModelBase(m,mf,c,l,s)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
      this->srcFile << "msg += \"no domain defined\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "} // end of " 
		    << mdata.className << "::" 
		    << mdata.className <<"\n\n";
    }
    this->srcFile << "//! Constructor to initialize local parameters\n";
    this->srcFile << mdata.className << "::" 
		  << mdata.className 
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
    this->srcFile << "using namespace pleiades::coupling;\n";
    this->srcFile << "static constexpr string ValidParametersNames[] = {";
    this->srcFile << "\"domain\",\"domains\",\"Active\",\n"
		  << "\"ActivatingEvents\",\"DesactivatingEvents\"";
    specializedParametersNumber=5u;
    for(p=mdata.localParameters.begin();p!=mdata.localParameters.end();++p){
      string name;
      name = this->getVariableName(p->name,mdata);
      this->srcFile << ",\n";
      this->srcFile << name;
      ++(specializedParametersNumber);
    }
    // domain specialisation
    this->srcFile << "};\n";
    this->srcFile << "vector<string> tmp;\n";
    this->srcFile << "map<string,Data>::const_iterator ptr;\n";
    this->srcFile << "vector<string>::const_iterator ptr2;\n";
    this->srcFile << "vector<string>::const_iterator ptr3;\n";
    this->srcFile << "for(ptr=mdata.begin();ptr!=mdata.end();++ptr){\n";
    this->srcFile << "if(find(ValidParametersNames,ValidParametersNames+" 
		  << specializedParametersNumber
		  << ",ptr->first)==ValidParametersNames+"
		  << specializedParametersNumber << "){\n";
    this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
    this->srcFile << "msg += \"unknown parameter '\"+ptr->first+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "ActivableObjectBase::handleSpecialisationData(data);\n";
    for(p=mdata.localParameters.begin();p!=mdata.localParameters.end();++p){
      string name;
      name = this->getVariableName(p->name,mdata);
      this->srcFile << "ptr = mdata.find(" << name << ");\n";
      this->srcFile << "if(ptr==mdata.end()){\n";
      if((p4=mdata.defaultValues.find(p->name))!=mdata.defaultValues.end()){
	this->writeAssignDefaultValue(p,p4);
	this->srcFile << "} else {\n";
      } else {
	this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
	this->srcFile << "msg += \"can't initialize parameter " << p->name << "\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "} else {\n";
      }
      this->srcFile << "if(!ptr->second." << this->getGenTypeIsMethod(p->type) << "()){\n";
      this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
      this->srcFile << "msg += \"wrong type for parameter '" << p->name << "' (expected a '"+p->type+"')\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "this->" << p->name << " = ptr->second." 
		    << this->getGenTypeGetMethod(p->type) << "();\n";
      this->srcFile << "}\n";
    }
    this->srcFile << "if((mdata.find(\"domain\")!=mdata.end())||(mdata.find(\"domains\")!=mdata.end())){\n";
    this->srcFile << "if(mdata.find(\"domain\")!=mdata.end()){\n";
    this->srcFile << "ptr = mdata.find(\"domain\");\n";
    this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,ptr->second.get<string>());\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
    this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << ": \");\n";
    this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "this->domains.push_back(*ptr2);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "if(mdata.find(\"domains\")!=mdata.end()){\n";
    this->srcFile << "ptr = mdata.find(\"domains\");\n";
    this->srcFile << "if(ptr!=mdata.end()){\n";
    this->srcFile << "if(!ptr->second.is<vector<string> >()){\n";
    this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
    this->srcFile << "msg += \"invalid type for parameter 'domains'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "for(ptr3=ptr->second.get<vector<string> >().begin();\n";
    this->srcFile << "ptr3!=ptr->second.get<vector<string> >().end();++ptr3){\n";
    this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,*ptr3);\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
    this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << ": \");\n";
    this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "this->domains.push_back(*ptr2);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "} else {\n";
    if(mdata.domains.empty()){
      this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << ": \");\n";
      this->srcFile << "msg += \"no domain defined\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
    } else {
      for(p15=mdata.domains.begin();p15!=mdata.domains.end();++p15){
	this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << *p15 << "\");\n";
	this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
	this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!=this->domains.end()){\n";
	this->srcFile << "string msg(\"" << mdata.className << "::" 
		      << mdata.className << " : \");\n";
	this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->domains.push_back(*ptr2);\n";
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "}\n";
    this->srcFile << "this->initializeOutputsVariablesDepths();\n";
    this->srcFile << "this->initializeInputsVariablesDepths();\n";
    this->srcFile <<"} // end of "
		  << mdata.className << "::" 
		  << mdata.className <<"\n\n";
    this->writeInitializeOutputsVariablesDepths(mdata);
    this->writeInitializeInputsVariablesDepths(mdata);
    // getName
    this->srcFile << "std::string\n"
		  << mdata.className
		  << "::getName(void) const\n"
		  << "{\n"
		  << "return \"" << mdata.className << "\";\n"
		  << "}\n\n";
    // initialize
    this->srcFile << "void\n"
		  << mdata.className
		  << "::initialize(const pleiades::parser::DataManager& data)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    for(p=mdata.globalParameters.begin();p!=mdata.globalParameters.end();++p){
      string name = this->getVariableName(p->name,mdata);
      this->srcFile << "if(!mdata.contains(" << name << ")){\n";
      if((p4=mdata.defaultValues.find(p->name))!=mdata.defaultValues.end()){
	this->srcFile << "this->" << p->name << " = " << p4->second << ";" << endl;
	this->srcFile << "} else {\n";
      } else {
	this->srcFile << "string msg(\"" << mdata.className << "::initialize : \");\n";
	this->srcFile << "msg += \"can't initialize parameter '" << p->name << "'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "} else {\n";
      }
      this->srcFile << "if(!mdata." << this->getGenTypeIsMethod(p->type) << "(" << name << ")){\n";
      this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
      this->srcFile << "msg += \"wrong type for parameter '" << p->name << "' (expected a '"+p->type+"')\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "this->" << p->name << " = mdata." 
		    << this->getGenTypeGetMethod(p->type) << "(" << name << ");\n";
      this->srcFile << "}\n";
    }
    this->srcFile << "this->initializeOutputsVariablesInitialValues(data);\n";
    if(!mdata.constantMaterialProperties.empty()){
      this->srcFile << "this->initializeConstantMaterialProperties(data);\n";
    }
    // initializing functor members
    for(i=0,p11=mdata.functions.begin();p11!=mdata.functions.end();++p11,++i){
      string functor = "functor"+toString(i);
      for(p15=p11->localParameters.begin();
	  p15!=p11->localParameters.end();++p15){
	this->srcFile << "this->" << functor << '.' << *p15 << " = this->" << *p15 << ";\n";
      }
      for(p15=p11->globalParameters.begin();p15!=p11->globalParameters.end();++p15){
	this->srcFile << "this->" << functor << '.' << *p15 << " = this->" << *p15 << ";\n";
      }
    }
    this->srcFile << "} // end of " << mdata.className << "::initialize\n\n";
    this->writeInitializeOutputsVariablesInitialValues(mdata);
    if(!mdata.constantMaterialProperties.empty()){
      this->writeInitializeConstantMaterialProperties(mdata);
    }
    // declareRequirements
    this->srcFile << "void\n";
    this->srcFile << mdata.className << "::declareRequirements(pleiades::coupling::SRequirementManager& requirementManager)\n";
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
    this->srcFile << "} // end of " << mdata.className << "::declareRequirements\n\n";
    // resolveDependencies
    this->srcFile << "void\n";
    this->srcFile << mdata.className << "::resolveRequirements(pleiades::coupling::SRequirementManager& requirementManager)\n";
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
    this->srcFile << "} // end of " << mdata.className << "::resolveRequirements\n\n";
    // initializeOutput
    this->srcFile << "void\n";
    this->srcFile << mdata.className << "::initializeOutput(pleiades::coupling::Requirement& requirement)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace boost;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "map<string,map<string,unsigned short> >::const_iterator ptr;\n";
    this->srcFile << "map<string,unsigned short>::const_iterator ptr2;\n";
    this->srcFile << "map<string,map<string,shared_ptr<StateVariableDescription> > >::const_iterator ptr3;\n";
    this->srcFile << "map<string,shared_ptr<StateVariableDescription> >::const_iterator ptr4;\n";
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
    this->srcFile << "string msg(\"" << mdata.className << "::initializeOutput: \");\n";
    this->srcFile << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr4 = ptr3->second.find(requirement.getLocation());\n";
    this->srcFile << "if(ptr4==ptr3->second.end()){\n";
    this->srcFile << "string msg(\"" << mdata.className << "::initializeOutput: \");\n";
    this->srcFile << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr4->second->initialize(this->smanager.getMTFieldManager(requirement.getLocation()),\n"
		  << "requirement,ptr2->second);\n";
    this->srcFile << "} // end of " << mdata.className << "::initializeOutput\n\n";
    // save
    this->srcFile << "void\n"
		  << mdata.className
		  << "::save(const std::string& directory,\npleiades::parser::DataMap& saveddata)\n"
		  << "{\n"
		  << "using namespace std;\n"
		  << "using namespace pleiades::parser;\n"
		  << "typedef map<string,string>::value_type MVType;\n"
		  << "vector<string>::const_iterator ptr;\n"
		  << "ModelBase::save(directory,saveddata);\n"
		  << "DataMap& msaveddata = savedmdata.get<DataMap>(this->getName(),true);\n"
		  << "map<string,string>& activestate = msavedmdata.get<map<string,string> >(\"ActiveState\",true);\n"
		  << "if(this->isActive()){\n" 
		  << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n"
		  << "activestate.insert(MVType(*ptr,\"true\"));\n"
		  << "}\n"
		  << "} else {\n"
		  << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n"
		  << "activestate.insert(MVType(*ptr,\"false\"));\n"
		  << "}\n"
		  << "}\n"
		  << "}\n\n";
    this->srcFile << "void\n"
		  << mdata.className
		  << "::restore(const pleiades::time::ptime t,\n"
		  << "const std::string& directory,\n" 
		  << "const pleiades::parser::DataMap&saveddata)\n"
		  << "{\n"
		  << "using namespace std;\n"
		  << "using namespace pleiades::parser;\n"
		  << "vector<string>::const_iterator ptr;\n"
		  << "map<string,string>::const_iterator ptr2;\n"
		  << "ModelBase::restore(t,directory,saveddata);\n"
		  <<  "if(!savedmdata.contains(this->getName())){\n"
		  << "return;"
		  << "}\n"
		  << "const DataMap& msaveddata = savedmdata.get<DataMap>(this->getName());\n"
		  << "if(!msavedmdata.contains(\"ActiveState\")){\n"
		  << "return;\n"
		  << "}\n"
		  << "bool first     = true;\n"
		  << "bool dactivate = true;\n"
		  << "const map<string,string>& activestate = msavedmdata.get<map<string,string> >(\"ActiveState\");\n"
		  << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n"
		  << "ptr2 = activestate.find(*ptr);\n"
		  << "if(ptr2!=activestate.end()){\n"
		  << "if(ptr2->second==\"true\"){\n"
		  <<  "dactivate = true;"
		  << "} else {\n"
      		  <<  "dactivate = false;"
		  << "}\n"
		  << "} else {\n"
		  <<  "dactivate = true;"
		  << "}\n"
		  << "if(first){\n"
		  << "this->active = dactivate;"
      		  << "} else {\n"
		  << "if(this->active != dactivate){"
		  << "string msg(\"" << mdata.className << "::restore : \");\n"
		  << "msg += \"incoherent activation state\";\n"
		  << "throw(runtime_error(msg));\n"
		  << "}"
		  << "}\n"
		  << "first = false;"
		  << "}\n"
		  << "}\n\n";
    // execute
    this->srcFile << "void\n"
		  << mdata.className
		  << "::execute(void){\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace boost;\n";
    this->srcFile << "using namespace pleiades;\n";
    this->srcFile << "using namespace pleiades::time;\n";
    this->srcFile << "using namespace pleiades::mesh;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    this->srcFile << "this->computeMaterialProperties();\n";
    // do we need time increment ?
    found = false;
    for(p11=mdata.functions.begin();(p11!=mdata.functions.end())&&(!found);++p11){
      if(p11->useTimeIncrement){
	this->srcFile << "const pleiades::field::real dt = convert_to_double(this->sclock.getCurrentTimeIncrement());\n";
	found = true;
      }
    }
    for(i=0,p11=mdata.functions.begin();p11!=mdata.functions.end();++p11,++i){
      string functor = "functor"+toString(i);
      if(p11->useTimeIncrement){
	this->srcFile << "this->" << functor << ".dt=dt;\n";
      }
      this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
      this->srcFile << "MTFieldManager& mm = this->smanager.getMTFieldManager(*ptr);\n";
      for(p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();++p15){
	pair<string,unsigned short> dv = decomposeVariableName(mdata,*p15);
	this->srcFile << "const NFieldHolder& " << "f_" << *p15 
		      << " = mm.getNFieldHolder(" << this->getVariableName(dv.first,mdata)
		      << "," << dv.second << ");\n";
      }
      for(p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15){
	this->srcFile << "NFieldHolder& " << "f_" << *p15 
		      << " = mm.getNFieldHolder(" << this->getVariableName(*p15,mdata) << ");\n";
      }
      for(p15=p11->constantMaterialProperties.begin();
	  p15!=p11->constantMaterialProperties.end();++p15){
	this->srcFile << "this->" << functor << "." << *p15 
		      << " = this->constantMaterialProperties[*ptr][\"" << *p15 << "\"];\n";
      }
      this->srcFile << "if(getVerboseMode()==VERBOSE_LEVEL1){\n";
      this->srcFile << "ostream& log = getLogStream();\n";
      this->srcFile << "log << \"**" << mdata.className << "::execute : \"" << "\n";
      this->srcFile << "<< \"executing function '" << p11->name << "' on domain '\" << *ptr << \"'\" << endl;\n";
      this->srcFile << "}\n";
      if((p11->modifiedVariables.size()==1)&&
	 (p11->usedVariables.size()<TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)){
	string output = *(p11->modifiedVariables.begin());
	if(p11->usedVariables.size()<=5){
	  this->srcFile << "sapply(this->" << functor << ",f_" << output;
	} else {
	  this->srcFile << "apply(this->" << functor << ",f_" << output;
	}
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
	  this->srcFile << "shared_ptr<Support> support = mm.getSupportPtr();\n";
	  this->srcFile << "if(";
	  for(p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();){
	    this->srcFile << "(f_" << *p15 << ".is<VNField<real> >())";
	    if(++p15!=p11->usedVariables.end()){
	      this->srcFile << "||\n";
	    }
	  }
	  this->srcFile << "){\n";
	  // a least one input is a field
	  this->srcFile << "map<unsigned short,const VNField<real> *> fields;\n";
	  this->srcFile << "VNField<real>::size_type ptr2;\n";
	  this->srcFile << "map<unsigned short,const VNField<real> *>::const_iterator ptr3;\n";
	  // converting the output
	  for(p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15){
	    this->srcFile << "if(!f_" << *p15<< ".is<VNField<real> >()){\n";
	    this->srcFile << "f_" << *p15 << " = NFieldHolder(shared_ptr<VNField<real> >(new VNField<real>(support,"
			  << this->getVariableName(*p15,mdata) <<",f_"
			  << *p15 << ".get<real>())));\n";
	    this->srcFile << "}\n";
	  }
	  // getting output field
	  for(p15=p11->modifiedVariables.begin();p15!=p11->modifiedVariables.end();++p15){
	    this->srcFile << "VNField<real>& ff_" << *p15 << " = f_" << *p15 << ".get<VNField<real> >();\n";  
	  }
	  for(i=0,p15=p11->usedVariables.begin();p15!=p11->usedVariables.end();++p15,++i){
	    this->srcFile << "if(f_" << *p15<< ".is<VNField<real> >()){\n";
	    this->srcFile << "fields[" << i << "] = &(f_" << *p15<< ".get<VNField<real> >());\n";
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
	    this->srcFile << "f_" << *p15 << " = NFieldHolder(UNField<real>(support,"
			  << this->getVariableName(*p15,mdata) << ",results[" << i <<"]));\n";
	  }
	  this->srcFile << "}\n";
	} else {
	  // no input variables
	  string msg("MFrontModelInterface : untreated case");
	  throw(runtime_error(msg));
	}
      }
      this->srcFile << "}\n";
    }
    this->srcFile << "} // end of " << mdata.className << "::execute\n\n";
    this->srcFile << "void\n" << mdata.className 
		  << "::executeInitialPostProcessingTasks(const bool)\n{} // end of " << mdata.className 
		  << "::executeInitialPostProcessingTasks\n\n";
    
    this->srcFile << "void\n" << mdata.className 
		  << "::executePostProcessingTasks(const bool)\n{} // end of " << mdata.className 
		  << "::executePostProcessingTasks\n\n";
    
    this->srcFile << mdata.className << "::~" << mdata.className << "()\n{}\n\n";
    
    this->srcFile << "PLEIADES_GENERATE_PROXY4(IModel," 
		  << mdata.className << "," 
		  << "\"1.2-svn:6534\","
		  << "\""<< getLibraryName(mdata) <<"\""
		  << ");\n\n";
    this->srcFile << "} // end of namespace model\n\n";
    this->srcFile << "} // end of namespace pleiades\n";
  } // end of MFrontModelInterface::writeSrcFile(void)
    
  void
  MFrontModelInterface::writeInitializeOutputsVariablesDepths(const ModelData& mdata)
  {
    using namespace std;
    ModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,unsigned short>::const_iterator p3;
    map<string,double>::const_iterator p4;
    unsigned short i;
    this->srcFile << "void\n" << mdata.className << "::initializeOutputsVariablesDepths(void)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=mdata.functions.begin();p!=mdata.functions.end();++p,++i){
      for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();++p2){
	string functor = "functor"+toString(i);
	p3 = p->depths.find(*p2);
	if(p3==p->depths.end()){
	  string msg("MFrontModelInterface::writeInitializeOutputsVariablesDepths : ");
	  msg += "internal error, no depth found for variable '"+*p2+"' in function '"+p->name+"'";
	  throw(runtime_error(msg));
	}
	this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	this->srcFile << "if(!this->outputsDepths[" << this->getVariableName(*p2,mdata)
		      << "].insert(make_pair(*ptr,";
	this->srcFile << p3->second;
	this->srcFile << ")).second){\n";
	this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
	this->srcFile << "msg += \"output '\";\n";
	this->srcFile << "msg += " << this->getVariableName(*p2,mdata) << ";\n";
	this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << mdata.className << "::initializeOutputsVariablesDepths\n\n";
  } // end of MFrontModelInterface::writeInitializeOutputsVariablesDepths()

  void
  MFrontModelInterface::writeInitializeConstantMaterialProperties(const ModelData& mdata)
  {
    using namespace std;
    ModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,string>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << mdata.className
		  << "::initializeConstantMaterialProperties(" 
		  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "typedef ConstantMaterialPropertyDescription CMPD;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=mdata.functions.begin();p!=mdata.functions.end();++p,++i){
      for(p2=p->constantMaterialProperties.begin();p2!=p->constantMaterialProperties.end();++p2){
	string functor = "functor"+toString(i);
	this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	// getting material description
	this->srcFile << "if(!mdata.hasMaterialDescription(*ptr)){\n";
	this->srcFile << "string msg(\"" << mdata.className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"no material description  on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const MaterialDescription& md = *(mdata.getMaterialDescription(*ptr));\n";
	this->srcFile << "if(md.containsMaterialProperty(" << this->getVariableName(*p2,mdata) << ")){\n";
	this->srcFile << "if(!md.isMaterialPropertyAccesible(" << this->getVariableName(*p2,mdata) << ")){\n";
	this->srcFile << "string msg(\"" << mdata.className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"material property '\";\n";
	this->srcFile << "msg += " <<  this->getVariableName(*p2,mdata) << ";\n";
	this->srcFile << "msg += \"' is not accessible on material '\"+*ptr+\"'\";\n";
	this->srcFile << "msg += \"' (this means that this property is define within a behaviour)\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "const MaterialPropertyDescription& mpd = *(md.getMaterialProperty(" 
		      << this->getVariableName(*p2,mdata) << "));\n";
	this->srcFile << "if(!mpd.is<CMPD>()){\n";
	this->srcFile << "string msg(\"" << mdata.className << "::initializeConstantMaterialProperties : \");\n";
	this->srcFile << "msg += \"material property '\";\n";
	this->srcFile << "msg += " <<  this->getVariableName(*p2,mdata) << ";\n";
	this->srcFile << "msg += \"' is not constant on material '\"+*ptr+\"'\";\n";
	this->srcFile << "throw(runtime_error(msg));\n";
	this->srcFile << "}\n";
	this->srcFile << "this->constantMaterialProperties[*ptr][\"" << *p2 << "\"]" 
		      << " = mpd.get<CMPD>().getValue();\n";
	this->srcFile << "} else {\n";
	if((p3=mdata.defaultValues.find(*p2))!=mdata.defaultValues.end()){
	  this->srcFile << "this->constantMaterialProperties[*ptr][\"" << *p2 
			<< "\"] = " << p3->second << ";\n";	  
	} else {
	  this->srcFile << "string msg(\"" << mdata.className << "::initializeConstantMaterialProperties : \");\n";
	  this->srcFile << "msg += \"material property '\";\n";
	  this->srcFile << "msg += " <<  this->getVariableName(*p2,mdata) << ";\n";
	  this->srcFile << "msg += \"' is undefined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	}
	this->srcFile << "}\n";
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << mdata.className << "::initializeConstantMaterialProperties\n\n";
  } // end of MFrontModelInterface::writeInitializeConstantMaterialProperties(void)

  void
  MFrontModelInterface::writeInitializeOutputsVariablesInitialValues(const ModelData& mdata)
  {
    using namespace std;
    ModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << mdata.className
		  << "::initializeOutputsVariablesInitialValues(" 
		  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace boost;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "typedef UniformScalarStateVariableDescription USSVD;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for(i=0,p=mdata.functions.begin();p!=mdata.functions.end();++p,++i){
      for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();++p2){
	string functor = "functor"+toString(i);
	p3=mdata.initialValues.find(*p2);
	this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	// getting material description
	if(p3!=mdata.initialValues.end()){
	  this->srcFile << "if(mdata.hasMaterialDescription(*ptr)){\n";
	  this->srcFile << "const MaterialDescription& md = *(mdata.getMaterialDescription(*ptr));\n";
	  this->srcFile << "if(md.containsStateVariable(" <<  this->getVariableName(*p2,mdata) << ")){\n";
	  this->srcFile << "shared_ptr<StateVariableDescription> tmp = md.getStateVariable("<<  this->getVariableName(*p2,mdata) << ");\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getVariableName(*p2,mdata)
			<< "].insert(make_pair(*ptr,tmp)).second){\n";
	  this->srcFile << "string msg(\"" << mdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getVariableName(*p2,mdata) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getVariableName(*p2,mdata)
			<< "].insert(make_pair(*ptr,new USSVD("
			<< this->getVariableName(*p2,mdata)
			<< ","
			<< p3->second;
	  this->srcFile << "))).second){\n";
	  this->srcFile << "string msg(\"" << mdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getVariableName(*p2,mdata) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getVariableName(*p2,mdata)
			<< "].insert(make_pair(*ptr,new USSVD("
			<< this->getVariableName(*p2,mdata)
			<< ","
			<< p3->second;
	  this->srcFile << "))).second){\n";
	  this->srcFile << "string msg(\"" << mdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " <<  this->getVariableName(*p2,mdata) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	} else {
	  this->srcFile << "if(!mdata.hasMaterialDescription(*ptr)){\n";
	  this->srcFile << "string msg(\"" << mdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"no material description  on material '\"+*ptr+\"', \";\n";
	  this->srcFile << "msg += \"required to initialize output value '\";\n";
	  this->srcFile << "msg += " << this->getVariableName(*p2,mdata) << ";\n";
	  this->srcFile << "msg += '\\\'';\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "const MaterialDescription& md = *(mdata.getMaterialDescription(*ptr));\n";
	  this->srcFile << "if(md.containsStateVariable(" <<  this->getVariableName(*p2,mdata) << ")){\n";
	  this->srcFile << "shared_ptr<StateVariableDescription> tmp = md.getStateVariable("<<  this->getVariableName(*p2,mdata) << ");\n";
	  this->srcFile << "if(!this->outputsInitialValues[" << this->getVariableName(*p2,mdata)
			<< "].insert(make_pair(*ptr,tmp)).second){\n";
	  this->srcFile << "string msg(\"" << mdata.className << "::initializeOutputsVariablesInitialValues : \");\n";
	  this->srcFile << "msg += \"output '\";\n";
	  this->srcFile << "msg += " << this->getVariableName(*p2,mdata) << ";\n";
	  this->srcFile << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
	  this->srcFile << "throw(runtime_error(msg));\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	}
	this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << mdata.className << "::initializeOutputsVariablesInitialValues\n\n";
  } // end of MFrontModelInterface::writeInitializeOutputsVariablesInitialValues()

  void
  MFrontModelInterface::writeInitializeInputsVariablesDepths(const ModelData& mdata)
  {
    using namespace std;
    set<string> treatedVars;
    ModelData::FunctionContainer::const_iterator p;
    set<string>::const_iterator p2;
    map<string,unsigned short>::const_iterator p3;
    unsigned short i;
    this->srcFile << "void\n" << mdata.className << "::initializeInputsVariablesDepths(void)\n{\n";
    for(i=0,p=mdata.functions.begin();p!=mdata.functions.end();++p){
      for(p2=p->usedVariables.begin();p2!=p->usedVariables.end();++p2){
	if(isInputVariable(mdata,*p2)){
	  ++i;
	}
      }
    }
    if(i!=0){
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace pleiades::glossary;\n";
      this->srcFile << "vector<string>::const_iterator ptr;\n";
      for(i=0,p=mdata.functions.begin();p!=mdata.functions.end();++p,++i){
	for(p2=p->usedVariables.begin();p2!=p->usedVariables.end();++p2){
	  if(isInputVariable(mdata,*p2)){
	    const string& v = decomposeVariableName(mdata,*p2).first;
	    if(treatedVars.find(v)==treatedVars.end()){
	      string functor = "functor"+toString(i);
	      p3 = p->depths.find(v);
	      if(p3==p->depths.end()){
		string msg("MFrontModelInterface::writeInitializeInputsVariablesDepths : ");
		msg += "internal error, no depth found for variable '"+v+"' in function '"+p->name+"'";
		throw(runtime_error(msg));
	      }
	      this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
	      this->srcFile << "map<string,unsigned short>& tmp = this->inputsDepths[" << this->getVariableName(v,mdata) << "];\n";
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
    }
    this->srcFile << "} // end of " << mdata.className << "::initializeInputsVariablesDepths\n\n";
  } // end of MFrontModelInterface::writeInitializeInputsVariablesDepths()

  std::string
  MFrontModelInterface::getVariableName(const std::string& v,
					const ModelData& mdata) const
  {
    using namespace std;
    map<string,string>::const_iterator p;
    if((p=mdata.glossaryNames.find(v))!=mdata.glossaryNames.end()){
      return "Glossary::" + p->second;
    } else if((p=mdata.entryNames.find(v))!=mdata.entryNames.end()){
      return "\""+p->second+"\"";
    }
    return "\""+v+"\"";
  } // end of MFrontModelInterface::getVariableName(const std::string& v)

  void
  MFrontModelInterface::writeStaticVariableInitialization(const FileDescription& fdata,
							  const ModelData& mdata,
							  const StaticVariableDescription& v)
  {
    using namespace std;
    if(this->debugMode){
      this->srcFile << "#line " << v.lineNumber << " \"" 
		    << fdata.fileName << "\"\n";
    }
    if(v.type=="short"){
      this->srcFile << "const short " << mdata.className << "::" << v.name 
		    << " = " << static_cast<short>(v.value) << ";" << endl;
    } else if(v.type=="ushort"){
      this->srcFile << "const unsigned short " << mdata.className << "::" << v.name 
		    << " = " << static_cast<unsigned short>(v.value) << ";" << endl;
    } else if(v.type=="int"){
      this->srcFile << "const int " << mdata.className << "::" << v.name 
		    << " = " << static_cast<int>(v.value) << ";" << endl;
    } else if(v.type=="uint"){
      this->srcFile << "const unsigned int " << mdata.className << "::" << v.name 
		    << " = " << static_cast<unsigned int>(v.value) << ";" << endl;
    } else if(v.type=="long"){
      this->srcFile << "const long " << mdata.className << "::" << v.name 
		    << " = " << static_cast<long>(v.value) << ";" << endl;
    } else if(v.type=="ulong"){
      this->srcFile << "const unsigned long " << mdata.className << "::" << v.name 
		    << " = " << static_cast<unsigned long>(v.value) << ";" << endl;
    } else if(v.type=="float"){
      this->srcFile << "const float " << mdata.className << "::" << v.name 
		    << " = " << static_cast<float>(v.value) << ";" << endl;
    } else if(v.type=="double"){
      this->srcFile << "const double " << mdata.className << "::" << v.name 
		    << " = " << static_cast<double>(v.value) << ";" << endl;
    } else if(v.type=="ldouble"){
      this->srcFile << "const long double " << mdata.className << "::" << v.name 
		    << " = " << static_cast<long double>(v.value) << ";" << endl;
    } else if(v.type=="real"){
      this->srcFile << "const pleiades::field::real " << mdata.className << "::" << v.name 
		    << " = " << static_cast<double>(v.value) << ";" << endl;
    } else {
      string msg("MFrontModelInterface::writeStaticVariableInitialisation : ");
      msg += "type " + v.type + " is not a supported type for a static variable.";
      msg += "Supported types are short, ushort, int, uint, long, ulong,";
      msg += "float, double, ldouble and real.\n";
      msg += "Error at line " + toString(v.lineNumber);
      throw(runtime_error(msg));
    }
  } // end of MFrontModelInterface::writeStaticVariableInitialisation
    
  void
  MFrontModelInterface::writeOutputFiles(const FileDescription& fdata,
					 const ModelData& mdata)
  {
    using namespace std;
    if(mdata.className.empty()){
      string msg("MFrontModelInterface::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->headerFileName  = "include/Pleiades/Model/"+mdata.className;
    this->headerFileName += "-@application@.hxx";
    this->srcFileName  = "src/"+mdata.className;
    this->srcFileName += "-@application@.cxx";
    this->openOutputFiles();
    this->generateOutputFiles(fdata,mdata);
    this->closeOutputFiles();
  } // end of class MFrontModelInterface::writeOutputFiles()

  void
  MFrontModelInterface::writeAssignDefaultValue(const VariableDescriptionContainer::const_iterator p,
						const std::map<std::string,std::string>::const_iterator p4)
  {
    using namespace std;
    if((p->type=="string")||(p->type=="double")||
       (p->type=="real")||(p->type=="bool")){
      this->srcFile << "this->" << p->name << " = "  << p4->second << ";" << endl;
    } else {
      string msg("MFrontModelInterface::writeAssignDefaultValue : ");
      msg+="type '"+p->type+"' is not supported.\n";
      throw(runtime_error(msg));
    }   
  } // end of MFrontModelInterface::writeAssignDefaultValue

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontModelInterface::treatKeyword(const std::string&,
				     tfel::utilities::CxxTokenizer::TokensContainer::const_iterator c,
				     const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef CxxTokenizer::TokensContainer::const_iterator TokenConstIterator;
    return pair<bool,TokenConstIterator>(false,c);
  } // end of MFrontModelInterface::treatKeyword

  void
  MFrontModelInterface::reset(void)
  {
    this->headerFileName.clear();
    this->srcFileName.clear();
    this->hasDefaultConstructor = false;
    if(this->headerFile.is_open()){
      this->headerFile.close();
    }
    if(this->srcFile.is_open()){
      this->srcFile.close();
    }
  } // end of MFrontModelInterface::reset

  std::string
  MFrontModelInterface::getName(void)
  {
    return "@application@";
  } // end of MFrontModelInterface::getName(void)

  std::map<std::string,std::vector<std::string> >
  MFrontModelInterface::getGeneratedSources(const ModelData& mdata)
  {
    using namespace std;
    map<string,vector<string> > src;
    const string& lib = getLibraryName(mdata);
    src[lib].push_back(mdata.className+"-@application@.cxx");
    return src;
  } // end of MFrontModelInterface::getGeneratedSources
  
  std::vector<std::string>
  MFrontModelInterface::getGeneratedIncludes(const ModelData& mdata)
  {
    using namespace std;
    vector<string> inc;
    inc.push_back("Pleiades/Model/"+mdata.className+"-@application@.hxx");
    return inc;
  } // end of MFrontModelInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontModelInterface::getGlobalIncludes(const ModelData& mdata)
  {
    using namespace std;
    map<string,vector<string> > incs;
    const string& lib = getLibraryName(mdata);
    incs[lib].push_back("`@application@-config --includes`\n");
    return incs;
  } // end of MFrontModelInterface::getGlobalIncludes
  
  std::map<std::string,std::vector<std::string> >
  MFrontModelInterface::getGlobalDependencies(const ModelData& mdata)
  {
    using namespace std;
    map<string,vector<string> > libs;
    const string& lib = getLibraryName(mdata);
    libs[lib].push_back("`@application@-config --libs` -lm");
    return libs;
  } // end of MFrontModelInterface::getGlobalDependencies
    
  std::map<std::string,std::vector<std::string> >
  MFrontModelInterface::getLibrariesDependencies(const ModelData&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontModelInterface::getLibrariesDependencies

  ModelInterfaceProxy<MFrontModelInterface>      ModelProxy;
  
} // end of namespace mfront  
