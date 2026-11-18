/*!
 * \file   TFEL/Material/FiniteStrainHeatTransfer.ixx
 * \brief  This file implements functions computing the thermal
 * conductivity matrix in the reference configuration.
 * \author Thomas Helfer
 * \date   30/01/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX
#define LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX

#include <type_traits>
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Tensor/PiolaTransformation.hxx"

namespace tfel::material {

  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(ThermalConductivityOrDerivativeConcept<
           ThermalConductivityOrDerivativeType>)  //
      TFEL_HOST_DEVICE constexpr auto             //
      computeThermalConductivityMatrixInReferenceConfiguration(
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
  }  // end of computeThermalConductivityMatrixInReferenceConfiguration

  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(ThermalConductivityOrDerivativeConcept<
               ThermalConductivityOrDerivativeType> &&
           (tfel::math::space_dimension<DeformationGradientType> == 2u))  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto                       //
      computeThermalConductivityMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType k1,
          const ThermalConductivityOrDerivativeType k2,
          const DeformationGradientType& F) noexcept {
    const auto iJ = 1 / det(F);
    const auto F2_2 = F[2] * F[2];
    const auto k00 = iJ * (F2_2 * F[3] * F[3] * k2 + F[1] * F[1] * F2_2 * k1);
    const auto k01 = iJ * (-F[0] * F2_2 * F[3] * k2 - F[1] * F2_2 * F[4] * k1);
    const auto k11 = iJ * (F[0] * F[0] * F2_2 * k2 + F2_2 * F[4] * F[4] * k1);
    using ResultType = tfel::math::tmatrix<2, 2, std::decay_t<decltype(k00)>>;
    return ResultType{k00, k01,  //
                      k01, k11};
  }  // end of computeThermalConductivityMatrixInReferenceConfiguration

  template <tfel::math::ScalarConcept ThermalConductivityOrDerivativeType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(ThermalConductivityOrDerivativeConcept<
               ThermalConductivityOrDerivativeType> &&
           (tfel::math::space_dimension<DeformationGradientType> == 3u))  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto                       //
      computeThermalConductivityMatrixInReferenceConfiguration(
          const ThermalConductivityOrDerivativeType k1,
          const ThermalConductivityOrDerivativeType k2,
          const ThermalConductivityOrDerivativeType k3,
          const DeformationGradientType& F) noexcept {
    const auto iJ = 1 / det(F);
    const auto k00 =
        iJ * ((F[3] * F[3] * F[7] * F[7] - 2 * F[1] * F[3] * F[5] * F[7] +
               F[1] * F[1] * F[5] * F[5]) *
                  k3 +
              (F[5] * F[5] * F[8] * F[8] - 2 * F[2] * F[3] * F[5] * F[8] +
               F[2] * F[2] * F[3] * F[3]) *
                  k2 +
              (F[7] * F[7] * F[8] * F[8] - 2 * F[1] * F[2] * F[7] * F[8] +
               F[1] * F[1] * F[2] * F[2]) *
                  k1);
    const auto k01 =
        iJ * ((-F[0] * F[3] * F[7] * F[7] +
               (F[3] * F[4] + F[0] * F[1]) * F[5] * F[7] -
               F[1] * F[4] * F[5] * F[5]) *
                  k3 +  //
              ((F[0] * F[2] * F[5] - F[5] * F[5] * F[6]) * F[8] +
               F[2] * F[3] * F[5] * F[6] - F[0] * F[2] * F[2] * F[3]) *
                  k2 +
              ((F[2] * F[4] * F[7] - F[6] * F[7] * F[7]) * F[8] +
               F[1] * F[2] * F[6] * F[7] - F[1] * F[2] * F[2] * F[4]) *
                  k1);
    const auto k02 = iJ * (((F[0] * F[1] * F[3] - F[3] * F[3] * F[4]) * F[7] +
                            (F[1] * F[3] * F[4] - F[0] * F[1] * F[1]) * F[5]) *
                               k3 +
                           (-F[0] * F[5] * F[8] * F[8] +
                            (F[3] * F[5] * F[6] + F[0] * F[2] * F[3]) * F[8] -
                            F[2] * F[3] * F[3] * F[6]) *
                               k2 +
                           (-F[4] * F[7] * F[8] * F[8] +
                            (F[1] * F[6] * F[7] + F[1] * F[2] * F[4]) * F[8] -
                            F[1] * F[1] * F[2] * F[6]) *
                               k1);
    const auto k11 =
        iJ * ((F[0] * F[0] * F[7] * F[7] - 2 * F[0] * F[4] * F[5] * F[7] +
               F[4] * F[4] * F[5] * F[5]) *
                  k3 +
              (F[5] * F[5] * F[6] * F[6] - 2 * F[0] * F[2] * F[5] * F[6] +
               F[0] * F[0] * F[2] * F[2]) *
                  k2 +
              (F[6] * F[6] * F[7] * F[7] - 2 * F[2] * F[4] * F[6] * F[7] +
               F[2] * F[2] * F[4] * F[4]) *
                  k1);
    const auto k12 =
        iJ * (((F[0] * F[3] * F[4] - F[0] * F[0] * F[1]) * F[7] +
               (F[0] * F[1] * F[4] - F[3] * F[4] * F[4]) * F[5]) *
                  k3 +
              ((F[0] * F[5] * F[6] - F[0] * F[0] * F[2]) * F[8] -
               F[3] * F[5] * F[6] * F[6] + F[0] * F[2] * F[3] * F[6]) *
                  k2 +
              ((F[4] * F[6] * F[7] - F[2] * F[4] * F[4]) * F[8] -
               F[1] * F[6] * F[6] * F[7] + F[1] * F[2] * F[4] * F[6]) *
                  k1);
    const auto k22 =
        iJ * ((F[3] * F[3] * F[4] * F[4] - 2 * F[0] * F[1] * F[3] * F[4] +
               F[0] * F[0] * F[1] * F[1]) *
                  k3 +
              (F[0] * F[0] * F[8] * F[8] - 2 * F[0] * F[3] * F[6] * F[8] +
               F[3] * F[3] * F[6] * F[6]) *
                  k2 +
              (F[4] * F[4] * F[8] * F[8] - 2 * F[1] * F[4] * F[6] * F[8] +
               F[1] * F[1] * F[6] * F[6]) *
                  k1);
    using ResultType = tfel::math::tmatrix<3, 3, std::decay_t<decltype(k00)>>;
    return ResultType{k00, k01, k02,  //
                      k01, k11, k12,  //
                      k02, k12, k22};
  }  // end of computeThermalConductivityMatrixInReferenceConfiguration

  template <unsigned short N,
            typename ThermalConductivityType,
            typename TemperatureGradientType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(tfel::math::getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeHeatFluxInReferenceConfigurationAndDerivatives(
          const tfel::math::tmatrix<N, N, ThermalConductivityType>& K,
          const tfel::math::tvector<N, TemperatureGradientType>& gT,
          const DeformationGradientType& F) noexcept {
    using namespace tfel::math;
    using namespace tfel::math::internals;
    static_assert((N == 1) || (N == 2) || (N == 3));
    using HeatFluxType =
        result_type<ThermalConductivityType, TemperatureGradientType, OpMult>;
    using VectorType = tvector<N, HeatFluxType>;
    using DerivativeType1 =
        derivative_type<VectorType, tvector<N, TemperatureGradientType>>;
    using DerivativeType2 =
        VectorDerivativeWithRespectToTensor<VectorType,
                                            DeformationGradientType>;
    const auto adj = computeAdjugateDeterminantDerivative(F);
    const auto J = det(F);
    const auto dQ_dgT = buildMatrix<N, DerivativeType1>(
        [&K](const unsigned short i, const unsigned short j) {
          return -K(i, j);
        });
    const auto Q = eval(dQ_dgT * gT);
    // temperature gradient in the current configuration, F^{-T}.gT
    const auto g = eval(applyTransposedTensor(adj, gT) / J);
    // dQ_i/dF_kl = Q_i.F^{-1}_lk - F^{-1}_ik.Q_l + K_il.g_k, K_il being
    // null if l is the out-of-plane direction in 1D and 2D
    const auto dQ_dF = buildDerivativeWithRespectToTensor<N, DerivativeType2>(
        [&K, &adj, &Q, &g, J](const unsigned short i, const unsigned short k,
                              const unsigned short l) {
          const auto Kil = (l < N) ? K(i, l) : ThermalConductivityType{0};
          return (Q[i] * getTensorComponent(adj, l, k) -
                  getTensorComponent(adj, i, k) * getVectorComponent(Q, l)) /
                     J +
                 Kil * getVectorComponent(g, k);
        });
    return std::tuple{Q, dQ_dgT, dQ_dF};
  }  // end of computeHeatFluxInReferenceConfigurationAndDerivatives

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINHEATTRANSFER_IXX */
