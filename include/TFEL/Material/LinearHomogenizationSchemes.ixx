/*!
 * \file   include/TFEL/Material/LinearHomogenizationSchemes.ixx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file defines some homogenization schemes based on solution of
 * Eshelby problem. \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_IXX
#define LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_IXX

#include <cmath>

namespace tfel::material::homogenization::elasticity {

  template <unsigned short N,
            tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
  struct EllipsoidMeanLocalisator {
     using real = types::real<StressType>;
     using LengthType=types::length<StressType>;
     
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
        const auto A = computeSphereLocalisationTensor<StressType>(
            young, nu, young_i, nu_i);
        mu = A(3, 3) / 2;
        ka = A(0, 0) - 4 * mu / 3;
      } else if ((a == b) || (a == c) || (b == c)) {
        tfel::math::st2tost2<3u, real> A_;
        if (a == b) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_1, c / a);
        } else if (a == c) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_1, b / a);
        } else {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
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
        const auto A_ = computeEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_1, a, n_2, b, c);
        const auto A1111 = A_(0, 0) / 5 + A_(1, 1) / 5 + A_(2, 2) / 5 +
                           (A_(0, 1) + A_(1, 0) + 2 * A_(3, 3)) / 15 +
                           (A_(0, 2) + A_(2, 0) + 2 * A_(4, 4)) / 15 +
                           (A_(1, 2) + A_(2, 1) + 2 * A_(5, 5)) / 15;
        const auto A1122 =
            A_(0, 0) / 15 + A_(1, 1) / 15 + A_(2, 2) / 15 + 2 * A_(0, 1) / 15 -
            2 * A_(3, 3) / 30 + 2 * A_(1, 0) / 15 + 2 * A_(2, 1) / 15 -
            2 * A_(5, 5) / 30 + 2 * A_(1, 2) / 15 + 2 * A_(2, 0) / 15 -
            2 * A_(4, 4) / 30 + 2 * A_(0, 2) / 15;
        mu = (A1111 - A1122) / 2;
        ka = (A1111 + 2 * A1122) / 3;
      }
      return {ka, mu};
    }  // end of Isotropic
    
    //overloading of the function, for IsotropicModuli objects
    TFEL_HOST_DEVICE static const std::pair<real, real> Isotropic(
        const IsotropicModuli<StressType>& IM0,
        const IsotropicModuli<StressType>& IM_i,
        const LengthType& a,
        const LengthType& b,
        const LengthType& c) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return Isotropic(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), std::get<0>(Enui),
        real(std::get<1>(Enui)), a, b, c);
     }//end of overloading of Isotropic

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
      if ((tfel::math::ieee754::fpclassify(n_a[1]) != FP_ZERO) ||
          (tfel::math::ieee754::fpclassify(n_a[2]) != FP_ZERO)) {
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
        A = computeSphereLocalisationTensor<StressType>(young, nu, young_i,
                                                        nu_i);
      } else if (b == c) {
        const auto A_ =
            computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
                young, nu, young_i, nu_i, n_z, a / b);
        const rotation_matrix<real> r = {n_2[0], n_2[1], n_2[2], n_3[0], n_3[1],
                                         n_3[2], n_a[0], n_a[1], n_a[2]};
        A = change_basis(A_, r);
      } else {
        st2tost2<3u, real> A_;
        if (a == b) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_y, c / a);
        } else if (a == c) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_x, b / a);
        } else {
          A_ = computeEllipsoidLocalisationTensor<StressType>(
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
        const auto A44 = 2 * ((A_(0, 0) + A_(1, 1) - A_(0, 1) - A_(1, 0)) / 8 +
                              A_(3, 3) / 4);
        const auto A55 = 2 * (A_(4, 4) / 4 + A_(5, 5) / 4);
        const auto A66 = A55;
        const auto zero = real{0};
        const st2tost2<3u, real> A_moy = {
            A11,  A12,  A13,  zero, zero, zero, A21,  A22,  A23,
            zero, zero, zero, A31,  A32,  A33,  zero, zero, zero,
            zero, zero, zero, A44,  zero, zero, zero, zero, zero,
            zero, A55,  zero, zero, zero, zero, zero, zero, A66};
        const rotation_matrix<real> r = {n_2[0], n_2[1], n_2[2], n_3[0], n_3[1],
                                         n_3[2], n_a[0], n_a[1], n_a[2]};
        A = change_basis(A_moy, r);
      }
      return A;
    }  // end of TransverseIsotropic
    
     //overloading of the function, for IsotropicModuli objects
    TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
    TransverseIsotropic(const IsotropicModuli<StressType>& IM0,
			const IsotropicModuli<StressType>& IM_i,
			const tfel::math::tvector<3u, real>& n_a,
                        const LengthType& a,
                        const LengthType& b,
                        const LengthType& c) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return TransverseIsotropic(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, b, c);
     }//end of overloading of TransverseIsotropic

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
      if (not(tfel::math::ieee754::fpclassify(n_a | n_b) == FP_ZERO)) {
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
        A = computeSphereLocalisationTensor<StressType>(young, nu, young_i,
                                                        nu_i);
      } else if (a == b) {
        const tvector<3u, real> n_1 = tfel::math::cross_product(n_a, n_b);
        A = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_1, c / a);
      } else if (a == c) {
        A = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_b, b / a);
      } else if (b == c) {
        A = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_a, a / b);
      } else {
        A = computeEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_a, a, n_b, b, c);
      }
      return A;
    }  // end of Oriented
    
     //overloading of the function, for IsotropicModuli objects
    TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
    Oriented(const IsotropicModuli<StressType>& IM0,
			const IsotropicModuli<StressType>& IM_i,
			const tfel::math::tvector<3u, real>& n_a,
                        const LengthType& a,
                        const tfel::math::tvector<3u, real>& n_b,
                        const LengthType& b,
                        const LengthType& c) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return Oriented(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, n_b, b, c);
     }//end of overloading of Oriented

  };  // end of struct EllipsoidMeanLocalisator ;

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeDiluteScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::st2tost2<3u, types::real<StressType>>& A) {
    using real = types::real<StressType>;
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
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);
    st2tost2<3u, StressType> C_i;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_i, young_i,
                                                                   nu_i);
    const auto C = C_i - C_0;
    const auto Pr = C * A;
    return C_0 + f * Pr;
  }  // end of DiluteScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeMoriTanakaScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::st2tost2<3u, types::real<StressType>>& A) {
    using real = types::real<StressType>;
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
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);
    st2tost2<3u, StressType> C_i;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_i, young_i,
                                                                   nu_i);
    const auto C = C_i - C_0;
    const auto Pr = C * A;
    const auto inv = invert(f * A + (1 - f) * st2tost2<3u, real>::Id());
    const auto PPr = Pr * inv;
    return C_0 + f * PPr;
  }  // end of MoriTanakaScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      std::pair<StressType, types::real<StressType>> computeSphereDiluteScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i) {
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
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      std::pair<StressType, types::real<StressType>> computeSphereDiluteScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeSphereDiluteScheme(std::get<0>(Enu0), real(std::get<1>(Enu0)),
                                     f, std::get<0>(Enui),
                                     real(std::get<1>(Enui)));
  }  // end of computeSphereDiluteScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const std::
      pair<StressType, types::real<StressType>> computeSphereMoriTanakaScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i) {
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
        k0 + f * (k_i - k0) / (1 + (1 - f) * (k_i - k0) / (k0 + 4 * mu0 / 3));
    const auto nuhom = (3 * khom - 2 * muhom) / (2 * muhom + 6 * khom);
    const auto Ehom = 2 * muhom * (1 + nuhom);
    return {Ehom, nuhom};
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const std::
      pair<StressType, types::real<StressType>> computeSphereMoriTanakaScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeSphereMoriTanakaScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)));
  }  // end of computeSphereMoriTanakaScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const std::
      pair<StressType, types::real<StressType>> computeIsotropicDiluteScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }

    const auto pair =
        EllipsoidMeanLocalisator<3u,StressType>::Isotropic(
            young, nu, young_i, nu_i, a, b, c);
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
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const std::
      pair<StressType, types::real<StressType>> computeIsotropicDiluteScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeIsotropicDiluteScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), a, b, c);
  }  // end of computeIsotropicDiluteScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicDiluteScheme(
              const StressType& young,
              const types::real<StressType>& nu,
              const types::real<StressType>& f,
              const StressType& young_i,
              const types::real<StressType>& nu_i,
              const tfel::math::tvector<3u, types::real<StressType>>& n_a,
              const types::length<StressType>& a,
              const types::length<StressType>& b,
              const types::length<StressType>& c) {
    
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    const auto A =
        EllipsoidMeanLocalisator<3u, StressType>::TransverseIsotropic(young, nu,
                                                                  young_i, nu_i,
                                                                  n_a, a, b, c);
    return computeDiluteScheme<StressType>(young, nu, f, young_i, nu_i, A);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicDiluteScheme(
              const IsotropicModuli<StressType>& IM0,
              const types::real<StressType>& f,
              const IsotropicModuli<StressType>& IM_i,
              const tfel::math::tvector<3u, types::real<StressType>>& n_a,
              const types::length<StressType>& a,
              const types::length<StressType>& b,
              const types::length<StressType>& c) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeTransverseIsotropicDiluteScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, b, c);
  }  // end of computeTransverseIsotropicDiluteScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeOrientedDiluteScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    const auto A =
        EllipsoidMeanLocalisator<3u, StressType>::Oriented(
            young, nu, young_i, nu_i, n_a, a, n_b, b, c);
    return computeDiluteScheme<StressType>(young, nu, f, young_i, nu_i, A);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeOrientedDiluteScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeOrientedDiluteScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, n_b, b, c);
  }  // end of computeOrientedDiluteScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const std::pair<
          StressType,
          types::real<
              StressType>> computeIsotropicMoriTanakaScheme(const StressType&
                                                                young,
                                                            const types::real<
                                                                StressType>& nu,
                                                            const types::real<
                                                                StressType>& f,
                                                            const StressType&
                                                                young_i,
                                                            const types::real<
                                                                StressType>&
                                                                nu_i,
                                                            const types::length<
                                                                StressType>& a,
                                                            const types::length<
                                                                StressType>& b,
                                                            const types::length<
                                                                StressType>&
                                                                c) {
    
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    const auto pair =
        EllipsoidMeanLocalisator<3u, StressType>::Isotropic(
            young, nu, young_i, nu_i, a, b, c);
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
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const std::
      pair<StressType, types::real<StressType>> computeIsotropicMoriTanakaScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeIsotropicMoriTanakaScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), a, b, c);
  }  // end of computeIsotropicMoriTanakaScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicMoriTanakaScheme(
              const StressType& young,
              const types::real<StressType>& nu,
              const types::real<StressType>& f,
              const StressType& young_i,
              const types::real<StressType>& nu_i,
              const tfel::math::tvector<3u, types::real<StressType>>& n_a,
              const types::length<StressType>& a,
              const types::length<StressType>& b,
              const types::length<StressType>& c) {
  
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    const auto A =
        EllipsoidMeanLocalisator<3u, StressType>::TransverseIsotropic(young, nu,
                                                                  young_i, nu_i,
                                                                  n_a, a, b, c);
    return computeMoriTanakaScheme<StressType>(young, nu, f, young_i, nu_i, A);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicMoriTanakaScheme(
              const IsotropicModuli<StressType>& IM0,
              const types::real<StressType>& f,
              const IsotropicModuli<StressType>& IM_i,
              const tfel::math::tvector<3u, types::real<StressType>>& n_a,
              const types::length<StressType>& a,
              const types::length<StressType>& b,
              const types::length<StressType>& c) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeTransverseIsotropicMoriTanakaScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, b, c);
  }  // end of computeTransverseIsotropicMoriTanakaScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      tfel::math::st2tost2<3u, StressType> computeOrientedMoriTanakaScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    const auto A =
        EllipsoidMeanLocalisator<3u, StressType>::Oriented(
            young, nu, young_i, nu_i, n_a, a, n_b, b, c);
    return computeMoriTanakaScheme<StressType>(young, nu, f, young_i, nu_i, A);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      tfel::math::st2tost2<3u, StressType> computeOrientedMoriTanakaScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeOrientedMoriTanakaScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, n_b, b, c);
  }  // end of computeOrientedMoriTanakaScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      const tfel::math::st2tost2<3u, StressType> computePCWScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::st2tost2<3u, types::real<StressType>>& A,
          const Distribution<StressType>& D) {
    using real = types::real<StressType>;
    using LengthType = types::length<StressType>;
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    if (not(tfel::math::ieee754::fpclassify(
                tfel::math::VectorVectorDotProduct::exe<
                    real, tfel::math::tvector<3u, real>,
                    tfel::math::tvector<3u, real>>(D.n_a, D.n_b)) == FP_ZERO)) {
      tfel::reportContractViolation(
          "n_a and n_b of the distribution are not normals");
    }
    tfel::math::st2tost2<3u, StressType> C_0;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);
    const auto S0 = invert(C_0);
    tfel::math::st2tost2<3u, StressType> C_i;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_i, young_i,
                                                                   nu_i);

    const auto n_a_d_ = D.n_a / norm(D.n_a);
    const auto n_b_d_ = D.n_b / norm(D.n_b);
    const auto n_c_d_ = tfel::math::cross_product<real>(n_a_d_, n_b_d_);
    const std::array<LengthType, 3> abc_ = {D.a, D.b, D.c};
    const auto sig = internals::sortEllipsoidLengths<LengthType>(D.a, D.b, D.c);
    const auto S = computeEshelbyTensor<StressType>(nu, abc_[sig[0]],
                                                    abc_[sig[1]], abc_[sig[2]]);
    const std::array<tfel::math::tvector<3u, real>, 3> nabc_ = {n_a_d_, n_b_d_,
                                                                n_c_d_};
    const auto n_1 = nabc_[sig[0]];
    const auto n_2 = nabc_[sig[1]];
    using namespace tfel::math;
    const auto n_3 = cross_product<real>(n_1, n_2);
    const rotation_matrix<real> r = {n_1[0], n_1[1], n_1[2], n_2[0], n_2[1],
                                     n_2[2], n_3[0], n_3[1], n_3[2]};
    const auto S_basis = change_basis(S, r);
    const auto P_d = S_basis * S0;
    const auto C = C_i - C_0;
    const auto Pr = C * A;
    const auto PPr = Pr * P_d;
    const auto I = tfel::math::st2tost2<3u, real>::Id();
    const auto inv = invert(I - f * PPr);
    return C_0 + f * inv * Pr;
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      const tfel::math::st2tost2<3u, StressType> computeIsotropicPCWScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const Distribution<StressType>& D) {
    using real = types::real<StressType>;
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    const auto pairA =
        EllipsoidMeanLocalisator<3u, StressType>::Isotropic(
            young, nu, young_i, nu_i, a, b, c);
    const auto ka = std::get<0>(pairA);
    const auto mu = std::get<1>(pairA);
    const auto A = 3 * ka * tfel::math::st2tost2<3u, real>::J() +
                   2 * mu * tfel::math::st2tost2<3u, real>::K();

    return computePCWScheme<StressType>(young, nu, f, young_i, nu_i, A, D);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      const tfel::math::st2tost2<3u, StressType> computeIsotropicPCWScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const Distribution<StressType>& D) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeIsotropicPCWScheme(std::get<0>(Enu0), real(std::get<1>(Enu0)),
                                     f, std::get<0>(Enui),
                                     real(std::get<1>(Enui)), a, b, c, D);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) const
      tfel::math::st2tost2<3u, StressType> computeTransverseIsotropicPCWScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const Distribution<StressType>& D) {
   
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }

    const auto A =
        EllipsoidMeanLocalisator<3u, StressType>::TransverseIsotropic(young, nu,
                                                                  young_i, nu_i,
                                                                  n_a, a, b, c);

    return computePCWScheme<StressType>(young, nu, f, young_i, nu_i, A, D);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) const
      tfel::math::st2tost2<3u, StressType> computeTransverseIsotropicPCWScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const Distribution<StressType>& D) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeTransverseIsotropicPCWScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, b, c, D);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      const tfel::math::st2tost2<3u, StressType> computeOrientedPCWScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const Distribution<StressType>& D) {
   
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }

    const auto A =
        EllipsoidMeanLocalisator<3u, StressType>::Oriented(
            young, nu, young_i, nu_i, n_a, a, n_b, b, c);

    return computePCWScheme<StressType>(young, nu, f, young_i, nu_i, A, D);
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      const tfel::math::st2tost2<3u, StressType> computeOrientedPCWScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c,
          const Distribution<StressType>& D) {
    using real = types::real<StressType>;
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeOrientedPCWScheme(
        std::get<0>(Enu0), real(std::get<1>(Enu0)), f, std::get<0>(Enui),
        real(std::get<1>(Enui)), n_a, a, n_b, b, c, D);
  }

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_IXX */
