/*!
 * \file  mfront/src/ImplicitDSLII.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/ImplicitDSLII.hxx"

namespace mfront {

  ImplicitDSLII::ImplicitDSLII() {
    this->mb.declareAsASmallStrainStandardBehaviour();
  }  // end of ImplicitDSLII::ImplicitDSLII

  std::string ImplicitDSLII::getName() {
    return "ImplicitII";
  }  // end of ImplicitDSLII::getName

  std::string ImplicitDSLII::getDescription() {
    return "this parser provides a generic integrator based on a theta method.";
  }  // end of ImplicitDSLII::getDescription

  ImplicitDSLII::~ImplicitDSLII() noexcept = default;

}  // end of namespace mfront
