/*!
 * \file   TFEL/Material/FiniteStrainHeatTransfer.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/01/2026
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
   * \param[in] k: thermal conductivity
   * \param[in] F: deformation gradient
   */
  template <tfel::math::ScalarConcept ThermalConductivityType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::ThermalConductivity,
                                         ThermalConductivityType>())  //
      TFEL_HOST_DEVICE
      constexpr auto computeThermalConductivyMatrixInReferenceConfiguration(
          const ThermalConductivityType,
          const DeformationGradientType&) noexcept;

}  // end of namespace tfel::material

#include "TFEL/Material/FiniteStrainHeatTransfer.ixx"

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_HXX */
