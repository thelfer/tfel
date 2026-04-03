/*!
 * \file
 * include/TFEL/Material/LogarithmicStrainComputeAxialStrainIncrementElasticPrediction.ixx
 * \brief
 * \author Thomas Helfer
 * \date   14/05/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LOGARITHMICSTRAINCOMPUTEAXIALSTRAININCREMENTELASTICPREDICTION_IXX
#define LIB_TFEL_MATERIAL_LOGARITHMICSTRAINCOMPUTEAXIALSTRAININCREMENTELASTICPREDICTION_IXX

#include <cmath>
#include <iostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Material/MaterialException.hxx"

namespace tfel::material {

  template <typename stress>
  tfel::typetraits::base_type<stress>
  computeAxialStrainIncrementElasticPrediction(
      const stress D10,
      const stress D11,
      const stress D12,
      const tfel::math::stensor<1u, tfel::typetraits::base_type<stress>>& eel,
      const tfel::typetraits::base_type<stress> eto_zz,
      const tfel::math::stensor<1u, tfel::typetraits::base_type<stress>>& deto,
      const stress Pzz,
      const tfel::typetraits::base_type<stress> t,
      const tfel::typetraits::base_type<stress> e) {
    using real = tfel::typetraits::base_type<stress>;
    using index_type = unsigned short;
    constexpr auto iter_max = index_type(5);
    const auto Tzz = D10 * eel(0) + D11 * eel(1) + D12 * eel(2);
    const auto a = Pzz / D11;
    const auto b = (Tzz + t * (D10 * deto(0) + D12 * deto(2))) / D11;
    auto fdf = [a, b, eto_zz, t, &deto](const real x) -> std::pair<real, real> {
      const auto exp_eto_zz = std::exp(eto_zz + t * (deto(1) + x));
      return {t * (deto(1) + x) + b - a * exp_eto_zz, t - a * t * exp_eto_zz};
    };
    auto x = real{};
    auto fdfv = fdf(x);
    const auto& f = fdfv.first;
    const auto& df = fdfv.second;
    auto iter = index_type{};
    while ((tfel::math::abs(f) > e) || (tfel::math::abs(f) > e * df)) {
      if (iter == iter_max) {
        tfel::raise<DivergenceException>(
            "computeAxialStrainIncrementElasticPrediction: "
            "no convergence");
      }
      x -= f / df;
      fdfv = fdf(x);
      ++iter;
    }
    return x;
  }  // end of computeAxialStrainIncrementElasticPrediction

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_LOGARITHMICSTRAINCOMPUTEAXIALSTRAININCREMENTELASTICPREDICTION_IXX \
        */
