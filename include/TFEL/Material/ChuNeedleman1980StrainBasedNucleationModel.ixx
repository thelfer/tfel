/*!
 * \file   include/TFEL/Material/ChuNeedleman1980StrainBasedNucleationModel.ixx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_IXX
#define LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_IXX

namespace tfel {

  namespace material {

    template <typename real>
    std::ostream& operator<<(
        std::ostream& os,
        const ChuNeedleman1980StrainBasedNucleationModelParameters<real>& p) {
      os << '{' << p.fn << ", " << p.en << ", " << p.sn << '}';
      return os;
    }  // end of operator<<

    template <typename real>
    std::tuple<real, real>
    computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactor(
        const real p,
        const ChuNeedleman1980StrainBasedNucleationModelParameters<real>&
            params) {
      // 1 / sqrt(2*pi)
      constexpr const auto cste = 0.398942280401433;
      const auto x = (p - params.en) / (params.sn);
      // porosity rate
      const auto fr = cste * params.fn / (params.sn) * std::exp(-x * x / 2);
      // derivative of the porosity rate with respect to the equivalent plastic
      // strain
      const auto dfr_dp = fr * x / (params.sn);
      //
      return std::make_tuple(fr, dfr_dp);
    }  // end of computeChuNeedleman1980StrainBasedNucleationModelPorosityRate

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_CHUNEEDLEMAN1980STRAINBASEDNUCLEATIONMODEL_IXX */
