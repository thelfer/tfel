/*!
 * \file   include/TFEL/Material/Cazacu2001YieldCriterion.ixx
 * \brief
 * \author Thomas Helfer
 * \date   11/05/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CAZACU2001YIELDCRITERION_IXX
#define LIB_TFEL_MATERIAL_CAZACU2001YIELDCRITERION_IXX

namespace tfel::material {

  template <typename StressStensor>
  Cazacu2001StressType<StressStensor> computeCazacu2001StressCriterion(
      const StressStensor& sig,
      const J2OCoefficients<StressStensor>& a,
      const J3OCoefficients<StressStensor>& b,
      const Cazacu2001BaseType<StressStensor> c) {
    using real = Cazacu2001BaseType<StressStensor>;
    constexpr const auto e = real(1) / 6;
    constexpr const auto sqrt3 = tfel::math::Cste<real>::sqrt3;
    const auto J2O = computeJ2O(sig, a);
    const auto J3O = computeJ3O(sig, b);
    return sqrt3 * pow(J2O * J2O * J2O - c * J3O * J3O, e);
  }  // end of computeCazacu2001StressCriterion

  template <typename StressStensor>
  std::tuple<Cazacu2001StressType<StressStensor>,
             Cazacu2001StressNormalType<StressStensor>>
  computeCazacu2001StressCriterionNormal(
      const StressStensor& sig,
      const J2OCoefficients<StressStensor>& a,
      const J3OCoefficients<StressStensor>& b,
      const Cazacu2001BaseType<StressStensor> c,
      const Cazacu2001StressType<StressStensor> seps) {
    using stress = Cazacu2001StressType<StressStensor>;
    using real = Cazacu2001BaseType<StressStensor>;
    constexpr const auto e = real(1) / 6;
    constexpr const auto sqrt3 = tfel::math::Cste<real>::sqrt3;
    const auto J2O = computeJ2O(sig, a);
    if (J2O > seps * seps) {
      const auto J2O_2 = J2O * J2O;
      const auto J2O_3 = J2O_2 * J2O;
      const auto J3O = computeJ3O(sig, b);
      const auto J3O_2 = J3O * J3O;
      const auto dJ2O = computeJ2ODerivative(sig, a);
      const auto dJ3O = computeJ3ODerivative(sig, b);
      const auto s6 = J2O_3 - c * J3O_2;
      const auto seq = sqrt3 * pow(s6, e);
      const auto dseq_dJ2O = J2O_2 / (2 * s6) * seq;
      const auto dseq_dJ3O = -c * J3O / (3 * s6) * seq;
      const auto n = dseq_dJ2O * dJ2O + dseq_dJ3O * dJ3O;
      return std::make_tuple(seq, n);
    }
    return std::make_tuple(Cazacu2001StressType<StressStensor>{stress{0}},
                           Cazacu2001StressNormalType<StressStensor>{real{0}});
  }  // end of computeCazacu2001StressCriterion

  template <typename StressStensor>
  std::tuple<Cazacu2001StressType<StressStensor>,
             Cazacu2001StressNormalType<StressStensor>,
             Cazacu2001StressSecondDerivativeType<StressStensor>>
  computeCazacu2001StressCriterionSecondDerivative(
      const StressStensor& sig,
      const J2OCoefficients<StressStensor>& a,
      const J3OCoefficients<StressStensor>& b,
      const Cazacu2001BaseType<StressStensor> c,
      const Cazacu2001StressType<StressStensor> seps) {
    using stress = Cazacu2001StressType<StressStensor>;
    using real = Cazacu2001BaseType<StressStensor>;
    using istress = Cazacu2001InvertStressType<StressStensor>;
    constexpr const auto e = real(1) / 6;
    constexpr const auto sqrt3 = tfel::math::Cste<real>::sqrt3;
    const auto J2O = computeJ2O(sig, a);
    if (J2O > seps * seps) {
      const auto J2O_2 = J2O * J2O;
      const auto J2O_3 = J2O_2 * J2O;
      const auto J2O_4 = J2O_2 * J2O_2;
      const auto J2O_6 = J2O_4 * J2O_2;
      const auto J3O = computeJ3O(sig, b);
      const auto J3O_2 = J3O * J3O;
      const auto J3O_4 = J3O_2 * J3O_2;
      const auto dJ2O = computeJ2ODerivative(sig, a);
      const auto d2J2O = computeJ2OSecondDerivative(sig, a);
      const auto dJ3O = computeJ3ODerivative(sig, b);
      const auto d2J3O = computeJ3OSecondDerivative(sig, b);
      const auto s6 = J2O_3 - c * J3O_2;
      const auto seq = sqrt3 * pow(s6, e);
      const auto dseq_dJ2O = J2O_2 / (2 * s6) * seq;
      const auto dseq_dJ3O = -c * J3O / (3 * s6) * seq;
      const auto n = dseq_dJ2O * dJ2O + dseq_dJ3O * dJ3O;
      const auto d2seq_dJ2OdJ2O =
          seq * (-4 * J2O * J3O_2 * c - J2O_4) /
          (4 * J3O_4 * c * c - 8 * J2O_3 * J3O_2 + 4 * J2O_6);
      const auto d2seq_dJ3OdJ3O =
          seq * (-2 * J3O_2 * c * c - 3 * J2O_3 * c) /
          (9 * J3O_4 * c * c - 18 * J2O_3 * J3O_2 + 9 * J2O_6);
      const auto d2seq_dJ2OdJ3O =
          seq * (5 * J2O_2 * J3O * c) /
          (6 * J3O_4 * c * c - 12 * J2O_3 * J3O_2 + 6 * J2O_6);
      // d2f=f*d(df/f)+df*df/f
      const auto dn = eval(d2seq_dJ2OdJ2O * (dJ2O ^ dJ2O) + dseq_dJ2O * d2J2O +
                           d2seq_dJ2OdJ3O * ((dJ2O ^ dJ3O) + (dJ3O ^ dJ2O)) +
                           d2seq_dJ3OdJ3O * (dJ3O ^ dJ3O) + dseq_dJ3O * d2J3O);
      return std::make_tuple(seq, n, dn);
    }
    return std::make_tuple(
        Cazacu2001StressType<StressStensor>{stress{0}},
        Cazacu2001StressNormalType<StressStensor>{real{0}},
        Cazacu2001StressSecondDerivativeType<StressStensor>{istress{0}});
  }  // end of computeCazacu2001StressCriterionSecondDerivative

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_CAZACU2001YIELDCRITERION_IXX */
