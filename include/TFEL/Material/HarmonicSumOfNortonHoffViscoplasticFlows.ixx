/*!
 * \file   include/TFEL/Material/HarmonicSumOfNortonHoffViscoplasticFlows.ixx
 * \brief
 * \author Thomas Helfer
 * \date   01/11/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_HARMONICSUMOFNORTONHOFFVISCOPLASTICFLOWS_IXX
#define LIB_TFEL_MATERIAL_HARMONICSUMOFNORTONHOFFVISCOPLASTICFLOWS_IXX

#include "TFEL/Math/fsarray.hxx"

namespace tfel::material {

  template <typename strainrate, typename stress, typename real>
  strainrate
  computeEquivalentStrainRateOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const strainrate A, const stress K, const real n, const stress seq) {
    if (!(seq > stress(0))) {
      return strainrate(0);
    }
    return A * pow(seq / K, n);
  }  // end of
     // computeEquivalentStrainRateOfHarmonicSumOfNortonHoffViscoplasticFlows

  template <typename strainrate, typename stress, typename real>
  std::tuple<
      strainrate,
      tfel::math::BinaryOperationResult<strainrate, stress, tfel::math::OpDiv>>
  computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const strainrate A, const stress K, const real n, const stress seq) {
    using dtype = tfel::math::BinaryOperationResult<strainrate, stress,
                                                    tfel::math::OpDiv>;
    using result = std::tuple<strainrate, dtype>;
    if (!(seq > stress(0))) {
      return result{strainrate(0), dtype(0)};
    }
    auto dp = A * pow(seq / K, n);
    auto ddp_dseq = n * dp / seq;
    return result{dp, ddp_dseq};
  }  // end of
     // computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows

  template <unsigned short N,
            typename strainrate,
            typename stress,
            typename real>
  strainrate
  computeEquivalentStrainRateOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const tfel::math::fsarray<N, strainrate>& A,
      const tfel::math::fsarray<N, stress>& K,
      const tfel::math::fsarray<N, real>& n,
      const stress seq) {
    if (!(seq > stress(0))) {
      return strainrate(0);
    }
    auto idp = 1 / (A[0] * pow(seq / K[0], n[0]));
    for (decltype(A.size()) i = 1; i != A.size(); ++i) {
      idp += 1 / (A[i] * pow(seq / K[i], n[i]));
    }
    return 1 / idp;
  }  // end of
     // computeEquivalentStrainRateOfHarmonicSumOfNortonHoffViscoplasticFlows

  template <unsigned short N,
            typename strainrate,
            typename stress,
            typename real>
  std::tuple<
      strainrate,
      tfel::math::BinaryOperationResult<strainrate, stress, tfel::math::OpDiv>>
  computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const tfel::math::fsarray<N, strainrate>& A,
      const tfel::math::fsarray<N, stress>& K,
      const tfel::math::fsarray<N, real>& n,
      const stress seq) {
    using dtype = tfel::math::BinaryOperationResult<strainrate, stress,
                                                    tfel::math::OpDiv>;
    using result = std::tuple<strainrate, dtype>;
    auto r = result{strainrate(0), dtype(0)};
    if (!(seq > stress(0))) {
      return r;
    }
    auto dp = tfel::math::fsarray<N, strainrate>{};
    auto idp = 1 / (dp[0] = A[0] * pow(seq / K[0], n[0]));
    for (decltype(A.size()) i = 1; i != A.size(); ++i) {
      dp[i] = A[i] * pow(seq / K[i], n[i]);
      idp += 1 / dp[i];
    }
    const auto vp = 1 / idp;
    if (!(vp > strainrate(0))) {
      return r;
    }
    // the derivative is computed from the ratios vp / dp[i], which lie in
    // [0, 1], to avoid an underflow of the squares of the strain rates
    auto s = n[0] * (vp / dp[0]);
    for (decltype(A.size()) i = 1; i != A.size(); ++i) {
      s += n[i] * (vp / dp[i]);
    }
    std::get<0>(r) = vp;
    std::get<1>(r) = s * vp / seq;
    return r;
  }  // end of
     // computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows

}  // end of namespace tfel::material

#include "TFEL/Material/HarmonicSumOfNortonHoffViscoplasticFlows.ixx"

#endif /* LIB_TFEL_MATERIAL_HARMONICSUMOFNORTONHOFFVISCOPLASTICFLOWS_IXX */
