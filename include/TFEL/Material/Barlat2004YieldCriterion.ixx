/*!
 * \file   Barlat.ixx
 * \brief
 * \author Thomas Helfer
 * \date   17 nov. 2017
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_BARLAT_IXX
#define LIB_TFEL_MATERIAL_BARLAT_IXX

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Material/OrthotropicStressLinearTransformation.hxx"

namespace tfel::material::internals {

  /*!
   * \brief add the terms relative to the eigenvectors derivatives
   * \param[out] d2Phi_ds2: second derivative of the Barlat equivalent stress
   * \param[in] dPhi_dvp: first derivative of the Barlat
   * equivalent stress with respect to the eigenvalues
   * \param[in] d2Phi_dvp2: second derivative of the Barlat
   * equivalent stress with respect to the eigenvalues
   * \param[in] vp: eigen values
   * \param[in] m: matrix for the eigen vectors
   * \param[in] e: criterion used to check if two eigenvalues are equal
   */
  template <tfel::math::StensorConcept StressStensor>
  TFEL_HOST_DEVICE constexpr void completeBaralatStressSecondDerivative(
      tfel::material::BarlatStressSecondDerivativeType<StressStensor>&,
      const tfel::math::tvector<3u,
                                tfel::material::BarlatBaseType<StressStensor>>&,
      const tfel::math::
          tvector<6u, tfel::material::BarlatInvertStressType<StressStensor>>&,
      const tfel::math::tvector<3u, BarlatStressType<StressStensor>>&,
      const tfel::math::tmatrix<3u, 3u, BarlatBaseType<StressStensor>>&,
      const tfel::material::BarlatStressType<StressStensor>) noexcept  //
      requires(tfel::math::getSpaceDimension<StressStensor>() == 1u) {
  }  // end of completeBaralatStressSecondDerivative
  /*!
   * \brief add the terms relative to the eigenvectors derivatives
   * \param[out] d2Phi_ds2: second derivative of the Barlat equivalent stress
   * \param[in] dPhi_dvp: first derivative of the Barlat
   * equivalent stress with respect to the eigenvalues
   * \param[in] d2Phi_dvp2: second derivative of the Barlat
   * equivalent stress with respect to the eigenvalues
   * \param[in] vp: eigen values
   * \param[in] m: matrix for the eigen vectors
   * \param[in] e: criterion used to check if two eigenvalues are equal
   */
  template <tfel::math::StensorConcept StressStensor>
  TFEL_HOST_DEVICE void completeBaralatStressSecondDerivative(
      tfel::material::BarlatStressSecondDerivativeType<StressStensor>&
          d2Phi_ds2,
      const tfel::math::tvector<3u,
                                tfel::material::BarlatBaseType<StressStensor>>&
          dPhi_dvp,
      const tfel::math::tvector<
          6u,
          tfel::material::BarlatInvertStressType<StressStensor>>& d2Phi_dvp2,
      const tfel::math::tvector<3u, BarlatStressType<StressStensor>>& vp,
      const tfel::math::tmatrix<3u, 3u, BarlatBaseType<StressStensor>>& m,
      const tfel::material::BarlatStressType<StressStensor> e)  //
      requires(tfel::math::getSpaceDimension<StressStensor>() == 2u) {
    using namespace tfel::math;
    using base = tfel::material::BarlatBaseType<StressStensor>;
    constexpr auto icste = Cste<base>::isqrt2;
    const tvector<3u, base> v0 = m.template column_view<0u>();
    const tvector<3u, base> v1 = m.template column_view<1u>();
    const stensor<2u, base> n01 =
        stensor<2u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v1) *
        icste;
    if (tfel::math::abs(vp[0] - vp[1]) < e) {
      d2Phi_ds2 += ((d2Phi_dvp2[0] + d2Phi_dvp2[1] - 2 * d2Phi_dvp2[3]) / 2) *
                   (n01 ^ n01);
    } else {
      d2Phi_ds2 += (dPhi_dvp[0] - dPhi_dvp[1]) / (vp[0] - vp[1]) * (n01 ^ n01);
    }
  }  // end of completeBaralatStressSecondDerivative
  /*!
   * \brief add the terms relative to the eigenvectors derivatives
   * \param[out] d2Phi_ds2: second derivative of the Barlat equivalent stress
   * \param[in] dPhi_dvp: first derivative of the Barlat
   * equivalent stress with respect to the eigenvalues
   * \param[in] d2Phi_dvp2: second derivative of the Barlat
   * equivalent stress with respect to the eigenvalues
   * \param[in] vp: eigen values
   * \param[in] m: matrix for the eigen vectors
   * \param[in] e: criterion used to check if two eigenvalues are equal
   */
  template <tfel::math::StensorConcept StressStensor>
  TFEL_HOST_DEVICE void completeBaralatStressSecondDerivative(
      tfel::material::BarlatStressSecondDerivativeType<StressStensor>&
          d2Phi_ds2,
      const tfel::math::tvector<3u,
                                tfel::material::BarlatBaseType<StressStensor>>&
          dPhi_dvp,
      const tfel::math::tvector<
          6u,
          tfel::material::BarlatInvertStressType<StressStensor>>& d2Phi_dvp2,
      const tfel::math::tvector<3u, BarlatStressType<StressStensor>>& vp,
      const tfel::math::tmatrix<3u, 3u, BarlatBaseType<StressStensor>>& m,
      const tfel::material::BarlatStressType<StressStensor> e)  //
      requires(tfel::math::getSpaceDimension<StressStensor>() == 3u) {
    using namespace tfel::math;
    using base = tfel::material::BarlatBaseType<StressStensor>;
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
    if (tfel::math::abs(vp[0] - vp[1]) < e) {
      d2Phi_ds2 += ((d2Phi_dvp2[0] + d2Phi_dvp2[1] - 2 * d2Phi_dvp2[3]) / 2) *
                   (n01 ^ n01);
    } else {
      d2Phi_ds2 += (dPhi_dvp[0] - dPhi_dvp[1]) / (vp[0] - vp[1]) * (n01 ^ n01);
    }
    if (tfel::math::abs(vp[0] - vp[2]) < e) {
      d2Phi_ds2 += ((d2Phi_dvp2[0] + d2Phi_dvp2[2] - 2 * d2Phi_dvp2[4]) / 2) *
                   (n02 ^ n02);
    } else {
      d2Phi_ds2 += (dPhi_dvp[0] - dPhi_dvp[2]) / (vp[0] - vp[2]) * (n02 ^ n02);
    }
    if (tfel::math::abs(vp[1] - vp[2]) < e) {
      d2Phi_ds2 += ((d2Phi_dvp2[1] + d2Phi_dvp2[2] - 2 * d2Phi_dvp2[5]) / 2) *
                   (n12 ^ n12);
    } else {
      d2Phi_ds2 += (dPhi_dvp[1] - dPhi_dvp[2]) / (vp[1] - vp[2]) * (n12 ^ n12);
    }
  }  // end of completeBaralatStressSecondDerivative

}  // end of namespace tfel::material::internals

namespace tfel::material {

  template <unsigned short N, typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::st2tost2<N, real>
  makeBarlatLinearTransformation(const real c12,
                                 const real c21,
                                 const real c13,
                                 const real c31,
                                 const real c23,
                                 const real c32,
                                 const real c44,
                                 const real c55,
                                 const real c66) noexcept {
    return makeOrthotropicStressLinearTransformation<N, real>(
        c12, c21, c13, c31, c23, c32, c44, c55, c66);
  }  // end of makeBarlatLinearTransformation

  template <unsigned short N, typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::st2tost2<N, real>
  makeBarlatLinearTransformation(
      const tfel::math::fsarray<9u, real>& c) noexcept {
    return makeOrthotropicStressLinearTransformation<N, real>(c);
  }  // end of makeBarlatLinearTransformation

  template <ModellingHypothesis::Hypothesis H,
            OrthotropicAxesConvention c,
            typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::
      st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
      makeBarlatLinearTransformation(const real c12,
                                     const real c21,
                                     const real c13,
                                     const real c31,
                                     const real c23,
                                     const real c32,
                                     const real c44,
                                     const real c55,
                                     const real c66) noexcept {
    return makeOrthotropicStressLinearTransformation<H, c, real>(
        c12, c21, c13, c31, c23, c32, c44, c55, c66);
  }  // end of makeBarlatLinearTransformation

  template <ModellingHypothesis::Hypothesis H,
            OrthotropicAxesConvention oac,
            typename real>
  TFEL_HOST_DEVICE constexpr tfel::math::
      st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
      makeBarlatLinearTransformation(
          const tfel::math::fsarray<9u, real>& c) noexcept {
    return makeOrthotropicStressLinearTransformation<H, oac, real>(c);
  }  // end of makeBarlatLinearTransformation

  template <tfel::math::StensorConcept StressStensor,
            typename BarlatExponentType,
            tfel::math::stensor_common::EigenSolver es>
  BarlatStressType<StressStensor> computeBarlatStress(
      const StressStensor& s,
      const BarlatLinearTransformationType<StressStensor>& l1,
      const BarlatLinearTransformationType<StressStensor>& l2,
      const BarlatExponentType a,
      const BarlatStressType<StressStensor> e) {
    using real = BarlatBaseType<StressStensor>;
    const auto seq = sigmaeq(s);
    if (seq < e) {
      return seq * 0;
    }
    const auto iseq = 1 / seq;
    const auto s1 = eval(l1 * s);
    const auto s2 = eval(l2 * s);
    const auto vp1 = s1.template computeEigenValues<es>() * iseq;
    const auto vp2 = s2.template computeEigenValues<es>() * iseq;
    return seq * std::pow((std::pow(tfel::math::abs(vp1[0] - vp2[0]), a) +
                           std::pow(tfel::math::abs(vp1[0] - vp2[1]), a) +
                           std::pow(tfel::math::abs(vp1[0] - vp2[2]), a) +
                           std::pow(tfel::math::abs(vp1[1] - vp2[0]), a) +
                           std::pow(tfel::math::abs(vp1[1] - vp2[1]), a) +
                           std::pow(tfel::math::abs(vp1[1] - vp2[2]), a) +
                           std::pow(tfel::math::abs(vp1[2] - vp2[0]), a) +
                           std::pow(tfel::math::abs(vp1[2] - vp2[1]), a) +
                           std::pow(tfel::math::abs(vp1[2] - vp2[2]), a)) /
                              4,
                          1 / real(a));
  }  // end of computeBarlatStress

  template <tfel::math::StensorConcept StressStensor,
            typename BarlatExponentType,
            tfel::math::stensor_common::EigenSolver es>
  std::tuple<BarlatStressType<StressStensor>,
             BarlatStressNormalType<StressStensor>>
  computeBarlatStressNormal(
      const StressStensor& s,
      const BarlatLinearTransformationType<StressStensor>& l1,
      const BarlatLinearTransformationType<StressStensor>& l2,
      const BarlatExponentType a,
      const BarlatStressType<StressStensor> e) {
    constexpr auto N = tfel::math::getSpaceDimension<StressStensor>();
    using stress = BarlatStressType<StressStensor>;
    using real = BarlatBaseType<StressStensor>;
    using normal = BarlatStressNormalType<StressStensor>;
    // Von Mises stress used to normalise the stress eigenvalues
    const auto seq = sigmaeq(s);
    if (seq < e) {
      return std::make_tuple(stress{0}, normal{real{0}});
    }
    const auto iseq = 1 / seq;
    // linear transformations
    const auto s1 = eval(l1 * s);
    const auto s2 = eval(l2 * s);
    // no structured bindings yet
    tfel::math::tvector<3u, stress> vp1;
    tfel::math::tmatrix<3u, 3u, real> m1;
    std::tie(vp1, m1) = s1.template computeEigenVectors<es>();
    const auto n1 = tfel::math::stensor<N, stress>::computeEigenTensors(m1);
    tfel::math::tvector<3u, stress> vp2;
    tfel::math::tmatrix<3u, 3u, real> m2;
    std::tie(vp2, m2) = s2.template computeEigenVectors<es>();
    const auto n2 = tfel::math::stensor<N, stress>::computeEigenTensors(m2);
    // Barlat stress
    const auto rvp1 = vp1 * iseq;
    const auto rvp2 = vp2 * iseq;
    const auto Phi_a = (std::pow(tfel::math::abs(rvp1[0] - rvp2[0]), a) +
                        std::pow(tfel::math::abs(rvp1[0] - rvp2[1]), a) +
                        std::pow(tfel::math::abs(rvp1[0] - rvp2[2]), a) +
                        std::pow(tfel::math::abs(rvp1[1] - rvp2[0]), a) +
                        std::pow(tfel::math::abs(rvp1[1] - rvp2[1]), a) +
                        std::pow(tfel::math::abs(rvp1[1] - rvp2[2]), a) +
                        std::pow(tfel::math::abs(rvp1[2] - rvp2[0]), a) +
                        std::pow(tfel::math::abs(rvp1[2] - rvp2[1]), a) +
                        std::pow(tfel::math::abs(rvp1[2] - rvp2[2]), a)) /
                       4;
    // Barlat equivalent stress
    const auto Phi = seq * std::pow(Phi_a, 1 / real(a));
    // For the derivatives, the stress eigenvalues are normalised by
    // the Barlat equivalent stress
    const auto rvp1b = tfel::math::eval(vp1 / Phi);
    const auto rvp2b = tfel::math::eval(vp2 / Phi);
    const tfel::math::tvector<9u, real> drvpb_am2 = {
        real(std::pow(tfel::math::abs(rvp1b[0] - rvp2b[0]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[0] - rvp2b[1]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[0] - rvp2b[2]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[1] - rvp2b[0]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[1] - rvp2b[1]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[1] - rvp2b[2]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[2] - rvp2b[0]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[2] - rvp2b[1]), a - 2)),
        real(std::pow(tfel::math::abs(rvp1b[2] - rvp2b[2]), a - 2))};
    const tfel::math::tvector<3u, real> dPhi_dsvp1 = {
        ((rvp1b[0] - rvp2b[0]) * drvpb_am2[0] +
         (rvp1b[0] - rvp2b[1]) * drvpb_am2[1] +
         (rvp1b[0] - rvp2b[2]) * drvpb_am2[2]) /
            4,
        ((rvp1b[1] - rvp2b[0]) * drvpb_am2[3] +
         (rvp1b[1] - rvp2b[1]) * drvpb_am2[4] +
         (rvp1b[1] - rvp2b[2]) * drvpb_am2[5]) /
            4,
        ((rvp1b[2] - rvp2b[0]) * drvpb_am2[6] +
         (rvp1b[2] - rvp2b[1]) * drvpb_am2[7] +
         (rvp1b[2] - rvp2b[2]) * drvpb_am2[8]) /
            4};
    const tfel::math::tvector<3u, real> dPhi_dsvp2 = {
        ((rvp2b[0] - rvp1b[0]) * drvpb_am2[0] +
         (rvp2b[0] - rvp1b[1]) * drvpb_am2[3] +
         (rvp2b[0] - rvp1b[2]) * drvpb_am2[6]) /
            4,
        ((rvp2b[1] - rvp1b[0]) * drvpb_am2[1] +
         (rvp2b[1] - rvp1b[1]) * drvpb_am2[4] +
         (rvp2b[1] - rvp1b[2]) * drvpb_am2[7]) /
            4,
        ((rvp2b[2] - rvp1b[0]) * drvpb_am2[2] +
         (rvp2b[2] - rvp1b[1]) * drvpb_am2[5] +
         (rvp2b[2] - rvp1b[2]) * drvpb_am2[8]) /
            4};
    const auto dPhi_ds1 = dPhi_dsvp1[0] * std::get<0>(n1) +
                          dPhi_dsvp1[1] * std::get<1>(n1) +
                          dPhi_dsvp1[2] * std::get<2>(n1);
    const auto dPhi_ds2 = dPhi_dsvp2[0] * std::get<0>(n2) +
                          dPhi_dsvp2[1] * std::get<1>(n2) +
                          dPhi_dsvp2[2] * std::get<2>(n2);
    return std::make_tuple(Phi, eval(dPhi_ds1 * l1 + dPhi_ds2 * l2));
  }  // end of computeBarlatStressNormal

  template <tfel::math::StensorConcept StressStensor,
            typename BarlatExponentType>
  BarlatStressAndDerivativesWithRespectToEigenvalues<StressStensor>
  computeBarlatStressSecondDerivative(
      const tfel::math::tvector<3u, BarlatStressType<StressStensor>>& vp1,
      const tfel::math::tvector<3u, BarlatStressType<StressStensor>>& vp2,
      const BarlatStressType<StressStensor> seq,
      const BarlatExponentType a) {
    using real = BarlatBaseType<StressStensor>;
    BarlatStressAndDerivativesWithRespectToEigenvalues<StressStensor> d;
    // Barlat equivalent stress
    const auto iseq = 1 / seq;
    const auto rvp1 = vp1 * iseq;
    const auto rvp2 = vp2 * iseq;
    const auto Phi_a = (std::pow(tfel::math::abs(rvp1[0] - rvp2[0]), a) +
                        std::pow(tfel::math::abs(rvp1[0] - rvp2[1]), a) +
                        std::pow(tfel::math::abs(rvp1[0] - rvp2[2]), a) +
                        std::pow(tfel::math::abs(rvp1[1] - rvp2[0]), a) +
                        std::pow(tfel::math::abs(rvp1[1] - rvp2[1]), a) +
                        std::pow(tfel::math::abs(rvp1[1] - rvp2[2]), a) +
                        std::pow(tfel::math::abs(rvp1[2] - rvp2[0]), a) +
                        std::pow(tfel::math::abs(rvp1[2] - rvp2[1]), a) +
                        std::pow(tfel::math::abs(rvp1[2] - rvp2[2]), a)) /
                       4;
    d.Phi = seq * std::pow(Phi_a, 1 / real(a));
    // For the derivatives, the stress eigenvalues are normalised by
    // the Barlat equivalent stress
    const auto rvp1b = tfel::math::eval(vp1 / d.Phi);
    const auto rvp2b = tfel::math::eval(vp2 / d.Phi);
    const tfel::math::tvector<9u, real> drvpb_am2 = {
        real(
            std::pow(tfel::math::abs(rvp1b[0] - rvp2b[0]), a - 2)),  // s'1-s''1
        real(
            std::pow(tfel::math::abs(rvp1b[0] - rvp2b[1]), a - 2)),  // s'1-s''2
        real(
            std::pow(tfel::math::abs(rvp1b[0] - rvp2b[2]), a - 2)),  // s'1-s''3
        real(
            std::pow(tfel::math::abs(rvp1b[1] - rvp2b[0]), a - 2)),  // s'2-s''1
        real(
            std::pow(tfel::math::abs(rvp1b[1] - rvp2b[1]), a - 2)),  // s'2-s''2
        real(
            std::pow(tfel::math::abs(rvp1b[1] - rvp2b[2]), a - 2)),  // s'2-s''3
        real(
            std::pow(tfel::math::abs(rvp1b[2] - rvp2b[0]), a - 2)),  // s'3-s''1
        real(
            std::pow(tfel::math::abs(rvp1b[2] - rvp2b[1]), a - 2)),  // s'3-s''2
        real(std::pow(tfel::math::abs(rvp1b[2] - rvp2b[2]),
                      a - 2))};  // s'3-s''3
    d.dPhi_dsvp1 = {((rvp1b[0] - rvp2b[0]) * drvpb_am2[0] +
                     (rvp1b[0] - rvp2b[1]) * drvpb_am2[1] +
                     (rvp1b[0] - rvp2b[2]) * drvpb_am2[2]) /
                        4,
                    ((rvp1b[1] - rvp2b[0]) * drvpb_am2[3] +
                     (rvp1b[1] - rvp2b[1]) * drvpb_am2[4] +
                     (rvp1b[1] - rvp2b[2]) * drvpb_am2[5]) /
                        4,
                    ((rvp1b[2] - rvp2b[0]) * drvpb_am2[6] +
                     (rvp1b[2] - rvp2b[1]) * drvpb_am2[7] +
                     (rvp1b[2] - rvp2b[2]) * drvpb_am2[8]) /
                        4};
    d.dPhi_dsvp2 = {((rvp2b[0] - rvp1b[0]) * drvpb_am2[0] +
                     (rvp2b[0] - rvp1b[1]) * drvpb_am2[3] +
                     (rvp2b[0] - rvp1b[2]) * drvpb_am2[6]) /
                        4,
                    ((rvp2b[1] - rvp1b[0]) * drvpb_am2[1] +
                     (rvp2b[1] - rvp1b[1]) * drvpb_am2[4] +
                     (rvp2b[1] - rvp1b[2]) * drvpb_am2[7]) /
                        4,
                    ((rvp2b[2] - rvp1b[0]) * drvpb_am2[2] +
                     (rvp2b[2] - rvp1b[1]) * drvpb_am2[5] +
                     (rvp2b[2] - rvp1b[2]) * drvpb_am2[8]) /
                        4};
    // second derivative
    const auto cste = (a - 1) / d.Phi;
    d.d2Phi_dsvp12 = {cste * ((drvpb_am2[0] + drvpb_am2[1] + drvpb_am2[2]) / 4 -
                              d.dPhi_dsvp1[0] * d.dPhi_dsvp1[0]),  // s1s1
                      cste * ((drvpb_am2[3] + drvpb_am2[4] + drvpb_am2[5]) / 4 -
                              d.dPhi_dsvp1[1] * d.dPhi_dsvp1[1]),  // s2s2
                      cste * ((drvpb_am2[6] + drvpb_am2[7] + drvpb_am2[8]) / 4 -
                              d.dPhi_dsvp1[2] * d.dPhi_dsvp1[2]),  // s3s3
                      -cste * d.dPhi_dsvp1[0] * d.dPhi_dsvp1[1],   // s1s2
                      -cste * d.dPhi_dsvp1[0] * d.dPhi_dsvp1[2],   // s1s3
                      -cste * d.dPhi_dsvp1[1] * d.dPhi_dsvp1[2]};  // s2s3
    d.d2Phi_dsvp22 = {cste * ((drvpb_am2[0] + drvpb_am2[3] + drvpb_am2[6]) / 4 -
                              d.dPhi_dsvp2[0] * d.dPhi_dsvp2[0]),  // s1s1
                      cste * ((drvpb_am2[1] + drvpb_am2[4] + drvpb_am2[7]) / 4 -
                              d.dPhi_dsvp2[1] * d.dPhi_dsvp2[1]),  // s2s2
                      cste * ((drvpb_am2[2] + drvpb_am2[5] + drvpb_am2[8]) / 4 -
                              d.dPhi_dsvp2[2] * d.dPhi_dsvp2[2]),  // s3s3
                      -cste * d.dPhi_dsvp2[0] * d.dPhi_dsvp2[1],   // s1s2
                      -cste * d.dPhi_dsvp2[0] * d.dPhi_dsvp2[2],   // s1s3
                      -cste * d.dPhi_dsvp2[1] * d.dPhi_dsvp2[2]};  // s2s3
    d.d2Phi_dsvp1dsvp2 = {
        -cste *
            (drvpb_am2[0] / 4 + d.dPhi_dsvp1[0] * d.dPhi_dsvp2[0]),  // s'1 s''1
        -cste *
            (drvpb_am2[1] / 4 + d.dPhi_dsvp1[0] * d.dPhi_dsvp2[1]),  // s'1 s''2
        -cste *
            (drvpb_am2[2] / 4 + d.dPhi_dsvp1[0] * d.dPhi_dsvp2[2]),  // s'1 s''3
        -cste *
            (drvpb_am2[3] / 4 + d.dPhi_dsvp1[1] * d.dPhi_dsvp2[0]),  // s'2 s''1
        -cste *
            (drvpb_am2[4] / 4 + d.dPhi_dsvp1[1] * d.dPhi_dsvp2[1]),  // s'2 s''2
        -cste *
            (drvpb_am2[5] / 4 + d.dPhi_dsvp1[1] * d.dPhi_dsvp2[2]),  // s'2 s''3
        -cste *
            (drvpb_am2[6] / 4 + d.dPhi_dsvp1[2] * d.dPhi_dsvp2[0]),  // s'3 s''1
        -cste *
            (drvpb_am2[7] / 4 + d.dPhi_dsvp1[2] * d.dPhi_dsvp2[1]),  // s'3 s''2
        -cste * (drvpb_am2[8] / 4 +
                 d.dPhi_dsvp1[2] * d.dPhi_dsvp2[2])};  // s'3 s''3
    return d;
  }  // end of BarlatStressSecondDerivative

  template <tfel::math::StensorConcept StressStensor,
            typename BarlatExponentType,
            tfel::math::stensor_common::EigenSolver es>
  std::tuple<BarlatStressType<StressStensor>,
             BarlatStressNormalType<StressStensor>,
             BarlatStressSecondDerivativeType<StressStensor>>
  computeBarlatStressSecondDerivative(
      const StressStensor& s,
      const BarlatLinearTransformationType<StressStensor>& l1,
      const BarlatLinearTransformationType<StressStensor>& l2,
      const BarlatExponentType a,
      const BarlatStressType<StressStensor> e) {
    constexpr auto N = tfel::math::getSpaceDimension<StressStensor>();
    using stress = BarlatStressType<StressStensor>;
    using real = BarlatBaseType<StressStensor>;
    using normal = BarlatStressNormalType<StressStensor>;
    using istress = BarlatInvertStressType<StressStensor>;
    using second_derivative = BarlatStressSecondDerivativeType<StressStensor>;
    // Von Mises stress used to normalise the stress eigenvalues
    const auto seq = sigmaeq(s);
    if (seq < e) {
      return std::make_tuple(stress{0}, normal{real{0}},
                             second_derivative{istress{0}});
    }
    // linear transformations
    const auto tl1 = transpose(l1);
    const auto tl2 = transpose(l2);
    const auto s1 = eval(l1 * s);
    const auto s2 = eval(l2 * s);
    // no structured bindings yet
    tfel::math::tvector<3u, stress> vp1;
    tfel::math::tmatrix<3u, 3u, real> m1;
    std::tie(vp1, m1) = s1.template computeEigenVectors<es>();
    const auto n1 = tfel::math::stensor<N, stress>::computeEigenTensors(m1);
    tfel::math::tvector<3u, stress> vp2;
    tfel::math::tmatrix<3u, 3u, real> m2;
    std::tie(vp2, m2) = s2.template computeEigenVectors<es>();
    const auto n2 = tfel::math::stensor<N, stress>::computeEigenTensors(m2);
    // derivatives of the Barlat potentiel with respect of the eigen
    // values of the s1 and s2
    const auto d =
        computeBarlatStressSecondDerivative<StressStensor>(vp1, vp2, seq, a);
    // Barlat normal
    const auto dPhi_ds1 =
        (d.dPhi_dsvp1[0] * std::get<0>(n1) + d.dPhi_dsvp1[1] * std::get<1>(n1) +
         d.dPhi_dsvp1[2] * std::get<2>(n1));
    const auto dPhi_ds2 =
        (d.dPhi_dsvp2[0] * std::get<0>(n2) + d.dPhi_dsvp2[1] * std::get<1>(n2) +
         d.dPhi_dsvp2[2] * std::get<2>(n2));
    const auto dPhi_ds = eval(dPhi_ds1 * l1 + dPhi_ds2 * l2);
    // second derivatives with respect to s'
    auto d2Phi_ds1ds1 = eval(
        d.d2Phi_dsvp12[0] * (std::get<0>(n1) ^ std::get<0>(n1)) +  // s'1s'1
        d.d2Phi_dsvp12[3] * (std::get<0>(n1) ^ std::get<1>(n1)) +  // s'1s'2
        d.d2Phi_dsvp12[4] * (std::get<0>(n1) ^ std::get<2>(n1)) +  // s'1s'3
        d.d2Phi_dsvp12[3] * (std::get<1>(n1) ^ std::get<0>(n1)) +  // s'2s'1
        d.d2Phi_dsvp12[1] * (std::get<1>(n1) ^ std::get<1>(n1)) +  // s'2s'2
        d.d2Phi_dsvp12[5] * (std::get<1>(n1) ^ std::get<2>(n1)) +  // s'2s'3
        d.d2Phi_dsvp12[4] * (std::get<2>(n1) ^ std::get<0>(n1)) +  // s'3s'1
        d.d2Phi_dsvp12[5] * (std::get<2>(n1) ^ std::get<1>(n1)) +  // s'3s'2
        d.d2Phi_dsvp12[2] * (std::get<2>(n1) ^ std::get<2>(n1)));  // s'3s'3
    // a now, terms related to the eigen vectors derivatives
    internals::completeBaralatStressSecondDerivative<StressStensor>(
        d2Phi_ds1ds1, d.dPhi_dsvp1, d.d2Phi_dsvp12, vp1, m1, e);
    // second derivatives with respect to s's''
    const auto d2Phi_ds1ds2 =
        eval(d.d2Phi_dsvp1dsvp2[0] *
                 (std::get<0>(n1) ^ std::get<0>(n2)) +  // s'1s''1
             d.d2Phi_dsvp1dsvp2[1] *
                 (std::get<0>(n1) ^ std::get<1>(n2)) +  // s'1s''2
             d.d2Phi_dsvp1dsvp2[2] *
                 (std::get<0>(n1) ^ std::get<2>(n2)) +  // s'1s''3
             d.d2Phi_dsvp1dsvp2[3] *
                 (std::get<1>(n1) ^ std::get<0>(n2)) +  // s'2s''1
             d.d2Phi_dsvp1dsvp2[4] *
                 (std::get<1>(n1) ^ std::get<1>(n2)) +  // s'2s''2
             d.d2Phi_dsvp1dsvp2[5] *
                 (std::get<1>(n1) ^ std::get<2>(n2)) +  // s'2s''3
             d.d2Phi_dsvp1dsvp2[6] *
                 (std::get<2>(n1) ^ std::get<0>(n2)) +  // s'3s''1
             d.d2Phi_dsvp1dsvp2[7] *
                 (std::get<2>(n1) ^ std::get<1>(n2)) +  // s'3s''2
             d.d2Phi_dsvp1dsvp2[8] *
                 (std::get<2>(n1) ^ std::get<2>(n2)));  // s'3s''3
    // second derivatives with respect to s''
    auto d2Phi_ds2ds2 = eval(
        d.d2Phi_dsvp22[0] * (std::get<0>(n2) ^ std::get<0>(n2)) +  // s''1s''1
        d.d2Phi_dsvp22[3] * (std::get<0>(n2) ^ std::get<1>(n2)) +  // s''1s''2
        d.d2Phi_dsvp22[4] * (std::get<0>(n2) ^ std::get<2>(n2)) +  // s''1s''3
        d.d2Phi_dsvp22[3] * (std::get<1>(n2) ^ std::get<0>(n2)) +  // s''2s''1
        d.d2Phi_dsvp22[1] * (std::get<1>(n2) ^ std::get<1>(n2)) +  // s''2s''2
        d.d2Phi_dsvp22[5] * (std::get<1>(n2) ^ std::get<2>(n2)) +  // s''2s''3
        d.d2Phi_dsvp22[4] * (std::get<2>(n2) ^ std::get<0>(n2)) +  // s''3s''1
        d.d2Phi_dsvp22[5] * (std::get<2>(n2) ^ std::get<1>(n2)) +  // s''3s''2
        d.d2Phi_dsvp22[2] * (std::get<2>(n2) ^ std::get<2>(n2)));  // s''3s''3
    // a now, terms related to the eigen vectors derivatives
    internals::completeBaralatStressSecondDerivative<StressStensor>(
        d2Phi_ds2ds2, d.dPhi_dsvp2, d.d2Phi_dsvp22, vp2, m2, e);
    // second derivatives with respect to s''s'
    const auto d2Phi_ds2ds1 =
        eval(d.d2Phi_dsvp1dsvp2[0] *
                 (std::get<0>(n2) ^ std::get<0>(n1)) +  // s''1s'1
             d.d2Phi_dsvp1dsvp2[3] *
                 (std::get<0>(n2) ^ std::get<1>(n1)) +  // s''1s'2
             d.d2Phi_dsvp1dsvp2[6] *
                 (std::get<0>(n2) ^ std::get<2>(n1)) +  // s''1s'3
             d.d2Phi_dsvp1dsvp2[1] *
                 (std::get<1>(n2) ^ std::get<0>(n1)) +  // s''2s'1
             d.d2Phi_dsvp1dsvp2[4] *
                 (std::get<1>(n2) ^ std::get<1>(n1)) +  // s''2s'2
             d.d2Phi_dsvp1dsvp2[7] *
                 (std::get<1>(n2) ^ std::get<2>(n1)) +  // s''2s'3
             d.d2Phi_dsvp1dsvp2[2] *
                 (std::get<2>(n2) ^ std::get<0>(n1)) +  // s''3s'1
             d.d2Phi_dsvp1dsvp2[5] *
                 (std::get<2>(n2) ^ std::get<1>(n1)) +  // s''3s'2
             d.d2Phi_dsvp1dsvp2[8] *
                 (std::get<2>(n2) ^ std::get<2>(n1)));  // s''3s'3
    const auto d2Phi_ds2 = eval(tl1 * (d2Phi_ds1ds1 * l1 + d2Phi_ds1ds2 * l2) +
                                tl2 * (d2Phi_ds2ds1 * l1 + d2Phi_ds2ds2 * l2));
    return std::make_tuple(d.Phi, dPhi_ds, d2Phi_ds2);
  }  // end of computeBarlatSecondDerivative

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_BARLAT_IXX */
