/*!
 * \file  include/TFEL/Math/Array/FixedSizeIndexingPolicies.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEINDEXINGPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEINDEXINGPOLICIES_HXX

#include <array>
#include <type_traits>
#include "TFEL/ContractViolation.hxx"
#include "TFEL/Metaprogramming/MakeIntegerRange.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"

namespace tfel::math {

  template <typename IndexingPolicy>
  constexpr auto getUnderlyingArrayMinimalSize() {
    IndexingPolicy p;
    return p.getUnderlyingArrayMinimalSize();
  }  // end of getUnderlyingArrayMinimalSize

  /*!
   * \brief an indexing policy suitable for scalar
   * \tparam SizeType: type used for indexing
   */
  template <typename SizeType>
  struct ScalarIndexingPolicy {
    //!
    static constexpr auto hasFixedSizes = true;
    //!
    using size_type = SizeType;
    //!
    using RowMajorIndexingPolicy = ScalarIndexingPolicy<SizeType>;
    //!
    static constexpr size_type arity = 0;
    //!
    static constexpr auto areDataContiguous = true;
    //!
    static constexpr auto unRollLoop = true;
    //! \return the minimal data size
    TFEL_HOST_DEVICE constexpr size_type getUnderlyingArrayMinimalSize()
        const noexcept {
      return 1;
    }
    //! \return if the array is empty
    TFEL_HOST_DEVICE constexpr bool empty() const noexcept { return false; }
    //! \return the logical size of the array
    TFEL_HOST_DEVICE constexpr size_type size() const noexcept { return 1; }
    //!
    constexpr auto getRowMajorIndexingPolicy() {
      return ScalarIndexingPolicy<SizeType>();
    }  // end of getFixedSizeVectorIndexingPolicy
  };

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
    static constexpr auto hasFixedSizes = true;
    //!
    using size_type = SizeType;
    //!
    using RowMajorIndexingPolicy = FixedSizeVectorIndexingPolicy<SizeType, N>;
    //!
    static constexpr size_type arity = 1;
    //!
    static constexpr auto areDataContiguous = (Stride == 1);
    //!
    static constexpr auto unRollLoop = N < 100;
    //! \return the minimal data size
    TFEL_HOST_DEVICE constexpr size_type getUnderlyingArrayMinimalSize()
        const noexcept {
      if constexpr (Stride == 1) {
        return this->size();
      } else {
        return (N - 1) * Stride + 1;
      }
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const size_type i) const noexcept {
      if constexpr (Stride == 1) {
        return i;
      } else {
        return i * Stride;
      }
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const std::array<size_type, 1u>& indices) const noexcept {
      return this->getIndex(indices[0]);
    }
    //! \return if the array is empty
    TFEL_HOST_DEVICE constexpr bool empty() const noexcept { return N == 0; }
    //! \return the logical size of the array
    TFEL_HOST_DEVICE constexpr size_type size() const noexcept { return N; }
    /*!
     * \return the logical size of the array for the given dimension
     * \param[in] i: dimension
     */
    TFEL_HOST_DEVICE constexpr size_type size(
        const size_type i) const noexcept {
      if (i != 0) {
        tfel::reportContractViolation(
            "FixedSizeVectorIndexingPolicy: "
            "invalid dimension");
      }
      return N;
    }
    //!
    constexpr auto getRowMajorIndexingPolicy() {
      return FixedSizeVectorIndexingPolicy<SizeType, N>();
    }  // end of getFixedSizeVectorIndexingPolicy
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
    static constexpr auto hasFixedSizes = true;
    //!
    static constexpr auto areDataContiguous = (Stride == M);
    //!
    static constexpr auto unRollLoop = true;
    //!
    using size_type = SizeType;
    //!
    static constexpr size_type arity = 2;
    //!
    using RowMajorIndexingPolicy =
        FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M>;
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const size_type i, const size_type j) const noexcept {
      return i * Stride + j;
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const std::array<size_type, 2u>& indices) const noexcept {
      return this->getIndex(indices[0], indices[1]);
    }
    //! \return the minimal data size
    TFEL_HOST_DEVICE constexpr size_type getUnderlyingArrayMinimalSize()
        const noexcept {
      if constexpr (Stride == M) {
        return this->size();
      } else {
        return (N - 1) * Stride + N;
      }
    }
    //!
    constexpr auto getRowMajorIndexingPolicy() {
      return FixedSizeRowMajorMatrixIndexingPolicy<SizeType, N, M>();
    }  // end of getRowMajorIndexingPolicy
    //! \return if the matrix is empty
    TFEL_HOST_DEVICE constexpr bool empty() const noexcept {
      return N * M == 0;
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type size() const noexcept { return N * M; }
    /*!
     * \return the logical size of the array for the given dimension
     * \param[in] i: dimension
     */
    TFEL_HOST_DEVICE constexpr size_type size(
        const size_type i) const noexcept {
#ifndef TFEL_NO_RUNTIME_CHECK_BOUNDS
      if (i > 1) {
        tfel::reportContractViolation(
            "FixedSizeMatrixIndexingPolicy: "
            "invalid dimension");
      }
#endif /* TFEL_NO_RUNTIME_CHECK_BOUNDS */
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

  /*!
   * \brief an indexing policy build as the cartesian product of the two
   * fixed size indexing policies.
   *
   * The memory position associed with the index (i0, ..., iN, j0, ..., jM) is
   * given by:
   *
   * \f[
   *   p1(i0, ..., iN) * Stride + p2(j0, ..., jM)
   * \f]
   *
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \tparam Stride: stride parameter
   */
  template <typename IndexingPolicy1,
            typename IndexingPolicy2,
            typename IndexingPolicy2::size_type Stride =
                getUnderlyingArrayMinimalSize<IndexingPolicy2>()>
  struct FixedSizeIndexingPoliciesCartesianProduct {
    static_assert(IndexingPolicy1::hasFixedSizes);
    static_assert(IndexingPolicy2::hasFixedSizes);
    static_assert(Stride >= getUnderlyingArrayMinimalSize<IndexingPolicy2>(),
                  "invalid stride value");
    //!
    static constexpr auto hasFixedSizes = true;
    //!
    static constexpr auto areDataContiguous =
        ((Stride == getUnderlyingArrayMinimalSize<IndexingPolicy2>()) &&
         (IndexingPolicy1::areDataContiguous) &&
         (IndexingPolicy2::areDataContiguous));
    //!
    static constexpr auto unRollLoop = true;
    //!
    using size_type = std::common_type_t<typename IndexingPolicy1::size_type,
                                         typename IndexingPolicy2::size_type>;
    //!
    static constexpr size_type arity =
        IndexingPolicy1::arity + IndexingPolicy2::arity;
    //!
    using RowMajorIndexingPolicy = FixedSizeIndexingPoliciesCartesianProduct<
        typename IndexingPolicy1::RowMajorIndexingPolicy,
        typename IndexingPolicy2::RowMajorIndexingPolicy>;
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex() const noexcept {
      static_assert(arity == 0);
      return size_type{};
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const size_type i) const noexcept {
      static_assert(arity == 1);
      constexpr auto a1 = IndexingPolicy1::arity;
      if constexpr (a1 == 1) {
        IndexingPolicy1 p1;
        return p1.getIndex(i) * Stride;
      } else {
        IndexingPolicy2 p2;
        return p2.getIndex(i);
      }
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const size_type i, const size_type j) const noexcept {
      static_assert(arity == 2);
      constexpr auto a1 = IndexingPolicy1::arity;
      if constexpr (a1 == 0) {
        IndexingPolicy2 p2;
        return p2.getIndex(i, j);
      } else if constexpr (a1 == 1) {
        IndexingPolicy1 p1;
        IndexingPolicy2 p2;
        return p1.getIndex(i) * Stride + p2.getIndex(j);
      } else {
        IndexingPolicy1 p1;
        return p1.getIndex(i, j) * Stride;
      }
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const size_type i,
        const size_type j,
        const size_type k) const noexcept {
      static_assert(arity == 3);
      constexpr auto a1 = IndexingPolicy1::arity;
      if constexpr (a1 == 0) {
        IndexingPolicy2 p2;
        return p2.getIndex(i, j, k);
      } else if constexpr (a1 == 1) {
        IndexingPolicy1 p1;
        IndexingPolicy2 p2;
        return p1.getIndex(i) * Stride + p2.getIndex(j, k);
      } else if constexpr (a1 == 2) {
        IndexingPolicy1 p1;
        IndexingPolicy2 p2;
        return p1.getIndex(i, j) * Stride + p2.getIndex(k);
      } else {
        IndexingPolicy1 p1;
        return p1.getIndex(i, j, k) * Stride;
      }
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const size_type i,
        const size_type j,
        const size_type k,
        const size_type l) const noexcept {
      static_assert(arity == 4);
      constexpr auto a1 = IndexingPolicy1::arity;
      if constexpr (a1 == 0) {
        IndexingPolicy2 p2;
        return p2.getIndex(i, j, k, l);
      } else if constexpr (a1 == 1) {
        IndexingPolicy1 p1;
        IndexingPolicy2 p2;
        return p1.getIndex(i) * Stride + p2.getIndex(j, k, l);
      } else if constexpr (a1 == 2) {
        IndexingPolicy1 p1;
        IndexingPolicy2 p2;
        return p1.getIndex(i, j) * Stride + p2.getIndex(k, l);
      } else if constexpr (a1 == 3) {
        IndexingPolicy1 p1;
        IndexingPolicy2 p2;
        return p1.getIndex(i, j, k) * Stride + p2.getIndex(l);
      } else {
        IndexingPolicy1 p1;
        return p1.getIndex(i, j, k, l) * Stride;
      }
    }
    //!
    template <typename... SizeType>
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const SizeType... i) const noexcept {
      static_assert(sizeof...(SizeType) == arity,
                    "invalid number of arguments");
      checkIndicesValiditity<FixedSizeIndexingPoliciesCartesianProduct,
                             SizeType...>();
      constexpr auto a1 = IndexingPolicy1::arity;
      constexpr auto a2 = IndexingPolicy2::arity;
      if constexpr (a1 == 0) {
        IndexingPolicy2 p2;
        return p2.getIndex(i...);
      } else if constexpr (a2 == 0) {
        IndexingPolicy1 p1;
        return p1.getIndex(i...) * Stride;
      } else {
        const auto indices =
            std::array<size_type, arity>{static_cast<size_type>(i)...};
        const tfel::meta::make_integer_range<size_type, 0, a1> r1;
        const tfel::meta::make_integer_range<size_type, a1, arity> r2;
        const auto i1 = getIndexPolicyIndex<IndexingPolicy1>(indices, r1);
        const auto i2 = getIndexPolicyIndex<IndexingPolicy2>(indices, r2);
        return i1 * Stride + i2;
      }
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type getIndex(
        const std::array<size_type, arity>& indices) const noexcept {
      constexpr auto a1 = IndexingPolicy1::arity;
      if constexpr (a1 == 0) {
        const tfel::meta::make_integer_range<size_type, a1, arity> r2;
        return getIndexPolicyIndex<IndexingPolicy2>(indices, r2);
      } else if constexpr (IndexingPolicy2::arity == 0) {
        const tfel::meta::make_integer_range<size_type, 0, arity> r1;
        return getIndexPolicyIndex<IndexingPolicy1>(indices, r1);
      } else {
        const tfel::meta::make_integer_range<size_type, 0, a1> r1;
        const tfel::meta::make_integer_range<size_type, a1, arity> r2;
        const auto i1 = getIndexPolicyIndex<IndexingPolicy1>(indices, r1);
        const auto i2 = getIndexPolicyIndex<IndexingPolicy2>(indices, r2);
        return i1 * Stride + i2;
      }
    }
    //! \return the minimal data size
    TFEL_HOST_DEVICE constexpr size_type getUnderlyingArrayMinimalSize()
        const noexcept {
      constexpr auto a1 = IndexingPolicy1::arity;
      if constexpr (a1 == 0) {
        return tfel::math::getUnderlyingArrayMinimalSize<IndexingPolicy2>();
      } else if constexpr (IndexingPolicy2::arity == 0) {
        const auto s1 =
            tfel::math::getUnderlyingArrayMinimalSize<IndexingPolicy1>();
        return (s1 - 1) * Stride + 1;
      } else {
        const auto s1 =
            tfel::math::getUnderlyingArrayMinimalSize<IndexingPolicy1>();
        const auto s2 =
            tfel::math::getUnderlyingArrayMinimalSize<IndexingPolicy2>();
        return (s1 - 1) * Stride + s2;
      }
    }
    //!
    constexpr auto getRowMajorIndexingPolicy() {
      return RowMajorIndexingPolicy();
    }  // end of getRowMajorIndexingPolicy
    //! \return if the array is empty
    TFEL_HOST_DEVICE constexpr bool empty() const noexcept {
      IndexingPolicy1 p1;
      IndexingPolicy2 p2;
      return p1.empty() || p2.empty();
    }
    //!
    TFEL_HOST_DEVICE constexpr size_type size() const noexcept {
      IndexingPolicy1 p1;
      IndexingPolicy2 p2;
      return p1.size() * p2.size();
    }
    /*!
     * \return the logical size of the array for the given dimension
     * \param[in] i: dimension
     */
    TFEL_HOST_DEVICE constexpr size_type size(
        const size_type i) const noexcept {
      if (i >= FixedSizeIndexingPoliciesCartesianProduct::arity) {
        tfel::reportContractViolation(
            "FixedSizeMatrixIndexingPolicy: "
            "invalid dimension");
      }
      if constexpr (IndexingPolicy1::arity == 0) {
        IndexingPolicy2 p2;
        return p2.size(i);
      } else if constexpr (IndexingPolicy2::arity == 0) {
        IndexingPolicy1 p1;
        return p1.size(i);
      } else {
        if (i < IndexingPolicy1::arity) {
          IndexingPolicy1 p1;
          return p1.size(i);
        }
        IndexingPolicy2 p2;
        return p2.size(i - IndexingPolicy1::arity);
      }
    }

   private:
    //
    template <typename IndexingPolicy, size_type... Indexes>
    static constexpr size_type getIndexPolicyIndex(
        const std::array<size_type, arity>& values,
        std::integer_sequence<size_type, Indexes...>) {
      const IndexingPolicy p;
      return p.getIndex(values[Indexes]...);
    }  // end of getIndexPolicyIndex
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEINDEXINGPOLICIES_HXX */
