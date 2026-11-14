/*!
 * \file   include/TFEL/Material/GursonTvergaardNeedleman.hxx
 * \brief
 * \author Jeremy Hure
 * \brief  04 Septembre 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_HXX
#define LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_HXX

#include <cmath>
#include <limits>
#include <algorithm>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::material {

  /*!
   * \brief This function computes the implicit flow stress
   * \f$\sigma^{\star}\f$ of the Gurson-Tvergaard-Needleman model.
   *
   * \f$\sigma^{\star}\f$ is implicitely defined by the following
   * relation:
   * \f[
   * \left(\frac{\sigma_{\mathrm{eq}}}{\sigma^{\star}}\right)^{2}+
   * 2\,q_{1}\,f^{\star}\,\cosh\left(\frac{q_{2}\,\mathrm{tr}\left(\underline{\sigma}\right)}{2\,\sigma^{\star}}\right)
   * -1-\left(q_{1}\,f^{\star}\right)^{2}=0
   * \f]
   * \return the value of \f$\sigma^{\star}\f$
   * \param[in] seq : Von Mises stress
   * \param[in] skk : stress trace
   * \param[in] fst : void volume fraction
   * \param[in] q1  : \f$q_1\f$ parameter
   * \param[in] q2  : \f$q_2\f$ parameter
   */
  template <typename T>
  TFEL_HOST_DEVICE T computeFlowStressGTN(
      const T seq, const T skk, const T fst, const T q1, const T q2) {
    // Here we use a Newton-Raphson algorithm to find the value of
    // 1/sigma*, as in computeGursonTvergaardNeedleman1982Stress
    const auto q1f = q1 * fst;
    const auto c = 1 + q1f * q1f;
    // special case: the von Mises stress vanishes
    if (seq <= std::numeric_limits<T>::epsilon() * std::abs(skk)) {
      return (q1f > 0) ? q2 * std::abs(skk) / (2 * std::acosh(c / (2 * q1f)))
                       : seq;
    }
    // second order approximation (development of the hyperbolic cosine to
    // the second order), bounded by the solution for a vanishing von Mises
    // stress to avoid an overflow of the hyperbolic cosine
    auto x = std::sqrt(4 * (c - 2 * q1f) /
                       (4 * seq * seq + q1f * q2 * q2 * skk * skk));
    if ((q1f > 0) && (std::abs(q2 * skk) > 0)) {
      x = std::min(x, 2 * std::acosh(c / (2 * q1f)) / std::abs(q2 * skk));
    }
    auto S = seq * seq * x * x + 2 * q1f * std::cosh(q2 * skk * x / 2) - c;
    while (S > T(1.e-10)) {
      const auto dS =
          2 * seq * seq * x + q1f * q2 * skk * std::sinh(q2 * skk * x / 2);
      const auto dx = S / dS;
      x -= dx;
      // stop if the accuracy of the floating point type is reached
      if (std::abs(dx) < std::numeric_limits<T>::epsilon() * x) {
        break;
      }
      S = seq * seq * x * x + 2 * q1f * std::cosh(q2 * skk * x / 2) - c;
    }
    return 1 / x;
  }  // end of computeFlowStressGTN

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_HXX */
