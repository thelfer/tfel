/*!
 * \file  include/TFEL/Math/Array/RuntimeIndexingPolicies.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_RUNTIMEINDEXINGPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_RUNTIMEINDEXINGPOLICIES_HXX

#include "TFEL/ContractViolation.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"

namespace tfel::math {

  /*!
   * \brief an indexing policy suitable for vectors whose size is given at
   * runtime.
   * \tparam Type: type used for indexing
   */
  struct RuntimeVectorIndexingPolicy {
    //!
    static constexpr const auto hasFixedSizes = false;
    //!
    using size_type = std::size_t;
    //!
    using RowMajorIndexingPolicy = RuntimeVectorIndexingPolicy;
    //!
    static constexpr const size_type arity = 1;
    //!
    static constexpr const auto areDataContiguous = true;
    //!
    static constexpr const auto unRollLoop = false;
    //! \brief constructor
    constexpr RuntimeVectorIndexingPolicy(const size_type s) : data_size(s) {}
    //! \brief move constructor
    constexpr RuntimeVectorIndexingPolicy(RuntimeVectorIndexingPolicy&&) =
        default;
    //! \brief copy constructor
    constexpr RuntimeVectorIndexingPolicy(const RuntimeVectorIndexingPolicy&) =
        default;
    //! \brief move constructor
    constexpr RuntimeVectorIndexingPolicy& operator=(
        RuntimeVectorIndexingPolicy&&) = default;
    //! \brief copy constructor
    constexpr RuntimeVectorIndexingPolicy& operator=(
        const RuntimeVectorIndexingPolicy&) = default;
    //! \return the minimal data size
    constexpr size_type getUnderlyingArrayMinimalSize() const noexcept {
      return this->size();
    }
    //!
    constexpr size_type getIndex(const size_type i) const noexcept { return i; }
    /*!
     * \return the logical size of the array
     */
    constexpr size_type size() const noexcept { return this->data_size; }
    /*!
     * \return the logical size of the array for the given dimension
     * \param[in] i: dimension
     */
    constexpr size_type size(const size_type i) const noexcept {
      if (i != 0) {
        tfel::reportContractViolation(
            "RuntimeVectorIndexingPolicy: invalid dimension");
      }
      return this->data_size;
    }
    //!
    constexpr RuntimeVectorIndexingPolicy getRowMajorIndexingPolicy() const {
      return RuntimeVectorIndexingPolicy(this->data_size);
    }

   private:
    size_type data_size;
  };

  /*!
   * \brief partial specialisation of the ``IndexingPoliciesCompatiblityCheck`
   * class for `RuntimeVectorIndexingPolicy` of the same size.
   */
  template <>
  struct IndexingPoliciesCompatiblityCheck<RuntimeVectorIndexingPolicy,
                                           RuntimeVectorIndexingPolicy>
      : public IndexingPoliciesCompatiblityCheckBase<
            RuntimeVectorIndexingPolicy,
            RuntimeVectorIndexingPolicy> {
    static constexpr bool haveTheSameMemoryLayout() {
      return true;
    }  // end of haveTheSameMemoryLayout
  };   // end of struct IndexingPolicyCompatiblityCheck

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_RUNTIMEINDEXINGPOLICIES_HXX */
