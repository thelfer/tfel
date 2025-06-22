/*!
 * \file  mfront/src/ImplicitFiniteStrainDSL.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/ImplicitFiniteStrainDSL.hxx"

namespace mfront {

  ImplicitFiniteStrainDSL::ImplicitFiniteStrainDSL() {
    this->mb.setDSLName("ImplicitFiniteStrain");
    this->mb.declareAsAFiniteStrainStandardBehaviour(false);
  }  // end of ImplicitFiniteStrainDSL::ImplicitFiniteStrainDSL

  std::string ImplicitFiniteStrainDSL::getName() {
    return "ImplicitFiniteStrain";
  }  // end of ImplicitFiniteStrainDSL::getName

  std::string ImplicitFiniteStrainDSL::getDescription() {
    return "this parser provides a generic integrator based on a theta method.";
  }  // end of ImplicitFiniteStrainDSL::getDescription

  ImplicitFiniteStrainDSL::~ImplicitFiniteStrainDSL() noexcept = default;

}  // end of namespace mfront
