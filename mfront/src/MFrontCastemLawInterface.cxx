/*!
 * \file   MFrontCastemLawInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontCastemLawInterface.hxx"

namespace mfront
{

  std::string 
  MFrontCastemLawInterface::toString(const unsigned short src)
  {
    std::ostringstream os;
    os << src;
    return os.str();
  }

  std::string
  MFrontCastemLawInterface::getName(void)
  {
    return "castem";
  }

  MFrontCastemLawInterface::MFrontCastemLawInterface()
    : verboseMode(false),
      debugMode(false),
      warningMode(false)
  {}

  void
  MFrontCastemLawInterface::reset(void)
  {} // end of MFrontCastemLawInterface::reset(void)
  
  void 
  MFrontCastemLawInterface::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void 
  MFrontCastemLawInterface::setWarningMode(void)
  {
    this->warningMode = true;
  }

  void 
  MFrontCastemLawInterface::setDebugMode(void)
  {
    this->debugMode = true;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontCastemLawInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  std::string
  MFrontCastemLawInterface::makeUpperCase(const std::string& s)
  {
    using namespace std;
    string res(s);
    string::iterator p;
    for(p=res.begin();p!=res.end();++p){
      *p = static_cast<char>(toupper(*p));
    }
    return res;
  }

  std::string
  MFrontCastemLawInterface::makeLowerCase(const std::string& s)
  {
    using namespace std;
    string res(s);
    string::iterator p;
    for(p=res.begin();p!=res.end();++p){
      *p = static_cast<char>(tolower(*p));
    }
    return res;
  }

  MFrontCastemLawInterface::~MFrontCastemLawInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontCastemLawInterface::getGlobalDependencies(const std::string& lib,
						  const std::string& mat,
						  const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string castlib = "libCastem"+getMaterialLawLibraryNameBase(lib,mat);
    libs[castlib].push_back("-lm");
    return libs;
  } // end of MFrontCastemLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontCastemLawInterface::getGlobalIncludes(const std::string& lib,
					      const std::string& mat,
					      const std::string&)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string castlib = "libCastem"+getMaterialLawLibraryNameBase(lib,mat);
    incs[castlib].push_back(CASTEM_CPPFLAGS);
    return incs;
  } // end of MFrontCastemLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontCastemLawInterface::getGeneratedSources(const std::string& lib,
						const std::string& mat,
						const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string castlib = "libCastem"+getMaterialLawLibraryNameBase(lib,mat);
    if(!mat.empty()){
      src[castlib].push_back(mat+"_"+className+"-castem.cxx");
    } else {
      src[castlib].push_back(className+"-castem.cxx");
    }
    return src;
  } // end of MFrontCastemLawInterface::getGeneratedSources

  std::vector<std::string>
  MFrontCastemLawInterface::getGeneratedIncludes(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    vector<string> incs;
    incs.push_back(this->headerFileName);
    return incs;
  } // end of MFrontCastemLawInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontCastemLawInterface::getLibrariesDependencies(const std::string& lib,
						     const std::string& mat,
						     const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string castlib = "libCastem"+getMaterialLawLibraryNameBase(lib,mat);
    libs[castlib].push_back("-lm");
    return libs;
  } // end of MFrontCastemLawInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontCastemLawInterface::getSpecificTargets(const std::string&,
					       const std::string&,
					       const std::string&,
					       const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontCastemLawInterface::getSpecificTargets


  void
  MFrontCastemLawInterface::writeOutputFiles(const std::string& file,
					     const std::string& ,
					     const std::string& material,
					     const std::string& className,
					     const std::string& author,
					     const std::string& date,
					     const std::string& description,
					     const std::string& includes,
					     const std::string& output,
					     const VarContainer& inputs,
					     const std::vector<std::string>& materialLaws,
					     const std::map<std::string,std::string>& glossaryNames,
					     const std::map<std::string,std::string>& entryNames,
					     const StaticVarContainer& staticVars,
					     const std::vector<std::string>& params,
					     const std::map<std::string,double>& paramValues,
					     const LawFunction& function,
					     const std::vector<VariableBoundsDescription>& bounds,
					     const std::vector<VariableBoundsDescription>& physicalBounds,
					     const bool,
					     const std::vector<std::string>&)
  {
    using namespace std;
    using namespace tfel::system;
    string dir;
    string name;
    vector<string>::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,string>::const_iterator p3;

    if(material.empty()){
      name = className;
    } else {
      name = material+"_"+className;
    }
    this->headerFileName  = "include/" + name;
    this->headerFileName += "-castem.hxx";
    this->srcFileName     = "src/" + name;
    this->srcFileName    += "-castem.cxx";
    this->headerFile.open(this->headerFileName.c_str());
    if(!this->headerFile){
      string msg("MFrontCastemLawInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MFrontCastemLawInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    this->writeHeaderFile(name,author,date,description);
    this->writeSrcFile(file,name,author,date,includes,output,
		       inputs,materialLaws,
		       staticVars,glossaryNames,
		       entryNames,params,paramValues,
		       function,bounds,physicalBounds);
  } // end of MFrontCastemLawInterface::writeOutputFiles

  void
  MFrontCastemLawInterface::writeHeaderFile(const std::string& name,
					    const std::string& author,
					    const std::string& date,
					    const std::string& description)
  {
    using namespace std;
    VarContainer::const_iterator p;
    StaticVarContainer::const_iterator p2;
    vector<string>::const_iterator p3;
    VarContainer::const_iterator p4;

    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
			   << name << " MaterialLaw.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!author.empty()){
      this->headerFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->headerFile << "* \\date   " << date       << endl;
    }
    if(!description.empty()){
      this->headerFile << description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _" 
			   << MFrontCastemLawInterface::makeUpperCase(name)
		     << "_CASTEM_HH\n";
    this->headerFile << "#define _"
		     << MFrontCastemLawInterface::makeUpperCase(name)
		     << "_CASTEM_HH\n\n";
    writeExportDirectives(this->headerFile);
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "extern \"C\"{\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
    this->headerFile << "MFRONT_SHAREDOBJ double MFRONT_STDCALL\n";
    this->headerFile << name << "(";
    this->headerFile << "const double * const";
    this->headerFile << ");\n\n";
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "} // end of extern \"C\"\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
    this->headerFile << "#endif /* _"
		     << MFrontCastemLawInterface::makeUpperCase(name)
		     << "_CASTEM_HH */\n";

    this->headerFile.close();
  } // end of MFrontCastemLawInterface::writeHeaderFile(void)

  void
  MFrontCastemLawInterface::writeSrcFile(const std::string& file,
					 const std::string& name,
					 const std::string& author,
					 const std::string& date,
					 const std::string& includes,
					 const std::string& output,
					 const VarContainer& inputs,
					 const std::vector<std::string>& materialLaws,
					 const StaticVarContainer& staticVars,
					 const std::map<std::string,std::string>& glossaryNames,
					 const std::map<std::string,std::string>& entryNames,
					 const std::vector<std::string>& params,
					 const std::map<std::string,double>& paramValues,
					 const LawFunction& function,
					 const std::vector<VariableBoundsDescription>& bounds,
					 const std::vector<VariableBoundsDescription>& physicalBounds)
  {
    using namespace std;
    vector<string>::const_iterator p;
    StaticVarContainer::const_iterator p2;
    VarContainer::const_iterator p3;
    map<string,string>::const_iterator p4;
    vector<VariableBoundsDescription>::const_iterator p5;
    map<string,double>::const_iterator p6;
    map<string,string>::const_iterator p7;

    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
			<< name << " MaterialLaw.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!author.empty()){
      this->srcFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->srcFile << "* \\date   " << date       << endl;
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<iostream>\n";
    this->srcFile << "#include<cmath>\n";
    this->srcFile << "#include<cstring>\n";
    this->srcFile << "#include<cstdlib>\n";
    if(!includes.empty()){
      this->srcFile << includes << endl << endl;
    }
    this->srcFile << "#include\"" << name << "-castem.hxx\"\n\n";
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";

    if(!inputs.empty()){
      this->srcFile << "MFRONT_SHAREDOBJ const char *\n";
      this->srcFile << name << "_args[" << inputs.size();
      this->srcFile << "] = {";
      for(p3=inputs.begin();p3!=inputs.end();){
	string iname;
	if((p4=glossaryNames.find(p3->name))!=glossaryNames.end()){
	  iname = "\""+p4->second+"\"";
	} else if((p4=entryNames.find(p3->name))!=entryNames.end()){
	  iname = "\""+p4->second+"\"";
	} else {
	  iname = "\""+p3->name+"\"";
	}
	this->srcFile << iname;
	if(++p3!=inputs.end()){
	  this->srcFile << ",";
	}
      }
      this->srcFile << "};\n\n";
    }

    this->srcFile << "MFRONT_SHAREDOBJ unsigned short\n";
    this->srcFile << name << "_nargs = " << inputs.size() << "u;\n\n";
    this->srcFile << "MFRONT_SHAREDOBJ double MFRONT_STDCALL\n";
    this->srcFile << name << "(";
    this->srcFile << "const double * const castem_params";
    this->srcFile << ")\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "typedef double real;\n";
    // material laws
    writeMaterialLaws("MFrontCastemLawInterface::writeOutputFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("MFrontCastemLawInterface::writeOutputFile",
			 this->srcFile,staticVars,file,debugMode);
    // parameters
    if(!params.empty()){
      for(p=params.begin();p!=params.end();++p){
	p6 = paramValues.find(*p);
	if(p6==paramValues.end()){
	  string msg("MFrontCastemLawInterface::writeOutputFile : ");
	  msg += "internal error (can't find value of parameter " + *p + ")";
	  throw(runtime_error(msg));
	}
	this->srcFile << "static const double " << *p << " = " << p6->second << ";\n";
      }
    }
    if(!inputs.empty()){
      unsigned short i;
      for(i=0,p3=inputs.begin();p3!=inputs.end();++p3,++i){
	this->srcFile << "const double " << p3->name << " = ";
	if(i==0){
	  this->srcFile << "*(castem_params);\n";
	} else {
	  this->srcFile << "*(castem_params+"+toString(i)+"u);\n";
	}
      }
    }
    this->srcFile << "double " << output << ";\n";
    if((!physicalBounds.empty())||
       (!bounds.empty())){
      this->srcFile << "#ifndef NO_CASTEM_BOUNDS_CHECK\n";
    }
    if(!physicalBounds.empty()){
      this->srcFile << "// treating physical bounds\n";
      for(p5=physicalBounds.begin();
	  p5!=physicalBounds.end();++p5){
	if(p5->boundsType==VariableBoundsDescription::Lower){
	  this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is below its physical lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  this->srcFile << "return nan(\""
			      << name << " : "
			      << p5->varName << " is not physically valid.\");\n";
	  this->srcFile << "}\n";
	} else if(p5->boundsType==VariableBoundsDescription::Upper){
	  this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is below its physical upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  this->srcFile << "return nan(\""
			      << name << " : "
			      << p5->varName << " is not physically valid.\");\n";
	  this->srcFile << "}\n";
	} else {
	  this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			      << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	  this->srcFile << "if(" << p5->varName<< " < " << p5->lowerBound << "){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is below its physical lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is over its physical upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  this->srcFile << "}\n";
	  this->srcFile << "return nan(\""
			      << name << " : "
			      << p5->varName << " is not physically valid.\");\n";
	  this->srcFile << "}\n";
	}
      }
    }
    if(!bounds.empty()){
      this->srcFile << "// treating standard bounds\n";
      for(p5=bounds.begin();
	  p5!=bounds.end();++p5){
	if(p5->boundsType==VariableBoundsDescription::Lower){
	  this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	  this->srcFile << "const char * const policy = "
			      << "::getenv(\"CASTEM_OUT_OF_BOUND_POLICY\");\n";
	  this->srcFile << "if(policy!=0){\n";
	  this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	  this->srcFile << "return nan(\""
			      << name << " : "
			      << p5->varName << " is out of bounds.\");\n";
	  this->srcFile << "} else if (strcmp(policy,\"WARNING\")==0){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is below its lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	} else if(p5->boundsType==VariableBoundsDescription::Upper){
	  this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	  this->srcFile << "const char * const policy = "
			      << "::getenv(\"CASTEM_OUT_OF_BOUND_POLICY\");\n";
	  this->srcFile << "if(policy!=0){\n";
	  this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  this->srcFile << "return nan(\""
			      << name << " : "
			      << p5->varName << " is out of bounds.\");\n";
	  this->srcFile << "} else if (strcmp(policy,\"WARNING\")==0){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	} else {
	  this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			      << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	  this->srcFile << "const char * const policy = "
			      << "::getenv(\"CASTEM_OUT_OF_BOUND_POLICY\");\n";
	  this->srcFile << "if(policy!=0){\n";
	  this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	  this->srcFile << "if(" << p5->varName<< " < " << p5->lowerBound << "){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is below its lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  this->srcFile << "}\n";
	  this->srcFile << "return nan(\""
			      << name << " : "
			      << p5->varName << " is out of bounds.\");\n";
	  this->srcFile << "} else if (strcmp(policy,\"WARNING\")==0){\n";
	  this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is below its lower bound (\"\n << "
			      << p5->varName << " << \"<" << p5->lowerBound << ").\\n\";\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "cerr << \"" << name << " : "
			      << p5->varName << " is over its upper bound (\"\n << "
			      << p5->varName << " << \">" << p5->upperBound << ").\\n\";\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	}
      }
    }
    if((!physicalBounds.empty())||
       (!bounds.empty())){
      this->srcFile << "#endif /* NO_CASTEM_BOUNDS_CHECK */\n";
    }
    this->srcFile << function.body;
    this->srcFile << "return " << output << ";\n";
    this->srcFile << "} // end of " << name << "\n\n";
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
    this->srcFile.close();
  } // end of MFrontCastemLawInterface::writeSrcFile(void)

} // end of namespace mfront
