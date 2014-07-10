/*!
 * \file   Token.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   20 Nov 2006
 */

#include"TFEL/Utilities/Token.hxx"

namespace tfel{

  namespace utilities{

    Token::Token()
      : line(0u),value(),flag(Standard)
    {}

    Token::Token(const unsigned short line_,const std::string& token_,
		 const TokenFlag flag_)
      : line(line_),value(token_),flag(flag_)
    {}
  
    Token::Token(const Token& src):
      line(src.line),value(src.value),flag(src.flag)
    {}
  
    Token& Token::operator=(const Token& src)
    {
      this->line  = src.line;
      this->value = src.value;
      this->flag  = src.flag;
      return *this;
    }

  } // end of namespace utilities

} // end of namespace tfel
