/*!
 * \file   mtest/src/CastemFiniteStrainBehaviour2.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MTest/CastemFiniteStrainBehaviour2.hxx"

namespace mtest {

  CastemFiniteStrainBehaviour2::CastemFiniteStrainBehaviour2(
      const Hypothesis h, const std::string& l, const std::string& b)
      : CastemSmallStrainBehaviour(h, l, b) {}

  CastemFiniteStrainBehaviour2::~CastemFiniteStrainBehaviour2() = default;

}  // end of namespace mtest
