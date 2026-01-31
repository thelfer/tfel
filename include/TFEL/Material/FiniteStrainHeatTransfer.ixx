/*!
 * \file   TFEL/Material/FiniteStrainHeatTransfer.ixx
 * \brief
 * \author Thomas Helfer
 * \date   30/01/2026
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX
#define LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX

#include "TFEL/Math/tmatrix.hxx"

namespace tfel::material {

  template <tfel::math::ScalarConcept ThermalConductivityType,
            tfel::math::TensorConcept DeformationGradientType>
  requires(
      (tfel::math::checkUnitCompatibility<tfel::math::unit::ThermalConductivity,
                                          ThermalConductivityType>()) &&
      (tfel::math::checkUnitCompatibility<tfel::math::unit::NoUnit,
                                          DeformationGradientType>()))  //
      TFEL_HOST_DEVICE
      constexpr auto computeThermalConductivyMatrixInReferenceFrame(
          const ThermalConductivityType k, const DeformationGradientType& F) {
    using namespace tfel::math;
    using real = promote_type<base_type<ThermalConductivityType>,
                              base_type<numeric_type<DeformationGradientType>>>;
    constexpr auto icste = Cste<real>::isqrt2;
    constexpr auto N = getSpaceDimension<DeformationGradientType>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    using ResultType = tmatrix<N, N, tfel::types::thermalconductivity<real>>;
    const auto C = computeRightCauchyGreenTensor(F);
    const auto iC = invert(C);
    const auto m = k * det(F) * iC;
    if constexpr (N==1){
      return ResultType{m[0]};

    } else if constexpr (N == 2) {
      return ResultType{m[0], m[3] * icste,  //
                        m[3] * icste, m[1]};
    } else {
      return ResultType{m[0],         m[3] * icste, m[4] * icste,  //
                        m[3] * icste, m[1],         m[5] * icste,
                        m[4] * icste, m[5] * icste, m[2]};
    }
  }  // end of computeThermalConductivyMatrix

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX */
