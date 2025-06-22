/*!
 * \file  include/TFEL/Math/Array/IndexingPolicies.hixx
 * \brief
 * \author Thomas Helfer
 * \date 02/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_IXX
#define LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_IXX

#include "TFEL/ContractViolation.hxx"

namespace tfel::math {

  namespace internals {

    template <typename T>
    struct tag {
      using type = T;
    };

    template <typename... Ts>
    using select_last = typename decltype((tag<Ts>{}, ...))::type;

    template <typename T, typename... Args>
    constexpr bool isLastArgumentConvertibleTo() {
      constexpr auto n = sizeof...(Args);
      if constexpr (n == 0) {
        return false;
      } else {
        using last_type = select_last<Args...>;
        return std::is_convertible_v<last_type, T>;
      }
    }  // end of isLastArgumentOfType

    template <typename T, typename = void>
    struct HasIndexingPolicyTraits : std::false_type {};

    template <typename T>
    struct HasIndexingPolicyTraits<T, std::void_t<typename T::indexing_policy>>
        : std::true_type {};

  }  // end of namespace internals

  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool IndexingPoliciesCompatiblityCheckBase<
      IndexingPolicy1,
      IndexingPolicy2>::performCompileTimeCheck() {
    if constexpr (IndexingPolicy1::arity != IndexingPolicy2::arity) {
      return false;
    }
    if constexpr (IndexingPolicy1::hasFixedSizes &&
                  IndexingPolicy2::hasFixedSizes) {
      IndexingPolicy1 p1;
      IndexingPolicy2 p2;
      using size_type = std::common_type_t<typename IndexingPolicy1::size_type,
                                           typename IndexingPolicy2::size_type>;
      for (size_type i = 0; i != IndexingPolicy1::arity; ++i) {
        if (p1.size(i) != p2.size(i)) {
          return false;
        }
      }
      return true;
    } else {
      return true;
    }
  }  // end of performCompileTimeCheck

  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool IndexingPoliciesCompatiblityCheckBase<
      IndexingPolicy1,
      IndexingPolicy2>::haveTheSameMemoryLayout() {
    static_assert(
        IndexingPoliciesCompatiblityCheckBase::performCompileTimeCheck(),
        "invalid call");
    return true;
  }  // end of haveTheSameMemoryLayout

  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool IndexingPoliciesCompatiblityCheckBase<
      IndexingPolicy1,
      IndexingPolicy2>::performRuntimeTimeCheck(const IndexingPolicy1& p1,
                                                const IndexingPolicy2& p2) {
    static_assert(
        IndexingPoliciesCompatiblityCheckBase::performCompileTimeCheck(),
        "invalid call");
    if constexpr (IndexingPolicy1::hasFixedSizes &&
                  IndexingPolicy2::hasFixedSizes) {
      return true;
    } else {
      using size_type = std::common_type_t<typename IndexingPolicy1::size_type,
                                           typename IndexingPolicy2::size_type>;
      for (size_type i = 0; i != IndexingPolicy1::arity; ++i) {
        if (p1.size(i) != p2.size(i)) {
          return false;
        }
      }
      return true;
    }
  }  // end of performRuntimeTimeCheck

  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool checkIndexingPoliciesCompatiblity() {
    return IndexingPoliciesCompatiblityCheck<
        IndexingPolicy1, IndexingPolicy2>::performCompileTimeCheck();
  }  // end of checkIndexingPoliciesCompatiblity

  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool haveIndexingPoliciesTheSameMemoryLayout() {
    return IndexingPoliciesCompatiblityCheck<
        IndexingPolicy1, IndexingPolicy2>::haveTheSameMemoryLayout();
  }  // end of haveIndexingPoliciesTheSameMemoryLayout

  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool areIndexingPoliciesCompatibleAtRunTime(
      const IndexingPolicy1& p1, const IndexingPolicy2& p2) {
    return IndexingPoliciesCompatiblityCheck<
        IndexingPolicy1, IndexingPolicy2>::performRuntimeTimeCheck(p1, p2);
  }  // end of areIndexingPoliciesAreCompatibleAtRunTime

  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr void checkIndexingPoliciesRuntimeCompatiblity(
      const IndexingPolicy1& p1, const IndexingPolicy2& p2) {
    static_assert(
        checkIndexingPoliciesCompatiblity<IndexingPolicy1, IndexingPolicy2>(),
        "invalid call");
    if (!areIndexingPoliciesCompatibleAtRunTime(p1, p2)) {
      tfel::reportContractViolation(
          "checkIndexingPoliciesRuntimeCompatiblity: "
          "given policies are not compatible");
    }
  }  // end of checkIndexingPoliciesRuntimeCompatiblity

  template <typename IndexingPolicy, typename... T, std::size_t... I>
  constexpr auto buildIndexingPolicyFromTuple(
      const std::tuple<T...>& t, const std::index_sequence<I...>&) {
    return IndexingPolicy(std::get<I>(t)...);
  }

  template <typename IndexingPolicy, typename... Args>
  constexpr auto buildIndexingPolicyAndExtractPointerToData(Args&... args) {
    constexpr auto n = sizeof...(Args);
    static_assert(n >= 2, "invalid call");
    const auto args_tuple = std::make_tuple(args...);
    const auto p = buildIndexingPolicyFromTuple<IndexingPolicy>(
        args_tuple, std::make_index_sequence<sizeof...(Args) - 1>());
    const auto data_values = std::get<n - 1>(args_tuple);
    return std::make_tuple(p, data_values);
  }

  template <typename T>
  constexpr bool hasIndexingPolicy() {
    return tfel::math::internals::HasIndexingPolicyTraits<
        std::remove_cv_t<T>>::value;
  }  // end of hasIndexingPolicy

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_IXX */
