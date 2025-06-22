/*!
 * \file   FSESSymmetricEigenSolver.ixx
 * \brief
 * \author Joachim Kopp/Thomas Helfer
 * \date   30/12/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * This file was adapted from Numerical diagonalization of 3x3
 * matrices with the courtesy of Joachim Kopp, see
 * https://arxiv.org/pdf/physics/0610206v3.pdf for details.
 *
 * Original licence:
 *
 * ----------------------------------------------------------------------------
 * Numerical diagonalization of 3x3 matrices
 * Copyright (C) 2006
 * ----------------------------------------------------------------------------
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
 * ----------------------------------------------------------------------------
 */

#ifndef LIB_TFEL_MATH_INTERNALS_FSESSYMMETRICEIGENSOLVER_IXX
#define LIB_TFEL_MATH_INTERNALS_FSESSYMMETRICEIGENSOLVER_IXX

#include "FSES/syevc3.hxx"
#include "FSES/syevv3.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      template <typename real>
      void FSESAnalyticalSymmetricEigensolver2x2<real>::computeEigenValues(
          tvector<3u, real>& vp, const real A, const real B, const real C) {
        constexpr const auto zero = real{0};
        constexpr const auto one = real{1};
        TFEL_CONSTEXPR const auto one_half = one / 2;
        const auto sm = A + C;
        const auto df = A - C;
        const auto rt = std::sqrt(df * df + 4 * B * B);
        if (sm > zero) {
          vp[0] = one_half * (sm + rt);
          const auto t = one / (vp[0]);
          vp[1] = (A * t) * C - (B * t) * B;
        } else if (sm < zero) {
          vp[1] = one_half * (sm - rt);
          const auto t = one / (vp[1]);
          vp[0] = (A * t) * C - (B * t) * B;
        } else {
          // This case needs to be treated separately to avoid div by 0
          vp[0] = one_half * rt;
          vp[1] = -one_half * rt;
        }
        vp[2] = zero;
      }

      template <typename real>
      void FSESAnalyticalSymmetricEigensolver2x2<real>::computeEigenVectors(
          tvector<3u, real>& vp,
          tmatrix<3u, 3u, real>& m,
          const real A,
          const real B,
          const real C) {
        constexpr const auto rmin = std::numeric_limits<real>::min();
        constexpr const auto zero = real{0};
        constexpr const auto one = real{1};
        TFEL_CONSTEXPR const auto one_half = one / 2;
        // computing eigen values
        FSESAnalyticalSymmetricEigensolver2x2::computeEigenValues(vp, A, B, C);
        // nullify unused terms
        m(0, 2) = m(1, 2) = m(2, 2) = m(2, 0) = m(2, 1) = zero;
        // computing eigen vectors
        const auto df = A - C;
        const auto rt = std::sqrt(df * df + 4 * B * B);
        // Calculate eigenvectors
        if (df > zero) {
          m(0, 0) = df + rt;
        } else {
          m(0, 0) = df - rt;
        }
        if (std::abs(m(0, 0)) > 2 * std::abs(B)) {
          const auto t = -2 * B / m(0, 0);
          m(1, 0) = one / std::sqrt(one + t * t);
          m(0, 0) = t * (m(1, 0));
        } else if (std::abs(B) < rmin) {
          m(0, 0) = one;
          m(1, 0) = zero;
        } else {
          const auto t = -one_half * (m(0, 0)) / B;
          m(0, 0) = one / std::sqrt(one + t * t);
          m(1, 0) = t * (m(0, 0));
        }
        if (df > zero) {
          const auto t = m(0, 0);
          m(0, 0) = -(m(1, 0));
          m(1, 0) = t;
        }
        m(1, 1) = m(0, 0);
        m(0, 1) = -m(1, 0);
      }

      template <typename real>
      void FSESAnalyticalSymmetricEigensolver3x3<real>::computeEigenValues(
          tvector<3u, real>& vp, const tmatrix<3u, 3u, real>& s) {
        fses::syevc3(vp, s);
      }

      template <typename real>
      void FSESAnalyticalSymmetricEigensolver3x3<real>::computeEigenVectors(
          tvector<3u, real>& vp,
          tmatrix<3u, 3u, real>& m,
          tmatrix<3u, 3u, real>& s) {
        fses::syevv3(m, vp, s);
      }

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_FSESSYMMETRICEIGENSOLVER_IXX */
