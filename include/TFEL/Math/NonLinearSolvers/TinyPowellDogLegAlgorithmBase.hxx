/*!
 * \file   include/TFEL/Math/NonLinearSolvers/TinyPowellDogLegAlgorithmBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date 06/07/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX
#define LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX

#include <cmath>
#include <algorithm>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  /*!
   * \brief apply Powell' dog leg algorithm: the correction is unchanged if it
   * lies in the trust region. Otherwise, it is replaced by the intersection of
   * the dog leg path and the boundary of the trust region.
   * \tparam N: size of the system of non linear equations
   * \tparam NumericType: numeric type
   * \param[in,out] delta_zeros: current correction to the solution
   * \param[in] jacobian: current estimate of the jacobian matrix
   * \param[in] fzeros: current residual
   * \param[in] radius: trust region radius
   */
  template <unsigned short N, typename NumericType>
  TFEL_HOST_DEVICE void applyPowellDogLegAlgorithm(
      tvector<N, NumericType>& delta_zeros,
      const tmatrix<N, N, NumericType>& jacobian,
      const tvector<N, NumericType>& fzeros,
      const NumericType radius) noexcept {
    if (norm(delta_zeros) <= radius) {
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
    if (norm(g) < radius) {
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

  /*!
   * \brief update the trust region radius from the ratio of the actual and
   * predicted reductions of the squared norm of the residual, see Algorithm 4.1
   * of Nocedal and Wright, Numerical Optimization.
   *
   * The upper bound of the radius of this algorithm is not used, as in the
   * `hybrd` routine of MINPACK: the radius only grows when the correction
   * reaches about the boundary of the trust region, and an upper bound would
   * limit the progress per iteration, whatever the magnitude of the unknowns.
   *
   * \return true if the correction is accepted, false if it must be rejected
   * \tparam N: size of the system of non linear equations
   * \tparam NumericType: numeric type
   * \param[in,out] radius: trust region radius
   * \param[in] delta_zeros: last correction
   * \param[in] jacobian: jacobian matrix used to compute the last correction
   * \param[in] fzeros_1: residual before the last correction
   * \param[in] fzeros: residual after the last correction
   */
  template <unsigned short N, typename NumericType>
  TFEL_HOST_DEVICE bool updatePowellDogLegTrustRegionRadius(
      NumericType& radius,
      const tvector<N, NumericType>& delta_zeros,
      const tmatrix<N, N, NumericType>& jacobian,
      const tvector<N, NumericType>& fzeros_1,
      const tvector<N, NumericType>& fzeros) noexcept {
    // the correction is rejected below this ratio
    constexpr auto acceptance_ratio = NumericType(1) / 10000;
    // the radius is reduced below this ratio
    constexpr auto reduction_ratio = NumericType(1) / 4;
    // the radius may be enlarged above this ratio
    constexpr auto enlargement_ratio = NumericType(3) / 4;
    constexpr auto reduction_factor = NumericType(1) / 4;
    constexpr auto enlargement_factor = NumericType(2);
    const auto n2 = fzeros_1 | fzeros_1;
    const tvector<N, NumericType> linearised_fzeros =
        fzeros_1 + jacobian * delta_zeros;
    const auto actual_reduction = n2 - (fzeros | fzeros);
    const auto predicted_reduction =
        n2 - (linearised_fzeros | linearised_fzeros);
    const auto rho = (predicted_reduction > NumericType(0))
                         ? actual_reduction / predicted_reduction
                         : NumericType(0);
    const auto step = norm(delta_zeros);
    if (rho < reduction_ratio) {
      // the correction may be much smaller than the radius: reducing the radius
      // alone could give the same correction again
      if ((step > NumericType(0)) && (step < radius)) {
        radius = step;
      }
      radius *= reduction_factor;
    } else if ((rho > enlargement_ratio) &&
               (enlargement_factor * step > radius)) {
      radius = enlargement_factor * step;
    }
    return rho >= acceptance_ratio;
  }  // end of updatePowellDogLegTrustRegionRadius

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYPOWELLDOGLEGALGORITHMBASE_HXX */
