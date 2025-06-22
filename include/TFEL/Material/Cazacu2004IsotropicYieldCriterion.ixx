/*!
 * \file   Cazacu2004IsotropicYieldCriterion.ixx
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

#ifndef LIB_TFEL_MATERIAL_CAZACU2004ISOTROPICYIELDCRITERION_IXX
#define LIB_TFEL_MATERIAL_CAZACU2004ISOTROPICYIELDCRITERION_IXX

#include <cmath>

namespace tfel {

  namespace material {

    template <typename StressStensor>
    Cazacu2004StressType<StressStensor>
    computeCazacu2004IsotropicStressCriterion(
        const StressStensor& sig, const Cazacu2004BaseType<StressStensor> c) {
      const auto s = tfel::math::deviator(sig);
      const auto J2 = (s | s) / 2;
      const auto J3 = det(s);
      return std::cbrt(tfel::math::power<3>(sqrt(J2)) - c * J3);
    }  // end of computeCazacu2004IsotropicStressCriterion

    template <typename StressStensor>
    std::tuple<Cazacu2004StressType<StressStensor>,
               Cazacu2004StressNormalType<StressStensor>>
    computeCazacu2004IsotropicStressCriterionNormal(
        const StressStensor& sig,
        const Cazacu2004BaseType<StressStensor> c,
        const Cazacu2004StressType<StressStensor> seps) {
      const auto s = tfel::math::deviator(sig);
      const auto J2 = (s | s) / 2;
      const auto J3 = tfel::math::det(s);
      const auto seq = std::cbrt(tfel::math::power<3>(std::sqrt(J2)) - c * J3);
      const auto iseq = 1 / std::max(seq, seps);
      // the derivative of J2 is s
      const auto dJ3 = tfel::math::computeDeviatorDeterminantDerivative(sig);
      // the normal is written n = nb/(seq*seq)
      const auto nb = (std::sqrt(J2) / 2) * s - (c / 3) * dJ3;
      return std::make_tuple(seq, tfel::math::eval(iseq * iseq * nb));
    }  // end of computeCazacu2004IsotropicStressCriterionNormal

    template <typename StressStensor>
    std::tuple<Cazacu2004StressType<StressStensor>,
               Cazacu2004StressNormalType<StressStensor>,
               Cazacu2004StressSecondDerivativeType<StressStensor>>
    computeCazacu2004IsotropicStressCriterionSecondDerivative(
        const StressStensor& sig,
        const Cazacu2004BaseType<StressStensor> c,
        const Cazacu2004StressType<StressStensor> seps) {
      using real = Cazacu2004BaseType<StressStensor>;
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      const auto s = tfel::math::deviator(sig);
      const auto J2 = (s | s) / 2;
      const auto J3 = tfel::math::det(s);
      const auto seq = std::cbrt(tfel::math::power<3>(std::sqrt(J2)) - c * J3);
      const auto iseq = 1 / std::max(seq, seps);
      // the derivative of J2 is s
      const auto d2J2 = tfel::math::st2tost2<N, real>::K();
      const auto dJ3 = tfel::math::computeDeviatorDeterminantDerivative(sig);
      const auto d2J3 =
          tfel::math::computeDeviatorDeterminantSecondDerivative(sig);
      // the normal is written n = nb/(seq*seq)
      const auto nb = (std::sqrt(J2) / 2) * s - (c / 3) * dJ3;
      const auto dnb = 1 / (4 * std::sqrt(J2)) * (s ^ s) +
                       (std::sqrt(J2) / 2) * d2J2 - (c / 3) * d2J3;
      const auto n = tfel::math::eval(iseq * iseq * nb);
      return std::make_tuple(
          seq, n, tfel::math::eval(iseq * (-2 * (n ^ n) + iseq * dnb)));
    }  // end of computeCazacu2004IsotropicStressCriterionSecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_CAZACU2004ISOTROPICYIELDCRITERION_IXX */
