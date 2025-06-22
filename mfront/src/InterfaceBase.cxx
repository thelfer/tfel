/*!
 * \file   mfront/src/InterfaceBase.cxx
 * \brief
 *
 * \author Helfer Thomas
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
      InterfaceBase::TokensContainer::const_iterator& current,
      const InterfaceBase::TokensContainer::const_iterator end,
      const std::string& method,
      const std::string& error) {
    using namespace std;
    if (current == end) {
      --(current);
      string msg(method + " : ");
      msg += "unexpected end of file.";
      if (!error.empty()) {
        msg += "\n" + error;
      }
      msg += "\nError at line " + to_string(current->line);
      throw(runtime_error(msg));
    }
  }  // end of InterfaceBase::checkNotEndOfFile

  void InterfaceBase::readSpecifiedToken(
      InterfaceBase::TokensContainer::const_iterator& current,
      const InterfaceBase::TokensContainer::const_iterator end,
      const std::string& method,
      const std::string& token) {
    using namespace std;
    InterfaceBase::checkNotEndOfFile(current, end, method,
                                     "Expected '" + token + "'.");
    if (current->value != token) {
      string msg(method + " : ");
      msg += "expected '" + token + "', read ";
      msg += current->value;
      msg += ".\n";
      msg += "Error at line : ";
      msg += to_string(current->line);
      throw(runtime_error(msg));
    }
    ++(current);
  }  // end of InterfaceBase::readSpecifiedToken

  void InterfaceBase::throwRuntimeError(
      InterfaceBase::TokensContainer::const_iterator& current,
      const InterfaceBase::TokensContainer::const_iterator end,
      const std::string& method,
      const std::string& m) {
    using namespace std;
    if (current == end) {
      --(current);
    }
    string msg(method);
    if (!m.empty()) {
      msg += " : " + m;
    }
    msg += "\nError at line " + to_string(current->line);
    throw(runtime_error(msg));
  }  // end of InterfaceBase::throwRuntimeError

  std::vector<std::string> InterfaceBase::readArrayOfString(
      InterfaceBase::TokensContainer::const_iterator& current,
      const InterfaceBase::TokensContainer::const_iterator end,
      const std::string& method) {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    InterfaceBase::readSpecifiedToken(current, end, method, "{");
    InterfaceBase::checkNotEndOfFile(current, end, method, "Expected '}'");
    while (current->value != "}") {
      if (current->flag != Token::String) {
        InterfaceBase::throwRuntimeError(current, end, method,
                                         "Expected a string");
      }
      res.push_back(current->value.substr(1, current->value.size() - 2));
      ++(current);
      InterfaceBase::checkNotEndOfFile(current, end, method, "Expected '}'");
      if (!((current->value == "}") || (current->value == ","))) {
        InterfaceBase::throwRuntimeError(
            current, end, method,
            "Expected ',' or '}', read '" + current->value + "'");
      }
      if (current->value == ",") {
        ++(current);
        InterfaceBase::checkNotEndOfFile(current, end, method, "Expected '}'");
        if (current->value == "}") {
          InterfaceBase::throwRuntimeError(current, end, method,
                                           "Expected a string");
        }
      }
    }
    ++(current);
    return res;
  }  // end of InterfaceBase::readArrayOfString

  std::vector<std::string> InterfaceBase::readStringOrArrayOfString(
      InterfaceBase::TokensContainer::const_iterator& current,
      const InterfaceBase::TokensContainer::const_iterator end,
      const std::string& method) {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    InterfaceBase::checkNotEndOfFile(current, end, method,
                                     "Expected a string or '{'");
    if (current->value == "{") {
      return InterfaceBase::readArrayOfString(current, end, method);
    }
    if (current->flag != Token::String) {
      InterfaceBase::throwRuntimeError(current, end, method,
                                       "Expected a string");
    }
    res.push_back(current->value.substr(1, current->value.size() - 2));
    ++(current);
    return res;
  }  // end of InterfaceBase::readStringOrArrayOfString

}  // end of namespace mfront
