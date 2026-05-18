/*!
 * \file   include/TFEL/Material/LinearHomogenizationSchemes.ixx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file defines some homogenization schemes based on solution of
 * Eshelby problem. \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_IXX
#define LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_IXX

#include <cmath>
#include "TFEL/Material/OrientationAverages.hxx"

namespace tfel::material::homogenization::elasticity {

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
    //if ((nu > 0.5) || (nu < -1)) {
    //  tfel::reportContractViolation("nu>0.5 or nu<-1");
    //}
    //if (not(young > StressType{0})) {
    //  tfel::reportContractViolation("E<=0");
    //}
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    //if ((nu_i > 0.5) || (nu_i < -1)) {
    //  tfel::reportContractViolation("nu_i>0.5 or nu_i<-1");
    //}
    //if (not(young_i > StressType{0})) {
    //  tfel::reportContractViolation("E_i<=0");
    //}
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
    //if ((nu > 0.5) || (nu < -1)) {
    //  tfel::reportContractViolation("nu>0.5 or nu<-1");
    //}
    //if (not(young > StressType{0})) {
    //  tfel::reportContractViolation("E<=0");
    //}
    if ((f < 0) || (f > 1)) {
      tfel::reportContractViolation("f<0 or f>1");
    }
    //if ((nu_i > 0.5) || (nu_i < -1)) {
    //  tfel::reportContractViolation("nu_i>0.5 or nu_i<-1");
    //}
    //if (not(young_i > StressType{0})) {
    //  tfel::reportContractViolation("E_i<=0");
    //}
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
                                              StressType>()) TFEL_HOST_DEVICE
      const YoungNuModuli<StressType> computeSphereDiluteScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i) {
    //if ((nu > 0.5) || (nu < -1)) {
    //  tfel::reportContractViolation("nu>0.5 or nu<-1");
    //}
    //if (not(young > StressType{0})) {
    //  tfel::reportContractViolation("E<=0");
    //}
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
    const auto kg = KGModuli<StressType>(khom, muhom);
    return kg.ToYoungNu();
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const KGModuli<StressType> computeSphereDiluteScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    const auto Enu =
        computeSphereDiluteScheme(Enu0.young, Enu0.nu, f, Enui.young, Enui.nu);
    return Enu.ToKG();
  }  // end of computeSphereDiluteScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const YoungNuModuli<StressType> computeSphereMoriTanakaScheme(
          const StressType& young,
          const types::real<StressType>& nu,
          const types::real<StressType>& f,
          const StressType& young_i,
          const types::real<StressType>& nu_i) {
    //if ((nu > 0.5) || (nu < -1)) {
    //  tfel::reportContractViolation("nu>0.5 or nu<-1");
    //}
    //if (not(young > StressType{0})) {
    //  tfel::reportContractViolation("E<=0");
    //}
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
    const auto kg = KGModuli<StressType>(khom, muhom);
    return kg.ToYoungNu();
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const KGModuli<StressType> computeSphereMoriTanakaScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    const auto Enu = computeSphereMoriTanakaScheme(Enu0.young, Enu0.nu, f,
                                                   Enui.young, Enui.nu);
    return Enu.ToKG();
  }  // end of computeSphereMoriTanakaScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const YoungNuModuli<StressType> computeIsotropicDiluteScheme(
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

    const auto KG = EllipsoidMeanLocalisator<3u, StressType>::Isotropic(
        young, nu, young_i, nu_i, a, b, c);
    const auto ka = std::get<0>(KG);
    const auto mu = std::get<1>(KG);
    const auto k0 = young / 3 / (1 - 2 * nu);
    const auto mu0 = young / 2 / (1 + nu);
    const auto k_i = young_i / 3 / (1 - 2 * nu_i);
    const auto mu_i = young_i / 2 / (1 + nu_i);
    const auto muhom = mu0 + 2 * f * (mu_i - mu0) * mu;
    const auto khom = k0 + 3 * f * (k_i - k0) * ka;
    const auto kg = KGModuli<StressType>(khom, muhom);
    return kg.ToYoungNu();
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const KGModuli<StressType> computeIsotropicDiluteScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    const auto Enu = computeIsotropicDiluteScheme(Enu0.young, Enu0.nu, f,
                                                  Enui.young, Enui.nu, a, b, c);
    return Enu.ToKG();
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
        EllipsoidMeanLocalisator<3u, StressType>::TransverseIsotropic(
            young, nu, young_i, nu_i, n_a, a, b, c);
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
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeTransverseIsotropicDiluteScheme(
        Enu0.young, Enu0.nu, f, Enui.young, Enui.nu, n_a, a, b, c);
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
    const auto A = EllipsoidMeanLocalisator<3u, StressType>::Oriented(
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
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeOrientedDiluteScheme(Enu0.young, Enu0.nu, f, Enui.young,
                                       Enui.nu, n_a, a, n_b, b, c);
  }  // end of computeOrientedDiluteScheme

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const YoungNuModuli<StressType> computeIsotropicMoriTanakaScheme(
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
    const auto KG = EllipsoidMeanLocalisator<3u, StressType>::Isotropic(
        young, nu, young_i, nu_i, a, b, c);
    const auto ka = std::get<0>(KG);
    const auto mu = std::get<1>(KG);
    const auto k0 = young / 3 / (1 - 2 * nu);
    const auto mu0 = young / 2 / (1 + nu);
    const auto k_i = young_i / 3 / (1 - 2 * nu_i);
    const auto mu_i = young_i / 2 / (1 + nu_i);
    const auto muhom = mu0 + f * (mu_i - mu0) * mu / (f * mu + (1 - f) / 2);
    const auto khom = k0 + f * (k_i - k0) * ka / (ka * f + (1 - f) / 3);
    const auto kg = KGModuli<StressType>(khom, muhom);
    return kg.ToYoungNu();
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const KGModuli<StressType> computeIsotropicMoriTanakaScheme(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IM_i,
          const types::length<StressType>& a,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    const auto Enu = computeIsotropicMoriTanakaScheme(
        Enu0.young, Enu0.nu, f, Enui.young, Enui.nu, a, b, c);
    return Enu.ToKG();
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
        EllipsoidMeanLocalisator<3u, StressType>::TransverseIsotropic(
            young, nu, young_i, nu_i, n_a, a, b, c);
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
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeTransverseIsotropicMoriTanakaScheme(
        Enu0.young, Enu0.nu, f, Enui.young, Enui.nu, n_a, a, b, c);
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
    const auto A = EllipsoidMeanLocalisator<3u, StressType>::Oriented(
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
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeOrientedMoriTanakaScheme(Enu0.young, Enu0.nu, f, Enui.young,
                                           Enui.nu, n_a, a, n_b, b, c);
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
    const auto KGA = EllipsoidMeanLocalisator<3u, StressType>::Isotropic(
        young, nu, young_i, nu_i, a, b, c);
    const auto ka = std::get<0>(KGA);
    const auto mu = std::get<1>(KGA);
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
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeIsotropicPCWScheme(Enu0.young, Enu0.nu, f, Enui.young,
                                     Enui.nu, a, b, c, D);
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
        EllipsoidMeanLocalisator<3u, StressType>::TransverseIsotropic(
            young, nu, young_i, nu_i, n_a, a, b, c);

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
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeTransverseIsotropicPCWScheme(
        Enu0.young, Enu0.nu, f, Enui.young, Enui.nu, n_a, a, b, c, D);
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

    const auto A = EllipsoidMeanLocalisator<3u, StressType>::Oriented(
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
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeOrientedPCWScheme(Enu0.young, Enu0.nu, f, Enui.young, Enui.nu,
                                    n_a, a, n_b, b, c, D);
  }

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_IXX */
