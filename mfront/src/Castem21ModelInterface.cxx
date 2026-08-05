/*!
 * \file   mfront/src/Castem21ModelInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/Castem21ModelInterface.hxx"

namespace mfront {

  std::string Castem21ModelInterface::getName() {
    return "castem21";
  }  // end of getName

  Castem21ModelInterface::Castem21ModelInterface() = default;

  Castem21ModelInterface::~Castem21ModelInterface() = default;

}  // end of namespace mfront
