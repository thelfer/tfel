/*!
 * \file  include/TFEL/Math/Array/IndexingPolicies.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_HXX

#include <tuple>
#include <utility>
#include <type_traits>

namespace tfel::math {

  /*!
   * \brief a simple function to check that the type of the indices are
   * compatible with the given indexing policy
   * \tparam IndexingPolicy: the indexing policy
   * \tparam Indices: list of indices types
   */
  template <typename IndexingPolicy, typename... Indices>
  TFEL_HOST_DEVICE constexpr void checkIndicesValiditity() noexcept {
    static_assert(sizeof...(Indices) == IndexingPolicy::arity,
                  "invalid number of indices");
    static_assert(
        (std::is_convertible_v<Indices, typename IndexingPolicy::size_type> &&
         ...) &&
            (std::is_integral_v<Indices> && ...),
        "invalid indices type");
  }  // end of checkIndicesValidity

  /*!
   * \brief a base class to compare indexing policies which provide default
   * implementations.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note the default implementations are very conservative. For example,
   * the memory layout are reported to be different by default.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  struct IndexingPoliciesCompatiblityCheckBase {
    /*!
     * \return if the given indexing policies may *a priori* be compatible.
     */
    TFEL_HOST_DEVICE static constexpr bool performCompileTimeCheck();
    /*!
     * \return if the indexing policies have the same memory layouts.
     *
     * Two indexing policies can be compatible (at compile-time and at runtime),
     * i.e. one may transfer data from one indexing policy to another, but the
     * memory layout of the data may differ. For example, one may assign a
     * matrix stored in column-major format to a matrix having a row-major
     * format.
     */
    TFEL_HOST_DEVICE static constexpr bool haveTheSameMemoryLayout();
    /*!
     * \return if the given policies are compatible at runtime time.
     * \pre this method is only meaningful if the `compileTimeCheck` method
     * returns `true`.
     * \param[in] p1: first indexing policy
     * \param[in] p2: second indexing policy
     * \note This methods does nothing if both indexing policies have fixed size
     * (i.e. known at compile-time).
     */
    TFEL_HOST_DEVICE static constexpr bool performRuntimeTimeCheck(
        const IndexingPolicy1&, const IndexingPolicy2&);
  };  // end of struct IndexingPoliciesCompatiblityCheck

  /*!
   * \brief a structure used to compare two indexing policies
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note this structure is meant to be specialised in specific cases.
   * \note this structure is no meant to be used directly in most cases and the
   * use the for `checkIndexingPoliciesCompatiblity`,
   * `checkIndexingPoliciesRuntimeCompatiblity`,
   * `haveIndexingPoliciesTheSameMemoryLayout` helper functions.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  struct IndexingPoliciesCompatiblityCheck
      : IndexingPoliciesCompatiblityCheckBase<IndexingPolicy1,
                                              IndexingPolicy2> {
  };  // end of struct IndexingPoliciesCompatiblityCheck

  /*!
   * \brief check if two indexing policies can be compatible.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note this function is a simple wrapper around
   * `IndexingPoliciesCompatiblityCheck::checkIndexingPoliciesCompatiblity`.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  TFEL_HOST_DEVICE constexpr bool checkIndexingPoliciesCompatiblity();
  /*!
   * \return if two indexing policies have the same memory layout.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note this function is a simple wrapper around
   * `IndexingPoliciesCompatiblityCheck::haveTheSameMemoryLayout`.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  TFEL_HOST_DEVICE constexpr bool haveIndexingPoliciesTheSameMemoryLayout();
  /*!
   * \return if two indexing policies are compatible at runtime.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \param[in] p1: first indexing policy
   * \param[in] p2: second indexing policy
   * \note this function is a simple wrapper around
   * `IndexingPoliciesCompatiblityCheck::runtimeTimeCheck`.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  TFEL_HOST_DEVICE constexpr bool areIndexingPoliciesCompatibleAtRunTime(
      const IndexingPolicy1&, const IndexingPolicy2&);

  /*!
   * \brief check if two indexing policies are compatible at runtime.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \param[in] p1: first indexing policy
   * \param[in] p2: second indexing policy
   * \note this function simply calls
   * `areIndexingPoliciesCompatibleAtRunTime` and reports a contract violation
   * if the two indexing policies are not compatible.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  TFEL_HOST_DEVICE constexpr void checkIndexingPoliciesRuntimeCompatiblity(
      const IndexingPolicy1&, const IndexingPolicy2&);

  template <typename IndexingPolicy, typename... T, std::size_t... I>
  constexpr auto buildIndexingPolicyFromTuple(const std::tuple<T...>&,
                                              const std::index_sequence<I...>&);

  template <typename IndexingPolicy, typename... Args>
  constexpr auto buildIndexingPolicyAndExtractPointerToData(Args&... args);

  /*!
   * \brief a simple metafunction which looks if the given type provides an
   * internal type called `indexing_policy`
   * \param[in] T: type checked
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr bool hasIndexingPolicy();

}  // end of namespace tfel::math

#include "TFEL/Math/Array/IndexingPolicies.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_HXX */
