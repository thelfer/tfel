/*!
 * \file   mfront/src/CMaterialPropertyInterface.cxx
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

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/CMaterialPropertyInterface.hxx"

namespace mfront
{

  std::string
  CMaterialPropertyInterface::getName(void)
  {
    return "c";
  }

  CMaterialPropertyInterface::CMaterialPropertyInterface()
    : CMaterialPropertyInterfaceBase()
  {}

  std::map<std::string,std::vector<std::string> >
  CMaterialPropertyInterface::getGlobalDependencies(const std::string& library,
					     const std::string& material,
					     const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["lib"+getMaterialLawLibraryNameBase(library,material)].push_back("-lm");
    return libs;
  } // end of CMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  CMaterialPropertyInterface::getGlobalIncludes(const std::string&,
					 const std::string&,
					 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of CMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  CMaterialPropertyInterface::getGeneratedSources(const std::string& library,
					   const std::string& material,
					   const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string name = this->getSrcFileName(material,className);
    src["lib"+getMaterialLawLibraryNameBase(library,material)].push_back(name+".cxx");
    return src;
  } // end of CMaterialPropertyInterface::getGeneratedSources

  std::vector<std::string>
  CMaterialPropertyInterface::getGeneratedIncludes(const std::string&,
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
  } // end of CMaterialPropertyInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  CMaterialPropertyInterface::getLibrariesDependencies(const std::string& library,
						const std::string& material,
						const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["lib"+getMaterialLawLibraryNameBase(library,material)].push_back("-lm");
    return libs;
  } // end of CMaterialPropertyInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  CMaterialPropertyInterface::getSpecificTargets(const std::string&,
					  const std::string&,
					  const std::string&,
					  const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of CMaterialPropertyInterface::getSpecificTargets

  std::string
  CMaterialPropertyInterface::getHeaderFileName(const std::string& material,
					 const std::string& className)
  {
    if(material.empty()){
      return className;
    }
    return material+"_"+className;
  } // end of CMaterialPropertyInterface::getHeaderFileName

  std::string
  CMaterialPropertyInterface::getSrcFileName(const std::string& material,
				      const std::string& className)
  {
    if(material.empty()){
      return className;
    }
    return material+"_"+className;
  } // end of CMaterialPropertyInterface::getSrcFileName
  
  void
  CMaterialPropertyInterface::writeHeaderPreprocessorDirectives(const std::string&,
							 const std::string&)
  {
    writeExportDirectives(this->headerFile);
  } // end of CMaterialPropertyInterface::writePreprocessorDirectives

  void
  CMaterialPropertyInterface::writeSrcPreprocessorDirectives(const std::string&,
						      const std::string&)
  {} // end of CMaterialPropertyInterface::writeSrcPreprocessorDirectives

  void
  CMaterialPropertyInterface::writeBeginHeaderNamespace(void)
  {
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "extern \"C\"{\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
  } // end of CMaterialPropertyInterface::writeBeginHeaderNamespace
  
  void
  CMaterialPropertyInterface::writeEndHeaderNamespace(void)
  {
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "} /* end of extern \"C\" */\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
  } // end of CMaterialPropertyInterface::writeEndHeaderNamespace(void)

  void
  CMaterialPropertyInterface::writeBeginSrcNamespace(void)
  {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  } // end of CMaterialPropertyInterface::writeBeginSrcNamespace
  
  void
  CMaterialPropertyInterface::writeEndSrcNamespace(void)
  {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  } // end of CMaterialPropertyInterface::writeEndSrcNamespace(void)

  std::string
  CMaterialPropertyInterface::getFunctionDeclaration(const std::string& material,
					      const std::string& className)
  {
    if(material.empty()){
      return "MFRONT_SHAREDOBJ double MFRONT_STDCALL\n"+className;
    }
    return "MFRONT_SHAREDOBJ double MFRONT_STDCALL\n"+material+"_"+className;
  } // end of CMaterialPropertyInterface::getFunctionDeclaration
  
  bool
  CMaterialPropertyInterface::requiresCheckBoundsFunction(void) const
  {
    return false;
  }

  std::string
  CMaterialPropertyInterface::getCheckBoundsFunctionDeclaration(const std::string& material,
							 const std::string& className)
  {
    if(material.empty()){
      return "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"+className+"_checkBounds";
    }
    return "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"+material+"_"+className+"_checkBounds";
  } // end of CMaterialPropertyInterface::getCheckBoundsFunctionDeclaration
  
  CMaterialPropertyInterface::~CMaterialPropertyInterface()
  {} // end of CMaterialPropertyInterface::~CMaterialPropertyInterface

} // end of namespace mfront
