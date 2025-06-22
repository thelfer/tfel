/*!
 * \file   tfel-unicode-filt.cxx
 * \brief
 * \author Thomas Helfer
 * \date   26/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <iostream>
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"

static void process(const std::string& s) {
  auto r = s;
  for (const auto d :
       tfel::unicode::getSupportedUnicodeCharactersDescriptions()) {
    r = tfel::utilities::replace_all(r, d.m, d.uc);
  }
  std::cout << r << '\n';
}  // end of process

int main(const int argc, const char* const* argv) {
  if (argc > 1) {
    const char* const* p = argv + 1;
    for (; p != argv + argc; ++p) {
      process(*p);
    }
  } else {
    while (!std::cin.eof()) {
      std::string l;
      std::getline(std::cin, l);
      if (!(std::cin.eof() && l.empty())) {
        process(l);
      }
    }
  }
  return EXIT_SUCCESS;
}
