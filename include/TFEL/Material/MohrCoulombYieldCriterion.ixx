/*!
 * \file   include/TFEL/Material/MohrCoulombYieldCriterion.ixx
 * \brief
 * \author Thomas Nagel, Thomas Helfer
 * \date   10/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_IXX
#define LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_IXX

#include <cmath>
#include "TFEL/Math/General/Abs.hxx"

namespace tfel::material {

  template <typename StressStensor>
  MohrCoulombParameters<StressStensor>::MohrCoulombParameters(
      const stress c_c,
      const real angle_c,
      const real lodeT_c,
      const stress a_c)
      : c(c_c),
        angle(angle_c),
        lodeT(lodeT_c),
        a(a_c),
        cos_angle(std::cos(angle_c)),
        sin_angle(std::sin(angle_c)),
        cos_lodeT(std::cos(lodeT_c)),
        sin_lodeT(std::sin(lodeT_c)),
        tan_lodeT(std::tan(lodeT_c)),
        cos_3_lodeT(std::cos(3 * lodeT_c)),
        sin_3_lodeT(std::sin(3 * lodeT_c)),
        cos_6_lodeT(std::cos(6 * lodeT_c)),
        sin_6_lodeT(std::sin(6 * lodeT_c)),
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
  MohrCoulombParameters<StressStensor>&
  MohrCoulombParameters<StressStensor>::operator=(MohrCoulombParameters&&) =
      default;

  template <typename StressStensor>
  MohrCoulombParameters<StressStensor>&
  MohrCoulombParameters<StressStensor>::operator=(
      const MohrCoulombParameters&) = default;

  template <typename StressStensor,
            typename MohrCoulombParameters<StressStensor>::AngleUnit u>
  MohrCoulombParameters<StressStensor> makeMohrCoulombParameters(
      const MohrCoulombStressType<StressStensor> c,
      const MohrCoulombBaseType<StressStensor> angle,
      const MohrCoulombBaseType<StressStensor> lodeT,
      const MohrCoulombStressType<StressStensor> a) {
    using real = MohrCoulombBaseType<StressStensor>;
    constexpr real pi = real(3.14159265358979323846);
    if (u == MohrCoulombParameters<StressStensor>::DEGREE) {
      const auto angle_c = angle * pi / 180;
      const auto lodeT_c = lodeT * pi / 180;
      return MohrCoulombParameters<StressStensor>{c, angle_c, lodeT_c, a};
    }
    return MohrCoulombParameters<StressStensor>{c, angle, lodeT, a};
  }  // end of makeMohrCoulombParameters

  template <typename StressStensor>
  MohrCoulombStressType<StressStensor> computeMohrCoulombStressCriterion(
      const MohrCoulombParameters<StressStensor>& p, const StressStensor& sig) {
    using real = MohrCoulombBaseType<StressStensor>;
    constexpr auto local_zero_tolerance = real(1e-14);
    constexpr auto sqrt3 = tfel::math::Cste<real>::sqrt3;
    constexpr auto isqrt3 = tfel::math::Cste<real>::isqrt3;
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
      if (tfel::math::abs(lode) < p.lodeT) {
        return cos_lode - isqrt3 * p.sin_angle * sin_lode;
      }
      const auto sign = std::min(
          std::max(lode / std::max(tfel::math::abs(lode), local_zero_tolerance),
                   -real(1)),
          real(1));
      const auto term1 = p.cos_lodeT - isqrt3 * p.sin_angle * p.sin_lodeT;
      const auto term2 =
          sign * p.sin_lodeT + isqrt3 * p.sin_angle * p.cos_lodeT;
      const auto term3 = 18. * p.cos_3_lodeT * p.cos_3_lodeT * p.cos_3_lodeT;

      const auto B =
          (sign * p.sin_6_lodeT * term1 - 6. * p.cos_6_lodeT * term2) / term3;
      const auto C =
          (-p.cos_3_lodeT * term1 - 3. * sign * p.sin_3_lodeT * term2) / term3;

      const auto A = -isqrt3 * p.sin_angle * sign * p.sin_lodeT -
                     B * sign * p.sin_3_lodeT -
                     C * p.sin_3_lodeT * p.sin_3_lodeT + p.cos_lodeT;
      return A + B * sin_3_lode + C * sin_3_lode * sin_3_lode;
    }();
    const auto rootF =
        std::max(std::sqrt(J2 * K * K + p.a * p.a * p.sin_angle * p.sin_angle),
                 local_zero_tolerance);
    const auto Fy1 = I1 * p.sin_angle / 3 + rootF;
    return Fy1 - p.c * p.cos_angle;
  }  // end of computeMohrCoulombStressCriterion

  template <typename StressStensor>
  std::tuple<MohrCoulombStressType<StressStensor>,
             MohrCoulombStressNormalType<StressStensor>>
  computeMohrCoulombStressCriterionNormal(
      const MohrCoulombParameters<StressStensor>& p, const StressStensor& sig) {
    constexpr auto N = tfel::math::getSpaceDimension<StressStensor>();
    using real = MohrCoulombBaseType<StressStensor>;
    using Stensor = tfel::math::stensor<N, real>;
    constexpr auto local_zero_tolerance = real(1e-14);
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
    const auto sin_6_lode = std::sin(6 * lode);
    const auto tan_3_lode = std::tan(3 * lode);
    auto K = real{0};
    auto dK_dlode = real{1};
    std::tie(K, dK_dlode) = [&]() -> std::pair<real, real> {
      if (tfel::math::abs(lode) < p.lodeT) {
        return {cos_lode - isqrt3 * p.sin_angle * sin_lode,
                -sin_lode - isqrt3 * p.sin_angle * cos_lode};
      }
      const auto sign = std::min(
          std::max(lode / std::max(tfel::math::abs(lode), local_zero_tolerance),
                   -real(1)),
          real(1));
      const auto term1 = p.cos_lodeT - isqrt3 * p.sin_angle * p.sin_lodeT;
      const auto term2 =
          sign * p.sin_lodeT + isqrt3 * p.sin_angle * p.cos_lodeT;
      const auto term3 = 18. * p.cos_3_lodeT * p.cos_3_lodeT * p.cos_3_lodeT;

      const auto B =
          (sign * p.sin_6_lodeT * term1 - 6. * p.cos_6_lodeT * term2) / term3;
      const auto C =
          (-p.cos_3_lodeT * term1 - 3. * sign * p.sin_3_lodeT * term2) / term3;

      const auto A = -isqrt3 * p.sin_angle * sign * p.sin_lodeT -
                     B * sign * p.sin_3_lodeT -
                     C * p.sin_3_lodeT * p.sin_3_lodeT + p.cos_lodeT;
      return {A + B * sin_3_lode + C * sin_3_lode * sin_3_lode,
              3. * B * cos_3_lode + 3. * C * sin_6_lode};
    }();
    // flow direction
    const auto dev_s_squared =
        tfel::math::computeDeviatorDeterminantDerivative(sig);
    const auto dG_dI1 = p.sin_angle / 3;
    const auto root =
        std::max(std::sqrt(J2 * K * K + p.a * p.a * p.sin_angle * p.sin_angle),
                 local_zero_tolerance);
    const auto dG_dJ2 = K / (2 * root) * (K - tan_3_lode * dK_dlode);
    const auto dG_dJ3 = J2 * K * tan_3_lode / (3 * J3 * root) * dK_dlode;
    const auto n =
        tfel::math::eval(dG_dI1 * id + dG_dJ2 * s + dG_dJ3 * dev_s_squared);
    const auto Fy1 = I1 * p.sin_angle / 3 + root;
    return std::make_tuple(Fy1 - p.c * p.cos_angle, n);
  }  // end of computeMohrCoulombStressCriterionNormal

  template <typename StressStensor>
  std::tuple<MohrCoulombStressType<StressStensor>,
             MohrCoulombStressNormalType<StressStensor>,
             MohrCoulombStressSecondDerivativeType<StressStensor>>
  computeMohrCoulombStressCriterionSecondDerivative(
      const MohrCoulombParameters<StressStensor>& p, const StressStensor& sig) {
    constexpr auto N = tfel::math::getSpaceDimension<StressStensor>();
    using real = MohrCoulombBaseType<StressStensor>;
    using Stensor = tfel::math::stensor<N, real>;
    using Stensor4 = tfel::math::st2tost2<N, real>;
    constexpr auto local_zero_tolerance = real(1e-14);
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
    const auto sin_6_lode = std::sin(6 * lode);
    const auto cos_6_lode = std::cos(6 * lode);
    auto K = real{};
    auto dK_dlode = real{};
    auto d2K_d2lode = real{};
    std::tie(K, dK_dlode, d2K_d2lode) = [&]() -> std::tuple<real, real, real> {
      if (tfel::math::abs(lode) < p.lodeT) {
        return {cos_lode - isqrt3 * p.sin_angle * sin_lode,
                -sin_lode - isqrt3 * p.sin_angle * cos_lode,
                -cos_lode + isqrt3 * p.sin_angle * sin_lode};
      }
      const auto sign = std::min(
          std::max(lode / std::max(tfel::math::abs(lode), local_zero_tolerance),
                   -real(1)),
          real(1));
      const auto term1 = p.cos_lodeT - isqrt3 * p.sin_angle * p.sin_lodeT;
      const auto term2 =
          sign * p.sin_lodeT + isqrt3 * p.sin_angle * p.cos_lodeT;
      const auto term3 = 18. * p.cos_3_lodeT * p.cos_3_lodeT * p.cos_3_lodeT;

      const auto B =
          (sign * p.sin_6_lodeT * term1 - 6. * p.cos_6_lodeT * term2) / term3;
      const auto C =
          (-p.cos_3_lodeT * term1 - 3. * sign * p.sin_3_lodeT * term2) / term3;

      const auto A = -isqrt3 * p.sin_angle * sign * p.sin_lodeT -
                     B * sign * p.sin_3_lodeT -
                     C * p.sin_3_lodeT * p.sin_3_lodeT + p.cos_lodeT;
      return {A + B * sin_3_lode + C * sin_3_lode * sin_3_lode,
              3. * B * cos_3_lode + 3. * C * sin_6_lode,
              -9. * B * sin_3_lode + 18. * C * cos_6_lode};
    }();
    // flow direction
    const auto dJ3 = tfel::math::computeDeviatorDeterminantDerivative(sig);
    const auto d2J3 =
        tfel::math::computeDeviatorDeterminantSecondDerivative(sig);
    const auto dG_dI1 = p.sin_angle / 3;
    const auto root =
        std::max(std::sqrt(J2 * K * K + p.a * p.a * p.sin_angle * p.sin_angle),
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
    const auto dG_dJ2dJ3 = d2G_dlodedJ2 * tan_3_lode / (3 * J3) -
                           tan_3_lode / (2 * J2) *
                               (d2G_d2lode * tan_3_lode / (3 * J3) +
                                dG_dlode * 1. / (J3 * cos_3_lode * cos_3_lode));
    const auto Fy1 = I1 * p.sin_angle / 3 + root;
    const auto n = tfel::math::eval(dG_dI1 * id + dG_dJ2 * s + dG_dJ3 * dJ3);
    const auto dn = tfel::math::eval(dG_dJ2 * Pdev + dG_dJ3 * d2J3 +
                                     dG_d2J2 * (s ^ s) + dG_d2J3 * (dJ3 ^ dJ3) +
                                     dG_dJ2dJ3 * ((dJ3 ^ s) + (s ^ dJ3)));
    return std::make_tuple(Fy1 - p.c * p.cos_angle, n, dn);
  }  // end of computeMohrCoulombStressCriterionSecondDerivative

  template <typename StressStensor>
  std::ostream& operator<<(std::ostream& os,
                           const MohrCoulombParameters<StressStensor>& p) {
    os << "c (cohesion): " << p.c << '\n';
    os << "angle (friction angle or dilatancy angle): " << p.angle << '\n';
    os << "lodeT (transition angle as defined by Abbo and Sloan): " << p.lodeT
       << '\n';
    os << "a (tension cuff-off parameter): " << p.a << '\n';
    return os;
  }  // end of operator <<

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_IXX */
