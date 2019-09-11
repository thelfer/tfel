/*!
 * \file   include/TFEL/Material/MohrCoulombYieldCriterion.ixx
 * \brief
 * \author Thomas Nagel, Thomas Helfer
 * \date   10/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_IXX
#define LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_IXX

#include <cmath>

namespace tfel {

  namespace material {

    template <typename StressStensor>
    MohrCoulombParameters<StressStensor>::MohrCoulombParameters(
        const stress c_c,
        const real phi_c,
        const real lodeT_c,
        const stress a_c)
        : c(c_c),
          phi(phi_c),
          lodeT(lodeT_c),
          a(a_c),
          cos_phi(std::cos(phi_c)),
          sin_phi(std::sin(phi_c)),
          cos_lodeT(std::cos(lodeT_c)),
          sin_lodeT(std::sin(lodeT_c)),
          tan_lodeT(std::tan(lodeT_c)),
          cos_3_lodeT(std::cos(3 * lodeT_c)),
          sin_3_lodeT(std::sin(3 * lodeT_c)),
          tan_3_lodeT(std::tan(3 * lodeT_c)) {
    }  // end of MohrCoulombParameters<StressStensor>::MohrCoulombParameters

    template <typename StressStensor>
    MohrCoulombParameters<StressStensor>::MohrCoulombParameters() = default;

    template <typename StressStensor>
    MohrCoulombParameters<StressStensor>::MohrCoulombParameters(
        MohrCoulombParameters&&) = default;

    template <typename StressStensor>
    MohrCoulombParameters<StressStensor>::MohrCoulombParameters(
        const MohrCoulombParameters&) = default;

    template <typename StressStensor>
    MohrCoulombParameters<StressStensor>& MohrCoulombParameters<StressStensor>::
    operator=(MohrCoulombParameters&&) = default;

    template <typename StressStensor>
    MohrCoulombParameters<StressStensor>& MohrCoulombParameters<StressStensor>::operator=(
        const MohrCoulombParameters&) = default;

    template <typename StressStensor,
              typename MohrCoulombParameters<StressStensor>::AngleUnit u>
    MohrCoulombParameters<StressStensor> makeMohrCoulombParameters(
        const MohrCoulombStressType<StressStensor> c,
        const MohrCoulombBaseType<StressStensor> phi,
        const MohrCoulombBaseType<StressStensor> lodeT,
        const MohrCoulombStressType<StressStensor> a){
      using real = MohrCoulombBaseType<StressStensor>;
      constexpr const real pi = real(3.14159265358979323846);
      if (u == MohrCoulombParameters<StressStensor>::DEGREE) {
        const auto phi_c = phi * pi / 180;
        const auto lodeT_c = phi * pi / 180;
        return MohrCoulombParameters<StressStensor>{c, phi_c, lodeT_c, a};
      }
      return MohrCoulombParameters<StressStensor>{c, phi, lodeT, a};
    }  // end of makeMohrCoulombParameters

    template <typename StressStensor>
    MohrCoulombStressType<StressStensor> computeMohrCoulombStressCriterion(
        const MohrCoulombParameters<StressStensor>& p, const StressStensor& sig) {
      using real = MohrCoulombBaseType<StressStensor>;
      constexpr const auto local_zero_tolerance = real(1e-14);
      constexpr const auto sqrt3 = tfel::math::Cste<real>::sqrt3;
      constexpr const auto isqrt3 = tfel::math::Cste<real>::isqrt3;
      const auto s = tfel::math::deviator(sig);
      const auto det_s = tfel::math::det(s);
      const auto I1 = tfel::math::trace(sig);
      const auto J2 = std::max((s | s) / 2, local_zero_tolerance);
      const auto J3 = real(det_s < 0 ? std::min(det_s, -local_zero_tolerance)
                                     : std::max(det_s, local_zero_tolerance));
      const auto arg =
          std::min(std::max(-3 * sqrt3 * J3 / (2 * J2 * std::sqrt(J2)),
                            -1 + local_zero_tolerance),
                   1 - local_zero_tolerance);
      const auto lode = std::asin(arg) / 3;
      const auto cos_lode = std::cos(lode);
      const auto sin_lode = std::sin(lode);
      const auto sin_3_lode = arg;
      const auto K = [&]() {
        if (std::abs(lode) < p.lodeT) {
          return cos_lode - isqrt3 * p.sin_phi * sin_lode;
        }
        const auto sign = std::min(
            std::max(lode / std::max(std::abs(lode), local_zero_tolerance), -real(1)), real(1));
        const auto A =
            (p.cos_lodeT / 3) * (3 + p.tan_lodeT * p.tan_3_lodeT +
                                 isqrt3 * sign * (p.tan_3_lodeT - 3 * p.tan_lodeT) * p.sin_phi);
        const auto B =
            1 / (3 * p.cos_3_lodeT) * (sign * p.sin_lodeT + isqrt3 * p.sin_phi * p.cos_lodeT);
        return A - B * sin_3_lode;
      }();
      const auto rootF =
          std::max(std::sqrt(J2 * K * K + p.a * p.a * p.sin_phi * p.sin_phi),
                   local_zero_tolerance);
      const auto Fy1 = I1 * p.sin_phi / 3 + rootF;
      return Fy1 - p.c * p.cos_phi;
    }  // end of computeMohrCoulombStressCriterion

    template <typename StressStensor>
    std::tuple<MohrCoulombStressType<StressStensor>, MohrCoulombStressNormalType<StressStensor>>
    computeMohrCoulombStressCriterionNormal(const MohrCoulombParameters<StressStensor>& p,
                                            const StressStensor& sig) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using real = MohrCoulombBaseType<StressStensor>;
      using Stensor = tfel::math::stensor<N,real>;
      constexpr const auto local_zero_tolerance = real(1e-14);
      constexpr auto sqrt3 = tfel::math::Cste<real>::sqrt3;
      constexpr auto isqrt3 = tfel::math::Cste<real>::isqrt3;
      constexpr auto id = Stensor::Id();
      const auto s = tfel::math::deviator(sig);
      const auto det_s = tfel::math::det(s);
      const auto I1 = tfel::math::trace(sig);
      const auto J2 = std::max((s | s) / 2, local_zero_tolerance);
      const auto J3 = real(det_s < 0 ? std::min(det_s, -local_zero_tolerance)
                                     : std::max(det_s, local_zero_tolerance));
      const auto arg =
          std::min(std::max(-3 * sqrt3 * J3 / (2 * J2 * std::sqrt(J2)),
                            -1 + local_zero_tolerance),
                   1 - local_zero_tolerance);
      const auto lode = std::asin(arg) / 3;
      const auto cos_lode = std::cos(lode);
      const auto sin_lode = std::sin(lode);
      const auto cos_3_lode = std::cos(3 * lode);
      const auto sin_3_lode = arg;
      const auto tan_3_lode = std::tan(3 * lode);
      auto K = real{0};
      auto dK_dlode = real{1};
      std::tie(K, dK_dlode) = [&]() -> std::pair<real, real> {
        if (std::abs(lode) < p.lodeT) {
          return {cos_lode - isqrt3 * p.sin_phi * sin_lode,
                  -sin_lode - isqrt3 * p.sin_phi * cos_lode};
        }
        const auto sign = std::min(
            std::max(lode / std::max(std::abs(lode), local_zero_tolerance), -real(1)), real(1));
        const auto A =
            (p.cos_lodeT / 3) * (3 + p.tan_lodeT * p.tan_3_lodeT +
                                 isqrt3 * sign * (p.tan_3_lodeT - 3 * p.tan_lodeT) * p.sin_phi);
        const auto B =
            1 / (3 * p.cos_3_lodeT) * (sign * p.sin_lodeT + isqrt3 * p.sin_phi * p.cos_lodeT);
        return {A - B * sin_3_lode, -3 * B * cos_3_lode};
      }();
      // flow direction
      const auto dev_s_squared = tfel::math::computeDeviatorDeterminantDerivative(sig);
      const auto dG_dI1 = p.sin_phi / 3;
      const auto root =
          std::max(std::sqrt(J2 * K * K + p.a * p.a * p.sin_phi * p.sin_phi),
                   local_zero_tolerance);
      const auto dG_dJ2 = K / (2 * root) * (K - tan_3_lode * dK_dlode);
      const auto dG_dJ3 = J2 * K * tan_3_lode / (3 * J3 * root) * dK_dlode;
      const auto n = tfel::math::eval(dG_dI1 * id + dG_dJ2 * s + dG_dJ3 * dev_s_squared);
      const auto Fy1 = I1 * p.sin_phi / 3 + root;
      return std::make_tuple(Fy1 - p.c * p.cos_phi, n);
    }  // end of computeMohrCoulombStressCriterionNormal

    template <typename StressStensor>
    std::tuple<MohrCoulombStressType<StressStensor>,
               MohrCoulombStressNormalType<StressStensor>,
               MohrCoulombStressSecondDerivativeType<StressStensor>>
    computeMohrCoulombStressCriterionSecondDerivative(
        const MohrCoulombParameters<StressStensor>& p,
        const StressStensor& sig) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using real = MohrCoulombBaseType<StressStensor>;
      using Stensor = tfel::math::stensor<N,real>;
      using Stensor4 = tfel::math::st2tost2<N,real>;
      constexpr const auto local_zero_tolerance = real(1e-14);
      constexpr auto sqrt3 = tfel::math::Cste<real>::sqrt3;
      constexpr auto isqrt3 = tfel::math::Cste<real>::isqrt3;
      constexpr auto id = Stensor::Id();
      constexpr auto id4 = Stensor4::Id();
      const auto s = tfel::math::deviator(sig);
      const auto det_s = tfel::math::det(s);
      const auto I1 = tfel::math::trace(sig);
      const auto J2 = std::max((s | s) / 2, local_zero_tolerance);
      const auto J3 = real(det_s < 0 ? std::min(det_s, -local_zero_tolerance)
                                     : std::max(det_s, local_zero_tolerance));
      const auto arg =
          std::min(std::max(-3 * sqrt3 * J3 / (2 * J2 * std::sqrt(J2)),
                            -1 + local_zero_tolerance),
                   1 - local_zero_tolerance);
      const auto lode = std::asin(arg) / 3;
      const auto cos_lode = std::cos(lode);
      const auto sin_lode = std::sin(lode);
      const auto cos_3_lode = std::cos(3 * lode);
      const auto sin_3_lode = arg;
      const auto tan_3_lode = std::tan(3 * lode);
      auto K = real{};
      auto dK_dlode = real{};
      auto d2K_d2lode = real{};
      std::tie(K, dK_dlode, d2K_d2lode) = [&]() -> std::tuple<real, real, real> {
        if (std::abs(lode) < p.lodeT) {
          return {cos_lode - isqrt3 * p.sin_phi * sin_lode,
                  -sin_lode - isqrt3 * p.sin_phi * cos_lode,
                  -cos_lode + isqrt3 * p.sin_phi * sin_lode};
        }
        const auto sign = std::min(
            std::max(lode / std::max(std::abs(lode), local_zero_tolerance), -real(1)), real(1));
        const auto A =
            (p.cos_lodeT / 3) * (3 + p.tan_lodeT * p.tan_3_lodeT +
                                 isqrt3 * sign * (p.tan_3_lodeT - 3 * p.tan_lodeT) * p.sin_phi);
        const auto B =
            1 / (3 * p.cos_3_lodeT) * (sign * p.sin_lodeT + isqrt3 * p.sin_phi * p.cos_lodeT);
        return {A - B * sin_3_lode, -3 * B * cos_3_lode, 9. * B * sin_3_lode};
      }();
      // flow direction
      const auto dJ3 = tfel::math::computeDeviatorDeterminantDerivative(sig);
      const auto d2J3 = tfel::math::computeDeviatorDeterminantSecondDerivative(sig);
      const auto dG_dI1 = p.sin_phi / 3;
      const auto root =
          std::max(std::sqrt(J2 * K * K + p.a * p.a * p.sin_phi * p.sin_phi),
                   local_zero_tolerance);
      const auto dG_dJ2 = K / (2 * root) * (K - tan_3_lode * dK_dlode);
      const auto dG_dJ3 = J2 * K * tan_3_lode / (3 * J3 * root) * dK_dlode;
      const auto Pdev = id4 - (id ^ id) / 3;
      const auto dG_dlode = K * J2 / (root)*dK_dlode;
      const auto d2G_d2lode =
          J2 / root *
          (dK_dlode * dK_dlode * (1. - J2 * K * K / (root * root)) +
           K * d2K_d2lode);
      const auto d2G_dlodedJ2 =
          K / root * dK_dlode * (1. - J2 * K * K / (2 * root * root));
      const auto dG_d2J2 =
          -K * K * K * K / (4. * root * root * root) +
          dG_dlode * tan_3_lode / (2 * J2 * J2) -
          tan_3_lode / (2 * J2) *
              (2 * d2G_dlodedJ2 - tan_3_lode / (2 * J2) * d2G_d2lode -
               3 / (2 * J2 * cos_3_lode * cos_3_lode) * dG_dlode);
      const auto dG_d2J3 = -tan_3_lode / (3 * J3 * J3) * dG_dlode +
                           tan_3_lode / (3 * J3) *
                               (d2G_d2lode * tan_3_lode / (3 * J3) +
                                dG_dlode * 1. / (J3 * cos_3_lode * cos_3_lode));
      const auto dG_dJ2dJ3 =
          d2G_dlodedJ2 * tan_3_lode / (3 * J3) -
          tan_3_lode / (2 * J2) *
              (d2G_d2lode * tan_3_lode / (3 * J3) +
               dG_dlode * 1. / (J3 * cos_3_lode * cos_3_lode));
      const auto Fy1 = I1 * p.sin_phi / 3 + root;
      const auto n =
          tfel::math::eval(dG_dI1 * id + dG_dJ2 * s + dG_dJ3 * dJ3);
      const auto dn = tfel::math::eval(
          dG_dJ2 * Pdev + dG_dJ3 * d2J3 + dG_d2J2 * (s ^ s) +
          dG_d2J3 * (dJ3 ^ dJ3) + dG_dJ2dJ3 * ((dJ3 ^ s) + (s ^ dJ3)));
      return std::make_tuple(Fy1 - p.c * p.cos_phi, n, dn);
    }  // end of computeMohrCoulombStressCriterionSecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_IXX */
