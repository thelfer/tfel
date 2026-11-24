/*!
 * \file  include/TFEL/Math/Array/StridedView.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_STRIDEDVIEW_HXX
#define LIB_TFEL_MATH_ARRAY_STRIDEDVIEW_HXX

#include <utility>
#include "TFEL/Math/Array/ScatteredView.hxx"

namespace tfel::math {

  /*!
   * \brief access policy storing a pointer to the first component of the
   * mapped object and the stride between two successive components
   */
  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType>
  struct StridedAccessPolicy {
    //
    static inline constexpr auto N = [] {
      typename MappedType::indexing_policy p;
      return p.size();
    }();
    //! \brief default constructor
    explicit TFEL_HOST_DEVICE constexpr StridedAccessPolicy(
        const ViewDataPointerType<MappedType> p, std::size_t s) noexcept
        : ptr(p), stride(s) {}
    //! \brief copy constructor
    constexpr StridedAccessPolicy(const StridedAccessPolicy&) noexcept =
        default;
    //! \brief move constructor
    constexpr StridedAccessPolicy(StridedAccessPolicy&&) noexcept = default;

    /*!
     * \return the pointer to the first component of the mapped object
     * and the stride between successive components
     */
    [[nodiscard]] TFEL_HOST_DEVICE constexpr auto data() const noexcept {
      return std::make_pair(this->ptr, this->stride);
    }

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

  /*!
   * \brief view storing a pointer to the first component of the mapped object
   * and the stride between two successive components
   */
  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  using StridedView = AccessPolicyBasedView<
      MappedType,
      IndexingPolicyType,
      StridedAccessPolicy<MappedType, IndexingPolicyType>>;

  template <MappableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  using ConstStridedView =
      StridedView<const std::remove_cv_t<MappedType>, IndexingPolicyType>;

  /*!
   * \brief return a strided view from a base pointer and a stride
   * \tparam MappedType: object mapped
   * \param[in] p: pointer to the first component
   * \param[in] s: stride between successive components
   */
  template <MappableMutableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType = typename MappedType::indexing_policy>
  TFEL_HOST_DEVICE constexpr StridedView<MappedType, IndexingPolicyType>
  map_strided(const ViewDataPointerType<MappedType> p,
              const std::size_t s)  //
      requires((!std::is_const_v<MappedType>)&&(
          std::remove_cv_t<MappedType>::hasFixedSizes)) {
    return StridedView<MappedType, IndexingPolicyType>{p, s};
  }  // end of map_strided

  template <MappableImmutableMathObjectUsingScatteredViewConcept MappedType,
            typename IndexingPolicyType =
                typename std::remove_cv_t<MappedType>::indexing_policy>
  TFEL_HOST_DEVICE constexpr StridedView<MappedType, IndexingPolicyType>
  map_strided(const ViewConstDataPointerType<std::remove_cv_t<MappedType>> p,
              const std::size_t s)  //
      requires((std::remove_cv_t<MappedType>::indexing_policy::hasFixedSizes)) {
    return StridedView<MappedType, IndexingPolicyType>{p, s};
  }  // end of map_strided

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_STRIDEDVIEW_HXX */
