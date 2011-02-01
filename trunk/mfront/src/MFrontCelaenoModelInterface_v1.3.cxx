/*!
 * \file   MFrontCelaenoModelInterface_v1_3.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<cassert>
#include<iterator>

#include"TFEL/System/System.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontCelaenoModelInterface_v1.3.hxx"

namespace mfront{

  std::string
  MFrontCelaenoModelInterface_v1_3::getName(void)
  {
    return "celaeno1.3";
  } // end of MFrontPleiadesModelInterfaceBase::getName(void)

  std::string
  MFrontCelaenoModelInterface_v1_3::getApplicationName(void) const
  {
    return "Celaeno-1_3";
  } // end of MFrontCelaenoModelInterface_v1_3::getLibraryBaseName

  void
  MFrontCelaenoModelInterface_v1_3::setOutputFileNames(const MFrontGenericData& pdata)
  {
    this->headerFileName  = "Pleiades/PMetier/PModels/"+pdata.className;
    this->headerFileName += "-celaeno_v1.3.hh";
    this->srcFileName  = pdata.className;
    this->srcFileName += "-celaeno_v1.3.cpp";
  } // end of MFrontCelaenoModelInterface_v1_3::setOutputFileNames(void)
  
} // end of namespace mfront  
