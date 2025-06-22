/*!
 * \file   mtest/src/CastemFiniteStrainBehaviour2.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <cstring>
#include <algorithm>

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Castem/Castem.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/Castem/CastemComputeStiffnessTensor.hxx"

#include "MTest/Evolution.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CastemFiniteStrainBehaviour2.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"

namespace mtest {

  CastemFiniteStrainBehaviour2::CastemFiniteStrainBehaviour2(
      const Hypothesis h, const std::string& l, const std::string& b)
      : CastemSmallStrainBehaviour(h, l, b) {}

  CastemFiniteStrainBehaviour2::~CastemFiniteStrainBehaviour2() = default;

}  // end of namespace mtest
