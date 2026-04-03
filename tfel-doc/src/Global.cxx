/*!
 * \file  tfel-doc/src/Global.cxx
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

#include <cstring>
#include <iostream>

#include "TFEL/Utilities/Global.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

namespace tfel::utilities {

  std::map<std::string, std::map<std::string, std::string>>& getTypeMap() {
    static std::map<std::string, std::map<std::string, std::string>> t;
    return t;
  }

  std::map<std::string, std::string>& getTypeMap(const std::string& l) {
    static std::map<std::string, std::map<std::string, std::string>> t;
    return t[l];
  }

  std::map<std::string, std::map<std::string, std::string>>&
  getCategoriesMap() {
    static std::map<std::string, std::map<std::string, std::string>> c;
    return c;
  }

  std::map<std::string, std::string>& getCategoriesMap(const std::string& l) {
    return getCategoriesMap()[l];
  }

  std::map<std::string, std::map<std::string, std::string>>& getKeysMap() {
    static std::map<std::string, std::map<std::string, std::string>> k;
    return k;
  }

  std::map<std::string, std::string>& getKeysMap(const std::string& l) {
    return getKeysMap()[l];
  }

  std::map<std::string, std::map<std::string, std::string>>&
  getTranslationsMap() {
    static std::map<std::string, std::map<std::string, std::string>> k;
    return k;
  }

  std::map<std::string, std::string>& getTranslationsMap(const std::string& l) {
    return getTranslationsMap()[l];
  }

  std::string getTranslation(const std::string& s, const std::string& l) {
    using namespace tfel::utilities;
    const auto& translations = getTranslationsMap(l);
    if (l == "english") {
      return s;
    }
    const auto pk = translations.find(s);
    if (pk != translations.end()) {
      return pk->second;
    }
    std::cerr << TerminalColors::Red;
    std::cerr << "getTranslation : no translation found for '" << s
              << "' in language '" + l + "', using default" << std::endl;
    std::cerr << TerminalColors::Reset;
    return s;
  }

  std::string getCategory(const std::string& s, const std::string& l) {
    using namespace tfel::utilities;
    const auto& categories = getCategoriesMap(l);
    const auto pk = categories.find(s);
    if (pk != categories.end()) {
      return pk->second;
    }
    std::cerr << TerminalColors::Red;
    std::cerr << "getCategory : no category found for '" << s
              << "' in language '" + l + "', using default" << std::endl;
    std::cerr << TerminalColors::Reset;
    return s;
  }

  std::string getKeyValue(const std::string& s, const std::string& l) {
    using namespace tfel::utilities;
    const auto& keys = getKeysMap(l);
    if (s.empty()) {
      return s;
    }
    if (s[0] == '!') {
      return s.substr(1);
    }
    const auto pk = keys.find(s);
    if (pk != keys.end()) {
      return pk->second;
    }
    std::cerr << TerminalColors::Red;
    std::cerr << "getKeyValue : no translation found for key '" << s
              << "' in language '" + l + "', using default" << std::endl;
    std::cerr << TerminalColors::Reset;
    return s;
  }

  std::string capitalize(const std::string& s) {
    std::string r{s};
    auto replace = [&r](const char* const s1, const char* const s2) {
      r.erase(0, std::strlen(s1));
      r.insert(0, s2);
    };
    if (tfel::utilities::starts_with(r, "é")) {
      replace("é", "É");
    } else if (tfel::utilities::starts_with(r, "è")) {
      replace("è", "È");
    } else if (tfel::utilities::starts_with(r, "ê")) {
      replace("ê", "Ê");
    } else if (tfel::utilities::starts_with(r, "à")) {
      replace("à", "À");
    } else if (tfel::utilities::starts_with(r, "ç")) {
      replace("ç", "Ç");
    } else {
      r[0] = static_cast<char>(toupper(r[0]));
    }
    return r;
  }  // end of capitalize

}  // end of namespace tfel::utilities
