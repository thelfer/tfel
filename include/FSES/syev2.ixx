/*!
 * \file   include/FSES/syev2.ixx
 * \brief
 * \author Joachim Kopp/Thomas Helfer
 * \date   02 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * ----------------------------------------------------------------------------
 * This file has been introduced in TFEL with the courtesy of Joachim Kopp.
 *
 * Original licence
 *
 * Numerical diagonalization of 3x3 matrcies
 * Copyright (C) 2006  Joachim Kopp
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef LIB_FSES_SYEV2_IXX
#define LIB_FSES_SYEV2_IXX

#include <cmath>
#include "FSES/Utilities.hxx"

namespace fses {

  template <typename real>
  void syev2(real& rt1,
             real& rt2,
             real& cs,
             real& sn,
             const real A,
             const real B,
             const real C) {
    constexpr auto zero = real(0);
    constexpr auto one = real(1);
    constexpr auto two = real(2);
    constexpr auto four = real(4);
    constexpr auto one_half = one / two;

    real sm = A + C;
    real df = A - C;
    real rt = std::sqrt(square(df) + four * B * B);
    real t;

    if (sm > zero) {
      rt1 = one_half * (sm + rt);
      t = one / rt1;
      rt2 = (A * t) * C - (B * t) * B;
    } else if (sm < zero) {
      rt2 = one_half * (sm - rt);
      t = one / rt2;
      rt1 = (A * t) * C - (B * t) * B;
    } else {
      // This case needs to be treated separately to avoid div by 0
      rt1 = one_half * rt;
      rt2 = -one_half * rt;
    }

    // Calculate eigenvectors
    if (df > zero) {
      cs = df + rt;
    } else {
      cs = df - rt;
    }

    if (std::abs(cs) > two * std::abs(B)) {
      t = -two * B / cs;
      sn = one / std::sqrt(one + square(t));
      cs = t * (sn);
    } else if (std::fpclassify(B) == FP_ZERO) {
      cs = one;
      sn = zero;
    } else {
      t = -one_half * (cs) / B;
      cs = one / std::sqrt(one + square(t));
      sn = t * (cs);
    }

    if (df > zero) {
      t = cs;
      cs = -(sn);
      sn = t;
    }
  }

}  // end of namespace fses

#endif /* LIB_FSES_SYEV2_IXX */
