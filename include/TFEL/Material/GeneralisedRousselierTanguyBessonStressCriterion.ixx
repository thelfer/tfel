/*!
 * \file
 * include/TFEL/Material/GeneralisedRousselierTanguyBessonStressCriterion.ixx
 * \brief  This file implements the generalised Rousselier-Tanguy-Besson stress
 * criterion.
 * \author Tristan Chenaille
 * \date   14/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESS_CRITERION_IXX
#define LIB_TFEL_MATERIAL_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESS_CRITERION_IXX

#include <cmath>
#include <tuple>
#include <limits>
#include <algorithm>
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/ScalarNewtonRaphson.hxx"
#include "TFEL/Material/MaterialException.hxx"

namespace tfel::material {

  template <typename StressStensor>
  RousselierTanguyBesson2002StressType<StressStensor>
  computeGeneralisedRousselierTanguyBessonStress(
      const StressStensor& sig,
      const RousselierTanguyBesson2002StressType<StressStensor> seq,
      const RousselierTanguyBesson2002PorosityType<StressStensor> f,
      const GeneralisedRousselierTanguyBessonStressCriterionParameters<
          StressStensor>& p,
      const RousselierTanguyBesson2002StressType<StressStensor> seps) {
    using stress = RousselierTanguyBesson2002StressType<StressStensor>;
    using stress2 = tfel::math::result_type<stress, stress, tfel::math::OpMult>;
    using real = RousselierTanguyBesson2002BaseType<StressStensor>;
    using size_type = unsigned short;
    using tfel::math::base_type_cast;
    /*
     * The generalised RousselierTanguyBesson equivalent $\sigma^{\star}$ stress
     * satisfies:
     *
     * \[
     * S\left(\sigma^{\star}\right) = 0
     * \]
     *
     * with:
     *
     * \[
     * S\left(\sigma^{\star}\right) =
     *     \frac{\sigma_{eq}}{(1-f)\,\sigma^{\star}}
     *     + \frac{2}{3} f DR \exp\left( qR
     * \frac{3\,\sigma_m}{2\,(1-f)\,\sigma^{\star}}\right)
     *     - 1
     * \]
     *
     * where $\sigma_{eq}$ is the equivalent stress given by the caller.
     */
    const auto pr = trace(sig) / 3;
    const auto iseq = 1 / std::max(seq, seps);
    const auto omfd = (1 - f) * (1 - (2 * f * p.DR) / 3);
    const auto iomfd = 1 / std::max(omfd, real(1.e-12));
    const auto iomf = 1 / std::max((1 - f), real(1.e-12));
    // special cases
    if (tfel::math::abs(pr) < seps) {
      return seq * iomfd;
    }
    if (seq < seps) {
      const auto lf = std::log(3 / (2 * f * p.DR));
      const auto ilf = 1 / std::max(lf, base_type_cast(seps));
      return (3 * p.qR * (pr / 2) * iomf * ilf);
    }
    // First order approximation
    const auto A = iomf * seq;
    const auto B = 2 * f * (p.DR / 3);
    // without porosity, the criterion reduces to seq / (1 - f)
    if (tfel::math::ieee754::fpclassify(B) == FP_ZERO) {
      return seq * iomf;
    }
    const auto C = 3 * p.qR * pr * iomf / 2;
    const auto a = B * C * C / 2;
    const auto b = A + B * C;
    const auto c = B - 1;

    const auto x01 = [&] {
      const auto d = b * b - 4 * a * c;
      if ((d < stress2(0)) ||
          (tfel::math::ieee754::fpclassify(base_type_cast(a)) == FP_ZERO)) {
        return std::min(-c / b, iseq);
      }
      return std::min(std::max((-b + tfel::math::power<1, 2>(d)) / (2 * a),
                               (-b - tfel::math::power<1, 2>(d)) / (2 * a)),
                      iseq);
    }();
    // under tension, the starting point is bounded by the solution for a
    // vanishing equivalent stress, so that the Newton-Raphson algorithm does
    // not start far in the exponential branch
    const auto x0 = (pr > stress(0)) ? std::min(x01, std::log(1 / B) / C) : x01;
    // The Newton-Raphson algorithm works on the base numeric type, since the
    // unknown and S do not have the same unit
    const auto bseq = base_type_cast(seq);
    const auto bpr = base_type_cast(pr);
    const auto bseps = base_type_cast(seps);
    const auto SdS = [bseq, bpr, f, iomf, p](const real x) {
      const auto e = std::exp(3 * p.qR * bpr * iomf * tfel::math::abs(x) / 2);
      const auto S =
          bseq * iomf * tfel::math::abs(x) + 2 * f * (p.DR / 3) * e - 1;
      const auto dS = bseq * iomf + f * p.qR * p.DR * iomf * bpr * e;
      return std::make_tuple(S, dS);
    };
    const auto criterion = [bseps](const real, const real dx, const real x,
                                   const size_type) {
      // d(1/y) = -dy/(y*y) => y*y*d(1/y) = -dy
      // so if I want |dy|<eps, |y*y*d(1/y)| must be lower than eps
      // Here, x is the inverse of the equivalent stress, so
      if (10 * tfel::math::abs(dx) < bseps * tfel::math::abs(x * x)) {
        return true;
      }
      // relative stationnarity of the solution, which can be reached in
      // single precision, contrary to the previous criterion
      return tfel::math::abs(dx) <
             10 * std::numeric_limits<real>::epsilon() * tfel::math::abs(x);
    };
    const auto r = tfel::math::scalarNewtonRaphson(
        SdS, criterion, base_type_cast(x0), size_type{100});
    if (!std::get<0>(r)) {
      throw(DivergenceException());
    }
    return stress(1 / std::get<1>(r));
  }  // end of computeGeneralisedRousselierTanguyBessonStress

  template <typename StressStensor>
  std::tuple<
      RousselierTanguyBesson2002StressType<StressStensor>,
      RousselierTanguyBesson2002StressNormalType<StressStensor>,
      RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeGeneralisedRousselierTanguyBessonStressNormal(
      const StressStensor& sig,
      const RousselierTanguyBesson2002StressType<StressStensor> seq,
      const RousselierTanguyBesson2002StressNormalType<StressStensor>& dseq,
      const RousselierTanguyBesson2002PorosityType<StressStensor> f,
      const GeneralisedRousselierTanguyBessonStressCriterionParameters<
          StressStensor>& p,
      const RousselierTanguyBesson2002StressType<StressStensor> seps) {
    using stress = RousselierTanguyBesson2002StressType<StressStensor>;
    using istress = RousselierTanguyBesson2002InvertStressType<StressStensor>;
    using real = RousselierTanguyBesson2002BaseType<StressStensor>;
    using normal = RousselierTanguyBesson2002StressNormalType<StressStensor>;
    using result = std::tuple<
        RousselierTanguyBesson2002StressType<StressStensor>,
        RousselierTanguyBesson2002StressNormalType<StressStensor>,
        RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
            StressStensor>>;
    using tfel::math::base_type_cast;

    const auto id = normal::Id();

    const auto ss =
        computeGeneralisedRousselierTanguyBessonStress(sig, seq, f, p, seps);
    const auto iss = 1 / std::max(ss, seps);
    const auto iss2 = iss * iss;
    const auto fDq = f * p.DR * p.qR;
    const auto iomf = 1 / std::max(1 - f, real(1.e-12));
    const auto iomf2 = iomf * iomf;
    const auto sm = trace(sig) / 3;
    const auto e =
        std::exp(std::min(real((3 * p.qR * sm * iomf * iss) / 2), real(40.)));

    const auto dS_dsig = dseq * iomf * iss +  //
                         fDq * iss * iomf * e * (id / 3);
    const auto dS_dss = -seq * iomf * iss2 - fDq * sm * iomf * iss2 * e;
    const auto dS_df = seq * iss * iomf2 +  //
                       e * (((2 * p.DR) / 3) + fDq * sm * iomf2 * iss);

    const auto sgn_dS_dss = (dS_dss > istress(0)) ? 1 : -1;
    const auto idS_dss =
        sgn_dS_dss *
        ((tfel::math::ieee754::fpclassify(base_type_cast(dS_dss)) == FP_ZERO)
             ? stress(1 / base_type_cast(seps))
             : 1 / tfel::math::abs(dS_dss));
    const auto dss_dsig = -dS_dsig * idS_dss;
    const auto dss_df = -dS_df * idS_dss;

    return result{ss, dss_dsig, dss_df};
  }  // end of computeGeneralisedRousselierTanguyBessonStressNormal

  template <typename StressStensor>
  std::tuple<
      RousselierTanguyBesson2002StressType<StressStensor>,
      RousselierTanguyBesson2002StressNormalType<StressStensor>,
      RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
          StressStensor>,
      RousselierTanguyBesson2002StressSecondDerivativeType<StressStensor>,
      RousselierTanguyBesson2002NormalDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeGeneralisedRousselierTanguyBessonStressSecondDerivative(
      const StressStensor& sig,
      const RousselierTanguyBesson2002StressType<StressStensor> seq,
      const RousselierTanguyBesson2002StressNormalType<StressStensor>& dseq,
      const RousselierTanguyBesson2002StressSecondDerivativeType<StressStensor>&
          d2seq,
      const RousselierTanguyBesson2002PorosityType<StressStensor> f,
      const GeneralisedRousselierTanguyBessonStressCriterionParameters<
          StressStensor>& p,
      const RousselierTanguyBesson2002StressType<StressStensor> seps) {
    using stress = RousselierTanguyBesson2002StressType<StressStensor>;
    using istress = RousselierTanguyBesson2002InvertStressType<StressStensor>;
    using real = RousselierTanguyBesson2002BaseType<StressStensor>;
    using normal = RousselierTanguyBesson2002StressNormalType<StressStensor>;
    using result = std::tuple<
        RousselierTanguyBesson2002StressType<StressStensor>,
        RousselierTanguyBesson2002StressNormalType<StressStensor>,
        RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
            StressStensor>,
        RousselierTanguyBesson2002StressSecondDerivativeType<StressStensor>,
        RousselierTanguyBesson2002NormalDerivativeWithRespectToPorosityType<
            StressStensor>>;
    using namespace tfel::math;
    const auto id = normal::Id();

    const auto ss =
        computeGeneralisedRousselierTanguyBessonStress(sig, seq, f, p, seps);
    const auto iss = 1 / std::max(ss, seps);
    const auto iss2 = iss * iss;
    const auto fDq = f * p.DR * p.qR;
    const auto iomf = 1 / std::max(1 - f, real(1.e-12));
    const auto iomf2 = iomf * iomf;
    const auto sm = trace(sig) / 3;
    const auto e =
        std::exp(std::min(real((3 * p.qR * sm * iomf * iss) / 2), real(40.)));

    const auto dS_dsig = dseq * iomf * iss +  //
                         fDq * iss * iomf * e * (id / 3);
    const auto dS_dss = -seq * iomf * iss2 - fDq * sm * iomf * iss2 * e;
    const auto dS_df = seq * iss * iomf2 +  //
                       e * (((2 * p.DR) / 3) + fDq * sm * iomf2 * iss);

    const auto sgn_dS_dss = (dS_dss > istress(0)) ? 1 : -1;
    const auto idS_dss =
        sgn_dS_dss *
        ((tfel::math::ieee754::fpclassify(base_type_cast(dS_dss)) == FP_ZERO)
             ? stress(1 / base_type_cast(seps))
             : 1 / tfel::math::abs(dS_dss));
    const auto n = -dS_dsig * idS_dss;
    const auto dss_df = -dS_df * idS_dss;

    const auto d2S_dsig2 = iomf * iss * d2seq +  //
                           (fDq / 6) * p.qR * iomf2 * iss2 * e * (id ^ id);
    const auto d2S_dsigdss =
        -dseq * iomf * iss2 -               //
        (fDq / 3) * iomf * iss2 * e * id -  //
        (fDq / 2) * p.qR * iomf2 * iss * iss2 * sm * e * id;
    const auto d2S_dsigdf =
        dseq * iomf2 * iss +                        //
        p.DR * (p.qR / 3) * iomf2 * iss * e * id +  //
        (fDq / 2) * p.qR * iomf * iomf2 * iss2 * sm * e * id;
    const auto d2S_dssdf =
        -seq * iomf2 * iss2 -                  //
        p.DR * p.qR * sm * iomf2 * iss2 * e -  //
        (3 * fDq / 2) * p.qR * iomf * iomf2 * iss2 * iss * sm * sm * e;
    const auto d2S_dss2 =
        2 * seq * iomf * iss * iss2 +           //
        2 * fDq * sm * iomf * iss * iss2 * e +  //
        3 * fDq * (p.qR / 2) * sm * sm * iomf2 * iss2 * iss2 * e;

    const auto dn_dsig =
        -idS_dss * (d2S_dsig2 + (d2S_dsigdss ^ n)) +  //
        power<2>(idS_dss) * (dS_dsig ^ (d2S_dsigdss + (d2S_dss2 * n)));
    const auto dn_df =
        -idS_dss * (d2S_dsigdf + (d2S_dssdf * n)) +  //
        power<2>(idS_dss) * ((d2S_dsigdss + (d2S_dss2 * n)) * dS_df);

    return result{ss, n, dss_df, dn_dsig, dn_df};
  }  // end of computeGeneralisedRousselierTanguyBessonStressSecondDerivative

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESS_CRITERION_IXX \
        */
