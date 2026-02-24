/*!
 * \file   mfront/src/DSLUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23/04/2025
 */

#include <sstream>
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontWarningMode.hxx"
#include "MFront/DSLUtilities.hxx"

namespace mfront {

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

  template <>
  tfel::utilities::DataMap read(
      tfel::utilities::CxxTokenizer::const_iterator& p,
      const tfel::utilities::CxxTokenizer::const_iterator pe) {
    auto c = p;
    auto d = tfel::utilities::Data::read(c, pe);
    if (d.empty()) {
      p = c;
      return tfel::utilities::DataMap{};
    }
    if (!d.is<tfel::utilities::DataMap>()) {
      tfel::raise("data read is not a DataMap");
    }
    p = c;
    return d.get<tfel::utilities::DataMap>();
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

  bool readSafeOptionTypeIfPresent(
      tfel::utilities::CxxTokenizer::const_iterator& p,
      const tfel::utilities::CxxTokenizer::const_iterator pe) {
    using tfel::utilities::CxxTokenizer;
    const auto m = "readSafeOptionTypeIfPresent";
    CxxTokenizer::checkNotEndOfLine(m, p, pe);
    if (p->value != "<") {
      return false;
    }
    CxxTokenizer::readSpecifiedToken(m, "<", p, pe);
    CxxTokenizer::readSpecifiedToken(m, "safe", p, pe);
    CxxTokenizer::readSpecifiedToken(m, ">", p, pe);
    if (ignoreSafeOptionForWarnings()) {
      return false;
    }
    return true;
  }

}  // end of namespace mfront
