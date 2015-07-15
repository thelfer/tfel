/*! 
 * \file   mfront/src/ExcelMaterialPropertyInternalInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \date   03 nov. 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/DSLUtilities.hxx"
#include"MFront/ExcelMaterialPropertyInternalInterface.hxx"

namespace mfront
{

  std::string
  ExcelMaterialPropertyInternalInterface::getName(void)
  {
    return "excel-internal";
  }

  ExcelMaterialPropertyInternalInterface::ExcelMaterialPropertyInternalInterface()
    : CMaterialPropertyInterface()
  {}

  std::string
  ExcelMaterialPropertyInternalInterface::getGeneratedLibraryName(const std::string& l,
								  const std::string& m) const
  {
    return "libExcel"+getMaterialLawLibraryNameBase(l,m);
  } // end of ExcelMaterialPropertyInternalInterface::getGeneratedLibraryName

  std::string
  ExcelMaterialPropertyInternalInterface::getHeaderFileName(const std::string& m,
						    const std::string& c)
  {
    return CMaterialPropertyInterface::getHeaderFileName(m,c)+"-Excel";
  } // end of ExcelMaterialPropertyInternalInterface::getHeaderFileName

  std::string
  ExcelMaterialPropertyInternalInterface::getSrcFileName(const std::string& m,
						 const std::string& c)
  {
    return CMaterialPropertyInterface::getSrcFileName(m,c)+"-Excel";
  } // end of ExcelMaterialPropertyInternalInterface::getSrcFileName
  
  void
  ExcelMaterialPropertyInternalInterface::writeHeaderPreprocessorDirectives(const std::string&,
									    const std::string&)
  {
    writeExportDirectives(this->headerFile,"__stdcall");
  } // end of ExcelMaterialPropertyInternalInterface::writePreprocessorDirectives
  
  ExcelMaterialPropertyInternalInterface::~ExcelMaterialPropertyInternalInterface()
  {} // end of ExcelMaterialPropertyInternalInterface::~ExcelMaterialPropertyInternalInterface

} // end of namespace mfront
