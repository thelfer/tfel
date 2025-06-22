/*!
 * \file   mfront/src/SpecificTargetDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/06/2015
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
#include "MFront/SpecificTargetDescription.hxx"

namespace mfront {

  SpecificTargetDescription::SpecificTargetDescription() = default;
  SpecificTargetDescription::SpecificTargetDescription(
      const SpecificTargetDescription&) = default;
  SpecificTargetDescription::SpecificTargetDescription(
      SpecificTargetDescription&&) = default;
  SpecificTargetDescription::~SpecificTargetDescription() = default;

  SpecificTargetDescription& SpecificTargetDescription::operator=(
      const SpecificTargetDescription&) = default;
  SpecificTargetDescription& SpecificTargetDescription::operator=(
      SpecificTargetDescription&&) = default;

}  // end of namespace mfront
