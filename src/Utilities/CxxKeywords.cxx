/*!
 * \file   src/Utilities/CxxKeywords.cxx
 * \brief
 * \author Thomas Helfer
 * \date   16 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <cstdlib>

#include "TFEL/Utilities/CxxKeywords.hxx"

namespace tfel::utilities {

  static const char *reservedCxxKeywords[64] = {
      "asm",          "auto",      "bool",
      "break",        "case",      "catch",
      "char ",        "class",     "const",
      "const_cast",   "continue",  "default",
      "delete",       "do",        "double",
      "dynamic_cast", "else",      "enum",
      "explicit",     "export",    "extern",
      "false",        "float",     "for",
      "friend",       "goto",      "if",
      "inline",       "int",       "long",
      "mutable",      "namespace", "new",
      "operator",     "private",   "protected",
      "public",       "register",  "reinterpret_cast",
      "return",       "short",     "signed",
      "sizeof",       "static",    "static_cast",
      "struct",       "switch",    "template",
      "this",         "throw",     "true",
      "try",          "typedef",   "typeid",
      "typename",     "union",     "unsigned",
      "using",        "virtual",   "void",
      "volatile",     "wchar_t",   "while",
      "constexpr"};

  std::size_t getNumberOfCxxKeywords() {
    return sizeof(reservedCxxKeywords) / sizeof(char *);
  }

  bool isReservedCxxKeywords(std::string_view s) {
    for (std::size_t i = 0; i < getNumberOfCxxKeywords(); ++i) {
      if (s == reservedCxxKeywords[i]) {
        return true;
      }
    }
    return false;
  }

}  // end of namespace tfel::utilities
