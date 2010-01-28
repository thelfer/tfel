/*!
 * \file   ExternalFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONMANAGER_HXX_
#define _LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONMANAGER_HXX_ 

#include<string>
#include<map>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
    
      struct ExternalFunctionManager
	: public std::map<std::string,
			  tfel::utilities::SmartPtr<ExternalFunction> >
      {}; // end of struct ExternalFunctionManager
      
    } // end of namespace parser

  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONMANAGER_HXX */

