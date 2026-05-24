/*!
 * \file   TFEL/Math/Tensor/PiolaTransformation.hxx
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

#ifndef LIB_TFEL_MATH_PIOLATRANSFORMATION_HXX
#define LIB_TFEL_MATH_PIOLATRANSFORMATION_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/types.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel::math {

  /*!
   * \brief return the Piola-transform of the id-matrix
   * \note This operation is useful when computing the thermal conductivity
   * matrix in the reference frame
   */
  template <tfel::math::NoUnitTensorConcept DeformationGradientType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr auto
  computePiolaTransformOfIdentityMatrixInReferenceConfiguration(
      const DeformationGradientType& F) noexcept;

}  // end of namespace tfel::math

#include "TFEL/Math/Tensor/PiolaTransformation.ixx"

#endif /* LIB_TFEL_MATH_PIOLATRANSFORMATION_HXX */
