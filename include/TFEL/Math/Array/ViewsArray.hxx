/*!
 * \file   include/TFEL/Math/Array/ViewsArray.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19/05/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VIEWSARRAY_HXX
#define LIB_TFEL_MATH_VIEWSARRAY_HXX

#include <array>
#include <type_traits>
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Array/FixedSizeIndexingPolicies.hxx"

namespace tfel::math {

  namespace internals {

    /*!
     * \brief an helper structure using to define the type of views generate by
     * `ViewsArray`.
     * \tparam is_scalar: boolean stating if the mapped type is a scalar
     * \tparam MappedType: mapped type
     * \tparam ViewIndexingPolicyType: indexing policy used by the view
     */
    template <bool is_scalar,
              typename MappedType,
              typename ViewIndexingPolicyType>
    struct ViewsArrayBase {
      static_assert(!tfel::math::isScalar<MappedType>());
      //! \brief type of views
      using view_type = tfel::math::View<MappedType, ViewIndexingPolicyType>;
      //! \brief type of read-only views
      using const_view_type =
          tfel::math::View<const MappedType, ViewIndexingPolicyType>;
    };  // end of ViewsArrayBase

    /*!
     * \brief an helper structure using to define the type of views generate by
     * `ViewsArray`.
     * \tparam MappedType: mapped type
     * \tparam ViewIndexingPolicyType: indexing policy used by the view
     */
    template <typename MappedType, typename ViewIndexingPolicyType>
    struct ViewsArrayBase<true, MappedType, ViewIndexingPolicyType> {
      static_assert(tfel::math::isScalar<MappedType>());
      //! \brief type of views
      using view_type = MappedType&;
      //! \brief type of read-only views
      using const_view_type = const MappedType&;
    };  // end of ViewsArrayBase

    /*!
     * \brief a metafunction returning the default indexing policy for the given
     * mapped type
     * \tparam is_scalar: boolean stating if the mapped type is a scalar
     * \tparam MappedType: mapped type
     */
    template <bool is_scalar, typename MappedType>
    struct ViewsArrayDefaultViewIndexingPolicy {
      static_assert(!tfel::math::isScalar<MappedType>());
      //! \brief default indexing type
      using type = typename MappedType::indexing_policy;
    };  // end of ViewsArrayDefaultViewIndexingPolicy

    /*!
     * \brief a metafunction returning the default indexing policy for the given
     * scalar mapped type
     * \tparam MappedType: mapped type
     */
    template <typename MappedType>
    struct ViewsArrayDefaultViewIndexingPolicy<true, MappedType> {
      static_assert(tfel::math::isScalar<MappedType>());
      //! \brief default indexing type
      using type = tfel::math::ScalarIndexingPolicy<unsigned short>;
    };  // end of ViewsArrayDefaultViewIndexingPolicy

  }  // end of namespace internals

  //! \brief the numeric type used by the mapped type
  template <typename MappedType>
  using ViewsArrayNumericType = std::conditional_t<isScalar<MappedType>(),
                                                   std::remove_cv_t<MappedType>,
                                                   numeric_type<MappedType>>;

  //! \brief the pointer type used to build the view
  template <typename MappedType>
  using ViewsArrayDataPointerType =
      std::conditional_t<std::is_const_v<MappedType>,
                         const ViewsArrayNumericType<MappedType>*,
                         ViewsArrayNumericType<MappedType>*>;

  //! \brief return the minimal size required to a store the given object
  template <typename MappedType>
  constexpr auto getViewsArrayMinimalStride() {
    if constexpr (isScalar<MappedType>()) {
      return 1;
    } else {
      return getArrayPolicyMinimalDataSize<
          typename std::remove_cv_t<MappedType>::array_policy>();
    }
  }  // end of getViewsArrayMinimalStride

  /*!
   * \brief a structure used to represent an array of views.
   * \tparam MappedType: mapped type
   * \tparam MemoryIndexingPolicyType: indexing policy used to access to the
   * underlying memory area.
   * \tparam ViewIndexingPolicyType: indexing policy used by the views
   */
  template <typename MappedType,
            typename MemoryIndexingPolicyType,
            typename ViewIndexingPolicyType = typename tfel::math::internals::
                ViewsArrayDefaultViewIndexingPolicy<isScalar<MappedType>(),
                                                    MappedType>::type>
  struct ViewsArray
      : MemoryIndexingPolicyType,
        tfel::math::internals::ViewsArrayBase<isScalar<MappedType>(),
                                              MappedType,
                                              ViewIndexingPolicyType> {
    //! \brief boolean stating if views to scalars are considered
    static constexpr bool is_scalar = isScalar<MappedType>();
    //! \brief type of the pointer to the underlying data
    using data_pointer_type = ViewsArrayDataPointerType<MappedType>;
    //! brief default constructor
    explicit TFEL_HOST_DEVICE constexpr ViewsArray(
        const data_pointer_type p) noexcept
        : data_pointer(p) {
      static_assert(MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
    }
    /*!
     * \brief constructor from an indexing policy
     * \param[in] p: indexing policy
     */
    explicit TFEL_HOST_DEVICE constexpr ViewsArray(
        const data_pointer_type p, const MemoryIndexingPolicyType& mp) noexcept
        : data_pointer(p), MemoryIndexingPolicyType(mp) {
      static_assert(!MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
    }
    /*!
     * \brief constructor from an indexing policy
     * \param[in] s: size used to initialize the memory' index policy
     */
    explicit TFEL_HOST_DEVICE constexpr ViewsArray(
        const data_pointer_type p,
        const typename MemoryIndexingPolicyType::size_type s) noexcept
        : data_pointer(p), MemoryIndexingPolicyType(s) {
      static_assert(!MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
    }
    /*!
     * \brief constructor from an indexing policy
     * \param[in] msizes: sizes used to initialize the memory' index policy
     */
    explicit TFEL_HOST_DEVICE constexpr ViewsArray(
        const data_pointer_type p,
        const std::array<typename MemoryIndexingPolicyType::size_type,
                         MemoryIndexingPolicyType::arity>& msizes) noexcept
        : data_pointer(p), MemoryIndexingPolicyType(msizes) {
      static_assert(!MemoryIndexingPolicyType::hasFixedSizes, "invalid call");
    }
    //! \brief copy constructor
    constexpr ViewsArray(const ViewsArray&) noexcept = default;
    //! \brief move constructor
    constexpr ViewsArray(ViewsArray&&) noexcept = default;
    //! \brief a simple alias
    using ViewsArrayBase =
        tfel::math::internals::ViewsArrayBase<isScalar<MappedType>(),
                                              MappedType,
                                              ViewIndexingPolicyType>;
    //! \brief access operator
    TFEL_HOST_DEVICE constexpr typename ViewsArray::view_type operator[](
        const typename MemoryIndexingPolicyType::size_type i) noexcept {
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      auto* const d = this->data_pointer + this->getIndex(i);
      if constexpr (is_scalar) {
        return *d;
      } else {
        return typename ViewsArray::view_type{d};
      }
    }
    //! \brief access operator
    TFEL_HOST_DEVICE constexpr typename ViewsArray::const_view_type operator[](
        const typename MemoryIndexingPolicyType::size_type i) const noexcept {
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      auto* const d = this->data_pointer + this->getIndex(i);
      if constexpr (is_scalar) {
        return *d;
      } else {
        return typename ViewsArray::const_view_type{d};
      }
    }
    //! \brief access operator
    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr typename ViewsArray::view_type operator()(
        const Indices... i) noexcept {
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      auto* const d = this->data_pointer + this->getIndex(i...);
      if constexpr (is_scalar) {
        return *d;
      } else {
        return typename ViewsArray::view_type{d};
      }
    }
    //! \brief access operator
    template <typename... Indices>
    TFEL_HOST_DEVICE constexpr typename ViewsArray::const_view_type operator()(
        const Indices... i) const noexcept {
      static_assert(ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      const auto* const d = this->data_pointer + this->getIndex(i...);
      if constexpr (is_scalar) {
        return *d;
      } else {
        return typename ViewsArray::const_view_type{d};
      }
    }
    //! \brief access operator
    constexpr auto operator()(
        const typename MemoryIndexingPolicyType::size_type i,
        const typename ViewIndexingPolicyType::size_type s) noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      static_assert(ViewIndexingPolicyType::arity == 1u, "invalid call");
      const auto p = ViewIndexingPolicyType{s};
      return typename ViewsArray::view_type{
          this->data_pointer + this->getIndex(i), p};
    }
    //! \brief access operator
    constexpr auto operator()(
        const typename MemoryIndexingPolicyType::size_type i,
        const typename ViewIndexingPolicyType::size_type s) const noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      static_assert(ViewIndexingPolicyType::arity == 1u, "invalid call");
      const auto p = ViewIndexingPolicyType{s};
      return typename ViewsArray::const_view_type{
          this->data_pointer + this->getIndex(i), p};
    }
    //! \brief access operator
    constexpr auto operator()(
        const typename MemoryIndexingPolicyType::size_type i,
        const std::array<typename ViewIndexingPolicyType::size_type,
                         ViewIndexingPolicyType::arity>& vsizes) noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      using view_type = typename ViewsArrayBase::view_type;
      const auto p = ViewIndexingPolicyType{vsizes};
      return view_type{this->data_pointer + this->getIndex(i), p};
    }
    //! \brief access operator
    constexpr auto operator()(
        const typename MemoryIndexingPolicyType::size_type i,
        const std::array<typename ViewIndexingPolicyType::size_type,
                         ViewIndexingPolicyType::arity>& vsizes)
        const noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(MemoryIndexingPolicyType::arity == 1u, "invalid call");
      using const_view_type = typename ViewsArrayBase::const_view_type;
      const auto p = ViewIndexingPolicyType{vsizes};
      return const_view_type{this->data_pointer + this->getIndex(i), p};
    }
    //! \brief access operator
    constexpr auto operator()(
        const std::array<typename MemoryIndexingPolicyType::size_type,
                         MemoryIndexingPolicyType::arity>& indices,
        const typename ViewIndexingPolicyType::size_type s) noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(ViewIndexingPolicyType::arity == 1u, "invalid call");
      using view_type = typename ViewsArrayBase::view_type;
      const auto p = ViewIndexingPolicyType{s};
      return view_type{this->data_pointer + this->getIndex(indices), p};
    }
    //! \brief access operator
    constexpr auto operator()(
        const std::array<typename MemoryIndexingPolicyType::size_type,
                         MemoryIndexingPolicyType::arity>& indices,
        const typename ViewIndexingPolicyType::size_type s) const noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      static_assert(ViewIndexingPolicyType::arity == 1u, "invalid call");
      using const_view_type = typename ViewsArrayBase::const_view_type;
      const auto p = ViewIndexingPolicyType{s};
      return const_view_type{this->data_pointer + this->getIndex(indices), p};
    }
    //! \brief access operator
    constexpr auto operator()(
        const std::array<typename MemoryIndexingPolicyType::size_type,
                         MemoryIndexingPolicyType::arity>& indices,
        const std::array<typename ViewIndexingPolicyType::size_type,
                         ViewIndexingPolicyType::arity>& vsizes) noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      using view_type = typename ViewsArrayBase::view_type;
      const auto p = ViewIndexingPolicyType{vsizes};
      return view_type{this->data_pointer + this->getIndex(indices), p};
    }
    //! \brief access operator
    constexpr auto operator()(
        const std::array<typename MemoryIndexingPolicyType::size_type,
                         MemoryIndexingPolicyType::arity>& indices,
        const std::array<typename ViewIndexingPolicyType::size_type,
                         ViewIndexingPolicyType::arity>& vsizes)
        const noexcept {
      static_assert(!ViewIndexingPolicyType::hasFixedSizes, "invalid call");
      using const_view_type = typename ViewsArrayBase::const_view_type;
      const auto p = ViewIndexingPolicyType{vsizes};
      return const_view_type{this->data_pointer + this->getIndex(indices), p};
    }
    // \brief multiplication by a scalar
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr std::enable_if_t<
        ((isScalar<ValueType2>()) &&
         (isAssignableTo<BinaryOperationResult<MappedType, ValueType2, OpMult>,
                         MappedType>())),
        ViewsArray&>
    operator*=(const ValueType2& s) noexcept {
      static_assert(!std::is_const_v<MappedType>, "invalid call");
      const auto f =
          makeMultiIndicesUnaryOperatorFunctor([s](auto& a) { a *= s; }, *this);
      auto g = [p = static_cast<MemoryIndexingPolicyType>(*this)](
                   const typename ViewsArray::size_type i) {
        return p.size(i);
      };
      if constexpr (MemoryIndexingPolicyType::unRollLoop) {
        IterateOverMultipleIndices<typename ViewsArray::size_type, 0,
                                   MemoryIndexingPolicyType::arity,
                                   true>::exe(f, g);
      } else {
        IterateOverMultipleIndices<typename ViewsArray::size_type, 0,
                                   MemoryIndexingPolicyType::arity,
                                   false>::exe(f, g);
      }
      return *this;
    }  // end of operator*=
    // \brief division by a scalar
    template <typename ValueType2>
    TFEL_HOST_DEVICE constexpr std::enable_if_t<
        ((isScalar<ValueType2>()) &&
         (isAssignableTo<BinaryOperationResult<MappedType, ValueType2, OpDiv>,
                         MappedType>())),
        ViewsArray&>
    operator/=(const ValueType2& s) noexcept {
      static_assert(!std::is_const_v<MappedType>, "invalid call");
      return this->operator*=(1 / s);
    }  // end of operator/=
    //! \brief destructor
    ~ViewsArray() noexcept = default;

   private:
    //! \brief memory area from which views are taken
    const data_pointer_type data_pointer;
  };  // end of struct ViewsArray

  template <typename MappedType,
            typename SizeType,
            SizeType N,
            SizeType Stride = getViewsArrayMinimalStride<MappedType>()>
  struct ViewsFixedSizeVectorIndexingPolicy
      : FixedSizeVectorIndexingPolicy<SizeType, N, Stride> {
    static_assert(Stride >= getViewsArrayMinimalStride<MappedType>(),
                  "invalid stride");
  };  // end of struct ViewsFixedSizeVectorIndexingPolicy

  template <typename MappedType,
            typename SizeType,
            SizeType N,
            SizeType Stride = getViewsArrayMinimalStride<MappedType>()>
  using ViewsFixedSizeVector = ViewsArray<
      MappedType,
      ViewsFixedSizeVectorIndexingPolicy<MappedType, SizeType, N, Stride>>;

  template <typename ArrayType>
  constexpr auto map_array(
      const ViewsArrayDataPointerType<numeric_type<ArrayType>> p) {
    using MappedType = numeric_type<ArrayType>;
    using IndexingPolicy = typename ArrayType::indexing_policy;
    using size_type = typename IndexingPolicy::size_type;
    static_assert(IndexingPolicy::hasFixedSizes);
    static_assert(IndexingPolicy::arity == 1u);
    constexpr auto N = IndexingPolicy().size(0);
    if constexpr (std::is_const_v<ArrayType>) {
      return ViewsFixedSizeVector<const MappedType, size_type, N>(p);
    } else {
      return ViewsFixedSizeVector<MappedType, size_type, N>(p);
    }
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_VIEWSARRAY_HXX */
