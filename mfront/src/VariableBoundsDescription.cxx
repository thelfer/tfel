/*!
 * \file  VariableBoundsDescription.cxx
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

#include "MFront/VariableBoundsDescription.hxx"

namespace mfront {

  bool VariableBoundsDescription::hasLowerBound() const {
    return (this->boundsType == LOWER) || (this->boundsType == LOWERANDUPPER);
  }  // end of VariableBoundsDescription::hasLowerBounds()

  bool VariableBoundsDescription::hasUpperBound() const {
    return (this->boundsType == UPPER) || (this->boundsType == LOWERANDUPPER);
  }  // end of VariableBoundsDescription::hasUpperBounds()

  VariableBoundsDescription::~VariableBoundsDescription() noexcept = default;

}  // end of namespace mfront
