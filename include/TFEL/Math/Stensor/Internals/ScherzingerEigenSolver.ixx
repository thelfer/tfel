/*!
 * \file   ScherzingerEigenSolver.ixx
 * \brief
 * \author Maxence Wangermez/Thomas Helfer
 * \date   13/02/2023
 * \copyright Copyright (C) 2006-2023 CEA/DEN, EDF R&D. All rights
 * reserved.
 *
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * This file was adapted from Numerical diagonalization of 3x3
 * matrices of Scherzinger, W. M. and Dohrmann, C. R., see
 * A robust algorithm for finding the eigenvalues and eigenvectors of 3×3
 * symmetric matrices for details.
 *
 */

#ifndef LIB_TFEL_MATH_INTERNALS_SCHERZINGEREIGENSOLVER_IXX
#define LIB_TFEL_MATH_INTERNALS_SCHERZINGEREIGENSOLVER_IXX

namespace tfel::math::internals {

  template <typename t>
  static int sign(t val) {
    return (t(0) < val) - (val < t(0));
  }

  template <typename Container>
  static int argmax(Container& vec) {
    return static_cast<int>(
        std::distance(vec.begin(), std::max_element(vec.begin(), vec.end())));
  }

  template <typename real>
  void ScherzingerEigensolver3x3<real>::computeFirstEigenValue(
      tvector<3u, real>& vp,
      const real A,
      const real B,
      const real C,
      const real D,
      const real E,
      const real F) {
    constexpr auto one = real{1};
    constexpr const auto one_half = one / 2;
    constexpr const auto one_third = one / 3;

    // compute the deviatoric part of M : dev(M)
    const auto I1 = A + B + C;
    const auto tr = one_third * I1;

    // compute second invariant : 1/2*tr(dev(M)*dev(M))
    const auto trMdMd = ((A - tr) * (A - tr) + D * D + E * E) +
                        (D * D + (B - tr) * (B - tr) + F * F) +
                        (E * E + F * F + (C - tr) * (C - tr));
    const auto J2 = one_half * trMdMd;

    if (abs(J2) > std::numeric_limits<real>::epsilon()) {
      // compute third invariant : det(dev(M))
      const auto devM = tmatrix<3u, 3u, real>{A - tr, D,      E,  //
                                              D,      B - tr, F,  //
                                              E,      F,      C - tr};
      const auto J3 = det(devM);

      const auto alpha = [&]() -> real {
        if (J3 / 2 * std::sqrt(power<3>(3 / J2)) + 1 <
            std::numeric_limits<real>::min()) {
          return M_PI;
        }
        if (J3 / 2 * std::sqrt(power<3>(3 / J2)) - 1 >
            std::numeric_limits<real>::min()) {
          return 0;
        }
        return one_third *
               std::acos(J3 / real{2} * std::sqrt(power<3>(real{3} / J2)));
      }();

      const auto eta1 = real{2} * std::sqrt(one_third * J2) * std::cos(alpha);
      vp[0] = eta1 + tr;

    } else {
      vp[0] = real{0} + tr;
    }
  }

  template <typename real>
  void ScherzingerEigensolver3x3<real>::computeProjectionBasis(
      tvector<3u, real>& vp,
      tmatrix<3u, 3u, real>& m,
      const real A,
      const real B,
      const real C,
      const real D,
      const real E,
      const real F) {
    constexpr auto one = real{1};
    constexpr const auto one_third = one / 3;

    // compute the deviatoric part of M : dev(M)
    const auto I1 = A + B + C;
    const auto tr = one_third * I1;

    const auto eta = vp[0] - tr;
    const auto fc = tr + eta;
    const auto r = tmatrix<3u, 3u, real>{A - fc, D,      E,  //
                                         D,      B - fc, F,  //
                                         E,      F,      C - fc};

    const auto listNormr =
        tvector<3u, real>{norm(r.template column_view<0u>()),  //
                          norm(r.template column_view<1u>()),  //
                          norm(r.template column_view<2u>())};
    const int rMaxId = argmax(listNormr);

    const auto s1 = [&]() -> tvector<3u, real> {
      if (listNormr(rMaxId) > std::numeric_limits<real>::min()) {
        if (rMaxId == 0) {
          return r.template column_view<0u>() / listNormr(0);
        } else if (rMaxId == 1) {
          return r.template column_view<1u>() / listNormr(1);
        } else {
          return r.template column_view<2u>() / listNormr(2);
        }
      } else {
        return tvector<3u, real>{1, 0, 0};
      }
    }();

    const auto ri = [&]() -> tmatrix<3u, 2u, real> {
      tmatrix<3u, 2u, real> rVects;
      if (rMaxId == 0) {
        rVects.template column_view<0u>() = r.template column_view<1u>();
        rVects.template column_view<1u>() = r.template column_view<2u>();
        return rVects;
      } else if (rMaxId == 1) {
        rVects.template column_view<0u>() = r.template column_view<0u>();
        rVects.template column_view<1u>() = r.template column_view<2u>();
        return rVects;
      } else {
        rVects.template column_view<0u>() = r.template column_view<0u>();
        rVects.template column_view<1u>() = r.template column_view<1u>();
        return rVects;
      }
    }();

    tmatrix<3u, 2u, real> t;
    const tvector<3u, real> r2 = ri.template column_view<0u>();
    const tvector<3u, real> r3 = ri.template column_view<1u>();
    t.template column_view<0u>() = r2 - (s1 | r2) * s1;
    t.template column_view<1u>() = r3 - (s1 | r3) * s1;

    const auto listNormt =
        tvector<2u, real>{norm(t.template column_view<0u>()),  //
                          norm(t.template column_view<1u>())};

    const int tMaxId = argmax(listNormt);

    const auto s2 = [&]() -> tvector<3u, real> {
      if (listNormr(rMaxId) > std::numeric_limits<real>::min()) {
        if (tMaxId == 0) {
          return t.template column_view<0u>() / listNormt(0);
        } else {
          return t.template column_view<1u>() / listNormt(1);
        }
      }
      return tvector<3u, real>{0, 1, 0};
    }();

    m.template column_view<0u>() =
        cross_product(s1, s2);  // compute first eigen vector
    m.template column_view<1u>() = s1;
    m.template column_view<2u>() = s2;
  }

  template <typename real>
  void ScherzingerEigensolver3x3<real>::computeEigenValues(
      tvector<3u, real>& vp,
      tmatrix<3u, 3u, real>& m,
      const real A,
      const real B,
      const real C,
      const real D,
      const real E,
      const real F) {
    constexpr auto one = real{1};
    constexpr const auto one_half = one / 2;
    constexpr const auto one_third = one / 3;
    const auto I1 = A + B + C;
    const auto tr = one_third * I1;

    const auto devM = tmatrix<3u, 3u, real>{A - tr, D,      E,  //
                                            D,      B - tr, F,  //
                                            E,      F,      C - tr};

    ScherzingerEigensolver3x3::computeFirstEigenValue(vp, A, B, C, D, E, F);
    ScherzingerEigensolver3x3::computeProjectionBasis(vp, m, A, B, C, D, E, F);

    const tvector<3u, real> s1 = m.template column_view<1u>();
    const tvector<3u, real> s2 = m.template column_view<2u>();

    const tmatrix<2u, 2u, real> Abarp = {s1 | devM * s1, s1 * devM | s2,
                                         s2 * devM | s1, s2 * devM | s2};

    const auto eta2 = one_half * (Abarp(0, 0) + Abarp(1, 1)) -
                      one_half * sign(Abarp(0, 0) - Abarp(1, 1)) *
                          std::sqrt(power<2>(Abarp(0, 0) - Abarp(1, 1)) +
                                    4 * Abarp(0, 1) * Abarp(1, 0));
    const auto eta3 = Abarp(0, 0) + Abarp(1, 1) - eta2;

    vp[1] = eta2 + tr;
    vp[2] = eta3 + tr;
  }

  template <typename real>
  void ScherzingerEigensolver3x3<real>::computeEigenVectors(
      tvector<3u, real>& vp,
      tmatrix<3u, 3u, real>& m,
      const real A,
      const real B,
      const real C,
      const real D,
      const real E,
      const real F) {
    // computing eigen values
    ScherzingerEigensolver3x3::computeEigenValues(vp, m, A, B, C, D, E, F);

    const auto a = tmatrix<3u, 3u, real>{A - vp[1], D,         E,  //
                                         D,         B - vp[1], F,  //
                                         E,         F,         C - vp[1]};

    const tvector<3u, real> v1 = m.template column_view<0u>();
    const tvector<3u, real> s1 = m.template column_view<1u>();
    const tvector<3u, real> s2 = m.template column_view<2u>();

    const tvector<3u, real> u1 = a * s1;
    const tvector<3u, real> u2 = a * s2;

    const auto listNormu = tvector<2u, real>{norm(u1), norm(u2)};

    const int uMaxId = argmax(listNormu);

    const auto w1 = [&]() -> tvector<3u, real> {
      if (listNormu(uMaxId) > std::numeric_limits<real>::min()) {
        if (uMaxId == 0) {
          return u1 / listNormu(0);
        } else {
          return u2 / listNormu(1);
        }
      } else {
        return tvector<3u, real>{0, 1, 0};
      }
    }();

    const auto v2 = cross_product(w1, v1);
    m.template column_view<0u>() = v1;
    m.template column_view<1u>() = v2;
    m.template column_view<2u>() = cross_product(v1, v2);
  }

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_INTERNALS_SCHERZINGEREIGENSOLVER_IXX */
