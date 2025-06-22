/*!
 * \file   include/TFEL/Material/PowerLawStrainBasedNucleationModel.ixx
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

#ifndef LIB_TFEL_MATERIAL_POWERLAWSTRAINBASEDNUCLEATIONMODEL_IXX
#define LIB_TFEL_MATERIAL_POWERLAWSTRAINBASEDNUCLEATIONMODEL_IXX

#include <cmath>

namespace tfel {

  namespace material {

    template <typename real>
    std::ostream& operator<<(
        std::ostream& os,
        const PowerLawStrainBasedNucleationModelParameters<real>& p) {
      os << '{' << p.fn << ", " << p.en << ", " << p.m << '}';
      return os;
    }  // end of operator<<

    template <typename real>
    real computePowerLawStrainBasedNucleationModelPorosityRateFactor(
        const PowerLawStrainBasedNucleationModelParameters<real>& params,
        const real p) {
      if (p < params.en) {
        return 0;
      }
      return (params.fn) * (std::pow(p / params.en - 1, params.m));
    }  // end of computePowerLawStrainBasedNucleationModelPorosityRate

    template <typename real>
    std::tuple<real, real>
    computePowerLawStrainBasedNucleationModelPorosityRateFactorAndDerivative(
        const PowerLawStrainBasedNucleationModelParameters<real>& params,
        const real p) {
      if (p < params.en) {
        return {real(0), real(0)};
      }
      const auto rp = p / params.en - 1;
      const auto irp = std::max(rp, 1e-12);
      const auto An = (params.fn) * std::pow(rp, params.m);
      const auto dAn_dp = (params.m / params.en) * An * irp;
      return std::make_tuple(An, dAn_dp);
    }  // end of computePowerLawStrainBasedNucleationModelPorosityRate

    template <typename real>
    real computePowerLawStrainBasedNucleationModelPorosityIncrement(
        const PowerLawStrainBasedNucleationModelParameters<real>& params,
        const real p,
        const real dp,
        const real) {
      if ((dp < 0) || (p + dp < params.en)) {
        return 0;
      }
      const auto pa = std::max(p, params.en);
      auto F = [&params](const real v) {
        return (params.fn) * (params.en) / (params.m + 1) *
               pow(v / (params.en) - 1, params.m + 1);
      };
      return F(p + dp) - F(pa);
    }

    template <typename real>
    std::tuple<real, real>
    computePowerLawStrainBasedNucleationModelPorosityIncrementAndDerivative(
        const PowerLawStrainBasedNucleationModelParameters<real>& params,
        const real p,
        const real dp,
        const real) {
      if ((dp < 0) || (p + dp < params.en)) {
        return std::make_tuple(real(0), real(0));
      }
      const auto pa = std::max(p, params.en);
      auto Fa = (params.fn) * (params.en) / (params.m + 1) *
                pow((pa / (params.en) - 1), params.m + 1);
      const auto rp = (p + dp) / (params.en) - 1;
      const auto rp_m = pow(rp, params.m);
      auto Fb = (params.fn) * (params.en) / (params.m + 1) * rp_m * rp;
      return std::make_tuple(Fb - Fa, (params.fn) * rp_m);
    }

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_POWERLAWSTRAINBASEDNUCLEATIONMODEL_IXX */
