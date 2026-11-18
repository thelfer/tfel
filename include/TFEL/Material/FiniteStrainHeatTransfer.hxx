/*!
 * \file   TFEL/Material/FiniteStrainHeatTransfer.hxx
 * \brief  This file declares functions computing the thermal conductivity
 * matrix in the reference configuration.
 * \author Thomas Helfer
 * \date   30/01/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_HXX
#define LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_HXX

#include "TFEL/Math/types.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/tensor.hxx"

namespace tfel::material {

  /*!
   * \brief compute the thermal conductivity matrix in the reference
   * configuration described by the isotropic Fourier law in the current
   * configuration.
   *
   * \param[in] k: thermal conductivity
   * \param[in] F: deformation gradient
   *
   * \note This function can also compute the derivative of the the thermal
   * conductivity matrix with respect to the temperature.
   * \note This function is also suitable for orthotropic materials in 1D
   */
  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires((tfel::math::checkUnitCompatibility<
               tfel::math::unit::ThermalConductivity,
               ThermalConductivityOrDerivativeType>()) ||
           (tfel::math::checkUnitCompatibility<
               tfel::math::unit::StandardUnit<1, 1, -3, 0, -2, 0, 0>,
               ThermalConductivityOrDerivativeType>()))  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto      //
      computeThermalConductivyMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType,
          const DeformationGradientType&) noexcept;
  /*!
   * \brief compute the thermal conductivity matrix in the reference
   * configuration described by the orthotropic Fourier law in the current
   * configuration.
   *
   * \param[in] k1: thermal conductivity in the first direction
   * \param[in] k2: thermal conductivity in the second direction
   * \param[in] F: deformation gradient
   *
   * \note This function can also compute the derivative of the the thermal
   * conductivity matrix with respect to the temperature.
   */
  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(((tfel::math::checkUnitCompatibility<
                tfel::math::unit::ThermalConductivity,
                ThermalConductivityOrDerivativeType>()) ||
            (tfel::math::checkUnitCompatibility<
                tfel::math::unit::StandardUnit<1, 1, -3, 0, -2, 0, 0>,
                ThermalConductivityOrDerivativeType>())) &&
           (tfel::math::space_dimension<DeformationGradientType> == 2u))  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto                       //
      computeThermalConductivyMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType,
          const ThermalConductivityOrDerivativeType,
          const DeformationGradientType&) noexcept;
  /*!
   * \brief compute the thermal conductivity matrix in the reference
   * configuration described by the orthotropic Fourier law in the current
   * configuration.
   *
   * \param[in] k1: thermal conductivity in the first direction
   * \param[in] k2: thermal conductivity in the second direction
   * \param[in] k3: thermal conductivity in the third direction
   * \param[in] F: deformation gradient
   *
   * \note This function can also compute the derivative of the the thermal
   * conductivity matrix with respect to the temperature.
   */
  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(((tfel::math::checkUnitCompatibility<
                tfel::math::unit::ThermalConductivity,
                ThermalConductivityOrDerivativeType>()) ||
            (tfel::math::checkUnitCompatibility<
                tfel::math::unit::StandardUnit<1, 1, -3, 0, -2, 0, 0>,
                ThermalConductivityOrDerivativeType>())) &&
           (tfel::math::space_dimension<DeformationGradientType> == 3u))  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto                       //
      computeThermalConductivyMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType,
          const ThermalConductivityOrDerivativeType,
          const ThermalConductivityOrDerivativeType,
          const DeformationGradientType&) noexcept;
  /*!
   * \brief compute the heat flux \f$Q=-K\,\nabla T\f$ in the reference
   * configuration and its derivatives with respect to the temperature
   * gradient and to the deformation gradient. Those derivatives are the
   * ones needed by strongly coupled resolutions.
   *
   * \return a tuple made of \f$Q\f$, its derivative with respect to
   * \f$\nabla T\f$, i.e. \f$-K\f$, and its derivative with respect to
   * \f$F\f$
   *
   * \param[in] K: thermal conductivity matrix in the reference
   * configuration
   * \param[in] gT: temperature gradient in the reference configuration
   * \param[in] F: deformation gradient
   *
   * \note the derivative with respect to \f$F\f$ assumes that \f$K\f$ is
   * the Piola transform \f$J\,F^{-1}\,k\,F^{-T}\f$ of a thermal
   * conductivity matrix \f$k\f$ of the current configuration which does
   * not depend on \f$F\f$ and, in 1D and 2D, for which the out-of-plane
   * direction is a principal direction. This is the case of the matrices
   * returned by the `computeThermalConductivyMatrixInReferenceConfiguration`
   * functions.
   */
  template <unsigned short N,
            typename ThermalConductivityType,
            typename TemperatureGradientType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(tfel::math::getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE
      [[nodiscard]] constexpr auto computeHeatFluxInReferenceConfigurationAndDerivatives(
          const tfel::math::tmatrix<N, N, ThermalConductivityType>&,
          const tfel::math::tvector<N, TemperatureGradientType>&,
          const DeformationGradientType&) noexcept;

}  // end of namespace tfel::material

#include "TFEL/Material/FiniteStrainHeatTransfer.ixx"

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_HXX */
