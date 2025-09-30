/*!
 * \file  LawFunction.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/LawFunction.hxx"

namespace mfront {

  LawFunction::LawFunction() = default;
  LawFunction::LawFunction(LawFunction&&) noexcept = default;
  LawFunction::LawFunction(const LawFunction&) = default;
  LawFunction& LawFunction::operator=(LawFunction&&) noexcept = default;
  LawFunction& LawFunction::operator=(const LawFunction&) = default;
  LawFunction::~LawFunction() = default;

}  // end of namespace mfront
