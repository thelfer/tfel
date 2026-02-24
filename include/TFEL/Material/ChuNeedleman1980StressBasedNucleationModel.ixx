/*!
 * \file   include/TFEL/Material/ChuNeedleman1980StressBasedNucleationModel.ixx
 * \brief
 * \author Thomas Helfer
 * \date   04/08/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRESSBASEDNUCLEATIONMODEL_IXX
#define LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRESSBASEDNUCLEATIONMODEL_IXX

namespace tfel::material {

  template <typename StressStensorType>
  std::ostream& operator<<(
      std::ostream& os,
      const ChuNeedleman1980StressBasedNucleationModelParameters<
          StressStensorType>& p) {
    os << '{' << p.fn << ", " << p.sigm << ", " << p.sn << '}';
    return os;
  }  // end of operator<<

  template <typename StressStensorType>
  ChuNeedleman1980StressBasedNucleationModelRealType<StressStensorType>
  computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactor(
      const ChuNeedleman1980StressBasedNucleationModelParameters<
          StressStensorType>& params,
      const StressStensorType& sig,
      const ChuNeedleman1980StressBasedNucleationModelRealType<
          StressStensorType>&) {
    using real =
        ChuNeedleman1980StressBasedNucleationModelRealType<StressStensorType>;
    // 1 / sqrt(2*pi)
    constexpr auto cste = real(0.398942280401433);
    auto vp = sig.computeEigenValues();
    const auto s1 = *(tfel::fsalgo::max_element<3u>::exe(vp.begin()));
    const auto x = (s1 - params.sigm) / (params.sn);
    const auto An = cste * params.fn / (params.sn) * std::exp(-x * x / 2);
    return An;
  }  // end of computeChuNeedleman1980StressBasedNucleationModelPorosityRate

  template <typename StressStensorType>
  std::tuple<
      ChuNeedleman1980StressBasedNucleationModelRealType<StressStensorType>,
      ChuNeedleman1980StressBasedNucleationModeDerivativeType<
          StressStensorType>>
  computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactorAndDerivative(
      const ChuNeedleman1980StressBasedNucleationModelParameters<
          StressStensorType>& params,
      const StressStensorType& sig,
      const ChuNeedleman1980StressBasedNucleationModelRealType<
          StressStensorType>&) {
    using real =
        ChuNeedleman1980StressBasedNucleationModelRealType<StressStensorType>;
    using stress =
        ChuNeedleman1980StressBasedNucleationModelStressType<StressStensorType>;
    constexpr auto N = tfel::math::getSpaceDimension<StressStensorType>();
    using Stensor = tfel::math::stensor<N, real>;
    // 1 / sqrt(2*pi)
    constexpr auto cste = real(0.398942280401433);
    //
    auto vp = tfel::math::tvector<3u, stress>{};
    auto m = tfel::math::tmatrix<3u, 3u, real>{};
    std::tie(vp, m) = sig.computeEigenVectors();
    const auto idx =
        tfel::fsalgo::max_element<3u>::exe(vp.begin()) - vp.begin();
    const auto s1 = vp[idx];
    auto dvp_dsig = tfel::math::tvector<3u, Stensor>{};
    sig.computeEigenTensors(dvp_dsig[0], dvp_dsig[1], dvp_dsig[2], m);
    const auto& n1 = dvp_dsig[idx];
    //
    const auto x = (s1 - params.sigm) / (params.sn);
    // porosity rate
    const auto An = cste * params.fn / (params.sn) * std::exp(-x * x / 2);
    // derivative of the porosity rate with respect to the equivalent plastic
    // strain
    const auto dAn_ds1 = An * x / (params.sn);
    //
    return std::make_tuple(An, dAn_ds1 * n1);
  }  // end of computeChuNeedleman1980StressBasedNucleationModelPorosityRate

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRESSBASEDNUCLEATIONMODEL_IXX */
