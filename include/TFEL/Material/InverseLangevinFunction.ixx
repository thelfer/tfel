/*!
 * \file   InverseLangevinFunction.ixx
 * \brief
 * \author Thomas Helfer
 * \date   08/09/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_INVERSELANGEVINFUNCTION_IXX
#define LIB_TFEL_MATERIAL_INVERSELANGEVINFUNCTION_IXX

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel::material::internals {

  /*!
   * \brief template class in charge of computing one of the approximation of
   * the inverse Langevin function.
   *
   * \tparam Approximation: approximation selected
   * \tparam NumericType: numeric type
   */
  template <tfel::material::InverseLangevinFunctionApproximations,
            typename NumericType>
  struct InverseLangevinFunction;

  /*!
   * \brief template class in charge of computing one of the approximation of
   * the inverse Langevin function following the paper of A. Cohen.
   *
   * Cohen, A. A Padé approximant to the inverse
   * Langevin function.
   * Rheol Acta 30, 270–273 (1991).
   * https://doi.org/10.1007/BF00366640
   *
   * \tparam NumericType: numeric type
   */
  template <typename NumericType>
  struct InverseLangevinFunction<
      tfel::material::InverseLangevinFunctionApproximations::COHEN_1991,
      NumericType> {
    /*!
     * \brief compute the approximation of the inverse of the Langenvin function
     * \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr NumericType computeFunction(
        const NumericType y) {
      const auto y2 = y * y;
      return y * (3 - y2) / (1 - y2);
    }  // end of computeFunction
    /*!
     * \brief compute the approximation of the inverse of the Langenvin function
     * and its derivative \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr std::pair<NumericType, NumericType>
    computeFunctionAndDerivative(const NumericType y) {
      const auto y2 = y * y;
      const auto y4 = y2 * y2;
      const auto id = 1 / (1 - y2);
      return {y * (3 - y2) * id, (y4 + 3) * id * id};
    }  // end of computeFunctionAndDerivative
  };

  /*!
   * \brief template class in charge of computing one of the approximation of
   * the inverse Langevin function following the paper of Jedynak.
   *
   * Jedynak, R. Approximation of the inverse Langevin
   * function revisited.
   * Rheol Acta 54, 29–39 (2015).
   * https://doi.org/10.1007/s00397-014-0802-2
   *
   * \tparam NumericType: numeric type
   * \param[in] y: argument
   */
  template <typename NumericType>
  struct InverseLangevinFunction<
      tfel::material::InverseLangevinFunctionApproximations::JEDYNAK_2015,
      NumericType> {
    static constexpr auto c0 = NumericType{2.99942};
    static constexpr auto c1 = NumericType{-2.57332};
    static constexpr auto c2 = NumericType{0.654805};
    static constexpr auto d1 = NumericType{-0.894936};
    static constexpr auto d2 = NumericType{-0.105064};
    /*!
     * \brief compute the approximation of the inverse of the Langenvin function
     * \tparam NumericType: numeric type
     * \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr NumericType computeFunction(
        const NumericType y) {
      const auto y2 = y * y;
      return y * (c0 + c1 * y + c2 * y2) / (1 + d1 * y + d2 * y2);
    }  // end of computeFunction
    /*!
     * \brief compute the approximation of the inverse of the Langenvin function
     * and its derivative \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr std::pair<NumericType, NumericType>
    computeFunctionAndDerivative(const NumericType y) {
      const auto y2 = y * y;
      const auto y3 = y2 * y;
      const auto y4 = y2 * y2;
      const auto id = 1 / (1 + d1 * y + d2 * y2);
      return {y * (c0 + c1 * y + c2 * y2) * id,
              (c2 * d2 * y4 + 2 * c2 * d1 * y3 +
               (-c0 * d2 + c1 * d1 + 3 * c2) * y2 + 2 * c1 * y + c0) *
                  id * id};
    }  // end of computeFunctionAndDerivative
  };

  /*!
   * \brief template class in charge of computing one of the approximation of
   * the inverse Langevin function using Taylor expansion.
   *
   * \tparam NumericType: numeric type
   */
  template <typename NumericType>
  struct InverseLangevinFunction<
      tfel::material::InverseLangevinFunctionApproximations::MORCH_2022,
      NumericType> {
    static constexpr auto c0 = NumericType{3};
    static constexpr auto c1 = NumericType{1.8};
    static constexpr auto c2 = NumericType{1.6971428571428571};
    static constexpr auto c3 = NumericType{1.7588571428571429};
    static constexpr auto c4 = NumericType{1.8718664192949908};
    static constexpr auto c5 = NumericType{1.9972456800342515};
    static constexpr auto c6 = NumericType{2.1128236517767944};
    static constexpr auto c7 = NumericType{2.2023028715016078};
    static constexpr auto c8 = NumericType{2.2529578706640487};
    static constexpr auto c9 = NumericType{2.2557674927967937};
    /*!
     * \brief compute the approximation of the inverse of the Langenvin function
     * \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr NumericType computeFunction(
        const NumericType y) {
      return y * computeP(y);
    }  // end of computeFunction
    /*!
     * \brief compute the approximation of the inverse of the Langenvin function
     * and its derivative \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr std::pair<NumericType, NumericType>
    computeFunctionAndDerivative(const NumericType y) {
      const auto P = computeP(y);
      const auto dP = computePDerivative(y);
      return {y * P, P + y * dP};
    }  // end of computeFunctionAndDerivative
   private:
    /*!
     * \brief an helper function computing an auxiliary polynomial.
     * \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr NumericType computeP(
        const NumericType y) {
      const auto y2 = tfel::math::power<2>(y);
      return ((((((((c9 * y2 + c8) * y2 + c7) * y2 + c6) * y2 + c5) * y2 + c4) *
                    y2 +
                c3) *
                   y2 +
               c2) *
                  y2 +
              c1) *
                 y2 +
             c0;
    }  // end of computeP
    /*!
     * \brief an helper function computing the derivative of the auxiliary
     * polynomial.
     * \param[in] y: argument
     */
    TFEL_HOST_DEVICE static constexpr NumericType computePDerivative(
        const NumericType y) {
      const auto y2 = tfel::math::power<2>(y);
      constexpr auto d9 = 9 * c9;
      constexpr auto d8 = 8 * c8;
      constexpr auto d7 = 7 * c7;
      constexpr auto d6 = 6 * c6;
      constexpr auto d5 = 5 * c5;
      constexpr auto d4 = 4 * c4;
      constexpr auto d3 = 3 * c3;
      constexpr auto d2 = 2 * c2;
      constexpr auto d1 = c1;
      return ((((((((d9 * y2 + d8) * y2 + d7) * y2 + d6) * y2 + d5) * y2 + d4) *
                    y2 +
                d3) *
                   y2 +
               d2) *
                  y2 +
              d1) *
             2 * y;
    }  // end of computePDerivative
  };

}  // namespace tfel::material::internals

namespace tfel::material {

  template <InverseLangevinFunctionApproximations Approximation,
            typename NumericType>
  TFEL_HOST_DEVICE constexpr NumericType
  computeApproximateInverseLangevinFunction(const NumericType y) {
    if constexpr (Approximation ==
                  InverseLangevinFunctionApproximations::KUHN_GRUN_1942) {
      return tfel::material::internals::InverseLangevinFunction<
          InverseLangevinFunctionApproximations::MORCH_2022,
          NumericType>::computeFunction(y);
    } else {
      return tfel::material::internals::InverseLangevinFunction<
          Approximation, NumericType>::computeFunction(y);
    }
  }  // end of computeApproximateInverseLangevinFunction

  template <InverseLangevinFunctionApproximations Approximation,
            typename NumericType>
  TFEL_HOST_DEVICE constexpr std::pair<NumericType, NumericType>
  computeApproximateInverseLangevinFunctionAndDerivative(const NumericType y) {
    if constexpr (Approximation ==
                  InverseLangevinFunctionApproximations::KUHN_GRUN_1942) {
      return tfel::material::internals::InverseLangevinFunction<
          InverseLangevinFunctionApproximations::MORCH_2022,
          NumericType>::computeFunctionAndDerivative(y);
    } else {
      return tfel::material::internals::InverseLangevinFunction<
          Approximation, NumericType>::computeFunctionAndDerivative(y);
    }
  }  // end of computeApproximateInverseLangevinFunctionAndDerivative

  template <typename NumericType>
  TFEL_HOST_DEVICE NumericType
  computeBergstromBoyce1998ApproximateInverseLangevinFunction(
      const NumericType y) {
    constexpr auto c0 = NumericType(0.84136);
    constexpr auto c1 = NumericType(1.31446);
    constexpr auto c2 = NumericType(1.58986);
    constexpr auto c3 = NumericType(0.91209);
    const auto ay = tfel::math::abs(y);
    if (ay < c0) {
      return c1 * std::tan(c2 * y) + c3 * y;
    }
    const auto sy = NumericType(y > 0 ? 1 : -1);
    return 1 / (sy - y);
  }  // end of computeBergstromBoyce1998InverseLangevinFunctionApproximation

  template <typename NumericType>
  TFEL_HOST_DEVICE std::pair<NumericType, NumericType>
  computeBergstromBoyce1998ApproximateInverseLangevinFunctionAndDerivative(
      const NumericType y) {
    constexpr auto c0 = NumericType(0.84136);
    constexpr auto c1 = NumericType(1.31446);
    constexpr auto c2 = NumericType(1.58986);
    constexpr auto c3 = NumericType(0.91209);
    const auto ay = tfel::math::abs(y);
    if (ay < c0) {
      return {c1 * std::tan(c2 * y) + c3 * y,
              c1 * c2 / tfel::math::power<2>(cos(c2 * y)) + c3};
    }
    const auto sy = NumericType(y > 0 ? 1 : -1);
    const auto fy = 1 / (sy - y);
    return {fy, fy * fy};
  }  // end of computeBergstromBoyce1998InverseLangevinFunctionApproximation

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_INVERSELANGEVINFUNCTION_IXX */
