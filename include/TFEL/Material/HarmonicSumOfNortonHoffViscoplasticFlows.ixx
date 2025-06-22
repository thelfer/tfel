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

#include "TFEL/Math/tvector.hxx"

namespace tfel {

  namespace material {

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
    std::tuple<strainrate,
               tfel::math::
                   BinaryOperationResult<strainrate, stress, tfel::math::OpDiv>>
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
        const tfel::math::tvector<N, strainrate>& A,
        const tfel::math::tvector<N, stress>& K,
        const tfel::math::tvector<N, real>& n,
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
    std::tuple<strainrate,
               tfel::math::
                   BinaryOperationResult<strainrate, stress, tfel::math::OpDiv>>
    computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows(
        const tfel::math::tvector<N, strainrate>& A,
        const tfel::math::tvector<N, stress>& K,
        const tfel::math::tvector<N, real>& n,
        const stress seq) {
      using dtype = tfel::math::BinaryOperationResult<strainrate, stress,
                                                      tfel::math::OpDiv>;
      using result = std::tuple<strainrate, dtype>;
      auto r = result{strainrate(0), dtype(0)};
      if (!(seq > stress(0))) {
        return r;
      }
      auto dp = A[0] * pow(seq / K[0], n[0]);
      auto ddp_dseq = n[0] * dp / seq;
      auto idp = 1 / dp;
      auto didp_dseq = -idp * idp * ddp_dseq;
      for (decltype(A.size()) i = 1; i != A.size(); ++i) {
        dp = A[i] * pow(seq / K[i], n[i]);
        ddp_dseq = n[i] * dp / seq;
        idp += 1 / dp;
        didp_dseq += -ddp_dseq / (dp * dp);
      }
      std::get<0>(r) = 1 / idp;
      std::get<1>(r) = -1 / (idp * idp) * didp_dseq;
      return r;
    }  // end of
       // computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/HarmonicSumOfNortonHoffViscoplasticFlows.ixx"

#endif /* LIB_TFEL_MATERIAL_HARMONICSUMOFNORTONHOFFVISCOPLASTICFLOWS_IXX */
