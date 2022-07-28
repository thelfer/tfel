/*!
 * \file   include/TFEL/Math/LinearInterpolation.ixx
 * \brief
 * \author Thomas Helfer
 * \date   27/07/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LINEARINTERPOLATION_IXX
#define LIB_TFEL_MATH_LINEARINTERPOLATION_IXX

#include <utility>
#include "TFEL/ContractViolation.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ResultType.hxx"

namespace tfel::math {

  namespace linear_interpolation_internals {

    template <typename AbscissaContainer, typename ValueContainer>
    TFEL_HOST_DEVICE constexpr void makeChecks(
        const AbscissaContainer& abscissae, const ValueContainer& values) {
      if (abscissae.empty()) {
        tfel::reportContractViolation(
            "computeLinearInterpolation: empty abscissae");
      }
      if (abscissae.size() != values.size()) {
        tfel::reportContractViolation(
            "computeLinearInterpolation: unmatched abscissae and value size");
      }
    }  // end of makeChecks

    template <typename AbscissaContainer, typename AbscissaType>
    TFEL_HOST_DEVICE constexpr auto findIndex(
        const AbscissaContainer& abscissae, const AbscissaType a) {
      const auto s = abscissae.size();
      auto i = decltype(s){};
      while (i + 1 != s) {
        if (a <= abscissae[i + 1]) {
          break;
        }
        ++i;
      }
      return i;
    }  // end of findIndex

  }  // end of namespace linear_interpolation_internals

  template <bool extrapolate,
            typename AbscissaContainer,
            typename ValueContainer,
            typename AbscissaType>
  constexpr auto computeLinearInterpolation(const AbscissaContainer& abscissae,
                                            const ValueContainer& values,
                                            const AbscissaType a) {
    auto interpolate = [&abscissae, &values, a](const auto i) {
      const auto ai = abscissae[i];
      const auto vi = values[i];
      const auto d = (values[i + 1] - vi) / (abscissae[i + 1] - ai);
      return vi + d * (a - ai);
    };
    linear_interpolation_internals::makeChecks(abscissae, values);
    if (abscissae.size() == 1u) {
      return values[0];
    }
    if (a <= abscissae[0]) {
      if constexpr (extrapolate) {
        return interpolate(0);
      } else {
        return values[0];
      }
    }
    if (a >= abscissae.back()) {
      if constexpr (extrapolate) {
        return interpolate(abscissae.size() - 2);
      } else {
        return values.back();
      }
    }
    const auto i = linear_interpolation_internals::findIndex(abscissae, a);
    return interpolate(i);
  }  // end of computeLinearInterpolation

  template <bool extrapolate,
            typename AbscissaContainer,
            typename ValueContainer,
            typename AbscissaType>
  constexpr auto computeLinearInterpolationAndDerivative(
      const AbscissaContainer& abscissae,
      const ValueContainer& values,
      const AbscissaType a) {
    using abscissa_type =
        result_type<AbscissaType, typename AbscissaContainer::value_type,
                    OpMinus>;
    using value_type = typename AbscissaContainer::value_type;
    auto interpolate = [&abscissae, &values, a](const auto i) {
      const auto ai = abscissae[i];
      const auto vi = values[i];
      const auto d = (values[i + 1] - vi) / (abscissae[i + 1] - ai);
      return std::make_pair(vi + d * (a - ai), d);
    };
    linear_interpolation_internals::makeChecks(abscissae, values);
    if (abscissae.size() == 1u){
      return std::make_pair(values[0], value_type{0} / (abscissa_type{1}));
    }
    if (a <= abscissae[0]) {
      if constexpr (extrapolate) {
       return interpolate(0);
      } else {
        return std::make_pair(values[0], value_type{0} / (abscissa_type{1}));
      }
    }
    if (a >= abscissae.back()) {
     if constexpr (extrapolate) {
       return interpolate(abscissae.size() - 2);
     } else {
       return std::make_pair(values.back(), value_type{0} / (abscissa_type{1}));
     }
    }
    const auto i = linear_interpolation_internals::findIndex(abscissae, a);
    return interpolate(i);
  }  // end of computeLinearInterpolation

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_LINEARINTERPOLATION_IXX */
