/*!
 * \file   ConvertFromPK1Derivative.ixx
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

#ifndef LIB_TFEL_MATH_T2TOT2_CONVERTFROMPK1DERIVATIVE_IXX
#define LIB_TFEL_MATH_T2TOT2_CONVERTFROMPK1DERIVATIVE_IXX

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * \brief an helper structure meant to be specialized in each spatial
       * dimension.
       * \tparam N: spatial dimension
       */
      template <unsigned short N>
      struct DPK1_DF_to_DTAU_DF_Converter;

      /*!
       * \brief partial specialisation in 1D
       */
      template <>
      struct DPK1_DF_to_DTAU_DF_Converter<1u> {
        /*!
         * \tparam stress: stress type
         * \tparam real: numeric type
         * \param[out] dtau: derivative of the Kirchhoff stress with respect to
         * the deformation gradient \param[in] dP: derivative of the first
         * Piola-Kirchhoff stress with respect to the deformation gradient
         * \param[in] F: deformation gradient
         * \param[in] s: Cauchy stress
         */
        template <typename stress, typename real>
        static void exe(tfel::math::t2tost2<1u, stress>& dtau,
                        const tfel::math::t2tot2<1u, stress>& dP,
                        const tfel::math::tensor<1u, real>& F,
                        const tfel::math::stensor<1u, stress>& s) {
          const auto P = convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
          // f90(diff(tau[1],F[0]));
          dtau(0, 0) = F[0] * dP(0, 0) + P[0];
          dtau(1, 0) = dP(1, 0) * F[1];
          dtau(2, 0) = dP(2, 0) * F[2];
          // f90(diff(tau[1],F[1]));
          dtau(0, 1) = F[0] * dP(0, 1);
          dtau(1, 1) = dP(1, 1) * F[1] + P[1];
          dtau(2, 1) = dP(2, 1) * F[2];
          // f90(diff(tau[1],F[2]));
          dtau(0, 2) = F[0] * dP(0, 2);
          dtau(1, 2) = dP(1, 2) * F[1];
          dtau(2, 2) = dP(2, 2) * F[2] + P[2];
        }  // end of struct DPK1_DF_to_DTAU_DF_Converter<1u>
      };
      /*!
       * \brief partial specialisation in 2D
       */
      template <>
      struct DPK1_DF_to_DTAU_DF_Converter<2u> {
        /*!
         * \tparam stress: stress type
         * \tparam real: numeric type
         * \param[out] dtau: derivative of the Kirchhoff stress with respect to
         * the deformation gradient \param[in] dP: derivative of the first
         * Piola-Kirchhoff stress with respect to the deformation gradient
         * \param[in] F: deformation gradient
         * \param[in] s: Cauchy stress
         */
        template <typename stress, typename real>
        static void exe(tfel::math::t2tost2<2u, stress>& dtau,
                        const tfel::math::t2tot2<2u, stress>& dP,
                        const tfel::math::tensor<2u, real>& F,
                        const tfel::math::stensor<2u, stress>& s) {
          constexpr const auto cste = tfel::math::Cste<real>::sqrt2;
          const auto P = convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
          // f90(diff(tau[1],F[0]));
          dtau(0, 0) = dP(3, 0) * F[3] + F[0] * dP(0, 0) + P[0];
          dtau(1, 0) = dP(4, 0) * F[4] + dP(1, 0) * F[1];
          dtau(2, 0) = dP(2, 0) * F[2];
          dtau(3, 0) = cste * (dP(1, 0) * F[3] + F[0] * dP(4, 0) + P[4]);
          // f90(diff(tau[1],F[1]));
          dtau(0, 1) = dP(3, 1) * F[3] + F[0] * dP(0, 1);
          dtau(1, 1) = dP(4, 1) * F[4] + dP(1, 1) * F[1] + P[1];
          dtau(2, 1) = dP(2, 1) * F[2];
          dtau(3, 1) = cste * (dP(1, 1) * F[3] + F[0] * dP(4, 1));
          // f90(diff(tau[1],F[2]));
          dtau(0, 2) = dP(3, 2) * F[3] + F[0] * dP(0, 2);
          dtau(1, 2) = dP(4, 2) * F[4] + dP(1, 2) * F[1];
          dtau(2, 2) = dP(2, 2) * F[2] + P[2];
          dtau(3, 2) = cste * (dP(1, 2) * F[3] + F[0] * dP(4, 2));
          // f90(diff(tau[1],F[3]));
          dtau(0, 3) = dP(3, 3) * F[3] + F[0] * dP(0, 3) + P[3];
          dtau(1, 3) = dP(4, 3) * F[4] + dP(1, 3) * F[1];
          dtau(2, 3) = dP(2, 3) * F[2];
          dtau(3, 3) = cste * (dP(1, 3) * F[3] + F[0] * dP(4, 3) + P[1]);
          // f90(diff(tau[1],F[4]));
          dtau(0, 4) = dP(3, 4) * F[3] + F[0] * dP(0, 4);
          dtau(1, 4) = dP(4, 4) * F[4] + dP(1, 4) * F[1] + P[4];
          dtau(2, 4) = dP(2, 4) * F[2];
          dtau(3, 4) = cste * (dP(1, 4) * F[3] + F[0] * dP(4, 4));
        }
      };  // end of struct DPK1_DF_to_DTAU_DF_Converter<2u>

      /*!
       * \brief partial specialisation in 3D
       */
      template <>
      struct DPK1_DF_to_DTAU_DF_Converter<3u> {
        /*!
         * \tparam stress: stress type
         * \tparam real: numeric type
         * \param[out] dtau: derivative of the Kirchhoff stress with respect to
         * the deformation gradient \param[in] dP: derivative of the first
         * Piola-Kirchhoff stress with respect to the deformation gradient
         * \param[in] F: deformation gradient
         * \param[in] s: Cauchy stress
         */
        template <typename stress, typename real>
        static void exe(tfel::math::t2tost2<3u, stress>& dtau,
                        const tfel::math::t2tot2<3u, stress>& dP,
                        const tfel::math::tensor<3u, real>& F,
                        const tfel::math::stensor<3u, stress>& s) {
          constexpr const auto cste = tfel::math::Cste<real>::sqrt2;
          const auto P = convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
          // f90(diff(tau[1],F[0]));
          dtau(0, 0) =
              dP(5, 0) * F[5] + dP(3, 0) * F[3] + F[0] * dP(0, 0) + P[0];
          dtau(1, 0) = dP(7, 0) * F[7] + dP(4, 0) * F[4] + dP(1, 0) * F[1];
          dtau(2, 0) = dP(8, 0) * F[8] + dP(6, 0) * F[6] + dP(2, 0) * F[2];
          dtau(3, 0) = cste * (dP(7, 0) * F[5] + dP(1, 0) * F[3] +
                               F[0] * dP(4, 0) + P[4]);
          dtau(4, 0) = cste * (dP(2, 0) * F[5] + dP(8, 0) * F[3] +
                               F[0] * dP(6, 0) + P[6]);
          dtau(5, 0) =
              cste * (dP(2, 0) * F[7] + dP(6, 0) * F[4] + dP(8, 0) * F[1]);
          // f90(diff(tau[1],F[1]));
          dtau(0, 1) = dP(5, 1) * F[5] + dP(3, 1) * F[3] + F[0] * dP(0, 1);
          dtau(1, 1) =
              dP(7, 1) * F[7] + dP(4, 1) * F[4] + dP(1, 1) * F[1] + P[1];
          dtau(2, 1) = dP(8, 1) * F[8] + dP(6, 1) * F[6] + dP(2, 1) * F[2];
          dtau(3, 1) =
              cste * (dP(7, 1) * F[5] + dP(1, 1) * F[3] + F[0] * dP(4, 1));
          dtau(4, 1) =
              cste * (dP(2, 1) * F[5] + dP(8, 1) * F[3] + F[0] * dP(6, 1));
          dtau(5, 1) = cste * (dP(2, 1) * F[7] + dP(6, 1) * F[4] +
                               dP(8, 1) * F[1] + P[8]);
          // f90(diff(tau[1],F[2]));
          dtau(0, 2) = dP(5, 2) * F[5] + dP(3, 2) * F[3] + F[0] * dP(0, 2);
          dtau(1, 2) = dP(7, 2) * F[7] + dP(4, 2) * F[4] + dP(1, 2) * F[1];
          dtau(2, 2) =
              dP(8, 2) * F[8] + dP(6, 2) * F[6] + dP(2, 2) * F[2] + P[2];
          dtau(3, 2) =
              cste * (dP(7, 2) * F[5] + dP(1, 2) * F[3] + F[0] * dP(4, 2));
          dtau(4, 2) =
              cste * (dP(2, 2) * F[5] + dP(8, 2) * F[3] + F[0] * dP(6, 2));
          dtau(5, 2) =
              cste * (dP(2, 2) * F[7] + dP(6, 2) * F[4] + dP(8, 2) * F[1]);
          // f90(diff(tau[1],F[3]));
          dtau(0, 3) =
              dP(5, 3) * F[5] + dP(3, 3) * F[3] + F[0] * dP(0, 3) + P[3];
          dtau(1, 3) = dP(7, 3) * F[7] + dP(4, 3) * F[4] + dP(1, 3) * F[1];
          dtau(2, 3) = dP(8, 3) * F[8] + dP(6, 3) * F[6] + dP(2, 3) * F[2];
          dtau(3, 3) = cste * (dP(7, 3) * F[5] + dP(1, 3) * F[3] +
                               F[0] * dP(4, 3) + P[1]);
          dtau(4, 3) = cste * (dP(2, 3) * F[5] + dP(8, 3) * F[3] +
                               F[0] * dP(6, 3) + P[8]);
          dtau(5, 3) =
              cste * (dP(2, 3) * F[7] + dP(6, 3) * F[4] + dP(8, 3) * F[1]);
          // f90(diff(tau[1],F[4]));
          dtau(0, 4) = dP(5, 4) * F[5] + dP(3, 4) * F[3] + F[0] * dP(0, 4);
          dtau(1, 4) =
              dP(7, 4) * F[7] + dP(4, 4) * F[4] + dP(1, 4) * F[1] + P[4];
          dtau(2, 4) = dP(8, 4) * F[8] + dP(6, 4) * F[6] + dP(2, 4) * F[2];
          dtau(3, 4) =
              cste * (dP(7, 4) * F[5] + dP(1, 4) * F[3] + F[0] * dP(4, 4));
          dtau(4, 4) =
              cste * (dP(2, 4) * F[5] + dP(8, 4) * F[3] + F[0] * dP(6, 4));
          dtau(5, 4) = cste * (dP(2, 4) * F[7] + dP(6, 4) * F[4] +
                               dP(8, 4) * F[1] + P[6]);
          // f90(diff(tau[1],F[5]));
          dtau(0, 5) =
              dP(5, 5) * F[5] + dP(3, 5) * F[3] + F[0] * dP(0, 5) + P[5];
          dtau(1, 5) = dP(7, 5) * F[7] + dP(4, 5) * F[4] + dP(1, 5) * F[1];
          dtau(2, 5) = dP(8, 5) * F[8] + dP(6, 5) * F[6] + dP(2, 5) * F[2];
          dtau(3, 5) = cste * (dP(7, 5) * F[5] + dP(1, 5) * F[3] +
                               F[0] * dP(4, 5) + P[7]);
          dtau(4, 5) = cste * (dP(2, 5) * F[5] + dP(8, 5) * F[3] +
                               F[0] * dP(6, 5) + P[2]);
          dtau(5, 5) =
              cste * (dP(2, 5) * F[7] + dP(6, 5) * F[4] + dP(8, 5) * F[1]);
          // f90(diff(tau[1],F[6]));
          dtau(0, 6) = dP(5, 6) * F[5] + dP(3, 6) * F[3] + F[0] * dP(0, 6);
          dtau(1, 6) = dP(7, 6) * F[7] + dP(4, 6) * F[4] + dP(1, 6) * F[1];
          dtau(2, 6) =
              dP(8, 6) * F[8] + dP(6, 6) * F[6] + dP(2, 6) * F[2] + P[6];
          dtau(3, 6) =
              cste * (dP(7, 6) * F[5] + dP(1, 6) * F[3] + F[0] * dP(4, 6));
          dtau(4, 6) =
              cste * (dP(2, 6) * F[5] + dP(8, 6) * F[3] + F[0] * dP(6, 6));
          dtau(5, 6) =
              cste * (dP(2, 6) * F[7] + dP(6, 6) * F[4] + dP(8, 6) * F[1]);
          // f90(7iff(tau[1],F[7]));
          dtau(0, 7) = dP(5, 7) * F[5] + dP(3, 7) * F[3] + F[0] * dP(0, 7);
          dtau(1, 7) =
              dP(7, 7) * F[7] + dP(4, 7) * F[4] + dP(1, 7) * F[1] + P[7];
          dtau(2, 7) = dP(8, 7) * F[8] + dP(6, 7) * F[6] + dP(2, 7) * F[2];
          dtau(3, 7) =
              cste * (dP(7, 7) * F[5] + dP(1, 7) * F[3] + F[0] * dP(4, 7));
          dtau(4, 7) =
              cste * (dP(2, 7) * F[5] + dP(8, 7) * F[3] + F[0] * dP(6, 7));
          dtau(5, 7) = cste * (dP(2, 7) * F[7] + dP(6, 7) * F[4] +
                               dP(8, 7) * F[1] + P[2]);
          // f90(diff(tau[1],F[8]));
          dtau(0, 8) = dP(5, 8) * F[5] + dP(3, 8) * F[3] + F[0] * dP(0, 8);
          dtau(1, 8) = dP(7, 8) * F[7] + dP(4, 8) * F[4] + dP(1, 8) * F[1];
          dtau(2, 8) =
              dP(8, 8) * F[8] + dP(6, 8) * F[6] + dP(2, 8) * F[2] + P[8];
          dtau(3, 8) =
              cste * (dP(7, 8) * F[5] + dP(1, 8) * F[3] + F[0] * dP(4, 8));
          dtau(4, 8) =
              cste * (dP(2, 8) * F[5] + dP(8, 8) * F[3] + F[0] * dP(6, 8));
          dtau(5, 8) =
              cste * (dP(2, 8) * F[7] + dP(6, 8) * F[4] + dP(8, 8) * F[1]);
        }
      };  // end of struct DPK1_DF_to_DTAU_DF_Converter<3u>

    }  // end of namespace internals

    template <unsigned short N, typename stress, typename real>
    void convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative(
        t2tost2<N, stress>& dtau,
        const t2tot2<N, stress>& dP,
        const tensor<N, real>& F,
        const stensor<N, stress>& s) {
      tfel::math::internals::DPK1_DF_to_DTAU_DF_Converter<N>::exe(dtau, dP, F,
                                                                  s);
    }  // end of
       // convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative

    template <unsigned short N, typename stress, typename real>
    t2tost2<N, stress>
    convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative(
        const t2tot2<N, stress>& dP,
        const tensor<N, real>& F,
        const stensor<N, stress>& s) {
      t2tost2<N, stress> dtau;
      tfel::math::internals::DPK1_DF_to_DTAU_DF_Converter<N>::exe(dtau, dP, F,
                                                                  s);
      return dtau;
    }  // end of
       // convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2_CONVERTFROMPK1DERIVATIVE_IXX */
