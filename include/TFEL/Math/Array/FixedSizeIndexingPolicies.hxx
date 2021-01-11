/*!
 * \file  include/TFEL/Math/Array/FixedSizeIndexingPolicies.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEINDEXINGPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEINDEXINGPOLICIES_HXX

#include "TFEL/ContractViolation.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"

namespace tfel::math {

  /*!
   * \brief an indexing policy suitable for fixed sized vectors
   * \tparam SizeType: type used for indexing
   * \tparam N: logical size of the array
   * \tparam Stride: Stride between successive values
   */
  template <typename SizeType, SizeType N, SizeType Stride = 1>
  struct FixedSizeVectorIndexingPolicy {
    static_assert(std::is_integral_v<SizeType>);
    //!
    static constexpr const auto hasFixedSizes = true;
    //!
    using size_type = SizeType;
    //!
    using RowMajorIndexingPolicy = FixedSizeVectorIndexingPolicy<SizeType, N>;
    //!
    static constexpr const size_type arity = 1;
    //!
    static constexpr const auto areDataContiguous = (Stride == 1);
    //!
    static constexpr const auto unRollLoop = N < 100;
    //! \return the minimal data size
    constexpr size_type getUnderlyingArrayMinimalSize() const noexcept {
      if constexpr (Stride == 1) {
        return this->size();
      } else {
        return (N - 1) * Stride + 1;
      }
    }
    //!
    constexpr size_type getIndex(const size_type i) const noexcept {
      if constexpr (Stride == 1) {
        return i;
      } else {
        return i * Stride;
      }
    }
    /*!
     * \return the logical size of the array
     */
    constexpr size_type size() const noexcept { return N; }
    /*!
     * \return the logical size of the array for the given dimension
     * \param[in] i: dimension
     */
    constexpr size_type size(const size_type i) const noexcept {
      if (i != 0) {
        tfel::reportContractViolation(
            "FixedSizeVectorIndexingPolicy: "
            "invalid dimension");
      }
      return N;
    }
    //! 
    constexpr FixedSizeVectorIndexingPolicy<SizeType, N>
    getRowMajorIndexingPolicy() {
      return FixedSizeVectorIndexingPolicy<SizeType, N>();
    }  // end of getRowMajorIndexingPolicy
  };

  /*!
   * \brief partial specialisation of the ``IndexingPoliciesCompatiblityCheck`
   * class for `FixedSizeVectorIndexingPolicy` of the same size.
   */
  template <typename SizeType, SizeType N, SizeType Stride1, SizeType Stride2>
  struct IndexingPoliciesCompatiblityCheck<
      FixedSizeVectorIndexingPolicy<SizeType, N, Stride1>,
      FixedSizeVectorIndexingPolicy<SizeType, N, Stride2>>
      : public IndexingPoliciesCompatiblityCheckBase<
            FixedSizeVectorIndexingPolicy<SizeType, N, Stride1>,
            FixedSizeVectorIndexingPolicy<SizeType, N, Stride2>> {
    static constexpr bool haveTheSameMemoryLayout() {
      return Stride1 == Stride2;
    }  // end of haveTheSameMemoryLayout
  };   // end of struct IndexingPolicyCompatiblityCheck

  /*!
   * \brief an indexing policy suitable for fixed sized matrices
   * \tparam SizeType: type used for indexing
   * \tparam N: logical number of rows
   * \tparam M: logical number of columns
   * \tparam Stride: Stride between successive lines of the underlying matrix
   */
  template <typename SizeType, SizeType N, SizeType M, SizeType Stride = M>
  struct FixedSizeRowMajorMatrixIndexingPolicy {
    static_assert(std::is_integral_v<SizeType>);
    static_assert(Stride >= M, "invalid stride value");
    //!
    static constexpr const auto hasFixedSizes = true;
    //!
    static constexpr const auto areDataContiguous = (Stride == M);
    //!
    static constexpr const auto unRollLoop = true;
    //!
    using size_type = SizeType;
    //!
    static constexpr const size_type arity = 2;
    //!
    using RowMajorIndexingPolicy =
        FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M>;
    //!
    constexpr size_type getIndex(const size_type i, const size_type j) const
        noexcept {
      return i * Stride + j;
    }
    //! \return the minimal data size
    constexpr size_type getUnderlyingArrayMinimalSize() const noexcept {
      if constexpr (Stride == M) {
        return this->size();
      } else {
        return (N - 1) * Stride + M * N;
      }
    }
    //!
    constexpr FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M>
    getRowMajorIndexingPolicy() {
      return FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M>();
    }  // end of getRowMajorIndexingPolicy
    //!
    constexpr size_type size() const noexcept { return N * M; }
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
      return i == 0 ? N : M;
    }
  };

  /*!
   * \brief partial specialisation of the `IndexingPoliciesCompatiblityCheck`
   * class for `FixedSizeRowMajorMatrixIndexingPolicy` of the same sizes.
   */
  template <typename SizeType,
            SizeType N,
            SizeType M,
            SizeType Stride1,
            SizeType Stride2>
  struct IndexingPoliciesCompatiblityCheck<
      FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M, Stride1>,
      FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M, Stride2>>
      : public IndexingPoliciesCompatiblityCheckBase<
            FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M, Stride1>,
            FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M, Stride2>> {
    static constexpr bool haveTheSameMemoryLayout() {
      return Stride1 == Stride2;
    }  // end of haveTheSameMemoryLayout
  };   // end of struct IndexingPoliciesCompatiblityCheck

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEINDEXINGPOLICIES_HXX */
