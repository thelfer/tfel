/*!
 * \file   include/TFEL/Material/Cazacu2006IsotropicYieldCriterion.ixx
 * \brief
 * \author Thomas Helfer
 * \date   15/11/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CAZACU2006ISOTROPICYIELDCRITERION_IXX
#define LIB_TFEL_MATERIAL_CAZACU2006ISOTROPICYIELDCRITERION_IXX

#include <cmath>
#include <tuple>
#include <algorithm>

namespace tfel {

  namespace material {

    namespace internals {

      /*!
       * \brief compute the second derivative of the Cazacu equivalent stress
       * \param[out] d2Psi_ds2: second derivative of the Cazacu equivalent
       * stress \param[in] dPsi_dvp: first derivative of the Cazacu equivalent
       * stress with respect to the eigenvalue \param[in] d2Psi_dvp2: second
       * derivative of the Cazacu equivalent stress with respect to the
       * eigenvalue \param[in] n0: first eigen tensor \param[in] n1: second
       * eigen tensor \param[in] n2: third eigen tensor \param[in] vp: eigen
       * values \param[in] m: matrix for the eigen vectors \param[in] e:
       * criterion used to check if two eigenvalues are equal
       */
      template <typename StressStensor>
      typename std::enable_if<tfel::math::StensorTraits<StressStensor>::dime ==
                                  1u,
                              void>::type
      computeCazacu2006IsotropicStressSecondDerivative(
          tfel::material::CazacuStressSecondDerivativeType<StressStensor>&
              d2Psi_ds2,
          const tfel::math::
              tvector<3u, tfel::material::CazacuBaseType<StressStensor>>&,
          const tfel::math::tvector<
              6u,
              tfel::material::CazacuInvertStressType<StressStensor>>&
              d2Psi_dvp2,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>&,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>&,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>&,
          const tfel::math::tvector<3u, CazacuStressType<StressStensor>>&,
          const tfel::math::tmatrix<3u, 3u, CazacuBaseType<StressStensor>>&,
          const tfel::material::CazacuStressType<StressStensor>) {
        d2Psi_ds2 = {d2Psi_dvp2[0], d2Psi_dvp2[3], d2Psi_dvp2[4],
                     d2Psi_dvp2[3], d2Psi_dvp2[1], d2Psi_dvp2[5],
                     d2Psi_dvp2[4], d2Psi_dvp2[5], d2Psi_dvp2[2]};
      }  // end of computeCazacu2006IsotropicStressSecondDerivative
      /*!
       * \brief compute the second derivative of the Cazacu equivalent stress
       * \param[out] d2Psi_ds2: second derivative of the Cazacu equivalent
       * stress \param[in] dPsi_dvp: first derivative of the Cazacu equivalent
       * stress with respect to the eigenvalue \param[in] d2Psi_dvp2: second
       * derivative of the Cazacu equivalent stress with respect to the
       * eigenvalue \param[in] n0: first eigen tensor \param[in] n1: second
       * eigen tensor \param[in] n2: third eigen tensor \param[in] vp: eigen
       * values \param[in] m: matrix for the eigen vectors \param[in] e:
       * criterion used to check if two eigenvalues are equal
       */
      template <typename StressStensor>
      typename std::enable_if<tfel::math::StensorTraits<StressStensor>::dime ==
                                  2u,
                              void>::type
      computeCazacu2006IsotropicStressSecondDerivative(
          tfel::material::CazacuStressSecondDerivativeType<StressStensor>&
              d2Psi_ds2,
          const tfel::math::tvector<
              3u,
              tfel::material::CazacuBaseType<StressStensor>>& dPsi_dvp,
          const tfel::math::tvector<
              6u,
              tfel::material::CazacuInvertStressType<StressStensor>>&
              d2Psi_dvp2,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>& n0,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>& n1,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>& n2,
          const tfel::math::tvector<3u, CazacuStressType<StressStensor>>& vp,
          const tfel::math::tmatrix<3u, 3u, CazacuBaseType<StressStensor>>& m,
          const tfel::material::CazacuStressType<StressStensor> e) {
        using namespace tfel::math;
        using base = tfel::material::CazacuBaseType<StressStensor>;
        constexpr const auto icste = Cste<base>::isqrt2;
        const tvector<3u, base> v0 = m.template column_view<0u>();
        const tvector<3u, base> v1 = m.template column_view<1u>();
        const stensor<2u, base> n01 =
            stensor<2u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v1) *
            icste;
        d2Psi_ds2 = (d2Psi_dvp2[0] * (n0 ^ n0) + d2Psi_dvp2[1] * (n1 ^ n1) +
                     d2Psi_dvp2[2] * (n2 ^ n2));
        if (std::abs(vp(0) - vp(1)) < e) {
          d2Psi_ds2 += ((d2Psi_dvp2[0] + d2Psi_dvp2[1]) / 2) * (n01 ^ n01);
        } else {
          // 0    1    2    3    4    5
          // s1s1 s2s2 s3s3 s1s2 s1s3 s2s3
          d2Psi_ds2 +=
              (dPsi_dvp[0] - dPsi_dvp[1]) / (vp[0] - vp[1]) * (n01 ^ n01);
        }
      }  // end of computeCazacu2006IsotropicStressSecondDerivative
      /*!
       * \brief compute the second derivative of the Cazacu equivalent stress
       * \param[out] d2Psi_ds2: second derivative of the Cazacu equivalent
       * stress \param[in] dPsi_dvp: first derivative of the Cazacu equivalent
       * stress with respect to the eigenvalue \param[in] d2Psi_dvp2: second
       * derivative of the Cazacu equivalent stress with respect to the
       * eigenvalue \param[in] n0: first eigen tensor \param[in] n1: second
       * eigen tensor \param[in] n2: third eigen tensor \param[in] vp: eigen
       * values \param[in] m: matrix for the eigen vectors \param[in] e:
       * criterion used to check if two eigenvalues are equal
       */
      template <typename StressStensor>
      typename std::enable_if<tfel::math::StensorTraits<StressStensor>::dime ==
                                  3u,
                              void>::type
      computeCazacu2006IsotropicStressSecondDerivative(
          tfel::material::CazacuStressSecondDerivativeType<StressStensor>&
              d2Psi_ds2,
          const tfel::math::tvector<
              3u,
              tfel::material::CazacuBaseType<StressStensor>>& dPsi_dvp,
          const tfel::math::tvector<
              3u,
              tfel::material::CazacuInvertStressType<StressStensor>>&
              d2Psi_dvp2,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>& n0,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>& n1,
          const tfel::material::CazacuStressEigenTensorType<StressStensor>& n2,
          const tfel::math::tvector<3u, CazacuStressType<StressStensor>>& vp,
          const tfel::math::tmatrix<3u, 3u, CazacuBaseType<StressStensor>>& m,
          const tfel::material::CazacuStressType<StressStensor> e) {
        using namespace tfel::math;
        using base = tfel::material::CazacuBaseType<StressStensor>;
        constexpr const auto cste = Cste<base>::isqrt2;
        const tvector<3u, base> v0 = m.template column_view<0u>();
        const tvector<3u, base> v1 = m.template column_view<1u>();
        const tvector<3u, base> v2 = m.template column_view<2u>();
        const stensor<3u, base> n01 =
            stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v1) *
            cste;
        const stensor<3u, base> n02 =
            stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v2) *
            cste;
        const stensor<3u, base> n12 =
            stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v1, v2) *
            cste;
        d2Psi_ds2 = d2Psi_dvp2[0] * (n0 ^ n0) + d2Psi_dvp2[1] * (n1 ^ n1) +
                    d2Psi_dvp2[2] * (n2 ^ n2);
        if ((std::abs(vp(0) - vp(1)) < e) && (std::abs(vp(0) - vp(2)) < e)) {
          d2Psi_ds2 += (((d2Psi_dvp2[0] + d2Psi_dvp2[1]) / 2) * (n01 ^ n01) +
                        ((d2Psi_dvp2[0] + d2Psi_dvp2[2]) / 2) * (n02 ^ n02) +
                        ((d2Psi_dvp2[1] + d2Psi_dvp2[2]) / 2) * (n12 ^ n12));
        } else if (std::abs(vp(0) - vp(1)) < e) {
          d2Psi_ds2 += (((d2Psi_dvp2[0] + d2Psi_dvp2[1]) / 2) * (n01 ^ n01) +
                        dPsi_dvp[0] * (n02 ^ n02) / (vp[0] - vp[2]) +
                        dPsi_dvp[1] * (n12 ^ n12) / (vp[1] - vp[2]) +
                        dPsi_dvp[2] * ((n12 ^ n12) / (vp[2] - vp[1]) +
                                       (n02 ^ n02) / (vp[2] - vp[0])));
        } else if (std::abs(vp(0) - vp(2)) < e) {
          d2Psi_ds2 += (((d2Psi_dvp2[0] + d2Psi_dvp2[2]) / 2) * (n02 ^ n02) +
                        dPsi_dvp[1] * ((n01 ^ n01) / (vp[1] - vp[0]) +
                                       (n12 ^ n12) / (vp[1] - vp[2])) +
                        dPsi_dvp[0] * (n01 ^ n01) / (vp[0] - vp[1]) +
                        dPsi_dvp[2] * (n12 ^ n12) / (vp[2] - vp[1]));
        } else if (std::abs(vp(1) - vp(2)) < e) {
          d2Psi_ds2 += (((d2Psi_dvp2[1] + d2Psi_dvp2[2]) / 2) * (n12 ^ n12) +
                        dPsi_dvp[0] * ((n01 ^ n01) / (vp[0] - vp[1]) +
                                       (n02 ^ n02) / (vp[0] - vp[2])) +
                        dPsi_dvp[1] * (n01 ^ n01) / (vp[1] - vp[0]) +
                        dPsi_dvp[2] * (n02 ^ n02) / (vp[2] - vp[0]));
        } else {
          d2Psi_ds2 += (dPsi_dvp[0] * ((n01 ^ n01) / (vp[0] - vp[1]) +
                                       (n02 ^ n02) / (vp[0] - vp[2])) +
                        dPsi_dvp[1] * ((n01 ^ n01) / (vp[1] - vp[0]) +
                                       (n12 ^ n12) / (vp[1] - vp[2])) +
                        dPsi_dvp[2] * ((n12 ^ n12) / (vp[2] - vp[1]) +
                                       (n02 ^ n02) / (vp[2] - vp[0])));
        }
      }  // end of computeCazacu2006IsotropicStressSecondDerivative

    }  // end of namespace internals

    template <typename StressStensor,
              typename CazacuExponentType,
              tfel::math::stensor_common::EigenSolver es>
    CazacuStressType<StressStensor> computeCazacu2006IsotropicStress(
        const StressStensor& sig,
        const CazacuExponentType a,
        const CazacuBaseType<StressTensor> k,
        const CazacuStressType<StressStensor> e) {
      using real = CazacuBaseType<StressStensor>;
      const auto seq = sigmaeq(s);
      if (seq < e) {
        return seq * 0;
      }
      const auto iseq = 1 / seq;
      const auto s = deviator(sig);
      const auto vp = s.template computeEigenValues<es>() * iseq;
      return seq *
             std::pow(std::pow(std::abs(std::abs(vp[0]) - k * vp[0]), a) +
                          std::pow(std::abs(std::abs(vp[1]) - k * vp[1]), a) +
                          std::pow(std::abs(std::abs(vp[2]) - k * vp[2]), a),
                      1 / real(a));
    }  // end of computeCazacu2006IsotropicYieldStress

    template <typename StressStensor,
              typename CazacuExponentType,
              tfel::math::stensor_common::EigenSolver es>
    std::tuple<CazacuStressType<StressStensor>,
               CazacuStressNormalType<StressStensor>>
    computeCazacu2006IsotropicStressNormal(
        const StressStensor& sig,
        const CazacuExponentType a,
        const CazacuBaseType<StressTensor> k,
        const CazacuStressType<StressStensor> e) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using stress = CazacuStressType<StressStensor>;
      using real = CazacuBaseType<StressStensor>;
      using normal = CazacuStressNormalType<StressStensor>;
      // Von Mises stress used to normalise the stress eigenvalues
      const auto s = deviator(sig);
      const auto seq = sigmaeq(s);
      if (seq < e) {
        return std::make_tuple(stress{0}, normal{real{0}});
      }
      const auto iseq = 1 / seq;
      // no structured bindings yet
      tfel::math::tvector<3u, stress> vp;
      tfel::math::tmatrix<3u, 3u, real> m;
      std::tie(vp, m) = s.template computeEigenVectors<es>();
      const auto n = tfel::math::stensor<N, stress>::computeEigenTensors(m);
      // eigenvalues are normalised by the Von Mises stress to avoid
      // overflow
      const auto rvp = vp * iseq;
      const real r[3] = {std::abs(std::abs(rvp[0]) - k * rvp[0]),
                         std::abs(std::abs(rvp[1]) - k * rvp[1]),
                         std::abs(std::abs(rvp[2]) - k * rvp[2])};
      const real rPsi_am1[3] = {std::pow(r[0], a - 1), std::pow(r[1], a - 1),
                                std::pow(r[2], a - 1)};
      const real rPsi_a =
          rPsi_am1[0] * r[0] + rPsi_am1[1] * r[1] + rPsi_am1[2] * r[2];
      // Cazacu equivalent stress
      const real Psi = seq * std::pow(rPsi_a, 1 / real(a));

      constexpr const auto K = tfel::math::st2tost2<N, real>::K();
      const auto dPsi_ds = ...;
      return std::make_tuple(Psi, eval(dPsi_ds * K));
    }  // end of computeCazacu2006IsotropicStressNormal

    template <typename StressStensor,
              typename CazacuExponentType,
              tfel::math::stensor_common::EigenSolver es>
    std::tuple<CazacuStressType<StressStensor>,
               CazacuStressNormalType<StressStensor>,
               CazacuStressSecondDerivativeType<StressStensor>>
    computeCazacu2006IsotropicStressSecondDerivative(
        const StressStensor& sig,
        const CazacuExponentType a,
        const CazacuBaseType<StressTensor> k,
        const CazacuStressType<StressStensor> e) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using stress = CazacuStressType<StressStensor>;
      using real = CazacuBaseType<StressStensor>;
      using istress = tfel::math::result_type<real, stress, tfel::math::OpDiv>;
      using normal = CazacuStressNormalType<StressStensor>;
      using second_derivative = CazacuStressSecondDerivativeType<StressStensor>;
      const auto s = deviator(sig);
      // Von Mises stress used to normalise the stress eigenvalues
      const auto seq = sigmaeq(s);
      if (seq < e) {
        return std::make_tuple(stress{0}, normal{real{0}},
                               n second_derivative{istress{0}});
      }
      const auto iseq = 1 / seq;
      // no structured bindings yet
      tfel::math::tvector<3u, stress> vp;
      tfel::math::tmatrix<3u, 3u, real> m;
      std::tie(vp, m) = s.template computeEigenVectors<es>();
      const auto n = tfel::math::stensor<N, stress>::computeEigenTensors(m);
      // eigenvalues are normalised by the Von Mises stress to avoid
      // overflow
      const auto rvp = vp * iseq;
      const real Psi_a = (std::pow(std::abs(std::abs(vp[0]) - k * vp[0]), a) +
                          std::pow(std::abs(std::abs(vp[1]) - k * vp[1]), a) +
                          std::pow(std::abs(std::abs(vp[2]) - k * vp[2]), a));
      // Cazacu equivalent stress
      const real Psi = seq * std::pow(Psi_a, 1 / real(a));
      // For the derivatives, the stress eigenvalues are normalised by
      // the Cazacu equivalent stress
      const auto rvp2 = vp / Psi;
      // first derivative of the Cazacu stress
      // const tfel::math::tvector<3u,real> drvp2  = {rvp2[0]-rvp2[1],
      // 						   rvp2[0]-rvp2[2],
      // 						   rvp2[1]-rvp2[2]};
      // const tfel::math::tvector<3u,real> drvp2_am2 =
      // 	{real(std::pow(std::abs(drvp2[0]),a-2)),
      // 	 real(std::pow(std::abs(drvp2[1]),a-2)),
      // 	 real(std::pow(std::abs(drvp2[2]),a-2))};
      // const tfel::math::tvector<3u,real> dPsi_dsvp =
      // 	{( drvp2[0]*drvp2_am2[0]+drvp2[1]*drvp2_am2[1])/2,
      // 	 (-drvp2[0]*drvp2_am2[0]+drvp2[2]*drvp2_am2[2])/2,
      // 	 (-drvp2[1]*drvp2_am2[1]-drvp2[2]*drvp2_am2[2])/2};
      // const auto dPsi_ds = tfel::math::eval(dPsi_dsvp[0]*std::get<0>(n)+
      // 					    dPsi_dsvp[1]*std::get<1>(n)+
      // 					    dPsi_dsvp[2]*std::get<2>(n));
      // // second derivative
      // const auto cste = (a-1)/Psi;
      // const tfel::math::tvector<6u,istress> d2Psi_dsvp2 =
      // 	{cste*(
      // (drvp2_am2[0]+drvp2_am2[1])/2-dPsi_dsvp[0]*dPsi_dsvp[0]), // s1s1
      // 	 cste*(
      // (drvp2_am2[0]+drvp2_am2[2])/2-dPsi_dsvp[1]*dPsi_dsvp[1]), // s2s2
      // 	 cste*(
      // (drvp2_am2[1]+drvp2_am2[2])/2-dPsi_dsvp[2]*dPsi_dsvp[2]), // s3s3
      // 	 cste*(-(drvp2_am2[0])/2-dPsi_dsvp[0]*dPsi_dsvp[1]), // s1s2
      // 	 cste*(-(drvp2_am2[1])/2-dPsi_dsvp[0]*dPsi_dsvp[2]), // s1s3
      // 	 cste*(-(drvp2_am2[2])/2-dPsi_dsvp[1]*dPsi_dsvp[2])}; // s2s3
      tfel::math::st2tost2<N, istress> d2Psi_ds2;
      internals::computeCazacu2006IsotropicStressSecondDerivative<
          StressStensor>(d2Psi_ds2, dPsi_dsvp, d2Psi_dsvp2, std::get<0>(n),
                         std::get<1>(n), std::get<2>(n), vp, m, e);
      constexpr const auto K = tfel::math::st2tost2<N, real>::K();
      // K is symmetric, so we can write K*d2Psi_ds2*K rather than
      // transpose(K)*d2Psi_ds2*K
      return std::make_tuple(Psi, eval(dPsi_ds * K), eval(K * d2Psi_ds2 * K));
    }  // end of computeCazacu2006IsotropicSecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_CAZACU2006ISOTROPICYIELDCRITERION_IXX */
