/*!
 * \file   include/TFEL/Material/LinearHomogenizationBounds.ixx
 * \author Antoine Martin
 * \date   23 January 2025
 * \brief  This file defines some well-known homogenization bounds.
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_IXX
#define LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_IXX

#include <cmath>
#include "TFEL/FSAlgorithm/max_element.hxx"

namespace tfel::material::homogenization::elasticity {

  template <unsigned short int d,
            unsigned int N,
            tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<d, StressType> computeVoigtStiffness(
          const std::array<types::real<StressType>, N> &tab_f,
          const std::array<tfel::math::st2tost2<d, StressType>, N> &tab_C) {
    tfel::math::st2tost2<d, StressType> C_V = tab_f[0] * tab_C[0];
    for (int i = 1; i < N; i++) {
      C_V += tab_f[i] * tab_C[i];
    }
    return C_V;
  }

  template <unsigned short int d,
            unsigned int N,
            tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<d, StressType> computeReussStiffness(
          const std::array<types::real<StressType>, N> &tab_f,
          const std::array<tfel::math::st2tost2<d, StressType>, N> &tab_C) {
    tfel::math::st2tost2<d, types::compliance<StressType>> S_R =
        tab_f[0] * invert(tab_C[0]);
    for (int i = 1; i < N; i++) {
      S_R += tab_f[i] * invert(tab_C[i]);
    }
    return invert(S_R);
  }

  template <unsigned short int d,
            unsigned int N,
            tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const std::pair<
          std::pair<StressType, StressType>,
          std::pair<
              StressType,
              StressType>> computeIsotropicHashinShtrikmanBounds(const std::
                                                                     array<
                                                                         types::real<
                                                                             StressType>,
                                                                         N>
                                                                         &tab_f,
                                                                 const std::array<
                                                                     StressType,
                                                                     N> &tab_K,
                                                                 const std::array<
                                                                     StressType,
                                                                     N>
                                                                     &tab_mu) {
    using real = types::real<StressType>;
    std::array<StressType, N> tab_H;
    for (int i = 0; i < N; i++) {
      tab_H[i] =
          tab_mu[i] *
          (real(d) * tab_K[i] / 2 + (d + 1) * (d - 2) * tab_mu[i] / real(d)) /
          (tab_K[i] + 2 * tab_mu[i]);
    }
    using namespace tfel::fsalgo;
    auto mu_max = *max_element<N>::exe(tab_mu.begin());
    auto mu_min = *min_element<N>::exe(tab_mu.begin());
    auto H_max = *max_element<N>::exe(tab_H.begin());
    auto H_min = *min_element<N>::exe(tab_H.begin());
    auto K_star_min = 2 * (d - 1) / real(d) * mu_min;
    auto K_star_max = 2 * (d - 1) / real(d) * mu_max;
    auto mu_star_min = H_min;
    auto mu_star_max = H_max;
    using compliance = types::compliance<StressType>;
    StressType Ke_U;
    StressType mue_U;
    StressType Ke_L;
    StressType mue_L;
    compliance Ce_U = compliance(0);
    compliance Ne_U = compliance(0);
    compliance Ce_L = compliance(0);
    compliance Ne_L = compliance(0);
    for (int i = 0; i < N; i++) {
      Ce_L += tab_f[i] / (K_star_min + tab_K[i]);
      Ne_L += tab_f[i] / (mu_star_min + tab_mu[i]);
      Ce_U += tab_f[i] / (K_star_max + tab_K[i]);
      Ne_U += tab_f[i] / (mu_star_max + tab_mu[i]);
    }
    Ke_L = 1 / Ce_L - K_star_min;
    mue_L = 1 / Ne_L - mu_star_min;
    Ke_U = 1 / Ce_U - K_star_max;
    mue_U = 1 / Ne_U - mu_star_max;

    return {{Ke_L, mue_L}, {Ke_U, mue_U}};
  }

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_IXX */
