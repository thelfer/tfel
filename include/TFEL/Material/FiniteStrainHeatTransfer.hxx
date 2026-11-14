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

}  // end of namespace tfel::material

#include "TFEL/Material/FiniteStrainHeatTransfer.ixx"

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_HXX */
