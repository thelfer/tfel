/*!
 * \file   ConvertToPK1Derivative.ixx
 * \brief
 * \author Thomas Helfer
 * \date   30/04/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2_CONVERTTOPK1DERIVATIVE_IXX
#define LIB_TFEL_MATH_T2TOT2_CONVERTTOPK1DERIVATIVE_IXX

namespace tfel::math::internals {

  /*!
   * \brief an helper structure meant to be specialized in each spatial
   * dimension.
   * \tparam N: spatial dimension
   */
  template <unsigned short N>
  struct DSIG_DF_to_DPK1_DF_Converter;

  /*!
   * \brief partial specialisation in 1D
   */
  template <>
  struct DSIG_DF_to_DPK1_DF_Converter<1u> {
    /*!
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
        TFEL_HOST_DEVICE static constexpr void                         //
        exe(PK1StressDerivativeType& dP,
            const CauchyStressDerivativeType& ds,
            const DeformationGradientType& F,
            const CauchyStressType& s) noexcept {
      dP(0, 0) = ds(0, 0) * F[1] * F[2];
      dP(1, 0) = F[0] * ds(1, 0) * F[2] + s[1] * F[2];
      dP(2, 0) = F[0] * ds(2, 0) * F[1] + s[2] * F[1];
      dP(0, 1) = ds(0, 1) * F[1] * F[2] + s[0] * F[2];
      dP(1, 1) = F[0] * ds(1, 1) * F[2];
      dP(2, 1) = F[0] * ds(2, 1) * F[1] + F[0] * s[2];
      dP(0, 2) = ds(0, 2) * F[1] * F[2] + s[0] * F[1];
      dP(1, 2) = F[0] * ds(1, 2) * F[2] + F[0] * s[1];
      dP(2, 2) = F[0] * ds(2, 2) * F[1];
    }  // end of struct DSIG_DF_to_DPK1_DF_Converter<1u>
  };
  /*!
   * \brief partial specialisation in 2D
   */
  template <>
  struct DSIG_DF_to_DPK1_DF_Converter<2u> {
    /*!
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
        TFEL_HOST_DEVICE static constexpr void                         //
        exe(PK1StressDerivativeType& dP,
            const CauchyStressDerivativeType& ds,
            const DeformationGradientType& F,
            const CauchyStressType& s) noexcept {
      using real = numeric_type<DeformationGradientType>;
      constexpr auto cste = tfel::math::Cste<real>::sqrt2;
      constexpr auto icste = tfel::math::Cste<real>::isqrt2;
      //-->  f90(diff(P[1],F[0]));
      dP(0, 0) =
          -(ds(3, 0) * F[2] * F[3] - cste * ds(0, 0) * F[1] * F[2]) * icste;
      dP(1, 0) = -(ds(3, 0) * F[2] * F[4] - cste * F[0] * ds(1, 0) * F[2] -
                   cste * s[1] * F[2]) *
                 icste;
      dP(2, 0) =
          (-ds(2, 0) * F[3] * F[4]) + F[0] * ds(2, 0) * F[1] + s[2] * F[1];
      dP(3, 0) = -(cste * ds(0, 0) * F[2] * F[4] - F[0] * ds(3, 0) * F[2] -
                   s[3] * F[2]) *
                 icste;
      dP(4, 0) =
          -(cste * ds(1, 0) * F[2] * F[3] - ds(3, 0) * F[1] * F[2]) * icste;
      //-->  f90(diff(P[1],F[1]));
      dP(0, 1) = -(ds(3, 1) * F[2] * F[3] - cste * ds(0, 1) * F[1] * F[2] -
                   cste * s[0] * F[2]) *
                 icste;
      dP(1, 1) =
          -(ds(3, 1) * F[2] * F[4] - cste * F[0] * ds(1, 1) * F[2]) * icste;
      dP(2, 1) =
          (-ds(2, 1) * F[3] * F[4]) + F[0] * ds(2, 1) * F[1] + F[0] * s[2];
      dP(3, 1) =
          -(cste * ds(0, 1) * F[2] * F[4] - F[0] * ds(3, 1) * F[2]) * icste;
      dP(4, 1) = -(cste * ds(1, 1) * F[2] * F[3] - ds(3, 1) * F[1] * F[2] -
                   s[3] * F[2]) *
                 icste;
      // -->  f90(diff(P[1],F[2]));
      dP(0, 2) = -(ds(3, 2) * F[2] * F[3] + s[3] * F[3] -
                   cste * ds(0, 2) * F[1] * F[2] - cste * s[0] * F[1]) *
                 icste;
      dP(1, 2) = -(ds(3, 2) * F[2] * F[4] + s[3] * F[4] -
                   cste * F[0] * ds(1, 2) * F[2] - cste * F[0] * s[1]) *
                 icste;
      dP(2, 2) = F[0] * ds(2, 2) * F[1] - ds(2, 2) * F[3] * F[4];
      dP(3, 2) = -(cste * ds(0, 2) * F[2] * F[4] + cste * s[0] * F[4] -
                   F[0] * ds(3, 2) * F[2] - F[0] * s[3]) *
                 icste;
      dP(4, 2) = -(cste * ds(1, 2) * F[2] * F[3] + cste * s[1] * F[3] -
                   ds(3, 2) * F[1] * F[2] - s[3] * F[1]) *
                 icste;
      // -->  f90(diff(P[1],F[3]));
      dP(0, 3) = -(ds(3, 3) * F[2] * F[3] - cste * ds(0, 3) * F[1] * F[2] +
                   s[3] * F[2]) *
                 icste;
      dP(1, 3) =
          -(ds(3, 3) * F[2] * F[4] - cste * F[0] * ds(1, 3) * F[2]) * icste;
      dP(2, 3) =
          (-ds(2, 3) * F[3] * F[4]) - s[2] * F[4] + F[0] * ds(2, 3) * F[1];
      dP(3, 3) =
          -(cste * ds(0, 3) * F[2] * F[4] - F[0] * ds(3, 3) * F[2]) * icste;
      dP(4, 3) = -(cste * ds(1, 3) * F[2] * F[3] - ds(3, 3) * F[1] * F[2] +
                   cste * s[1] * F[2]) *
                 icste;
      // -->  f90(diff(P[1],F[4]));
      dP(0, 4) =
          -(ds(3, 4) * F[2] * F[3] - cste * ds(0, 4) * F[1] * F[2]) * icste;
      dP(1, 4) = -(ds(3, 4) * F[2] * F[4] - cste * F[0] * ds(1, 4) * F[2] +
                   s[3] * F[2]) *
                 icste;
      dP(2, 4) =
          (-ds(2, 4) * F[3] * F[4]) - s[2] * F[3] + F[0] * ds(2, 4) * F[1];
      dP(3, 4) = -(cste * ds(0, 4) * F[2] * F[4] - F[0] * ds(3, 4) * F[2] +
                   cste * s[0] * F[2]) *
                 icste;
      dP(4, 4) =
          -(cste * ds(1, 4) * F[2] * F[3] - ds(3, 4) * F[1] * F[2]) * icste;
    }
  };  // end of struct DSIG_DF_to_DPK1_DF_Converter<2u>

  /*!
   * \brief partial specialisation in 3D
   */
  template <>
  struct DSIG_DF_to_DPK1_DF_Converter<3u> {
    /*!
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
        TFEL_HOST_DEVICE static constexpr void                         //
        exe(PK1StressDerivativeType& dP,
            const CauchyStressDerivativeType& ds,
            const DeformationGradientType& F,
            const CauchyStressType& s) noexcept {
      using real = numeric_type<DeformationGradientType>;
      constexpr auto cste = tfel::math::Cste<real>::sqrt2;
      // -->  f90(diff(P[1],F_0));
      dP(0, 0) =
          -((2 * ds(0, 0) * F[7] - cste * ds(3, 0) * F[5]) * F[8] -
            cste * ds(4, 0) * F[3] * F[7] + cste * ds(4, 0) * F[1] * F[5] +
            cste * ds(3, 0) * F[2] * F[3] - 2 * ds(0, 0) * F[1] * F[2]) /
          2;
      dP(1, 0) =
          ((cste * ds(3, 0) * F[6] - cste * F[0] * ds(5, 0) - cste * s[5]) *
               F[7] -
           2 * ds(1, 0) * F[5] * F[6] + cste * ds(5, 0) * F[4] * F[5] -
           cste * ds(3, 0) * F[2] * F[4] + 2 * F[0] * ds(1, 0) * F[2] +
           2 * s[1] * F[2]) /
          2;
      dP(2, 0) =
          ((cste * ds(4, 0) * F[4] - cste * F[0] * ds(5, 0) - cste * s[5]) *
               F[8] +
           (cste * ds(5, 0) * F[3] - cste * ds(4, 0) * F[1]) * F[6] -
           2 * ds(2, 0) * F[3] * F[4] + 2 * F[0] * ds(2, 0) * F[1] +
           2 * s[2] * F[1]) /
          2;
      dP(3, 0) =
          ((2 * ds(0, 0) * F[6] - cste * F[0] * ds(4, 0) - cste * s[4]) * F[7] -
           cste * ds(3, 0) * F[5] * F[6] + cste * ds(4, 0) * F[4] * F[5] -
           2 * ds(0, 0) * F[2] * F[4] + cste * F[0] * ds(3, 0) * F[2] +
           cste * s[3] * F[2]) /
          2;
      dP(4, 0) =
          -((cste * ds(3, 0) * F[7] - 2 * ds(1, 0) * F[5]) * F[8] -
            cste * ds(5, 0) * F[3] * F[7] + cste * ds(5, 0) * F[1] * F[5] +
            2 * ds(1, 0) * F[2] * F[3] - cste * ds(3, 0) * F[1] * F[2]) /
          2;
      dP(5, 0) =
          ((2 * ds(0, 0) * F[4] - cste * F[0] * ds(3, 0) - cste * s[3]) * F[8] +
           (cste * ds(3, 0) * F[3] - 2 * ds(0, 0) * F[1]) * F[6] -
           cste * ds(4, 0) * F[3] * F[4] + cste * F[0] * ds(4, 0) * F[1] +
           cste * s[4] * F[1]) /
          2;
      dP(6, 0) =
          -((cste * ds(4, 0) * F[7] - cste * ds(5, 0) * F[5]) * F[8] -
            2 * ds(2, 0) * F[3] * F[7] + 2 * ds(2, 0) * F[1] * F[5] +
            cste * ds(5, 0) * F[2] * F[3] - cste * ds(4, 0) * F[1] * F[2]) /
          2;
      dP(7, 0) =
          ((cste * ds(3, 0) * F[4] - 2 * F[0] * ds(1, 0) - 2 * s[1]) * F[8] +
           (2 * ds(1, 0) * F[3] - cste * ds(3, 0) * F[1]) * F[6] -
           cste * ds(5, 0) * F[3] * F[4] + cste * F[0] * ds(5, 0) * F[1] +
           cste * s[5] * F[1]) /
          2;
      dP(8, 0) =
          ((cste * ds(4, 0) * F[6] - 2 * F[0] * ds(2, 0) - 2 * s[2]) * F[7] -
           cste * ds(5, 0) * F[5] * F[6] + 2 * ds(2, 0) * F[4] * F[5] -
           cste * ds(4, 0) * F[2] * F[4] + cste * F[0] * ds(5, 0) * F[2] +
           cste * s[5] * F[2]) /
          2;
      // -->  f90(diff(P[1],F[1]));
      dP(0, 1) =
          -((2 * ds(0, 1) * F[7] - cste * ds(3, 1) * F[5]) * F[8] -
            cste * ds(4, 1) * F[3] * F[7] + cste * ds(4, 1) * F[1] * F[5] +
            cste * s[4] * F[5] + cste * ds(3, 1) * F[2] * F[3] -
            2 * ds(0, 1) * F[1] * F[2] - 2 * s[0] * F[2]) /
          2;
      dP(1, 1) = ((cste * ds(3, 1) * F[6] - cste * F[0] * ds(5, 1)) * F[7] -
                  2 * ds(1, 1) * F[5] * F[6] + cste * ds(5, 1) * F[4] * F[5] -
                  cste * ds(3, 1) * F[2] * F[4] + 2 * F[0] * ds(1, 1) * F[2]) /
                 2;
      dP(2, 1) =
          ((cste * ds(4, 1) * F[4] - cste * F[0] * ds(5, 1)) * F[8] +
           (cste * ds(5, 1) * F[3] - cste * ds(4, 1) * F[1] - cste * s[4]) *
               F[6] -
           2 * ds(2, 1) * F[3] * F[4] + 2 * F[0] * ds(2, 1) * F[1] +
           2 * F[0] * s[2]) /
          2;
      dP(3, 1) =
          ((2 * ds(0, 1) * F[6] - cste * F[0] * ds(4, 1)) * F[7] -
           cste * ds(3, 1) * F[5] * F[6] + cste * ds(4, 1) * F[4] * F[5] -
           2 * ds(0, 1) * F[2] * F[4] + cste * F[0] * ds(3, 1) * F[2]) /
          2;
      dP(4, 1) =
          -((cste * ds(3, 1) * F[7] - 2 * ds(1, 1) * F[5]) * F[8] -
            cste * ds(5, 1) * F[3] * F[7] + cste * ds(5, 1) * F[1] * F[5] +
            cste * s[5] * F[5] + 2 * ds(1, 1) * F[2] * F[3] -
            cste * ds(3, 1) * F[1] * F[2] - cste * s[3] * F[2]) /
          2;
      dP(5, 1) =
          ((2 * ds(0, 1) * F[4] - cste * F[0] * ds(3, 1)) * F[8] +
           (cste * ds(3, 1) * F[3] - 2 * ds(0, 1) * F[1] - 2 * s[0]) * F[6] -
           cste * ds(4, 1) * F[3] * F[4] + cste * F[0] * ds(4, 1) * F[1] +
           cste * F[0] * s[4]) /
          2;
      dP(6, 1) = -((cste * ds(4, 1) * F[7] - cste * ds(5, 1) * F[5]) * F[8] -
                   2 * ds(2, 1) * F[3] * F[7] + 2 * ds(2, 1) * F[1] * F[5] +
                   2 * s[2] * F[5] + cste * ds(5, 1) * F[2] * F[3] -
                   cste * ds(4, 1) * F[1] * F[2] - cste * s[4] * F[2]) /
                 2;
      dP(7, 1) =
          ((cste * ds(3, 1) * F[4] - 2 * F[0] * ds(1, 1)) * F[8] +
           (2 * ds(1, 1) * F[3] - cste * ds(3, 1) * F[1] - cste * s[3]) * F[6] -
           cste * ds(5, 1) * F[3] * F[4] + cste * F[0] * ds(5, 1) * F[1] +
           cste * F[0] * s[5]) /
          2;
      dP(8, 1) =
          ((cste * ds(4, 1) * F[6] - 2 * F[0] * ds(2, 1)) * F[7] -
           cste * ds(5, 1) * F[5] * F[6] + 2 * ds(2, 1) * F[4] * F[5] -
           cste * ds(4, 1) * F[2] * F[4] + cste * F[0] * ds(5, 1) * F[2]) /
          2;
      // -->  f90(diff(P[1],F[2]));
      dP(0, 2) =
          -((2 * ds(0, 2) * F[7] - cste * ds(3, 2) * F[5]) * F[8] -
            cste * ds(4, 2) * F[3] * F[7] + cste * ds(4, 2) * F[1] * F[5] +
            cste * ds(3, 2) * F[2] * F[3] + cste * s[3] * F[3] -
            2 * ds(0, 2) * F[1] * F[2] - 2 * s[0] * F[1]) /
          2;
      dP(1, 2) = ((cste * ds(3, 2) * F[6] - cste * F[0] * ds(5, 2)) * F[7] -
                  2 * ds(1, 2) * F[5] * F[6] + cste * ds(5, 2) * F[4] * F[5] -
                  cste * ds(3, 2) * F[2] * F[4] - cste * s[3] * F[4] +
                  2 * F[0] * ds(1, 2) * F[2] + 2 * F[0] * s[1]) /
                 2;
      dP(2, 2) = ((cste * ds(4, 2) * F[4] - cste * F[0] * ds(5, 2)) * F[8] +
                  (cste * ds(5, 2) * F[3] - cste * ds(4, 2) * F[1]) * F[6] -
                  2 * ds(2, 2) * F[3] * F[4] + 2 * F[0] * ds(2, 2) * F[1]) /
                 2;
      dP(3, 2) =
          ((2 * ds(0, 2) * F[6] - cste * F[0] * ds(4, 2)) * F[7] -
           cste * ds(3, 2) * F[5] * F[6] + cste * ds(4, 2) * F[4] * F[5] -
           2 * ds(0, 2) * F[2] * F[4] - 2 * s[0] * F[4] +
           cste * F[0] * ds(3, 2) * F[2] + cste * F[0] * s[3]) /
          2;
      dP(4, 2) =
          -((cste * ds(3, 2) * F[7] - 2 * ds(1, 2) * F[5]) * F[8] -
            cste * ds(5, 2) * F[3] * F[7] + cste * ds(5, 2) * F[1] * F[5] +
            2 * ds(1, 2) * F[2] * F[3] + 2 * s[1] * F[3] -
            cste * ds(3, 2) * F[1] * F[2] - cste * s[3] * F[1]) /
          2;
      dP(5, 2) =
          ((2 * ds(0, 2) * F[4] - cste * F[0] * ds(3, 2)) * F[8] +
           (cste * ds(3, 2) * F[3] - 2 * ds(0, 2) * F[1]) * F[6] -
           cste * ds(4, 2) * F[3] * F[4] + cste * F[0] * ds(4, 2) * F[1]) /
          2;
      dP(6, 2) = -((cste * ds(4, 2) * F[7] - cste * ds(5, 2) * F[5]) * F[8] -
                   2 * ds(2, 2) * F[3] * F[7] + 2 * ds(2, 2) * F[1] * F[5] +
                   cste * ds(5, 2) * F[2] * F[3] + cste * s[5] * F[3] -
                   cste * ds(4, 2) * F[1] * F[2] - cste * s[4] * F[1]) /
                 2;
      dP(7, 2) =
          ((cste * ds(3, 2) * F[4] - 2 * F[0] * ds(1, 2)) * F[8] +
           (2 * ds(1, 2) * F[3] - cste * ds(3, 2) * F[1]) * F[6] -
           cste * ds(5, 2) * F[3] * F[4] + cste * F[0] * ds(5, 2) * F[1]) /
          2;
      dP(8, 2) = ((cste * ds(4, 2) * F[6] - 2 * F[0] * ds(2, 2)) * F[7] -
                  cste * ds(5, 2) * F[5] * F[6] + 2 * ds(2, 2) * F[4] * F[5] -
                  cste * ds(4, 2) * F[2] * F[4] - cste * s[4] * F[4] +
                  cste * F[0] * ds(5, 2) * F[2] + cste * F[0] * s[5]) /
                 2;
      // -->  f90(diff(P[1],F[3]));
      dP(0, 3) =
          -((2 * ds(0, 3) * F[7] - cste * ds(3, 3) * F[5]) * F[8] -
            cste * ds(4, 3) * F[3] * F[7] - cste * s[4] * F[7] +
            cste * ds(4, 3) * F[1] * F[5] + cste * ds(3, 3) * F[2] * F[3] -
            2 * ds(0, 3) * F[1] * F[2] + cste * s[3] * F[2]) /
          2;
      dP(1, 3) = ((cste * ds(3, 3) * F[6] - cste * F[0] * ds(5, 3)) * F[7] -
                  2 * ds(1, 3) * F[5] * F[6] + cste * ds(5, 3) * F[4] * F[5] -
                  cste * ds(3, 3) * F[2] * F[4] + 2 * F[0] * ds(1, 3) * F[2]) /
                 2;
      dP(2, 3) =
          ((cste * ds(4, 3) * F[4] - cste * F[0] * ds(5, 3)) * F[8] +
           (cste * ds(5, 3) * F[3] - cste * ds(4, 3) * F[1] + cste * s[5]) *
               F[6] -
           2 * ds(2, 3) * F[3] * F[4] - 2 * s[2] * F[4] +
           2 * F[0] * ds(2, 3) * F[1]) /
          2;
      dP(3, 3) =
          ((2 * ds(0, 3) * F[6] - cste * F[0] * ds(4, 3)) * F[7] -
           cste * ds(3, 3) * F[5] * F[6] + cste * ds(4, 3) * F[4] * F[5] -
           2 * ds(0, 3) * F[2] * F[4] + cste * F[0] * ds(3, 3) * F[2]) /
          2;
      dP(4, 3) = -((cste * ds(3, 3) * F[7] - 2 * ds(1, 3) * F[5]) * F[8] -
                   cste * ds(5, 3) * F[3] * F[7] - cste * s[5] * F[7] +
                   cste * ds(5, 3) * F[1] * F[5] + 2 * ds(1, 3) * F[2] * F[3] -
                   cste * ds(3, 3) * F[1] * F[2] + 2 * s[1] * F[2]) /
                 2;
      dP(5, 3) =
          ((2 * ds(0, 3) * F[4] - cste * F[0] * ds(3, 3)) * F[8] +
           (cste * ds(3, 3) * F[3] - 2 * ds(0, 3) * F[1] + cste * s[3]) * F[6] -
           cste * ds(4, 3) * F[3] * F[4] - cste * s[4] * F[4] +
           cste * F[0] * ds(4, 3) * F[1]) /
          2;
      dP(6, 3) = -((cste * ds(4, 3) * F[7] - cste * ds(5, 3) * F[5]) * F[8] -
                   2 * ds(2, 3) * F[3] * F[7] - 2 * s[2] * F[7] +
                   2 * ds(2, 3) * F[1] * F[5] + cste * ds(5, 3) * F[2] * F[3] -
                   cste * ds(4, 3) * F[1] * F[2] + cste * s[5] * F[2]) /
                 2;
      dP(7, 3) =
          ((cste * ds(3, 3) * F[4] - 2 * F[0] * ds(1, 3)) * F[8] +
           (2 * ds(1, 3) * F[3] - cste * ds(3, 3) * F[1] + 2 * s[1]) * F[6] -
           cste * ds(5, 3) * F[3] * F[4] - cste * s[5] * F[4] +
           cste * F[0] * ds(5, 3) * F[1]) /
          2;
      dP(8, 3) =
          ((cste * ds(4, 3) * F[6] - 2 * F[0] * ds(2, 3)) * F[7] -
           cste * ds(5, 3) * F[5] * F[6] + 2 * ds(2, 3) * F[4] * F[5] -
           cste * ds(4, 3) * F[2] * F[4] + cste * F[0] * ds(5, 3) * F[2]) /
          2;
      // -->  f90(diff(P[1],F[4]));
      dP(0, 4) =
          -((2 * ds(0, 4) * F[7] - cste * ds(3, 4) * F[5]) * F[8] -
            cste * ds(4, 4) * F[3] * F[7] + cste * ds(4, 4) * F[1] * F[5] +
            cste * ds(3, 4) * F[2] * F[3] - 2 * ds(0, 4) * F[1] * F[2]) /
          2;
      dP(1, 4) = ((cste * ds(3, 4) * F[6] - cste * F[0] * ds(5, 4)) * F[7] -
                  2 * ds(1, 4) * F[5] * F[6] + cste * ds(5, 4) * F[4] * F[5] +
                  cste * s[5] * F[5] - cste * ds(3, 4) * F[2] * F[4] +
                  2 * F[0] * ds(1, 4) * F[2] - cste * s[3] * F[2]) /
                 2;
      dP(2, 4) =
          ((cste * ds(4, 4) * F[4] - cste * F[0] * ds(5, 4) + cste * s[4]) *
               F[8] +
           (cste * ds(5, 4) * F[3] - cste * ds(4, 4) * F[1]) * F[6] -
           2 * ds(2, 4) * F[3] * F[4] - 2 * s[2] * F[3] +
           2 * F[0] * ds(2, 4) * F[1]) /
          2;
      dP(3, 4) =
          ((2 * ds(0, 4) * F[6] - cste * F[0] * ds(4, 4)) * F[7] -
           cste * ds(3, 4) * F[5] * F[6] + cste * ds(4, 4) * F[4] * F[5] +
           cste * s[4] * F[5] - 2 * ds(0, 4) * F[2] * F[4] +
           cste * F[0] * ds(3, 4) * F[2] - 2 * s[0] * F[2]) /
          2;
      dP(4, 4) =
          -((cste * ds(3, 4) * F[7] - 2 * ds(1, 4) * F[5]) * F[8] -
            cste * ds(5, 4) * F[3] * F[7] + cste * ds(5, 4) * F[1] * F[5] +
            2 * ds(1, 4) * F[2] * F[3] - cste * ds(3, 4) * F[1] * F[2]) /
          2;
      dP(5, 4) =
          ((2 * ds(0, 4) * F[4] - cste * F[0] * ds(3, 4) + 2 * s[0]) * F[8] +
           (cste * ds(3, 4) * F[3] - 2 * ds(0, 4) * F[1]) * F[6] -
           cste * ds(4, 4) * F[3] * F[4] - cste * s[4] * F[3] +
           cste * F[0] * ds(4, 4) * F[1]) /
          2;
      dP(6, 4) =
          -((cste * ds(4, 4) * F[7] - cste * ds(5, 4) * F[5]) * F[8] -
            2 * ds(2, 4) * F[3] * F[7] + 2 * ds(2, 4) * F[1] * F[5] +
            cste * ds(5, 4) * F[2] * F[3] - cste * ds(4, 4) * F[1] * F[2]) /
          2;
      dP(7, 4) =
          ((cste * ds(3, 4) * F[4] - 2 * F[0] * ds(1, 4) + cste * s[3]) * F[8] +
           (2 * ds(1, 4) * F[3] - cste * ds(3, 4) * F[1]) * F[6] -
           cste * ds(5, 4) * F[3] * F[4] - cste * s[5] * F[3] +
           cste * F[0] * ds(5, 4) * F[1]) /
          2;
      dP(8, 4) = ((cste * ds(4, 4) * F[6] - 2 * F[0] * ds(2, 4)) * F[7] -
                  cste * ds(5, 4) * F[5] * F[6] + 2 * ds(2, 4) * F[4] * F[5] +
                  2 * s[2] * F[5] - cste * ds(4, 4) * F[2] * F[4] +
                  cste * F[0] * ds(5, 4) * F[2] - cste * s[4] * F[2]) /
                 2;
      // -->  f90(diff(P[1],F[5]));
      dP(0, 5) =
          -((2 * ds(0, 5) * F[7] - cste * ds(3, 5) * F[5] - cste * s[3]) *
                F[8] -
            cste * ds(4, 5) * F[3] * F[7] + cste * ds(4, 5) * F[1] * F[5] +
            cste * ds(3, 5) * F[2] * F[3] - 2 * ds(0, 5) * F[1] * F[2] +
            cste * s[4] * F[1]) /
          2;
      dP(1, 5) =
          ((cste * ds(3, 5) * F[6] - cste * F[0] * ds(5, 5)) * F[7] -
           2 * ds(1, 5) * F[5] * F[6] - 2 * s[1] * F[6] +
           cste * ds(5, 5) * F[4] * F[5] - cste * ds(3, 5) * F[2] * F[4] +
           cste * s[5] * F[4] + 2 * F[0] * ds(1, 5) * F[2]) /
          2;
      dP(2, 5) = ((cste * ds(4, 5) * F[4] - cste * F[0] * ds(5, 5)) * F[8] +
                  (cste * ds(5, 5) * F[3] - cste * ds(4, 5) * F[1]) * F[6] -
                  2 * ds(2, 5) * F[3] * F[4] + 2 * F[0] * ds(2, 5) * F[1]) /
                 2;
      dP(3, 5) = ((2 * ds(0, 5) * F[6] - cste * F[0] * ds(4, 5)) * F[7] -
                  cste * ds(3, 5) * F[5] * F[6] - cste * s[3] * F[6] +
                  cste * ds(4, 5) * F[4] * F[5] - 2 * ds(0, 5) * F[2] * F[4] +
                  cste * s[4] * F[4] + cste * F[0] * ds(3, 5) * F[2]) /
                 2;
      dP(4, 5) =
          -((cste * ds(3, 5) * F[7] - 2 * ds(1, 5) * F[5] - 2 * s[1]) * F[8] -
            cste * ds(5, 5) * F[3] * F[7] + cste * ds(5, 5) * F[1] * F[5] +
            2 * ds(1, 5) * F[2] * F[3] - cste * ds(3, 5) * F[1] * F[2] +
            cste * s[5] * F[1]) /
          2;
      dP(5, 5) =
          ((2 * ds(0, 5) * F[4] - cste * F[0] * ds(3, 5)) * F[8] +
           (cste * ds(3, 5) * F[3] - 2 * ds(0, 5) * F[1]) * F[6] -
           cste * ds(4, 5) * F[3] * F[4] + cste * F[0] * ds(4, 5) * F[1]) /
          2;
      dP(6, 5) =
          -((cste * ds(4, 5) * F[7] - cste * ds(5, 5) * F[5] - cste * s[5]) *
                F[8] -
            2 * ds(2, 5) * F[3] * F[7] + 2 * ds(2, 5) * F[1] * F[5] +
            cste * ds(5, 5) * F[2] * F[3] - cste * ds(4, 5) * F[1] * F[2] +
            2 * s[2] * F[1]) /
          2;
      dP(7, 5) =
          ((cste * ds(3, 5) * F[4] - 2 * F[0] * ds(1, 5)) * F[8] +
           (2 * ds(1, 5) * F[3] - cste * ds(3, 5) * F[1]) * F[6] -
           cste * ds(5, 5) * F[3] * F[4] + cste * F[0] * ds(5, 5) * F[1]) /
          2;
      dP(8, 5) = ((cste * ds(4, 5) * F[6] - 2 * F[0] * ds(2, 5)) * F[7] -
                  cste * ds(5, 5) * F[5] * F[6] - cste * s[5] * F[6] +
                  2 * ds(2, 5) * F[4] * F[5] - cste * ds(4, 5) * F[2] * F[4] +
                  2 * s[2] * F[4] + cste * F[0] * ds(5, 5) * F[2]) /
                 2;
      // -->  f90(diff(P[1],F[6]));
      dP(0, 6) =
          -((2 * ds(0, 6) * F[7] - cste * ds(3, 6) * F[5]) * F[8] -
            cste * ds(4, 6) * F[3] * F[7] + cste * ds(4, 6) * F[1] * F[5] +
            cste * ds(3, 6) * F[2] * F[3] - 2 * ds(0, 6) * F[1] * F[2]) /
          2;
      dP(1, 6) =
          ((cste * ds(3, 6) * F[6] - cste * F[0] * ds(5, 6) + cste * s[3]) *
               F[7] -
           2 * ds(1, 6) * F[5] * F[6] + cste * ds(5, 6) * F[4] * F[5] -
           2 * s[1] * F[5] - cste * ds(3, 6) * F[2] * F[4] +
           2 * F[0] * ds(1, 6) * F[2]) /
          2;
      dP(2, 6) = ((cste * ds(4, 6) * F[4] - cste * F[0] * ds(5, 6)) * F[8] +
                  (cste * ds(5, 6) * F[3] - cste * ds(4, 6) * F[1]) * F[6] -
                  2 * ds(2, 6) * F[3] * F[4] + cste * s[5] * F[3] +
                  2 * F[0] * ds(2, 6) * F[1] - cste * s[4] * F[1]) /
                 2;
      dP(3, 6) =
          ((2 * ds(0, 6) * F[6] - cste * F[0] * ds(4, 6) + 2 * s[0]) * F[7] -
           cste * ds(3, 6) * F[5] * F[6] + cste * ds(4, 6) * F[4] * F[5] -
           cste * s[3] * F[5] - 2 * ds(0, 6) * F[2] * F[4] +
           cste * F[0] * ds(3, 6) * F[2]) /
          2;
      dP(4, 6) =
          -((cste * ds(3, 6) * F[7] - 2 * ds(1, 6) * F[5]) * F[8] -
            cste * ds(5, 6) * F[3] * F[7] + cste * ds(5, 6) * F[1] * F[5] +
            2 * ds(1, 6) * F[2] * F[3] - cste * ds(3, 6) * F[1] * F[2]) /
          2;
      dP(5, 6) = ((2 * ds(0, 6) * F[4] - cste * F[0] * ds(3, 6)) * F[8] +
                  (cste * ds(3, 6) * F[3] - 2 * ds(0, 6) * F[1]) * F[6] -
                  cste * ds(4, 6) * F[3] * F[4] + cste * s[3] * F[3] +
                  cste * F[0] * ds(4, 6) * F[1] - 2 * s[0] * F[1]) /
                 2;
      dP(6, 6) =
          -((cste * ds(4, 6) * F[7] - cste * ds(5, 6) * F[5]) * F[8] -
            2 * ds(2, 6) * F[3] * F[7] + 2 * ds(2, 6) * F[1] * F[5] +
            cste * ds(5, 6) * F[2] * F[3] - cste * ds(4, 6) * F[1] * F[2]) /
          2;
      dP(7, 6) = ((cste * ds(3, 6) * F[4] - 2 * F[0] * ds(1, 6)) * F[8] +
                  (2 * ds(1, 6) * F[3] - cste * ds(3, 6) * F[1]) * F[6] -
                  cste * ds(5, 6) * F[3] * F[4] + 2 * s[1] * F[3] +
                  cste * F[0] * ds(5, 6) * F[1] - cste * s[3] * F[1]) /
                 2;
      dP(8, 6) =
          ((cste * ds(4, 6) * F[6] - 2 * F[0] * ds(2, 6) + cste * s[4]) * F[7] -
           cste * ds(5, 6) * F[5] * F[6] + 2 * ds(2, 6) * F[4] * F[5] -
           cste * s[5] * F[5] - cste * ds(4, 6) * F[2] * F[4] +
           cste * F[0] * ds(5, 6) * F[2]) /
          2;
      // -->  f90(diff(P[1],F[7]));
      dP(0, 7) =
          -((2 * ds(0, 7) * F[7] - cste * ds(3, 7) * F[5] + 2 * s[0]) * F[8] -
            cste * ds(4, 7) * F[3] * F[7] + cste * ds(4, 7) * F[1] * F[5] +
            cste * ds(3, 7) * F[2] * F[3] - cste * s[4] * F[3] -
            2 * ds(0, 7) * F[1] * F[2]) /
          2;
      dP(1, 7) =
          ((cste * ds(3, 7) * F[6] - cste * F[0] * ds(5, 7)) * F[7] -
           2 * ds(1, 7) * F[5] * F[6] + cste * s[3] * F[6] +
           cste * ds(5, 7) * F[4] * F[5] - cste * ds(3, 7) * F[2] * F[4] +
           2 * F[0] * ds(1, 7) * F[2] - cste * F[0] * s[5]) /
          2;
      dP(2, 7) = ((cste * ds(4, 7) * F[4] - cste * F[0] * ds(5, 7)) * F[8] +
                  (cste * ds(5, 7) * F[3] - cste * ds(4, 7) * F[1]) * F[6] -
                  2 * ds(2, 7) * F[3] * F[4] + 2 * F[0] * ds(2, 7) * F[1]) /
                 2;
      dP(3, 7) = ((2 * ds(0, 7) * F[6] - cste * F[0] * ds(4, 7)) * F[7] -
                  cste * ds(3, 7) * F[5] * F[6] + 2 * s[0] * F[6] +
                  cste * ds(4, 7) * F[4] * F[5] - 2 * ds(0, 7) * F[2] * F[4] +
                  cste * F[0] * ds(3, 7) * F[2] - cste * F[0] * s[4]) /
                 2;
      dP(4, 7) =
          -((cste * ds(3, 7) * F[7] - 2 * ds(1, 7) * F[5] + cste * s[3]) *
                F[8] -
            cste * ds(5, 7) * F[3] * F[7] + cste * ds(5, 7) * F[1] * F[5] +
            2 * ds(1, 7) * F[2] * F[3] - cste * s[5] * F[3] -
            cste * ds(3, 7) * F[1] * F[2]) /
          2;
      dP(5, 7) =
          ((2 * ds(0, 7) * F[4] - cste * F[0] * ds(3, 7)) * F[8] +
           (cste * ds(3, 7) * F[3] - 2 * ds(0, 7) * F[1]) * F[6] -
           cste * ds(4, 7) * F[3] * F[4] + cste * F[0] * ds(4, 7) * F[1]) /
          2;
      dP(6, 7) =
          -((cste * ds(4, 7) * F[7] - cste * ds(5, 7) * F[5] + cste * s[4]) *
                F[8] -
            2 * ds(2, 7) * F[3] * F[7] + 2 * ds(2, 7) * F[1] * F[5] +
            cste * ds(5, 7) * F[2] * F[3] - 2 * s[2] * F[3] -
            cste * ds(4, 7) * F[1] * F[2]) /
          2;
      dP(7, 7) =
          ((cste * ds(3, 7) * F[4] - 2 * F[0] * ds(1, 7)) * F[8] +
           (2 * ds(1, 7) * F[3] - cste * ds(3, 7) * F[1]) * F[6] -
           cste * ds(5, 7) * F[3] * F[4] + cste * F[0] * ds(5, 7) * F[1]) /
          2;
      dP(8, 7) = ((cste * ds(4, 7) * F[6] - 2 * F[0] * ds(2, 7)) * F[7] -
                  cste * ds(5, 7) * F[5] * F[6] + cste * s[4] * F[6] +
                  2 * ds(2, 7) * F[4] * F[5] - cste * ds(4, 7) * F[2] * F[4] +
                  cste * F[0] * ds(5, 7) * F[2] - 2 * F[0] * s[2]) /
                 2;
      // -->  f90(diff(P[1],F[8]));
      dP(0, 8) = -((2 * ds(0, 8) * F[7] - cste * ds(3, 8) * F[5]) * F[8] -
                   cste * ds(4, 8) * F[3] * F[7] + 2 * s[0] * F[7] +
                   cste * ds(4, 8) * F[1] * F[5] - cste * s[3] * F[5] +
                   cste * ds(3, 8) * F[2] * F[3] - 2 * ds(0, 8) * F[1] * F[2]) /
                 2;
      dP(1, 8) = ((cste * ds(3, 8) * F[6] - cste * F[0] * ds(5, 8)) * F[7] -
                  2 * ds(1, 8) * F[5] * F[6] + cste * ds(5, 8) * F[4] * F[5] -
                  cste * ds(3, 8) * F[2] * F[4] + 2 * F[0] * ds(1, 8) * F[2]) /
                 2;
      dP(2, 8) = ((cste * ds(4, 8) * F[4] - cste * F[0] * ds(5, 8)) * F[8] +
                  (cste * ds(5, 8) * F[3] - cste * ds(4, 8) * F[1]) * F[6] -
                  2 * ds(2, 8) * F[3] * F[4] + cste * s[4] * F[4] +
                  2 * F[0] * ds(2, 8) * F[1] - cste * F[0] * s[5]) /
                 2;
      dP(3, 8) =
          ((2 * ds(0, 8) * F[6] - cste * F[0] * ds(4, 8)) * F[7] -
           cste * ds(3, 8) * F[5] * F[6] + cste * ds(4, 8) * F[4] * F[5] -
           2 * ds(0, 8) * F[2] * F[4] + cste * F[0] * ds(3, 8) * F[2]) /
          2;
      dP(4, 8) = -((cste * ds(3, 8) * F[7] - 2 * ds(1, 8) * F[5]) * F[8] -
                   cste * ds(5, 8) * F[3] * F[7] + cste * s[3] * F[7] +
                   cste * ds(5, 8) * F[1] * F[5] - 2 * s[1] * F[5] +
                   2 * ds(1, 8) * F[2] * F[3] - cste * ds(3, 8) * F[1] * F[2]) /
                 2;
      dP(5, 8) = ((2 * ds(0, 8) * F[4] - cste * F[0] * ds(3, 8)) * F[8] +
                  (cste * ds(3, 8) * F[3] - 2 * ds(0, 8) * F[1]) * F[6] -
                  cste * ds(4, 8) * F[3] * F[4] + 2 * s[0] * F[4] +
                  cste * F[0] * ds(4, 8) * F[1] - cste * F[0] * s[3]) /
                 2;
      dP(6, 8) =
          -((cste * ds(4, 8) * F[7] - cste * ds(5, 8) * F[5]) * F[8] -
            2 * ds(2, 8) * F[3] * F[7] + cste * s[4] * F[7] +
            2 * ds(2, 8) * F[1] * F[5] - cste * s[5] * F[5] +
            cste * ds(5, 8) * F[2] * F[3] - cste * ds(4, 8) * F[1] * F[2]) /
          2;
      dP(7, 8) = ((cste * ds(3, 8) * F[4] - 2 * F[0] * ds(1, 8)) * F[8] +
                  (2 * ds(1, 8) * F[3] - cste * ds(3, 8) * F[1]) * F[6] -
                  cste * ds(5, 8) * F[3] * F[4] + cste * s[3] * F[4] +
                  cste * F[0] * ds(5, 8) * F[1] - 2 * F[0] * s[1]) /
                 2;
      dP(8, 8) =
          ((cste * ds(4, 8) * F[6] - 2 * F[0] * ds(2, 8)) * F[7] -
           cste * ds(5, 8) * F[5] * F[6] + 2 * ds(2, 8) * F[4] * F[5] -
           cste * ds(4, 8) * F[2] * F[4] + cste * F[0] * ds(5, 8) * F[2]) /
          2;
    }
  };  // end of struct DSIG_DF_to_DPK1_DF_Converter<3u>

  /*!
   * \brief an helper structure meant to be specialized in each spatial
   * dimension. The derivative of the first Piola-Kirchhoff stress
   * \f$P=\tau\,F^{-T}\f$ is computed column by column, i.e. component by
   * component of the deformation gradient:
   * \f[
   * \frac{\partial P_{iJ}}{\partial F_{pq}}=
   * \frac{\partial \tau_{ik}}{\partial F_{pq}}\,F^{-1}_{Jk}-
   * F^{-1}_{Jp}\,P_{iq}
   * \f]
   * \tparam N: spatial dimension
   */
  template <unsigned short N>
  struct DTAU_DF_to_DPK1_DF_Converter;

  /*!
   * \brief partial specialisation in 1D
   */
  template <>
  struct DTAU_DF_to_DPK1_DF_Converter<1u> {
    /*!
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] dtau: derivative of the Kirchhoff stress with respect to the
     *                  deformation gradient
     * \param[in] F: deformation gradient
     * \param[in] s: Cauchy stress
     */
    template <StressT2toT2Concept PK1StressDerivativeType,
              StressT2toST2Concept KirchhoffStressDerivativeType,
              NoUnitTensorConcept DeformationGradientType,
              StressStensorConcept CauchyStressType>
    requires(checkThatAllSpaceDimensionsAreEqual<PK1StressDerivativeType,
                                                 KirchhoffStressDerivativeType,
                                                 DeformationGradientType,
                                                 CauchyStressType>())  //
        TFEL_HOST_DEVICE static constexpr void                         //
        exe(PK1StressDerivativeType& dP,
            const KirchhoffStressDerivativeType& dtau,
            const DeformationGradientType& F,
            const CauchyStressType& s) noexcept {
      const auto iF = invert(F);
      const auto P = convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
      dP(0, 0) = dtau(0, 0) * iF[0] - iF[0] * P[0];
      dP(1, 0) = dtau(1, 0) * iF[1];
      dP(2, 0) = dtau(2, 0) * iF[2];
      dP(0, 1) = dtau(0, 1) * iF[0];
      dP(1, 1) = dtau(1, 1) * iF[1] - iF[1] * P[1];
      dP(2, 1) = dtau(2, 1) * iF[2];
      dP(0, 2) = dtau(0, 2) * iF[0];
      dP(1, 2) = dtau(1, 2) * iF[1];
      dP(2, 2) = dtau(2, 2) * iF[2] - iF[2] * P[2];
    }
  };  // end of struct DTAU_DF_to_DPK1_DF_Converter<1u>

  /*!
   * \brief partial specialisation in 2D
   */
  template <>
  struct DTAU_DF_to_DPK1_DF_Converter<2u> {
    /*!
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] dtau: derivative of the Kirchhoff stress with respect to the
     *                  deformation gradient
     * \param[in] F: deformation gradient
     * \param[in] s: Cauchy stress
     */
    template <StressT2toT2Concept PK1StressDerivativeType,
              StressT2toST2Concept KirchhoffStressDerivativeType,
              NoUnitTensorConcept DeformationGradientType,
              StressStensorConcept CauchyStressType>
    requires(checkThatAllSpaceDimensionsAreEqual<PK1StressDerivativeType,
                                                 KirchhoffStressDerivativeType,
                                                 DeformationGradientType,
                                                 CauchyStressType>())  //
        TFEL_HOST_DEVICE static constexpr void                         //
        exe(PK1StressDerivativeType& dP,
            const KirchhoffStressDerivativeType& dtau,
            const DeformationGradientType& F,
            const CauchyStressType& s) noexcept {
      using real = numeric_type<DeformationGradientType>;
      constexpr auto cste = Cste<real>::isqrt2;
      const auto iF = invert(F);
      const auto P = convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
      dP(0, 0) = dtau(0, 0) * iF[0] + cste * dtau(3, 0) * iF[3] - iF[0] * P[0];
      dP(1, 0) = cste * dtau(3, 0) * iF[4] + dtau(1, 0) * iF[1] - iF[4] * P[4];
      dP(2, 0) = dtau(2, 0) * iF[2];
      dP(3, 0) = dtau(0, 0) * iF[4] + cste * dtau(3, 0) * iF[1] - iF[4] * P[0];
      dP(4, 0) = cste * dtau(3, 0) * iF[0] + dtau(1, 0) * iF[3] - iF[0] * P[4];
      dP(0, 1) = dtau(0, 1) * iF[0] + cste * dtau(3, 1) * iF[3] - iF[3] * P[3];
      dP(1, 1) = cste * dtau(3, 1) * iF[4] + dtau(1, 1) * iF[1] - iF[1] * P[1];
      dP(2, 1) = dtau(2, 1) * iF[2];
      dP(3, 1) = dtau(0, 1) * iF[4] + cste * dtau(3, 1) * iF[1] - iF[1] * P[3];
      dP(4, 1) = cste * dtau(3, 1) * iF[0] + dtau(1, 1) * iF[3] - iF[3] * P[1];
      dP(0, 2) = dtau(0, 2) * iF[0] + cste * dtau(3, 2) * iF[3];
      dP(1, 2) = cste * dtau(3, 2) * iF[4] + dtau(1, 2) * iF[1];
      dP(2, 2) = dtau(2, 2) * iF[2] - iF[2] * P[2];
      dP(3, 2) = dtau(0, 2) * iF[4] + cste * dtau(3, 2) * iF[1];
      dP(4, 2) = cste * dtau(3, 2) * iF[0] + dtau(1, 2) * iF[3];
      dP(0, 3) = dtau(0, 3) * iF[0] + cste * dtau(3, 3) * iF[3] - iF[0] * P[3];
      dP(1, 3) = cste * dtau(3, 3) * iF[4] + dtau(1, 3) * iF[1] - iF[4] * P[1];
      dP(2, 3) = dtau(2, 3) * iF[2];
      dP(3, 3) = dtau(0, 3) * iF[4] + cste * dtau(3, 3) * iF[1] - iF[4] * P[3];
      dP(4, 3) = cste * dtau(3, 3) * iF[0] + dtau(1, 3) * iF[3] - iF[0] * P[1];
      dP(0, 4) = dtau(0, 4) * iF[0] + cste * dtau(3, 4) * iF[3] - iF[3] * P[0];
      dP(1, 4) = cste * dtau(3, 4) * iF[4] + dtau(1, 4) * iF[1] - iF[1] * P[4];
      dP(2, 4) = dtau(2, 4) * iF[2];
      dP(3, 4) = dtau(0, 4) * iF[4] + cste * dtau(3, 4) * iF[1] - iF[1] * P[0];
      dP(4, 4) = cste * dtau(3, 4) * iF[0] + dtau(1, 4) * iF[3] - iF[3] * P[4];
    }
  };  // end of struct DTAU_DF_to_DPK1_DF_Converter<2u>

  /*!
   * \brief partial specialisation in 3D
   */
  template <>
  struct DTAU_DF_to_DPK1_DF_Converter<3u> {
    /*!
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] dtau: derivative of the Kirchhoff stress with respect to the
     *                  deformation gradient
     * \param[in] F: deformation gradient
     * \param[in] s: Cauchy stress
     */
    template <StressT2toT2Concept PK1StressDerivativeType,
              StressT2toST2Concept KirchhoffStressDerivativeType,
              NoUnitTensorConcept DeformationGradientType,
              StressStensorConcept CauchyStressType>
    requires(checkThatAllSpaceDimensionsAreEqual<PK1StressDerivativeType,
                                                 KirchhoffStressDerivativeType,
                                                 DeformationGradientType,
                                                 CauchyStressType>())  //
        TFEL_HOST_DEVICE static constexpr void                         //
        exe(PK1StressDerivativeType& dP,
            const KirchhoffStressDerivativeType& dtau,
            const DeformationGradientType& F,
            const CauchyStressType& s) noexcept {
      using real = numeric_type<DeformationGradientType>;
      constexpr auto cste = Cste<real>::isqrt2;
      const auto iF = invert(F);
      const auto P = convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
      dP(0, 0) = dtau(0, 0) * iF[0] + cste * dtau(3, 0) * iF[3] +
                 cste * dtau(4, 0) * iF[5] - iF[0] * P[0];
      dP(1, 0) = cste * dtau(3, 0) * iF[4] + dtau(1, 0) * iF[1] +
                 cste * dtau(5, 0) * iF[7] - iF[4] * P[4];
      dP(2, 0) = cste * dtau(4, 0) * iF[6] + cste * dtau(5, 0) * iF[8] +
                 dtau(2, 0) * iF[2] - iF[6] * P[6];
      dP(3, 0) = dtau(0, 0) * iF[4] + cste * dtau(3, 0) * iF[1] +
                 cste * dtau(4, 0) * iF[7] - iF[4] * P[0];
      dP(4, 0) = cste * dtau(3, 0) * iF[0] + dtau(1, 0) * iF[3] +
                 cste * dtau(5, 0) * iF[5] - iF[0] * P[4];
      dP(5, 0) = dtau(0, 0) * iF[6] + cste * dtau(3, 0) * iF[8] +
                 cste * dtau(4, 0) * iF[2] - iF[6] * P[0];
      dP(6, 0) = cste * dtau(4, 0) * iF[0] + cste * dtau(5, 0) * iF[3] +
                 dtau(2, 0) * iF[5] - iF[0] * P[6];
      dP(7, 0) = cste * dtau(3, 0) * iF[6] + dtau(1, 0) * iF[8] +
                 cste * dtau(5, 0) * iF[2] - iF[6] * P[4];
      dP(8, 0) = cste * dtau(4, 0) * iF[4] + cste * dtau(5, 0) * iF[1] +
                 dtau(2, 0) * iF[7] - iF[4] * P[6];
      dP(0, 1) = dtau(0, 1) * iF[0] + cste * dtau(3, 1) * iF[3] +
                 cste * dtau(4, 1) * iF[5] - iF[3] * P[3];
      dP(1, 1) = cste * dtau(3, 1) * iF[4] + dtau(1, 1) * iF[1] +
                 cste * dtau(5, 1) * iF[7] - iF[1] * P[1];
      dP(2, 1) = cste * dtau(4, 1) * iF[6] + cste * dtau(5, 1) * iF[8] +
                 dtau(2, 1) * iF[2] - iF[8] * P[8];
      dP(3, 1) = dtau(0, 1) * iF[4] + cste * dtau(3, 1) * iF[1] +
                 cste * dtau(4, 1) * iF[7] - iF[1] * P[3];
      dP(4, 1) = cste * dtau(3, 1) * iF[0] + dtau(1, 1) * iF[3] +
                 cste * dtau(5, 1) * iF[5] - iF[3] * P[1];
      dP(5, 1) = dtau(0, 1) * iF[6] + cste * dtau(3, 1) * iF[8] +
                 cste * dtau(4, 1) * iF[2] - iF[8] * P[3];
      dP(6, 1) = cste * dtau(4, 1) * iF[0] + cste * dtau(5, 1) * iF[3] +
                 dtau(2, 1) * iF[5] - iF[3] * P[8];
      dP(7, 1) = cste * dtau(3, 1) * iF[6] + dtau(1, 1) * iF[8] +
                 cste * dtau(5, 1) * iF[2] - iF[8] * P[1];
      dP(8, 1) = cste * dtau(4, 1) * iF[4] + cste * dtau(5, 1) * iF[1] +
                 dtau(2, 1) * iF[7] - iF[1] * P[8];
      dP(0, 2) = dtau(0, 2) * iF[0] + cste * dtau(3, 2) * iF[3] +
                 cste * dtau(4, 2) * iF[5] - iF[5] * P[5];
      dP(1, 2) = cste * dtau(3, 2) * iF[4] + dtau(1, 2) * iF[1] +
                 cste * dtau(5, 2) * iF[7] - iF[7] * P[7];
      dP(2, 2) = cste * dtau(4, 2) * iF[6] + cste * dtau(5, 2) * iF[8] +
                 dtau(2, 2) * iF[2] - iF[2] * P[2];
      dP(3, 2) = dtau(0, 2) * iF[4] + cste * dtau(3, 2) * iF[1] +
                 cste * dtau(4, 2) * iF[7] - iF[7] * P[5];
      dP(4, 2) = cste * dtau(3, 2) * iF[0] + dtau(1, 2) * iF[3] +
                 cste * dtau(5, 2) * iF[5] - iF[5] * P[7];
      dP(5, 2) = dtau(0, 2) * iF[6] + cste * dtau(3, 2) * iF[8] +
                 cste * dtau(4, 2) * iF[2] - iF[2] * P[5];
      dP(6, 2) = cste * dtau(4, 2) * iF[0] + cste * dtau(5, 2) * iF[3] +
                 dtau(2, 2) * iF[5] - iF[5] * P[2];
      dP(7, 2) = cste * dtau(3, 2) * iF[6] + dtau(1, 2) * iF[8] +
                 cste * dtau(5, 2) * iF[2] - iF[2] * P[7];
      dP(8, 2) = cste * dtau(4, 2) * iF[4] + cste * dtau(5, 2) * iF[1] +
                 dtau(2, 2) * iF[7] - iF[7] * P[2];
      dP(0, 3) = dtau(0, 3) * iF[0] + cste * dtau(3, 3) * iF[3] +
                 cste * dtau(4, 3) * iF[5] - iF[0] * P[3];
      dP(1, 3) = cste * dtau(3, 3) * iF[4] + dtau(1, 3) * iF[1] +
                 cste * dtau(5, 3) * iF[7] - iF[4] * P[1];
      dP(2, 3) = cste * dtau(4, 3) * iF[6] + cste * dtau(5, 3) * iF[8] +
                 dtau(2, 3) * iF[2] - iF[6] * P[8];
      dP(3, 3) = dtau(0, 3) * iF[4] + cste * dtau(3, 3) * iF[1] +
                 cste * dtau(4, 3) * iF[7] - iF[4] * P[3];
      dP(4, 3) = cste * dtau(3, 3) * iF[0] + dtau(1, 3) * iF[3] +
                 cste * dtau(5, 3) * iF[5] - iF[0] * P[1];
      dP(5, 3) = dtau(0, 3) * iF[6] + cste * dtau(3, 3) * iF[8] +
                 cste * dtau(4, 3) * iF[2] - iF[6] * P[3];
      dP(6, 3) = cste * dtau(4, 3) * iF[0] + cste * dtau(5, 3) * iF[3] +
                 dtau(2, 3) * iF[5] - iF[0] * P[8];
      dP(7, 3) = cste * dtau(3, 3) * iF[6] + dtau(1, 3) * iF[8] +
                 cste * dtau(5, 3) * iF[2] - iF[6] * P[1];
      dP(8, 3) = cste * dtau(4, 3) * iF[4] + cste * dtau(5, 3) * iF[1] +
                 dtau(2, 3) * iF[7] - iF[4] * P[8];
      dP(0, 4) = dtau(0, 4) * iF[0] + cste * dtau(3, 4) * iF[3] +
                 cste * dtau(4, 4) * iF[5] - iF[3] * P[0];
      dP(1, 4) = cste * dtau(3, 4) * iF[4] + dtau(1, 4) * iF[1] +
                 cste * dtau(5, 4) * iF[7] - iF[1] * P[4];
      dP(2, 4) = cste * dtau(4, 4) * iF[6] + cste * dtau(5, 4) * iF[8] +
                 dtau(2, 4) * iF[2] - iF[8] * P[6];
      dP(3, 4) = dtau(0, 4) * iF[4] + cste * dtau(3, 4) * iF[1] +
                 cste * dtau(4, 4) * iF[7] - iF[1] * P[0];
      dP(4, 4) = cste * dtau(3, 4) * iF[0] + dtau(1, 4) * iF[3] +
                 cste * dtau(5, 4) * iF[5] - iF[3] * P[4];
      dP(5, 4) = dtau(0, 4) * iF[6] + cste * dtau(3, 4) * iF[8] +
                 cste * dtau(4, 4) * iF[2] - iF[8] * P[0];
      dP(6, 4) = cste * dtau(4, 4) * iF[0] + cste * dtau(5, 4) * iF[3] +
                 dtau(2, 4) * iF[5] - iF[3] * P[6];
      dP(7, 4) = cste * dtau(3, 4) * iF[6] + dtau(1, 4) * iF[8] +
                 cste * dtau(5, 4) * iF[2] - iF[8] * P[4];
      dP(8, 4) = cste * dtau(4, 4) * iF[4] + cste * dtau(5, 4) * iF[1] +
                 dtau(2, 4) * iF[7] - iF[1] * P[6];
      dP(0, 5) = dtau(0, 5) * iF[0] + cste * dtau(3, 5) * iF[3] +
                 cste * dtau(4, 5) * iF[5] - iF[0] * P[5];
      dP(1, 5) = cste * dtau(3, 5) * iF[4] + dtau(1, 5) * iF[1] +
                 cste * dtau(5, 5) * iF[7] - iF[4] * P[7];
      dP(2, 5) = cste * dtau(4, 5) * iF[6] + cste * dtau(5, 5) * iF[8] +
                 dtau(2, 5) * iF[2] - iF[6] * P[2];
      dP(3, 5) = dtau(0, 5) * iF[4] + cste * dtau(3, 5) * iF[1] +
                 cste * dtau(4, 5) * iF[7] - iF[4] * P[5];
      dP(4, 5) = cste * dtau(3, 5) * iF[0] + dtau(1, 5) * iF[3] +
                 cste * dtau(5, 5) * iF[5] - iF[0] * P[7];
      dP(5, 5) = dtau(0, 5) * iF[6] + cste * dtau(3, 5) * iF[8] +
                 cste * dtau(4, 5) * iF[2] - iF[6] * P[5];
      dP(6, 5) = cste * dtau(4, 5) * iF[0] + cste * dtau(5, 5) * iF[3] +
                 dtau(2, 5) * iF[5] - iF[0] * P[2];
      dP(7, 5) = cste * dtau(3, 5) * iF[6] + dtau(1, 5) * iF[8] +
                 cste * dtau(5, 5) * iF[2] - iF[6] * P[7];
      dP(8, 5) = cste * dtau(4, 5) * iF[4] + cste * dtau(5, 5) * iF[1] +
                 dtau(2, 5) * iF[7] - iF[4] * P[2];
      dP(0, 6) = dtau(0, 6) * iF[0] + cste * dtau(3, 6) * iF[3] +
                 cste * dtau(4, 6) * iF[5] - iF[5] * P[0];
      dP(1, 6) = cste * dtau(3, 6) * iF[4] + dtau(1, 6) * iF[1] +
                 cste * dtau(5, 6) * iF[7] - iF[7] * P[4];
      dP(2, 6) = cste * dtau(4, 6) * iF[6] + cste * dtau(5, 6) * iF[8] +
                 dtau(2, 6) * iF[2] - iF[2] * P[6];
      dP(3, 6) = dtau(0, 6) * iF[4] + cste * dtau(3, 6) * iF[1] +
                 cste * dtau(4, 6) * iF[7] - iF[7] * P[0];
      dP(4, 6) = cste * dtau(3, 6) * iF[0] + dtau(1, 6) * iF[3] +
                 cste * dtau(5, 6) * iF[5] - iF[5] * P[4];
      dP(5, 6) = dtau(0, 6) * iF[6] + cste * dtau(3, 6) * iF[8] +
                 cste * dtau(4, 6) * iF[2] - iF[2] * P[0];
      dP(6, 6) = cste * dtau(4, 6) * iF[0] + cste * dtau(5, 6) * iF[3] +
                 dtau(2, 6) * iF[5] - iF[5] * P[6];
      dP(7, 6) = cste * dtau(3, 6) * iF[6] + dtau(1, 6) * iF[8] +
                 cste * dtau(5, 6) * iF[2] - iF[2] * P[4];
      dP(8, 6) = cste * dtau(4, 6) * iF[4] + cste * dtau(5, 6) * iF[1] +
                 dtau(2, 6) * iF[7] - iF[7] * P[6];
      dP(0, 7) = dtau(0, 7) * iF[0] + cste * dtau(3, 7) * iF[3] +
                 cste * dtau(4, 7) * iF[5] - iF[3] * P[5];
      dP(1, 7) = cste * dtau(3, 7) * iF[4] + dtau(1, 7) * iF[1] +
                 cste * dtau(5, 7) * iF[7] - iF[1] * P[7];
      dP(2, 7) = cste * dtau(4, 7) * iF[6] + cste * dtau(5, 7) * iF[8] +
                 dtau(2, 7) * iF[2] - iF[8] * P[2];
      dP(3, 7) = dtau(0, 7) * iF[4] + cste * dtau(3, 7) * iF[1] +
                 cste * dtau(4, 7) * iF[7] - iF[1] * P[5];
      dP(4, 7) = cste * dtau(3, 7) * iF[0] + dtau(1, 7) * iF[3] +
                 cste * dtau(5, 7) * iF[5] - iF[3] * P[7];
      dP(5, 7) = dtau(0, 7) * iF[6] + cste * dtau(3, 7) * iF[8] +
                 cste * dtau(4, 7) * iF[2] - iF[8] * P[5];
      dP(6, 7) = cste * dtau(4, 7) * iF[0] + cste * dtau(5, 7) * iF[3] +
                 dtau(2, 7) * iF[5] - iF[3] * P[2];
      dP(7, 7) = cste * dtau(3, 7) * iF[6] + dtau(1, 7) * iF[8] +
                 cste * dtau(5, 7) * iF[2] - iF[8] * P[7];
      dP(8, 7) = cste * dtau(4, 7) * iF[4] + cste * dtau(5, 7) * iF[1] +
                 dtau(2, 7) * iF[7] - iF[1] * P[2];
      dP(0, 8) = dtau(0, 8) * iF[0] + cste * dtau(3, 8) * iF[3] +
                 cste * dtau(4, 8) * iF[5] - iF[5] * P[3];
      dP(1, 8) = cste * dtau(3, 8) * iF[4] + dtau(1, 8) * iF[1] +
                 cste * dtau(5, 8) * iF[7] - iF[7] * P[1];
      dP(2, 8) = cste * dtau(4, 8) * iF[6] + cste * dtau(5, 8) * iF[8] +
                 dtau(2, 8) * iF[2] - iF[2] * P[8];
      dP(3, 8) = dtau(0, 8) * iF[4] + cste * dtau(3, 8) * iF[1] +
                 cste * dtau(4, 8) * iF[7] - iF[7] * P[3];
      dP(4, 8) = cste * dtau(3, 8) * iF[0] + dtau(1, 8) * iF[3] +
                 cste * dtau(5, 8) * iF[5] - iF[5] * P[1];
      dP(5, 8) = dtau(0, 8) * iF[6] + cste * dtau(3, 8) * iF[8] +
                 cste * dtau(4, 8) * iF[2] - iF[2] * P[3];
      dP(6, 8) = cste * dtau(4, 8) * iF[0] + cste * dtau(5, 8) * iF[3] +
                 dtau(2, 8) * iF[5] - iF[5] * P[8];
      dP(7, 8) = cste * dtau(3, 8) * iF[6] + dtau(1, 8) * iF[8] +
                 cste * dtau(5, 8) * iF[2] - iF[2] * P[1];
      dP(8, 8) = cste * dtau(4, 8) * iF[4] + cste * dtau(5, 8) * iF[1] +
                 dtau(2, 8) * iF[7] - iF[7] * P[8];
    }
  };  // end of struct DTAU_DF_to_DPK1_DF_Converter<3u>

}  // end of namespace tfel::math::internals

namespace tfel::math {

  template <StressT2toST2Concept CauchyStressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<CauchyStressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      [[nodiscard]] TFEL_HOST_DEVICE constexpr                       //
      t2tot2<space_dimension<CauchyStressDerivativeType>,
             numeric_type<CauchyStressDerivativeType>>  //
      convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
          const CauchyStressDerivativeType& ds,
          const DeformationGradientType& F,
          const CauchyStressType& s) noexcept {
    constexpr auto N = space_dimension<CauchyStressDerivativeType>;
    t2tot2<N, numeric_type<CauchyStressDerivativeType>> dP;
    tfel::math::internals::DSIG_DF_to_DPK1_DF_Converter<N>::exe(dP, ds, F, s);
    return dP;
  }  // end of
     // convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative

  template <StressT2toT2Concept PK1StressDerivativeType,
            StressT2toST2Concept CauchyStressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<PK1StressDerivativeType,
                                               CauchyStressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      TFEL_HOST_DEVICE constexpr void                                //
      convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
          PK1StressDerivativeType& dP,
          const CauchyStressDerivativeType& ds,
          const DeformationGradientType& F,
          const CauchyStressType& s) noexcept {
    constexpr auto N = space_dimension<CauchyStressDerivativeType>;
    tfel::math::internals::DSIG_DF_to_DPK1_DF_Converter<N>::exe(dP, ds, F, s);
  }  // end of
     // convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative

  template <StressT2toST2Concept KirchhoffStressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<KirchhoffStressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      [[nodiscard]] TFEL_HOST_DEVICE constexpr                       //
      t2tot2<space_dimension<KirchhoffStressDerivativeType>,
             numeric_type<KirchhoffStressDerivativeType>>  //
      convertKirchhoffStressDerivativeToFirstPiolaKirchhoffStressDerivative(
          const KirchhoffStressDerivativeType& dtau,
          const DeformationGradientType& F,
          const CauchyStressType& s) noexcept {
    constexpr auto N = space_dimension<KirchhoffStressDerivativeType>;
    t2tot2<N, numeric_type<KirchhoffStressDerivativeType>> dP;
    tfel::math::internals::DTAU_DF_to_DPK1_DF_Converter<N>::exe(dP, dtau, F,
                                                                s);
    return dP;
  }  // end of
     // convertKirchhoffStressDerivativeToFirstPiolaKirchhoffStressDerivative

  template <StressT2toT2Concept PK1StressDerivativeType,
            StressT2toST2Concept KirchhoffStressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<PK1StressDerivativeType,
                                               KirchhoffStressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      TFEL_HOST_DEVICE constexpr void                                //
      convertKirchhoffStressDerivativeToFirstPiolaKirchhoffStressDerivative(
          PK1StressDerivativeType& dP,
          const KirchhoffStressDerivativeType& dtau,
          const DeformationGradientType& F,
          const CauchyStressType& s) noexcept {
    constexpr auto N = space_dimension<KirchhoffStressDerivativeType>;
    tfel::math::internals::DTAU_DF_to_DPK1_DF_Converter<N>::exe(dP, dtau, F,
                                                                s);
  }  // end of
     // convertKirchhoffStressDerivativeToFirstPiolaKirchhoffStressDerivative

  template <StressST2toST2Concept PK2StressDerivativeType,
            NoUnitTensorConcept DeformationGradientType,
            StressStensorConcept CauchyStressType>
  requires(checkThatAllSpaceDimensionsAreEqual<PK2StressDerivativeType,
                                               DeformationGradientType,
                                               CauchyStressType>())  //
      [[nodiscard]] TFEL_HOST_DEVICE constexpr                       //
      t2tot2<space_dimension<PK2StressDerivativeType>,
             numeric_type<PK2StressDerivativeType>>  //
      convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
          const PK2StressDerivativeType& dS,
          const DeformationGradientType& F,
          const CauchyStressType& s) noexcept {
    constexpr auto N = space_dimension<PK2StressDerivativeType>;
    t2tot2<N, numeric_type<PK2StressDerivativeType>> dP;
    convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
        dP, dS, F, s);
    return dP;
  }  // end of
     // convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative

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
          PK1StressDerivativeType& dP,
          const PK2StressDerivativeType& dS,
          const DeformationGradientType& F,
          const CauchyStressType& s) noexcept {
    using real = numeric_type<DeformationGradientType>;
    using stress = numeric_type<CauchyStressType>;
    constexpr auto N = space_dimension<PK1StressDerivativeType>;
    const auto dE_dF = eval(t2tost2<N, real>::dCdF(F) / 2);
    const auto dS_dF = t2tot2<N, stress>{dS * dE_dF};
    const auto S =
        unsyme(convertCauchyStressToSecondPiolaKirchhoffStress(s, F));
    dP = t2tot2<N, stress>::tpld(S) + t2tot2<N, stress>::tprd(F, dS_dF);
  }  // end of
     // convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOT2_CONVERTTOPK1DERIVATIVE_IXX */
