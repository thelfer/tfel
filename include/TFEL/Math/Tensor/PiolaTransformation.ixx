/*!
 * \file   TFEL/Math/Tensor/PiolaTransformation.ixx
 * \brief  This file implements functions related to the Piola
 * transformation.
 * \author Thomas Helfer
 * \date   04/02/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PIOLATRANSFORMATION_IXX
#define LIB_TFEL_MATH_PIOLATRANSFORMATION_IXX

#include <type_traits>

namespace tfel::math::internals {

  /*!
   * \return the product of a tensor and a vector
   * \param[in] t: tensor
   * \param[in] v: vector
   */
  template <unsigned short N, typename ValueType, TensorConcept TensorType>
  requires(getSpaceDimension<TensorType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto
      applyTensor(const TensorType& t, const tvector<N, ValueType>& v) noexcept {
    using ResultType = tvector<N, std::decay_t<decltype(t[0] * v[0])>>;
    if constexpr (N == 1) {
      return ResultType{t[0] * v[0]};
    } else if constexpr (N == 2) {
      return ResultType{t[0] * v[0] + t[3] * v[1],  //
                        t[4] * v[0] + t[1] * v[1]};
    } else {
      return ResultType{t[0] * v[0] + t[3] * v[1] + t[5] * v[2],
                        t[4] * v[0] + t[1] * v[1] + t[7] * v[2],
                        t[6] * v[0] + t[8] * v[1] + t[2] * v[2]};
    }
  }  // end of applyTensor

  /*!
   * \return the product of the transpose of a tensor and a vector
   * \param[in] t: tensor
   * \param[in] v: vector
   */
  template <unsigned short N, typename ValueType, TensorConcept TensorType>
  requires(getSpaceDimension<TensorType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto applyTransposedTensor(
          const TensorType& t, const tvector<N, ValueType>& v) noexcept {
    using ResultType = tvector<N, std::decay_t<decltype(t[0] * v[0])>>;
    if constexpr (N == 1) {
      return ResultType{t[0] * v[0]};
    } else if constexpr (N == 2) {
      return ResultType{t[0] * v[0] + t[4] * v[1],  //
                        t[3] * v[0] + t[1] * v[1]};
    } else {
      return ResultType{t[0] * v[0] + t[4] * v[1] + t[6] * v[2],
                        t[3] * v[0] + t[1] * v[1] + t[8] * v[2],
                        t[5] * v[0] + t[7] * v[1] + t[2] * v[2]};
    }
  }  // end of applyTransposedTensor

}  // end of namespace tfel::math::internals

namespace tfel::math {

  template <tfel::math::NoUnitTensorConcept DeformationGradientType>
  constexpr auto computePiolaTransformOfIdentityMatrixInReferenceConfiguration(
      const DeformationGradientType& F) noexcept {
    using real = base_type<numeric_type<DeformationGradientType>>;
    constexpr auto icste = Cste<real>::isqrt2;
    constexpr auto N = getSpaceDimension<DeformationGradientType>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    using ResultType = tmatrix<N, N, numeric_type<DeformationGradientType>>;
    const auto C = computeRightCauchyGreenTensor(F);
    const auto iC = invert(C);
    const auto J = det(F);
    if constexpr (N == 1) {
      return ResultType{J * iC[0]};
    } else if constexpr (N == 2) {
      return ResultType{J * iC[0], J * iC[3] * icste,  //
                        J * iC[3] * icste, J * iC[1]};
    } else {
      return ResultType{
          J * iC[0],         J * iC[3] * icste, J * iC[4] * icste,  //
          J * iC[3] * icste, J * iC[1],         J * iC[5] * icste,
          J * iC[4] * icste, J * iC[5] * icste, J * iC[2]};
    }
  }  // end of computePiolaTransformOfIdentityMatrixInReferenceConfiguration

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computePiolaTransform(
          const tvector<N, ValueType>& q,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTensor(computeAdjugateDeterminantDerivative(F), q);
  }  // end of computePiolaTransform

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeInversePiolaTransform(
          const tvector<N, ValueType>& Q,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTensor(F, Q) / det(F);
  }  // end of computeInversePiolaTransform

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeNansonTransform(
          const tvector<N, ValueType>& n,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTransposedTensor(
        computeAdjugateDeterminantDerivative(F), n);
  }  // end of computeNansonTransform

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeInverseNansonTransform(
          const tvector<N, ValueType>& n,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTransposedTensor(F, n) / det(F);
  }  // end of computeInverseNansonTransform

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_PIOLATRANSFORMATION_IXX */
