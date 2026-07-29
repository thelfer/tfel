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
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/TinyMatrixInvert.hxx"

namespace tfel::math {

  template <typename real>
  struct TransverseIsotropicWalpoleBasis {
    TFEL_HOST_DEVICE [[nodiscard]] static constexpr stensor<3u, real> p(
        const tvector<3u, real>& n) {
      return {n(0) * n(0), n(1) * n(1), n(2) * n(2),
              n(0) * n(1), n(0) * n(2), n(1) * n(2)};
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr stensor<3u, real> q(
        const tvector<3u, real>& n) {
      return stensor<3u, real>::Id() - p(n);
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr st2tost2<3u, real>
    dyadic_ov(const stensor<3u, real>& a, const stensor<3u, real>& b) {
      constexpr auto sqrt2 = Cste<real>::sqrt2;
      constexpr auto isqrt2 = Cste<real>::isqrt2;
      const st2tost2<3u, real> a_b = {a(0) * b(0),
                                      a(3) * b(3),
                                      a(4) * b(4),
                                      isqrt2 * (a(0) * b(3) + a(3) * b(0)),
                                      isqrt2 * (a(0) * b(4) + a(4) * b(0)),
                                      isqrt2 * (a(3) * b(4) + a(4) * b(3)),
                                      a(3) * b(3),
                                      a(1) * b(1),
                                      a(5) * b(5),
                                      isqrt2 * (a(1) * b(3) + a(3) * b(1)),
                                      isqrt2 * (a(3) * b(5) + a(5) * b(3)),
                                      isqrt2 * (a(1) * b(5) + a(5) * b(1)),
                                      a(4) * b(4),
                                      a(5) * b(5),
                                      a(2) * b(2),
                                      isqrt2 * (a(4) * b(5) + a(5) * b(4)),
                                      isqrt2 * (a(2) * b(4) + a(4) * b(2)),
                                      isqrt2 * (a(2) * b(5) + a(5) * b(2)),

                                      sqrt2 * a(0) * b(3),
                                      sqrt2 * a(3) * b(1),
                                      sqrt2 * a(4) * b(5),
                                      a(3) * b(3) + a(0) * b(1),
                                      a(4) * b(3) + a(0) * b(5),
                                      a(4) * b(1) + a(3) * b(5),
                                      sqrt2 * a(0) * b(4),
                                      sqrt2 * a(3) * b(5),
                                      sqrt2 * a(4) * b(2),
                                      a(3) * b(4) + a(0) * b(5),
                                      a(4) * b(4) + a(0) * b(2),
                                      a(4) * b(5) + a(3) * b(2),
                                      sqrt2 * a(3) * b(4),
                                      sqrt2 * a(1) * b(5),
                                      sqrt2 * a(5) * b(2),
                                      a(3) * b(5) + a(1) * b(4),
                                      a(3) * b(2) + a(5) * b(4),
                                      a(5) * b(5) + a(1) * b(2)};
      return a_b;
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr st2tost2<3u, real> E1(
        const tvector<3u, real>& n) {
      return p(n) ^ p(n);
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr st2tost2<3u, real> E2(
        const tvector<3u, real>& n) {
      return (q(n) ^ q(n)) / 2;
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr st2tost2<3u, real> E3(
        const tvector<3u, real>& n) {
      constexpr auto isqrt2 = Cste<real>::isqrt2;
      return isqrt2 * (p(n) ^ q(n));
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr st2tost2<3u, real> E4(
        const tvector<3u, real>& n) {
      constexpr auto isqrt2 = Cste<real>::isqrt2;
      return isqrt2 * (q(n) ^ p(n));
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr st2tost2<3u, real> F(
        const tvector<3u, real>& n) {
      const st2tost2<3u, real> F_ = dyadic_ov(q(n), q(n));
      return F_ - E2(n);
    }

    TFEL_HOST_DEVICE [[nodiscard]] static constexpr st2tost2<3u, real> G(
        const tvector<3u, real>& n) {
      st2tost2<3u, real> G_ = dyadic_ov(p(n), q(n));
      G_ += dyadic_ov(q(n), p(n));
      return G_;
    }

    template <typename T>
    TFEL_HOST_DEVICE [[nodiscard]] static constexpr std::array<T, 6> components(
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
