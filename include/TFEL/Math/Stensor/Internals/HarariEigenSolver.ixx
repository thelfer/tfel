/*!
 * \file   TFEL/Math/Stensor/Internals/HarariEigenSolver.ixx
 * \brief
 * \author Maxence Wangermez/Thomas Helfer
 * \date   13/02/2024
 * \copyright Copyright (C) 2006-2024 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * This file was adapted from Numerical diagonalization of 3x3
 * matrices of Isaac Harari and Uri Albocher, see
 * [1] Computation of eigenvalues of a real, symmetric 3 × 3
 * matrix with particular reference to the pernicious
 * case of two nearly equal eigenvalues
 *
 */

#ifndef LIB_TFEL_MATH_INTERNALS_HARARIEIGENSOLVER_IXX
#define LIB_TFEL_MATH_INTERNALS_HARARIEIGENSOLVER_IXX

namespace tfel::math::internals {

  template <typename t>
  int sign(t val) {
    return (t(0) < val) - (val < t(0));
  }


  template <typename real>
  void HarariEigensolver3x3<real>::computeEigenValues(tvector<3u, real>& vp,
                                                      const real A,
                                                      const real B,
                                                      const real C,
                                                      const real D,
                                                      const real E,
                                                      const real F) {
    // std::cout.precision(14);
    constexpr auto one = real{1};
    constexpr const auto one_half = one / 2;
    constexpr const auto one_third = one / 3;

    tmatrix<3u, 3u, real> eye;
    eye(0, 0) = one;
    eye(1, 1) = one;
    eye(2, 2) = one;

    // compute the trace of A
    const real I1 = (A + B + C);

    // compute deviatoric part of M
    tmatrix<3u, 3u, real> S;
    S(0, 0) = (A - I1 / 3);
    S(1, 1) = (B - I1 / 3);
    S(2, 2) = (C - I1 / 3);
    S(0, 1) = D;
    S(0, 2) = E;
    S(1, 2) = F;
    S(1, 0) = D;
    S(2, 0) = E;
    S(2, 1) = F;

    // compute second invariant : J2
    const real J2 =
        ((A - B) * (A - B) + (B - C) * (B - C) + (C - A) * (C - A)) / 6 +
        (D * D + E * E + F * F);

    // compute s
    const real s = std::sqrt(J2 / 3);

    if (s < std::numeric_limits<real>::min()) {
      vp[0] = 0 + I1 / 3;
      vp[1] = 0 + I1 / 3;
      vp[2] = 0 + I1 / 3;
      return;
    }

    // compute T
    tmatrix<3u, 3u, real> S2;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        S2(i, j) = 0;
        for (int k = 0; k < 3; k++) {
          S2(i, j) += S(i, k) * S(k, j);
        }
      }
    }
    const tmatrix<3u, 3u, real> T = S2 - (real{2} * J2 / 3) * eye;

    // compute d
    real TmsS = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        TmsS += (T(i, j) - s * S(i, j)) * (T(i, j) - s * S(i, j));
      }
    }
    real TpsS = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        TpsS += (T(i, j) + s * S(i, j)) * (T(i, j) + s * S(i, j));
      }
    }
    const real d = std::sqrt((TmsS) / (TpsS));

    const auto sj = [](const real value) -> int {
      return (real(0) < value) - (value < real(0));
    }(1 - d);

    if (sj * (1 - d) < std::numeric_limits<real>::min()) {
      vp[0] = std::sqrt(J2) + I1 / 3;
      vp[1] = 0 + I1 / 3;
      vp[2] = -vp[0] + I1 / 3;
      return;
    }

    const auto alpha = real{2} / 3 * std::atan(std::pow(d, sj));

    // distinct eigenvalue
    const auto cd = sj * s * std::cos(alpha);
    vp[0] = 2 * cd + I1 / 3;

    // other eigenvalues
    const auto sd = std::sqrt(J2) * std::sin(alpha);
    vp[1] = -cd + sd + I1 / 3;
    vp[2] = -cd - sd + I1 / 3;
  }

  template <typename real>
  void HarariEigensolver3x3<real>::computeEigenVectors(tvector<3u, real>& vp,
                                                       tmatrix<3u, 3u, real>& m,
                                                       const real A,
                                                       const real B,
                                                       const real C,
                                                       const real D,
                                                       const real E,
                                                       const real F) {
    constexpr auto one = real{1};
    constexpr const auto one_third = one / 3;
constexpr const auto cste = Cste<real>::sqrt2;
    // compute the deviatoric part of M : dev(M)
    const auto trM_3 = one_third * (A + B + C);
    real s[6u] = {A, B, C, D*Cste<real>::sqrt2, E*Cste<real>::sqrt2, F*Cste<real>::sqrt2};
    // computing eigen values
    HarariEigensolver3x3::computeEigenValues(vp, A, B, C, D, E, F);
    StensorComputeEigenVectors<3u>::computeEigenVectors(s, vp, m);
  }

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_INTERNALS_HARARIEIGENSOLVER_IXX */
