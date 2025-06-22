/*!
 * \file  include/TFEL/Math/Array/GenericRuntimeArray.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_IXX
#define LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_IXX

namespace tfel::math {

  template <typename Child, typename ArrayPolicy>
  GenericRuntimeArray<Child, ArrayPolicy>::GenericRuntimeArray(
      const typename ArrayPolicy::IndexingPolicy& p)
      : MutableRuntimeArrayBase<GenericRuntimeArray<Child, ArrayPolicy>,
                                ArrayPolicy>(p),
        data_values(p.getUnderlyingArrayMinimalSize()) {
  }  // end of GenericRuntimeArray

  template <typename Child, typename ArrayPolicy>
  template <typename ValueType>
  GenericRuntimeArray<Child, ArrayPolicy>::GenericRuntimeArray(
      const typename ArrayPolicy::IndexingPolicy& p,
      const ValueType& value)  //
      requires(isAssignableTo<
               ValueType,
               typename GenericRuntimeArray<Child, ArrayPolicy>::value_type>())
      : GenericRuntimeArray(p) {
    this->fill(value);
  }  // end of GenericRuntimeArray

  template <typename Child, typename ArrayPolicy>
  template <typename OtherArray>
  GenericRuntimeArray<Child, ArrayPolicy>::GenericRuntimeArray(
      const OtherArray& src) requires((isAssignableTo<OtherArray, Child>()) &&
                                      (!std::is_same_v<OtherArray, Child>))
      : GenericRuntimeArray(src.getIndexingPolicy()) {
    this->operator=(src);
  }  // end of GenericRuntimeArray

  template <typename Child, typename ArrayPolicy>
  template <typename ValueType>
  GenericRuntimeArray<Child, ArrayPolicy>::GenericRuntimeArray(
      const std::initializer_list<ValueType>&
          values) requires((isAssignableTo<ValueType,
                                           typename GenericRuntimeArray<
                                               Child,
                                               ArrayPolicy>::value_type>()) &&
                           (ArrayPolicy::IndexingPolicy::arity == 1) &&
                           (ArrayPolicy::IndexingPolicy::areDataContiguous))
      : GenericRuntimeArray(values.size()) {
    this->operator=(values);
  }  // end of GenericRuntimeArray

  template <typename Child, typename ArrayPolicy>
  GenericRuntimeArray<Child, ArrayPolicy>&
  GenericRuntimeArray<Child, ArrayPolicy>::operator=(
      const GenericRuntimeArray& src) {
    //     checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
    //                                              src.getIndexingPolicy());
    const auto f = makeMultiIndicesBinaryOperatorFunctor(
        [](auto& a, const auto& b) { a = b; }, *this, src);
    this->iterate(f);
    return *this;
  }  // end of GenericRuntimeArray

  template <typename Child, typename ArrayPolicy>
  GenericRuntimeArray<Child, ArrayPolicy>&
  GenericRuntimeArray<Child, ArrayPolicy>::operator=(
      GenericRuntimeArray&& src) {
    //     checkIndexingPoliciesRuntimeCompatiblity(this->getIndexingPolicy(),
    //                                              src.getIndexingPolicy());
    if constexpr (ArrayPolicy::IndexingPolicy::areDataContiguous) {
      if (this->getContainerSize() == src.getContainerSize()) {
        this->data_values.operator=(std::move(src.data_values));
      } else {
        const auto f = makeMultiIndicesBinaryOperatorFunctor(
            [](auto& a, const auto& b) { a = b; }, *this, src);
        this->iterate(f);
      }
    } else {
      const auto f = makeMultiIndicesBinaryOperatorFunctor(
          [](auto& a, const auto& b) { a = b; }, *this, src);
      this->iterate(f);
    }
    return *this;
  }  // end of GenericRuntimeArray

  template <typename Child, typename ArrayPolicy>
  template <typename OtherArray>
  Child& GenericRuntimeArray<Child, ArrayPolicy>::operator=(
      const OtherArray& src) requires(isAssignableTo<OtherArray, Child>()) {
    auto& child = static_cast<Child&>(*this);
    //     checkIndexingPoliciesRuntimeCompatiblity(child.getIndexingPolicy(),
    //                                              src.getIndexingPolicy());
    child.assign(src);
    return child;
  }

  template <typename Child, typename ArrayPolicy>
  template <typename OtherArray>
  Child& GenericRuntimeArray<Child, ArrayPolicy>::operator+=(
      const OtherArray& src) requires(isAssignableTo<OtherArray, Child>()) {
    auto& child = static_cast<Child&>(*this);
    //     checkIndexingPoliciesRuntimeCompatiblity(child.getIndexingPolicy(),
    //                                              src.getIndexingPolicy());
    child.addAndAssign(src);
    return child;
  }  // end of GenericRuntimeArray<Child, ArrayPolicy>

  template <typename Child, typename ArrayPolicy>
  template <typename OtherArray>
  Child& GenericRuntimeArray<Child, ArrayPolicy>::operator-=(
      const OtherArray& src) requires(isAssignableTo<OtherArray, Child>()) {
    auto& child = static_cast<Child&>(*this);
    //     checkIndexingPoliciesRuntimeCompatiblity(child.getIndexingPolicy(),
    //                                              src.getIndexingPolicy());
    child.substractAndAssign(src);
    return child;
  }  // end of GenericRuntimeArray<Child, ArrayPolicy>

  template <typename Child, typename ArrayPolicy>
  template <typename ValueType2>
  Child& GenericRuntimeArray<Child, ArrayPolicy>::operator*=(
      const ValueType2& v) noexcept
      requires(
          isAssignableTo<
              BinaryOperationResult<
                  ValueType2,
                  typename GenericRuntimeArray<Child, ArrayPolicy>::value_type,
                  OpMult>,
              typename GenericRuntimeArray<Child, ArrayPolicy>::value_type>()) {
    auto& child = static_cast<Child&>(*this);
    child.multiplyByScalar(v);
    return child;
  }
  //
  template <typename Child, typename ArrayPolicy>
  template <typename ValueType2>
  Child& GenericRuntimeArray<Child, ArrayPolicy>::operator/=(
      const ValueType2& v) noexcept
      requires(
          isAssignableTo<
              BinaryOperationResult<
                  typename GenericRuntimeArray<Child, ArrayPolicy>::value_type,
                  ValueType2,
                  OpDiv>,
              typename GenericRuntimeArray<Child, ArrayPolicy>::value_type>()) {
    auto& child = static_cast<Child&>(*this);
    child.multiplyByScalar(1 / v);
    return child;
  }  // end of operator /=

  template <typename Child, typename ArrayPolicy>
  typename GenericRuntimeArray<Child, ArrayPolicy>::pointer
  GenericRuntimeArray<Child, ArrayPolicy>::data() noexcept {
    return this->data_values.data();
  }  // end of data

  template <typename Child, typename ArrayPolicy>
  typename GenericRuntimeArray<Child, ArrayPolicy>::const_pointer
  GenericRuntimeArray<Child, ArrayPolicy>::data() const noexcept {
    return this->data_values.data();
  }  // end of data

  template <typename Child, typename ArrayPolicy>
  typename GenericRuntimeArray<Child, ArrayPolicy>::size_type
  GenericRuntimeArray<Child, ArrayPolicy>::getContainerSize() const noexcept {
    return this->data_values.size();
  }  // end of getContainerSize

  template <typename Child, typename ArrayPolicy>
  void GenericRuntimeArray<Child, ArrayPolicy>::resize(
      const typename ArrayPolicy::IndexingPolicy& p) {
    static_cast<typename ArrayPolicy::IndexingPolicy&>(*this) = p;
    this->data_values.resize(this->getUnderlyingArrayMinimalSize());
  }

  template <typename Child, typename ArrayPolicy>
  bool GenericRuntimeArray<Child, ArrayPolicy>::empty() const {
    return this->data_values.empty();
  }  // end of empty

  template <typename Child, typename ArrayPolicy>
  void GenericRuntimeArray<Child, ArrayPolicy>::clear() {
    this->data_values.clear();
    static_cast<typename ArrayPolicy::IndexingPolicy&>(*this) =
        typename ArrayPolicy::IndexingPolicy{};
  }  // end of clear

  template <typename Child, typename ArrayPolicy>
  void GenericRuntimeArray<Child, ArrayPolicy>::shrink_to_fit() {
    this->data_values.shrink_to_fit();
  }  // end of shrink_to_fit

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_GENERICRUNTIMEARRAY_IXX */
