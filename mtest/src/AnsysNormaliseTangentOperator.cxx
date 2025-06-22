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
#include "TFEL/Math/ST2toST2/ConstST2toST2View.hxx"
#include "MTest/AnsysNormaliseTangentOperator.hxx"

namespace mtest {

  void AnsysNormaliseTangentOperator::exe(real* const Kt,
                                          const tfel::math::matrix<real>& D,
                                          const unsigned short d) {
    using namespace tfel::math;
    if (d == 1u) {
      ST2toST2View<1u, real> k(Kt);
      ConstST2toST2View<1u, real> m(&D(0, 0));
      // transpose
      for (unsigned short i = 0; i != 3u; ++i) {
        for (unsigned short j = 0; j != 3u; ++j) {
          k(i, j) = m(i, j);
        }
      }
    } else if (d == 2u) {
      constexpr const auto icste = Cste<real>::isqrt2;
      ST2toST2View<2u, real> k(Kt);
      ConstST2toST2View<2u, real> m(&D(0, 0));
      // transpose
      for (unsigned short i = 0; i != 4u; ++i) {
        for (unsigned short j = 0; j != 4u; ++j) {
          k(i, j) = m(j, i);
        }
      }
      k(0, 3) *= icste;
      k(1, 3) *= icste;
      k(2, 3) *= icste;
      k(3, 0) *= icste;
      k(3, 1) *= icste;
      k(3, 2) *= icste;
    } else if (d == 3u) {
      constexpr const auto icste = Cste<real>::isqrt2;
      ST2toST2View<3u, real> k(Kt);
      ConstST2toST2View<3u, real> m(&D(0, 0));
      // transpose
      for (unsigned short i = 0; i != 6u; ++i) {
        for (unsigned short j = 0; j != 6u; ++j) {
          k(i, j) = m(j, i);
        }
      }
      k(0, 3) *= icste;
      k(1, 3) *= icste;
      k(2, 3) *= icste;
      k(0, 4) *= icste;
      k(1, 4) *= icste;
      k(2, 4) *= icste;
      k(0, 5) *= icste;
      k(1, 5) *= icste;
      k(2, 5) *= icste;
      k(3, 0) *= icste;
      k(3, 1) *= icste;
      k(3, 2) *= icste;
      k(4, 0) *= icste;
      k(4, 1) *= icste;
      k(4, 2) *= icste;
      k(5, 0) *= icste;
      k(5, 1) *= icste;
      k(5, 2) *= icste;
    }
  }  // end of AnsysNormaliseTangentOperator::exe

}  // end of namespace mtest
