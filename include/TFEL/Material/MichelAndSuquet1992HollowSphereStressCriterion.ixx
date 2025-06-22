/*!
 * \file
 * include/TFEL/Material/MichelAndSuquet1992HollowSphereStressCriterion.ixx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   20/07/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MICHEL_SUQUET_1992_HOLLOW_SPHERE_IXX
#define LIB_TFEL_MATERIAL_MICHEL_SUQUET_1992_HOLLOW_SPHERE_IXX

#include "TFEL/Math/General/IEEE754.hxx"

#include <algorithm>
#include <cmath>
#include <ostream>

namespace tfel {

  namespace material {

    template <typename StressStensor>
    std::ostream &operator<<(
        std::ostream &os,
        const MichelAndSuquet1992HollowSphereStressCriterionParameters<
            StressStensor> &p) {
      os << "{n: " << p.n << ", feps: " << p.feps << "}";
      return os;
    }  // end of operator <<

    template <typename StressStensor>
    MichelAndSuquet1992HollowSphereStressType<StressStensor>
    computeMichelAndSuquet1992HollowSphereStress(
        const StressStensor &sig,
        const MichelAndSuquet1992HollowSpherePorosityType<StressStensor> f,
        const MichelAndSuquet1992HollowSphereStressCriterionParameters<
            StressStensor> &p,
        const MichelAndSuquet1992HollowSphereStressType<StressStensor> seps) {
      // a simple alias to the underlying numeric type
      using real = MichelAndSuquet1992HollowSphereBaseType<StressStensor>;
      constexpr const auto cste_3_2 = real(3) / 2;
      constexpr const auto cste_9_4 = real(9) / 4;
      const auto s = deviator(sig);
      const auto s2 = cste_3_2 * (s | s);
      const auto pr = trace(sig) / 3;
      const auto n = p.n;
      const auto is_zero = tfel::math::ieee754::fpclassify(f) == FP_ZERO;
      const auto A0 =
          f < p.feps ? pow((f + p.feps) / 2, -1 / n) : pow(f, -1 / n);
      const auto A = is_zero ? 0 : pow(n * (A0 - 1), -2 * n / (n + 1));
      const auto B = (1 + (2 * f) / 3) * pow(1 - f, -2 * n / (n + 1));
      return std::sqrt(cste_9_4 * A * pr * pr + B * s2);
    }  // end of computeMichelAndSuquet1992HollowSphereYieldStress

    template <typename StressStensor>
    std::tuple<
        MichelAndSuquet1992HollowSphereStressType<StressStensor>,
        MichelAndSuquet1992HollowSphereStressNormalType<StressStensor>,
        MichelAndSuquet1992HollowSphereStressDerivativeWithRespectToPorosityType<
            StressStensor>>
    computeMichelAndSuquet1992HollowSphereStressNormal(
        const StressStensor &sig,
        const MichelAndSuquet1992HollowSpherePorosityType<StressStensor> f,
        const MichelAndSuquet1992HollowSphereStressCriterionParameters<
            StressStensor> &p,
        const MichelAndSuquet1992HollowSphereStressType<StressStensor> seps) {
      using real = MichelAndSuquet1992HollowSphereBaseType<StressStensor>;
      constexpr const auto id =
          MichelAndSuquet1992HollowSphereStressNormalType<StressStensor>::Id();
      constexpr const auto cste_2_3 = real(2) / 3;
      constexpr const auto cste_3_2 = real(3) / 2;
      constexpr const auto cste_9_4 = real(9) / 4;
      const auto s = deviator(sig);
      const auto s2 = cste_3_2 * (s | s);
      const auto pr = trace(sig) / 3;
      const auto n = p.n;
      const auto d = 1 - f;
      const auto inv_d = 1 / std::max(d, p.feps);
      const auto is_zero = tfel::math::ieee754::fpclassify(f) == FP_ZERO;
      const auto A0 =
          f < p.feps ? pow((f + p.feps) / 2, -1 / n) : pow(f, -1 / n);
      const auto A = is_zero ? 0 : pow(n * (A0 - 1), -2 * n / (n + 1));
      const auto B1 = 1 + cste_2_3 * f;
      const auto B2 = pow(1 - f, -2 * n / (n + 1));
      const auto B = B1 * B2;
      const auto seq = std::sqrt(cste_9_4 * A * pr * pr + B * s2);
      const auto iseq = 1 / (std::max(seq, seps));
      const auto dseq_dsig = cste_3_2 * iseq * ((A * pr / 2) * id + B * s);
      const auto dA_df =
          2 * A * A0 / (std::max(f, p.feps2) * (A0 - 1) * (n + 1));
      const auto dB1_df = cste_2_3;
      const auto dB2_df = 2 * n * B2 * inv_d / (n + 1);
      const auto dB_df = B1 * dB2_df + B2 * dB1_df;
      const auto dseq_df =
          (cste_9_4 * pr * pr * dA_df + s2 * dB_df) * (iseq / 2);
      return {seq, dseq_dsig, dseq_df};
    }  // end of computeMichelAndSuquet1992HollowSphereStressNormal

    template <typename StressStensor>
    std::tuple<
        MichelAndSuquet1992HollowSphereStressType<StressStensor>,
        MichelAndSuquet1992HollowSphereStressNormalType<StressStensor>,
        MichelAndSuquet1992HollowSphereStressDerivativeWithRespectToPorosityType<
            StressStensor>,
        MichelAndSuquet1992HollowSphereStressSecondDerivativeType<
            StressStensor>,
        MichelAndSuquet1992HollowSphereNormalDerivativeWithRespectToPorosityType<
            StressStensor>>
    computeMichelAndSuquet1992HollowSphereStressSecondDerivative(
        const StressStensor &sig,
        const MichelAndSuquet1992HollowSpherePorosityType<StressStensor> f,
        const MichelAndSuquet1992HollowSphereStressCriterionParameters<
            StressStensor> &p,
        const MichelAndSuquet1992HollowSphereStressType<StressStensor> seps) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using real = MichelAndSuquet1992HollowSphereBaseType<StressStensor>;
      using normal =
          MichelAndSuquet1992HollowSphereStressNormalType<StressStensor>;
      using Stensor4 = tfel::math::st2tost2<N, real>;
      constexpr const auto id = normal::Id();
      constexpr const auto cste_2_3 = real(2) / 3;
      constexpr const auto cste_3_2 = real(3) / 2;
      constexpr const auto cste_9_4 = real(9) / 4;
      const auto s = deviator(sig);
      const auto s2 = cste_3_2 * (s | s);
      const auto pr = trace(sig) / 3;
      const auto n = p.n;
      const auto d = 1 - f;
      const auto inv_d = 1 / std::max(d, p.feps);
      const auto is_zero = tfel::math::ieee754::fpclassify(f) == FP_ZERO;
      const auto A0 =
          f < p.feps ? pow((f + p.feps) / 2, -1 / n) : pow(f, -1 / n);
      const auto A = is_zero ? 0 : pow(n * (A0 - 1), -2 * n / (n + 1));
      const auto B1 = 1 + cste_2_3 * f;
      const auto B2 = pow(1 - f, -2 * n / (n + 1));
      const auto B = B1 * B2;
      const auto seq = std::sqrt(cste_9_4 * A * pr * pr + B * s2);
      // derivatives with respect to the stress
      const auto iseq = 1 / (std::max(seq, seps));
      const auto C = cste_3_2 * ((A * pr / 2) * id + B * s);
      const auto dseq_dsig = iseq * C;
      const auto dC_dsig = cste_3_2 * ((A / 6) * (id ^ id) + B * Stensor4::K());
      const auto d2seq_dsigdsig = iseq * (dC_dsig - (dseq_dsig ^ dseq_dsig));
      // derivatives with respect to the porosity
      const auto dA_df =
          2 * A * A0 / (std::max(f, p.feps2) * (A0 - 1) * (n + 1));
      const auto dB1_df = cste_2_3;
      const auto dB2_df = 2 * n * inv_d * B2 / (n + 1);
      const auto dB_df = B1 * dB2_df + B2 * dB1_df;
      const auto dseq_df =
          (cste_9_4 * pr * pr * dA_df + s2 * dB_df) * (iseq / 2);
      // derivative with respect to the porosity and the stress
      const auto dC_df = cste_3_2 * (pr * dA_df / 2 * id + dB_df * s);
      const auto d2seq_dsigdf = iseq * dC_df - iseq * dseq_df * dseq_dsig;
      return {seq, dseq_dsig, dseq_df, d2seq_dsigdsig, d2seq_dsigdf};
    }  // end of computeMichelAndSuquet1992HollowSphereSecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_MICHEL_SUQUET_1992_HOLLOW_SPHERE_IXX */
