/*!
 * \file   include/TFEL/Material/GursonTvergaardNeedleman1982StressCriterion.ixx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   24/03/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_GURSON_1975_IXX
#define LIB_TFEL_MATERIAL_GURSON_1975_IXX

#include <algorithm>
#include <cmath>
#include <ostream>
#include <tuple>
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/ScalarNewtonRaphson.hxx"
#include "TFEL/Material/MaterialException.hxx"

namespace tfel {

  namespace material {

    template <typename StressStensor>
    std::ostream &operator<<(
        std::ostream &os,
        const GursonTvergaardNeedleman1982StressCriterionParameters<
            StressStensor> &p) {
      os << "{f_c: " << p.f_c << ", f_r: " << p.f_r << ", q_1: " << p.q_1
         << ", q_2: " << p.q_2 << ", q_3: " << p.q_3 << "}";
      return os;
    }  // end of operator <<

    template <typename StressStensor>
    GursonTvergaardNeedleman1982StressType<StressStensor>
    computeGursonTvergaardNeedleman1982Stress(
        const StressStensor &sig,
        const GursonTvergaardNeedleman1982PorosityType<StressStensor> f,
        const GursonTvergaardNeedleman1982StressCriterionParameters<
            StressStensor> &p,
        const GursonTvergaardNeedleman1982StressType<StressStensor> seps) {
      using istress =
          GursonTvergaardNeedleman1982InvertStressType<StressStensor>;
      using real = GursonTvergaardNeedleman1982BaseType<StressStensor>;
      using size_type = unsigned short;
      using namespace tfel::math;
      /*
       * The GTN equivalent $\sigma^{\star}$ stress satisfies:
       *
       * \[
       * S\left(\sigma^{\star}\right) = 0
       * \]
       *
       * with:
       *
       * \[
       * S\left(\sigma^{\star}\right) =
       *     \left(\frac{\sigma_{vM}}{\sigma^{\star}} \right)^2
       *     + 2 q1 f_{*} \cosh\left( \frac{3\, q2\,
       * \sigma_m}{2\,\sigma^{\star}}\right)
       *     - 1 - q3 (f_{*})^2
       * \]
       *
       * Here we use a Newton-Raphson algorithm to find the value of
       * $1/\sigma^{\star}$.
       */
      const auto seq = sigmaeq(sig);
      const auto pr = trace(sig) / 3;
      const auto fult = (p.q_1 - sqrt(p.q_1 * p.q_1 - p.q_3)) / p.q_3;
      const auto delta = (fult - p.f_c) / (p.f_r - p.f_c);
      const auto fstar = (f < p.f_c) ? f : (p.f_c + delta * (f - p.f_c));
      // square of the positive value
      const auto seq2 = seq * seq;
      const auto pr2 = pr * pr;
      // special cases
      if (std::abs(pr) < seps) {
        const auto iomf =
            1 / std::max(1 - 2 * p.q_1 * fstar + p.q_3 * fstar * fstar,
                         real(1.e-12));
        return seq * std::sqrt(iomf);
      }
      if (seq < seps) {
        const auto argach = (1 + p.q_3 * fstar * fstar) /
                            std::max(2 * p.q_1 * fstar, real(1.e-12));
        const auto den = 2 * std::acosh(argach);
        return (3 * p.q_2 * pr) / den;
      }
      // Second order approximation (development of the hyperbolic cosine to the
      // second order)
      const auto x0 =
          std::sqrt((4 * (1 + p.q_3 * fstar * fstar - 2 * p.q_1 * fstar)) /
                    (4 * seq2 + 9 * p.q_1 * p.q_2 * p.q_2 * fstar * pr2));
      //
      const auto SdS = [seq2, pr, fstar, p](const istress x) {
        const auto fstar2 = fstar * fstar;
        const auto e = std::exp(3 * p.q_2 * pr * (x / 2));
        const auto ch = (e + 1 / e) / 2;
        const auto sh = (e - 1 / e) / 2;
        const auto S =
            seq2 * x * x + 2 * p.q_1 * fstar * ch - 1 - p.q_3 * fstar2;
        const auto dS = 2 * seq2 * x + 3 * p.q_1 * p.q_2 * fstar * pr * sh;
        return std::make_tuple(S, dS);
      };
      const auto c = [&p, seps](const real S, const istress dx, const istress x,
                                const size_type) {
        // First criterion based on the value of the yield surface
        if (std::abs(S) < p.eps) {
          return true;
        }
        // Second criterion based on the stationnarity of the solution
        // This criterion may become much more restrictive than the first one
        // when the equivalent stress tends to zero.
        //
        // d(1/y) = -dy/(y*y) => y*y*d(1/y) = -dy
        // so if I want |dy|<eps, |y*y*d(1/y)| must be lower than eps
        // Here, x is the inverse of the equivalent stress, so
        return 10 * std::abs(dx) < seps * std::abs(x * x);
      };
      const auto r =
          tfel::math::scalarNewtonRaphson(SdS, c, x0, size_type{100});
      if (!std::get<0>(r)) {
        throw(DivergenceException());
      }
      return 1 / std::get<1>(r);
    }  // end of computeGursonTvergaardNeedleman1982Stress

    template <typename StressStensor>
    std::tuple<
        GursonTvergaardNeedleman1982StressType<StressStensor>,
        GursonTvergaardNeedleman1982StressNormalType<StressStensor>,
        GursonTvergaardNeedleman1982StressDerivativeWithRespectToPorosityType<
            StressStensor>>
    computeGursonTvergaardNeedleman1982StressNormal(
        const StressStensor &sig,
        const GursonTvergaardNeedleman1982PorosityType<StressStensor> f,
        const GursonTvergaardNeedleman1982StressCriterionParameters<
            StressStensor> &p,
        const GursonTvergaardNeedleman1982StressType<StressStensor> seps) {
      using normal =
          GursonTvergaardNeedleman1982StressNormalType<StressStensor>;
      using result = std::tuple<
          GursonTvergaardNeedleman1982StressType<StressStensor>,
          GursonTvergaardNeedleman1982StressNormalType<StressStensor>,
          GursonTvergaardNeedleman1982StressDerivativeWithRespectToPorosityType<
              StressStensor>>;
      using namespace tfel::math;
      const auto id = normal::Id();
      const auto ss =
          computeGursonTvergaardNeedleman1982Stress(sig, f, p, seps);
      const auto iss = 1 / std::max(ss, seps);
      const auto seq = sigmaeq(sig);
      const auto pr = trace(sig) / 3;
      const auto fult = (p.q_1 - sqrt(p.q_1 * p.q_1 - p.q_3)) / p.q_3;
      const auto delta = (fult - p.f_c) / (p.f_r - p.f_c);
      const auto fstar = (f < p.f_c) ? f : (p.f_c + delta * (f - p.f_c));
      const auto dfstar = (f < p.f_c) ? 1. : delta;
      const auto e = std::exp(3 * p.q_2 * pr * iss / 2);
      const auto ch = (e + 1 / e) / 2;
      const auto sh = (e - 1 / e) / 2;
      const auto q1q2 = p.q_1 * p.q_2;
      // normal
      const auto dS_dss =
          (-2 * power<2>(seq) * iss - 3 * q1q2 * fstar * pr * sh) *
          power<2>(iss);
      const auto sgn = (dS_dss < 0) ? -1 : 1;
      const auto idS_dss =
          sgn * ((tfel::math::ieee754::fpclassify(dS_dss) == FP_ZERO)
                     ? 1 / seps
                     : 1 / std::abs(dS_dss));
      const auto dS_dsig = 3 * power<2>(iss) * deviator(sig) +  //
                           q1q2 * fstar * iss * sh * id;
      const auto n = eval(-dS_dsig * idS_dss);

      // derivative of \(\sigma^{\star}\) with respect to \(f\)
      const auto dS_df = (2 * p.q_1 * ch - 2 * p.q_3 * fstar) * dfstar;
      const auto dss_df = -dS_df * idS_dss;
      // result
      return result{ss, n, dss_df};
    }  // end of computeGursonTvergaardNeedleman1982StressNormal

    template <typename StressStensor>
    std::tuple<
        GursonTvergaardNeedleman1982StressType<StressStensor>,
        GursonTvergaardNeedleman1982StressNormalType<StressStensor>,
        GursonTvergaardNeedleman1982StressDerivativeWithRespectToPorosityType<
            StressStensor>,
        GursonTvergaardNeedleman1982StressSecondDerivativeType<StressStensor>,
        GursonTvergaardNeedleman1982NormalDerivativeWithRespectToPorosityType<
            StressStensor>>
    computeGursonTvergaardNeedleman1982StressSecondDerivative(
        const StressStensor &sig,
        const GursonTvergaardNeedleman1982PorosityType<StressStensor> f,
        const GursonTvergaardNeedleman1982StressCriterionParameters<
            StressStensor> &p,
        const GursonTvergaardNeedleman1982StressType<StressStensor> seps) {
      using namespace tfel::math;
      using normal =
          GursonTvergaardNeedleman1982StressNormalType<StressStensor>;
      using result = std::tuple<
          GursonTvergaardNeedleman1982StressType<StressStensor>,
          GursonTvergaardNeedleman1982StressNormalType<StressStensor>,
          GursonTvergaardNeedleman1982StressDerivativeWithRespectToPorosityType<
              StressStensor>,
          GursonTvergaardNeedleman1982StressSecondDerivativeType<StressStensor>,
          GursonTvergaardNeedleman1982NormalDerivativeWithRespectToPorosityType<
              StressStensor>>;
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      const auto M = tfel::math::st2tost2<N, double>::M();
      const auto id = normal::Id();
      const auto s = deviator(sig);
      const auto ss =
          computeGursonTvergaardNeedleman1982Stress(sig, f, p, seps);
      const auto iss = 1 / std::max(ss, seps);
      const auto seq = sigmaeq(sig);
      const auto seq2 = seq * seq;
      const auto pr = trace(sig) / 3;
      const auto fult = (p.q_1 - sqrt(p.q_1 * p.q_1 - p.q_3)) / p.q_3;
      const auto delta = (fult - p.f_c) / (p.f_r - p.f_c);
      const auto fstar = (f < p.f_c) ? f : (p.f_c + delta * (f - p.f_c));
      const auto dfstar = (f < p.f_c) ? 1. : delta;
      const auto e = std::exp(3 * p.q_2 * pr * iss / 2);
      const auto ch = (e + 1 / e) / 2;
      const auto sh = (e - 1 / e) / 2;
      const auto q1q2 = p.q_1 * p.q_2;
      // normal
      const auto dS_dss =
          (-2 * power<2>(seq) * iss - 3 * q1q2 * fstar * pr * sh) *
          power<2>(iss);
      const auto sgn = (dS_dss < 0) ? -1 : 1;
      const auto idS_dss =
          sgn * ((tfel::math::ieee754::fpclassify(dS_dss) == FP_ZERO)
                     ? 1 / seps
                     : 1 / std::abs(dS_dss));
      const auto dS_dsig = 3 * power<2>(iss) * deviator(sig) +  //
                           q1q2 * fstar * iss * sh * id;
      const auto n = eval(-dS_dsig * idS_dss);

      // second derivative of the normal
      const auto d2S_dsig2 =
          (2. * M + (q1q2 * p.q_2 * (fstar / 2) * ch * (id ^ id))) *
          power<2>(iss);
      const auto d2S_dsigdss =
          (-6. * s * iss - q1q2 * fstar * sh * id -  //
           3 * q1q2 * p.q_2 * (fstar / 2) * pr * iss * ch * id) *
          power<2>(iss);
      const auto d2S_dss2 =
          ((6 * seq2 * iss) + (3 * q1q2 * fstar * pr) *  //
                                  (2 * sh + 3 * p.q_2 * pr * (iss / 2) * ch)) *
          power<3>(iss);
      const auto dn_dsig =
          -idS_dss * (d2S_dsig2 + (d2S_dsigdss ^ n)) +  //
          power<2>(idS_dss) * (dS_dsig ^ (d2S_dsigdss + (d2S_dss2 * n)));

      // derivatives with respect to \(f\)
      const auto dS_df = (2 * p.q_1 * ch - 2 * p.q_3 * fstar) * dfstar;
      const auto dss_df = -dS_df * idS_dss;
      const auto d2S_dssdf = (-3. * q1q2 * pr * power<2>(iss) * sh) * dfstar;
      const auto d2S_dsigdf = (q1q2 * iss * sh * id) * dfstar;
      const auto dn_df =
          -idS_dss * (d2S_dsigdf + (d2S_dssdf * n)) +  //
          power<2>(idS_dss) * ((d2S_dsigdss + (d2S_dss2 * n)) * dS_df);

      return result{ss, n, dss_df, dn_dsig, dn_df};
    }  // end of computeGursonTvergaardNeedleman1982SecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_GURSON_1975_IXX */
