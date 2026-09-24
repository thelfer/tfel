/*!
 * \file   include/TFEL/Material/MicrostructureLinearHomogenization.ixx
 * \author Antoine Martin
 * \date   25 January 2025
 * \brief  This file implements some homogenization schemes for generic
 * microstructures. \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX
#define LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX

#include <cmath>
#include <numbers>
#include <stdexcept>

namespace tfel::material::homogenization::elasticity {

  namespace internals {
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                StressType>())
        std::vector<tfel::math::stensor<N, StressType>> initialize_polarisation(
            const std::vector<tfel::math::stensor<N, StressType>>&
                polarisations,
            const std::size_t np) {
      std::vector<tfel::math::stensor<N, StressType>> polarisations_;
      if (polarisations.size() == 0) {
        const auto zero = tfel::math::stensor<N, StressType>::zero();
        polarisations_.resize(np, zero);
      } else {
        if (polarisations.size() != np) {
          tfel::reportContractViolation(
              "the size of your polarization vector does not correspond to the "
              "number of phases");
        }
        polarisations_.insert(polarisations_.end(), polarisations.begin(),
                              polarisations.end());
      }
      return polarisations_;
    };
  }  // namespace internals

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeDilute(
          ParticulateMicrostructure<N, StressType>& micro,
          int max_iter_anisotropic_integration,
          const std::vector<tfel::math::stensor<N, StressType>>& polarisations,
          bool with_Chom_derivatives) {
    using real = tfel::types::real<StressType>;
    const auto np = micro.getNumberOfPhases();
    const auto polarisations_ =
        internals::initialize_polarisation<N, StressType>(polarisations, np);

    const auto C0 = micro.getMatrixElasticity();
    KGModuli<StressType> KG0(StressType(0), StressType(0));
    if (micro.isIsotropicMatrix()) {
      KG0 = computeKGModuli<StressType>(C0);
    }

    auto Chom = C0;
    auto tau_eff = tfel::math::stensor<N, StressType>::zero();
    const auto f0 = micro.getMatrixFraction();

    std::vector<tfel::math::st2tost2<N, real>> dChom_dkr = {
        tfel::math::st2tost2<N, real>::zero()};
    std::vector<tfel::math::st2tost2<N, real>> dChom_dmur{
        tfel::math::st2tost2<N, real>::zero()};
    const auto J = tfel::math::st2tost2<3u, real>::J();
    const auto K = tfel::math::st2tost2<3u, real>::K();
    if (with_Chom_derivatives && (not(micro.is_isotropic_matrix()))) {
      tfel::reportContractViolation(
          "I cannot compute the derivatives of the homogenized stiffness "
          "when the matrix elasticity is a st2tost2 object."
          "Try to use a IsotropicModuli for the definition of the matrix "
          "elasticity.");
    }

    std::vector<tfel::math::st2tost2<N, real>> localisators = {
        tfel::math::st2tost2<N, real>::Id() / f0};
    const auto u = real(1.);
    const auto z = real(0.);
    const std::array<real, 4> dk0 = {u, z, z, z};
    const std::array<real, 4> dmu0 = {z, u, z, z};
    const std::array<real, 4> dki = {z, z, u, z};
    const std::array<real, 4> dmui = {z, z, z, u};

    for (std::size_t i = 0; i < np - 1; i++) {
      auto phasei = micro.getInclusionPhase(i);
      auto Ci = (*phasei).getElasticityOfPhase();
      if (not((*phasei).isIsotropic()) && with_Chom_derivatives) {
        tfel::reportContractViolation(
            "I cannot compute the derivatives of the homogenized stiffness "
            "when the elasticity of a phase is a st2tost2 object."
            "Try to use a IsotropicModuli for the elasticity of this phase.");
      }
      auto fi = (*phasei).fraction;
      auto taui = polarisations_[i + 1];
      tfel::math::st2tost2<N, real> Ai;

      if (micro.is_isotropic_matrix()) {
        Ai = (*phasei).computeMeanLocalisator(KG0);
        if (with_Chom_derivatives && (N == 3)) {
          const auto dAi_dk0 =
              (*phasei).computeDerivativesOfMeanLocalisator(KG0, dk0);
          const auto dAi_dmu0 =
              (*phasei).computeDerivativesOfMeanLocalisator(KG0, dmu0);
          const auto dAi_dki =
              (*phasei).computeDerivativesOfMeanLocalisator(KG0, dki);
          const auto dAi_dmui =
              (*phasei).computeDerivativesOfMeanLocalisator(KG0, dmui);
          const auto cdAk = (Ci - C0) * dAi_dk0;
          const auto cdAm = (Ci - C0) * dAi_dmu0;
          dChom_dkr[0] += fi * cdAk;
          dChom_dmur[0] += fi * cdAm;
          auto dChom_dki = fi * ((Ci - C0) * dAi_dki + 3 * J * Ai);
          auto dChom_dmui = fi * ((Ci - C0) * dAi_dmui + 2 * K * Ai);
          dChom_dkr.push_back(dChom_dki);
          dChom_dmur.push_back(dChom_dmui);
        }
      } else {
        Ai = (*phasei).computeMeanLocalisator(C0,
                                              max_iter_anisotropic_integration);
      }
      Chom += fi * (Ci - C0) * Ai;
      tau_eff += fi * tfel::math::transpose(Ai) * taui;
      localisators.push_back(Ai);
      localisators[0] -= fi * Ai / f0;
    }
    const auto A0 = localisators[0];
    dChom_dkr[0] += 3 * f0 * J * A0;
    dChom_dmur[0] += 2 * f0 * K * A0;
    tau_eff += f0 * tfel::math::transpose(A0) * polarisations_[0];

    HomogenizationScheme<N, StressType> h_s = {
        .homogenized_stiffness = Chom,
        .effective_polarisation = tau_eff,
        .mean_strain_localisation_tensors = localisators,
        .derivative_of_homogenized_stiffness_wrt_kr = dChom_dkr,
        .derivative_of_homogenized_stiffness_wrt_mur = dChom_dmur};

    return h_s;
  };

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeMoriTanaka(
          ParticulateMicrostructure<N, StressType>& micro,
          int max_iter_anisotropic_integration,
          const std::vector<tfel::math::stensor<N, StressType>>& polarisations,
          bool with_Chom_derivatives) {
    using real = tfel::types::real<StressType>;

    const auto np = micro.getNumberOfPhases();
    const auto polarisations_ =
        internals::initialize_polarisation<N, StressType>(polarisations, np);

    const auto C0 = micro.getMatrixElasticity();
    KGModuli<StressType> KG0(StressType(0), StressType(0));
    if (micro.isIsotropicMatrix()) {
      KG0 = computeKGModuli<StressType>(C0);
    }

    const auto tau0 = polarisations_[0];
    const auto f0 = micro.getMatrixFraction();
    auto Chom = C0;
    auto tau_eff = tau0;
    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    tfel::math::st2tost2<N, real> A0 = f0 * tfel::math::st2tost2<N, real>::Id();
    std::vector<tfel::math::st2tost2<N, real>> dChom_dkr = {};
    std::vector<tfel::math::st2tost2<N, real>> dChom_dmur = {};
    const auto J = tfel::math::st2tost2<3u, real>::J();
    const auto K = tfel::math::st2tost2<3u, real>::K();

    for (std::size_t i = 0; i < np - 1; i++) {
      auto phasei = micro.getInclusionPhase(i);
      auto fi = (*phasei).fraction;
      tfel::math::st2tost2<N, real> Ai;
      if (micro.isIsotropicMatrix()) {
        Ai = (*phasei).computeMeanLocalisator(KG0);
      } else {
        Ai = (*phasei).computeMeanLocalisator(C0,
                                              max_iter_anisotropic_integration);
      }
      localisators.push_back(Ai);
      A0 += fi * Ai;
    }
    A0 = invert(A0);
    localisators.insert(localisators.begin(), A0);

    if (with_Chom_derivatives && (not(micro.is_isotropic_matrix()))) {
      tfel::reportContractViolation(
          "I cannot compute the derivatives of the homogenized stiffness "
          "when the matrix elasticity is a st2tost2 object."
          "Try to use a IsotropicModuli for the definition of the matrix "
          "elasticity.");
    }
    using compliance = tfel::types::compliance<StressType>;
    auto sum_dAi_dk0 = tfel::math::st2tost2<N, compliance>::zero();
    auto sum_dAi_dmu0 = tfel::math::st2tost2<N, compliance>::zero();
    if (with_Chom_derivatives && (N == 3)) {
      dChom_dkr.push_back(3 * f0 * J * A0);
      dChom_dmur.push_back(2 * f0 * K * A0);
    }

    const auto u = real(1.);
    const auto z = real(0.);
    const std::array<real, 4> dk0 = {u, z, z, z};
    const std::array<real, 4> dmu0 = {z, u, z, z};
    const std::array<real, 4> dki = {z, z, u, z};
    const std::array<real, 4> dmui = {z, z, z, u};

    for (std::size_t i = 0; i < np - 1; i++) {
      auto phasei = micro.getInclusionPhase(i);
      auto Ci = (*phasei).getElasticityOfPhase();
      auto fi = (*phasei).fraction;
      auto taui = polarisations_[i + 1];
      localisators[i + 1] = localisators[i + 1] * A0;
      auto Ai = localisators[i + 1];
      Chom += fi * (Ci - C0) * Ai;
      tau_eff += fi * transpose(Ai) * (taui - tau0);
    }

    if (with_Chom_derivatives && (N == 3)) {
      for (std::size_t i = 0; i < np - 1; i++) {
        auto phasei = micro.getInclusionPhase(i);
        auto Ci = (*phasei).getElasticityOfPhase();
        auto fi = (*phasei).fraction;
        auto Ai = localisators[i + 1];
        if (not((*phasei).isIsotropic())) {
          tfel::reportContractViolation(
              "I cannot compute the derivatives of the homogenized stiffness "
              "when the elasticity of a phase is a st2tost2 object."
              "Try to use a IsotropicModuli for the elasticity of this phase.");
        }
        const auto dChom_dki = 3 * fi * J * Ai;
        const auto dChom_dmui = 2 * fi * K * Ai;
        dChom_dkr.push_back(dChom_dki);
        dChom_dmur.push_back(dChom_dmui);

        const auto dAi_dk0 =
            (*phasei).computeDerivativesOfMeanLocalisator(KG0, dk0);
        const auto dAi_dmu0 =
            (*phasei).computeDerivativesOfMeanLocalisator(KG0, dmu0);
        const auto dAi_dki =
            (*phasei).computeDerivativesOfMeanLocalisator(KG0, dki);
        const auto dAi_dmui =
            (*phasei).computeDerivativesOfMeanLocalisator(KG0, dmui);
        sum_dAi_dk0 += fi * dAi_dk0;
        sum_dAi_dmu0 += fi * dAi_dmu0;
        dChom_dkr[i + 1] -= f0 * fi * C0 * A0 * dAi_dki * A0;
        dChom_dmur[i + 1] -= f0 * fi * C0 * A0 * dAi_dmui * A0;
        dChom_dkr[i + 1] += fi * Ci * dAi_dki * A0;
        dChom_dmur[i + 1] += fi * Ci * dAi_dmui * A0;
        dChom_dkr[i + 1] -= fi * (Chom - f0 * C0 * A0) * dAi_dki * A0;
        dChom_dmur[i + 1] -= fi * (Chom - f0 * C0 * A0) * dAi_dmui * A0;

        dChom_dkr[0] += fi * Ci * dAi_dk0 * A0;
        dChom_dmur[0] += fi * Ci * dAi_dmu0 * A0;
      }
      dChom_dkr[0] -= f0 * C0 * A0 * sum_dAi_dk0 * A0;
      dChom_dmur[0] -= f0 * C0 * A0 * sum_dAi_dmu0 * A0;
      for (std::size_t i = 0; i < np - 1; i++) {
        auto phasei = micro.getInclusionPhase(i);
        auto Ci = (*phasei).getElasticityOfPhase();
        auto fi = (*phasei).fraction;
        auto Ai = localisators[i + 1];
        dChom_dkr[0] -= fi * Ci * Ai * sum_dAi_dk0 * A0;
        dChom_dmur[0] -= fi * Ci * Ai * sum_dAi_dmu0 * A0;
      }
    }

    HomogenizationScheme<N, StressType> h_s = {
        .homogenized_stiffness = Chom,
        .effective_polarisation = tau_eff,
        .mean_strain_localisation_tensors = localisators,
        .derivative_of_homogenized_stiffness_wrt_kr = dChom_dkr,
        .derivative_of_homogenized_stiffness_wrt_mur = dChom_dmur};

    return h_s;
  };  // end of computeMoriTanaka

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeAsymmetricSelfConsistent(
          ParticulateMicrostructure<N, StressType>& micro,
          const tfel::types::real<StressType>& tolerance,
          bool isotropic,
          int max_iter_anisotropic_integration,
          const std::vector<tfel::math::stensor<N, StressType>>& polarisations,
          bool with_Chom_derivatives) {
    using real = tfel::types::real<StressType>;
    using compliance = tfel::types::compliance<StressType>;

    const auto np = micro.getNumberOfPhases();
    const auto f0 = micro.getMatrixFraction();
    const auto C0 = micro.getMatrixElasticity();

    const auto polarisations_ =
        internals::initialize_polarisation<N, StressType>(polarisations, np);
    auto tau_eff = tfel::math::stensor<N, StressType>::zero();
    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    const auto J = tfel::math::st2tost2<3u, real>::J();
    const auto K = tfel::math::st2tost2<3u, real>::K();
    std::vector<tfel::math::st2tost2<N, real>> dChom_dkr = {};
    std::vector<tfel::math::st2tost2<N, real>> dChom_dmur = {};

    if (with_Chom_derivatives && (not(micro.is_isotropic_matrix()))) {
      tfel::reportContractViolation(
          "I cannot compute the derivatives of the homogenized stiffness "
          "when the matrix elasticity is a st2tost2 object."
          "Try to use a IsotropicModuli for the definition of the matrix "
          "elasticity.");
    }

    if (with_Chom_derivatives && (not(isotropic))) {
      tfel::reportContractViolation(
          "I cannot compute the derivatives of the homogenized stiffness "
          "when the homogenized stiffness is not projected on the isotropic "
          "basis."
          "Try to use isotropic=true.");
    }

    if (with_Chom_derivatives && (N == 3)) {
      std::vector<real> dkhom_dkr = {real(1)};
      std::vector<real> dkhom_dmur = {real(0)};
      std::vector<real> dmuhom_dkr = {real(0)};
      std::vector<real> dmuhom_dmur = {real(1)};
      for (std::size_t i = 0; i < np - 1; i++) {
        dkhom_dkr.push_back(real(0));
        dkhom_dmur.push_back(real(0));
        dmuhom_dkr.push_back(real(0));
        dmuhom_dmur.push_back(real(0));
      }
    }

    const auto u = real(1.);
    const auto z = real(0.);
    const std::array<real, 4> dk0 = {u, z, z, z};
    const std::array<real, 4> dmu0 = {z, u, z, z};
    const std::array<real, 4> dki = {z, z, u, z};
    const std::array<real, 4> dmui = {z, z, z, u};

    auto Chom = C0;
    real rel_err = tolerance + 1;

    //beginning of while loop
    while (rel_err > tolerance) {
      std::vector<tfel::math::st2tost2<N, real>> localisators_ = {
          tfel::math::st2tost2<N, real>::Id()};

      for (std::size_t i = 0; i < np - 1; i++) {
        auto phasei = micro.getInclusionPhase(i);
        tfel::math::st2tost2<N, real> Ai;
        if (isotropic) {
          auto KGhom = computeKGModuli<StressType>(Chom);
          Ai = (*phasei).computeMeanLocalisator(KGhom);
        } else {
          Ai = (*phasei).computeMeanLocalisator(
              Chom, max_iter_anisotropic_integration);
        }
        localisators_.push_back(Ai);
      }

      tfel::math::st2tost2<N, StressType> Ch = C0;

      std::vector<tfel::math::st2tost2<N, real>> dChom_dkr_ = {3 * J};
      std::vector<tfel::math::st2tost2<N, real>> dChom_dmur_ = {2 * K};

      if (with_Chom_derivatives && (N == 3)) {
        for (std::size_t i = 0; i < np - 1; i++) {
          auto phasei = micro.getInclusionPhase(i);
          auto fi = (*phasei).fraction;
          auto Ai = localisators_[i + 1];
          dChom_dkr_.push_back(3 * fi * J * Ai);
          dChom_dmur_.push_back(2 * fi * K * Ai);
        }
      }

      for (std::size_t i = 0; i < np - 1; i++) {
        auto phasei = micro.getInclusionPhase(i);
        auto Ci = (*phasei).getElasticityOfPhase();
        auto fi = (*phasei).fraction;
        auto Ai = localisators_[i + 1];
        Ch += fi * (Ci - C0) * Ai;

        if (with_Chom_derivatives && (N == 3)) {
          if (not((*phasei).isIsotropic())) {
            tfel::reportContractViolation(
                "I cannot compute the derivatives of the homogenized stiffness "
                "when the elasticity of a phase is a st2tost2 object."
                "Try to use a IsotropicModuli for the elasticity of this "
                "phase.");
          }

          auto KGhom = computeKGModuli<StressType>(Chom);
          const auto dAi_dkhom =
              (*phasei).computeDerivativesOfMeanLocalisator(KGhom, dk0);
          const auto dAi_dmuhom =
              (*phasei).computeDerivativesOfMeanLocalisator(KGhom, dmu0);
          const auto dAi_dki =
              (*phasei).computeDerivativesOfMeanLocalisator(KGhom, dki);
          const auto dAi_dmui =
              (*phasei).computeDerivativesOfMeanLocalisator(KGhom, dmui);

          const auto dAi_dk0 =
              dkhom_dkr[0] * dAi_dkhom + dmuhom_dkr[0] * dAi_dmuhom;
          const auto dAi_dmu0 =
              dkhom_dmur[0] * dAi_dkhom + dmuhom_dmur[0] * dAi_dmuhom;
          const auto CdA_dk0 = (Ci - C0) * dAi_dk0;
          const auto CdA_dmu0 = (Ci - C0) * dAi_dmu0;
          dChom_dkr_[0] += fi * (-3 * J * Ai + CdA_dk0);
          dChom_dmur_[0] += fi * (-2 * K * Ai + CdA_dmu0);
          for (std::size_t j = 0; j < np - 1; j++) {
            tfel::math::st2tost2<N, compliance> dAi_dkj =
                dkhom_dkr[j + 1] * dAi_dkhom + dmuhom_dkr[j + 1] * dAi_dmuhom;
            tfel::math::st2tost2<N, compliance> dAi_dmuj =
                dkhom_dmur[j + 1] * dAi_dkhom + dmuhom_dmur[j + 1] * dAi_dmuhom;
            if (i == j) {
              dAi_dkj += dAi_dki;
              dAi_dmuj += dAi_dmui;
            }
            const auto CdA_dkj = (Ci - C0) * dAi_dkj;
            const auto CdA_dmuj = (Ci - C0) * dAi_dmuj;
            dChom_dkr_[j + 1] += fi * CdA_dkj;
            dChom_dmur_[j + 1] += fi * CdA_dmuj;
          }
        }
      }
      if (with_Chom_derivatives && (N == 3)) {
        for (std::size_t i = 0; i < np; i++) {
          const auto dKGhom_dkr =
              computeKGModuli<StressType>(StressType(1) * dChom_dkr_[i]);
          const auto dKGhom_dmur =
              computeKGModuli<StressType>(StressType(1) * dChom_dmur_[i]);
          dkhom_dkr[i] = dKGhom_dkr.kappa / StressType(1);
          dkhom_dmur[i] = dKGhom_dmur.kappa / StressType(1);
          dmuhom_dkr[i] = dKGhom_dkr.mu / StressType(1);
          dmuhom_dmur[i] = dKGhom_dmur.mu / StressType(1);
        }
      }

      rel_err = relative_error(Chom, Ch);
      if (rel_err <= tolerance) {
        for (std::size_t i = 0; i < np; i++) {
          auto Ai = localisators_[i];
          localisators.push_back(Ai);
        }

        if (with_Chom_derivatives && (N == 3)) {
          for (std::size_t i = 0; i < np; i++) {
            dChom_dkr.push_back(dChom_dkr_[i]);
            dChom_dmur.push_back(dChom_dmur_[i]);
          }
        }
      }
      auto size = tfel::math::StensorDimeToSize<N>::value;
      for (std::size_t i = 0; i < size; i++)
        for (std::size_t j = 0; j < size; j++) {
          Chom(i, j) = Ch(i, j);
        }

    }  // end of while (rel_err>tolerance)

    const auto A0 = localisators[0];
    const auto tau0 = polarisations_[0];
    tau_eff += f0 * transpose(A0) * tau0;
    for (std::size_t i = 0; i < np - 1; i++) {
      auto taui = polarisations_[i + 1];
      auto phasei = micro.getInclusionPhase(i);
      auto fi = (*phasei).fraction;
      auto Ai = localisators[i + 1];
      tau_eff += fi * transpose(Ai) * taui;
    }
    HomogenizationScheme<N, StressType> h_s = {
        .homogenized_stiffness = Chom,
        .effective_polarisation = tau_eff,
        .mean_strain_localisation_tensors = localisators,
        .derivative_of_homogenized_stiffness_wrt_kr = dChom_dkr,
        .derivative_of_homogenized_stiffness_wrt_mur = dChom_dmur};
    return h_s;

  }  // end of computeAsymmetricSelfConsistent

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeSelfConsistent(
          Polycrystal<StressType>& poly,
          const tfel::types::real<StressType>& tolerance,
          const tfel::math::st2tost2<N, StressType>& Cini,
          bool isotropic,
          int max_iter_anisotropic_integration,
          const std::vector<tfel::math::stensor<N, StressType>>& polarisations,
          bool with_Chom_derivatives) {
    using real = tfel::types::real<StressType>;
    using compliance = tfel::types::compliance<StressType>;

    const auto ng = poly.getNumberOfGrains();
    const auto polarisations_ =
        internals::initialize_polarisation<N, StressType>(polarisations, ng);
    auto tau_eff = tfel::math::stensor<N, StressType>::zero();

    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    std::vector<tfel::math::st2tost2<N, real>> dChom_dkr = {};
    std::vector<tfel::math::st2tost2<N, real>> dChom_dmur = {};
    const auto J = tfel::math::st2tost2<3u, real>::J();
    const auto K = tfel::math::st2tost2<3u, real>::K();

    std::vector<real> dkhom_dkr = {};
    std::vector<real> dkhom_dmur = {};
    std::vector<real> dmuhom_dkr = {};
    std::vector<real> dmuhom_dmur = {};

    if (with_Chom_derivatives && (not(isotropic))) {
      tfel::reportContractViolation(
          "I cannot compute the derivatives of the homogenized stiffness "
          "when the homogenized stiffness is not projected on the isotropic "
          "basis."
          "Try to use isotropic=true.");
    }
    if (with_Chom_derivatives && (N == 3)) {
      for (std::size_t i = 0; i < ng; i++) {
        dkhom_dkr.push_back(real(0));
        dkhom_dmur.push_back(real(0));
        dmuhom_dkr.push_back(real(0));
        dmuhom_dmur.push_back(real(0));
      }
    }

    const auto u = real(1.);
    const auto z = real(0.);
    const std::array<real, 4> dk0 = {u, z, z, z};
    const std::array<real, 4> dmu0 = {z, u, z, z};
    const std::array<real, 4> dki = {z, z, u, z};
    const std::array<real, 4> dmui = {z, z, z, u};

    auto Chom = Cini;
    auto Chom_ = Cini;
    real rel_err = tolerance + 1;

    while (rel_err > tolerance) {
      std::vector<tfel::math::st2tost2<N, real>> localisators_try = {};
      tfel::math::st2tost2<N, real> Ai_mean =
          tfel::math::st2tost2<N, real>::zero();
      auto CDSC = tfel::math::st2tost2<N, StressType>::zero();
      for (std::size_t i = 0; i < ng; i++) {
        auto graini = poly.getGrain(i);
        auto fi = (*graini).fraction;
        tfel::math::st2tost2<N, real> Ai;
        if (isotropic) {
          auto KGhom = computeKGModuli<StressType>(Chom);
          Ai = (*graini).computeMeanLocalisator(KGhom);
        } else {
          Ai = (*graini).computeMeanLocalisator(
              Chom, max_iter_anisotropic_integration);
        }
        Ai_mean += fi * Ai;
        if (with_Chom_derivatives && (N == 3)) {
          auto Ci = (*graini).getElasticityOfPhase();
          CDSC += fi * Ci * Ai;
        }
        localisators_try.push_back(Ai);
      }
      Ai_mean = tfel::math::invert(Ai_mean);

      tfel::math::st2tost2<N, StressType> Ch =
          tfel::math::st2tost2<N, StressType>::zero();
      std::vector<tfel::math::st2tost2<N, real>> dChom_dkr_ = {};
      std::vector<tfel::math::st2tost2<N, real>> dChom_dmur_ = {};

      for (std::size_t i = 0; i < ng; i++) {
        auto graini = poly.getGrain(i);
        auto Ci = (*graini).getElasticityOfPhase();
        auto fi = (*graini).fraction;
        localisators_try[i] = localisators_try[i] * Ai_mean;
        auto Ai = localisators_try[i];
        Ch += fi * Ci * Ai;

        if (with_Chom_derivatives && (N == 3)) {
          dChom_dkr_.push_back(3 * fi * J * Ai);
          dChom_dmur_.push_back(2 * fi * K * Ai);
        }
      }

      if (with_Chom_derivatives && (N == 3)) {
        std::vector<tfel::math::st2tost2<N, compliance>> sum_dADk_dkj = {};
        std::vector<tfel::math::st2tost2<N, compliance>> sum_dADk_dmuj = {};

        std::vector<tfel::math::st2tost2<N, real>> sum_CdADk_dkj = {};
        std::vector<tfel::math::st2tost2<N, real>> sum_CdADk_dmuj = {};

        for (std::size_t j = 0; j < ng; j++) {
          sum_dADk_dkj.push_back(tfel::math::st2tost2<N, compliance>::zero());
          sum_dADk_dmuj.push_back(tfel::math::st2tost2<N, compliance>::zero());
          sum_CdADk_dkj.push_back(tfel::math::st2tost2<N, real>::zero());
          sum_CdADk_dmuj.push_back(tfel::math::st2tost2<N, real>::zero());
        }

        for (std::size_t k = 0; k < ng; k++) {
          auto graink = poly.getGrain(k);
          auto Ck = (*graink).getElasticityOfPhase();
          auto fk = (*graink).fraction;

          if (not((*graink).isIsotropic())) {
            tfel::reportContractViolation(
                "I cannot compute the derivatives of the homogenized stiffness "
                "when the elasticity of a grain is a st2tost2 object."
                "Try to use a IsotropicModuli for the elasticity of this "
                "grain.");
          }

          auto KGhom = computeKGModuli<StressType>(Chom);
          const auto dADk_dkhom =
              (*graink).computeDerivativesOfMeanLocalisator(KGhom, dk0);
          const auto dADk_dmuhom =
              (*graink).computeDerivativesOfMeanLocalisator(KGhom, dmu0);
          const auto dADk_dki =
              (*graink).computeDerivativesOfMeanLocalisator(KGhom, dki);
          const auto dADk_dmui =
              (*graink).computeDerivativesOfMeanLocalisator(KGhom, dmui);

          for (std::size_t j = 0; j < ng; j++) {
            tfel::math::st2tost2<N, compliance> dADk_dkj =
                dkhom_dkr[j] * dADk_dkhom + dmuhom_dkr[j] * dADk_dmuhom;
            tfel::math::st2tost2<N, compliance> dADk_dmuj =
                dkhom_dmur[j] * dADk_dkhom + dmuhom_dmur[j] * dADk_dmuhom;
            if (k == j) {
              dADk_dkj += dADk_dki;
              dADk_dmuj += dADk_dmui;
            }
            sum_dADk_dkj[j] += fk * dADk_dkj;
            sum_dADk_dmuj[j] += fk * dADk_dmuj;
            sum_CdADk_dkj[j] += fk * Ck * dADk_dkj;
            sum_CdADk_dmuj[j] += fk * Ck * dADk_dmuj;
          }
        }
        for (std::size_t j = 0; j < ng; j++) {
          dChom_dkr_[j] += sum_CdADk_dkj[j] * Ai_mean;
          dChom_dmur_[j] += sum_CdADk_dmuj[j] * Ai_mean;
          const auto AskjA = Ai_mean * sum_dADk_dkj[j] * Ai_mean;
          const auto AsmjA = Ai_mean * sum_dADk_dmuj[j] * Ai_mean;
          dChom_dkr_[j] -= CDSC * AskjA;
          dChom_dmur_[j] -= CDSC * AsmjA;
        }

        for (std::size_t i = 0; i < ng; i++) {
          const auto dKGhom_dkr =
              computeKGModuli<StressType>(StressType(1) * dChom_dkr_[i]);
          const auto dKGhom_dmur =
              computeKGModuli<StressType>(StressType(1) * dChom_dmur_[i]);
          dkhom_dkr[i] = dKGhom_dkr.kappa / StressType(1);
          dkhom_dmur[i] = dKGhom_dmur.kappa / StressType(1);
          dmuhom_dkr[i] = dKGhom_dkr.mu / StressType(1);
          dmuhom_dmur[i] = dKGhom_dmur.mu / StressType(1);
        }
      }

      auto size = tfel::math::StensorDimeToSize<N>::value;
      for (std::size_t i = 0; i < size; i++)
        for (std::size_t j = 0; j < size; j++) {
          Chom_(i, j) = Chom(i, j);
          Chom(i, j) = Ch(i, j);
        }
      rel_err = relative_error(Chom, Chom_);
      if (rel_err <= tolerance) {
        for (std::size_t i = 0; i < ng; i++) {
          auto Ai = localisators_try[i];
          localisators.push_back(Ai);
        }
        if (with_Chom_derivatives && (N == 3)) {
          for (std::size_t i = 0; i < ng; i++) {
            dChom_dkr.push_back(dChom_dkr_[i]);
            dChom_dmur.push_back(dChom_dmur_[i]);
          }
        }
      }
    }  // end of while (rel_err>tolerance)
    for (std::size_t i = 0; i < ng; i++) {
      auto taui = polarisations_[i];
      auto graini = poly.getGrain(i);
      auto fi = (*graini).fraction;
      auto Ai = localisators[i];
      tau_eff += fi * transpose(Ai) * taui;
    }

    HomogenizationScheme<N, StressType> h_s = {
        .homogenized_stiffness = Chom,
        .effective_polarisation = tau_eff,
        .mean_strain_localisation_tensors = localisators,
        .derivative_of_homogenized_stiffness_wrt_kr = dChom_dkr,
        .derivative_of_homogenized_stiffness_wrt_mur = dChom_dmur};
    return h_s;

  }  // end of computeSelfConsistent

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX */
