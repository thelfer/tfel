/*!
 * \file  mtest/src/AnsysNormaliseTangentOperator.cxx
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

#include <cmath>
#include <cstdlib>
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Math/ST2toST2/UmatNormaliseTangentOperator.hxx"
#include "MTest/AnsysNormaliseTangentOperator.hxx"

namespace mtest {

  void AnsysNormaliseTangentOperator::exe(real* const Kt,
                                          const tfel::math::matrix<real>& D,
                                          const unsigned short d) {
    if (d == 1u) {
      tfel::math::UmatNormaliseTangentOperatorBase<1u, real>::exe(Kt, &D(0, 0));
    } else if (d == 2u) {
      tfel::math::UmatNormaliseTangentOperatorBase<2u, real>::exe(Kt, &D(0, 0));
    } else if (d == 3u) {
      tfel::math::UmatNormaliseTangentOperatorBase<3u, real>::exe(Kt, &D(0, 0));
      // now changing to Voigt conventions: 23 <-> 13
      // changing last columns
      tfel::math::ST2toST2View<3u, real> K(Kt);
      std::swap(K(0, 4), K(0, 5));
      std::swap(K(1, 4), K(1, 5));
      std::swap(K(2, 4), K(2, 5));
      std::swap(K(3, 4), K(3, 5));
      std::swap(K(4, 4), K(4, 5));
      std::swap(K(5, 4), K(5, 5));
      // changing last rows
      std::swap(K(4, 0), K(5, 0));
      std::swap(K(4, 1), K(5, 1));
      std::swap(K(4, 2), K(5, 2));
      std::swap(K(4, 3), K(5, 3));
      std::swap(K(4, 4), K(5, 4));
      std::swap(K(4, 5), K(5, 5));
    }
  }  // end of AnsysNormaliseTangentOperator::exe

}  // end of namespace mtest
