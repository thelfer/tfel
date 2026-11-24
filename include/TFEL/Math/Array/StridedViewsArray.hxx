/*!
 * \file   include/TFEL/Math/Array/StridedViewsArray.hxx
 * \brief
 * \author Tristan Chenaille
 * \date 15/01/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STRIDEDVIEWSARRAY_HXX
#define LIB_TFEL_MATH_STRIDEDVIEWSARRAY_HXX

#include <array>
#include <type_traits>
#include "TFEL/Math/Array/ViewsArray.hxx"
#include "TFEL/Math/Array/StridedView.hxx"
#include "TFEL/Math/Array/FixedSizeIndexingPolicies.hxx"

namespace tfel::math::internals {

  /*!
   * \brief an helper structure using to define the type of views generate by
   * `StridedViewsArray`.
   * \tparam is_scalar: boolean stating if the mapped type is a scalar
   * \tparam MappedType: mapped type
   * \tparam ViewIndexingPolicyType: indexing policy used by the view
   */
  template <bool is_scalar,
            typename MappedType,
            typename ViewIndexingPolicyType>
  struct StridedViewsArrayBase {
    static_assert(!tfel::math::isScalar<MappedType>());
    //! \brief type of views
    using view_type =
        tfel::math::StridedView<MappedType, ViewIndexingPolicyType>;
    //! \brief type of read-only views
    using const_view_type =
        tfel::math::StridedView<const MappedType, ViewIndexingPolicyType>;
  };  // end of StridedViewsArrayBase

  /*!
   * \brief an helper structure using to define the type of views generate by
   * `StridedViewsArray`.
   * \tparam MappedType: mapped type
   * \tparam ViewIndexingPolicyType: indexing policy used by the view
   */
  template <typename MappedType, typename ViewIndexingPolicyType>
  struct StridedViewsArrayBase<true, MappedType, ViewIndexingPolicyType> {
    static_assert(tfel::math::isScalar<MappedType>());
    //! \brief type of views
    using view_type = tfel::math::scalar_view<MappedType>;
    //! \brief type of read-only views
    using const_view_type = tfel::math::scalar_view<const MappedType>;
  };  // end of StridedViewsArrayBase

  /*!
   * \brief a metafunction returning the default indexing policy for the given
   * mapped type
   * \tparam is_scalar: boolean stating if the mapped type is a scalar
   * \tparam MappedType: mapped type
   */
  template <bool is_scalar, typename MappedType>
  struct StridedViewsArrayDefaultViewIndexingPolicy {
    static_assert(!tfel::math::isScalar<MappedType>());
    //! \brief default indexing type
    using type = typename MappedType::indexing_policy;
  };  // end of StridedViewsArrayDefaultViewIndexingPolicy

  /*!
   * \brief a metafunction returning the default indexing policy for the given
   * scalar mapped type
   * \tparam MappedType: mapped type
   */
  template <typename MappedType>
  struct StridedViewsArrayDefaultViewIndexingPolicy<true, MappedType> {
    static_assert(tfel::math::isScalar<MappedType>());
    //! \brief default indexing type
    using type = tfel::math::ScalarIndexingPolicy<unsigned int>;
  };  // end of StridedViewsArrayDefaultViewIndexingPolicy

}  // end of namespace tfel::math::internals

namespace tfel::math {

  /*!
   * \brief a structure used to represent an array of strided views.
   * \tparam MappedType: mapped type
   * \tparam MemoryIndexingPolicyType: indexing policy used to access to the
   * underlying memory area.
   * \tparam ViewIndexingPolicyType: indexing policy used by the views
   */
  template <
      typename MappedType,
      typename MemoryIndexingPolicyType,
      typename ViewIndexingPolicyType = typename tfel::math::internals::
          StridedViewsArrayDefaultViewIndexingPolicy<isScalar<MappedType>(),
                                                     MappedType>::type>
  struct StridedViewsArray
      : MemoryIndexingPolicyType,
        tfel::math::internals::StridedViewsArrayBase<isScalar<MappedType>(),
                                                     MappedType,
                                                     ViewIndexingPolicyType> {
    //! \brief boolean stating if views to scalars are considered
    static constexpr bool is_scalar = isScalar<MappedType>();
    //! \brief type of the pointer to the underlying data
    using data_pointer_type = ViewsArrayDataPointerType<MappedType>;
    //! brief default constructor
    explicit TFEL_HOST_DEVICE constexpr StridedViewsArray(
        const data_pointer_type p) noexcept
        : data_pointer(p) {
      static_assert(MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
    }
    /*!
     * \brief constructor from an indexing policy
     * \param[in] p: indexing policy
     */
    explicit TFEL_HOST_DEVICE constexpr StridedViewsArray(
        const data_pointer_type p, const MemoryIndexingPolicyType& mp) noexcept
        : MemoryIndexingPolicyType(mp), data_pointer(p) {
      static_assert(!MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
    }
    /*!
     * \brief constructor from an indexing policy
     * \param[in] s: size used to initialize the memory' index policy
     */
    explicit TFEL_HOST_DEVICE constexpr StridedViewsArray(
        const data_pointer_type p,
        const typename MemoryIndexingPolicyType::size_type s) noexcept
        : MemoryIndexingPolicyType(s), data_pointer(p) {
      static_assert(!MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
    }
    /*!
     * \brief constructor from an indexing policy
     * \param[in] msizes: sizes used to initialize the memory' index policy
     */
    explicit TFEL_HOST_DEVICE constexpr StridedViewsArray(
        const data_pointer_type p,
        const std::array<typename MemoryIndexingPolicyType::size_type,
                         MemoryIndexingPolicyType::arity>& msizes) noexcept
        : MemoryIndexingPolicyType(msizes), data_pointer(p) {
      static_assert(!MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
    }
    //! \brief copy constructor
    constexpr StridedViewsArray(const StridedViewsArray&) noexcept = default;
    //! \brief move constructor
    constexpr StridedViewsArray(StridedViewsArray&&) noexcept = default;
    //! \brief a simple alias
    using StridedViewsArrayBase =
        tfel::math::internals::StridedViewsArrayBase<isScalar<MappedType>(),
                                                     MappedType,
                                                     ViewIndexingPolicyType>;
    //! \brief access operator
    TFEL_HOST_DEVICE constexpr typename StridedViewsArray::view_type operator[](
        const typename MemoryIndexingPolicyType::size_type i) noexcept {
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      auto* const d = this->data_pointer + this->getIndex(i);
      if constexpr (is_scalar) {
        return typename StridedViewsArray::view_type(*d);
      } else {
        return typename StridedViewsArray::view_type{d, this->size()};
      }
    }
    //! \brief access operator
    TFEL_HOST_DEVICE constexpr typename StridedViewsArray::const_view_type
    operator[](
        const typename MemoryIndexingPolicyType::size_type i) const noexcept {
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      auto* const d = this->data_pointer + this->getIndex(i);
      if constexpr (is_scalar) {
        return typename StridedViewsArray::const_view_type(*d);
      } else {
        return typename StridedViewsArray::const_view_type{d, this->size()};
      }
    }
    //! \brief access operator
    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr typename StridedViewsArray::view_type operator()(
        const Indices... i) noexcept {
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      auto* const d = this->data_pointer + this->getIndex(i...);
      if constexpr (is_scalar) {
        return typename StridedViewsArray::view_type(*d);
      } else {
        return typename StridedViewsArray::view_type{d, this->size()};
      }
    }
    //! \brief access operator
    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr typename StridedViewsArray::const_view_type
    operator()(const Indices... i) const noexcept {
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      const auto* const d = this->data_pointer + this->getIndex(i...);
      if constexpr (is_scalar) {
        return typename StridedViewsArray::const_view_type(*d);
      } else {
        return typename StridedViewsArray::const_view_type{d, this->size()};
      }
    }
    // \brief multiplication by a scalar
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr StridedViewsArray&
    operator*=(const ValueType2& s) noexcept requires(
        (isScalar<ValueType2>()) &&
        (isAssignableTo<BinaryOperationResult<MappedType, ValueType2, OpMult>,
                        MappedType>())) {
      static_assert(!std::is_const_v<MappedType>, "invalid call");
      this->applyToEachView([s](auto&& a) { a *= s; });
      return *this;
    }  // end of operator*=
    // \brief division by a scalar
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr StridedViewsArray&
    operator/=(const ValueType2& s) noexcept requires(
        (isScalar<ValueType2>()) &&
        (isAssignableTo<BinaryOperationResult<MappedType, ValueType2, OpDiv>,
                        MappedType>())) {
      static_assert(!std::is_const_v<MappedType>, "invalid call");
      using base = base_type<numeric_type<MappedType>>;
      if constexpr (std::is_integral_v<base>) {
        // the reciprocal of an integer is zero in general
        this->applyToEachView([s](auto&& a) { a /= s; });
        return *this;
      } else {
        return this->operator*=(base{1} / s);
      }
    }  // end of operator/=
    //! \brief destructor
    ~StridedViewsArray() noexcept = default;

   private:
    /*!
     * \brief apply an operation to each view
     * \param[in] op: operation
     */
    template <typename Operation>
    TFEL_HOST_DEVICE constexpr void applyToEachView(
        const Operation& op) noexcept {
      const auto f = makeMultiIndicesUnaryOperatorFunctor(op, *this);
      auto g = [p = static_cast<MemoryIndexingPolicyType>(*this)](
                   const typename StridedViewsArray::size_type i) {
        return p.size(i);
      };
      if constexpr (MemoryIndexingPolicyType::unRollLoop) {
        IterateOverMultipleIndices<typename StridedViewsArray::size_type, 0,
                                   MemoryIndexingPolicyType::arity,
                                   true>::exe(f, g);
      } else {
        IterateOverMultipleIndices<typename StridedViewsArray::size_type, 0,
                                   MemoryIndexingPolicyType::arity,
                                   false>::exe(f, g);
      }
    }  // end of applyToEachView
    //! \brief memory area from which views are taken
    const data_pointer_type data_pointer;
  };  // end of struct StridedViewsArray

  template <typename MappedType, typename SizeType, SizeType N>
  struct StridedViewsFixedSizeVectorIndexingPolicy
      : FixedSizeVectorIndexingPolicy<SizeType, N, 1> {
  };  // end of struct StridedViewsFixedSizeVectorIndexingPolicy

  template <typename MappedType, typename SizeType, SizeType N>
  using StridedViewsFixedSizeVector = StridedViewsArray<
      MappedType,
      StridedViewsFixedSizeVectorIndexingPolicy<MappedType, SizeType, N>>;

  template <typename ArrayType>
  constexpr auto map_strided_array(
      const ViewsArrayDataPointerType<
          std::conditional_t<std::is_const_v<ArrayType>,
                             const numeric_type<ArrayType>,
                             numeric_type<ArrayType>>> p) {
    using MappedType = numeric_type<ArrayType>;
    using IndexingPolicy = typename ArrayType::indexing_policy;
    using size_type = typename IndexingPolicy::size_type;
    static_assert(IndexingPolicy::hasFixedSizes);
    static_assert(IndexingPolicy::arity == 1u);
    constexpr auto N = IndexingPolicy().size(0);
    if constexpr (std::is_const_v<ArrayType>) {
      return StridedViewsFixedSizeVector<const MappedType, size_type, N>(p);
    } else {
      return StridedViewsFixedSizeVector<MappedType, size_type, N>(p);
    }
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STRIDEDVIEWSARRAY_HXX */
