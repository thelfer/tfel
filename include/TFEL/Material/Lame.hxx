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

#ifndef LIB_TFEL_LAME_HXX
#define LIB_TFEL_LAME_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace tfel {

  namespace material {

    namespace lame {}

    /*
     * \brief compute the first Lame's coefficient
     */
    template <typename T>
    constexpr T computeLambda(const T young, const T nu) {
      return nu * young / ((1 + nu) * (1 - 2 * nu));
    }

    /*
     * \brief compute the first Lame's coefficient
     */
    template <typename T>
    constexpr tfel::math::qt<tfel::math::Stress, T> computeLambda(
        const tfel::math::qt<tfel::math::Stress, T> young,
        const tfel::math::qt<tfel::math::NoUnit, T> nu) {
      return nu * young / ((1 + nu) * (1 - 2 * nu));
    }

    /*
     * \brief compute the second Lame's coefficient
     */
    template <typename T>
    constexpr T computeMu(const T young, const T nu) {
      return young / (2 * (1 + nu));
    }

    /*
     * \brief compute the second Lame's coefficient
     */
    template <typename T>
    constexpr tfel::math::qt<tfel::math::Stress, T> computeMu(
        const tfel::math::qt<tfel::math::Stress, T> young,
        const tfel::math::qt<tfel::math::NoUnit, T> nu) {
      return young / (2 * (1 + nu));
    }

    /*!
     * \class computeElasticStiffness
     * \brief compute the elastic stiffness given Lame's coefficients
     * \param N : space dimension
     * \param T : numerical type
     * \author Thomas Helfer
     * \date   12 Sep 2006
     */
    template <unsigned short, typename>
    struct computeElasticStiffness;

    /*
     * Partial specialisation in 1D
     */
    template <typename T>
    struct computeElasticStiffness<1u, T> {
      static void exe(
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
      static void exe(
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
    struct computeElasticStiffness<2u, T> {
      static void exe(
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

      static void exe(
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
    struct computeElasticStiffness<3u, T> {
      static void exe(
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

      static void exe(
          typename tfel::config::Types<3u, T, false>::StiffnessTensor& D,
          const typename tfel::config::Types<3u, T, false>::stress lambda,
          const typename tfel::config::Types<3u, T, false>::stress mu) {
        typedef typename tfel::config::Types<3u, T, false>::stress stress;
        const stress G = 2 * mu;
        const stress tmp = lambda + G;
        constexpr const auto zero = stress(0);
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
     * \class computeUnalteredElasticStiffness
     * \brief compute the elastic stiffness given Lame's coefficients
     * \param N : space dimension
     * \param T : numerical type
     * \author Thomas Helfer
     * \date   12 Sep 2006
     */
    template <unsigned short N, typename T>
    struct computeUnalteredElasticStiffness {
      static void exe(
          typename tfel::config::Types<N, T, true>::StiffnessTensor& D,
          const typename tfel::config::Types<N, T, true>::stress lambda,
          const typename tfel::config::Types<N, T, true>::stress mu) {
        computeElasticStiffness<N, T>::exe(D, lambda, mu);
      }
      static void exe(
          typename tfel::config::Types<N, T, false>::StiffnessTensor& D,
          const typename tfel::config::Types<N, T, false>::stress lambda,
          const typename tfel::config::Types<N, T, false>::stress mu) {
        computeElasticStiffness<N, T>::exe(D, lambda, mu);
      }
    };

    /*!
     * \class computeUnalteredElasticStiffness
     * \brief compute the elastic stiffness given Lame's coefficients
     * \param H : modelling hypothesis
     * \param T : numerical type
     * \author Thomas Helfer
     * \date   12 Sep 2006
     */
    template <ModellingHypothesis::Hypothesis H, typename T>
    struct computeAlteredElasticStiffness {
      static constexpr unsigned short N =
          ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(
          typename tfel::config::Types<N, T, true>::StiffnessTensor& D,
          const typename tfel::config::Types<N, T, true>::stress lambda,
          const typename tfel::config::Types<N, T, true>::stress mu) {
        computeElasticStiffness<N, T>::exe(D, lambda, mu);
      }
      static void exe(
          typename tfel::config::Types<N, T, false>::StiffnessTensor& D,
          const typename tfel::config::Types<N, T, false>::stress lambda,
          const typename tfel::config::Types<N, T, false>::stress mu) {
        computeElasticStiffness<N, T>::exe(D, lambda, mu);
      }
    };  // end of struct computeAlteredElasticStiffness

    template <typename T>
    struct computeAlteredElasticStiffness<
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
        T> {
      static void exe(
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
      static void exe(
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
    };  // end of struct computeAlteredElasticStiffness

    template <typename T>
    struct computeAlteredElasticStiffness<ModellingHypothesis::PLANESTRESS, T> {
      static void exe(
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
        D(0, 2) = D(0, 3) = D(1, 2) = D(1, 3) = D(2, 0) = D(2, 1) =
            stress(T(0));
        D(2, 2) = D(2, 3) = D(3, 0) = D(3, 1) = D(3, 2) = stress(T(0));
      }
      static void exe(
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
        D(0, 2) = D(0, 3) = D(1, 2) = D(1, 3) = D(2, 0) = D(2, 1) =
            stress(T(0));
        D(2, 2) = D(2, 3) = D(3, 0) = D(3, 1) = D(3, 2) = stress(T(0));
      }
    };  // end of struct computeAlteredElasticStiffness

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_LAME_HXX */
