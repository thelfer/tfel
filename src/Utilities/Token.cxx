/*!
 * \file   src/Utilities/Token.cxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   20 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Utilities/Token.hxx"

namespace tfel {

  namespace utilities {

    Token::Token() = default;
    Token::Token(const Token&) = default;
    Token::Token(Token&&) = default;
    Token& Token::operator=(Token&&) = default;
    Token& Token::operator=(const Token&) = default;

    Token::Token(const std::string& v,
                 const size_type l,
                 const size_type o,
                 const TokenFlag f)
        : value(v), line(l), offset(o), flag(f) {}

    Token::~Token() noexcept = default;

  }  // end of namespace utilities

}  // end of namespace tfel
