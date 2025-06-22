/*!
 * \file   Drucker1949YieldCriterion.ixx
 * \brief
 * \author Thomas Helfer
 * \date   10/05/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_DRUCKER1949YIELDCRITERION_IXX
#define LIB_TFEL_MATERIAL_DRUCKER1949YIELDCRITERION_IXX

#include <cmath>

namespace tfel {

  namespace material {

    template <typename StressStensor>
    Drucker1949StressType<StressStensor> computeDrucker1949StressCriterion(
        const StressStensor& sig, const Drucker1949BaseType<StressStensor> c) {
      using real = Drucker1949BaseType<StressStensor>;
      constexpr const auto e = real(1) / 6;
      constexpr const auto sqrt3 = tfel::math::Cste<real>::sqrt3;
      const auto s = deviator(sig);
      const auto J2 = (s | s) / 2;
      const auto J3 = det(s);
      return sqrt3 * std::pow(J2 * J2 * J2 - c * J3 * J3, e);
    }  // end of computeDrucker1949StressCriterion

    template <typename StressStensor>
    std::tuple<Drucker1949StressType<StressStensor>,
               Drucker1949StressNormalType<StressStensor>>
    computeDrucker1949StressCriterionNormal(
        const StressStensor& sig,
        const Drucker1949BaseType<StressStensor> c,
        const Drucker1949StressType<StressStensor> seps) {
      using stress = Drucker1949StressType<StressStensor>;
      using real = Drucker1949BaseType<StressStensor>;
      constexpr const auto e = real(1) / 6;
      constexpr const auto sqrt3 = tfel::math::Cste<real>::sqrt3;
      const auto s = deviator(sig);
      const auto J2 = (s | s) / 2;
      if (J2 > seps * seps) {
        const auto J2_2 = J2 * J2;
        const auto J2_3 = J2_2 * J2;
        const auto J3 = det(s);
        const auto J3_2 = J3 * J3;
        const auto& dJ2 = s;
        const auto dJ3 = computeJ3Derivative(sig);
        const auto s6 = J2_3 - c * J3_2;
        const auto seq = sqrt3 * std::pow(s6, e);
        const auto dseq_dJ2 = J2_2 / (2 * s6) * seq;
        const auto dseq_dJ3 = -c * J3 / (3 * s6) * seq;
        const auto n = eval(dseq_dJ2 * dJ2 + dseq_dJ3 * dJ3);
        return std::make_tuple(seq, n);
      }
      return std::make_tuple(
          Drucker1949StressType<StressStensor>{stress{0}},
          Drucker1949StressNormalType<StressStensor>{real{0}});
    }  // end of computeDrucker1949StressCriterionNormal

    template <typename StressStensor>
    std::tuple<Drucker1949StressType<StressStensor>,
               Drucker1949StressNormalType<StressStensor>,
               Drucker1949StressSecondDerivativeType<StressStensor>>
    computeDrucker1949StressCriterionSecondDerivative(
        const StressStensor& sig,
        const Drucker1949BaseType<StressStensor> c,
        const Drucker1949StressType<StressStensor> seps) {
      using stress = Drucker1949StressType<StressStensor>;
      using real = Drucker1949BaseType<StressStensor>;
      using istress = Drucker1949InvertStressType<StressStensor>;
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      constexpr const auto e = real(1) / 6;
      constexpr const auto sqrt3 = tfel::math::Cste<real>::sqrt3;
      const auto s = deviator(sig);
      const auto J2 = (s | s) / 2;
      if (J2 > seps * seps) {
        const auto J2_2 = J2 * J2;
        const auto J2_3 = J2_2 * J2;
        const auto J2_4 = J2_2 * J2_2;
        const auto J2_6 = J2_4 * J2_2;
        const auto J3 = det(s);
        const auto J3_2 = J3 * J3;
        const auto J3_4 = J3_2 * J3_2;
        const auto& dJ2 = s;
        const auto d2J2 = tfel::math::st2tost2<N, real>::K();
        const auto dJ3 = computeJ3Derivative(sig);
        const auto d2J3 = computeJ3SecondDerivative(sig);
        const auto s6 = J2_3 - c * J3_2;
        const auto seq = sqrt3 * std::pow(s6, e);
        const auto dseq_dJ2 = J2_2 / (2 * s6) * seq;
        const auto dseq_dJ3 = -c * J3 / (3 * s6) * seq;
        const auto n = eval(dseq_dJ2 * dJ2 + dseq_dJ3 * dJ3);
        const auto d2seq_dJ2dJ2 =
            seq * (-4 * J2 * J3_2 * c - J2_4) /
            (4 * J3_4 * c * c - 8 * J2_3 * J3_2 + 4 * J2_6);
        const auto d2seq_dJ3dJ3 =
            seq * (-2 * J3_2 * c * c - 3 * J2_3 * c) /
            (9 * J3_4 * c * c - 18 * J2_3 * J3_2 + 9 * J2_6);
        const auto d2seq_dJ2dJ3 =
            seq * (5 * J2_2 * J3 * c) /
            (6 * J3_4 * c * c - 12 * J2_3 * J3_2 + 6 * J2_6);
        // d2f=f*d(df/f)+df*df/f
        const auto dn = eval(d2seq_dJ2dJ2 * (dJ2 ^ dJ2) + dseq_dJ2 * d2J2 +
                             d2seq_dJ2dJ3 * ((dJ2 ^ dJ3) + (dJ3 ^ dJ2)) +
                             d2seq_dJ3dJ3 * (dJ3 ^ dJ3) + dseq_dJ3 * d2J3);
        return std::make_tuple(seq, n, dn);
      }
      return std::make_tuple(
          Drucker1949StressType<StressStensor>{stress{0}},
          Drucker1949StressNormalType<StressStensor>{real{0}},
          Drucker1949StressSecondDerivativeType<StressStensor>{istress{0}});
    }  // end of computeDrucker1949StressCriterionSecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_DRUCKER1949YIELDCRITERION_IXX */
