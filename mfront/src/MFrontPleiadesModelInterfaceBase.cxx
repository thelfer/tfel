/*! 
 * \file  MFrontPleiadesModelInterfaceBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 17 jun 2010
 */

#include<cassert>
#include<iterator>

#include"TFEL/System/System.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontPleiadesModelInterfaceBase.hxx"

namespace mfront{

  MFrontPleiadesModelInterfaceBase::MFrontPleiadesModelInterfaceBase()
  {
    this->reset();
  } // end of MFrontPleiadesModelInterfaceBase::MFrontPleiadesModelInterfaceBase

  void 
  MFrontPleiadesModelInterfaceBase::setVerboseMode()
  {
    this->verboseMode = true;
  } // end of MFrontPleiadesModelInterfaceBase::setVerboseMode

  void 
  MFrontPleiadesModelInterfaceBase::setDebugMode()
  {
    this->debugMode = true;
  } // end of MFrontPleiadesModelInterfaceBase::setDebugMode

  void 
  MFrontPleiadesModelInterfaceBase::setWarningMode()
  {
    this->warningMode = true;
  } // end of MFrontPleiadesModelInterfaceBase::setWarningMode

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontPleiadesModelInterfaceBase::treatKeyword(const std::string&,
						 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator c,
						 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef CxxTokenizer::TokensContainer::const_iterator TokenConstIterator;
    return pair<bool,TokenConstIterator>(false,c);
  } // end of MFrontPleiadesModelInterfaceBase::treatKeyword

  void
  MFrontPleiadesModelInterfaceBase::declareReservedNames(std::set<std::string>& v)
  {
    v.insert("msg");
    v.insert("oss");
    v.insert("mesh");
    v.insert("nodes");
    v.insert("mesh");
    v.insert("node_k");
    v.insert("gnode_k");
    v.insert("dt");
    v.insert("ptr");
    v.insert("ptr2");
    v.insert("domains");
    v.insert("domain");
    v.insert("domainName");
    v.insert("PleiadesError");
    v.insert("initialize");
    v.insert("initializeInput");
    v.insert("initializeParameter");
    v.insert("initializeInputIFieldDouble");
  } // end of MFrontPleiadesModelInterfaceBase::declareReservedNames

  void
  MFrontPleiadesModelInterfaceBase::reset(void)
  {
    this->headerFileName.clear();
    this->srcFileName.clear();
    this->domains.clear();
    this->hasDefaultConstructor = false;
    this->hasSpecializedDomain  = false;
    this->verboseMode = false;
    this->debugMode   = false;
    this->warningMode = false;
  } // end of MFrontPleiadesModelInterfaceBase::reset

  void
  MFrontPleiadesModelInterfaceBase::openOutputFiles(void)
  {
    using namespace std;
    using namespace tfel::system;
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/PMetier");
    systemCall::mkdir("include/Pleiades/PMetier/PModels");
    systemCall::mkdir("src");
    this->headerFile.open(("include/"+this->headerFileName).c_str());
    if(!this->headerFile){
      string msg("MFrontPleiadesModelInterfaceBase::openOutputFiles : ");
      msg += "unable to open file '";
      msg += "include/"+this->headerFileName+"'";
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.open(("src/"+this->srcFileName).c_str());
    if(!this->srcFile){
      string msg("MFrontPleiadesModelInterfaceBase::openOutputFiles : ");
      msg += "unable to open file '";
      msg += "src/"+this->srcFileName+"'";
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.exceptions(ios::badbit|ios::failbit);
  } // end of MFrontPleiadesModelInterfaceBase::openOutputFiles()

  void
  MFrontPleiadesModelInterfaceBase::closeOutputFiles(void)
  {
    this->headerFile.close();
    this->srcFile.close();
  } // end of MFrontPleiadesModelInterfaceBase::closeOutputFiles()

  void
  MFrontPleiadesModelInterfaceBase::writeOutputFiles(const MFrontGenericData& pdata,
						     const MFrontModelData& data)
  {
    using namespace std;
    if(pdata.className.empty()){
      string msg("MFrontPleiadesModelInterfaceBase::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->setOutputFileNames(pdata);
    this->generateDomainsList(data);
    this->openOutputFiles();
    this->generateOutputFiles(pdata,data);
    this->closeOutputFiles();
  } // end of MFrontPleiadesModelInterfaceBase::writeOutputFiles

  void
  MFrontPleiadesModelInterfaceBase::generateOutputFiles(const MFrontGenericData& pdata,
							const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    this->hasDefaultConstructor=true;
    if(this->domains.empty()){
      this->hasDefaultConstructor=false;
    }
    if(!data.localParameters.empty()){
      for(p=data.localParameters.begin();
	  (p!=data.localParameters.end())&&(this->hasDefaultConstructor);++p){
	if(data.defaultValues.find(p->name)==data.defaultValues.end()){
	  this->hasDefaultConstructor = false;
	}
      }
    }
    this->writeHeaderFile(pdata,data);
    this->writeSrcFile(pdata,data);
  } // end of MFrontPleiadesModelInterfaceBase::generateOutputFiles

  void
  MFrontPleiadesModelInterfaceBase::writeHeaderFile(const MFrontGenericData& pdata,
						    const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    StaticVarContainer::const_iterator p2;
    MFrontModelData::FunctionContainer::const_iterator p3;
    set<string>::iterator p4;
    set<string>::iterator p5;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << pdata.className << " PleiadesModel.\n";
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
    this->headerFile << "#include\"Pleiades/PMetier/IncPMetier.hh\"\n";
    if(!pdata.includes.empty()){
      this->headerFile << pdata.includes << endl << endl;
    }
    //    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace Pleiades\n{\n";
    this->headerFile << "namespace PMetier\n{\n";
    this->headerFile << "namespace PModels\n{\n\n";
    //    this->headerFile << "class MFRONT_SHAREDOBJ " << pdata.className << endl;
    this->headerFile << "class " << pdata.className << endl;
    this->headerFile << ": public IModel\n";
    this->headerFile << "{\n\n";
    this->headerFile << "public:\n";
    if((!data.localParameters.empty())||(this->hasSpecializedDomain)){
      this->headerFile << "//! Constructor to initialize local parameters\n";
      this->headerFile << pdata.className
		       << "(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    }
    if(this->hasDefaultConstructor){
      this->headerFile << "//! Default constructor\n";
      this->headerFile << pdata.className << "();\n\n";
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
    this->headerFile << "typedef double real;\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "//! Copy constructor (disabled)\n";
    this->headerFile << pdata.className << "(const " 
		     << pdata.className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << pdata.className << "&\n"
		     << "operator=(const " 
		     << pdata.className << "&);\n\n";
    this->writeSpecificPrivateMethodDeclaration(pdata,data);
    // Treat functions
    for(p3=data.functions.begin();p3!=data.functions.end();++p3){
      if(p3->modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelInterfaceBase::writeHeaderFile : ");
	msg+="function " + p3->name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->headerFile << "#line " << p3->line << " \"" 
			 << pdata.fileName << "\"\n";
      }
      if(p3->modifiedVariables.size()==1){
	this->headerFile << "inline double\n";
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
      if((p3->usedVariables.size()==0)&&(!p3->useTimeIncrement)){
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
	if(p3->useTimeIncrement){
	  if(p3->usedVariables.size()!=0){
	    this->headerFile << ",";
	  }
	  this->headerFile << "const double";
	}
      }
      this->headerFile << ");\n\n";
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
    for(p=data.outputs.begin();p!=data.outputs.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << "VARIABLE(Pleiades::PMetier::PField::PleiadesField,ple" << p->name << ")\n\n";
    }
    this->headerFile << endl;
    for(p=data.inputs.begin();p!=data.inputs.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << "VARIABLE(Pleiades::PMetier::PField::PleiadesField,ple" << p->name << ")\n\n";
    }
    this->headerFile << endl;
    this->headerFile << "private:"<< endl;
    this->headerFile << endl;
    for(p=data.globalParameters.begin();p!=data.globalParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      if(p->type=="StringArray"){
	this->headerFile << "TabString " << p->name << ";\n\n";
      } else if(p->type=="DoubleArray"){
	this->headerFile << "TabDouble " << p->name << ";\n\n";
      } else {
	this->headerFile << p->type << " " << p->name << ";\n\n";
      }
    }
    for(p=data.constantMaterialProperties.begin();p!=data.constantMaterialProperties.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for(p=data.localParameters.begin();p!=data.localParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      if(p->type=="StringArray"){
	this->headerFile << "TabString " << p->name << ";\n\n";
      } else if(p->type=="DoubleArray"){
	this->headerFile << "TabDouble " << p->name << ";\n\n";
      } else {
	this->headerFile << p->type << " " << p->name << ";\n\n";
      }
    }
    this->headerFile << endl;
    this->headerFile << "std::set<std::string> domains;" << endl;
    this->headerFile << endl;
    this->headerFile << "}; // end of class " << pdata.className << endl << endl;
    this->headerFile << "} // end of namespace PModels\n\n";
    this->headerFile << "} // end of namespace PMetier\n\n";
    this->headerFile << "} // end of namespace Pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << makeUpperCase(pdata.className)
		     << "_HH */\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeHeaderFile(void)

  std::string
  MFrontPleiadesModelInterfaceBase::getGenTypeMethod(const std::string& type) const
  {
    using namespace std;
    if((type=="double")||(type=="real")){
      return "getDouble";
    } else if(type=="string"){
      return "getString";
    } else if(type=="StringArray"){
      return "getTabString";
    } else if(type=="DoubleArray"){
      return "getTabDouble";
    } else {
      string msg("MFrontPleiadesModelInterfaceBase::getGenTypeMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontPleiadesModelInterfaceBase::getGenTypeMethod

  static std::pair<std::string,unsigned short>
  getFieldAndDepthFromFieldName(const MFrontModelData& data,
				const std::string& n){
    // This is a silly hack
    using namespace std;
    VarContainer::const_iterator p;
    map<string,unsigned short>::const_iterator p2;
    unsigned short j;
    for(p=data.outputs.begin();p!=data.outputs.end();++p){
      if(p->name==n){
	return make_pair(p->name,0u);
      }
      if((p2=data.depths.find(p->name))!=data.depths.end()){
	for(j=1;j<=p2->second;++j){
	  ostringstream fn;
	  fn << p->name << "_" << j;
	  if(fn.str()==n){
	    return make_pair(p->name,j);
	  }
	}
      }
    }
    for(p=data.inputs.begin();p!=data.inputs.end();++p){
      if(p->name==n){
	return make_pair(p->name,0);
      }
      if((p2=data.depths.find(p->name))!=data.depths.end()){
	for(j=1;j<=p2->second;++j){
	  ostringstream fn;
	  fn << p->name << "_" << j;
	  if(fn.str()==n){
	    return make_pair(p->name,j);
	  }
	}
      }
    }
    string msg("getFieldAndDepthFromFieldName : ");
    msg += "field name '"+n+"' has not been found";
    throw(runtime_error(msg));
    return pair<string,unsigned short>("",0u);
  } // end of getFieldAndDepthFromFieldName

  void
  MFrontPleiadesModelInterfaceBase::writeSrcFile(const MFrontGenericData& pdata,
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
    set<string>::const_iterator p16;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << pdata.className << " PleiadesModel.\n";
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
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include\"Pleiades/PMetier/pglossaire.hh\"\n";
    this->srcFile << "#include\"Pleiades/PExamplars/ClassProxy.hh\"\n";
    
    this->srcFile << "#include\""+this->headerFileName+"\"\n\n";

    this->srcFile << "namespace Pleiades\n{\n";
    this->srcFile << "namespace PMetier\n{\n";
    this->srcFile << "namespace PModels\n{\n\n";
    // static variables
    if(!pdata.staticVars.empty()){
      for(p10=pdata.staticVars.begin();p10!=pdata.staticVars.end();++p10){
	if(this->debugMode){
	  this->srcFile << "#line " << p10->lineNumber << " \"" 
			<< pdata.fileName << "\"\n";
	}
	if(p10->type=="short"){
	  this->srcFile << "const short " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<short>(p10->value) << ";" << endl;
	} else if(p10->type=="ushort"){
	  this->srcFile << "const unsigned short " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<unsigned short>(p10->value) << ";" << endl;
	} else if(p10->type=="int"){
	  this->srcFile << "const int " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<int>(p10->value) << ";" << endl;
	} else if(p10->type=="uint"){
	  this->srcFile << "const unsigned int " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<unsigned int>(p10->value) << ";" << endl;
	} else if(p10->type=="long"){
	  this->srcFile << "const long " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<long>(p10->value) << ";" << endl;
	} else if(p10->type=="ulong"){
	  this->srcFile << "const unsigned long " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<unsigned long>(p10->value) << ";" << endl;
	} else if(p10->type=="float"){
	  this->srcFile << "const float " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<float>(p10->value) << ";" << endl;
	} else if((p10->type=="double")||(p10->type=="real")){
	  this->srcFile << "const double " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<double>(p10->value) << ";" << endl;
	} else if(p10->type=="ldouble"){
	  this->srcFile << "const long double " << pdata.className << "::" << p10->name 
			<< " = " << static_cast<long double>(p10->value) << ";" << endl;
	} else {
	  string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
	  msg += "type " + p10->type + " is not a supported type for a static variable. ";
	  msg += "Supported types are short, ushort, int, uint, long, ulong,";
	  msg += "float, double, real and ldouble.\n";
	  msg += "Error at line " + toString(p10->lineNumber);
	  throw(runtime_error(msg));
	}
      }
      this->srcFile << endl;
    }
    // Constructors
    if(this->hasDefaultConstructor){
      this->srcFile << pdata.className << "::" 
		    << pdata.className << "()\n{\n";
      for(p=data.localParameters.begin();p!=data.localParameters.end();++p){
	string name;
	if((p4=data.glossaryNames.find(p->name))!=data.glossaryNames.end()){
	  name = "GlossaireParam::" + p4->second;
	} else if((p4=data.entryNames.find(p->name))!=data.entryNames.end()){
	  name = "\""+p4->second+"\"";
	} else {
	  name = "\""+p->name+"\"";
	}
	p4=data.defaultValues.find(p->name);
	assert(p4!=data.defaultValues.end());
	this->writeAssignDefaultValue(pdata,p,p4);
      }
      if(this->initializeDefaultDomainListInConstrutor(data)){
	for(p12=this->domains.begin();p12!=this->domains.end();++p12){
	  this->srcFile << "this->domains.insert(" << *p12 << ");\n";
	}
      }
      this->srcFile << "} // end of " 
		    << pdata.className << "::" 
		    << pdata.className <<"\n\n";
    }
    if((!data.localParameters.empty())||(this->hasSpecializedDomain)){
      this->srcFile << "//! Constructor to initialize local parameters\n";
      this->srcFile << pdata.className << "::" 
		    << pdata.className 
		    << "(const Pleiades::PMetier::IArgumentMetier& arg)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
      this->srcFile << "using namespace Pleiades::PExceptions;\n";
      for(p=data.localParameters.begin();p!=data.localParameters.end();++p){
	string name;
	if((p4=data.glossaryNames.find(p->name))!=data.glossaryNames.end()){
	  name = "GlossaireParam::" + p4->second;
	} else if((p4=data.entryNames.find(p->name))!=data.entryNames.end()){
	  name = "\""+p4->second+"\"";
	} else {
	  name = "\""+p->name+"\"";
	}
	this->srcFile << "if(!arg.contains(" << name << ")){\n";
	if((p4=data.defaultValues.find(p->name))!=data.defaultValues.end()){
	  this->writeAssignDefaultValue(pdata,p,p4);
	  this->srcFile << "} else {\n";
	} else {
	  this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
	  this->srcFile << "msg += \"can't initialize parameter " << p->name << "\";\n";
	  this->srcFile << "throw(PleiadesError(msg));\n";
	  this->srcFile << "} else {\n";
	}
	this->srcFile << "this->" << p->name << " = arg[" << name << "]." 
		      << this->getGenTypeMethod(p->type) << "();\n";
	this->srcFile << "}\n";
      }
      if(this->hasSpecializedDomain){
	this->srcFile << "if((arg.contains(\"domain\"))||(arg.contains(\"domains\"))){\n";
	this->srcFile << "if(arg.contains(\"domain\")){\n";
	this->srcFile << "this->domains.insert(arg[\"domain\"].getString());\n";
	this->srcFile << "}\n";
	this->srcFile << "if(arg.contains(\"domains\")){\n";
	this->srcFile << "TabString sdomains = arg[\"domains\"].getTabString();\n";
	this->srcFile << "for(int i=0;i!=sdomains.getTaille();++i){\n";
	this->srcFile << "this->domains.insert(sdomains[i]);\n";
	this->srcFile << "}\n";
	this->srcFile << "}\n";
	this->srcFile << "} else {\n";
	if(!data.domains.empty()){
	  for(p12=this->domains.begin();p12!=this->domains.end();++p12){
	    this->srcFile << "this->domains.insert(" << *p12 << ");\n";
	  }
	} else {
	  this->srcFile << "string msg(\"" << pdata.className << "::" << pdata.className << " : \");\n";
	  this->srcFile << "msg += \"no domain specified\";\n";
	  this->srcFile << "throw(PleiadesError(msg));\n";
	}
	this->srcFile << "}\n";
      }
      this->srcFile <<"} // end of "
		    << pdata.className << "::" 
		    << pdata.className <<"\n\n";
    }
    // Functions
    for(p11=data.functions.begin();p11!=data.functions.end();++p11){
      if(p11->modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
	msg+="function " + p11->name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(this->debugMode){
	this->srcFile << "#line " << p11->line << " \"" 
		      << pdata.fileName << "\"\n";
      }
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "double\n";
	this->srcFile << pdata.className << "::" << p11->name << "(";
      } else {
	this->srcFile << "void\n";
	this->srcFile << pdata.className << "::" << p11->name << "(";
	for(p12=p11->modifiedVariables.begin();p12!=p11->modifiedVariables.end();){
	  this->srcFile << "double& " << *p12;
	  if((++p12)!=p11->modifiedVariables.end()){
	    this->srcFile << ",\n";
	  }
	}
      }
      if((p11->usedVariables.size()==0)&&(p11->useTimeIncrement)){
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
	    this->srcFile << ",\n";
	  }
	}
	if(p11->useTimeIncrement){
	  if(p11->usedVariables.size()!=0){
	    this->srcFile << ",\n";
	  }
	  this->srcFile << "const double dt";
	}
      }
      this->srcFile << ")\n";
      this->srcFile << "{\n";
      this->srcFile << "using namespace std;\n";
      writeMaterialLaws("MFrontPleiadesModelInterfaceBase::writeSrcFile",
			this->srcFile,pdata.materialLaws);		      
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "double " << *(p11->modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << p11->body;
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "return " << *(p11->modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << "} // end of " << pdata.className << "::" << p11->name << "\n\n";
    }
    // initializeOutput
    this->writeInitializeOutputMethod(pdata,data);
    // initializeInput
    this->writeInitializeInputMethod(pdata,data);
    // initializeParamaters
    this->writeInitializeParametersMethod(pdata,data);
    // initialize
    this->writeInitializeMethod(pdata,data);
    // execute
    this->srcFile << "bool\n"
		  << pdata.className
		  << "::execute(void){\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PTime;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PMesh;\n\n";
//     for(p=data.outputs.begin();p!=data.outputs.end();++p){
//       this->srcFile << "MEDField " << p->name 
// 		    << " = *(static_cast<MEDFieldPtr>(this->ple" << p->name << "->getMed()));\n";
//       if((p5=data.depths.find(p->name))!=data.depths.end()){
// 	for(j=1;j<=p5->second;++j){
// 	  ostringstream converter;
// 	  converter << j;
// 	  string varName = p->name+"_"+converter.str();
// 	  this->srcFile << "MEDField " << varName 
// 			<< " = *(static_cast<MEDFieldPtr>(this->ple" 
// 			<< p->name << "->getFieldPreceding("<< j <<")->getMed()));\n";
// 	}
//       }
//     }
//     for(p=data.inputs.begin();p!=data.inputs.end();++p){
//       this->srcFile << "MEDField " << p->name 
// 		    << " = *(static_cast<MEDFieldPtr>(this->ple" << p->name << "->getMed()));\n";
//       if((p5=data.depths.find(p->name))!=data.depths.end()){
// 	for(j=1;j<=p5->second;++j){
// 	  this->srcFile << "MEDField " << p->name << "_" << j
// 			<< " = *(static_cast<MEDFieldPtr>(this->ple" 
// 			<< p->name << "->getFieldPreceding("<< j <<")->getMed()));\n";
// 	}
//       }
//     }
    this->srcFile << "const double t  = ITime::GlobalTime->get(\"time\");\n";
    this->srcFile << "const double dt = ITime::GlobalTime->get(\"time increment\");\n";
    this->srcFile << "static_cast<void>(t);  // suppress unused variable warning\n";
    this->srcFile << "static_cast<void>(dt); // suppress unused variable warning\n\n";

    // calling functions
    if(data.functions.empty()){
      string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
      msg += "no function defined";
      throw(runtime_error(msg));
    }
    if(data.outputs.size()==0){
      string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
      msg += "no output defined";
      throw(runtime_error(msg));
    }
    this->srcFile << "PtrIMesh mesh = this->_ple" << data.outputs.front().name << "->getMesh();\n";
    this->srcFile << "set<string>::const_iterator ptr;\n";
    this->srcFile << "if(this->domains.empty()){\n";
    this->srcFile << "string msg(\"" << pdata.className << "::execute : \");\n";
    this->srcFile << "msg += \"no domain specified\";\n";
    this->srcFile << "throw(PleiadesError(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "int k;\n";
//     this->srcFile << "map<string,vector<int> >::const_iterator ptr2;\n";
//     this->srcFile << "vector<int>::const_iterator ptr3;\n";

    this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
    this->buildDomainName();
    this->srcFile << "const int nodes = mesh->getNumberOfElements(NODE,domainName);\n";
//     this->srcFile << "ptr2 = this->nodesOfZones.find(*ptr);\n";
//     this->srcFile << "if(ptr2==this->nodesOfZones.end()){\n";
//     this->srcFile << "string msg(\"" << pdata.className << "::execute : \");\n";
//     this->srcFile << "msg += \"internal error (no group named '\"+*ptr+\"' defined\";\n";
//     this->srcFile << "throw(PleiadesError(msg));\n";
//     this->srcFile << "}\n";
//     this->srcFile << "const vector<int>& nodes = ptr2->second;\n";
    this->srcFile << "for(node_k=0;node_k!=nodes;++node_k){\n";
    this->srcFile << "const int gnode_k = mesh->getElement(NODE,domainName,k);\n";
    this->writeFunctionCall(data,data.functions);
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << pdata.className << "::execute\n\n";
    
    this->srcFile << "void\n" << pdata.className 
		  << "::executeInitialPostProcessingTasks(const bool)\n{} // end of " << pdata.className 
		  << "::executeInitialPostProcessingTasks\n\n";

    this->srcFile << "void\n" << pdata.className 
		  << "::executePostProcessingTasks(const bool)\n{} // end of " << pdata.className 
		  << "::executePostProcessingTasks\n\n";

    if(!data.localParameters.empty()){
      this->srcFile << "using Pleiades::PExamplars::TemplateModelClassProxy;\n";
      this->srcFile << "TemplateModelClassProxy<"
		    << pdata.className
		    << "> proxy" << pdata.className << "(\""
		    << pdata.className << "\");\n";
    }
    if(this->hasDefaultConstructor){
      this->srcFile << "GENERATE_PROXY(IModel," << pdata.className << ");\n\n";
    }
    this->srcFile << "} // end of namespace PModels\n\n";
    this->srcFile << "} // end of namespace PMetier\n\n";
    this->srcFile << "} // end of namespace Pleiades\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeSrcFile(void)

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getGeneratedSources(const MFrontGenericData& pdata)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib;
    if(pdata.library.empty()){
      if(!pdata.material.empty()){
	lib = "lib"+this->getApplicationName()+pdata.material+"Models";
      } else {
	lib = "lib"+this->getApplicationName()+"MaterialModels";
      }
    } else {
      lib = pdata.library;
    }
    src[lib].push_back(this->srcFileName);
    return src;
  } // end of MFrontPleiadesModelInterfaceBase::getGeneratedSources
  
  std::vector<std::string>
  MFrontPleiadesModelInterfaceBase::getGeneratedIncludes(const MFrontGenericData&)
  {
    using namespace std;
    vector<string> inc;
    inc.push_back(this->headerFileName);
    return inc;
  } // end of MFrontPleiadesModelInterfaceBase::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getGlobalIncludes(const MFrontGenericData& pdata)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib;
    if(pdata.library.empty()){
      if(!pdata.material.empty()){
	lib = "lib"+this->getApplicationName()+pdata.material+"Models";
      } else {
	lib = "lib"+this->getApplicationName()+"MaterialModels";
      }
    } else {
      lib = pdata.library;
    }
    incs[lib].push_back("`pleiades-config --includes`\n");
    return incs;
  } // end of MFrontPleiadesModelInterfaceBase::getGlobalIncludes
  
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getGlobalDependencies(const MFrontGenericData& pdata)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string lib;
    if(pdata.library.empty()){
      if(!pdata.material.empty()){
	lib = "lib"+this->getApplicationName()+pdata.material+"Models";
      } else {
	lib = "lib"+this->getApplicationName()+"MaterialModels";
      }
    } else {
      lib = pdata.library;
    }
    libs[lib].push_back("`pleiades-config --libs` -lm");
    return libs;
  } // end of MFrontPleiadesModelInterfaceBase::getGlobalDependencies
    
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getLibrariesDependencies(const MFrontGenericData&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontPleiadesModelInterfaceBase::getLibrariesDependencies
  void
  MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue(const MFrontGenericData& pdata,
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
      string msg("MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue : ");
      msg+="type "+p->type+" is not supported.\n";
      throw(runtime_error(msg));
    }   
  } // end of MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue

  void
  MFrontPleiadesModelInterfaceBase::writeGetGlobalParameter(const VarHandler& v,
							    const MFrontGenericData& pdata,
							    const MFrontModelData& data)
  {
    using namespace std;
    string name;
    map<string,string>::const_iterator p;
    if((p=data.glossaryNames.find(v.name))!=data.glossaryNames.end()){
      name = "GlossaireParam::" + p->second;
    } else if((p=data.entryNames.find(v.name))!=data.entryNames.end()){
      name = "\""+p->second+"\"";
    } else {
      name = "\""+v.name+"\"";
    }
    this->srcFile << "if(!arg.contains(" << name << ")){\n";
    if((p=data.defaultValues.find(v.name))!=data.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";" << endl;
      this->srcFile << "} else {\n";
    } else {
      this->srcFile << "string msg(\"" << pdata.className << "::initializeParameters : \");\n";
      this->srcFile << "msg += \"can't initialize parameter '" << v.name << "' using '\";\n";
      this->srcFile << "msg += " << name << ";\n";
      this->srcFile << "msg += '\\'';\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "} else {\n";
    }
    this->srcFile << "this->" << v.name << " = arg[" << name << "]." 
		  << this->getGenTypeMethod(v.type) << "();\n";
    this->srcFile << "}\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeGetGlobalParameter

  void
  MFrontPleiadesModelInterfaceBase::writeGetConstantMaterialProperty(const VarHandler& v,
								     const MFrontGenericData& pdata,
								     const MFrontModelData& data)
  {
    using namespace std;
    string name;
    string name2;
    map<string,string>::const_iterator p;
    if((p=data.glossaryNames.find(v.name))!=data.glossaryNames.end()){
      name  = "GlossaireParam::" + p->second;
    } else if((p=data.entryNames.find(v.name))!=data.entryNames.end()){
      name = "\""+p->second+"\"";
    } else {
      name = "\""+v.name+"\"";
    }
    this->srcFile << "if(arg.contains(" << name << ")){\n";
    this->srcFile << "this->" << v.name << " = arg[" << name << "]." 
		  << this->getGenTypeMethod(v.type) << "();\n";
    this->srcFile << "} else {\n";
    if((p=data.defaultValues.find(v.name))!=data.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";" << endl;
    } else {
      this->srcFile << "string msg(\"" << pdata.className << "::initializeParameters : \");\n";
      this->srcFile << "msg += \"can't initialize constant material property  '"
		    << v.name << "' using '\";\n";
      this->srcFile << "msg += " << name << ";\n";
      this->srcFile << "msg += '\\'';\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
    }
    this->srcFile << "}\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeGetConstantMaterialProperty

  void
  MFrontPleiadesModelInterfaceBase::writeFunctionCall(const MFrontModelData& data,
						      const std::vector<MFrontModelData::Function>& functions)
  {
    using namespace std;
    typedef unsigned short ushort;
    set<string> usedVariables;
    set<string> modifiedVariables;
    vector<MFrontModelData::Function>::const_iterator p;
    set<string>::const_iterator p2;
    set<string>::const_iterator p3;
    map<string,unsigned short>::const_iterator p4;
    usedVariables.clear();
    modifiedVariables.clear();
    // generating temporaries for variables of thoses fields
    for(p=functions.begin();p!= functions.end();++p){
      for(p2=p->usedVariables.begin();p2!=p->usedVariables.end();++p2){
	usedVariables.insert(*p2);	  
      }
      for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();++p2){
	modifiedVariables.insert(*p2);	  
      }
    }
    for(p2=modifiedVariables.begin();p2!=modifiedVariables.end();++p2){
      if((p3=usedVariables.find(*p2))!=usedVariables.end()){
	usedVariables.erase(*p3);
      }
    }
    // declaring temporaries
    for(p2=usedVariables.begin();p2!=usedVariables.end();++p2){
      if(data.fieldNames.find(*p2)!=data.fieldNames.end()){
	const pair<string,ushort>& fn = getFieldAndDepthFromFieldName(data,*p2);
	if(fn.second==0){
	  this->srcFile << "const double tmp_" << *p2 << " = this->_ple" 
			<< fn.first << "[gnode_k];\n";
	} else {
	  this->srcFile << "const double tmp_" << *p2 << " = this->_ple" 
			<< fn.first << "->getValuePrecedingType(" << fn.second << ",gnode_k);\n";
	}
      }
    }
    for(p2=modifiedVariables.begin();p2!=modifiedVariables.end();++p2){
      if(data.fieldNames.find(*p2)!=data.fieldNames.end()){
	const pair<string,ushort>& fn = getFieldAndDepthFromFieldName(data,*p2);
	if(fn.second==0){
	  this->srcFile << "double tmp_" << *p2 << " = this->_ple" 
			<< fn.first << "[gnode_k];\n";
	} else {
	  string msg("MFrontPleiadesModelInterfaceBase::writeFunctionCall : ");
	  msg += "variable '"+*p2+"' shall not be modified";
	  throw(runtime_error(msg));
	}

      }
    }
    for(p=functions.begin();p!= functions.end();++p){
      if(p->modifiedVariables.size()==1){
	this->srcFile << "tmp_" << *(p->modifiedVariables.begin()) << " = ";
	this->srcFile << p->name << "(";
	for(p2=p->usedVariables.begin();p2!=p->usedVariables.end();){
	  if(data.fieldNames.find(*p2)!=data.fieldNames.end()){
	    this->srcFile << "tmp_" << *p2;
	  } else {
	    this->srcFile << *p2;
	  }
	  if((++p2)!=p->usedVariables.end()){
	    this->srcFile << ",";
	  }
	}
      } else{
	this->srcFile << "this->" << p->name << "(";
	for(p2=p->modifiedVariables.begin();p2!=p->modifiedVariables.end();){
	  this->srcFile << "tmp_" << *p2;
	  if((++p2)!=p->modifiedVariables.end()){
	    this->srcFile << ",";
	  }
	}
	if(!p->usedVariables.empty()){
	  this->srcFile << ",";
	}
	for(p2=p->usedVariables.begin();p2!=p->usedVariables.end();){
	  if(data.fieldNames.find(*p2)!=data.fieldNames.end()){
	    this->srcFile << "tmp_" << *p2;
	  } else {
	    this->srcFile << *p2;
	  }
	  if((++p2)!=p->usedVariables.end()){
	    this->srcFile << ",";
	  }
	}
      }
      if(p->useTimeIncrement){
	this->srcFile << ",dt";
      }
      this->srcFile << ");\n";
    }
    for(p2=modifiedVariables.begin();p2!=modifiedVariables.end();++p2){
      this->srcFile << *p2 << ".setValueType(gnode_k,tmp_" << *p2 << ",1);\n";
    }
  }

  void
  MFrontPleiadesModelInterfaceBase::generateDomainsList(const MFrontModelData& data)
  {
    using namespace std;
    set<string>::const_iterator p;
    this->hasSpecializedDomain  = true;
    if(data.domains.empty()){
      this->hasDefaultConstructor = false;
    } else {
      for(p=data.domains.begin();p!=data.domains.end();++p){
	this->domains.insert('\"'+*p+'\"');
      }
    }
  } // end of MFrontPleiadesModelInterfaceBase::generateDomainsList

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeMethod(const MFrontGenericData& pdata,
							  const MFrontModelData&)
  {
    this->srcFile << "bool\n"
		  << pdata.className 
		  << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "this->initializeOutput(arg);\n";
    this->srcFile << "this->initializeInput(arg);\n";
    this->srcFile << "this->initializeParameters(arg);\n";
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << pdata.className << "::initialize\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod(const MFrontGenericData& pdata,
								    const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    this->srcFile << "bool\n"
		  << pdata.className 
		  << "::initializeParameters(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=data.globalParameters.begin();p!=data.globalParameters.end();++p){
      this->writeGetGlobalParameter(*p,pdata,data);
    }
    for(p=data.constantMaterialProperties.begin();p!=data.constantMaterialProperties.end();++p){
      this->writeGetConstantMaterialProperty(*p,pdata,data);
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << pdata.className << "::initializeParameters\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod(const MFrontGenericData& pdata,
								const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    this->srcFile << "bool\n"
		  << pdata.className
		  << "::initializeOutput(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=data.outputs.begin();p!=data.outputs.end();++p){
      string name;
      if((p2=data.glossaryNames.find(p->name))!=data.glossaryNames.end()){
	name = "GlossaireField::"+p2->second;
      } else if((p2=data.entryNames.find(p->name))!=data.entryNames.end()){
	name = "\""+p2->second+"\"";
      } else {
	name = "\""+p->name+"\"";
      }
      this->srcFile << "if(!this->initializeOutputIFieldDouble(arg," << name;
      this->srcFile << ",\nthis->_ple" << p->name << ",\"\",";
      if((p3=data.initialValues.find(p->name))!=data.initialValues.end()){
	this->srcFile << p3->second;
      } else {
	this->srcFile << "0.";
      }
      this->srcFile << ")){\n";
      this->srcFile << "string msg(\"" << pdata.className << "::initializeOutput : \");\n";
      this->srcFile << "msg += \"can't initialize output field '\";\n";
      this->srcFile << "msg += " << name << ";\n";
      this->srcFile << "msg += \"\'\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "}\n";
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << pdata.className << "::initializeOutput \n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod(const MFrontGenericData& pdata,
							       const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    this->srcFile << "bool\n"
		  << pdata.className 
		  << "::initializeInput(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=data.inputs.begin();p!=data.inputs.end();++p){
      string name;
      if((p2=data.glossaryNames.find(p->name))!=data.glossaryNames.end()){
	name = "GlossaireField::"+p2->second;
      } else if((p2=data.entryNames.find(p->name))!=data.entryNames.end()){
	name = "\""+p2->second+"\"";
      } else {
	name = "\""+p->name+"\"";
      }
      this->srcFile << "if(!this->initializeInputIFieldDouble(this->_ple"
		    << p->name << "," << name;
      this->srcFile << ",\"\",arg)){\n";
      this->srcFile << "string msg(\"" << pdata.className << "::initializeInput : \");\n";
      this->srcFile << "msg += \"can't initialize input field " << p->name << "\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "}\n";
    }
    this->srcFile << endl;
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << pdata.className << "::initializeInput\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod

  void
  MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration(const MFrontGenericData&,
									  const MFrontModelData&)
  {} // end of MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration

  bool
  MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor(const MFrontModelData&) const
  {
    return true;
  } // end of MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor

} // end of namespace mfront
