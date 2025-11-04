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
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray() noexcept
      : v() {}  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray(const GenericFixedSizeArray& src) noexcept
      : GenericFixedSizeArray() {
    this->operator=(src);
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray(GenericFixedSizeArray&& src) noexcept
      : GenericFixedSizeArray() {
    this->operator=(src);
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename ValueType>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray(const ValueType& value) noexcept
      requires(isAssignableTo<ValueType, value_type>())
      : GenericFixedSizeArray() {
    this->fill(value);
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray(const OtherArray& src) noexcept  //
      requires((isAssignableTo<OtherArray, Child>()) &&
               (!std::is_same_v<OtherArray, Child>))
      : GenericFixedSizeArray() {
    //     static_assert(checkIndexingPoliciesCompatiblity<
    //                   typename ArrayPolicy::indexing_policy,
    //                   typename OtherArray<Child, ArrayPolicy,
    //                   N>::ArrayPolicy::indexing_policy>());
    this->operator=(src);
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename ValueType>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray(
          const std::initializer_list<ValueType>& values) noexcept  //
      requires(isAssignableTo<ValueType, value_type>())
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
  template <typename InputIterator>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>::
      GenericFixedSizeArray(const InputIterator p) noexcept  //
      requires(std::is_same_v<
               typename std::iterator_traits<InputIterator>::value_type,
               base_type<value_type>>) {
    const auto& policy = this->getRowMajorIndexingPolicy();
    this->import(policy, p, p + this->size());
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator=(
      const GenericFixedSizeArray& src) noexcept {
    this->assign(src);
    return *this;
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  TFEL_HOST_DEVICE constexpr GenericFixedSizeArray<Child, ArrayPolicy, N>&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator=(
      GenericFixedSizeArray&& src) noexcept {
    this->assign(src);
    return *this;
  }  // end of GenericFixedSizeArray

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray>
  TFEL_HOST_DEVICE constexpr Child&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator=(
      const OtherArray& src) noexcept
      requires(isAssignableTo<OtherArray, Child>()) {
    auto& child = static_cast<Child&>(*this);
    child.assign(src);
    return child;
  }  // end of GenericFixedSizeArray<Child, ArrayPolicy, N>

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray>
  TFEL_HOST_DEVICE constexpr Child&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator+=(
      const OtherArray& src) noexcept  //
      requires(isAssignableTo<OtherArray, Child>()) {
    auto& child = static_cast<Child&>(*this);
    child.addAndAssign(src);
    return child;
  }  // end of operator=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename OtherArray>
  TFEL_HOST_DEVICE constexpr Child&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator-=(
      const OtherArray& src) noexcept
      requires(isAssignableTo<OtherArray, Child>()) {
    auto& child = static_cast<Child&>(*this);
    child.substractAndAssign(src);
    return child;
  }  // end of operator-=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename ValueType2>
  TFEL_HOST_DEVICE constexpr Child&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator*=(
      const ValueType2& s) noexcept  //
      requires(
          isAssignableTo<BinaryOperationResult<ValueType2, value_type, OpMult>,
                         value_type>()) {
    auto& child = static_cast<Child&>(*this);
    child.multiplyByScalar(s);
    return child;
  }  // end of operator*=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  template <typename ValueType2>
  TFEL_HOST_DEVICE constexpr Child&
  GenericFixedSizeArray<Child, ArrayPolicy, N>::operator/=(
      const ValueType2& s) noexcept  //
      requires(
          isAssignableTo<BinaryOperationResult<value_type, ValueType2, OpDiv>,
                         value_type>()) {
    auto& child = static_cast<Child&>(*this);
    child.multiplyByScalar(1 / s);
    return child;
  }  // end of operator/=

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  TFEL_HOST_DEVICE constexpr
      typename GenericFixedSizeArray<Child, ArrayPolicy, N>::pointer
      GenericFixedSizeArray<Child, ArrayPolicy, N>::data() noexcept {
    return this->v;
  }  // end of data

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  TFEL_HOST_DEVICE constexpr
      typename GenericFixedSizeArray<Child, ArrayPolicy, N>::const_pointer
      GenericFixedSizeArray<Child, ArrayPolicy, N>::data() const noexcept {
    return this->v;
  }  // end of data

  template <typename Child,
            typename ArrayPolicy,
            typename ArrayPolicy::IndexingPolicy::size_type N>
  TFEL_HOST_DEVICE constexpr
      typename GenericFixedSizeArray<Child, ArrayPolicy, N>::size_type
      GenericFixedSizeArray<Child, ArrayPolicy, N>::getContainerSize()
          const noexcept {
    return N;
  }  // end of data

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZEARRAY_IXX */
