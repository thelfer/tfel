/*!
 * \file   include/TFEL/Math/Parser/EvaluatorBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_PARSERBASE_HXX_
#define _LIB_TFEL_PARSERBASE_HXX_ 

#include<vector>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      struct TFELMATHPARSER_VISIBILITY_EXPORT EvaluatorBase
      {
      protected:
	void
	analyse(const std::string&);
	std::vector<std::string> tokens;
      private:
	static TFEL_VISIBILITY_LOCAL void
	splitAtTokenSeperator(std::vector<std::string>&);
	static TFEL_VISIBILITY_LOCAL
	std::string
	readNumber(std::string::iterator&,const std::string::iterator);
      }; // end of struct EvaluatorBase

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_PARSERBASE_HXX */

