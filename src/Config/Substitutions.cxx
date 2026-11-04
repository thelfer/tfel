/*!
 * \file   src/Config/Substitutions.cxx
 * \brief
 * \author Thomas Helfer
 * \date 12/06/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Config/Substitutions.hxx"
#include "TFEL/Config/Substitutions-defines.hxx"

namespace tfel::config {

  std::map<std::string, std::string> getTFELDefaultSubstitutions() {
    auto r = std::map<std::string, std::string>{};
    for (const auto& [k, v] : internals::default_substitutions) {
      r.insert({"@" + std::string{k} + "@", v});
    }
    return r;
  }  // end of getTFELDefaultSubstitutions

}  // end of namespace tfel::config