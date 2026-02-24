/*!
 * \file   mfront-query/src/QueryUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12/07/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/QueryUtilities.hxx"

namespace mfront {

  void displayBounds(std::ostream& os, const VariableBoundsDescription& b) {
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      os << "Lower\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      os << "Upper\n";
    } else if (b.boundsType == VariableBoundsDescription::LOWERANDUPPER) {
      os << "LowerAndUpper\n";
    } else {
      tfel::raise("unsupported physical bounds type");
    }
  }  // end of displayBounds

  void displayBoundsValue(std::ostream& os,
                          const VariableBoundsDescription& b) {
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      os << "[" << b.lowerBound << ":*[\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      os << "]*:" << b.upperBound << "]\n";
    } else if (b.boundsType == VariableBoundsDescription::LOWERANDUPPER) {
      os << "[" << b.lowerBound << ":" << b.upperBound << "]\n";
    } else {
      tfel::raise("unsupported bounds type");
    }
  }  // end of displayBoundsValue

}  // end of namespace mfront
