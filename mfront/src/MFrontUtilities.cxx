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
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/MFrontUtilities.hxx"

namespace mfront {

  std::string makeUpperCase(std::string_view n) {
    std::string s(n);
    auto p = n.begin();
    auto p2 = s.begin();
    for (; p != n.end(); ++p, ++p2) {
      *p2 = static_cast<char>(toupper(*p));
    }
    return s;
  }  // end of makeUpperCase

  std::string makeLowerCase(std::string_view n) {
    std::string s(n);
    auto p = n.begin();
    auto p2 = s.begin();
    for (; p != n.end(); ++p, ++p2) {
      *p2 = static_cast<char>(tolower(*p));
    }
    return s;
  }  // end of makeLowerCase

  void displayGlossaryEntryCompleteDescription(
      std::ostream& os, const tfel::glossary::GlossaryEntry& e) {
    const auto& k = e.getKey();
    const auto& n = e.getNames();
    const auto& sd = e.getShortDescription();
    const auto& d = e.getDescription();
    if ((!sd.empty()) || (!d.empty())) {
      os << '\n'
         << "For your information, the description of the glossary entry '" << k
         << "' is:\n";
      if (!n.empty()) {
        os << k << ":";
        for (const auto& elem : n) {
          os << " '" << elem << "'";
        }
        os << '\n';
      }
      if (!sd.empty()) {
        os << sd << '\n';
      }
      if (!d.empty()) {
        for (const auto& elem : d) {
          os << elem << '\n';
        }
      }
    }
  }  // end of displayGlossaryEntryCompleteDescription

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

  void addSymbol(std::map<std::string, std::string>& symbols,
                 const std::string_view s,
                 const std::string_view r) {
    const auto sname = std::string{s};
    if (symbols.find(sname) != symbols.end()) {
      if (symbols.at(sname) != r) {
        tfel::raise("addSymbol: symbol '" + sname +
                    "' has multiple replacement strings ('" +
                    symbols.at(sname) + "' and '" + std::string{r} + "')");
      }
    }
    symbols.insert({sname, std::string{r}});
  }  // end of addSymbol

}  // end of namespace mfront
