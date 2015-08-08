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
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
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

  std::string
  CMaterialPropertyInterface::getGeneratedLibraryName(const std::string& l,
					       const std::string& m) const
  {
    return "lib"+getMaterialLawLibraryNameBase(l,m);
  } // end of CMaterialPropertyInterface::getGeneratedLibraryName

  void
  CMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
						    const MaterialPropertyDescription& mpd)
  {
    const auto lib  = this->getGeneratedLibraryName(mpd.library,mpd.material);
    const auto name = this->getSrcFileName(mpd.material,mpd.className);
    const auto f    = mpd.material.empty() ? mpd.className : mpd.material+"_"+mpd.className;
    const auto header = this->getHeaderFileName(mpd.material,mpd.className);
    d[lib].dependencies.push_back("-lm");
    d[lib].sources.push_back(name+".cxx");
    d[lib].epts.insert(d[lib].epts.end(),{f,f+"_checkBounds"});
    if(!header.empty()){
      d.headers.push_back(header+".hxx");
    }
  } // end of CMaterialPropertyInterface::getTargetsDescription

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
      return "MFRONT_SHAREDOBJ double MFRONT_CALLING_CONVENTION\n"+className;
    }
    return "MFRONT_SHAREDOBJ double MFRONT_CALLING_CONVENTION\n"+material+"_"+className;
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
      return "MFRONT_SHAREDOBJ int MFRONT_CALLING_CONVENTION\n"+className+"_checkBounds";
    }
    return "MFRONT_SHAREDOBJ int MFRONT_CALLING_CONVENTION\n"+material+"_"+className+"_checkBounds";
  } // end of CMaterialPropertyInterface::getCheckBoundsFunctionDeclaration
  
  CMaterialPropertyInterface::~CMaterialPropertyInterface()
  {} // end of CMaterialPropertyInterface::~CMaterialPropertyInterface

} // end of namespace mfront
