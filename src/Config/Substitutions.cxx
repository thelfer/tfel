/*!
 * \file   src/Config/Substitutions.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12/06/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <vector>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Config/Substitutions.hxx"

namespace tfel::config {

  [[nodiscard]] static std::vector<std::string> tokenize(std::string_view s,
                                                         const char c) {
    auto res = std::vector<std::string>{};
    auto b = std::string::size_type{};
    auto e = s.find_first_of(c, b);
    while (std::string::npos != e || std::string::npos != b) {
      // Found a token, add it to the vector.
      res.push_back(std::string{s.substr(b, e - b)});
      b = s.find_first_not_of(c, e);
      e = s.find_first_of(c, b);
    }
    return res;
  }  // end of tokenize

  std::map<std::string, std::string> getTFELDefaultSubstitutions() {
    auto r = std::map<std::string, std::string>{};
#ifdef TFEL_SUBSTITUTIONS
    for (const auto& co : tokenize(TFEL_SUBSTITUTIONS, '@')) {
      const auto kv = tokenize(co, '%');
      if (kv.size() != 2) {
        tfel::raise("internal error: invalid substitution '" + co + "'");
      }
      r.insert({"@" + kv.at(0) + "@", kv.at(1)});
    }
#endif /* TFEL_SUBSTITUTIONS */
    return r;
  }  // end of getTFELDefaultSubstitutions

}  // end of namespace tfel::config