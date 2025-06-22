/*!
 * \file   include/TFEL/Material/EshelbyBasedHomogenization.ixx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file defines some homogenization schemes based on solution of
 * Eshelby problem. \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_IXX
#define LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_IXX

#include <cmath>

namespace tfel::material {

  namespace homogenization {
    namespace elasticity {

      template <unsigned short N,
                typename real,
                typename StressType,
                typename LengthType>
      struct EllipsoidMeanLocalisator {
        static constexpr auto eps = std::numeric_limits<real>::epsilon();

        TFEL_HOST_DEVICE static const std::pair<real, real> Isotropic(
            const StressType& young,
            const real& nu,
            const StressType& young_i,
            const real& nu_i,
            const LengthType& a,
            const LengthType& b,
            const LengthType& c) {
          if ((nu > 0.5) || (nu < -1)) {
            tfel::reportContractViolation("nu>0.5 or nu<-1");
          }
          if (not(young > StressType{0})) {
            tfel::reportContractViolation("E<=0");
          }
          if (not((a > LengthType{0}) and (b > LengthType{0}) and
                  (c > LengthType{0}))) {
            tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
          }
          const tfel::math::tvector<3u, real> n_1 = {1., 0., 0.};
          const tfel::math::tvector<3u, real> n_2 = {0., 1., 0.};
          real mu;
          real ka;
          if ((a == b) and (b == c)) {
            const auto A = computeSphereLocalisationTensor<real, StressType>(
                young, nu, young_i, nu_i);
            mu = A(3, 3) / 2;
            ka = A(0, 0) - 4 * mu / 3;
          } else if ((a == b) || (a == c) || (b == c)) {
            tfel::math::st2tost2<3u, real> A_;
            if (a == b) {
              A_ = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                    StressType>(
                  young, nu, young_i, nu_i, n_1, c / a);
            } else if (a == c) {
              A_ = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                    StressType>(
                  young, nu, young_i, nu_i, n_1, b / a);
            } else {
              A_ = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                    StressType>(
                  young, nu, young_i, nu_i, n_1, a / b);
            }
            const auto A1111 = 8 * A_(1, 1) / 15 + A_(0, 0) / 5 +
                               2 * (A_(2, 0) + A_(0, 2) + 2 * A_(4, 4)) / 15;
            const auto A1122 = A_(1, 1) / 15 + A_(0, 0) / 15 + A_(1, 2) / 3 +
                               4 * A_(0, 1) / 15 + 4 * A_(1, 0) / 15 -
                               2 * A_(3, 3) / 15;
            mu = (A1111 - A1122) / 2;
            ka = (A1111 + 2 * A1122) / 3;
          } else {
            const auto A_ = computeEllipsoidLocalisationTensor<real, StressType,
                                                               LengthType>(
                young, nu, young_i, nu_i, n_1, a, n_2, b, c);
            const auto A1111 = A_(0, 0) / 5 + A_(1, 1) / 5 + A_(2, 2) / 5 +
                               (A_(0, 1) + A_(1, 0) + 2 * A_(3, 3)) / 15 +
                               (A_(0, 2) + A_(2, 0) + 2 * A_(4, 4)) / 15 +
                               (A_(1, 2) + A_(2, 1) + 2 * A_(5, 5)) / 15;
            const auto A1122 =
                A_(0, 0) / 15 + A_(1, 1) / 15 + A_(2, 2) / 15 +
                2 * A_(0, 1) / 15 - 2 * A_(3, 3) / 30 + 2 * A_(1, 0) / 15 +
                2 * A_(2, 1) / 15 - 2 * A_(5, 5) / 30 + 2 * A_(1, 2) / 15 +
                2 * A_(2, 0) / 15 - 2 * A_(4, 4) / 30 + 2 * A_(0, 2) / 15;
            mu = (A1111 - A1122) / 2;
            ka = (A1111 + 2 * A1122) / 3;
          }
          return {ka, mu};
        }  // end of Isotropic

        TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
        TransverseIsotropic(const StressType& young,
                            const real& nu,
                            const StressType& young_i,
                            const real& nu_i,
                            const tfel::math::tvector<3u, real>& n_a,
                            const LengthType& a,
                            const LengthType& b,
                            const LengthType& c) {
          if ((nu > 0.5) || (nu < -1)) {
            tfel::reportContractViolation("nu>0.5 or nu<-1");
          }
          if (not(young > StressType{0})) {
            tfel::reportContractViolation("E<=0");
          }
          if (not((a > LengthType{0}) and (b > LengthType{0}) and
                  (c > LengthType{0}))) {
            tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
          }
          if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
            tfel::reportContractViolation("n_a is null");
          }
          using namespace tfel::math;
          tvector<3u, real> n_;
          if ((n_a[1] != 0.) || (n_a[2] != 0.)) {
            n_ = {1., 0., 0.};
          } else {
            n_ = {0., 1., 0.};
          }
          const auto n_2 = cross_product(n_a, n_);
          const auto n_3 = cross_product(n_a, n_2);
          const tvector<3u, real> n_x = {1., 0., 0.};
          const tvector<3u, real> n_y = {0., 1., 0.};
          const tvector<3u, real> n_z = {0., 0., 1.};
          st2tost2<3u, real> A;
          if ((a == b) and (b == c)) {
            A = computeSphereLocalisationTensor<real, StressType>(
                young, nu, young_i, nu_i);
          } else if (b == c) {
            const auto A_ =
                computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                 StressType>(
                    young, nu, young_i, nu_i, n_z, a / b);
            const rotation_matrix<real> r = {n_2[0], n_2[1], n_2[2],
                                             n_3[0], n_3[1], n_3[2],
                                             n_a[0], n_a[1], n_a[2]};
            A = change_basis(A_, r);
          } else {
            st2tost2<3u, real> A_;
            if (a == b) {
              A_ = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                    StressType>(
                  young, nu, young_i, nu_i, n_y, c / a);
            } else if (a == c) {
              A_ = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                    StressType>(
                  young, nu, young_i, nu_i, n_x, b / a);
            } else {
              A_ = computeEllipsoidLocalisationTensor<real, StressType,
                                                      LengthType>(
                  young, nu, young_i, nu_i, n_z, a, n_x, b, c);
            }
            const auto A11 = 3 * (A_(0, 0) + A_(1, 1)) / 8 +
                             (A_(0, 1) + A_(1, 0) + 2 * A_(3, 3)) / 8;
            const auto A22 = A11;
            const auto A33 = A_(2, 2);
            const auto A12 = (A_(0, 0) + A_(1, 1)) / 8 +
                             3 * (A_(0, 1) + A_(1, 0)) / 8 - 2 * A_(3, 3) / 8;
            const auto A21 = A12;
            const auto A13 = (A_(0, 2) + A_(1, 2)) / 2;
            const auto A23 = A13;
            const auto A31 = (A_(2, 1) + A_(2, 0)) / 2;
            const auto A32 = A31;
            const auto A44 =
                2 * ((A_(0, 0) + A_(1, 1) - A_(0, 1) - A_(1, 0)) / 8 +
                     A_(3, 3) / 4);
            const auto A55 = 2 * (A_(4, 4) / 4 + A_(5, 5) / 4);
            const auto A66 = A55;
            const auto zero = real{0};
            const st2tost2<3u, real> A_moy = {
                A11,  A12,  A13,  zero, zero, zero, A21,  A22,  A23,
                zero, zero, zero, A31,  A32,  A33,  zero, zero, zero,
                zero, zero, zero, A44,  zero, zero, zero, zero, zero,
                zero, A55,  zero, zero, zero, zero, zero, zero, A66};
            const rotation_matrix<real> r = {n_2[0], n_2[1], n_2[2],
                                             n_3[0], n_3[1], n_3[2],
                                             n_a[0], n_a[1], n_a[2]};
            A = change_basis(A_moy, r);
          }
          return A;
        }  // end of TransverseIsotropic

        TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real> Oriented(
            const StressType& young,
            const real& nu,
            const StressType& young_i,
            const real& nu_i,
            const tfel::math::tvector<3u, real>& n_a,
            const LengthType& a,
            const tfel::math::tvector<3u, real>& n_b,
            const LengthType& b,
            const LengthType& c) {
          if ((nu > 0.5) || (nu < -1)) {
            tfel::reportContractViolation("nu>0.5 or nu<-1");
          }
          if (not(young > StressType{0})) {
            tfel::reportContractViolation("E<=0");
          }
          if (not((a > LengthType{0}) and (b > LengthType{0}) and
                  (c > LengthType{0}))) {
            tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
          }
          if (not(tfel::math::ieee754::fpclassify(
                      tfel::math::VectorVectorDotProduct::exe<
                          real, tfel::math::tvector<3u, real>,
                          tfel::math::tvector<3u, real>>(n_a, n_b)) ==
                  FP_ZERO)) {
            tfel::reportContractViolation("n_a and n_b not normals");
          }
          if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
            tfel::reportContractViolation("n_a is null");
          }
          if (tfel::math::ieee754::fpclassify(norm(n_b)) == FP_ZERO) {
            tfel::reportContractViolation("n_b is null");
          }
          using namespace tfel::math;
          st2tost2<3u, real> A;
          if ((a == b) and (b == c)) {
            A = computeSphereLocalisationTensor<real, StressType>(
                young, nu, young_i, nu_i);
          } else if (a == b) {
            const tvector<3u, real> n_1 = tfel::math::cross_product(n_a, n_b);
            A = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                 StressType>(
                young, nu, young_i, nu_i, n_1, c / a);
          } else if (a == c) {
            A = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                 StressType>(
                young, nu, young_i, nu_i, n_b, b / a);
          } else if (b == c) {
            A = computeAxisymmetricalEllipsoidLocalisationTensor<real,
                                                                 StressType>(
                young, nu, young_i, nu_i, n_a, a / b);
          } else {
            A = computeEllipsoidLocalisationTensor<real, StressType,
                                                   LengthType>(
                young, nu, young_i, nu_i, n_a, a, n_b, b, c);
          }
          return A;
        }  // end of Oriented

      };  // end of struct EllipsoidMeanLocalisator ;

      template <typename real, typename StressType>
      TFEL_HOST_DEVICE const tfel::math::st2tost2<3u, StressType>
      computeDiluteScheme(const StressType& young,
                          const real& nu,
                          const real& f,
                          const StressType& young_i,
                          const real& nu_i,
                          const tfel::math::st2tost2<3u, real>& A) {
        if ((nu > 0.5) || (nu < -1)) {
          tfel::reportContractViolation("nu>0.5 or nu<-1");
        }
        if (not(young > StressType{0})) {
          tfel::reportContractViolation("E<=0");
        }
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        if ((nu_i > 0.5) || (nu_i < -1)) {
          tfel::reportContractViolation("nu_i>0.5 or nu_i<-1");
        }
        if (not(young_i > StressType{0})) {
          tfel::reportContractViolation("E_i<=0");
        }
        using namespace tfel::math;
        st2tost2<3u, StressType> C_0;
        static constexpr auto value =
            StiffnessTensorAlterationCharacteristic::UNALTERED;
        computeIsotropicStiffnessTensorII<3u, value, StressType, real>(
            C_0, young, nu);
        st2tost2<3u, StressType> C_i;
        computeIsotropicStiffnessTensorII<3u, value, StressType, real>(
            C_i, young_i, nu_i);
        const auto C = C_i - C_0;
        const auto Pr = C * A;
        return C_0 + f * Pr;
      }  // end of DiluteScheme

      template <typename real, typename StressType>
      TFEL_HOST_DEVICE const tfel::math::st2tost2<3u, StressType>
      computeMoriTanakaScheme(const StressType& young,
                              const real& nu,
                              const real& f,
                              const StressType& young_i,
                              const real& nu_i,
                              const tfel::math::st2tost2<3u, real>& A) {
        if ((nu > 0.5) || (nu < -1)) {
          tfel::reportContractViolation("nu>0.5 or nu<-1");
        }
        if (not(young > StressType{0})) {
          tfel::reportContractViolation("E<=0");
        }
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        if ((nu_i > 0.5) || (nu_i < -1)) {
          tfel::reportContractViolation("nu_i>0.5 or nu_i<-1");
        }
        if (not(young_i > StressType{0})) {
          tfel::reportContractViolation("E_i<=0");
        }
        using namespace tfel::math;
        st2tost2<3u, StressType> C_0;
        static constexpr auto value =
            StiffnessTensorAlterationCharacteristic::UNALTERED;
        computeIsotropicStiffnessTensorII<3u, value, StressType, real>(
            C_0, young, nu);
        st2tost2<3u, StressType> C_i;
        computeIsotropicStiffnessTensorII<3u, value, StressType, real>(
            C_i, young_i, nu_i);
        const auto C = C_i - C_0;
        const auto Pr = C * A;
        const auto inv = invert(f * A + (1 - f) * st2tost2<3u, real>::Id());
        const auto PPr = Pr * inv;
        return C_0 + f * PPr;
      }  // end of MoriTanakaScheme

      template <typename real, typename StressType>
      TFEL_HOST_DEVICE const std::pair<StressType, real>
      computeSphereDiluteScheme(const StressType& young,
                                const real& nu,
                                const real& f,
                                const StressType& young_i,
                                const real& nu_i) {
        if ((nu > 0.5) || (nu < -1)) {
          tfel::reportContractViolation("nu>0.5 or nu<-1");
        }
        if (not(young > StressType{0})) {
          tfel::reportContractViolation("E<=0");
        }
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }

        const auto k0 = young / 3 / (1 - 2 * nu);
        const auto mu0 = young / 2 / (1 + nu);
        const auto k_i = young_i / 3 / (1 - 2 * nu_i);
        const auto mu_i = young_i / 2 / (1 + nu_i);
        const auto alpha0 = 3 * k0 / (3 * k0 + 4 * mu0);
        const auto beta0 = 6 * (k0 + 2 * mu0) / 5 / (3 * k0 + 4 * mu0);
        const auto muhom =
            mu0 + f * (mu_i - mu0) / (1 + beta0 * (mu_i - mu0) / mu0);
        const auto khom = k0 + f * (k_i - k0) / (1 + alpha0 * (k_i - k0) / k0);
        const auto nuhom = (3 * khom - 2 * muhom) / (2 * muhom + 6 * khom);
        const auto Ehom = 2 * muhom * (1 + nuhom);
        return {Ehom, nuhom};
      };

      template <typename real, typename StressType>
      TFEL_HOST_DEVICE const std::pair<StressType, real>
      computeSphereMoriTanakaScheme(const StressType& young,
                                    const real& nu,
                                    const real& f,
                                    const StressType& young_i,
                                    const real& nu_i) {
        if ((nu > 0.5) || (nu < -1)) {
          tfel::reportContractViolation("nu>0.5 or nu<-1");
        }
        if (not(young > StressType{0})) {
          tfel::reportContractViolation("E<=0");
        }
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        const auto k0 = young / 3 / (1 - 2 * nu);
        const auto mu0 = young / 2 / (1 + nu);
        const auto k_i = young_i / 3 / (1 - 2 * nu_i);
        const auto mu_i = young_i / 2 / (1 + nu_i);
        const auto muhom =
            mu0 +
            f * (mu_i - mu0) /
                (1 + (1 - f) * (mu_i - mu0) /
                         (mu0 + mu0 * (9 * k0 + 8 * mu0) / 6 / (k0 + 2 * mu0)));
        const auto khom =
            k0 +
            f * (k_i - k0) / (1 + (1 - f) * (k_i - k0) / (k0 + 4 * mu0 / 3));
        const auto nuhom = (3 * khom - 2 * muhom) / (2 * muhom + 6 * khom);
        const auto Ehom = 2 * muhom * (1 + nuhom);
        return {Ehom, nuhom};
      };

      template <typename real, typename StressType, typename LengthType>
      const std::pair<StressType, real> computeIsotropicDiluteScheme(
          const StressType& young,
          const real& nu,
          const real& f,
          const StressType& young_i,
          const real& nu_i,
          const LengthType& a,
          const LengthType& b,
          const LengthType& c) {
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }

        const auto pair =
            EllipsoidMeanLocalisator<3u, real, StressType,
                                     LengthType>::Isotropic(young, nu, young_i,
                                                            nu_i, a, b, c);
        const auto ka = std::get<0>(pair);
        const auto mu = std::get<1>(pair);
        const auto k0 = young / 3 / (1 - 2 * nu);
        const auto mu0 = young / 2 / (1 + nu);
        const auto k_i = young_i / 3 / (1 - 2 * nu_i);
        const auto mu_i = young_i / 2 / (1 + nu_i);
        const auto muhom = mu0 + 2 * f * (mu_i - mu0) * mu;
        const auto khom = k0 + 3 * f * (k_i - k0) * ka;
        const auto nuhom = (3 * khom - 2 * muhom) / (2 * muhom + 6 * khom);
        const auto Ehom = 2 * muhom * (1 + nuhom);
        return {Ehom, nuhom};
      };

      template <typename real, typename StressType, typename LengthType>
      const tfel::math::st2tost2<3u, StressType>
      computeTransverseIsotropicDiluteScheme(
          const StressType& young,
          const real& nu,
          const real& f,
          const StressType& young_i,
          const real& nu_i,
          const tfel::math::tvector<3u, real>& n_a,
          const LengthType& a,
          const LengthType& b,
          const LengthType& c) {
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        const auto A =
            EllipsoidMeanLocalisator<3u, real, StressType,
                                     LengthType>::TransverseIsotropic(young, nu,
                                                                      young_i,
                                                                      nu_i, n_a,
                                                                      a, b, c);
        return computeDiluteScheme<real, StressType>(young, nu, f, young_i,
                                                     nu_i, A);
      };

      template <typename real, typename StressType, typename LengthType>
      const tfel::math::st2tost2<3u, StressType> computeOrientedDiluteScheme(
          const StressType& young,
          const real& nu,
          const real& f,
          const StressType& young_i,
          const real& nu_i,
          const tfel::math::tvector<3u, real>& n_a,
          const LengthType& a,
          const tfel::math::tvector<3u, real>& n_b,
          const LengthType& b,
          const LengthType& c) {
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        const auto A =
            EllipsoidMeanLocalisator<3u, real, StressType,
                                     LengthType>::Oriented(young, nu, young_i,
                                                           nu_i, n_a, a, n_b, b,
                                                           c);
        return computeDiluteScheme<real, StressType>(young, nu, f, young_i,
                                                     nu_i, A);
      };

      template <typename real, typename StressType, typename LengthType>
      const std::pair<StressType, real> computeIsotropicMoriTanakaScheme(
          const StressType& young,
          const real& nu,
          const real& f,
          const StressType& young_i,
          const real& nu_i,
          const LengthType& a,
          const LengthType& b,
          const LengthType& c) {
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        const auto pair =
            EllipsoidMeanLocalisator<3u, real, StressType,
                                     LengthType>::Isotropic(young, nu, young_i,
                                                            nu_i, a, b, c);
        const auto ka = std::get<0>(pair);
        const auto mu = std::get<1>(pair);
        const auto k0 = young / 3 / (1 - 2 * nu);
        const auto mu0 = young / 2 / (1 + nu);
        const auto k_i = young_i / 3 / (1 - 2 * nu_i);
        const auto mu_i = young_i / 2 / (1 + nu_i);
        const auto muhom = mu0 + f * (mu_i - mu0) * mu / (f * mu + (1 - f) / 2);
        const auto khom = k0 + f * (k_i - k0) * ka / (ka * f + (1 - f) / 3);
        const auto nuhom = (3 * khom - 2 * muhom) / (2 * muhom + 6 * khom);
        const auto Ehom = 2 * muhom * (1 + nuhom);
        return {Ehom, nuhom};
      };

      template <typename real, typename StressType, typename LengthType>
      const tfel::math::st2tost2<3u, StressType>
      computeTransverseIsotropicMoriTanakaScheme(
          const StressType& young,
          const real& nu,
          const real& f,
          const StressType& young_i,
          const real& nu_i,
          const tfel::math::tvector<3u, real>& n_a,
          const LengthType& a,
          const LengthType& b,
          const LengthType& c) {
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        const auto A =
            EllipsoidMeanLocalisator<3u, real, StressType,
                                     LengthType>::TransverseIsotropic(young, nu,
                                                                      young_i,
                                                                      nu_i, n_a,
                                                                      a, b, c);
        return computeMoriTanakaScheme<real, StressType>(young, nu, f, young_i,
                                                         nu_i, A);
      };

      template <typename real, typename StressType, typename LengthType>
      const tfel::math::st2tost2<3u, StressType>
      computeOrientedMoriTanakaScheme(const StressType& young,
                                      const real& nu,
                                      const real& f,
                                      const StressType& young_i,
                                      const real& nu_i,
                                      const tfel::math::tvector<3u, real>& n_a,
                                      const LengthType& a,
                                      const tfel::math::tvector<3u, real>& n_b,
                                      const LengthType& b,
                                      const LengthType& c) {
        if ((f < 0) || (f > 1)) {
          tfel::reportContractViolation("f<0 or f>1");
        }
        const auto A =
            EllipsoidMeanLocalisator<3u, real, StressType,
                                     LengthType>::Oriented(young, nu, young_i,
                                                           nu_i, n_a, a, n_b, b,
                                                           c);
        return computeMoriTanakaScheme<real, StressType>(young, nu, f, young_i,
                                                         nu_i, A);
      };

    }  // end of namespace elasticity
  }    // end of namespace homogenization

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_IXX */
