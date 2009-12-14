/*!
 * \file   Token.hxx
 * \brief  This file declares the Token class
 * \author Helfer Thomas
 * \date   20 Nov 2006
 */

#ifndef _LIB_TFEL_TOKEN_H_
#define _LIB_TFEL_TOKEN_H_ 

#include<string>

namespace tfel{

  namespace utilities{

    struct Token{

      enum TokenFlag{Standard,Comment,String,Char,Preprocessor};

      unsigned short line;
      std::string value;
      TokenFlag flag;

      Token();

      Token(const unsigned short,const std::string&,const TokenFlag = Standard);

      Token(const Token&);

      Token& operator=(const Token&);

    }; // end of struct Token

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_TOKEN_H */

