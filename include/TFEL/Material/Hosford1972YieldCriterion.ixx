/*!
 * \file   include/TFEL/Material/Hosford1972YieldCriterion.ixx
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

#ifndef LIB_TFEL_MATERIAL_HOSFORD_IXX
#define LIB_TFEL_MATERIAL_HOSFORD_IXX

#include <cmath>
#include <tuple>
#include <algorithm>
#include "TFEL/Math/General/Abs.hxx"

namespace tfel::material {

  namespace internals {

    /*!
     * \brief compute the second derivative of the Hosford equivalent stress
     * \param[out] d2Psi_ds2: second derivative of the Hosford equivalent
     * stress
     * \param[in] dPsi_dvp: first derivative of the Hosford
     * equivalent stress with respect to the eigenvalue
     * \param[in] d2Psi_dvp2: second derivative of the Hosford
     * equivalent stress with respect to the eigenvalue
     * \param[in] n0: first eigen tensor
     * \param[in] n1: second eigen tensor
     * \param[in] n2: third eigen tensor
     * \param[in] vp: eigen values
     * \param[in] m: matrix for the eigen vectors
     * \param[in] e: criterion used to check if two eigenvalues are equal
     */
    template <typename StressStensor>
    typename std::enable_if<tfel::math::getSpaceDimension<StressStensor>() ==
                                1u,
                            void>::type
    computeHosfordStressSecondDerivative(
        tfel::material::HosfordStressSecondDerivativeType<StressStensor>&
            d2Psi_ds2,
        const tfel::math::
            tvector<3u, tfel::material::HosfordBaseType<StressStensor>>&,
        const tfel::math::tvector<
            6u,
            tfel::material::HosfordInvertStressType<StressStensor>>& d2Psi_dvp2,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>&,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>&,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>&,
        const tfel::math::tvector<3u, HosfordStressType<StressStensor>>&,
        const tfel::math::tmatrix<3u, 3u, HosfordBaseType<StressStensor>>&,
        const tfel::material::HosfordStressType<StressStensor>) {
      d2Psi_ds2 = {d2Psi_dvp2[0], d2Psi_dvp2[3], d2Psi_dvp2[4],
                   d2Psi_dvp2[3], d2Psi_dvp2[1], d2Psi_dvp2[5],
                   d2Psi_dvp2[4], d2Psi_dvp2[5], d2Psi_dvp2[2]};
    }  // end of computeHosfordStressSecondDerivative
       /*!
        * \brief compute the second derivative of the Hosford equivalent stress
        * \param[out] d2Psi_ds2: second derivative of the Hosford equivalent
        * stress
        * \param[in] dPsi_dvp: first derivative of the Hosford
        * equivalent stress with respect to the eigenvalue
        * \param[in] d2Psi_dvp2: second derivative of the Hosford
        * equivalent stress with respect to the eigenvalue
        * \param[in] n0: first eigen tensor
        * \param[in] n1: second eigen tensor
        * \param[in] n2: third eigen tensor
        * \param[in] vp: eigen values
        * \param[in] m: matrix for the eigen vectors
        * \param[in] e: criterion used to check if two eigenvalues are equal
        */
    template <typename StressStensor>
    typename std::enable_if<tfel::math::getSpaceDimension<StressStensor>() ==
                                2u,
                            void>::type
    computeHosfordStressSecondDerivative(
        tfel::material::HosfordStressSecondDerivativeType<StressStensor>&
            d2Psi_ds2,
        const tfel::math::tvector<
            3u,
            tfel::material::HosfordBaseType<StressStensor>>& dPsi_dvp,
        const tfel::math::tvector<
            6u,
            tfel::material::HosfordInvertStressType<StressStensor>>& d2Psi_dvp2,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>& n0,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>& n1,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>& n2,
        const tfel::math::tvector<3u, HosfordStressType<StressStensor>>& vp,
        const tfel::math::tmatrix<3u, 3u, HosfordBaseType<StressStensor>>& m,
        const tfel::material::HosfordStressType<StressStensor> e) {
      using namespace tfel::math;
      using base = tfel::material::HosfordBaseType<StressStensor>;
      constexpr auto icste = Cste<base>::isqrt2;
      const tvector<3u, base> v0 = m.template column_view<0u>();
      const tvector<3u, base> v1 = m.template column_view<1u>();
      const stensor<2u, base> n01 =
          stensor<2u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v1) *
          icste;
      d2Psi_ds2 = (d2Psi_dvp2[0] * (n0 ^ n0) + d2Psi_dvp2[3] * (n0 ^ n1) +
                   d2Psi_dvp2[4] * (n0 ^ n2) + d2Psi_dvp2[1] * (n1 ^ n1) +
                   d2Psi_dvp2[3] * (n1 ^ n0) + d2Psi_dvp2[5] * (n1 ^ n2) +
                   d2Psi_dvp2[2] * (n2 ^ n2) + d2Psi_dvp2[4] * (n2 ^ n0) +
                   d2Psi_dvp2[5] * (n2 ^ n1));
      if (tfel::math::abs(vp(0) - vp(1)) < e) {
        d2Psi_ds2 += ((d2Psi_dvp2[0] + d2Psi_dvp2[1] - 2 * d2Psi_dvp2[3]) / 2) *
                     (n01 ^ n01);
      } else {
        // 0    1    2    3    4    5
        // s1s1 s2s2 s3s3 s1s2 s1s3 s2s3
        d2Psi_ds2 +=
            (dPsi_dvp[0] - dPsi_dvp[1]) / (vp[0] - vp[1]) * (n01 ^ n01);
      }
    }  // end of computeHosfordStressSecondDerivative
       /*!
        * \brief compute the second derivative of the Hosford equivalent stress
        * \param[out] d2Psi_ds2: second derivative of the Hosford equivalent
        * stress
        * \param[in] dPsi_dvp: first derivative of the Hosford
        * equivalent stress with respect to the eigenvalue
        * \param[in] d2Psi_dvp2: second derivative of the Hosford
        * equivalent stress with respect to the eigenvalue
        * \param[in] n0: first eigen tensor
        * \param[in] n1: second eigen tensor
        * \param[in] n2: third eigen tensor
        * \param[in] vp: eigen values
        * \param[in] m: matrix for the eigen vectors
        * \param[in] e: criterion used to check if two eigenvalues are equal
        */
    template <typename StressStensor>
    typename std::enable_if<tfel::math::getSpaceDimension<StressStensor>() ==
                                3u,
                            void>::type
    computeHosfordStressSecondDerivative(
        tfel::material::HosfordStressSecondDerivativeType<StressStensor>&
            d2Psi_ds2,
        const tfel::math::tvector<
            3u,
            tfel::material::HosfordBaseType<StressStensor>>& dPsi_dvp,
        const tfel::math::tvector<
            6u,
            tfel::material::HosfordInvertStressType<StressStensor>>& d2Psi_dvp2,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>& n0,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>& n1,
        const tfel::material::HosfordStressEigenTensorType<StressStensor>& n2,
        const tfel::math::tvector<3u, HosfordStressType<StressStensor>>& vp,
        const tfel::math::tmatrix<3u, 3u, HosfordBaseType<StressStensor>>& m,
        const tfel::material::HosfordStressType<StressStensor> e) {
      using namespace tfel::math;
      using base = tfel::material::HosfordBaseType<StressStensor>;
      constexpr auto cste = Cste<base>::isqrt2;
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
      d2Psi_ds2 = d2Psi_dvp2[0] * (n0 ^ n0) + d2Psi_dvp2[3] * (n0 ^ n1) +
                  d2Psi_dvp2[4] * (n0 ^ n2) + d2Psi_dvp2[1] * (n1 ^ n1) +
                  d2Psi_dvp2[3] * (n1 ^ n0) + d2Psi_dvp2[5] * (n1 ^ n2) +
                  d2Psi_dvp2[2] * (n2 ^ n2) + d2Psi_dvp2[4] * (n2 ^ n0) +
                  d2Psi_dvp2[5] * (n2 ^ n1);
      if ((tfel::math::abs(vp(0) - vp(1)) < e) &&
          (tfel::math::abs(vp(0) - vp(2)) < e)) {
        d2Psi_ds2 +=
            (((d2Psi_dvp2[0] + d2Psi_dvp2[1] - 2 * d2Psi_dvp2[3]) / 2) *
                 (n01 ^ n01) +
             ((d2Psi_dvp2[0] + d2Psi_dvp2[2] - 2 * d2Psi_dvp2[4]) / 2) *
                 (n02 ^ n02) +
             ((d2Psi_dvp2[1] + d2Psi_dvp2[2] - 2 * d2Psi_dvp2[5]) / 2) *
                 (n12 ^ n12));
      } else if (tfel::math::abs(vp(0) - vp(1)) < e) {
        d2Psi_ds2 +=
            (((d2Psi_dvp2[0] + d2Psi_dvp2[1] - 2 * d2Psi_dvp2[3]) / 2) *
                 (n01 ^ n01) +
             dPsi_dvp[0] * (n02 ^ n02) / (vp[0] - vp[2]) +
             dPsi_dvp[1] * (n12 ^ n12) / (vp[1] - vp[2]) +
             dPsi_dvp[2] * ((n12 ^ n12) / (vp[2] - vp[1]) +
                            (n02 ^ n02) / (vp[2] - vp[0])));
      } else if (tfel::math::abs(vp(0) - vp(2)) < e) {
        d2Psi_ds2 +=
            (((d2Psi_dvp2[0] + d2Psi_dvp2[2] - 2 * d2Psi_dvp2[4]) / 2) *
                 (n02 ^ n02) +
             dPsi_dvp[1] * ((n01 ^ n01) / (vp[1] - vp[0]) +
                            (n12 ^ n12) / (vp[1] - vp[2])) +
             dPsi_dvp[0] * (n01 ^ n01) / (vp[0] - vp[1]) +
             dPsi_dvp[2] * (n12 ^ n12) / (vp[2] - vp[1]));
      } else if (tfel::math::abs(vp(1) - vp(2)) < e) {
        d2Psi_ds2 +=
            (((d2Psi_dvp2[1] + d2Psi_dvp2[2] - 2 * d2Psi_dvp2[5]) / 2) *
                 (n12 ^ n12) +
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
    }  // end of computeHosfordStressSecondDerivative

  }  // end of namespace internals

  template <typename StressStensor,
            typename HosfordExponentType,
            tfel::math::stensor_common::EigenSolver es>
  HosfordStressType<StressStensor> computeHosfordStress(
      const StressStensor& s,
      const HosfordExponentType a,
      const HosfordStressType<StressStensor> e) {
    using real = HosfordBaseType<StressStensor>;
    const auto seq = sigmaeq(s);
    if (seq < e) {
      return seq * 0;
    }
    const auto iseq = 1 / seq;
    const auto vp = s.template computeEigenValues<es>() * iseq;
    return seq * std::pow((std::pow(tfel::math::abs(vp[0] - vp[1]), a) +
                           std::pow(tfel::math::abs(vp[0] - vp[2]), a) +
                           std::pow(tfel::math::abs(vp[1] - vp[2]), a)) /
                              2,
                          1 / real(a));
  }  // end of computeHosfordYieldStress

  template <typename StressStensor,
            typename HosfordExponentType,
            tfel::math::stensor_common::EigenSolver es>
  std::tuple<HosfordStressType<StressStensor>,
             HosfordStressNormalType<StressStensor>>
  computeHosfordStressNormal(const StressStensor& s,
                             const HosfordExponentType a,
                             const HosfordStressType<StressStensor> e) {
    constexpr auto N = tfel::math::getSpaceDimension<StressStensor>();
    using stress = HosfordStressType<StressStensor>;
    using real = HosfordBaseType<StressStensor>;
    using normal = HosfordStressNormalType<StressStensor>;
    // Von Mises stress used to normalise the stress eigenvalues
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
    const real rPsi_a = (std::pow(tfel::math::abs(rvp[0] - rvp[1]), a) +
                         std::pow(tfel::math::abs(rvp[0] - rvp[2]), a) +
                         std::pow(tfel::math::abs(rvp[1] - rvp[2]), a)) /
                        2;
    // Hosford equivalent stress
    const real Psi = seq * std::pow(rPsi_a, 1 / real(a));
    // For the derivatives, the stress eigenvalues are normalised by
    // the Hosford equivalent stress
    const auto rvp2 = vp / Psi;
    // this structure is used for consistency with the code
    // computing the second derivative of the Hosford stress
    const tfel::math::tvector<3u, real> drvp2 = {
        rvp2[0] - rvp2[1], rvp2[0] - rvp2[2], rvp2[1] - rvp2[2]};
    const tfel::math::tvector<3u, real> drvp2_am2 = {
        real(std::pow(tfel::math::abs(drvp2[0]), a - 2)),
        real(std::pow(tfel::math::abs(drvp2[1]), a - 2)),
        real(std::pow(tfel::math::abs(drvp2[2]), a - 2))};
    const tfel::math::tvector<3u, real> dPsi_ds = {
        (drvp2[0] * drvp2_am2[0] + drvp2[1] * drvp2_am2[1]) / 2,
        (-drvp2[0] * drvp2_am2[0] + drvp2[2] * drvp2_am2[2]) / 2,
        (-drvp2[1] * drvp2_am2[1] - drvp2[2] * drvp2_am2[2]) / 2};
    return std::make_tuple(Psi, tfel::math::eval(dPsi_ds[0] * std::get<0>(n) +
                                                 dPsi_ds[1] * std::get<1>(n) +
                                                 dPsi_ds[2] * std::get<2>(n)));
  }  // end of computeHosfordStressNormal

  template <typename StressStensor,
            typename HosfordExponentType,
            tfel::math::stensor_common::EigenSolver es>
  std::tuple<HosfordStressType<StressStensor>,
             HosfordStressNormalType<StressStensor>,
             HosfordStressSecondDerivativeType<StressStensor>>
  computeHosfordStressSecondDerivative(
      const StressStensor& s,
      const HosfordExponentType a,
      const HosfordStressType<StressStensor> e) {
    constexpr auto N = tfel::math::getSpaceDimension<StressStensor>();
    using stress = HosfordStressType<StressStensor>;
    using real = HosfordBaseType<StressStensor>;
    using istress = tfel::math::result_type<real, stress, tfel::math::OpDiv>;
    using normal = HosfordStressNormalType<StressStensor>;
    using second_derivative = HosfordStressSecondDerivativeType<StressStensor>;
    // Von Mises stress used to normalise the stress eigenvalues
    const auto seq = sigmaeq(s);
    if (seq < e) {
      return std::make_tuple(stress{0}, normal{real{0}},
                             second_derivative{istress{0}});
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
    const real rPsi_a = (std::pow(tfel::math::abs(rvp[0] - rvp[1]), a) +
                         std::pow(tfel::math::abs(rvp[0] - rvp[2]), a) +
                         std::pow(tfel::math::abs(rvp[1] - rvp[2]), a)) /
                        2;
    // Hosford equivalent stress
    const real Psi = seq * std::pow(rPsi_a, 1 / real(a));
    // For the derivatives, the stress eigenvalues are normalised by
    // the Hosford equivalent stress
    const auto rvp2 = vp / Psi;
    // first derivative of the Hosford stress
    const tfel::math::tvector<3u, real> drvp2 = {
        rvp2[0] - rvp2[1], rvp2[0] - rvp2[2], rvp2[1] - rvp2[2]};
    const tfel::math::tvector<3u, real> drvp2_am2 = {
        real(std::pow(tfel::math::abs(drvp2[0]), a - 2)),
        real(std::pow(tfel::math::abs(drvp2[1]), a - 2)),
        real(std::pow(tfel::math::abs(drvp2[2]), a - 2))};
    const tfel::math::tvector<3u, real> dPsi_dsvp = {
        (drvp2[0] * drvp2_am2[0] + drvp2[1] * drvp2_am2[1]) / 2,
        (-drvp2[0] * drvp2_am2[0] + drvp2[2] * drvp2_am2[2]) / 2,
        (-drvp2[1] * drvp2_am2[1] - drvp2[2] * drvp2_am2[2]) / 2};
    const auto dPsi_ds = tfel::math::eval(dPsi_dsvp[0] * std::get<0>(n) +
                                          dPsi_dsvp[1] * std::get<1>(n) +
                                          dPsi_dsvp[2] * std::get<2>(n));
    // second derivative
    const auto cste = (a - 1) / Psi;
    const tfel::math::tvector<6u, istress> d2Psi_dsvp2 = {
        cste * ((drvp2_am2[0] + drvp2_am2[1]) / 2 -
                dPsi_dsvp[0] * dPsi_dsvp[0]),  // s1s1
        cste * ((drvp2_am2[0] + drvp2_am2[2]) / 2 -
                dPsi_dsvp[1] * dPsi_dsvp[1]),  // s2s2
        cste * ((drvp2_am2[1] + drvp2_am2[2]) / 2 -
                dPsi_dsvp[2] * dPsi_dsvp[2]),                         // s3s3
        cste * (-(drvp2_am2[0]) / 2 - dPsi_dsvp[0] * dPsi_dsvp[1]),   // s1s2
        cste * (-(drvp2_am2[1]) / 2 - dPsi_dsvp[0] * dPsi_dsvp[2]),   // s1s3
        cste * (-(drvp2_am2[2]) / 2 - dPsi_dsvp[1] * dPsi_dsvp[2])};  // s2s3
    tfel::math::st2tost2<N, istress> d2Psi_ds2;
    internals::computeHosfordStressSecondDerivative<StressStensor>(
        d2Psi_ds2, dPsi_dsvp, d2Psi_dsvp2, std::get<0>(n), std::get<1>(n),
        std::get<2>(n), vp, m, e);
    return std::make_tuple(Psi, dPsi_ds, d2Psi_ds2);
  }  // end of computeHosfordSecondDerivative

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_HOSFORD_IXX */
