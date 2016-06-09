/*! 
 * \file  mfront/tests/models/src/MFrontPleiadesModelInterfaceBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 17 jun 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cassert>
#include<sstream>
#include<iterator>

#include"TFEL/System/System.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MFrontPleiadesModelInterfaceBase.hxx"

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

  static std::string
  getDeclarationName(const ModelDescription& md,
		     const std::string& g,
		     const std::string& v){
    if(md.hasGlossaryName(v)){
      return g+"::"+md.getGlossaryName(v);
    }
    return "\""+md.getExternalName(v)+"\"";
  }
  
  MFrontPleiadesModelInterfaceBase::MFrontPleiadesModelInterfaceBase()
  {
    this->hasDefaultConstructor = false;
    this->hasSpecializedDomain  = false;
  } // end of MFrontPleiadesModelInterfaceBase::MFrontPleiadesModelInterfaceBase

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
  MFrontPleiadesModelInterfaceBase::writeOutputFiles(const FileDescription& pdata,
						     const ModelDescription& md)
  {
    using namespace std;
    if(md.className.empty()){
      string msg("MFrontPleiadesModelInterfaceBase::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->setOutputFileNames(md);
    this->generateDomainsList(md);
    this->openOutputFiles();
    this->generateOutputFiles(pdata,md);
    this->closeOutputFiles();
  } // end of MFrontPleiadesModelInterfaceBase::writeOutputFiles

  void
  MFrontPleiadesModelInterfaceBase::generateOutputFiles(const FileDescription& pdata,
							const ModelDescription& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    this->hasDefaultConstructor=true;
    if(this->domains.empty()){
      this->hasDefaultConstructor=false;
    }
    if(!data.parameters.empty()){
      for(p=data.parameters.begin();
	  (p!=data.parameters.end())&&(this->hasDefaultConstructor);++p){
	if(data.defaultValues.find(p->name)==data.defaultValues.end()){
	  this->hasDefaultConstructor = false;
	}
      }
    }
    this->writeHeaderFile(pdata,data);
    this->writeSrcFile(pdata,data);
  } // end of MFrontPleiadesModelInterfaceBase::generateOutputFiles

  void
  MFrontPleiadesModelInterfaceBase::writeHeaderFile(const FileDescription& pdata,
						    const ModelDescription& md)
  {
    using namespace std;
    VarContainer::const_iterator p;
    StaticVarContainer::const_iterator p2;
    set<string>::iterator p4;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << md.className << " PleiadesModel.\n";
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
		     << makeUpperCase(md.className)
		     << "_HH\n";
    this->headerFile << "#define _PLEIADES_"
		     << makeUpperCase(md.className)
		     << "_HH\n\n";

    this->headerFile << "#include<string>\n\n";
    this->headerFile << "#include\"Pleiades/PMetier/IncPMetier.hh\"\n";
    //    this->headerFile << "#include\"Pleiades/PMetier/PModels/MEDModel.hh\"\n";
    if(!md.includes.empty()){
      this->headerFile << md.includes << endl << endl;
    }
    //    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace Pleiades\n{\n";
    this->headerFile << "namespace PMetier\n{\n";
    this->headerFile << "namespace PModels\n{\n\n";
    //    this->headerFile << "class MFRONT_SHAREDOBJ " << md.className << endl;
    this->headerFile << "class " << md.className << endl;
    this->headerFile << ": public IModel\n";
    //    this->headerFile << "protected MEDModel\n";
    this->headerFile << "{\n\n";
    this->headerFile << "public:\n";
    if((!md.parameters.empty())||(this->hasSpecializedDomain)){
      this->headerFile << "//! Constructor to initialize local parameters\n";
      this->headerFile << md.className
		       << "(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    }
    if(this->hasDefaultConstructor){
      this->headerFile << "//! Default constructor\n";
      this->headerFile << md.className << "();\n\n";
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
    this->headerFile << md.className << "(const " 
		     << md.className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << md.className << "&\n"
		     << "operator=(const " 
		     << md.className << "&);\n\n";
    this->headerFile << "static const std::vector<int>&\n"
		     << "getNodesOfCELLZone(Pleiades::PMetier::PMesh::PtrIMesh Mesh," << endl
		     << "const std::string&);" << endl << endl;
    this->writeSpecificPrivateMethodDeclaration(pdata,md);
    // Treat functions
    for(const auto& f : md.functions){
      if(f.modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelInterfaceBase::writeHeaderFile : ");
	msg+="function " + f.name + " does not modify any variable (internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(getDebugMode()){
	this->headerFile << "#line " << f.line << " \"" 
			 << pdata.fileName << "\"\n";
      }
      if(f.modifiedVariables.size()==1){
	this->headerFile << "inline double\n";
	this->headerFile << f.name << "(";
      } else {
	this->headerFile << "inline\n";
	this->headerFile << "void\n";
	this->headerFile << f.name << "(";
	for(p4=f.modifiedVariables.begin();p4!=f.modifiedVariables.end();){
	  this->headerFile << "double&";
	  if((++p4)!=f.modifiedVariables.end()){
	    this->headerFile << ",";
	  }
	}
      }
      if((f.usedVariables.size()==0)&&(!f.useTimeIncrement)){
	if(f.modifiedVariables.size()==1){
	  this->headerFile << "void";
	}
      } else {
	if(f.modifiedVariables.size()!=1){
	  this->headerFile << ",";
	}
	for(p4=f.usedVariables.begin();p4!=f.usedVariables.end();){
	  this->headerFile << "const double";
	  if((++p4)!=f.usedVariables.end()){
	    this->headerFile << ",";
	  }
	}
	if(f.useTimeIncrement){
	  if(f.usedVariables.size()!=0){
	    this->headerFile << ",";
	  }
	  this->headerFile << "const double";
	}
      }
      this->headerFile << ");\n\n";
    }

    if(!md.staticVars.empty()){
      for(p2=md.staticVars.begin();p2!=md.staticVars.end();++p2){
	if(getDebugMode()){
	  this->headerFile << "#line " << p2->lineNumber << " \"" 
			   << pdata.fileName << "\"\n";
	}
	this->headerFile << "static constexpr " << p2->type << " " << p2->name << ";" << endl;
      }
      this->headerFile << endl;
    }
    for(p=md.outputs.begin();p!=md.outputs.end();++p){
      if(getDebugMode()){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << "VARIABLE(Pleiades::PMetier::PField::PtrIFieldDouble,ple" << p->name << ")\n\n";
    }
    this->headerFile << endl;
    for(p=md.inputs.begin();p!=md.inputs.end();++p){
      if(getDebugMode()){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << "VARIABLE(Pleiades::PMetier::PField::PtrIFieldDouble,ple" << p->name << ")\n\n";
    }
    this->headerFile << endl;
    this->headerFile << "private:"<< endl;
    this->headerFile << endl;
    for(p=md.constantMaterialProperties.begin();
	p!=md.constantMaterialProperties.end();++p){
      if(getDebugMode()){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for(p=md.parameters.begin();
	p!=md.parameters.end();++p){
      if(getDebugMode()){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    this->headerFile << endl;
    this->headerFile << "std::set<std::string> domains;" << endl;
    this->headerFile << endl;
    this->headerFile << "}; // end of class " << md.className << endl << endl;
    this->headerFile << "} // end of namespace PModels\n\n";
    this->headerFile << "} // end of namespace PMetier\n\n";
    this->headerFile << "} // end of namespace Pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << makeUpperCase(md.className)
		     << "_HH */\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeHeaderFile(void)

  std::string
  MFrontPleiadesModelInterfaceBase::getGenTypeMethod(const std::string& type) const
  {
    if((type=="double")||(type=="real")){
      return "getDouble";
    } else if(type=="string"){
      return "getString";
    }
    throw(std::runtime_error("MFrontPleiadesModelInterfaceBase::getGenTypeMethod : "
			     "no method associated with type " + type));
  } // end of MFrontPleiadesModelInterfaceBase::getGenTypeMethod

  static std::pair<std::string,unsigned short>
  getFieldAndDepthFromFieldName(const ModelDescription& data,
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
    throw(runtime_error("getFieldAndDepthFromFieldName : "
			"field name '"+n+"' has not been found"));
  } // end of getFieldAndDepthFromFieldName

  void
  MFrontPleiadesModelInterfaceBase::writeSrcFile(const FileDescription& pdata,
						 const ModelDescription& md)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p4;
    StaticVarContainer::const_iterator p10;
    set<string>::const_iterator p12;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << md.className << " PleiadesModel.\n";
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
    if(!md.staticVars.empty()){
      for(p10=md.staticVars.begin();p10!=md.staticVars.end();++p10){
	if(getDebugMode()){
	  this->srcFile << "#line " << p10->lineNumber << " \"" 
			<< pdata.fileName << "\"\n";
	}
	if(p10->type=="short"){
	  this->srcFile << "const short " << md.className << "::" << p10->name 
			<< " = " << static_cast<short>(p10->value) << ";" << endl;
	} else if(p10->type=="ushort"){
	  this->srcFile << "const unsigned short " << md.className << "::" << p10->name 
			<< " = " << static_cast<unsigned short>(p10->value) << ";" << endl;
	} else if(p10->type=="int"){
	  this->srcFile << "const int " << md.className << "::" << p10->name 
			<< " = " << static_cast<int>(p10->value) << ";" << endl;
	} else if(p10->type=="uint"){
	  this->srcFile << "const unsigned int " << md.className << "::" << p10->name 
			<< " = " << static_cast<unsigned int>(p10->value) << ";" << endl;
	} else if(p10->type=="long"){
	  this->srcFile << "const long " << md.className << "::" << p10->name 
			<< " = " << static_cast<long>(p10->value) << ";" << endl;
	} else if(p10->type=="ulong"){
	  this->srcFile << "const unsigned long " << md.className << "::" << p10->name 
			<< " = " << static_cast<unsigned long>(p10->value) << ";" << endl;
	} else if(p10->type=="float"){
	  this->srcFile << "const float " << md.className << "::" << p10->name 
			<< " = " << static_cast<float>(p10->value) << ";" << endl;
	} else if((p10->type=="double")||(p10->type=="real")){
	  this->srcFile << "const double " << md.className << "::" << p10->name 
			<< " = " << static_cast<double>(p10->value) << ";" << endl;
	} else if(p10->type=="ldouble"){
	  this->srcFile << "const long double " << md.className << "::" << p10->name 
			<< " = " << static_cast<long double>(p10->value) << ";" << endl;
	} else {
	  string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
	  msg += "type " + p10->type + " is not a supported type for a static variable. ";
	  msg += "Supported types are short, ushort, int, uint, long, ulong,";
	  msg += "float, double, real and ldouble.\n";
	  msg += "Error at line " + to_string(p10->lineNumber);
	  throw(runtime_error(msg));
	}
      }
      this->srcFile << endl;
    }

    // getNodesOfCELLZone
    this->srcFile << "const std::vector<int>&" << endl;
    this->srcFile << md.className << "::getNodesOfCELLZone(Pleiades::PMetier::PMesh::PtrIMesh Mesh," << endl;
    this->srcFile << "                   const std::string& ZoneName)" << endl;
    this->srcFile << "{" << endl;
    this->srcFile << "  using namespace std ;" << endl;
    this->srcFile << "  using namespace Pleiades::PMetier::PMesh ;" << endl;
    this->srcFile << "  static std::map<std::string,std::vector<int> > nodes;" << endl;
    this->srcFile << "  if(nodes.find(ZoneName)==nodes.end()){" << endl;
    this->srcFile << "    int nbel = Mesh->getNumberOfElements( CELL, ZoneName ) ;" << endl;
    this->srcFile << "    int nbnoel = 0, elem_j = 0, node_i = 0 ;" << endl;
    this->srcFile << "    std::vector<int> NodesStack ;" << endl;
    this->srcFile << "    for( int j=0 ; j<nbel ; j++ ){" << endl;
    this->srcFile << "	  elem_j = Mesh->getElement( CELL, ZoneName, j ) ;" << endl;
    this->srcFile << "	  nbnoel = Mesh->getNumberOfNodesOfElement( CELL, elem_j ) ;" << endl;
    this->srcFile << "	  for( int i=0 ; i<nbnoel ; i++){" << endl;
    this->srcFile << "	    node_i = Mesh->getNodeOfElement( CELL, elem_j, i ) ;" << endl;
    this->srcFile << "	    NodesStack.push_back(node_i) ;" << endl;
    this->srcFile << "	  }" << endl;
    this->srcFile << "    }" << endl;
    this->srcFile << "    int NbNodes = static_cast<int>(NodesStack.size()) ;" << endl;
    this->srcFile << "    bool newnode ;" << endl;
    this->srcFile << "    std::vector<int> NodesOfCELLZone ;" << endl;
    this->srcFile << "    NodesOfCELLZone.push_back(NodesStack.at(0)) ;" << endl;
    this->srcFile << "    for( int i=1 ; i<NbNodes ; i++){" << endl;
    this->srcFile << "	  node_i = NodesStack.at(i) ;" << endl;
    this->srcFile << "	  newnode = true ;" << endl;
    this->srcFile << "	  for(size_t j=0 ; j<NodesOfCELLZone.size() ; j++ ){" << endl;
    this->srcFile << "	    if (NodesOfCELLZone.at(j) == node_i){" << endl;
    this->srcFile << "	      newnode = false ;" << endl;
    this->srcFile << "	      break ;" << endl;
    this->srcFile << "	    }" << endl;
    this->srcFile << "	  }" << endl;
    this->srcFile << "	  if (newnode) { NodesOfCELLZone.push_back(node_i) ; }" << endl;
    this->srcFile << "    }" << endl;
    this->srcFile << "    nodes[ZoneName] = NodesOfCELLZone ;" << endl;
    this->srcFile << "  }" << endl;
    this->srcFile << "  return nodes.at(ZoneName);" << endl;
    this->srcFile << "}" << endl << endl;

    // Constructors
    if(this->hasDefaultConstructor){
      this->srcFile << md.className << "::" 
		    << md.className << "()\n{\n";
      for(p=md.parameters.begin();p!=md.parameters.end();++p){
	p4=md.defaultValues.find(p->name);
	assert(p4!=md.defaultValues.end());
	this->writeAssignDefaultValue(p,p4);
      }
      if(this->initializeDefaultDomainListInConstrutor(md)){
	for(p12=this->domains.begin();p12!=this->domains.end();++p12){
	  this->srcFile << "this->domains.insert(" << *p12 << ");\n";
	}
      }
      this->srcFile << "} // end of " 
		    << md.className << "::" 
		    << md.className <<"\n\n";
    }
    if((!md.parameters.empty())||(this->hasSpecializedDomain)){
      this->srcFile << "//! Constructor to initialize local parameters\n";
      this->srcFile << md.className << "::" 
		    << md.className 
		    << "(const Pleiades::PMetier::IArgumentMetier& arg)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
      this->srcFile << "using namespace Pleiades::PExceptions;\n";
      for(const auto& v : md.parameters){
	const auto name = getDeclarationName(md,"GlossaireParam",v.name);
	this->srcFile << "if(!arg.contains(" << name << ")){\n";
	if((p4=md.defaultValues.find(v.name))!=md.defaultValues.end()){
	  this->writeAssignDefaultValue(p,p4);
	  this->srcFile << "} else {\n";
	} else {
	  this->srcFile << "string msg(\"" << md.className << "::" << md.className << " : \");\n"
			<< "msg += \"can't initialize parameter " << v.name << "\";\n"
			<< "throw(PleiadesError(msg));\n"
			<< "} else {\n";
	}
	this->srcFile << "this->" << v.name << " = arg[" << name << "]." 
		      << this->getGenTypeMethod(v.type) << "();\n";
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
	if(!md.domains.empty()){
	  for(p12=this->domains.begin();p12!=this->domains.end();++p12){
	    this->srcFile << "this->domains.insert(" << *p12 << ");\n";
	  }
	} else {
	  this->srcFile << "string msg(\"" << md.className << "::" << md.className << " : \");\n";
	  this->srcFile << "msg += \"no domain specified\";\n";
	  this->srcFile << "throw(PleiadesError(msg));\n";
	}
	this->srcFile << "}\n";
      }
      this->srcFile <<"} // end of "
		    << md.className << "::" 
		    << md.className <<"\n\n";
    }
    // Functions
    for(const auto& f : md.functions){
      if(f.modifiedVariables.size()==0){
	string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
	msg+="function " + f.name + " does not modify any variable "
	  "(internal error, this shall have been verified long ago).";
	throw(runtime_error(msg));
      }
      if(getDebugMode()){
	this->srcFile << "#line " << f.line << " \"" 
		      << pdata.fileName << "\"\n";
      }
      if(f.modifiedVariables.size()==1){
	this->srcFile << "double\n";
	this->srcFile << md.className << "::" << f.name << "(";
      } else {
	this->srcFile << "void\n";
	this->srcFile << md.className << "::" << f.name << "(";
	for(p12=f.modifiedVariables.begin();p12!=f.modifiedVariables.end();){
	  this->srcFile << "double& " << *p12;
	  if((++p12)!=f.modifiedVariables.end()){
	    this->srcFile << ",\n";
	  }
	}
      }
      if((f.usedVariables.size()==0)&&(f.useTimeIncrement)){
	if(f.modifiedVariables.size()==1){
	  this->srcFile << "void";
	}
      } else {
	if(f.modifiedVariables.size()!=1){
	  this->srcFile << ",";
	}
	for(p12=f.usedVariables.begin();p12!=f.usedVariables.end();){
	  this->srcFile << "const double " << *p12;
	  if((++p12)!=f.usedVariables.end()){
	    this->srcFile << ",\n";
	  }
	}
	if(f.useTimeIncrement){
	  if(f.usedVariables.size()!=0){
	    this->srcFile << ",\n";
	  }
	  this->srcFile << "const double dt";
	}
      }
      this->srcFile << ")\n";
      this->srcFile << "{\n";
      this->srcFile << "using namespace std;\n";
      writeMaterialLaws("MFrontPleiadesModelInterfaceBase::writeSrcFile",
			this->srcFile,md.materialLaws);		      
      if(f.modifiedVariables.size()==1){
	this->srcFile << "double " << *(f.modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << f.body;
      if(f.modifiedVariables.size()==1){
	this->srcFile << "return " << *(f.modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << "} // end of " << md.className << "::" << f.name << "\n\n";
    }
    // initializeOutput
    this->writeInitializeOutputMethod(md);
    // initializeInput
    this->writeInitializeInputMethod(md);
    // initializeParamaters
    this->writeInitializeParametersMethod(md);
    // initialize
    this->writeInitializeMethod(md);
    // execute
    this->srcFile << "bool\n"
		  << md.className
		  << "::execute(void){\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PTime;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PMesh;\n\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n\n";
//     for(p=data.outputs.begin();p!=data.outputs.end();++p){
//       this->srcFile << "MEDField " << p->name 
// 		    << " = *(static_cast<MEDFieldPtr>(this->_ple" << p->name << "->getMed()));\n";
//       if((p5=data.depths.find(p->name))!=data.depths.end()){
// 	for(j=1;j<=p5->second;++j){
// 	  ostringstream converter;
// 	  converter << j;
// 	  string varName = p->name+"_"+converter.str();
// 	  this->srcFile << "MEDField " << varName 
// 			<< " = *(static_cast<MEDFieldPtr>(this->_ple" 
// 			<< p->name << "->getFieldPreceding("<< j <<")->getMed()));\n";
// 	}
//       }
//     }
//     for(p=data.inputs.begin();p!=data.inputs.end();++p){
//       this->srcFile << "MEDField " << p->name 
// 		    << " = *(static_cast<MEDFieldPtr>(this->_ple" << p->name << "->getMed()));\n";
//       if((p5=data.depths.find(p->name))!=data.depths.end()){
// 	for(j=1;j<=p5->second;++j){
// 	  this->srcFile << "MEDField " << p->name << "_" << j
// 			<< " = *(static_cast<MEDFieldPtr>(this->_ple" 
// 			<< p->name << "->getFieldPreceding("<< j <<")->getMed()));\n";
// 	}
//       }
//     }
    this->srcFile << "const double t  = ITime::GlobalTime->get(Glossaire::Time);\n";
    this->srcFile << "const double dt = ITime::GlobalTime->get(Glossaire::TimeStep);\n";
    this->srcFile << "static_cast<void>(t);  // suppress unused variable warning\n";
    this->srcFile << "static_cast<void>(dt); // suppress unused variable warning\n\n";

    // calling functions
    if(md.functions.empty()){
      string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
      msg += "no function defined";
      throw(runtime_error(msg));
    }
    if(md.outputs.size()==0){
      string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
      msg += "no output defined";
      throw(runtime_error(msg));
    }
    this->srcFile << "PtrIMesh mesh = this->_ple" << md.outputs.front().name << "->getMesh();\n";
    this->srcFile << "std::set<string>::const_iterator ptr;\n";
    this->srcFile << "if(this->domains.empty()){\n";
    this->srcFile << "string msg(\"" << md.className << "::execute : \");\n";
    this->srcFile << "msg += \"no domain specified\";\n";
    this->srcFile << "throw(PleiadesError(msg));\n";
    this->srcFile << "}\n";
//     this->srcFile << "map<string,vector<int> >::const_iterator ptr2;\n";
//     this->srcFile << "vector<int>::const_iterator ptr3;\n";
    this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
    this->buildDomainName();
    this->srcFile << "const vector<int>& nodes = "<< md.className
    		  << "::getNodesOfCELLZone(mesh,*ptr);\n";
//     this->srcFile << "ptr2 = this->nodesOfZones.find(*ptr);\n";
//     this->srcFile << "if(ptr2==this->nodesOfZones.end()){\n";
//     this->srcFile << "string msg(\"" << md.className << "::execute : \");\n";
//     this->srcFile << "msg += \"internal error (no group named '\"+*ptr+\"' defined\";\n";
//     this->srcFile << "throw(PleiadesError(msg));\n";
//     this->srcFile << "}\n";
//     this->srcFile << "const auto& nodes = ptr2->second;\n";
    this->srcFile << "for(int node_k=0;node_k!=static_cast<int>(nodes.size());++node_k){\n";
    this->srcFile << "const int gnode_k=nodes[node_k];\n";
    this->writeFunctionCall(md,md.functions);
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << md.className << "::execute\n\n";
    
    this->srcFile << "void\n" << md.className 
		  << "::executeInitialPostProcessingTasks(const bool)\n{} // end of " << md.className 
		  << "::executeInitialPostProcessingTasks\n\n";

    this->srcFile << "void\n" << md.className 
		  << "::executePostProcessingTasks(const bool)\n{} // end of " << md.className 
		  << "::executePostProcessingTasks\n\n";

    if(!md.parameters.empty()){
      this->srcFile << "using Pleiades::PExamplars::TemplateModelClassProxy;\n";
      this->srcFile << "TemplateModelClassProxy<"
		    << md.className
		    << "> proxy" << md.className << "(\""
		    << md.className << "\");\n";
    }
    if(this->hasDefaultConstructor){
      this->srcFile << "GENERATE_PROXY(IModel," << md.className << ");\n\n";
    }
    this->srcFile << "} // end of namespace PModels\n\n";
    this->srcFile << "} // end of namespace PMetier\n\n";
    this->srcFile << "} // end of namespace Pleiades\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeSrcFile(void)

  void
  MFrontPleiadesModelInterfaceBase::getTargetsDescription(TargetsDescription& td,
							  const ModelDescription&  md)
  {
    std::string lib;
    if(md.library.empty()){
      if(!md.material.empty()){
	lib = "lib"+this->getApplicationName()+md.material+"Models";
      } else {
	lib = "lib"+this->getApplicationName()+"MaterialModels";
      }
    } else {
      lib = md.library;
    }
    td[lib].sources.push_back(this->srcFileName);
    td[lib].epts.push_back(md.className);
    td.headers.push_back(this->headerFileName);
    td[lib].cppflags.push_back("`pleiades-config --includes`\n");
    td[lib].ldflags.push_back("`pleiades-config --libs` -lm");
  } // end of MFrontPleiadesModelInterfaceBase::getTargetsDescription
  
  void
  MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue(const VarContainer::const_iterator p,
							    const std::map<std::string,std::string>::const_iterator p4)
  {
    using namespace std;
    if((p->type=="string")||(p->type=="double")||(p->type=="real")){
      this->srcFile << "this->" << p->name << " = "  << p4->second << ";" << endl;
    } else {
      string msg("MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue : ");
      msg+="type "+p->type+" is not supported.\n";
      throw(runtime_error(msg));
    }   
  } // end of MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue

  void
  MFrontPleiadesModelInterfaceBase::writeGetGlobalParameter(const VarHandler& v,
							    const ModelDescription& md)
  {
    const auto name = getDeclarationName(md,"GlossaireParam",v.name);
    const auto p=md.defaultValues.find(v.name);
    this->srcFile << "if(!arg.contains(" << name << ")){\n";
    if(p!=md.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";\n"
		    << "} else {\n";
    } else {
      this->srcFile << "string msg(\"" << md.className << "::initializeParameters : \");\n"
		    << "msg += \"can't initialize parameter '" << v.name << "' using '\";\n"
		    << "msg += " << name << ";\n"
		    << "msg += '\\'';\n"
		    << "throw(PleiadesError(msg));\n"
		    << "} else {\n";
    }
    this->srcFile << "this->" << v.name << " = arg[" << name << "]." 
		  << this->getGenTypeMethod(v.type) << "();\n"
		  << "}\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeGetGlobalParameter

  void
  MFrontPleiadesModelInterfaceBase::writeGetConstantMaterialProperty(const VarHandler& v,
								     const ModelDescription& md)
  {
    const auto name = getDeclarationName(md,"GlossaireParam",v.name);
    const auto p=md.defaultValues.find(v.name);
    this->srcFile << "if(arg.contains(" << name << ")){\n"
		  << "this->" << v.name << " = arg[" << name << "]." 
		  << this->getGenTypeMethod(v.type) << "();\n"
		  << "} else {\n";
    if(p!=md.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";\n";
    } else {
      this->srcFile << "string msg(\"" << md.className << "::initializeParameters : \");\n"
		    << "msg += \"can't initialize constant material property  '"
		    << v.name << "' using '\";\n"
		    << "msg += " << name << ";\n"
		    << "msg += '\\'';\n"
		    << "throw(PleiadesError(msg));\n";
    }
    this->srcFile << "}\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeGetConstantMaterialProperty

  void
  MFrontPleiadesModelInterfaceBase::writeFunctionCall(const ModelDescription& data,
						      const std::vector<ModelDescription::Function>& functions)
  {
    using namespace std;
    set<string> usedVariables;
    set<string> modifiedVariables;
    vector<ModelDescription::Function>::const_iterator p;
    set<string>::const_iterator p2;
    set<string>::const_iterator p3;
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
	const auto& fn = getFieldAndDepthFromFieldName(data,*p2);
	if(fn.second==0){
	  this->srcFile << "const double tmp_" << *p2 << " = this->_ple" 
			<< fn.first << "->getValueType(gnode_k,0);\n";
	} else {
	  this->srcFile << "const double tmp_" << *p2 << " = this->_ple" 
			<< fn.first << "->getValuePrecedingType(" << fn.second << ",gnode_k);\n";
	}
      }
    }
    for(p2=modifiedVariables.begin();p2!=modifiedVariables.end();++p2){
      if(data.fieldNames.find(*p2)!=data.fieldNames.end()){
	const auto& fn = getFieldAndDepthFromFieldName(data,*p2);
	if(fn.second==0){
	  this->srcFile << "double tmp_" << *p2 << " = this->_ple" 
			<< fn.first << "->getValueType(gnode_k,0);\n";
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
	const auto& fn = getFieldAndDepthFromFieldName(data,*p2);
	if(fn.second==0){
	  this->srcFile << "this->_ple" << fn.first << "->setValueType(gnode_k,tmp_" << *p2 << ",0);\n";
	} else {
	  string msg("MFrontPleiadesModelInterfaceBase::writeFunctionCall : ");
	  msg += "variable '"+*p2+"' shall not be modified";
	  throw(runtime_error(msg));
	}
	//      this->srcFile << *p2 << ".setValueType(gnode_k,tmp_" << *p2 << ",1);\n";
    }
  }

  void
  MFrontPleiadesModelInterfaceBase::generateDomainsList(const ModelDescription& data)
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
  MFrontPleiadesModelInterfaceBase::writeInitializeMethod(const ModelDescription& md)
  {
    this->srcFile << "bool\n"
		  << md.className 
		  << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "this->initializeOutput(arg);\n";
    this->srcFile << "this->initializeInput(arg);\n";
    this->srcFile << "this->initializeParameters(arg);\n";
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << md.className << "::initialize\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod(const ModelDescription& md)
  {
    this->srcFile << "bool\n"
		  << md.className 
		  << "::initializeParameters(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(auto p=md.constantMaterialProperties.cbegin();p!=md.constantMaterialProperties.cend();++p){
      this->writeGetConstantMaterialProperty(*p,md);
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << md.className << "::initializeParameters\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod(const ModelDescription& md)
  {
    this->srcFile << "bool\n"
		  << md.className
		  << "::initializeOutput(const Pleiades::PMetier::IArgumentMetier& arg)"
		  << "{\n"
		  << "using namespace std;\n"
		  << "using namespace Pleiades::PUtilitaires;\n"
		  << "using namespace Pleiades::PExceptions;\n"
		  << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(const auto& v : md.outputs){
      const auto name = getDeclarationName(md,"GlossaireField",v.name);
      const auto p    = md.initialValues.find(v.name);
      this->srcFile << "if(!this->initializeOutputIFieldDouble(arg," << name;
      this->srcFile << ",\nthis->_ple" << v.name << ",\"\",";
      if(p!=md.initialValues.end()){
	this->srcFile << p->second;
      } else {
	this->srcFile << "0";
      }
      this->srcFile << ")){\n"
		    << "string msg(\"" << md.className << "::initializeOutput : \");\n"
		    << "msg += \"can't initialize output field '\";\n"
		    << "msg += " << name << ";\n"
		    << "msg += \"\'\";\n"
		    << "throw(PleiadesError(msg));\n"
		    << "}\n";
    }
    this->srcFile << "return true;\n"
		  << "} // end of " << md.className << "::initializeOutput \n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod(const ModelDescription& md)
  {
    this->srcFile << "bool\n"
		  << md.className 
		  << "::initializeInput(const Pleiades::PMetier::IArgumentMetier& arg)"
		  << "{\n"
		  << "using namespace std;\n"
		  << "using namespace Pleiades::PUtilitaires;\n"
		  << "using namespace Pleiades::PExceptions;\n"
		  << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(const auto& v : md.inputs){
      const auto name = getDeclarationName(md,"GlossaireField",v.name);
      this->srcFile << "if(!this->initializeInputIFieldDouble(this->_ple"
		    << v.name << "," << name
		    << ",\"\",arg)){\n"
		    << "string msg(\"" << md.className << "::initializeInput : \");\n"
		    << "msg += \"can't initialize input field " << v.name << "\";\n"
		    << "throw(PleiadesError(msg));\n"
		    << "}\n";
    }
    this->srcFile << '\n'
		  << "return true;\n"
		  << "} // end of " << md.className << "::initializeInput\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod

  void
  MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration(const FileDescription&,
									  const ModelDescription&)
  {} // end of MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration

  bool
  MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor(const ModelDescription&) const
  {
    return true;
  } // end of MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor

} // end of namespace mfront
