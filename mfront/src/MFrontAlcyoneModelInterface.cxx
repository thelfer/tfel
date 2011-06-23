/*!
 * \file   MFrontAlcyoneModelInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<cassert>
#include<iterator>

#include"TFEL/System/System.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontAlcyoneModelInterface.hxx"

namespace mfront{

  std::string
  MFrontAlcyoneModelInterface::getName(void)
  {
    return "alcyone";
  } // end of MFrontPleiadesModelInterfaceBase::getName(void)

  void
  MFrontAlcyoneModelInterface::setOutputFileNames(const MFrontGenericData& pdata)
  {
    this->headerFileName  = "Pleiades/PMetier/PModels/"+pdata.className;
    this->headerFileName += "-alcyone.hh";
    this->srcFileName  = pdata.className;
    this->srcFileName += "-alcyone.cpp";
  } // end of MFrontAlcyoneModelInterface::setOutputFileNames(void)
  
  std::string
  MFrontAlcyoneModelInterface::getApplicationName(void) const
  {
    return "Alcyone";
  } // end of MFrontAlcyoneModelInterface::getLibraryBaseName

  void
  MFrontAlcyoneModelInterface::buildDomainName(void){
    this->srcFile << "// building current zone name\n";
    this->srcFile << "ostringstream oss;\n";
    this->srcFile << "oss << this->getCurrentSpace()->getNumMeshCurrent()+1;\n";
    this->srcFile << "string domainName = *ptr+'_'+oss.str();\n";
  }

} // end of namespace mfront  
