/*!
 * \file  mfront/mtest/AccelerationAlgorithm.cxx
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

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  unsigned short AccelerationAlgorithm::convertToUnsignedShort(
      const std::string& m, const std::string& v) {
    std::istringstream converter(v);
    unsigned short res;
    converter >> res;
    tfel::raise_if((!converter) && (!converter.eof()),
                   m + ": can't convert string '" + v +
                       "' "
                       "to unsigned short");
    return res;
  }

  AccelerationAlgorithm::~AccelerationAlgorithm() = default;

}  // end of namespace mtest
