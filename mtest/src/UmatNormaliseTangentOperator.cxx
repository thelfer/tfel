/*!
 * \file  mfront/mtest/UmatNormaliseTangentOperator.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 15 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <cmath>
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "TFEL/Math/ST2toST2/UmatNormaliseTangentOperator.hxx"

namespace mtest {

  void UmatNormaliseTangentOperator::exe(real* const Kt,
                                         const tfel::math::matrix<real>& D,
                                         const unsigned short d) {
    if (d == 1u) {
      tfel::math::UmatNormaliseTangentOperatorBase<1u, real>::exe(Kt, &D(0, 0));
    } else if (d == 2u) {
      tfel::math::UmatNormaliseTangentOperatorBase<2u, real>::exe(Kt, &D(0, 0));
    } else if (d == 3u) {
      tfel::math::UmatNormaliseTangentOperatorBase<3u, real>::exe(Kt, &D(0, 0));
    }
  }  // end of UmatNormaliseTangentOperator::exe

}  // end of namespace mtest
