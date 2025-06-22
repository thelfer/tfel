/*!
 * \file   include/TFEL/Material/__StressCriterionName__StressCriterion.ixx
 * \brief
 * \author __Author__
 * \date   __Date__
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL___STRESS_CRITERION_NAME___IXX
#define LIB_TFEL_MATERIAL___STRESS_CRITERION_NAME___IXX

#include <cmath>
#include <ostream>
#include <algorithm>

namespace tfel {

  namespace material {

    template <typename StressStensor>
    std::ostream& operator<<(
        std::ostream& os,
        const __StressCriterionName__StressCriterionParameters<StressStensor>&
            p) {
      return os;
    }  // end of operator <<

    template <typename StressStensor>
    __StressCriterionName__StressType<StressStensor>
    compute__StressCriterionName__Stress(
        const StressStensor& sig,
        const __StressCriterionName__StressCriterionParameters<StressStensor>&
            p,
        const __StressCriterionName__StressType<StressStensor> seps) {
      using stress = __StressCriterionName__StressType<StressStensor>;
      using real = __StressCriterionName__BaseType<StressStensor>;
      return stress{};
    }  // end of compute__StressCriterionName__YieldStress

    template <typename StressStensor>
    std::tuple<__StressCriterionName__StressType<StressStensor>,
               __StressCriterionName__StressNormalType<StressStensor>>
    compute__StressCriterionName__StressNormal(
        const StressStensor& sig,
        const __StressCriterionName__StressCriterionParameters<StressStensor>&
            p,
        const __StressCriterionName__StressType<StressStensor> seps) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using stress = __StressCriterionName__StressType<StressStensor>;
      using real = __StressCriterionName__BaseType<StressStensor>;
      using istress = tfel::math::result_type<real, stress, tfel::math::OpDiv>;
      using normal = __StressCriterionName__StressNormalType<StressStensor>;
      return {stress{}, normal{}};
    }  // end of compute__StressCriterionName__StressNormal

    template <typename StressStensor>
    std::tuple<__StressCriterionName__StressType<StressStensor>,
               __StressCriterionName__StressNormalType<StressStensor>,
               __StressCriterionName__StressSecondDerivativeType<StressStensor>>
    compute__StressCriterionName__StressSecondDerivative(
        const StressStensor& sig,
        const __StressCriterionName__StressCriterionParameters<StressStensor>&
            p,
        const __StressCriterionName__StressType<StressStensor> seps) {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using stress = __StressCriterionName__StressType<StressStensor>;
      using real = __StressCriterionName__BaseType<StressStensor>;
      using istress = tfel::math::result_type<real, stress, tfel::math::OpDiv>;
      using normal = __StressCriterionName__StressNormalType<StressStensor>;
      using normal_derivative =
          __StressCriterionName__StressSecondDerivativeType<StressStensor>;
      return {stress{}, normal{}, normal_derivative{}};
    }  // end of compute__StressCriterionName__SecondDerivative

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL___STRESS_CRITERION_NAME___IXX */
