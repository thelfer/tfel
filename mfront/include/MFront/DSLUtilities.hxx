/*!
 * \file   mfront/include/MFront/DSLUtilities.hxx
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

#ifndef _LIB_MFRONT_PARSERUTILITIES_HXX_
#define _LIB_MFRONT_PARSERUTILITIES_HXX_ 

#include<iosfwd>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Glossary/Forward/Glossary.hxx"
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

  /*!
   * \brief define export directives (visibility attributes) and
   * calling conventions through the MFRONT_SHAREDOBJ and the
   * MFRONT_CALLING_CONVENTION macros
   * \param[out] f : output file
   * \param[in]  c : calling convention
   */
  TFEL_VISIBILITY_EXPORT void
  writeExportDirectives(std::ofstream&,
			const std::string& = "__cdecl");

  TFEL_VISIBILITY_EXPORT std::string
  makeUpperCase(const std::string&);

  TFEL_VISIBILITY_EXPORT std::string 
  makeLowerCase(const std::string&);

  /*!
   * \brief display the complete description of a glossary entry
   * \param[out] os : output stream
   * \param[in]  e  : glossary entry
   */
  TFEL_VISIBILITY_EXPORT void
  displayGlossaryEntryCompleteDescription(std::ostream&,
					  const tfel::glossary::GlossaryEntry&);

} // end of namespace mfront  

#endif /* _LIB_MFRONT_PARSERUTILITIES_HXX */

