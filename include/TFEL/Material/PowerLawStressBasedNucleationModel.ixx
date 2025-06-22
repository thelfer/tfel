/*!
 * \file   include/TFEL/Material/PowerLawStressBasedNucleationModel.ixx
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

#ifndef LIB_TFEL_MATERIAL_POWERLAWSTRESSBASEDNUCLEATIONMODEL_IXX
#define LIB_TFEL_MATERIAL_POWERLAWSTRESSBASEDNUCLEATIONMODEL_IXX

namespace tfel {

  namespace material {

    template <typename StressStensorType>
    std::ostream& operator<<(
        std::ostream& os,
        const PowerLawStressBasedNucleationModelParameters<StressStensorType>&
            p) {
      os << '{' << p.fn << ", " << p.sn << ", " << p.m << '}';
      return os;
    }  // end of operator<<

    template <typename StressStensorType>
    PowerLawStressBasedNucleationModelRealType<StressStensorType>
    computePowerLawStressBasedNucleationModelPorosityRateFactor(
        const PowerLawStressBasedNucleationModelParameters<StressStensorType>&
            params,
        const StressStensorType& sig,
        const PowerLawStressBasedNucleationModelRealType<StressStensorType>&
            p) {
      if (p < params.pmin) {
        return 0;
      }
      auto vp = sig.computeEigenValues();
      const auto s1 = *(tfel::fsalgo::max_element<3u>::exe(vp.begin()));
      const auto rs = s1 / params.sn - 1;
      if (rs < 0) {
        return 0;
      }
      return (params.fn) * std::pow(rs, params.m);
    }  // end of computePowerLawStressBasedNucleationModelPorosityRate

    template <typename StressStensorType>
    std::tuple<
        PowerLawStressBasedNucleationModelRealType<StressStensorType>,
        PowerLawStressBasedNucleationModeDerivativeType<StressStensorType>>
    computePowerLawStressBasedNucleationModelPorosityRateFactorAndDerivative(
        const PowerLawStressBasedNucleationModelParameters<StressStensorType>&
            params,
        const StressStensorType& sig,
        const PowerLawStressBasedNucleationModelRealType<StressStensorType>&
            p) {
      using real =
          PowerLawStressBasedNucleationModelRealType<StressStensorType>;
      using stress =
          PowerLawStressBasedNucleationModelStressType<StressStensorType>;
      constexpr const auto N =
          tfel::math::StensorTraits<StressStensorType>::dime;
      using Stensor = tfel::math::stensor<N, real>;
      //
      if (p < params.pmin) {
        return std::make_tuple(real(0), Stensor(real(0)));
      }
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
      const auto rs1 = s1 / params.sn - 1;
      if (rs1 < 0) {
        return std::make_tuple(real(0), Stensor(real(0)));
      }
      const auto irs1 = std::max(rs1, 1e-12);
      const auto An = (params.fn) * std::pow(rs1, params.m);
      const auto dAn_ds1 = (params.m / params.sn) * An * irs1;
      //
      return std::make_tuple(An, dAn_ds1 * n1);
    }  // end of computePowerLawStressBasedNucleationModelPorosityRate

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_POWERLAWSTRESSBASEDNUCLEATIONMODEL_IXX */
