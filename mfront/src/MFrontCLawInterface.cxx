/*!
 * \file   mfront/src/MFrontCLawInterface.cxx
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

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontCLawInterface.hxx"

namespace mfront
{

  std::string
  MFrontCLawInterface::getName(void)
  {
    return "c";
  }

  MFrontCLawInterface::MFrontCLawInterface()
    : MFrontCLawInterfaceBase()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontCLawInterface::getGlobalDependencies(const std::string& library,
					     const std::string& material,
					     const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["lib"+getMaterialLawLibraryNameBase(library,material)].push_back("-lm");
    return libs;
  } // end of MFrontCLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontCLawInterface::getGlobalIncludes(const std::string&,
					 const std::string&,
					 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontCLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontCLawInterface::getGeneratedSources(const std::string& library,
					   const std::string& material,
					   const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string name = this->getSrcFileName(material,className);
    src["lib"+getMaterialLawLibraryNameBase(library,material)].push_back(name+".cxx");
    return src;
  } // end of MFrontCLawInterface::getGeneratedSources

  std::vector<std::string>
  MFrontCLawInterface::getGeneratedIncludes(const std::string&,
					    const std::string& material,
					    const std::string& className)
  {
    using namespace std;
    vector<string> incs;
    string header = this->getHeaderFileName(material,className);
    if(!header.empty()){
      incs.push_back(header+".hxx");
    }
    return incs;
  } // end of MFrontCLawInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontCLawInterface::getLibrariesDependencies(const std::string& library,
						const std::string& material,
						const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["lib"+getMaterialLawLibraryNameBase(library,material)].push_back("-lm");
    return libs;
  } // end of MFrontCLawInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontCLawInterface::getSpecificTargets(const std::string&,
					  const std::string&,
					  const std::string&,
					  const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontCLawInterface::getSpecificTargets

  std::string
  MFrontCLawInterface::getHeaderFileName(const std::string& material,
					 const std::string& className)
  {
    if(material.empty()){
      return className;
    }
    return material+"_"+className;
  } // end of MFrontCLawInterface::getHeaderFileName

  std::string
  MFrontCLawInterface::getSrcFileName(const std::string& material,
				      const std::string& className)
  {
    if(material.empty()){
      return className;
    }
    return material+"_"+className;
  } // end of MFrontCLawInterface::getSrcFileName
  
  void
  MFrontCLawInterface::writeHeaderPreprocessorDirectives(const std::string&,
							 const std::string&)
  {
    writeExportDirectives(this->headerFile);
  } // end of MFrontCLawInterface::writePreprocessorDirectives

  void
  MFrontCLawInterface::writeSrcPreprocessorDirectives(const std::string&,
						      const std::string&)
  {} // end of MFrontCLawInterface::writeSrcPreprocessorDirectives

  void
  MFrontCLawInterface::writeBeginHeaderNamespace(void)
  {
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "extern \"C\"{\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
  } // end of MFrontCLawInterface::writeBeginHeaderNamespace
  
  void
  MFrontCLawInterface::writeEndHeaderNamespace(void)
  {
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "} /* end of extern \"C\" */\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
  } // end of MFrontCLawInterface::writeEndHeaderNamespace(void)

  void
  MFrontCLawInterface::writeBeginSrcNamespace(void)
  {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  } // end of MFrontCLawInterface::writeBeginSrcNamespace
  
  void
  MFrontCLawInterface::writeEndSrcNamespace(void)
  {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  } // end of MFrontCLawInterface::writeEndSrcNamespace(void)

  std::string
  MFrontCLawInterface::getFunctionDeclaration(const std::string& material,
					      const std::string& className)
  {
    if(material.empty()){
      return "MFRONT_SHAREDOBJ double MFRONT_STDCALL\n"+className;
    }
    return "MFRONT_SHAREDOBJ double MFRONT_STDCALL\n"+material+"_"+className;
  } // end of MFrontCLawInterface::getFunctionDeclaration
  
  bool
  MFrontCLawInterface::requiresCheckBoundsFunction(void) const
  {
    return false;
  }

  std::string
  MFrontCLawInterface::getCheckBoundsFunctionDeclaration(const std::string& material,
							 const std::string& className)
  {
    if(material.empty()){
      return "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"+className+"_checkBounds";
    }
    return "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"+material+"_"+className+"_checkBounds";
  } // end of MFrontCLawInterface::getCheckBoundsFunctionDeclaration
  
  MFrontCLawInterface::~MFrontCLawInterface()
  {} // end of MFrontCLawInterface::~MFrontCLawInterface

} // end of namespace mfront
