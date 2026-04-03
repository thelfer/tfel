/*!
 * \file   include/TFEL/Material/AnisotropicEshelbyTensor.ixx
 * \author Antoine Martin
 * \date   18 November 2024
 * \brief  This file defines the Eshelby tensor for an ellipsoidal inclusion
 * embedded in an anisotropic matrix. \copyright Copyright (C) 2006-2018
 * CEA/DEN, EDF R&D. All rights reserved. This project is publicly released
 * under either the GNU GPL Licence with linking exception or the CECILL-A
 * licence. A copy of thoses licences are delivered with the sources of TFEL.
 * CEA or EDF may also distribute this project under specific licensing
 * conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ANISOTROPICESHELBYTENSOR_IXX
#define LIB_TFEL_MATERIAL_ANISOTROPICESHELBYTENSOR_IXX

#include <cmath>
#include <numbers>
#include <stdexcept>
#include "TFEL/Math/General/Abs.hxx"

namespace tfel::material::homogenization::elasticity {

  namespace internals {

    template <typename real>
    TFEL_HOST_DEVICE auto integrate1D(
        const std::function<real(const real&)> f,
        const real a,
        const real b,
        const std::size_t max_it = 12,
        const real tol = std::sqrt(std::numeric_limits<real>::epsilon())) {
      auto h = (b - a) / 2;
      auto I0 = (f(a) + f(b)) * h;
      auto IL0 = (std::abs(f(a)) + std::abs(f(b))) * h;
      auto I1 = I0 / 2 + f(a + h) * h;
      auto IL1 = IL0 / 2 + std::abs(f(a + h)) * h;

      std::size_t k = 2;
      std::size_t p = 2;
      real error = std::abs(I0 - I1);
      while (k < 4 || (k < max_it && error > tol * IL1)) {
        I0 = I1;
        IL0 = IL1;

        I1 = I0 / 2;
        IL1 = IL0 / 2;
        p *= 2;
        h /= 2;

        for (std::size_t j = 1; j < p; j += 2) {
          const auto x = a + static_cast<real>(j) * h;
          const auto v = f(x);
          I1 += v * h;
          IL1 += tfel::math::abs(v) * h;
        }

        ++k;
        error = tfel::math::abs(I0 - I1);
      }
      return I1;
    }

    template <typename real>
    TFEL_HOST_DEVICE auto dblintegrate(
        const std::function<real(const real&, const real&)> f,
        const real a,
        const real b,
        const real c,
        const real d,
        const std::size_t max_it = 12) {
      const auto f_ = [f, c, d, max_it](const real& x) {
        return integrate1D<real>([x, f](const real& y) { return f(x, y); }, c,
                                 d, max_it);
      };
      return integrate1D<real>(f_, a, b, max_it);
    }

    template <unsigned short int N, typename real, typename StressType>
    TFEL_HOST_DEVICE tfel::math::tmatrix<N, N, real> Acoustic(
        const tfel::math::st2tost2<N, StressType>& C,
        const tfel::math::tvector<N, real>& X) {
      tfel::math::tmatrix<N, N, real> A;
      for (int i = 0; i < N; i++)
        for (int k = i; k < N; k++) {
          real A_ik = real(0);
          for (int j = 0; j < N; j++)
            for (int l = 0; l < N; l++) {
              A_ik += real((tfel::math::getComponent(C, i, j, k, l)) /
                           StressType(1) * X[j] * X[l]);
            }
          A(i, k) = A_ik;
          if (i != k) {
            A(k, i) = A_ik;
          }
        }
      return A;
    }

    template <typename real, typename StressType, typename LengthType>
    TFEL_HOST_DEVICE real
    p_ijkl_2D(const tfel::math::st2tost2<2u, StressType>& C,
              const LengthType& a,
              const LengthType& b,
              const real theta,
              const int& i,
              const int& j,
              const int& k,
              const int& l) {
      const real pi = std::numbers::pi_v<tfel::math::base_type<real>>;
      const tfel::math::tvector<2u, real> X = {
          std::cos(theta) / a * LengthType(1),
          std::sin(theta) / b * LengthType(1)};
      auto A_inv = Acoustic<2u, real, StressType>(C, X);
      tfel::math::TinyMatrixInvert<2u, real>::exe(A_inv);
      const auto Mijkl =
          (A_inv(j, k) * X[i] * X[l] + A_inv(i, k) * X[j] * X[l] +
           A_inv(j, l) * X[i] * X[k] + A_inv(i, l) * X[j] * X[k]) /
          4;
      return Mijkl / 2 / pi;
    }

    template <typename real, typename StressType, typename LengthType>
    TFEL_HOST_DEVICE real p_ijkl(const tfel::math::st2tost2<3u, StressType>& C,
                                 const real theta,
                                 const real phi,
                                 const LengthType& a,
                                 const LengthType& b,
                                 const LengthType& c,
                                 const int& i,
                                 const int& j,
                                 const int& k,
                                 const int& l) {
      const real pi = std::numbers::pi_v<tfel::math::base_type<real>>;
      const tfel::math::tvector<3u, real> X = {
          std::sin(theta) * std::cos(phi) / a * LengthType(1),
          std::sin(theta) * std::sin(phi) / b * LengthType(1),
          std::cos(theta) / c * LengthType(1)};
      auto A_inv = Acoustic<3u, real, StressType>(C, X);
      tfel::math::TinyMatrixInvert<3u, real>::exe(A_inv);
      const auto Mijkl =
          (A_inv(j, k) * X[i] * X[l] + A_inv(i, k) * X[j] * X[l] +
           A_inv(j, l) * X[i] * X[k] + A_inv(i, l) * X[j] * X[k]) /
          4;
      return Mijkl * std::sin(theta) / 4 / pi;
    }

  }  // end of namespace internals

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::compliance<StressType>> computePlaneStrainAnisotropicHillTensor(
          const tfel::math::st2tost2<2u, StressType>& C,
          const tfel::math::tvector<2u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const std::size_t max_it) {
    using real = types::real<StressType>;
    using LengthType = types::length<StressType>;
    if (not((a > LengthType{0}) and (b > LengthType{0}))) {
      tfel::reportContractViolation("a<=0 or b<=0");
    }
    if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
      tfel::reportContractViolation("n_a is null");
    }
    using namespace tfel::math;
    const auto n_a_ = n_a / norm(n_a);
    tvector<2u, real> n_b_ = {-n_a_[1], n_a_[0]};

    const rotation_matrix<real> r_glob_loc = {n_a_[0], n_b_[0], real(0),
                                              n_a_[1], n_b_[1], real(0),
                                              real(0), real(0), real(1)};
    const rotation_matrix<real> r_loc_glob = transpose(r_glob_loc);
    const real pi = std::numbers::pi_v<tfel::math::base_type<real>>;
    const real zero = real(0);
    using compliance = types::compliance<StressType>;
    tfel::math::st2tost2<2u, compliance> P;
    for (int i = 0; i < 2; i++)
      for (int j = i; j < 2; j++)
        for (int k = 0; k < 2; k++)
          for (int l = k; l < 2; l++) {
            const auto p_ = [C, a, b, i, j, k, l](const real& theta) {
              return internals::p_ijkl_2D<real, StressType, LengthType>(
                  C, a, b, theta, i, j, k, l);
            };
            const auto int_p = compliance(
                internals::integrate1D<real>(p_, zero, 2 * pi, max_it));
            tfel::math::setComponent<compliance>(P, i, j, k, l, int_p);
          }
    return change_basis(P * StressType(1), r_loc_glob) / StressType(1);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeAnisotropicHillTensor(
          const tfel::math::st2tost2<3u, StressType>& C,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const std::size_t max_it) {
    using LengthType = types::length<StressType>;
    using real = types::real<StressType>;
    if (not((a > LengthType{0}) and (b > LengthType{0}) and
            (c > LengthType{0}))) {
      tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
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
    using namespace tfel::math;
    const auto n_a_ = n_a / norm(n_a);
    const auto n_b_ = n_b / norm(n_b);
    const auto n_c_ = cross_product<real>(n_a_, n_b_);

    const rotation_matrix<real> r_glob_loc = {n_a_[0], n_b_[0], n_c_[0],
                                              n_a_[1], n_b_[1], n_c_[1],
                                              n_a_[2], n_b_[2], n_c_[2]};
    const rotation_matrix<real> r_loc_glob = transpose(r_glob_loc);
    const auto C_loc =
        change_basis(C / StressType(1), r_glob_loc) * StressType(1);

    const real pi = std::numbers::pi_v<tfel::math::base_type<real>>;
    const real zero = real(0);
    using compliance = types::compliance<StressType>;
    tfel::math::st2tost2<3u, compliance> P;
    for (int i = 0; i < 3; i++)
      for (int j = i; j < 3; j++)
        for (int k = 0; k < 3; k++)
          for (int l = k; l < 3; l++) {
            const auto p_ = [C_loc, a, b, c, i, j, k, l](const real& theta,
                                                         const real& phi) {
              return internals::p_ijkl<real, StressType, LengthType>(
                  C_loc, theta, phi, a, b, c, i, j, k, l);
            };
            const compliance int_p = compliance(internals::dblintegrate<real>(
                p_, zero, pi, zero, 2 * pi, max_it));
            tfel::math::setComponent<compliance>(P, i, j, k, l, int_p);
          }
    return change_basis(P * StressType(1), r_loc_glob) / StressType(1);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::
          st2tost2<3u, types::real<StressType>> computeAnisotropicEshelbyTensor(
              const tfel::math::st2tost2<3u, StressType>& C,
              const tfel::math::tvector<3u, types::real<StressType>>& n_a,
              const types::length<StressType>& a,
              const tfel::math::tvector<3u, types::real<StressType>>& n_b,
              const types::length<StressType>& b,
              const types::length<StressType>& c,
              const std::size_t max_it) {
    return computeAnisotropicHillTensor<StressType>(C, n_a, a, n_b, b, c,
                                                    max_it) *
           C;
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computePlaneStrainAnisotropicEshelbyTensor(
          const tfel::math::st2tost2<2u, StressType>& C,
          const tfel::math::tvector<2u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const std::size_t max_it) {
    return computePlaneStrainAnisotropicHillTensor<StressType>(C, n_a, a, b,
                                                               max_it) *
           C;
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAnisotropicLocalisationTensor(
          const tfel::math::st2tost2<3u, StressType>& C_0_glob,
          const tfel::math::st2tost2<3u, StressType>& C_i_loc,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const std::size_t max_it) {
    using real = types::real<StressType>;
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

    using namespace tfel::math;
    const rotation_matrix<real> r_glob_loc = {n_a_[0], n_b_[0], n_c_[0],
                                              n_a_[1], n_b_[1], n_c_[1],
                                              n_a_[2], n_b_[2], n_c_[2]};
    const rotation_matrix<real> r_loc_glob = transpose(r_glob_loc);
    const auto P_0_glob = computeAnisotropicHillTensor<StressType>(
        C_0_glob, n_a_, a, n_b_, b, c, max_it);
    const auto C_i_glob =
        change_basis(C_i_loc / StressType(1), r_loc_glob) * StressType(1);
    const st2tost2<3u, StressType> C = C_i_glob - C_0_glob;
    const auto Pr = P_0_glob * C;
    const auto A = invert(st2tost2<3u, real>::Id() + Pr);
    return A;
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computePlaneStrainAnisotropicLocalisationTensor(
          const tfel::math::st2tost2<2u, StressType>& C_0_glob,
          const tfel::math::st2tost2<2u, StressType>& C_i_loc,
          const tfel::math::tvector<2u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const std::size_t max_it) {
    using real = types::real<StressType>;
    if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
      tfel::reportContractViolation("n_a is null");
    }
    using namespace tfel::math;
    const auto n_a_ = n_a / norm(n_a);
    tvector<2u, real> n_b_ = {-n_a_[1], n_a_[0]};

    const rotation_matrix<real> r_glob_loc = {n_a_[0], n_b_[0], real(0),
                                              n_a_[1], n_b_[1], real(0),
                                              real(0), real(0), real(1)};
    const rotation_matrix<real> r_loc_glob = transpose(r_glob_loc);
    const auto P_0_glob = computePlaneStrainAnisotropicHillTensor<StressType>(
        C_0_glob, n_a_, a, b, max_it);
    const auto C_i_glob =
        change_basis(C_i_loc / StressType(1), r_loc_glob) * StressType(1);
    const st2tost2<2u, StressType> C = C_i_glob - C_0_glob;
    const auto Pr = P_0_glob * C;
    const auto A = invert(st2tost2<2u, real>::Id() + Pr);
    return A;
  }  // end of computePlaneStrainAnisotropicLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) struct AnisotropicLocalisationTensor<2u, StressType> {
    static TFEL_HOST_DEVICE tfel::math::st2tost2<2u, types::real<StressType>>
    exe(const tfel::math::st2tost2<2u, StressType>& C_0_glob,
        const tfel::math::st2tost2<2u, StressType>& C_i_loc,
        const tfel::math::tvector<2u, types::real<StressType>>& n_a,
        const std::array<types::length<StressType>, 2u>& semiLengths,
        const std::size_t max_it) {
      return computePlaneStrainAnisotropicLocalisationTensor<StressType>(
          C_0_glob, C_i_loc, n_a, semiLengths[0], semiLengths[1], max_it);
    }
  };  // end of AnisotropicLocalisationTensor<2u, StressType>

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) struct AnisotropicLocalisationTensor<3u, StressType> {
    static TFEL_HOST_DEVICE tfel::math::st2tost2<3u, types::real<StressType>>
    exe(const tfel::math::st2tost2<3u, StressType>& C_0_glob,
        const tfel::math::st2tost2<3u, StressType>& C_i_loc,
        const tfel::math::tvector<3u, types::real<StressType>>& n_a,
        const tfel::math::tvector<3u, types::real<StressType>>& n_b,
        const std::array<types::length<StressType>, 3u>& semiLengths,
        const std::size_t max_it) {
      return computeAnisotropicLocalisationTensor<StressType>(
          C_0_glob, C_i_loc, n_a, semiLengths[0], n_b, semiLengths[1],
          semiLengths[2], max_it);
    }
  };  // end of AnisotropicLocalisationTensor<3u, StressType>

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<N, types::real<StressType>> computeGeneralAnisotropicLocalisationTensor(
          const tfel::math::st2tost2<N, StressType>& C_0_glob,
          const tfel::math::st2tost2<N, StressType>& C_i_loc,
          const tfel::math::tvector<N, types::real<StressType>>& n_a,
          const tfel::math::tvector<N, types::real<StressType>>& n_b,
          const std::array<types::length<StressType>, N>& semiLengths,
          const std::size_t max_it) {
    return AnisotropicLocalisationTensor<N, StressType>::exe(
        C_0_glob, C_i_loc, n_a, n_b, semiLengths, max_it);
  }

}  // namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_ANISOTROPICESHELBYTENSOR_IXX */
