/*!
 * \file   include/TFEL/Utilities/Token.hxx
 * \brief  This file declares the Token class
 * \author Helfer Thomas
 * \date   20 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_UTILITIES_TOKEN_H_
#define _LIB_TFEL_UTILITIES_TOKEN_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"


namespace tfel{

  namespace utilities{

    /*!
     * \brief class representing a token in a C++-like file
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT Token
    {

      //! possible type of the token
      enum TokenFlag{Standard,Comment,DoxygenComment,
		     DoxygenBackwardComment,
		     String,Char,Preprocessor};
      //! line number
      unsigned int line;
      //! string holded by the token
      std::string value;
      //! comment
      std::string comment;
      //! type of the token
      TokenFlag flag;

      /*!
       * \brief default constructor
       */
      Token();

      /*!
       * \brief constructor
       * \param line_  : line number
       * \param token_ : token value
       * \param flag_  : token type
       */
      Token(const unsigned int,
	    const std::string&,
	    const TokenFlag = Standard);

      /*!
       * \brief copy constructor
       * \param src : token to be copied
       */
      Token(const Token&);

      /*!
       * \brief assignement operator
       * \param src : token to be assigned
       * \return this token
       */
      Token&
      operator=(const Token&);

    }; // end of struct Token

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TOKEN_H */

