/*!
 * \file   MFrontUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cctype>
#include <sstream>
#include <ostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/MFrontUtilities.hxx"

namespace mfront {

  void insert_if(std::vector<std::string>& d, const std::string& v) {
    if (v.empty()) {
      return;
    }
    if (std::find(d.begin(), d.end(), v) == d.end()) {
      d.push_back(v);
    }
  }

  void insert_if(std::vector<std::string>& d, const char* const v) {
    if (v == nullptr) {
      return;
    }
    if (strlen(v) == 0) {
      return;
    }
    if (std::find(d.begin(), d.end(), v) == d.end()) {
      d.emplace_back(v);
    }
  }

  void write(std::ostream& os,
             const std::vector<std::string>& v,
             const std::string& id) {
    using tfel::utilities::replace_all;
    if (v.empty()) {
      return;
    }
    os << id << " : {\n";
    for (auto p = v.begin(); p != v.end();) {
      os << "\"" << replace_all(*p, "\"", "\\\"") << "\"";
      if (++p != v.end()) {
        os << ",\n";
      } else {
        os << "\n";
      }
    }
    os << "};\n";
  }  // end of write

  template <>
  double read(tfel::utilities::CxxTokenizer::const_iterator& p,
              const tfel::utilities::CxxTokenizer::const_iterator pe) {
    auto c = p;
    auto r = tfel::utilities::CxxTokenizer::readDouble(c, pe);
    p = c;
    return r;
  }  // end of read

  template <>
  std::string read(tfel::utilities::CxxTokenizer::const_iterator& p,
                   const tfel::utilities::CxxTokenizer::const_iterator pe) {
    auto c = p;
    const auto r = tfel::utilities::CxxTokenizer::readString(c, pe);
    p = c;
    const auto v = tfel::utilities::replace_all(r, "\\\"", "\"");
    return v;
  }  // end of read

  template <>
  std::vector<std::string> read(
      tfel::utilities::CxxTokenizer::const_iterator& p,
      const tfel::utilities::CxxTokenizer::const_iterator pe) {
    auto c = p;
    auto r = tfel::utilities::CxxTokenizer::readStringArray(c, pe);
    for (auto& s : r) {
      s = tfel::utilities::replace_all(s, "\\\"", "\"");
    }
    p = c;
    return r;
  }  // end of read

  std::pair<std::string, VariableBoundsDescription> readVariableBounds(
      tfel::utilities::CxxTokenizer::const_iterator& p,
      const tfel::utilities::CxxTokenizer::const_iterator pe) {
    using tfel::utilities::CxxTokenizer;
    const std::string m = "mfront::readVariableBounds";
    auto throw_if = [&m](const bool b, const std::string& msg) {
      tfel::raise_if(b, m + ": " + msg);
    };
    VariableBoundsDescription b;
    CxxTokenizer::checkNotEndOfLine(m, p, pe);
    auto n = tfel::unicode::getMangledString(p->value);
    ++p;
    CxxTokenizer::checkNotEndOfLine(m, "Expected '[' or '(' or 'in'.", p, pe);
    if (p->value == "[") {
      CxxTokenizer::readSpecifiedToken(m, "[", p, pe);
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      std::string position(p->value);
      std::istringstream converter(position);
      unsigned int pos;
      converter >> pos;
      throw_if(!converter || (!converter.eof()),
               "could not read position number for variable '" + n + "'");
      ++p;
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      CxxTokenizer::readSpecifiedToken(m, "]", p, pe);
      n += '[' + position + ']';
    }
    CxxTokenizer::checkNotEndOfLine(m, "Expected '(' or 'in'.", p, pe);
    if (p->value == "(") {
      CxxTokenizer::readSpecifiedToken(m, "(", p, pe);
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      if (p->value != "*") {
        unsigned int component;
        std::istringstream converter(p->value);
        converter >> component;
        throw_if(!converter || (!converter.eof()),
                 "could not read component number for variable '" + n + "'");
        b.component = component;
      }
      ++(p);
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      CxxTokenizer::readSpecifiedToken(m, ")", p, pe);
    }
    CxxTokenizer::readSpecifiedToken(m, "in", p, pe);
    CxxTokenizer::checkNotEndOfLine(
        m, "Expected '\u211D\u208A', '\u211D\u208B', ']' or '['.", p, pe);
    if (p->value == "\u211D\u208A") {
      b.lowerBound = 0.;
      b.boundsType = VariableBoundsDescription::LOWER;
    } else if (p->value == "\u211D\u208B") {
      b.upperBound = 0.;
      b.boundsType = VariableBoundsDescription::UPPER;
    } else {
      if (p->value == "]") {
        ++p;
        CxxTokenizer::checkNotEndOfLine(m, "Expected '*'.", p, pe);
        throw_if(p->value != "*", "Expected '*' (read '" + p->value + "')");
        b.boundsType = VariableBoundsDescription::UPPER;
        ++p;
      } else if (p->value == "[") {
        ++p;
        CxxTokenizer::checkNotEndOfLine(
            m, "Expected lower bound value for variable '" + n + "'", p, pe);
        b.lowerBound = mfront::read<double>(p, pe);
        b.boundsType = VariableBoundsDescription::LOWERANDUPPER;
      } else {
        throw_if(true, "Expected ']' or '[' (read '" + p->value + "')");
      }
      CxxTokenizer::readSpecifiedToken(m, ":", p, pe);
      CxxTokenizer::checkNotEndOfLine(
          m, "expected upper bound value for variable '" + n + "'", p, pe);
      if (p->value == "*") {
        throw_if(b.boundsType == VariableBoundsDescription::UPPER,
                 "Upper and lower values bounds are both infinity. "
                 "This is inconsistent.");
        b.boundsType = VariableBoundsDescription::LOWER;
        ++p;
        CxxTokenizer::readSpecifiedToken(m, "[", p, pe);
      } else {
        b.upperBound = mfront::read<double>(p, pe);
        if (b.boundsType == VariableBoundsDescription::LOWERANDUPPER) {
          throw_if(b.lowerBound > b.upperBound,
                   "Lower bound value is greater than upper "
                   "bound value for variable '" +
                       n + "'");
        }
        CxxTokenizer::readSpecifiedToken(m, "]", p, pe);
      }
    }
    return std::make_pair(n, b);
  }  // end of readVariableBounds

  std::tuple<std::string, bool, unsigned short>
  extractVariableNameAndArrayPosition(const std::string& n) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "mfront::extractVariableNameAndArrayPosition: " + m);
    };
    unsigned short i = 0;
    auto p = n.cbegin();
    auto pe = n.cend();
    while ((p != pe) && (*p != '[')) {
      ++p;
    }
    if (p == pe) {
      return std::make_tuple(n, false, i);
    }
    auto r = std::string{n.cbegin(), p};
    ++p;
    throw_if(p == pe, "unexpected end of string 'n'");
    throw_if(!std::isdigit(*p), "unexpected a digit 'n'");
    while ((p != pe) && (std::isdigit(*p))) {
      i *= 10;
      i += *p - '0';
      ++p;
    }
    throw_if(p == pe, "unexpected end of string '" + n + "'");
    throw_if(*p != ']', "invalid variable name '" + n + "'");
    ++p;
    throw_if(p != pe, "invalid variable name '" + n + "'");
    return std::make_tuple(r, true, i);
  }  // end of extractVariableNameAndArrayPosition

}  // end of namespace mfront
