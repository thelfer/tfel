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

  void
  MFrontCelaenoModelInterface_v1_3::setOutputFileNames(const MFrontGenericData& pdata)
  {
    this->headerFileName  = "include/Pleiades/PMetier/PModels/"+pdata.className;
    this->headerFileName += "-celaeno_v1.3.hh";
    this->srcFileName  = "src/"+pdata.className;
    this->srcFileName += "-celaeno_v1.3.cpp";
  } // end of MFrontCelaenoModelInterface_v1_3::setOutputFileNames(void)
  
} // end of namespace mfront  
