/*!
 * \file   TFEL/Math/Tensor/PiolaTransformation.ixx
 * \brief
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

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_PIOLATRANSFORMATION_IXX */
