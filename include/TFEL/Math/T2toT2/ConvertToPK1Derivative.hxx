/*!
 * \file   include/TFEL/Math/T2toT2/ConvertToPK1Derivative.hxx
 * \brief
 * \author Thomas Heler
 * \date   25/04/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONVERTTOPK1DERIVATIVE_HXX
#define LIB_TFEL_MATH_CONVERTTOPK1DERIVATIVE_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/types.hxx"

namespace tfel::math {

  /*!
   * \brief convert the derivative of the Cauchy stress with respect to the
   * deformation gradient to the derivative of the first Piola-Kirchhoff
   * stress with respect to the deformation gradient.
   *
   * \param[in] ds: derivative of the Cauchy stress with respect to the
   *                deformation gradient
   * \param[in] F: deformation gradient
   * \param[in] s: Cauchy stress
   *
   * \return the derivative of the first Piola-Kirchhoff
   * stress with respect to the deformation gradient.
   */
  template <StressT2toST2Concept CauchyStressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<CauchyStressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr                       //
      t2tot2<space_dimension<CauchyStressDerivativeType>,
             numeric_type<CauchyStressDerivativeType>>  //
      convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
          const CauchyStressDerivativeType&,
          const DeformationGradientType&,
          const CauchyStressType&) noexcept;
  /*!
   * \brief convert the derivative of the Cauchy stress with respect to the
   * deformation gradient to the derivative of the first Piola-Kirchhoff
   * stress with respect to the deformation gradient.
   *
   * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
   *                 respect to the deformation gradient
   * \param[in] ds: derivative of the Cauchy stress with respect to the
   *                deformation gradient
   * \param[in] F: deformation gradient
   * \param[in] s: Cauchy stress
   */
  template <StressT2toT2Concept PK1StressDerivativeType,
            StressT2toST2Concept CauchyStressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<PK1StressDerivativeType,
                                               CauchyStressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      TFEL_HOST_DEVICE
      constexpr void convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
          PK1StressDerivativeType& dP,
          const CauchyStressDerivativeType& ds,
          const DeformationGradientType& F,
          const CauchyStressType& s) noexcept;
  /*!
   * \brief convert the derivative of the second Piola-Kirchoff stress with
   * respect to the Green-Lagrange strain to the derivative of the
   * first Piola-Kirchhoff stress with respect to the deformation gradient.
   *
   * \param[in] dS: derivative of the second Piola-Kirchoff stress with
   *                respect to the Green-Lagrange strain
   * \param[in] F: deformation gradient
   * \param[in] s: Cauchy stress
   *
   * \return the derivative of the first Piola-Kirchhoff
   * stress with respect to the deformation gradient.
   */
  template <StressST2toST2Concept PK2StressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<PK2StressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr                       //
      t2tot2<space_dimension<PK2StressDerivativeType>,
             numeric_type<PK2StressDerivativeType>>  //
      convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
          const PK2StressDerivativeType&,
          const DeformationGradientType&,
          const CauchyStressType&) noexcept;
  /*!
   * \brief convert the derivative of the second Piola-Kirchoff stress with
   * respect to the Green-Lagrange strain to the derivative of the
   * first Piola-Kirchhoff stress with respect to the deformation gradient.
   *
   * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
   *                 respect to the deformation gradient
   * \param[in] dS: derivative of the second Piola-Kirchoff stress with
   *                respect to the Green-Lagrange strain
   * \param[in] F: deformation gradient
   * \param[in] s: Cauchy stress
   */
  template <StressT2toT2Concept PK1StressDerivativeType,
            StressST2toST2Concept PK2StressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<PK1StressDerivativeType,
                                               PK2StressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      TFEL_HOST_DEVICE constexpr void                                //
      convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
          PK1StressDerivativeType&,
          const PK2StressDerivativeType&,
          const DeformationGradientType&,
          const CauchyStressType&) noexcept;

}  // end of namespace tfel::math

#include "TFEL/Math/T2toT2/ConvertToPK1Derivative.ixx"

#endif /* LIB_TFEL_MATH_CONVERTTOPK1DERIVATIVE_HXX */
