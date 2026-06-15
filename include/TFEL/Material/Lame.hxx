/*!
 * \file   include/TFEL/Material/Lame.hxx
 * \brief  This file defines various functions dealing with Lame's coefficients.
 * Using Lame's coefficients, linear elastic behaviour may be written~:
 * \f[ \underline{\sigma} =
 * \lambda*\mathrm{tr}\left(\underline{\epsilon}\right)\underline{I}+2\mu\underline{\epsilon}
 * \f]
 * \author Thomas Helfer
 * \date   31 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LAME_HXX
#define LIB_TFEL_MATERIAL_LAME_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/types.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace tfel::material {

  //! \brief empty namespace for backward compatibility
  namespace lame {}

  /*
   * \brief compute the first Lame's coefficient
   */
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE constexpr StressType computeLambda(
      const StressType& young, const types::real<StressType>& nu) noexcept
      requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                  StressType>()) {
    return nu * young / ((1 + nu) * (1 - 2 * nu));
  }

  /*
   * \brief compute the second Lame's coefficient
   */
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE constexpr StressType computeMu(
      const StressType& young, const types::real<StressType>& nu) noexcept
      requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                  StressType>()) {
    return young / (2 * (1 + nu));
  }  // end of computeMu

  /*!
   * \class ComputeElasticStiffnessBase
   * \brief compute the elastic stiffness given Lame's coefficients
   * \param N : space dimension
   * \param T : numerical type
   * \author Thomas Helfer
   * \date   12 Sep 2006
   */
  template <unsigned short, typename>
  struct ComputeElasticStiffnessBase;

  /*
   * Partial specialisation in 1D
   */
  template <typename T>
  struct ComputeElasticStiffnessBase<1u, T> {
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<1u, T, true>::StiffnessTensor& D,
        const typename tfel::config::Types<1u, T, true>::stress lambda,
        const typename tfel::config::Types<1u, T, true>::stress mu) {
      typedef typename tfel::config::Types<1u, T, true>::stress stress;
      stress G = 2 * mu;
      stress tmp = lambda + G;
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
    }
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<1u, T, false>::StiffnessTensor& D,
        const typename tfel::config::Types<1u, T, false>::stress lambda,
        const typename tfel::config::Types<1u, T, false>::stress mu) {
      typedef typename tfel::config::Types<1u, T, false>::stress stress;
      stress G = 2 * mu;
      stress tmp = lambda + G;
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
    }
  };

  /*
   * Partial specialisation in 2D
   */
  template <typename T>
  struct ComputeElasticStiffnessBase<2u, T> {
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<2u, T, true>::StiffnessTensor& D,
        const typename tfel::config::Types<2u, T, true>::stress lambda,
        const typename tfel::config::Types<2u, T, true>::stress mu) {
      typedef typename tfel::config::Types<2u, T, true>::stress stress;
      const stress G = 2 * mu;
      const stress tmp = lambda + G;
      const stress zero(T(0));
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
      D(0, 3) = D(3, 0) = zero;
      D(1, 3) = D(3, 1) = zero;
      D(2, 3) = D(3, 2) = zero;
      D(3, 3) = G;
    }

    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<2u, T, false>::StiffnessTensor& D,
        const typename tfel::config::Types<2u, T, false>::stress lambda,
        const typename tfel::config::Types<2u, T, false>::stress mu) {
      typedef typename tfel::config::Types<2u, T, false>::stress stress;
      const stress G = 2 * mu;
      const stress tmp = lambda + G;
      const stress zero(T(0));
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
      D(0, 3) = D(3, 0) = zero;
      D(1, 3) = D(3, 1) = zero;
      D(2, 3) = D(3, 2) = zero;
      D(3, 3) = G;
    }
  };

  /*
   * Partial specialisation in 3D
   */
  template <typename T>
  struct ComputeElasticStiffnessBase<3u, T> {
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<3u, T, true>::StiffnessTensor& D,
        const typename tfel::config::Types<3u, T, true>::stress lambda,
        const typename tfel::config::Types<3u, T, true>::stress mu) {
      typedef typename tfel::config::Types<3u, T, true>::stress stress;
      const stress G = 2 * mu;
      const stress tmp = lambda + G;
      const stress zero(T(0));
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
      D(0, 3) = D(3, 0) = D(0, 4) = D(4, 0) = D(0, 5) = D(5, 0) = zero;
      D(1, 3) = D(3, 1) = D(1, 4) = D(4, 1) = D(1, 5) = D(5, 1) = zero;
      D(2, 3) = D(3, 2) = D(2, 4) = D(4, 2) = D(2, 5) = D(5, 2) = zero;
      D(3, 4) = D(4, 3) = D(3, 5) = D(5, 3) = zero;
      D(4, 5) = D(5, 4) = zero;
      D(3, 3) = G;
      D(4, 4) = G;
      D(5, 5) = G;
    }

    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<3u, T, false>::StiffnessTensor& D,
        const typename tfel::config::Types<3u, T, false>::stress lambda,
        const typename tfel::config::Types<3u, T, false>::stress mu) {
      typedef typename tfel::config::Types<3u, T, false>::stress stress;
      const stress G = 2 * mu;
      const stress tmp = lambda + G;
      constexpr auto zero = stress(0);
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
      D(0, 0) = D(1, 1) = D(2, 2) = tmp;
      D(0, 1) = D(0, 2) = D(1, 2) = lambda;
      D(1, 0) = D(2, 0) = D(2, 1) = lambda;
      D(0, 3) = D(3, 0) = D(0, 4) = D(4, 0) = D(0, 5) = D(5, 0) = zero;
      D(1, 3) = D(3, 1) = D(1, 4) = D(4, 1) = D(1, 5) = D(5, 1) = zero;
      D(2, 3) = D(3, 2) = D(2, 4) = D(4, 2) = D(2, 5) = D(5, 2) = zero;
      D(3, 4) = D(4, 3) = D(3, 5) = D(5, 3) = zero;
      D(4, 5) = D(5, 4) = zero;
      D(3, 3) = G;
      D(4, 4) = G;
      D(5, 5) = G;
    }
  };

  /*!
   * \class computeElasticStiffness
   * \brief compute the elastic stiffness given Lame's coefficients
   * \param N : space dimension
   * \param T : numerical type
   * \author Thomas Helfer
   * \date   12 Sep 2006
   */
  template <unsigned short N, typename T>
  struct computeElasticStiffness
      : ComputeElasticStiffnessBase<N, tfel::math::base_type<T>> {
  };  // end of struct computeElasticStiffness

  /*!
   * \class computeUnalteredElasticStiffness
   * \brief compute the elastic stiffness given Lame's coefficients
   * \param N : space dimension
   * \param T : numerical type
   * \author Thomas Helfer
   * \date   12 Sep 2006
   */
  template <unsigned short N, typename T>
  struct computeUnalteredElasticStiffness : computeElasticStiffness<N, T> {
  };  // end of struct computeUnalteredElasticStiffness;

  /*!
   * \class ComputeAlteredElasticStiffnessBase
   * \brief compute the elastic stiffness given Lame's coefficients
   * \param H : modelling hypothesis
   * \param T : numerical type
   * \author Thomas Helfer
   * \date   12 Sep 2006
   */
  template <ModellingHypothesis::Hypothesis H, typename T>
  struct ComputeAlteredElasticStiffnessBase
      : computeElasticStiffness<ModellingHypothesisToSpaceDimension<H>::value,
                                T> {
  };  // end of struct ComputeAlteredElasticStiffnessBase

  template <typename T>
  struct ComputeAlteredElasticStiffnessBase<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
      T> {
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<1u, T, true>::StiffnessTensor& D,
        const typename tfel::config::Types<1u, T, true>::stress lambda,
        const typename tfel::config::Types<1u, T, true>::stress mu) {
      typedef typename tfel::config::Types<1u, T, true>::stress stress;
      const stress D1 = 4 * mu * (lambda + mu) / (lambda + 2 * mu);
      const stress D2 = 2 * mu * lambda / (lambda + 2 * mu);
      D(0, 0) = D1;
      D(0, 1) = D2;
      D(1, 0) = D2;
      D(1, 1) = D1;
      D(2, 2) = D(0, 2) = D(1, 2) = D(2, 0) = D(2, 1) = stress(T(0));
    }
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<1u, T, false>::StiffnessTensor& D,
        const typename tfel::config::Types<1u, T, false>::stress lambda,
        const typename tfel::config::Types<1u, T, false>::stress mu) {
      typedef typename tfel::config::Types<1u, T, false>::stress stress;
      const stress D1 = 4 * mu * (lambda + mu) / (lambda + 2 * mu);
      const stress D2 = 2 * mu * lambda / (lambda + 2 * mu);
      D(0, 0) = D1;
      D(0, 1) = D2;
      D(1, 0) = D2;
      D(1, 1) = D1;
      D(2, 2) = D(0, 2) = D(1, 2) = D(2, 0) = D(2, 1) = stress(T(0));
    }
  };  // end of struct ComputeAlteredElasticStiffnessBase

  template <typename T>
  struct ComputeAlteredElasticStiffnessBase<ModellingHypothesis::PLANESTRESS,
                                            T> {
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<2u, T, true>::StiffnessTensor& D,
        const typename tfel::config::Types<2u, T, true>::stress lambda,
        const typename tfel::config::Types<2u, T, true>::stress mu) {
      typedef typename tfel::config::Types<2u, T, true>::stress stress;
      const stress D1 = 4 * mu * (lambda + mu) / (lambda + 2 * mu);
      const stress D2 = 2 * mu * lambda / (lambda + 2 * mu);
      D(0, 0) = D1;
      D(0, 1) = D2;
      D(1, 0) = D2;
      D(1, 1) = D1;
      D(3, 3) = 2 * mu;
      D(0, 2) = D(0, 3) = D(1, 2) = D(1, 3) = D(2, 0) = D(2, 1) = stress(T(0));
      D(2, 2) = D(2, 3) = D(3, 0) = D(3, 1) = D(3, 2) = stress(T(0));
    }
    TFEL_HOST_DEVICE static constexpr void exe(
        typename tfel::config::Types<2u, T, false>::StiffnessTensor& D,
        const typename tfel::config::Types<2u, T, false>::stress lambda,
        const typename tfel::config::Types<2u, T, false>::stress mu) {
      typedef typename tfel::config::Types<2u, T, false>::stress stress;
      const stress D1 = 4 * mu * (lambda + mu) / (lambda + 2 * mu);
      const stress D2 = 2 * mu * lambda / (lambda + 2 * mu);
      D(0, 0) = D1;
      D(0, 1) = D2;
      D(1, 0) = D2;
      D(1, 1) = D1;
      D(3, 3) = 2 * mu;
      D(0, 2) = D(0, 3) = D(1, 2) = D(1, 3) = D(2, 0) = D(2, 1) = stress(T(0));
      D(2, 2) = D(2, 3) = D(3, 0) = D(3, 1) = D(3, 2) = stress(T(0));
    }
  };  // end of struct ComputeAlteredElasticStiffnessBase

  /*!
   * \class computeUnalteredElasticStiffness
   * \brief compute the elastic stiffness given Lame's coefficients
   * \param H : modelling hypothesis
   * \param T : numerical type
   * \author Thomas Helfer
   * \date   12 Sep 2006
   */
  template <ModellingHypothesis::Hypothesis H, typename T>
  struct computeAlteredElasticStiffness
      : ComputeAlteredElasticStiffnessBase<H, tfel::math::base_type<T>> {
  };  // end of struct computeAlteredElasticStiffness

}  // end of namespace tfel::material

#endif /* LIB_TFEL_LAME_HXX */
