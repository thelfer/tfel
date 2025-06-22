/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  /*!
   * \brief apply Powell' dog leg algorithm
   * \tparam N: size of the system of non linear equations
   * \tparam NumericType: numeric type
   * \param[in,out] delta_zeros: current correction to the solution
   * \param[in] jacobian: current estimate of the jacobian matrix
   * \param[in] fzeros: current residual
   * \param[in] radius: trust region size
   */
  template <unsigned short N, typename NumericType>
  TFEL_HOST_DEVICE void applyPowellDogLegAlgorithm(
      tvector<N, NumericType>& delta_zeros,
      const tmatrix<N, N, NumericType>& jacobian,
      const tvector<N, NumericType>& fzeros,
      const NumericType radius) noexcept {
    if (tfel::math::abs(delta_zeros) < N * radius) {
      return;
    }
    tvector<N, NumericType> g;
    tvector<N, NumericType> g2;
    for (unsigned short idx = 0; idx != N; ++idx) {
      g[idx] = NumericType(0);
      for (unsigned short idx2 = 0; idx2 != N; ++idx2) {
        g[idx] += jacobian(idx2, idx) * fzeros(idx2);
      }
    }
    for (unsigned short idx = 0; idx != N; ++idx) {
      g2[idx] = NumericType(0);
      for (unsigned short idx2 = 0; idx2 != N; ++idx2) {
        g2[idx] += jacobian(idx, idx2) * g(idx2);
      }
    }
    const auto cste = (g | g) / (g2 | g2);
    g *= cste;
    if (tfel::math::abs(g) < N * radius) {
      const auto c0 = radius * radius;
      const auto c1 = (g | g);
      const auto c2 = -delta_zeros | g;
      const auto c3 = delta_zeros | delta_zeros;
      const auto c4 = (c2 - c0) * (c2 - c0) + (c3 - c0) * (c0 - c1);
      const auto alpha =
          (c0 - c1) / (c2 - c1 + std::sqrt(std::max(c4, NumericType(0))));
      g = -alpha * delta_zeros + (1 - alpha) * g;
    } else {
      const auto alpha = radius / norm(g);
      g *= alpha;
    }
    delta_zeros = -g;
  }  // end of applyPowellDogLegAlgorithm

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX */
