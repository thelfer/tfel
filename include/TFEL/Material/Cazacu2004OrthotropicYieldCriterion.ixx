/*!
 * \file   include/TFEL/Material/Cazacu2004OrthotropicYieldCriterion.ixx
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

#ifndef LIB_TFEL_MATERIAL_CAZACU2004ORTHOTROPICYIELDCRITERION_IXX
#define LIB_TFEL_MATERIAL_CAZACU2004ORTHOTROPICYIELDCRITERION_IXX

namespace tfel {

  namespace material {

    template <typename StressStensor>
    Cazacu2004StressType<StressStensor>
    computeCazacu2004OrthotropicStressCriterion(
        const StressStensor& sig,
        const J2OCoefficients<StressStensor>& a,
        const J3OCoefficients<StressStensor>& b,
        const Cazacu2004BaseType<StressStensor> c) {
      const auto J2O = computeJ2O(sig, a);
      const auto J3O = computeJ3O(sig, b);
      return std::cbrt(tfel::math::power<3>(sqrt(J2O)) - c * J3O);
    }  // end of computeCazacu2004OrthotropicStressCriterion

    template <typename StressStensor>
    std::tuple<Cazacu2004StressType<StressStensor>,
               Cazacu2004StressNormalType<StressStensor>>
    computeCazacu2004OrthotropicStressCriterionNormal(
        const StressStensor& sig,
        const J2OCoefficients<StressStensor>& a,
        const J3OCoefficients<StressStensor>& b,
        const Cazacu2004BaseType<StressStensor> c,
        const Cazacu2004StressType<StressStensor> seps) {
      const auto J2O = computeJ2O(sig, a);
      const auto J3O = computeJ3O(sig, b);
      const auto seq = std::cbrt(tfel::math::power<3>(sqrt(J2O)) - c * J3O);
      const auto iseq = 1 / std::max(seq, seps);
      const auto dJ2O = computeJ2ODerivative(sig, a);
      const auto dJ3O = computeJ3ODerivative(sig, b);
      // the normal is written n = nb/(seq*seq)
      const auto nb = (std::sqrt(J2O) / 2) * dJ2O - (c / 3) * dJ3O;
      return std::make_tuple(seq, tfel::math::eval(iseq * iseq * nb));
    }  // end of computeCazacu2004OrthotropicStressCriterion

    template <typename StressStensor>
    std::tuple<Cazacu2004StressType<StressStensor>,
               Cazacu2004StressNormalType<StressStensor>,
               Cazacu2004StressSecondDerivativeType<StressStensor>>
    computeCazacu2004OrthotropicStressCriterionSecondDerivative(
        const StressStensor& sig,
        const J2OCoefficients<StressStensor>& a,
        const J3OCoefficients<StressStensor>& b,
        const Cazacu2004BaseType<StressStensor> c,
        const Cazacu2004StressType<StressStensor> seps) {
      const auto J2O = computeJ2O(sig, a);
      const auto J3O = computeJ3O(sig, b);
      const auto seq = std::cbrt(tfel::math::power<3>(sqrt(J2O)) - c * J3O);
      const auto iseq = 1 / std::max(seq, seps);
      const auto dJ2O = computeJ2ODerivative(sig, a);
      const auto d2J2O = computeJ2OSecondDerivative(sig, a);
      const auto dJ3O = computeJ3ODerivative(sig, b);
      const auto d2J3O = computeJ3OSecondDerivative(sig, b);
      // the normal is written n = nb/(seq*seq)
      const auto nb = (std::sqrt(J2O) / 2) * dJ2O - (c / 3) * dJ3O;
      const auto dnb = 1 / (4 * std::sqrt(J2O)) * (dJ2O ^ dJ2O) +
                       (std::sqrt(J2O) / 2) * d2J2O - (c / 3) * d2J3O;
      const auto n = tfel::math::eval(iseq * iseq * nb);
      return std::make_tuple(
          seq, n, tfel::math::eval(iseq * (-2 * (n ^ n) + iseq * dnb)));
    }  // end of computeCazacu2004OrthotropicStressCriterionSecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_CAZACU2004ORTHOTROPICYIELDCRITERION_IXX */
