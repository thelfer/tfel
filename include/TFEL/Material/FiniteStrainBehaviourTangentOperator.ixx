/*!
 * \file   FiniteStrainBehaviourTangentOperator.ixx
 * \brief
 * \author Thomas Helfer
 * \date   19 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX
#define LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX

#include <stdexcept>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/ST2toST2/ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx"
#include "TFEL/Math/T2toT2/ConvertToPK1Derivative.hxx"
#include "TFEL/Math/T2toT2/ConvertFromPK1Derivative.hxx"
#include "TFEL/Material/LogarithmicStrainHandler.hxx"

/*!
 * \brief a convenient macro for shorted declaration of partial
 * specialisation of the FiniteStrainBehaviourTangentOperatorConverter
 * structure
 */
#define TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(OP1, OP2) \
  template <>                                                                 \
  struct FiniteStrainBehaviourTangentOperatorConverter<                       \
      FiniteStrainBehaviourTangentOperatorBase::OP1,                          \
      FiniteStrainBehaviourTangentOperatorBase::OP2>                          \
      : public FiniteStrainBehaviourTangentOperatorConverterBase<             \
            FiniteStrainBehaviourTangentOperatorBase::OP1,                    \
            FiniteStrainBehaviourTangentOperatorBase::OP2>

namespace tfel::material::internals {

  /*!
   * \brief an helper structure meant to be specialized in each spatial
   * dimension, converting the derivative of the Kirchhoff stress with
   * respect to the deformation gradient increment
   * \f$\Delta\,F=F\,F_{0}^{-1}\f$ to the derivative of the first
   * Piola-Kirchhoff stress \f$P=\tau\,F^{-T}\f$ with respect to the
   * deformation gradient. The latter is computed column by column, i.e.
   * component by component of the deformation gradient:
   * \f[
   * \frac{\partial \tau_{ik}}{\partial F_{pq}}=
   * \frac{\partial \tau_{ik}}{\partial \Delta\,F_{pm}}\,
   * F^{-1}_{0\,qm}
   * \quad\mathrm{and}\quad
   * \frac{\partial P_{iJ}}{\partial F_{pq}}=
   * \frac{\partial \tau_{ik}}{\partial F_{pq}}\,F^{-1}_{Jk}-
   * F^{-1}_{Jp}\,P_{iq}
   * \f]
   * so that the derivative of the Kirchhoff stress with respect to the
   * deformation gradient is never stored.
   * \tparam N: spatial dimension
   */
  template <unsigned short N>
  struct DTAU_DDF_to_DPK1_DF_Converter;

  /*!
   * \brief partial specialisation in 1D
   */
  template <>
  struct DTAU_DDF_to_DPK1_DF_Converter<1u> {
    /*!
     * \tparam stress: stress type
     * \tparam real: numeric type
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] dtau: derivative of the Kirchhoff stress with respect to the
     *                  deformation gradient increment
     * \param[in] F0: deformation gradient at the beginning of the time step
     * \param[in] F: deformation gradient at the end of the time step
     * \param[in] s: Cauchy stress
     */
    template <typename stress, typename real>
    static constexpr void exe(
        tfel::math::t2tot2<1u, stress>& dP,
        const tfel::math::t2tost2<1u, stress>& dtau,
        const tfel::math::tensor<1u, real>& F0,
        const tfel::math::tensor<1u, real>& F,
        const tfel::math::stensor<1u, stress>& s) noexcept {
      const auto iF0 = tfel::math::invert(F0);
      const auto iF = tfel::math::invert(F);
      const auto P =
          tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
      {
        // derivative of the Kirchhoff stress with respect to F(0, 0)
        const auto a0 = dtau(0, 0) * iF0[0];
        const auto a1 = dtau(1, 0) * iF0[0];
        const auto a2 = dtau(2, 0) * iF0[0];
        dP(0, 0) = a0 * iF[0] - iF[0] * P[0];
        dP(1, 0) = a1 * iF[1];
        dP(2, 0) = a2 * iF[2];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(1, 1)
        const auto a0 = dtau(0, 1) * iF0[1];
        const auto a1 = dtau(1, 1) * iF0[1];
        const auto a2 = dtau(2, 1) * iF0[1];
        dP(0, 1) = a0 * iF[0];
        dP(1, 1) = a1 * iF[1] - iF[1] * P[1];
        dP(2, 1) = a2 * iF[2];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(2, 2)
        const auto a0 = dtau(0, 2) * iF0[2];
        const auto a1 = dtau(1, 2) * iF0[2];
        const auto a2 = dtau(2, 2) * iF0[2];
        dP(0, 2) = a0 * iF[0];
        dP(1, 2) = a1 * iF[1];
        dP(2, 2) = a2 * iF[2] - iF[2] * P[2];
      }
    }
  };  // end of struct DTAU_DDF_to_DPK1_DF_Converter<1u>

  /*!
   * \brief partial specialisation in 2D
   */
  template <>
  struct DTAU_DDF_to_DPK1_DF_Converter<2u> {
    /*!
     * \tparam stress: stress type
     * \tparam real: numeric type
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] dtau: derivative of the Kirchhoff stress with respect to the
     *                  deformation gradient increment
     * \param[in] F0: deformation gradient at the beginning of the time step
     * \param[in] F: deformation gradient at the end of the time step
     * \param[in] s: Cauchy stress
     */
    template <typename stress, typename real>
    static constexpr void exe(
        tfel::math::t2tot2<2u, stress>& dP,
        const tfel::math::t2tost2<2u, stress>& dtau,
        const tfel::math::tensor<2u, real>& F0,
        const tfel::math::tensor<2u, real>& F,
        const tfel::math::stensor<2u, stress>& s) noexcept {
      constexpr auto cste = tfel::math::Cste<real>::isqrt2;
      const auto iF0 = tfel::math::invert(F0);
      const auto iF = tfel::math::invert(F);
      const auto P =
          tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
      {
        // derivative of the Kirchhoff stress with respect to F(0, 0)
        const auto a0 = dtau(0, 0) * iF0[0] + dtau(0, 3) * iF0[3];
        const auto a1 = dtau(1, 0) * iF0[0] + dtau(1, 3) * iF0[3];
        const auto a2 = dtau(2, 0) * iF0[0] + dtau(2, 3) * iF0[3];
        const auto a3 = dtau(3, 0) * iF0[0] + dtau(3, 3) * iF0[3];
        dP(0, 0) = a0 * iF[0] + cste * a3 * iF[3] - iF[0] * P[0];
        dP(1, 0) = cste * a3 * iF[4] + a1 * iF[1] - iF[4] * P[4];
        dP(2, 0) = a2 * iF[2];
        dP(3, 0) = a0 * iF[4] + cste * a3 * iF[1] - iF[4] * P[0];
        dP(4, 0) = cste * a3 * iF[0] + a1 * iF[3] - iF[0] * P[4];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(1, 1)
        const auto a0 = dtau(0, 4) * iF0[4] + dtau(0, 1) * iF0[1];
        const auto a1 = dtau(1, 4) * iF0[4] + dtau(1, 1) * iF0[1];
        const auto a2 = dtau(2, 4) * iF0[4] + dtau(2, 1) * iF0[1];
        const auto a3 = dtau(3, 4) * iF0[4] + dtau(3, 1) * iF0[1];
        dP(0, 1) = a0 * iF[0] + cste * a3 * iF[3] - iF[3] * P[3];
        dP(1, 1) = cste * a3 * iF[4] + a1 * iF[1] - iF[1] * P[1];
        dP(2, 1) = a2 * iF[2];
        dP(3, 1) = a0 * iF[4] + cste * a3 * iF[1] - iF[1] * P[3];
        dP(4, 1) = cste * a3 * iF[0] + a1 * iF[3] - iF[3] * P[1];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(2, 2)
        const auto a0 = dtau(0, 2) * iF0[2];
        const auto a1 = dtau(1, 2) * iF0[2];
        const auto a2 = dtau(2, 2) * iF0[2];
        const auto a3 = dtau(3, 2) * iF0[2];
        dP(0, 2) = a0 * iF[0] + cste * a3 * iF[3];
        dP(1, 2) = cste * a3 * iF[4] + a1 * iF[1];
        dP(2, 2) = a2 * iF[2] - iF[2] * P[2];
        dP(3, 2) = a0 * iF[4] + cste * a3 * iF[1];
        dP(4, 2) = cste * a3 * iF[0] + a1 * iF[3];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(0, 1)
        const auto a0 = dtau(0, 0) * iF0[4] + dtau(0, 3) * iF0[1];
        const auto a1 = dtau(1, 0) * iF0[4] + dtau(1, 3) * iF0[1];
        const auto a2 = dtau(2, 0) * iF0[4] + dtau(2, 3) * iF0[1];
        const auto a3 = dtau(3, 0) * iF0[4] + dtau(3, 3) * iF0[1];
        dP(0, 3) = a0 * iF[0] + cste * a3 * iF[3] - iF[0] * P[3];
        dP(1, 3) = cste * a3 * iF[4] + a1 * iF[1] - iF[4] * P[1];
        dP(2, 3) = a2 * iF[2];
        dP(3, 3) = a0 * iF[4] + cste * a3 * iF[1] - iF[4] * P[3];
        dP(4, 3) = cste * a3 * iF[0] + a1 * iF[3] - iF[0] * P[1];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(1, 0)
        const auto a0 = dtau(0, 4) * iF0[0] + dtau(0, 1) * iF0[3];
        const auto a1 = dtau(1, 4) * iF0[0] + dtau(1, 1) * iF0[3];
        const auto a2 = dtau(2, 4) * iF0[0] + dtau(2, 1) * iF0[3];
        const auto a3 = dtau(3, 4) * iF0[0] + dtau(3, 1) * iF0[3];
        dP(0, 4) = a0 * iF[0] + cste * a3 * iF[3] - iF[3] * P[0];
        dP(1, 4) = cste * a3 * iF[4] + a1 * iF[1] - iF[1] * P[4];
        dP(2, 4) = a2 * iF[2];
        dP(3, 4) = a0 * iF[4] + cste * a3 * iF[1] - iF[1] * P[0];
        dP(4, 4) = cste * a3 * iF[0] + a1 * iF[3] - iF[3] * P[4];
      }
    }
  };  // end of struct DTAU_DDF_to_DPK1_DF_Converter<2u>

  /*!
   * \brief partial specialisation in 3D
   */
  template <>
  struct DTAU_DDF_to_DPK1_DF_Converter<3u> {
    /*!
     * \tparam stress: stress type
     * \tparam real: numeric type
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] dtau: derivative of the Kirchhoff stress with respect to the
     *                  deformation gradient increment
     * \param[in] F0: deformation gradient at the beginning of the time step
     * \param[in] F: deformation gradient at the end of the time step
     * \param[in] s: Cauchy stress
     */
    template <typename stress, typename real>
    static constexpr void exe(
        tfel::math::t2tot2<3u, stress>& dP,
        const tfel::math::t2tost2<3u, stress>& dtau,
        const tfel::math::tensor<3u, real>& F0,
        const tfel::math::tensor<3u, real>& F,
        const tfel::math::stensor<3u, stress>& s) noexcept {
      constexpr auto cste = tfel::math::Cste<real>::isqrt2;
      const auto iF0 = tfel::math::invert(F0);
      const auto iF = tfel::math::invert(F);
      const auto P =
          tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
      {
        // derivative of the Kirchhoff stress with respect to F(0, 0)
        const auto a0 =
            dtau(0, 0) * iF0[0] + dtau(0, 3) * iF0[3] + dtau(0, 5) * iF0[5];
        const auto a1 =
            dtau(1, 0) * iF0[0] + dtau(1, 3) * iF0[3] + dtau(1, 5) * iF0[5];
        const auto a2 =
            dtau(2, 0) * iF0[0] + dtau(2, 3) * iF0[3] + dtau(2, 5) * iF0[5];
        const auto a3 =
            dtau(3, 0) * iF0[0] + dtau(3, 3) * iF0[3] + dtau(3, 5) * iF0[5];
        const auto a4 =
            dtau(4, 0) * iF0[0] + dtau(4, 3) * iF0[3] + dtau(4, 5) * iF0[5];
        const auto a5 =
            dtau(5, 0) * iF0[0] + dtau(5, 3) * iF0[3] + dtau(5, 5) * iF0[5];
        dP(0, 0) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[0] * P[0];
        dP(1, 0) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[4] * P[4];
        dP(2, 0) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[6] * P[6];
        dP(3, 0) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[4] * P[0];
        dP(4, 0) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[0] * P[4];
        dP(5, 0) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[6] * P[0];
        dP(6, 0) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[0] * P[6];
        dP(7, 0) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[6] * P[4];
        dP(8, 0) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[4] * P[6];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(1, 1)
        const auto a0 =
            dtau(0, 4) * iF0[4] + dtau(0, 1) * iF0[1] + dtau(0, 7) * iF0[7];
        const auto a1 =
            dtau(1, 4) * iF0[4] + dtau(1, 1) * iF0[1] + dtau(1, 7) * iF0[7];
        const auto a2 =
            dtau(2, 4) * iF0[4] + dtau(2, 1) * iF0[1] + dtau(2, 7) * iF0[7];
        const auto a3 =
            dtau(3, 4) * iF0[4] + dtau(3, 1) * iF0[1] + dtau(3, 7) * iF0[7];
        const auto a4 =
            dtau(4, 4) * iF0[4] + dtau(4, 1) * iF0[1] + dtau(4, 7) * iF0[7];
        const auto a5 =
            dtau(5, 4) * iF0[4] + dtau(5, 1) * iF0[1] + dtau(5, 7) * iF0[7];
        dP(0, 1) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[3] * P[3];
        dP(1, 1) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[1] * P[1];
        dP(2, 1) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[8] * P[8];
        dP(3, 1) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[1] * P[3];
        dP(4, 1) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[3] * P[1];
        dP(5, 1) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[8] * P[3];
        dP(6, 1) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[3] * P[8];
        dP(7, 1) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[8] * P[1];
        dP(8, 1) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[1] * P[8];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(2, 2)
        const auto a0 =
            dtau(0, 6) * iF0[6] + dtau(0, 8) * iF0[8] + dtau(0, 2) * iF0[2];
        const auto a1 =
            dtau(1, 6) * iF0[6] + dtau(1, 8) * iF0[8] + dtau(1, 2) * iF0[2];
        const auto a2 =
            dtau(2, 6) * iF0[6] + dtau(2, 8) * iF0[8] + dtau(2, 2) * iF0[2];
        const auto a3 =
            dtau(3, 6) * iF0[6] + dtau(3, 8) * iF0[8] + dtau(3, 2) * iF0[2];
        const auto a4 =
            dtau(4, 6) * iF0[6] + dtau(4, 8) * iF0[8] + dtau(4, 2) * iF0[2];
        const auto a5 =
            dtau(5, 6) * iF0[6] + dtau(5, 8) * iF0[8] + dtau(5, 2) * iF0[2];
        dP(0, 2) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[5] * P[5];
        dP(1, 2) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[7] * P[7];
        dP(2, 2) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[2] * P[2];
        dP(3, 2) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[7] * P[5];
        dP(4, 2) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[5] * P[7];
        dP(5, 2) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[2] * P[5];
        dP(6, 2) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[5] * P[2];
        dP(7, 2) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[2] * P[7];
        dP(8, 2) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[7] * P[2];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(0, 1)
        const auto a0 =
            dtau(0, 0) * iF0[4] + dtau(0, 3) * iF0[1] + dtau(0, 5) * iF0[7];
        const auto a1 =
            dtau(1, 0) * iF0[4] + dtau(1, 3) * iF0[1] + dtau(1, 5) * iF0[7];
        const auto a2 =
            dtau(2, 0) * iF0[4] + dtau(2, 3) * iF0[1] + dtau(2, 5) * iF0[7];
        const auto a3 =
            dtau(3, 0) * iF0[4] + dtau(3, 3) * iF0[1] + dtau(3, 5) * iF0[7];
        const auto a4 =
            dtau(4, 0) * iF0[4] + dtau(4, 3) * iF0[1] + dtau(4, 5) * iF0[7];
        const auto a5 =
            dtau(5, 0) * iF0[4] + dtau(5, 3) * iF0[1] + dtau(5, 5) * iF0[7];
        dP(0, 3) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[0] * P[3];
        dP(1, 3) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[4] * P[1];
        dP(2, 3) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[6] * P[8];
        dP(3, 3) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[4] * P[3];
        dP(4, 3) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[0] * P[1];
        dP(5, 3) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[6] * P[3];
        dP(6, 3) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[0] * P[8];
        dP(7, 3) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[6] * P[1];
        dP(8, 3) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[4] * P[8];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(1, 0)
        const auto a0 =
            dtau(0, 4) * iF0[0] + dtau(0, 1) * iF0[3] + dtau(0, 7) * iF0[5];
        const auto a1 =
            dtau(1, 4) * iF0[0] + dtau(1, 1) * iF0[3] + dtau(1, 7) * iF0[5];
        const auto a2 =
            dtau(2, 4) * iF0[0] + dtau(2, 1) * iF0[3] + dtau(2, 7) * iF0[5];
        const auto a3 =
            dtau(3, 4) * iF0[0] + dtau(3, 1) * iF0[3] + dtau(3, 7) * iF0[5];
        const auto a4 =
            dtau(4, 4) * iF0[0] + dtau(4, 1) * iF0[3] + dtau(4, 7) * iF0[5];
        const auto a5 =
            dtau(5, 4) * iF0[0] + dtau(5, 1) * iF0[3] + dtau(5, 7) * iF0[5];
        dP(0, 4) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[3] * P[0];
        dP(1, 4) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[1] * P[4];
        dP(2, 4) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[8] * P[6];
        dP(3, 4) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[1] * P[0];
        dP(4, 4) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[3] * P[4];
        dP(5, 4) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[8] * P[0];
        dP(6, 4) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[3] * P[6];
        dP(7, 4) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[8] * P[4];
        dP(8, 4) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[1] * P[6];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(0, 2)
        const auto a0 =
            dtau(0, 0) * iF0[6] + dtau(0, 3) * iF0[8] + dtau(0, 5) * iF0[2];
        const auto a1 =
            dtau(1, 0) * iF0[6] + dtau(1, 3) * iF0[8] + dtau(1, 5) * iF0[2];
        const auto a2 =
            dtau(2, 0) * iF0[6] + dtau(2, 3) * iF0[8] + dtau(2, 5) * iF0[2];
        const auto a3 =
            dtau(3, 0) * iF0[6] + dtau(3, 3) * iF0[8] + dtau(3, 5) * iF0[2];
        const auto a4 =
            dtau(4, 0) * iF0[6] + dtau(4, 3) * iF0[8] + dtau(4, 5) * iF0[2];
        const auto a5 =
            dtau(5, 0) * iF0[6] + dtau(5, 3) * iF0[8] + dtau(5, 5) * iF0[2];
        dP(0, 5) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[0] * P[5];
        dP(1, 5) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[4] * P[7];
        dP(2, 5) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[6] * P[2];
        dP(3, 5) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[4] * P[5];
        dP(4, 5) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[0] * P[7];
        dP(5, 5) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[6] * P[5];
        dP(6, 5) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[0] * P[2];
        dP(7, 5) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[6] * P[7];
        dP(8, 5) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[4] * P[2];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(2, 0)
        const auto a0 =
            dtau(0, 6) * iF0[0] + dtau(0, 8) * iF0[3] + dtau(0, 2) * iF0[5];
        const auto a1 =
            dtau(1, 6) * iF0[0] + dtau(1, 8) * iF0[3] + dtau(1, 2) * iF0[5];
        const auto a2 =
            dtau(2, 6) * iF0[0] + dtau(2, 8) * iF0[3] + dtau(2, 2) * iF0[5];
        const auto a3 =
            dtau(3, 6) * iF0[0] + dtau(3, 8) * iF0[3] + dtau(3, 2) * iF0[5];
        const auto a4 =
            dtau(4, 6) * iF0[0] + dtau(4, 8) * iF0[3] + dtau(4, 2) * iF0[5];
        const auto a5 =
            dtau(5, 6) * iF0[0] + dtau(5, 8) * iF0[3] + dtau(5, 2) * iF0[5];
        dP(0, 6) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[5] * P[0];
        dP(1, 6) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[7] * P[4];
        dP(2, 6) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[2] * P[6];
        dP(3, 6) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[7] * P[0];
        dP(4, 6) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[5] * P[4];
        dP(5, 6) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[2] * P[0];
        dP(6, 6) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[5] * P[6];
        dP(7, 6) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[2] * P[4];
        dP(8, 6) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[7] * P[6];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(1, 2)
        const auto a0 =
            dtau(0, 4) * iF0[6] + dtau(0, 1) * iF0[8] + dtau(0, 7) * iF0[2];
        const auto a1 =
            dtau(1, 4) * iF0[6] + dtau(1, 1) * iF0[8] + dtau(1, 7) * iF0[2];
        const auto a2 =
            dtau(2, 4) * iF0[6] + dtau(2, 1) * iF0[8] + dtau(2, 7) * iF0[2];
        const auto a3 =
            dtau(3, 4) * iF0[6] + dtau(3, 1) * iF0[8] + dtau(3, 7) * iF0[2];
        const auto a4 =
            dtau(4, 4) * iF0[6] + dtau(4, 1) * iF0[8] + dtau(4, 7) * iF0[2];
        const auto a5 =
            dtau(5, 4) * iF0[6] + dtau(5, 1) * iF0[8] + dtau(5, 7) * iF0[2];
        dP(0, 7) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[3] * P[5];
        dP(1, 7) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[1] * P[7];
        dP(2, 7) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[8] * P[2];
        dP(3, 7) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[1] * P[5];
        dP(4, 7) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[3] * P[7];
        dP(5, 7) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[8] * P[5];
        dP(6, 7) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[3] * P[2];
        dP(7, 7) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[8] * P[7];
        dP(8, 7) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[1] * P[2];
      }
      {
        // derivative of the Kirchhoff stress with respect to F(2, 1)
        const auto a0 =
            dtau(0, 6) * iF0[4] + dtau(0, 8) * iF0[1] + dtau(0, 2) * iF0[7];
        const auto a1 =
            dtau(1, 6) * iF0[4] + dtau(1, 8) * iF0[1] + dtau(1, 2) * iF0[7];
        const auto a2 =
            dtau(2, 6) * iF0[4] + dtau(2, 8) * iF0[1] + dtau(2, 2) * iF0[7];
        const auto a3 =
            dtau(3, 6) * iF0[4] + dtau(3, 8) * iF0[1] + dtau(3, 2) * iF0[7];
        const auto a4 =
            dtau(4, 6) * iF0[4] + dtau(4, 8) * iF0[1] + dtau(4, 2) * iF0[7];
        const auto a5 =
            dtau(5, 6) * iF0[4] + dtau(5, 8) * iF0[1] + dtau(5, 2) * iF0[7];
        dP(0, 8) =
            a0 * iF[0] + cste * a3 * iF[3] + cste * a4 * iF[5] - iF[5] * P[3];
        dP(1, 8) =
            cste * a3 * iF[4] + a1 * iF[1] + cste * a5 * iF[7] - iF[7] * P[1];
        dP(2, 8) =
            cste * a4 * iF[6] + cste * a5 * iF[8] + a2 * iF[2] - iF[2] * P[8];
        dP(3, 8) =
            a0 * iF[4] + cste * a3 * iF[1] + cste * a4 * iF[7] - iF[7] * P[3];
        dP(4, 8) =
            cste * a3 * iF[0] + a1 * iF[3] + cste * a5 * iF[5] - iF[5] * P[1];
        dP(5, 8) =
            a0 * iF[6] + cste * a3 * iF[8] + cste * a4 * iF[2] - iF[2] * P[3];
        dP(6, 8) =
            cste * a4 * iF[0] + cste * a5 * iF[3] + a2 * iF[5] - iF[5] * P[8];
        dP(7, 8) =
            cste * a3 * iF[6] + a1 * iF[8] + cste * a5 * iF[2] - iF[2] * P[1];
        dP(8, 8) =
            cste * a4 * iF[4] + cste * a5 * iF[1] + a2 * iF[7] - iF[7] * P[8];
      }
    }
  };  // end of struct DTAU_DDF_to_DPK1_DF_Converter<3u>

}  // end of namespace tfel::material::internals

namespace tfel::material {

  /*!
   * \brief An helper class used to introduce some convenient aliases
   */
  template <FiniteStrainBehaviourTangentOperatorBase::Flag ResultFlag,
            FiniteStrainBehaviourTangentOperatorBase::Flag SourceFlag>
  struct FiniteStrainBehaviourTangentOperatorConverterBase {
    //! a simple alias
    template <typename T>
    using base_type = tfel::typetraits::base_type<T>;
    //! a simple alias
    template <unsigned short N, typename stress>
    using Result = tangent_operator<ResultFlag, N, stress>;
    //! a simple alias
    template <unsigned short N, typename stress>
    using Source = tangent_operator<SourceFlag, N, stress>;
    //! a simple alias
    template <unsigned short N, typename stress>
    using DeformationGradientTensor = tfel::math::tensor<N, base_type<stress>>;
    //! a simple alias
    template <unsigned short N, typename stress>
    using StressStensor = tfel::math::stensor<N, stress>;
    //! a simple alias
    using size_type = unsigned short;

   protected:
    /*!
     * \return the vector index associated with a the matrix indexes
     * of a symmetric tensor
     * \param[in] i: row index
     * \param[in] j: column index
     */
    TFEL_MATERIAL_INLINE static size_type index(const size_type i,
                                                const size_type j) {
      // i,j are valid for the space dimension considered
      if (i == j) {
        return i;
      } else if ((i == 0) && (j == 1)) {
        return 3;
      } else if ((i == 1) && (j == 0)) {
        return 3;
      } else if ((i == 0) && (j == 2)) {
        return 4;
      } else if ((i == 2) && (j == 0)) {
        return 4;
      } else if ((i == 1) && (j == 2)) {
        return 5;
      }
      return 5;
    }
    /*!
     * \brief: generate a deformation gradient pertubation dFkl such
     * that the associated deformation rate is:
     * D_rkl = ((ek^el)+(el^ek))/2
     * with D = (L+tL)/2 and L = dF.F^{-1}
     * \param[in] F: deformation gradient
     * \param[in] idx: index associated to (k,l) in the stensor vector
     * convention
     */
    template <unsigned short N, typename real>
    static TFEL_MATERIAL_INLINE tfel::math::tensor<N, real>
    getDeformationGradient(const tfel::math::tensor<N, real>& F,
                           const size_type idx) {
      const auto c = [&idx]() -> std::pair<size_type, size_type> {
        if ((idx == 0) || (idx == 1) || (idx == 2)) {
          return {idx, idx};
        }
        return {2 * idx - 3, 2 * idx - 2};
      }();
      // i,j are valid for the space dimension considered
      tfel::math::tensor<N, real> ekel(real(0));
      const auto v = ((idx > 2) ? tfel::math::Cste<real>::sqrt2 : real(1)) / 2;
      ekel(c.first) += v;
      ekel(c.second) += v;
      return ekel * F;
    }
  };  // end of struct FiniteStrainBehaviourTangentOperatorConverterBase

  /*!
   * \brief partial specialisation of
   * FiniteStrainBehaviourTangentOperatorConverter structure
   */
  TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DC, DS_DEGL){
      /*!
       * \param[out] Kr: the result of the convertion
       * \param[in]  Ks: the initial stiffness tensor
       * \param[in]  F0:  the deformation gradient
       * \param[in]  F1:  the deformation gradient
       * \param[in]  s:  the Cauchy stress tensor
       */
      template <unsigned short N, typename stress>
      static TFEL_MATERIAL_INLINE void exe(
          Result<N, stress> & Kr,
          const Source<N, stress>& Ks,
          const DeformationGradientTensor<N, stress>&,
          const DeformationGradientTensor<N, stress>&,
          const StressStensor<N, stress>&){Kr = Ks / 2;
}  // namespace tfel::material
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DEGL, DS_DC){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){Kr = 2 * Ks;
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = tfel::math::push_forward(Ks, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DEGL,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = tfel::math::pull_back(Ks, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Cs =
    convert<TangentOperator::SPATIAL_MODULI, TangentOperator::DS_DEGL>(
        Ks, F0, F1, s);
const auto Dt =
    convert<TangentOperator::DTAU_DF, TangentOperator::SPATIAL_MODULI>(
        Cs, F0, F1, s);
Kr = convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(Dt, F0, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DF, DS_DC){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){
        Kr = Ks * tfel::math::t2tost2<N, base_type<stress>>::dCdF(F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DF, DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){
        Kr = Ks * tfel::math::t2tost2<N, base_type<stress>>::dCdF(F1) / 2;
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks, s* J) / J;
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS, DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto J = tfel::math::det(F1);
const auto C =
    convert<TangentOperator::SPATIAL_MODULI, TangentOperator::DS_DEGL>(
        Ks, F0, F1, s);
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(C, s* J) / J;
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF,
                                                            C_TRUESDELL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Cs =
    convert<TangentOperator::SPATIAL_MODULI, TangentOperator::C_TRUESDELL>(
        Ks, F0, F1, s);
const auto Dt =
    convert<TangentOperator::DTAU_DF, TangentOperator::SPATIAL_MODULI>(
        Cs, F0, F1, s);
Kr = convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(Dt, F0, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
// a little trick: the invert of the transformation from the
// spatial moduli to the KirchhoffJaumanRateModuli is obtained
// by changing the sign of the Kirchoff stress
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks * J, -s* J);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks / (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,
                                                            DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = tfel::math::push_forward(Ks, F1) /
                                              (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            C_TRUESDELL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks * (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DDF, DSIG_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(F0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, DSIG_DDF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        const auto iF0 = tfel::math::invert(F0);
Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(iF0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DDF, DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(F0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF, DTAU_DDF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>&,
        const StressStensor<N, stress>&){
        const auto iF0 = tfel::math::invert(F0);
Kr = Ks * tfel::math::t2tot2<N, base_type<stress>>::tpld(iF0);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        tfel::math::computeCauchyStressDerivativeFromKirchhoffStressDerivative(
            Kr, Ks, s, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF, DS_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        const auto sk2 =
            tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s, F1);
tfel::math::computePushForwardDerivative(Kr, Ks, sk2, F1);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto CtJ =
    convert<TangentOperator::C_TAU_JAUMANN, TangentOperator::DTAU_DF>(
        Ks, F0, F1, s);
const auto J = tfel::math::det(F1);
Kr = convertSpatialModuliToKirchhoffJaumanRateModuli(CtJ, -J* s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,
                                                            DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){
        for (size_type k = 0; k != 3; ++k){for (size_type l = 0; l != 3; ++l){
            if (((k == 1) && (l == 0)) || ((k == 2) && (l == 1))){continue;
}
const auto rkl = FiniteStrainBehaviourTangentOperatorConverterBase::index(k, l);
if (rkl >= tfel::math::StensorDimeToSize<N>::value) {
  continue;
}
const auto cCj = Ks * getDeformationGradient(F1, rkl);
for (size_type i = 0; i != cCj.size(); ++i) {
  Kr(i, rkl) = cCj(i);
}
}
}
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,
                                                            DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Cs =
    convert<TangentOperator::SPATIAL_MODULI, TangentOperator::DTAU_DF>(
        Ks, F0, F1, s);
Kr = Cs / (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,
                                                            C_TAU_JAUMANN){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks / (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,
                                                            ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){Kr = Ks * (tfel::math::det(F1));
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks, J* s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            C_TAU_JAUMANN){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){const auto J = tfel::math::det(F1);
// a little trick: the invert of the transformation from the
// spatial moduli to the KirchhoffJaumanRateModuli is obtained
// by changing the sign of the Kirchoff stress
Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks, -J* s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS, DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>&){const auto iJ = 1 /
                                                         tfel::math::det(F1);
for (size_type k = 0; k != 3; ++k) {
  for (size_type l = 0; l != 3; ++l) {
    if (((k == 1) && (l == 0)) || ((k == 2) && (l == 1))) {
      continue;
    }
    const auto rkl =
        FiniteStrainBehaviourTangentOperatorConverterBase::index(k, l);
    if (rkl >= tfel::math::StensorDimeToSize<N>::value) {
      continue;
    }
    const auto cCj = Ks * getDeformationGradient(F1, rkl);
    for (size_type i = 0; i != cCj.size(); ++i) {
      Kr(i, rkl) = cCj(i) * iJ;
    }
  }
}
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,
                                                            C_TAU_JAUMANN){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        // not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) ->
        // t2tost2{
        auto toT2toST2 = [&](tfel::math::t2tot2<N, stress> src) {
          tfel::math::t2tost2<N, stress> r;
          for (unsigned short i = 0; i != 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(i, j) = src(i, j);
            }
          }
          for (unsigned short i = 0;
               i != tfel::math::StensorDimeToSize<N>::value - 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(3 + i, j) = (src(3 + 2 * i, j) + src(3 + 2 * i + 1, j)) *
                            tfel::math::Cste<stress>::isqrt2;
            }
          }
          return r;
        };
const auto t = s * tfel::math::det(F1);
const auto tus = tfel::math::unsyme(t);
const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
const auto dW = tfel::math::computeSpinRateDerivative(F1);
Kr = Ks * dD + toT2toST2((tfel::math::t2tot2<N, base_type<stress>>::tpld(tus) -
                          tfel::math::t2tot2<N, base_type<stress>>::tprd(tus)) *
                         dW);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF, ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        // not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) ->
        // t2tost2{
        auto toT2toST2 = [&](tfel::math::t2tot2<N, stress> src) {
          tfel::math::t2tost2<N, stress> r;
          for (unsigned short i = 0; i != 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(i, j) = src(i, j);
            }
          }
          for (unsigned short i = 0;
               i != tfel::math::StensorDimeToSize<N>::value - 3; ++i) {
            for (unsigned short j = 0;
                 j != tfel::math::TensorDimeToSize<N>::value; ++j) {
              r(3 + i, j) = (src(3 + 2 * i, j) + src(3 + 2 * i + 1, j)) *
                            tfel::math::Cste<stress>::isqrt2;
            }
          }
          return r;
        };
const auto J = tfel::math::det(F1);
const auto t = s * J;
const auto tus = tfel::math::unsyme(t);
const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
const auto dW = tfel::math::computeSpinRateDerivative(F1);
Kr = J * Ks * dD +
     toT2toST2((tfel::math::t2tot2<N, base_type<stress>>::tpld(tus) -
                tfel::math::t2tot2<N, base_type<stress>>::tprd(tus)) *
               dW);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,
                                                            SPATIAL_MODULI){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
// not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) -> t2tost2{
auto toT2toST2 = [&](const tfel::math::t2tot2<N, stress>& src)
    -> tfel::math::t2tost2<N, stress> {
  tfel::math::t2tost2<N, stress> r;
  for (unsigned short i = 0; i != 3; ++i) {
    for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
         ++j) {
      r(i, j) = src(i, j);
    }
  }
  for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value - 3;
       ++i) {
    for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
         ++j) {
      r(3 + i, j) = (src(3 + 2 * i, j) + src(3 + 2 * i + 1, j)) *
                    tfel::math::Cste<stress>::isqrt2;
    }
  }
  return r;
};
const auto CJ =
    convert<TangentOperator::C_TAU_JAUMANN, TangentOperator::SPATIAL_MODULI>(
        Ks, F0, F1, s);
const auto t = s * det(F1);
const auto tus = tfel::math::unsyme(t);
const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
const auto dW = tfel::math::computeSpinRateDerivative(F1);
Kr = CJ * dD + toT2toST2((tfel::math::t2tot2<N, base_type<stress>>::tpld(tus) -
                          tfel::math::t2tot2<N, base_type<stress>>::tprd(tus)) *
                         dW);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DEGL, DT_DELOG){
    /*!
     * \brief convert the stiffness matrix expressed in the logarithmic
     * space into dS_dEGL.
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        constexpr auto ls = LogarithmicStrainHandler<N, stress>::LAGRANGIAN;
LogarithmicStrainHandler<N, stress> l(ls, F1);
const auto T = l.convertFromCauchyStress(s);
Kr = l.convertToMaterialTangentModuli(Ks, T);
}  // end of exe
}
;

TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DC, DT_DELOG){
    /*!
     * \brief convert the stiffness matrix expressed in the logarithmic
     * space into dS_dC.
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        constexpr auto ls = LogarithmicStrainHandler<N, stress>::LAGRANGIAN;
LogarithmicStrainHandler<N, stress> l(ls, F1);
const auto T = l.convertFromCauchyStress(s);
Kr = l.convertToMaterialTangentModuli(Ks, T) / 2;
}  // end of exe
}
;

TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,
                                                            DT_DELOG){
    /*!
     * \brief convert the stiffness matrix expressed in the logarithmic
     * space to the spatial moduli
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        constexpr auto ls = LogarithmicStrainHandler<N, stress>::EULERIAN;
LogarithmicStrainHandler<N, stress> l(ls, F1);
const auto T = l.convertFromCauchyStress(s);
Kr = l.convertToSpatialTangentModuli(Ks, T);
}  // end of exe
}
;

TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,
                                                            DT_DELOG){
    /*!
     * \brief convert the stiffness matrix expressed in the logarithmic
     * space to the spatial moduli
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        constexpr auto ls = LogarithmicStrainHandler<N, stress>::EULERIAN;
LogarithmicStrainHandler<N, stress> l(ls, F1);
const auto T = l.convertFromCauchyStress(s);
Kr = l.convertToCauchyStressTruesdellRateTangentModuli(Ks, T);
}  // end of exe
}
;

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, ABAQUS){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Dt =
    convert<TangentOperator::DTAU_DF, TangentOperator::ABAQUS>(Ks, F0, F1, s);
Kr = convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(Dt, F0, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DPK1_DF, DSIG_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        tfel::math::
            convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
                Kr, Ks, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DPK1_DF, DTAU_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        tfel::math::
            convertKirchhoffStressDerivativeToFirstPiolaKirchhoffStressDerivative(
                Kr, Ks, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DPK1_DF, DTAU_DDF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        internals::DTAU_DDF_to_DPK1_DF_Converter<N>::exe(Kr, Ks, F0, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF, DPK1_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        tfel::math::
            convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative(
                Kr, Ks, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF, DPK1_DF){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>& F0,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
const auto Dt =
    convert<TangentOperator::DTAU_DF, TangentOperator::DPK1_DF>(Ks, F0, F1, s);
Kr = convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(Dt, F0, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

/*!
 * \brief partial specialisation of
 * FiniteStrainBehaviourTangentOperatorConverter structure
 */
TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DPK1_DF, DS_DEGL){
    /*!
     * \param[out] Kr: the result of the convertion
     * \param[in]  Ks: the initial stiffness tensor
     * \param[in]  F0:  the deformation gradient
     * \param[in]  F1:  the deformation gradient
     * \param[in]  s:  the Cauchy stress tensor
     */
    template <unsigned short N, typename stress>
    static TFEL_MATERIAL_INLINE void exe(
        Result<N, stress> & Kr,
        const Source<N, stress>& Ks,
        const DeformationGradientTensor<N, stress>&,
        const DeformationGradientTensor<N, stress>& F1,
        const StressStensor<N, stress>& s){
        tfel::math::
            convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
                Kr, Ks, F1, s);
}  // end of exe
}
;  // end of struct FiniteStrainBehaviourTangentOperatorConverter

template <FiniteStrainBehaviourTangentOperatorBase::Flag ResultFlag,
          FiniteStrainBehaviourTangentOperatorBase::Flag SourceFlag,
          unsigned short N,
          typename StressType>
tangent_operator<ResultFlag, N, StressType> convert(
    const tangent_operator<SourceFlag, N, StressType>& K,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F0,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F1,
    const tfel::math::stensor<N, StressType>& s) {
  tangent_operator<ResultFlag, N, StressType> r;
  FiniteStrainBehaviourTangentOperatorConverter<ResultFlag, SourceFlag>::exe(
      r, K, F0, F1, s);
  return r;
}  // end of convert

template <FiniteStrainBehaviourTangentOperatorBase::Flag ResultFlag,
          FiniteStrainBehaviourTangentOperatorBase::Flag SourceFlag,
          unsigned short N,
          typename StressType>
void convert(
    tangent_operator<ResultFlag, N, StressType>& Kr,
    const tangent_operator<SourceFlag, N, StressType>& Ks,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F0,
    const tfel::math::tensor<N, tfel::typetraits::base_type<StressType>>& F1,
    const tfel::math::stensor<N, StressType>& s) {
  FiniteStrainBehaviourTangentOperatorConverter<ResultFlag, SourceFlag>::exe(
      Kr, Ks, F0, F1, s);
}  // end of convert

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX */
