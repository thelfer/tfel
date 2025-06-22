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

namespace tfel {

  namespace math {

    namespace internals {

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
         * \tparam stress: stress type
         * \tparam real: numeric type
         * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
         *                 respect to the deformation gradient
         * \param[in] ds: derivative of the Cauchy stress with respect to the
         *                deformation gradient
         * \param[in] F: deformation gradient
         * \param[in] s: Cauchy stress
         */
        template <typename stress, typename real>
        static void exe(tfel::math::t2tot2<1u, stress>& dP,
                        const tfel::math::t2tost2<1u, stress>& ds,
                        const tfel::math::tensor<1u, real>& F,
                        const tfel::math::stensor<1u, stress>& s) {
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
         * \tparam stress: stress type
         * \tparam real: numeric type
         * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
         *                 respect to the deformation gradient
         * \param[in] ds: derivative of the Cauchy stress with respect to the
         *                deformation gradient
         * \param[in] F: deformation gradient
         * \param[in] s: Cauchy stress
         */
        template <typename stress, typename real>
        static void exe(tfel::math::t2tot2<2u, stress>& dP,
                        const tfel::math::t2tost2<2u, stress>& ds,
                        const tfel::math::tensor<2u, real>& F,
                        const tfel::math::stensor<2u, stress>& s) {
          constexpr const auto cste = tfel::math::Cste<real>::sqrt2;
          constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
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
         * \tparam stress: stress type
         * \tparam real: numeric type
         * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
         *                 respect to the deformation gradient
         * \param[in] ds: derivative of the Cauchy stress with respect to the
         *                deformation gradient
         * \param[in] F: deformation gradient
         * \param[in] s: Cauchy stress
         */
        template <typename stress, typename real>
        static void exe(tfel::math::t2tot2<3u, stress>& dP,
                        const tfel::math::t2tost2<3u, stress>& ds,
                        const tfel::math::tensor<3u, real>& F,
                        const tfel::math::stensor<3u, stress>& s) {
          constexpr const auto cste = tfel::math::Cste<real>::sqrt2;
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
              ((2 * ds(0, 0) * F[6] - cste * F[0] * ds(4, 0) - cste * s[4]) *
                   F[7] -
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
              ((2 * ds(0, 0) * F[4] - cste * F[0] * ds(3, 0) - cste * s[3]) *
                   F[8] +
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
              ((cste * ds(3, 0) * F[4] - 2 * F[0] * ds(1, 0) - 2 * s[1]) *
                   F[8] +
               (2 * ds(1, 0) * F[3] - cste * ds(3, 0) * F[1]) * F[6] -
               cste * ds(5, 0) * F[3] * F[4] + cste * F[0] * ds(5, 0) * F[1] +
               cste * s[5] * F[1]) /
              2;
          dP(8, 0) =
              ((cste * ds(4, 0) * F[6] - 2 * F[0] * ds(2, 0) - 2 * s[2]) *
                   F[7] -
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
          dP(1, 1) =
              ((cste * ds(3, 1) * F[6] - cste * F[0] * ds(5, 1)) * F[7] -
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
               (cste * ds(3, 1) * F[3] - 2 * ds(0, 1) * F[1] - 2 * s[0]) *
                   F[6] -
               cste * ds(4, 1) * F[3] * F[4] + cste * F[0] * ds(4, 1) * F[1] +
               cste * F[0] * s[4]) /
              2;
          dP(6, 1) =
              -((cste * ds(4, 1) * F[7] - cste * ds(5, 1) * F[5]) * F[8] -
                2 * ds(2, 1) * F[3] * F[7] + 2 * ds(2, 1) * F[1] * F[5] +
                2 * s[2] * F[5] + cste * ds(5, 1) * F[2] * F[3] -
                cste * ds(4, 1) * F[1] * F[2] - cste * s[4] * F[2]) /
              2;
          dP(7, 1) =
              ((cste * ds(3, 1) * F[4] - 2 * F[0] * ds(1, 1)) * F[8] +
               (2 * ds(1, 1) * F[3] - cste * ds(3, 1) * F[1] - cste * s[3]) *
                   F[6] -
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
          dP(1, 2) =
              ((cste * ds(3, 2) * F[6] - cste * F[0] * ds(5, 2)) * F[7] -
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
          dP(6, 2) =
              -((cste * ds(4, 2) * F[7] - cste * ds(5, 2) * F[5]) * F[8] -
                2 * ds(2, 2) * F[3] * F[7] + 2 * ds(2, 2) * F[1] * F[5] +
                cste * ds(5, 2) * F[2] * F[3] + cste * s[5] * F[3] -
                cste * ds(4, 2) * F[1] * F[2] - cste * s[4] * F[1]) /
              2;
          dP(7, 2) =
              ((cste * ds(3, 2) * F[4] - 2 * F[0] * ds(1, 2)) * F[8] +
               (2 * ds(1, 2) * F[3] - cste * ds(3, 2) * F[1]) * F[6] -
               cste * ds(5, 2) * F[3] * F[4] + cste * F[0] * ds(5, 2) * F[1]) /
              2;
          dP(8, 2) =
              ((cste * ds(4, 2) * F[6] - 2 * F[0] * ds(2, 2)) * F[7] -
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
          dP(1, 3) =
              ((cste * ds(3, 3) * F[6] - cste * F[0] * ds(5, 3)) * F[7] -
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
          dP(4, 3) =
              -((cste * ds(3, 3) * F[7] - 2 * ds(1, 3) * F[5]) * F[8] -
                cste * ds(5, 3) * F[3] * F[7] - cste * s[5] * F[7] +
                cste * ds(5, 3) * F[1] * F[5] + 2 * ds(1, 3) * F[2] * F[3] -
                cste * ds(3, 3) * F[1] * F[2] + 2 * s[1] * F[2]) /
              2;
          dP(5, 3) =
              ((2 * ds(0, 3) * F[4] - cste * F[0] * ds(3, 3)) * F[8] +
               (cste * ds(3, 3) * F[3] - 2 * ds(0, 3) * F[1] + cste * s[3]) *
                   F[6] -
               cste * ds(4, 3) * F[3] * F[4] - cste * s[4] * F[4] +
               cste * F[0] * ds(4, 3) * F[1]) /
              2;
          dP(6, 3) =
              -((cste * ds(4, 3) * F[7] - cste * ds(5, 3) * F[5]) * F[8] -
                2 * ds(2, 3) * F[3] * F[7] - 2 * s[2] * F[7] +
                2 * ds(2, 3) * F[1] * F[5] + cste * ds(5, 3) * F[2] * F[3] -
                cste * ds(4, 3) * F[1] * F[2] + cste * s[5] * F[2]) /
              2;
          dP(7, 3) =
              ((cste * ds(3, 3) * F[4] - 2 * F[0] * ds(1, 3)) * F[8] +
               (2 * ds(1, 3) * F[3] - cste * ds(3, 3) * F[1] + 2 * s[1]) *
                   F[6] -
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
          dP(1, 4) =
              ((cste * ds(3, 4) * F[6] - cste * F[0] * ds(5, 4)) * F[7] -
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
              ((2 * ds(0, 4) * F[4] - cste * F[0] * ds(3, 4) + 2 * s[0]) *
                   F[8] +
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
              ((cste * ds(3, 4) * F[4] - 2 * F[0] * ds(1, 4) + cste * s[3]) *
                   F[8] +
               (2 * ds(1, 4) * F[3] - cste * ds(3, 4) * F[1]) * F[6] -
               cste * ds(5, 4) * F[3] * F[4] - cste * s[5] * F[3] +
               cste * F[0] * ds(5, 4) * F[1]) /
              2;
          dP(8, 4) =
              ((cste * ds(4, 4) * F[6] - 2 * F[0] * ds(2, 4)) * F[7] -
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
          dP(3, 5) =
              ((2 * ds(0, 5) * F[6] - cste * F[0] * ds(4, 5)) * F[7] -
               cste * ds(3, 5) * F[5] * F[6] - cste * s[3] * F[6] +
               cste * ds(4, 5) * F[4] * F[5] - 2 * ds(0, 5) * F[2] * F[4] +
               cste * s[4] * F[4] + cste * F[0] * ds(3, 5) * F[2]) /
              2;
          dP(4, 5) =
              -((cste * ds(3, 5) * F[7] - 2 * ds(1, 5) * F[5] - 2 * s[1]) *
                    F[8] -
                cste * ds(5, 5) * F[3] * F[7] + cste * ds(5, 5) * F[1] * F[5] +
                2 * ds(1, 5) * F[2] * F[3] - cste * ds(3, 5) * F[1] * F[2] +
                cste * s[5] * F[1]) /
              2;
          dP(5, 5) =
              ((2 * ds(0, 5) * F[4] - cste * F[0] * ds(3, 5)) * F[8] +
               (cste * ds(3, 5) * F[3] - 2 * ds(0, 5) * F[1]) * F[6] -
               cste * ds(4, 5) * F[3] * F[4] + cste * F[0] * ds(4, 5) * F[1]) /
              2;
          dP(6, 5) = -((cste * ds(4, 5) * F[7] - cste * ds(5, 5) * F[5] -
                        cste * s[5]) *
                           F[8] -
                       2 * ds(2, 5) * F[3] * F[7] + 2 * ds(2, 5) * F[1] * F[5] +
                       cste * ds(5, 5) * F[2] * F[3] -
                       cste * ds(4, 5) * F[1] * F[2] + 2 * s[2] * F[1]) /
                     2;
          dP(7, 5) =
              ((cste * ds(3, 5) * F[4] - 2 * F[0] * ds(1, 5)) * F[8] +
               (2 * ds(1, 5) * F[3] - cste * ds(3, 5) * F[1]) * F[6] -
               cste * ds(5, 5) * F[3] * F[4] + cste * F[0] * ds(5, 5) * F[1]) /
              2;
          dP(8, 5) =
              ((cste * ds(4, 5) * F[6] - 2 * F[0] * ds(2, 5)) * F[7] -
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
              ((2 * ds(0, 6) * F[6] - cste * F[0] * ds(4, 6) + 2 * s[0]) *
                   F[7] -
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
              ((cste * ds(4, 6) * F[6] - 2 * F[0] * ds(2, 6) + cste * s[4]) *
                   F[7] -
               cste * ds(5, 6) * F[5] * F[6] + 2 * ds(2, 6) * F[4] * F[5] -
               cste * s[5] * F[5] - cste * ds(4, 6) * F[2] * F[4] +
               cste * F[0] * ds(5, 6) * F[2]) /
              2;
          // -->  f90(diff(P[1],F[7]));
          dP(0, 7) =
              -((2 * ds(0, 7) * F[7] - cste * ds(3, 7) * F[5] + 2 * s[0]) *
                    F[8] -
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
          dP(3, 7) =
              ((2 * ds(0, 7) * F[6] - cste * F[0] * ds(4, 7)) * F[7] -
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
          dP(6, 7) = -((cste * ds(4, 7) * F[7] - cste * ds(5, 7) * F[5] +
                        cste * s[4]) *
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
          dP(8, 7) =
              ((cste * ds(4, 7) * F[6] - 2 * F[0] * ds(2, 7)) * F[7] -
               cste * ds(5, 7) * F[5] * F[6] + cste * s[4] * F[6] +
               2 * ds(2, 7) * F[4] * F[5] - cste * ds(4, 7) * F[2] * F[4] +
               cste * F[0] * ds(5, 7) * F[2] - 2 * F[0] * s[2]) /
              2;
          // -->  f90(diff(P[1],F[8]));
          dP(0, 8) =
              -((2 * ds(0, 8) * F[7] - cste * ds(3, 8) * F[5]) * F[8] -
                cste * ds(4, 8) * F[3] * F[7] + 2 * s[0] * F[7] +
                cste * ds(4, 8) * F[1] * F[5] - cste * s[3] * F[5] +
                cste * ds(3, 8) * F[2] * F[3] - 2 * ds(0, 8) * F[1] * F[2]) /
              2;
          dP(1, 8) =
              ((cste * ds(3, 8) * F[6] - cste * F[0] * ds(5, 8)) * F[7] -
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
          dP(4, 8) =
              -((cste * ds(3, 8) * F[7] - 2 * ds(1, 8) * F[5]) * F[8] -
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

    }  // end of namespace internals

    template <unsigned short N, typename stress, typename real>
    void convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
        t2tot2<N, stress>& dP,
        const t2tost2<N, stress>& ds,
        const tensor<N, real>& F,
        const stensor<N, stress>& s) {
      tfel::math::internals::DSIG_DF_to_DPK1_DF_Converter<N>::exe(dP, ds, F, s);
    }  // end of
       // convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative

    template <unsigned short N, typename stress, typename real>
    t2tot2<N, stress>
    convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
        const t2tost2<N, stress>& ds,
        const tensor<N, real>& F,
        const stensor<N, stress>& s) {
      t2tot2<N, stress> dP;
      tfel::math::internals::DSIG_DF_to_DPK1_DF_Converter<N>::exe(dP, ds, F, s);
      return dP;
    }  // end of
       // convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative

    template <unsigned short N, typename stress, typename real>
    t2tot2<N, stress>
    convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
        const st2tost2<N, stress>& dS,
        const tensor<N, real>& F,
        const stensor<N, stress>& s) {
      t2tot2<N, stress> dP;
      convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
          dP, dS, F, s);
      return dP;
    }  // end of
       // convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative

    template <unsigned short N, typename stress, typename real>
    void
    convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
        t2tot2<N, stress>& dP,
        const st2tost2<N, stress>& dS,
        const tensor<N, real>& F,
        const stensor<N, stress>& s) {
      const auto dE_dF = eval(t2tost2<N, real>::dCdF(F) / 2);
      const auto dS_dF = t2tot2<N, stress>{dS * dE_dF};
      const auto S =
          unsyme(convertCauchyStressToSecondPiolaKirchhoffStress(s, F));
      dP = t2tot2<N, stress>::tpld(S) + t2tot2<N, stress>::tprd(F, dS_dF);
    }  // end of
       // convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2_CONVERTTOPK1DERIVATIVE_IXX */
