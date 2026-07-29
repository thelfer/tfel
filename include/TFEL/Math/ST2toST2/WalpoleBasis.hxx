/*!
 * \file   include/TFEL/Math/ST2toST2/WalpoleBasis.hxx
 * \brief  This file enables some operations in different Walpole
 * bases relative to different cases of symmetry for st2tost2.
 * \author Antoine Martin
 * \date   15 may 2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_WALPOLEBASIS_HXX
#define LIB_TFEL_MATH_WALPOLEBASIS_HXX

#include <cmath>

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/TinyMatrixInvert.hxx"

namespace tfel::math {

  template <typename real>
  struct TransverseIsotropicWalpoleBasis {
    TFEL_HOST_DEVICE static stensor<3u, real> p(const tvector<3u, real>& n) {
      constexpr auto sqrt2 = Cste<real>::sqrt2;
      const stensor<3u, real> p_ = {n(0) * n(0),        n(1) * n(1),
                                    n(2) * n(2),        sqrt2 * n(0) * n(1),
                                    sqrt2 * n(0) * n(2), sqrt2 * n(1) * n(2)};
      return p_;
    }

    TFEL_HOST_DEVICE static stensor<3u, real> q(const tvector<3u, real>& n) {
      return stensor<3u, real>::Id() - p(n);
    }

    TFEL_HOST_DEVICE static st2tost2<3u, real> dyadic_ov(
        const stensor<3u, real>& a, const stensor<3u, real>& b) {
      constexpr auto sqrt2 = Cste<real>::sqrt2;
      constexpr auto isqrt2 = Cste<real>::isqrt2;
      const auto a11 = a(0);
      const auto a22 = a(1);
      const auto a33 = a(2);
      const auto a12 = a(3) / sqrt2;
      const auto a13 = a(4) / sqrt2;
      const auto a23 = a(5) / sqrt2;
      const auto b11 = b(0);
      const auto b22 = b(1);
      const auto b33 = b(2);
      const auto b12 = b(3) / sqrt2;
      const auto b13 = b(4) / sqrt2;
      const auto b23 = b(5) / sqrt2;
      const st2tost2<3u, real> a_b = {a11 * b11,
                                      a12 * b12,
                                      a13 * b13,
                                      isqrt2 * (a11 * b12 + a12 * b11),
                                      isqrt2 * (a11 * b13 + a13 * b11),
                                      isqrt2 * (a12 * b13 + a13 * b12),
                                      a12 * b12,
                                      a22 * b22,
                                      a23 * b23,
                                      isqrt2 * (a22 * b12 + a12 * b22),
                                      isqrt2 * (a12 * b23 + a23 * b12),
                                      isqrt2 * (a22 * b23 + a23 * b22),
                                      a13 * b13,
                                      a23 * b23,
                                      a33 * b33,
                                      isqrt2 * (a13 * b23 + a23 * b13),
                                      isqrt2 * (a33 * b13 + a13 * b33),
                                      isqrt2 * (a33 * b23 + a23 * b33),

                                      sqrt2 * a11 * b12,
                                      sqrt2 * a12 * b22,
                                      sqrt2 * a13 * b23,
                                      a12 * b12 + a11 * b22,
                                      a13 * b12 + a11 * b23,
                                      a13 * b22 + a12 * b23,
                                      sqrt2 * a11 * b13,
                                      sqrt2 * a12 * b23,
                                      sqrt2 * a13 * b33,
                                      a12 * b13 + a11 * b23,
                                      a13 * b13 + a11 * b33,
                                      a13 * b23 + a12 * b33,
                                      sqrt2 * a12 * b13,
                                      sqrt2 * a22 * b23,
                                      sqrt2 * a23 * b33,
                                      a12 * b23 + a22 * b13,
                                      a12 * b33 + a23 * b13,
                                      a23 * b23 + a22 * b33};
      return a_b;
    }

    TFEL_HOST_DEVICE static st2tost2<3u, real> E1(const tvector<3u, real>& n) {
      return p(n) ^ p(n);
    }

    TFEL_HOST_DEVICE static st2tost2<3u, real> E2(const tvector<3u, real>& n) {
      return 0.5 * (q(n) ^ q(n));
    }

    TFEL_HOST_DEVICE static st2tost2<3u, real> E3(const tvector<3u, real>& n) {
      return (1 / sqrt(2)) * p(n) ^ q(n);
    }

    TFEL_HOST_DEVICE static st2tost2<3u, real> E4(const tvector<3u, real>& n) {
      return (1 / sqrt(2)) * q(n) ^ p(n);
    }

    TFEL_HOST_DEVICE static st2tost2<3u, real> F(const tvector<3u, real>& n) {
      const st2tost2<3u, real> F_ = dyadic_ov(q(n), q(n));
      return F_ - E2(n);
    }

    TFEL_HOST_DEVICE static st2tost2<3u, real> G(const tvector<3u, real>& n) {
      st2tost2<3u, real> G_ = dyadic_ov(p(n), q(n));
      G_ += dyadic_ov(q(n), p(n));
      return G_;
    }

    template <typename T>
    TFEL_HOST_DEVICE static std::array<T, 6> components(
        const tvector<3u, real>& n, const st2tost2<3u, T>& P) {
      const auto p1 = trace(E1(n) * P * E1(n));
      const auto p2 = trace(E2(n) * P * E2(n));
      const auto p3 = trace(E1(n) * P * E4(n));
      const auto p4 = trace(E2(n) * P * E3(n));
      const auto pF = quaddot(P, F(n)) / 2;
      const auto pG = quaddot(P, G(n)) / 2;
      return {p1, p2, p3, p4, pF, pG};
    }

  };  // end of TransverseIsotropicWalpoleBasis

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_WALPOLEBASIS_HXX */
