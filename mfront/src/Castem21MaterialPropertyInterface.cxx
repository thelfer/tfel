/*!
 * \file   mfront/src/Castem21MaterialPropertyInterface.cxx
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
#include "MFront/Castem21MaterialPropertyInterface.hxx"

namespace mfront {

  std::string Castem21MaterialPropertyInterface::getName() {
    return "castem21";
  }  // end of getName

  Castem21MaterialPropertyInterface::Castem21MaterialPropertyInterface() =
      default;

  std::pair<bool, Castem21MaterialPropertyInterface::tokens_iterator>
  Castem21MaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator end) {
    tfel::raise_if((std::find(i.begin(), i.end(), "castem21") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Castem21") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Cast3M21") != i.end()),
                   "Castem21MaterialPropertyInterface::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return CastemMaterialPropertyInterface::treatKeyword(k, i, current, end);
  }  // end of treatKeyword

  Castem21MaterialPropertyInterface::~Castem21MaterialPropertyInterface() =
      default;

}  // end of namespace mfront
