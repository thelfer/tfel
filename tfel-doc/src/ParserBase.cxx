/*!
 * \file  tfel-doc/src/ParserBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/ParserBase.hxx"

namespace tfel {

  namespace utilities {

    void ParserBase::checkNotEndOfFile(const_iterator p) {
      raise_if(p == this->end(),
               "ParserBase::checkNotEndOfFile: "
               "unexpected end of file");
    }  // end of ParserBase::checkNotEndOfFile

    void ParserBase::readSpecifiedToken(const std::string& v,
                                        const_iterator& p) {
      this->checkNotEndOfFile(p);
      raise_if(p->value != v,
               "ParserBase::readSpecifiedToken: "
               "expected token '" +
                   v +
                   "',"
                   "read '" +
                   p->value + "'");
      ++p;
    }  // end of ParserBase::readSpecifiedToken

    void ParserBase::readString(std::string& s, const_iterator& p) {
      this->checkNotEndOfFile(p);
      raise_if(p->flag != tfel::utilities::Token::String,
               "TestParser::readString: "
               "expected to read a string");
      s = p->value.substr(1, p->value.size() - 2);
      ++p;
    }  // end of ParserBase::readString

    std::string ParserBase::readString(const_iterator& p) {
      std::string r;
      this->readString(r, p);
      return r;
    }  // end of ParserBase::readString

    ParserBase::~ParserBase() noexcept = default;

  }  // end of namespace utilities

}  // end of namespace tfel
