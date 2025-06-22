/*!
 * \file   mfront/src/CompiledTargetDescriptionBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 mai 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/CompiledTargetDescriptionBase.hxx"

namespace mfront {

  CompiledTargetDescriptionBase::CompiledTargetDescriptionBase(
      const std::string& n, const std::string& p, const std::string& s)
      : name(n),
        prefix(p),
        suffix(s) {
  }  // end of CompiledTargetDescriptionBase::CompiledTargetDescriptionBase

  CompiledTargetDescriptionBase::CompiledTargetDescriptionBase(
      const CompiledTargetDescriptionBase&) = default;
  CompiledTargetDescriptionBase::CompiledTargetDescriptionBase(
      CompiledTargetDescriptionBase&&) = default;
  CompiledTargetDescriptionBase::~CompiledTargetDescriptionBase() = default;

}  // end of namespace mfront
