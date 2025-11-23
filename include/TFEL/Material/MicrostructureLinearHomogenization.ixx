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
            const std::vector<tfel::math::stensor<N, StressType>>
                &polarisations,
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
          ParticulateMicrostructure<N, StressType> &micro,
          int max_iter_anisotropic_integration,
          const std::vector<tfel::math::stensor<N, StressType>>
              &polarisations) {
    using real = tfel::types::real<StressType>;
    const auto np = micro.get_number_of_phases();
    const auto polarisations_ =
        internals::initialize_polarisation<N, StressType>(polarisations, np);

    const auto C0 = micro.get_matrix_elasticity();
    KGModuli<StressType> KG0(StressType(0), StressType(0));
    if (micro.is_isotropic_matrix()) {
      KG0 = computeKGModuli<StressType>(C0);
    }
    const auto tau0 = polarisations_[0];
    HomogenizationScheme<N, StressType> h_s;
    auto Chom = C0;
    auto tau_eff = tau0;

    std::vector<tfel::math::st2tost2<N, real>> localisators = {
        tfel::math::st2tost2<N, real>::Id()};

    for (std::size_t i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto Ci = (*phasei).stiffness;
      auto fi = (*phasei).fraction;
      auto taui = polarisations_[i + 1];
      tfel::math::st2tost2<N, real> Ai;
      if (micro.is_isotropic_matrix()) {
        Ai = (*phasei).computeMeanLocalisator(KG0);
      } else {
        Ai = (*phasei).computeMeanLocalisator(C0,
                                              max_iter_anisotropic_integration);
      }
      Chom += fi * (Ci - C0) * Ai;
      tau_eff += fi * tfel::math::transpose(Ai) * (taui - tau0);
      localisators.push_back(Ai);
    }
    h_s.homogenized_stiffness = Chom;
    h_s.effective_polarisation = tau_eff;
    h_s.mean_strain_localisation_tensors = localisators;

    return h_s;
  };

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeMoriTanaka(
          ParticulateMicrostructure<N, StressType> &micro,
          int max_iter_anisotropic_integration,
          const std::vector<tfel::math::stensor<N, StressType>>
              &polarisations) {
    using real = tfel::types::real<StressType>;

    const auto np = micro.get_number_of_phases();
    const auto polarisations_ =
        internals::initialize_polarisation<N, StressType>(polarisations, np);

    const auto C0 = micro.get_matrix_elasticity();
    KGModuli<StressType> KG0(StressType(0), StressType(0));
    if (micro.is_isotropic_matrix()) {
      KG0 = computeKGModuli<StressType>(C0);
    }
    const auto tau0 = polarisations_[0];
    const auto f0 = micro.get_matrix_fraction();
    HomogenizationScheme<N, StressType> h_s;
    auto Chom = C0;
    auto tau_eff = tau0;
    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    tfel::math::st2tost2<N, real> A0 = f0 * tfel::math::st2tost2<N, real>::Id();
    for (std::size_t i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto fi = (*phasei).fraction;
      tfel::math::st2tost2<N, real> Ai;
      if (micro.is_isotropic_matrix()) {
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
    for (std::size_t i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto Ci = (*phasei).stiffness;
      auto fi = (*phasei).fraction;
      auto taui = polarisations_[i + 1];
      localisators[i + 1] = localisators[i + 1] * A0;
      auto Ai = localisators[i + 1];
      Chom += fi * (Ci - C0) * Ai;
      tau_eff += fi * transpose(Ai) * (taui - tau0);
    }
    h_s.homogenized_stiffness = Chom;
    h_s.effective_polarisation = tau_eff;
    h_s.mean_strain_localisation_tensors = localisators;

    return h_s;
  };

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeSelfConsistent(
          ParticulateMicrostructure<N, StressType> &micro,
          int max_iter,
          bool isotropic,
          int max_iter_anisotropic_integration,
          const std::vector<tfel::math::stensor<N, StressType>>
              &polarisations) {
    using real = tfel::types::real<StressType>;
    const auto np = micro.get_number_of_phases();
    const auto f0 = micro.get_matrix_fraction();
    const auto C0 = micro.get_matrix_elasticity();

    const auto polarisations_ =
        internals::initialize_polarisation<N, StressType>(polarisations, np);

    HomogenizationScheme<N, StressType> h_s;
    auto tau_eff = tfel::math::stensor<N, StressType>::zero();
    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    auto Chom = C0;
    auto Chom_ = C0;
    int iter = 0;
    while (iter < max_iter) {
      std::vector<tfel::math::st2tost2<N, real>> localisators_try = {};
      tfel::math::st2tost2<N, real> A0 =
          f0 * tfel::math::st2tost2<N, real>::Id();
      for (std::size_t i = 0; i < np - 1; i++) {
        auto phasei = micro.get_inclusionPhase(i);
        auto fi = (*phasei).fraction;
        tfel::math::st2tost2<N, real> Ai;
        if (isotropic) {
          auto KGhom = computeKGModuli<StressType>(Chom);
          Ai = (*phasei).computeMeanLocalisator(KGhom);
        } else {
          Ai = (*phasei).computeMeanLocalisator(
              Chom, max_iter_anisotropic_integration);
        }
        localisators_try.push_back(Ai);
        A0 += fi * Ai;
      }
      A0 = invert(A0);
      localisators_try.insert(localisators_try.begin(), A0);
      tfel::math::st2tost2<N, StressType> Ch = f0 * C0 * A0;
      for (std::size_t i = 0; i < np - 1; i++) {
        auto phasei = micro.get_inclusionPhase(i);
        auto Ci = (*phasei).stiffness;
        auto fi = (*phasei).fraction;
        localisators_try[i + 1] = localisators_try[i + 1] * A0;
        auto Ai = localisators_try[i + 1];
        Ch += fi * Ci * Ai;
      }
      auto size = tfel::math::StensorDimeToSize<N>::value;
      for (std::size_t i = 0; i < size; i++)
        for (std::size_t j = 0; j < size; j++) {
          Chom_(i, j) = Chom(i, j);
          Chom(i, j) = Ch(i, j);
        }
      iter++;
      if (iter >= max_iter) {
        for (std::size_t i = 0; i < np; i++) {
          auto Ai = localisators_try[i];
          localisators.push_back(Ai);
        }
      }
    }
    const auto A0 = localisators[0];
    const auto tau0 = polarisations_[0];
    tau_eff += f0 * transpose(A0) * tau0;
    for (std::size_t i = 0; i < np - 1; i++) {
      auto taui = polarisations_[i + 1];
      auto phasei = micro.get_inclusionPhase(i);
      auto fi = (*phasei).fraction;
      auto Ai = localisators[i + 1];
      tau_eff += fi * transpose(Ai) * taui;
    }
    h_s.homogenized_stiffness = Chom;
    h_s.effective_polarisation = tau_eff;
    h_s.mean_strain_localisation_tensors = localisators;
    return h_s;
  }

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX */
