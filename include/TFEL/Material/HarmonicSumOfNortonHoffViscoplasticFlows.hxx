/*!
 * \file   include/TFEL/Material/HarmonicSumOfNortonHoffViscoplasticFlows.hxx
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

#ifndef LIB_TFEL_MATERIAL_HARMONICSUMOFNORTONHOFFVISCOPLASTICFLOWS_HXX
#define LIB_TFEL_MATERIAL_HARMONICSUMOFNORTONHOFFVISCOPLASTICFLOWS_HXX

#include <tuple>
#include "TFEL/Math/General/ComputeBinaryResult.hxx"
#include "TFEL/Math/fsarray.hxx"

namespace tfel::material {

  /*!
   * \return the equivalent strain rate associated given by the harmonic sum
   * of \f$N\f$ Norton-Hoff viscoplastic flows.
   * \tparam N: number of Norton-Hoff viscoplastic flows
   * \tparam strainrate: type of the strain rates.
   * \tparam stress: type of the stresses.
   * \tparam real: type of the Norton exponents.
   * \param[in] A: array of Norton coefficients.
   * \param[in] K: array of Norton normalisation factors.
   * \param[in] n: array of Norton exponents.
   * \param[in] seq: effective equivalent stress.
   */
  template <unsigned short N,
            typename strainrate,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE strainrate
  computeEquivalentStrainRateOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const tfel::math::fsarray<N, strainrate>&,
      const tfel::math::fsarray<N, stress>&,
      const tfel::math::fsarray<N, real>&,
      const stress);
  /*!
   * \return the equivalent strain rate associated given by the harmonic sum
   * of \f$N\f$ Norton-Hoff viscoplastic flows and its derivative with respect
   * to the effective equivalent stress.
   * \tparam N: number of Norton-Hoff viscoplastic flows
   * \tparam strainrate: type of the strain rates.
   * \tparam stress: type of the stresses.
   * \tparam real: type of the Norton exponents.
   * \param[in] A: array of Norton coefficients.
   * \param[in] K: array of Norton normalisation factors.
   * \param[in] n: array of Norton exponents.
   * \param[in] seq: effective equivalent stress.
   */
  template <unsigned short N,
            typename strainrate,
            typename stress,
            typename real>
  TFEL_HOST_DEVICE std::tuple<
      strainrate,
      tfel::math::BinaryOperationResult<strainrate, stress, tfel::math::OpDiv>>
  computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const tfel::math::fsarray<N, strainrate>&,
      const tfel::math::fsarray<N, stress>&,
      const tfel::math::fsarray<N, real>&,
      const stress);
  /*!
   * \return the equivalent strain rate associated with a Norton-Hoff
   * viscoplastic flow.
   * \tparam strainrate: type of the strain rates.
   * \tparam stress: type of the stresses.
   * \tparam real: type of the Norton exponents.
   * \param[in] A: Norton coefficient.
   * \param[in] K: Norton normalisation factor.
   * \param[in] n: Norton exponent.
   * \param[in] seq: effective equivalent stress.
   */
  template <typename strainrate, typename stress, typename real>
  TFEL_HOST_DEVICE strainrate
  computeEquivalentStrainRateOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const strainrate, const stress, const real, const stress);
  /*!
   * \return the equivalent strain rate of a Norton-Hoff viscoplastic flow and
   * its derivative with respect to the effective equivalent stress.
   * \tparam strainrate: type of the strain rates.
   * \tparam stress: type of the stresses.
   * \tparam real: type of the Norton exponents.
   * \param[in] A: Norton coefficient.
   * \param[in] K: Norton normalisation factor.
   * \param[in] n: Norton exponent.
   * \param[in] seq: effective equivalent stress.
   */
  template <typename strainrate, typename stress, typename real>
  TFEL_HOST_DEVICE std::tuple<
      strainrate,
      tfel::math::BinaryOperationResult<strainrate, stress, tfel::math::OpDiv>>
  computeEquivalentStrainRateAndDerivativeOfHarmonicSumOfNortonHoffViscoplasticFlows(
      const strainrate, const stress, const real, const stress);

}  // end of namespace tfel::material

#include "TFEL/Material/HarmonicSumOfNortonHoffViscoplasticFlows.ixx"

#endif /* LIB_TFEL_MATERIAL_HARMONICSUMOFNORTONHOFFVISCOPLASTICFLOWS_HXX */
