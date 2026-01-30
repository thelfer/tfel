/*!
 * \file   include/TFEL/Material/MicrostructureLinearHomogenization.hxx
 * \author Antoine Martin
 * \date   25 January 2025
 * \brief  This file implements some homogenization schemes for generic
 * microstructures. \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_HXX
#define LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_HXX

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
   * An internal function for initialization of the polarisation vector
   */
  namespace internals {
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                StressType>())
        std::vector<tfel::math::stensor<N, StressType>> initialize_polarisation(
            const std::vector<tfel::math::stensor<N, StressType>> &,
            const std::size_t);
  }  // namespace internals

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
          ParticulateMicrostructure<N, StressType> &,
          int max_iter_anisotropic_integration = 12,
          const std::vector<tfel::math::stensor<N, StressType>> & = {});

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
          ParticulateMicrostructure<N, StressType> &,
          int max_iter_anisotropic_integration = 12,
          const std::vector<tfel::math::stensor<N, StressType>> & = {});

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
          ParticulateMicrostructure<N, StressType> &,
          const tfel::types::real<StressType> &,
          bool isotropic,
          int max_iter_anisotropic_integration = 8,
          const std::vector<tfel::math::stensor<N, StressType>> & = {});

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/MicrostructureLinearHomogenization.ixx"

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_HXX */
