/*!
 * \file   include/FSES/syevv3.ixx
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

#ifndef LIB_FSES_SYEVV3_IXX
#define LIB_FSES_SYEVV3_IXX

#include <cmath>
#include <limits>
#include "FSES/Utilities.hxx"
#include "FSES/syevc3.hxx"

namespace fses {

  template <typename MatrixType, typename VectorType, typename MatrixType2>
  int syevv3(MatrixType& Q, VectorType& w, MatrixType2& A) {
    using real = GetMatrixNumericType_t<MatrixType2>;

    constexpr const auto zero = real(0);
    constexpr const auto one = real(1);
    constexpr const auto eight = real(8);
    constexpr const auto c64 = real(64);
    constexpr const auto c256 = real(256);
    constexpr const auto ulp = std::numeric_limits<real>::epsilon();

#ifndef FSES_EVALS_ONLY
    real norm;    // Squared norm or inverse norm of current eigenvector
    real n0, n1;  // Norm of first and second columns of A
    real n0tmp,
        n1tmp;  // "Templates" for the calculation of n0/n1 - saves a few FLOPS
    real thresh;  // Small number used as threshold for floating point
                  // comparisons
    real error;   // Estimated maximum roundoff error in some steps
    real wmax;    // The eigenvalue of maximum modulus
    real f, t;    // Intermediate storage
    int i, j;     // Loop counters
#endif

    // Calculate eigenvalues
    syevc3(w, A);

#ifndef FSES_EVALS_ONLY
    wmax = std::abs(at(w, 0));
    if ((t = std::abs(at(w, 1))) > wmax) wmax = t;
    if ((t = std::abs(at(w, 2))) > wmax) wmax = t;
    thresh = square(eight * ulp * wmax);

    // Prepare calculation of eigenvectors
    n0tmp = square(at(A, 0, 1)) + square(at(A, 0, 2));
    n1tmp = square(at(A, 0, 1)) + square(at(A, 1, 2));
    at(Q, 0, 1) = at(A, 0, 1) * at(A, 1, 2) - at(A, 0, 2) * at(A, 1, 1);
    at(Q, 1, 1) = at(A, 0, 2) * at(A, 0, 1) - at(A, 1, 2) * at(A, 0, 0);
    at(Q, 2, 1) = square(at(A, 0, 1));

    // Calculate first eigenvector by the formula
    //   v[0] = (A - at(w,0)).e1 x (A - at(w,0)).e2
    at(A, 0, 0) -= at(w, 0);
    at(A, 1, 1) -= at(w, 0);
    at(Q, 0, 0) = at(Q, 0, 1) + at(A, 0, 2) * at(w, 0);
    at(Q, 1, 0) = at(Q, 1, 1) + at(A, 1, 2) * at(w, 0);
    at(Q, 2, 0) = at(A, 0, 0) * at(A, 1, 1) - at(Q, 2, 1);
    norm = square(at(Q, 0, 0)) + square(at(Q, 1, 0)) + square(at(Q, 2, 0));
    n0 = n0tmp + square(at(A, 0, 0));
    n1 = n1tmp + square(at(A, 1, 1));
    error = n0 * n1;

    if (n0 <= thresh) {
      // If the first column is zero, then (1,0,0) is an eigenvector
      at(Q, 0, 0) = one;
      at(Q, 1, 0) = zero;
      at(Q, 2, 0) = zero;
    } else if (n1 <= thresh) {
      // If the second column is zero, then (0,1,0) is an eigenvector
      at(Q, 0, 0) = zero;
      at(Q, 1, 0) = one;
      at(Q, 2, 0) = zero;
    } else if (norm < square(c64 * ulp) * error) {
      // If angle between A[0] and A[1] is too small, don't use
      // cross product, but calculate v ~ (1, -A0/A1, 0)
      t = square(at(A, 0, 1));
      f = -at(A, 0, 0) / at(A, 0, 1);
      if (square(at(A, 1, 1)) > t) {
        t = square(at(A, 1, 1));
        f = -at(A, 0, 1) / at(A, 1, 1);
      }
      if (square(at(A, 1, 2)) > t) {
        f = -at(A, 0, 2) / at(A, 1, 2);
      }
      norm = one / std::sqrt(1 + square(f));
      at(Q, 0, 0) = norm;
      at(Q, 1, 0) = f * norm;
      at(Q, 2, 0) = zero;
    } else {
      // This is the standard branch
      norm = std::sqrt(one / norm);
      for (j = 0; j < 3; j++) {
        at(Q, j, 0) = at(Q, j, 0) * norm;
      }
    }

    // Prepare calculation of second eigenvector
    t = at(w, 0) - at(w, 1);
    if (std::abs(t) > eight * ulp * wmax) {
      // For non-degenerate eigenvalue, calculate second eigenvector by the
      // formula
      //   v[1] = (A - at(w,1)).e1 x (A - at(w,1)).e2
      at(A, 0, 0) += t;
      at(A, 1, 1) += t;
      at(Q, 0, 1) = at(Q, 0, 1) + at(A, 0, 2) * at(w, 1);
      at(Q, 1, 1) = at(Q, 1, 1) + at(A, 1, 2) * at(w, 1);
      at(Q, 2, 1) = at(A, 0, 0) * at(A, 1, 1) - at(Q, 2, 1);
      norm = square(at(Q, 0, 1)) + square(at(Q, 1, 1)) + square(at(Q, 2, 1));
      n0 = n0tmp + square(at(A, 0, 0));
      n1 = n1tmp + square(at(A, 1, 1));
      error = n0 * n1;

      if (n0 <= thresh) {
        // If the first column is zero, then (1,0,0) is an eigenvector
        at(Q, 0, 1) = one;
        at(Q, 1, 1) = zero;
        at(Q, 2, 1) = zero;
      } else if (n1 <= thresh) {
        // If the second column is zero, then (0,1,0) is an eigenvector
        at(Q, 0, 1) = zero;
        at(Q, 1, 1) = one;
        at(Q, 2, 1) = zero;
      } else if (norm < square(c64 * ulp) * error) {
        // If angle between A[0] and A[1] is too small, don't use
        // cross product, but calculate v ~ (1, -A0/A1, 0)
        t = square(at(A, 0, 1));
        f = -at(A, 0, 0) / at(A, 0, 1);
        if (square(at(A, 1, 1)) > t) {
          t = square(at(A, 1, 1));
          f = -at(A, 0, 1) / at(A, 1, 1);
        }
        if (square(at(A, 1, 2)) > t) {
          f = -at(A, 0, 2) / at(A, 1, 2);
        }
        norm = one / std::sqrt(1 + square(f));
        at(Q, 0, 1) = norm;
        at(Q, 1, 1) = f * norm;
        at(Q, 2, 1) = zero;
      } else {
        norm = std::sqrt(one / norm);
        for (j = 0; j < 3; j++) {
          at(Q, j, 1) = at(Q, j, 1) * norm;
        }
      }
    } else {
      // For degenerate eigenvalue, calculate second eigenvector according to
      //   v[1] = v[0] x (A - at(w,1)).e[i]
      //
      // This would really get to complicated if we could not assume all of A to
      // contain meaningful values.
      at(A, 1, 0) = at(A, 0, 1);
      at(A, 2, 0) = at(A, 0, 2);
      at(A, 2, 1) = at(A, 1, 2);
      at(A, 0, 0) += at(w, 0);
      at(A, 1, 1) += at(w, 0);
      for (i = 0; i < 3; i++) {
        at(A, i, i) -= at(w, 1);
        n0 = square(at(A, 0, i)) + square(at(A, 1, i)) + square(at(A, 2, i));
        if (n0 > thresh) {
          at(Q, 0, 1) = at(Q, 1, 0) * at(A, 2, i) - at(Q, 2, 0) * at(A, 1, i);
          at(Q, 1, 1) = at(Q, 2, 0) * at(A, 0, i) - at(Q, 0, 0) * at(A, 2, i);
          at(Q, 2, 1) = at(Q, 0, 0) * at(A, 1, i) - at(Q, 1, 0) * at(A, 0, i);
          norm =
              square(at(Q, 0, 1)) + square(at(Q, 1, 1)) + square(at(Q, 2, 1));
          if (norm > square(c256 * ulp) * n0) {
            // Accept cross product only if the angle between
            // the two vectors was not too small
            norm = std::sqrt(one / norm);
            for (j = 0; j < 3; j++) at(Q, j, 1) = at(Q, j, 1) * norm;
            break;
          }
        }
      }

      if (i == 3) {
        // This means that any vector orthogonal to v[0] is an EV.
        for (j = 0; j < 3; j++) {
          if (std::fpclassify(at(Q, j, 0)) != FP_ZERO) {
            // Find nonzero element of v[0] ...
            // ... and swap it with the next one
            norm = one / std::sqrt(square(at(Q, j, 0)) +
                                   square(at(Q, (j + 1) % 3, 0)));
            at(Q, j, 1) = at(Q, (j + 1) % 3, 0) * norm;
            at(Q, (j + 1) % 3, 1) = -at(Q, j, 0) * norm;
            at(Q, (j + 2) % 3, 1) = zero;
            break;
          }
        }
      }
    }
    // Calculate third eigenvector according to
    //   v[2] = v[0] x v[1]
    at(Q, 0, 2) = at(Q, 1, 0) * at(Q, 2, 1) - at(Q, 2, 0) * at(Q, 1, 1);
    at(Q, 1, 2) = at(Q, 2, 0) * at(Q, 0, 1) - at(Q, 0, 0) * at(Q, 2, 1);
    at(Q, 2, 2) = at(Q, 0, 0) * at(Q, 1, 1) - at(Q, 1, 0) * at(Q, 0, 1);
#endif

    return 0;
  }

}  // end of namespace fses

#endif /* LIB_FSES_SYEVV3_IXX */
