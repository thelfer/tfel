/*!
 * \file   include/TFEL/Material/IsotropicEshelbyTensor.ixx
 * \author Antoine Martin
 * \date   15 October 2024
 * \brief  This file defines the Eshelby tensor for an ellipsoidal inclusion
 * embedded in an isotropic matrix. \copyright Copyright (C) 2006-2018 CEA/DEN,
 * EDF R&D. All rights reserved. This project is publicly released under either
 * the GNU GPL Licence or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_IXX
#define LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_IXX

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <typeinfo>
#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel::material::homogenization::elasticity {

  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<2u, real>
  computeCircularCylinderEshelbyTensor(const real& nu) {
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    using namespace tfel::math;
    const auto zero = real{0};
    const auto Q = 3 / (1 - nu) / 8;
    const auto R = (1 - 2 * nu) / 8 / (1 - nu);

    const auto S11 = Q + 2 * R;
    const auto S12 = Q / 3 - 2 * R;
    const auto S44 = 2 * (Q / 3 + 2 * R);
    const auto S13 = 2 * Q / 3 - 2 * R;
    return {S11,  S12,  S13,  zero, S12,  S11,  S13,  zero,
            zero, zero, zero, zero, zero, zero, zero, S44};
  }  // end of function computeCircularCylinderEshelbyTensor

  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<2u, real>
  computeEllipticCylinderEshelbyTensor(const real& nu, const real& e) {
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    if (not(e > real(0))) {
      tfel::reportContractViolation("e<=0");
    }
    const auto zero = real{0};
    const auto e2 = e * e;
    const auto Q = 3 / (1 - nu) / 8;
    const auto R = (1 - 2 * nu) / 8 / (1 - nu);

    const auto Ia = 4 / (1 + e);
    const auto Ib = 4 * e / (1 + e);
    const auto a2Iab = 4 * e2 / (e + 1) / (e + 1) / 3;
    const auto a2Iaa = real{4} / 3 - a2Iab;
    const auto b2Iab = 4 / (e + 1) / (e + 1) / 3;
    const auto b2Ibb = real{4} / 3 - b2Iab;

    const auto S11 = Q * a2Iaa + R * Ia;
    const auto S12 = Q * b2Iab - R * Ia;
    const auto S44 = 2 * (Q / 2 * (a2Iab + b2Iab) + R / 2 * (Ia + Ib));
    const auto S22 = Q * b2Ibb + R * Ib;
    const auto S21 = Q * a2Iab - R * Ib;
    const auto S13 = Q * Ia / 3 - R * Ia;
    const auto S23 = Q * Ib / 3 - R * Ib;
    if (e > real(1)) {
      return {S11,  S12,  S13,  zero, S21,  S22,  S23,  zero,
              zero, zero, zero, zero, zero, zero, zero, S44};
    }
    return {S22,  S21,  S23,  zero, S12,  S11,  S13,  zero,
            zero, zero, zero, zero, zero, zero, zero, S44};
  }  // end of function computeEllipticCylinderEshelbyTensor

  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real> computeSphereEshelbyTensor(
      const real& nu) {
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    const auto a = (1 + nu) / 3 / (1 - nu);
    const auto b = 2 * (4 - 5 * nu) / 15 / (1 - nu);
    using namespace tfel::math;
    return a * st2tost2<3u, real>::J() + b * st2tost2<3u, real>::K();
  }  // end of function computeSphereEshelbyTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::st2tost2<
          3u,
          types::compliance<
              StressType>> computeSphereHillPolarisationTensor(const StressType&
                                                                   young,
                                                               const types::real<
                                                                   StressType>&
                                                                   nu) {
    using real = types::real<StressType>;
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    if (not(young > StressType{0})) {
      tfel::reportContractViolation("E<=0");
    }
    const auto a = (1 + nu) * (1 - 2 * nu) / 3 / young / (1 - nu);
    const auto b = 2 * (4 - 5 * nu) * (1 + nu) / 15 / young / (1 - nu);
    using namespace tfel::math;
    return a * st2tost2<3u, real>::J() + b * st2tost2<3u, real>::K();
  }  // end of function computeSphereHillPolarisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeSphereHillPolarisationTensor(
          const IsotropicModuli<StressType>& IM0) {
    const auto Enu0 = IM0.ToYoungNu();
    return computeSphereHillPolarisationTensor<StressType>(std::get<0>(Enu0),
                                                           std::get<1>(Enu0));
  }  // end of function computeSphereHillPolarisationTensor

  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real>
  computeAxisymmetricalEshelbyTensor(const real& nu,
                                     const real& e,
                                     const real precf,
                                     const real precd,
                                     const real precld) {
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    if (not(e > real(0))) {
      tfel::reportContractViolation("e<=0");
    }

    const auto precision = [precf, precd, precld]() {
      if constexpr (std::same_as<tfel::math::base_type<real>, long double>){
        return precld;
      } else if constexpr (std::same_as<tfel::math::base_type<real>, double>){
        return precd;
      } else {
        return precf;
      }
    }();
    
    if (std::abs(e - 1) < precision) {
      return computeSphereEshelbyTensor<real>(nu);
    }
    const auto zero = real{0};
    const auto e2 = e * e;
    const auto e21 = e2 - 1;
    const auto u2nu = 1 - 2 * nu;
    const auto q = [e]() {
      if (e < 1) {
        return e / (1 - e * e) / std::sqrt(1 - e * e) *
               (std::acos(e) - e * std::sqrt(1 - e * e));
      } else if (e > 1) {
        return e / (e * e - 1) / std::sqrt(e * e - 1) *
               (e * std::sqrt(e * e - 1) - std::acosh(e));
      }
      return real{2} / 3;
    }();
    const auto S11 =
        3 / (8 * (1 - nu)) * e2 / e21 + q / 4 / (1 - nu) * (u2nu - 9 / e21 / 4);
    const auto S12 =
        1 / (1 - nu) / 4 * (e2 / 2 / e21 - q * (u2nu + 3 / e21 / 4));
    const auto S13 =
        1 / (1 - nu) / 2 * (-e2 / e21 + q / 2 * (3 * e2 / e21 - u2nu));
    const auto S31 =
        1 / (1 - nu) / 2 * (-u2nu - 1 / e21 + q * (u2nu + 3 / e21 / 2));
    const auto S33 = 1 / (1 - nu) / 2 *
                     (u2nu + (3 * e2 - 1) / e21 - q * (u2nu + 3 * e2 / e21));
    const auto S44 =
        2 / (1 - nu) / 4 * (e2 / 2 / e21 + q * (u2nu - 3 / e21 / 4));
    const auto S55 =
        2 / (1 - nu) / 4 *
        (u2nu - (e2 + 1) / e21 - q / 2 * (u2nu - 3 * (e2 + 1) / e21));
    const tfel::math::st2tost2<3u, real> S = {
        S11,  S12,  S13,  zero, zero, zero, S12,  S11,  S13,  zero, zero, zero,
        S31,  S31,  S33,  zero, zero, zero, zero, zero, zero, S44,  zero, zero,
        zero, zero, zero, zero, S55,  zero, zero, zero, zero, zero, zero, S55};
    if (e > 1) {
      using namespace tfel::math;
      const tvector<3u, real> n_1 = {0., 0., 1.};
      const tvector<3u, real> n_2 = {0., -1., 0.};
      const tvector<3u, real> n_3 = {1., 0., 0.};
      const rotation_matrix<real> r = {n_1[0], n_1[1], n_1[2], n_2[0], n_2[1],
                                       n_2[2], n_3[0], n_3[1], n_3[2]};
      return change_basis(S, r);
    }
    return S;
  }  // end of function computeAxisymmetricalEshelbyTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeAxisymmetricalHillPolarisationTensor(
          const StressType& young,
          const types::real<StressType>& nu,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::real<StressType>& e,
          const types::real<StressType> precf,
          const types::real<StressType> precd,
          const types::real<StressType> precld) {
    using real = types::real<StressType>;
    if (not(young > StressType{0})) {
      tfel::reportContractViolation("E<=0");
    }
    if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
      tfel::reportContractViolation("n_a is null");
    }
    const auto n_a_ = n_a / norm(n_a);
    tfel::math::tvector<3u, real> n_;
    if ((tfel::math::ieee754::fpclassify(n_a[1]) != FP_ZERO) ||
        (tfel::math::ieee754::fpclassify(n_a[2]) != FP_ZERO)) {
      n_ = {1., 0., 0.};
    } else {
      n_ = {0., 1., 0.};
    }
    auto n_b_ = cross_product<real>(n_a_, n_);
    n_b_ = n_b_ / norm(n_b_);
    const auto n_c_ = cross_product<real>(n_a_, n_b_);
    tfel::math::tvector<3u, real> n_1;
    tfel::math::tvector<3u, real> n_2;
    if (e < 1) {
      n_1 = n_b_;
      n_2 = n_c_;
    } else {
      n_1 = n_a_;
      n_2 = n_b_;
    }
    using namespace tfel::math;
    const auto n_3 = cross_product<real>(n_1, n_2);
    const auto S0 = computeAxisymmetricalEshelbyTensor<real>(nu, e, precf, precd, precld);
    const tfel::math::rotation_matrix<real> r = {
        n_1[0], n_1[1], n_1[2], n_2[0], n_2[1], n_2[2], n_3[0], n_3[1], n_3[2]};
    const auto S0_basis = change_basis(S0, r);
    tfel::math::st2tost2<3u, StressType> C_0;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);

    const auto invC0 = tfel::math::invert(C_0);
    return S0_basis * invC0;
  }  // end of function computeAxisymmetricalHillPolarisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeAxisymmetricalHillPolarisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::real<StressType>& e,
          const types::real<StressType> precf,
          const types::real<StressType> precd,
          const types::real<StressType> precld) {
    const auto Enu0 = IM0.ToYoungNu();
    return computeAxisymmetricalHillPolarisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), n_a, e, precf, precd, precld);
  }  // end of function computeAxisymmetricalHillPolarisationTensor

  namespace internals {
    /*!
     * This function takes a,b,c and returns the indices of the lengths (a,b,c)
     * sorted from the biggest to the smallest \return an object of type
     * std::array<int,3> \tparam LengthType: type of the lengths \param[in] nu:
     * Poisson's ratio of the matrix
     */
    template <typename LengthType>
    TFEL_HOST_DEVICE std::array<int, 3> sortEllipsoidLengths(
        const LengthType& a, const LengthType& b, const LengthType& c) {
      if ((a > b) and (a > c)) {
        if (b > c)
          return {0, 1, 2};
        else
          return {0, 2, 1};
      } else if ((b > a) and (b > c)) {
        if (a > c)
          return {1, 0, 2};
        else
          return {1, 2, 0};
      } else {
        if (a > b)
          return {2, 0, 1};
        else
          return {2, 1, 0};
      }
    }  // end of sortEllipsoidLengths
  }    // end of namespace internals

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      tfel::math::st2tost2<3u, types::real<StressType>> computeEshelbyTensor(
          const types::real<StressType>& nu,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const types::real<StressType> precf,
          const types::real<StressType> precd,
          const types::real<StressType> precld) {
    using real = types::real<StressType>;
    using LengthType = types::length<StressType>;
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    if (not((a > LengthType{0}) and (b > LengthType{0}) and
            (c > LengthType{0}))) {
      tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
    }

    const auto precision = [precf, precd, precld]() {
      if constexpr (std::same_as<tfel::math::base_type<real>, long double>){
        return precld;
      } else if constexpr (std::same_as<tfel::math::base_type<real>, double>){
        return precd;
      } else {
        return precf;
      }
    }();
    if (std::abs((b - a) / c) < precision ||
        std::abs((a - c) / b) < precision ||
        std::abs((b - c) / a) < precision) {
      if (std::abs((b - a) / c) < precision) {
        return computeAxisymmetricalEshelbyTensor<real>(nu, c / b);
      } else if (std::abs((a - c) / b) < precision) {
        return computeAxisymmetricalEshelbyTensor<real>(nu, b / a);
      } else if (std::abs((b - c) / a) < precision) {
        return computeAxisymmetricalEshelbyTensor<real>(nu, a / b);
      }
    }
    const std::array<LengthType, 3> abc_ = {a, b, c};
    const auto sig = internals::sortEllipsoidLengths<LengthType>(a, b, c);
    const auto a_ = abc_[sig[0]];
    const auto b_ = abc_[sig[1]];
    const auto c_ = abc_[sig[2]];
    constexpr real pi = std::numbers::pi_v<tfel::math::base_type<real>>;
    const auto a2 = a_ * a_;
    const auto b2 = b_ * b_;
    const auto c2 = c_ * c_;
    const auto Q = 3 / pi / 8 / (1 - nu);
    const auto R = (1 - 2 * nu) / 8 / pi / (1 - nu);
    const auto k = std::sqrt((a2 - b2) / (a2 - c2));
    const auto theta = std::asin(std::sqrt(1 - c2 / a2));
    const auto F = std::ellint_1(k, theta);
    const auto E = std::ellint_2(k, theta);

    const auto Ia = 4 * pi * a_ * b_ * c_ / (a2 - b2) / a_ /
                    std::sqrt(1 - c2 / a2) * (F - E);
    const auto Ic = 4 * pi * a_ * b_ * c_ / (b2 - c2) / a_ /
                    std::sqrt(1 - c2 / a2) *
                    (b_ * a_ * std::sqrt(1 - c2 / a2) / a_ / c_ - E);
    const auto Ib = 4 * pi - Ia - Ic;
    const auto Iab = (Ib - Ia) / 3 / (a2 - b2);
    const auto Iac = (4 * pi / 3 - Ia - (a2 - b2) * Iab) / (a2 - c2);
    const auto Ibc = (4 * pi / 3 - Ic - (c2 - a2) * Iac) / (c2 - b2);
    const auto Iaa = 4 * pi / 3 / a2 - Iab - Iac;
    const auto Ibb = 4 * pi / 3 / b2 - Iab - Ibc;
    const auto Icc = 4 * pi / 3 / c2 - Ibc - Iac;

    const auto S11 = Q * a2 * Iaa + R * Ia;
    const auto S12 = Q * b2 * Iab - R * Ia;
    const auto S44 = 2 * (Q / 2 * (a2 + b2) * Iab + R / 2 * (Ia + Ib));
    const auto S22 = Q * b2 * Ibb + R * Ib;
    const auto S21 = Q * a2 * Iab - R * Ib;
    const auto S13 = Q * c2 * Iac - R * Ia;
    const auto S31 = Q * a2 * Iac - R * Ic;
    const auto S33 = Q * c2 * Icc + R * Ic;
    const auto S23 = Q * c2 * Ibc - R * Ib;
    const auto S32 = Q * b2 * Ibc - R * Ic;
    const auto S55 = 2 * (Q / 2 * (a2 + c2) * Iac + R / 2 * (Ia + Ic));
    const auto S66 = 2 * (Q / 2 * (b2 + c2) * Ibc + R / 2 * (Ib + Ic));

    const auto zero = real{0};

    return {S11,  S12,  S13,  zero, zero, zero, S21,  S22,  S23,
            zero, zero, zero, S31,  S32,  S33,  zero, zero, zero,
            zero, zero, zero, S44,  zero, zero, zero, zero, zero,
            zero, S55,  zero, zero, zero, zero, zero, zero, S66};
  }  // end of function computeEshelbyTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeHillPolarisationTensor(
          const StressType& young,
          const types::real<StressType>& nu,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const types::real<StressType> precf,
          const types::real<StressType> precd,
          const types::real<StressType> precld) {
    using real = types::real<StressType>;
    using LengthType = types::length<StressType>;
    if (not(young > StressType{0})) {
      tfel::reportContractViolation("E<=0");
    }
    if (not(tfel::math::ieee754::fpclassify(
                tfel::math::VectorVectorDotProduct::exe<
                    real, tfel::math::tvector<3u, real>,
                    tfel::math::tvector<3u, real>>(n_a, n_b)) == FP_ZERO)) {
      tfel::reportContractViolation("n_a and n_b not normals");
    }
    if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
      tfel::reportContractViolation("n_a is null");
    }
    if (tfel::math::ieee754::fpclassify(norm(n_b)) == FP_ZERO) {
      tfel::reportContractViolation("n_b is null");
    }
    const auto n_a_ = n_a / norm(n_a);
    const auto n_b_ = n_b / norm(n_b);
    const auto n_c_ = tfel::math::cross_product<real>(n_a_, n_b_);
    const std::array<LengthType, 3> abc_ = {a, b, c};
    const auto sig = internals::sortEllipsoidLengths<LengthType>(a, b, c);
    const auto S0 = computeEshelbyTensor<StressType>(
        nu, abc_[sig[0]], abc_[sig[1]], abc_[sig[2]],precf,precd,precld);
    const std::array<tfel::math::tvector<3u, real>, 3> nabc_ = {n_a_, n_b_,
                                                                n_c_};
    const auto n_1 = nabc_[sig[0]];
    const auto n_2 = nabc_[sig[1]];
    using namespace tfel::math;
    const auto n_3 = cross_product<real>(n_1, n_2);
    //r is the global basis expressed in the local sorted basis (n1,n2,n3)
    const rotation_matrix<real> r = {n_1[0], n_1[1], n_1[2], n_2[0], n_2[1],
                                     n_2[2], n_3[0], n_3[1], n_3[2]};
    const auto S0_basis = change_basis(S0, r);
    tfel::math::st2tost2<3u, StressType> C_0;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);
    const auto invC0 = tfel::math::invert(C_0);
    return S0_basis * invC0;

  }  // end of function computeHillPolarisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeHillPolarisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const types::real<StressType> precf,
          const types::real<StressType> precd,
          const types::real<StressType> precld) {
    const auto Enu0 = IM0.ToYoungNu();
    return computeHillPolarisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), n_a, a, n_b, b, c, precf, precd,
        precld);
  }  // end of function computeHillPolarisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::
          st2tost2<3u, types::real<StressType>> computeSphereLocalisationTensor(
              const StressType& young,
              const types::real<StressType>& nu,
              const StressType& young_i,
              const types::real<StressType>& nu_i) {
    using real = types::real<StressType>;
    if (not(young > StressType{0})) {
      tfel::reportContractViolation("E<=0");
    }
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    const auto kaS = (1 + nu) / 9 / (1 - nu);
    const auto muS = 2 * (4 - 5 * nu) / 30 / (1 - nu);
    const auto k0 = young / 3 / (1 - 2 * nu);
    const auto mu0 = young / 2 / (1 + nu);
    const auto k_i = young_i / 3 / (1 - 2 * nu_i);
    const auto mu_i = young_i / 2 / (1 + nu_i);
    const auto mu = 1 / (2 + 4 * muS * (mu_i - mu0) / mu0);
    const auto ka = 1 / (3 + 9 * kaS * (k_i - k0) / k0);
    using namespace tfel::math;
    return 3 * ka * st2tost2<3u, real>::J() + 2 * mu * st2tost2<3u, real>::K();
  }

  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, types::real<StressType>>
  computeSphereLocalisationTensor(const IsotropicModuli<StressType>& IM0,
                                  const IsotropicModuli<StressType>& IM_i) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeSphereLocalisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), std::get<0>(Enui),
        std::get<1>(Enui));
  }  // end of function SphereLocalisationTensor


  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAxisymmetricalEllipsoidLocalisationTensor(
          const StressType& young,
          const types::real<StressType>& nu,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::real<StressType>& e) {
    using real = types::real<StressType>;
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    if (not(e > 0)) {
      tfel::reportContractViolation("e<=0");
    }
    const auto P0 = computeAxisymmetricalHillPolarisationTensor<StressType>(
        young, nu, n_a, e);

    tfel::math::st2tost2<3u, StressType> C_0;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);
    tfel::math::st2tost2<3u, StressType> C_i;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_i, young_i,
                                                                   nu_i);
    using namespace tfel::math;
    const st2tost2<3u, StressType> C = C_i - C_0;
    const auto Pr = P0 * C;
    const auto A = invert(st2tost2<3u, real>::Id() + Pr);
    return A;
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAxisymmetricalEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::real<StressType>& e) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), std::get<0>(Enui),
        std::get<1>(Enui), n_a, e);
  }  // end of function computeAxisymmetricalEllipsoidLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const StressType& young,
          const types::real<StressType>& nu,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    using real = types::real<StressType>;
    using LengthType = types::length<StressType>;
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    if (not((a > LengthType{0}) and (b > LengthType{0}) and
            (c > LengthType{0}))) {
      tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
    }
    const auto P0 =
        computeHillPolarisationTensor<StressType>(young, nu, n_a, a, n_b, b, c);

    tfel::math::st2tost2<3u, StressType> C_0;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);
    tfel::math::st2tost2<3u, StressType> C_i;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_i, young_i,
                                                                   nu_i);
    using namespace tfel::math;
    const st2tost2<3u, StressType> C = C_i - C_0;
    const auto Pr = P0 * C;
    const auto A = invert(st2tost2<3u, real>::Id() + Pr);
    return A;
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeEllipsoidLocalisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), std::get<0>(Enui),
        std::get<1>(Enui), n_a, a, n_b, b, c);
  }  // end of function computeEllipsoidLocalisationTensor

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_IXX */
