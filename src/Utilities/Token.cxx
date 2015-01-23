/*!
 * \file   src/Utilities/Token.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   20 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Utilities/Token.hxx"

namespace tfel{

  namespace utilities{

    Token::Token()
      : line(0u),value(),flag(Standard)
    {}

    Token::Token(const unsigned int line_,const std::string& token_,
		 const TokenFlag flag_)
      : line(line_),value(token_),flag(flag_)
    {}

    Token::~Token() noexcept
    {}

  } // end of namespace utilities

} // end of namespace tfel
