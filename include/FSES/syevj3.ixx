/*!
 * \file   include/FSES/syevj3.ixx
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

#ifndef LIB_FSES_SYEVJ3_IXX
#define LIB_FSES_SYEVJ3_IXX

#include <cmath>
#include "FSES/Utilities.hxx"

namespace fses {

  template <typename MatrixType, typename VectorType, typename MatrixType2>
  int syevj3(MatrixType& Q, VectorType& w, MatrixType2& A) {
    using real = GetMatrixNumericType_t<MatrixType2>;

    constexpr auto zero = real(0);
    constexpr auto one = real(1);
    constexpr auto c100 = real(100);
    constexpr const auto one_half = one / 2;
    constexpr const auto c1_5 = one / 5;

    const int n = 3;
    real sd, so;  // Sums of diagonal resp. off-diagonal elements
    real s, c,
        t;  // std::sin(phi), std::cos(phi), tan(phi) and temporary storage
    real g, h, z, theta;  // More temporary storage
    real thresh;

    // Initialize Q to the identitity matrix
#ifndef FSES_EVALS_ONLY
    for (int i = 0; i < n; i++) {
      at(Q, i, i) = one;
      for (int j = 0; j < i; j++) {
        at(Q, i, j) = at(Q, j, i) = zero;
      }
    }
#endif

    // Initialize w to diag(A)
    for (int i = 0; i < n; i++) at(w, i) = at(A, i, i);

    // Calculate square(tr(A))
    sd = zero;
    for (int i = 0; i < n; i++) sd += std::abs(at(w, i));
    sd = square(sd);

    // Main iteration loop
    for (int nIter = 0; nIter < 50; nIter++) {
      // Test for convergence
      so = zero;
      for (int p = 0; p < n; p++)
        for (int q = p + 1; q < n; q++) so += std::abs(at(A, p, q));
      if (std::fpclassify(so) == FP_ZERO) return 0;

      if (nIter < 4)
        thresh = c1_5 * so / square(n);
      else
        thresh = zero;

      // Do sweep
      for (int p = 0; p < n; p++) {
        for (int q = p + 1; q < n; q++) {
          g = c100 * std::abs(at(A, p, q));
          if (nIter > 4 && is_negligible(g, std::abs(at(w, p))) &&
              is_negligible(g, std::abs(at(w, q)))) {
            at(A, p, q) = zero;
          } else if (std::abs(at(A, p, q)) > thresh) {
            // Calculate Jacobi transformation
            h = at(w, q) - at(w, p);
            if (is_negligible(g, std::abs(h))) {
              t = at(A, p, q) / h;
            } else {
              theta = one_half * h / at(A, p, q);
              if (theta < zero)
                t = -one / (std::sqrt(one + square(theta)) - theta);
              else
                t = one / (std::sqrt(one + square(theta)) + theta);
            }
            c = one / std::sqrt(one + square(t));
            s = t * c;
            z = t * at(A, p, q);

            // Apply Jacobi transformation
            at(A, p, q) = zero;
            at(w, p) -= z;
            at(w, q) += z;
            for (int r = 0; r < p; r++) {
              t = at(A, r, p);
              at(A, r, p) = c * t - s * at(A, r, q);
              at(A, r, q) = s * t + c * at(A, r, q);
            }
            for (int r = p + 1; r < q; r++) {
              t = at(A, p, r);
              at(A, p, r) = c * t - s * at(A, r, q);
              at(A, r, q) = s * t + c * at(A, r, q);
            }
            for (int r = q + 1; r < n; r++) {
              t = at(A, p, r);
              at(A, p, r) = c * t - s * at(A, q, r);
              at(A, q, r) = s * t + c * at(A, q, r);
            }

            // Update eigenvectors
#ifndef FSES_EVALS_ONLY
            for (int r = 0; r < n; r++) {
              t = at(Q, r, p);
              at(Q, r, p) = c * t - s * at(Q, r, q);
              at(Q, r, q) = s * t + c * at(Q, r, q);
            }
#endif
          }
        }
      }
    }

    return -1;
  }

}  // end of namespace fses

#endif /* LIB_FSES_SYEVJ3_IXX */
