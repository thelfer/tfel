/*!
 * \file  include/TFEL/Math/Array/GenericFixedSizeArray.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_IXX
#define LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_IXX

namespace tfel::math {

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray() noexcept
      : v() {}  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::GenericFixedSizeArray(
      const GenericFixedSizeArray& src) noexcept
      : GenericFixedSizeArray() {
    this->operator=(src);
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::GenericFixedSizeArray(
      GenericFixedSizeArray&& src) noexcept
      : GenericFixedSizeArray() {
    this->operator=(src);
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <
      typename ValueType,
      typename std::enable_if<
          isAssignableTo<ValueType,
                         typename GenericFixedSizeArray<Child, ArrayPolicy, N>::
                             value_type>(),
          bool>::type>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::GenericFixedSizeArray(
      const ValueType& value) noexcept
      : GenericFixedSizeArray() {
    this->fill(value);
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray,
            typename std::enable_if<((isAssignableTo<OtherArray, Child>()) &&
                                     (!std::is_same_v<OtherArray, Child>)),
                                    bool>::type>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::GenericFixedSizeArray(
      const OtherArray& src) noexcept
      : GenericFixedSizeArray() {
    //     static_assert(checkIndexingPoliciesCompatiblity<
    //                   typename ArrayPolicy::indexing_policy,
    //                   typename OtherArray::ArrayPolicy::indexing_policy>());
    this->operator=(src);
  }  // end of GenericFixedSizeArray

  //   template <typename Child, typename ArrayPolicy, typename
  //   ArrayPolicy::IndexingPolicy::size_type N> template <
  //       typename ArrayPolicy2,
  //       typename std::enable_if<((checkIndexingPoliciesCompatiblity<
  //                                    typename ArrayPolicy::IndexingPolicy,
  //                                    typename
  //                                    ArrayPolicy2::IndexingPolicy>()) &&
  //                                (isAssignableTo<
  //                                    typename ArrayPolicy2::value_type,
  //                                    typename ArrayPolicy::value_type>())),
  //                               bool>::type>
  //   constexpr GenericFixedSizeArray<Child, N,
  //   ArrayPolicy>::GenericFixedSizeArray(
  //       const GenericFixedSizeArray<N, ArrayPolicy2>& src) noexcept
  //       : GenericFixedSizeArray() {
  //     this->operator=(src);
  //   }

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <
      typename ValueType,
      typename std::enable_if<
          isAssignableTo<ValueType,
                         typename GenericFixedSizeArray<Child, ArrayPolicy, N>::
                             value_type>(),
          bool>::type>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::GenericFixedSizeArray(
      const std::initializer_list<ValueType>& values) noexcept
      : GenericFixedSizeArray() {
    if (values.size() == 1u) {
      this->fill(*(values.begin()));
    } else {
      this->operator=(values);
    }
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <
      typename InputIterator,
      std::enable_if_t<
          std::is_same_v<
              typename std::iterator_traits<InputIterator>::value_type,
              base_type<typename GenericFixedSizeArray<Child, ArrayPolicy, N>::
                            value_type>>,
          bool>>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::GenericFixedSizeArray(
      const InputIterator p) {
    const auto& policy = this->getRowMajorIndexingPolicy();
    this->import(policy, p, p + this->size());
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator=(
      const GenericFixedSizeArray& src) noexcept {
    this->assign(src);
    return *this;
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator=(
      GenericFixedSizeArray&& src) noexcept {
    this->assign(src);
    return *this;
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray>
  constexpr std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&>
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator=(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    child.assign(src);
    return child;
  }  // end of GenericFixedSizeArray<Child, ArrayPolicy, N>

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray>
  constexpr std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&>
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator+=(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    child.addAndAssign(src);
    return child;
  }  // end of operator=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray>
  constexpr std::enable_if_t<isAssignableTo<OtherArray, Child>(), Child&>
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator-=(
      const OtherArray& src) noexcept {
    auto& child = static_cast<Child&>(*this);
    child.substractAndAssign(src);
    return child;
  }  // end of operator-=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename ValueType2>
  constexpr std::enable_if_t<
      isAssignableTo<
          BinaryOperationResult<
              ValueType2,
              typename GenericFixedSizeArray<Child, ArrayPolicy, N>::value_type,
              OpMult>,
          typename GenericFixedSizeArray<Child, ArrayPolicy, N>::value_type>(),
      Child&>
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator*=(
      const ValueType2& s) noexcept {
    auto& child = static_cast<Child&>(*this);
    child.multiplyByScalar(s);
    return child;
  }  // end of operator*=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename ValueType2>
  constexpr std::enable_if_t<
      isAssignableTo<
          BinaryOperationResult<
              typename GenericFixedSizeArray<Child, ArrayPolicy, N>::value_type,
              ValueType2,
              OpDiv>,
          typename GenericFixedSizeArray<Child, ArrayPolicy, N>::value_type>(),
      Child&>
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator/=(
      const ValueType2& s) noexcept {
    auto& child = static_cast<Child&>(*this);
    child.multiplyByScalar(1 / s);
    return child;
  }  // end of operator/=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr typename GenericFixedSizeArray<Child, ArrayPolicy, N>::pointer
  GenericFixedSizeArray<Child, ArrayPolicy, N>::data() noexcept {
    return this->v;
  }  // end of data

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr typename GenericFixedSizeArray<Child, ArrayPolicy, N>::const_pointer
  GenericFixedSizeArray<Child, ArrayPolicy, N>::data() const noexcept {
    return this->v;
  }  // end of data

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  constexpr typename GenericFixedSizeArray<Child, ArrayPolicy, N>::size_type
  GenericFixedSizeArray<Child, ArrayPolicy, N>::getContainerSize()
      const noexcept {
    return N;
  }  // end of data

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_IXX */
