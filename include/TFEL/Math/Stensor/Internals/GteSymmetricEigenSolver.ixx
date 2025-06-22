/*!
 * \file   GteSymmetricEigenSolver.ixx
 * \brief
 * \author David Eberly, Geometric Tools, Redmond WA 98052
 * \date   20/09/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * This file was adapted from Geometric Tools, which is
 * distributed under the Boost Software License, Version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 * http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
 */

#ifndef LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_IXX
#define LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_IXX

namespace tfel {

  namespace math {

    namespace internals {

      template <typename real>
      int GteSymmetricEigensolver3x3<real>::exe(tvector<3u, real>& vp,
                                                tmatrix<3u, 3u, real>& m,
                                                real a00,
                                                real a01,
                                                real a02,
                                                real a11,
                                                real a12,
                                                real a22,
                                                bool aggressive) {
        // compute the Householder reflection H and B = H*A*H, where b02 = 0.
        constexpr const real zero = real(0);
        constexpr const real one = real(1);
        TFEL_CONSTEXPR const real one_half = one / 2;
        bool isRotation = false;
        real c, s;
        getCosSin(a12, -a02, c, s);
        real Q[3][3] = {{c, s, zero}, {s, -c, zero}, {zero, zero, one}};
        real term0 = c * a00 + s * a01;
        real term1 = c * a01 + s * a11;
        real b00 = c * term0 + s * term1;
        real b01 = s * term0 - c * term1;
        term0 = s * a00 - c * a01;
        term1 = s * a01 - c * a11;
        real b11 = s * term0 - c * term1;
        real b12 = s * a02 - c * a12;
        real b22 = a22;

        // Givens reflections, B' = G^T*B*G, preserve tridiagonal matrices.
        int const maxIteration = 2 * (1 + std::numeric_limits<real>::digits -
                                      std::numeric_limits<real>::min_exponent);
        int iteration;
        real c2, s2;

        if (std::abs(b12) <= std::abs(b01)) {
          real saveB00, saveB01, saveB11;
          for (iteration = 0; iteration < maxIteration; ++iteration) {
            // compute the Givens reflection.
            getCosSin(one_half * (b00 - b11), b01, c2, s2);
            s = std::sqrt(one_half * (one - c2));  // >= 1/std::sqrt(2)
            c = one_half * s2 / s;

            // update Q by the Givens reflection.
            update0(Q, c, s);
            isRotation = !isRotation;

            // update B <- Q^T*B*Q, ensuring that b02 is zero and |b12| has
            // strictly decreased.
            saveB00 = b00;
            saveB01 = b01;
            saveB11 = b11;
            term0 = c * saveB00 + s * saveB01;
            term1 = c * saveB01 + s * saveB11;
            b00 = c * term0 + s * term1;
            b11 = b22;
            term0 = c * saveB01 - s * saveB00;
            term1 = c * saveB11 - s * saveB01;
            b22 = c * term1 - s * term0;
            b01 = s * b12;
            b12 = c * b12;

            if (converged(aggressive, b00, b11, b01)) {
              // compute the Householder reflection.
              getCosSin(one_half * (b00 - b11), b01, c2, s2);
              s = std::sqrt(one_half * (one - c2));
              c = one_half * s2 / s;  // >= 1/std::sqrt(2)

              // update Q by the Householder reflection.
              update2(Q, c, s);
              isRotation = !isRotation;

              // update D = Q^T*B*Q.
              saveB00 = b00;
              saveB01 = b01;
              saveB11 = b11;
              term0 = c * saveB00 + s * saveB01;
              term1 = c * saveB01 + s * saveB11;
              b00 = c * term0 + s * term1;
              term0 = s * saveB00 - c * saveB01;
              term1 = s * saveB01 - c * saveB11;
              b11 = s * term0 - c * term1;
              break;
            }
          }
        } else {
          real saveB11, saveB12, saveB22;
          for (iteration = 0; iteration < maxIteration; ++iteration) {
            // compute the Givens reflection.
            getCosSin(one_half * (b22 - b11), b12, c2, s2);
            s = std::sqrt(one_half * (one - c2));  // >= 1/std::sqrt(2)
            c = one_half * s2 / s;

            // update Q by the Givens reflection.
            update1(Q, c, s);
            isRotation = !isRotation;

            // update B <- Q^T*B*Q, ensuring that b02 is zero and |b12| has
            // strictly decreased.  MODIFY...
            saveB11 = b11;
            saveB12 = b12;
            saveB22 = b22;
            term0 = c * saveB22 + s * saveB12;
            term1 = c * saveB12 + s * saveB11;
            b22 = c * term0 + s * term1;
            b11 = b00;
            term0 = c * saveB12 - s * saveB22;
            term1 = c * saveB11 - s * saveB12;
            b00 = c * term1 - s * term0;
            b12 = s * b01;
            b01 = c * b01;

            if (converged(aggressive, b11, b22, b12)) {
              // compute the Householder reflection.
              getCosSin(one_half * (b11 - b22), b12, c2, s2);
              s = std::sqrt(one_half * (one - c2));
              c = one_half * s2 / s;  // >= 1/std::sqrt(2)

              // update Q by the Householder reflection.
              update3(Q, c, s);
              isRotation = !isRotation;

              // update D = Q^T*B*Q.
              saveB11 = b11;
              saveB12 = b12;
              saveB22 = b22;
              term0 = c * saveB11 + s * saveB12;
              term1 = c * saveB12 + s * saveB22;
              b11 = c * term0 + s * term1;
              term0 = s * saveB11 - c * saveB12;
              term1 = s * saveB12 - c * saveB22;
              b22 = s * term0 - c * term1;
              break;
            }
          }
        }

        tvector<3u, real> diagonal = {b00, b11, b22};
        int i0, i1, i2;
        // diagonal[i0] >= diagonal[i1] >= diagonal[i2]
        bool isOdd = sort(diagonal, i0, i1, i2);
        std::swap(i0, i2);  // (i0,i1,i2)->(i2,i1,i0) is odd
        if (isOdd) {
          isRotation = !isRotation;
        }

        vp[0] = diagonal[i0];
        vp[1] = diagonal[i1];
        vp[2] = diagonal[i2];
        m(0, 0) = Q[0][i0];
        m(1, 0) = Q[1][i0];
        m(2, 0) = Q[2][i0];
        m(0, 1) = Q[0][i1];
        m(1, 1) = Q[1][i1];
        m(2, 1) = Q[2][i1];
        m(0, 2) = Q[0][i2];
        m(1, 2) = Q[1][i2];
        m(2, 2) = Q[2][i2];

        // Ensure the columns of Q form a right-handed set.
        if (!isRotation) {
          for (int j = 0; j < 3; ++j) {
            m(j, 2) = -m(j, 2);
          }
        }
        return iteration;
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update0(real Q[3][3],
                                                     real c,
                                                     real s) {
        for (int r = 0; r < 3; ++r) {
          const auto tmp0 = c * Q[r][0] + s * Q[r][1];
          const auto tmp1 = Q[r][2];
          const auto tmp2 = c * Q[r][1] - s * Q[r][0];
          Q[r][0] = tmp0;
          Q[r][1] = tmp1;
          Q[r][2] = tmp2;
        }
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update1(real Q[3][3],
                                                     real c,
                                                     real s) {
        for (int r = 0; r < 3; ++r) {
          const auto tmp0 = c * Q[r][1] - s * Q[r][2];
          const auto tmp1 = Q[r][0];
          const auto tmp2 = c * Q[r][2] + s * Q[r][1];
          Q[r][0] = tmp0;
          Q[r][1] = tmp1;
          Q[r][2] = tmp2;
        }
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update2(real Q[3][3],
                                                     real c,
                                                     real s) {
        for (int r = 0; r < 3; ++r) {
          const auto tmp0 = c * Q[r][0] + s * Q[r][1];
          const auto tmp1 = s * Q[r][0] - c * Q[r][1];
          Q[r][0] = tmp0;
          Q[r][1] = tmp1;
        }
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::update3(real Q[3][3],
                                                     real c,
                                                     real s) {
        for (int r = 0; r < 3; ++r) {
          const auto tmp0 = c * Q[r][1] + s * Q[r][2];
          const auto tmp1 = s * Q[r][1] - c * Q[r][2];
          Q[r][1] = tmp0;
          Q[r][2] = tmp1;
        }
      }

      template <typename real>
      void GteSymmetricEigensolver3x3<real>::getCosSin(real u,
                                                       real v,
                                                       real& cs,
                                                       real& sn) {
        constexpr const real zero = real(0);
        constexpr const real one = real(1);
        const real maxAbsComp = std::max(std::abs(u), std::abs(v));
        if (maxAbsComp > zero) {
          u /= maxAbsComp;  // in [-1,1]
          v /= maxAbsComp;  // in [-1,1]
          const real l = std::sqrt(u * u + v * v);
          cs = u / l;
          sn = v / l;
          if (cs > zero) {
            cs = -cs;
            sn = -sn;
          }
        } else {
          cs = -one;
          sn = zero;
        }
      }

      template <typename real>
      bool GteSymmetricEigensolver3x3<real>::converged(bool aggressive,
                                                       real bDiag0,
                                                       real bDiag1,
                                                       real bSuper) {
        TFEL_CONSTEXPR const auto rmin = std::numeric_limits<real>::min();
        TFEL_CONSTEXPR const auto re = std::numeric_limits<real>::epsilon();
        if (aggressive) {
          return std::abs(bSuper) < rmin;
        } else {
          const auto sum = std::abs(bDiag0) + std::abs(bDiag1);
          return std::abs(bSuper) < rmin + sum * re;
        }
      }

      template <typename real>
      bool GteSymmetricEigensolver3x3<real>::sort(const tvector<3u, real>& d,
                                                  int& i0,
                                                  int& i1,
                                                  int& i2) {
        bool odd;
        if (d[0] < d[1]) {
          if (d[2] < d[0]) {
            i0 = 2;
            i1 = 0;
            i2 = 1;
            odd = true;
          } else if (d[2] < d[1]) {
            i0 = 0;
            i1 = 2;
            i2 = 1;
            odd = false;
          } else {
            i0 = 0;
            i1 = 1;
            i2 = 2;
            odd = true;
          }
        } else {
          if (d[2] < d[1]) {
            i0 = 2;
            i1 = 1;
            i2 = 0;
            odd = false;
          } else if (d[2] < d[0]) {
            i0 = 1;
            i1 = 2;
            i2 = 0;
            odd = true;
          } else {
            i0 = 1;
            i1 = 0;
            i2 = 2;
            odd = false;
          }
        }
        return odd;
      }

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_GTESYMMETRICEIGENSOLVER_IXX */
