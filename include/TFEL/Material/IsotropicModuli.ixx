/*!
 * \file   include/TFEL/Material/IsotropicModuli.ixx
 * \brief  This file defines various functions dealing with elastic
 * coefficients. \author Antoine MARTIN \date   15 May 2025 \copyright Copyright
 * (C) 2006-2025 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence with linking exception or the
 * CECILL-A licence. A copy of thoses licences are delivered with the sources of
 * TFEL. CEA or EDF may also distribute this project under specific licensing
 * conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICMODULI_IXX
#define LIB_TFEL_MATERIAL_ISOTROPICMODULI_IXX


namespace tfel::material {
 
  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>())
  TFEL_HOST_DEVICE constexpr tfel::math::st2tost2<3u,StressType> computeIsotropicStiffnessTensor(const IsotropicModuli<StressType> &IM) {
    const auto KG=IM.ToKG();
    const auto kappa = KG.kappa;
    const auto mu = KG.mu;
    constexpr auto J = tfel::math::st2tost2<3u, tfel::math::base_type<StressType>>::J();
    constexpr auto K = tfel::math::st2tost2<3u, tfel::math::base_type<StressType>>::K();
    return 3 * kappa * J + 2 * mu * K;
  }  // end of computeIsotropicStiffnessTensor
  
 
  template <tfel::math::ScalarConcept T>
  TFEL_HOST_DEVICE constexpr std::pair<T, T> computeKappaMu(const tfel::math::st2tost2<3u, T> &A) {
    const auto siz = tfel::math::StensorDimeToSize<3u>::value;
    constexpr auto J = tfel::math::st2tost2<3u, tfel::math::base_type<T>>::J();
    constexpr auto K = tfel::math::st2tost2<3u, tfel::math::base_type<T>>::K();
    const T kappai = tfel::math::quaddot(A, J) / 3;
    const T mui = tfel::math::quaddot(A, K) / (siz - 1) / 2;
    return {kappai, mui};
  }  // end of computeKappaMu
  
  
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE constexpr KGModuli<StressType> computeKGModuli(const tfel::math::st2tost2<3u, StressType> &A) {
    const auto pair = computeKappaMu<StressType>(A);
    const auto kappa = std::get<0>(pair);
    const auto mu = std::get<1>(pair);
    return KGModuli<StressType>(kappa,mu);
  }  // end of computeKGModuli

  
  template <unsigned short int N, tfel::math::ScalarConcept T>
  TFEL_HOST_DEVICE constexpr tfel::math::base_type<T> relative_error(const tfel::math::st2tost2<N, T> &C1,
                      const tfel::math::st2tost2<N, T> &C2) {
    const auto val = tfel::math::norm(C1 - C2) / tfel::math::norm(C2);
    return val;
  }  // end of relative_error

  
  template <tfel::math::ScalarConcept T>
  TFEL_HOST_DEVICE constexpr bool isIsotropic(const tfel::math::st2tost2<3u, T> &Ai, const tfel::math::base_type<T> eps) {
    const auto pair = computeKappaMu<T>(Ai);
    const auto kappai = std::get<0>(pair);
    const auto mui = std::get<1>(pair);
    constexpr auto J = tfel::math::st2tost2<3u, tfel::math::base_type<T>>::J();
    constexpr auto K = tfel::math::st2tost2<3u, tfel::math::base_type<T>>::K();
    const auto A_comp = 3 * kappai * J + 2 * mui * K;
    const auto val = relative_error<3u, T>(Ai, A_comp);
    return val < eps;
  }  // end of isIsotropic

}  // end of namespace tfel::material

#endif /* LIB_TFEL_ISOTROPICMODULI_IXX */
