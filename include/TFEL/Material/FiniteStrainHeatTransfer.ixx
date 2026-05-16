/*!
 * \file   TFEL/Material/FiniteStrainHeatTransfer.ixx
 * \brief
 * \author Thomas Helfer
 * \date   30/01/2026
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX
#define LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Tensor/PiolaTransformation.hxx"

namespace tfel::material {

  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires((tfel::math::checkUnitCompatibility<
               tfel::math::unit::ThermalConductivity,
               ThermalConductivityOrDerivativeType>()) ||
           (tfel::math::checkUnitCompatibility<
               tfel::math::unit::generate_unit<1, 1, -3, 0, -2, 0, 0>,
               ThermalConductivityOrDerivativeType>()))  //
      TFEL_HOST_DEVICE constexpr auto                    //
      computeThermalConductivyMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType k,
          const DeformationGradientType& F) noexcept {
    using namespace tfel::math;
    using real = promote_type<base_type<ThermalConductivityOrDerivativeType>,
                              base_type<numeric_type<DeformationGradientType>>>;
    constexpr auto icste = Cste<real>::isqrt2;
    constexpr auto N = getSpaceDimension<DeformationGradientType>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    const auto C = computeRightCauchyGreenTensor(F);
    const auto iC = invert(C);
    const auto f = k * det(F);
    using ResultType = tmatrix<N, N, decltype(f * iC[0])>;
    if constexpr (N == 1) {
      return ResultType{f * iC[0]};
    } else if constexpr (N == 2) {
      return ResultType{f * iC[0], f * iC[3] * icste,  //
                        f * iC[3] * icste, f * iC[1]};
    } else {
      return ResultType{
          f * iC[0],         f * iC[3] * icste, f * iC[4] * icste,  //
          f * iC[3] * icste, f * iC[1],         f * iC[5] * icste,
          f * iC[4] * icste, f * iC[5] * icste, f * iC[2]};
    }
  }  // end of computeThermalConductivyMatrix

  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(((tfel::math::checkUnitCompatibility<
                tfel::math::unit::ThermalConductivity,
                ThermalConductivityOrDerivativeType>()) ||
            (tfel::math::checkUnitCompatibility<
                tfel::math::unit::generate_unit<1, 1, -3, 0, -2, 0, 0>,
                ThermalConductivityOrDerivativeType>())) &&
           (tfel::math::space_dimension<DeformationGradientType> == 2u))  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto                       //
      computeThermalConductivyMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType k1,
          const ThermalConductivityOrDerivativeType k2,
          const DeformationGradientType& F) noexcept {
    const auto iJ = 1 / det(F);
    const auto F2_2 = F[2] * F[2];
    const auto k00 = iJ * (F2_2 * F[3] * F[3] * k2 + F[1] * F[1] * F2_2 * k1);
    const auto k01 = iJ * (-F[0] * F2_2 * F[3] * k2 - F[1] * F2_2 * F[4] * k1);
    const auto k11 = iJ * (F[0] * F[0] * F2_2 * k2 + F2_2 * F[4] * F[4] * k1);
    return tfel::math::tmatrix<2, 2, decltype(k00)>{k00, k01,  //
                                                    k01, k11};
  }  // end of computeThermalConductivyMatrixInReferenceConfiguration

  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(((tfel::math::checkUnitCompatibility<
                tfel::math::unit::ThermalConductivity,
                ThermalConductivityOrDerivativeType>()) ||
            (tfel::math::checkUnitCompatibility<
                tfel::math::unit::generate_unit<1, 1, -3, 0, -2, 0, 0>,
                ThermalConductivityOrDerivativeType>())) &&
           (tfel::math::space_dimension<DeformationGradientType> == 3u))  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto                       //
      computeThermalConductivyMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType k1,
          const ThermalConductivityOrDerivativeType k2,
          const ThermalConductivityOrDerivativeType k3,
          const DeformationGradientType& F) noexcept {
    const auto iJ = 1 / det(F);
    const auto k00 =
        iJ * ((F[3] * F[3] * F[7] * F[7] - 2 * F[1] * F[3] * F[5] * F[7] +
               *F[1] * F[1] * F[5] * F[5]) *
                  k3 +
              (F[5] * F[5] * F[8] * F[8] - 2 * F[2] * F[3] * F[5] * F[8] +
               *F[2] * F[2] * F[3] * F[3]) *
                  k2 +
              (F[7] * F[7] * F[8] * F[8] - 2 * F[1] * F[2] * F[7] * F[8] +
               *F[1] * F[1] * F[2] * F[2]) *
                  k1);
    const auto k01 =
        iJ * ((-F[0] * F[3] * F[7] * F[7] +
               (F[3] * F[4] + *F[0] * F[1]) * F[5] * F[7] -
               F[1] * F[4] * F[5] * F[5]) *
                  k3 +  //
              ((F[0] * F[2] * F[5] - F[5] * F[5] * F[6]) * F[8] +
               *F[2] * F[3] * F[5] * F[6] - F[0] * F[2] * F[2] * F[3]) *
                  k2 +
              ((F[2] * F[4] * F[7] - F[6] * F[7] * F[7]) * F[8] +
               *F[1] * F[2] * F[6] * F[7] - F[1] * F[2] * F[2] * F[4]) *
                  k1);
    const auto k02 = iJ * (((F[0] * F[1] * F[3] - F[3] * F[3] * F[4]) * F[7] +
                            (F[1] * F[3] * F[4] - F[0] * F[1] * F[1]) * F[5]) *
                               k3 +
                           (-F[0] * F[5] * F[8] * F[8] +
                            (F[3] * F[5] * F[6] + *F[0] * F[2] * F[3]) * F[8] -
                            F[2] * F[3] * F[3] * F[6]) *
                               k2 +
                           (-F[4] * F[7] * F[8] * F[8] +
                            (F[1] * F[6] * F[7] + *F[1] * F[2] * F[4]) * F[8] -
                            F[1] * F[1] * F[2] * F[6]) *
                               k1);
    const auto k11 =
        iJ * ((F[0] * F[0] * F[7] * F[7] - 2 * F[0] * F[4] * F[5] * F[7] +
               *F[4] * F[4] * F[5] * F[5]) *
                  k3 +
              (F[5] * F[5] * F[6] * F[6] - 2 * F[0] * F[2] * F[5] * F[6] +
               *F[0] * F[0] * F[2] * F[2]) *
                  k2 +
              (F[6] * F[6] * F[7] * F[7] - 2 * F[2] * F[4] * F[6] * F[7] +
               *F[2] * F[2] * F[4] * F[4]) *
                  k1);
    const auto k12 =
        iJ * (((F[0] * F[3] * F[4] - F[0] * F[0] * F[1]) * F[7] +
               (F[0] * F[1] * F[4] - F[3] * F[4] * F[4]) * F[5]) *
                  k3 +
              ((F[0] * F[5] * F[6] - F[0] * F[0] * F[2]) * F[8] -
               F[3] * F[5] * F[6] * F[6] + *F[0] * F[2] * F[3] * F[6]) *
                  k2 +
              ((F[4] * F[6] * F[7] - F[2] * F[4] * F[4]) * F[8] -
               F[1] * F[6] * F[6] * F[7] + *F[1] * F[2] * F[4] * F[6]) *
                  k1);
    const auto k22 =
        iJ * ((F[3] * F[3] * F[4] * F[4] - 2 * F[0] * F[1] * F[3] * F[4] +
               *F[0] * F[0] * F[1] * F[1]) *
                  k3 +
              (F[0] * F[0] * F[8] * F[8] - 2 * F[0] * F[3] * F[6] * F[8] +
               *F[3] * F[3] * F[6] * F[6]) *
                  k2 +
              (F[4] * F[4] * F[8] * F[8] - 2 * F[1] * F[4] * F[6] * F[8] +
               *F[1] * F[1] * F[6] * F[6]) *
                  k1);
    return tfel::math::tmatrix<3, 3, decltype(k00)>{k00, k01, k02,  //
                                                    k01, k11, k12,  //
                                                    k02, k12, k22};
  }  // end of computeThermalConductivyMatrixInReferenceConfiguration

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX */
