/*!
 * \file   include/FSES/syevd3.ixx
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

#ifndef LIB_FSES_SYEVD3_IXX
#define LIB_FSES_SYEVD3_IXX

#include <cmath>
#include <limits>
#include <algorithm>
#include "FSES/syev2.hxx"
#include "FSES/slvsec3.hxx"
#include "FSES/sytrd3.hxx"

namespace fses {

  template <typename MatrixType, typename VectorType, typename MatrixType2>
  int syevd3(MatrixType& Q, VectorType& w, const MatrixType2& A) {
    using real = typename std::decay<decltype(at(A, 0, 0))>::type;
    constexpr const auto zero = real(0);
    constexpr const auto one = real(1);
    constexpr const auto eight = real(8);
    constexpr const auto ulp = std::numeric_limits<real>::epsilon();

    const int n = 3;
    real R[3][3];  // Householder transformation matrix
    real P[3][3];  // Unitary transformation matrix which diagonalizes D + w w^T
    real e[2];     // Off-diagonal elements after Householder transformation
    real d[3];     // Eigenvalues of split matrix in the "divide" step)
    real c, s;     // Eigenvector of 2x2 block in the "divide" step
    real z[3];     // Numerators of secular equation / Updating vector
    real t;        // Miscellaenous temporary stuff

    // Initialize Q
#ifndef FSES_EVALS_ONLY
    fses::fill(Q, zero);
#endif

    // Transform A to real tridiagonal form by the Householder method
    sytrd3(R, w, e, A);

    // "Divide"
    // --------

    // Detect matrices that factorize to avoid multiple eigenvalues in the
    // Divide/Conquer algorithm
    for (int i = 0; i < n - 1; i++) {
      t = std::abs(at(w, i)) + std::abs(at(w, i + 1));
      if (std::abs(e[i]) <= eight * ulp * t) {
        if (i == 0) {
          syev2(d[1], d[2], c, s, at(w, 1), e[1], at(w, 2));
          at(w, 1) = d[1];
          at(w, 2) = d[2];
#ifndef FSES_EVALS_ONLY
          at(Q, 0, 0) = one;
          for (int j = 1; j < n; j++) {
            at(Q, j, 1) = s * R[j][2] + c * R[j][1];
            at(Q, j, 2) = c * R[j][2] - s * R[j][1];
          }
#endif
        } else {
          syev2(d[0], d[1], c, s, at(w, 0), e[0], at(w, 1));
          at(w, 0) = d[0];
          at(w, 1) = d[1];
#ifndef FSES_EVALS_ONLY
          at(Q, 0, 0) = c;
          at(Q, 0, 1) = -s;
          at(Q, 1, 0) = R[1][1] * s;
          at(Q, 1, 1) = R[1][1] * c;
          at(Q, 1, 2) = R[1][2];
          at(Q, 2, 0) = R[2][1] * s;
          at(Q, 2, 1) = R[2][1] * c;
          at(Q, 2, 2) = R[2][2];
#endif
        }
        return 0;
      }
    }

    // Calculate eigenvalues and eigenvectors of 2x2 block
    syev2(d[1], d[2], c, s, at(w, 1) - e[0], e[1], at(w, 2));
    d[0] = at(w, 0) - e[0];

    // "Conquer"
    // ---------

    // Determine coefficients of secular equation
    z[0] = e[0];
    z[1] = e[0] * square(c);
    z[2] = e[0] * square(s);

    // Call slvsec3 with d sorted in ascending order. We make
    // use of the fact that syev2 guarantees d[1] >= d[2].
    if (d[0] < d[2]) {
      slvsec3(P, w, d, z, 0, 2, 1);
    } else if (d[0] < d[1]) {
      slvsec3(P, w, d, z, 2, 0, 1);
    } else {
      slvsec3(P, w, d, z, 2, 1, 0);
    }

#ifndef FSES_EVALS_ONLY
    // Calculate eigenvectors of matrix D + beta * z * z^t and store them in the
    // columns of P
    z[0] = std::sqrt(std::abs(e[0]));
    z[1] = c * z[0];
    z[2] = -s * z[0];

    // Detect duplicate elements in d to avoid division by zero
    t = eight * ulp * (std::abs(d[0]) + std::abs(d[1]) + std::abs(d[2]));
    if (std::abs(d[1] - d[0]) <= t) {
      for (int j = 0; j < n; j++) {
        if (P[0][j] * P[1][j] <= zero) {
          P[0][j] = z[1];
          P[1][j] = -z[0];
          P[2][j] = zero;
        } else {
          for (int i = 0; i < n; i++) {
            P[i][j] = z[i] / P[i][j];
          }
        }
      }
    } else if (std::abs(d[2] - d[0]) <= t) {
      for (int j = 0; j < n; j++) {
        if (P[0][j] * P[2][j] <= zero) {
          P[0][j] = z[2];
          P[1][j] = zero;
          P[2][j] = -z[0];
        } else {
          for (int i = 0; i < n; i++) {
            P[i][j] = z[i] / P[i][j];
          }
        }
      }
    } else {
      for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
          if (std::fpclassify(P[i][j]) == FP_ZERO) {
            P[i][j] = one;
            P[(i + 1) % n][j] = zero;
            P[(i + 2) % n][j] = zero;
            break;
          } else {
            P[i][j] = z[i] / P[i][j];
          }
        }
      }
    }

    // Normalize eigenvectors of D + beta * z * z^t
    for (int j = 0; j < n; j++) {
      t = square(P[0][j]) + square(P[1][j]) + square(P[2][j]);
      t = one / std::sqrt(t);
      for (int i = 0; i < n; i++) {
        P[i][j] *= t;
      }
    }

    // Undo diagonalization of 2x2 block
    for (int j = 0; j < n; j++) {
      t = P[1][j];
      P[1][j] = c * t - s * P[2][j];
      P[2][j] = s * t + c * P[2][j];
    }

    // Undo Householder transformation
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        t = P[k][j];
        for (int i = 0; i < n; i++) {
          at(Q, i, j) += t * R[i][k];
        }
      }
    }
#endif

    return 0;
  }

}  // namespace fses

#endif /* LIB_FSES_SYEVD3_IXX */
