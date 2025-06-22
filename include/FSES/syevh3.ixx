/*!
 * \file   include/FSES/syevh3.ixx
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

#ifndef LIB_FSES_SYEVH3_IXX
#define LIB_FSES_SYEVH3_IXX

#include <cmath>
#include <limits>
#include "FSES/Utilities.hxx"
#include "FSES/syevc3.hxx"
#include "FSES/syevq3.hxx"

namespace fses {

  template <typename MatrixType, typename VectorType, typename MatrixType2>
  int syevh3(MatrixType& Q, VectorType& w, const MatrixType2& A) {
    using real = GetMatrixNumericType_t<MatrixType2>;
    constexpr const auto one = real(1);
    constexpr const auto c256 = real(256);
    constexpr const auto ulp = std::numeric_limits<real>::epsilon();

#ifndef FSES_EVALS_ONLY
    real norm;  // Squared norm or inverse norm of current eigenvector
    //  real n0, n1;        // Norm of first and second columns of A
    real error;  // Estimated maximum roundoff error
    real t, u;   // Intermediate storage
    int j;       // Loop counter
#endif

    // Calculate eigenvalues
    syevc3(w, A);

#ifndef FSES_EVALS_ONLY
    //  n0 = square(A[0][0]) + square(A[0][1]) + square(A[0][2]);
    //  n1 = square(A[0][1]) + square(A[1][1]) + square(A[1][2]);

    t = std::abs(at(w, 0));
    if ((u = std::abs(at(w, 1))) > t) {
      t = u;
    }
    if ((u = std::abs(at(w, 2))) > t) {
      t = u;
    }
    if (t < one) {
      u = t;
    } else {
      u = square(t);
    }
    error = c256 * ulp * square(u);
    //  error = c256 * ulp * (n0 + u) * (n1 + u);

    at(Q, 0, 1) = at(A, 0, 1) * at(A, 1, 2) - at(A, 0, 2) * at(A, 1, 1);
    at(Q, 1, 1) = at(A, 0, 2) * at(A, 0, 1) - at(A, 1, 2) * at(A, 0, 0);
    at(Q, 2, 1) = square(at(A, 0, 1));

    // Calculate first eigenvector by the formula
    //   v[0] = (A - at(w,0)).e1 x (A - at(w,0)).e2
    at(Q, 0, 0) = at(Q, 0, 1) + at(A, 0, 2) * at(w, 0);
    at(Q, 1, 0) = at(Q, 1, 1) + at(A, 1, 2) * at(w, 0);
    at(Q, 2, 0) =
        (at(A, 0, 0) - at(w, 0)) * (at(A, 1, 1) - at(w, 0)) - at(Q, 2, 1);
    norm = square(at(Q, 0, 0)) + square(at(Q, 1, 0)) + square(at(Q, 2, 0));

    // If vectors are nearly linearly dependent, or if there might have
    // been large cancellations in the calculation of at(A,i,i) - at(w,0), fall
    // back to QL algorithm
    // Note that this simultaneously ensures that multiple eigenvalues do
    // not cause problems: If at(w,0) = at(w,1), then A - at(w,0) * I has rank
    // 1, i.e. all columns of A - at(w,0) * I are linearly dependent.
    if (norm <= error) {
      return syevq3(Q, w, A);
    } else {  // This is the standard branch
      norm = std::sqrt(one / norm);
      for (j = 0; j < 3; j++) at(Q, j, 0) = at(Q, j, 0) * norm;
    }

    // Calculate second eigenvector by the formula
    //   v[1] = (A - at(w,1)).e1 x (A - at(w,1)).e2
    at(Q, 0, 1) = at(Q, 0, 1) + at(A, 0, 2) * at(w, 1);
    at(Q, 1, 1) = at(Q, 1, 1) + at(A, 1, 2) * at(w, 1);
    at(Q, 2, 1) =
        (at(A, 0, 0) - at(w, 1)) * (at(A, 1, 1) - at(w, 1)) - at(Q, 2, 1);
    norm = square(at(Q, 0, 1)) + square(at(Q, 1, 1)) + square(at(Q, 2, 1));
    if (norm <= error) {
      return syevq3(Q, w, A);
    } else {
      norm = std::sqrt(one / norm);
      for (j = 0; j < 3; j++) {
        at(Q, j, 1) = at(Q, j, 1) * norm;
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

#endif /* LIB_FSES_SYEVH3_IXX */
