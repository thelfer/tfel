/*!
 * \file   mfront/src/MFrontCastemLawInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontCastemLawInterface.hxx"

namespace mfront
{
  
  std::string
  MFrontCastemLawInterface::getName(void)
  {
    return "castem";
  }

  MFrontCastemLawInterface::MFrontCastemLawInterface()
  {}

  void
  MFrontCastemLawInterface::reset(void)
  {
    if(this->headerFile.is_open()){
      this->headerFile.close();
    }
    if(this->srcFile.is_open()){
      this->srcFile.close();
    }
  } // end of MFrontCastemLawInterface::reset(void)
  
  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontCastemLawInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

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
    string name          = this->getCastemFunctionName(mat,className);
    src[castlib].push_back(this->getSourceFileName(name));
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

  std::string
  MFrontCastemLawInterface::getCastemFunctionName(const std::string& material,
						  const std::string& className)
  {
    if(material.empty()){
      return className;
    }
    return material+"_"+className;
  }

  std::string
  MFrontCastemLawInterface::getHeaderFileName(const std::string& name)
  {
    return name + "-castem.hxx";
  }

  std::string
  MFrontCastemLawInterface::getSourceFileName(const std::string& name)
  {
    return name + "-castem.cxx";
  }

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
					     const VariableDescriptionContainer& inputs,
					     const std::vector<std::string>& materialLaws,
					     const std::map<std::string,std::string>& glossaryNames,
					     const std::map<std::string,std::string>& entryNames,
					     const StaticVariableDescriptionContainer& staticVars,
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
    map<string,string>::const_iterator p2;
    map<string,string>::const_iterator p3;
    string name          = this->getCastemFunctionName(material,className);
    this->headerFileName = "include/"+this->getHeaderFileName(name);
    this->srcFileName    = "src/"+this->getSourceFileName(name);
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
    this->writeHeaderFile(name,author,date,description,params);
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
					    const std::string& description,
					    const std::vector<std::string>& params)
  {
    using namespace std;
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
		     << makeUpperCase(name)
		     << "_CASTEM_HH\n";
    this->headerFile << "#define _"
		     << makeUpperCase(name)
		     << "_CASTEM_HH\n\n";
    writeExportDirectives(this->headerFile);
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "extern \"C\"{\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
    this->headerFile << "MFRONT_SHAREDOBJ double MFRONT_CALLING_CONVENTION\n";
    this->headerFile << name << "(";
    this->headerFile << "const double * const";
    this->headerFile << ");\n\n";
    if(!params.empty()){
      this->headerFile << "MFRONT_SHAREDOBJ int MFRONT_CALLING_CONVENTION\n";
      this->headerFile << name << "_setParameter(const char *const,";
      this->headerFile << "const double";
      this->headerFile << ");\n\n";
    }
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "} // end of extern \"C\"\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
    this->headerFile << "#endif /* _"
		     << makeUpperCase(name)
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
					 const VariableDescriptionContainer& inputs,
					 const std::vector<std::string>& materialLaws,
					 const StaticVariableDescriptionContainer& staticVars,
					 const std::map<std::string,std::string>& glossaryNames,
					 const std::map<std::string,std::string>& entryNames,
					 const std::vector<std::string>& params,
					 const std::map<std::string,double>& paramValues,
					 const LawFunction& function,
					 const std::vector<VariableBoundsDescription>& bounds,
					 const std::vector<VariableBoundsDescription>& physicalBounds)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    vector<string>::const_iterator p;
    VariableDescriptionContainer::const_iterator p3;
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
    this->srcFile << "#include<algorithm>\n";
    this->srcFile << "#include<cstring>\n";
    this->srcFile << "#include<cstdlib>\n";
    if(!includes.empty()){
      this->srcFile << includes << endl << endl;
    }
    this->srcFile << "#include\"" << name << "-castem.hxx\"\n\n";

    if(!params.empty()){
      vector<string>::const_iterator pp;
      for(pp=params.begin();pp!=params.end();++pp){
	this->srcFile << "static double&\n";
	this->srcFile << name << "_getParameter_" << *pp << "(){\n";
	p6 = paramValues.find(*pp);
	if(p6==paramValues.end()){
	  string msg("MFrontCastemLawInterface::writeOutputFile : ");
	  msg += "internal error (can't find value of parameter " + *pp + ")";
	  throw(runtime_error(msg));
	}
	this->srcFile << "static double " << *pp << " = " << p6->second << ";\n";
	this->srcFile << "return " << *pp << ";\n";
	this->srcFile << "}\n\n";
      }	
    }

    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";

    this->srcFile << "MFRONT_SHAREDOBJ const char *\n";
    this->srcFile << name << "_src = \""
		  << tokenize(file,dirSeparator()).back()
		  << "\";\n\n";

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

    if(!params.empty()){
      this->srcFile << "MFRONT_SHAREDOBJ int MFRONT_CALLING_CONVENTION\n";
      this->srcFile << name << "_setParameter(const char *const p,";
      this->srcFile << "const double v";
      this->srcFile << "){\n";
      vector<string>::const_iterator pp;
      for(pp=params.begin();pp!=params.end();++pp){
	this->srcFile << "if(strcmp(\"" << *pp << "\",p)==0){\n";
	this->srcFile << name << "_getParameter_" << *pp << "() = v;\n";
	this->srcFile << "return 1;\n";
	this->srcFile << "}\n";
      }	
      this->srcFile << "return 0;\n";
      this->srcFile << "}\n\n";
    }

    this->srcFile << "MFRONT_SHAREDOBJ double MFRONT_CALLING_CONVENTION\n";
    this->srcFile << name << "(";
    if(!inputs.empty()){
      this->srcFile << "const double * const castem_params";
    } else {
      this->srcFile << "const double * const";
    }
    this->srcFile << ")\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "typedef double real;\n";
    // material laws
    writeMaterialLaws("MFrontCastemLawInterface::writeOutputFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("MFrontCastemLawInterface::writeOutputFile",
			 this->srcFile,staticVars,file);
    // parameters
    if(!params.empty()){
      for(p=params.begin();p!=params.end();++p){
	this->srcFile << "const double " << *p << " = ";
	this->srcFile << name << "_getParameter_" << *p << "();\n";
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
			      << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n";
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
			      << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n";
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
			      << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n";
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
