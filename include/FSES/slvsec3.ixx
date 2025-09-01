/*!
 * \file   include/FSES/slvsec3.ixx
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

#ifndef LIB_FSES_SLVSEC3_IXX
#define LIB_FSES_SLVSEC3_IXX

#include <cmath>
#include <limits>
#include <utility>
#include "FSES/Utilities.hxx"

namespace fses {

  template <typename MatrixType, typename VectorType, typename real>
  void slvsec3(MatrixType& R,
               VectorType& w,
               const real d[3],
               const real z[3],
               int i0,
               int i1,
               int i2) {
    constexpr auto c_sqrt3 = real(1.73205080756887729352744634151);
    constexpr auto zero = real(0);
    constexpr auto one = real(1);
    constexpr auto two = real(2);
    constexpr auto three = real(3);
    constexpr auto four = real(4);
    constexpr auto eight = real(8);
    constexpr auto c27 = real(27);
    constexpr auto one_half = one / two;

    real a[4];   // Bounds of the intervals bracketing the roots
    real delta;  // Shift of the d_i which ensures better accuracy
    real dd[3];  // Shifted coefficients dd_i = d_i - delta
    real xl,
        xh;  // Interval which straddles the current root. f(xl) < 0, f(xh) > 0
    real x;  // Current estimates for the root
    real x0[3];      // Analytically calculated roots, used as starting values
    real F, dF;      // Function value f(x) and derivative f'(x)
    real dx, dxold;  // Current and last stepsizes
    real error;      // Numerical error estimate, used for termination condition
    real t[3];       // Temporary storage used for evaluating f
    real alpha, beta,
        gamma;  // Coefficients of polynomial f(x) * Product [ d_i - x ]
    real p, sqrt_p, q, c, s,
        phi;  // Intermediate results of analytical calculation

    // Determine intervals which must contain the roots
    if (z[0] > 0) {
      a[0] = d[i0];
      a[1] = d[i1];
      a[2] = d[i2];
      a[3] = std::abs(d[0] + three * z[0]) + std::abs(d[1] + three * z[1]) +
             std::abs(d[2] + three * z[2]);
    } else {
      a[0] = -std::abs(d[0] + three * z[0]) - std::abs(d[1] + three * z[1]) -
             std::abs(d[2] + three * z[2]);
      a[1] = d[i0];
      a[2] = d[i1];
      a[3] = d[i2];
    }

    // Calculate roots of f(x) = 0 analytically (analogous to ZHEEVC3)
    t[0] = d[1] * d[2];
    t[1] = d[0] * d[2];
    t[2] = d[0] * d[1];
    gamma = t[0] * d[0] +
            (z[0] * t[0] + z[1] * t[1] + z[2] * t[2]);  // Coefficients
    beta =
        (z[0] * (d[1] + d[2]) + z[1] * (d[0] + d[2]) + z[2] * (d[0] + d[1])) +
        (t[0] + t[1] + t[2]);
    alpha = (z[0] + z[1] + z[2]) + (d[0] + d[1] + d[2]);

    p = square(alpha) -
        three * beta;  // Transformation that removes the x^2 term
    q = alpha * (p - (three / two) * beta) + (c27 / two) * gamma;
    sqrt_p = std::sqrt(std::abs(p));

    phi = c27 *
          (square(beta) * (p - beta) / four - gamma * (q - c27 / four * gamma));
    phi = (one / three) * std::atan2(std::sqrt(std::abs(phi)), q);
    c = sqrt_p * std::cos(phi);
    s = (one / c_sqrt3) * sqrt_p * std::abs(std::sin(phi));

    x0[0] = x0[1] = x0[2] = (one / three) * (alpha - c);
    if (c > s) {  // Make sure the roots are in ascending order
      x0[0] -= s;
      x0[1] += s;
      x0[2] += c;
    } else if (c < -s) {
      x0[0] += c;
      x0[1] -= s;
      x0[2] += s;
    } else {
      x0[0] -= s;
      x0[1] += c;
      x0[2] += s;
    }

    // Refine roots with a combined Bisection/Newton-Raphson method
    for (int i = 0; i < 3; i++) {
      xl = a[i];      // Lower bound of bracketing interval
      xh = a[i + 1];  // Upper bound of bracketing interval
      dx = dxold = one_half * (xh - xl);

      // Make sure that xl != xh
      if (std::fpclassify(dx) == FP_ZERO) {
        w[i] = xl;
        for (int j = 0; j < 3; j++) {
          R[j][i] = d[j] - xl;
        }
        continue;
      }

      // Shift the root close to zero to achieve better accuracy
      if (x0[i] >= xh) {
        delta = xh;
        x = -dx;
        for (int j = 0; j < 3; j++) {
          dd[j] = d[j] - delta;
          R[j][i] = dd[j] - x;
        }
      } else if (x0[i] <= xl) {
        delta = xl;
        x = dx;
        for (int j = 0; j < 3; j++) {
          dd[j] = d[j] - delta;
          R[j][i] = dd[j] - x;
        }
      } else {
        delta = x0[i];
        x = zero;
        for (int j = 0; j < 3; j++) {
          R[j][i] = dd[j] = d[j] - delta;
        }
      }
      xl -= delta;
      xh -= delta;

      // Make sure that f(xl) < 0 and f(xh) > 0
      if (z[0] < zero) {
        std::swap(xh, xl);
      }

      // Main iteration loop
      for (int nIter = 0; nIter < 500; nIter++) {
        // Evaluate f and f', and calculate an error estimate
        F = one;
        dF = zero;
        error = one;
        for (int j = 0; j < 3; j++) {
          t[0] = one / R[j][i];
          t[1] = z[j] * t[0];
          t[2] = t[1] * t[0];
          F += t[1];
          error += std::abs(t[1]);
          dF += t[2];
        }

        // Check for convergence
        if (std::abs(F) <= std::numeric_limits<real>::epsilon() *
                               (eight * error + std::abs(x * dF)))
          break;

        // Adjust interval boundaries
        if (F < zero)
          xl = x;
        else
          xh = x;

        // Check, whether Newton-Raphson would converge fast enough. If so,
        // give it a try. If not, or if it would run out of bounds, use
        // bisection
        if (std::abs(two * F) < std::abs(dxold * dF)) {
          dxold = dx;
          dx = F / dF;
          x = x - dx;
          if ((x - xh) * (x - xl) >= zero) {
            dx = one_half * (xh - xl);
            x = xl + dx;
          }
        } else {
          dx = one_half * (xh - xl);
          x = xl + dx;
        }
        // Prepare next iteration
        for (int j = 0; j < 3; j++) R[j][i] = dd[j] - x;
      }
      // Un-shift result
      w[i] = x + delta;
    }
  }

}  // end of namespace fses

#endif /* LIB_FSES_SLVSEC3_IXX */
