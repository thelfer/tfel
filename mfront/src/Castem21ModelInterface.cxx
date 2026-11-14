/*!
 * \file   mfront/src/Castem21ModelInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date 15/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/Castem21ModelInterface.hxx"

namespace mfront {

  std::string Castem21ModelInterface::getName() {
    return "castem21";
  }  // end of getName

  Castem21ModelInterface::Castem21ModelInterface() = default;

  std::pair<bool, Castem21ModelInterface::tokens_iterator>
  Castem21ModelInterface::treatKeyword(const std::string& k,
                                       const std::vector<std::string>& i,
                                       tokens_iterator current,
                                       const tokens_iterator end) {
    tfel::raise_if(
        ((std::find(i.begin(), i.end(), this->getName()) != i.end()) ||
         (std::find(i.begin(), i.end(), "Castem21") != i.end()) ||
         (std::find(i.begin(), i.end(), "Cast3M21") != i.end())),
        "Castem21ModelInterface::treatKeyword: unsupported keyword '" + k +
            "'");
    return CastemModelInterface::treatKeyword(k, i, current, end);
  }  // end of treatKeyword

  Castem21ModelInterface::~Castem21ModelInterface() = default;

}  // end of namespace mfront
