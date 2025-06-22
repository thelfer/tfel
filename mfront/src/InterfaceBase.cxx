/*!
 * \file   mfront/src/InterfaceBase.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   07 nov 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/InterfaceBase.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  void InterfaceBase::checkNotEndOfFile(
      TokensContainer::const_iterator& c,
      const TokensContainer::const_iterator ce,
      const std::string& m,
      const std::string& msg) {
    if (c == ce) {
      --c;
      auto e = m + ": unexpected end of file.";
      if (!msg.empty()) {
        e += "\n" + msg;
      }
      e += "\nError at line " + std::to_string(c->line);
      tfel::raise(e);
    }
  }  // end of InterfaceBase::checkNotEndOfFile

  void InterfaceBase::readSpecifiedToken(
      TokensContainer::const_iterator& c,
      const TokensContainer::const_iterator ce,
      const std::string& m,
      const std::string& v) {
    InterfaceBase::checkNotEndOfFile(c, ce, m, "expected '" + v + "'.");
    tfel::raise_if(c->value != v, m + ": expected '" + v +
                                      "', "
                                      "read '" +
                                      c->value +
                                      "'.\n"
                                      "Error at line:" +
                                      std::to_string(c->line));
    ++c;
  }  // end of InterfaceBase::readSpecifiedToken

  void InterfaceBase::throwRuntimeError(
      TokensContainer::const_iterator& c,
      const TokensContainer::const_iterator ce,
      const std::string& m,
      const std::string& msg) {
    if (c == ce) {
      --c;
    }
    auto e = m;
    if (!msg.empty()) {
      e += ": " + msg;
    }
    e += "\nError at line " + std::to_string(c->line);
    tfel::raise(e);
  }  // end of InterfaceBase::throwRuntimeError

  std::vector<std::string> InterfaceBase::readArrayOfString(
      TokensContainer::const_iterator& c,
      const TokensContainer::const_iterator ce,
      const std::string& m) {
    std::vector<std::string> res;
    InterfaceBase::readSpecifiedToken(c, ce, m, "{");
    InterfaceBase::checkNotEndOfFile(c, ce, m, "expected '}'");
    while (c->value != "}") {
      if (c->flag != tfel::utilities::Token::String) {
        InterfaceBase::throwRuntimeError(c, ce, m, "expected a string");
      }
      res.push_back(c->value.substr(1, c->value.size() - 2));
      ++c;
      InterfaceBase::checkNotEndOfFile(c, ce, m, "expected '}'");
      if (!((c->value == "}") || (c->value == ","))) {
        InterfaceBase::throwRuntimeError(c, ce, m,
                                         "expected ',' or '}',"
                                         " read '" +
                                             c->value + "'");
      }
      if (c->value == ",") {
        ++c;
        InterfaceBase::checkNotEndOfFile(c, ce, m, "expected '}'");
        if (c->value == "}") {
          InterfaceBase::throwRuntimeError(c, ce, m, "expected a string");
        }
      }
    }
    ++c;
    return res;
  }  // end of InterfaceBase::readArrayOfString

  std::vector<std::string> InterfaceBase::readStringOrArrayOfString(
      TokensContainer::const_iterator& c,
      const TokensContainer::const_iterator ce,
      const std::string& m) {
    std::vector<std::string> res;
    InterfaceBase::checkNotEndOfFile(c, ce, m, "expected a string or '{'");
    if (c->value == "{") {
      return InterfaceBase::readArrayOfString(c, ce, m);
    }
    if (c->flag != tfel::utilities::Token::String) {
      InterfaceBase::throwRuntimeError(c, ce, m, "expected a string");
    }
    res.push_back(c->value.substr(1, c->value.size() - 2));
    ++c;
    return res;
  }  // end of InterfaceBase::readStringOrArrayOfString

}  // end of namespace mfront
