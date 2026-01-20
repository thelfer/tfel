/*!
 * \file  include/TFEL/Math/Array/StridedCoalescedView.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_STRIDEDCOALESCEDVIEW_HXX
#define LIB_TFEL_MATH_ARRAY_STRIDEDCOALESCEDVIEW_HXX

#include "TFEL/Math/Array/CoalescedView.hxx"

namespace tfel::math {

  template <MappableMathObjectUsingCoalescedViewConcept MappedType,
            typename IndexingPolicyType>
  struct StridedCoalescedAccessPolicy {
    //
    static inline constexpr auto N = [] {
      typename MappedType::indexing_policy p;
      return p.size();
    }();
    //! \brief default constructor
    explicit TFEL_HOST_DEVICE constexpr StridedCoalescedAccessPolicy(
        const ViewDataPointerType<MappedType> p, std::size_t s) noexcept
        : ptr(p), stride(s) {}
    //! \brief copy constructor
    constexpr StridedCoalescedAccessPolicy(
        const StridedCoalescedAccessPolicy&) noexcept = default;
    //! \brief move constructor
    constexpr StridedCoalescedAccessPolicy(
        StridedCoalescedAccessPolicy&&) noexcept = default;

   protected:
    [[nodiscard]] TFEL_HOST_DEVICE constexpr auto& getValue(
        const typename IndexingPolicyType::size_type i) noexcept {
      return this->ptr[i * (this->stride)];
    }
    [[nodiscard]] TFEL_HOST_DEVICE constexpr const auto& getValue(
        const typename IndexingPolicyType::size_type i) const noexcept {
      return this->ptr[i * (this->stride)];
    }
    //! \brief pointer to the memory buffer
    const ViewDataPointerType<MappedType> ptr;
    //! \brief data stride
    std::size_t stride;
  };

  template <MappableMathObjectUsingCoalescedViewConcept MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  using StridedCoalescedView = CoalescedViewBase<
    MappedType,
    IndexingPolicyType,
    StridedCoalescedAccessPolicy<MappedType, IndexingPolicyType>>;

  template <MappableMathObjectUsingCoalescedViewConcept MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  using ConstStridedCoalescedView = StridedCoalescedView<
    const std::remove_cv_t<MappedType>,
    IndexingPolicyType>;
  
  /*!
   * \brief return a strided coalesced view from a base pointer and a stride
   * \tparam MappedType: object mapped
   * \param[in] p: pointer to the first component
   * \param[in] s: stride between successive components
   */
  template <MappableMutableMathObjectUsingCoalescedViewConcept MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  TFEL_HOST_DEVICE constexpr StridedCoalescedView<MappedType,
                                                  IndexingPolicyType>
  map_strided(const ViewDataPointerType<MappedType> p,
              const typename IndexingPolicyType::size_type s)  //
      requires((!std::is_const_v<MappedType>)&&(
          std::remove_cv_t<MappedType>::hasFixedSizes)) {
    return StridedCoalescedView<MappedType, IndexingPolicyType>{p, s};
  }  // end of map_strided

  template <MappableImmutableMathObjectUsingCoalescedViewConcept MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  TFEL_HOST_DEVICE constexpr StridedCoalescedView<MappedType,
                                                  IndexingPolicyType>
  map_strided(const ViewConstDataPointerType<std::remove_cv_t<MappedType>> p,
              const typename IndexingPolicyType::size_type s)  //
      requires((std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes)) {
    return StridedCoalescedView<MappedType, IndexingPolicyType>{p, s};
  }  // end of map_strided


  
}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_STRIDEDCOALESCEDVIEW_HXX */
