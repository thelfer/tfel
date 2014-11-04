/*!
 * \file   mfront/src/MFrontMaterialPropertyInterface.cxx
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
#include"MFront/MFrontMaterialPropertyInterface.hxx"

namespace mfront
{

  std::string
  MFrontMaterialPropertyInterface::getName(void)
  {
    return "mfront";
  }

  MFrontMaterialPropertyInterface::MFrontMaterialPropertyInterface()
    : CMaterialPropertyInterfaceBase()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialPropertyInterface::getGlobalDependencies(const std::string&,
						  const std::string&,
						  const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["libMFrontMaterialLaw"].push_back("-lm");
    return libs;
  } // end of MFrontMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialPropertyInterface::getGlobalIncludes(const std::string&,
					      const std::string&,
					      const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialPropertyInterface::getGeneratedSources(const std::string&,
						const std::string& material,
						const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string name = this->getSrcFileName(material,className);
    src["libMFrontMaterialLaw"].push_back(name+".cxx");
    return src;
  } // end of MFrontMaterialPropertyInterface::getGeneratedSources

  std::vector<std::string>
  MFrontMaterialPropertyInterface::getGeneratedIncludes(const std::string&,
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
  } // end of MFrontMaterialPropertyInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontMaterialPropertyInterface::getLibrariesDependencies(const std::string&,
						     const std::string&,
						     const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["libMFrontMaterialLaw"].push_back("-lm");
    return libs;
  } // end of MFrontMaterialPropertyInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontMaterialPropertyInterface::getSpecificTargets(const std::string&,
					       const std::string&,
					       const std::string&,
					       const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontMaterialPropertyInterface::getSpecificTargets

  std::string
  MFrontMaterialPropertyInterface::getHeaderFileName(const std::string& material,
					      const std::string& className)
  {
    if(material.empty()){
      return className+"-mfront";
    }
    return material+"_"+className+"-mfront";
  } // end of MFrontMaterialPropertyInterface::getHeaderFileName

  std::string
  MFrontMaterialPropertyInterface::getSrcFileName(const std::string& material,
					   const std::string& className)
  {
    if(material.empty()){
      return className+"-mfront";
    }
    return material+"_"+className+"-mfront";
  } // end of MFrontMaterialPropertyInterface::getSrcFileName
  
  void
  MFrontMaterialPropertyInterface::writeHeaderPreprocessorDirectives(const std::string&,
							      const std::string&)
  {
    writeExportDirectives(this->headerFile);
  } // end of MFrontMaterialPropertyInterface::writePreprocessorDirectives

  void
  MFrontMaterialPropertyInterface::writeSrcPreprocessorDirectives(const std::string&,
							   const std::string&)
  {} // end of MFrontMaterialPropertyInterface::writeSrcPreprocessorDirectives

  void
  MFrontMaterialPropertyInterface::writeBeginHeaderNamespace(void)
  {
    this->headerFile << "namespace mfront{\n";
  } // end of MFrontMaterialPropertyInterface::writeBeginHeaderNamespace
  
  void
  MFrontMaterialPropertyInterface::writeEndHeaderNamespace(void)
  {
    this->headerFile << "} // end of namespace mfront\n";
  } // end of MFrontMaterialPropertyInterface::writeEndHeaderNamespace(void)

  void
  MFrontMaterialPropertyInterface::writeBeginSrcNamespace(void)
  {
    this->srcFile << "namespace mfront{\n";
  } // end of MFrontMaterialPropertyInterface::writeBeginSrcNamespace
  
  void
  MFrontMaterialPropertyInterface::writeEndSrcNamespace(void)
  {
    this->srcFile << "} // end of namespace mfront\n";
  } // end of MFrontMaterialPropertyInterface::writeEndSrcNamespace(void)

  std::string
  MFrontMaterialPropertyInterface::getFunctionDeclaration(const std::string& material,
						   const std::string& law)
  {
    return "MFRONT_SHAREDOBJ double MFRONT_CALLING_CONVENTION\n"+this->getFunctionName(material,law);
  } // end of MFrontMaterialPropertyInterface::getFunctionDeclaration

  std::string
  MFrontMaterialPropertyInterface::getFunctionName(const std::string& material,
					    const std::string& law) const
  {
    if(material.empty()){
      return law;
    }
    return material+'_'+law;
  }
  
  bool
  MFrontMaterialPropertyInterface::requiresCheckBoundsFunction(void) const
  {
    return true;
  }

  std::string
  MFrontMaterialPropertyInterface::getCheckBoundsFunctionDeclaration(const std::string& material,
							      const std::string& className)
  {
    if(material.empty()){
      return  "MFRONT_SHAREDOBJ int MFRONT_CALLING_CONVENTION\n"+className+"_checkBounds";
    }
    return  "MFRONT_SHAREDOBJ int MFRONT_CALLING_CONVENTION\n"+material+"_"+className+"_checkBounds";
  } // end of MFrontMaterialPropertyInterface::getCheckBoundsFunctionDeclaration
  
  MFrontMaterialPropertyInterface::~MFrontMaterialPropertyInterface()
  {} // end of MFrontMaterialPropertyInterface::~MFrontMaterialPropertyInterface

} // end of namespace mfront
