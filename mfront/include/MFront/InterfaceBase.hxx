/*!
 * \file   mfront/include/MFront/InterfaceBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 nov 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_INTERFACEBASE_HXX_
#define _LIB_INTERFACEBASE_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT InterfaceBase
  {

  protected :

    typedef tfel::utilities::CxxTokenizer::TokensContainer TokensContainer;

    static void
    checkNotEndOfFile(TokensContainer::const_iterator&,
		      const TokensContainer::const_iterator,
		      const std::string&,
		      const std::string& = "");

    static void
    readSpecifiedToken(TokensContainer::const_iterator&,
		       const TokensContainer::const_iterator,
		       const std::string&,
		       const std::string&);

    static void
    throwRuntimeError(TokensContainer::const_iterator&,
		      const TokensContainer::const_iterator,
		      const std::string&,
		      const std::string&);

    static std::vector<std::string>
    readArrayOfString(TokensContainer::const_iterator&,
		      const TokensContainer::const_iterator,
		      const std::string&);

    static std::vector<std::string>
    readStringOrArrayOfString(TokensContainer::const_iterator&,
			      const TokensContainer::const_iterator,
			      const std::string&);

  }; // end of struct InterfaceBase 

} // end of namespace mfront  

#endif /* _LIB_INTERFACEBASE_HXX */

