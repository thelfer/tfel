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
#include"MFront/DSLUtilities.hxx"
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
  MFrontPleiadesModelInterfaceBase::writeOutputFiles(const GenericData& pdata,
						     const ModelData& mdata)
  {
    using namespace std;
    if(mdata.className.empty()){
      string msg("MFrontPleiadesModelInterfaceBase::writeOutputFiles : ");
      msg += "no behaviour name defined.";
      throw(runtime_error(msg));      
    }
    this->setOutputFileNames(mdata);
    this->generateDomainsList(mdata);
    this->openOutputFiles();
    this->generateOutputFiles(pdata,mdata);
    this->closeOutputFiles();
  } // end of MFrontPleiadesModelInterfaceBase::writeOutputFiles

  void
  MFrontPleiadesModelInterfaceBase::generateOutputFiles(const GenericData& pdata,
							const ModelData& data)
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
  MFrontPleiadesModelInterfaceBase::writeHeaderFile(const GenericData& pdata,
						    const ModelData& mdata)
  {
    using namespace std;
    VarContainer::const_iterator p;
    StaticVarContainer::const_iterator p2;
    ModelData::FunctionContainer::const_iterator p3;
    set<string>::iterator p4;
    set<string>::iterator p5;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << mdata.className << " PleiadesModel.\n";
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
		     << makeUpperCase(mdata.className)
		     << "_HH\n";
    this->headerFile << "#define _PLEIADES_"
		     << makeUpperCase(mdata.className)
		     << "_HH\n\n";

    this->headerFile << "#include<string>\n\n";
    this->headerFile << "#include\"Pleiades/PMetier/IncPMetier.hh\"\n";
    //    this->headerFile << "#include\"Pleiades/PMetier/PModels/MEDModel.hh\"\n";
    if(!mdata.includes.empty()){
      this->headerFile << mdata.includes << endl << endl;
    }
    //    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace Pleiades\n{\n";
    this->headerFile << "namespace PMetier\n{\n";
    this->headerFile << "namespace PModels\n{\n\n";
    //    this->headerFile << "class MFRONT_SHAREDOBJ " << mdata.className << endl;
    this->headerFile << "class " << mdata.className << endl;
    this->headerFile << ": public IModel\n";
    //    this->headerFile << "protected MEDModel\n";
    this->headerFile << "{\n\n";
    this->headerFile << "public:\n";
    if((!mdata.localParameters.empty())||(this->hasSpecializedDomain)){
      this->headerFile << "//! Constructor to initialize local parameters\n";
      this->headerFile << mdata.className
		       << "(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    }
    if(this->hasDefaultConstructor){
      this->headerFile << "//! Default constructor\n";
      this->headerFile << mdata.className << "();\n\n";
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
    this->headerFile << mdata.className << "(const " 
		     << mdata.className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << mdata.className << "&\n"
		     << "operator=(const " 
		     << mdata.className << "&);\n\n";
    this->headerFile << "static const std::vector<int>&\n"
		     << "getNodesOfCELLZone(Pleiades::PMetier::PMesh::PtrIMesh Mesh," << endl
		     << "const std::string&);" << endl << endl;
    this->writeSpecificPrivateMethodDeclaration(pdata,mdata);
    // Treat functions
    for(p3=mdata.functions.begin();p3!=mdata.functions.end();++p3){
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

    if(!mdata.staticVars.empty()){
      for(p2=mdata.staticVars.begin();p2!=mdata.staticVars.end();++p2){
	if(this->debugMode){
	  this->headerFile << "#line " << p2->lineNumber << " \"" 
			   << pdata.fileName << "\"\n";
	}
	this->headerFile << "static constexpr " << p2->type << " " << p2->name << ";" << endl;
      }
      this->headerFile << endl;
    }
    for(p=mdata.outputs.begin();p!=mdata.outputs.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << "VARIABLE(Pleiades::PMetier::PField::PtrIFieldDouble,ple" << p->name << ")\n\n";
    }
    this->headerFile << endl;
    for(p=mdata.inputs.begin();p!=mdata.inputs.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << "VARIABLE(Pleiades::PMetier::PField::PtrIFieldDouble,ple" << p->name << ")\n\n";
    }
    this->headerFile << endl;
    this->headerFile << "private:"<< endl;
    this->headerFile << endl;
    for(p=mdata.globalParameters.begin();p!=mdata.globalParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for(p=mdata.constantMaterialProperties.begin();
	p!=mdata.constantMaterialProperties.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for(p=mdata.localParameters.begin();
	p!=mdata.localParameters.end();++p){
      if(this->debugMode){
	this->headerFile << "#line " << p->lineNumber << " \"" 
			 << pdata.fileName << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    this->headerFile << endl;
    this->headerFile << "std::set<std::string> domains;" << endl;
    this->headerFile << endl;
    this->headerFile << "}; // end of class " << mdata.className << endl << endl;
    this->headerFile << "} // end of namespace PModels\n\n";
    this->headerFile << "} // end of namespace PMetier\n\n";
    this->headerFile << "} // end of namespace Pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << makeUpperCase(mdata.className)
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
    } else {
      string msg("MFrontPleiadesModelInterfaceBase::getGenTypeMethod : ");
      msg += "no method associated with type " + type;
      throw(runtime_error(msg));
    }
    return "";
  } // end of MFrontPleiadesModelInterfaceBase::getGenTypeMethod

  static std::pair<std::string,unsigned short>
  getFieldAndDepthFromFieldName(const ModelData& data,
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
  MFrontPleiadesModelInterfaceBase::writeSrcFile(const GenericData& pdata,
						 const ModelData& mdata)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    map<string,string>::const_iterator p4;
    map<string,unsigned short>::const_iterator p5;
    StaticVarContainer::const_iterator p10;
    ModelData::FunctionContainer::const_iterator p11;
    set<string>::const_iterator p12;
    std::map<std::string,std::vector<ModelData::Function> >::iterator p13;
    set<string>::const_iterator p16;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << mdata.className << " PleiadesModel.\n";
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
    if(!mdata.staticVars.empty()){
      for(p10=mdata.staticVars.begin();p10!=mdata.staticVars.end();++p10){
	if(this->debugMode){
	  this->srcFile << "#line " << p10->lineNumber << " \"" 
			<< pdata.fileName << "\"\n";
	}
	if(p10->type=="short"){
	  this->srcFile << "const short " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<short>(p10->value) << ";" << endl;
	} else if(p10->type=="ushort"){
	  this->srcFile << "const unsigned short " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<unsigned short>(p10->value) << ";" << endl;
	} else if(p10->type=="int"){
	  this->srcFile << "const int " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<int>(p10->value) << ";" << endl;
	} else if(p10->type=="uint"){
	  this->srcFile << "const unsigned int " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<unsigned int>(p10->value) << ";" << endl;
	} else if(p10->type=="long"){
	  this->srcFile << "const long " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<long>(p10->value) << ";" << endl;
	} else if(p10->type=="ulong"){
	  this->srcFile << "const unsigned long " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<unsigned long>(p10->value) << ";" << endl;
	} else if(p10->type=="float"){
	  this->srcFile << "const float " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<float>(p10->value) << ";" << endl;
	} else if((p10->type=="double")||(p10->type=="real")){
	  this->srcFile << "const double " << mdata.className << "::" << p10->name 
			<< " = " << static_cast<double>(p10->value) << ";" << endl;
	} else if(p10->type=="ldouble"){
	  this->srcFile << "const long double " << mdata.className << "::" << p10->name 
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
    this->srcFile << mdata.className << "::getNodesOfCELLZone(Pleiades::PMetier::PMesh::PtrIMesh Mesh," << endl;
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
      this->srcFile << mdata.className << "::" 
		    << mdata.className << "()\n{\n";
      for(p=mdata.localParameters.begin();p!=mdata.localParameters.end();++p){
	string name;
	if((p4=mdata.glossaryNames.find(p->name))!=mdata.glossaryNames.end()){
	  name = "GlossaireParam::" + p4->second;
	} else if((p4=mdata.entryNames.find(p->name))!=mdata.entryNames.end()){
	  name = "\""+p4->second+"\"";
	} else {
	  name = "\""+p->name+"\"";
	}
	p4=mdata.defaultValues.find(p->name);
	assert(p4!=mdata.defaultValues.end());
	this->writeAssignDefaultValue(p,p4);
      }
      if(this->initializeDefaultDomainListInConstrutor(mdata)){
	for(p12=this->domains.begin();p12!=this->domains.end();++p12){
	  this->srcFile << "this->domains.insert(" << *p12 << ");\n";
	}
      }
      this->srcFile << "} // end of " 
		    << mdata.className << "::" 
		    << mdata.className <<"\n\n";
    }
    if((!mdata.localParameters.empty())||(this->hasSpecializedDomain)){
      this->srcFile << "//! Constructor to initialize local parameters\n";
      this->srcFile << mdata.className << "::" 
		    << mdata.className 
		    << "(const Pleiades::PMetier::IArgumentMetier& arg)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
      this->srcFile << "using namespace Pleiades::PExceptions;\n";
      for(p=mdata.localParameters.begin();p!=mdata.localParameters.end();++p){
	string name;
	if((p4=mdata.glossaryNames.find(p->name))!=mdata.glossaryNames.end()){
	  name = "GlossaireParam::" + p4->second;
	} else if((p4=mdata.entryNames.find(p->name))!=mdata.entryNames.end()){
	  name = "\""+p4->second+"\"";
	} else {
	  name = "\""+p->name+"\"";
	}
	this->srcFile << "if(!arg.contains(" << name << ")){\n";
	if((p4=mdata.defaultValues.find(p->name))!=mdata.defaultValues.end()){
	  this->writeAssignDefaultValue(p,p4);
	  this->srcFile << "} else {\n";
	} else {
	  this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
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
	if(!mdata.domains.empty()){
	  for(p12=this->domains.begin();p12!=this->domains.end();++p12){
	    this->srcFile << "this->domains.insert(" << *p12 << ");\n";
	  }
	} else {
	  this->srcFile << "string msg(\"" << mdata.className << "::" << mdata.className << " : \");\n";
	  this->srcFile << "msg += \"no domain specified\";\n";
	  this->srcFile << "throw(PleiadesError(msg));\n";
	}
	this->srcFile << "}\n";
      }
      this->srcFile <<"} // end of "
		    << mdata.className << "::" 
		    << mdata.className <<"\n\n";
    }
    // Functions
    for(p11=mdata.functions.begin();p11!=mdata.functions.end();++p11){
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
	this->srcFile << mdata.className << "::" << p11->name << "(";
      } else {
	this->srcFile << "void\n";
	this->srcFile << mdata.className << "::" << p11->name << "(";
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
			this->srcFile,mdata.materialLaws);		      
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "double " << *(p11->modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << p11->body;
      if(p11->modifiedVariables.size()==1){
	this->srcFile << "return " << *(p11->modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << "} // end of " << mdata.className << "::" << p11->name << "\n\n";
    }
    // initializeOutput
    this->writeInitializeOutputMethod(mdata);
    // initializeInput
    this->writeInitializeInputMethod(mdata);
    // initializeParamaters
    this->writeInitializeParametersMethod(mdata);
    // initialize
    this->writeInitializeMethod(mdata);
    // execute
    this->srcFile << "bool\n"
		  << mdata.className
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
    if(mdata.functions.empty()){
      string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
      msg += "no function defined";
      throw(runtime_error(msg));
    }
    if(mdata.outputs.size()==0){
      string msg("MFrontPleiadesModelInterfaceBase::writeSrcFile : ");
      msg += "no output defined";
      throw(runtime_error(msg));
    }
    this->srcFile << "PtrIMesh mesh = this->_ple" << mdata.outputs.front().name << "->getMesh();\n";
    this->srcFile << "std::set<string>::const_iterator ptr;\n";
    this->srcFile << "if(this->domains.empty()){\n";
    this->srcFile << "string msg(\"" << mdata.className << "::execute : \");\n";
    this->srcFile << "msg += \"no domain specified\";\n";
    this->srcFile << "throw(PleiadesError(msg));\n";
    this->srcFile << "}\n";
//     this->srcFile << "map<string,vector<int> >::const_iterator ptr2;\n";
//     this->srcFile << "vector<int>::const_iterator ptr3;\n";
    this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
    this->buildDomainName();
    this->srcFile << "const vector<int>& nodes = "<< mdata.className
    		  << "::getNodesOfCELLZone(mesh,*ptr);\n";
//     this->srcFile << "ptr2 = this->nodesOfZones.find(*ptr);\n";
//     this->srcFile << "if(ptr2==this->nodesOfZones.end()){\n";
//     this->srcFile << "string msg(\"" << mdata.className << "::execute : \");\n";
//     this->srcFile << "msg += \"internal error (no group named '\"+*ptr+\"' defined\";\n";
//     this->srcFile << "throw(PleiadesError(msg));\n";
//     this->srcFile << "}\n";
//     this->srcFile << "const vector<int>& nodes = ptr2->second;\n";
    this->srcFile << "for(int node_k=0;node_k!=static_cast<int>(nodes.size());++node_k){\n";
    this->srcFile << "const int gnode_k=nodes[node_k];\n";
    this->writeFunctionCall(mdata,mdata.functions);
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << mdata.className << "::execute\n\n";
    
    this->srcFile << "void\n" << mdata.className 
		  << "::executeInitialPostProcessingTasks(const bool)\n{} // end of " << mdata.className 
		  << "::executeInitialPostProcessingTasks\n\n";

    this->srcFile << "void\n" << mdata.className 
		  << "::executePostProcessingTasks(const bool)\n{} // end of " << mdata.className 
		  << "::executePostProcessingTasks\n\n";

    if(!mdata.localParameters.empty()){
      this->srcFile << "using Pleiades::PExamplars::TemplateModelClassProxy;\n";
      this->srcFile << "TemplateModelClassProxy<"
		    << mdata.className
		    << "> proxy" << mdata.className << "(\""
		    << mdata.className << "\");\n";
    }
    if(this->hasDefaultConstructor){
      this->srcFile << "GENERATE_PROXY(IModel," << mdata.className << ");\n\n";
    }
    this->srcFile << "} // end of namespace PModels\n\n";
    this->srcFile << "} // end of namespace PMetier\n\n";
    this->srcFile << "} // end of namespace Pleiades\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeSrcFile(void)

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getGeneratedSources(const ModelData& mdata)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib;
    if(mdata.library.empty()){
      if(!mdata.material.empty()){
	lib = "lib"+this->getApplicationName()+mdata.material+"Models";
      } else {
	lib = "lib"+this->getApplicationName()+"MaterialModels";
      }
    } else {
      lib = mdata.library;
    }
    src[lib].push_back(this->srcFileName);
    return src;
  } // end of MFrontPleiadesModelInterfaceBase::getGeneratedSources
  
  std::vector<std::string>
  MFrontPleiadesModelInterfaceBase::getGeneratedIncludes(const ModelData&)
  {
    using namespace std;
    vector<string> inc;
    inc.push_back(this->headerFileName);
    return inc;
  } // end of MFrontPleiadesModelInterfaceBase::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getGlobalIncludes(const ModelData& mdata)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib;
    if(mdata.library.empty()){
      if(!mdata.material.empty()){
	lib = "lib"+this->getApplicationName()+mdata.material+"Models";
      } else {
	lib = "lib"+this->getApplicationName()+"MaterialModels";
      }
    } else {
      lib = mdata.library;
    }
    incs[lib].push_back("`pleiades-config --includes`\n");
    return incs;
  } // end of MFrontPleiadesModelInterfaceBase::getGlobalIncludes
  
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getGlobalDependencies(const ModelData& mdata)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string lib;
    if(mdata.library.empty()){
      if(!mdata.material.empty()){
	lib = "lib"+this->getApplicationName()+mdata.material+"Models";
      } else {
	lib = "lib"+this->getApplicationName()+"MaterialModels";
      }
    } else {
      lib = mdata.library;
    }
    libs[lib].push_back("`pleiades-config --libs` -lm");
    return libs;
  } // end of MFrontPleiadesModelInterfaceBase::getGlobalDependencies
    
  std::map<std::string,std::vector<std::string> >
  MFrontPleiadesModelInterfaceBase::getLibrariesDependencies(const ModelData&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontPleiadesModelInterfaceBase::getLibrariesDependencies

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
							    const ModelData& mdata)
  {
    using namespace std;
    string name;
    map<string,string>::const_iterator p;
    if((p=mdata.glossaryNames.find(v.name))!=mdata.glossaryNames.end()){
      name = "GlossaireParam::" + p->second;
    } else if((p=mdata.entryNames.find(v.name))!=mdata.entryNames.end()){
      name = "\""+p->second+"\"";
    } else {
      name = "\""+v.name+"\"";
    }
    this->srcFile << "if(!arg.contains(" << name << ")){\n";
    if((p=mdata.defaultValues.find(v.name))!=mdata.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";" << endl;
      this->srcFile << "} else {\n";
    } else {
      this->srcFile << "string msg(\"" << mdata.className << "::initializeParameters : \");\n";
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
								     const ModelData& mdata)
  {
    using namespace std;
    string name;
    string name2;
    map<string,string>::const_iterator p;
    if((p=mdata.glossaryNames.find(v.name))!=mdata.glossaryNames.end()){
      name  = "GlossaireParam::" + p->second;
    } else if((p=mdata.entryNames.find(v.name))!=mdata.entryNames.end()){
      name = "\""+p->second+"\"";
    } else {
      name = "\""+v.name+"\"";
    }
    this->srcFile << "if(arg.contains(" << name << ")){\n";
    this->srcFile << "this->" << v.name << " = arg[" << name << "]." 
		  << this->getGenTypeMethod(v.type) << "();\n";
    this->srcFile << "} else {\n";
    if((p=mdata.defaultValues.find(v.name))!=mdata.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";" << endl;
    } else {
      this->srcFile << "string msg(\"" << mdata.className << "::initializeParameters : \");\n";
      this->srcFile << "msg += \"can't initialize constant material property  '"
		    << v.name << "' using '\";\n";
      this->srcFile << "msg += " << name << ";\n";
      this->srcFile << "msg += '\\'';\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
    }
    this->srcFile << "}\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeGetConstantMaterialProperty

  void
  MFrontPleiadesModelInterfaceBase::writeFunctionCall(const ModelData& data,
						      const std::vector<ModelData::Function>& functions)
  {
    using namespace std;
    typedef unsigned short ushort;
    set<string> usedVariables;
    set<string> modifiedVariables;
    vector<ModelData::Function>::const_iterator p;
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
			<< fn.first << "->getValueType(gnode_k,0);\n";
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
	const pair<string,ushort>& fn = getFieldAndDepthFromFieldName(data,*p2);
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
  MFrontPleiadesModelInterfaceBase::generateDomainsList(const ModelData& data)
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
  MFrontPleiadesModelInterfaceBase::writeInitializeMethod(const ModelData& mdata)
  {
    this->srcFile << "bool\n"
		  << mdata.className 
		  << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "this->initializeOutput(arg);\n";
    this->srcFile << "this->initializeInput(arg);\n";
    this->srcFile << "this->initializeParameters(arg);\n";
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << mdata.className << "::initialize\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod(const ModelData& mdata)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    this->srcFile << "bool\n"
		  << mdata.className 
		  << "::initializeParameters(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=mdata.globalParameters.begin();p!=mdata.globalParameters.end();++p){
      this->writeGetGlobalParameter(*p,mdata);
    }
    for(p=mdata.constantMaterialProperties.begin();p!=mdata.constantMaterialProperties.end();++p){
      this->writeGetConstantMaterialProperty(*p,mdata);
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << mdata.className << "::initializeParameters\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod(const ModelData& mdata)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    this->srcFile << "bool\n"
		  << mdata.className
		  << "::initializeOutput(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=mdata.outputs.begin();p!=mdata.outputs.end();++p){
      string name;
      if((p2=mdata.glossaryNames.find(p->name))!=mdata.glossaryNames.end()){
	name = "GlossaireField::"+p2->second;
      } else if((p2=mdata.entryNames.find(p->name))!=mdata.entryNames.end()){
	name = "\""+p2->second+"\"";
      } else {
	name = "\""+p->name+"\"";
      }
      this->srcFile << "if(!this->initializeOutputIFieldDouble(arg," << name;
      this->srcFile << ",\nthis->_ple" << p->name << ",\"\",";
      if((p3=mdata.initialValues.find(p->name))!=mdata.initialValues.end()){
	this->srcFile << p3->second;
      } else {
	this->srcFile << "0.";
      }
      this->srcFile << ")){\n";
      this->srcFile << "string msg(\"" << mdata.className << "::initializeOutput : \");\n";
      this->srcFile << "msg += \"can't initialize output field '\";\n";
      this->srcFile << "msg += " << name << ";\n";
      this->srcFile << "msg += \"\'\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "}\n";
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << mdata.className << "::initializeOutput \n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod

  void
  MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod(const ModelData& mdata)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    this->srcFile << "bool\n"
		  << mdata.className 
		  << "::initializeInput(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=mdata.inputs.begin();p!=mdata.inputs.end();++p){
      string name;
      if((p2=mdata.glossaryNames.find(p->name))!=mdata.glossaryNames.end()){
	name = "GlossaireField::"+p2->second;
      } else if((p2=mdata.entryNames.find(p->name))!=mdata.entryNames.end()){
	name = "\""+p2->second+"\"";
      } else {
	name = "\""+p->name+"\"";
      }
      this->srcFile << "if(!this->initializeInputIFieldDouble(this->_ple"
		    << p->name << "," << name;
      this->srcFile << ",\"\",arg)){\n";
      this->srcFile << "string msg(\"" << mdata.className << "::initializeInput : \");\n";
      this->srcFile << "msg += \"can't initialize input field " << p->name << "\";\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
      this->srcFile << "}\n";
    }
    this->srcFile << endl;
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << mdata.className << "::initializeInput\n\n";
  } // end of MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod

  void
  MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration(const GenericData&,
									  const ModelData&)
  {} // end of MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration

  bool
  MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor(const ModelData&) const
  {
    return true;
  } // end of MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor

} // end of namespace mfront
