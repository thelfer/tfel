/*!
 * \file   mfront/include/MFront/ParserUtilities.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_PARSERUTILITIES_HXX_
#define _LIB_PARSERUTILITIES_HXX_ 

#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/StaticVariableDescription.hxx"

namespace mfront{

  TFEL_VISIBILITY_EXPORT void
  writeMaterialLaws(const std::string&,
		    std::ofstream&,
		    const std::vector<std::string>&);

  TFEL_VISIBILITY_EXPORT void
  writeStaticVariables(const std::string&,
		       std::ofstream&,
		       const StaticVariableDescriptionContainer&,
		       const std::string&);
  
  TFEL_VISIBILITY_EXPORT std::string
  getMaterialLawLibraryNameBase(const std::string&,
				const std::string&);

  TFEL_VISIBILITY_EXPORT void
  writeExportDirectives(std::ofstream&);

  TFEL_VISIBILITY_EXPORT std::string
  toString(const unsigned short);

  TFEL_VISIBILITY_EXPORT std::string
  makeUpperCase(const std::string&);

  TFEL_VISIBILITY_EXPORT std::string 
  makeLowerCase(const std::string&);

} // end of namespace mfront  

#endif /* _LIB_PARSERUTILITIES_HXX */

