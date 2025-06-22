/*!
 * \file   TFEL/Math/Stensor/Internals/HarariEigenSolver.ixx
 * \brief
 * \author Maxence Wangermez/Thomas Helfer
 * \date   13/02/2024
 * \copyright Copyright (C) 2006-2024 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
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

  template <typename real>
  void HarariEigensolver3x3<real>::computeEigenValues(tvector<3u, real>& vp,
                                                      const real A,
                                                      const real B,
                                                      const real C,
                                                      const real D,
                                                      const real E,
                                                      const real F) {
    constexpr auto sqrt3 = Cste<real>::sqrt3;
    constexpr auto one = real{1};
    constexpr const auto one_third = one / 3;
    constexpr auto eye = tmatrix<3u, 3u, real>::Id();
    constexpr const auto eps = std::numeric_limits<real>::min();

    // compute the trace of A
    const auto I1 = (A + B + C);
    const auto tr = one_third * I1;

    // compute deviatoric part of M
    const auto S = tmatrix<3u, 3u, real>{A - tr, D,      E,  //
                                         D,      B - tr, F,  //
                                         E,      F,      C - tr};

    // compute second invariant : J2
    const real J2 =
        ((A - B) * (A - B) + (B - C) * (B - C) + (C - A) * (C - A)) / 6 +
        (D * D + E * E + F * F);

    // compute s
    const real s = std::sqrt(J2 / 3);

    if (s < eps) {
      vp = {tr, tr, tr};
      return;
    }

    // compute T
    const auto S2 = eval(S * S);
    const tmatrix<3u, 3u, real> T = S2 - (one_third * real{2} * J2) * eye;

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
    const auto d = std::sqrt((TmsS) / (TpsS));

    const auto sj = [](const real value) -> int {
      return (real(0) < value) - (value < real(0));
    }(1 - d);

    if (sj * (1 - d) < eps) {
      vp[0] = sqrt3 * s + tr;
      vp[1] = 0 + tr;
      vp[2] = -vp[0] + tr;
      return;
    }

    // compute alpha
    const auto dsj = sj < 0 ? real{1} / d : d;
    const auto alpha = 2 * one_third * std::atan(dsj);

    // distinct eigenvalue
    const auto cd = sj * s * std::cos(alpha);
    vp[0] = 2 * cd + tr;

    // other eigenvalues
    const auto sd = sqrt3 * s * std::sin(alpha);
    vp[1] = -cd + sd + tr;
    vp[2] = -cd - sd + tr;
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
    constexpr const auto cste = Cste<real>::sqrt2;
    const real s[6u] = {A, B, C, D * cste, E * cste, F * cste};
    // computing eigen values
    HarariEigensolver3x3::computeEigenValues(vp, A, B, C, D, E, F);
    StensorComputeEigenVectors<3u>::computeEigenVectors(s, vp, m);
  }

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_INTERNALS_HARARIEIGENSOLVER_IXX */
