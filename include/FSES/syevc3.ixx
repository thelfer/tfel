/*!
 * \file   include/FSES/syevc3.ixx
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

#ifndef LIB_FSES_SYEVC3_IXX
#define LIB_FSES_SYEVC3_IXX

#include <cmath>
#include "FSES/Utilities.hxx"

namespace fses {

  template <typename MatrixType, typename VectorType>
  int syevc3(VectorType& w, const MatrixType& A) {
    using real = GetMatrixNumericType_t<MatrixType>;

    constexpr auto c_sqrt3 = real(1.73205080756887729352744634151);
    constexpr auto one = real(1);
    constexpr auto two = real(2);
    constexpr auto three = real(3);
    constexpr auto one_third = real(1) / real(3);
    constexpr auto c1_4 = real(1) / real(4);
    constexpr auto c3_2 = three / real(2);
    constexpr auto c27 = real(27);
    constexpr auto c27_2 = real(27) / real(2);
    constexpr auto c27_4 = real(27) / real(4);

    // Determine coefficients of characteristic poynomial. We write
    //       | a   d   f  |
    //  A =  | d*  b   e  |
    //       | f*  e*  c  |
    const real de = at(A, 0, 1) * at(A, 1, 2);  // d * e
    const real dd = square(at(A, 0, 1));        // d^2
    const real ee = square(at(A, 1, 2));        // e^2
    const real ff = square(at(A, 0, 2));        // f^2
    const real m = at(A, 0, 0) + at(A, 1, 1) + at(A, 2, 2);
    // a*b + a*c + b*c - d^2 - e^2 - f^2
    const real c1 = (at(A, 0, 0) * at(A, 1, 1) + at(A, 0, 0) * at(A, 2, 2) +
                     at(A, 1, 1) * at(A, 2, 2)) -
                    (dd + ee + ff);
    // c*d^2 + a*e^2 + b*f^2 - a*b*c - 2*f*d*e)
    const real c0 =
        (at(A, 2, 2) * dd + at(A, 0, 0) * ee + at(A, 1, 1) * ff -
         at(A, 0, 0) * at(A, 1, 1) * at(A, 2, 2) - two * at(A, 0, 2) * de);

    const real p = square(m) - three * c1;
    const real q = m * (p - c3_2 * c1) - c27_2 * c0;
    const real sqrt_p = std::sqrt(std::abs(p));

    real phi = c27 * (c1_4 * square(c1) * (p - c1) + c0 * (q + c27_4 * c0));
    phi = (one_third)*std::atan2(std::sqrt(std::abs(phi)), q);

    const real c = sqrt_p * std::cos(phi);
    const real s = (one / c_sqrt3) * sqrt_p * std::sin(phi);

    at(w, 1) = one_third * (m - c);
    at(w, 2) = at(w, 1) + s;
    at(w, 0) = at(w, 1) + c;
    at(w, 1) -= s;

    return 0;
  }

}  // end of namespace fses

#endif /* LIB_FSES_SYEVC3_IXX */
