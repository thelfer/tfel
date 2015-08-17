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
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
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

  void
  MFrontMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
							 const MaterialPropertyDescription& mpd)
  {
    const auto lib   = "libMFrontMaterialLaw";
    const auto  name = this->getSrcFileName(mpd.material,mpd.className);
    const auto f = mpd.material.empty() ? mpd.className : mpd.material+"_"+mpd.className;
    const auto header = this->getHeaderFileName(mpd.material,mpd.className);
    insert_if(d[lib].ldflags,"-lm");
    insert_if(d[lib].sources,name+".cxx");
    if(!header.empty()){
      insert_if(d.headers,header+".hxx");
    }
    insert_if(d[lib].epts,{f,f+"_checkBounds"});
  } // end of MFrontMaterialPropertyInterface::getTargetsDescription

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
  {} // end of MFrontMaterialPropertyInterface::writePreprocessorDirectives

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
    return "double "+this->getFunctionName(material,law);
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
      return  "int "+className+"_checkBounds";
    }
    return  "int "+material+"_"+className+"_checkBounds";
  } // end of MFrontMaterialPropertyInterface::getCheckBoundsFunctionDeclaration
  
  MFrontMaterialPropertyInterface::~MFrontMaterialPropertyInterface()
  {} // end of MFrontMaterialPropertyInterface::~MFrontMaterialPropertyInterface

} // end of namespace mfront
