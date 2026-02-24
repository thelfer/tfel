/*!
 * \file   include/TFEL/Material/ChuNeedleman1980StrainBasedNucleationModel.ixx
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

#ifndef LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_IXX
#define LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_IXX

namespace tfel::material {

  template <typename real>
  std::ostream& operator<<(
      std::ostream& os,
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>& p) {
    os << '{' << p.fn << ", " << p.en << ", " << p.sn << '}';
    return os;
  }  // end of operator<<

  template <typename real>
  real computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactor(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>& params,
      const real p) {
    // 1 / sqrt(2*pi)
    constexpr auto cste = real(0.398942280401433);
    const auto x = (p - params.en) / (params.sn);
    // porosity rate
    const auto fr = cste * params.fn / (params.sn) * std::exp(-x * x / 2);
    return fr;
  }  // end of computeChuNeedleman1980StrainBasedNucleationModelPorosityRate

  template <typename real>
  std::tuple<real, real>
  computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactorAndDerivative(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>& params,
      const real p) {
    // 1 / sqrt(2*pi)
    constexpr auto cste = real(0.398942280401433);
    const auto x = (p - params.en) / (params.sn);
    // porosity rate
    const auto fr = cste * params.fn / (params.sn) * std::exp(-x * x / 2);
    // derivative of the porosity rate with respect to the equivalent plastic
    // strain
    const auto dfr_dp = fr * x / (params.sn);
    //
    return std::make_tuple(fr, dfr_dp);
  }  // end of computeChuNeedleman1980StrainBasedNucleationModelPorosityRate

  template <typename real>
  real computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrement(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>& params,
      const real p,
      const real dp,
      const real) {
    if (dp < 0) {
      return 0;
    }
    constexpr auto cste = tfel::math::Cste<real>::sqrt2;
    const auto Fa = std::erf((p - params.en) / (cste * (params.sn)));
    const auto Fb = std::erf((p + dp - params.en) / (cste * (params.sn)));
    return (params.fn) * (Fb - Fa) / 2;
  }  // end of
     // computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrement

  template <typename real>
  std::tuple<real, real>
  computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrementAndDerivative(
      const ChuNeedleman1980StrainBasedNucleationModelParameters<real>& params,
      const real p,
      const real dp,
      const real) {
    if (dp < 0) {
      return std::make_tuple(real(0), real(0));
    }
    constexpr auto cste = tfel::math::Cste<real>::sqrt2;
    const auto Fa = std::erf((p - params.en) / (cste * (params.sn)));
    const auto Fb = std::erf((p + dp - params.en) / (cste * (params.sn)));
    const auto An =
        computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactor(
            params, p);
    return std::make_tuple((params.fn) * (Fb - Fa) / 2, An);
  }  // end of
     // computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrement

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_IXX */
