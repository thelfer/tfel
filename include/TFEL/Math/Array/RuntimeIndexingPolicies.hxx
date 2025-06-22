/*!
 * \file  include/TFEL/Math/Array/RuntimeIndexingPolicies.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_RUNTIMEINDEXINGPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_RUNTIMEINDEXINGPOLICIES_HXX

#include <array>
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
    static constexpr auto hasFixedSizes = false;
    //!
    using size_type = std::size_t;
    //!
    using RowMajorIndexingPolicy = RuntimeVectorIndexingPolicy;
    //!
    static constexpr size_type arity = 1;
    //!
    static constexpr auto areDataContiguous = true;
    //!
    static constexpr auto unRollLoop = false;
    //! \brief constructor
    constexpr RuntimeVectorIndexingPolicy() : data_size(0) {}
    //! \brief constructor
    constexpr RuntimeVectorIndexingPolicy(const size_type s) : data_size(s) {}
    //! \brief constructor
    constexpr RuntimeVectorIndexingPolicy(const std::array<size_type, 1u>& s)
        : data_size(s[0]) {}
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
    //!
    constexpr size_type getIndex(
        const std::array<size_type, 1u>& i) const noexcept {
      return this->getIndex(i[0]);
    }
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

  /*!
   * \brief an indexing policy suitable for runtime matrices
   */
  struct RuntimeRowMajorMatrixIndexingPolicy {
    //!
    static constexpr auto hasFixedSizes = true;
    //!
    static constexpr auto areDataContiguous = true;
    //!
    static constexpr auto unRollLoop = false;
    //!
    using size_type = std::size_t;
    //!
    static constexpr size_type arity = 2;
    //!
    using RowMajorIndexingPolicy = RuntimeRowMajorMatrixIndexingPolicy;
    //! \brief constructor
    constexpr RuntimeRowMajorMatrixIndexingPolicy() : n_rows(0), n_columns(0) {}
    //! \brief constructor
    constexpr RuntimeRowMajorMatrixIndexingPolicy(const size_type r,
                                                  const size_type c)
        : n_rows(r), n_columns(c) {}
    //! \brief constructor
    constexpr RuntimeRowMajorMatrixIndexingPolicy(
        const std::array<size_type, 2u>& sizes)
        : n_rows(sizes[0]), n_columns(sizes[1]) {}
    //! \brief move constructor
    constexpr RuntimeRowMajorMatrixIndexingPolicy(
        RuntimeRowMajorMatrixIndexingPolicy&&) = default;
    //! \brief copy constructor
    constexpr RuntimeRowMajorMatrixIndexingPolicy(
        const RuntimeRowMajorMatrixIndexingPolicy&) = default;
    //! \brief move constructor
    constexpr RuntimeRowMajorMatrixIndexingPolicy& operator=(
        RuntimeRowMajorMatrixIndexingPolicy&&) = default;
    //! \brief copy constructor
    constexpr RuntimeRowMajorMatrixIndexingPolicy& operator=(
        const RuntimeRowMajorMatrixIndexingPolicy&) = default;
    //!
    constexpr size_type getIndex(const size_type i,
                                 const size_type j) const noexcept {
      return i * this->n_columns + j;
    }
    //!
    constexpr size_type getIndex(
        const std::array<size_type, 2u> indices) const noexcept {
      return this->getIndex(indices[0], indices[1]);
    }
    //! \return the minimal data size
    constexpr size_type getUnderlyingArrayMinimalSize() const noexcept {
      return this->size();
    }
    //!
    constexpr RuntimeRowMajorMatrixIndexingPolicy getRowMajorIndexingPolicy() {
      return RuntimeRowMajorMatrixIndexingPolicy(*this);
    }  // end of getRowMajorIndexingPolicy
    //!
    constexpr size_type size() const noexcept {
      return this->getNumberOfRows() * this->getNumberOfColumns();
    }
    /*!
     * \return the logical size of the array for the given dimension
     * \param[in] i: dimension
     */
    constexpr size_type size(const size_type i) const noexcept {
      if (i > 1) {
        tfel::reportContractViolation(
            "FixedSizeMatrixIndexingPolicy: "
            "invalid dimension");
      }
      return i == 0 ? this->getNumberOfRows() : this->getNumberOfColumns();
    }
    //! \return the number of rows
    constexpr size_type getNumberOfRows() const noexcept {
      return this->n_rows;
    }
    //! \return the number of cols
    constexpr size_type getNumberOfColumns() const noexcept {
      return this->n_columns;
    }

   private:
    //! \brief number of rows
    size_type n_rows;
    //! \brief number of columns
    size_type n_columns;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_RUNTIMEINDEXINGPOLICIES_HXX */
