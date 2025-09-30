/*!
 * \file   include/TFEL/Math/General/DerivativeType.ixx
 * \brief
 * \author Thomas Helfer
 * \date   24/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DERIVATIVETYPE_IXX
#define LIB_TFEL_MATH_DERIVATIVETYPE_IXX

#include "TFEL/TypeTraits/IsScalar.hxx"

namespace tfel::math {

  template <typename FunctionType, typename VariableType, typename ScalarType>
  derivative_type<std::invoke_result_t<FunctionType, VariableType>,
                  VariableType>
  computeNumericalDerivative(const FunctionType& f,
                             const VariableType& v,
                             const ScalarType& e) {
    using ResultType = std::invoke_result_t<FunctionType, VariableType>;
    static_assert(isScalar<ScalarType>(), "invalid third argument");
    if constexpr (isScalar<VariableType>()) {
      const auto fp = f(v + e);
      const auto fm = f(v - e);
      return (fp - fm) / (2 * e);
    } else if constexpr (isScalar<ResultType>()) {
      using VariableTypeIndexingPolicy = typename VariableType::indexing_policy;
      static_assert(VariableTypeIndexingPolicy::arity == 1u,
                    "unsupported case");
      using size_type = typename VariableTypeIndexingPolicy::size_type;
      auto d = derivative_type<ResultType, VariableType>{};
      for (size_type i = 0; i != v.size(); ++i) {
        auto vp = v;
        vp[i] += e;
        const auto fp = f(vp);
        auto vm = v;
        vm[i] -= e;
        const auto fm = f(vm);
        d[i] = (fp - fm) / (2 * e);
      }
      return d;
    } else {
      using ResultTypeIndexingPolicy = typename ResultType::indexing_policy;
      using VariableTypeIndexingPolicy = typename VariableType::indexing_policy;
      static_assert(((ResultTypeIndexingPolicy::arity == 1u) &&
                     (VariableTypeIndexingPolicy::arity == 1u)),
                    "unsupported case");
      using size_type_r = typename ResultTypeIndexingPolicy::size_type;
      using size_type_v = typename VariableTypeIndexingPolicy::size_type;
      auto d = derivative_type<ResultType, VariableType>{};
      for (size_type_v i = 0; i != v.size(); ++i) {
        auto vp = v;
        vp[i] += e;
        const auto fp = f(vp);
        auto vm = v;
        vm[i] -= e;
        const auto fm = f(vm);
        for (size_type_r j = 0; j != d.size(0); ++j) {
          d(j, i) = (fp[j] - fm[j]) / (2 * e);
        }
      }
      return d;
    }
  }  // end of computeNumericalDerivative

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_DERIVATIVETYPE_IXX */
