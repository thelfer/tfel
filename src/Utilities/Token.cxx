/*!
 * \file   src/Utilities/Token.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date 20/11/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Utilities/Token.hxx"

namespace tfel::utilities {

  Token::Token() = default;
  Token::Token(const Token&) = default;
  Token::Token(Token&&) noexcept = default;
  Token& Token::operator=(Token&&) noexcept = default;
  Token& Token::operator=(const Token&) = default;

  Token::Token(const std::string& v,
               const size_type l,
               const size_type o,
               const TokenFlag f)
      : value(v), line(l), offset(o), flag(f) {}

  Token::~Token() noexcept = default;

  bool isComment(const Token& t) {
    return ((t.flag == Token::Comment) || (t.flag == Token::DoxygenComment) ||
            (t.flag == Token::DoxygenBackwardComment));
  }  // end of isComment

  namespace {
    // \return true if the given value is the spelling of a raw string
    // literal, that is if it starts with the `R"` of the `R"delim(` prefix
    // which opens such a literal
    bool isRawStringLiteral(const std::string& v) {
      return ((v.size() > 2u) && (v[0u] == 'R') && (v[1u] == '"') &&
              (v.find('(', 2u) != std::string::npos));
    }  // end of isRawStringLiteral
  }    // end of namespace

  bool isRawString(const Token& t) {
    return isRawStringLiteral(t.value);
  }  // end of isRawString

  std::string getRawStringDelimiter(const Token& t) {
    if (!isRawStringLiteral(t.value)) {
      return {};
    }
    const auto p = t.value.find('(', 2u);
    return t.value.substr(2u, p - 2u);
  }  // end of getRawStringDelimiter

  std::string getContents(const Token& t) {
    const auto& v = t.value;
    if (isRawStringLiteral(v)) {
      const auto p = v.find('(', 2u);
      const auto c = p + 1u;
      const auto suffix = ")" + getRawStringDelimiter(t) + "\"";
      if ((v.size() >= c + suffix.size()) &&
          (v.compare(v.size() - suffix.size(), suffix.size(), suffix) == 0)) {
        return v.substr(c, v.size() - c - suffix.size());
      }
      // the literal is not closed at the end of the file: the contents is
      // what follows the opening prefix
      return v.substr(c);
    }
    // quoted string or character literal: the contents is the value without
    // its first and last characters
    if (v.size() < 2u) {
      return {};
    }
    return v.substr(1u, v.size() - 2u);
  }  // end of getContents

}  // end of namespace tfel::utilities
