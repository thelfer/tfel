/*!
 * \file   include/FSES/syevq3.ixx
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

#ifndef LIB_FSES_SYEVQ3_IXX
#define LIB_FSES_SYEVQ3_IXX

#include <cmath>
#include "FSES/Utilities.hxx"
#include "FSES/sytrd3.hxx"

namespace fses {

  template <typename MatrixType, typename VectorType, typename MatrixType2>
  int syevq3(MatrixType& Q, VectorType& w, const MatrixType2& A) {
    using real = GetMatrixNumericType_t<MatrixType2>;
    constexpr auto zero = real(0);
    constexpr auto one = real(1);
    constexpr auto two = real(2);

    const int n = 3;
    real e[3];  // The third element is used only as temporary workspace
    real g, r, p, f, b, s, c, t;  // Intermediate storage
    int nIter;
    int m;

    // Transform A to real tridiagonal form by the Householder method
    sytrd3(Q, w, e, A);

    // Calculate eigensystem of the remaining real symmetric tridiagonal matrix
    // with the QL method
    //
    // Loop over all off-diagonal elements
    for (int l = 0; l < n - 1; l++) {
      nIter = 0;
      while (1) {
        // Check for convergence and exit iteration loop if off-diagonal
        // element e(l) is zero
        for (m = l; m <= n - 2; m++) {
          g = std::abs(at(w, m)) + std::abs(at(w, m + 1));
          if (is_negligible(std::abs(e[m]), g)) break;
        }
        if (m == l) break;

        if (nIter++ >= 30) return -1;

        // Calculate g = d_m - k
        g = (at(w, l + 1) - at(w, l)) / (e[l] + e[l]);
        r = std::sqrt(square(g) + one);
        if (g > 0) {
          g = at(w, m) - at(w, l) + e[l] / (g + r);
        } else {
          g = at(w, m) - at(w, l) + e[l] / (g - r);
        }
        s = c = one;
        p = zero;
        for (int i = m - 1; i >= l; i--) {
          f = s * e[i];
          b = c * e[i];
          if (std::abs(f) > std::abs(g)) {
            c = g / f;
            r = std::sqrt(square(c) + one);
            e[i + 1] = f * r;
            c *= (s = one / r);
          } else {
            s = f / g;
            r = std::sqrt(square(s) + one);
            e[i + 1] = g * r;
            s *= (c = one / r);
          }

          g = at(w, i + 1) - p;
          r = (at(w, i) - g) * s + two * c * b;
          p = s * r;
          at(w, i + 1) = g + p;
          g = c * r - b;

          // Form eigenvectors
#ifndef FSES_EVALS_ONLY
          for (int k = 0; k < n; k++) {
            t = at(Q, k, i + 1);
            at(Q, k, i + 1) = s * at(Q, k, i) + c * t;
            at(Q, k, i) = c * at(Q, k, i) - s * t;
          }
#endif
        }
        at(w, l) -= p;
        e[l] = g;
        e[m] = zero;
      }
    }
    return 0;
  }

}  // end of namespace fses

#endif /* LIB_FSES_SYEVQ3_IXX */
