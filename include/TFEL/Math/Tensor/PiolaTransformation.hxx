/*!
 * \file   TFEL/Math/Tensor/PiolaTransformation.hxx
 * \brief  This file declares functions related to the Piola
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

#ifndef LIB_TFEL_MATH_PIOLATRANSFORMATION_HXX
#define LIB_TFEL_MATH_PIOLATRANSFORMATION_HXX

#include <tuple>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/types.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Array/FixedSizeArrayDerivative.hxx"

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
  /*!
   * \return the Piola transform \f$J\,F^{-1}\,q\f$ of a vector defined in
   * the current configuration
   * \param[in] q: vector in the current configuration
   * \param[in] F: deformation gradient
   * \note This operation gives the heat flux in the reference configuration
   * from the heat flux in the current configuration.
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto computePiolaTransform(
          const tvector<N, ValueType>&,
          const DeformationGradientType&) noexcept;
  /*!
   * \return the vector \f$F\,Q/J\f$ of the current configuration whose
   * Piola transform is the given vector
   * \param[in] Q: vector in the reference configuration
   * \param[in] F: deformation gradient
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto
      computeInversePiolaTransform(const tvector<N, ValueType>&,
                                   const DeformationGradientType&) noexcept;
  /*!
   * \return the vector \f$J\,F^{-T}\,N\f$ given by Nanson's formula. If
   * \f$N\f$ is an oriented surface element of the reference configuration,
   * the result is the oriented surface element of the current
   * configuration.
   * \param[in] n: vector in the reference configuration
   * \param[in] F: deformation gradient
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto computeNansonTransform(
          const tvector<N, ValueType>&,
          const DeformationGradientType&) noexcept;
  /*!
   * \return the vector \f$F^{T}\,n/J\f$ of the reference configuration
   * whose transform by Nanson's formula is the given vector
   * \param[in] n: vector in the current configuration
   * \param[in] F: deformation gradient
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto
      computeInverseNansonTransform(const tvector<N, ValueType>&,
                                    const DeformationGradientType&) noexcept;
  /*
   * The following functions return a tuple made of the result of the
   * previous functions, its derivative with respect to the transformed
   * vector and its derivative with respect to the deformation gradient.
   * Those derivatives are the ones needed by strongly coupled resolutions.
   */
  /*!
   * \return the Piola transform \f$Q=J\,F^{-1}\,q\f$ of a vector, its
   * derivative with respect to \f$q\f$ and its derivative with respect to
   * \f$F\f$
   * \param[in] q: vector in the current configuration
   * \param[in] F: deformation gradient
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE
      [[nodiscard]] constexpr auto computePiolaTransformAndDerivatives(
          const tvector<N, ValueType>&,
          const DeformationGradientType&) noexcept;
  /*!
   * \return the vector \f$q=F\,Q/J\f$ of the current configuration whose
   * Piola transform is the given vector, its derivative with respect to
   * \f$Q\f$ and its derivative with respect to \f$F\f$
   * \param[in] Q: vector in the reference configuration
   * \param[in] F: deformation gradient
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE
      [[nodiscard]] constexpr auto computeInversePiolaTransformAndDerivatives(
          const tvector<N, ValueType>&,
          const DeformationGradientType&) noexcept;
  /*!
   * \return the vector \f$n=J\,F^{-T}\,N\f$ given by Nanson's formula,
   * its derivative with respect to \f$N\f$ and its derivative with respect
   * to \f$F\f$
   * \param[in] N: vector in the reference configuration
   * \param[in] F: deformation gradient
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE
      [[nodiscard]] constexpr auto computeNansonTransformAndDerivatives(
          const tvector<N, ValueType>&,
          const DeformationGradientType&) noexcept;
  /*!
   * \return the vector \f$N=F^{T}\,n/J\f$ of the reference configuration
   * whose transform by Nanson's formula is the given vector, its derivative
   * with respect to \f$n\f$ and its derivative with respect to \f$F\f$
   * \param[in] n: vector in the current configuration
   * \param[in] F: deformation gradient
   */
  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      TFEL_HOST_DEVICE
      [[nodiscard]] constexpr auto computeInverseNansonTransformAndDerivatives(
          const tvector<N, ValueType>&,
          const DeformationGradientType&) noexcept;

}  // end of namespace tfel::math

#include "TFEL/Math/Tensor/PiolaTransformation.ixx"

#endif /* LIB_TFEL_MATH_PIOLATRANSFORMATION_HXX */
