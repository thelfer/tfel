/*!
 * \file  mtest/src/AccelerationAlgorithm.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <limits>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  unsigned short AccelerationAlgorithm::convertToUnsignedShort(
      const std::string& m, const std::string& v) {
    // the value is read as a signed integer so that negative values are
    // detected, and only trailing white spaces are allowed after it
    std::istringstream converter(v);
    auto res = long{};
    converter >> res;
    tfel::raise_if((!converter) || (!(converter >> std::ws).eof()) ||
                       (res < 0) ||
                       (res > std::numeric_limits<unsigned short>::max()),
                   m + ": can't convert string '" + v +
                       "' "
                       "to unsigned short");
    return static_cast<unsigned short>(res);
  }

  AccelerationAlgorithm::~AccelerationAlgorithm() = default;

}  // end of namespace mtest
