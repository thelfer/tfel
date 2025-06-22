/*!
 * \file   include/FSES/sytrd3.ixx
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

#ifndef LIB_FSES_SYTRD3_IXX
#define LIB_FSES_SYTRD3_IXX

#include <cmath>
#include "FSES/Utilities.hxx"

namespace fses {

  template <typename MatrixType,
            typename VectorType,
            typename VectorType2,
            typename MatrixType2>
  void sytrd3(MatrixType& Q,
              VectorType& d,
              VectorType2& e,
              const MatrixType2& A) {
    using real = GetMatrixNumericType_t<MatrixType2>;

    constexpr const auto zero = real(0);
    constexpr const auto one = real(1);
    constexpr const auto two = real(2);
    TFEL_CONSTEXPR const auto one_half = real(1) / real(2);

    const int n = 3;
    real u[n], q[n];
    real omega, f;
    real K, h, g;

    // Initialize Q to the identitity matrix
#ifndef FSES_EVALS_ONLY
    for (int i = 0; i < n; i++) {
      at(Q, i, i) = one;
      for (int j = 0; j < i; j++) {
        at(Q, i, j) = at(Q, j, i) = zero;
      }
    }
#endif

    // Bring first row and column to the desired form
    h = square(at(A, 0, 1)) + square(at(A, 0, 2));
    if (at(A, 0, 1) > 0) {
      g = -std::sqrt(h);
    } else {
      g = std::sqrt(h);
    }
    at(e, 0) = g;
    f = g * at(A, 0, 1);
    u[1] = at(A, 0, 1) - g;
    u[2] = at(A, 0, 2);

    omega = h - f;
    if (omega > zero) {
      omega = one / omega;
      K = zero;
      for (int i = 1; i < n; i++) {
        f = at(A, 1, i) * u[1] + at(A, i, 2) * u[2];
        q[i] = omega * f;  // p
        K += u[i] * f;     // u* A u
      }
      K *= one_half * square(omega);

      for (int i = 1; i < n; i++) {
        q[i] = q[i] - K * u[i];
      }
      at(d, 0) = at(A, 0, 0);
      at(d, 1) = at(A, 1, 1) - two * q[1] * u[1];
      at(d, 2) = at(A, 2, 2) - two * q[2] * u[2];

      // Store inverse Householder transformation in Q
#ifndef FSES_EVALS_ONLY
      for (int j = 1; j < n; j++) {
        f = omega * u[j];
        for (int i = 1; i < n; i++) {
          at(Q, i, j) = at(Q, i, j) - f * u[i];
        }
      }
#endif

      // Calculate updated at(A,1,2) and store it in e[1]
      at(e, 1) = at(A, 1, 2) - q[1] * u[2] - u[1] * q[2];
    } else {
      for (int i = 0; i < n; i++) {
        at(d, i) = at(A, i, i);
      }
      at(e, 1) = at(A, 1, 2);
    }
  }

}  // end of namespace fses

#endif /* LIB_FSES_SYTRD3_IXX */
