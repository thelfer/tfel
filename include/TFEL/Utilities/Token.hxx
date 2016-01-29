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

#ifndef LIB_TFEL_UTILITIES_TOKEN_H_
#define LIB_TFEL_UTILITIES_TOKEN_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"


namespace tfel{

  namespace utilities{

    /*!
     * \brief class representing a token in a C++-like file
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT Token
    {
      //! a simple alias
      using size_type = size_t;
      //! possible type of the token
      enum TokenFlag{Standard,Comment,DoxygenComment,
		     DoxygenBackwardComment,
		     String,Char,Preprocessor};
      /*!
       * \brief default constructor
       */
      Token();
      /*!
       * \brief constructor
       * \param[in] l: line number
       * \param[in] v: token value
       * \param[in] f: token type
       */
      Token(const size_type,
	    const std::string&,
	    const TokenFlag = Standard);

      Token(const Token&);
      Token(Token&&);
      Token& operator=(const Token&);
      Token& operator=(Token&&);
      ~Token() noexcept;
      //! line number
      size_type line = 0u;
      //! string holded by the token
      std::string value;
      //! comment
      std::string comment;
      //! type of the token
      TokenFlag flag = Standard;
    }; // end of struct Token

  } // end of namespace utilities

} // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_TOKEN_H_ */

