/*!
 * \file   ParserUtilities.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2008
 */

#ifndef _LIB_PARSERUTILITIES_HXX_
#define _LIB_PARSERUTILITIES_HXX_ 

#include<fstream>
#include"StaticVarHandler.hxx"

namespace mfront{

  void
  writeMaterialLaws(const std::string&,
		    std::ofstream&,
		    const std::vector<std::string>&);

  void
  writeStaticVariables(const std::string&,
		       std::ofstream&,
		       const StaticVarContainer&,
		       const std::string&,
		       const bool);
  
  std::string
  getMaterialLawLibraryNameBase(const std::string&,
				const std::string&);

} // end of namespace mfront  

#endif /* _LIB_PARSERUTILITIES_HXX */

