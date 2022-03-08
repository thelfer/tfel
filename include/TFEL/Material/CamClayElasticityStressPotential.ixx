/*!
 * \file   include/TFEL/Material/CamClayElasticityStressPotential.ixx
 * \brief
 * \author Thomas Helfer
 * \date   07/03/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CAMCLAYELASTICITYSTRESSPOTENTIAL_IXX
#define LIB_TFEL_MATERIAL_CAMCLAYELASTICITYSTRESSPOTENTIAL_IXX

namespace tfel::material {

  template <typename StrainStensorType, typename stress, typename real>
  std::tuple<
      tfel::math::stensor<tfel::math::getSpaceDimension<StrainStensorType>(),
                          stress>,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StrainStensorType>(),
                           stress>>
  computeCamClayElasticity(const StrainStensorType& eel,
                           const real e0,
                           const stress kappa,
                           const real nu,
                           const stress p_min) {
    constexpr auto N = tfel::math::getSpaceDimension<StrainStensorType>();
    using Stensor4 = tfel::math::st2tost2<N, real>;
    constexpr auto id = tfel::math::stensor<N, real>::Id();
    const auto k0 = (1 + e0) / kappa;
    const auto e = tfel::math::trace(eel);
    const auto s = tfel::math::deviator(eel);
    const auto r = 3 * (1 - 2 * nu) / (2 * (1 + nu));
    if (k0 * e >= -1) {
      const auto K = k0 * p_min;
      const auto G = K * r;
      const auto p = -K * e;
      return {2 * G * s - p * id,  //
              2 * G * Stensor4::K() + K * Stensor4::IxI()};
    }
    const auto p = p_min * exp(-1 - k0 * e);
    const auto dp_de = -k0 * p;
    const auto K = k0 * p;
    const auto dK_de = k0 * dp_de;
    const auto G = r * K;
    const auto dG_de = r * dK_de;
    return {2 * G * s - p * id,                           //
            2 * G * Stensor4::K() + 2 * dG_de * (s ^ id)  //
                - dp_de * Stensor4::IxI()};
  }  // end of computeCamClayElasticity

  template <typename StressStensorType,
            typename StrainStensorType,
            typename StrainStensorType2,
            typename stress,
            typename real>
  std::tuple<
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensorType>(),
                          stress>,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StressStensorType>(),
                           stress>>
  computeCamClayElasticityII(const StressStensorType& sig0,
                             const StrainStensorType& eel,
                             const StrainStensorType2& deel,
                             const real e0,
                             const stress kappa,
                             const real nu,
                             const stress p_min) {
    constexpr auto N = tfel::math::getSpaceDimension<StressStensorType>();
    using Stensor4 = tfel::math::st2tost2<N, real>;
    constexpr auto id = tfel::math::stensor<N, real>::Id();
    const auto k0 = (1 + e0) / kappa;
    const auto e = tfel::math::trace(eel + deel);
    const auto ds = tfel::math::deviator(deel);
    const auto r = 3 * (1 - 2 * nu) / (2 * (1 + nu));
    if (k0 * e >= -1) {
      const auto K = k0 * p_min;
      const auto G = K * r;
      const auto p = -K * e;
      return {tfel::math::deviator(sig0) + 2 * G * ds - p * id,  //
              2 * G * Stensor4::K() + K * Stensor4::IxI()};
    }
    const auto p = p_min * exp(-1 - k0 * e);
    const auto dp_de = -k0 * p;
    const auto K = k0 * p;
    const auto dK_de = k0 * dp_de;
    const auto G = r * K;
    const auto dG_de = r * dK_de;
    return {tfel::math::deviator(sig0) + 2 * G * ds - p * id,  //
            2 * G * Stensor4::K() + 2 * dG_de * (ds ^ id)      //
                - dp_de * Stensor4::IxI()};
  }  // end of computeCamClayElasticityII

  template <typename StrainStensorType, typename stress, typename real>
  std::tuple<
      tfel::math::stensor<tfel::math::getSpaceDimension<StrainStensorType>(),
                          stress>,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StrainStensorType>(),
                           stress>>
  computeCamClayElasticityIII(const StrainStensorType& eel,
                              const real e0,
                              const stress kappa,
                              const stress mu,
                              const stress p_min) {
    constexpr auto N = tfel::math::getSpaceDimension<StrainStensorType>();
    using Stensor4 = tfel::math::st2tost2<N, real>;
    constexpr auto id = tfel::math::stensor<N, real>::Id();
    const auto k0 = (1 + e0) / kappa;
    const auto e = tfel::math::trace(eel);
    const auto s = tfel::math::deviator(eel);
    if (k0 * e >= -1) {
      const auto K = k0 * p_min;
      const auto p = -K * e;
      return {2 * mu * s - p * id,  //
              2 * mu * Stensor4::K() + K * Stensor4::IxI()};
    }
    const auto p = p_min * exp(-1 - k0 * e);
    const auto dp_de = -k0 * p;
    return {2 * mu * s - p * id,  //
            2 * mu * Stensor4::K() - dp_de * Stensor4::IxI()};
  }  // end of computeCamClayElasticityIII

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_CAMCLAYELASTICITYSTRESSPOTENTIAL_IXX */
