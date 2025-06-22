/*!
 * \file   include/TFEL/Material/RousselierTanguyBesson2002StressCriterion.ixx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   14/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ROUSSELIER_TANGUY_BESSON_2002_IXX
#define LIB_TFEL_MATERIAL_ROUSSELIER_TANGUY_BESSON_2002_IXX

#include <cmath>
#include <ostream>
#include <algorithm>
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/ScalarNewtonRaphson.hxx"
#include "TFEL/Material/MaterialException.hxx"

namespace tfel {

  namespace material {

    template <typename StressStensor>
    std::ostream& operator<<(
        std::ostream& os,
        const RousselierTanguyBesson2002StressCriterionParameters<
            StressStensor>& p) {
      os << "{DR: " << p.DR << ", qR: " << p.qR << "}";
      return os;
    }  // end of operator <<

    template <typename StressStensor>
    RousselierTanguyBesson2002StressType<StressStensor>
    computeRousselierTanguyBesson2002Stress(
        const StressStensor& sig,
        const RousselierTanguyBesson2002PorosityType<StressStensor> f,
        const RousselierTanguyBesson2002StressCriterionParameters<
            StressStensor>& p,
        const RousselierTanguyBesson2002StressType<StressStensor> seps) {
      using istress = RousselierTanguyBesson2002InvertStressType<StressStensor>;
      using real = RousselierTanguyBesson2002BaseType<StressStensor>;
      using size_type = unsigned short;
      /*
       * The RousselierTanguyBesson equivalent $\sigma^{\star}$ stress
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
       *     \frac{\sigma_{vM}}{(1-f)\,\sigma^{\star}}
       *     + \frac{2}{3} f DR \exp\left( qR
       * \frac{3\,\sigma_m}{2\,(1-f)\,\sigma^{\star}}\right)
       *     - 1
       * \]
       *
       */
      // using boost::math::lambert_w0;

      const auto seq = sigmaeq(sig);
      const auto pr = trace(sig) / 3;
      const auto iseq = 1 / std::max(seq, seps);
      const auto omfd = (1 - f) * (1 - (2 * f * p.DR) / 3);
      const auto iomfd = 1 / std::max(omfd, real(1.e-12));
      const auto iomf = 1 / std::max((1 - f), real(1.e-12));
      // special cases
      if (std::abs(pr) < seps) {
        return seq * iomfd;
      }
      if (seq < seps) {
        const auto lf = std::log(3 / (2 * f * p.DR));
        const auto ilf = 1 / std::max(lf, seps);
        return (3 * p.qR * (pr / 2) * iomf * ilf);
      }
      // First order approximation
      const auto A = iomf * seq;
      const auto B = 2 * f * (p.DR / 3);
      const auto C = 3 * p.qR * pr * iomf / 2;
      const auto a = B * C * C / 2;
      const auto b = A + B * C;
      const auto c = B - 1;

      const auto x01 = [&] {
        const auto d = b * b - 4 * a * c;
        if ((d < 0) || (tfel::math::ieee754::fpclassify(a) == FP_ZERO)) {
          return std::min(-c / b, iseq);
        }
        return std::min(
            std::max((-b + sqrt(d)) / (2 * a), (-b - sqrt(d)) / (2 * a)), iseq);
      }();
      // const auto lf = std::log(3 / (2 * f * p.DR));
      // const auto ilf = 1 / std::max(lf, seps);
      // const auto x0 = (x01 + 1 / (3 * p.qR * (pr / 2) * iomf * ilf)) / 2;
      const auto x0 = x01;
      // const auto d = sqrt(b * b - 4) auto x0 = c / b;  // iseq * omfd;
      const auto SdS = [seq, pr, f, iomf, p](const istress x) {
        const auto e = std::exp(3 * p.qR * pr * iomf * std::abs(x) / 2);
        const auto S = seq * iomf * std::abs(x) + 2 * f * (p.DR / 3) * e - 1;
        const auto dS = seq * iomf + f * p.qR * p.DR * iomf * pr * e;
        return std::make_tuple(S, dS);
      };
      const auto criterion = [seps](const real, const istress dx,
                                    const istress x, const size_type) {
        // d(1/y) = -dy/(y*y) => y*y*d(1/y) = -dy
        // so if I want |dy|<eps, |y*y*d(1/y)| must be lower than eps
        // Here, x is the inverse of the equivalent stress, so
        return 10 * std::abs(dx) < seps * std::abs(x * x);
      };
      const auto r =
          tfel::math::scalarNewtonRaphson(SdS, criterion, x0, size_type{100});
      if (!std::get<0>(r)) {
        throw(DivergenceException());
      }
      return 1 / std::get<1>(r);
    }  // end of computeRousselierTanguyBesson2002YieldStress

    template <typename StressStensor>
    std::tuple<
        RousselierTanguyBesson2002StressType<StressStensor>,
        RousselierTanguyBesson2002StressNormalType<StressStensor>,
        RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
            StressStensor>>
    computeRousselierTanguyBesson2002StressNormal(
        const StressStensor& sig,
        const RousselierTanguyBesson2002PorosityType<StressStensor> f,
        const RousselierTanguyBesson2002StressCriterionParameters<
            StressStensor>& p,
        const RousselierTanguyBesson2002StressType<StressStensor> seps) {
      using real = RousselierTanguyBesson2002BaseType<StressStensor>;
      using normal = RousselierTanguyBesson2002StressNormalType<StressStensor>;
      using result = std::tuple<
          RousselierTanguyBesson2002StressType<StressStensor>,
          RousselierTanguyBesson2002StressNormalType<StressStensor>,
          RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
              StressStensor>>;

      const auto id = normal::Id();

      const auto ss = computeRousselierTanguyBesson2002Stress(sig, f, p, seps);
      const auto iss = 1 / std::max(ss, seps);
      const auto iss2 = iss * iss;
      const auto seq = sigmaeq(sig);
      const auto iseq = 1 / std::max(seq, seps);
      const auto fDq = f * p.DR * p.qR;
      const auto iomf = 1 / std::max(1 - f, real(1.e-12));
      const auto iomf2 = iomf * iomf;
      const auto sm = trace(sig) / 3;
      const auto e =
          std::exp(std::min((3 * p.qR * sm * iomf * iss) / 2, real(40.)));

      const auto dS_dsig = (3 * deviator(sig) * iomf * iseq * iss) / 2 +  //
                           fDq * iss * iomf * e * (id / 3);
      const auto dS_dss = -seq * iomf * iss2 - fDq * sm * iomf * iss2 * e;
      const auto dS_df = seq * iss * iomf2 +  //
                         e * (((2 * p.DR) / 3) + fDq * sm * iomf2 * iss);

      const auto sgn_dS_dss = (dS_dss > 0) ? 1 : -1;
      const auto idS_dss =
          sgn_dS_dss * ((tfel::math::ieee754::fpclassify(dS_dss) == FP_ZERO)
                            ? 1 / seps
                            : 1 / std::abs(dS_dss));
      const auto dss_dsig = -dS_dsig * idS_dss;
      const auto dss_df = -dS_df * idS_dss;

      return result{ss, dss_dsig, dss_df};
    }  // end of computeRousselierTanguyBesson2002StressNormal

    template <typename StressStensor>
    std::tuple<
        RousselierTanguyBesson2002StressType<StressStensor>,
        RousselierTanguyBesson2002StressNormalType<StressStensor>,
        RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
            StressStensor>,
        RousselierTanguyBesson2002StressSecondDerivativeType<StressStensor>,
        RousselierTanguyBesson2002NormalDerivativeWithRespectToPorosityType<
            StressStensor>>
    computeRousselierTanguyBesson2002StressSecondDerivative(
        const StressStensor& sig,
        const RousselierTanguyBesson2002PorosityType<StressStensor> f,
        const RousselierTanguyBesson2002StressCriterionParameters<
            StressStensor>& p,
        const RousselierTanguyBesson2002StressType<StressStensor> seps) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      // using stress = RousselierTanguyBesson2002StressType<StressStensor>;
      using real = RousselierTanguyBesson2002BaseType<StressStensor>;
      // using istress = tfel::math::result_type<real, stress,
      // tfel::math::OpDiv>;
      using normal = RousselierTanguyBesson2002StressNormalType<StressStensor>;
      using result = std::tuple<
          RousselierTanguyBesson2002StressType<StressStensor>,
          RousselierTanguyBesson2002StressNormalType<StressStensor>,
          RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
              StressStensor>,
          RousselierTanguyBesson2002StressSecondDerivativeType<StressStensor>,
          RousselierTanguyBesson2002NormalDerivativeWithRespectToPorosityType<
              StressStensor>>;
      // using normal_derivative =
      // RousselierTanguyBesson2002StressSecondDerivativeType<StressStensor>;
      using namespace tfel::math;
      const auto id = normal::Id();
      const auto M = tfel::math::st2tost2<N, real>::M();

      const auto ss = computeRousselierTanguyBesson2002Stress(sig, f, p, seps);
      const auto iss = 1 / std::max(ss, seps);
      const auto iss2 = iss * iss;
      const auto seq = sigmaeq(sig);
      const auto iseq = 1 / std::max(seq, seps);
      const auto fDq = f * p.DR * p.qR;
      const auto iomf = 1 / std::max(1 - f, real(1.e-12));
      const auto iomf2 = iomf * iomf;
      const auto sm = trace(sig) / 3;
      const auto e =
          std::exp(std::min((3 * p.qR * sm * iomf * iss) / 2, real(40.)));

      const auto dS_dsig = (3 * deviator(sig) * iomf * iseq * iss) / 2 +  //
                           fDq * iss * iomf * e * (id / 3);
      const auto dS_dss = -seq * iomf * iss2 - fDq * sm * iomf * iss2 * e;
      const auto dS_df = seq * iss * iomf2 +  //
                         e * (((2 * p.DR) / 3) + fDq * sm * iomf2 * iss);

      const auto sgn_dS_dss = (dS_dss > 0) ? 1 : -1;
      const auto idS_dss =
          sgn_dS_dss * ((tfel::math::ieee754::fpclassify(dS_dss) == FP_ZERO)
                            ? 1 / seps
                            : 1 / std::abs(dS_dss));
      const auto n = -dS_dsig * idS_dss;
      const auto dss_df = -dS_df * idS_dss;

      const auto nvm = (3 * deviator(sig) * iseq) / 2;

      const auto d2S_dsig2 = iomf * iss * iseq * (M - (nvm ^ nvm)) +  //
                             (fDq / 6) * p.qR * iomf2 * iss2 * e * (id ^ id);
      const auto d2S_dsigdss =
          -nvm * iomf * iss2 -                //
          (fDq / 3) * iomf * iss2 * e * id -  //
          (fDq / 2) * p.qR * iomf2 * iss * iss2 * sm * e * id;
      const auto d2S_dsigdf =
          nvm * iomf2 * iss +                         //
          p.DR * (p.qR / 3) * iomf2 * iss * e * id +  //
          (fDq / 2) * p.qR * iomf * iomf2 * iss2 * sm * e * id;
      const auto d2S_dssdf =
          -seq * iomf2 * iss2 -                  //
          p.DR * p.qR * sm * iomf2 * iss2 * e -  //
          (1.5 * fDq) * p.qR * iomf * iomf2 * iss2 * iss * sm * sm * e;
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
    }  // end of computeRousselierTanguyBesson2002SecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_ROUSSELIER_TANGUY_BESSON_2002_IXX */
