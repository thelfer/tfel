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

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/MicrostructureDescription.hxx"
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"

namespace tfel::material::homogenization::elasticity {

  /*!
   * The struct HomogenizationScheme is a struct whose attributes are
   * various objects that are useful in homogenization
   * theory, as homogenized stiffness, mean strain localisation tensors,
   * effective_polarisation...
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) struct HomogenizationScheme {
    using real = tfel::types::real<StressType>;

    tfel::math::st2tost2<N, StressType> homogenized_stiffness;
    tfel::math::stensor<N, StressType> effective_polarisation;
    std::vector<tfel::math::st2tost2<N, real>> mean_strain_localisation_tensors;
  };

  /*!
   * Here is the Dilute scheme which returns an object of
   * type HomogenizationScheme from a ParticulateMicrostructure.
   * \tparam unsigned short int: dimension
   * \tparam StressType: type of the elastic constants related to
   * the inclusion
   * \return an object of type HomogenizationScheme
   * \param[in] micro: ParticulateMicrostructure
   */
  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeDilute(
          ParticulateMicrostructure<N, StressType> &micro,
          const std::vector<tfel::math::stensor<N, StressType>> &polarisations,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 12) {
    using real = tfel::types::real<StressType>;
    using compliance = tfel::types::compliance<StressType>;

    const auto np = micro.get_number_of_phases();
    if (polarisations.size() != np) {
      tfel::reportContractViolation(
          "the size of your polarization vector does not correspond to the "
          "number of phases");
    }
    const auto C0 = micro.get_matrix_elasticity();
    const auto tau0 = polarisations[0];
    HomogenizationScheme<N, StressType> h_s;
    auto Chom = C0;
    auto tau_eff = tau0;

    std::vector<tfel::math::st2tost2<N, real>> localisators = {
        tfel::math::st2tost2<N, real>::Id()};
    std::vector<tfel::math::st2tost2<N, compliance>> hill_tensors = {};
    
    for (unsigned int i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto Ci = (*phasei).stiffness;
      auto fi = (*phasei).fraction;
      auto taui = polarisations[i + 1];
      auto Ai = (*phasei).computeMeanLocalisator(
          C0, isotropic_matrix,verbose, max_iter_anisotropic_integration);
      Chom += fi * (Ci - C0) * Ai;
      tau_eff += fi * tfel::math::transpose(Ai) * (taui - tau0);
      localisators.push_back(Ai);
    }
    h_s.homogenized_stiffness = Chom;
    h_s.effective_polarisation = tau_eff;
    h_s.mean_strain_localisation_tensors = localisators;

    return h_s;
  };

  /*!
   * Here is the MoriTanaka scheme which returns an object of
   * type HomogenizationScheme from a ParticulateMicrostructure.
   * \tparam unsigned short int: dimension
   * \tparam StressType: type of the elastic constants related to
   * the inclusion
   * \return an object of type HomogenizationScheme
   */
  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeMoriTanaka(
          ParticulateMicrostructure<N, StressType> &micro,
          const std::vector<tfel::math::stensor<N, StressType>> &polarisations,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 12) {
    using real = tfel::types::real<StressType>;
    using compliance = tfel::types::compliance<StressType>;

    const auto np = micro.get_number_of_phases();
    if (polarisations.size() != np) {
      tfel::reportContractViolation(
          "the size of your polarization vector does not correspond to the "
          "number of phases");
    }
    const auto C0 = micro.get_matrix_elasticity();
    const auto tau0 = polarisations[0];
    const auto f0 = micro.get_matrix_fraction();
    HomogenizationScheme<N, StressType> h_s;
    auto Chom = C0;
    auto tau_eff = tau0;

    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    std::vector<tfel::math::st2tost2<N, compliance>> hill_tensors = {};
    tfel::math::st2tost2<N, real> A0 = f0 * tfel::math::st2tost2<N, real>::Id();
    for (unsigned int i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto fi = (*phasei).fraction;
      auto Ai = (*phasei).computeMeanLocalisator(
          C0, isotropic_matrix,verbose, max_iter_anisotropic_integration);
      localisators.push_back(Ai);
      A0 += fi * Ai;
    }
    A0 = invert(A0);
    localisators.insert(localisators.begin(), A0);
    for (unsigned int i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto Ci = (*phasei).stiffness;
      auto fi = (*phasei).fraction;
      auto taui = polarisations[i + 1];
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

  /*!
   * Here is the Self-consistent scheme which returns an object of
   * type HomogenizationScheme from a ParticulateMicrostructure.
   * \tparam unsigned short int: dimension
   * \tparam StressType: type of the elastic constants related to
   * the inclusion
   * \return an object of type HomogenizationScheme
   */

  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      HomogenizationScheme<N, StressType> computeSelfConsistent(
          ParticulateMicrostructure<N, StressType> &micro,
          const std::vector<tfel::math::stensor<N, StressType>> &polarisations,
          int max_iter,
          bool isotropic_matrix = true,
          bool verbose = true,
          int max_iter_anisotropic_integration = 8) {
    using real = tfel::types::real<StressType>;
    real error = 1.;
    const auto np = micro.get_number_of_phases();
    const auto f0 = micro.get_matrix_fraction();
    const auto C0 = micro.get_matrix_elasticity();
    HomogenizationScheme<N, StressType> h_s;
    const auto tau0 = polarisations[0];
    auto tau_eff=tau0;
    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    auto Chom = C0;
    auto Chom_ = C0;
    int iter = 0;
    while (iter < max_iter) {
      std::vector<tfel::math::st2tost2<N, real>> localisators_try = {};
      tfel::math::st2tost2<N, real> A0 = f0 * tfel::math::st2tost2<N, real>::Id();
      for (unsigned int i = 0; i < np - 1; i++) {
        auto phasei = micro.get_inclusionPhase(i);
        auto fi = (*phasei).fraction;
        auto Ai = (*phasei).computeMeanLocalisator(
            Chom, isotropic_matrix,verbose, max_iter_anisotropic_integration);
        localisators_try.push_back(Ai);
        A0 += fi * Ai;
      }
      A0 = invert(A0);
      localisators_try.insert(localisators_try.begin(), A0);
      tfel::math::st2tost2<N, StressType> Ch = f0*C0*A0;
      for (unsigned int i = 0; i < np -1 ; i++) {
        auto phasei = micro.get_inclusionPhase(i);
        auto Ci = (*phasei).stiffness;
        auto fi = (*phasei).fraction;
        localisators_try[i] = localisators_try[i] * A0;
        auto Ai = localisators_try[i];
        Ch += fi * Ci * Ai;
      }
      auto size = tfel::math::StensorDimeToSize<N>::value;
      for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
          Chom_(i, j) = Chom(i, j);
          Chom(i, j) = Ch(i, j);
        }
      error = tfel::material::relative_error<N, StressType>(Chom_, Chom);
      //if (verbose) {
      //  std::cout << "iteration " << iter << " "
      //      << "relative Delta Chom: "
      //      << error << std::endl;
      //  auto C11 = Chom(0, 0)/StressType(1);
      //  std::cout << "actual Chom_11: " << C11 << std::endl;
      //}
      iter++;
    }
    h_s.homogenized_stiffness = Chom;
    h_s.effective_polarisation = tau_eff; //////TO DO ///////////
    h_s.mean_strain_localisation_tensors = localisators;
    
    return h_s;
  }

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX */
